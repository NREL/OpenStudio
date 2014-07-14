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

#ifndef RESULTSVIEWER_RESULTSVIEWERDATA_HPP
#define RESULTSVIEWER_RESULTSVIEWERDATA_HPP

#include "../utilities/plot/Plot2D.hpp"
#include "../utilities/plot/LinePlot.hpp"
#include "../utilities/plot/FloodPlot.hpp"
#include "../utilities/sql/SqlFile.hpp"

#include <QMainWindow>
#include <QTableWidget>
#include <string>
#include <QApplication>

namespace resultsviewer{


/** ResultsViewerPlotData structure for translating data from ui widgets to plotting format
*/
  struct ResultsViewerPlotData 
  {
    int dataType; // plot data type for specific handling - e.g. illuminance maps
    QString dbIdentifier; // additional information needed to retrieve data from database - e.g. map name for illuminance maps
    QString filename; // most likely an eplusout.sql file
    QString alias; // shorthand notation for plot source - used to distinguish legend values
    QString envPeriod;
    QString reportFreq;
    QString variableName;
    QString keyName;
    QString idName() {return "(" + alias + ") " + variableName + " " + keyName;}
  };



// flags
const int RVD_SUCCESS = 0;
const int RVD_UNSUPPORTEDFILEFORMAT = 1;
const int RVD_FILEALREADYOPENED = 2;
const int RVD_UNSUPPORTEDVERSION = 3;
const int RVD_FILEDOESNOTEXIST = 4;
// data types 
const int RVD_TIMESERIES = 3;
const int RVD_ILLUMINANCEMAP = 4;

/** ResultsViewerData tracks open files and aliases used in ResultsViewer
*/
class ResultsViewerData
{
public:
  ResultsViewerData();
  ~ResultsViewerData();

  bool isFileOpen(const QString& filename);
  bool isFileOpen(openstudio::SqlFile sqlFile);
  openstudio::SqlFile sqlFile(const QString& filename);
  int addFile(const QString& alias, openstudio::SqlFile sqlFile);
  int addFile(const QString& alias, const QString& filename);
  void removeFile(const QString& filename);
  bool isSupportedSqlFileFormat(const QString& filename);

  // return alias for a given filename
  const QString alias(const QString& filename);

  // default alias bases on filename
  const QString defaultAlias(QString& filename);

  // update alias
  void updateAlias(const QString& alias, const QString& filename);

  // check if alias exists
  bool aliasExists(const QString& alias);

private:
  std::map<openstudio::SqlFile, QString> m_sqlFileMap;
};


}; // resultsviewer namespace

#endif // RESULTSVIEWER_RESULTSVIEWERDATA_HPP
