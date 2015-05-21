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

#include "ModelObjectTreeItems.hpp"
#include "ModelObjectItem.hpp"
#include "IconLibrary.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"
#include "../model/BuildingStory.hpp"
#include "../model/BuildingStory_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/SpaceType.hpp"
#include "../model/SpaceType_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/ShadingSurfaceGroup.hpp"
#include "../model/ShadingSurfaceGroup_Impl.hpp"
#include "../model/ShadingSurface.hpp"
#include "../model/ShadingSurface_Impl.hpp"
#include "../model/InteriorPartitionSurfaceGroup.hpp"
#include "../model/InteriorPartitionSurfaceGroup_Impl.hpp"
#include "../model/InteriorPartitionSurface.hpp"
#include "../model/InteriorPartitionSurface_Impl.hpp"
#include "../model/Surface.hpp"
#include "../model/Surface_Impl.hpp"
#include "../model/SubSurface.hpp"
#include "../model/SubSurface_Impl.hpp"
#include "../model/DaylightingControl.hpp"
#include "../model/DaylightingControl_Impl.hpp"
#include "../model/GlareSensor.hpp"
#include "../model/GlareSensor_Impl.hpp"
#include "../model/IlluminanceMap.hpp"
#include "../model/IlluminanceMap_Impl.hpp"
#include "../model/InternalMass.hpp"
#include "../model/InternalMass_Impl.hpp"
#include "../model/People.hpp"
#include "../model/People_Impl.hpp"
#include "../model/Lights.hpp"
#include "../model/Lights_Impl.hpp"
#include "../model/Luminaire.hpp"
#include "../model/Luminaire_Impl.hpp"
#include "../model/ElectricEquipment.hpp"
#include "../model/ElectricEquipment_Impl.hpp"
#include "../model/GasEquipment.hpp"
#include "../model/GasEquipment_Impl.hpp"
#include "../model/HotWaterEquipment.hpp"
#include "../model/HotWaterEquipment_Impl.hpp"
#include "../model/SteamEquipment.hpp"
#include "../model/SteamEquipment_Impl.hpp"
#include "../model/OtherEquipment.hpp"
#include "../model/OtherEquipment_Impl.hpp"
#include "../model/SpaceInfiltrationDesignFlowRate.hpp"
#include "../model/SpaceInfiltrationDesignFlowRate_Impl.hpp"
#include "../model/SpaceInfiltrationEffectiveLeakageArea.hpp"
#include "../model/SpaceInfiltrationEffectiveLeakageArea_Impl.hpp"
#include "../model/DesignSpecificationOutdoorAir.hpp"
#include "../model/DesignSpecificationOutdoorAir_Impl.hpp"

#include <utilities/idd/OS_Building_FieldEnums.hxx>
#include <utilities/idd/OS_BuildingStory_FieldEnums.hxx>
#include <utilities/idd/OS_ThermalZone_FieldEnums.hxx>
#include <utilities/idd/OS_SpaceType_FieldEnums.hxx>
#include <utilities/idd/OS_Space_FieldEnums.hxx>
#include <utilities/idd/OS_ShadingSurfaceGroup_FieldEnums.hxx>
#include <utilities/idd/OS_ShadingSurface_FieldEnums.hxx>
#include <utilities/idd/OS_InteriorPartitionSurfaceGroup_FieldEnums.hxx>
#include <utilities/idd/OS_InteriorPartitionSurface_FieldEnums.hxx>
#include <utilities/idd/OS_Surface_FieldEnums.hxx>
#include <utilities/idd/OS_SubSurface_FieldEnums.hxx>
#include <utilities/idd/OS_Daylighting_Control_FieldEnums.hxx>
#include <utilities/idd/OS_IlluminanceMap_FieldEnums.hxx>
#include <utilities/idd/OS_InternalMass_FieldEnums.hxx>
#include <utilities/idd/OS_People_FieldEnums.hxx>
#include <utilities/idd/OS_Lights_FieldEnums.hxx>
#include <utilities/idd/OS_Luminaire_FieldEnums.hxx>
#include <utilities/idd/OS_ElectricEquipment_FieldEnums.hxx>
#include <utilities/idd/OS_GasEquipment_FieldEnums.hxx>
#include <utilities/idd/OS_HotWaterEquipment_FieldEnums.hxx>
#include <utilities/idd/OS_SteamEquipment_FieldEnums.hxx>
#include <utilities/idd/OS_OtherEquipment_FieldEnums.hxx>
#include <utilities/idd/OS_SpaceInfiltration_DesignFlowRate_FieldEnums.hxx>
#include <utilities/idd/OS_SpaceInfiltration_EffectiveLeakageArea_FieldEnums.hxx>
#include <utilities/idd/OS_DesignSpecification_OutdoorAir_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"

#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QScrollArea>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QButtonGroup>
#include <QResizeEvent>
#include <QTimer>
#include <QMouseEvent>
#include <QCalendarWidget>
#include <QGridLayout>
#include <QRadioButton>

#include <iostream>

namespace openstudio {

const OSItemType ModelObjectTreeItem::m_type = ModelObjectTreeItem::initializeOSItemType();

OSItemType ModelObjectTreeItem::initializeOSItemType()
{
  return OSItemType::ListItem;
}

OSItemType initializeOSItemType();

ModelObjectTreeItem::ModelObjectTreeItem(const openstudio::model::ModelObject& modelObject, bool isDefaulted, OSItemType type, QTreeWidgetItem* parent)
  : QTreeWidgetItem(parent),
  m_handle(modelObject.handle()),
  m_modelObject(modelObject),
  m_model(modelObject.model()),
  m_dirty(false)
{
  m_item = new ModelObjectItem(modelObject,isDefaulted,type);
  m_item->setVisible(false);

  this->setText(0, toQString(modelObject.name().get()));
  this->setStyle(0, "");

  connect(m_modelObject->getImpl<model::detail::ModelObject_Impl>().get(), &model::detail::ModelObject_Impl::onNameChange, this, &ModelObjectTreeItem::changeName);
   
  connect(m_modelObject->getImpl<model::detail::ModelObject_Impl>().get(), &model::detail::ModelObject_Impl::onChange, this, &ModelObjectTreeItem::change);

  connect(m_modelObject->getImpl<model::detail::ModelObject_Impl>().get(), &model::detail::ModelObject_Impl::onRelationshipChange, this, &ModelObjectTreeItem::changeRelationship);
}

ModelObjectTreeItem::ModelObjectTreeItem(const std::string& name, const openstudio::model::Model& model, QTreeWidgetItem* parent)
  : QTreeWidgetItem(parent), m_model(model), m_name(name), m_dirty(false)
{
  m_item = nullptr;

  this->setText(0, toQString(name));
  this->setStyle(0, "");
}

ModelObjectTreeItem::~ModelObjectTreeItem()
{
  if (m_item){
    delete m_item;
  }
}

boost::optional<openstudio::Handle> ModelObjectTreeItem::handle() const
{
  return m_handle;
}

boost::optional<openstudio::model::ModelObject> ModelObjectTreeItem::modelObject() const
{
  return m_modelObject;
}

openstudio::model::Model ModelObjectTreeItem::model() const
{
  return m_model;
}

std::string ModelObjectTreeItem::name() const
{
  if (m_modelObject){
    return m_modelObject->name().get();
  }
  return m_name;
}

OSItem* ModelObjectTreeItem::item() const
{
  return m_item;
}

std::vector<ModelObjectTreeItem*> ModelObjectTreeItem::children() const
{
  std::vector<ModelObjectTreeItem*> result;

  int n = this->childCount();
  for (int i = 0; i < n; ++i){
    QTreeWidgetItem* child = this->child(i);
    ModelObjectTreeItem* modelObjectTreeItem = dynamic_cast<ModelObjectTreeItem*>(child);
    OS_ASSERT(modelObjectTreeItem);
    result.push_back(modelObjectTreeItem);  
  }

  return result;
}

std::vector<ModelObjectTreeItem*> ModelObjectTreeItem::recursiveChildren() const
{
  std::vector<ModelObjectTreeItem*> result;

  int n = this->childCount();
  for (int i = 0; i < n; ++i){
    QTreeWidgetItem* child = this->child(i);
    ModelObjectTreeItem* modelObjectTreeItem = dynamic_cast<ModelObjectTreeItem*>(child);
    OS_ASSERT(modelObjectTreeItem);
    result.push_back(modelObjectTreeItem);  

    std::vector<ModelObjectTreeItem*> childChildren = modelObjectTreeItem->recursiveChildren();
    result.insert(result.end(), childChildren.begin(), childChildren.end());
  }

  return result;
}

void ModelObjectTreeItem::setStyle(int headerLevel, const QString& color)
{
  if (this->modelObject()){
    //this->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    this->setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    static QIcon defaultIcon(":images/bug.png");

    QIcon icon(defaultIcon);
    const QPixmap* pixMap = IconLibrary::Instance().findMiniIcon(this->modelObject()->iddObjectType().value());
    if (pixMap){
      icon = QIcon(*pixMap);
    }
    this->setIcon(0, icon);

  }else{
    this->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    this->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled);

    static QIcon icon(":images/mini_icons/folder.png");
    this->setIcon(0, icon);
  }

