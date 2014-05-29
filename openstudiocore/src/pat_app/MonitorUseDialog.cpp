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

#include "MonitorUseDialog.hpp"

#include "CloudMonitor.hpp"

#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QDesktopServices>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QShowEvent>
#include <QTimer>
#include <QUrl>

#define BILLING_CHARGE "Estimated EC2 Charges: "
#define BILLING_MESSAGE "Amazon billing data is delayed and will not reflect instantaneous charges"
#define MINUTES " minutes"
#define TIME_RUNNING "Project Run Time: "
#define NUM_INSTANCES "Instances Running on Current Project: "
#define TOTAL_NUM_INSTANCES "Total Instances Running: " 
#define WARNING "Stopping the cloud will terminate all instances.  Any detailed results not selected for download will be lost."
#define TEXT_WIDTH 300
#define STATUS_WIDTH 150

namespace openstudio {

namespace pat {

MonitorUseDialog::MonitorUseDialog(QWidget* parent)
  : OSDialog(false, parent),
  m_totalInstancesAvailable(false),
  m_estimatedChargesAvailable(false)
{
  setWindowTitle("Monitor Use");
  cancelButton()->hide();
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  createWidgets();
}

MonitorUseDialog::~MonitorUseDialog()
{
}

void MonitorUseDialog::createWidgets()
{
  auto layout = new QVBoxLayout;
  layout->setContentsMargins(QMargins(0,0,0,0));
  layout->setSpacing(5);
  upperLayout()->addLayout(layout);

  QHBoxLayout * hlayout = nullptr;

  QLabel * label = nullptr;

  /////

  hlayout = new QHBoxLayout;
  hlayout->setContentsMargins(QMargins(0,0,0,0));
  hlayout->setSpacing(5);
  layout->addLayout(hlayout);

  label = new QLabel();
  label->setText(BILLING_CHARGE);
  hlayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_billingCharge = new QLabel("N/A");
  m_billingCharge->setObjectName("H2");
  hlayout->addWidget(m_billingCharge,0,Qt::AlignTop | Qt::AlignLeft);

  QPixmap infoIconPixmap = QPixmap(":/shared_gui_components/images/warning_icon.png").scaled(QSize(16,16), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
  m_billingInfoButton = new QPushButton(infoIconPixmap, QString(), this);
  QString style = "QPushButton {"
                  "  border:none;"
                  "}";
  m_billingInfoButton->setStyleSheet(style);
  m_billingInfoButton->setFlat(true);
  m_billingInfoButton->setToolTip(BILLING_MESSAGE);
  bool isConnected = connect(m_billingInfoButton, SIGNAL(clicked(bool)),
                             this, SLOT(displayBillingMessage(bool)));
  OS_ASSERT(isConnected);
  hlayout->addWidget(m_billingInfoButton);

  hlayout->addStretch();

  /////

  hlayout = new QHBoxLayout;
  hlayout->setContentsMargins(QMargins(0,0,0,0));
  hlayout->setSpacing(5);
  layout->addLayout(hlayout);
  
  label = new QLabel();
  label->setText(TIME_RUNNING);
  hlayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);
  
  m_timeRunning = new QLabel("N/A");
  m_timeRunning->setObjectName("H2");
  hlayout->addWidget(m_timeRunning,0,Qt::AlignTop | Qt::AlignLeft);

  hlayout->addStretch();

  /////

  hlayout = new QHBoxLayout;
  hlayout->setContentsMargins(QMargins(0,0,0,0));
  hlayout->setSpacing(5);
  layout->addLayout(hlayout);
  
  label = new QLabel();
  label->setText(NUM_INSTANCES);
  hlayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_numInstances = new QLabel("N/A");
  m_numInstances->setObjectName("H2");
  hlayout->addWidget(m_numInstances,0,Qt::AlignTop | Qt::AlignLeft);

  hlayout->addStretch();

  /////

  hlayout = new QHBoxLayout;
  hlayout->setContentsMargins(QMargins(0,0,0,0));
  hlayout->setSpacing(5);
  layout->addLayout(hlayout);
  
  label = new QLabel();
  label->setText(TOTAL_NUM_INSTANCES);
  hlayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_totalNumInstances = new QLabel("N/A");
  m_totalNumInstances->setObjectName("H2");
  hlayout->addWidget(m_totalNumInstances,0,Qt::AlignTop | Qt::AlignLeft);

  hlayout->addStretch();

  /////

  m_cloudStatus = new QPushButton;
  m_cloudStatus->setFixedSize(QSize(192,47));
  style = "QPushButton {"
          "  background-image:url(':/shared_gui_components/images/manage_all_button.png');"
          "  border:none;"
          "}";
  m_cloudStatus->setStyleSheet(style);
  layout->addWidget(m_cloudStatus,0,Qt::AlignTop | Qt::AlignLeft);
  isConnected = connect(m_cloudStatus, SIGNAL(clicked(bool)),
                             this, SLOT(on_cloudStatus(bool)));
  OS_ASSERT(isConnected);

  layout->addSpacing(5);

  /////

  label = new QLabel();
  label->setObjectName("H2");
  label->setWordWrap(true);
  label->setFixedWidth(TEXT_WIDTH);
  label->setText(WARNING);
  layout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  /////

  layout->addStretch();

  /////
 
  isConnected = connect(m_awsProvider.getImpl<detail::CloudProvider_Impl>().get(), SIGNAL(estimatedChargesAvailable()),
                        this, SLOT(on_estimatedChargesAvailable()));
  OS_ASSERT(isConnected);

  isConnected = connect(m_awsProvider.getImpl<detail::CloudProvider_Impl>().get(), SIGNAL(totalInstancesAvailable()),
                        this, SLOT(on_totalInstancesAvailable()));
  OS_ASSERT(isConnected);

  m_timer = new QTimer(this);
  isConnected = connect(m_timer, SIGNAL(timeout()),
                        this, SLOT(updateData()));
  OS_ASSERT(isConnected);

  // OS SETTINGS

  #ifdef Q_OS_MAC
    setWindowFlags(Qt::FramelessWindowHint);
  #elif defined(Q_OS_WIN32)
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
  #endif
}

void MonitorUseDialog::showEvent(QShowEvent *event) {
  if (!event->spontaneous() && !testAttribute(Qt::WA_Moved)) {
    Qt::WindowStates state = windowState();
    adjustPosition(parentWidget());
    setAttribute(Qt::WA_Moved, false); // not really an explicit position
    if (state != windowState()) setWindowState(state);
  }

  if (!m_timer->isActive()) {
    m_timer->start(30000);
    updateData();
  }
}

void MonitorUseDialog::displayErrors()
{
  m_timer->stop();
  QString msg;
  std::vector<std::string> errors = m_awsProvider.errors();
  if (errors.size()) {
    msg += errors[0].c_str();
    msg += ".  Verify settings from Cloud menu.";
  }
  if (msg.size()) QMessageBox::warning(this, "Monitor Use: AWS Error", msg);
}

//// SLOTS

void  MonitorUseDialog::updateData()
{
  // Should this use currentProjectSettings at all?
  if (CloudMonitor::currentProjectSettings()) {
    bool bingo = m_awsProvider.setSettings(*CloudMonitor::currentProjectSettings());
    OS_ASSERT(bingo);
  } else {
    bool bingo = m_awsProvider.setSettings(AWSSettings());
    OS_ASSERT(bingo);
  }

  QString temp;

  m_awsProvider.requestEstimatedCharges();
  m_awsProvider.requestTotalInstances();

  boost::optional<CloudSession> session = CloudMonitor::currentProjectSession();
  if (session) {
    boost::optional<AWSSession> awsSession = session->optionalCast<AWSSession>();
    if (awsSession) {
      temp = temp.setNum(awsSession->totalSessionUptime());
      temp += MINUTES; 
      m_timeRunning->setText(temp);

      m_numInstances->setText(temp.setNum(awsSession->totalSessionInstances()));

      return;
    }
  }

  temp = temp.setNum(0);
  temp += MINUTES; 
  m_timeRunning->setText(temp);

  m_numInstances->setText("0");
}

void MonitorUseDialog::displayBillingMessage(bool checked)
{
  QMessageBox::information(this, "Amazon Billing Data Delay", BILLING_MESSAGE);
}

void MonitorUseDialog::on_cloudStatus(bool checked)
{
  QDesktopServices::openUrl(QUrl("http://aws.amazon.com/console"));
}

void MonitorUseDialog::on_estimatedChargesAvailable()
{
  if (m_timer->isActive()) {
    if(m_awsProvider.errors().size()){
      displayErrors();
      m_billingCharge->setText("N/A");
      return;
    }

    QString temp;

    temp = temp.setNum(m_awsProvider.lastEstimatedCharges(), 'f', 2);
    temp.prepend('$');
    m_billingCharge->setText(temp);
  } else {
    m_billingCharge->setText("N/A");
  }
}

void MonitorUseDialog::on_totalInstancesAvailable()
{
  if (m_timer->isActive()) {
    if(m_awsProvider.errors().size()){
      displayErrors();
      m_totalNumInstances->setText("N/A");
      return;
    }

    QString temp;

    m_totalNumInstances->setText(temp.setNum(m_awsProvider.lastTotalInstances()));
  } else {
    m_totalNumInstances->setText("N/A");
  }
}

} // pat

} // openstudio
