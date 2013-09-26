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

#include <pat_app/CloudMonitor.hpp>

#include <utilities/cloud/AWSProvider.hpp>
#include <utilities/cloud/AWSProvider_Impl.hpp>
#include <utilities/cloud/CloudProvider.hpp>
#include <utilities/cloud/CloudProvider_Impl.hpp>
#include <utilities/cloud/VagrantProvider.hpp>
#include <utilities/cloud/VagrantProvider_Impl.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/core/Url.hpp>

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QUrl>

#define NO_PROVIDER ""
#define VAGRANT_PROVIDER "Vagrant"
#define AMAZON_PROVIDER "Amazon EC2"
#define EDIT_WIDTH 150
#define ADDRESS_WIDTH 110
#define PORT_WIDTH 30
#define TEXT_WIDTH 350
#define TEXT_HEIGHT 400

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
  pat::CloudMonitor cloudMonitor;
  bool showVagrant = cloudMonitor.showVagrantOption();
  if(showVagrant) m_cloudResourceComboBox->addItem(VAGRANT_PROVIDER);
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

  m_legalAgreement = new QLabel;
  m_legalAgreement->hide();
  m_legalAgreement->setWordWrap(true);
  m_legalAgreement->setFixedHeight(TEXT_HEIGHT);
  m_legalAgreement->setFixedWidth(TEXT_WIDTH);

  AWSProvider awsProvider;
  m_legalAgreement->setText(awsProvider.userAgreementText().c_str());

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
    if( m_cloudResourceComboBox->currentText() == AMAZON_PROVIDER){
      AWSProvider awsProvider;
      // TODO these can be used in realtime (i.e. per keystroke)
      //bool validAccessKey = awsProvider.validAccessKey(m_amazonProviderWidget->m_accessKeyLineEdit->text().toStdString());
      //bool validSecretKey = awsProvider.validSecretKey(m_amazonProviderWidget->m_secretKeyLineEdit->text().toStdString());
      bool validAccessKey = awsProvider.setAccessKey(m_amazonProviderWidget->m_accessKeyLineEdit->text().toStdString());
      if(!validAccessKey){
        QString error("You have entered an invalid Access Key");
        QMessageBox::critical(this, "Login Failed", error);
        return;
      }
      bool validSecretKey = awsProvider.setSecretKey(m_amazonProviderWidget->m_secretKeyLineEdit->text().toStdString());
      if(!validSecretKey){
        QString error("You have entered an invalid Secret Key");
        QMessageBox::critical(this, "Login Failed", error);
        return;
      }
    }
    m_pageStackedWidget->setCurrentIndex(m_settingsPageIdx);
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
  this->okButton()->setEnabled(checked);
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
    this->okButton()->setEnabled(m_iAcceptCheckBox->isChecked());
    m_legalAgreement->show();
    m_iAcceptCheckBox->show();
    this->m_pageStackedWidget->setCurrentIndex(m_loginPageIdx);
    this->m_loginStackedWidget->setCurrentIndex(m_vagrantProviderIdx);
    this->m_settingsStackedWidget->setCurrentIndex(m_vagrantProviderIdx);
  } else if(text == AMAZON_PROVIDER) {
    this->okButton()->setEnabled(m_iAcceptCheckBox->isChecked());
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
  label->setText("Username");
  m_leftLoginLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_serverUsernameLineEdit = new QLineEdit();
  m_serverUsernameLineEdit->setFixedWidth(EDIT_WIDTH);
  m_leftLoginLayout->addWidget(m_serverUsernameLineEdit);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Password");
  m_leftLoginLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_serverPasswordLineEdit = new QLineEdit();
  m_serverPasswordLineEdit->setFixedWidth(EDIT_WIDTH);
  m_leftLoginLayout->addWidget(m_serverPasswordLineEdit,0,Qt::AlignTop | Qt::AlignLeft);

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
  VagrantSettings vagrantSettings;
  QUrl url;
  QString temp;  

  bool isChecked = true;
  m_runOnStartUpCheckBox->setChecked(isChecked);

  m_serverUsernameLineEdit->setText(vagrantSettings.username().c_str());
  m_serverPasswordLineEdit->setText(vagrantSettings.password().c_str());

  url = vagrantSettings.serverUrl();
  m_serverAddressIpLineEdit->setText(url.path());
  m_serverPortIpLineEdit->setText(temp.setNum(url.port()));
  m_serverDirLineEdit->setText(toQString(vagrantSettings.serverPath()));

  url = vagrantSettings.serverUrl();
  m_workerAddressIpLineEdit->setText(url.path());
  m_workerPortIpLineEdit->setText(temp.setNum(url.port()));
  m_workerDirLineEdit->setText(toQString(vagrantSettings.workerPath()));

  // TODO
  //m_waitCheckBox->setChecked(vagrantSettings.terminationDelayEnabled());

  //m_waitLineEdit->setText(temp.setNum(vagrantSettings.terminationDelay()));
}

void  VagrantProviderWidget::saveData()
{
  VagrantSettings vagrantSettings;

  bool isChecked = true;
  isChecked = m_runOnStartUpCheckBox->isChecked();

  vagrantSettings.setUsername(m_serverUsernameLineEdit->text().toStdString());
  vagrantSettings.setPassword(m_serverPasswordLineEdit->text().toStdString());
  vagrantSettings.setServerPath(toPath(m_serverDirLineEdit->text().toStdString()));

  Url url;
  
  url.setPath( m_serverAddressIpLineEdit->text());
  url.setPort(m_serverPortIpLineEdit->text().toInt());
  vagrantSettings.setServerUrl(url);
  
  vagrantSettings.setWorkerPath(toPath(m_workerDirLineEdit->text().toStdString()));

  url.setPath(m_workerAddressIpLineEdit->text());
  url.setPort(m_workerPortIpLineEdit->text().toInt());
  vagrantSettings.setWorkerUrl(url);

  // TODO
  //vagrantSettings.setTerminationDelayEnabled(m_waitCheckBox->isChecked());

  //unsigned wait = m_waitLineEdit->text().toUInt();
  //vagrantSettings.setTerminationDelay(wait);
}

