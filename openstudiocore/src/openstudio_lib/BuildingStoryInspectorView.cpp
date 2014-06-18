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

#include "BuildingStoryInspectorView.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "OSDropZone.hpp"
#include "ModelObjectItem.hpp"
#include "RenderingColorWidget.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/BuildingStory_Impl.hpp"
#include "../model/RenderingColor.hpp"
#include "../model/DefaultConstructionSet.hpp"
#include "../model/DefaultConstructionSet_Impl.hpp"
#include "../model/DefaultScheduleSet.hpp"
#include "../model/DefaultScheduleSet_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"

#include <utilities/idd/OS_BuildingStory_FieldEnums.hxx>
#include <utilities/idd/OS_Space_FieldEnums.hxx>
#include "../utilities/core/Assert.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QScrollArea>
#include <QStackedWidget>

namespace openstudio {

// BuildingStoryDefaultConstructionSetVectorController

void BuildingStoryDefaultConstructionSetVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_BuildingStoryFields::DefaultConstructionSetName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> BuildingStoryDefaultConstructionSetVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::BuildingStory buildingStory = m_modelObject->cast<model::BuildingStory>();
    boost::optional<model::DefaultConstructionSet> defaultConstructionSet = buildingStory.defaultConstructionSet();
    if (defaultConstructionSet){
      result.push_back(modelObjectToItemId(*defaultConstructionSet, false));
    }
  }
  return result;
}

void BuildingStoryDefaultConstructionSetVectorController::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::BuildingStory buildingStory = m_modelObject->cast<model::BuildingStory>();
    buildingStory.resetDefaultConstructionSet();
  }
}

void BuildingStoryDefaultConstructionSetVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void BuildingStoryDefaultConstructionSetVectorController::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::BuildingStory buildingStory = m_modelObject->cast<model::BuildingStory>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::DefaultConstructionSet>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        buildingStory.setDefaultConstructionSet(modelObject->cast<model::DefaultConstructionSet>());
      }
    }
  }
}

// BuildingStoryDefaultScheduleSetVectorController

void BuildingStoryDefaultScheduleSetVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_BuildingStoryFields::DefaultScheduleSetName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> BuildingStoryDefaultScheduleSetVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::BuildingStory buildingStory = m_modelObject->cast<model::BuildingStory>();
    boost::optional<model::DefaultScheduleSet> defaultScheduleSet = buildingStory.defaultScheduleSet();
    if (defaultScheduleSet){
      result.push_back(modelObjectToItemId(*defaultScheduleSet, false));
    }
  }
  return result;
}

void BuildingStoryDefaultScheduleSetVectorController::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::BuildingStory buildingStory = m_modelObject->cast<model::BuildingStory>();
    buildingStory.resetDefaultScheduleSet();
  }
}

void BuildingStoryDefaultScheduleSetVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void BuildingStoryDefaultScheduleSetVectorController::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::BuildingStory buildingStory = m_modelObject->cast<model::BuildingStory>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::DefaultScheduleSet>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        buildingStory.setDefaultScheduleSet(modelObject->cast<model::DefaultScheduleSet>());
      }
    }
  }
}

// BuildingStorySpacesVectorController

void BuildingStorySpacesVectorController::attach(const model::ModelObject& modelObject)
{
  ModelObjectVectorController::attach(modelObject);
  for (const model::Space& space : modelObject.model().getConcreteModelObjects<model::Space>()){
    attachOtherModelObject(space);
  }
}

std::vector<OSItemId> BuildingStorySpacesVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::BuildingStory buildingStory = m_modelObject->cast<model::BuildingStory>();
    for (const model::Space& space : buildingStory.spaces()){
      result.push_back(modelObjectToItemId(space, false));
    }
  }
  return result;
}

void BuildingStorySpacesVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (modelObject.optionalCast<model::Space>()){
    if (index == OS_SpaceFields::BuildingStoryName){
      emit itemIds(makeVector());
    }
  }
}

