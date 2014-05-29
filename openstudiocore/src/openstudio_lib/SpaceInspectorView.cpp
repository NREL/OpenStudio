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

#include "SpaceInspectorView.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSSwitch.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "OSVectorController.hpp"
#include "OSDropZone.hpp"
#include "ModelObjectItem.hpp"
#include "SpaceLoadInstancesWidget.hpp"

#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/SpaceType.hpp"
#include "../model/SpaceType_Impl.hpp"
#include "../model/BuildingStory.hpp"
#include "../model/BuildingStory_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/DefaultConstructionSet.hpp"
#include "../model/DefaultConstructionSet_Impl.hpp"
#include "../model/DefaultScheduleSet.hpp"
#include "../model/DefaultScheduleSet_Impl.hpp"
#include "../model/DesignSpecificationOutdoorAir.hpp"
#include "../model/DesignSpecificationOutdoorAir_Impl.hpp"
#include "../model/SpaceInfiltrationDesignFlowRate.hpp"
#include "../model/SpaceInfiltrationDesignFlowRate_Impl.hpp"
#include "../model/SpaceInfiltrationEffectiveLeakageArea.hpp"
#include "../model/SpaceInfiltrationEffectiveLeakageArea_Impl.hpp"
#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"
#include "../model/Component.hpp"
#include "../model/Component_Impl.hpp"
#include "../model/ComponentData.hpp"
#include "../model/ComponentData_Impl.hpp"

#include <utilities/idd/OS_Space_FieldEnums.hxx>
#include <utilities/idd/OS_SpaceType_FieldEnums.hxx>
#include <utilities/idd/OS_BuildingStory_FieldEnums.hxx>
#include <utilities/idd/OS_ThermalZone_FieldEnums.hxx>
#include <utilities/idd/OS_DefaultConstructionSet_FieldEnums.hxx>
#include <utilities/idd/OS_DefaultScheduleSet_FieldEnums.hxx>
#include <utilities/idd/OS_DesignSpecification_OutdoorAir_FieldEnums.hxx>
#include <utilities/idd/OS_SpaceInfiltration_DesignFlowRate_FieldEnums.hxx>
#include <utilities/idd/OS_SpaceInfiltration_EffectiveLeakageArea_FieldEnums.hxx>
#include <utilities/idd/OS_Building_FieldEnums.hxx>
#include "../utilities/core/Assert.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>
#include <QColor>
#include <QGridLayout>
#include <QScrollArea>
#include <QStackedWidget>

namespace openstudio {

// SpaceBuildingStoryVectorController

void SpaceBuildingStoryVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_SpaceFields::BuildingStoryName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> SpaceBuildingStoryVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    boost::optional<model::BuildingStory> buildingStory = space.buildingStory();
    if (buildingStory){
      result.push_back(modelObjectToItemId(*buildingStory, false));
    }
  }
  return result;
}

void SpaceBuildingStoryVectorController::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    space.resetBuildingStory();
  }
}

void SpaceBuildingStoryVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void SpaceBuildingStoryVectorController::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::BuildingStory>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        model::BuildingStory buildingStory = modelObject->cast<model::BuildingStory>();
        space.setBuildingStory(buildingStory);
      }
    }
  }
}

// SpaceThermalZoneVectorController

void SpaceThermalZoneVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_SpaceFields::ThermalZoneName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> SpaceThermalZoneVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    boost::optional<model::ThermalZone> thermalZone = space.thermalZone();
    if (thermalZone){
      result.push_back(modelObjectToItemId(*thermalZone, false));
    }
  }
  return result;
}

void SpaceThermalZoneVectorController::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    space.resetThermalZone();
  }
}

void SpaceThermalZoneVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void SpaceThermalZoneVectorController::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::ThermalZone>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        model::ThermalZone thermalZone = modelObject->cast<model::ThermalZone>();
        space.setThermalZone(thermalZone);
      }
    }
  }
}

// SpaceSpaceTypeVectorController

void SpaceSpaceTypeVectorController::attach(const model::ModelObject& modelObject)
{
  ModelObjectVectorController::attach(modelObject);

  model::Building building = modelObject.model().getUniqueModelObject<model::Building>();
  attachOtherModelObject(building);
}

void SpaceSpaceTypeVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (modelObject.optionalCast<model::Space>()){
    if (index == OS_SpaceFields::SpaceTypeName){
      emit itemIds(makeVector());
    }
  }else if (modelObject.optionalCast<model::Building>()){
    if (index == OS_BuildingFields::SpaceTypeName){
      emit itemIds(makeVector());
    }
  }
}

std::vector<OSItemId> SpaceSpaceTypeVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    boost::optional<model::SpaceType> spaceType = space.spaceType();
    if (spaceType){
      bool isDefaulted = space.isSpaceTypeDefaulted();
      OSItemId itemId = modelObjectToItemId(*spaceType, isDefaulted);
      result.push_back(itemId);
    }
  }
  return result;
}

void SpaceSpaceTypeVectorController::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    space.resetSpaceType();
  }
}

void SpaceSpaceTypeVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void SpaceSpaceTypeVectorController::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::SpaceType>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        space.setSpaceType(modelObject->cast<model::SpaceType>());
      }
    }else{
      boost::optional<model::Component> component = this->getComponent(itemId);
      if (component){
        if (component->primaryObject().optionalCast<model::SpaceType>()){
          boost::optional<model::ComponentData> componentData = m_modelObject->model().insertComponent(*component);
          if (componentData){
            if (componentData->primaryComponentObject().optionalCast<model::SpaceType>()){
              space.setSpaceType(componentData->primaryComponentObject().cast<model::SpaceType>());
            }
          }
        }
      }
    }
  }
}

// SpaceDefaultConstructionSetVectorController

void SpaceDefaultConstructionSetVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_SpaceFields::DefaultConstructionSetName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> SpaceDefaultConstructionSetVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    boost::optional<model::DefaultConstructionSet> defaultConstructionSet = space.defaultConstructionSet();
    if (defaultConstructionSet){
      result.push_back(modelObjectToItemId(*defaultConstructionSet, false));
    }
  }
  return result;
}

void SpaceDefaultConstructionSetVectorController::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    space.resetDefaultConstructionSet();
  }
}

void SpaceDefaultConstructionSetVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void SpaceDefaultConstructionSetVectorController::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::DefaultConstructionSet>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        space.setDefaultConstructionSet(modelObject->cast<model::DefaultConstructionSet>());
      }
    }
  }
}

// SpaceDefaultScheduleSetVectorController

void SpaceDefaultScheduleSetVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_SpaceFields::DefaultScheduleSetName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> SpaceDefaultScheduleSetVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    boost::optional<model::DefaultScheduleSet> defaultScheduleSet = space.defaultScheduleSet();
    if (defaultScheduleSet){
      result.push_back(modelObjectToItemId(*defaultScheduleSet, false));
    }
  }
  return result;
}

void SpaceDefaultScheduleSetVectorController::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    space.resetDefaultScheduleSet();
  }
}

void SpaceDefaultScheduleSetVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void SpaceDefaultScheduleSetVectorController::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::DefaultScheduleSet>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        space.setDefaultScheduleSet(modelObject->cast<model::DefaultScheduleSet>());
      }
    }
  }
}

// SpaceDesignSpecificationOutdoorAirVectorController

void SpaceDesignSpecificationOutdoorAirVectorController::attach(const model::ModelObject& modelObject)
{
  ModelObjectVectorController::attach(modelObject);

  model::Space space = m_modelObject->cast<model::Space>();
  attachOtherModelObjects(space);
}

void SpaceDesignSpecificationOutdoorAirVectorController::attachOtherModelObjects(const model::Space& space)
{
  if (space.spaceType() && !space.isSpaceTypeDefaulted()){
    attachOtherModelObject(space.spaceType().get());
  }

  model::Building building = space.model().getUniqueModelObject<model::Building>();
  attachOtherModelObject(building);

  if (building.spaceType()){
    attachOtherModelObject(building.spaceType().get());
  }
}

void SpaceDesignSpecificationOutdoorAirVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (modelObject.optionalCast<model::Space>()){
    if (index == OS_SpaceFields::DesignSpecificationOutdoorAirObjectName){
      emit itemIds(makeVector());
    }else if (index == OS_SpaceFields::SpaceTypeName){
      detachOtherModelObjects();
      model::Space space = m_modelObject->cast<model::Space>();
      attachOtherModelObjects(space);
      emit itemIds(makeVector());
    }
  }else if (modelObject.optionalCast<model::Building>()){
    if (index == OS_BuildingFields::SpaceTypeName){
      detachOtherModelObjects();
      model::Space space = m_modelObject->cast<model::Space>();
      attachOtherModelObjects(space);
      emit itemIds(makeVector());
    }
  }else if (modelObject.optionalCast<model::SpaceType>()){
    if (index == OS_SpaceTypeFields::DesignSpecificationOutdoorAirObjectName){
      emit itemIds(makeVector());
    }
  }
}

std::vector<OSItemId> SpaceDesignSpecificationOutdoorAirVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    boost::optional<model::DesignSpecificationOutdoorAir> designSpecificationOutdoorAir = space.designSpecificationOutdoorAir();
    if (designSpecificationOutdoorAir){
      bool isDefaulted = space.isDesignSpecificationOutdoorAirDefaulted();
      OSItemId itemId = modelObjectToItemId(*designSpecificationOutdoorAir, isDefaulted);
      result.push_back(itemId);
    }
  }
  return result;
}

void SpaceDesignSpecificationOutdoorAirVectorController::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    space.resetDesignSpecificationOutdoorAir();
  }
}

void SpaceDesignSpecificationOutdoorAirVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void SpaceDesignSpecificationOutdoorAirVectorController::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::DesignSpecificationOutdoorAir>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        space.setDesignSpecificationOutdoorAir(modelObject->cast<model::DesignSpecificationOutdoorAir>());
      }
    }
  }
}

void SpaceDesignSpecificationOutdoorAirVectorController::onMakeNewItem()
{
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    model::DesignSpecificationOutdoorAir designSpecificationOutdoorAir(space.model());
    space.setDesignSpecificationOutdoorAir(designSpecificationOutdoorAir);
  }
}

// SpaceSpaceInfiltrationDesignFlowRateVectorController

void SpaceSpaceInfiltrationDesignFlowRateVectorController::attach(const model::ModelObject& modelObject)
{
  ModelObjectVectorController::attach(modelObject);
  for (const model::SpaceInfiltrationDesignFlowRate& spaceInfiltrationDesignFlowRate : modelObject.model().getConcreteModelObjects<model::SpaceInfiltrationDesignFlowRate>()){
    attachOtherModelObject(spaceInfiltrationDesignFlowRate);
  }
}

void SpaceSpaceInfiltrationDesignFlowRateVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (modelObject.optionalCast<model::SpaceInfiltrationDesignFlowRate>()){
    if (index == OS_SpaceInfiltration_DesignFlowRateFields::SpaceorSpaceTypeName){
      emit itemIds(makeVector());
    }
  }
}

std::vector<OSItemId> SpaceSpaceInfiltrationDesignFlowRateVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();

    // inherited
    if (space.spaceType()){
      for (const model::SpaceInfiltrationDesignFlowRate& spaceInfiltrationDesignFlowRate : space.spaceType()->spaceInfiltrationDesignFlowRates()){
        OSItemId itemId = modelObjectToItemId(spaceInfiltrationDesignFlowRate, true);
        result.push_back(itemId);
      }
    }

    // direct
    for (const model::SpaceInfiltrationDesignFlowRate& spaceInfiltrationDesignFlowRate : space.spaceInfiltrationDesignFlowRates()){
      OSItemId itemId = modelObjectToItemId(spaceInfiltrationDesignFlowRate, false);
      result.push_back(itemId);
    }
  }
  return result;
}

void SpaceSpaceInfiltrationDesignFlowRateVectorController::onObjectAdded(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  if (modelObject.optionalCast<model::SpaceInfiltrationDesignFlowRate>()){
    attachOtherModelObject(modelObject);
    emit itemIds(makeVector());
  }
}