  QFont font;
  if (headerLevel == 1){
    font.setBold(true);
    font.setPixelSize(14);
  }else if(headerLevel == 2){
    font.setBold(true);
    font.setPixelSize(12);
  }else{
    font.setPixelSize(12);
  }
  this->setFont(0, font);

  QBrush brush(Qt::black, Qt::SolidPattern);
  if (!color.isEmpty()){
    brush = QBrush(color, Qt::SolidPattern);
  }
  this->setForeground(0, brush);
}

bool ModelObjectTreeItem::isDirty()
{
  return m_dirty;
}

void ModelObjectTreeItem::makeDirty()
{
  m_dirty = true;
}

void ModelObjectTreeItem::refresh()
{
  m_dirty = false;

  std::vector<std::string> nonModelObjectChildren = this->nonModelObjectChildren();
  std::vector<model::ModelObject> defaultedModelObjectChildren = this->defaultedModelObjectChildren();
  std::vector<model::ModelObject> modelObjectChildren = this->modelObjectChildren();
  std::vector<QTreeWidgetItem*> childrenToRemove;

  std::set<std::string> nonModelObjectChildrenSet;
  for (const std::string& child : nonModelObjectChildren){
    nonModelObjectChildrenSet.insert(child);
  }

  std::set<openstudio::Handle> allModelObjectChildrenHandleSet;
  for (const model::ModelObject& child : defaultedModelObjectChildren){
    allModelObjectChildrenHandleSet.insert(child.handle());
  }
  for (const model::ModelObject& child : modelObjectChildren){
    allModelObjectChildrenHandleSet.insert(child.handle());
  }

  int n = this->childCount();
  for (int i = 0; i < n; ++i){
    QTreeWidgetItem* child = this->child(i);
    ModelObjectTreeItem* modelObjectTreeItem = dynamic_cast<ModelObjectTreeItem*>(child);
    OS_ASSERT(modelObjectTreeItem);

    boost::optional<openstudio::model::ModelObject> modelObject = modelObjectTreeItem->modelObject();
    if (modelObject){
      if (allModelObjectChildrenHandleSet.find(modelObject->handle()) != allModelObjectChildrenHandleSet.end()){
        // this item's model object is for a model object we should have
        modelObjectTreeItem->refresh();

        // erase from set so we don't add later
        allModelObjectChildrenHandleSet.erase(modelObject->handle());
      }else{
        // this item's model object is for a model object we should not have

        // add it to list to remove
        childrenToRemove.push_back(child);
      }
    }else{
      if (nonModelObjectChildrenSet.find(modelObjectTreeItem->name()) != nonModelObjectChildrenSet.end()){
        // this item's name is a name we should have
        modelObjectTreeItem->refresh();

        // erase from set so we don't add later
        nonModelObjectChildrenSet.erase(modelObjectTreeItem->name());
      }else{
        // this item's name is not a name we should not have

        // add it to list to remove
        childrenToRemove.push_back(child);
      }
    }
  }

  for (QTreeWidgetItem* child : childrenToRemove){
    this->removeChild(child);
    delete child;
  }

  // todo: insert in correct order

  for (const std::string& child : nonModelObjectChildren){
    // still need this object
    if (nonModelObjectChildrenSet.find(child) != nonModelObjectChildrenSet.end()){
      nonModelObjectChildrenSet.erase(child);
      this->addNonModelObjectChild(child);
    }
  }
  for (const model::ModelObject& child : defaultedModelObjectChildren){
    // still need this object
    if (allModelObjectChildrenHandleSet.find(child.handle()) != allModelObjectChildrenHandleSet.end()){
      allModelObjectChildrenHandleSet.erase(child.handle());
      this->addModelObjectChild(child, true);
    }
  }
  for (const model::ModelObject& child : modelObjectChildren){
    // still need this object
    if (allModelObjectChildrenHandleSet.find(child.handle()) != allModelObjectChildrenHandleSet.end()){
      allModelObjectChildrenHandleSet.erase(child.handle());
      this->addModelObjectChild(child, false);
    }
  }

  finalize();
}

void ModelObjectTreeItem::refreshTree()
{
  // refresh each top level object
  QTreeWidget* treeWidget = this->treeWidget();
  
  int N = treeWidget->topLevelItemCount();
  for (int i = 0; i < N; ++i){
    QTreeWidgetItem* treeItem = treeWidget->topLevelItem(i);
    ModelObjectTreeItem* modelObjectTreeItem = dynamic_cast<ModelObjectTreeItem*>(treeItem);
    OS_ASSERT(modelObjectTreeItem);

    if (!modelObjectTreeItem->isDirty()){
      modelObjectTreeItem->makeDirty();
      QTimer::singleShot(0, modelObjectTreeItem, SLOT(refresh()));
    }
  }

}

