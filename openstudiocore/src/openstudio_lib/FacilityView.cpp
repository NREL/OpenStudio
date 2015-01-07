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

#include "FacilityView.hpp"
#include "FacilityTreeWidget.hpp"
#include "BuildingInspectorView.hpp"
#include "BuildingStoryInspectorView.hpp"
#include "SpaceInspectorView.hpp"
#include "ThermalZonesView.hpp"
#include "SpaceTypeInspectorView.hpp"
#include "SurfaceInspectorView.hpp"
#include "SubSurfaceInspectorView.hpp"
#include "ShadingSurfaceGroupInspectorView.hpp"
#include "ShadingSurfaceInspectorView.hpp"
#include "InteriorPartitionSurfaceGroupInspectorView.hpp"
#include "InteriorPartitionSurfaceInspectorView.hpp"
#include "DaylightingControlInspectorView.hpp"
#include "IlluminanceMapInspectorView.hpp"
#include "GlareSensorInspectorView.hpp"
#include "PeopleInspectorView.hpp"
#include "OSItem.hpp"
#include "OSItemSelectorButtons.hpp"

#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include <utilities/idd/IddEnums.hxx>

#include <QVBoxLayout>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QStackedWidget>
#include <QScrollArea>

#include <sstream>

namespace openstudio {


FacilityView::FacilityView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelSubTabView(new FacilityTreeWidget(model, parent),
                    new FacilityInspectorView(isIP, model, parent),
                    false,
                    parent)
{
  this->itemSelectorButtons()->hide();

  connect(this, &FacilityView::toggleUnitsClicked, modelObjectInspectorView(), &ModelObjectInspectorView::toggleUnitsClicked);
}

void FacilityView::toggleUnits(bool displayIP)
{
}

FacilityInspectorView::FacilityInspectorView(bool isIP,
                                             const model::Model& model,
                                             QWidget * parent )
  : ModelObjectInspectorView(model, false, parent)
{
  // index of hidden widget is 0
  QWidget* hiddenWidget = new QWidget();
  int index = this->stackedWidget()->addWidget(hiddenWidget);
  OS_ASSERT(index == 0);

  // index of the default is 1
  DefaultInspectorView* defaultInspectorView = new DefaultInspectorView(model, parent);
  connect(defaultInspectorView, &DefaultInspectorView::dropZoneItemClicked, this, &FacilityInspectorView::dropZoneItemClicked);
  index = this->stackedWidget()->addWidget(defaultInspectorView);
  OS_ASSERT(index == 1);

  BuildingInspectorView* buildingInspectorView = new BuildingInspectorView(isIP, model, parent);
  connect(this, &FacilityInspectorView::toggleUnitsClicked, buildingInspectorView, &BuildingInspectorView::toggleUnitsClicked);
  connect(buildingInspectorView, &BuildingInspectorView::dropZoneItemClicked, this, &FacilityInspectorView::dropZoneItemClicked);
  index = this->stackedWidget()->addWidget(buildingInspectorView);
  m_inspectorIndexMap[IddObjectType::OS_Building] = index;

  BuildingStoryInspectorView* buildingStoryInspectorView = new BuildingStoryInspectorView(isIP, model, parent);
  connect(this, &FacilityInspectorView::toggleUnitsClicked, buildingStoryInspectorView, &BuildingStoryInspectorView::toggleUnitsClicked);
  connect(buildingStoryInspectorView, &BuildingStoryInspectorView::dropZoneItemClicked, this, &FacilityInspectorView::dropZoneItemClicked);
  index = this->stackedWidget()->addWidget(buildingStoryInspectorView);
  m_inspectorIndexMap[IddObjectType::OS_BuildingStory] = index;

  ThermalZoneView* thermalZoneView = new ThermalZoneView(isIP,model, parent);
  connect(this, &FacilityInspectorView::toggleUnitsClicked, thermalZoneView, &ThermalZoneView::toggleUnitsClicked);
  connect(thermalZoneView, &ThermalZoneView::dropZoneItemClicked, this, &FacilityInspectorView::dropZoneItemClicked);
  index = this->stackedWidget()->addWidget(thermalZoneView);
  m_inspectorIndexMap[IddObjectType::OS_ThermalZone] = index;

  SpaceTypeInspectorView* spaceTypeInspectorView = new SpaceTypeInspectorView(isIP, model, parent);
  connect(this, &FacilityInspectorView::toggleUnitsClicked, spaceTypeInspectorView, &SpaceTypeInspectorView::toggleUnitsClicked);
  connect(spaceTypeInspectorView, &SpaceTypeInspectorView::dropZoneItemClicked, this, &FacilityInspectorView::dropZoneItemClicked);
  index = this->stackedWidget()->addWidget(spaceTypeInspectorView);
  m_inspectorIndexMap[IddObjectType::OS_SpaceType] = index;

  SpaceInspectorView* spaceInspectorView = new SpaceInspectorView(isIP, model, parent);
  connect(this, &FacilityInspectorView::toggleUnitsClicked, spaceInspectorView, &SpaceInspectorView::toggleUnitsClicked);
  connect(spaceInspectorView, &SpaceInspectorView::dropZoneItemClicked, this, &FacilityInspectorView::dropZoneItemClicked);
  index = this->stackedWidget()->addWidget(spaceInspectorView);
  m_inspectorIndexMap[IddObjectType::OS_Space] = index;

  SurfaceInspectorView* surfaceInspectorView = new SurfaceInspectorView(isIP, model, parent);
  connect(this, &FacilityInspectorView::toggleUnitsClicked, surfaceInspectorView, &SurfaceInspectorView::toggleUnitsClicked);
  connect(surfaceInspectorView, &SurfaceInspectorView::dropZoneItemClicked, this, &FacilityInspectorView::dropZoneItemClicked);
  index = this->stackedWidget()->addWidget(surfaceInspectorView);
  m_inspectorIndexMap[IddObjectType::OS_Surface] = index;

  SubSurfaceInspectorView* subSurfaceInspectorView = new SubSurfaceInspectorView(isIP, model, parent);
  connect(this, &FacilityInspectorView::toggleUnitsClicked, subSurfaceInspectorView, &SubSurfaceInspectorView::toggleUnitsClicked);
  connect(subSurfaceInspectorView, &SubSurfaceInspectorView::dropZoneItemClicked, this, &FacilityInspectorView::dropZoneItemClicked);
  index = this->stackedWidget()->addWidget(subSurfaceInspectorView);
  m_inspectorIndexMap[IddObjectType::OS_SubSurface] = index;
  //ShadingSurfaceGroupInspectorView* shadingSurfaceGroupInspectorView = new ShadingSurfaceGroupInspectorView(model, parent);
  //isConnected = connect(this, 
  //                      SIGNAL(toggleUnitsClicked(bool)),
  //                      shadingSurfaceGroupInspectorView, 
  //                      SIGNAL(toggleUnitsClicked(bool)));
  //OS_ASSERT(isConnected);
  //isConnected = connect(shadingSurfaceGroupInspectorView, 
  //                      SIGNAL(dropZoneItemClicked(OSItem*)),
  //                      this, 
  //                      SIGNAL(dropZoneItemClicked(OSItem*)));
  //OS_ASSERT(isConnected);
  //index = this->stackedWidget()->addWidget(shadingSurfaceGroupInspectorView);
  //m_inspectorIndexMap[IddObjectType::OS_ShadingSurfaceGroup] = index;

  //ShadingSurfaceInspectorView* shadingSurfaceInspectorView = new ShadingSurfaceInspectorView(model, parent);
  //isConnected = connect(this, 
  //                      SIGNAL(toggleUnitsClicked(bool)),
  //                      shadingSurfaceInspectorView, 
  //                      SIGNAL(toggleUnitsClicked(bool)));
  //OS_ASSERT(isConnected);
  //isConnected = connect(shadingSurfaceInspectorView, 
  //                      SIGNAL(dropZoneItemClicked(OSItem*)),
  //                      this, 
  //                      SIGNAL(dropZoneItemClicked(OSItem*)));
  //OS_ASSERT(isConnected);
  //index = this->stackedWidget()->addWidget(shadingSurfaceInspectorView);
  //m_inspectorIndexMap[IddObjectType::OS_ShadingSurface] = index;

  //InteriorPartitionSurfaceGroupInspectorView* interiorPartitionSurfaceGroupInspectorView = new InteriorPartitionSurfaceGroupInspectorView(model, parent);
  //isConnected = connect(this, 
  //                      SIGNAL(toggleUnitsClicked(bool)),
  //                      interiorPartitionSurfaceGroupInspectorView, 
  //                      SIGNAL(toggleUnitsClicked(bool)));
  //OS_ASSERT(isConnected);
  //isConnected = connect(interiorPartitionSurfaceGroupInspectorView, 
  //                      SIGNAL(dropZoneItemClicked(OSItem*)),
  //                      this, 
  //                      SIGNAL(dropZoneItemClicked(OSItem*)));
  //OS_ASSERT(isConnected);  
  //index = this->stackedWidget()->addWidget(interiorPartitionSurfaceGroupInspectorView);
  //m_inspectorIndexMap[IddObjectType::OS_InteriorPartitionSurfaceGroup] = index;

  //InteriorPartitionSurfaceInspectorView* interiorPartitionSurfaceInspectorView = new InteriorPartitionSurfaceInspectorView(model, parent);
  //isConnected = connect(this, 
  //                      SIGNAL(toggleUnitsClicked(bool)),
  //                      interiorPartitionSurfaceInspectorView, 
  //                      SIGNAL(toggleUnitsClicked(bool)));
  //OS_ASSERT(isConnected);
  //isConnected = connect(interiorPartitionSurfaceInspectorView, 
  //                      SIGNAL(dropZoneItemClicked(OSItem*)),
  //                      this, 
  //                      SIGNAL(dropZoneItemClicked(OSItem*)));
  //OS_ASSERT(isConnected);  
  //index = this->stackedWidget()->addWidget(interiorPartitionSurfaceInspectorView);
  //m_inspectorIndexMap[IddObjectType::OS_InteriorPartitionSurface] = index;

  //DaylightingControlInspectorView* daylightingControlInspectorView = new DaylightingControlInspectorView(model, parent);
  //isConnected = connect(this, 
  //                    SIGNAL(toggleUnitsClicked(bool)),
  //                    daylightingControlInspectorView, 
  //                    SIGNAL(toggleUnitsClicked(bool)));
  //OS_ASSERT(isConnected);
  //isConnected = connect(daylightingControlInspectorView, 
  //                      SIGNAL(dropZoneItemClicked(OSItem*)),
  //                      this, 
  //                      SIGNAL(dropZoneItemClicked(OSItem*)));
  //OS_ASSERT(isConnected);
  //index = this->stackedWidget()->addWidget(daylightingControlInspectorView);
  //m_inspectorIndexMap[IddObjectType::OS_Daylighting_Control] = index;
  
  //GlareSensorInspectorView* glareSensorInspectorView = new GlareSensorInspectorView(model, parent);
  //isConnected = connect(this, 
  //                    SIGNAL(toggleUnitsClicked(bool)),
  //                    daylightingControlInspectorView, 
  //                    SIGNAL(toggleUnitsClicked(bool)));
  //OS_ASSERT(isConnected);
  //isConnected = connect(glareSensorInspectorView, 
  //                      SIGNAL(dropZoneItemClicked(OSItem*)),
  //                      this, 
  //                      SIGNAL(dropZoneItemClicked(OSItem*)));
  //OS_ASSERT(isConnected);
  //index = this->stackedWidget()->addWidget(glareSensorInspectorView);
  //m_inspectorIndexMap[IddObjectType::OS_GlareSensor] = index;
  
  //IlluminanceMapInspectorView* illuminanceMapInspectorView = new IlluminanceMapInspectorView(model, parent);
  //isConnected = connect(this, 
  //                    SIGNAL(toggleUnitsClicked(bool)),
  //                    illuminanceMapInspectorView, 
  //                    SIGNAL(toggleUnitsClicked(bool)));
  //OS_ASSERT(isConnected);
  //isConnected = connect(illuminanceMapInspectorView, 
  //                      SIGNAL(dropZoneItemClicked(OSItem*)),
  //                      this, 
  //                      SIGNAL(dropZoneItemClicked(OSItem*)));
  //OS_ASSERT(isConnected);
  //index = this->stackedWidget()->addWidget(illuminanceMapInspectorView);
  //m_inspectorIndexMap[IddObjectType::OS_IlluminanceMap] = index;

  //PeopleInspectorView* peopleInspectorView = new PeopleInspectorView(model, parent);
  //isConnected = connect(this, 
  //                      SIGNAL(toggleUnitsClicked(bool)),
  //                      peopleInspectorView, 
  //                      SIGNAL(toggleUnitsClicked(bool)));
  //OS_ASSERT(isConnected);
  //isConnected = connect(peopleInspectorView, 
  //                      SIGNAL(dropZoneItemClicked(OSItem*)),
  //                      this, 
  //                      SIGNAL(dropZoneItemClicked(OSItem*)));
  //OS_ASSERT(isConnected);
  //index = this->stackedWidget()->addWidget(peopleInspectorView);
  //m_inspectorIndexMap[IddObjectType::OS_People] = index;

  this->stackedWidget()->setCurrentIndex(0);
}

void FacilityInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  QWidget* widget = this->stackedWidget()->currentWidget();
  ModelObjectInspectorView* modelObjectInspectorView = qobject_cast<ModelObjectInspectorView*>(widget);
  if(modelObjectInspectorView){
    modelObjectInspectorView->clearSelection();
  }

  this->stackedWidget()->setCurrentIndex(0);
}

void FacilityInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  int index = 1;
  std::map<openstudio::IddObjectType, int>::const_iterator it = m_inspectorIndexMap.find(modelObject.iddObjectType());
  if (it != m_inspectorIndexMap.end()){
    index = it->second;
  }

  QWidget* widget = this->stackedWidget()->widget(index);
  ModelObjectInspectorView* modelObjectInspectorView = qobject_cast<ModelObjectInspectorView*>(widget);
  OS_ASSERT(modelObjectInspectorView);
  modelObjectInspectorView->selectModelObject(modelObject);
  this->stackedWidget()->setCurrentIndex(index);
}

void FacilityInspectorView::onUpdate()
{
}

void FacilityInspectorView::toggleUnits(bool displayIP)
{
}

} // openstudio
