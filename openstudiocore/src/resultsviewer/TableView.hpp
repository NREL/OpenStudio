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

#ifndef RESULTSVIEWER_TABLEVIEW_HPP
#define RESULTSVIEWER_TABLEVIEW_HPP

#include "../utilities/plot/Plot2D.hpp"
#include "../utilities/plot/LinePlot.hpp"
#include "../utilities/plot/FloodPlot.hpp"
#include "../utilities/sql/SqlFile.hpp"
#include "ResultsViewerData.hpp"

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
  void mouseMoveEvent(QMouseEvent *e);
  void mousePressEvent(QMouseEvent *e);
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
