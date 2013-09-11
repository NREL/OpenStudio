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

#include <utilities/cloud/AWSProvider.hpp>
#include <utilities/cloud/AWSProvider_Impl.hpp>
#include <utilities/cloud/CloudProvider.hpp>
#include <utilities/cloud/CloudProvider_Impl.hpp>
#include <utilities/cloud/VagrantProvider.hpp>
#include <utilities/cloud/VagrantProvider_Impl.hpp>
#include <utilities/core/Assert.hpp>

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedWidget>

#define NO_PROVIDER ""
#define VAGRANT_PROVIDER "VagrantProviderWidget"
#define AMAZON_PROVIDER "AmazonProviderWidget"
#define EDIT_WIDTH 150
#define ADDRESS_WIDTH 110
#define PORT_WIDTH 30
#define TEXT_WIDTH 300

namespace openstudio {

CloudDialog::CloudDialog(QWidget* parent)
  : OSDialog(false, parent),
  m_iAcceptCheckBox(0),
  m_cloudResourceComboBox(0),
  m_pageStackedWidget(0),
  m_leftLoginLayout(0),
  m_rightLoginLayout(0),
  m_mainSettingsLayout(0),
  m_blankPageIdx(-1),
  m_loginPageIdx(-1),
  m_settingsPageIdx(-1),
  m_amazonProviderWidget(0),
  m_blankProviderWidget(0),
  m_vagrantProviderWidget(0),
  m_legalAgreement(0)
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
  bool isConnected = false;

  m_amazonProviderWidget = new AmazonProviderWidget(this);
  m_blankProviderWidget = new BlankProviderWidget(this);
  m_vagrantProviderWidget = new VagrantProviderWidget(this);

  // BLANK PAGE
  QWidget * blankPageWidget = new QWidget();

  // LOGIN PAGE

  QHBoxLayout * mainLoginLayout = new QHBoxLayout;
  mainLoginLayout->setContentsMargins(QMargins(0,0,0,0));
  mainLoginLayout->setSpacing(5);

  QWidget * loginPageWidget = new QWidget;
  loginPageWidget->setLayout(mainLoginLayout);

  // LEFT LOGIN LAYOUT

  m_leftLoginLayout = new QVBoxLayout();
  mainLoginLayout->addLayout(m_leftLoginLayout);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Cloud Resources");
  m_leftLoginLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_cloudResourceComboBox = new QComboBox();
  m_leftLoginLayout->addWidget(m_cloudResourceComboBox,0,Qt::AlignTop | Qt::AlignLeft);
 
  m_cloudResourceComboBox->addItem(NO_PROVIDER);
  m_cloudResourceComboBox->addItem(VAGRANT_PROVIDER);
  m_cloudResourceComboBox->addItem(AMAZON_PROVIDER);

  isConnected = connect(m_cloudResourceComboBox, SIGNAL(currentIndexChanged(const QString &)),
    this, SLOT(cloudResourceChanged(const QString &)));
  OS_ASSERT(isConnected); 

  // LOGIN STACKED WIDGET

  m_loginStackedWidget = new  QStackedWidget();
  m_leftLoginLayout->addWidget(m_loginStackedWidget);

  m_loginStackedWidget->addWidget(m_blankProviderWidget->m_loginWidget);
  m_loginStackedWidget->addWidget(m_vagrantProviderWidget->m_loginWidget);
  m_loginStackedWidget->addWidget(m_amazonProviderWidget->m_loginWidget);

  m_loginStackedWidget->setCurrentIndex(m_blankProviderIdx);

  // RIGHT LOGIN LAYOUT
  
  m_rightLoginLayout = new QVBoxLayout();
  mainLoginLayout->addLayout(m_rightLoginLayout);

  AWSProvider awsProvider;

  m_legalAgreement = new QLabel;
  m_legalAgreement->hide();
  m_legalAgreement->setText("TBD NREL Legal statement");
  // TODO
  //boost::optional<CloudProviderWidget *> cloudProviderWidget = this->getCurrentCloudProviderWidget();
  //if(cloudProviderWidget.is_initialized()){
  //  cloudProviderWidget.get()->saveData();
  //  m_legalAgreement->setText(awsProvider.userAgreementText().c_str()); 
  //}

  m_rightLoginLayout->addWidget(m_legalAgreement,0,Qt::AlignTop | Qt::AlignLeft);

