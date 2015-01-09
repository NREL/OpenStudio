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

#include "BuildingStoryInspectorView.hpp"

#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"

#include "ModelObjectItem.hpp"
#include "OSDropZone.hpp"
#include "RenderingColorWidget.hpp"

#include "../model/BuildingStory_Impl.hpp"
#include "../model/DefaultConstructionSet.hpp"
#include "../model/DefaultConstructionSet_Impl.hpp"
#include "../model/DefaultScheduleSet.hpp"
#include "../model/DefaultScheduleSet_Impl.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/RenderingColor.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"

#include <utilities/idd/OS_BuildingStory_FieldEnums.hxx>
#include <utilities/idd/OS_Space_FieldEnums.hxx>
#include "../utilities/core/Assert.hpp"

#include <QBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
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

BuildingStoryInspectorView::BuildingStoryInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
  m_isIP(isIP)
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

  int row = 0;

  mainGridLayout->addLayout(vLayout, row, 0, 1, 2, Qt::AlignTop);

  ++row;

  /* DLM: disable for now

  // Measure Tags
  QFrame * line;
  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainGridLayout->addWidget(line, row, 0, 1, 2);

  ++row;

  label = new QLabel();
  label->setText("Measure Tags (Optional):");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  vLayout = new QVBoxLayout();

  // Building Story Nominal Z Coordinate
  label = new QLabel();
  label->setText("Building Story Nominal Z Coordinate: ");
  label->setObjectName("StandardsInfo");
  vLayout->addWidget(label);

  m_zCoordinate = new OSQuantityEdit2("m", "m", "ft", m_isIP);
  connect(this, &BuildingStoryInspectorView::toggleUnitsClicked, m_zCoordinate, &OSQuantityEdit2::onUnitSystemChange);
  vLayout->addWidget(m_zCoordinate);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout, row, 0);
  mainGridLayout->setRowMinimumHeight(row, 30);

  // DLM: We could undeprecate "Building Sector Type" and put
  // it here too?

  ++row;

  // Nominal Floor to Ceiling Height 
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Nominal Floor to Ceiling Height: ");
  label->setObjectName("StandardsInfo");
  vLayout->addWidget(label);

  m_floorToCeilingHeight = new OSQuantityEdit2("m", "m", "ft", m_isIP);
  m_floorToCeilingHeight->doubleValidator()->setBottom(0);
  connect(this, &BuildingStoryInspectorView::toggleUnitsClicked, m_zCoordinate, &OSQuantityEdit2::onUnitSystemChange);
  vLayout->addWidget(m_floorToCeilingHeight);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout, row, 0);
  mainGridLayout->setRowMinimumHeight(row, 30);

  // Nominal Floor to Floor Height
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Nominal Floor to Floor Height: ");
  label->setObjectName("StandardsInfo");
  vLayout->addWidget(label);

  m_floorToFloorHeight = new OSQuantityEdit2("m", "m", "ft", m_isIP);
  m_floorToFloorHeight->doubleValidator()->setBottom(0);
  vLayout->addWidget(m_floorToFloorHeight);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout, row, 1);
  mainGridLayout->setRowMinimumHeight(row, 30);

  ++row;

  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  mainGridLayout->addWidget(line, row, 0, 1, 2);
  
  ++row;
  */
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

  mainGridLayout->addLayout(vLayout,row,0);

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

  mainGridLayout->addLayout(vLayout,row,1);

  ++row;

  // rendering color
  m_renderingColorWidget = new RenderingColorWidget();
  mainGridLayout->addWidget(m_renderingColorWidget, row, 0, 1, 2, Qt::AlignTop | Qt::AlignLeft);

  //++row;

  // spaces
  //vLayout = new QVBoxLayout();

  //label = new QLabel();
  //label->setText("Spaces: ");
  //label->setStyleSheet("QLabel { font: bold; }");
  //vLayout->addWidget(label);

  //m_spacesVectorController = new BuildingStorySpacesVectorController();
  //m_spacesDropZone = new OSDropZone(m_spacesVectorController);
  //vLayout->addWidget(m_spacesDropZone);

  //mainGridLayout->addLayout(vLayout,row,0,1,2);

  mainGridLayout->setColumnMinimumWidth(0, 80);
  mainGridLayout->setColumnMinimumWidth(1, 80);
  mainGridLayout->setColumnStretch(2, 1);
  mainGridLayout->setRowStretch(row, 1);
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

  /* DLM: disable for now

  m_zCoordinate->bind(
    m_isIP,
    buildingStory,
    OptionalDoubleGetter(std::bind(&model::BuildingStory::nominalZCoordinate, buildingStory)),
    // Evan note: "auto bs" required as a copy because the passed in buildingStory object became const in the lambda function
    DoubleSetterVoidReturn([buildingStory](double d) { auto bs = buildingStory; bs.setNominalZCoordinate(d); }),
    boost::optional<NoFailAction>([buildingStory]() { auto bs = buildingStory; bs.resetNominalZCoordinate(); }),
    boost::optional<NoFailAction>(),
    boost::optional<NoFailAction>(),
    boost::optional<BasicQuery>(),
    boost::optional<BasicQuery>(),
    boost::optional<BasicQuery>());

  m_floorToFloorHeight->bind(
    m_isIP,
    buildingStory,
    OptionalDoubleGetter([buildingStory]() { return buildingStory.nominalFloortoFloorHeight(); }),
    boost::optional<DoubleSetter>([buildingStory](double d) { auto bs = buildingStory; return bs.setNominalFloortoFloorHeight(d); }),
    boost::optional<NoFailAction>([buildingStory]() { auto bs = buildingStory; bs.resetNominalFloortoFloorHeight(); }),
    boost::optional<NoFailAction>(),
    boost::optional<NoFailAction>(),
    boost::optional<BasicQuery>(),
    boost::optional<BasicQuery>(),
    boost::optional<BasicQuery>());

  m_floorToCeilingHeight->bind(
    m_isIP,
    buildingStory,
    OptionalDoubleGetter([buildingStory]() { return buildingStory.nominalFloortoCeilingHeight(); }),
    boost::optional<DoubleSetter>([buildingStory](double d) { auto bs = buildingStory; return bs.setNominalFloortoCeilingHeight(d); }),
    boost::optional<NoFailAction>([buildingStory]() { auto bs = buildingStory; bs.resetNominalFloortoCeilingHeight(); }),
    boost::optional<NoFailAction>(),
    boost::optional<NoFailAction>(),
    boost::optional<BasicQuery>(),
    boost::optional<BasicQuery>(),
    boost::optional<BasicQuery>());
    */

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

  /* DLM: disable for now 
  m_zCoordinate->unbind();
  m_floorToFloorHeight->unbind();
  m_floorToCeilingHeight->unbind();
  */

  //m_spacesVectorController->detach();
}

void BuildingStoryInspectorView::toggleUnits(bool displayIP)
{
}

} // openstudio

