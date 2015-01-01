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

#include "FacilityTreeWidget.hpp"
#include "ModelObjectTreeItems.hpp"
#include "ModelObjectItem.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/Facility.hpp"
#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"
#include "../model/BuildingStory.hpp"
#include "../model/BuildingStory_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/SpaceType.hpp"
#include "../model/SpaceType_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Surface.hpp"
#include "../model/SubSurface.hpp"

#include "../utilities/core/Assert.hpp"

#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
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
#include <QHeaderView>
#include <QComboBox>

#include <utilities/idd/IddEnums.hxx>

#include <iostream>

namespace openstudio {

FacilityTreeWidget::FacilityTreeWidget(const model::Model& model, QWidget* parent )
  : ModelObjectTreeWidget(model, parent), m_sortByType(IddObjectType::OS_BuildingStory)
{ 
  this->setObjectName("GrayWidget"); 

  QVBoxLayout* vLayout = new QVBoxLayout();
  vLayout->setContentsMargins(0,0,0,0);

  QHBoxLayout* hLayout = new QHBoxLayout();
  hLayout->setContentsMargins(10,0,10,0);

  QLabel* comboLabel = new QLabel();
  comboLabel->setText("Sort Building by: ");
  comboLabel->setStyleSheet("QLabel { font: bold; }");
  hLayout->addWidget(comboLabel);

  QComboBox* comboBox = new QComboBox();
  comboBox->addItem("Building Story");
  comboBox->addItem("Thermal Zone");
  comboBox->addItem("Space Type");
  hLayout->addWidget(comboBox);

  vLayout->addLayout(hLayout);

  connect(comboBox, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this, &FacilityTreeWidget::onSortByChanged);

  this->vLayout()->insertLayout(0, vLayout);
  this->treeWidget()->header()->close();
  this->treeWidget()->setMinimumWidth(320);
  this->treeWidget()->setIndentation(10);
  this->treeWidget()->setSelectionBehavior(QAbstractItemView::SelectRows);
  this->treeWidget()->setSelectionMode(QAbstractItemView::SingleSelection);
  this->treeWidget()->setAlternatingRowColors(true);
  this->treeWidget()->setColumnCount(1);

  connect(this->treeWidget(), &QTreeWidget::currentItemChanged, this, &FacilityTreeWidget::currentItemChanged);

  // allow time for signals to be connected between this and inspector
  QTimer::singleShot(0, this, SLOT(initialize()));
}

IddObjectType FacilityTreeWidget::currentIddObjectType() const
{
  return m_iddObjectType;
}

boost::optional<openstudio::model::ModelObject> FacilityTreeWidget::selectedModelObject() const
{
  return m_selectedObject;
}

void FacilityTreeWidget::onObjectAdded(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::Handle& handle)
{
  if ((iddObjectType == IddObjectType::OS_Facility) ||
      (iddObjectType == IddObjectType::OS_ShadingSurfaceGroup) ||
      (iddObjectType == IddObjectType::OS_ShadingSurface) ||
      (iddObjectType == IddObjectType::OS_Building) ||
      (iddObjectType == IddObjectType::OS_BuildingStory) ||
      (iddObjectType == IddObjectType::OS_ThermalZone) ||
      (iddObjectType == IddObjectType::OS_SpaceType) ||
      (iddObjectType == IddObjectType::OS_Space) ||
      (iddObjectType == IddObjectType::OS_InteriorPartitionSurfaceGroup) ||
      (iddObjectType == IddObjectType::OS_InteriorPartitionSurface) ||
      (iddObjectType == IddObjectType::OS_Daylighting_Control) ||
      (iddObjectType == IddObjectType::OS_IlluminanceMap) ||
      (iddObjectType == IddObjectType::OS_InternalMass) ||
      (iddObjectType == IddObjectType::OS_People) ||
      (iddObjectType == IddObjectType::OS_Lights) ||
      (iddObjectType == IddObjectType::OS_Luminaire) ||
      (iddObjectType == IddObjectType::OS_ElectricEquipment) ||
      (iddObjectType == IddObjectType::OS_GasEquipment) ||
      (iddObjectType == IddObjectType::OS_SteamEquipment) ||
      (iddObjectType == IddObjectType::OS_OtherEquipment) ||
      (iddObjectType == IddObjectType::OS_SpaceInfiltration_DesignFlowRate) ||
      (iddObjectType == IddObjectType::OS_SpaceInfiltration_EffectiveLeakageArea) ||
      (iddObjectType == IddObjectType::OS_DesignSpecification_OutdoorAir)){

    refresh();
  }
}

void FacilityTreeWidget::onObjectRemoved(const openstudio::model::ModelObject& modelObject, const openstudio::IddObjectType& iddObjectType, const openstudio::Handle& handle)
{
  if ((iddObjectType == IddObjectType::OS_Facility) ||
      (iddObjectType == IddObjectType::OS_ShadingSurfaceGroup) ||
      (iddObjectType == IddObjectType::OS_ShadingSurface) ||
      (iddObjectType == IddObjectType::OS_Building) ||
      (iddObjectType == IddObjectType::OS_BuildingStory) ||
      (iddObjectType == IddObjectType::OS_ThermalZone) ||
      (iddObjectType == IddObjectType::OS_SpaceType) ||
      (iddObjectType == IddObjectType::OS_Space) ||
      (iddObjectType == IddObjectType::OS_InteriorPartitionSurfaceGroup) ||
      (iddObjectType == IddObjectType::OS_InteriorPartitionSurface) ||
      (iddObjectType == IddObjectType::OS_Daylighting_Control) ||
      (iddObjectType == IddObjectType::OS_IlluminanceMap) ||
      (iddObjectType == IddObjectType::OS_InternalMass) ||
      (iddObjectType == IddObjectType::OS_People) ||
      (iddObjectType == IddObjectType::OS_Lights) ||
      (iddObjectType == IddObjectType::OS_Luminaire) ||
      (iddObjectType == IddObjectType::OS_ElectricEquipment) ||
      (iddObjectType == IddObjectType::OS_GasEquipment) ||
      (iddObjectType == IddObjectType::OS_SteamEquipment) ||
      (iddObjectType == IddObjectType::OS_OtherEquipment) ||
      (iddObjectType == IddObjectType::OS_SpaceInfiltration_DesignFlowRate) ||
      (iddObjectType == IddObjectType::OS_SpaceInfiltration_EffectiveLeakageArea) ||
      (iddObjectType == IddObjectType::OS_DesignSpecification_OutdoorAir)){

    refresh();
  }
}

void FacilityTreeWidget::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void FacilityTreeWidget::onSortByChanged(const QString& text)
{
  if (text == "Building Story"){
    if (m_sortByType != IddObjectType::OS_BuildingStory){
      m_sortByType = IddObjectType::OS_BuildingStory;
      this->treeWidget()->clear();
      QTimer::singleShot(0, this, SLOT(initialize()));
    }
  }else if (text == "Thermal Zone"){
    if (m_sortByType != IddObjectType::OS_ThermalZone){
      m_sortByType = IddObjectType::OS_ThermalZone;
      this->treeWidget()->clear();
      QTimer::singleShot(0, this, SLOT(initialize()));
    }
  }else if (text == "Space Type"){
    if (m_sortByType != IddObjectType::OS_SpaceType){
      m_sortByType = IddObjectType::OS_SpaceType;
      this->treeWidget()->clear();
      QTimer::singleShot(0, this, SLOT(initialize()));
    }
  }else{
    OS_ASSERT(false);
  }
}

void FacilityTreeWidget::currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
  QTreeWidget* treeWidget = this->treeWidget();