void ModelObjectTreeItem::change()
{
  boost::optional<model::ModelObject> modelObject = this->modelObject();
  OS_ASSERT(modelObject);

  // these objects have 'type' fields that are not relationships but change tree structure
  switch (modelObject->iddObjectType().value()){
    case IddObjectType::OS_ShadingSurfaceGroup:
      refreshTree();
      break;
    case IddObjectType::OS_InteriorPartitionSurfaceGroup:
      refreshTree();
      break;
    case IddObjectType::OS_Surface:
      refreshTree();
      break;
    case IddObjectType::OS_SubSurface:
      refreshTree();
      break;
    default:
      ;
  }
}

void ModelObjectTreeItem::changeRelationship(int index, Handle newHandle, Handle oldHandle)
{
  if (newHandle == oldHandle){
    return;
  }

  boost::optional<model::ModelObject> modelObject = this->modelObject();
  OS_ASSERT(modelObject);

  switch (modelObject->iddObjectType().value()){
    case IddObjectType::OS_Building:
      if (index == OS_BuildingFields::SpaceTypeName){
        refreshTree();
      }
      break;
    case IddObjectType::OS_BuildingStory:
      break;
    case IddObjectType::OS_ThermalZone:
      break;
    case IddObjectType::OS_SpaceType:
      if (index == OS_SpaceTypeFields::DesignSpecificationOutdoorAirObjectName){
        refreshTree();
      }
      break;
    case IddObjectType::OS_Space:
      if (index == OS_SpaceFields::BuildingStoryName ||
          index == OS_SpaceFields::SpaceTypeName ||
          index == OS_SpaceFields::ThermalZoneName ||
          index == OS_SpaceFields::DesignSpecificationOutdoorAirObjectName){
        refreshTree();
      }
      break;
    case IddObjectType::OS_ShadingSurfaceGroup:
      // handle in onChange
      break;
    case IddObjectType::OS_ShadingSurface:
      if (index == OS_ShadingSurfaceFields::ShadingSurfaceGroupName){
        refreshTree();
      }
      break;
    case IddObjectType::OS_InteriorPartitionSurfaceGroup:
      // handle in onChange
      break;
    case IddObjectType::OS_InteriorPartitionSurface:
      if (index == OS_InteriorPartitionSurfaceFields::InteriorPartitionSurfaceGroupName){
        refreshTree();
      }
      break;
    case IddObjectType::OS_Surface:
      // handle in onChange
      if (index == OS_SurfaceFields::SpaceName){
        refreshTree();
      }
      break;
    case IddObjectType::OS_SubSurface:
      // handle in onChange
      break;
    case IddObjectType::OS_Daylighting_Control:
      if (index == OS_Daylighting_ControlFields::SpaceName){
        refreshTree();
      }
      break;
    case IddObjectType::OS_IlluminanceMap:
      if (index == OS_IlluminanceMapFields::SpaceName){
        refreshTree();
      }
      break;
    case IddObjectType::OS_InternalMass:
      if (index == OS_InternalMassFields::SpaceorSpaceTypeName){
        refreshTree();
      }
      break;
    case IddObjectType::OS_People:
      if (index == OS_PeopleFields::SpaceorSpaceTypeName){
        refreshTree();
      }
      break;
    case IddObjectType::OS_Lights:
      if (index == OS_LightsFields::SpaceorSpaceTypeName){
        refreshTree();
      }
      break;
    case IddObjectType::OS_Luminaire:
      if (index == OS_LuminaireFields::SpaceorSpaceTypeName){
        refreshTree();
      }
      break;
    case IddObjectType::OS_ElectricEquipment:
      if (index == OS_ElectricEquipmentFields::SpaceorSpaceTypeName){
        refreshTree();
      }
      break;
    case IddObjectType::OS_GasEquipment:
      if (index == OS_GasEquipmentFields::SpaceorSpaceTypeName){
        refreshTree();
      }
      break;
    case IddObjectType::OS_HotWaterEquipment:
      if (index == OS_HotWaterEquipmentFields::SpaceorSpaceTypeName){
        refreshTree();
      }
      break;
    case IddObjectType::OS_SteamEquipment:
      if (index == OS_SteamEquipmentFields::SpaceorSpaceTypeName){
        refreshTree();
      }
      break;
    case IddObjectType::OS_OtherEquipment:
      if (index == OS_OtherEquipmentFields::SpaceorSpaceTypeName){
        refreshTree();
      }
      break;
    case IddObjectType::OS_SpaceInfiltration_DesignFlowRate:
      if (index == OS_SpaceInfiltration_DesignFlowRateFields::SpaceorSpaceTypeName){
        refreshTree();
      }
      break;
    case IddObjectType::OS_SpaceInfiltration_EffectiveLeakageArea:
      if (index == OS_SpaceInfiltration_EffectiveLeakageAreaFields::SpaceorSpaceTypeName){
        refreshTree();
      }
      break;
    case IddObjectType::OS_DesignSpecification_OutdoorAir:
      break;
    default:
      refreshTree();
  }
}

void ModelObjectTreeItem::makeChildren()
{
  for (const std::string& child : this->nonModelObjectChildren()){
    this->addNonModelObjectChild(child);
  }
  for (const model::ModelObject& child : this->defaultedModelObjectChildren()){
    this->addModelObjectChild(child, true);
  }
  for (const model::ModelObject& child : this->modelObjectChildren()){
    this->addModelObjectChild(child, false);
  }

  finalize();
}

std::vector<std::string> ModelObjectTreeItem::nonModelObjectChildren() const
{
  return std::vector<std::string>();
}

std::vector<model::ModelObject> ModelObjectTreeItem::modelObjectChildren() const
{
  return std::vector<model::ModelObject>();
}

std::vector<model::ModelObject> ModelObjectTreeItem::defaultedModelObjectChildren() const
{
  return std::vector<model::ModelObject>();
}
    
void ModelObjectTreeItem::addNonModelObjectChild(const std::string& child)
{
  ModelObjectTreeItem* treeItem = new ModelObjectTreeItem(child, this->model(), this);
  this->addChild(treeItem);
}

void ModelObjectTreeItem::addModelObjectChild(const model::ModelObject& child, bool isDefaulted)
{
  auto treeItem = new ModelObjectTreeItem(child, isDefaulted, m_type, this);
  this->addChild(treeItem);
  if (isDefaulted){
    treeItem->setStyle(0, "#006837");
  }
}

void ModelObjectTreeItem::finalize()
{
}

void ModelObjectTreeItem::changeName()
{
  boost::optional<model::ModelObject> modelObject = this->modelObject();
  if (modelObject){
    this->setText(0, toQString(modelObject->name().get()));
  }
}

///////////////////// SiteShading ////////////////////////////////////////////////

SiteShadingTreeItem::SiteShadingTreeItem(const openstudio::model::Model& model, 
                                         QTreeWidgetItem* parent )
                                         : ModelObjectTreeItem(SiteShadingTreeItem::itemName(), model, parent)
{
  this->setStyle(1, "");
  this->makeChildren();
}

std::string SiteShadingTreeItem::itemName()
{
  return "Site Shading";
}

std::vector<model::ModelObject> SiteShadingTreeItem::modelObjectChildren() const
{
  std::vector<model::ModelObject> result;
  for (const model::ShadingSurfaceGroup& shadingSurfaceGroup : this->model().getConcreteModelObjects<model::ShadingSurfaceGroup>()){
    if (openstudio::istringEqual("Site", shadingSurfaceGroup.shadingSurfaceType())){
      result.push_back(shadingSurfaceGroup);
    }
  }
  std::sort(result.begin(), result.end(), WorkspaceObjectNameLess());
  return result;
}

