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

#include "WaterUseEquipmentInspectorView.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "OSDropZone.hpp"
#include "ModelObjectItem.hpp"
#include "../model/WaterUseEquipmentDefinition.hpp"
#include "../model/WaterUseEquipmentDefinition_Impl.hpp"
#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"
#include <utilities/idd/OS_WaterUse_Equipment_Definition_FieldEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGridLayout>
#include <QScrollArea>
#include <QStackedWidget>

namespace openstudio {

// TargetTemperatureScheduleVC

boost::optional<model::WaterUseEquipmentDefinition> TargetTemperatureScheduleVC::waterUseEquipmentDefinition() const
{
  boost::optional<model::WaterUseEquipmentDefinition> result;

  if( m_modelObject )
  {
    result = m_modelObject->optionalCast<model::WaterUseEquipmentDefinition>();
  }

  return result;
}

void TargetTemperatureScheduleVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_WaterUse_Equipment_DefinitionFields::TargetTemperatureScheduleName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> TargetTemperatureScheduleVC::makeVector()
{
  std::vector<OSItemId> result;
  if ( boost::optional<model::WaterUseEquipmentDefinition> waterUseEquipmentDefinition = this->waterUseEquipmentDefinition() ){
    boost::optional<model::Schedule> schedule = waterUseEquipmentDefinition->targetTemperatureSchedule();
    if (schedule){
      result.push_back(modelObjectToItemId(*schedule, false));
    }
  }
  return result;
}

void TargetTemperatureScheduleVC::onRemoveItem(OSItem* item)
{
  if ( boost::optional<model::WaterUseEquipmentDefinition> waterUseEquipmentDefinition = this->waterUseEquipmentDefinition() ){
    waterUseEquipmentDefinition->resetTargetTemperatureSchedule();
  }
}

void TargetTemperatureScheduleVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void TargetTemperatureScheduleVC::onDrop(const OSItemId& itemId)
{
  if ( boost::optional<model::WaterUseEquipmentDefinition> waterUseEquipmentDefinition = this->waterUseEquipmentDefinition() ){
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::Schedule>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        waterUseEquipmentDefinition->setTargetTemperatureSchedule(modelObject->cast<model::Schedule>());
      }
    }
  }
}

// SensibleFractionScheduleVC

boost::optional<model::WaterUseEquipmentDefinition> SensibleFractionScheduleVC::waterUseEquipmentDefinition() const
{
  boost::optional<model::WaterUseEquipmentDefinition> result;

  if( m_modelObject )
  {
    result = m_modelObject->optionalCast<model::WaterUseEquipmentDefinition>();
  }

  return result;
}

void SensibleFractionScheduleVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_WaterUse_Equipment_DefinitionFields::SensibleFractionScheduleName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> SensibleFractionScheduleVC::makeVector()
{
  std::vector<OSItemId> result;
  if ( boost::optional<model::WaterUseEquipmentDefinition> waterUseEquipmentDefinition = this->waterUseEquipmentDefinition() ){
    boost::optional<model::Schedule> schedule = waterUseEquipmentDefinition->sensibleFractionSchedule();
    if (schedule){
      result.push_back(modelObjectToItemId(*schedule, false));
    }
  }
  return result;
}

void SensibleFractionScheduleVC::onRemoveItem(OSItem* item)
{
  if ( boost::optional<model::WaterUseEquipmentDefinition> waterUseEquipmentDefinition = this->waterUseEquipmentDefinition() ){
    waterUseEquipmentDefinition->resetSensibleFractionSchedule();
  }
}

void SensibleFractionScheduleVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void SensibleFractionScheduleVC::onDrop(const OSItemId& itemId)
{
  if ( boost::optional<model::WaterUseEquipmentDefinition> waterUseEquipmentDefinition = this->waterUseEquipmentDefinition() ){
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::Schedule>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        waterUseEquipmentDefinition->setSensibleFractionSchedule(modelObject->cast<model::Schedule>());
      }
    }
  }
}

// LatentFractionScheduleVC

