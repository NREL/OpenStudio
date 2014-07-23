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
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNUerererererererererererererererererererererererererererererererererererererererererererererererererererer
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include "SubSurfaceInspectorView.hpp"
#include "PlanarSurfaceWidget.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSDoubleEdit.hpp"
#include "OSVectorController.hpp"
#include "OSDropZone.hpp"
#include "ModelObjectItem.hpp"

#include "../model/SubSurface.hpp"
#include "../model/SubSurface_Impl.hpp"
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
#include "../model/DefaultSubSurfaceConstructions.hpp"
#include "../model/DefaultSubSurfaceConstructions_Impl.hpp"
#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"
#include "../model/BuildingStory.hpp"
#include "../model/BuildingStory_Impl.hpp"
#include "../model/Component.hpp"
#include "../model/Component_Impl.hpp"
#include "../model/ComponentData.hpp"
#include "../model/ComponentData_Impl.hpp"

#include <utilities/idd/OS_SubSurface_FieldEnums.hxx>
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

void SubSurfaceConstructionVectorController::attach(const model::ModelObject& modelObject)
{
  ModelObjectVectorController::attach(modelObject);

  model::SubSurface subSurface = m_modelObject->cast<model::SubSurface>();
  attachOtherModelObjects(subSurface);
}

void SubSurfaceConstructionVectorController::attachOtherModelObjects(const model::SubSurface& subSurface)
{
  boost::optional<model::Surface> surface = subSurface.surface();
  if (surface){
    attachOtherModelObjects(*surface);
  }

  boost::optional<model::Space> space = subSurface.space();
  if (space){
    attachOtherModelObjects(*space);
  }

  model::Building building = subSurface.model().getUniqueModelObject<model::Building>();
  attachOtherModelObjects(building);
}

void SubSurfaceConstructionVectorController::attachOtherModelObjects(const model::Surface& surface)
{
  attachOtherModelObject(surface);
}

void SubSurfaceConstructionVectorController::attachOtherModelObjects(const model::Space& space)
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

void SubSurfaceConstructionVectorController::attachOtherModelObjects(const model::SpaceType& spaceType)
{
  attachOtherModelObject(spaceType);

  boost::optional<model::DefaultConstructionSet> defaultConstructionSet = spaceType.defaultConstructionSet();
  if (defaultConstructionSet){
    attachOtherModelObjects(*defaultConstructionSet);
  }
}

void SubSurfaceConstructionVectorController::attachOtherModelObjects(const model::BuildingStory& buildingStory)
{
  attachOtherModelObject(buildingStory);

  boost::optional<model::DefaultConstructionSet> defaultConstructionSet = buildingStory.defaultConstructionSet();
  if (defaultConstructionSet){
    attachOtherModelObjects(*defaultConstructionSet);
  }
}

void SubSurfaceConstructionVectorController::attachOtherModelObjects(const model::Building& building)
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

void SubSurfaceConstructionVectorController::attachOtherModelObjects(const model::DefaultConstructionSet& defaultConstructionSet)
{
  attachOtherModelObject(defaultConstructionSet);

  boost::optional<model::DefaultSubSurfaceConstructions> defaultSubSurfaceConstructions = defaultConstructionSet.defaultExteriorSubSurfaceConstructions();
  if (defaultSubSurfaceConstructions){
    attachOtherModelObjects(*defaultSubSurfaceConstructions);
  }

  defaultSubSurfaceConstructions = defaultConstructionSet.defaultInteriorSubSurfaceConstructions();
  if (defaultSubSurfaceConstructions){
    attachOtherModelObject(*defaultSubSurfaceConstructions);
  }
}

void SubSurfaceConstructionVectorController::attachOtherModelObjects(const model::DefaultSubSurfaceConstructions& defaultSubSurfaceConstructions)
{
  attachOtherModelObject(defaultSubSurfaceConstructions);
}

void SubSurfaceConstructionVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  model::SubSurface subSurface = m_modelObject->cast<model::SubSurface>();

  if (modelObject.optionalCast<model::SubSurface>()){
    if (index == OS_SubSurfaceFields::ConstructionName){
      emit itemIds(makeVector());
    }else if (index == OS_SubSurfaceFields::SurfaceName){
      detachOtherModelObjects();
      attachOtherModelObjects(subSurface);
      emit itemIds(makeVector());
    }
  }else if (modelObject.optionalCast<model::Surface>()){
    if (index == OS_SurfaceFields::SpaceName){
      detachOtherModelObjects();
      attachOtherModelObjects(subSurface);
      emit itemIds(makeVector());
    }
  }else if (modelObject.optionalCast<model::Space>()){
    if (index == OS_SpaceFields::SpaceTypeName){
      detachOtherModelObjects();
      attachOtherModelObjects(subSurface);
      emit itemIds(makeVector());
    }else if (index == OS_SpaceFields::BuildingStoryName){
      detachOtherModelObjects();
      attachOtherModelObjects(subSurface);
      emit itemIds(makeVector());
    }else if (index == OS_SpaceFields::DefaultConstructionSetName){
      detachOtherModelObjects();
      attachOtherModelObjects(subSurface);
      emit itemIds(makeVector());
    }
  }else if (modelObject.optionalCast<model::SpaceType>()){
    if (index == OS_SpaceTypeFields::DefaultConstructionSetName){
      detachOtherModelObjects();
      attachOtherModelObjects(subSurface);
      emit itemIds(makeVector());
    }
  }else if (modelObject.optionalCast<model::Building>()){
    if (index == OS_BuildingFields::DefaultConstructionSetName){
      detachOtherModelObjects();
      attachOtherModelObjects(subSurface);
      emit itemIds(makeVector());
    }else if (index == OS_BuildingFields::SpaceTypeName){
      detachOtherModelObjects();
      attachOtherModelObjects(subSurface);
      emit itemIds(makeVector());
    }
  }else if (modelObject.optionalCast<model::BuildingStory>()){
    if (index == OS_BuildingStoryFields::DefaultConstructionSetName){
      detachOtherModelObjects();
      attachOtherModelObjects(subSurface);
      emit itemIds(makeVector());
    }
  }else if (modelObject.optionalCast<model::DefaultConstructionSet>()){
    if (index == OS_DefaultConstructionSetFields::DefaultExteriorSubSurfaceConstructionsName){
      detachOtherModelObjects();
      attachOtherModelObjects(subSurface);
      emit itemIds(makeVector());
    }else if (index == OS_DefaultConstructionSetFields::DefaultInteriorSubSurfaceConstructionsName){
      detachOtherModelObjects();
      attachOtherModelObjects(subSurface);
      emit itemIds(makeVector());
    }
  }else if (modelObject.optionalCast<model::DefaultSubSurfaceConstructions>()){
    emit itemIds(makeVector());
  }
}

void SubSurfaceConstructionVectorController::onDataChange(const model::ModelObject& modelObject)
{
  if (m_modelObject && (modelObject.handle() == m_modelObject->handle())){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> SubSurfaceConstructionVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::SubSurface subSurface = m_modelObject->cast<model::SubSurface>();
    boost::optional<model::ConstructionBase> construction = subSurface.construction();
    if (construction){
      bool isDefaulted = subSurface.isConstructionDefaulted();
      OSItemId itemId = modelObjectToItemId(*construction, isDefaulted);
      result.push_back(itemId);
    }
  }
  return result;
}

void SubSurfaceConstructionVectorController::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::SubSurface subSurface = m_modelObject->cast<model::SubSurface>();
    subSurface.resetConstruction();
  }
}

void SubSurfaceConstructionVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void SubSurfaceConstructionVectorController::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::SubSurface subSurface = m_modelObject->cast<model::SubSurface>();
    boost::optional<model::ConstructionBase> constructionBase = this->addToModel<model::ConstructionBase>(itemId);
    if (constructionBase){
      subSurface.setConstruction(*constructionBase);
    }
  }
}

// SubSurfaceOutsideBoundaryConditionObjectVectorController

void SubSurfaceOutsideBoundaryConditionObjectVectorController::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_SubSurfaceFields::OutsideBoundaryConditionObject){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> SubSurfaceOutsideBoundaryConditionObjectVectorController::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::SubSurface subSurface = m_modelObject->cast<model::SubSurface>();
    boost::optional<model::SubSurface> adjacentSubSurface = subSurface.adjacentSubSurface();
    if (adjacentSubSurface){
      result.push_back(modelObjectToItemId(*adjacentSubSurface, false));
    }
  }
  return result;
}

