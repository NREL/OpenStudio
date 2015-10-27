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

  connect(this->mainContentWidget(), &MainTabView::tabSelected, this, &SpacesTabController::setSubTab);

  // Evan note: TODO SpacesDaylightingGridView may be a dead class, given the latest Radiance plans
  //this->mainContentWidget()->addSubTab("Daylighting", new SpacesDaylightingGridView(m_isIP, m_model), DAYLIGHTING);
}

SpacesTabController::~SpacesTabController()
{
  disconnect(this->mainContentWidget(), &MainTabView::tabSelected, this, &SpacesTabController::setSubTab);
}

void SpacesTabController::toggleUnits(bool displayIP)
{
}

void SpacesTabController::setSubTab(int index)
{
  if (m_currentIndex == index) {
    return;
  }
  else {
    m_currentIndex = index;
  }

  if (qobject_cast<SpacesSpacesGridView *>(m_currentView)) {
    disconnect(this, &SpacesTabController::toggleUnitsClicked, qobject_cast<SpacesSpacesGridView *>(m_currentView), &SpacesSpacesGridView::toggleUnitsClicked);
    disconnect(qobject_cast<SpacesSpacesGridView *>(m_currentView), &SpacesSpacesGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);
  }
  else if (qobject_cast<SpacesLoadsGridView *>(m_currentView)) {
    disconnect(this, &SpacesTabController::toggleUnitsClicked, qobject_cast<SpacesLoadsGridView *>(m_currentView), &SpacesLoadsGridView::toggleUnitsClicked);
    disconnect(qobject_cast<SpacesLoadsGridView *>(m_currentView), &SpacesLoadsGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);
  }
  else if (qobject_cast<SpacesSurfacesGridView *>(m_currentView)) {
    disconnect(this, &SpacesTabController::toggleUnitsClicked, qobject_cast<SpacesSurfacesGridView *>(m_currentView), &SpacesSurfacesGridView::toggleUnitsClicked);
    disconnect(qobject_cast<SpacesSurfacesGridView *>(m_currentView), &SpacesSurfacesGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);
  }
  else if (qobject_cast<SpacesSubsurfacesGridView *>(m_currentView)) {
    disconnect(this, &SpacesTabController::toggleUnitsClicked, qobject_cast<SpacesSubsurfacesGridView *>(m_currentView), &SpacesSubsurfacesGridView::toggleUnitsClicked);
    disconnect(qobject_cast<SpacesSubsurfacesGridView *>(m_currentView), &SpacesSubsurfacesGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);
  }
  else if (qobject_cast<SpacesInteriorPartitionsGridView *>(m_currentView)) {
    disconnect(this, &SpacesTabController::toggleUnitsClicked, qobject_cast<SpacesInteriorPartitionsGridView *>(m_currentView), &SpacesInteriorPartitionsGridView::toggleUnitsClicked);
    disconnect(qobject_cast<SpacesInteriorPartitionsGridView *>(m_currentView), &SpacesInteriorPartitionsGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);
  }
  else if (qobject_cast<SpacesShadingGridView *>(m_currentView)) {
    disconnect(this, &SpacesTabController::toggleUnitsClicked, qobject_cast<SpacesShadingGridView *>(m_currentView), &SpacesShadingGridView::toggleUnitsClicked);
    disconnect(qobject_cast<SpacesShadingGridView *>(m_currentView), &SpacesShadingGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);
  }
  else if (m_currentView) {
    // Oops! Should never get here
    OS_ASSERT(false);
  }

  switch (index){
  case 0:
  {
    auto spacesSpacesGridView = new SpacesSpacesGridView(m_isIP, m_model);
    connect(this, &SpacesTabController::toggleUnitsClicked, spacesSpacesGridView, &SpacesSpacesGridView::toggleUnitsClicked);
    connect(spacesSpacesGridView, &SpacesSpacesGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);
    mainContentWidget()->setSubTab(spacesSpacesGridView);
    m_currentView = spacesSpacesGridView;
    break;
  }
  case 1:
  {
    auto spacesLoadsGridView = new SpacesLoadsGridView(m_isIP, m_model);
    connect(this, &SpacesTabController::toggleUnitsClicked, spacesLoadsGridView, &SpacesLoadsGridView::toggleUnitsClicked);
    connect(spacesLoadsGridView, &SpacesLoadsGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);
    mainContentWidget()->setSubTab(spacesLoadsGridView);
    m_currentView = spacesLoadsGridView;
    break;
  }
  case 2:
  {
    auto spacesSurfacesGridView = new SpacesSurfacesGridView(m_isIP, m_model);
    connect(this, &SpacesTabController::toggleUnitsClicked, spacesSurfacesGridView, &SpacesSurfacesGridView::toggleUnitsClicked);
    connect(spacesSurfacesGridView, &SpacesSurfacesGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);
    mainContentWidget()->setSubTab(spacesSurfacesGridView);
    m_currentView = spacesSurfacesGridView;
    break;
  }
  case 3:
  {
    auto spacesSubsurfacesGridView = new SpacesSubsurfacesGridView(m_isIP, m_model);
    connect(this, &SpacesTabController::toggleUnitsClicked, spacesSubsurfacesGridView, &SpacesSubsurfacesGridView::toggleUnitsClicked);
    connect(spacesSubsurfacesGridView, &SpacesSubsurfacesGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);
    mainContentWidget()->setSubTab(spacesSubsurfacesGridView);
    m_currentView = spacesSubsurfacesGridView;
    break;
  }
  case 4:
  {
    auto spacesInteriorPartitionsGridView = new SpacesInteriorPartitionsGridView(m_isIP, m_model);
    connect(this, &SpacesTabController::toggleUnitsClicked, spacesInteriorPartitionsGridView, &SpacesInteriorPartitionsGridView::toggleUnitsClicked);
    connect(spacesInteriorPartitionsGridView, &SpacesInteriorPartitionsGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);
    mainContentWidget()->setSubTab(spacesInteriorPartitionsGridView);
    m_currentView = spacesInteriorPartitionsGridView;
    break;
  }
  case 5:
  {
    auto spacesShadingGridView = new SpacesShadingGridView(m_isIP, m_model);
    connect(this, &SpacesTabController::toggleUnitsClicked, spacesShadingGridView, &SpacesShadingGridView::toggleUnitsClicked);
    connect(spacesShadingGridView, &SpacesShadingGridView::dropZoneItemSelected, this, &SpacesTabController::dropZoneItemSelected);
    mainContentWidget()->setSubTab(spacesShadingGridView);
    m_currentView = spacesShadingGridView;
    break;
  }
  default:
    OS_ASSERT(false);
    break;
  }
}

} // openstudio