void SiteShadingTreeItem::addModelObjectChild(const model::ModelObject& child, bool isDefaulted)
{
  if (child.optionalCast<model::ShadingSurfaceGroup>()){
    ShadingSurfaceGroupTreeItem* treeItem = new ShadingSurfaceGroupTreeItem(child.cast<model::ShadingSurfaceGroup>(), this);
    this->addChild(treeItem);
    treeItem->setStyle(2, "");
  }else{
    OS_ASSERT(false);
  }
}


///////////////////// ShadingSurfaceGroup ////////////////////////////////////////////////

ShadingSurfaceGroupTreeItem::ShadingSurfaceGroupTreeItem(const openstudio::model::ShadingSurfaceGroup& shadingSurfaceGroup, 
                                                         QTreeWidgetItem* parent )
  : ModelObjectTreeItem(shadingSurfaceGroup, false, m_type, parent)
{
  this->makeChildren();
}

std::vector<model::ModelObject> ShadingSurfaceGroupTreeItem::modelObjectChildren() const
{
  model::ShadingSurfaceGroup shadingSurfaceGroup = this->modelObject()->cast<model::ShadingSurfaceGroup>();
  std::vector<model::ShadingSurface> shadingSurfaces = shadingSurfaceGroup.shadingSurfaces();

  std::vector<model::ModelObject> result(shadingSurfaces.begin(), shadingSurfaces.end());
  std::sort(result.begin(), result.end(), WorkspaceObjectNameLess());

  return result;
}

void ShadingSurfaceGroupTreeItem::addModelObjectChild(const model::ModelObject& child, bool isDefaulted)
{
  if (child.optionalCast<model::ShadingSurface>()){
    auto treeItem = new ModelObjectTreeItem(child, false, m_type, this);
    this->addChild(treeItem);
  }else{
    OS_ASSERT(false);
  }
}

///////////////////// Building ////////////////////////////////////////////////

BuildingTreeItem::BuildingTreeItem(const openstudio::model::Building& building, 
                                   const openstudio::IddObjectType& sortByType,
                                   QTreeWidgetItem* parent)
  : ModelObjectTreeItem(building, false, m_type, parent), m_sortByType(sortByType)
{
  this->setStyle(1, "");
  this->makeChildren();
}

std::vector<model::ModelObject> BuildingTreeItem::modelObjectChildren() const
{
  openstudio::model::Model model = this->model();
  std::vector<model::ModelObject> result;

  if (m_sortByType == IddObjectType::OS_BuildingStory){
    std::vector<model::BuildingStory> buildingStories = model.getConcreteModelObjects<model::BuildingStory>();
    result.insert(result.end(), buildingStories.begin(), buildingStories.end());
  }else if (m_sortByType == IddObjectType::OS_ThermalZone){
    std::vector<model::ThermalZone> thermalZones = model.getConcreteModelObjects<model::ThermalZone>();
    result.insert(result.end(), thermalZones.begin(), thermalZones.end());
  }else if (m_sortByType == IddObjectType::OS_SpaceType){
    std::vector<model::SpaceType> spaceTypes = model.getConcreteModelObjects<model::SpaceType>();
    result.insert(result.end(), spaceTypes.begin(), spaceTypes.end());
  }

  std::sort(result.begin(), result.end(), WorkspaceObjectNameLess());

  return result;
}

void BuildingTreeItem::addModelObjectChild(const model::ModelObject& child, bool isDefaulted)
{
  if (child.optionalCast<model::BuildingStory>()){
    BuildingStoryTreeItem* treeItem = new BuildingStoryTreeItem(child.cast<model::BuildingStory>(), this);
    this->addChild(treeItem);
  }else if (child.optionalCast<model::ThermalZone>()){
    ThermalZoneTreeItem* treeItem = new ThermalZoneTreeItem(child.cast<model::ThermalZone>(), this);
    this->addChild(treeItem);
  }else if (child.optionalCast<model::SpaceType>()){
    SpaceTypeTreeItem* treeItem = new SpaceTypeTreeItem(child.cast<model::SpaceType>(), this);
    this->addChild(treeItem);
  }else{
    OS_ASSERT(false);
  }
}

std::vector<std::string> BuildingTreeItem::nonModelObjectChildren() const
{
  std::vector<std::string> result;
  result.push_back(BuildingShadingTreeItem::itemName());

  if (m_sortByType == IddObjectType::OS_BuildingStory){
    result.push_back(NoBuildingStoryTreeItem::itemName());
  }else if (m_sortByType == IddObjectType::OS_ThermalZone){
    result.push_back(NoThermalZoneTreeItem::itemName());
  }else if (m_sortByType == IddObjectType::OS_SpaceType){
    result.push_back(NoSpaceTypeTreeItem::itemName());
  }

  return result;
}

void BuildingTreeItem::addNonModelObjectChild(const std::string& child)
{
  if (child == BuildingShadingTreeItem::itemName()){
    BuildingShadingTreeItem* treeItem = new BuildingShadingTreeItem(this->model(), this);
    this->addChild(treeItem);
  }else if (child == NoBuildingStoryTreeItem::itemName()){
    NoBuildingStoryTreeItem* treeItem = new NoBuildingStoryTreeItem(this->model(), this);
    this->addChild(treeItem);
  }else if (child == NoThermalZoneTreeItem::itemName()){
    NoThermalZoneTreeItem* treeItem = new NoThermalZoneTreeItem(this->model(), this);
    this->addChild(treeItem);
  }else if (child == NoSpaceTypeTreeItem::itemName()){
    NoSpaceTypeTreeItem* treeItem = new NoSpaceTypeTreeItem(this->model(), this);
    this->addChild(treeItem);
  }else{
    OS_ASSERT(false);
  }
}


///////////////////// BuildingShading ////////////////////////////////////////////////

BuildingShadingTreeItem::BuildingShadingTreeItem(const openstudio::model::Model& model, 
                                         QTreeWidgetItem* parent )
                                         : ModelObjectTreeItem(BuildingShadingTreeItem::itemName(), model, parent)
{
  this->setStyle(2, "");
  this->makeChildren();
}

std::string BuildingShadingTreeItem::itemName()
{
  return "Building Shading";
}

std::vector<model::ModelObject> BuildingShadingTreeItem::modelObjectChildren() const
{
  std::vector<model::ModelObject> result;
  for (const model::ShadingSurfaceGroup& shadingSurfaceGroup : this->model().getConcreteModelObjects<model::ShadingSurfaceGroup>()){
    if (openstudio::istringEqual("Building", shadingSurfaceGroup.shadingSurfaceType())){
      result.push_back(shadingSurfaceGroup);
    }
  }
  std::sort(result.begin(), result.end(), WorkspaceObjectNameLess());
  return result;
}

