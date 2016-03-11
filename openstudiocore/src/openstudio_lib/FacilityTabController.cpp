/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

#include "FacilityTabController.hpp"

#include "BuildingInspectorView.hpp"
#include "FacilityExteriorEquipmentGridView.hpp"
#include "FacilityShadingGridView.hpp"
#include "FacilityStoriesGridView.hpp"
#include "FacilityTabView.hpp"

namespace openstudio {

FacilityTabController::FacilityTabController(bool isIP, const model::Model& model)
  : MainTabController(new FacilityTabView()),
  m_model(model),
  m_isIP(isIP)
{
  mainContentWidget()->addSubTab("Building", BUILDING);
  mainContentWidget()->addSubTab("Stories", STORIES);
  mainContentWidget()->addSubTab("Shading", SHADING);
  mainContentWidget()->addSubTab("Exterior Equipment", EXTERIOR_EQUIPMENT);

  connect(this->mainContentWidget(), &MainTabView::tabSelected, this, &FacilityTabController::setSubTab);
}

FacilityTabController::~FacilityTabController()
{
  disconnect(this->mainContentWidget(), &MainTabView::tabSelected, this, &FacilityTabController::setSubTab);
}

void FacilityTabController::toggleUnits(bool displayIP)
{
}

void FacilityTabController::setSubTab(int index)
{
  if (m_currentIndex == index) {
    return;
  }
  else {
    m_currentIndex = index;
  }

  if (qobject_cast<BuildingInspectorView *>(m_currentView)) {
    disconnect(this, &FacilityTabController::toggleUnitsClicked, qobject_cast<BuildingInspectorView *>(m_currentView), &BuildingInspectorView::toggleUnitsClicked);
    disconnect(qobject_cast<BuildingInspectorView *>(m_currentView), &BuildingInspectorView::dropZoneItemClicked, this, &FacilityTabController::dropZoneItemClicked);
  }
  else if (qobject_cast<FacilityStoriesGridView *>(m_currentView)) {
    disconnect(this, &FacilityTabController::toggleUnitsClicked, qobject_cast<FacilityStoriesGridView *>(m_currentView), &FacilityStoriesGridView::toggleUnitsClicked);
    disconnect(qobject_cast<FacilityStoriesGridView *>(m_currentView), &FacilityStoriesGridView::dropZoneItemSelected, this, &FacilityTabController::dropZoneItemSelected);
  }
  else if (qobject_cast<FacilityShadingGridView *>(m_currentView)) {
    disconnect(this, &FacilityTabController::toggleUnitsClicked, qobject_cast<FacilityShadingGridView *>(m_currentView), &FacilityShadingGridView::toggleUnitsClicked);
    disconnect(qobject_cast<FacilityShadingGridView *>(m_currentView), &FacilityShadingGridView::dropZoneItemSelected, this, &FacilityTabController::dropZoneItemSelected);
  }
  else if (qobject_cast<FacilityExteriorEquipmentGridView *>(m_currentView)) {
    disconnect(this, &FacilityTabController::toggleUnitsClicked, qobject_cast<FacilityExteriorEquipmentGridView *>(m_currentView), &FacilityExteriorEquipmentGridView::toggleUnitsClicked);
    disconnect(qobject_cast<FacilityExteriorEquipmentGridView *>(m_currentView), &FacilityExteriorEquipmentGridView::dropZoneItemSelected, this, &FacilityTabController::dropZoneItemSelected);
  }
  else if (m_currentView) {
    // Oops! Should never get here
    OS_ASSERT(false);
  }

  switch (index){
  case 0:
  {
    auto buildingInspectorView = new BuildingInspectorView(m_isIP, m_model);
    connect(this, &FacilityTabController::toggleUnitsClicked, buildingInspectorView, &BuildingInspectorView::toggleUnitsClicked);
    connect(buildingInspectorView, &BuildingInspectorView::dropZoneItemClicked, this, &FacilityTabController::dropZoneItemClicked);
    this->mainContentWidget()->setSubTab(buildingInspectorView);
    m_currentView = buildingInspectorView;
    break;
  }
  case 1:
  {
    auto facilityStoriesGridView = new FacilityStoriesGridView(m_isIP, m_model);
    connect(this, &FacilityTabController::toggleUnitsClicked, facilityStoriesGridView, &FacilityStoriesGridView::toggleUnitsClicked);
    connect(facilityStoriesGridView, &FacilityStoriesGridView::dropZoneItemSelected, this, &FacilityTabController::dropZoneItemSelected);
    this->mainContentWidget()->setSubTab(facilityStoriesGridView);
    m_currentView = facilityStoriesGridView;
    break;
  }
  case 2:
  {
    auto facilityShadingGridView = new FacilityShadingGridView(m_isIP, m_model);
    connect(this, &FacilityTabController::toggleUnitsClicked, facilityShadingGridView, &FacilityShadingGridView::toggleUnitsClicked);
    connect(facilityShadingGridView, &FacilityShadingGridView::dropZoneItemSelected, this, &FacilityTabController::dropZoneItemSelected);
    this->mainContentWidget()->setSubTab(facilityShadingGridView);
    m_currentView = facilityShadingGridView;
    break;
  }
  case 3:
  {
    auto facilityExteriorEquipmentGridView = new FacilityExteriorEquipmentGridView(m_isIP, m_model);
    connect(this, &FacilityTabController::toggleUnitsClicked, facilityExteriorEquipmentGridView, &FacilityExteriorEquipmentGridView::toggleUnitsClicked);
    connect(facilityExteriorEquipmentGridView, &FacilityExteriorEquipmentGridView::dropZoneItemSelected, this, &FacilityTabController::dropZoneItemSelected);
    this->mainContentWidget()->setSubTab(facilityExteriorEquipmentGridView);
    m_currentView = facilityExteriorEquipmentGridView;
    break;
  }
  default:
    OS_ASSERT(false);
    break;
  }
}

} // openstudio
