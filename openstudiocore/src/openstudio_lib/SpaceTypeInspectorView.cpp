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

#include "SpaceTypeInspectorView.hpp"
#include "ModelObjectItem.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "OSVectorController.hpp"
#include "OSDropZone.hpp"
#include "SpaceLoadInstancesWidget.hpp"
#include "RenderingColorWidget.hpp"

#include "../model/SpaceType.hpp"
#include "../model/SpaceType_Impl.hpp"
#include "../model/RenderingColor.hpp"
#include "../model/SpaceInfiltrationDesignFlowRate.hpp"
#include "../model/SpaceInfiltrationDesignFlowRate_Impl.hpp"
#include "../model/SpaceInfiltrationEffectiveLeakageArea.hpp"
#include "../model/SpaceInfiltrationEffectiveLeakageArea_Impl.hpp"
#include "../model/DesignSpecificationOutdoorAir.hpp"
#include "../model/DesignSpecificationOutdoorAir_Impl.hpp"
#include "../model/DefaultConstructionSet.hpp"
#include "../model/DefaultConstructionSet_Impl.hpp"
#include "../model/DefaultScheduleSet.hpp"
#include "../model/DefaultScheduleSet_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"


#include <utilities/idd/OS_SpaceType_FieldEnums.hxx>
#include <utilities/idd/OS_Building_FieldEnums.hxx>
#include <utilities/idd/OS_Space_FieldEnums.hxx>
#include <utilities/idd/OS_SpaceInfiltration_DesignFlowRate_FieldEnums.hxx>
#include <utilities/idd/OS_SpaceInfiltration_EffectiveLeakageArea_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QScrollArea>
#include <QStackedWidget>
#include <QComboBox>

namespace openstudio {

// SpaceTypeDefaultConstructionSetVectorController

void SpaceTypeDefaultConstructionSetVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_SpaceTypeFields::DefaultConstructionSetName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> SpaceTypeDefaultConstructionSetVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::SpaceType spaceType = m_modelObject->cast<model::SpaceType>();
    boost::optional<model::DefaultConstructionSet> defaultConstructionSet = spaceType.defaultConstructionSet();
    if (defaultConstructionSet){
      result.push_back(modelObjectToItemId(*defaultConstructionSet, false));
    }
  }
  return result;
}

void SpaceTypeDefaultConstructionSetVectorController::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::SpaceType spaceType = m_modelObject->cast<model::SpaceType>();
    spaceType.resetDefaultConstructionSet();
  }
}

void SpaceTypeDefaultConstructionSetVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void SpaceTypeDefaultConstructionSetVectorController::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::SpaceType spaceType = m_modelObject->cast<model::SpaceType>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::DefaultConstructionSet>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        spaceType.setDefaultConstructionSet(modelObject->cast<model::DefaultConstructionSet>());
      }
    }
  }
}

// SpaceTypeDefaultScheduleSetVectorController

void SpaceTypeDefaultScheduleSetVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_SpaceTypeFields::DefaultScheduleSetName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> SpaceTypeDefaultScheduleSetVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::SpaceType spaceType = m_modelObject->cast<model::SpaceType>();
    boost::optional<model::DefaultScheduleSet> defaultScheduleSet = spaceType.defaultScheduleSet();
    if (defaultScheduleSet){
      result.push_back(modelObjectToItemId(*defaultScheduleSet, false));
    }
  }
  return result;
}

void SpaceTypeDefaultScheduleSetVectorController::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::SpaceType spaceType = m_modelObject->cast<model::SpaceType>();
    spaceType.resetDefaultScheduleSet();
  }
}

void SpaceTypeDefaultScheduleSetVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void SpaceTypeDefaultScheduleSetVectorController::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::SpaceType spaceType = m_modelObject->cast<model::SpaceType>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::DefaultScheduleSet>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        spaceType.setDefaultScheduleSet(modelObject->cast<model::DefaultScheduleSet>());
      }
    }
  }
}

