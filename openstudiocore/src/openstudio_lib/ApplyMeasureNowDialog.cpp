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

#include <openstudio_lib/OSAppBase.hpp>
#include <openstudio_lib/OSDocument.hpp>
#include <openstudio_lib/OSItem.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>

#include <analysis/NullMeasure.hpp>

#include <analysisdriver/SimpleProject.hpp>

#include <utilities/core/RubyException.hpp>

#include <runmanager/lib/Job.hpp>
#include <runmanager/lib/RunManager.hpp>
#include <runmanager/lib/Workflow.hpp>
#include <runmanager/lib/RubyJobUtils.hpp>
#include <runmanager/lib/WorkItem.hpp>

#include <QBoxLayout>
#include <QCloseEvent>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QPointer>
#include <QPushButton>
#include <QScrollArea>
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

  layout = new QHBoxLayout();
  layout->addWidget(m_localLibraryController->localLibraryView);
  layout->addWidget(m_rightPaneStackedWidget);

  widget = new QWidget();
  widget->setLayout(layout);
  m_inputPageIdx = m_mainPaneStackedWidget->addWidget(widget);

  // TODO Should at least indicate if Radiance is being used?

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
    m_bclMeasure = bclMeasure;

    // Use this?
    //std::pair<bool,std::string> updateMeasure(analysisdriver::SimpleProject &t_project, const BCLMeasure &t_measure);

    // prep discrete variable
    std::string name = app->measureManager().suggestMeasureGroupName(*bclMeasure);
    analysis::MeasureGroup dv(name, analysis::MeasureVector());
    dv.setDisplayName(name);
   
    // measure
    analysis::RubyMeasure rubyMeasure(*bclMeasure);
    m_rubyMeasure = rubyMeasure;
    try{
      rubyMeasure.setArguments(app->measureManager().getArguments(*project, *bclMeasure));
    } catch (const RubyException & e) {
      QString errorMessage("Failed to compute arguments for measure: \n\n");
      errorMessage += QString::fromStdString(e.what());
      errorMessage.prepend(FAILED_ARG_TEXT);
      m_argumentsFailedTextEdit->setText(errorMessage);
      m_rightPaneStackedWidget->setCurrentIndex(m_argumentsFailedPageIdx);
      return;
    }

    // TODO Should this be "fixed" ?
    // null measure
    analysis::NullMeasure nullPert;
    dv.push(nullPert);

    // the new measure
    name = app->measureManager().suggestMeasureName(*bclMeasure, true); // TODO is this "fixed" ?
    rubyMeasure.setName(name);
    rubyMeasure.setDisplayName(name);
    rubyMeasure.setDescription(bclMeasure->description());
    dv.push(rubyMeasure);

    QSharedPointer<measuretab::MeasureItem> item = QSharedPointer<measuretab::MeasureItem>(new measuretab::MeasureItem(rubyMeasure, app));

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
}

void ApplyMeasureNowDialog::runMeasure()
{
  runmanager::ConfigOptions co(true);

  if (co.getTools().getAllByName("energyplus").tools().size() == 0)
  {
    QMessageBox::information(this,
        "Missing EnergyPlus",
        "EnergyPlus could not be located, simulation aborted.",
        QMessageBox::Ok);

    return;
  }
      
  if (co.getTools().getAllByName("ruby").tools().size() == 0)
  {
    QMessageBox::information(this,
        "Missing Ruby",
        "Ruby could not be located, simulation aborted.",
        QMessageBox::Ok);

    return;
  }

  openstudio::BaseApp * app = OSAppBase::instance();

  // clone model
  boost::optional<model::Model> model = app->currentModel();
  OS_ASSERT(model);
  model::Model modelClone = model->clone().cast<model::Model>();

  openstudio::path outDir = openstudio::tempDir() / openstudio::toPath("ApplyMeasureNow");

  openstudio::path modelPath = outDir / openstudio::toPath("modelClone.osm");

  // save cloned model to temp directory
  Workspace(modelClone).save(modelPath,true); 

  openstudio::runmanager::Workflow wf;
 
  runmanager::RubyJobBuilder rjb(*m_bclMeasure,m_rubyMeasure->arguments());
  rjb.addToWorkflow(wf);

  wf.add(co.getTools());

  openstudio::runmanager::Job j = wf.create(outDir, modelPath);

  openstudio::runmanager::RunManager rm;
  rm.enqueue(j, true);

  j.waitForFinished();
  rm.waitForFinished();

  j.errors();

  openstudio::runmanager::JobErrors jobErrors = j.errors();
}

void ApplyMeasureNowDialog::displayResults()
{
  m_mainPaneStackedWidget->setCurrentIndex(m_outputPageIdx);
  m_timer->stop();
  this->okButton()->setText(ACCEPT_CHANGES);
  this->okButton()->show();
  this->okButton()->setEnabled(true);
}

void ApplyMeasureNowDialog::closeEvent(QCloseEvent *e)
{
  e->accept(); // TODO
}

void ApplyMeasureNowDialog::disableOkButton(bool disable)
{
  this->okButton()->setDisabled(disable);
}

} // openstudio
