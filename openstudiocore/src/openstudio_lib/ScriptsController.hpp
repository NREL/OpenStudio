/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_SCRIPTSCONTROLLER_H
#define OPENSTUDIO_SCRIPTSCONTROLLER_H

#include <openstudio_lib/SubTabController.hpp>

namespace openstudio {

class ScriptFolderListView;

class ScriptsController : public SubTabController
{
  Q_OBJECT

  public:

    ScriptsController(const openstudio::path &t_scriptsPath);

    virtual ~ScriptsController() {}

    const ScriptFolderListView* scriptFolderListView() const;

    ScriptFolderListView* scriptFolderListView();

  signals:
    void updateRubyInterpreterWarning();
    void toolsUpdated();

  protected:

    virtual void onSelectItem(OSItem *item);

    virtual void onAddItem();

    virtual void onCopyItem();

    virtual void onRemoveItem(OSItem *item);

    virtual void onReplaceItem(OSItem *item, const OSItemId& replacementItemId);

    virtual void onPurgeItems();

    virtual void onDrop(const OSItemId& itemId);

    virtual void onInspectItem(OSItem* item);

  private:
    REGISTER_LOGGER("openstudio::ScriptsController");
};

} // openstudio

#endif // OPENSTUDIO_SCRIPTSCONTROLLER_H