void BuildingStorySpacesVectorController::onObjectAdded(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  if (modelObject.optionalCast<model::Space>()){
    attachOtherModelObject(modelObject);
  }
}

void BuildingStorySpacesVectorController::onObjectRemoved(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  if (modelObject.optionalCast<model::Space>()){
    emit itemIds(makeVector());
  }
}

void BuildingStorySpacesVectorController::onRemoveItem(OSItem* item)
{
  ModelObjectItem* modelObjectItem = qobject_cast<ModelObjectItem*>(item);
  if (modelObjectItem){
    boost::optional<model::Space> space = modelObjectItem->modelObject().optionalCast<model::Space>();
    if (space){
      space->resetBuildingStory();
    }
  }
}

void BuildingStorySpacesVectorController::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::BuildingStory buildingStory = m_modelObject->cast<model::BuildingStory>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::Space>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        modelObject->cast<model::Space>().setBuildingStory(buildingStory);
      }
    }
  }
}

// BuildingStoryUnassignedSpacesVectorController

void BuildingStoryUnassignedSpacesVectorController::attachModel(const model::Model& model)
{
  ModelObjectVectorController::attachModel(model);
  for (const model::Space& space : model.getConcreteModelObjects<model::Space>()){
    attachOtherModelObject(space);
  }
}

void BuildingStoryUnassignedSpacesVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (modelObject.optionalCast<model::Space>()){
    if (index == OS_SpaceFields::BuildingStoryName){
      emit itemIds(makeVector());
    }
  }
}

std::vector<OSItemId> BuildingStoryUnassignedSpacesVectorController::makeVector()
{
  std::vector<OSItemId> result;
  for (const model::Space& space : m_model->getConcreteModelObjects<model::Space>()){
    if (!space.handle().isNull()){
      if (!space.buildingStory()){
        result.push_back(modelObjectToItemId(space, false));
      }
    }
  }
  return result;
}

void BuildingStoryUnassignedSpacesVectorController::onObjectAdded(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  if (modelObject.optionalCast<model::Space>()){
    attachOtherModelObject(modelObject);
    emit itemIds(makeVector());
  }
}

void BuildingStoryUnassignedSpacesVectorController::onObjectRemoved(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  if (modelObject.optionalCast<model::Space>()){
    emit itemIds(makeVector());
  }
}

// BuildingStoryInspectorView

