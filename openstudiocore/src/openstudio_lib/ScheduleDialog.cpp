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

#include "ScheduleDialog.hpp"
#include "SchedulesTabController.hpp"

#include "../model/ScheduleRuleset.hpp"
#include "../model/ScheduleTypeLimits.hpp"
#include "../model/ScheduleDay.hpp"

#include "../utilities/units/OSOptionalQuantity.hpp"
#include "../utilities/units/Quantity.hpp"
#include "../utilities/time/Time.hpp"
#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStringList>

namespace openstudio {

ScheduleDialog::ScheduleDialog(bool isIP,
                               const model::Model & model,
                               QWidget * parent)
  : OSDialog(parent),
  m_model(model),
  m_scheduleTypes(std::vector<model::ScheduleType>()),
  m_className(NULL),
  m_scheduleType(NULL),
  m_scheduleUnits(NULL),
  m_lowerLimit(NULL),
  m_upperLimit(NULL)
{
  createLayout();
}

void ScheduleDialog::createLayout()
{
  okButton()->setText("Apply");

  QLabel * label = NULL;

  label = new QLabel("Define New Schedule",this);
  label->setObjectName("H1");
  upperLayout()->addWidget(label);

  QVBoxLayout * vertLayout = new QVBoxLayout();
  vertLayout->setContentsMargins(20,10,10,10);
  vertLayout->setSpacing(20);
  upperLayout()->addLayout(vertLayout);

  QHBoxLayout * hLayout = NULL;

  QVBoxLayout * vLayout = NULL;

  bool isConnected = false;

  //CLASS NAME
  {
    vLayout = new QVBoxLayout();
    vLayout->setSpacing(5);

    label = new QLabel("Class Name",this);
    label->setObjectName("H2");
    vLayout->addWidget(label);

    hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(20,0,10,0);
    hLayout->setSpacing(5);

    m_className = new QComboBox(this);
    m_className->setObjectName("ScheduleDialog");
    isConnected = connect(m_className, SIGNAL(currentIndexChanged(const QString &)),
                          this, SLOT(on_classNameComboBox(const QString &)));
    OS_ASSERT(isConnected);
    isConnected = connect(m_className, SIGNAL(currentIndexChanged(const QString &)),
                          this, SIGNAL(classNameCurrentIndexChanged(const QString &)));
    OS_ASSERT(isConnected);
    hLayout->addWidget(m_className);
    hLayout->addStretch();

    std::vector<std::string> classNames = model::ScheduleTypeRegistry::instance().classNames();
    for (std::string className : classNames){
      m_className->addItem(className.c_str());
    }
    hLayout->addWidget(m_className);
    hLayout->addStretch();
   
    vLayout->addLayout(hLayout);

    vertLayout->addLayout(vLayout);
  }

  // SCHEDULE TYPE
  {
    vLayout = new QVBoxLayout();
    vLayout->setSpacing(5);

    label = new QLabel("Schedule Type",this);
    label->setObjectName("H2");
    vLayout->addWidget(label);

    hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(20,0,10,0);
    hLayout->setSpacing(5);

    m_scheduleType = new QComboBox(this);
    m_scheduleType->setObjectName("ScheduleDialog");
    isConnected = connect(m_scheduleType, SIGNAL(currentIndexChanged(int)),
                          this, SLOT(on_scheduleTypeComboBox(int)));
    OS_ASSERT(isConnected);
    isConnected = connect(m_scheduleType, SIGNAL(currentIndexChanged(const QString &)),
                          this, SIGNAL(scheduleTypeCurrentIndexChanged(const QString &)));
    OS_ASSERT(isConnected);
    hLayout->addWidget(m_scheduleType);
    hLayout->addStretch();
   
    vLayout->addLayout(hLayout);

    vertLayout->addLayout(vLayout);
  }

  // SCHEDULE UNITS
  {
    vLayout = new QVBoxLayout();
    vLayout->setSpacing(5);

    label = new QLabel("Schedule Units",this);
    label->setObjectName("H2");
    vLayout->addWidget(label);

    hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(20,0,10,0);
    hLayout->setSpacing(5);

    m_scheduleUnits = new QLineEdit(this);
    m_scheduleUnits->setObjectName("ScheduleDialog");
    m_scheduleUnits->setReadOnly(true);
    hLayout->addWidget(m_scheduleUnits);
    hLayout->addStretch();

    vLayout->addLayout(hLayout);

    vertLayout->addLayout(vLayout);
  }

  // SCHEDULE RANGE
  {
    vLayout = new QVBoxLayout();
    vLayout->setSpacing(5);

    label = new QLabel("Schedule Range",this);
    label->setObjectName("H2");
    vLayout->addWidget(label);

    hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(20,0,10,0);
    hLayout->setSpacing(20);

    QVBoxLayout * vertLimitLayout = NULL;

    {
      vertLimitLayout = new QVBoxLayout();
      vertLimitLayout->setSpacing(5);
      
      // UPPER LIMIT
      label = new QLabel("Upper Limit",this);
      label->setObjectName("H2");
      vertLimitLayout->addWidget(label);

      m_upperLimit = new QLineEdit(this);
      m_upperLimit->setReadOnly(true);
      m_upperLimit->setObjectName("ScheduleDialogShort");
      vertLimitLayout->addWidget(m_upperLimit);

      vertLimitLayout->addSpacing(10);

      // LOWER LIMIT
      label = new QLabel("Lower Limit",this);
      label->setObjectName("H2");
      vertLimitLayout->addWidget(label);

      m_lowerLimit = new QLineEdit(this);
      m_lowerLimit->setReadOnly(true);
      m_lowerLimit->setObjectName("ScheduleDialogShort");
      vertLimitLayout->addWidget(m_lowerLimit);
    }

    hLayout->addLayout(vertLimitLayout);

    hLayout->addStretch();

    vLayout->addLayout(hLayout);

    vertLayout->addLayout(vLayout);
  }

  vertLayout->addStretch();

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        this, SLOT(toggleUnits(bool)));
  OS_ASSERT(isConnected);