  m_iAcceptCheckBox = new QCheckBox("I Agree");
  m_iAcceptCheckBox->hide();
  m_rightLoginLayout->addWidget(m_iAcceptCheckBox,0,Qt::AlignTop | Qt::AlignLeft);

  isConnected = connect(m_iAcceptCheckBox, SIGNAL(clicked(bool)),
    this, SLOT(iAcceptClicked(bool)));
  OS_ASSERT(isConnected);

  m_rightLoginLayout->addStretch();
    
  // SETTINGS PAGE

  m_mainSettingsLayout = new QVBoxLayout;
  m_mainSettingsLayout->setContentsMargins(QMargins(0,0,0,0));
  m_mainSettingsLayout->setSpacing(5);

  QWidget * settingsPageWidget = new QWidget;
  settingsPageWidget->setLayout(m_mainSettingsLayout);

  // SETTINGS STACKED WIDGET

  m_settingsStackedWidget = new  QStackedWidget();
  m_mainSettingsLayout->addWidget(m_settingsStackedWidget);

  QWidget * widget = new QWidget();

  QHBoxLayout * hLayout = new QHBoxLayout;
  hLayout->setContentsMargins(QMargins(0,0,0,0));
  hLayout->setSpacing(5);
  widget->setLayout(hLayout);

  hLayout->addWidget(m_blankProviderWidget->m_leftSettingsWidget);
  hLayout->addWidget(m_blankProviderWidget->m_rightSettingsWidget);

  m_settingsStackedWidget->addWidget(widget);

  widget = new QWidget();

  hLayout = new QHBoxLayout;
  hLayout->setContentsMargins(QMargins(0,0,0,0));
  hLayout->setSpacing(5);
  widget->setLayout(hLayout);

  hLayout->addWidget(m_vagrantProviderWidget->m_leftSettingsWidget);
  hLayout->addWidget(m_vagrantProviderWidget->m_rightSettingsWidget);

  m_settingsStackedWidget->addWidget(widget);

  widget = new QWidget();

  hLayout = new QHBoxLayout;
  hLayout->setContentsMargins(QMargins(0,0,0,0));
  hLayout->setSpacing(5);
  widget->setLayout(hLayout);

  hLayout->addWidget(m_amazonProviderWidget->m_leftSettingsWidget);
  hLayout->addWidget(m_amazonProviderWidget->m_rightSettingsWidget);

  m_settingsStackedWidget->addWidget(widget);

  m_settingsStackedWidget->setCurrentIndex(m_blankProviderIdx);

  // PAGE STACKED WIDGET

  m_pageStackedWidget = new  QStackedWidget();
  upperLayout()->addWidget(m_pageStackedWidget);

  m_blankPageIdx = m_pageStackedWidget->addWidget(blankPageWidget);
  m_loginPageIdx = m_pageStackedWidget->addWidget(loginPageWidget);
  m_settingsPageIdx = m_pageStackedWidget->addWidget(settingsPageWidget);

  m_pageStackedWidget->setCurrentIndex(m_loginPageIdx);

  // BUTTONS

  this->okButton()->setText("Continue");
  this->okButton()->setEnabled(false);

  // OS SETTINGS

  #ifdef Q_WS_MAC
    setWindowFlags(Qt::FramelessWindowHint);
  #else
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
  #endif

}

boost::optional<CloudProviderWidget *> CloudDialog::getCurrentCloudProviderWidget()
{
  boost::optional<CloudProviderWidget *> cloudProviderWidget;

  if(m_cloudResourceComboBox->currentIndex() == m_vagrantProviderIdx){
    cloudProviderWidget = m_vagrantProviderWidget;
  } else if(m_cloudResourceComboBox->currentIndex() == m_amazonProviderIdx){
    cloudProviderWidget = m_amazonProviderWidget;
  }

  return cloudProviderWidget;
}

void  CloudDialog::loadData()
{
  bool checked = false;
  
  m_iAcceptCheckBox->setChecked(checked);

  CloudProviderWidget * cloudProviderWidget = qobject_cast<CloudProviderWidget *>(this->m_pageStackedWidget->currentWidget());
  cloudProviderWidget->loadData();
}

void  CloudDialog::saveData()
{
  bool checked = false;
  
  checked = m_iAcceptCheckBox->isChecked();

  CloudProviderWidget * cloudProviderWidget = qobject_cast<CloudProviderWidget *>(this->m_pageStackedWidget->currentWidget());
  cloudProviderWidget->saveData();
}

