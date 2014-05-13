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

#include "ApplyMeasureNowDialog.hpp"

#include <shared_gui_components/BusyWidget.hpp>
#include <shared_gui_components/EditController.hpp>
#include <shared_gui_components/EditView.hpp>
#include <shared_gui_components/LocalLibraryController.hpp>
#include <shared_gui_components/LocalLibraryView.hpp>
#include <shared_gui_components/MeasureManager.hpp>
#include <shared_gui_components/OSViewSwitcher.hpp>
#include <shared_gui_components/VariableList.hpp>
//#include <shared_gui_components/WorkflowTools.hpp>

//#include <openstudio_lib/MainRightColumnController.hpp>
#include <openstudio_lib/OSAppBase.hpp>
#include <openstudio_lib/OSDocument.hpp>
#include <openstudio_lib/OSItem.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>

//#include <analysis/Analysis.hpp>
//#include <analysis/DataPoint.hpp>
#include <analysis/NullMeasure.hpp>
//#include <analysis/Problem.hpp>

#include <analysisdriver/SimpleProject.hpp>

#include <utilities/core/RubyException.hpp>

#include <QBoxLayout>
#include <QCloseEvent>
#include <QLabel>
#include <QMessageBox>
#include <QPointer>
#include <QPushButton>
#include <QSharedPointer>
#include <QStackedWidget>
#include <QTextEdit>
#include <QTimer>

#define FAILED_ARG_TEXT "<FONT COLOR = RED>Failed to Show Arguments<FONT COLOR = BLACK> <br> <br>Reason(s): <br> <br>"

#define ACCEPT_CHANGES "Accept Changes"
#define APPLY_MEASURE "Apply Measure"

namespace openstudio {

ApplyMeasureNowDialog::ApplyMeasureNowDialog(QWidget* parent)
  : OSDialog(false, parent)
{
  setWindowTitle("Apply Measure Now");
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  createWidgets();
}

ApplyMeasureNowDialog::~ApplyMeasureNowDialog()
{
}

QSize ApplyMeasureNowDialog::sizeHint() const
{
  return QSize(770,540);
}

void ApplyMeasureNowDialog::createWidgets()
{
  QWidget * widget = 0;
  QBoxLayout * layout = 0;
  QLabel * label = 0;
  bool isConnected = false;

  // PAGE STACKED WIDGET

  m_mainPaneStackedWidget = new  QStackedWidget();
  upperLayout()->addWidget(m_mainPaneStackedWidget);

  // INPUT

  m_argumentsFailedTextEdit = new QTextEdit(FAILED_ARG_TEXT);
  m_argumentsFailedTextEdit->setReadOnly(true);
  
  m_editController = QSharedPointer<EditController>( new EditController() );

  m_localLibraryController = QSharedPointer<LocalLibraryController>( new LocalLibraryController(OSAppBase::instance()) );

  m_rightPaneStackedWidget = new  QStackedWidget();
  m_argumentsFailedPageIdx = m_rightPaneStackedWidget->addWidget(m_argumentsFailedTextEdit);

  OSViewSwitcher * viewSwitcher = new OSViewSwitcher();
  viewSwitcher->setView(m_editController->editView);
  m_argumentsOkPageIdx = m_rightPaneStackedWidget->addWidget(viewSwitcher);

  m_rightPaneStackedWidget->setCurrentIndex(m_argumentsOkPageIdx);

  layout = new QHBoxLayout();
  layout->addWidget(m_localLibraryController->localLibraryView);
  layout->addWidget(m_rightPaneStackedWidget);

  widget = new QWidget();
  widget->setLayout(layout);
  m_inputPageIdx = m_mainPaneStackedWidget->addWidget(widget);

  // RUNNING

  label = new QLabel("Running Measure");
  label->setObjectName("H2");

  BusyWidget * busyWidget = new BusyWidget();

  m_timer = new QTimer(this);
  connect(m_timer,SIGNAL(timeout()),busyWidget,SLOT(rotate()));

  layout = new QVBoxLayout();
  layout->addStretch();
  layout->addWidget(label,0,Qt::AlignCenter);
  layout->addWidget(busyWidget,0,Qt::AlignCenter);
  layout->addStretch();
  
  widget = new QWidget();
  widget->setLayout(layout);
  m_runningPageIdx = m_mainPaneStackedWidget->addWidget(widget);

  m_mainPaneStackedWidget->setCurrentIndex(m_inputPageIdx);

  // OUTPUT

  label = new QLabel("Measure Output");
  label->setObjectName("H1");

  m_outputWindow = new QTextEdit("Results go here");
  m_outputWindow->setReadOnly(true);

  layout = new QVBoxLayout();
  layout->addWidget(label);
  layout->addWidget(m_outputWindow);

  widget = new QWidget();
  widget->setLayout(layout);
  m_outputPageIdx = m_mainPaneStackedWidget->addWidget(widget);

  // BUTTONS

  this->okButton()->setText(APPLY_MEASURE);
  this->okButton()->setEnabled(false);

  // OS SETTINGS

  #ifdef Q_OS_MAC
    setWindowFlags(Qt::FramelessWindowHint);
  #elif defined(Q_OS_WIN32)
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
  #endif

}

//***** SLOTS *****

void ApplyMeasureNowDialog::on_cancelButton(bool checked)
{
  if(m_mainPaneStackedWidget->currentIndex() == m_inputPageIdx){
    // N/A
  } else if(m_mainPaneStackedWidget->currentIndex() == m_runningPageIdx) {
    m_mainPaneStackedWidget->setCurrentIndex(m_inputPageIdx);
    m_timer->stop();
    this->okButton()->show();
    return;
  } else if(m_mainPaneStackedWidget->currentIndex() == m_outputPageIdx) {
    // N/A
  }
  
  OSDialog::on_cancelButton(checked);
}

void ApplyMeasureNowDialog::on_okButton(bool checked)
{
  if(m_mainPaneStackedWidget->currentIndex() == m_inputPageIdx){
    m_mainPaneStackedWidget->setCurrentIndex(m_runningPageIdx);
    m_timer->start(50);
    this->okButton()->hide();
    runMeasure();
  } else if(m_mainPaneStackedWidget->currentIndex() == m_runningPageIdx) {
    // N/A
    m_mainPaneStackedWidget->setCurrentIndex(m_outputPageIdx); // TODO remove
  } else if(m_mainPaneStackedWidget->currentIndex() == m_outputPageIdx) {
    // N/A
    m_mainPaneStackedWidget->setCurrentIndex(m_inputPageIdx); // TODO remove
  }
}