void BuildingShadingTreeItem::addModelObjectChild(const model::ModelObject& child, bool isDefaulted)
{
  if (child.optionalCast<model::ShadingSurfaceGroup>()){
    ShadingSurfaceGroupTreeItem* treeItem = new ShadingSurfaceGroupTreeItem(child.cast<model::ShadingSurfaceGroup>(), this);
    this->addChild(treeItem);
  }else{
    OS_ASSERT(false);
  }
}

///////////////////// BuildingStory ////////////////////////////////////////////////

BuildingStoryTreeItem::BuildingStoryTreeItem(const openstudio::model::BuildingStory& buildingStory, QTreeWidgetItem* parent)
  : ModelObjectTreeItem(buildingStory, false, m_type, parent)
{
  this->setStyle(2, "");
  this->makeChildren();
}

std::vector<model::ModelObject> BuildingStoryTreeItem::modelObjectChildren() const
{
  boost::optional<model::ModelObject> modelObject = this->modelObject();
  OS_ASSERT(modelObject);
  std::vector<model::Space> spaces = modelObject->getModelObjectSources<model::Space>();
  std::vector<model::ModelObject> result(spaces.begin(), spaces.end());
  std::sort(result.begin(), result.end(), WorkspaceObjectNameLess());
  return result;
}

void BuildingStoryTreeItem::addModelObjectChild(const model::ModelObject& child, bool isDefaulted)
{
  if (child.optionalCast<model::Space>()){
    SpaceTreeItem* treeItem = new SpaceTreeItem(child.cast<model::Space>(), this);
    this->addChild(treeItem);
    if (isDefaulted){
      treeItem->setStyle(0, "#006837");
    }
  }else{
    OS_ASSERT(false);
  }
}

NoBuildingStoryTreeItem::NoBuildingStoryTreeItem(const openstudio::model::Model& model, QTreeWidgetItem* parent)
  : ModelObjectTreeItem(NoBuildingStoryTreeItem::itemName(), model, parent)
{
  this->makeChildren();
}

std::string NoBuildingStoryTreeItem::itemName()
{
  return "Unassigned Building Story";
}

std::vector<model::ModelObject> NoBuildingStoryTreeItem::modelObjectChildren() const
{
  model::Model model = this->model();
  std::vector<model::ModelObject> result;
  for (const model::Space& space : model.getConcreteModelObjects<model::Space>()){
    if (!space.buildingStory()){
      result.push_back(space);
    }
  }
  std::sort(result.begin(), result.end(), WorkspaceObjectNameLess());
  return result;
}

void NoBuildingStoryTreeItem::addModelObjectChild(const model::ModelObject& child, bool isDefaulted)
{
  if (child.optionalCast<model::Space>()){
    SpaceTreeItem* treeItem = new SpaceTreeItem(child.cast<model::Space>(), this);
    this->addChild(treeItem);
    if (isDefaulted){
      treeItem->setStyle(0, "#006837");
    }
  }else{
    OS_ASSERT(false);
  }
}

void NoBuildingStoryTreeItem::finalize()
{
  if (this->childCount() == 0){
    this->setDisabled(true);
    this->setStyle(2, "");
  }else{
    this->setDisabled(false);
    this->setStyle(2, "#F15A24");
  }
}

///////////////////// ThermalZone ////////////////////////////////////////////////

ThermalZoneTreeItem::ThermalZoneTreeItem(const openstudio::model::ThermalZone& thermalZone, QTreeWidgetItem* parent)
  : ModelObjectTreeItem(thermalZone, false, m_type, parent)
{
  this->setStyle(2, "");
  this->makeChildren();
}

std::vector<model::ModelObject> ThermalZoneTreeItem::modelObjectChildren() const
{
  boost::optional<model::ModelObject> modelObject = this->modelObject();
  OS_ASSERT(modelObject);
  std::vector<model::Space> spaces = modelObject->getModelObjectSources<model::Space>();
  std::vector<model::ModelObject> result(spaces.begin(), spaces.end());
  std::sort(result.begin(), result.end(), WorkspaceObjectNameLess());
  return result;
}

void ThermalZoneTreeItem::addModelObjectChild(const model::ModelObject& child, bool isDefaulted)
{
  if (child.optionalCast<model::Space>()){
    SpaceTreeItem* treeItem = new SpaceTreeItem(child.cast<model::Space>(), this);
    this->addChild(treeItem);
    if (isDefaulted){
      treeItem->setStyle(0, "#006837");
    }
  }else{
    OS_ASSERT(false);
  }
}

NoThermalZoneTreeItem::NoThermalZoneTreeItem(const openstudio::model::Model& model, QTreeWidgetItem* parent)
  : ModelObjectTreeItem(NoThermalZoneTreeItem::itemName(), model, parent)
{
  this->setStyle(2, "#F15A24");
  this->makeChildren();
}

std::string NoThermalZoneTreeItem::itemName()
{
  return "Unassigned Thermal Zone";
}

std::vector<model::ModelObject> NoThermalZoneTreeItem::modelObjectChildren() const
{
  model::Model model = this->model();
  std::vector<model::ModelObject> result;
  for (const model::Space& space : model.getConcreteModelObjects<model::Space>()){
    if (!space.thermalZone()){
      result.push_back(space);
    }
  }
  std::sort(result.begin(), result.end(), WorkspaceObjectNameLess());
  return result;
}

void NoThermalZoneTreeItem::addModelObjectChild(const model::ModelObject& child, bool isDefaulted)
{
  if (child.optionalCast<model::Space>()){
    SpaceTreeItem* treeItem = new SpaceTreeItem(child.cast<model::Space>(), this);
    this->addChild(treeItem);
    if (isDefaulted){
      treeItem->setStyle(0, "#006837");
    }
  }else{
    OS_ASSERT(false);
  }
}

void NoThermalZoneTreeItem::finalize()
{
  if (this->childCount() == 0){
    this->setDisabled(true);
    this->setStyle(2, "");
  }else{
    this->setDisabled(false);
    this->setStyle(2, "#F15A24");
  }
}

///////////////////// SpaceType ////////////////////////////////////////////////

SpaceTypeTreeItem::SpaceTypeTreeItem(const openstudio::model::SpaceType& spaceType, QTreeWidgetItem* parent)
  : ModelObjectTreeItem(spaceType, false, m_type, parent)
{
  this->setStyle(2, "");
  this->makeChildren();
}

std::vector<model::ModelObject> SpaceTypeTreeItem::modelObjectChildren() const
{
  boost::optional<model::ModelObject> modelObject = this->modelObject();
  OS_ASSERT(modelObject);
  std::vector<model::Space> spaces = modelObject->getModelObjectSources<model::Space>();
  std::vector<model::ModelObject> result(spaces.begin(), spaces.end());
  std::sort(result.begin(), result.end(), WorkspaceObjectNameLess());
  return result;
}

std::vector<model::ModelObject> SpaceTypeTreeItem::defaultedModelObjectChildren() const
{
  std::vector<model::ModelObject> result;

  boost::optional<model::ModelObject> modelObject = this->modelObject();
  OS_ASSERT(modelObject);
  model::SpaceType spaceType = modelObject->cast<model::SpaceType>();
  
  // get spaces that inherit this space type as default
  for (const model::Space& space : spaceType.spaces()){
    if (space.isSpaceTypeDefaulted()){
      result.push_back(space);
    }
  }

  std::sort(result.begin(), result.end(), WorkspaceObjectNameLess());
  return result;
}

