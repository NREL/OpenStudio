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

#ifndef PATAPP_RUNTABCONTROLLER_HPP
#define PATAPP_RUNTABCONTROLLER_HPP

#include "../shared_gui_components/OSListController.hpp"
#include "../shared_gui_components/OSListView.hpp"
#include "PatConstants.hpp"

#include "../analysis/Analysis.hpp"
#include "../analysis/DataPoint.hpp"
#include "../analysis/Measure.hpp"
#include "../analysis/Problem.hpp"
#include "../analysisdriver/AnalysisDriverEnums.hpp"

#include "../runmanager/lib/Job.hpp"

#include <QObject>
#include <QPointer>
#include <QSharedPointer>

namespace openstudio {

class VagrantProvider;

namespace pat {

class RunView;
class DataPointRunListController;
class DataPointRunItemDelegate;

/// Controller class for the entire run tab
class RunTabController : public QObject
{
  Q_OBJECT

  public:

    QPointer<RunView> runView;

    RunTabController();

    virtual ~RunTabController();

  public slots:

    void onPlayButtonClicked();

    void onIterationProgress();

    void onDataPointQueued(const openstudio::UUID& analysis, const openstudio::UUID& dataPoint);

    void onDataPointResultsCleared(const openstudio::UUID& dataPoint);

    void requestRefresh();

    void refresh();

    void onRadianceEnabledChanged(bool t_radianceEnabled);

    void seedChanged();

    bool checkSeedForRadianceWarningsAndErrors(boost::optional<model::Model> &t_seedModel, runmanager::RunManager &t_runManager);

  private:

    bool m_refreshScheduled;

    bool m_radianceEnabled;

    QSharedPointer<DataPointRunListController> m_dataPointRunListController;

    QSharedPointer<DataPointRunItemDelegate> m_dataPointRunItemDelegate;

    REGISTER_LOGGER("openstudio.pat.RunTabController");

    // converts UUID to index i and emits signal to get DataPoint job details to refresh
    void emitDataPointChanged(const openstudio::UUID& dataPoint);

    void showRadianceWarningsAndErrors(const std::vector<std::string> & warnings,
                                            const std::vector<std::string> & errors);
};

/// Controller class for the list of data points on the run tab
class DataPointRunListController : public OSListController
{
  Q_OBJECT

 public:

  DataPointRunListController(const openstudio::analysis::Analysis& analysis);

  virtual ~DataPointRunListController() {}

  /// The OSListItem returned will be a DataPointRunListItem
  QSharedPointer<OSListItem> itemAt(int i);

  int count();

  void emitItemChanged(int i);

 private:

   openstudio::analysis::Analysis m_analysis;
};

/// Item representing a data point on the run tab
class DataPointRunListItem : public OSListItem
{
  Q_OBJECT

 public:

  DataPointRunListItem(const openstudio::analysis::DataPoint& dataPoint);

  virtual ~DataPointRunListItem() {}

  openstudio::analysis::DataPoint dataPoint() const;

 private:

  std::vector<openstudio::analysis::DataPoint> dataPoints();

  openstudio::analysis::DataPoint m_dataPoint;
};

/// Delegate which creates a DataPointRunListItem
class DataPointRunItemDelegate : public OSItemDelegate
{
  Q_OBJECT

 public:

  virtual ~DataPointRunItemDelegate() {}

  /// Widget returned will be a DataPointRunListItem
  QWidget * view(QSharedPointer<OSListItem> dataSource);

 signals:

  void dataPointResultsCleared(const openstudio::UUID& dataPoint);
};

/// Controller class for the list jobs for a data points on the run tab
class DataPointJobController : public OSListController
{
  Q_OBJECT

 public:

  DataPointJobController(const analysis::DataPoint& dataPoint);

  virtual ~DataPointJobController() {}

  /// The OSListItem returned will be a DataPointJobItem
  QSharedPointer<OSListItem> itemAt(int i);

  int count();

 private:

  std::vector<analysis::WorkflowStepJob> getJobsByWorkflowStep(const openstudio::analysis::DataPoint& dataPoint);

  analysis::DataPoint m_dataPoint;
};

/// Item representing a job for a data point on the run tab
class DataPointJobItem : public OSListItem
{
  Q_OBJECT

 public:

  DataPointJobItem(const analysis::WorkflowStepJob& workflowStepJob);

  virtual ~DataPointJobItem() {}

  analysis::WorkflowStepJob workflowStepJob() const;

 private:

  analysis::WorkflowStepJob m_workflowStepJob;

};

/// Delegate which creates a DataPointJobItemView
class DataPointJobItemDelegate : public OSItemDelegate
{
  Q_OBJECT

  public:

  virtual ~DataPointJobItemDelegate() {}

  /// Widget returned will be a DataPointJobItemView
  QWidget * view(QSharedPointer<OSListItem> dataSource);
};

}

} // openstudio

#endif // PATAPP_RUNTABCONTROLLER_HPP