boost::optional<model::WaterUseEquipmentDefinition> LatentFractionScheduleVC::waterUseEquipmentDefinition() const
{
  boost::optional<model::WaterUseEquipmentDefinition> result;

  if( m_modelObject )
  {
    result = m_modelObject->optionalCast<model::WaterUseEquipmentDefinition>();
  }

  return result;
}

void LatentFractionScheduleVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_WaterUse_Equipment_DefinitionFields::LatentFractionScheduleName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> LatentFractionScheduleVC::makeVector()
{
  std::vector<OSItemId> result;
  if ( boost::optional<model::WaterUseEquipmentDefinition> waterUseEquipmentDefinition = this->waterUseEquipmentDefinition() ){
    boost::optional<model::Schedule> schedule = waterUseEquipmentDefinition->latentFractionSchedule();
    if (schedule){
      result.push_back(modelObjectToItemId(*schedule, false));
    }
  }
  return result;
}

void LatentFractionScheduleVC::onRemoveItem(OSItem* item)
{
  if ( boost::optional<model::WaterUseEquipmentDefinition> waterUseEquipmentDefinition = this->waterUseEquipmentDefinition() ){
    waterUseEquipmentDefinition->resetLatentFractionSchedule();
  }
}

void LatentFractionScheduleVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void LatentFractionScheduleVC::onDrop(const OSItemId& itemId)
{
  if ( boost::optional<model::WaterUseEquipmentDefinition> waterUseEquipmentDefinition = this->waterUseEquipmentDefinition() ){
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::Schedule>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        waterUseEquipmentDefinition->setLatentFractionSchedule(modelObject->cast<model::Schedule>());
      }
    }
  }
}

