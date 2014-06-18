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

#include "TreeView.hpp"

#include "../utilities/sql/SqlFileEnums.hpp"

#include <QTreeWidgetItemIterator>

using openstudio::toString;
using openstudio::toQString;
using openstudio::ReportingFrequency;

namespace resultsviewer{

TreeView::TreeView( QWidget* parent):QTreeWidget(parent)
{
    // The tree supports dragging 
  setDragEnabled(true);
  m_defaultDisplayMode = tvdtVariableName;
}

void TreeView::mouseMoveEvent(QMouseEvent *e)
{
  if (e->buttons() & Qt::LeftButton) {
    int distance = (e->pos()-m_startPos).manhattanLength();
    if (distance >= QApplication::startDragDistance()) {
      performResultsViewerPlotDataDrag();
    }
  }
  QTreeWidget::mouseMoveEvent(e);
}

void TreeView::mousePressEvent(QMouseEvent *e)
{
  if (e->button() == Qt::LeftButton) {
    m_startPos = e->pos();
  }
  QTreeWidget::mousePressEvent(e);
}

void TreeView::performResultsViewerPlotDataDrag()
{
  std::vector<resultsviewer::ResultsViewerPlotData> rvPlotDataVec;

  for (QTreeWidgetItem *item : selectedItems())
  {
    resultsviewer::ResultsViewerPlotData plot2DData = resultsViewerPlotDataFromTreeItem(item);
    rvPlotDataVec.push_back(plot2DData);
  }
  emit(signalDragResultsViewerPlotData(rvPlotDataVec));
}



resultsviewer::ResultsViewerPlotData TreeView::resultsViewerPlotDataFromTreeItem(QTreeWidgetItem* treeItem)
{
  QString keyValue = "", variableName = "", envPeriod = "", filename = "", reportFreq = "", s = "";
  int dataType=0;
  QString mapName="";
  resultsviewer::ResultsViewerPlotData resultsViewerPlotData;
  if (!treeItem) return resultsViewerPlotData;
  QTreeWidgetItem *fileItem = treeItem;
  while (treeItem) {
    switch (treeItem->type()) {
      case ddtKeyValue:
        keyValue = treeItem->text(0);
        mapName = treeItem->data(0, Qt::UserRole).toString();
        break;
      case ddtVariableName:
        variableName = treeItem->text(0);
        dataType = treeItem->data(0, Qt::UserRole).toInt();
        break;
      case ddtReportingFrequency:
        reportFreq = treeItem->text(0);
        break;
      case ddtEnv:
        envPeriod = treeItem->text(0);
        break;
      case ddtFile:
        fileItem = treeItem;
        filename = treeItem->text(0);
        break;
    }
    treeItem = treeItem->parent();
  }

  resultsViewerPlotData.filename=filenameFromTopLevelItem(fileItem);
  resultsViewerPlotData.alias=aliasFromTopLevelItem(fileItem);
  resultsViewerPlotData.envPeriod=envPeriod;
  resultsViewerPlotData.reportFreq=reportFreq;
  resultsViewerPlotData.variableName=variableName;
  resultsViewerPlotData.keyName=keyValue;
  resultsViewerPlotData.dataType=dataType;
  resultsViewerPlotData.dbIdentifier=mapName;

  return resultsViewerPlotData;

}


openstudio::OptionalTimeSeries TreeView::timeseriesFromTreeItem(QTreeWidgetItem* treeItem)
{
  openstudio::OptionalTimeSeries ts;
  std::string keyValue="", variableName="", envPeriod="", filename="", reportFreq="";
  QTreeWidgetItem *fileItem; //ETH@20100412 Darwin warning that this may be used uninitialized
  while (treeItem) {
    switch (treeItem->type()) {
      case ddtKeyValue:
        keyValue = toString(treeItem->text(0));
        break;
      case ddtVariableName:
        variableName = toString(treeItem->text(0));
        break;
      case ddtReportingFrequency:
        reportFreq = toString(treeItem->text(0));
        break;
      case ddtEnv:
        envPeriod = toString(treeItem->text(0));
        break;
      case ddtFile:
        fileItem = treeItem;
        filename = toString(treeItem->text(0));
        break;
    }
    treeItem = treeItem->parent();
  }

  filename = openstudio::toString(filenameFromTopLevelItem(fileItem));
  openstudio::path p = openstudio::toPath(filename);
  openstudio::SqlFile sqlFile(p);
  ts = sqlFile.timeSeries(envPeriod, reportFreq, variableName, keyValue);

  return ts;
}

std::vector<resultsviewer::ResultsViewerPlotData> TreeView::generateResultsViewerPlotData()
{
  std::vector<resultsviewer::ResultsViewerPlotData> resultsViewerPlotDataVec;
  if (selectedItems().count() > 0)
  {
    for (QTreeWidgetItem *item : selectedItems())
    {
      resultsviewer::ResultsViewerPlotData resultsViewerPlotData = resultsViewerPlotDataFromTreeItem(item);
      resultsViewerPlotDataVec.push_back(resultsViewerPlotData);
    } // foreach
  }
  return resultsViewerPlotDataVec;
}

void TreeView::generateIlluminancePlotData()
{
  std::vector<resultsviewer::ResultsViewerPlotData> fpVec = generateResultsViewerPlotData();
  signalAddIlluminancePlot(fpVec);
}

void TreeView::generateIlluminancePlotComparisonData()
{
  std::vector<resultsviewer::ResultsViewerPlotData> ipVec = generateResultsViewerPlotData();
  signalAddIlluminancePlotComparison(ipVec);
}

void TreeView::generateReverseIlluminancePlotComparisonData()
{
  std::vector<resultsviewer::ResultsViewerPlotData> ipVec = generateResultsViewerPlotData();
  std::vector<resultsviewer::ResultsViewerPlotData> ipVecRev(ipVec.rbegin(),ipVec.rend()); 
  signalAddIlluminancePlotComparison(ipVecRev);
}


void TreeView::generateLinePlotData()
{
  std::vector<resultsviewer::ResultsViewerPlotData> lpVec = generateResultsViewerPlotData();
  signalAddLinePlot(lpVec);
}

void TreeView::generateFloodPlotData()
{
  std::vector<resultsviewer::ResultsViewerPlotData> fpVec = generateResultsViewerPlotData();
  signalAddFloodPlot(fpVec);
}

void TreeView::generateLinePlotComparisonData()
{
  std::vector<resultsviewer::ResultsViewerPlotData> lpVec = generateResultsViewerPlotData();
  signalAddLinePlotComparison(lpVec);
}

void TreeView::generateReverseLinePlotComparisonData()
{
  std::vector<resultsviewer::ResultsViewerPlotData> lpVec = generateResultsViewerPlotData();
  std::vector<resultsviewer::ResultsViewerPlotData> lpVecRev(lpVec.rbegin(),lpVec.rend()); 
  signalAddLinePlotComparison(lpVecRev);
}

void TreeView::generateFloodPlotComparisonData()
{
  std::vector<resultsviewer::ResultsViewerPlotData> fpVec = generateResultsViewerPlotData();
  signalAddFloodPlotComparison(fpVec);
}

void TreeView::generateReverseFloodPlotComparisonData()
{
  std::vector<resultsviewer::ResultsViewerPlotData> fpVec = generateResultsViewerPlotData();
  std::vector<resultsviewer::ResultsViewerPlotData> fpVecRev(fpVec.rbegin(),fpVec.rend()); 
  signalAddFloodPlotComparison(fpVecRev);
}


QString TreeView::filenameFromTreeItem(QTreeWidgetItem *item)
{
  QString filename ="";
  if (item) 
  {
    if (item->type() == ddtFile) 
    { 
      filename = filenameFromTopLevelItem(item);
    }
    else 
    {
      QTreeWidgetItem* p = item->parent();
      while (p) 
      {
        if (p->type() == ddtFile) 
        {
          filename = filenameFromTopLevelItem(p);
          break;
        }
        p = p->parent();
      }
    }
  }
  return filename;
}



void TreeView::displayFile(const QString &alias, openstudio::SqlFile sqlFile, treeViewDisplayType treeViewDisplay)
{
  if (sqlFile.connectionOpen()) {
    //    m_alias = alias;
    //    m_filename = toQString(sqlFile.energyPlusSqliteFile());
    switch (treeViewDisplay) {
      case tvdtVariableName:
        /// file, env period, reporting freq, variable name, key value
        displaySqlFileVariableName(alias, sqlFile);
        break;
      case tvdtKeyValue:
        break;
      default:
        /// throw
        break;
    }
  }
}

QString TreeView::filenameFromTopLevelItem(QTreeWidgetItem *item)
{
//      fileItem->setText(0,QString("(%1) - %2").arg(alias).arg(sqlFile.energyPlusSqliteFile()));
//      fileItem->setData(0,Qt::UserRole, QStringList()<<alias<<filename);
  QString filename = "";
  if (item)
  {
    QStringList aliasFilename = item->data(0, Qt::UserRole).toStringList();
    if (aliasFilename.count() == 2) filename = aliasFilename[1];
  }
  return filename;
}

QString TreeView::aliasFromTopLevelItem(QTreeWidgetItem *item)
{
//      fileItem->setText(0,QString("(%1) - %2").arg(alias).arg(sqlFile.energyPlusSqliteFile()));
//      fileItem->setData(0,Qt::UserRole, QStringList()<<alias<<filename);
  QString alias = "";
  if (item)
  {
    QStringList aliasFilename = item->data(0, Qt::UserRole).toStringList();
    if (aliasFilename.count() == 2) alias = aliasFilename[0];
  }
  return alias;
}


void TreeView::displaySqlFileVariableName(const QString& alias, openstudio::SqlFile sqlFile)
{
  QString s;
  if (sqlFile.connectionOpen()) {

    //    this->clear();

    QTreeWidgetItem *parent = this->invisibleRootItem();

    auto fileItem = new QTreeWidgetItem(parent,ddtFile);
    //    fileItem->setText(0,QString("(%1) - %2").arg(m_alias).arg(m_filename));
    QString filename = openstudio::toQString(sqlFile.energyPlusSqliteFile());
    fileItem->setText(0,QString("(%1) - %2").arg(alias).arg(filename));
    fileItem->setData(0,Qt::UserRole, QStringList()<<alias<<filename);

    int textWidth = fontMetrics().width(fileItem->text(0))+30;
    if (textWidth > this->columnWidth(0)) this->setColumnWidth(0,textWidth);

    QTreeWidgetItem* reportFreqItem = nullptr;
    QTreeWidgetItem* variableNameItem = nullptr;
    QTreeWidgetItem* keyValueItem = nullptr;
    QTreeWidgetItem* hourlyBranch = nullptr;

    // environment period branch
    std::vector<std::string> vecEnvPeriods(sqlFile.availableEnvPeriods());
    std::vector<std::string>::iterator iterEnv;
    for (iterEnv=vecEnvPeriods.begin();iterEnv!=vecEnvPeriods.end();++iterEnv)
    {
      s = toQString(*iterEnv);
      // add to file branch
      auto envItem = new QTreeWidgetItem(fileItem, ddtEnv);
      envItem->setText(0, s);

      // setup reporting frequency branch
      std::vector<std::string> vecReportFreq(sqlFile.availableReportingFrequencies(*iterEnv));
      std::vector<std::string>::iterator iterReportFreq;
      for (iterReportFreq=vecReportFreq.begin();iterReportFreq!=vecReportFreq.end();++iterReportFreq)
      {
        // skip runPeriod
        if (sqlFile.reportingFrequencyFromDB(*iterReportFreq))
//          if (*(sqlFile.reportingFrequencyFromDB(*iterReportFreq)) != ReportingFrequency::RunPeriod)
        {
          s = toQString(*iterReportFreq);
          // add to environment branch
          reportFreqItem = new QTreeWidgetItem(envItem, ddtReportingFrequency);
          reportFreqItem->setText(0, s);

          if (*(sqlFile.reportingFrequencyFromDB(*iterReportFreq)) == ReportingFrequency::Hourly){
            hourlyBranch = reportFreqItem;
          }

          // setup variable name branch
          std::vector<std::string> vecVariableName(sqlFile.availableVariableNames(*iterEnv, *iterReportFreq));
          std::vector<std::string>::iterator iterVariableName;
          for (iterVariableName=vecVariableName.begin();iterVariableName!=vecVariableName.end();++iterVariableName)
          {
            s = toQString(*iterVariableName);
            // add to reporting frequency branch
            variableNameItem = new QTreeWidgetItem(reportFreqItem,ddtVariableName);
            variableNameItem->setText(0, s);
            variableNameItem->setData(0, Qt::UserRole, RVD_TIMESERIES);

            std::vector<std::string> vecKeyValue(sqlFile.availableKeyValues(*iterEnv, *iterReportFreq,*iterVariableName));
            std::vector<std::string>::iterator iterKeyValue;
            for (iterKeyValue=vecKeyValue.begin();iterKeyValue!=vecKeyValue.end();++iterKeyValue)
            {
              s = toQString(*iterKeyValue);
              // add to variable name branch
              if (!s.isEmpty())
              {
                if (sqlFile.reportingFrequencyFromDB(*iterReportFreq))
                {
                  if (*(sqlFile.reportingFrequencyFromDB(*iterReportFreq)) == ReportingFrequency::RunPeriod)
                  {
                    //                keyValueItem->setText(0, s + " = " +  QString::number(sqlFile.runPeriodValue(*iterEnv, *iterVariableName, *iterKeyValue)));
                    // "Facility:Electricity->Cumulative" should go to "Cumulative Facility:Electricity".
                    boost::optional<double> runPeriodValue = sqlFile.runPeriodValue(*iterEnv, *iterVariableName, *iterKeyValue);
                    if (runPeriodValue){
                      variableNameItem->setText(0, s + " " + variableNameItem->text(0) + " = " +  QString::number(*runPeriodValue));
                    }

                  }
                  else
                  {
                    keyValueItem = new QTreeWidgetItem(variableNameItem,ddtKeyValue);
                    keyValueItem->setText(0, s);
                  }
                }
              }
            }
            variableNameItem->setExpanded(true);
          }
          reportFreqItem->setExpanded(true);
        } // end skip runPeriod
      }
      // for the specified environment period retrieve all illuminance maps - if any
      std::vector<std::string> vecIlluminanceMaps( sqlFile.illuminanceMapNames(*iterEnv) );
      if ( vecIlluminanceMaps.size() > 0)
      {
        if (!hourlyBranch){
          hourlyBranch = new QTreeWidgetItem(envItem, ddtReportingFrequency);
        }

        // add to hourly branch
        variableNameItem = new QTreeWidgetItem(hourlyBranch, ddtVariableName);
        variableNameItem->setText(0, "Illuminance Map");
        variableNameItem->setData(0, Qt::UserRole, RVD_ILLUMINANCEMAP);
        std::vector<std::string>::iterator iterIlluminanceMap;
        for (iterIlluminanceMap=vecIlluminanceMaps.begin();iterIlluminanceMap!=vecIlluminanceMaps.end();++iterIlluminanceMap)
        {
          std::vector<std::string> vecKeyValue(sqlFile.illuminanceMapZoneNames(*iterIlluminanceMap));
          std::vector<std::string>::iterator iterKeyValue;
          for (iterKeyValue=vecKeyValue.begin();iterKeyValue!=vecKeyValue.end();++iterKeyValue)
          {
            s = toQString(*iterKeyValue);
            // add to variable name branch
            if (!s.isEmpty())
            {
              keyValueItem = new QTreeWidgetItem(variableNameItem,ddtKeyValue);
              keyValueItem->setText(0, s);
              keyValueItem->setData(0, Qt::UserRole, toQString(*iterIlluminanceMap) );// map name
            }
          }
          variableNameItem->setExpanded(true);
        }
        hourlyBranch->setExpanded(true);
      }
      envItem->setExpanded(true);
    }
    fileItem->setExpanded(true);
  }
}

void TreeView::updateFileAlias(const QString& alias, const QString& filename)
{
  for (int i=0;i<topLevelItemCount();i++)
  {
    QTreeWidgetItem *fileItem = topLevelItem(i);
    if (fileItem) 
    {
      QString matchingFilename = filenameFromTopLevelItem(fileItem);
      if (matchingFilename.toUpper() == filename.toUpper())
      {
        fileItem->setText(0,QString("(%1) - %2").arg(alias).arg(filename));
        fileItem->setData(0,Qt::UserRole, QStringList()<<alias<<filename);
        break;
      }
    }
  }
}

void TreeView::removeFile(const QString& filename)
{
  for (int i=0;i<topLevelItemCount();i++)
  {
    QTreeWidgetItem *fileItem = topLevelItem(i);
    if (fileItem) 
    {
      QString matchingFilename = filenameFromTopLevelItem(fileItem);
      if (matchingFilename.toUpper() == filename.toUpper())
      {
        removeBranch(fileItem);
        break;
      }
    }
  }
}

void TreeView::removeBranch(QTreeWidgetItem *item)
{
  if (item)
  {
    QTreeWidgetItemIterator itAll(item);
    while (*itAll) 
    {
      delete (*itAll);
      ++itAll;
    }
  }
}

bool TreeView::isEmpty()
{
/*  bool emptyTree = true;
  QTreeWidgetItem *parent = this->invisibleRootItem();
  if (parent) 
  {
    QTreeWidgetItem *fileItem = parent->child(0);
    if (fileItem) emptyTree = false;
  }
  return emptyTree;
  */
  return (topLevelItemCount() ==0);
}

void TreeView::applyFilter(QString& filterText)
{
//  QRegExp regExp(filterText); //strict regular expression matching
  QRegExp regExp(filterText, Qt::CaseInsensitive, QRegExp::Wildcard); // text wild card *, ? matching
  // hide all
  QTreeWidgetItemIterator itAll(this);
  while (*itAll) 
  {
    (*itAll)->setHidden(true);
    ++itAll;
  }
  // check all leaf nodes
  QTreeWidgetItemIterator it(this,QTreeWidgetItemIterator::NoChildren); // bottom level
  while (*it) 
  {
    QTreeWidgetItem *parent;
    bool showBranch = false;
    if (regExp.exactMatch((*it)->text(0)))
    {
      showBranch = true;
    }
    else
    {
      parent = (*it)->parent();
      while ((parent) && (!showBranch))
      {
        if (regExp.exactMatch(parent->text(0)))
        {
          showBranch = true;
        }
        parent = parent->parent();
      }
    }
    if (showBranch)
    {
      (*it)->setHidden(false);
      parent = (*it)->parent();
      // set branch visible
      while(parent)
      {
        parent->setHidden(false);
        parent = parent->parent();
      }
    }
    ++it;
  }
}

void TreeView::clearFilter()
{
  QTreeWidgetItemIterator it(this);
  while (*it) 
  {
    (*it)->setHidden(false);
    ++it;
  }
}


void TreeView::goToFile(const QString& filename)
{
  for (int i=0;i<topLevelItemCount();i++)
  {
    QTreeWidgetItem *fileItem = topLevelItem(i);
    if (fileItem) 
    {
      QString matchingFilename = filenameFromTopLevelItem(fileItem);
      if (matchingFilename.toUpper() == filename.toUpper())
      {
        scrollToItem(fileItem, QAbstractItemView::PositionAtTop); 
        break;
      }
    }
  }
}


}; 
