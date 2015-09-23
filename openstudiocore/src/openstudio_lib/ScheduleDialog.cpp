/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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
#include "../model/ScheduleTypeLimits_Impl.hpp"
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

struct ScheduleTypeCompare {
  bool operator() (const model::ScheduleType& lhs, const model::ScheduleType& rhs) const{
    // only compare the fields that matter
    if (lhs.unitType != rhs.unitType){
      return lhs.unitType < rhs.unitType;
    }

    if (lhs.lowerLimitValue && rhs.lowerLimitValue){
      return lhs.lowerLimitValue.get() < rhs.lowerLimitValue.get();
    } else if (lhs.lowerLimitValue){
      return false;
    } else if (rhs.lowerLimitValue){
      return true;
    }

    if (lhs.upperLimitValue && rhs.upperLimitValue){
      return lhs.upperLimitValue.get() < rhs.upperLimitValue.get();
    } else if (lhs.upperLimitValue){
      return false;
    } else if (rhs.upperLimitValue){
      return true;
    }

    if (lhs.isContinuous && !rhs.isContinuous){
      return false;
    } else if (!lhs.isContinuous && rhs.isContinuous){
      return true;
    }

    return false;
  }
};


struct ScheduleTypeLimitsCompare {
  bool operator() (const model::ScheduleTypeLimits& lhs, const model::ScheduleTypeLimits& rhs) const{
    return lhs.name().get() < rhs.name().get();
  }
};

ScheduleDialog::ScheduleDialog(bool isIP,
                               const model::Model & model,
                               QWidget * parent)
  : OSDialog(parent),
  m_isIP(isIP),
  m_model(model),
  m_scheduleTypeComboBox(nullptr)
{
  setWindowModality(Qt::ApplicationModal);
  createLayout();
}

void ScheduleDialog::setIsIP(bool isIP)
{
  m_isIP = isIP;

  onCurrentIndexChanged(m_scheduleTypeComboBox->currentIndex());
}

void ScheduleDialog::createLayout()
{
  okButton()->setText("Apply");

  // make all possible schedule type limits
  std::set<model::ScheduleType, ScheduleTypeCompare> scheduleTypes;
  for (const std::string& className : model::ScheduleTypeRegistry::instance().classNames()){
    for (const model::ScheduleType& scheduleType : model::ScheduleTypeRegistry::instance().getScheduleTypesByClassName(className)){
      scheduleTypes.insert(scheduleType);
    }
  }

  //std::set<model::ScheduleTypeLimits> scheduleTypeLimitsSet;
  //for (const model::ScheduleType& scheduleType : scheduleTypes){
  //  model::ScheduleTypeLimits tmp = model::ScheduleTypeRegistry::instance().getOrCreateScheduleTypeLimits(scheduleType, m_model);
  //  scheduleTypeLimitsSet.insert(tmp);
  //}

  //std::vector<model::ScheduleTypeLimits> scheduleTypeLimits;

  // DLM: put all schedule types in the model or just the ones found by the registry
  //scheduleTypeLimits = m_model.getConcreteModelObjects<model::ScheduleTypeLimits>();
  //scheduleTypeLimits.insert(scheduleTypeLimits.end(), scheduleTypeLimitsSet.begin(), scheduleTypeLimitsSet.end()); 
  
  //std::sort(scheduleTypeLimits.begin(), scheduleTypeLimits.end(), WorkspaceObjectNameLess());
  //OS_ASSERT(!scheduleTypeLimits.empty());

  QLabel * label = nullptr;

  label = new QLabel("Define New Schedule",this);
  label->setObjectName("H1");
  upperLayout()->addWidget(label);

  auto vertLayout = new QVBoxLayout();
  vertLayout->setContentsMargins(20,10,10,10);
  vertLayout->setSpacing(20);
  upperLayout()->addLayout(vertLayout);

  QHBoxLayout * hLayout = nullptr;

  QVBoxLayout * vLayout = nullptr;

  // SCHEDULE TYPE
  {
    vLayout = new QVBoxLayout();
    vLayout->setSpacing(5);

    label = new QLabel("Schedule Type", this);
    label->setObjectName("H2");
    vLayout->addWidget(label);

    hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 10, 0);
    hLayout->setSpacing(5);

    m_scheduleTypeComboBox = new QComboBox(this);
    m_scheduleTypeComboBox->setObjectName("ScheduleDialog");

    //for (const model::ScheduleTypeLimits& scheduleTypeLimit : scheduleTypeLimits){
    for (const model::ScheduleType& scheduleType : scheduleTypes){
      QString name = toQString(model::ScheduleTypeRegistry::instance().getDefaultName(scheduleType));
      model::ScheduleTypeLimits tmp = model::ScheduleTypeRegistry::instance().getOrCreateScheduleTypeLimits(scheduleType, m_model);
      m_scheduleTypeComboBox->addItem(name, tmp.handle().toString());
    }

    hLayout->addWidget(m_scheduleTypeComboBox);
    hLayout->addStretch();
   
    vLayout->addLayout(hLayout);

    vertLayout->addLayout(vLayout);
  }

  // Numeric Type
  {
    hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 10, 0);
    hLayout->setSpacing(5);

    label = new QLabel("Numeric Type: ", this);
    label->setObjectName("H2");
    hLayout->addWidget(label);

    m_numericTypeLabel = new QLabel(this);
    m_numericTypeLabel->setObjectName("ScheduleDialog");
    hLayout->addWidget(m_numericTypeLabel);
    hLayout->addStretch();

    vertLayout->addLayout(hLayout);
  }

  // Lower Limit
  {
    hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 10, 0);
    hLayout->setSpacing(5);

    label = new QLabel("Lower Limit: ", this);
    label->setObjectName("H2");
    hLayout->addWidget(label);

    m_lowerLimitLabel = new QLabel(this);
    m_lowerLimitLabel->setObjectName("ScheduleDialog");
    hLayout->addWidget(m_lowerLimitLabel);
    hLayout->addStretch();

    vertLayout->addLayout(hLayout);
  }

  // Upper Limit
  {
    hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 10, 0);
    hLayout->setSpacing(5);

    label = new QLabel("Upper Limit: ", this);
    label->setObjectName("H2");
    hLayout->addWidget(label);

    m_upperLimitLabel = new QLabel(this);
    m_upperLimitLabel->setObjectName("ScheduleDialog");
    hLayout->addWidget(m_upperLimitLabel);
    hLayout->addStretch();

    vertLayout->addLayout(hLayout);
  }

  vertLayout->addStretch();

  m_scheduleTypeComboBox->setCurrentIndex(0);
  onCurrentIndexChanged(0);

  connect(m_scheduleTypeComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &ScheduleDialog::onCurrentIndexChanged);

}

