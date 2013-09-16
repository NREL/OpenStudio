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

// TODO is VagrantProvider required?
#include <utilities/cloud/AWSProvider.hpp>
#include <utilities/cloud/AWSProvider_Impl.hpp>
#include <utilities/cloud/CloudProvider.hpp>
#include <utilities/cloud/CloudProvider_Impl.hpp>
#include <utilities/cloud/VagrantProvider.hpp>
#include <utilities/cloud/VagrantProvider_Impl.hpp>
#include <utilities/core/Assert.hpp>

#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

#define BILLING_CHARGE "Estimated Monthly Billing Charge: $"
#define MINUTES " minutes"
#define TIME_RUNNING "Total Time Running: "
#define NUM_INSTANCES "Instances Running on Current Project: "
#define TOTAL_NUM_INSTANCES "Total Instances Running: " 
#define CLOUD_OFF "Cloud Off for All Projects"
#define WARNING "Stopping the cloud will terminate all instances.  Any detailed results not selected for download will be lost."
#define TEXT_WIDTH 300
#define STATUS_WIDTH 150

namespace openstudio {

MonitorUseDialog::MonitorUseDialog(QWidget* parent)
  : OSDialog(false, parent)
{
  this->setWindowTitle("Monitor Use"); // TODO not showing
  this->setSizeHint(QSize(350,300));
  this->cancelButton()->hide();
  createWidgets();
  updateData();
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

  m_cloudStatus = new QLabel;
  m_cloudStatus->setText(CLOUD_OFF);
  m_cloudStatus->setFixedWidth(STATUS_WIDTH);
  m_cloudStatus->setStyleSheet("QLabel {color: orange;}");
  // TODO m_cloudStatus->setPixmap(QPixmap(":/images/???.png"));
  layout->addWidget(m_cloudStatus,0,Qt::AlignTop | Qt::AlignLeft);

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

  QTimer * timer = new QTimer(this);
  timer->start(10000);
  bool isConnected = connect(timer, SIGNAL(timeout()),
                             this, SLOT(updateData()));
  OS_ASSERT(isConnected);
}

//// SLOTS

void  MonitorUseDialog::updateData()
{
  //VagrantProvider vagrantProvider;
  //AWSProvider awsProvider;

  QString text("N/A");

  m_billingCharge->setText(text);

  QString temp(text);
  temp += MINUTES; 
  m_timeRunning->setText(temp);

  m_numInstances->setText(text);

  m_totalNumInstances->setText(text);

  // TODO m_cloudStatus->setText(text);
}

} // openstudio
