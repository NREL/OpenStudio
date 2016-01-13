/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

#include "URLHelpers.hpp"
#include "Path.hpp"
#include "String.hpp"

#include <QRegularExpression>

namespace openstudio {
  
QUrl completeURL(const QUrl &t_source, const std::vector<QUrl> &t_searchPaths, bool throwOnFailure)
{
  if (t_source.scheme() == "file" || t_source.scheme().isEmpty())
  {
    openstudio::path origpath = toPath(t_source.toLocalFile());

    if (origpath.is_complete())
    {
      // source is already complete, no reason to search
      return QUrl::fromLocalFile(toQString(origpath));
    }

    for (const auto & searchitr : t_searchPaths)
    {
      //search for files here... 
      /// \todo support remote urls
      if (searchitr.scheme() == "file" || searchitr.scheme().isEmpty())
      {
        openstudio::path searchpath = toPath(searchitr.toLocalFile());

        openstudio::path completepath = searchpath / origpath;

        if (boost::filesystem::exists(completepath))
        {
          return QUrl::fromLocalFile(toQString(completepath));
        }
      }    
    }
  } else {
    return t_source;
  }

  if (throwOnFailure)
  {
    throw std::runtime_error("Unable to locate file in search paths");
  } else {
    return QUrl();
  }
}

QUrl toURL(const openstudio::path& p) {
  return QUrl::fromLocalFile(toQString(p));
}

QUrl toURL(const std::string& s) {
  return QUrl(toQString(s));
}

std::string toString(const QUrl& url) {
  // reimplementation of toString(QString) b/c direct use of that method led to 
  // MSVS warning C4717: recursive on all control paths
  const QByteArray& qb = url.toString().toUtf8();
  return std::string(qb.data());
}

openstudio::path getOriginalPath(const Url& url)
{
  boost::optional<openstudio::path> result = getOptionalOriginalPath(url);
  if (!result){
    throw std::runtime_error("Unable to convert url to path");
  }
  return result.get();
}

boost::optional<openstudio::path> getOptionalOriginalPath(const Url& url)
{
  boost::optional<openstudio::path> result;
  if (url.scheme() == "file" || url.scheme().isEmpty()){
    QString localFile = url.toLocalFile();

    // test for root slash added to windows paths, "/E:/test/CloudTest/scripts/StandardReports/measure.rb" 
    const QRegularExpression regexp("^\\/([a-zA-Z]:\\/.*)");
    QRegularExpressionMatch match = regexp.match(localFile);
    if (match.hasMatch()){
      localFile = match.captured(1);
    }

    result = toPath(localFile);
  }
  return result;
}

} // openstudio

