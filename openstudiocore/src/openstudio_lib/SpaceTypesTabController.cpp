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

#include "SpaceTypesTabController.hpp"
#include "SpaceTypesTabView.hpp"
#include "SpaceTypesController.hpp"

#include "../model/Model.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

SpaceTypesTabController::SpaceTypesTabController(const model::Model& model)
  : MainTabController(new SpaceTypesTabView())
{
  m_spaceTypesController = std::shared_ptr<SpaceTypesController>(new SpaceTypesController(model));

  bool isConnected = false;

  isConnected = connect(m_spaceTypesController.get(),
                        SIGNAL(modelObjectSelected(model::OptionalModelObject &, bool )),
                        this,
                        SIGNAL(modelObjectSelected(model::OptionalModelObject &, bool )));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_spaceTypesController.get(), SIGNAL(downloadComponentsClicked()),
                                 this, SIGNAL(downloadComponentsClicked()));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_spaceTypesController.get(), SIGNAL(openLibDlgClicked()),
                                 this, SIGNAL(openLibDlgClicked()));
  OS_ASSERT(isConnected);

  this->mainContentWidget()->addTabWidget(m_spaceTypesController->subTabView());
}

} // openstudio