void SpaceTypeTreeItem::addModelObjectChild(const model::ModelObject& child, bool isDefaulted)
{
  if (child.optionalCast<model::Space>()){
    SpaceTreeItem* treeItem = new SpaceTreeItem(child.cast<model::Space>(), this);
    this->addChild(treeItem);
    if (isDefaulted){
      treeItem->setStyle(0, "#006837");
    }
  }else{
    OS_ASSERT(false);
  }
}

NoSpaceTypeTreeItem::NoSpaceTypeTreeItem(const openstudio::model::Model& model, QTreeWidgetItem* parent)
  : ModelObjectTreeItem(NoSpaceTypeTreeItem::itemName(), model, parent)
{
  this->makeChildren();
}

std::string NoSpaceTypeTreeItem::itemName()
{
  return "Unassigned Space Type";
}

std::vector<model::ModelObject> NoSpaceTypeTreeItem::modelObjectChildren() const
{
  model::Model model = this->model();
  std::vector<model::ModelObject> result;
  for (const model::Space& space : model.getConcreteModelObjects<model::Space>()){
    if (!space.spaceType()){
      result.push_back(space);
    }
  }
  std::sort(result.begin(), result.end(), WorkspaceObjectNameLess());
  return result;
}

void NoSpaceTypeTreeItem::addModelObjectChild(const model::ModelObject& child, bool isDefaulted)
{
  if (child.optionalCast<model::Space>()){
    SpaceTreeItem* treeItem = new SpaceTreeItem(child.cast<model::Space>(), this);
    this->addChild(treeItem);
    if (isDefaulted){
      treeItem->setStyle(0, "#006837");
    }
  }else{
    OS_ASSERT(false);
  }
}

void NoSpaceTypeTreeItem::finalize()
{
  if (this->childCount() == 0){
    this->setDisabled(true);
    this->setStyle(2, "");
  }else{
    this->setDisabled(false);
    this->setStyle(2, "#F15A24");
  }
}

///////////////////// Space ////////////////////////////////////////////////

SpaceTreeItem::SpaceTreeItem(const openstudio::model::Space& space, QTreeWidgetItem* parent)
  : ModelObjectTreeItem(space, false, m_type, parent)
{
  this->makeChildren();
}

std::vector<std::string> SpaceTreeItem::nonModelObjectChildren() const
{
  std::vector<std::string> result;
  result.push_back(RoofsTreeItem::itemName());
  result.push_back(WallsTreeItem::itemName());
  result.push_back(FloorsTreeItem::itemName());
  result.push_back(SpaceShadingTreeItem::itemName());
  result.push_back(InteriorPartitionsTreeItem::itemName());
  result.push_back(DaylightingObjectsTreeItem::itemName());
  result.push_back(LoadsTreeItem::itemName());
  return result;
}

void SpaceTreeItem::addNonModelObjectChild(const std::string& child)
{
  boost::optional<model::ModelObject> modelObject = this->modelObject();
  OS_ASSERT(modelObject);
  model::Space space = modelObject->cast<model::Space>();

  if (child == RoofsTreeItem::itemName()){
    auto treeItem = new RoofsTreeItem(space, this);
    this->addChild(treeItem);
  }else if (child == WallsTreeItem::itemName()){
    auto treeItem = new WallsTreeItem(space, this);
    this->addChild(treeItem);
  }else if (child == FloorsTreeItem::itemName()){
    auto treeItem = new FloorsTreeItem(space, this);
    this->addChild(treeItem);
  }else if (child == SpaceShadingTreeItem::itemName()){
    auto treeItem = new SpaceShadingTreeItem(space, this);
    this->addChild(treeItem);
  }else if (child == InteriorPartitionsTreeItem::itemName()){
    auto treeItem = new InteriorPartitionsTreeItem(space, this);
    this->addChild(treeItem);
  }else if (child == DaylightingObjectsTreeItem::itemName()){
    auto treeItem = new DaylightingObjectsTreeItem(space, this);
    this->addChild(treeItem);
  }else if (child == LoadsTreeItem::itemName()){
    auto treeItem = new LoadsTreeItem(space, this);
    this->addChild(treeItem);
  }else{
    OS_ASSERT(false);
  }
}

///////////////////// Roofs ////////////////////////////////////////////////

RoofsTreeItem::RoofsTreeItem(const openstudio::model::Space& space, QTreeWidgetItem* parent)
  : ModelObjectTreeItem(RoofsTreeItem::itemName(), space.model(), parent), m_space(space)
{
  this->makeChildren();
}

std::string RoofsTreeItem::itemName()
{
  return "Roof/Ceilings";
}

std::vector<model::ModelObject> RoofsTreeItem::modelObjectChildren() const
{
  std::vector<model::ModelObject> result;
  for (const model::Surface& surface : m_space.surfaces()){
    if (istringEqual("RoofCeiling", surface.surfaceType())){
      result.push_back(surface);
    }
  }
  std::sort(result.begin(), result.end(), WorkspaceObjectNameLess());
  return result;
}

void RoofsTreeItem::addModelObjectChild(const model::ModelObject& child, bool isDefaulted)
{
  if (child.optionalCast<model::Surface>()){
    SurfaceTreeItem* treeItem = new SurfaceTreeItem(child.cast<model::Surface>(), this);
    this->addChild(treeItem);
  }else{
    OS_ASSERT(false);
  }
}

///////////////////// Walls ////////////////////////////////////////////////

WallsTreeItem::WallsTreeItem(const openstudio::model::Space& space, QTreeWidgetItem* parent)
  : ModelObjectTreeItem(WallsTreeItem::itemName(), space.model(), parent), m_space(space)
{
  this->makeChildren();
}

std::string WallsTreeItem::itemName()
{
  return "Walls";
}

std::vector<model::ModelObject> WallsTreeItem::modelObjectChildren() const
{
  std::vector<model::ModelObject> result;
  for (const model::Surface& surface : m_space.surfaces()){
    if (istringEqual("Wall", surface.surfaceType())){
      result.push_back(surface);
    }
  }
  std::sort(result.begin(), result.end(), WorkspaceObjectNameLess());
  return result;
}

void WallsTreeItem::addModelObjectChild(const model::ModelObject& child, bool isDefaulted)
{
  if (child.optionalCast<model::Surface>()){
    SurfaceTreeItem* treeItem = new SurfaceTreeItem(child.cast<model::Surface>(), this);
    this->addChild(treeItem);
  }else{
    OS_ASSERT(false);
  }
}

///////////////////// Floors ////////////////////////////////////////////////

FloorsTreeItem::FloorsTreeItem(const openstudio::model::Space& space, QTreeWidgetItem* parent)
  : ModelObjectTreeItem(FloorsTreeItem::itemName(), space.model(), parent), m_space(space)
{
  this->makeChildren();
}

std::string FloorsTreeItem::itemName()
{
  return "Floors";
}

