/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
  connect(this, &ConstructionsTabController::toggleUnitsClicked, this, &ConstructionsTabController::toggleUnits);

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

  if (m_currentController) {
    m_currentController->disconnect();
    delete m_currentController;
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

/****************** SLOTS ******************/

void ConstructionsTabController::toggleUnits(bool displayIP) {
  m_isIP = displayIP;
}

} // openstudio