WaterUseEquipmentDefinitionInspectorView::WaterUseEquipmentDefinitionInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent)
{
  m_isIP = isIP;

  auto hiddenWidget = new QWidget();
  this->stackedWidget()->addWidget(hiddenWidget);

  auto visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  auto mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7,7,7,7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  // Name

  QLabel* label = new QLabel("Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,0,0);

  m_nameEdit = new OSLineEdit2();
  mainGridLayout->addWidget(m_nameEdit,1,0,1,3);

  // End Use Subcategory

  label = new QLabel("End Use Subcategory: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,2,0);

  m_endUseSubcategoryEdit = new OSLineEdit2();
  mainGridLayout->addWidget(m_endUseSubcategoryEdit,3,0,1,3);

  // Peak Flow Rate

  label = new QLabel("Peak Flow Rate: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,4,0);

  m_peakFlowRateEdit = new OSQuantityEdit2("m^3/s","m^3/s","gal/min", m_isIP);
  connect(this, &WaterUseEquipmentDefinitionInspectorView::toggleUnitsClicked, m_peakFlowRateEdit, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_peakFlowRateEdit,5,0,1,3);

  // Target Temperature Schedule

  label = new QLabel("Target Temperature Schedule: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,6,0);

  m_targetTemperatureScheduleVC = new TargetTemperatureScheduleVC();
  m_targetTemperatureScheduleDZ = new OSDropZone(m_targetTemperatureScheduleVC);
  m_targetTemperatureScheduleDZ->setMaxItems(1);
  mainGridLayout->addWidget(m_targetTemperatureScheduleDZ,7,0,1,3);

  // Sensible Fraction Schedule

  label = new QLabel("Sensible Fraction Schedule: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,8,0);

  m_sensibleFractionScheduleVC = new SensibleFractionScheduleVC();
  m_sensibleFractionScheduleDZ = new OSDropZone(m_sensibleFractionScheduleVC);
  m_sensibleFractionScheduleDZ->setMaxItems(1);
  mainGridLayout->addWidget(m_sensibleFractionScheduleDZ,9,0,1,3);

  // Latent Fraction Schedule

  label = new QLabel("Latent Fraction Schedule: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,10,0);

  m_latentFractionScheduleVC = new LatentFractionScheduleVC();
  m_latentFractionScheduleDZ = new OSDropZone(m_latentFractionScheduleVC);
  m_latentFractionScheduleDZ->setMaxItems(1);
  mainGridLayout->addWidget(m_latentFractionScheduleDZ,11,0,1,3);

  // Stretch

  mainGridLayout->setRowStretch(12,100);

  mainGridLayout->setColumnStretch(3,100);
}

void WaterUseEquipmentDefinitionInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void WaterUseEquipmentDefinitionInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::WaterUseEquipmentDefinition waterUseEquipmentDefinition = modelObject.cast<model::WaterUseEquipmentDefinition>();
  attach(waterUseEquipmentDefinition);
  refresh();
}

void WaterUseEquipmentDefinitionInspectorView::onUpdate()
{
  refresh();
}

void WaterUseEquipmentDefinitionInspectorView::attach(openstudio::model::WaterUseEquipmentDefinition & waterUseEquipmentDefinition)
{
  // m_nameEdit->bind(waterUseEquipmentDefinition,"name");
  m_waterUseEquipmentDefinition = waterUseEquipmentDefinition;
  m_nameEdit->bind(
    *m_waterUseEquipmentDefinition,
    OptionalStringGetter(std::bind(&model::WaterUseEquipmentDefinition::name, m_waterUseEquipmentDefinition.get_ptr(),true)),
    boost::optional<StringSetter>(std::bind(&model::WaterUseEquipmentDefinition::setName, m_waterUseEquipmentDefinition.get_ptr(),std::placeholders::_1))
  );

  // m_endUseSubcategoryEdit->bind(waterUseEquipmentDefinition,"endUseSubcategory");
  m_endUseSubcategoryEdit->bind(
    *m_waterUseEquipmentDefinition,
    StringGetter(std::bind(&model::WaterUseEquipmentDefinition::endUseSubcategory, m_waterUseEquipmentDefinition.get_ptr())),
    boost::optional<StringSetter>(std::bind(&model::WaterUseEquipmentDefinition::setEndUseSubcategory, m_waterUseEquipmentDefinition.get_ptr(),std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::WaterUseEquipmentDefinition::resetEndUseSubcategory, m_waterUseEquipmentDefinition.get_ptr())),
    boost::optional<BasicQuery>(std::bind(&model::WaterUseEquipmentDefinition::isEndUseSubcategoryDefaulted, m_waterUseEquipmentDefinition.get_ptr()))
  );

  // m_peakFlowRateEdit->bind(waterUseEquipmentDefinition,"peakFlowRate",m_isIP);
  m_peakFlowRateEdit->bind(
    m_isIP,
    *m_waterUseEquipmentDefinition,
    DoubleGetter(std::bind(&model::WaterUseEquipmentDefinition::peakFlowRate, m_waterUseEquipmentDefinition.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::WaterUseEquipmentDefinition::*)(double)>(&model::WaterUseEquipmentDefinition::setPeakFlowRate), m_waterUseEquipmentDefinition.get_ptr(), std::placeholders::_1))
  );

  m_targetTemperatureScheduleVC->attach(waterUseEquipmentDefinition);
  m_targetTemperatureScheduleVC->reportItems();

  m_latentFractionScheduleVC->attach(waterUseEquipmentDefinition);
  m_latentFractionScheduleVC->reportItems();

  m_sensibleFractionScheduleVC->attach(waterUseEquipmentDefinition);
  m_sensibleFractionScheduleVC->reportItems();

  this->stackedWidget()->setCurrentIndex(1);
}

void WaterUseEquipmentDefinitionInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_endUseSubcategoryEdit->unbind();
  m_peakFlowRateEdit->unbind();

  m_waterUseEquipmentDefinition = boost::none;

  m_targetTemperatureScheduleVC->detach();
  m_targetTemperatureScheduleVC->reportItems();

  m_latentFractionScheduleVC->detach();
  m_latentFractionScheduleVC->reportItems();

  m_sensibleFractionScheduleVC->detach();
  m_sensibleFractionScheduleVC->reportItems();
}

void WaterUseEquipmentDefinitionInspectorView::refresh()
{
}

void WaterUseEquipmentDefinitionInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio

