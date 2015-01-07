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

#include "BuildingStoriesTabController.hpp"

#include "BuildingStoriesController.hpp"
#include "BuildingStoriesTabView.hpp"

#include "../model/Model.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

BuildingStoriesTabController::BuildingStoriesTabController(bool isIP, const model::Model& model)
  : MainTabController(new BuildingStoriesTabView())
{
  m_buildingStoriesController = std::shared_ptr<BuildingStoriesController>(new BuildingStoriesController(isIP, model));
  this->mainContentWidget()->addTabWidget(m_buildingStoriesController->subTabView());

  connect(m_buildingStoriesController.get(), &BuildingStoriesController::downloadComponentsClicked, this, &BuildingStoriesTabController::downloadComponentsClicked);

  connect(m_buildingStoriesController.get(), &BuildingStoriesController::openLibDlgClicked, this, &BuildingStoriesTabController::openLibDlgClicked);
}

} // openstudio