//***** SLOTS *****

void VagrantProviderWidget::serverDirButtonClicked(bool checked)
{
  QString dir = QFileDialog::getExistingDirectory( this,
                                                   tr("Choose Directory"),
                                                   QDir::homePath());

  if(dir.length()){
    m_serverDirLineEdit->setText(dir);
    m_serverDirLineEdit->setToolTip(dir);
  }
}

void VagrantProviderWidget::workerDirButtonClicked(bool checked)
{
  QString dir = QFileDialog::getExistingDirectory( this,
                                                   tr("Choose Directory"),
                                                   QDir::homePath());

  if(dir.length()){
    m_workerDirLineEdit->setText(dir);
    m_workerDirLineEdit->setToolTip(dir);
  }
}


//****************************************************************************************************


AmazonProviderWidget::AmazonProviderWidget(QWidget * parent)
  : CloudProviderWidget(parent),
  m_regionComboBox(0),
  m_serverInstanceTypeComboBox(0),
  m_workerInstanceTypeComboBox(0),
  m_accessKeyLineEdit(0),
  m_secretKeyLineEdit(0),
  m_numberOfWorkerInstancesLineEdit(0)
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
  AWSSettings awsSettings;

  m_accessKeyLineEdit->setText(awsSettings.accessKey().c_str());

  m_secretKeyLineEdit->setText(awsSettings.secretKey().c_str());

  int index = -1;

  for(int idx = 0; idx < m_regionComboBox->count(); idx++){
    m_regionComboBox->removeItem(idx);
  }

  std::vector<std::string> availableRegions = awsProvider.availableRegions();
  Q_FOREACH(const std::string & region, availableRegions){
    m_regionComboBox->addItem(region.c_str());
  }

  // TODO should awsSettings API have this call?
  index = m_regionComboBox->findText(awsProvider.region().c_str());
  if(index == -1) index = 0;
  m_regionComboBox->setCurrentIndex(index);


  for(int idx = 0; idx < m_serverInstanceTypeComboBox->count(); idx++){
    m_serverInstanceTypeComboBox->removeItem(idx);
  }

  std::vector<std::string> serverInstanceTypes = awsProvider.serverInstanceTypes();
  Q_FOREACH(const std::string & serverInstanceType, serverInstanceTypes){
    m_serverInstanceTypeComboBox->addItem(serverInstanceType.c_str());
  }

  // TODO should awsSettings API have this call?
  index = m_serverInstanceTypeComboBox->findText(awsProvider.serverInstanceType().c_str());
  if(index == -1){
    index = m_serverInstanceTypeComboBox->findText(awsProvider.defaultServerInstanceType().c_str());
  }
  m_serverInstanceTypeComboBox->setCurrentIndex(index);

  for(int idx = 0; idx < m_workerInstanceTypeComboBox->count(); idx++){
    m_workerInstanceTypeComboBox->removeItem(idx);
  }

  std::vector<std::string> workerInstanceTypes = awsProvider.workerInstanceTypes();
  Q_FOREACH(const std::string & workerInstanceType, workerInstanceTypes){
    m_workerInstanceTypeComboBox->addItem(workerInstanceType.c_str());
  }

  // TODO should awsSettings API have this call?
  index = m_workerInstanceTypeComboBox->findText(awsProvider.workerInstanceType().c_str());
  if(index == -1){
    index = m_workerInstanceTypeComboBox->findText(awsProvider.defaultWorkerInstanceType().c_str());
  }
  m_workerInstanceTypeComboBox->setCurrentIndex(index);

  QString temp;
  // TODO should awsSettings API have this call?
  m_numberOfWorkerInstancesLineEdit->setText(temp.setNum(awsProvider.numWorkers()));

  m_waitCheckBox->setChecked(awsSettings.terminationDelayEnabled());

  m_waitLineEdit->setText(temp.setNum(awsSettings.terminationDelay()));
}

void  AmazonProviderWidget::saveData()
{
  AWSProvider awsProvider;
  AWSSettings awsSettings;

  bool validAccessKey = awsProvider.setAccessKey(m_accessKeyLineEdit->text().toStdString());
  OS_ASSERT(validAccessKey);

  bool validSecretKey = awsProvider.setSecretKey(m_secretKeyLineEdit->text().toStdString());
  OS_ASSERT(validSecretKey);

  awsProvider.setRegion(m_regionComboBox->currentText().toStdString());

  awsProvider.setServerInstanceType(m_serverInstanceTypeComboBox->currentText().toStdString());

  awsProvider.setWorkerInstanceType(m_workerInstanceTypeComboBox->currentText().toStdString());

  QString temp;

  unsigned numWorkers = m_numberOfWorkerInstancesLineEdit->text().toUInt();
  awsProvider.setNumWorkers(numWorkers);
 
  awsProvider.setTerminationDelayEnabled(m_waitCheckBox->isChecked());

  unsigned wait = m_waitLineEdit->text().toUInt();
  awsProvider.setTerminationDelay(wait);
}

//***** SLOTS *****

} // openstudio
