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

#include "LostCloudConnectionDialog.hpp"

#include <pat_app/CloudMonitor.hpp>

#include <utilities/core/Assert.hpp>

#include <QBoxLayout>
#include <QDesktopServices>
#include <QLabel>
#include <QPushButton>

#define RED '#FF0000'
#define GREEN '#00FF00'
#define BLACK '#000000'
#define LABEL_WIDTH 500
#define INDENT 20

namespace openstudio {

LostCloudConnectionDialog::LostCloudConnectionDialog(QWidget* parent)
  : OSDialog(false, parent)
{
  this->setWindowTitle("Lost Cloud Connection");
  this->setSizeHint(QSize(500,300));
  createWidgets();
}

LostCloudConnectionDialog::~LostCloudConnectionDialog()
{
}

void LostCloudConnectionDialog::createWidgets()
{
  pat::CloudMonitor cloudMonitor;

  QLabel * label = 0;

  QPushButton * button = 0;

  bool isConnected = false;

  bool isOK = true;

  QHBoxLayout * hLayout = 0;

  QSpacerItem * horizontalSpacer = 0;
  
  QVBoxLayout * mainLayout = new QVBoxLayout;
  mainLayout->setContentsMargins(QMargins(0,0,0,0));
  mainLayout->setSpacing(5);
  this->upperLayout()->addLayout(mainLayout);

  label = new QLabel;
  label->setWordWrap(true);
  label->setFixedSize(QSize(LABEL_WIDTH,27));
  label->setText("The project was running in the cloud when the connection was lost.  The cloud may still be running and accruing charges.");
  mainLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  label = new QLabel;
  label->setText("<b>" + (tr("Steps to reconnect to the cloud:")) + "</b>");
  mainLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  QVBoxLayout * vLayout = new QVBoxLayout;
  vLayout->setContentsMargins(QMargins(INDENT,0,0,0));
  vLayout->setSpacing(5);
  mainLayout->addLayout(vLayout);

  // 1
  label = new QLabel;
  vLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);
  cloudMonitor; // TODO make a call and get a bool response
  if(isOK){
    label->setText("<FONT COLOR = GREEN>1. <FONT COLOR = BLACK>" + tr("Internet Connection: ") + "<b> <FONT COLOR = GREEN>" + tr("yes") + "</b>");
  } else {
    label->setText("<FONT COLOR = RED>1. <FONT COLOR = BLACK>" + tr("Internet Connection: ") + "<b> <FONT COLOR = RED>" + tr("no") + "</b>");
    mainLayout->addStretch();
    return;
  }