//***** SLOTS *****

void CloudDialog::on_backButton(bool checked)
{
  if(m_pageStackedWidget->currentIndex() == m_settingsPageIdx){
    m_pageStackedWidget->setCurrentIndex(m_loginPageIdx);
    this->backButton()->hide();
    this->okButton()->setText("Continue");
  }
}

void CloudDialog::on_cancelButton(bool checked)
{
  OSDialog::on_cancelButton(checked);
}

void CloudDialog::on_okButton(bool checked)
{

 if(m_pageStackedWidget->currentIndex() == m_loginPageIdx){
    m_pageStackedWidget->setCurrentIndex(m_settingsPageIdx);
    // TODO test login settings, pop error and exit if need be
    //QString error("Error");
    //QMessageBox::critical(this, "Alert", error);
    this->backButton()->show();
    this->okButton()->setText("Save");
  } else if(m_pageStackedWidget->currentIndex() == m_settingsPageIdx){
    // Save data
     boost::optional<CloudProviderWidget *> cloudProviderWidget = this->getCurrentCloudProviderWidget();
     if(cloudProviderWidget.is_initialized()){
       cloudProviderWidget.get()->saveData();
     }
  }
}

void CloudDialog::iAcceptClicked(bool checked)
{
  // TODO Update this to work with current CloudSettings API
  //AWSProvider awsProvider;
  //awsProvider.signUserAgreement(checked);

  this->okButton()->setEnabled(checked);

  // show / hide leftLayout
}

void CloudDialog::cloudResourceChanged(const QString & text)
{
  if(text == NO_PROVIDER){
    this->okButton()->setEnabled(false);
    m_legalAgreement->hide();
    m_iAcceptCheckBox->hide();
    this->m_pageStackedWidget->setCurrentIndex(m_loginPageIdx);
    this->m_loginStackedWidget->setCurrentIndex(m_blankProviderIdx);
    this->m_settingsStackedWidget->setCurrentIndex(m_blankProviderIdx);
  } else if(text == VAGRANT_PROVIDER) {
    this->okButton()->setEnabled(true);
    m_legalAgreement->show();
    m_iAcceptCheckBox->show();
    this->m_pageStackedWidget->setCurrentIndex(m_loginPageIdx);
    this->m_loginStackedWidget->setCurrentIndex(m_vagrantProviderIdx);
    this->m_settingsStackedWidget->setCurrentIndex(m_vagrantProviderIdx);
  } else if(text == AMAZON_PROVIDER) {
    this->okButton()->setEnabled(true);
    m_legalAgreement->show();
    m_iAcceptCheckBox->show();
    this->m_pageStackedWidget->setCurrentIndex(m_loginPageIdx);
    this->m_loginStackedWidget->setCurrentIndex(m_amazonProviderIdx);
    this->m_settingsStackedWidget->setCurrentIndex(m_amazonProviderIdx);
  } else {
    // should never get here
    OS_ASSERT(false);
  }
}


//****************************************************************************************************


CloudProviderWidget::CloudProviderWidget(QWidget * parent)
  : QWidget(parent),
  m_waitCheckBox(0),
  m_waitLineEdit(0),
  m_loginWidget(0),
  m_leftSettingsWidget(0),
  m_rightSettingsWidget(0),
  m_leftLoginLayout(0),
  //m_rightLoginLayout(0),
  m_leftSettingsLayout(0),
  m_rightSettingsLayout(0)
{
  createWidgets();
}

CloudProviderWidget::~CloudProviderWidget()
{
}

