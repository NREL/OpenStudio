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

#include "ScriptsController.hpp"
#include "ScriptsView.hpp"
#include "ScriptFolderListView.hpp"
#include "OSItemSelectorButtons.hpp"

#include "../model/Model.hpp"
#include "../model/BuildingStory.hpp"
#include "../model/BuildingStory_Impl.hpp"
#include "../model/Space.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

ScriptsController::ScriptsController(const openstudio::path &t_scriptsPath)
  : SubTabController(new ScriptsView(t_scriptsPath))
{
  connect(this, SIGNAL(updateRubyInterpreterWarning()),
    subTabView(), SIGNAL(updateRubyInterpreterWarning()));

  connect(subTabView(), SIGNAL(toolsUpdated()),
    this, SIGNAL(toolsUpdated()));

  subTabView()->itemSelectorButtons()->disablePurgeButton();
}

const ScriptFolderListView* ScriptsController::scriptFolderListView() const {
  return qobject_cast<const ScriptFolderListView *>(subTabView()->itemSelector());
}

ScriptFolderListView* ScriptsController::scriptFolderListView() {
  return qobject_cast<ScriptFolderListView *>(subTabView()->itemSelector());
}

void ScriptsController::onSelectItem(OSItem *item)
{
  subTabView()->inspectorView()->selectItem(item);
  subTabView()->itemSelectorButtons()->enableCopyButton();
  subTabView()->itemSelectorButtons()->enableRemoveButton();
  //subTabView()->itemSelectorButtons()->enablePurgeButton();
}

void ScriptsController::onAddItem()
{
  const OSItem* item = subTabView()->itemSelector()->selectedItem();
  ScriptFolderListView *lv = qobject_cast<ScriptFolderListView *>(subTabView()->itemSelector());
  OS_ASSERT(lv);

  if (item)
  {
    lv->createEmptyScript(openstudio::toPath(item->itemId().sourceId()));
  } else {
    try {
      lv->createEmptyScript(lv->selectedFolder());
    } catch (...) {
      LOG(Info, "No selected folder");
    }
  }
}

void ScriptsController::onCopyItem()
{
  const OSItem* item = subTabView()->itemSelector()->selectedItem();
  if (item)
  {
    qobject_cast<ScriptFolderListView *>(subTabView()->itemSelector())->duplicateScript(openstudio::toPath(item->itemId().itemId()));
  }
}


void ScriptsController::onRemoveItem(OSItem *item)
{
  if (item)
  {
    qobject_cast<ScriptFolderListView *>(subTabView()->itemSelector())->removeScript(openstudio::toPath(item->itemId().itemId()));
    qobject_cast<ScriptItem *>(item)->deleteDb();
  }
}

void ScriptsController::onReplaceItem(OSItem *item, const OSItemId& replacementItemId)
{
}

void ScriptsController::onPurgeItems()
{
  // not yet implemented
}

void ScriptsController::onDrop(const OSItemId& itemId)
{
  qobject_cast<ScriptFolderListView *>(subTabView()->itemSelector())->addScriptToFolder(openstudio::toPath(itemId.itemId()), 
      openstudio::toPath(itemId.sourceId()));
}

void ScriptsController::onInspectItem(OSItem* item)
{
}

} // openstudio

