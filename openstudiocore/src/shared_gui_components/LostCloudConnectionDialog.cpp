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

#include "LostCloudConnectionDialog.hpp"
#include "../shared_gui_components/Buttons.hpp"

#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QDesktopServices>
#include <QLabel>
#include <QPushButton>
#include <QUrl>

#define RED '#FF0000'
#define GREEN '#00FF00'
#define BLACK '#000000'
#define LABEL_WIDTH 500
#define INDENT 20

namespace openstudio {

LostCloudConnectionDialog::LostCloudConnectionDialog(bool internetAvailable,
  bool authenticated,
  bool cloudRunning,
  QWidget* parent)
  : OSDialog(false, parent),
    m_clearCloudSession(false)
{
  this->setModal(true);
  this->setWindowTitle("Lost Cloud Connection");
  setFixedWidth(650);
  createWidgets(internetAvailable,authenticated,cloudRunning);
}

LostCloudConnectionDialog::~LostCloudConnectionDialog()
{
}

void LostCloudConnectionDialog::createWidgets(bool internetAvailable,
  bool authenticated,
  bool cloudRunning)
{
  //// OS SETTINGS

  #ifdef Q_OS_MAC
    setWindowFlags(Qt::FramelessWindowHint);
  #else
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
  #endif

  ///// BUTTONS

  this->cancelButton()->hide();
  this->okButton()->hide();
  this->backButton()->hide();

  QLabel * label = nullptr;
  
  QVBoxLayout * mainLayout = this->upperLayout();
  mainLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

  label = new QLabel;
  label->setWordWrap(true);
  label->setText("The cloud connection was interrupted.  The cloud may still be running and accruing charges.");
  mainLayout->addWidget(label);

  label = new QLabel;
  label->setText("<b>" + (tr("Requirements for cloud:")) + "</b>");
  mainLayout->addWidget(label);

  auto vLayout = new QVBoxLayout;
  vLayout->setContentsMargins(QMargins(0,0,0,0));
  vLayout->setSpacing(5);
  vLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  mainLayout->addLayout(vLayout);

  // 1 Determine if there is an internet connection
  label = new QLabel;
  vLayout->addWidget(label);

  if(internetAvailable){
    label->setText("<FONT COLOR = GREEN>1. <FONT COLOR = BLACK>" + tr("Internet Connection: ") + "<b> <FONT COLOR = GREEN>" + tr("yes") + "</b>");
  } else {
    label->setText("<FONT COLOR = RED>1. <FONT COLOR = BLACK>" + tr("Internet Connection: ") + "<b> <FONT COLOR = RED>" + tr("no") + "</b>");
  }

  // 2 Determine if the cloud login is accepted
  label = new QLabel;
  vLayout->addWidget(label);
  if(authenticated){
    label->setText("<FONT COLOR = GREEN>2. <FONT COLOR = BLACK>" + tr("Cloud Log-in: ") + "<b> <FONT COLOR = GREEN>" + tr("accepted") + "</b>");
  } else {
    label->setText("<FONT COLOR = RED>2. <FONT COLOR = BLACK>" + tr("Cloud Log-in: ") + "<b> <FONT COLOR = RED>" + tr("denied") + "</b>");
  }

  // 3 Determine if there is a cloud connection
  label = new QLabel;
  vLayout->addWidget(label);
  if(cloudRunning){
    label->setText("<FONT COLOR = GREEN>3. <FONT COLOR = BLACK>" + tr("Cloud Connection: ") + "<b> <FONT COLOR = GREEN>" + tr("reconnected") + "</b>");

  } else {
    label->setWordWrap(true);
    label->setText("<FONT COLOR = RED>3. <FONT COLOR = BLACK>" + tr("Cloud Connection: ") + "<b> <FONT COLOR = RED>" + tr("unable to reconnect. ") + "</b>" + "<FONT COLOR = BLACK>" + tr("Remember that cloud charges may currently be accruing."));
    createCloudConnectionWidgets(vLayout);
  }
}

void LostCloudConnectionDialog::createCloudConnectionWidgets(QVBoxLayout * vLayout)
{
  QHBoxLayout * hLayout = nullptr;

  QSpacerItem * horizontalSpacer = nullptr;

  QPushButton * button = nullptr;

  QLabel * label = nullptr;
        
  label = new QLabel;
  label->setText("<b>" + tr("Options to correct the problem:") + "</b>");
  vLayout->addWidget(label);

  hLayout = new QHBoxLayout;
  hLayout->setContentsMargins(QMargins(0,0,0,0));
  hLayout->setSpacing(5);
  vLayout->addLayout(hLayout);

  horizontalSpacer = new QSpacerItem(INDENT, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
  hLayout->addSpacerItem(horizontalSpacer); 

  label = new QLabel;
  label->setWordWrap(true);
  label->setText("<b>" + tr("Try Again Later. ") + "</b>" + tr("Verify your computer's internet connection then click \"Lost Cloud Connection\" to recover the lost cloud session."));
  hLayout->addWidget(label);

  label = new QLabel;
  label->setText("<b>" + tr("Or") + "</b>");
  vLayout->addWidget(label);

  hLayout = new QHBoxLayout;
  hLayout->setContentsMargins(QMargins(0,0,0,0));
  hLayout->setSpacing(5);
  vLayout->addLayout(hLayout);

  horizontalSpacer = new QSpacerItem(INDENT, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
  hLayout->addSpacerItem(horizontalSpacer); 

  label = new QLabel;
  label->setWordWrap(true);
  label->setText("<b>" + tr("Stop Cloud. ") + "</b>" + tr("Disconnect from cloud.  This option will make the failed cloud session unavailable to Pat.  Any data that has not been downloaded to Pat will be lost.  Use the AWS Console to verify that the Amazon service have been completely shutdown."));
  hLayout->addWidget(label);

  label = new QLabel;
  label->setText("<b>" + tr("Or") + "</b>");
  vLayout->addWidget(label);

  hLayout = new QHBoxLayout;
  hLayout->setContentsMargins(QMargins(0,0,0,0));
  hLayout->setSpacing(5);
  vLayout->addLayout(hLayout);

  horizontalSpacer = new QSpacerItem(INDENT, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
  hLayout->addSpacerItem(horizontalSpacer); 

  label = new QLabel;
  label->setWordWrap(true);
  label->setText("<b>" + tr("Launch AWS Console. ") + "</b>" + tr("Use the AWS Console to diagnose Amazon services.  You may still attempt to recover the lost cloud session."));
  hLayout->addWidget(label,0);

  vLayout->addStretch();

  hLayout = new QHBoxLayout;
  hLayout->setContentsMargins(QMargins(0,0,0,0));
  hLayout->setSpacing(5);
  vLayout->addLayout(hLayout);

  //horizontalSpacer = new QSpacerItem(INDENT, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
  //hLayout->addSpacerItem(horizontalSpacer); 

  button = new OrangeButton(this);
  button->setText("Launch AWS Console");
  button->setCheckable(false);
  connect(button, &OrangeButton::clicked, this, &LostCloudConnectionDialog::on_launchAWSConsole);
  hLayout->addWidget(button);
  hLayout->addStretch();

  button = new OrangeButton(this);
  button->setText("Stop Cloud");
  button->setCheckable(false);
  connect(button, &OrangeButton::clicked, this, &LostCloudConnectionDialog::on_clearCloudSession);
  hLayout->addWidget(button);

  button = new OrangeButton(this);
  button->setText("Try Again Later");
  button->setCheckable(false);
  connect(button, &OrangeButton::clicked, this, &LostCloudConnectionDialog::accept);
  hLayout->addWidget(button);
}

//***** SLOTS *****

//void LostCloudConnectionDialog::on_okButton(bool checked)
//{
// TODO
//}

void LostCloudConnectionDialog::on_launchAWSConsole(bool checked)
{
  QDesktopServices::openUrl(QUrl("http://aws.amazon.com/console"));
}

void LostCloudConnectionDialog::on_clearCloudSession(bool checked)
{
  m_clearCloudSession = true;

  accept();
}

} // openstudio