void SpaceSpaceInfiltrationDesignFlowRateVectorController::onObjectRemoved(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  if (modelObject.optionalCast<model::SpaceInfiltrationDesignFlowRate>()){
    detachOtherModelObject(modelObject);
    emit itemIds(makeVector());
  }
}

void SpaceSpaceInfiltrationDesignFlowRateVectorController::onRemoveItem(OSItem* item)
{
  ModelObjectItem* modelObjectItem = qobject_cast<ModelObjectItem*>(item);
  if (modelObjectItem){
    model::ModelObject modelObject = modelObjectItem->modelObject();
    if (modelObject.optionalCast<model::SpaceInfiltrationDesignFlowRate>()){
      modelObject.cast<model::SpaceInfiltrationDesignFlowRate>().remove();
    }
  }
}

void SpaceSpaceInfiltrationDesignFlowRateVectorController::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::SpaceInfiltrationDesignFlowRate>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        modelObject->cast<model::SpaceInfiltrationDesignFlowRate>().setSpace(space);
      }
    }
  }
}

void SpaceSpaceInfiltrationDesignFlowRateVectorController::onMakeNewItem()
{
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    model::SpaceInfiltrationDesignFlowRate spaceInfiltrationDesignFlowRate(space.model());
    spaceInfiltrationDesignFlowRate.setSpace(space);
  }
}

// SpaceSpaceInfiltrationEffectiveLeakageAreaVectorController

void SpaceSpaceInfiltrationEffectiveLeakageAreaVectorController::attach(const model::ModelObject& modelObject)
{
  ModelObjectVectorController::attach(modelObject);
  for (const model::SpaceInfiltrationEffectiveLeakageArea& spaceInfiltrationEffectiveLeakageArea : modelObject.model().getConcreteModelObjects<model::SpaceInfiltrationEffectiveLeakageArea>()){
    attachOtherModelObject(spaceInfiltrationEffectiveLeakageArea);
  }
}

void SpaceSpaceInfiltrationEffectiveLeakageAreaVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (modelObject.optionalCast<model::SpaceInfiltrationEffectiveLeakageArea>()){
    if (index == OS_SpaceInfiltration_EffectiveLeakageAreaFields::SpaceorSpaceTypeName){
      emit itemIds(makeVector());
    }
  }
}

std::vector<OSItemId> SpaceSpaceInfiltrationEffectiveLeakageAreaVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();

    // inherited
    if (space.spaceType()){
      for (const model::SpaceInfiltrationEffectiveLeakageArea& spaceInfiltrationEffectiveLeakageArea : space.spaceType()->spaceInfiltrationEffectiveLeakageAreas()){
        OSItemId itemId = modelObjectToItemId(spaceInfiltrationEffectiveLeakageArea, true);
        result.push_back(itemId);
      }
    }

    // direct
    for (const model::SpaceInfiltrationEffectiveLeakageArea& spaceInfiltrationEffectiveLeakageArea : space.spaceInfiltrationEffectiveLeakageAreas()){
      OSItemId itemId = modelObjectToItemId(spaceInfiltrationEffectiveLeakageArea, false);
      result.push_back(itemId);
    }
  }
  return result;
}

void SpaceSpaceInfiltrationEffectiveLeakageAreaVectorController::onObjectAdded(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  if (modelObject.optionalCast<model::SpaceInfiltrationEffectiveLeakageArea>()){
    attachOtherModelObject(modelObject);
    emit itemIds(makeVector());
  }
}

void SpaceSpaceInfiltrationEffectiveLeakageAreaVectorController::onObjectRemoved(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  if (modelObject.optionalCast<model::SpaceInfiltrationEffectiveLeakageArea>()){
    detachOtherModelObject(modelObject);
    emit itemIds(makeVector());
  }
}

void SpaceSpaceInfiltrationEffectiveLeakageAreaVectorController::onRemoveItem(OSItem* item)
{
  ModelObjectItem* modelObjectItem = qobject_cast<ModelObjectItem*>(item);
  if (modelObjectItem){
    model::ModelObject modelObject = modelObjectItem->modelObject();
    if (modelObject.optionalCast<model::SpaceInfiltrationEffectiveLeakageArea>()){
      modelObject.cast<model::SpaceInfiltrationEffectiveLeakageArea>().remove();
    }
  }
}

