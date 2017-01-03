/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "ResultsViewerData.hpp"

#include "../utilities/core/String.hpp"

#include <QFileInfo>
#include <QString>

using openstudio::toString;
using openstudio::toQString;
using openstudio::toPath;

namespace resultsviewer{

  ResultsViewerData::ResultsViewerData()
  {
  }

  ResultsViewerData::~ResultsViewerData()
  {
  }

  int ResultsViewerData::addFile(const QString& alias, openstudio::SqlFile sqlFile)
  {
    if (!sqlFile.connectionOpen()) return RVD_UNSUPPORTEDFILEFORMAT;
    if (isFileOpen(sqlFile)) return RVD_FILEALREADYOPENED;

    m_sqlFileMap[sqlFile] = alias;

    if (!sqlFile.supportedVersion()) return RVD_UNSUPPORTEDVERSION;

    return RVD_SUCCESS;
  }

  int ResultsViewerData::addFile(const QString& alias, const QString& filename)
  {
    if (!QFileInfo(filename).exists()) return RVD_FILEDOESNOTEXIST;
    if (isFileOpen(filename)) return RVD_FILEALREADYOPENED;
    openstudio::path p = toPath(filename);

    return addFile(alias, openstudio::SqlFile(p));

  }

  void ResultsViewerData::removeFile(const QString& filename)
  {
    if (isFileOpen(filename))
    {
      for(auto iter=m_sqlFileMap.begin(); iter!=m_sqlFileMap.end(); ++iter) 
      {
        if ( toPath(filename.toUpper()) == toPath(toQString((*iter).first.energyPlusSqliteFile()).toUpper()) ) 
        {
          m_sqlFileMap.erase(iter);
          break;
        }
      }
    }
  }

  bool ResultsViewerData::isFileOpen(const QString& filename)
  {
    if (m_sqlFileMap.empty()) return false;

    bool isOpen = false;
    for (auto map : m_sqlFileMap)
    {
      if (toPath(filename.toUpper()) == toPath(toQString(map.first.energyPlusSqliteFile()).toUpper()) )
      {
        isOpen = true;
        break;
      }
    }
    return isOpen;
  }

  bool ResultsViewerData::isFileOpen(openstudio::SqlFile sqlFile)
  {
    if (!sqlFile.connectionOpen()) return false;
    if (m_sqlFileMap.empty()) return false;

    bool isOpen = false;
    for (auto map : m_sqlFileMap) {
      if (toPath(sqlFile.energyPlusSqliteFile()) == toPath(map.first.energyPlusSqliteFile())) {
        isOpen = true;
        break;
      }
    }
    return isOpen;
  }


  openstudio::SqlFile ResultsViewerData::sqlFile(const QString& filename)
  {
    openstudio::SqlFile sqlFP;
    for(auto map : m_sqlFileMap) {
      if (toPath(filename) == toPath(map.first.energyPlusSqliteFile())) {
        sqlFP = map.first;
        break;
      }
    }
    return sqlFP;
  }

  const QString ResultsViewerData::alias(const QString& filename)
  {
    QString aliasValue = "";
    for (auto map : m_sqlFileMap) {
      if (toPath(filename) == toPath(map.first.energyPlusSqliteFile()) ) {
        aliasValue = map.second;
        break;
      }
    }
    return aliasValue;
  }

  const QString ResultsViewerData::defaultAlias(QString& filename)
  {
    QString alias ="";
    // ticket #79 - last part of path
    QFileInfo fileInfo(filename); // handles windows links and "\"
    QString path = fileInfo.canonicalFilePath();
    QStringList pathSegments = path.split("/");
    if (pathSegments.count() > 1) 
      alias = pathSegments[pathSegments.count()-2];
    else
      alias = "default";
    for (int i=0;i<1000;i++)
      if (aliasExists(alias))
        alias = alias + "(" + QString::number(i) + ")";
      else 
        break;
    return alias;
  }


  bool ResultsViewerData::aliasExists(const QString &alias)
  {
    bool exists = false;
    for (auto map : m_sqlFileMap)
    {
      if (alias.toUpper() == map.second.toUpper() ) 
      {
        exists = true;
        break;
      }
    }
    return exists;
  }


  void ResultsViewerData::updateAlias(const QString& alias, const QString& filename)
  {
    if (!m_sqlFileMap.empty())
    {
      for (auto map : m_sqlFileMap)
      {
        if ( toPath(filename.toUpper()) == toPath(toQString(map.first.energyPlusSqliteFile()).toUpper()) ) 
        {
          m_sqlFileMap[map.first] = alias;
        }
      }
    }
  }


} // resultsviewer