std::vector<model::ModelObject> FloorsTreeItem::modelObjectChildren() const
{
  std::vector<model::ModelObject> result;
  for (const model::Surface& surface : m_space.surfaces()){
    if (istringEqual("Floor", surface.surfaceType())){
      result.push_back(surface);
    }
  }
  std::sort(result.begin(), result.end(), WorkspaceObjectNameLess());
  return result;
}

void FloorsTreeItem::addModelObjectChild(const model::ModelObject& child, bool isDefaulted)
{
  if (child.optionalCast<model::Surface>()){
    SurfaceTreeItem* treeItem = new SurfaceTreeItem(child.cast<model::Surface>(), this);
    this->addChild(treeItem);
  }else{
    OS_ASSERT(false);
  }
}

///////////////////// Surface ////////////////////////////////////////////////

SurfaceTreeItem::SurfaceTreeItem(const openstudio::model::Surface& surface, QTreeWidgetItem* parent)
  : ModelObjectTreeItem(surface, false, m_type, parent)
{
  this->makeChildren();
}

std::vector<model::ModelObject> SurfaceTreeItem::modelObjectChildren() const
{
  boost::optional<model::ModelObject> modelObject = this->modelObject();
  OS_ASSERT(modelObject);
  model::Surface surface = modelObject->cast<model::Surface>();

  std::vector<model::SubSurface> subSurfaces = surface.subSurfaces();
  std::vector<model::ModelObject> result(subSurfaces.begin(), subSurfaces.end());
  std::sort(result.begin(), result.end(), WorkspaceObjectNameLess());
  return result;
}


///////////////////// SpaceShading ////////////////////////////////////////////////

SpaceShadingTreeItem::SpaceShadingTreeItem(const openstudio::model::Space& space, QTreeWidgetItem* parent)
  : ModelObjectTreeItem(SpaceShadingTreeItem::itemName(), space.model(), parent), m_space(space)
{
  this->makeChildren();
}

std::string SpaceShadingTreeItem::itemName()
{
  return "Space Shading";
}

std::vector<model::ModelObject> SpaceShadingTreeItem::modelObjectChildren() const
{
  std::vector<model::ShadingSurfaceGroup> shadingSurfaceGroups = m_space.shadingSurfaceGroups();
  std::vector<model::ModelObject> result(shadingSurfaceGroups.begin(), shadingSurfaceGroups.end());
  std::sort(result.begin(), result.end(), WorkspaceObjectNameLess());
  return result;
}

void SpaceShadingTreeItem::addModelObjectChild(const model::ModelObject& child, bool isDefaulted)
{
  if (child.optionalCast<model::ShadingSurfaceGroup>()){
    ShadingSurfaceGroupTreeItem* treeItem = new ShadingSurfaceGroupTreeItem(child.cast<model::ShadingSurfaceGroup>(), this);
    this->addChild(treeItem);
  }else{
    OS_ASSERT(false);
  }
}

///////////////////// InteriorPartitions ////////////////////////////////////////////////

InteriorPartitionsTreeItem::InteriorPartitionsTreeItem(const openstudio::model::Space& space, QTreeWidgetItem* parent)
  : ModelObjectTreeItem(InteriorPartitionsTreeItem::itemName(), space.model(), parent), m_space(space)
{
  this->makeChildren();
}

std::string InteriorPartitionsTreeItem::itemName()
{
  return "Interior Partitions";
}

std::vector<model::ModelObject> InteriorPartitionsTreeItem::modelObjectChildren() const
{
  std::vector<model::InteriorPartitionSurfaceGroup> interiorPartitionSurfaceGroups = m_space.interiorPartitionSurfaceGroups();
  std::vector<model::ModelObject> result(interiorPartitionSurfaceGroups.begin(), interiorPartitionSurfaceGroups.end());
  std::sort(result.begin(), result.end(), WorkspaceObjectNameLess());
  return result;
}

void InteriorPartitionsTreeItem::addModelObjectChild(const model::ModelObject& child, bool isDefaulted)
{
  if (child.optionalCast<model::InteriorPartitionSurfaceGroup>()){
    InteriorPartitionSurfaceGroupTreeItem* treeItem = new InteriorPartitionSurfaceGroupTreeItem(child.cast<model::InteriorPartitionSurfaceGroup>(), this);
    this->addChild(treeItem);
  }else{
    OS_ASSERT(false);
  }
}

///////////////////// InteriorPartitionSurfaceGroup ////////////////////////////////////////////////

InteriorPartitionSurfaceGroupTreeItem::InteriorPartitionSurfaceGroupTreeItem(const openstudio::model::InteriorPartitionSurfaceGroup& interiorPartitionSurfaceGroup, 
                                                                             QTreeWidgetItem* parent)
  : ModelObjectTreeItem(interiorPartitionSurfaceGroup, false, m_type, parent)
{
  this->makeChildren();
}

std::vector<model::ModelObject> InteriorPartitionSurfaceGroupTreeItem::modelObjectChildren() const
{
  boost::optional<model::ModelObject> modelObject = this->modelObject();
  OS_ASSERT(modelObject);
  model::InteriorPartitionSurfaceGroup interiorPartitionSurfaceGroup = modelObject->cast<model::InteriorPartitionSurfaceGroup>();
  
  std::vector<model::InteriorPartitionSurface> interiorPartitionSurfaces = interiorPartitionSurfaceGroup.interiorPartitionSurfaces();
  std::vector<model::ModelObject> result(interiorPartitionSurfaces.begin(), interiorPartitionSurfaces.end());
  std::sort(result.begin(), result.end(), WorkspaceObjectNameLess());
  return result;
}

///////////////////// DaylightingControls ////////////////////////////////////////////////

DaylightingObjectsTreeItem::DaylightingObjectsTreeItem(const openstudio::model::Space& space, QTreeWidgetItem* parent)
  : ModelObjectTreeItem(DaylightingObjectsTreeItem::itemName(), space.model(), parent), m_space(space)
{
  this->makeChildren();
}

std::string DaylightingObjectsTreeItem::itemName()
{
  return "Daylighting Objects";
}

std::vector<model::ModelObject> DaylightingObjectsTreeItem::modelObjectChildren() const
{
  std::vector<model::DaylightingControl> daylightingControls = m_space.daylightingControls();
  std::sort(daylightingControls.begin(), daylightingControls.end(), WorkspaceObjectNameLess());
  std::vector<model::ModelObject> result(daylightingControls.begin(), daylightingControls.end());

  std::vector<model::GlareSensor> glareSensors = m_space.glareSensors();
  result.insert(result.end(), glareSensors.begin(), glareSensors.end());

  std::vector<model::IlluminanceMap> illuminanceMaps = m_space.illuminanceMaps();
  std::sort(illuminanceMaps.begin(), illuminanceMaps.end(), WorkspaceObjectNameLess());
  result.insert(result.end(), illuminanceMaps.begin(), illuminanceMaps.end());

  return result;
}

///////////////////// Loads ////////////////////////////////////////////////

LoadsTreeItem::LoadsTreeItem(const openstudio::model::Space& space, QTreeWidgetItem* parent)
  : ModelObjectTreeItem(LoadsTreeItem::itemName(), space.model(), parent), m_space(space)
{
  this->makeChildren();
}