// SpaceTypeDesignSpecificationOutdoorAirVectorController

void SpaceTypeDesignSpecificationOutdoorAirVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_SpaceTypeFields::DesignSpecificationOutdoorAirObjectName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> SpaceTypeDesignSpecificationOutdoorAirVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::SpaceType spaceType = m_modelObject->cast<model::SpaceType>();
    boost::optional<model::DesignSpecificationOutdoorAir> designSpecificationOutdoorAir = spaceType.designSpecificationOutdoorAir();
    if (designSpecificationOutdoorAir){
      result.push_back(modelObjectToItemId(*designSpecificationOutdoorAir, false));
    }
  }
  return result;
}

void SpaceTypeDesignSpecificationOutdoorAirVectorController::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::SpaceType spaceType = m_modelObject->cast<model::SpaceType>();
    spaceType.resetDesignSpecificationOutdoorAir();
  }
}

void SpaceTypeDesignSpecificationOutdoorAirVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void SpaceTypeDesignSpecificationOutdoorAirVectorController::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::SpaceType spaceType = m_modelObject->cast<model::SpaceType>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::DesignSpecificationOutdoorAir>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        spaceType.setDesignSpecificationOutdoorAir(modelObject->cast<model::DesignSpecificationOutdoorAir>());
      }
    }
  }
}

void SpaceTypeDesignSpecificationOutdoorAirVectorController::onMakeNewItem()
{
  if (m_modelObject){
    model::SpaceType spaceType = m_modelObject->cast<model::SpaceType>();
    model::DesignSpecificationOutdoorAir designSpecificationOutdoorAir(spaceType.model());
    spaceType.setDesignSpecificationOutdoorAir(designSpecificationOutdoorAir);
  }
}

// SpaceTypeSpaceInfiltrationDesignFlowRateVectorController

void SpaceTypeSpaceInfiltrationDesignFlowRateVectorController::attach(const model::ModelObject& modelObject)
{
  ModelObjectVectorController::attach(modelObject);
  for (const model::SpaceInfiltrationDesignFlowRate& spaceInfiltrationDesignFlowRate : modelObject.model().getConcreteModelObjects<model::SpaceInfiltrationDesignFlowRate>()){
    attachOtherModelObject(spaceInfiltrationDesignFlowRate);
  }
}

void SpaceTypeSpaceInfiltrationDesignFlowRateVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (modelObject.optionalCast<model::SpaceInfiltrationDesignFlowRate>()){
    if (index == OS_SpaceInfiltration_DesignFlowRateFields::SpaceorSpaceTypeName){
      emit itemIds(makeVector());
    }
  }
}

std::vector<OSItemId> SpaceTypeSpaceInfiltrationDesignFlowRateVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::SpaceType spaceType = m_modelObject->cast<model::SpaceType>();
    for (const model::SpaceInfiltrationDesignFlowRate& spaceInfiltrationDesignFlowRate : spaceType.spaceInfiltrationDesignFlowRates()){
      result.push_back(modelObjectToItemId(spaceInfiltrationDesignFlowRate, false));
    }
  }
  return result;
}

void SpaceTypeSpaceInfiltrationDesignFlowRateVectorController::onObjectAdded(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  if (modelObject.optionalCast<model::SpaceInfiltrationDesignFlowRate>()){
    attachOtherModelObject(modelObject);
    emit itemIds(makeVector());
  }
}

void SpaceTypeSpaceInfiltrationDesignFlowRateVectorController::onObjectRemoved(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  if (modelObject.optionalCast<model::SpaceInfiltrationDesignFlowRate>()){
    emit itemIds(makeVector());
  }
}