void CloudProviderWidget::createWidgets()
{
  QHBoxLayout * hLayout = 0;
  QLabel * label = 0;
  bool isConnected = false;

  // LOGIN PAGE
  m_loginWidget = new QWidget();

  // LEFT LOGIN PAGE
  m_leftLoginLayout = new QVBoxLayout(this);
  m_loginWidget->setLayout(m_leftLoginLayout);

  // RIGHT LOGIN PAGE

  //m_rightLoginLayout = new QVBoxLayout(this);

  // SETTINGS PAGE

  // LEFT SETTINGS PAGE
  m_leftSettingsWidget = new QWidget();

  m_leftSettingsLayout = new QVBoxLayout(this);
  m_leftSettingsWidget->setLayout(m_leftSettingsLayout);

  // RIGHT SETTINGS PAGE
  m_rightSettingsWidget = new QWidget();

  m_rightSettingsLayout = new QVBoxLayout(this);
  m_rightSettingsWidget->setLayout(m_rightSettingsLayout);  

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Cloud Session Default Setting");
  m_rightSettingsLayout->addWidget(label);

  hLayout = new QHBoxLayout;
  hLayout->setContentsMargins(QMargins(0,0,0,0));
  hLayout->setSpacing(5);
  m_rightSettingsLayout->addLayout(hLayout);

  m_waitCheckBox = new QCheckBox();
  hLayout->addWidget(m_waitCheckBox,0,Qt::AlignTop | Qt::AlignLeft);

  isConnected = connect(m_waitCheckBox, SIGNAL(clicked(bool)),
    this, SLOT(waitClicked(bool)));
  OS_ASSERT(isConnected);

  label = new QLabel("After simulations complete and selected detailed results are downloaded, wait the number of minutes below and automatically terminate cloud session.  Parametric Analysis Tool must be on to stop cloud session.");
  label->setFixedWidth(TEXT_WIDTH);
  label->setWordWrap(true);
  hLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  hLayout->addStretch();

  hLayout = new QHBoxLayout;
  hLayout->setContentsMargins(QMargins(0,0,0,0));
  hLayout->setSpacing(5);
  m_rightSettingsLayout->addLayout(hLayout);

  m_waitLineEdit = new QLineEdit();
  m_waitLineEdit->setFixedWidth(EDIT_WIDTH);
  hLayout->addWidget(m_waitLineEdit,0,Qt::AlignTop | Qt::AlignLeft);

  label = new QLabel;
  label->setText("minutes");
  hLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  hLayout->addStretch();

  label = new QLabel;
  label->setFixedWidth(TEXT_WIDTH);
  label->setWordWrap(true);
  label->setObjectName("H2");
  label->setText("Stopping the cloud will terminate all instances.  Any detailed results not selected for download will lost.");
  m_rightSettingsLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);
}

//***** SLOTS *****

void CloudProviderWidget::waitClicked(bool checked)
{
}


//****************************************************************************************************


  BlankProviderWidget::BlankProviderWidget(QWidget * parent)
    : CloudProviderWidget(parent)
  {
  }

  BlankProviderWidget::~BlankProviderWidget()
  {
  }

  void BlankProviderWidget::loadData()
  {
  }

  void BlankProviderWidget::saveData()
  {
  }

  void BlankProviderWidget::createLoginWidget()
  {
  }

  void BlankProviderWidget::createSettingsWidget()
  {
  }


//****************************************************************************************************


VagrantProviderWidget::VagrantProviderWidget(QWidget * parent)
  : CloudProviderWidget(parent),
  m_runOnStartUpCheckBox(0),
  m_serverUsernameLineEdit(0),
  m_serverPasswordLineEdit(0),
  m_workerUsernameLineEdit(0),
  m_workerPasswordLineEdit(0),
  m_serverDirLineEdit(0),
  m_serverAddressIpLineEdit(0),
  m_serverPortIpLineEdit(0),
  m_workerDirLineEdit(0),
  m_workerAddressIpLineEdit(0),
  m_workerPortIpLineEdit(0)
{
  createLoginWidget();
  createSettingsWidget();
  loadData();
}

VagrantProviderWidget::~VagrantProviderWidget()
{
}

void VagrantProviderWidget::createLoginWidget()
{
  QLabel * label = 0;

  // LEFT LOGIN PAGE

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Server Username");
  m_leftLoginLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_serverUsernameLineEdit = new QLineEdit();
  m_serverUsernameLineEdit->setFixedWidth(EDIT_WIDTH);
  m_leftLoginLayout->addWidget(m_serverUsernameLineEdit);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Server Password");
  m_leftLoginLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_serverPasswordLineEdit = new QLineEdit();
  m_serverPasswordLineEdit->setFixedWidth(EDIT_WIDTH);
  m_leftLoginLayout->addWidget(m_serverPasswordLineEdit,0,Qt::AlignTop | Qt::AlignLeft);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Worker Instance");
  m_leftLoginLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_workerUsernameLineEdit = new QLineEdit();
  m_workerUsernameLineEdit->setFixedWidth(EDIT_WIDTH);
  m_leftLoginLayout->addWidget(m_workerUsernameLineEdit,0,Qt::AlignTop | Qt::AlignLeft);
  
  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Worker Password");
  m_leftLoginLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);
    
  m_workerPasswordLineEdit = new QLineEdit();
  m_workerPasswordLineEdit->setFixedWidth(EDIT_WIDTH);
  m_leftLoginLayout->addWidget(m_workerPasswordLineEdit);

  m_leftLoginLayout->addStretch();

  // RIGHT LOGIN PAGE

  // m_rightLoginLayout N/A
}

