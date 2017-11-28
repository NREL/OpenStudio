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

  if (m_currentView) {
    m_currentView->disconnect();
    delete m_currentView;
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
