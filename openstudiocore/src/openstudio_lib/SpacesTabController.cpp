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
    : MainTabController(new SpacesTabView()),
    m_model(model),
    m_isIP(isIP)
{
  mainContentWidget()->addSubTab("Properties", SPACES);
  mainContentWidget()->addSubTab("Loads", LOADS);
  mainContentWidget()->addSubTab("Surfaces", SURFACES);
  mainContentWidget()->addSubTab("Subsurfaces", SUBSURFACES);
  mainContentWidget()->addSubTab("Interior Partitions", INTERIOR_PARTITIONS);
  mainContentWidget()->addSubTab("Shading", SHADING);

  setSubTab(0);

  // Evan note: TODO SpacesDaylightingGridView may be a dead class, given the latest Radiance plans
  //this->mainContentWidget()->addSubTab("Daylighting", new SpacesDaylightingGridView(m_isIP, m_model), DAYLIGHTING);
}

void SpacesTabController::toggleUnits(bool displayIP)
{
}

void SpacesTabController::setSubTab(int index)
{
  switch (index){
  case 0:
  {
    auto spacesSpacesGridView = new SpacesSpacesGridView(m_isIP, m_model);
    connect(this, &SpacesTabController::toggleUnitsClicked, spacesSpacesGridView, &SpacesSpacesGridView::toggleUnitsClicked);
    connect(spacesSpacesGridView, &SpacesSpacesGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);
    connect(this->mainContentWidget(), &MainTabView::tabSelected, this, &SpacesTabController::setSubTab);
    this->mainContentWidget()->setSubTab(spacesSpacesGridView);
    break;
  }
  case 1:
  {
    auto spacesLoadsGridView = new SpacesLoadsGridView(m_isIP, m_model);
    connect(this, &SpacesTabController::toggleUnitsClicked, spacesLoadsGridView, &SpacesLoadsGridView::toggleUnitsClicked);
    connect(spacesLoadsGridView, &SpacesLoadsGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);
    connect(this->mainContentWidget(), &MainTabView::tabSelected, this, &SpacesTabController::setSubTab);
    this->mainContentWidget()->setSubTab(spacesLoadsGridView);
    break;
  }
  case 2:
  {
    auto spacesSurfacesGridView = new SpacesSurfacesGridView(m_isIP, m_model);
    connect(this, &SpacesTabController::toggleUnitsClicked, spacesSurfacesGridView, &SpacesSurfacesGridView::toggleUnitsClicked);
    connect(spacesSurfacesGridView, &SpacesSurfacesGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);
    connect(this->mainContentWidget(), &MainTabView::tabSelected, this, &SpacesTabController::setSubTab);
    this->mainContentWidget()->setSubTab(spacesSurfacesGridView);
    break;
  }
  case 3:
  {
    auto spacesSubsurfacesGridView = new SpacesSubsurfacesGridView(m_isIP, m_model);
    connect(this, &SpacesTabController::toggleUnitsClicked, spacesSubsurfacesGridView, &SpacesSubsurfacesGridView::toggleUnitsClicked);
    connect(spacesSubsurfacesGridView, &SpacesSubsurfacesGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);
    connect(this->mainContentWidget(), &MainTabView::tabSelected, this, &SpacesTabController::setSubTab);
    this->mainContentWidget()->setSubTab(spacesSubsurfacesGridView);
    break;
  }
  case 4:
  {
    auto spacesInteriorPartitionsGridView = new SpacesInteriorPartitionsGridView(m_isIP, m_model);
    connect(this, &SpacesTabController::toggleUnitsClicked, spacesInteriorPartitionsGridView, &SpacesInteriorPartitionsGridView::toggleUnitsClicked);
    connect(spacesInteriorPartitionsGridView, &SpacesInteriorPartitionsGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);
    connect(this->mainContentWidget(), &MainTabView::tabSelected, this, &SpacesTabController::setSubTab);
    this->mainContentWidget()->setSubTab(spacesInteriorPartitionsGridView);
    break;
  }
  case 5:
  {
    auto spacesShadingGridView = new SpacesShadingGridView(m_isIP, m_model);
    connect(this, &SpacesTabController::toggleUnitsClicked, spacesShadingGridView, &SpacesShadingGridView::toggleUnitsClicked);
    connect(spacesShadingGridView, &SpacesShadingGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);
    connect(this->mainContentWidget(), &MainTabView::tabSelected, this, &SpacesTabController::setSubTab);
    this->mainContentWidget()->setSubTab(spacesShadingGridView);
    break;
  }
  default:
    OS_ASSERT(false);
    break;
  }
}

} // openstudio
