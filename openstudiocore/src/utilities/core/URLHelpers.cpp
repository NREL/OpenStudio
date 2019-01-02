/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "URLHelpers.hpp"

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

        if (openstudio::filesystem::exists(completepath))
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