void SpaceTypeSpaceInfiltrationDesignFlowRateVectorController::onRemoveItem(OSItem* item)
{
  ModelObjectItem* modelObjectItem = qobject_cast<ModelObjectItem*>(item);
  if (modelObjectItem){
    model::ModelObject modelObject = modelObjectItem->modelObject();
    if (modelObject.optionalCast<model::SpaceInfiltrationDesignFlowRate>() && ! modelObject.handle().isNull()){
      modelObject.cast<model::SpaceInfiltrationDesignFlowRate>().remove();
    }
  }
}

void SpaceTypeSpaceInfiltrationDesignFlowRateVectorController::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::SpaceType spaceType = m_modelObject->cast<model::SpaceType>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::SpaceInfiltrationDesignFlowRate>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        modelObject->cast<model::SpaceInfiltrationDesignFlowRate>().setSpaceType(spaceType);
      }
    }
  }
}

void SpaceTypeSpaceInfiltrationDesignFlowRateVectorController::onMakeNewItem()
{
  if (m_modelObject){
    model::SpaceType spaceType = m_modelObject->cast<model::SpaceType>();
    model::SpaceInfiltrationDesignFlowRate spaceInfiltrationDesignFlowRate(spaceType.model());
    spaceInfiltrationDesignFlowRate.setSpaceType(spaceType);
  }
}

// SpaceTypeSpaceInfiltrationEffectiveLeakageAreaVectorController

void SpaceTypeSpaceInfiltrationEffectiveLeakageAreaVectorController::attach(const model::ModelObject& modelObject)
{
  ModelObjectVectorController::attach(modelObject);
  for (const model::SpaceInfiltrationEffectiveLeakageArea& spaceInfiltrationEffectiveLeakageArea : modelObject.model().getConcreteModelObjects<model::SpaceInfiltrationEffectiveLeakageArea>()){
    attachOtherModelObject(spaceInfiltrationEffectiveLeakageArea);
  }
}

void SpaceTypeSpaceInfiltrationEffectiveLeakageAreaVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (modelObject.optionalCast<model::SpaceInfiltrationEffectiveLeakageArea>()){
    if (index == OS_SpaceInfiltration_EffectiveLeakageAreaFields::SpaceorSpaceTypeName){
      emit itemIds(makeVector());
    }
  }
}

std::vector<OSItemId> SpaceTypeSpaceInfiltrationEffectiveLeakageAreaVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::SpaceType spaceType = m_modelObject->cast<model::SpaceType>();
    for (const model::SpaceInfiltrationEffectiveLeakageArea& spaceInfiltrationEffectiveLeakageArea : spaceType.spaceInfiltrationEffectiveLeakageAreas()){
      result.push_back(modelObjectToItemId(spaceInfiltrationEffectiveLeakageArea, false));
    }
  }
  return result;
}

void SpaceTypeSpaceInfiltrationEffectiveLeakageAreaVectorController::onObjectAdded(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  if (modelObject.optionalCast<model::SpaceInfiltrationEffectiveLeakageArea>()){
    attachOtherModelObject(modelObject);
    emit itemIds(makeVector());
  }
}

void SpaceTypeSpaceInfiltrationEffectiveLeakageAreaVectorController::onObjectRemoved(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  if (modelObject.optionalCast<model::SpaceInfiltrationEffectiveLeakageArea>()){
    emit itemIds(makeVector());
  }
}

void SpaceTypeSpaceInfiltrationEffectiveLeakageAreaVectorController::onRemoveItem(OSItem* item)
{
  ModelObjectItem* modelObjectItem = qobject_cast<ModelObjectItem*>(item);
  if (modelObjectItem){
    model::ModelObject modelObject = modelObjectItem->modelObject();
    if (modelObject.optionalCast<model::SpaceInfiltrationEffectiveLeakageArea>() && ! modelObject.handle().isNull()){
      modelObject.cast<model::SpaceInfiltrationEffectiveLeakageArea>().remove();
    }
  }
}

