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

#include "SpaceTypesTabController.hpp"

#include "OSItem.hpp"
#include "SpaceTypeInspectorView.hpp"
#include "SpaceTypesController.hpp"
#include "SpaceTypesTabView.hpp"

#include "../model/Model.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

SpaceTypesTabController::SpaceTypesTabController(bool isIP,
  const model::Model& model)
  : MainTabController(new SpaceTypesTabView())
{
  m_spaceTypesController = std::shared_ptr<SpaceTypesController>(new SpaceTypesController(isIP, model));

  connect(m_spaceTypesController.get(), &SpaceTypesController::modelObjectSelected, this, &SpaceTypesTabController::modelObjectSelected);

  connect(m_spaceTypesController.get(), &SpaceTypesController::dropZoneItemSelected, this, &SpaceTypesTabController::dropZoneItemSelected);

  connect(m_spaceTypesController.get(), &SpaceTypesController::downloadComponentsClicked, this, &SpaceTypesTabController::downloadComponentsClicked);

  connect(m_spaceTypesController.get(), &SpaceTypesController::openLibDlgClicked, this, &SpaceTypesTabController::openLibDlgClicked);

  SpaceTypeInspectorView * spaceTypeInspectorView = qobject_cast<SpaceTypeInspectorView *>(m_spaceTypesController->subTabView()->inspectorView());
  OS_ASSERT(spaceTypeInspectorView);

  bool isConnected = false;

  isConnected = connect(this, SIGNAL(itemRemoveClicked(OSItem *)), m_spaceTypesController.get(), SLOT(removeItem(OSItem *)));
  OS_ASSERT(isConnected);

  mainContentWidget()->addTabWidget(m_spaceTypesController->subTabView());
}

} // openstudio
