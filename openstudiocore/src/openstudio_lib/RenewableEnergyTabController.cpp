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

#include "RenewableEnergyTabController.hpp"
#include "RenewableEnergyTabView.hpp"
#include "RenewableEnergyController.hpp"

#include "../model/Model.hpp"
#include "../model/Construction.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

RenewableEnergyTabController::RenewableEnergyTabController(bool isIP, const model::Model& model)
  : MainTabController(new RenewableEnergyTabView(model, "Renewable Energy"))
{
  m_RenewableEnergyController = std::shared_ptr<RenewableEnergyController>(new RenewableEnergyController(isIP, model));
  this->mainContentWidget()->addTabWidget(m_RenewableEnergyController->subTabView());

  connect(this, &RenewableEnergyTabController::toggleUnitsClicked, m_RenewableEnergyController.get(), &RenewableEnergyController::toggleUnitsClicked);

  connect(m_RenewableEnergyController.get(), &RenewableEnergyController::downloadComponentsClicked, this, &RenewableEnergyTabController::downloadComponentsClicked);

  connect(m_RenewableEnergyController.get(), &RenewableEnergyController::openLibDlgClicked, this, &RenewableEnergyTabController::openLibDlgClicked);
}

RenewableEnergyTabController::~RenewableEnergyTabController()
{
}

void RenewableEnergyTabController::toggleUnits(bool displayIP)
{
}

} // openstudio
