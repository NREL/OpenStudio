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

  bool isConnected = false;

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
  isConnected = connect(button, SIGNAL(clicked(bool)),
    this, SLOT(on_launchAWSConsole(bool)));
  OS_ASSERT(isConnected);
  hLayout->addWidget(button);
  hLayout->addStretch();

  button = new OrangeButton(this);
  button->setText("Stop Cloud");
  button->setCheckable(false);
  isConnected = connect(button, SIGNAL(clicked(bool)),
    this, SLOT(on_clearCloudSession(bool)));
  OS_ASSERT(isConnected);
  hLayout->addWidget(button);

  button = new OrangeButton(this);
  button->setText("Try Again Later");
  button->setCheckable(false);
  isConnected = connect(button, SIGNAL(clicked(bool)),
    this, SLOT(accept()));
  OS_ASSERT(isConnected);
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
