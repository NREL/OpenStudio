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

#include "TableView.hpp"

#include "../utilities/sql/SqlFileEnums.hpp"

#include<QHeaderView>

using openstudio::toString;
using openstudio::toQString;
using openstudio::ReportingFrequency;
using namespace openstudio;

namespace resultsviewer{

  TableView::TableView( QWidget* parent):QTableWidget(0,6,parent)
  {
    setSortingEnabled(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    horizontalHeader()->setSortIndicator(0,Qt::AscendingOrder);
    horizontalHeader()->setSortIndicatorShown(true);
    horizontalHeader()->setSectionsClickable(true);
    horizontalHeader()->setSectionsMovable(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_slHeaders  << tr("Variable Name") << tr("Key Value") << tr("Reporting Frequency") << tr("Alias") << tr("Environment Period") << tr("File");
    setHorizontalHeaderLabels( m_slHeaders );
    horizontalHeader()->setStretchLastSection(true);

    verticalHeader()->hide();
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setEditTriggers(QAbstractItemView::NoEditTriggers); // no editing
    setDragEnabled(true);

  }


  void TableView::mouseMoveEvent(QMouseEvent *e)
  {
    if (e->buttons() & Qt::LeftButton) {
      int distance = (e->pos()-m_startPos).manhattanLength();
      if (distance >= QApplication::startDragDistance()) {
        performResultsViewerPlotDataDrag();
      }
    }
    QTableWidget::mouseMoveEvent(e);
  }

  void TableView::mousePressEvent(QMouseEvent *e)
  {
    if (e->button() == Qt::LeftButton) {
      m_startPos = e->pos();
    }
    QTableWidget::mousePressEvent(e);
  }

  void TableView::performResultsViewerPlotDataDrag()
  {
    std::vector<resultsviewer::ResultsViewerPlotData> rvPlotDataVec;
    std::vector<int> selectedRows;
    if (selectedItems().count() > 0)
    {
      for (QTableWidgetItem *item : selectedItems())
      {
        int row = item->row();
        if ( std::find(selectedRows.begin(), selectedRows.end(), row) == selectedRows.end() )
        {
          selectedRows.push_back(row);
          rvPlotDataVec.push_back(resultsViewerPlotDataFromTableItem(item));
        }
      }
    }
    emit(signalDragResultsViewerPlotData(rvPlotDataVec));
  }



  void TableView::removeFile(const QString& filename)
  {
    for (int row=rowCount()-1; row>-1; row--)
    {
      if ( item(row, m_slHeaders.indexOf(tr("File")))->text().toUpper() == filename.toUpper() ) removeRow(row);
    }
  }

  bool TableView::addFile(const QString& alias, openstudio::SqlFile sqlFile)
  {
    if (alias.isEmpty() || !sqlFile.connectionOpen()) return false;

    setSortingEnabled(false);

    detail::DataDictionaryTable ddTable = sqlFile.dataDictionary();

    detail::DataDictionaryTable::iterator iter;
    for (iter=ddTable.begin();iter!=ddTable.end();++iter)
    {
      // skip runPeriod
      if (sqlFile.reportingFrequencyFromDB((*iter).reportingFrequency)
          && *(sqlFile.reportingFrequencyFromDB((*iter).reportingFrequency)) != ReportingFrequency::RunPeriod)
      {

        int row = addRow();
        item(row, m_slHeaders.indexOf("Alias"))->setText(alias);
        item(row, m_slHeaders.indexOf("File"))->setText(openstudio::toQString(sqlFile.energyPlusSqliteFile()));
        item(row, m_slHeaders.indexOf("Environment Period"))->setText(openstudio::toQString((*iter).envPeriod));
        item(row, m_slHeaders.indexOf("Reporting Frequency"))->setText(openstudio::toQString((*iter).reportingFrequency));
        item(row, m_slHeaders.indexOf("Key Value"))->setText(openstudio::toQString((*iter).keyValue));
        item(row, m_slHeaders.indexOf("Variable Name"))->setText(openstudio::toQString((*iter).name));
        item(row, m_slHeaders.indexOf("File"))->setData(Qt::UserRole, RVD_TIMESERIES);
      } // end skip runPeriod
    }

    /* illuminance maps */
    std::vector<std::string> mapNames(sqlFile.illuminanceMapNames());
    std::vector<std::string>::iterator nameIter;
    for (nameIter=mapNames.begin(); nameIter!=mapNames.end(); ++nameIter)
    {
      // retrieve mapIndex for map name to retrieve environment period and zone name
      QString envPeriod = "";
      QString keyValue = "";
      boost::optional<int> mapIndex = sqlFile.illuminanceMapIndex(*nameIter);
      if (mapIndex)
      {
        boost::optional<std::string> strValue;
        boost::optional<int> intValue;
        std::stringstream s;
        s << "select Environment from daylightmaps where MapNumber=" << *mapIndex;
        strValue = sqlFile.execAndReturnFirstString(s.str());
        if (strValue) envPeriod = openstudio::toQString(*strValue);
        s.str("");
        s << "select Zone from daylightmaps where MapNumber=" << *mapIndex;
        intValue = sqlFile.execAndReturnFirstInt(s.str());
        if (intValue)
        {
          s.str("");
          s << "select ZoneName from zones where ZoneIndex=" << *intValue;
          strValue = sqlFile.execAndReturnFirstString(s.str());
          if (strValue) keyValue = openstudio::toQString(*strValue);
        }
      }
      int row = addRow();
      item(row, m_slHeaders.indexOf("Alias"))->setText(alias);
      item(row, m_slHeaders.indexOf("File"))->setText(openstudio::toQString(sqlFile.energyPlusSqliteFile()));
      item(row,  m_slHeaders.indexOf("Environment Period"))->setText(envPeriod); // environment period
      /* update based on email from Dan 8/10/10
         item(row, 3)->setText("Illuminance"); // reporting frequency
         item(row, 4)->setText(keyValue); // illuminance zone?
         item(row, 5)->setText(openstudio::toQString(*nameIter));
         */
      item(row, m_slHeaders.indexOf("Reporting Frequency"))->setText("Hourly"); // reporting frequency
      item(row, m_slHeaders.indexOf("Key Value"))->setText(keyValue); // illuminance zone
      item(row, m_slHeaders.indexOf("Variable Name"))->setText("Illuminance Map"); // Variable Name
      item(row, m_slHeaders.indexOf("File"))->setData(Qt::UserRole, RVD_ILLUMINANCEMAP);
      item(row, m_slHeaders.indexOf("Alias"))->setData(Qt::UserRole, openstudio::toQString(*nameIter)); // map name for retrieving from database
    }

    resizeColumnToContents(m_slHeaders.indexOf("Alias"));
    hideColumn(m_slHeaders.indexOf("File"));
    resizeColumnToContents(m_slHeaders.indexOf("Variable Name"));
    resizeColumnToContents(m_slHeaders.indexOf("Key Value"));
    resizeColumnToContents(m_slHeaders.indexOf("Reporting Frequency"));
    //  resizeColumnToContents(5);

    setSortingEnabled(true);

    emit( fileAdded() );

    return true;
  }

  int TableView::addRow()
  {
    int row = rowCount();
    insertRow(row);

    auto aliasCol = new QTableWidgetItem;
    aliasCol->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(row,m_slHeaders.indexOf("Alias"),aliasCol);

    auto fileCol = new QTableWidgetItem;
    fileCol->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(row,m_slHeaders.indexOf("File"),fileCol);

    auto envPerCol = new QTableWidgetItem;
    envPerCol->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(row,m_slHeaders.indexOf("Environment Period"),envPerCol);

    auto timestepCol = new QTableWidgetItem;
    timestepCol->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(row,m_slHeaders.indexOf("Reporting Frequency"),timestepCol);

    auto zoneNameCol = new QTableWidgetItem;
    zoneNameCol->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(row,m_slHeaders.indexOf("Key Value"),zoneNameCol);

    auto varNameCol = new QTableWidgetItem;
    varNameCol->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setItem(row,m_slHeaders.indexOf("Variable Name"),varNameCol);

    return row;
  }

  int TableView::selectedRowCount()
  {
    std::vector<int> selectedRows;
    if (selectedItems().count() > 0)
    {
      for (QTableWidgetItem *item : selectedItems())  
      {
        int row = item->row();
        if ( std::find(selectedRows.begin(), selectedRows.end(), row) == selectedRows.end() )
        {
          selectedRows.push_back(row);
        }
      } // foreach
    }
    return selectedRows.size();
  }

  std::vector<int> TableView::selectedRows()
  {
    std::vector<int> selectedRows;
    if (selectedItems().count() > 0)
    {
      for (QTableWidgetItem *item : selectedItems())  
      {
        int row = item->row();
        if ( std::find(selectedRows.begin(), selectedRows.end(), row) == selectedRows.end() )
        {
          selectedRows.push_back(row);
        }
      } // foreach
    }
    return selectedRows;
  }


  std::vector<resultsviewer::ResultsViewerPlotData> TableView::generateResultsViewerPlotData()
  {
    std::vector<resultsviewer::ResultsViewerPlotData> resultsViewerPlotDataVec;
    std::vector<int> selectedRows;


    for (QTableWidgetItem *item : selectedItems())
    {
      int row = item->row();
      if ( std::find(selectedRows.begin(), selectedRows.end(), row) == selectedRows.end() )
      {
        selectedRows.push_back(row);
        resultsviewer::ResultsViewerPlotData resultsViewerPlotData = resultsViewerPlotDataFromTableItem(item);
        resultsViewerPlotDataVec.push_back(resultsViewerPlotData);
      }
    } // foreach

    return resultsViewerPlotDataVec;
  }

  void TableView::generateLinePlotData()
  {
    std::vector<resultsviewer::ResultsViewerPlotData> lpVec = generateResultsViewerPlotData();
    signalAddLinePlot(lpVec);
  }

  void TableView::generateFloodPlotData()
  {
    std::vector<resultsviewer::ResultsViewerPlotData> fpVec = generateResultsViewerPlotData();
    signalAddFloodPlot(fpVec);
  }

  void TableView::generateIlluminancePlotData()
  {
    std::vector<resultsviewer::ResultsViewerPlotData> fpVec = generateResultsViewerPlotData();
    signalAddIlluminancePlot(fpVec);
  }

  void TableView::generateIlluminancePlotComparisonData()
  {
    std::vector<resultsviewer::ResultsViewerPlotData> ipVec = generateResultsViewerPlotData();
    signalAddIlluminancePlotComparison(ipVec);
  }

  void TableView::generateReverseIlluminancePlotComparisonData()
  {
    std::vector<resultsviewer::ResultsViewerPlotData> ipVec = generateResultsViewerPlotData();
    std::vector<resultsviewer::ResultsViewerPlotData> ipVecRev(ipVec.rbegin(),ipVec.rend()); 
    signalAddIlluminancePlotComparison(ipVecRev);
  }


  void TableView::generateLinePlotComparisonData()
  {
    std::vector<resultsviewer::ResultsViewerPlotData> lpVec = generateResultsViewerPlotData();
    signalAddLinePlotComparison(lpVec);
  }

  void TableView::generateReverseLinePlotComparisonData()
  {
    std::vector<resultsviewer::ResultsViewerPlotData> lpVec = generateResultsViewerPlotData();
    std::vector<resultsviewer::ResultsViewerPlotData> lpVecRev(lpVec.rbegin(),lpVec.rend()); 
    signalAddLinePlotComparison(lpVecRev);
  }

  void TableView::generateFloodPlotComparisonData()
  {
    std::vector<resultsviewer::ResultsViewerPlotData> fpVec = generateResultsViewerPlotData();
    signalAddFloodPlotComparison(fpVec);
  }

  void TableView::generateReverseFloodPlotComparisonData()
  {
    std::vector<resultsviewer::ResultsViewerPlotData> fpVec = generateResultsViewerPlotData();
    std::vector<resultsviewer::ResultsViewerPlotData> fpVecRev(fpVec.rbegin(),fpVec.rend()); 
    signalAddFloodPlotComparison(fpVecRev);
  }

  resultsviewer::ResultsViewerPlotData TableView::resultsViewerPlotDataFromTableRow(int row)
  {
    resultsviewer::ResultsViewerPlotData rvPlotData;
    if ((row > -1) && (row < rowCount()))
    {
      rvPlotData = resultsViewerPlotDataFromTableItem(item(row,0));
    }
    return rvPlotData;
  }


  resultsviewer::ResultsViewerPlotData TableView::resultsViewerPlotDataFromTableItem(QTableWidgetItem* tableItem)
  {
    //  m_slHeaders  << tr("Alias") << tr("File") << tr("Environment Period") << tr("Reporting Frequency") << tr("Key Value") << tr("Variable Name");
    QString keyValue = "", variableName = "", envPeriod = "", filename = "", reportFreq = "", s = "";
    resultsviewer::ResultsViewerPlotData rvPlotData;
    if (tableItem) 
    {
      int row = tableItem->row();
      rvPlotData.keyName = item(row, m_slHeaders.indexOf("Key Value") )->text();
      rvPlotData.variableName = item(row, m_slHeaders.indexOf("Variable Name") )->text();
      rvPlotData.reportFreq = item(row, m_slHeaders.indexOf("Reporting Frequency") )->text();
      rvPlotData.envPeriod = item(row, m_slHeaders.indexOf("Environment Period") )->text();
      rvPlotData.filename = item(row, m_slHeaders.indexOf("File") )->text();
      rvPlotData.alias = item(row, m_slHeaders.indexOf("Alias") )->text();
      rvPlotData.dataType = item(row, m_slHeaders.indexOf("File") )->data(Qt::UserRole).toInt();
      rvPlotData.dbIdentifier = item(row, m_slHeaders.indexOf("Alias") )->data(Qt::UserRole).toString();
    }
    return rvPlotData;
  }



  bool TableView::updateFileAlias(const QString& alias, const QString& filename)
  {
    setSortingEnabled(false);
    for (int i=0; i<rowCount();i++)
    {
      if (item(i,1)->text().toUpper() == filename.toUpper()) item(i,0)->setText(alias);
    }
    setSortingEnabled(true);
    return true;
  }


  void TableView::applyFilter(QString& filterText)
  {
    //  QRegExp regExp(filterText); //strict regular expression matching
    QRegExp regExp(filterText, Qt::CaseInsensitive, QRegExp::Wildcard); // text wild card *, ? matching
    std::vector<int> rowsToShow;

    for (int row=0;row<rowCount();row++)
    {
      if (  (regExp.exactMatch(item(row,m_slHeaders.indexOf("Variable Name"))->text()))
          || (regExp.exactMatch(item(row,m_slHeaders.indexOf("Key Value"))->text()))
          || (regExp.exactMatch(item(row,m_slHeaders.indexOf("Reporting Frequency"))->text()))
          || (regExp.exactMatch(item(row,m_slHeaders.indexOf("Environment Period"))->text()))
          //       || (regExp.exactMatch(item(row,m_slHeaders.indexOf("File"))->text()))
          || (regExp.exactMatch(item(row,m_slHeaders.indexOf("Alias"))->text())) )   rowsToShow.push_back(row);
    }
    for (int i=0; i<rowCount();i++)
    {
      if ( std::find(rowsToShow.begin(), rowsToShow.end(), i) == rowsToShow.end() )
        hideRow(i);
      else
        showRow(i);
    }
  }

  void TableView::clearFilter()
  {
    for (int i=0; i<rowCount();i++) showRow(i);
  }



  void TableView::goToFile(const QString& filename)
  {
    for (int i=0;i<rowCount();i++)
    {

      if ( (!isRowHidden(i)) && (item(i,m_slHeaders.indexOf("File"))->text().toUpper() == filename.toUpper()) )
      {
        scrollToItem(item(i,0), QAbstractItemView::PositionAtTop);
        break;
      }
    }
  }

};
