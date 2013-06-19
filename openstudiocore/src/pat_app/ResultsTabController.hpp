/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_RESULTSTABCONTROLLER_H
#define OPENSTUDIO_RESULTSTABCONTROLLER_H

#include "../shared_gui_components/OSListController.hpp"
#include "../shared_gui_components/OSListView.hpp"

#include <analysis/Analysis.hpp>
#include <analysis/DataPoint.hpp>

#include <QObject>
#include <QSharedPointer>
#include <QPointer>

namespace openstudio {
  
namespace pat { 

class ResultsView;
class BaselineDataPointResultListController;
class DataPointResultsListController;
class DataPointResultItemDelegate;

class ResultsTabController : public QObject
{
  Q_OBJECT

  public:

    QPointer<ResultsView> resultsView;

    ResultsTabController();

    virtual ~ResultsTabController();

  private slots:

    void onOpenButtonClicked();

    void openDirectory();

    void enableViewFileButton();

    void disableViewFileButton();

    void enableOpenDirectoryButton();

    void disableOpenDirectoryButton();

  private:

    QSharedPointer<BaselineDataPointResultListController> m_baselineDataPointResultListController;
    QSharedPointer<DataPointResultsListController> m_dataPointResultsListController;
    QSharedPointer<DataPointResultItemDelegate> m_dataPointResultItemDelegate;
};

/// Item representing a data point on the results tab
class DataPointResultListItem : public OSListItem
{
  Q_OBJECT

 public:

  DataPointResultListItem(const openstudio::analysis::DataPoint& dataPoint,
                          const openstudio::analysis::DataPoint& baselineDataPoint,
                          bool aleternateRow);

  virtual ~DataPointResultListItem() {}

  openstudio::analysis::DataPoint dataPoint() const;
  openstudio::analysis::DataPoint baselineDataPoint() const;
  bool aleternateRow() const;

 private:

  openstudio::analysis::DataPoint m_dataPoint;
  openstudio::analysis::DataPoint m_baselineDataPoint;
  bool m_aleternateRow;
};

/// Delegate which creates a DataPointResultListItem
class DataPointResultItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  public:

  virtual ~DataPointResultItemDelegate() {}

  /// Widget returned will be a DataPointResultListItem
  QWidget * view(QSharedPointer<OSListItem> dataSource);
};

/// Controller class for the list of baseline data points on the run tab
class BaselineDataPointResultListController : public OSListController
{
  Q_OBJECT

 public:

  BaselineDataPointResultListController(const openstudio::analysis::Analysis& analysis);

  virtual ~BaselineDataPointResultListController() {}

  /// The OSListItem returned will be a DataPointResultListItem
  QSharedPointer<OSListItem> itemAt(int i);

  int count();

 private:

   openstudio::analysis::Analysis m_analysis;
};

/// Controller class for the list of non data points on the run tab
class DataPointResultsListController : public OSListController
{
  Q_OBJECT

 public:

  DataPointResultsListController(const openstudio::analysis::Analysis& analysis);

  virtual ~DataPointResultsListController() {}

  /// The OSListItem returned will be a DataPointResultListItem
  QSharedPointer<OSListItem> itemAt(int i);

  int count();

 private:

   std::vector<openstudio::analysis::DataPoint> dataPoints();

   openstudio::analysis::Analysis m_analysis;
};

}

} // openstudio

#endif // OPENSTUDIO_RESULTSTABCONTROLLER_H