std::string LoadsTreeItem::itemName()
{
  return "Loads";
}

std::vector<model::ModelObject> LoadsTreeItem::modelObjectChildren() const
{
  std::vector<model::ModelObject> result;

  boost::optional<model::DesignSpecificationOutdoorAir> designSpecificationOutdoorAir = m_space.designSpecificationOutdoorAir();
  if (designSpecificationOutdoorAir){
    if (!m_space.isDesignSpecificationOutdoorAirDefaulted()){
      result.push_back(*designSpecificationOutdoorAir);
    }
  }

  std::vector<model::SpaceInfiltrationDesignFlowRate> spaceInfiltrationDesignFlowRates = m_space.spaceInfiltrationDesignFlowRates();
  std::sort(spaceInfiltrationDesignFlowRates.begin(), spaceInfiltrationDesignFlowRates.end(), WorkspaceObjectNameLess());
  result.insert(result.end(), spaceInfiltrationDesignFlowRates.begin(), spaceInfiltrationDesignFlowRates.end());

  std::vector<model::SpaceInfiltrationEffectiveLeakageArea> spaceInfiltrationEffectiveLeakageAreas = m_space.spaceInfiltrationEffectiveLeakageAreas();
  std::sort(spaceInfiltrationEffectiveLeakageAreas.begin(), spaceInfiltrationEffectiveLeakageAreas.end(), WorkspaceObjectNameLess());
  result.insert(result.end(), spaceInfiltrationEffectiveLeakageAreas.begin(), spaceInfiltrationEffectiveLeakageAreas.end());

  std::vector<model::People> people = m_space.people();
  std::sort(people.begin(), people.end(), WorkspaceObjectNameLess());
  result.insert(result.end(), people.begin(), people.end());

  std::vector<model::Lights> lights = m_space.lights();
  std::sort(lights.begin(), lights.end(), WorkspaceObjectNameLess());
  result.insert(result.end(), lights.begin(), lights.end());

  std::vector<model::Luminaire> luminaires = m_space.luminaires();
  std::sort(luminaires.begin(), luminaires.end(), WorkspaceObjectNameLess());
  result.insert(result.end(), luminaires.begin(), luminaires.end());

  std::vector<model::ElectricEquipment> electricEquipment = m_space.electricEquipment();
  std::sort(electricEquipment.begin(), electricEquipment.end(), WorkspaceObjectNameLess());
  result.insert(result.end(), electricEquipment.begin(), electricEquipment.end());

  std::vector<model::GasEquipment> gasEquipment = m_space.gasEquipment();
  std::sort(gasEquipment.begin(), gasEquipment.end(), WorkspaceObjectNameLess());
  result.insert(result.end(), gasEquipment.begin(), gasEquipment.end());

  std::vector<model::SteamEquipment> steamEquipment = m_space.steamEquipment();
  std::sort(steamEquipment.begin(), steamEquipment.end(), WorkspaceObjectNameLess());
  result.insert(result.end(), steamEquipment.begin(), steamEquipment.end());

  std::vector<model::OtherEquipment> otherEquipment = m_space.otherEquipment();
  std::sort(otherEquipment.begin(), otherEquipment.end(), WorkspaceObjectNameLess());
  result.insert(result.end(), otherEquipment.begin(), otherEquipment.end());

  std::vector<model::InternalMass> internalMass = m_space.internalMass();
  std::sort(internalMass.begin(), internalMass.end(), WorkspaceObjectNameLess());
  result.insert(result.end(), internalMass.begin(), internalMass.end());

  return result;
}

std::vector<model::ModelObject> LoadsTreeItem::defaultedModelObjectChildren() const
{
  std::vector<model::ModelObject> result;

  boost::optional<model::SpaceType> spaceType = m_space.spaceType();
  if (spaceType){

    boost::optional<model::DesignSpecificationOutdoorAir> designSpecificationOutdoorAir = m_space.designSpecificationOutdoorAir();
    if (designSpecificationOutdoorAir){
      if (m_space.isDesignSpecificationOutdoorAirDefaulted()){
        result.push_back(*designSpecificationOutdoorAir);
      }
    }

    std::vector<model::SpaceInfiltrationDesignFlowRate> spaceInfiltrationDesignFlowRates = spaceType->spaceInfiltrationDesignFlowRates();
    std::sort(spaceInfiltrationDesignFlowRates.begin(), spaceInfiltrationDesignFlowRates.end(), WorkspaceObjectNameLess());
    result.insert(result.end(), spaceInfiltrationDesignFlowRates.begin(), spaceInfiltrationDesignFlowRates.end());

    std::vector<model::SpaceInfiltrationEffectiveLeakageArea> spaceInfiltrationEffectiveLeakageAreas = spaceType->spaceInfiltrationEffectiveLeakageAreas();
    std::sort(spaceInfiltrationEffectiveLeakageAreas.begin(), spaceInfiltrationEffectiveLeakageAreas.end(), WorkspaceObjectNameLess());
    result.insert(result.end(), spaceInfiltrationEffectiveLeakageAreas.begin(), spaceInfiltrationEffectiveLeakageAreas.end());

    std::vector<model::People> people = spaceType->people();
    std::sort(people.begin(), people.end(), WorkspaceObjectNameLess());
    result.insert(result.end(), people.begin(), people.end());

    std::vector<model::Lights> lights = spaceType->lights();
    std::sort(lights.begin(), lights.end(), WorkspaceObjectNameLess());
    result.insert(result.end(), lights.begin(), lights.end());

    std::vector<model::Luminaire> luminaires = spaceType->luminaires();
    std::sort(luminaires.begin(), luminaires.end(), WorkspaceObjectNameLess());
    result.insert(result.end(), luminaires.begin(), luminaires.end());

    std::vector<model::ElectricEquipment> electricEquipment = spaceType->electricEquipment();
    std::sort(electricEquipment.begin(), electricEquipment.end(), WorkspaceObjectNameLess());
    result.insert(result.end(), electricEquipment.begin(), electricEquipment.end());

    std::vector<model::GasEquipment> gasEquipment = spaceType->gasEquipment();
    std::sort(gasEquipment.begin(), gasEquipment.end(), WorkspaceObjectNameLess());
    result.insert(result.end(), gasEquipment.begin(), gasEquipment.end());

    std::vector<model::SteamEquipment> steamEquipment = spaceType->steamEquipment();
    std::sort(steamEquipment.begin(), steamEquipment.end(), WorkspaceObjectNameLess());
    result.insert(result.end(), steamEquipment.begin(), steamEquipment.end());

    std::vector<model::OtherEquipment> otherEquipment = spaceType->otherEquipment();
    std::sort(otherEquipment.begin(), otherEquipment.end(), WorkspaceObjectNameLess());
    result.insert(result.end(), otherEquipment.begin(), otherEquipment.end());

    std::vector<model::InternalMass> internalMass = spaceType->internalMass();
    std::sort(internalMass.begin(), internalMass.end(), WorkspaceObjectNameLess());
    result.insert(result.end(), internalMass.begin(), internalMass.end());

  }
  return result;
}

} // openstudio

