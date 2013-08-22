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

#include <utilities/core/Assert.hpp>
#include <utilities/core/String.hpp>

#include <qjson/parser.h>

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
        m_validSecretKey(false)
    {
    }

    std::string AWSProvider_Impl::type() const
    {
      return "AWSProvider";
    }

    bool AWSProvider_Impl::internetAvailable() const
    {
      return false;
    }

    bool AWSProvider_Impl::serviceAvailable() const
    {
      QTemporaryFile file;
      if (file.open()) {
        QTextStream out(&file);
        out << "require 'aws'\n"
               "require 'json'\n"
               "#require 'active_support/core_ext/hash/conversions'\n"
               "\n"
               "def error(code, msg)\n"
               "  puts ({:error=>{:code=>code,:message=>msg}}.to_json)\n"
               "  exit(1)\n"
               "end\n"
               "\n"
               "if ARGV.length < 2\n"
               "  error(-1, 'Invalid number of args')\n"
               "end\n"
               "\n"
               "AWS.config(\n"
               "  :access_key_id => ARGV[0],\n"
               "  :secret_access_key => ARGV[1],\n"
               "  :ssl_verify_peer => false\n"
               ")\n"
               "\n"
               "ec2 = AWS::EC2.new\n"
               "begin\n"
               "  resp = ec2.client.describe_availability_zones\n"
               "  puts \"Status: #{resp.http_response.status}\"\n"
               "rescue Exception => e\n"
               "  if e.message == 'getaddrinfo: No such host is known. '\n"
               "    error(503, 'Offline')\n"
               "  end\n"
               "  #puts Hash.from_xml(e.http_response.body).to_json\n"
               "  error(e.http_response.status, e.code)\n"
               "end\n"
               "puts resp.data.to_json\n";

        out.flush();
   
        QProcess *ruby2 = new QProcess();
        ruby2->start("\"C:\\Ruby200\\bin\\ruby.exe\"", QStringList() << file.fileName().toUtf8() << "[accessKey]" << "[secretKey]");
        ruby2->waitForFinished();
        QString output = ruby2->readAllStandardOutput();
        QString errors = ruby2->readAllStandardError();
        if (errors.length()) std::cerr << errors.toStdString() << std::endl;

        std::cout << output.toStdString() << std::endl;

        QJson::Parser parser;
        bool ok = false;
        QVariantMap map = parser.parse(output.toUtf8(), &ok).toMap();

        if (ok) {
          Q_FOREACH(QVariant metric, map["metrics"].toList()) {
            QVariant var = metric.toMap()["dimensions"].toList()[0].toMap()["value"];
          }
        } else {
          std::cerr << "Error parsing JSON: " + toString(parser.errorString()) << std::endl;
        }
      }

      return true;
    }

    bool AWSProvider_Impl::validateCredentials() const
    {
      if (!QDir::home().exists(".ssh")) {
        QDir::home().mkdir(".ssh");
      } else if (QFile::exists(QDir::homePath() + "/.ssh/aws")) {
        QFile file(QDir::homePath() + "/.ssh/aws");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
          return false;
        }
        
        QJson::Parser parser;
        bool ok = false;
        QVariantMap map = parser.parse(QString(file.readAll()).toUtf8(), &ok).toMap();
        
        if (ok) {
          //this->setKeys(toString(map["accessKey"].toString()), toString(map["secretKey"].toString());
        }

        // Load credentials
      }
      return false;
    }

    bool AWSProvider_Impl::reconnect(const CloudSession& session)
    {
      return false;
    }

    CloudSession AWSProvider_Impl::session() const
    {
      return CloudSession("", "", OptionalUrl(), UrlVector());
    }

    boost::optional<Url> AWSProvider_Impl::serverUrl() const
    {
      return OptionalUrl();
    }

    bool AWSProvider_Impl::startServer()
    {
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

    bool AWSProvider_Impl::validAccessKey(std::string accessKey) const {
      return QRegExp("[A-Z0-9]{20}").exactMatch(toQString(accessKey));
    }

    bool AWSProvider_Impl::validSecretKey(std::string secretKey) const {
      return QRegExp("[a-zA-Z0-9/+]{40}").exactMatch(toQString(secretKey));
    }

    std::string AWSProvider_Impl::accessKey() const {
      return m_accessKey;
    }
    
    std::string AWSProvider_Impl::secretKey() const {
      return m_secretKey;
    }
    
    bool AWSProvider_Impl::setKeys(std::string accessKey, std::string secretKey) {
      m_accessKey = accessKey;
      m_secretKey = secretKey;
      return false;
    }

  } // detail

  AWSProvider::AWSProvider()
    : CloudProvider(boost::shared_ptr<detail::AWSProvider_Impl>(new detail::AWSProvider_Impl()))
  {
    this->validateCredentials();
    
  }

} // openstudio
