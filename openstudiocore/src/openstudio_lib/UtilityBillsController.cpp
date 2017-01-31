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

#include "UtilityBillsController.hpp"

#include "UtilityBillAllFuelTypesListView.hpp"
#include "UtilityBillsView.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/UtilityBill.hpp"
#include "../model/UtilityBill_Impl.hpp"

#include "OSInspectorView.hpp"
#include "OSItemSelectorButtons.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {

UtilityBillsController::UtilityBillsController(const model::Model& model)
  : ModelSubTabController(new UtilityBillsView(model), model)
{
  subTabView()->itemSelectorButtons()->disableCopyButton();
  subTabView()->itemSelectorButtons()->disablePurgeButton();
  subTabView()->itemSelectorButtons()->hideDropZone();

  UtilityBillsInspectorView * utilityBillsInspectorView = static_cast<UtilityBillsInspectorView *>(subTabView()->inspectorView());

  connect(this, &UtilityBillsController::toggleUnitsClicked, utilityBillsInspectorView, &UtilityBillsInspectorView::toggleUnitsClicked);

  connect(utilityBillsInspectorView, &UtilityBillsInspectorView::enableAddNewObjectButton, this, &UtilityBillsController::enableAddNewObjectButton);
}

void UtilityBillsController::onAddObject(const openstudio::IddObjectType& iddObjectType)
{
  OSItemSelector* itemSelector = this->subTabView()->itemSelector(); 
  UtilityBillAllFuelTypesListView * utilityBillAllFuelTypesListView = qobject_cast<UtilityBillAllFuelTypesListView *>(itemSelector);
  OS_ASSERT(utilityBillAllFuelTypesListView);
  FuelType fuelType = utilityBillAllFuelTypesListView->currentFuelType();
  model::Model model = this->model();
  openstudio::model::UtilityBill(fuelType,model);
}

void UtilityBillsController::onCopyObject(const openstudio::model::ModelObject& modelObject)
{
  // not desired
}

void UtilityBillsController::onRemoveObject(openstudio::model::ModelObject modelObject)
{
  modelObject.remove();
}

void UtilityBillsController::onReplaceObject(openstudio::model::ModelObject modelObject, const OSItemId& replacementItemId)
{
  // not yet implemented
}

void UtilityBillsController::onPurgeObjects(const openstudio::IddObjectType& iddObjectType)
{
  // DLM: we don't want the purge button enabled
}

void UtilityBillsController::onDrop(const OSItemId& itemId)
{
  // DLM: we don't want the drop zone enabled
}

void UtilityBillsController::onInspectItem(OSItem* item)
{
  subTabView()->inspectorView()->selectItem(item);
}

void UtilityBillsController::onSelectItem(OSItem* item)
{
  m_subTabView->inspectorView()->selectItem(item);
  m_subTabView->itemSelectorButtons()->disableAddButton();
  m_subTabView->itemSelectorButtons()->enableRemoveButton();
}

void UtilityBillsController::onClearSelection()
{
  m_subTabView->inspectorView()->clearSelection();
  m_subTabView->itemSelectorButtons()->disableRemoveButton();

  openstudio::OSInspectorView * inspectorView = subTabView()->inspectorView();
  UtilityBillsInspectorView * utilityBillsInspectorView = qobject_cast<UtilityBillsInspectorView *>(inspectorView);
  OS_ASSERT(utilityBillsInspectorView);

  enableAddNewObjectButton(utilityBillsInspectorView->runPeriodDates());
}

///// SLOTS

void UtilityBillsController::enableAddNewObjectButton(bool enable)
{
  if(enable){
    m_subTabView->itemSelectorButtons()->enableAddButton();
  } else {
    m_subTabView->itemSelectorButtons()->disableAddButton();
  };
}

} // openstudio
