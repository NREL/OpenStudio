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

#include "CloudDialog.hpp"

#include <utilities/core/Assert.hpp>

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>

namespace openstudio {
  

CloudDialog::CloudDialog(QWidget* parent)
  : OSDialog(false, parent),
  m_iAcceptCheckBox(0),
  m_cloudResourceComboBox(0),
  m_stackedWidget(0),
  m_leftLayout(0)
{
  this->setWindowTitle("Cloud Settings");
  createWidgets();
}

CloudDialog::~CloudDialog()
{
}

void CloudDialog::createWidgets()
{
  QLabel * label = 0;
  QPushButton * pushButton = 0;
  bool isConnected = false;

  QHBoxLayout * mainLayout = new QHBoxLayout;
  mainLayout->setContentsMargins(QMargins(0,0,0,0));

  m_leftLayout = new QVBoxLayout();
  mainLayout->addLayout(m_leftLayout);
  
  QVBoxLayout * rightLayout = new QVBoxLayout();
  mainLayout->addLayout(rightLayout);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Cloud Resources");
  m_leftLayout->addWidget(label);

  m_cloudResourceComboBox = new QComboBox();
  //
  //Q_FOREACH(const QString & item, possibleItems){
  //  comboBox->addItem(item);
  //}
  //int index = comboBox->findText(savedItem);
  //// verify the saved item is in the set of possible items
  //OS_ASSERT(index != -1);
  //comboBox->setCurrentIndex(index);

  isConnected = connect(m_cloudResourceComboBox, SIGNAL(currentIndexChanged(const QString&)),
    this, SLOT(nistSectorChanged(const QString&)));
  OS_ASSERT(isConnected);

  m_leftLayout->addWidget(m_cloudResourceComboBox);

  label = new QLabel;
  label->setText("TBD NREL Legal statement");
  rightLayout->addWidget(label);

  m_iAcceptCheckBox = new QCheckBox("I Agree");

  isConnected = connect(m_iAcceptCheckBox, SIGNAL(clicked(bool)),
    this, SLOT(iAcceptClicked(bool)));
  OS_ASSERT(isConnected);

  rightLayout->addWidget(m_iAcceptCheckBox);

  this->setLayout(mainLayout);

  pushButton = this->cancelButton();
  pushButton->setText("Cancel");

  pushButton = this->okButton();
  pushButton->setText("Continue");

  #ifdef Q_WS_MAC
    setWindowFlags(Qt::FramelessWindowHint);
  #else
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
  #endif
}

void CloudDialog::setLoginWidget(QWidget * widget)
{
  OS_ASSERT(widget);
  OS_ASSERT(m_leftLayout);
  m_leftLayout->addWidget(widget);
}

//***** SLOTS *****

void CloudDialog::on_cancelButton(bool checked)
{
}

void CloudDialog::on_okButton(bool checked)
{
  if(m_stackedWidget->currentIndex() == 0){
     m_stackedWidget->setCurrentIndex(1);
  } else {
    this->close();
  }
}

void CloudDialog::iAcceptClicked(bool checked)
{
  QPushButton * pushButton = 0;
  pushButton = this->okButton();
  pushButton->setEnabled(checked);
  // persist this to qsettings and osp
}


//****************************************************************************************************


CloudProvider::CloudProvider(QWidget * parent)
  : QWidget(parent),
  m_waitCheckBox(0),
  m_waitineEdit(0),
  m_leftSettingLayout(0),
  m_rightSettingsLayout(0),
  m_loginWidget(0),
  m_settingsWidget(0)
{
  createWidgets();
}

CloudProvider::~CloudProvider()
{
}

void CloudProvider::createWidgets()
{
  QHBoxLayout * hLayout = 0;
  QLabel * label = 0;
  bool isConnected = false;

  m_loginWidget = new QWidget();

  m_loginLayout = new QVBoxLayout();
  m_loginLayout->setContentsMargins(QMargins(0,0,0,0)); 
  m_loginWidget->setLayout(m_loginLayout);
  
  m_settingsWidget = new QWidget();
  
  QHBoxLayout * mainLayout = new QHBoxLayout;
  mainLayout->setContentsMargins(QMargins(0,0,0,0)); 
  m_settingsWidget->setLayout(mainLayout);
  
  m_leftSettingLayout = new QVBoxLayout();
  mainLayout->addLayout(m_leftSettingLayout);
  
  m_rightSettingsLayout = new QVBoxLayout();
  mainLayout->addLayout(m_rightSettingsLayout);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Cloud Session Default Setting");
  m_rightSettingsLayout->addWidget(label);

  m_waitCheckBox = new QCheckBox("After simulations complete and selected detailed results are downloaded, wait the number of minutes below and automatically terminate cloud session.  Parametric Analysis Tool must be on to stop cloud session.");

  isConnected = connect(m_waitCheckBox, SIGNAL(clicked(bool)),
    this, SLOT(waitClicked(bool)));
  OS_ASSERT(isConnected);

  m_rightSettingsLayout->addWidget(m_waitCheckBox);

  hLayout = new QHBoxLayout;
  hLayout->setContentsMargins(QMargins(0,0,0,0));
  m_rightSettingsLayout->addLayout(hLayout);

  m_waitineEdit = new QLineEdit();
  hLayout->addWidget(m_waitineEdit);

  label = new QLabel;
  label->setText("minutes");
  hLayout->addWidget(label);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Stopping the cloud will terminate all instances.  Any detailed results not selected for download will lost.");

  hLayout->addWidget(label);
}

//***** SLOTS *****


//****************************************************************************************************


Vagrant::Vagrant(QWidget * parent)
  : CloudProvider(parent),
  m_runOnStartUpCheckBox(0),
  m_serverUsernameLineEdit(0),
  m_serverPasswordLineEdit(0),
  m_workerUsernameLineEdit(0),
  m_workerPasswordLineEdit(0),
  m_serverDirLineEdit(0),
  m_serverIpLineEdit(0),
  m_serverIp2LineEdit(0),
  m_workerDirLineEdit(0),
  m_workerIpLineEdit(0),
  m_workerIp2LineEdit(0)
{
}

Vagrant::~Vagrant()
{
}

void Vagrant::createLoginWidget()
{
  QLabel * label = 0;
  QVBoxLayout * vLayout = 0;
  
  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Server Username");
  vLayout->addWidget(label);

  m_serverUsernameLineEdit = new QLineEdit();
  vLayout->addWidget(m_serverUsernameLineEdit);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Server Password");
  vLayout->addWidget(label);

  m_serverPasswordLineEdit = new QLineEdit();
  vLayout->addWidget(m_serverPasswordLineEdit);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Worker Instance");
  vLayout->addWidget(label);

  m_workerUsernameLineEdit = new QLineEdit();
  vLayout->addWidget(m_workerUsernameLineEdit); 
  
  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Worker Password");
  vLayout->addWidget(label);
    
  m_workerPasswordLineEdit = new QLineEdit();
  vLayout->addWidget(m_workerPasswordLineEdit);

  QWidget * widget = new QWidget();
  widget->setLayout(vLayout);
  m_loginLayout->addWidget(widget);
}

void Vagrant::createSettingsWidget()
{
  QVBoxLayout * vLayout = 0;
  QHBoxLayout * hLayout = 0;
  QLabel * label = 0;
  QPushButton * pushButton = 0;
  QWidget * widget = 0;
  bool isConnected = false;

  vLayout = new QVBoxLayout;
  vLayout->setContentsMargins(QMargins(0,0,0,0));

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Server Dir");
  vLayout->addWidget(label);

  hLayout = new QHBoxLayout;
  hLayout->setContentsMargins(QMargins(0,0,0,0));
  vLayout->addLayout(hLayout);

  m_serverDirLineEdit = new QLineEdit();
  hLayout->addWidget(m_serverDirLineEdit);
  
  pushButton = new QPushButton();
  pushButton->setCheckable(false);
  hLayout->addWidget(pushButton);
  isConnected = connect(pushButton, SIGNAL(clicked(bool)),
    this, SLOT(serverDirButtonClicked(bool)));
  OS_ASSERT(isConnected);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Server IP");
  vLayout->addWidget(label);

  hLayout = new QHBoxLayout;
  hLayout->setContentsMargins(QMargins(0,0,0,0));
  vLayout->addLayout(hLayout);

  m_serverIpLineEdit = new QLineEdit();
  hLayout->addWidget(m_serverIpLineEdit);

  m_serverIp2LineEdit = new QLineEdit();
  hLayout->addWidget(m_serverIp2LineEdit);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Worker Dir");
  vLayout->addWidget(label);

  hLayout = new QHBoxLayout;
  hLayout->setContentsMargins(QMargins(0,0,0,0));
  vLayout->addLayout(hLayout);

  m_workerDirLineEdit = new QLineEdit();
  hLayout->addWidget(m_workerDirLineEdit);

  pushButton = new QPushButton();
  pushButton->setCheckable(false);
  hLayout->addWidget(pushButton);
  isConnected = connect(pushButton, SIGNAL(clicked(bool)),
    this, SLOT(workerDirButtonClicked(bool)));
  OS_ASSERT(isConnected);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Worker IP");
  vLayout->addWidget(label);

  hLayout = new QHBoxLayout;
  hLayout->setContentsMargins(QMargins(0,0,0,0));
  vLayout->addLayout(hLayout);

  m_workerIpLineEdit = new QLineEdit();
  hLayout->addWidget(m_workerIpLineEdit);

  m_workerIp2LineEdit = new QLineEdit();
  hLayout->addWidget(m_workerIp2LineEdit);

  widget = new QWidget();
  widget->setLayout(vLayout);
  m_leftSettingLayout->addWidget(widget);

  vLayout = new QVBoxLayout;
  vLayout->setContentsMargins(QMargins(0,0,0,0));

  m_runOnStartUpCheckBox = new QCheckBox("Run Vagrant Up on Start Cloud");
  vLayout->addWidget(m_runOnStartUpCheckBox);
  
  widget = new QWidget();
  widget->setLayout(vLayout);
  m_rightSettingsLayout->addWidget(widget);
}

void  Vagrant::loadData()
{
  bool isChecked = true;
  m_runOnStartUpCheckBox->setChecked(isChecked);

  QString text;
  m_serverUsernameLineEdit->setText(text);
  m_serverPasswordLineEdit->setText(text);
  m_workerUsernameLineEdit->setText(text);
  m_workerPasswordLineEdit->setText(text);
  m_serverDirLineEdit->setText(text);
  m_serverIpLineEdit->setText(text);
  m_serverIp2LineEdit->setText(text);
  m_workerDirLineEdit->setText(text);
  m_workerIpLineEdit->setText(text);
  m_workerIp2LineEdit->setText(text);
}

void  Vagrant::saveData()
{
  bool isChecked = true;
  isChecked = m_runOnStartUpCheckBox->isChecked();

  QString text;
  text = m_serverUsernameLineEdit->text();
  text = m_serverPasswordLineEdit->text();
  text = m_workerUsernameLineEdit->text();
  text = m_workerPasswordLineEdit->text();
  text = m_serverDirLineEdit->text();
  text = m_serverIpLineEdit->text();
  text = m_serverIp2LineEdit->text();
  text = m_workerDirLineEdit->text();
  text = m_workerIpLineEdit->text();
  text = m_workerIp2LineEdit->text();
}


//***** SLOTS *****

void Vagrant::serverDirButtonClicked(bool checked)
{
}

void Vagrant::workerDirButtonClicked(bool checked)
{
}


//****************************************************************************************************


Amazon::Amazon(QWidget * parent)
  : CloudProvider(parent),
  m_regionComboBox(0),
  m_serverInstanceTypeComboBox(0),
  m_workerInstanceTypeComboBox(0),
  m_accessKeyLineEdit(0),
  m_secretKeyLineEdit(0),
  m_selectPrivateKeyLineEdit(0),
  m_numberOfWorkerInstancesLineEdit(0),
  m_elasticStorageCapacityLineEdit(0)
{
}

Amazon::~Amazon()
{
}

void Amazon::createLoginWidget()
{
  QVBoxLayout * vLayout = 0;
  QLabel * label = 0;

  vLayout = new QVBoxLayout;
  vLayout->setContentsMargins(QMargins(0,0,0,0));
    
  label = new QLabel;
  label->setText("To create an account go to http://aws.amazon.com");
  vLayout->addWidget(label);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Access Key");
  vLayout->addWidget(label);

  m_accessKeyLineEdit = new QLineEdit();
  vLayout->addWidget(m_accessKeyLineEdit); 
  
  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Secret Key");
  vLayout->addWidget(label);

  m_secretKeyLineEdit = new QLineEdit();
  vLayout->addWidget(m_secretKeyLineEdit); 
  
  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Select Private Key File");
  vLayout->addWidget(label);
    
  m_selectPrivateKeyLineEdit = new QLineEdit();
  vLayout->addWidget(m_selectPrivateKeyLineEdit);

  QWidget * widget = new QWidget();
  widget->setLayout(vLayout);
  m_loginLayout->addWidget(widget);
}

void Amazon::createSettingsWidget()
{
  QVBoxLayout * vLayout = 0;
  QLabel * label = 0;
  QWidget * widget = 0;

  vLayout = new QVBoxLayout;
  vLayout->setContentsMargins(QMargins(0,0,0,0));

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Region");
  vLayout->addWidget(label);

  m_regionComboBox = new QComboBox();
  vLayout->addWidget(m_regionComboBox);

  label = new QLabel;
  label->setText("This is the only region with CloudWatch enabled.  CloudWatch allows you to monitor your cost and performance.  The AWS Management Console provides a dashboard, alarms, and graphs.");
  vLayout->addWidget(label);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Server Instance Type");
  vLayout->addWidget(label);

  m_serverInstanceTypeComboBox = new QComboBox();
  vLayout->addWidget(m_serverInstanceTypeComboBox);
  
  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Worker Instance Type");
  vLayout->addWidget(label);

  m_workerInstanceTypeComboBox = new QComboBox();
  vLayout->addWidget(m_workerInstanceTypeComboBox);
  
  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Number of Worker Instances");
  vLayout->addWidget(label);

  m_numberOfWorkerInstancesLineEdit = new QLineEdit();
  vLayout->addWidget(m_numberOfWorkerInstancesLineEdit);
  
  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Elastic Storage Capacity (GB)");
  vLayout->addWidget(label);
  
  m_elasticStorageCapacityLineEdit = new QLineEdit();
  vLayout->addWidget(m_elasticStorageCapacityLineEdit);

  widget = new QWidget();
  widget->setLayout(vLayout);
  m_leftSettingLayout->addWidget(widget);

  vLayout = new QVBoxLayout;
  vLayout->setContentsMargins(QMargins(0,0,0,0));

  label = new QLabel;
  label->setText("Monitor your cloud use with CloudWatch and AWS Management Console.");
  vLayout->addWidget(label);
  
  label = new QLabel;
  label->setText("Review pricing for cloud service at:\nhttp://aws.amazon.com/ec2/pricing/");
  vLayout->addWidget(label);
  
  widget = new QWidget();
  widget->setLayout(vLayout);
  m_rightSettingsLayout->addWidget(widget);
}

void  Amazon::loadData()
{
  QString text;

  //m_regionComboBox;
  //m_serverInstanceTypeComboBox;
  //m_workerInstanceTypeComboBox;
  //
  //Q_FOREACH(const QString & item, possibleItems){
  //  comboBox->addItem(item);
  //}
  //int index = comboBox->findText(savedItem);
  //// verify the saved item is in the set of possible items
  //OS_ASSERT(index != -1);
  //comboBox->setCurrentIndex(index);

  m_accessKeyLineEdit->setText(text);
  m_secretKeyLineEdit->setText(text);
  m_selectPrivateKeyLineEdit->setText(text);
  m_numberOfWorkerInstancesLineEdit->setText(text);
  m_elasticStorageCapacityLineEdit->setText(text);
}

void  Amazon::saveData()
{
  QString text;

  text = m_regionComboBox->currentText();
  text = m_serverInstanceTypeComboBox->currentText();
  text = m_workerInstanceTypeComboBox->currentText();

  text = m_accessKeyLineEdit->text();
  text = m_secretKeyLineEdit->text();
  text = m_selectPrivateKeyLineEdit->text();
  text = m_numberOfWorkerInstancesLineEdit->text();
  text = m_elasticStorageCapacityLineEdit->text();
}

//***** SLOTS *****

} // openstudio
