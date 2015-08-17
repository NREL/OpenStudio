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

#include "ThermalZonesTabController.hpp"

#include "OSItem.hpp"
#include "ThermalZonesController.hpp"
#include "ThermalZonesTabView.hpp"
#include "ThermalZonesView.hpp"

namespace openstudio {

ThermalZonesTabController::ThermalZonesTabController(bool isIP, const model::Model& model)
  : MainTabController(new ThermalZonesTabView())
{
  m_thermalZonesController = std::shared_ptr<ThermalZonesController>(new ThermalZonesController(isIP,model));

  this->mainContentWidget()->addTabWidget(m_thermalZonesController->subTabView());

  bool isConnected = false;

  isConnected = connect(this, SIGNAL(itemRemoveClicked(OSItem *)), m_thermalZonesController.get(), SLOT(removeItem(OSItem *)));
  OS_ASSERT(isConnected);

  connect(m_thermalZonesController.get(), &ThermalZonesController::modelObjectSelected, this, &ThermalZonesTabController::modelObjectSelected);

  connect(m_thermalZonesController.get(), &ThermalZonesController::dropZoneItemSelected, this, &ThermalZonesTabController::dropZoneItemSelected);

  connect(this, &ThermalZonesTabController::toggleUnitsClicked, m_thermalZonesController.get(), &ThermalZonesController::toggleUnitsClicked);
}

} // openstudio