BuildingStoryInspectorView::BuildingStoryInspectorView(const openstudio::model::Model& model, QWidget * parent )
  : ModelObjectInspectorView(model, true, parent)
{
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

  mainGridLayout->addLayout(vLayout,0,0,1,2, Qt::AlignTop);

  // floor height and floor to floor height
  //vLayout = new QVBoxLayout();

  //label = new QLabel();
  //label->setText("Floor Height: ");
  //label->setStyleSheet("QLabel { font: bold; }");
  //vLayout->addWidget(label);

  //m_floorHeightEdit = new OSQuantityEdit(m_isIP);
  //isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), , SLOT(onUnitSystemChange(bool)));
  //OS_ASSERT(isConnected);
  //vLayout->addWidget(m_floorHeightEdit);

  //mainGridLayout->addLayout(vLayout,1,0, Qt::AlignTop|Qt::AlignLeft);

  //vLayout = new QVBoxLayout();

  //label = new QLabel();
  //label->setText("Floor To Floor Height: ");
  //label->setStyleSheet("QLabel { font: bold; }");
  //vLayout->addWidget(label);

  //m_floorToFloorHeightEdit = new OSQuantityEdit(m_isIP);
  //isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), , SLOT(onUnitSystemChange(bool)));
  //OS_ASSERT(isConnected);
  //vLayout->addWidget(m_floorToFloorHeightEdit);

  //mainGridLayout->addLayout(vLayout,1,1, Qt::AlignTop|Qt::AlignLeft);

  // default construction and schedule sets
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Default Construction Set: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  // this controller will be owned by the drop zone
  m_defaultConstructionSetVectorController = new BuildingStoryDefaultConstructionSetVectorController();
  m_defaultConstructionSetDropZone = new OSDropZone(m_defaultConstructionSetVectorController);
  m_defaultConstructionSetDropZone->setMinItems(0);
  m_defaultConstructionSetDropZone->setMaxItems(1);
  m_defaultConstructionSetDropZone->setItemsAcceptDrops(true);
  vLayout->addWidget(m_defaultConstructionSetDropZone);

  mainGridLayout->addLayout(vLayout,1,0);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Default Schedule Set: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  // this controller will be owned by the drop zone
  m_defaultScheduleSetVectorController = new BuildingStoryDefaultScheduleSetVectorController();
  m_defaultScheduleSetDropZone = new OSDropZone(m_defaultScheduleSetVectorController);
  m_defaultScheduleSetDropZone->setMinItems(0);
  m_defaultScheduleSetDropZone->setMaxItems(1);
  m_defaultScheduleSetDropZone->setItemsAcceptDrops(true);
  vLayout->addWidget(m_defaultScheduleSetDropZone);

  mainGridLayout->addLayout(vLayout,1,1);

  // rendering color
  m_renderingColorWidget = new RenderingColorWidget();
  mainGridLayout->addWidget(m_renderingColorWidget,2,0,1,2, Qt::AlignTop|Qt::AlignLeft);

  // spaces
  //vLayout = new QVBoxLayout();

  //label = new QLabel();
  //label->setText("Spaces: ");
  //label->setStyleSheet("QLabel { font: bold; }");
  //vLayout->addWidget(label);

  //m_spacesVectorController = new BuildingStorySpacesVectorController();
  //m_spacesDropZone = new OSDropZone(m_spacesVectorController);
  //vLayout->addWidget(m_spacesDropZone);

  //mainGridLayout->addLayout(vLayout,4,0,1,2);

  mainGridLayout->setColumnMinimumWidth(0, 100);
  mainGridLayout->setColumnMinimumWidth(1, 100);
  mainGridLayout->setColumnStretch(2,1);
  mainGridLayout->setRowMinimumHeight(0, 30);
  mainGridLayout->setRowMinimumHeight(1, 30);
  mainGridLayout->setRowMinimumHeight(2, 30);
  mainGridLayout->setRowStretch(3,1);
}

void BuildingStoryInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void BuildingStoryInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::BuildingStory buildingStory = modelObject.cast<model::BuildingStory>();
  attach(buildingStory);
}

void BuildingStoryInspectorView::onUpdate()
{
}

void BuildingStoryInspectorView::attach(openstudio::model::BuildingStory& buildingStory)
{
  m_nameEdit->bind(buildingStory, "name");
  //m_floorHeightEdit->bind(buildingStory, "nominalZCoordinate", m_isIP);
  //m_floorToFloorHeightEdit->bind(buildingStory, "nominalFloortoFloorHeight", m_isIP, std::string("isNominalFloortoFloorHeightDefaulted"));

  m_defaultConstructionSetVectorController->attach(buildingStory);
  m_defaultConstructionSetVectorController->reportItems();

  m_defaultScheduleSetVectorController->attach(buildingStory);
  m_defaultScheduleSetVectorController->reportItems();

  boost::optional<model::RenderingColor> renderingColor = buildingStory.renderingColor();
  if (!renderingColor){
    renderingColor = model::RenderingColor(buildingStory.model());
    buildingStory.setRenderingColor(*renderingColor);
  } 
  m_renderingColorWidget->attach(*renderingColor);

  //m_spacesVectorController->attach(buildingStory);
  //m_spacesVectorController->reportItems();

  this->stackedWidget()->setCurrentIndex(1);
}

void BuildingStoryInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  //m_floorHeightEdit->unbind();
  //m_floorToFloorHeightEdit->unbind();

  m_defaultConstructionSetVectorController->detach();

  m_defaultScheduleSetVectorController->detach();

  m_renderingColorWidget->detach();

  //m_spacesVectorController->detach();
}

void BuildingStoryInspectorView::toggleUnits(bool displayIP)
{
}

} // openstudio

