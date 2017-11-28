/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
      m_scheduleTypeComboBox->addItem(name, toQString(tmp.handle()));
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

  UUID handle = toUUID(m_scheduleTypeComboBox->itemData(index).toString());
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