  // 2
  label = new QLabel;
  vLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);
  cloudMonitor; // TODO make a call and get a bool response
  if(isOK){
    label->setText("<FONT COLOR = GREEN>2. <FONT COLOR = BLACK>" + tr("Cloud Log-in: ") + "<b> <FONT COLOR = GREEN>" + tr("accepted") + "</b>");
  } else {
    label->setText("<FONT COLOR = RED>2. <FONT COLOR = BLACK>" + tr("Cloud Log-in: ") + "<b> <FONT COLOR = RED>" + tr("denied") + "</b>");
    mainLayout->addStretch();
    return;
  }

  // 3
  label = new QLabel;
  vLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);
  cloudMonitor; // TODO make a call and get a bool response
  if(isOK){
    label->setText("<FONT COLOR = GREEN>3. <FONT COLOR = BLACK>" + tr("Cloud Connection: ") + "<b> <FONT COLOR = GREEN>" + tr("reconnected") + "</b>");
  } else {
    label->setText("<FONT COLOR = RED>3. <FONT COLOR = BLACK>" + tr("Cloud Connection: ") + "<b> <FONT COLOR = RED>" + tr("unable to reconnect. ") + "</b>" + "<FONT COLOR = BLACK>" + tr("Remember that cloud charges may currently be accruing."));
    mainLayout->addStretch();
    return;
  }

  //if(isOK){
  //  mainLayout->addStretch();
  //} else {

    label = new QLabel;
    label->setText("<b>" + tr("Options:") + "</b>");
    vLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

    hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(QMargins(0,0,0,0));
    hLayout->setSpacing(5);
    vLayout->addLayout(hLayout);

    horizontalSpacer = new QSpacerItem(INDENT, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
    hLayout->addSpacerItem(horizontalSpacer); 

    label = new QLabel;
    label->setWordWrap(true);
    label->setFixedSize(QSize(LABEL_WIDTH - INDENT - INDENT,27));

    label->setText("<b>" + tr("Try again now. ") + "</b>" + tr("Close this dialog and click the Run Simulations tab\'s button labeled \"Try to Reconnect Cloud\"."));
    hLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

    label = new QLabel;
    label->setText("<b>" + tr("Or") + "</b>");
    vLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

    hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(QMargins(0,0,0,0));
    hLayout->setSpacing(5);
    vLayout->addLayout(hLayout);

    horizontalSpacer = new QSpacerItem(INDENT, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
    hLayout->addSpacerItem(horizontalSpacer); 

    label = new QLabel;
    label->setWordWrap(true);
    label->setFixedSize(QSize(LABEL_WIDTH - INDENT - INDENT,27));

    label->setText("<b>" + tr("Quit the ParametricAnalysisTool and try again later. ") + "</b>" + tr("Close this dialog and quit the application."));
    hLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);
           
    label = new QLabel;
    label->setText("<b>" + tr("Or") + "</b>");
    vLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);
    
    hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(QMargins(0,0,0,0));
    hLayout->setSpacing(5);
    vLayout->addLayout(hLayout);

    horizontalSpacer = new QSpacerItem(INDENT, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
    hLayout->addSpacerItem(horizontalSpacer); 

    label = new QLabel;
    label->setWordWrap(true);
    label->setFixedSize(QSize(LABEL_WIDTH - INDENT - INDENT,40));
    label->setText("<b>" + tr("Stop the cloud and clear the session. ") + "</b>" + tr("Use the AWS Console to cancel the cloud session and stop charges, and clear this session in the ParametricAnalysisTool.  This will stop the cloud charges, but will also result in the loss of all session results."));
    hLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

    hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(QMargins(0,0,0,0));
    hLayout->setSpacing(5);
    vLayout->addLayout(hLayout);

    horizontalSpacer = new QSpacerItem(INDENT, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
    hLayout->addSpacerItem(horizontalSpacer); 

    button = new QPushButton("Launch AWS Console",this);
    //button->setIcon(QIcon(":/shared_gui_components/images/???.png"));
    //button->setFlat(true);
    button->setFixedSize(120,20);
    button->setCheckable(false);
    isConnected = connect(button, SIGNAL(clicked(bool)),
      this, SLOT(on_launchAWSConsole(bool)));
    OS_ASSERT(isConnected);
    hLayout->addWidget(button,0,Qt::AlignTop | Qt::AlignLeft);

    button = new QPushButton("Clear Cloud Session",this);
    //button->setIcon(QIcon(":/shared_gui_components/images/???.png"));
    //button->setFlat(true);
    button->setFixedSize(120,20);
    button->setCheckable(false);
    isConnected = connect(button, SIGNAL(clicked(bool)),
      this, SLOT(on_clearCloudSession(bool)));
    OS_ASSERT(isConnected);
    hLayout->addWidget(button,0,Qt::AlignTop | Qt::AlignLeft);

    hLayout->addStretch();

    mainLayout->addStretch();

 // }

  //// OS SETTINGS

  #ifdef Q_WS_MAC
    setWindowFlags(Qt::FramelessWindowHint);
  #else
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
  #endif

}

//***** SLOTS *****

//void LostCloudConnectionDialog::on_cancelButton(bool checked)
//{
//  TODO
//}

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
  // TODO cleasr session
}

} // openstudio
