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

#include "MonitorUseDialog.hpp"

#include "CloudMonitor.hpp"

#include <utilities/core/Assert.hpp>

#include <QBoxLayout>
#include <QDesktopServices>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QUrl>

#define BILLING_CHARGE "Estimated EC2 Charges: "
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
  this->setWindowTitle("Monitor Use");
  this->setFixedSize(QSize(350,300));
  this->cancelButton()->hide();
  createWidgets();
}

MonitorUseDialog::~MonitorUseDialog()
{
}

void MonitorUseDialog::createWidgets()
{
  QVBoxLayout * layout = new QVBoxLayout;
  layout->setContentsMargins(QMargins(0,0,0,0));
  layout->setSpacing(5);
  upperLayout()->addLayout(layout);

  QHBoxLayout * hlayout = 0;

  QLabel * label = 0;

  /////

  hlayout = new QHBoxLayout;
  hlayout->setContentsMargins(QMargins(0,0,0,0));
  hlayout->setSpacing(5);
  layout->addLayout(hlayout);

  label = new QLabel;
  label->setText(BILLING_CHARGE);
  hlayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_billingCharge = new QLabel;
  m_billingCharge->setObjectName("H2");
  hlayout->addWidget(m_billingCharge,0,Qt::AlignTop | Qt::AlignLeft);

  hlayout->addStretch();

  /////

  hlayout = new QHBoxLayout;
  hlayout->setContentsMargins(QMargins(0,0,0,0));
  hlayout->setSpacing(5);
  layout->addLayout(hlayout);
  
  label = new QLabel;
  label->setText(TIME_RUNNING);
  hlayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);
  
  m_timeRunning = new QLabel;
  m_timeRunning->setObjectName("H2");
  hlayout->addWidget(m_timeRunning,0,Qt::AlignTop | Qt::AlignLeft);

  hlayout->addStretch();

  /////

  hlayout = new QHBoxLayout;
  hlayout->setContentsMargins(QMargins(0,0,0,0));
  hlayout->setSpacing(5);
  layout->addLayout(hlayout);
  
  label = new QLabel;
  label->setText(NUM_INSTANCES);
  hlayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);
  label = new QLabel;

  m_numInstances = new QLabel;
  m_numInstances->setObjectName("H2");
  hlayout->addWidget(m_numInstances,0,Qt::AlignTop | Qt::AlignLeft);

  hlayout->addStretch();

  /////

  hlayout = new QHBoxLayout;
  hlayout->setContentsMargins(QMargins(0,0,0,0));
  hlayout->setSpacing(5);
  layout->addLayout(hlayout);
  
  label->setText(TOTAL_NUM_INSTANCES);
  hlayout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  m_totalNumInstances = new QLabel;
  m_totalNumInstances->setObjectName("H2");
  hlayout->addWidget(m_totalNumInstances,0,Qt::AlignTop | Qt::AlignLeft);

  hlayout->addStretch();

  /////

  m_cloudStatus = new QPushButton;
  m_cloudStatus->setFixedSize(QSize(192,47));
  QString style;
  style.append("QPushButton {"
               "  background-image:url(':/shared_gui_components/images/manage_all_button.png');"
               "  border:none;"
               "}");
  m_cloudStatus->setStyleSheet(style);
  layout->addWidget(m_cloudStatus,0,Qt::AlignTop | Qt::AlignLeft);
  bool isConnected = connect(m_cloudStatus, SIGNAL(clicked(bool)),
                             this, SLOT(on_cloudStatus(bool)));
  OS_ASSERT(isConnected);

  /////

  label = new QLabel;
  label->setObjectName("H2");
  label->setWordWrap(true);
  label->setFixedWidth(TEXT_WIDTH);
  label->setText(WARNING);
  layout->addWidget(label,0,Qt::AlignTop | Qt::AlignLeft);

  /////

  layout->addStretch();

  /////

  bool success = false;
  success = m_awsProvider.requestEstimatedCharges();
  OS_ASSERT(success);
  success = m_awsProvider.requestTotalInstances();
  OS_ASSERT(success);
  
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

  #ifdef Q_WS_MAC
    setWindowFlags(Qt::FramelessWindowHint);
  #else
    setWindowFlags(Qt::WindowCloseButtonHint);
  #endif
}

//// SLOTS

void  MonitorUseDialog::updateData()
{
  QString temp;

  temp = temp.setNum(m_awsProvider.estimatedCharges(), 'f', 2);
  temp.prepend('$');
  m_billingCharge->setText(temp);

  m_totalNumInstances->setText(temp.setNum(m_awsProvider.totalInstances()));

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

void  MonitorUseDialog::on_cloudStatus(bool checked)
{
  QDesktopServices::openUrl(QUrl("http://aws.amazon.com/console"));
}

void  MonitorUseDialog::on_estimatedChargesAvailable()
{
  m_estimatedChargesAvailable = true;
  if(m_estimatedChargesAvailable && m_totalInstancesAvailable){
    m_timer->start(30000);
    updateData();
  }
}

void  MonitorUseDialog::on_totalInstancesAvailable()
{
  m_totalInstancesAvailable = true;
  if(m_estimatedChargesAvailable && m_totalInstancesAvailable){
    m_timer->start(30000);
    updateData();
  }
}

} // pat

} // openstudio
