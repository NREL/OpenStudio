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

#ifndef OPENSTUDIO_RUNVIEW_H
#define OPENSTUDIO_RUNVIEW_H

#include <pat_app/PatMainTabView.hpp>

#include "../shared_gui_components/OSListView.hpp"
#include "../shared_gui_components/HeaderViews.hpp"
#include "../shared_gui_components/Buttons.hpp"

#include <analysis/Problem.hpp>
#include <analysis/DataPoint.hpp>

#include <boost/optional.hpp>

#include <QProgressBar>

class QPushButton;

namespace openstudio{

class DateTime;

namespace runmanager {
  class AdvancedStatus;
}

namespace analysis {
  class DataPoint;
}

class PlayButton;

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

class ToggleCloudButton : public GrayButton
{
  Q_OBJECT

 public:

  enum Status { STARTING, RUNNING, STOPPING, STOPPED };

  ToggleCloudButton();

  void setStatus(Status status);

 protected:

  void updateText();

 private:

  Status m_status;

  QString m_turnOnText;

  QString m_turnOffText;
};

class RunStatusView : public QWidget
{
  Q_OBJECT

 public:

   RunStatusView();

   virtual ~RunStatusView() {}

   void setRunning(bool isRunning);

   void setProgress(int numCompletedJobs, int numFailedJobs, int numJobsInIteration, bool isRunning);

   ToggleCloudButton * toggleCloudButton;

   void paintEvent(QPaintEvent * e);

 signals:

   void playButtonClicked(bool);

   void internetAvailable(bool isAvailable);

 private slots:

  void on_internetAvailable(bool isAvailable);

  void on_cloudStatus(ToggleCloudButton::Status status);

  void on_selectAllDownloads(bool checked);

  void on_selectAllClears(bool checked);

 private:

  PlayButton* m_playButton;
  PatProgressBar* m_progressBar;
  QLabel* m_percentComplete;
  QLabel* m_percentFailed;
  QLabel* m_cloudProviderStatus;
  QPushButton * m_selectAllClears;
  QPushButton * m_selectAllDownloads;
};

class DataPointRunHeaderView : public OSHeader
{
  Q_OBJECT

 public:

  DataPointRunHeaderView(const openstudio::analysis::DataPoint& dataPoint);

  virtual ~DataPointRunHeaderView() {}

  enum ClearBtnState{
    CAN_CLEAR,
    CANT_CLEAR
  };

  enum DownloadBtnState{
    CAN_DOWNLOAD,
    CANT_DOWNLOAD,
    DOWNLOADED,
    NOT_VISIBLE
  };

  ClearBtnState m_clearBtnState;

  DownloadBtnState m_downloadBtnState;

 public slots:

  void update();

 private slots:

  void on_clicked(bool checked);

  void on_downloadClicked(bool checked);

  void on_clearClicked(bool checked);

  void on_internetAvailable(bool isAvailable);

 private:

  void setDownloadEnabled(const bool enabled);

  void setClearState(bool hasDataToClear);

  openstudio::analysis::DataPoint m_dataPoint;

  QLabel* m_name;
  QLabel* m_lastRunTime;
  QLabel* m_status;
  QLabel* m_nas;
  QLabel* m_warnings;
  QLabel* m_errors;

  QPushButton* m_download;
  QPushButton* m_clear;
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
};

class DataPointJobHeaderView : public OSHeader
{
  Q_OBJECT

 public:

  DataPointJobHeaderView();

  virtual ~DataPointJobHeaderView() {}

  void setName(const std::string& name);

  void setLastRunTime(const boost::optional<openstudio::DateTime>& lastRunTime);

  void setStatus(const openstudio::runmanager::AdvancedStatus& status);

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

  void addWarningMessage(const std::string& message);

  void addErrorMessage(const std::string& message);

  void addStdErrorMessage(const std::string& message);

 private:

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

private:

  analysis::WorkflowStepJob m_workflowStepJob;

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

#endif // OPENSTUDIO_RUNVIEW_H
