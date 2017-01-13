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

#ifndef RESULTSVIEWER_TABLEVIEW_HPP
#define RESULTSVIEWER_TABLEVIEW_HPP

#include "LinePlot.hpp"
#include "FloodPlot.hpp"
#include "ResultsViewerData.hpp"

#include "../utilities/sql/SqlFile.hpp"

#include <QMainWindow>
#include <QTableWidget>
#include <string>
#include <QApplication>

namespace resultsviewer{

/** TableView is a ui widget to present EnergyPlus output in a table which can be sorted and filtered.
*/
class TableView : public QTableWidget
{
  Q_OBJECT
public:

  TableView( QWidget* parent=nullptr);
  bool addFile(const QString& alias, openstudio::SqlFile sqlFile);
  void removeFile(const QString& filename);
  bool updateFileAlias(const QString& alias, const QString& filename);
  void applyFilter(QString& filterText);
  void clearFilter();
  void goToFile(const QString& filename);
  int selectedRowCount();
  std::vector<int> selectedRows();
  resultsviewer::ResultsViewerPlotData resultsViewerPlotDataFromTableRow(int row);
  // for determining illuminance maps
  const QStringList& headerNames() const {return m_slHeaders;}

public slots:
  void generateLinePlotData();
  void generateFloodPlotData();
  void generateIlluminancePlotData();
  void generateLinePlotComparisonData();
  void generateFloodPlotComparisonData();
  void generateIlluminancePlotComparisonData();
  void generateReverseLinePlotComparisonData();
  void generateReverseFloodPlotComparisonData();
  void generateReverseIlluminancePlotComparisonData();


protected:
  void mouseMoveEvent(QMouseEvent *e) override;
  void mousePressEvent(QMouseEvent *e) override;
  void performDrag();
  // internal plot2ddata creation without caching
  void performPlot2DDataDrag();
  // external resultsviewerplotdata creation with caching of timeseries
  void performResultsViewerPlotDataDrag();

signals:
  void columnsResized();
  void fileAdded();
  // plotting within resultsviewer with timeseries caching
  void signalAddLinePlot(const std::vector<resultsviewer::ResultsViewerPlotData> &plotDataVec);
  void signalAddFloodPlot(const std::vector<resultsviewer::ResultsViewerPlotData> &plotDataVec);
  void signalAddIlluminancePlot(const std::vector<resultsviewer::ResultsViewerPlotData> &plotDataVec);
  void signalAddLinePlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData> &plotDataVec);
  void signalAddFloodPlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData> &plotDataVec);
  void signalAddIlluminancePlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData> &plotDataVec);
  void signalDragResultsViewerPlotData(const std::vector<resultsviewer::ResultsViewerPlotData> &plotDataVec);

private:
  QPoint m_startPos;
  // adds row to table and sets format
  int addRow();
//  void adjustColumnSizes();
  QStringList m_slHeaders;

  resultsviewer::ResultsViewerPlotData resultsViewerPlotDataFromTableItem(QTableWidgetItem* tableItem);
  std::vector<resultsviewer::ResultsViewerPlotData> generateResultsViewerPlotData();

};


}; // resultsviewer namespace

#endif // RESULTSVIEWER_TABLEVIEW_HPP
