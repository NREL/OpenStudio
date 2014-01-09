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

#include <openstudio_lib/BuildingStoriesTabController.hpp>
#include <openstudio_lib/BuildingStoriesTabView.hpp>
#include <openstudio_lib/BuildingStoriesController.hpp>

#include <model/Model.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {

BuildingStoriesTabController::BuildingStoriesTabController(const model::Model& model)
  : MainTabController(new BuildingStoriesTabView())
{
  m_buildingStoriesController = boost::shared_ptr<BuildingStoriesController>(new BuildingStoriesController(model));
  this->mainContentWidget()->addTabWidget(m_buildingStoriesController->subTabView());

  bool isConnected = QObject::connect(m_buildingStoriesController.get(), SIGNAL(openBclDlgClicked()),
                                 this, SIGNAL(openBclDlgClicked()));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_buildingStoriesController.get(), SIGNAL(openLibDlgClicked()),
                                 this, SIGNAL(openLibDlgClicked()));
  OS_ASSERT(isConnected);
}

} // openstudio
