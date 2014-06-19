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

#include "LoadsTabController.hpp"
#include "LoadsTabView.hpp"
#include "LoadsController.hpp"

#include "../model/Model.hpp"
#include "../model/Construction.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

LoadsTabController::LoadsTabController(bool isIP, const model::Model& model)
  : MainTabController(new LoadsTabView(model, "Loads"))
{
  m_LoadsController = std::shared_ptr<LoadsController>(new LoadsController(isIP, model));
  this->mainContentWidget()->addTabWidget(m_LoadsController->subTabView());

  bool isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        m_LoadsController.get(), SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_LoadsController.get(), SIGNAL(downloadComponentsClicked()),
                                 this, SIGNAL(downloadComponentsClicked()));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_LoadsController.get(), SIGNAL(openLibDlgClicked()),
                                 this, SIGNAL(openLibDlgClicked()));
  OS_ASSERT(isConnected);
}

LoadsTabController::~LoadsTabController()
{
}

void LoadsTabController::toggleUnits(bool displayIP)
{
}

} // openstudio