  ModelObjectTreeItem* modelObjectTreeItem = dynamic_cast<ModelObjectTreeItem*>(current);
  if(modelObjectTreeItem){
    boost::optional<model::ModelObject> modelObject = modelObjectTreeItem->modelObject();
    if (modelObject){
      if (!modelObject->handle().isNull()){
        if (m_selectedObjectHandle != modelObject->handle()){
          m_selectedObject = modelObject;
          m_selectedObjectHandle = modelObject->handle();
          emit itemSelected(modelObjectTreeItem->item());
        }
      }
    }else{
      current->setSelected(false);
      if (previous){
        bool wasBlocked = treeWidget->blockSignals(true);
        treeWidget->setCurrentItem(previous);
        previous->setSelected(true);
        treeWidget->blockSignals(wasBlocked);
      }
    }
  }
}

void FacilityTreeWidget::initialize()
{
  openstudio::model::Model model = this->model();
  model::Building building = model.getUniqueModelObject<model::Building>();
  QTreeWidget* treeWidget = this->treeWidget();

  // add site shading
  SiteShadingTreeItem* siteShadingTreeItem = new SiteShadingTreeItem(model);
  treeWidget->addTopLevelItem(siteShadingTreeItem);

  // add building
  BuildingTreeItem* buildingTreeItem = new BuildingTreeItem(building, m_sortByType);
  treeWidget->addTopLevelItem(buildingTreeItem);

  // set the selected object
  treeWidget->setCurrentItem(buildingTreeItem);
  m_selectedObject = building;
  m_selectedObjectHandle = building.handle();
  emit itemSelected(buildingTreeItem->item());
}

} // openstudio

