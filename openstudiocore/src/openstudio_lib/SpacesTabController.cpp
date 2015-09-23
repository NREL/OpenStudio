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

#include "SpacesTabController.hpp"

#include "SpacesDaylightingGridView.hpp"
#include "SpacesInteriorPartitionsGridView.hpp"
#include "SpacesLoadsGridView.hpp"
#include "SpacesShadingGridView.hpp"
#include "SpacesSpacesGridView.hpp"
#include "SpacesSubsurfacesGridView.hpp"
#include "SpacesSurfacesGridView.hpp"
#include "SpacesTabView.hpp"

namespace openstudio {

  SpacesTabController::SpacesTabController(bool isIP, const model::Model& model)
: MainTabController(new SpacesTabView())
{
  auto spacesSpacesGridView = new SpacesSpacesGridView(isIP, model);
  this->mainContentWidget()->addSubTab("Properties", spacesSpacesGridView, SPACES);
  connect(this, &SpacesTabController::toggleUnitsClicked, spacesSpacesGridView, &SpacesSpacesGridView::toggleUnitsClicked);
  connect(spacesSpacesGridView, &SpacesSpacesGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);

  auto spacesLoadsGridView = new SpacesLoadsGridView(isIP, model);
  this->mainContentWidget()->addSubTab("Loads", spacesLoadsGridView, LOADS);
  connect(this, &SpacesTabController::toggleUnitsClicked, spacesLoadsGridView, &SpacesLoadsGridView::toggleUnitsClicked);
  connect(spacesLoadsGridView, &SpacesLoadsGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);

  auto spacesSurfacesGridView = new SpacesSurfacesGridView(isIP, model);
  this->mainContentWidget()->addSubTab("Surfaces", spacesSurfacesGridView, SURFACES);
  connect(this, &SpacesTabController::toggleUnitsClicked, spacesSurfacesGridView, &SpacesSurfacesGridView::toggleUnitsClicked);
  connect(spacesSurfacesGridView, &SpacesSurfacesGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);

  auto spacesSubsurfacesGridView = new SpacesSubsurfacesGridView(isIP, model);
  this->mainContentWidget()->addSubTab("Subsurfaces", spacesSubsurfacesGridView, SUBSURFACES);
  connect(this, &SpacesTabController::toggleUnitsClicked, spacesSubsurfacesGridView, &SpacesSubsurfacesGridView::toggleUnitsClicked);
  connect(spacesSubsurfacesGridView, &SpacesSubsurfacesGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);

  auto spacesInteriorPartitionsGridView = new SpacesInteriorPartitionsGridView(isIP, model);
  this->mainContentWidget()->addSubTab("Interior Partitions", spacesInteriorPartitionsGridView, INTERIOR_PARTITIONS);
  connect(this, &SpacesTabController::toggleUnitsClicked, spacesInteriorPartitionsGridView, &SpacesInteriorPartitionsGridView::toggleUnitsClicked);
  connect(spacesInteriorPartitionsGridView, &SpacesInteriorPartitionsGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);

  auto spacesShadingGridView = new SpacesShadingGridView(isIP, model);
  this->mainContentWidget()->addSubTab("Shading", spacesShadingGridView, SHADING);
  connect(this, &SpacesTabController::toggleUnitsClicked, spacesShadingGridView, &SpacesShadingGridView::toggleUnitsClicked);
  connect(spacesShadingGridView, &SpacesShadingGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);

  // Evan note: TODO SpacesDaylightingGridView may be a dead class, given the latest Radiance plans
  //this->mainContentWidget()->addSubTab("Daylighting", new SpacesDaylightingGridView(isIP, model), DAYLIGHTING);
}

  void SpacesTabController::toggleUnits(bool displayIP)
{
}

  } // openstudio