void SpaceTypeSpaceInfiltrationEffectiveLeakageAreaVectorController::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::SpaceType spaceType = m_modelObject->cast<model::SpaceType>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::SpaceInfiltrationEffectiveLeakageArea>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        modelObject->cast<model::SpaceInfiltrationEffectiveLeakageArea>().setSpaceType(spaceType);
      }
    }
  }
}

void SpaceTypeSpaceInfiltrationEffectiveLeakageAreaVectorController::onMakeNewItem()
{
  if (m_modelObject){
    model::SpaceType spaceType = m_modelObject->cast<model::SpaceType>();
    model::SpaceInfiltrationEffectiveLeakageArea spaceInfiltrationEffectiveLeakageArea(spaceType.model());
    spaceInfiltrationEffectiveLeakageArea.setSpaceType(spaceType);
  }
}

// SpaceTypeSpacesVectorController

void SpaceTypeSpacesVectorController::attach(const model::ModelObject& modelObject)
{
  ModelObjectVectorController::attach(modelObject);
  for (const model::Space& space : modelObject.model().getConcreteModelObjects<model::Space>()){
    attachOtherModelObject(space);
  }
}

std::vector<OSItemId> SpaceTypeSpacesVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::SpaceType spaceType = m_modelObject->cast<model::SpaceType>();
    for (const model::Space& space : spaceType.spaces()){
      bool isDefaulted = space.isSpaceTypeDefaulted();
      OSItemId itemId = modelObjectToItemId(space, isDefaulted);
      result.push_back(itemId);
    }
  }
  return result;
}

void SpaceTypeSpacesVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
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

void SpaceTypeSpacesVectorController::onObjectAdded(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  if (modelObject.optionalCast<model::Space>()){
    attachOtherModelObject(modelObject);
    emit itemIds(makeVector());
  }
}

void SpaceTypeSpacesVectorController::onObjectRemoved(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  if (modelObject.optionalCast<model::Space>()){
    emit itemIds(makeVector());
  }
}

void SpaceTypeSpacesVectorController::onRemoveItem(OSItem* item)
{
  ModelObjectItem* modelObjectItem = qobject_cast<ModelObjectItem*>(item);
  if (modelObjectItem){
    boost::optional<model::Space> space = modelObjectItem->modelObject().optionalCast<model::Space>();
    if (space){
      space->resetSpaceType();
    }
  }
}

void SpaceTypeSpacesVectorController::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::SpaceType spaceType = m_modelObject->cast<model::SpaceType>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::Space>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        modelObject->cast<model::Space>().setSpaceType(spaceType);
      }
    }
  }
}

// SpaceTypeUnassignedSpacesVectorController

void SpaceTypeUnassignedSpacesVectorController::attachModel(const model::Model& model)
{
  ModelObjectVectorController::attachModel(model);
  for (const model::Space& space : model.getConcreteModelObjects<model::Space>()){
    attachOtherModelObject(space);
  }
}

void SpaceTypeUnassignedSpacesVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (modelObject.optionalCast<model::Space>()){
    if (index == OS_SpaceFields::SpaceTypeName){
      emit itemIds(makeVector());
    }
  }
}

std::vector<OSItemId> SpaceTypeUnassignedSpacesVectorController::makeVector()
{
  std::vector<OSItemId> result;
  for (const model::Space& space : m_model->getConcreteModelObjects<model::Space>()){
    if (!space.handle().isNull()){
      if (space.isSpaceTypeDefaulted()){
        result.push_back(modelObjectToItemId(space, false));
      }
    }
  }
  return result;
}

void SpaceTypeUnassignedSpacesVectorController::onObjectAdded(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  if (modelObject.optionalCast<model::Space>()){
    attachOtherModelObject(modelObject);
    emit itemIds(makeVector());
  }
}

void SpaceTypeUnassignedSpacesVectorController::onObjectRemoved(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  if (modelObject.optionalCast<model::Space>()){
    emit itemIds(makeVector());
  }
}

