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

#ifndef OPENSTUDIO_SUBTABCONTROLLER_H
#define OPENSTUDIO_SUBTABCONTROLLER_H

#include "../shared_gui_components/OSQObjectController.hpp"

#include <model/Model.hpp>

#include <QObject>

namespace openstudio {

class OSItem;
class OSItemId;
class SubTabView;

class SubTabController : public OSQObjectController
{
  Q_OBJECT

  public:

    SubTabController(SubTabView* subTabView);
    virtual ~SubTabController() {}

    SubTabView* subTabView();
    const SubTabView* subTabView() const;

  protected:

    virtual void onSelectItem(OSItem *item);

    virtual void onClearSelection();

    virtual void onAddItem() = 0;

    virtual void onCopyItem() = 0;

    virtual void onRemoveItem(OSItem *item) = 0;

    virtual void onReplaceItem(OSItem *item, const OSItemId& replacementItemId) = 0;

    virtual void onPurgeItems() = 0;

    virtual void onDrop(const OSItemId& itemId) = 0;

    virtual void onInspectItem(OSItem* item) = 0;

    SubTabView* m_subTabView;

signals:

    void openBclDlgClicked();

    void openLibDlgClicked();

  private slots:

    void selectItem(OSItem* item);

    void removeItem(OSItem* item);

    void replaceItem(OSItem* item, const OSItemId& replacementItemId);

    void clearSelection();

    void handleDrop(const OSItemId&);
  
    void doAdd();

    void doCopy();

    void doRemove();

    void doPurge();

    void inspectItem(OSItem* item);

  private:

    boost::optional<openstudio::model::ModelObject> selectedModelObject() const;

    openstudio::IddObjectType currentIddObjectType() const;

    openstudio::model::Model m_model;
};

} // openstudio

#endif // OPENSTUDIO_SUBTABCONTROLLER_H
