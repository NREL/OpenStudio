/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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
      std::map<openstudio::SqlFile, QString>::iterator iter;
      for(iter=m_sqlFileMap.begin(); iter!=m_sqlFileMap.end(); ++iter) 
      {
        if ( filename.toUpper() == toQString((*iter).first.energyPlusSqliteFile()).toUpper() ) 
        {
          m_sqlFileMap.erase(iter);
          break;
        }
      }
    }
  }

  bool ResultsViewerData::isFileOpen(const QString& filename)
  {
    if (m_sqlFileMap.size() == 0) return false;

    bool isOpen = false;
    std::map<openstudio::SqlFile, QString>::iterator iter;
    for(iter=m_sqlFileMap.begin(); iter!=m_sqlFileMap.end(); ++iter) 
    {
      if ( filename.toUpper() == toQString((*iter).first.energyPlusSqliteFile()).toUpper() ) 
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
    if (m_sqlFileMap.size() == 0) return false;

    bool isOpen = false;
    std::map<openstudio::SqlFile, QString>::iterator iter;
    for(iter=m_sqlFileMap.begin(); iter!=m_sqlFileMap.end(); ++iter) {
      if (sqlFile.energyPlusSqliteFile() == (*iter).first.energyPlusSqliteFile()) {
        isOpen = true;
        break;
      }
    }
    return isOpen;
  }


  openstudio::SqlFile ResultsViewerData::sqlFile(const QString& filename)
  {
    openstudio::SqlFile sqlFP;
    std::map<openstudio::SqlFile, QString>::iterator iter;
    for(iter=m_sqlFileMap.begin(); iter!=m_sqlFileMap.end(); ++iter) {
      if (filename == toQString((*iter).first.energyPlusSqliteFile()) ) {
        sqlFP = (*iter).first;
        break;
      }
    }
    return sqlFP;
  }

  const QString ResultsViewerData::alias(const QString& filename)
  {
    QString aliasValue = "";
    std::map<openstudio::SqlFile, QString>::iterator iter;
    for(iter=m_sqlFileMap.begin(); iter!=m_sqlFileMap.end(); ++iter) {
      if (filename == toQString((*iter).first.energyPlusSqliteFile()) ) {
        aliasValue = (*iter).second;
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
    std::map<openstudio::SqlFile, QString>::iterator iter;
    for(iter=m_sqlFileMap.begin(); iter!=m_sqlFileMap.end(); ++iter) 
    {
      if (alias.toUpper() == ((*iter).second.toUpper()) ) 
      {
        exists = true;
        break;
      }
    }
    return exists;
  }


  void ResultsViewerData::updateAlias(const QString& alias, const QString& filename)
  {
    if (m_sqlFileMap.size() > 0)
    {
      std::map<openstudio::SqlFile, QString>::iterator iter;
      for(iter=m_sqlFileMap.begin(); iter!=m_sqlFileMap.end(); ++iter) 
      {
        if ( filename.toUpper() == toQString((*iter).first.energyPlusSqliteFile()).toUpper() ) 
        {
          m_sqlFileMap[(*iter).first] = alias;
        }
      }
    }
  }


} // resultsviewer
