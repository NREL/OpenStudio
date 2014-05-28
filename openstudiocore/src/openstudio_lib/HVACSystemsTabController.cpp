/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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

#include "HVACSystemsTabController.hpp"
#include "HVACSystemsController.hpp"
#include "HVACSystemsView.hpp"
#include "HVACSystemsTabView.hpp"

namespace openstudio {

HVACSystemsTabController::HVACSystemsTabController(bool isIP, const model::Model& model)
  : MainTabController(new HVACSystemsTabView()),
  m_isIP(isIP)
{
  m_hvacSystemsController = std::shared_ptr<HVACSystemsController>(new HVACSystemsController(isIP, model));

  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                             m_hvacSystemsController.get(), SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        this, SLOT(toggleUnits(bool)));
  OS_ASSERT(isConnected);

  this->mainContentWidget()->addTabWidget(m_hvacSystemsController->hvacSystemsView());
}

void HVACSystemsTabController::clearSceneSelection()
{
  m_hvacSystemsController->clearSceneSelection();
}

void HVACSystemsTabController::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio

