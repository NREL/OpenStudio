/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

  if (m_currentView) {
    m_currentView->disconnect();
    delete m_currentView;
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
