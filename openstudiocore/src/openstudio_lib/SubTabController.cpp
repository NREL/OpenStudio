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

#include "SubTabController.hpp"
#include "SubTabView.hpp"
#include "ModelObjectInspectorView.hpp"
#include "ModelObjectItem.hpp"
#include "ModelObjectListView.hpp"
#include "ModelObjectTypeListView.hpp"
#include "OSItemSelector.hpp"
#include "OSItemSelectorButtons.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

SubTabController::SubTabController(SubTabView* subTabView)
  : OSQObjectController(),
    m_subTabView(subTabView)
{
  addQObject(subTabView);

  bool isConnected = false;
  isConnected = connect(subTabView, SIGNAL(itemSelected(OSItem*)),
                        this, SLOT(selectItem(OSItem*)));
  OS_ASSERT(isConnected);

  isConnected = connect(subTabView, SIGNAL(itemRemoveClicked(OSItem*)),
                        this, SLOT(removeItem(OSItem*)));
  OS_ASSERT(isConnected);

  isConnected = connect(subTabView, SIGNAL(itemReplacementDropped(OSItem*, const OSItemId&)),
                        this, SLOT(replaceItem(OSItem*, const OSItemId&)));
  OS_ASSERT(isConnected);

  isConnected = connect(subTabView, SIGNAL(selectionCleared()),
                        this, SLOT(clearSelection()));
  OS_ASSERT(isConnected);

  isConnected = connect(subTabView, SIGNAL(itemDropped(const OSItemId&)),
                        this, SLOT(handleDrop(const OSItemId&)));
  OS_ASSERT(isConnected);

  isConnected = connect(subTabView, SIGNAL(addClicked()),
                        this, SLOT(doAdd()));
  OS_ASSERT(isConnected);

  isConnected = connect(subTabView, SIGNAL(copyClicked()),
                        this, SLOT(doCopy()));
  OS_ASSERT(isConnected);

  isConnected = connect(subTabView, SIGNAL(removeClicked()),
                        this, SLOT(doRemove()));
  OS_ASSERT(isConnected);

  isConnected = connect(subTabView, SIGNAL(purgeClicked()),
                        this, SLOT(doPurge()));
  OS_ASSERT(isConnected);

  isConnected = connect(subTabView, SIGNAL(downloadComponentsClicked()),
                        this, SIGNAL(downloadComponentsClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(subTabView, SIGNAL(openLibDlgClicked()),
                        this, SIGNAL(openLibDlgClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(subTabView, SIGNAL(dropZoneItemClicked(OSItem*)),
                        this, SLOT(inspectItem(OSItem*)));
  OS_ASSERT(isConnected);
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

