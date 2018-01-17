/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

    //void onRadianceEnabledChanged(bool t_radianceEnabled);

    void seedChanged();

    //bool checkSeedForRadianceWarningsAndErrors(boost::optional<model::Model> &t_seedModel, runmanager::RunManager &t_runManager);

  private:

    bool m_refreshScheduled;

    //bool m_radianceEnabled;

    QSharedPointer<DataPointRunListController> m_dataPointRunListController;

    QSharedPointer<DataPointRunItemDelegate> m_dataPointRunItemDelegate;

    REGISTER_LOGGER("openstudio.pat.RunTabController");

    // converts UUID to index i and emits signal to get DataPoint job details to refresh
    void emitDataPointChanged(const openstudio::UUID& dataPoint);

    //void showRadianceWarningsAndErrors(const std::vector<std::string> & warnings,
    //                                        const std::vector<std::string> & errors);
};

/// Controller class for the list of data points on the run tab
class DataPointRunListController : public OSListController
{
  Q_OBJECT

 public:

  DataPointRunListController(const openstudio::analysis::Analysis& analysis);

  virtual ~DataPointRunListController() {}

  /// The OSListItem returned will be a DataPointRunListItem
  QSharedPointer<OSListItem> itemAt(int i) override;

  int count() override;

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
  QWidget * view(QSharedPointer<OSListItem> dataSource) override;

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
  QSharedPointer<OSListItem> itemAt(int i) override;

  int count() override;

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
  QWidget * view(QSharedPointer<OSListItem> dataSource) override;
};

}

} // openstudio

#endif // PATAPP_RUNTABCONTROLLER_HPP
