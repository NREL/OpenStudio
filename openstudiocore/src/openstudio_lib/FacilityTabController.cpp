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
#include "FacilityTabView.hpp"
#include "FacilityController.hpp"
#include "OSItem.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

FacilityTabController::FacilityTabController(bool isIP, const model::Model& model)
  : MainTabController(new FacilityTabView())
{
  m_facilityController = std::shared_ptr<FacilityController>(new FacilityController(isIP, model));

  connect(m_facilityController.get(), &FacilityController::modelObjectSelected, this, &FacilityTabController::modelObjectSelected);

  connect(m_facilityController.get(), &FacilityController::dropZoneItemSelected, this, &FacilityTabController::dropZoneItemSelected);

  connect(this, &FacilityTabController::toggleUnitsClicked, m_facilityController.get(), &FacilityController::toggleUnitsClicked);

  connect(m_facilityController.get(), &FacilityController::downloadComponentsClicked, this, &FacilityTabController::downloadComponentsClicked);

  connect(m_facilityController.get(), &FacilityController::openLibDlgClicked, this, &FacilityTabController::openLibDlgClicked);
  
  this->mainContentWidget()->addTabWidget(m_facilityController->subTabView());
}

void FacilityTabController::toggleUnits(bool displayIP)
{
}

} // openstudio