  m_className->setCurrentIndex(1); // TODO awkward way to load all 3 comboBoxes
  m_className->setCurrentIndex(0);
}

void ScheduleDialog::initScheduleRange()
{
  int index = m_scheduleType->currentIndex();

  if(index < 0) return;

  QString standardString;
  QString text;
  const double dummy = -999.99;
  double value;
  
  value = dummy;
  OSOptionalQuantity upperOptQuant = m_scheduleTypes[index].getUpperLimitValue(m_isIP);
  if(upperOptQuant.isSet()){
    standardString = upperOptQuant.get().units().standardString().c_str();
    value = upperOptQuant.get().value();
    if(value != dummy){
      text.setNum(value,'f',6);
    }
    if(standardString.length() > 0){
      text += " ";
      text += standardString;
    }
  }
  m_upperLimit->setText(text);

  standardString = "";
  text = "";
  value = dummy;
  OSOptionalQuantity lowerOptQuant = m_scheduleTypes[index].getLowerLimitValue(m_isIP);
  if(lowerOptQuant.isSet()){
    standardString = lowerOptQuant.get().units().standardString().c_str();
    value = lowerOptQuant.get().value();
    if(value != dummy){
      text.setNum(value,'f',6);
    }
    if(standardString.length() > 0){
      text += " ";
      text += standardString;
    }
  }
  m_lowerLimit->setText(text);
}

// ***** SLOTS *****

void ScheduleDialog::on_okButton(bool checked)
{
  // create a schedule
  int index = m_scheduleType->currentIndex();

  model::ScheduleTypeLimits scheduleTypeLimits = model::ScheduleTypeRegistry::instance().getOrCreateScheduleTypeLimits(m_scheduleTypes.at(index), m_model);

  model::ScheduleRuleset schedule(m_model);
  bool ok = schedule.setScheduleTypeLimits(scheduleTypeLimits);
  OS_ASSERT(ok);
  model::ScheduleDay daySchedule = schedule.defaultDaySchedule();
  daySchedule.addValue(Time(1,0),SchedulesTabController::defaultStartingValue(daySchedule));

  OSDialog::on_okButton(checked);
}

void ScheduleDialog::on_classNameComboBox(const QString & text)
{
  if(text.size() == 0) return;

  if(!m_scheduleType) return;

  // Load schedule types combo
  m_scheduleTypes = model::ScheduleTypeRegistry::instance().getScheduleTypesByClassName(text.toStdString());

  m_scheduleType->clear();

  for (model::ScheduleType scheduletype : m_scheduleTypes){
    m_scheduleType->addItem(scheduletype.scheduleDisplayName.c_str());
  }
}

void ScheduleDialog::on_scheduleTypeComboBox(int index)
{
  if(index < 0) return;

  if(!m_scheduleUnits) return;

  m_scheduleUnits->setText(m_scheduleTypes[index].unitType.c_str());

  initScheduleRange();

  // ETH@20121004 - The other combo box would have a blank option corresponding to using 
  // getOrCreateScheduleTypeLimits, and then would have a list of the names of the ScheduleTypeLimits
  // returned by:
  //
  // getCompatibleScheduleTypeLimits(m_model,m_scheduleTypes[index].className,m_scheduleTypes[index].scheduleDisplayName);
  //
  // If one of those names was selected when the user hit okay, then scheduleTypeLimits would just
  // get set to the ScheduleTypeLimits at the index in that list.
}

// ***** SLOTS *****

void ScheduleDialog::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
  initScheduleRange();
}

} // openstudio

