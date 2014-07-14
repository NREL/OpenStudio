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

#include "SurfaceInspectorView.hpp"
#include "PlanarSurfaceWidget.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSComboBox.hpp"
#include "OSVectorController.hpp"
#include "OSDropZone.hpp"
#include "ModelObjectItem.hpp"

#include "../model/Surface.hpp"
#include "../model/Surface_Impl.hpp"
#include "../model/ConstructionBase.hpp"
#include "../model/ConstructionBase_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/SpaceType.hpp"
#include "../model/SpaceType_Impl.hpp"
#include "../model/DefaultConstructionSet.hpp"
#include "../model/DefaultConstructionSet_Impl.hpp"
#include "../model/DefaultSurfaceConstructions.hpp"
#include "../model/DefaultSurfaceConstructions_Impl.hpp"
#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"
#include "../model/BuildingStory.hpp"
#include "../model/BuildingStory_Impl.hpp"
#include "../model/Component.hpp"
#include "../model/Component_Impl.hpp"
#include "../model/ComponentData.hpp"
#include "../model/ComponentData_Impl.hpp"

#include <utilities/idd/OS_Surface_FieldEnums.hxx>
#include <utilities/idd/OS_Space_FieldEnums.hxx>
#include <utilities/idd/OS_SpaceType_FieldEnums.hxx>
#include <utilities/idd/OS_DefaultConstructionSet_FieldEnums.hxx>
#include <utilities/idd/OS_DefaultSubSurfaceConstructions_FieldEnums.hxx>
#include <utilities/idd/OS_Building_FieldEnums.hxx>
#include <utilities/idd/OS_BuildingStory_FieldEnums.hxx>
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

// SurfaceConstructionVectorController

void SurfaceConstructionVectorController::attach(const model::ModelObject& modelObject)
{
  ModelObjectVectorController::attach(modelObject);

  model::Surface surface = m_modelObject->cast<model::Surface>();
  attachOtherModelObjects(surface);
}

void SurfaceConstructionVectorController::attachOtherModelObjects(const model::Surface& surface)
{
  boost::optional<model::Space> space = surface.space();
  if (space){
    attachOtherModelObjects(*space);
  }

  model::Building building = surface.model().getUniqueModelObject<model::Building>();
  attachOtherModelObjects(building);
}

void SurfaceConstructionVectorController::attachOtherModelObjects(const model::Space& space)
{
  attachOtherModelObject(space);

  boost::optional<model::DefaultConstructionSet> defaultConstructionSet = space.defaultConstructionSet();
  if (defaultConstructionSet){
    attachOtherModelObjects(*defaultConstructionSet);
  }

  boost::optional<model::SpaceType> spaceType = space.spaceType();
  if (spaceType && !space.isSpaceTypeDefaulted()){
    attachOtherModelObjects(*spaceType);
  }

  boost::optional<model::BuildingStory> buildingStory = space.buildingStory();
  if (buildingStory){
    attachOtherModelObjects(*buildingStory);
  }
}

void SurfaceConstructionVectorController::attachOtherModelObjects(const model::SpaceType& spaceType)
{
  attachOtherModelObject(spaceType);

  boost::optional<model::DefaultConstructionSet> defaultConstructionSet = spaceType.defaultConstructionSet();
  if (defaultConstructionSet){
    attachOtherModelObjects(*defaultConstructionSet);
  }
}

void SurfaceConstructionVectorController::attachOtherModelObjects(const model::BuildingStory& buildingStory)
{
  attachOtherModelObject(buildingStory);

  boost::optional<model::DefaultConstructionSet> defaultConstructionSet = buildingStory.defaultConstructionSet();
  if (defaultConstructionSet){
    attachOtherModelObjects(*defaultConstructionSet);
  }
}

void SurfaceConstructionVectorController::attachOtherModelObjects(const model::Building& building)
{
  attachOtherModelObject(building);

  boost::optional<model::SpaceType> spaceType = building.spaceType();
  if (spaceType){
    attachOtherModelObjects(*spaceType);
  }

  boost::optional<model::DefaultConstructionSet> defaultConstructionSet = building.defaultConstructionSet();
  if (defaultConstructionSet){
    attachOtherModelObjects(*defaultConstructionSet);
  }
}

void SurfaceConstructionVectorController::attachOtherModelObjects(const model::DefaultConstructionSet& defaultConstructionSet)
{
  attachOtherModelObject(defaultConstructionSet);

  boost::optional<model::DefaultSurfaceConstructions> defaultSurfaceConstructions = defaultConstructionSet.defaultExteriorSurfaceConstructions();
  if (defaultSurfaceConstructions){
    attachOtherModelObjects(*defaultSurfaceConstructions);
  }

  defaultSurfaceConstructions = defaultConstructionSet.defaultInteriorSurfaceConstructions();
  if (defaultSurfaceConstructions){
    attachOtherModelObject(*defaultSurfaceConstructions);
  }

  defaultSurfaceConstructions = defaultConstructionSet.defaultGroundContactSurfaceConstructions();
  if (defaultSurfaceConstructions){
    attachOtherModelObject(*defaultSurfaceConstructions);
  }
}

