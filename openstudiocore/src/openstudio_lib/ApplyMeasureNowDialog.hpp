/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef OPENSTUDIO_APPLYMEASURENOWDIALOG_HPP
#define OPENSTUDIO_APPLYMEASURENOWDIALOG_HPP

#include "../shared_gui_components/HeaderViews.hpp"
#include "../shared_gui_components/OSDialog.hpp"
#include "../shared_gui_components/OSListView.hpp"

#include "../model/Model.hpp"

#include "../utilities/bcl/BCLMeasure.hpp"

class QPushButton;
class QStackedWidget;
class QTextEdit;
class QTimer;
class QProcess;

namespace openstudio{

class DateTime;
class EditController;
class LocalLibraryController;
class WorkflowStepResult;
class WorkflowJSON;
class TextEditDialog;

namespace measuretab{
  class MeasureStepItem;
}

// Local Forward Decs
class DataPointJobItemView;

class ApplyMeasureNowDialog : public OSDialog
{
  Q_OBJECT

public:

  ApplyMeasureNowDialog(QWidget * parent = nullptr);

  virtual ~ApplyMeasureNowDialog();

  QSize sizeHint() const override;

  void displayMeasure();

  QSharedPointer<EditController> m_editController;

  QSharedPointer<LocalLibraryController> m_localLibraryController;

protected slots:

  virtual void on_cancelButton(bool checked) override;

  virtual void on_backButton(bool checked) override;

  virtual void on_okButton(bool checked) override;

protected:

  void closeEvent(QCloseEvent * event) override;

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

  QSharedPointer<measuretab::MeasureStepItem> m_currentMeasureStepItem;

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

  QProcess * m_runProcess;

  QLabel * m_jobPath;

  QPushButton * m_showAdvancedOutput;

  QString m_advancedOutput;

  openstudio::path m_workingDir;

  TextEditDialog * m_advancedOutputDialog;

  WorkflowJSON m_modelWorkflowJSON;

  WorkflowJSON m_tempWorkflowJSON;
};

class DataPointJobHeaderView : public OSHeader
{
  Q_OBJECT

 public:

  DataPointJobHeaderView();

  virtual ~DataPointJobHeaderView() {}

  void setName(const std::string& name);

  void setLastRunTime(const boost::optional<openstudio::DateTime>& lastRunTime);

  void setStatus(const std::string& status, bool isCanceled);

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

  void paintEvent(QPaintEvent * e) override;

public slots:

  void update(const BCLMeasure & bclMeasure, const boost::optional<WorkflowJSON>& outWorkflowJSON, bool canceled);

};

} // openstudio

#endif // OPENSTUDIO_APPLYMEASURENOWDIALOG_HPP