void VagrantProviderWidget::createSettingsWidget()
{
  QHBoxLayout * hLayout = 0;
  QLabel * label = 0;
  QPushButton * pushButton = 0;
  QWidget * widget = 0;
  bool isConnected = false;

  // LEFT SETTINGS PAGE

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Server Dir");
  m_leftSettingsLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  hLayout = new QHBoxLayout;
  hLayout->setContentsMargins(QMargins(0,0,0,0));
  hLayout->setSpacing(5);
  m_leftSettingsLayout->addLayout(hLayout);

  m_serverDirLineEdit = new QLineEdit();
  m_serverDirLineEdit->setFixedWidth(EDIT_WIDTH);
  hLayout->addWidget(m_serverDirLineEdit,0,Qt::AlignTop | Qt::AlignLeft);
  
  pushButton = new QPushButton("Browse");
  pushButton->setCheckable(false);
  hLayout->addWidget(pushButton,0,Qt::AlignTop | Qt::AlignLeft);
  isConnected = connect(pushButton, SIGNAL(clicked(bool)),
    this, SLOT(serverDirButtonClicked(bool)));
  OS_ASSERT(isConnected);

  hLayout->addStretch();

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Server IP");
  m_leftSettingsLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  hLayout = new QHBoxLayout;
  hLayout->setContentsMargins(QMargins(0,0,0,0));
  hLayout->setSpacing(5);
  m_leftSettingsLayout->addLayout(hLayout);

  m_serverAddressIpLineEdit = new QLineEdit();
  m_serverAddressIpLineEdit->setFixedWidth(ADDRESS_WIDTH);
  hLayout->addWidget(m_serverAddressIpLineEdit,0,Qt::AlignTop | Qt::AlignLeft);

  m_serverPortIpLineEdit = new QLineEdit();
  m_serverPortIpLineEdit->setFixedWidth(PORT_WIDTH);
  hLayout->addWidget(m_serverPortIpLineEdit,0,Qt::AlignTop | Qt::AlignLeft);

  hLayout->addStretch();

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Worker Dir");
  m_leftSettingsLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  hLayout = new QHBoxLayout;
  hLayout->setContentsMargins(QMargins(0,0,0,0));
  hLayout->setSpacing(5);
  m_leftSettingsLayout->addLayout(hLayout);

  m_workerDirLineEdit = new QLineEdit();
  m_workerDirLineEdit->setFixedWidth(EDIT_WIDTH);
  hLayout->addWidget(m_workerDirLineEdit,0,Qt::AlignTop | Qt::AlignLeft);

  pushButton = new QPushButton("Browse");
  pushButton->setCheckable(false);
  hLayout->addWidget(pushButton,0,Qt::AlignTop | Qt::AlignLeft);
  isConnected = connect(pushButton, SIGNAL(clicked(bool)),
    this, SLOT(workerDirButtonClicked(bool)));
  OS_ASSERT(isConnected);

  hLayout->addStretch();

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Worker IP");
  m_leftSettingsLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  hLayout = new QHBoxLayout;
  hLayout->setContentsMargins(QMargins(0,0,0,0));
  hLayout->setSpacing(5);
  m_leftSettingsLayout->addLayout(hLayout);

  m_workerAddressIpLineEdit = new QLineEdit();
  m_workerAddressIpLineEdit->setFixedWidth(ADDRESS_WIDTH);
  hLayout->addWidget(m_workerAddressIpLineEdit,0,Qt::AlignTop | Qt::AlignLeft);

  m_workerPortIpLineEdit = new QLineEdit();
  m_workerPortIpLineEdit->setFixedWidth(PORT_WIDTH);
  hLayout->addWidget(m_workerPortIpLineEdit,0,Qt::AlignTop | Qt::AlignLeft);

  hLayout->addStretch();

  m_leftSettingsLayout->addStretch();

  // RIGHT SETTINGS PAGE

  m_runOnStartUpCheckBox = new QCheckBox("Run Vagrant Halt on Stop");
  m_rightSettingsLayout->addWidget(m_runOnStartUpCheckBox,0,Qt::AlignTop | Qt::AlignLeft);

  m_rightSettingsLayout->addStretch();
}

