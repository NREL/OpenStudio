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

#include "CloudDialog.hpp"

#include <pat_app/VagrantConfiguration.hxx>

#include "../utilities/cloud/AWSProvider.hpp"
#include "../utilities/cloud/AWSProvider_Impl.hpp"
#include "../utilities/cloud/CloudProvider.hpp"
#include "../utilities/cloud/CloudProvider_Impl.hpp"
#include "../utilities/cloud/VagrantProvider.hpp"
#include "../utilities/cloud/VagrantProvider_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Path.hpp"
#include "../utilities/core/Url.hpp"

#include <QBoxLayout>
#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QUrl>

#define NO_PROVIDER ""
#define VAGRANT_PROVIDER "Vagrant"
#define AMAZON_PROVIDER "Amazon EC2"
#define EDIT_WIDTH 150
#define KEY_WIDTH 300
#define ADDRESS_WIDTH 110
#define PORT_WIDTH 30
#define TEXT_WIDTH 350
#define TEXT_HEIGHT 390

using namespace openstudio::pat;

namespace openstudio {

CloudDialog::CloudDialog(QWidget* parent)
  : OSDialog(false, parent),
  m_iAcceptCheckBox(nullptr),
  m_cloudResourceComboBox(nullptr),
  m_pageStackedWidget(nullptr),
  m_leftLoginLayout(nullptr),
  m_rightLoginLayout(nullptr),
  m_mainSettingsLayout(nullptr),
  m_blankPageIdx(-1),
  m_loginPageIdx(-1),
  m_settingsPageIdx(-1),
  m_amazonProviderWidget(nullptr),
  m_blankProviderWidget(nullptr),
  m_vagrantProviderWidget(nullptr),
  m_legalAgreement(nullptr)
{
  setWindowTitle("Cloud Settings");
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  createWidgets();
}

CloudDialog::~CloudDialog()
{
}

void CloudDialog::createWidgets()
{
  QLabel * label = nullptr;
  bool isConnected = false;

  m_amazonProviderWidget = new AmazonProviderWidget(this);
  m_blankProviderWidget = new BlankProviderWidget(this);
  m_vagrantProviderWidget = new VagrantProviderWidget(this);

  // BLANK PAGE
  auto blankPageWidget = new QWidget();

  // LOGIN PAGE

  auto mainLoginLayout = new QHBoxLayout;
  mainLoginLayout->setContentsMargins(QMargins(0,0,0,0));
  mainLoginLayout->setSpacing(5);

  auto loginPageWidget = new QWidget;
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
 
  //m_cloudResourceComboBox->addItem(NO_PROVIDER);
  if(showVagrant()) m_cloudResourceComboBox->addItem(VAGRANT_PROVIDER);
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

  AWSSettings awsSettings;
  m_legalAgreement->setText(awsSettings.userAgreementText().c_str());

  auto scrollArea = new QScrollArea();
  scrollArea->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
  scrollArea->setFixedWidth(TEXT_WIDTH + 15);
  scrollArea->setMinimumHeight(TEXT_HEIGHT);
  scrollArea->setStyleSheet("background-color:transparent;");
  scrollArea->setWidget(m_legalAgreement);
  scrollArea->setWidgetResizable(true);
  scrollArea->setAlignment(Qt::AlignTop);
  scrollArea->setFrameShape(QFrame::NoFrame);
  m_rightLoginLayout->addWidget(scrollArea);

  m_iAcceptCheckBox = new QCheckBox("I Agree");
  m_iAcceptCheckBox->hide();
  m_rightLoginLayout->addWidget(m_iAcceptCheckBox,0,Qt::AlignTop | Qt::AlignLeft);

  m_rightLoginLayout->addSpacing(5);

  isConnected = connect(m_iAcceptCheckBox, SIGNAL(clicked(bool)),
    this, SLOT(iAcceptClicked(bool)));
  OS_ASSERT(isConnected);

  m_rightLoginLayout->addSpacing(5);
    
  // SETTINGS PAGE

  m_mainSettingsLayout = new QVBoxLayout;
  m_mainSettingsLayout->setContentsMargins(QMargins(0,0,0,0));
  m_mainSettingsLayout->setSpacing(5);

  auto settingsPageWidget = new QWidget;
  settingsPageWidget->setLayout(m_mainSettingsLayout);

  // SETTINGS STACKED WIDGET

  m_settingsStackedWidget = new  QStackedWidget();
  m_mainSettingsLayout->addWidget(m_settingsStackedWidget);

  auto widget = new QWidget();

  auto hLayout = new QHBoxLayout;
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
  this->okButton()->setText("Save");
  this->backButton()->setText("Continue");
  this->backButton()->setEnabled(false);
  this->backButton()->show();

  // OS SETTINGS

  #ifdef Q_OS_MAC
    setWindowFlags(Qt::FramelessWindowHint);
  #elif defined(Q_OS_WIN32)
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
  #endif

  cloudResourceChanged(m_cloudResourceComboBox->currentText());
}

boost::optional<CloudProviderWidget *> CloudDialog::getCurrentCloudProviderWidget()
{
  boost::optional<CloudProviderWidget *> cloudProviderWidget;

  if( m_cloudResourceComboBox->currentText() == AMAZON_PROVIDER){
    cloudProviderWidget = m_amazonProviderWidget;
  } else if( m_cloudResourceComboBox->currentText() == VAGRANT_PROVIDER){
    cloudProviderWidget = m_vagrantProviderWidget;
  }

  return cloudProviderWidget;
}

void CloudDialog::loadData()
{
  CloudProviderWidget * cloudProviderWidget = qobject_cast<CloudProviderWidget *>(this->m_pageStackedWidget->currentWidget());
  cloudProviderWidget->loadData();
}

void CloudDialog::saveData()
{
  CloudProviderWidget * cloudProviderWidget = qobject_cast<CloudProviderWidget *>(this->m_pageStackedWidget->currentWidget());
  cloudProviderWidget->saveData();
}

//***** SLOTS *****

void CloudDialog::on_backButton(bool checked)
{
  if(m_pageStackedWidget->currentIndex() == m_settingsPageIdx){
    m_pageStackedWidget->setCurrentIndex(m_loginPageIdx);
    this->backButton()->setText("Continue");
  } else if(m_pageStackedWidget->currentIndex() == m_loginPageIdx) {
    if( m_cloudResourceComboBox->currentText() == AMAZON_PROVIDER){
      AWSSettings awsSettings;
      std::string accessKey = m_amazonProviderWidget->m_accessKeyLineEdit->text().toStdString();
      std::string secretKey = m_amazonProviderWidget->m_secretKeyLineEdit->text().toStdString();
      if (accessKey.empty() || secretKey.empty()) {
        QString error("The Access Key and Secret Key cannot be empty");
        QMessageBox::critical(this, "Authentication Failed", error);
        return;
      }
      bool validAccessKey = awsSettings.validAccessKey(accessKey);
      if(!validAccessKey){
        QString error("You have entered an invalid Access Key");
        QMessageBox::critical(this, "Authentication Failed", error);
        return;
      }
      bool validSecretKey = awsSettings.validSecretKey(secretKey);
      if(!validSecretKey){
        QString error("You have entered an invalid Secret Key");
        QMessageBox::critical(this, "Authentication Failed", error);
        return;
      }
    }else if( m_cloudResourceComboBox->currentText() == VAGRANT_PROVIDER){
      VagrantSettings vagrantSettings;
      std::string serverUsername = m_vagrantProviderWidget->m_serverUsernameLineEdit->text().toStdString();
      std::string serverPassword = m_vagrantProviderWidget->m_serverPasswordLineEdit->text().toStdString();
      if (serverUsername.empty() || serverPassword.empty()) {
        QString error("The User Name and Password cannot be empty");
        QMessageBox::critical(this, "Authentication Failed", error);
        return;
      }
    }
    m_pageStackedWidget->setCurrentIndex(m_settingsPageIdx);
    this->backButton()->setText("Back");
  }
}

void CloudDialog::on_cancelButton(bool checked)
{
  OSDialog::on_cancelButton(checked);
  cloudResourceChanged(m_cloudResourceComboBox->currentText());
}

void CloudDialog::on_okButton(bool checked)
{
  if( m_cloudResourceComboBox->currentText() == AMAZON_PROVIDER){
    AWSSettings awsSettings;
    std::string accessKey = m_amazonProviderWidget->m_accessKeyLineEdit->text().toStdString();
    std::string secretKey = m_amazonProviderWidget->m_secretKeyLineEdit->text().toStdString();
    if (accessKey.empty() || secretKey.empty()) {
      QString error("The Access Key and Secret Key cannot be empty");
      QMessageBox::critical(this, "Authentication Failed", error);
      return;
    }
    bool validAccessKey = awsSettings.validAccessKey(accessKey);
    if(!validAccessKey){
      QString error("You have entered an invalid Access Key");
      QMessageBox::critical(this, "Authentication Failed", error);
      return;
    }
    bool validSecretKey = awsSettings.validSecretKey(secretKey);
    if(!validSecretKey){
      QString error("You have entered an invalid Secret Key");
      QMessageBox::critical(this, "Authentication Failed", error);
      return;
    }
  }else if( m_cloudResourceComboBox->currentText() == VAGRANT_PROVIDER){
    VagrantSettings vagrantSettings;

    std::string serverUsername = m_vagrantProviderWidget->m_serverUsernameLineEdit->text().toStdString();
    std::string serverPassword = m_vagrantProviderWidget->m_serverPasswordLineEdit->text().toStdString();
    if (serverUsername.empty() || serverPassword.empty()) {
      QString error("The User Name and Password cannot be empty");
      QMessageBox::critical(this, "Authentication Failed", error);
      return;
    }

    std::string serverDir = m_vagrantProviderWidget->m_serverDirLineEdit->text().toStdString();
    std::string serverAddress = m_vagrantProviderWidget->m_serverAddressIpLineEdit->text().toStdString();
    std::string workerDir = m_vagrantProviderWidget->m_workerDirLineEdit->text().toStdString();
    std::string workerAddress = m_vagrantProviderWidget->m_workerAddressIpLineEdit->text().toStdString();
    if (serverDir.empty() || serverAddress.empty() || workerDir.empty() || workerAddress.empty()) {
      QString error("Server and Worker Directories and IP Addresses are required");
      QMessageBox::critical(this, "Authentication Failed", error);
      return;
    }
  }

  // Save data
  boost::optional<CloudProviderWidget *> cloudProviderWidget = this->getCurrentCloudProviderWidget();
  if(cloudProviderWidget.is_initialized()){
    cloudProviderWidget.get()->saveData();
  }
  done(QDialog::Accepted);
  cloudResourceChanged(m_cloudResourceComboBox->currentText());
}

void CloudDialog::closeEvent(QCloseEvent *e)
{
  cloudResourceChanged(m_cloudResourceComboBox->currentText());
  e->accept();
}

void CloudDialog::iAcceptClicked(bool checked)
{
  this->backButton()->setEnabled(checked);
}

void CloudDialog::cloudResourceChanged(const QString & text)
{
  if(text == NO_PROVIDER){
    m_blankProviderWidget->loadData();
    this->backButton()->setEnabled(false);
    m_legalAgreement->hide();
    m_iAcceptCheckBox->hide();
    this->m_pageStackedWidget->setCurrentIndex(m_loginPageIdx);
    this->m_loginStackedWidget->setCurrentIndex(m_blankProviderIdx);
    this->m_settingsStackedWidget->setCurrentIndex(m_blankProviderIdx);
  } else if(text == VAGRANT_PROVIDER) {
    m_vagrantProviderWidget->loadData();
    this->backButton()->setEnabled(m_iAcceptCheckBox->isChecked());
    m_legalAgreement->show();
    m_iAcceptCheckBox->show();
    this->m_pageStackedWidget->setCurrentIndex(m_loginPageIdx);
    this->m_loginStackedWidget->setCurrentIndex(m_vagrantProviderIdx);
    this->m_settingsStackedWidget->setCurrentIndex(m_vagrantProviderIdx);
  } else if(text == AMAZON_PROVIDER) {
    m_amazonProviderWidget->loadData();
    this->backButton()->setEnabled(m_iAcceptCheckBox->isChecked());
    m_legalAgreement->show();
    m_iAcceptCheckBox->show();
    this->m_pageStackedWidget->setCurrentIndex(m_loginPageIdx);
    this->m_loginStackedWidget->setCurrentIndex(m_amazonProviderIdx);
    this->m_settingsStackedWidget->setCurrentIndex(m_amazonProviderIdx);
  } else {
    // should never get here
    OS_ASSERT(false);
  }
  this->backButton()->setText("Continue");
}


//****************************************************************************************************


CloudProviderWidget::CloudProviderWidget(CloudDialog * cloudDialog)
  : QWidget(cloudDialog),
  m_loginWidget(nullptr),
  m_leftSettingsWidget(nullptr),
  m_rightSettingsWidget(nullptr),
  m_leftLoginLayout(nullptr),
  m_leftSettingsLayout(nullptr),
  m_rightSettingsLayout(nullptr),
  m_waitCheckBox(nullptr),
  m_waitLineEdit(nullptr),
  m_cloudDialog(cloudDialog)
{
  createWidgets();
}

CloudProviderWidget::~CloudProviderWidget()
{
}

void CloudProviderWidget::createWidgets()
{
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

  //QLabel * label = new QLabel;
  //label->setObjectName("H2");
  //label->setText("Cloud Session Default Setting");
  //m_rightSettingsLayout->addWidget(label);

  //QHBoxLayout * hLayout = new QHBoxLayout;
  //hLayout->setContentsMargins(QMargins(0,0,0,0));
  //hLayout->setSpacing(5);
  //m_rightSettingsLayout->addLayout(hLayout);

  //m_waitCheckBox = new QCheckBox();
  //hLayout->addWidget(m_waitCheckBox,0,Qt::AlignTop | Qt::AlignLeft);

  //bool isConnected = connect(m_waitCheckBox, SIGNAL(clicked(bool)),
  //  this, SLOT(waitClicked(bool)));
  //OS_ASSERT(isConnected);

  //label = new QLabel("After simulations complete and selected detailed results are downloaded, wait the number of minutes below and automatically terminate cloud session.  ParametricAnalysisTool must be running to stop cloud session.");
  //label->setFixedWidth(TEXT_WIDTH);
  //label->setWordWrap(true);
  //hLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  //hLayout->addStretch();

  //hLayout = new QHBoxLayout;
  //hLayout->setContentsMargins(QMargins(0,0,0,0));
  //hLayout->setSpacing(5);
  //m_rightSettingsLayout->addLayout(hLayout);

  //m_waitLineEdit = new QLineEdit();
  //m_waitLineEdit->setFixedWidth(EDIT_WIDTH);
  //hLayout->addWidget(m_waitLineEdit,0,Qt::AlignTop | Qt::AlignLeft);
  //QValidator *waitLineEditValidator = new QIntValidator(0, 10080, this);
  //m_waitLineEdit->setValidator(waitLineEditValidator);

  //label = new QLabel;
  //label->setText("minutes");
  //hLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  //hLayout->addStretch();

  auto label = new QLabel;
  label->setFixedWidth(TEXT_WIDTH);
  label->setWordWrap(true);
  label->setObjectName("H1");
  label->setText("<FONT COLOR = RED>Results for simulations run on the cloud are only available while the cloud is running.  Stopping the cloud will terminate all instances, and any results not downloaded will be lost.");
  m_rightSettingsLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  QSpacerItem * spacerItem = nullptr;
  spacerItem = new QSpacerItem(0,25,QSizePolicy::Fixed,QSizePolicy::Fixed);
  m_rightSettingsLayout->addItem(spacerItem);
}

//***** SLOTS *****

void CloudProviderWidget::waitClicked(bool checked)
{
}


//****************************************************************************************************


BlankProviderWidget::BlankProviderWidget(CloudDialog * cloudDialog)
  : CloudProviderWidget(cloudDialog)
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


VagrantProviderWidget::VagrantProviderWidget(CloudDialog * cloudDialog)
  : CloudProviderWidget(cloudDialog),
  m_runOnStartUpCheckBox(nullptr),
  m_serverUsernameLineEdit(nullptr),
  m_serverPasswordLineEdit(nullptr),
  m_serverDirLineEdit(nullptr),
  m_serverAddressIpLineEdit(nullptr),
  m_serverPortIpLineEdit(nullptr),
  m_workerDirLineEdit(nullptr),
  m_workerAddressIpLineEdit(nullptr),
  m_workerPortIpLineEdit(nullptr)
{
  createLoginWidget();
  createSettingsWidget();
}

VagrantProviderWidget::~VagrantProviderWidget()
{
}

void VagrantProviderWidget::createLoginWidget()
{
  QLabel * label = nullptr;

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
  QHBoxLayout * hLayout = nullptr;
  QLabel * label = nullptr;
  QPushButton * pushButton = nullptr;
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

void VagrantProviderWidget::loadData()
{
  VagrantSettings vagrantSettings;
  QUrl url;
  QString temp;  

  m_cloudDialog->m_iAcceptCheckBox->setChecked(vagrantSettings.userAgreementSigned()); 

  bool isChecked = true;
  m_runOnStartUpCheckBox->setChecked(isChecked);

  m_serverUsernameLineEdit->setText(vagrantSettings.username().c_str());
  m_serverPasswordLineEdit->setText(vagrantSettings.password().c_str());

  url = vagrantSettings.serverUrl();
  m_serverAddressIpLineEdit->setText(url.host());
  m_serverPortIpLineEdit->setText(temp.setNum(url.port()));
  m_serverDirLineEdit->setText(toQString(vagrantSettings.serverPath()));

  url = vagrantSettings.serverUrl();
  m_workerAddressIpLineEdit->setText(url.host());
  m_workerPortIpLineEdit->setText(temp.setNum(url.port()));
  m_workerDirLineEdit->setText(toQString(vagrantSettings.workerPath()));

  //m_waitCheckBox->setChecked(vagrantSettings.terminationDelayEnabled());

  //m_waitLineEdit->setText(temp.setNum(vagrantSettings.terminationDelay()));
}

void VagrantProviderWidget::saveData()
{
  VagrantSettings vagrantSettings;

  vagrantSettings.signUserAgreement(m_cloudDialog->m_iAcceptCheckBox->isChecked()); 

  vagrantSettings.setUsername(m_serverUsernameLineEdit->text().toStdString());
  vagrantSettings.setPassword(m_serverPasswordLineEdit->text().toStdString());
  vagrantSettings.setServerPath(toPath(m_serverDirLineEdit->text().toStdString()));

  Url url;
  
  url.setHost( m_serverAddressIpLineEdit->text());
  url.setPort(m_serverPortIpLineEdit->text().toInt());
  vagrantSettings.setServerUrl(url);
  
  vagrantSettings.setWorkerPath(toPath(m_workerDirLineEdit->text().toStdString()));

  url.setHost(m_workerAddressIpLineEdit->text());
  url.setPort(m_workerPortIpLineEdit->text().toInt());
  vagrantSettings.setWorkerUrl(url);

  //vagrantSettings.setTerminationDelayEnabled(m_waitCheckBox->isChecked());

  //unsigned wait = m_waitLineEdit->text().toUInt();
  //vagrantSettings.setTerminationDelay(wait);

  vagrantSettings.saveToSettings(true);
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


AmazonProviderWidget::AmazonProviderWidget(CloudDialog * cloudDialog)
  : CloudProviderWidget(cloudDialog),
  m_accessKeyLineEdit(nullptr),
  m_secretKeyLineEdit(nullptr),
  m_regionComboBox(nullptr),
  m_serverInstanceTypeComboBox(nullptr),
  m_workerInstanceTypeComboBox(nullptr),
  m_numberOfWorkerInstancesLineEdit(nullptr)
{
  createLoginWidget();
  createSettingsWidget();
}

AmazonProviderWidget::~AmazonProviderWidget()
{
}

void AmazonProviderWidget::createLoginWidget()
{
  m_leftLoginLayout->setContentsMargins(QMargins(0,0,0,0));

  QLabel * label = nullptr;

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
  m_accessKeyLineEdit->setMinimumWidth(KEY_WIDTH);
  m_accessKeyLineEdit->setMaxLength(20);
  m_leftLoginLayout->addWidget(m_accessKeyLineEdit,0,Qt::AlignTop | Qt::AlignLeft);
  
  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Secret Key");
  m_leftLoginLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_secretKeyLineEdit = new QLineEdit();
  m_secretKeyLineEdit->setMinimumWidth(KEY_WIDTH);
  m_secretKeyLineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
  m_secretKeyLineEdit->setMaxLength(40);
  m_leftLoginLayout->addWidget(m_secretKeyLineEdit,0,Qt::AlignTop | Qt::AlignLeft);

  m_leftLoginLayout->addSpacing(30);

  label = new QLabel;
  label->setFixedWidth(TEXT_WIDTH);
  label->setWordWrap(true);
  label->setObjectName("H1");
  label->setOpenExternalLinks(true);
  label->setText("<FONT COLOR = RED>PAT cloud support with Amazon EC2 is a new feature, and is still under active development to improve interprocess reliability and performance. The user assumes all responsibility for orphaned EC2 processes, and it is strongly recommended that you monitor EC2 cloud usage at <a href=\"http://aws.amazon.com\">aws.amazon.com</a> to avoid any unwanted charges.");
  label->setAlignment(Qt::AlignTop);
  m_leftLoginLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_leftLoginLayout->addStretch();

  // RIGHT LOGIN PAGE

  // m_rightLoginLayout N/A
}

void AmazonProviderWidget::createSettingsWidget()
{
  QHBoxLayout * hLayout = nullptr;

  QVBoxLayout * vLayout = nullptr;

  QLabel * label = nullptr;

  bool isConnected = false;

  // LEFT SETTINGS PAGE

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Region");
  m_leftSettingsLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_regionComboBox = new QComboBox();
  m_leftSettingsLayout->addWidget(m_regionComboBox,0,Qt::AlignTop | Qt::AlignLeft);

  label = new QLabel;
  label->setFixedWidth(TEXT_WIDTH);
  label->setWordWrap(true);
  label->setText("This is the only region with CloudWatch enabled.  CloudWatch allows you to monitor your cost and performance.  The AWS Management Console provides a dashboard, alarms, and graphs.");
  m_leftSettingsLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Server Information");
  m_leftSettingsLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  hLayout = new QHBoxLayout;
  hLayout->setContentsMargins(QMargins(0,0,0,0));
  hLayout->setSpacing(15);
  m_leftSettingsLayout->addLayout(hLayout);

  vLayout = new QVBoxLayout;
  vLayout->setContentsMargins(QMargins(0,0,0,0));
  vLayout->setSpacing(5);
  hLayout->addLayout(vLayout);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Instance Type");
  vLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_serverInstanceTypeComboBox = new QComboBox();
  vLayout->addWidget(m_serverInstanceTypeComboBox,0,Qt::AlignTop | Qt::AlignLeft);

  isConnected = connect(m_serverInstanceTypeComboBox, SIGNAL(currentIndexChanged(const QString &)),
    this, SLOT(on_serverInstanceTypeComboBox(const QString &)));
  OS_ASSERT(isConnected); 

  vLayout = new QVBoxLayout;
  vLayout->setContentsMargins(QMargins(0,0,0,0));
  vLayout->setSpacing(5);
  hLayout->addLayout(vLayout);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("CPU Count");
  vLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_serverCpuCountLabel = new QLabel;
  vLayout->addWidget(m_serverCpuCountLabel,0,Qt::AlignTop | Qt::AlignLeft);

  vLayout = new QVBoxLayout;
  vLayout->setContentsMargins(QMargins(0,0,0,0));
  vLayout->setSpacing(5);
  hLayout->addLayout(vLayout);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Name");
  vLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_serverNameLabel = new QLabel;
  vLayout->addWidget(m_serverNameLabel,0,Qt::AlignTop | Qt::AlignLeft);

  QSpacerItem * vSpacer = nullptr;
  vSpacer = new QSpacerItem(0, 15, QSizePolicy::Fixed, QSizePolicy::Fixed);
  m_leftSettingsLayout->addSpacerItem(vSpacer); 

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Worker Information");
  m_leftSettingsLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  hLayout = new QHBoxLayout;
  hLayout->setContentsMargins(QMargins(0,0,0,0));
  hLayout->setSpacing(15);
  m_leftSettingsLayout->addLayout(hLayout);

  vLayout = new QVBoxLayout;
  vLayout->setContentsMargins(QMargins(0,0,0,0));
  vLayout->setSpacing(5);
  hLayout->addLayout(vLayout);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Instance Type");
  vLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_workerInstanceTypeComboBox = new QComboBox();
  vLayout->addWidget(m_workerInstanceTypeComboBox,0,Qt::AlignTop | Qt::AlignLeft);

  isConnected = connect(m_workerInstanceTypeComboBox, SIGNAL(currentIndexChanged(const QString &)),
    this, SLOT(on_workerInstanceTypeComboBox(const QString &)));
  OS_ASSERT(isConnected); 

  vLayout = new QVBoxLayout;
  vLayout->setContentsMargins(QMargins(0,0,0,0));
  vLayout->setSpacing(5);
  hLayout->addLayout(vLayout);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("CPU Count");
  vLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_workerCpuCountLabel = new QLabel;
  vLayout->addWidget(m_workerCpuCountLabel,0,Qt::AlignTop | Qt::AlignLeft);

  vLayout = new QVBoxLayout;
  vLayout->setContentsMargins(QMargins(0,0,0,0));
  vLayout->setSpacing(5);
  hLayout->addLayout(vLayout);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Name");
  vLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_workerNameLabel = new QLabel;
  vLayout->addWidget(m_workerNameLabel,0,Qt::AlignTop | Qt::AlignLeft);

  label = new QLabel;
  label->setObjectName("H2");
  label->setText("Number of Worker Instances");
  m_leftSettingsLayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_numberOfWorkerInstancesLineEdit = new QLineEdit();
  m_leftSettingsLayout->addWidget(m_numberOfWorkerInstancesLineEdit,0,Qt::AlignTop | Qt::AlignLeft);
  QValidator *numberOfWorkerInstancesValidator = new QIntValidator(1, 19, this);
  m_numberOfWorkerInstancesLineEdit->setValidator(numberOfWorkerInstancesValidator);

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

void AmazonProviderWidget::loadData()
{
  AWSSettings awsSettings;

  m_accessKeyLineEdit->setText(awsSettings.accessKey().c_str());

  m_secretKeyLineEdit->setText(awsSettings.secretKey().c_str());

  m_cloudDialog->m_iAcceptCheckBox->setChecked(awsSettings.userAgreementSigned()); 

  if (!m_regionComboBox->count()) {
    for (const std::string & region : AWSProvider::availableRegions()) {
      m_regionComboBox->addItem(region.c_str());
    }
  }

  int index = m_regionComboBox->findText(awsSettings.region().c_str());
  if(index == -1){
    index = m_regionComboBox->findText(AWSProvider::defaultRegion().c_str());
  }
  m_regionComboBox->setCurrentIndex(index);


  if (!m_serverInstanceTypeComboBox->count()) {
    for (const std::string & serverInstanceType : AWSProvider::serverInstanceTypes()) {
      m_serverInstanceTypeComboBox->addItem(serverInstanceType.c_str());
    }
  }

  index = m_serverInstanceTypeComboBox->findText(awsSettings.serverInstanceType().c_str());
  if(index == -1){
    index = m_serverInstanceTypeComboBox->findText(AWSProvider::defaultServerInstanceType().c_str());
  }
  m_serverInstanceTypeComboBox->setCurrentIndex(index);

  if (!m_workerInstanceTypeComboBox->count()) {
    for (const std::string & workerInstanceType : AWSProvider::workerInstanceTypes()) {
      m_workerInstanceTypeComboBox->addItem(workerInstanceType.c_str());
    }
  }

  index = m_workerInstanceTypeComboBox->findText(awsSettings.workerInstanceType().c_str());
  if(index == -1){
    index = m_workerInstanceTypeComboBox->findText(AWSProvider::defaultWorkerInstanceType().c_str());
  }
  m_workerInstanceTypeComboBox->setCurrentIndex(index);

  QString temp;
  m_numberOfWorkerInstancesLineEdit->setText(temp.setNum(awsSettings.numWorkers()));

  //m_waitCheckBox->setChecked(awsSettings.terminationDelayEnabled());

  //m_waitLineEdit->setText(temp.setNum(awsSettings.terminationDelay()));
}

void AmazonProviderWidget::saveData()
{
  AWSSettings awsSettings;

  bool validAccessKey = awsSettings.setAccessKey(m_accessKeyLineEdit->text().toStdString());
  OS_ASSERT(validAccessKey);

  bool validSecretKey = awsSettings.setSecretKey(m_secretKeyLineEdit->text().toStdString());
  OS_ASSERT(validSecretKey);

  awsSettings.signUserAgreement(m_cloudDialog->m_iAcceptCheckBox->isChecked()); 

  awsSettings.setRegion(m_regionComboBox->currentText().toStdString());

  awsSettings.setServerInstanceType(m_serverInstanceTypeComboBox->currentText().toStdString());

  awsSettings.setWorkerInstanceType(m_workerInstanceTypeComboBox->currentText().toStdString());

  QString temp;

  unsigned numWorkers = m_numberOfWorkerInstancesLineEdit->text().toUInt();
  awsSettings.setNumWorkers(numWorkers);
 
  //awsSettings.setTerminationDelayEnabled(m_waitCheckBox->isChecked());

  //unsigned wait = m_waitLineEdit->text().toUInt();
  //awsSettings.setTerminationDelay(wait);

  awsSettings.saveToSettings(true);
}

//***** SLOTS *****

void AmazonProviderWidget::on_serverInstanceTypeComboBox(const QString & text)
{
  QString temp;

  m_serverNameLabel->setText(AWSProvider::getServerPrettyName(text.toStdString()).c_str());
  m_serverCpuCountLabel->setText(temp.setNum(AWSProvider::getServerProcessorCount(text.toStdString())));
}

void AmazonProviderWidget::on_workerInstanceTypeComboBox(const QString & text)
{
  QString temp;

  m_workerNameLabel->setText(AWSProvider::getWorkerPrettyName(text.toStdString()).c_str());
  m_workerCpuCountLabel->setText(temp.setNum(AWSProvider::getWorkerProcessorCount(text.toStdString())));
}

} // openstudio
