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

#include "WaitDialog.hpp"

#include "BusyWidget.hpp"

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
  setWindowModality(Qt::ApplicationModal);
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
  QLabel * label = nullptr;

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
  #elif defined(Q_OS_WIN)
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