void  VagrantProviderWidget::loadData()
{
  openstudio::path serverPath;
  openstudio::Url serverUrl;
  openstudio::path workerPath;
  openstudio::Url workerUrl;

  // TODO fix this to use new VagrantSettings API
  // VagrantProvider vagrantProvider(serverPath,serverUrl,workerPath,workerUrl);
  VagrantProvider vagrantProvider;
  //std::vector<Url> workerUrls = vagrantProvider.workerUrls();
  //boost::optional<Url> optionalServerUrl = vagrantProvider.serverUrl();

  bool isChecked = true;
  m_runOnStartUpCheckBox->setChecked(isChecked);

  QString text("N/A");

  m_serverUsernameLineEdit->setText(text);
  m_serverPasswordLineEdit->setText(text);
  m_workerUsernameLineEdit->setText(text);
  m_workerPasswordLineEdit->setText(text);
  m_serverDirLineEdit->setText(text);
  m_serverAddressIpLineEdit->setText(text);
  m_serverPortIpLineEdit->setText(text);
  m_workerDirLineEdit->setText(text);
  m_workerAddressIpLineEdit->setText(text);
  m_workerPortIpLineEdit->setText(text);
}

void  VagrantProviderWidget::saveData()
{

  bool isChecked = true;
  isChecked = m_runOnStartUpCheckBox->isChecked();

  QString text;
  text = m_serverUsernameLineEdit->text();
  text = m_serverPasswordLineEdit->text();
  text = m_workerUsernameLineEdit->text();
  text = m_workerPasswordLineEdit->text();
  text = m_serverDirLineEdit->text();
  text = m_serverAddressIpLineEdit->text();
  text = m_serverPortIpLineEdit->text();
  text = m_workerDirLineEdit->text();
  text = m_workerAddressIpLineEdit->text();
  text = m_workerPortIpLineEdit->text();
}

//***** SLOTS *****

void VagrantProviderWidget::serverDirButtonClicked(bool checked)
{
  // TODO
  QString dir = QFileDialog::getExistingDirectory( this,
                                                   tr("Choose Directory"),
                                                   QDir::homePath());

  if(!dir.length()) return;

}

void VagrantProviderWidget::workerDirButtonClicked(bool checked)
{
  // TODO
  QString dir = QFileDialog::getExistingDirectory( this,
                                                   tr("Choose Directory"),
                                                   QDir::homePath());

  if(!dir.length()) return;

}


//****************************************************************************************************


AmazonProviderWidget::AmazonProviderWidget(QWidget * parent)
  : CloudProviderWidget(parent),
  m_regionComboBox(0),
  m_serverInstanceTypeComboBox(0),
  m_workerInstanceTypeComboBox(0),
  m_accessKeyLineEdit(0),
  m_secretKeyLineEdit(0),
  m_selectPrivateKeyLineEdit(0),
  m_numberOfWorkerInstancesLineEdit(0),
  m_elasticStorageCapacityLineEdit(0)
{
  createLoginWidget();
  createSettingsWidget();
  loadData();
}

AmazonProviderWidget::~AmazonProviderWidget()
{
}

void AmazonProviderWidget::createLoginWidget()
{
  QLabel * label = 0;

  // LEFT LOGIN PAGE
  
  label = new QLabel;
  label->setOpenExternalLinks(true);
  label->setText("To create an account go to <a href=\"http://aws.amazon.com\">aws.amazon.com</a>");
  m_leftLoginLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Access Key");
  m_leftLoginLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_accessKeyLineEdit = new QLineEdit();
  m_accessKeyLineEdit->setFixedWidth(EDIT_WIDTH);
  m_leftLoginLayout->addWidget(m_accessKeyLineEdit,0,Qt::AlignTop | Qt::AlignLeft);
  
  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Secret Key");
  m_leftLoginLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_secretKeyLineEdit = new QLineEdit();
  m_secretKeyLineEdit->setFixedWidth(EDIT_WIDTH);
  m_leftLoginLayout->addWidget(m_secretKeyLineEdit,0,Qt::AlignTop | Qt::AlignLeft);
  
  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Select Private Key File");
  m_leftLoginLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);
    
  m_selectPrivateKeyLineEdit = new QLineEdit();
  m_secretKeyLineEdit->setFixedWidth(EDIT_WIDTH);
  m_leftLoginLayout->addWidget(m_selectPrivateKeyLineEdit,0,Qt::AlignTop | Qt::AlignLeft);

  m_leftLoginLayout->addStretch();

  // RIGHT LOGIN PAGE

  // m_rightLoginLayout N/A

}