void SpaceSpaceInfiltrationEffectiveLeakageAreaVectorController::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::SpaceInfiltrationEffectiveLeakageArea>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        modelObject->cast<model::SpaceInfiltrationEffectiveLeakageArea>().setSpace(space);
      }
    }
  }
}

void SpaceSpaceInfiltrationEffectiveLeakageAreaVectorController::onMakeNewItem()
{
  if (m_modelObject){
    model::Space space = m_modelObject->cast<model::Space>();
    model::SpaceInfiltrationEffectiveLeakageArea spaceInfiltrationEffectiveLeakageArea(space.model());
    spaceInfiltrationEffectiveLeakageArea.setSpace(space);
  }
}

// SpaceInspectorView

SpaceInspectorView::SpaceInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent )
  : ModelObjectInspectorView(model, true, parent)
{
  m_isIP = isIP;

  QWidget* hiddenWidget = new QWidget();
  this->stackedWidget()->insertWidget(0, hiddenWidget);

  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->insertWidget(1, visibleWidget);

  this->stackedWidget()->setCurrentIndex(0);

  QGridLayout* mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7,7,7,7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  // name
  QVBoxLayout* vLayout = new QVBoxLayout();

  QLabel* label = new QLabel();
  label->setText("Name: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_nameEdit = new OSLineEdit();
  vLayout->addWidget(m_nameEdit);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout,0,0,1,2, Qt::AlignTop|Qt::AlignLeft);

  // building story and thermal zone
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Building Story: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_buildingStoryVectorController = new SpaceBuildingStoryVectorController();
  m_buildingStoryDropZone = new OSDropZone(m_buildingStoryVectorController);
  m_buildingStoryDropZone->setMinItems(0);
  m_buildingStoryDropZone->setMaxItems(1);
  m_buildingStoryDropZone->setItemsAcceptDrops(true);
  vLayout->addWidget(m_buildingStoryDropZone);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout,1,0);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Thermal Zone: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_thermalZoneVectorController = new SpaceThermalZoneVectorController();
  m_thermalZoneDropZone = new OSDropZone(m_thermalZoneVectorController);
  m_thermalZoneDropZone->setMinItems(0);
  m_thermalZoneDropZone->setMaxItems(1);
  m_thermalZoneDropZone->setItemsAcceptDrops(true);
  vLayout->addWidget(m_thermalZoneDropZone);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout,1,1);

  // space type
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Space Type: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_spaceTypeVectorController = new SpaceSpaceTypeVectorController();
  m_spaceTypeDropZone = new OSDropZone(m_spaceTypeVectorController);
  m_spaceTypeDropZone->setMinItems(0);
  m_spaceTypeDropZone->setMaxItems(1);
  m_spaceTypeDropZone->setItemsAcceptDrops(true);
  vLayout->addWidget(m_spaceTypeDropZone);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout,2,0,1,2);

  // default construction and schedule sets
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Default Construction Set: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_defaultConstructionSetVectorController = new SpaceDefaultConstructionSetVectorController();
  m_defaultConstructionSetDropZone = new OSDropZone(m_defaultConstructionSetVectorController);
  m_defaultConstructionSetDropZone->setMinItems(0);
  m_defaultConstructionSetDropZone->setMaxItems(1);
  m_defaultConstructionSetDropZone->setItemsAcceptDrops(true);
  vLayout->addWidget(m_defaultConstructionSetDropZone);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout,3,0);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Default Schedule Set: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_defaultScheduleSetVectorController = new SpaceDefaultScheduleSetVectorController();
  m_defaultScheduleSetDropZone = new OSDropZone(m_defaultScheduleSetVectorController);
  m_defaultScheduleSetDropZone->setMinItems(0);
  m_defaultScheduleSetDropZone->setMaxItems(1);
  m_defaultScheduleSetDropZone->setItemsAcceptDrops(true);
  vLayout->addWidget(m_defaultScheduleSetDropZone);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout,3,1);

  // design oa
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Design Specification Outdoor Air: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_designSpecificationOutdoorAirVectorController = new SpaceDesignSpecificationOutdoorAirVectorController();
  m_designSpecificationOutdoorAirDropZone = new OSDropZone(m_designSpecificationOutdoorAirVectorController);
  m_designSpecificationOutdoorAirDropZone->setMinItems(0);
  m_designSpecificationOutdoorAirDropZone->setMaxItems(1);
  m_designSpecificationOutdoorAirDropZone->setItemsAcceptDrops(true);  
  m_designSpecificationOutdoorAirDropZone->showAddButton();
  vLayout->addWidget(m_designSpecificationOutdoorAirDropZone);

  bool isConnected = false;
  isConnected = connect(m_designSpecificationOutdoorAirDropZone, 
                             SIGNAL(itemClicked(OSItem*)),
                             this, 
                             SIGNAL(dropZoneItemClicked(OSItem*)));
  OS_ASSERT(isConnected);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout,4,0,1,2);

  // space infiltration design flow rate
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Space Infiltration Design Flow Rates: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_spaceInfiltrationDesignFlowRateVectorController = new SpaceSpaceInfiltrationDesignFlowRateVectorController();
  m_spaceInfiltrationDesignFlowRateDropZone = new OSDropZone(m_spaceInfiltrationDesignFlowRateVectorController);
  m_spaceInfiltrationDesignFlowRateDropZone->setMinItems(0);
  //m_spaceInfiltrationDesignFlowRateDropZone->setMaxItems(1);
  m_spaceInfiltrationDesignFlowRateDropZone->setItemsAcceptDrops(false);   
  m_spaceInfiltrationDesignFlowRateDropZone->showAddButton();
  vLayout->addWidget(m_spaceInfiltrationDesignFlowRateDropZone);

  isConnected = connect(m_spaceInfiltrationDesignFlowRateDropZone, 
                        SIGNAL(itemClicked(OSItem*)),
                        this, 
                        SIGNAL(dropZoneItemClicked(OSItem*)));
  OS_ASSERT(isConnected);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout,5,0,1,2);

  // space infiltration effective leakage area
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Space Infiltration Effective Leakage Areas: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_spaceInfiltrationEffectiveLeakageAreaVectorController = new SpaceSpaceInfiltrationEffectiveLeakageAreaVectorController();
  m_spaceInfiltrationEffectiveLeakageAreaDropZone = new OSDropZone(m_spaceInfiltrationEffectiveLeakageAreaVectorController);
  m_spaceInfiltrationEffectiveLeakageAreaDropZone->setMinItems(0);
  //m_spaceInfiltrationEffectiveLeakageAreaDropZone->setMaxItems(1);
  m_spaceInfiltrationEffectiveLeakageAreaDropZone->setItemsAcceptDrops(false);   
  m_spaceInfiltrationEffectiveLeakageAreaDropZone->showAddButton();
  vLayout->addWidget(m_spaceInfiltrationEffectiveLeakageAreaDropZone);

  isConnected = connect(m_spaceInfiltrationEffectiveLeakageAreaDropZone, 
                        SIGNAL(itemClicked(OSItem*)),
                        this, 
                        SIGNAL(dropZoneItemClicked(OSItem*)));
  OS_ASSERT(isConnected);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout,6,0,1,2);

  // direction of relative north and part of floor area
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Direction of Relative North: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_directionofRelativeNorthEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_directionofRelativeNorthEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_directionofRelativeNorthEdit);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout,7,0);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Part of total floor area: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_partofTotalFloorAreaSwitch = new OSSwitch();
  vLayout->addWidget(m_partofTotalFloorAreaSwitch);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout,7,1);

  // x, y, and z origin
  QHBoxLayout* hLayout = new QHBoxLayout();

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("X Origin: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_xOriginEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_xOriginEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_xOriginEdit);

  hLayout->addLayout(vLayout);

  vLayout->addStretch();

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Y Origin: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_yOriginEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_yOriginEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_yOriginEdit);

  vLayout->addStretch();

  hLayout->addLayout(vLayout);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Z Origin: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_zOriginEdit = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_zOriginEdit, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  vLayout->addWidget(m_zOriginEdit);

  vLayout->addStretch();

  hLayout->addLayout(vLayout);

  mainGridLayout->addLayout(hLayout,8,0,1,2);

  // loads
  m_spaceLoadInstancesWidget = new SpaceLoadInstancesWidget();
  mainGridLayout->addWidget(m_spaceLoadInstancesWidget,9,0,1,2);

  mainGridLayout->setColumnMinimumWidth(0, 80);
  mainGridLayout->setColumnMinimumWidth(1, 80);
  mainGridLayout->setColumnStretch(2,1);
  mainGridLayout->setRowMinimumHeight(0, 30);
  mainGridLayout->setRowMinimumHeight(1, 30);
  mainGridLayout->setRowMinimumHeight(2, 30);
  mainGridLayout->setRowMinimumHeight(3, 30);
  mainGridLayout->setRowMinimumHeight(4, 30);
  mainGridLayout->setRowMinimumHeight(5, 30);
  mainGridLayout->setRowMinimumHeight(6, 30);
  mainGridLayout->setRowMinimumHeight(7, 30);
  mainGridLayout->setRowMinimumHeight(8, 30);
  mainGridLayout->setRowStretch(9,1);
}

void SpaceInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void SpaceInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::Space space = modelObject.cast<model::Space>();
  attach(space);
  refresh();
}

void SpaceInspectorView::onUpdate()
{
  refresh();
}

void SpaceInspectorView::attach(openstudio::model::Space& space)
{
  m_nameEdit->bind(space, "name");
        
  m_buildingStoryVectorController->attach(space);
  m_buildingStoryVectorController->reportItems();

  m_thermalZoneVectorController->attach(space);
  m_thermalZoneVectorController->reportItems();

  m_spaceTypeVectorController->attach(space);
  m_spaceTypeVectorController->reportItems();

  m_defaultConstructionSetVectorController->attach(space);
  m_defaultConstructionSetVectorController->reportItems();

  m_defaultScheduleSetVectorController->attach(space);
  m_defaultScheduleSetVectorController->reportItems();

  m_designSpecificationOutdoorAirVectorController->attach(space);
  m_designSpecificationOutdoorAirVectorController->reportItems();

  m_spaceInfiltrationDesignFlowRateVectorController->attach(space);
  m_spaceInfiltrationDesignFlowRateVectorController->reportItems();

  m_spaceInfiltrationEffectiveLeakageAreaVectorController->attach(space);
  m_spaceInfiltrationEffectiveLeakageAreaVectorController->reportItems();

  m_directionofRelativeNorthEdit->bind(space, "directionofRelativeNorth", m_isIP, std::string("isDirectionofRelativeNorthDefaulted"));
  m_partofTotalFloorAreaSwitch->bind(space, "partofTotalFloorArea");
  
  m_xOriginEdit->bind(space, "xOrigin", m_isIP, std::string("isXOriginDefaulted"));
  m_yOriginEdit->bind(space, "yOrigin", m_isIP, std::string("isYOriginDefaulted"));
  m_zOriginEdit->bind(space, "zOrigin", m_isIP, std::string("isZOriginDefaulted"));

  m_spaceLoadInstancesWidget->attach(space);

  this->stackedWidget()->setCurrentIndex(1);
}

void SpaceInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_buildingStoryVectorController->detach();
  m_thermalZoneVectorController->detach();
  m_spaceTypeVectorController->detach();
  m_defaultConstructionSetVectorController->detach();
  m_defaultScheduleSetVectorController->detach();
  m_designSpecificationOutdoorAirVectorController->detach();
  m_spaceInfiltrationDesignFlowRateVectorController->detach();
  m_spaceInfiltrationEffectiveLeakageAreaVectorController->detach();
  m_directionofRelativeNorthEdit->unbind();
  m_partofTotalFloorAreaSwitch->unbind();
  m_xOriginEdit->unbind();
  m_yOriginEdit->unbind();
  m_zOriginEdit->unbind();
  m_spaceLoadInstancesWidget->detach();
}

void SpaceInspectorView::refresh()
{
}

void SpaceInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio
