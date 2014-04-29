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
#include <shared_gui_components/OSViewSwitcher.hpp>

#include <openstudio_lib/MainRightColumnController.hpp>
#include <openstudio_lib/OSAppBase.hpp>
#include <openstudio_lib/OSDocument.hpp>
#include <openstudio_lib/OSItem.hpp>

#include <QBoxLayout>
#include <QCloseEvent>
#include <QLabel>
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
  m_argumentsOkPageIdx = m_rightPaneStackedWidget->addWidget(m_editController->editView->view());

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
  //this->okButton()->setEnabled(false); TODO use

  // OS SETTINGS

  #ifdef Q_OS_MAC
    setWindowFlags(Qt::FramelessWindowHint);
  #elif defined(Q_OS_WIN32)
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
  #endif

}

//***** SLOTS *****
  
  //m_mainPaneStackedWidget->setCurrentIndex(m_outputPageIdx);
  //m_timer->stop(); // TODO
  //this->okButton()->setText(ACCEPT_CHANGES);
  //this->okButton()->show();

  //this->okButton()->setEnabled(true); // TODO

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
    //this->okButton()->hide(); TODO use
  } else if(m_mainPaneStackedWidget->currentIndex() == m_runningPageIdx) {
    // N/A
    m_mainPaneStackedWidget->setCurrentIndex(m_outputPageIdx); // TODO remove
  } else if(m_mainPaneStackedWidget->currentIndex() == m_outputPageIdx) {
    // N/A
    m_mainPaneStackedWidget->setCurrentIndex(m_inputPageIdx); // TODO remove
  }
}

void ApplyMeasureNowDialog::closeEvent(QCloseEvent *e)
{
  e->accept(); // TODO
}

} // openstudio
