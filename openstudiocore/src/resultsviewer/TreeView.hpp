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

#ifndef RESULTSVIEWER_TREEVIEW_HPP
#define RESULTSVIEWER_TREEVIEW_HPP

#include "../utilities/plot/Plot2D.hpp"
#include "../utilities/plot/LinePlot.hpp"
#include "../utilities/plot/FloodPlot.hpp"
#include "../utilities/sql/SqlFile.hpp"
#include "ResultsViewerData.hpp"

#include <QMainWindow>
#include <QList>
#include <QMenu>
#include <QTreeWidget>
#include <string>
#include <QApplication>


namespace resultsviewer{
  /// node type identifying type in tree
  enum dataDictionaryType {ddtFile = 1001, ddtEnv = 1002, ddtReportingFrequency = 1003, ddtVariableName = 1004, ddtKeyValue = 1005};

/** TreeView is a ui widget to present EnergyPlus output in a tree format which can be sorted and filtered and expanded and collapsed.
*/
class TreeView : public QTreeWidget
{
  Q_OBJECT

public:

  /// display type for tree view of sql file
  enum treeViewDisplayType { tvdtVariableName, tvdtKeyValue };


  TreeView( QWidget* parent=nullptr );
  openstudio::OptionalTimeSeries timeseriesFromTreeItem(QTreeWidgetItem* treeItem);
  resultsviewer::ResultsViewerPlotData resultsViewerPlotDataFromTreeItem(QTreeWidgetItem* treeItem);
  void displayFile(const QString &alias, openstudio::SqlFile sqlFile, treeViewDisplayType treeViewDisplay);
  void displaySqlFileVariableName(const QString &alias, openstudio::SqlFile sqlFile);
  void updateFileAlias(const QString& alias, const QString& filename);
  void removeFile(const QString& filename);
  bool isEmpty();
  // general plotting without caching - can be placed in Plot2DMimeData 
  void applyFilter(QString& filterText);
  void clearFilter();
  QString filenameFromTreeItem(QTreeWidgetItem *item);
  void goToFile(const QString& filename);

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
  // internal plot2ddata creation without caching
  void performPlot2DDataDrag();
  // external resultsviewerplotdata creation with caching of timeseries
  void performResultsViewerPlotDataDrag();

private:
  QPoint m_startPos;
  treeViewDisplayType m_defaultDisplayMode;
  QString filenameFromTopLevelItem(QTreeWidgetItem *topLevelItem);
  QString aliasFromTopLevelItem(QTreeWidgetItem *topLevelItem);
  void removeBranch(QTreeWidgetItem *item);
  std::vector<resultsviewer::ResultsViewerPlotData> generateResultsViewerPlotData();

signals: 
  // plotting within resultsviewer with timeseries caching
  void signalAddLinePlot(const std::vector<resultsviewer::ResultsViewerPlotData> &plotDataVec);
  void signalAddFloodPlot(const std::vector<resultsviewer::ResultsViewerPlotData> &plotDataVec);
  void signalAddIlluminancePlot(const std::vector<resultsviewer::ResultsViewerPlotData> &plotDataVec);
  void signalAddLinePlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData> &plotDataVec);
  void signalAddFloodPlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData> &plotDataVec);
  void signalAddIlluminancePlotComparison(const std::vector<resultsviewer::ResultsViewerPlotData> &plotDataVec);
  void signalDragResultsViewerPlotData(const std::vector<resultsviewer::ResultsViewerPlotData> &plotDataVec);
};


}; // resultsviewer namespace

#endif // RESULTSVIEWER_TREEVIEW_HPP