void SubSurfaceOutsideBoundaryConditionObjectVectorController::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::SubSurface subSurface = m_modelObject->cast<model::SubSurface>();
    subSurface.resetAdjacentSubSurface();
  }
}

void SubSurfaceOutsideBoundaryConditionObjectVectorController::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void SubSurfaceOutsideBoundaryConditionObjectVectorController::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::SubSurface subSurface = m_modelObject->cast<model::SubSurface>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::SubSurface>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        model::SubSurface other = modelObject->cast<model::SubSurface>();
        subSurface.setAdjacentSubSurface(other);
      }
    }
  }
}

// SubSurfaceInspectorView

SubSurfaceInspectorView::SubSurfaceInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent )
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

  // subsurface type
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Sub Surface Type: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_subSurfaceType = new OSComboBox();
  vLayout->addWidget(m_subSurfaceType);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout,1,0);

  // construction 
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Construction: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_constructionVectorController = new SubSurfaceConstructionVectorController();
  m_constructionDropZone = new OSDropZone(m_constructionVectorController);
  m_constructionDropZone->setMinItems(0);
  m_constructionDropZone->setMaxItems(1);
  m_constructionDropZone->setItemsAcceptDrops(true);
  vLayout->addWidget(m_constructionDropZone);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout,1,1);

  // outside boundary condition object
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Outside Boundary Condition Object: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_outsideBoundaryConditionObjectVectorController = new SubSurfaceOutsideBoundaryConditionObjectVectorController();
  m_outsideBoundaryConditionObjectDropZone = new OSDropZone(m_outsideBoundaryConditionObjectVectorController);
  m_outsideBoundaryConditionObjectDropZone->setMinItems(0);
  m_outsideBoundaryConditionObjectDropZone->setMaxItems(1);
  m_outsideBoundaryConditionObjectDropZone->setItemsAcceptDrops(true);
  vLayout->addWidget(m_outsideBoundaryConditionObjectDropZone);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout,2,0);

  // multiplier
  vLayout = new QVBoxLayout();

  label = new QLabel();
  label->setText("Multiplier: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  m_multiplier = new OSDoubleEdit();
  vLayout->addWidget(m_multiplier);

  vLayout->addStretch();

  mainGridLayout->addLayout(vLayout,3,0);

  // separator
  vLayout = new QVBoxLayout();

  QWidget * hLine = new QWidget();
  hLine->setObjectName("HLine");
  hLine->setStyleSheet("QWidget#HLine { background: #445051;}");
  hLine->setFixedHeight(2);
  vLayout->addWidget(hLine);

  label = new QLabel();
  label->setText("Vertices: ");
  label->setStyleSheet("QLabel { font: bold; }");
  vLayout->addWidget(label);

  mainGridLayout->addWidget(hLine,4,0,1,2);

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

void SubSurfaceInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void SubSurfaceInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::SubSurface subSurface = modelObject.cast<model::SubSurface>();
  attach(subSurface);
  refresh();
}

void SubSurfaceInspectorView::onUpdate()
{
  refresh();
}

void SubSurfaceInspectorView::attach(openstudio::model::SubSurface& subSurface)
{
  m_nameEdit->bind(subSurface, "name");

  m_subSurfaceType->bind(subSurface, "subSurfaceType");
  
  m_constructionVectorController->attach(subSurface);
  m_constructionVectorController->reportItems();

  m_outsideBoundaryConditionObjectVectorController->attach(subSurface);
  m_outsideBoundaryConditionObjectVectorController->reportItems();

  m_multiplier->bind(subSurface, "multiplier");

  m_planarSurfaceWidget->attach(subSurface);

  this->stackedWidget()->setCurrentIndex(1);
}

void SubSurfaceInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_subSurfaceType->unbind();
  m_constructionVectorController->detach();
  m_outsideBoundaryConditionObjectVectorController->detach();
  m_multiplier->unbind();
  m_planarSurfaceWidget->detach();
}

void SubSurfaceInspectorView::refresh()
{
}

void SubSurfaceInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio

