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
  this->mainContentWidget()->addSubTab("Properties", new SpacesSpacesGridView(isIP, model), SPACES);
  this->mainContentWidget()->addSubTab("Loads", new SpacesLoadsGridView(isIP, model), LOADS);
  this->mainContentWidget()->addSubTab("Surfaces", new SpacesSurfacesGridView(isIP, model), SURFACES);
  this->mainContentWidget()->addSubTab("Subsurfaces", new SpacesSubsurfacesGridView(isIP, model), SUBSURFACES);
  this->mainContentWidget()->addSubTab("Interior Partitions", new SpacesInteriorPartitionsGridView(isIP, model), INTERIOR_PARTITIONS);
  this->mainContentWidget()->addSubTab("Shading", new SpacesShadingGridView(isIP, model), SHADING);
  this->mainContentWidget()->addSubTab("Daylighting", new SpacesDaylightingGridView(isIP, model), DAYLIGHTING);
}

  void SpacesTabController::toggleUnits(bool displayIP)
{
}

  } // openstudio
