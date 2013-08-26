/**********************************************************************
* Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/
#include <utilities/cloud/AWSProvider.hpp>
#include <utilities/cloud/AWSProvider_Impl.hpp>

#include <utilities/core/ApplicationPathHelpers.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/core/String.hpp>

#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <QDir>
#include <QFile>
#include <QProcess>
#include <QTemporaryFile>
#include <QTextStream>
#include <QUrl>

namespace openstudio{
  namespace detail{

    AWSProvider_Impl::AWSProvider_Impl()
      : CloudProvider_Impl(),
        m_validAccessKey(false),
        m_validSecretKey(false),
        m_ami("ami-d0f89fb9")
    {
      if (loadCredentials()) {
        validateCredentials();
      }
    }

    std::string AWSProvider_Impl::type() const
    {
      return "AWSProvider";
    }

    bool AWSProvider_Impl::internetAvailable() const
    {
      return serviceAvailable();
    }

    bool AWSProvider_Impl::serviceAvailable() const
    {
      QVariantMap map = awsRequest("describe_availability_zones");
      if (!map.keys().contains("error")) {
        return true;
      }
      int code = map["error"].toMap()["code"].toInt();
      return (code != 503);
    }

    bool AWSProvider_Impl::validateCredentials() const
    {
      if (m_validAccessKey && m_validSecretKey) {
        return true;
      }

      QVariantMap map = awsRequest("describe_availability_zones");
      if (!map.keys().contains("error")) {
        m_validAccessKey = true;
        m_validSecretKey = true;
        return true;
      }

      int code = map["error"].toMap()["code"].toInt();
      if (code == 401) {
        m_validAccessKey = false;
      } else if (code == 403) {
        m_validAccessKey = true;
        m_validSecretKey = false;
      }
      return false;
    }

    bool AWSProvider_Impl::reconnect(const CloudSession& session)
    {
      return false;
    }

    CloudSession AWSProvider_Impl::session() const
    {
      return CloudSession(std::string(), std::string(), OptionalUrl(), UrlVector());
    }

    boost::optional<Url> AWSProvider_Impl::serverUrl() const
    {
      return OptionalUrl();
    }

    bool AWSProvider_Impl::startServer()
    {
      QVariantMap map = awsRequest("launch_instance");
      if (!map.keys().contains("error")) {
        return true;
      }
      return false;
    }

    std::vector<Url> AWSProvider_Impl::workerUrls() const
    {
      return UrlVector();
    }

    unsigned AWSProvider_Impl::numWorkers() const
    {
      return 0;
    }

    bool AWSProvider_Impl::startWorkers()
    {
      return false;
    }

    bool AWSProvider_Impl::running() const
    {
      return false;
    }

    bool AWSProvider_Impl::terminate()
    {
      return false;
    }

    bool AWSProvider_Impl::terminated() const
    {
      return false;
    }

    std::vector<std::string> AWSProvider_Impl::errors() const
    {
      return std::vector<std::string>();
    }

    std::vector<std::string> AWSProvider_Impl::warnings() const
    {
      return std::vector<std::string>();
    }



    bool AWSProvider_Impl::loadCredentials() const {
      QFile file(QDir::homePath() + "/.ssh/aws");
      if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJson::Parser parser;
        bool ok = false;
        QVariantMap map = parser.parse(QString(file.readAll()).toUtf8(), &ok).toMap();
        
        if (ok) {
          if (validAccessKey(toString(map["accessKey"].toString())) && validSecretKey(toString(map["secretKey"].toString()))) {
            m_accessKey = toString(map["accessKey"].toString());
            m_secretKey = toString(map["secretKey"].toString());
            return true;
          }
        }
      }
      return false;
    }

    bool AWSProvider_Impl::saveCredentials() const {
      QVariantMap json;
      json.insert("accessKey", toQString(m_accessKey));
      json.insert("secretKey", toQString(m_secretKey));
      
      QJson::Serializer serializer;
      if (QDir::home().exists(".ssh") || QDir::home().mkdir(".ssh")) {
        QFile file(QDir::homePath() + "/.ssh/aws");
        if (file.open(QIODevice::WriteOnly)) {
          file.write(serializer.serialize(json));
          file.close();
          return true;
        }
      }
      return false;
    }

    bool AWSProvider_Impl::validAccessKey(std::string accessKey) const {
      return QRegExp("[A-Z0-9]{20}").exactMatch(toQString(accessKey));
    }

    bool AWSProvider_Impl::validSecretKey(std::string secretKey) const {
      return QRegExp("[a-zA-Z0-9/+]{40}").exactMatch(toQString(secretKey));
    }

    QVariantMap AWSProvider_Impl::awsRequest(std::string request, std::string service) const {
      QString script = toQString(getOpenStudioRubyScriptsPath() / toPath("cloud/aws.rb"));
      QProcess *ruby2 = new QProcess();
      ruby2->start("\"C:\\Ruby200\\bin\\ruby.exe\"", QStringList() << script << toQString(accessKey()) << toQString(secretKey()) << toQString(service) << toQString(request));
      ruby2->waitForFinished();
      QString output = ruby2->readAllStandardOutput();
      QString errors = ruby2->readAllStandardError();
      LOG(Debug, output.toStdString());

      if (errors.length()) {
        LOG(Error, errors.toStdString());
      } else {
        QJson::Parser parser;
        bool ok = false;
        QVariantMap map = parser.parse(output.toUtf8(), &ok).toMap();
        if (ok) {
          return map;
        } else {
          LOG(Error, "Error parsing JSON: " + toString(parser.errorString()));
        }
      }
      return QVariantMap();
    }

    std::string AWSProvider_Impl::accessKey() const {
      return m_accessKey;
    }
    
    std::string AWSProvider_Impl::secretKey() const {
      return m_secretKey;
    }
    
    bool AWSProvider_Impl::setKeys(std::string accessKey, std::string secretKey) const {
      if (validAccessKey(accessKey) && validSecretKey(secretKey)) {
        m_accessKey = accessKey;
        m_secretKey = secretKey;

        return true;
      }
      return false;
    }

  } // detail

  AWSProvider::AWSProvider()
    : CloudProvider(boost::shared_ptr<detail::AWSProvider_Impl>(new detail::AWSProvider_Impl()))
  {
  }

} // openstudio