// SpaceTypeInspectorView

SpaceTypeInspectorView::SpaceTypeInspectorView(const openstudio::model::Model& model, QWidget * parent )
  : ModelObjectInspectorView(model, true, parent)
{
  QWidget* hiddenWidget = new QWidget();
  this->stackedWidget()->insertWidget(0, hiddenWidget);

  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->insertWidget(1, visibleWidget);

  this->stackedWidget()->setCurrentIndex(0);

  //QVBoxLayout* mainVLayout = new QVBoxLayout();
  //visibleWidget->setLayout(mainVLayout);

  QGridLayout* mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(10,10,10,10);
  mainGridLayout->setSpacing(10);
  //mainVLayout->addLayout(mainGridLayout);
  visibleWidget->setLayout(mainGridLayout);

  // name
  QVBoxLayout* vLayout = new QVBoxLayout();

  QLabel* label = new QLabel();
  label->setText("Name: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_nameEdit = new OSLineEdit();
  vLayout->addWidget(m_nameEdit);

  int row = 0;

  mainGridLayout->addLayout(vLayout,row,0,1,2, Qt::AlignTop|Qt::AlignLeft);

  ++row;

  // standards info
  QFrame * line;
  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainGridLayout->addWidget(line,row,0,1,2);

  ++row;

  label = new QLabel();
  label->setText("Measure Tags (Optional):");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row,0);

  ++row;

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Standards Building Type: ");
  label->setObjectName("StandardsInfo");
  vLayout->addWidget(label);

  m_standardsBuildingTypeComboBox = new QComboBox();
  m_standardsBuildingTypeComboBox->setEditable(true);
  m_standardsBuildingTypeComboBox->setDuplicatesEnabled(false);
  m_standardsBuildingTypeComboBox->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_standardsBuildingTypeComboBox);

  mainGridLayout->addLayout(vLayout,row,0);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Standards Space Type: ");
  label->setObjectName("StandardsInfo");
  vLayout->addWidget(label);

  m_standardsSpaceTypeComboBox = new QComboBox();
  m_standardsSpaceTypeComboBox->setEditable(true);
  m_standardsSpaceTypeComboBox->setDuplicatesEnabled(false);
  m_standardsSpaceTypeComboBox->setFixedWidth(OSItem::ITEM_WIDTH);
  vLayout->addWidget(m_standardsSpaceTypeComboBox);

  mainGridLayout->addLayout(vLayout,row,1);

  ++row;

  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainGridLayout->addWidget(line,row,0,1,2);

  ++row;

  // default construction and schedule sets
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Default Construction Set: ");
  label->setObjectName("H2");
  vLayout->addWidget(label);

  m_defaultConstructionSetVectorController = new SpaceTypeDefaultConstructionSetVectorController();
  m_defaultConstructionSetDropZone = new OSDropZone(m_defaultConstructionSetVectorController);
  m_defaultConstructionSetDropZone->setMinItems(0);
  m_defaultConstructionSetDropZone->setMaxItems(1);
  m_defaultConstructionSetDropZone->setItemsAcceptDrops(true);
  vLayout->addWidget(m_defaultConstructionSetDropZone);

  mainGridLayout->addLayout(vLayout,row,0);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Default Schedule Set: ");
  label->setObjectName("H2");

  vLayout->addWidget(label);

  m_defaultScheduleSetVectorController = new SpaceTypeDefaultScheduleSetVectorController();
  m_defaultScheduleSetDropZone = new OSDropZone(m_defaultScheduleSetVectorController);
  m_defaultScheduleSetDropZone->setMinItems(0);
  m_defaultScheduleSetDropZone->setMaxItems(1);
  m_defaultScheduleSetDropZone->setItemsAcceptDrops(true);
  vLayout->addWidget(m_defaultScheduleSetDropZone);

  mainGridLayout->addLayout(vLayout,row,1);

  ++row;

  // rendering color
  m_renderingColorWidget = new RenderingColorWidget();
  mainGridLayout->addWidget(m_renderingColorWidget,row,0,1,2, Qt::AlignTop|Qt::AlignLeft);

  ++row;

  // design oa
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Design Specification Outdoor Air: ");
  label->setObjectName("H2");

  vLayout->addWidget(label);

  m_designSpecificationOutdoorAirVectorController = new SpaceTypeDesignSpecificationOutdoorAirVectorController();
  m_designSpecificationOutdoorAirDropZone = new OSDropZone(m_designSpecificationOutdoorAirVectorController);
  m_designSpecificationOutdoorAirDropZone->setMinItems(0);
  m_designSpecificationOutdoorAirDropZone->setMaxItems(1);
  m_designSpecificationOutdoorAirDropZone->setItemsAcceptDrops(true);
  m_designSpecificationOutdoorAirDropZone->showAddButton();
  vLayout->addWidget(m_designSpecificationOutdoorAirDropZone);

  bool isConnected = connect(m_designSpecificationOutdoorAirDropZone, 
                             SIGNAL(itemClicked(OSItem*)),
                             this, 
                             SIGNAL(dropZoneItemClicked(OSItem*)));
  OS_ASSERT(isConnected);

  mainGridLayout->addLayout(vLayout,row,0,1,2);

  ++row;

  // space infiltration design flow rate
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Space Infiltration Design Flow Rates: ");
  label->setObjectName("H2");

  vLayout->addWidget(label);

  m_spaceInfiltrationDesignFlowRateVectorController = new SpaceTypeSpaceInfiltrationDesignFlowRateVectorController();
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

  mainGridLayout->addLayout(vLayout,row,0,1,2);

  ++row;

  // space infiltration effective leakage area
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Space Infiltration Effective Leakage Areas: ");
  label->setObjectName("H2");

  vLayout->addWidget(label);

  m_spaceInfiltrationEffectiveLeakageAreaVectorController = new SpaceTypeSpaceInfiltrationEffectiveLeakageAreaVectorController();
  m_spaceInfiltrationEffectiveLeakageAreaDropZone = new OSDropZone(m_spaceInfiltrationEffectiveLeakageAreaVectorController);
  m_spaceInfiltrationEffectiveLeakageAreaDropZone->setMinItems(0);
  //m_spaceInfiltrationDesignFlowRateDropZone->setMaxItems(1);
  m_spaceInfiltrationEffectiveLeakageAreaDropZone->setItemsAcceptDrops(false);
  m_spaceInfiltrationEffectiveLeakageAreaDropZone->showAddButton();
  vLayout->addWidget(m_spaceInfiltrationEffectiveLeakageAreaDropZone);

  isConnected = connect(m_spaceInfiltrationEffectiveLeakageAreaDropZone, 
                        SIGNAL(itemClicked(OSItem*)),
                        this, 
                        SIGNAL(dropZoneItemClicked(OSItem*)));
  OS_ASSERT(isConnected);

  mainGridLayout->addLayout(vLayout,row,0,1,2);

  ++row;

  // spaces
  //vLayout = new QVBoxLayout();

  //label = new QLabel();
  //label->setText("Spaces: ");
  //label->setStyleSheet("QLabel { font: bold; }");
  //vLayout->addWidget(label);

  //m_spacesVectorController = new SpaceTypeSpacesVectorController();
  //m_spacesDropZone = new OSDropZone(m_spacesVectorController);
  //vLayout->addWidget(m_spacesDropZone);

  //mainGridLayout->addLayout(vLayout,row,0,1,2);

  //++row;

  // loads
  m_spaceLoadInstancesWidget = new SpaceLoadInstancesWidget();
  mainGridLayout->addWidget(m_spaceLoadInstancesWidget,row,0,1,2);

  ++row;

  mainGridLayout->setColumnStretch(2,100);

  //mainGridLayout->setColumnMinimumWidth(0, 100);
  //mainGridLayout->setColumnMinimumWidth(1, 100);
  //mainGridLayout->setColumnStretch(2,1);
  //mainGridLayout->setRowMinimumHeight(0, 30);
  //mainGridLayout->setRowMinimumHeight(1, 30);
  //mainGridLayout->setRowMinimumHeight(2, 30);
  //mainGridLayout->setRowMinimumHeight(3, 30);
  //mainGridLayout->setRowMinimumHeight(4, 30);
  //mainGridLayout->setRowMinimumHeight(5, 30);
  mainGridLayout->setRowStretch(row, 100);
}

void SpaceTypeInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void SpaceTypeInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::SpaceType spaceType = modelObject.cast<model::SpaceType>();
  attach(spaceType);
}

void SpaceTypeInspectorView::onUpdate()
{
}

void SpaceTypeInspectorView::editStandardsBuildingType(const QString & text)
{
  if (m_spaceType){
    std::string standardsBuildingType = toString(text);
    if (standardsBuildingType.empty()){
      m_spaceType->resetStandardsBuildingType();
    }else{
      m_spaceType->setStandardsBuildingType(standardsBuildingType);
    }

    m_spaceType->resetStandardsSpaceType();
    populateStandardsSpaceTypes();
  }
}

void SpaceTypeInspectorView::standardsBuildingTypeChanged(const QString & text)
{
  if (m_spaceType){
    std::string standardsBuildingType = toString(text);
    if (standardsBuildingType.empty()){
      m_spaceType->resetStandardsBuildingType();
    }else{
      m_spaceType->setStandardsBuildingType(standardsBuildingType);
    }
    populateStandardsBuildingTypes();

    m_spaceType->resetStandardsSpaceType();
    populateStandardsSpaceTypes();
  }
}

void SpaceTypeInspectorView::editStandardsSpaceType(const QString & text)
{
  if (m_spaceType){
    std::string standardsSpaceType = toString(text);
    if (standardsSpaceType.empty()){
      m_spaceType->resetStandardsSpaceType();
    }else{
      m_spaceType->setStandardsSpaceType(standardsSpaceType);
    }
  }
}

