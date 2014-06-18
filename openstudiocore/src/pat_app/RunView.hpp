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

#ifndef PATAPP_RUNVIEW_HPP
#define PATAPP_RUNVIEW_HPP

#include "PatMainTabView.hpp"
#include "PatConstants.hpp"

#include "../shared_gui_components/OSListView.hpp"
#include "../shared_gui_components/HeaderViews.hpp"
#include "../shared_gui_components/Buttons.hpp"

#include "../analysis/Problem.hpp"
#include "../analysis/DataPoint.hpp"

#include "../analysisdriver/AnalysisDriverEnums.hpp"

#include <boost/optional.hpp>

#include <QProgressBar>

class QPushButton;
class QRadioButton;

namespace openstudio{

class DateTime;

namespace runmanager {
  class AdvancedStatus;
}

namespace analysis {
  class DataPoint;
}

namespace pat {

class PatProgressBar;
class RunStatusView;
class ToggleCloudButton;

class RunView : public PatMainTabView
{
  Q_OBJECT

  public:

    RunView();

    virtual ~RunView() {}

    QWidget * mainContent;
    
    RunStatusView * runStatusView;

    OSListView * dataPointRunListView;
};


class RunStatusView : public QWidget
{
  Q_OBJECT

  public:

   RunStatusView();

   virtual ~RunStatusView() {}

   void setProgress(int numCompletedJobs, int numFailedJobs, int numJobsInIteration);

   PlayButton * playButton;

   QLabel * m_playLabel;

   QString m_runText;


   CloudOnButton * cloudOnButton;

   CloudStartingButton * cloudStartingButton;

   CloudOffButton * cloudOffButton;

   CloudStoppingButton * cloudStoppingButton;

   CloudLostConnectionButton * cloudLostConnectionButton;

   void paintEvent(QPaintEvent * e);

 signals:

   void dataPointResultsCleared(const openstudio::UUID& dataPoint);

   void radianceEnabledChanged(bool enabled);

 public slots:
   
  void setStatus(const CloudStatus & cloudStatus, analysisdriver::AnalysisStatus analysisStatus);

  void setRadianceEnabled(bool t_radianceEnabled);

  void radianceToggled(bool t_state);

 private slots:

  void on_selectAllDataPoints(bool checked);

  void on_clearSelectionDataPoints(bool checked);

  void on_selectAllDownloads(bool checked);

  void on_selectAllClears(bool checked);

  void updateCloudData();

 private:

  friend class RunTabController;

  PatProgressBar* m_progressBar;
  QLabel* m_percentComplete;
  QLabel* m_percentFailed;
  QPushButton * m_selectAllClears;
  QPushButton * m_selectAllDownloads;
  QLabel * m_selectAllDownloadsLabel;
  QPushButton * m_selectAllDataPoints;
  QPushButton * m_clearSelectionDataPoints;
  QLabel * m_cloudTime;
  QLabel * m_cloudInstances;
  QTimer * m_timer;
  QRadioButton *m_energyPlus;
  QRadioButton *m_radiance;
  bool m_disableRadianceEvents;
};

class DataPointRunHeaderView : public OSHeader
{
  Q_OBJECT

 public:

  DataPointRunHeaderView(const openstudio::analysis::DataPoint& dataPoint);

  virtual ~DataPointRunHeaderView() {}

 signals:

  void dataPointResultsCleared(const openstudio::UUID& dataPoint);

 public slots:

  void update();

  void requestUpdate();

 private slots:

  void on_clicked(bool checked);

  void on_downloadClicked(bool checked);

  void on_clearClicked(bool checked);

 private:

  openstudio::analysis::DataPoint m_dataPoint;

  QLabel* m_name;
  QLabel* m_lastRunTime;
  QLabel* m_status;
  QLabel* m_nas;
  QLabel* m_warnings;
  QLabel* m_errors;

  QPushButton* m_download;
  QPushButton* m_clear;

  bool m_updateRequested;
};

class DataPointRunContentView : public OSListView
{
  Q_OBJECT

  public:

  DataPointRunContentView();

  virtual ~DataPointRunContentView() {}

};

class DataPointRunItemView : public OSCollapsibleView
{
  Q_OBJECT

 public:

  DataPointRunItemView(const openstudio::analysis::DataPoint& dataPoint);

  virtual ~DataPointRunItemView() {}

  DataPointRunHeaderView * dataPointRunHeaderView;

  DataPointRunContentView * dataPointRunContentView;
 
 signals:

  void dataPointResultsCleared(const openstudio::UUID& dataPoint);

 public slots:

  void checkForUpdate();

 private:

  openstudio::analysis::DataPoint m_dataPoint;
  UUID m_topLevelJobUUID;
};

class DataPointJobHeaderView : public OSHeader
{
  Q_OBJECT

 public:

  DataPointJobHeaderView();

  virtual ~DataPointJobHeaderView() {}

  void setName(const std::string& name);

  void setLastRunTime(const boost::optional<openstudio::DateTime>& lastRunTime);

  void setStatus(const openstudio::runmanager::AdvancedStatus& status, bool isCanceled);

  void setNA(bool na);

  void setNumWarnings(unsigned numWarnings);

  void setNumErrors(unsigned numErrors);

 private:

  QLabel* m_name;
  QLabel* m_lastRunTime;
  QLabel* m_status;
  QLabel* m_na;
  QLabel* m_warnings;
  QLabel* m_errors;
};

class DataPointJobContentView : public QWidget
{
  Q_OBJECT

 public:

  DataPointJobContentView();

  virtual ~DataPointJobContentView() {}

  void clear();

  void addInitialConditionMessage(const std::string& message);

  void addFinalConditionMessage(const std::string& message);

  void addInfoMessage(const std::string& message);

  void addWarningMessage(const std::string& message, int count = 1);

  void addErrorMessage(const std::string& message);

  void addStdErrorMessage(const std::string& message);

 private:
  static QString formatMessageForHTML(const std::string &t_message);

  QLabel * m_textEdit;

};

class DataPointJobItemView : public OSCollapsibleView
{
  Q_OBJECT

public:

  DataPointJobItemView(const analysis::WorkflowStepJob& workflowStepJob);

  virtual ~DataPointJobItemView() {}

  DataPointJobHeaderView * dataPointJobHeaderView;

  DataPointJobContentView * dataPointJobContentView;

protected:

  void paintEvent(QPaintEvent * e);

public slots:

  void update();

  void requestUpdate();

private:

  analysis::WorkflowStepJob m_workflowStepJob;

  bool m_updateRequested;

};

class PatProgressBar : public QProgressBar
{
  Q_OBJECT

  public:

  PatProgressBar();

  virtual ~PatProgressBar() {}

  public slots:

  void setValue(int value);
};

}

} // openstudio

#endif // PATAPP_RUNVIEW_HPP