  //m_mainPaneStackedWidget->setCurrentIndex(m_outputPageIdx);
  //m_timer->stop(); // TODO
  //this->okButton()->setText(ACCEPT_CHANGES);
  //this->okButton()->show();

void ApplyMeasureNowDialog::displayMeasure()
{
  this->okButton()->setText(ACCEPT_CHANGES);
  this->okButton()->show();
  this->okButton()->setEnabled(false);
  m_rightPaneStackedWidget->setCurrentIndex(m_argumentsOkPageIdx);

  openstudio::BaseApp * app = OSAppBase::instance();

  QPointer<LibraryItem> selectedItem = m_localLibraryController->selectedItem();
  UUID id = selectedItem->uuid();

  boost::optional<analysisdriver::SimpleProject> project = OSAppBase::instance()->project();
  OS_ASSERT(project);

  try {
    // Make a new variable
    boost::optional<BCLMeasure> bclMeasure = app->measureManager().getMeasure(id);
    OS_ASSERT(bclMeasure);

    // prep discrete variable
    std::string name = app->measureManager().suggestMeasureGroupName(*bclMeasure);
    analysis::MeasureGroup dv(name, analysis::MeasureVector());
    dv.setDisplayName(name);
   
    // measure
    analysis::RubyMeasure measure(*bclMeasure);
    try{
      measure.setArguments(app->measureManager().getArguments(*project, *bclMeasure));
    } catch (const RubyException & e) {
      QString errorMessage("Failed to compute arguments for measure: \n\n");
      errorMessage += QString::fromStdString(e.what());
      errorMessage.prepend(FAILED_ARG_TEXT);
      m_argumentsFailedTextEdit->setText(errorMessage);
      m_rightPaneStackedWidget->setCurrentIndex(m_argumentsFailedPageIdx);
      return;
    }

    // TODO Should this be "fixed"
    // null measure
    analysis::NullMeasure nullPert;
    dv.push(nullPert);

    // the new measure
    name = app->measureManager().suggestMeasureName(*bclMeasure, true); // TODO is this "fixed"
    measure.setName(name);
    measure.setDisplayName(name);
    measure.setDescription(bclMeasure->description());
    dv.push(measure);

    QSharedPointer<measuretab::MeasureItem> item = QSharedPointer<measuretab::MeasureItem>(new measuretab::MeasureItem(measure, app));

    bool isConnected = false;
    isConnected = connect(item.data(),SIGNAL(argumentsChanged(bool)),
      this,SLOT(disableOkButton(bool)));
    OS_ASSERT(isConnected);

    bool hasIncompleteArguments = item->hasIncompleteArguments();
    disableOkButton(hasIncompleteArguments);

    m_editController->setMeasureItem(item.data(), app);

  } catch (const std::exception & e) {
    QString errorMessage("Failed to display measure: \n\n");
    errorMessage += QString::fromStdString(e.what());
    errorMessage.prepend(FAILED_ARG_TEXT);
    m_argumentsFailedTextEdit->setText(errorMessage);
    m_rightPaneStackedWidget->setCurrentIndex(m_argumentsFailedPageIdx);
    return;
  }


  // TODO use when ready to run
  // clone model
  boost::optional<model::Model> model = app->currentModel();
  OS_ASSERT(model);
  model::Model modelCopy = model->clone().cast<model::Model>();

}

void ApplyMeasureNowDialog::runMeasure()
{
  // TODO
}

void ApplyMeasureNowDialog::closeEvent(QCloseEvent *e)
{
  e->accept(); // TODO
}

//***** SLOTS *****

void ApplyMeasureNowDialog::disableOkButton(bool disable)
{
  this->okButton()->setDisabled(disable);
}

} // openstudio