void ScheduleDialog::onCurrentIndexChanged(int index)
{
  OS_ASSERT(index >= 0);

  UUID handle = UUID(m_scheduleTypeComboBox->itemData(index).toString());
  m_scheduleTypeLimits = m_model.getModelObject<model::ScheduleTypeLimits>(handle);
  OS_ASSERT(m_scheduleTypeLimits);

  boost::optional<Unit> units = m_scheduleTypeLimits->units(m_isIP);
  QString unitsLabel;
  if (units){
    QString temp;
    if (!units->prettyString().empty()){
      temp = toQString(units->prettyString());
    } else if (!units->standardString().empty()){
      temp = toQString(units->standardString());
    }
    
    if (temp.isEmpty()){
      unitsLabel.append(" (");
      unitsLabel.append("unitless");
      unitsLabel.append(")");
    } else{
      unitsLabel.append(" (");
      unitsLabel.append(temp);
      unitsLabel.append(")");
    }
  }

  boost::optional<std::string> numericType = m_scheduleTypeLimits->numericType();

  QString numericTypeLabel;;
  if (numericType){
    numericTypeLabel.append(toQString(*numericType));
  } else{
    numericTypeLabel.append("None");
  }
  numericTypeLabel.append(unitsLabel);
  m_numericTypeLabel->setText(numericTypeLabel);

  OSOptionalQuantity lowerLimit = m_scheduleTypeLimits->getLowerLimitValue(m_isIP);
  QString lowerLimitLabel;
  if (lowerLimit.isSet()){
    lowerLimitLabel.append(QString::number(lowerLimit.get().value()));
    lowerLimitLabel.append(unitsLabel);
  } else{
    lowerLimitLabel.append("None");
  }
  m_lowerLimitLabel->setText(lowerLimitLabel);

  OSOptionalQuantity upperLimit = m_scheduleTypeLimits->getUpperLimitValue(m_isIP);
  QString upperLimitLabel;
  if (upperLimit.isSet()){
    upperLimitLabel.append(QString::number(upperLimit.get().value()));
    upperLimitLabel.append(unitsLabel);
  } else{
    upperLimitLabel.append("None");
  }
  m_upperLimitLabel->setText(upperLimitLabel);
}

void ScheduleDialog::on_okButton(bool checked)
{
  OS_ASSERT(m_scheduleTypeLimits);

  model::ScheduleRuleset schedule(m_model);
  bool ok = schedule.setScheduleTypeLimits(*m_scheduleTypeLimits);
  OS_ASSERT(ok);

  model::ScheduleDay daySchedule = schedule.defaultDaySchedule();
  daySchedule.addValue(Time(1,0),SchedulesTabController::defaultStartingValue(daySchedule));

  OSDialog::on_okButton(checked);
}


} // openstudio