void SpaceTypeInspectorView::standardsSpaceTypeChanged(const QString & text)
{
  if (m_spaceType){
    std::string standardsSpaceType = toString(text);
    if (standardsSpaceType.empty()){
      m_spaceType->resetStandardsSpaceType();
    }else{
      m_spaceType->setStandardsSpaceType(standardsSpaceType);
    }
    populateStandardsSpaceTypes();
  }
}

void SpaceTypeInspectorView::attach(openstudio::model::SpaceType& spaceType)
{
  m_spaceType = spaceType;

  m_nameEdit->bind(spaceType, "name");

  m_defaultConstructionSetVectorController->attach(spaceType);
  m_defaultConstructionSetVectorController->reportItems();

  m_defaultScheduleSetVectorController->attach(spaceType);
  m_defaultScheduleSetVectorController->reportItems();

  boost::optional<model::RenderingColor> renderingColor = spaceType.renderingColor();
  if (!renderingColor){
    renderingColor = model::RenderingColor(spaceType.model());
    spaceType.setRenderingColor(*renderingColor);
  } 
  m_renderingColorWidget->attach(*renderingColor);

  populateStandardsBuildingTypes();

  populateStandardsSpaceTypes();

  m_designSpecificationOutdoorAirVectorController->attach(spaceType);
  m_designSpecificationOutdoorAirVectorController->reportItems();

  m_spaceInfiltrationDesignFlowRateVectorController->attach(spaceType);
  m_spaceInfiltrationDesignFlowRateVectorController->reportItems();

  m_spaceInfiltrationEffectiveLeakageAreaVectorController->attach(spaceType);
  m_spaceInfiltrationEffectiveLeakageAreaVectorController->reportItems();

  //m_spacesVectorController->attach(spaceType);
  //m_spacesVectorController->reportItems();

  m_spaceLoadInstancesWidget->attach(spaceType);

  this->stackedWidget()->setCurrentIndex(1);
}

