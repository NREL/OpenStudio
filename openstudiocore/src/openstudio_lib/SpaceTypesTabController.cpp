/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
