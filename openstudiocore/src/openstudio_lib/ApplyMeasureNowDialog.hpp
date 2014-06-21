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

#ifndef OPENSTUDIO_APPLYMEASURENOWDIALOG_H
#define OPENSTUDIO_APPLYMEASURENOWDIALOG_H

#include <shared_gui_components/HeaderViews.hpp>
#include <shared_gui_components/OSDialog.hpp>
#include <shared_gui_components/OSListView.hpp>

#include <analysis/Problem.hpp>
#include <analysis/RubyMeasure.hpp>

#include <shared_gui_components/VariableList.hpp>

#include <utilities/bcl/BCLMeasure.hpp>

#include <model/Model.hpp>

#include <analysis/Problem.hpp>
#include <analysis/RubyMeasure.hpp>

#include <utilities/bcl/BCLMeasure.hpp>

class QPushButton;
class QStackedWidget;
class QTextEdit;
class QTimer;

namespace openstudio{

class DateTime;
class EditController;
class LocalLibraryController;

// Local Forward Decs
class DataPointJobItemView;

namespace runmanager {
  class AdvancedStatus;
}

class ApplyMeasureNowDialog : public OSDialog
{
  Q_OBJECT

public:

  ApplyMeasureNowDialog(QWidget * parent = 0);

  virtual ~ApplyMeasureNowDialog();

  QSize sizeHint() const;

  void displayMeasure();

  QSharedPointer<EditController> m_editController;

  QSharedPointer<LocalLibraryController> m_localLibraryController;

protected slots:

  virtual void on_cancelButton(bool checked);

  virtual void on_backButton(bool checked);

  virtual void on_okButton(bool checked);

protected:

  void closeEvent(QCloseEvent * event);

private slots:

  void disableOkButton(bool disable);

  void requestReload();

  void showAdvancedOutput();

  void displayResults();

signals:

  void reloadFile(const QString& fileToLoad, bool modified, bool saveCurrentTabs);

  void toolsUpdated();

private:

  void createWidgets();

  void runMeasure();

  void searchForExistingResults(const openstudio::path &t_runDir);

  void removeWorkingDir();

  boost::optional<BCLMeasure> m_bclMeasure;

  QSharedPointer<measuretab::MeasureItem> m_currentMeasureItem;

  boost::optional<runmanager::Job> m_job;

  boost::optional<model::Model> m_model;

  boost::optional<openstudio::path> m_reloadPath;

  QStackedWidget * m_mainPaneStackedWidget;

  QStackedWidget * m_rightPaneStackedWidget;

  QTextEdit * m_argumentsFailedTextEdit;

  DataPointJobItemView * m_jobItemView;

  QTimer * m_timer;

  int m_inputPageIdx;

  int m_runningPageIdx;

  int m_outputPageIdx;

  int m_argumentsFailedPageIdx;

  int m_argumentsOkPageIdx;

  QLabel * m_jobPath;

  QPushButton * m_showAdvancedOutput;

  QString m_advancedOutput;

  openstudio::path m_workingDir;

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

  static QString formatMessageForHTML(const std::string &t_message);

  QLabel * m_textEdit;

};

class DataPointJobItemView : public OSCollapsibleView
{
  Q_OBJECT

public:

  DataPointJobItemView();

  virtual ~DataPointJobItemView() {}

  DataPointJobHeaderView * m_dataPointJobHeaderView;

  DataPointJobContentView * m_dataPointJobContentView;

protected:

  void paintEvent(QPaintEvent * e);

public slots:

  void update(analysis::RubyMeasure & rubyMeasure, BCLMeasure & bclMeasure, openstudio::runmanager::JobErrors jobErrors, openstudio::runmanager::Job job);

};

} // openstudio

#endif // OPENSTUDIO_APPLYMEASURENOWDIALOG_H