void AmazonProviderWidget::createSettingsWidget()
{
  QLabel * label = 0;
  QWidget * widget = 0;

  // LEFT SETTINGS PAGE

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Region");
  m_leftSettingsLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_regionComboBox = new QComboBox();
  m_regionComboBox->addItem("East","us-east-1");
  m_leftSettingsLayout->addWidget(m_regionComboBox,0,Qt::AlignTop | Qt::AlignLeft);

  label = new QLabel;
  label->setFixedWidth(TEXT_WIDTH);
  label->setFixedHeight(60);
  label->setWordWrap(true);
  label->setText("This is the only region with CloudWatch enabled.  CloudWatch allows you to monitor your cost and performance.  The AWS Management Console provides a dashboard, alarms, and graphs.");
  m_leftSettingsLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Server Instance Type");
  m_leftSettingsLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_serverInstanceTypeComboBox = new QComboBox();
  m_leftSettingsLayout->addWidget(m_serverInstanceTypeComboBox,0,Qt::AlignTop | Qt::AlignLeft);
  
  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Worker Instance Type");
  m_leftSettingsLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_workerInstanceTypeComboBox = new QComboBox();
  m_leftSettingsLayout->addWidget(m_workerInstanceTypeComboBox,0,Qt::AlignTop | Qt::AlignLeft);
  
  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Number of Worker Instances");
  m_leftSettingsLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_numberOfWorkerInstancesLineEdit = new QLineEdit();
  m_numberOfWorkerInstancesLineEdit->setFixedWidth(EDIT_WIDTH);
  m_leftSettingsLayout->addWidget(m_numberOfWorkerInstancesLineEdit,0,Qt::AlignTop | Qt::AlignLeft);
  
  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Elastic Storage Capacity (GB)");
  m_leftSettingsLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);
  
  m_elasticStorageCapacityLineEdit = new QLineEdit();
  m_elasticStorageCapacityLineEdit->setFixedWidth(EDIT_WIDTH);
  m_leftSettingsLayout->addWidget(m_elasticStorageCapacityLineEdit,0,Qt::AlignTop | Qt::AlignLeft);

  m_leftSettingsLayout->addStretch();

  // RIGHT SETTINGS PAGE

  label = new QLabel;
  label->setOpenExternalLinks(true);
  label->setText("Monitor your cloud use with <a href=\"http://aws.amazon.com/cloudwatch\">CloudWatch</a> and <a href=\"http://aws.amazon.com/console\">AWS Management Console</a>.");
  m_rightSettingsLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);
  
  label = new QLabel;
  label->setOpenExternalLinks(true);
  label->setText("Review pricing for cloud service at <a href=\"http://aws.amazon.com/ec2/pricing\">aws.amazon.com/ec2/pricing</a>");
  m_rightSettingsLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_rightSettingsLayout->addStretch();
}

void  AmazonProviderWidget::loadData()
{
  AWSProvider awsProvider;
  
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

  //m_accessKeyLineEdit->setText(awsProvider.accessKey());
  //m_secretKeyLineEdit->setText(awsProvider.secretKey());

  QString text;
  m_numberOfWorkerInstancesLineEdit->setText(text.setNum(awsProvider.numWorkers()));
}

void  AmazonProviderWidget::saveData()
{
  AWSProvider awsProvider;

  QString text;

  text = m_regionComboBox->currentText();
  text = m_serverInstanceTypeComboBox->currentText();
  text = m_workerInstanceTypeComboBox->currentText();

  //awsProvider.setKeys(m_accessKeyLineEdit->text(),m_secretKeyLineEdit->text());

  text = m_numberOfWorkerInstancesLineEdit->text();
  unsigned numWorkers = text.toUInt();
  //awsProvider.setNumWorkers();
}

//***** SLOTS *****

} // openstudio