void SurfaceConstructionVectorController::attachOtherModelObjects(const model::DefaultSurfaceConstructions& defaultSurfaceConstructions)
{
  attachOtherModelObject(defaultSurfaceConstructions);
}

void SurfaceConstructionVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  model::Surface surface = m_modelObject->cast<model::Surface>();

  if (modelObject.optionalCast<model::Surface>()){
    if (index == OS_SurfaceFields::ConstructionName){
      emit itemIds(makeVector());
    }else if (index == OS_SurfaceFields::SpaceName){
      detachOtherModelObjects();
      attachOtherModelObjects(surface);
      emit itemIds(makeVector());
    }
  }else if (modelObject.optionalCast<model::Space>()){
    if (index == OS_SpaceFields::SpaceTypeName){
      detachOtherModelObjects();
      attachOtherModelObjects(surface);
      emit itemIds(makeVector());
    }else if (index == OS_SpaceFields::BuildingStoryName){
      detachOtherModelObjects();
      attachOtherModelObjects(surface);
      emit itemIds(makeVector());
    }else if (index == OS_SpaceFields::DefaultConstructionSetName){
      detachOtherModelObjects();
      attachOtherModelObjects(surface);
      emit itemIds(makeVector());
    }
  }else if (modelObject.optionalCast<model::SpaceType>()){
    if (index == OS_SpaceTypeFields::DefaultConstructionSetName){
      detachOtherModelObjects();
      attachOtherModelObjects(surface);
      emit itemIds(makeVector());
    }
  }else if (modelObject.optionalCast<model::Building>()){
    if (index == OS_BuildingFields::DefaultConstructionSetName){
      detachOtherModelObjects();
      attachOtherModelObjects(surface);
      emit itemIds(makeVector());
    }else if (index == OS_BuildingFields::SpaceTypeName){
      detachOtherModelObjects();
      attachOtherModelObjects(surface);
      emit itemIds(makeVector());
    }
  }else if (modelObject.optionalCast<model::BuildingStory>()){
    if (index == OS_BuildingStoryFields::DefaultConstructionSetName){
      detachOtherModelObjects();
      attachOtherModelObjects(surface);
      emit itemIds(makeVector());
    }
  }else if (modelObject.optionalCast<model::DefaultConstructionSet>()){
    if (index == OS_DefaultConstructionSetFields::DefaultExteriorSurfaceConstructionsName){
      detachOtherModelObjects();
      attachOtherModelObjects(surface);
      emit itemIds(makeVector());
    }else if (index == OS_DefaultConstructionSetFields::DefaultInteriorSurfaceConstructionsName){
      detachOtherModelObjects();
      attachOtherModelObjects(surface);
      emit itemIds(makeVector());
    }else if (index == OS_DefaultConstructionSetFields::DefaultGroundContactSurfaceConstructionsName){
      detachOtherModelObjects();
      attachOtherModelObjects(surface);
      emit itemIds(makeVector());
    }
  }else if (modelObject.optionalCast<model::DefaultSurfaceConstructions>()){
    emit itemIds(makeVector());
  }
}

void SurfaceConstructionVectorController::onDataChange(const model::ModelObject& modelObject)
{
  if (m_modelObject && (modelObject.handle() == m_modelObject->handle())){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> SurfaceConstructionVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::Surface surface = m_modelObject->cast<model::Surface>();
    boost::optional<model::ConstructionBase> construction = surface.construction();
    if (construction){
      bool isDefaulted = surface.isConstructionDefaulted();
      OSItemId itemId = modelObjectToItemId(*construction, isDefaulted);
      result.push_back(itemId);
    }
  }
  return result;
}

void SurfaceConstructionVectorController::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::Surface surface = m_modelObject->cast<model::Surface>();
    surface.resetConstruction();
  }
}

void SurfaceConstructionVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void SurfaceConstructionVectorController::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::Surface surface = m_modelObject->cast<model::Surface>();
    boost::optional<model::ConstructionBase> constructionBase = this->addToModel<model::ConstructionBase>(itemId);
    if (constructionBase){
      surface.setConstruction(*constructionBase);
    }
  }
}

// SurfaceOutsideBoundaryConditionObjectVectorController

void SurfaceOutsideBoundaryConditionObjectVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_SurfaceFields::OutsideBoundaryConditionObject){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> SurfaceOutsideBoundaryConditionObjectVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::Surface surface = m_modelObject->cast<model::Surface>();
    boost::optional<model::Surface> adjacentSurface = surface.adjacentSurface();
    if (adjacentSurface){
      result.push_back(modelObjectToItemId(*adjacentSurface, false));
    }
  }
  return result;
}

void SurfaceOutsideBoundaryConditionObjectVectorController::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::Surface surface = m_modelObject->cast<model::Surface>();
    surface.resetAdjacentSurface();
  }
}

void SurfaceOutsideBoundaryConditionObjectVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void SurfaceOutsideBoundaryConditionObjectVectorController::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::Surface surface = m_modelObject->cast<model::Surface>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::Surface>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        model::Surface other = modelObject->cast<model::Surface>();
        surface.setAdjacentSurface(other);
      }
    }
  }
}

// SurfaceInspectorView

SurfaceInspectorView::SurfaceInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent )
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

  mainGridLayout->addLayout(vLayout,0,0,1,2, Qt::AlignTop|Qt::AlignLeft);

  // surface type
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Surface Type: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_surfaceType = new OSComboBox();
  vLayout->addWidget(m_surfaceType);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout,1,0);

  // construction
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Construction: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_constructionVectorController = new SurfaceConstructionVectorController();
  m_constructionDropZone = new OSDropZone(m_constructionVectorController);
  m_constructionDropZone->setMinItems(0);
  m_constructionDropZone->setMaxItems(1);
  m_constructionDropZone->setItemsAcceptDrops(true);
  vLayout->addWidget(m_constructionDropZone);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout,1,1);

  // outside boundary condition and object
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Outside Boundary Condition: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_outsideBoundaryCondition = new OSComboBox();
  vLayout->addWidget(m_outsideBoundaryCondition);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout,2,0);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Outside Boundary Condition Object: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_surfaceOutsideBoundaryConditionObjectVectorController = new SurfaceOutsideBoundaryConditionObjectVectorController();
  m_outsideBoundaryConditionObjectDropZone = new OSDropZone(m_surfaceOutsideBoundaryConditionObjectVectorController);
  m_outsideBoundaryConditionObjectDropZone->setMinItems(0);
  m_outsideBoundaryConditionObjectDropZone->setMaxItems(1);
  m_outsideBoundaryConditionObjectDropZone->setItemsAcceptDrops(true);
  vLayout->addWidget(m_outsideBoundaryConditionObjectDropZone);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout,2,1);

  // sun and wind exposure
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Sun Exposure: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_sunExposure = new OSComboBox();
  vLayout->addWidget(m_sunExposure);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout,3,0);

  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Wind Exposure: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_windExposure = new OSComboBox();
  vLayout->addWidget(m_windExposure);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout,3,1);

  // separator
  vLayout = new QVBoxLayout();
  vLayout->setContentsMargins(0,0,0,0);

  QWidget * hLine = new QWidget();
  hLine->setObjectName("HLine");
  hLine->setStyleSheet("QWidget#HLine { background: #445051;}");
  hLine->setFixedHeight(2);
  vLayout->addWidget(hLine);
  
  label = new QLabel();
  label->setText("Vertices: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  mainGridLayout->addLayout(vLayout,4,0,1,2);

  // planar surface widget
  m_planarSurfaceWidget = new PlanarSurfaceWidget(m_isIP);
  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_planarSurfaceWidget, SLOT(toggleUnits(bool)));
  OS_ASSERT(isConnected);

  mainGridLayout->addWidget(m_planarSurfaceWidget,5,0,1,2);

  mainGridLayout->setColumnMinimumWidth(0, 80);
  mainGridLayout->setColumnMinimumWidth(1, 80);
  mainGridLayout->setColumnStretch(2,1);
  mainGridLayout->setRowMinimumHeight(0, 30);
  mainGridLayout->setRowMinimumHeight(1, 30);
  mainGridLayout->setRowMinimumHeight(2, 30);
  mainGridLayout->setRowMinimumHeight(3, 30);
  mainGridLayout->setRowMinimumHeight(4, 30);
  mainGridLayout->setRowMinimumHeight(5, 30);
  mainGridLayout->setRowStretch(6,1);
}

void SurfaceInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void SurfaceInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::Surface surface = modelObject.cast<model::Surface>();
  attach(surface);
  refresh();
}

void SurfaceInspectorView::onUpdate()
{
  refresh();
}

void SurfaceInspectorView::attach(openstudio::model::Surface& surface)
{
  m_nameEdit->bind(surface, "name");

  m_surfaceType->bind(surface, "surfaceType");

  m_constructionVectorController->attach(surface);
  m_constructionVectorController->reportItems();

  m_outsideBoundaryCondition->bind(surface, "outsideBoundaryCondition");

  m_surfaceOutsideBoundaryConditionObjectVectorController->attach(surface);
  m_surfaceOutsideBoundaryConditionObjectVectorController->reportItems();

  m_sunExposure->bind(surface, "sunExposure");

  m_windExposure->bind(surface, "windExposure");

  m_planarSurfaceWidget->attach(surface);

  this->stackedWidget()->setCurrentIndex(1);
}

void SurfaceInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_surfaceType->unbind();
  m_constructionVectorController->detach();
  m_outsideBoundaryCondition->unbind();
  m_surfaceOutsideBoundaryConditionObjectVectorController->detach();
  m_sunExposure->unbind();
  m_windExposure->unbind();
  m_planarSurfaceWidget->detach();
}

void SurfaceInspectorView::refresh()
{
}

void SurfaceInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio

