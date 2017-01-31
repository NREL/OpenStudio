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

#include "SubTabController.hpp"

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectItem.hpp"
#include "ModelObjectListView.hpp"
#include "ModelObjectTypeListView.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "OSItemSelector.hpp"
#include "OSItemSelectorButtons.hpp"
#include "SubTabView.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

SubTabController::SubTabController(SubTabView* subTabView)
  : OSQObjectController(),
    m_subTabView(subTabView)
{
  addQObject(subTabView);

  connect(subTabView, &SubTabView::itemSelected, this, &SubTabController::selectItem);

  connect(subTabView, &SubTabView::itemRemoveClicked, this, &SubTabController::removeItem);
  
  connect(subTabView, &SubTabView::itemReplacementDropped, this, &SubTabController::replaceItem);

  connect(subTabView, &SubTabView::selectionCleared, this, &SubTabController::clearSelection);

  connect(subTabView, &SubTabView::itemDropped, this, &SubTabController::handleDrop);

  connect(subTabView, &SubTabView::addClicked, this, &SubTabController::doAdd);

  connect(subTabView, &SubTabView::copyClicked, this, &SubTabController::doCopy);

  connect(subTabView, &SubTabView::removeClicked, this, &SubTabController::doRemove);

  connect(subTabView, &SubTabView::purgeClicked, this, &SubTabController::doPurge);

  connect(subTabView, &SubTabView::downloadComponentsClicked, this, &SubTabController::downloadComponentsClicked);

  connect(subTabView, &SubTabView::openLibDlgClicked, this, &SubTabController::openLibDlgClicked);

  connect(subTabView, &SubTabView::dropZoneItemClicked, this, &SubTabController::inspectItem);
}

SubTabView * SubTabController::subTabView()
{
  return m_subTabView;
}

const SubTabView * SubTabController::subTabView() const
{
  return m_subTabView;
}

void SubTabController::onSelectItem(OSItem* item)
{
  m_subTabView->inspectorView()->selectItem(item);
  m_subTabView->itemSelectorButtons()->enableCopyButton();
  m_subTabView->itemSelectorButtons()->enableRemoveButton();
  m_subTabView->itemSelectorButtons()->enablePurgeButton();
}

void SubTabController::onClearSelection()
{
  m_subTabView->inspectorView()->clearSelection();
  m_subTabView->itemSelectorButtons()->disableCopyButton();
  m_subTabView->itemSelectorButtons()->disableRemoveButton();
  m_subTabView->itemSelectorButtons()->disablePurgeButton();
}

void SubTabController::selectItem(OSItem* item)
{
  onSelectItem(item);
}

void SubTabController::removeItem(OSItem* item)
{
  onRemoveItem(item);
}

void SubTabController::replaceItem(OSItem* item, const OSItemId& replacementItemId)
{
  onReplaceItem(item, replacementItemId);
}

void SubTabController::clearSelection()
{
  onClearSelection();
}

void SubTabController::handleDrop(const OSItemId& itemId)
{
  onDrop(itemId);
}

void SubTabController::doAdd()
{
  onAddItem();
}

void SubTabController::doCopy()
{
  onCopyItem();
}

void SubTabController::doRemove()
{
  onRemoveItem(m_subTabView->itemSelector()->selectedItem());
}

void SubTabController::doPurge()
{
  onPurgeItems();
}

void SubTabController::inspectItem(OSItem* item)
{
  onInspectItem(item);
}

} // openstudio

