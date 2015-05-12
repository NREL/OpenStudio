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

#include "FacilityTabController.hpp"

#include "BuildingInspectorView.hpp"
#include "FacilityExteriorEquipmentGridView.hpp"
#include "FacilityShadingGridView.hpp"
#include "FacilityStoriesGridView.hpp"
#include "FacilityTabView.hpp"

namespace openstudio {

FacilityTabController::FacilityTabController(bool isIP, const model::Model& model)
  : MainTabController(new FacilityTabView())
{
  auto buildingInspectorView = new BuildingInspectorView(isIP, model);
  this->mainContentWidget()->addSubTab("Building", buildingInspectorView, BUILDING);
  connect(this, &FacilityTabController::toggleUnitsClicked, buildingInspectorView, &BuildingInspectorView::toggleUnitsClicked);
  connect(buildingInspectorView, &BuildingInspectorView::dropZoneItemClicked, this, &FacilityTabController::dropZoneItemClicked);

  this->mainContentWidget()->addSubTab("Stories", new FacilityStoriesGridView(isIP, model), STORIES);
  this->mainContentWidget()->addSubTab("Shading", new FacilityShadingGridView(isIP, model), SHADING);
  this->mainContentWidget()->addSubTab("Exterior Equipment", new FacilityExteriorEquipmentGridView(isIP, model), EXTERIOR_EQUIPMENT);
}

void FacilityTabController::toggleUnits(bool displayIP)
{
}

} // openstudio
