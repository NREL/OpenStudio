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
  : MainTabController(new ConstructionsTabView(model, "Constructions")),
  m_model(model),
  m_isIP(isIP)
{
  this->mainContentWidget()->addSubTab("Construction Sets", DEFAULT_CONSTRUCTIONS);
  this->mainContentWidget()->addSubTab("Constructions", CONSTRUCTIONS);
  this->mainContentWidget()->addSubTab("Materials", MATERIALS);

  connect(this->mainContentWidget(), &MainTabView::tabSelected, this, &ConstructionsTabController::setSubTab);
}

ConstructionsTabController::~ConstructionsTabController()
{
  disconnect(this->mainContentWidget(), &MainTabView::tabSelected, this, &ConstructionsTabController::setSubTab);
}

void ConstructionsTabController::setSubTab(int index)
{
  if (m_currentIndex == index) {
    return;
  }
  else {
    m_currentIndex = index;
  }

  if (qobject_cast<DefaultConstructionSetsController *>(m_currentController)) {
    disconnect(this, &ConstructionsTabController::toggleUnitsClicked, static_cast<ModelSubTabView*>((qobject_cast<DefaultConstructionSetsController *>(m_currentController))->subTabView()), &ModelSubTabView::toggleUnitsClicked);
    disconnect(qobject_cast<DefaultConstructionSetsController *>(m_currentController), &DefaultConstructionSetsController::downloadComponentsClicked, this, &ConstructionsTabController::downloadComponentsClicked);
    disconnect(qobject_cast<DefaultConstructionSetsController *>(m_currentController), &DefaultConstructionSetsController::openLibDlgClicked, this, &ConstructionsTabController::openLibDlgClicked);
  }
  else if (qobject_cast<ConstructionsController *>(m_currentController)) {
    disconnect(this, &ConstructionsTabController::toggleUnitsClicked, static_cast<ModelSubTabView*>((qobject_cast<ConstructionsController *>(m_currentController))->subTabView()), &ModelSubTabView::toggleUnitsClicked);
  }
  else if (qobject_cast<MaterialsController *>(m_currentController)) {
    disconnect(this, &ConstructionsTabController::toggleUnitsClicked, static_cast<ModelSubTabView*>((qobject_cast<MaterialsController *>(m_currentController))->subTabView()), &ModelSubTabView::toggleUnitsClicked);
  }
  else if (m_currentController) {
    // Oops! Should never get here
    OS_ASSERT(false);
  }

  switch (index){
  case 0:
  {
    auto defaultConstructionSetsController = new DefaultConstructionSetsController(m_model);
    connect(this, &ConstructionsTabController::toggleUnitsClicked, static_cast<ModelSubTabView*>(defaultConstructionSetsController->subTabView()), &ModelSubTabView::toggleUnitsClicked);
    connect(defaultConstructionSetsController, &DefaultConstructionSetsController::downloadComponentsClicked, this, &ConstructionsTabController::downloadComponentsClicked);
    connect(defaultConstructionSetsController, &DefaultConstructionSetsController::openLibDlgClicked, this, &ConstructionsTabController::openLibDlgClicked);
    this->mainContentWidget()->setSubTab(defaultConstructionSetsController->subTabView());
    m_currentController = defaultConstructionSetsController;
    break;
  }

  case 1:
  {
    auto constructionsController = new ConstructionsController(m_isIP, m_model);
    connect(this, &ConstructionsTabController::toggleUnitsClicked, static_cast<ModelSubTabView*>(constructionsController->subTabView()), &ModelSubTabView::toggleUnitsClicked);
    this->mainContentWidget()->setSubTab(constructionsController->subTabView());
    m_currentController = constructionsController;
    break;
  }

  case 2:
  {
    auto materialsController = new MaterialsController(m_isIP, m_model);
    connect(this, &ConstructionsTabController::toggleUnitsClicked, static_cast<ModelSubTabView*>(materialsController->subTabView()), &ModelSubTabView::toggleUnitsClicked);
    this->mainContentWidget()->setSubTab(materialsController->subTabView());
    m_currentController = materialsController;
    break;
  }
  default:
    OS_ASSERT(false);
    break;
  }
}

} // openstudio