void SpaceTypeInspectorView::detach()
{
  m_spaceType.reset();

  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();

  m_defaultConstructionSetVectorController->detach();
  m_defaultScheduleSetVectorController->detach();
  m_renderingColorWidget->detach();

  disconnect(m_standardsBuildingTypeComboBox, 0, this, 0);
  m_standardsBuildingTypeComboBox->clear();

  disconnect(m_standardsSpaceTypeComboBox, 0, this, 0);
  m_standardsSpaceTypeComboBox->clear();

  m_designSpecificationOutdoorAirVectorController->detach();
  m_spaceInfiltrationDesignFlowRateVectorController->detach();
  m_spaceInfiltrationEffectiveLeakageAreaVectorController->detach();
  //m_spacesVectorController->detach();
  m_spaceLoadInstancesWidget->detach();
}

void SpaceTypeInspectorView::populateStandardsBuildingTypes()
{
  disconnect(m_standardsBuildingTypeComboBox, 0, this, 0);

  m_standardsBuildingTypeComboBox->clear();
  if (m_spaceType){
    m_standardsBuildingTypeComboBox->addItem("");
    std::vector<std::string> suggestedStandardsBuildingTypes = m_spaceType->suggestedStandardsBuildingTypes();
    for (const std::string& standardsBuildingType : suggestedStandardsBuildingTypes) {
      m_standardsBuildingTypeComboBox->addItem(toQString(standardsBuildingType));
    }
    boost::optional<std::string> standardsBuildingType = m_spaceType->standardsBuildingType();
    if (standardsBuildingType){
      OS_ASSERT(!suggestedStandardsBuildingTypes.empty());
      m_standardsBuildingTypeComboBox->setCurrentIndex(1);
    }else{
      m_standardsBuildingTypeComboBox->setCurrentIndex(0);
    }
  }

  bool isConnected = false;
  isConnected = connect(m_standardsBuildingTypeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(standardsBuildingTypeChanged(const QString&)));
  OS_ASSERT(isConnected);
  isConnected = connect(m_standardsBuildingTypeComboBox, SIGNAL(editTextChanged(const QString&)), this, SLOT(editStandardsBuildingType(const QString&)));
  OS_ASSERT(isConnected);
}

void SpaceTypeInspectorView::populateStandardsSpaceTypes()
{
  disconnect(m_standardsSpaceTypeComboBox, 0, this, 0);

  m_standardsSpaceTypeComboBox->clear();
  if (m_spaceType){
    m_standardsSpaceTypeComboBox->addItem("");
    std::vector<std::string> suggestedStandardsSpaceTypes = m_spaceType->suggestedStandardsSpaceTypes();
    for (const std::string& standardsSpaceType : suggestedStandardsSpaceTypes) {
      m_standardsSpaceTypeComboBox->addItem(toQString(standardsSpaceType));
    }
    boost::optional<std::string> standardsSpaceType = m_spaceType->standardsSpaceType();
    if (standardsSpaceType){
      OS_ASSERT(!suggestedStandardsSpaceTypes.empty());
      m_standardsSpaceTypeComboBox->setCurrentIndex(1);
    }else{
      m_standardsSpaceTypeComboBox->setCurrentIndex(0);
    }
  }

  bool isConnected = false;
  isConnected = connect(m_standardsSpaceTypeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(standardsSpaceTypeChanged(const QString&)));
  OS_ASSERT(isConnected);
  isConnected = connect(m_standardsSpaceTypeComboBox, SIGNAL(editTextChanged(const QString&)), this, SLOT(editStandardsSpaceType(const QString&)));
  OS_ASSERT(isConnected);
}


} // openstudio

