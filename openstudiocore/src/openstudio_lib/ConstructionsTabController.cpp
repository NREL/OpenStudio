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

#include "ConstructionsTabController.hpp"

#include "ConstructionsController.hpp"
#include "ConstructionsTabView.hpp"
#include "DefaultConstructionSetsController.hpp"
#include "MaterialsController.hpp"

#include "../model/Construction.hpp"
#include "../model/Model.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

ConstructionsTabController::ConstructionsTabController(bool isIP, const model::Model& model)
  : MainTabController(new ConstructionsTabView(model, "Constructions"))
{
  m_defaultConstructionSetsController = std::shared_ptr<DefaultConstructionSetsController>(new DefaultConstructionSetsController(model));
  m_constructionsController = std::shared_ptr<ConstructionsController>(new ConstructionsController(isIP, model));
  m_materialsController = std::shared_ptr<MaterialsController>(new MaterialsController(isIP, model));

  this->mainContentWidget()->addSubTab("Construction Sets", m_defaultConstructionSetsController->subTabView(),DEFAULT_CONSTRUCTIONS);
  this->mainContentWidget()->addSubTab("Constructions", m_constructionsController->subTabView(),CONSTRUCTIONS);
  this->mainContentWidget()->addSubTab("Materials", m_materialsController->subTabView(),MATERIALS);

  bool isConnected = false;

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        m_defaultConstructionSetsController->subTabView(), SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        m_constructionsController->subTabView(), SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)),
                        m_materialsController->subTabView(), SIGNAL(toggleUnitsClicked(bool)));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_defaultConstructionSetsController.get(), SIGNAL(downloadComponentsClicked()),
                                 this, SIGNAL(downloadComponentsClicked()));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_defaultConstructionSetsController.get(), SIGNAL(openLibDlgClicked()),
                                 this, SIGNAL(openLibDlgClicked()));
  OS_ASSERT(isConnected);
}

ConstructionsTabController::~ConstructionsTabController()
{
}

} // openstudio
