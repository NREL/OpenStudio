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

#ifndef RESULTSVIEWER_RESULTSVIEWERDATA_HPP
#define RESULTSVIEWER_RESULTSVIEWERDATA_HPP

#include "LinePlot.hpp"
#include "FloodPlot.hpp"
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
