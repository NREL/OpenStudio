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

#include <shared_gui_components/WaitDialog.hpp>

#include <shared_gui_components/BusyWidget.hpp>

#include <QBoxLayout>
#include <QCloseEvent>
#include <QLabel>
#include <QPointer>
#include <QPushButton>
#include <QTimer>

namespace openstudio {

WaitDialog::WaitDialog(const QString & windowTitle,
  const QString & windowMessage,
  QWidget* parent)
  : OSDialog(false, parent),
  m_windowMessage(windowMessage)
{
  setWindowTitle(windowTitle);
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  createWidgets();
}

WaitDialog::~WaitDialog()
{
}

QSize WaitDialog::sizeHint() const
{
  return QSize(770,540);
}

void WaitDialog::createWidgets()
{
  QWidget * widget = 0;
  QBoxLayout * layout = 0;
  QLabel * label = 0;
  bool isConnected = false;

  upperLayout()->addStretch();

  label = new QLabel(m_windowMessage);
  label->setObjectName("H2");
  upperLayout()->addWidget(label,0,Qt::AlignCenter);

  //BusyWidget * busyWidget = new BusyWidget();
  //upperLayout()->addWidget(busyWidget,0,Qt::AlignCenter);

  //QTimer * timer = new QTimer(this);
  //connect(timer,SIGNAL(timeout()),busyWidget,SLOT(rotate()));
  //timer->start(50);
  
  label = new QLabel("This may take a minute...");
  label->setObjectName("H2");
  upperLayout()->addWidget(label,0,Qt::AlignCenter);

  upperLayout()->addStretch();

  // BUTTONS

  this->okButton()->hide();
  this->m_cancelButton->hide();

  // OS SETTINGS

  #ifdef Q_OS_MAC
    setWindowFlags(Qt::FramelessWindowHint);
  #elif defined(Q_OS_WIN32)
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
  #endif

}

//***** SLOTS *****

void WaitDialog::on_cancelButton(bool checked)
{  
  OSDialog::on_cancelButton(checked);
}

void WaitDialog::closeEvent(QCloseEvent *e)
{
  // DLM: do not accept the event, we don't allow this dialog to close
  // would be better to set window flags to not have the close button
  e->ignore();
}

} // openstudio
