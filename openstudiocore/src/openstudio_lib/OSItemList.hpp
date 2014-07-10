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

#ifndef OPENSTUDIO_OSITEMLIST_HPP
#define OPENSTUDIO_OSITEMLIST_HPP

#include "OSItemSelector.hpp"
#include "OSItem.hpp"

class QVBoxLayout;

namespace openstudio {

class OSItem;
class OSVectorController;

class OSItemList : public OSItemSelector {
  Q_OBJECT
 public:

  OSItemList(OSVectorController* vectorController,
             bool addScrollArea,
             QWidget* parent = 0);

  virtual ~OSItemList() {}

  // return the selected item in the list, NULL if list is empty
  OSItem* selectedItem() const;

  // return the first item in the list, NULL if list is empty
  OSItem* firstItem();

  // return the last item in the list, NULL if list is empty
  OSItem* lastItem();

  // return all items in the list
  std::vector<OSItem *> items();

  OSVectorController* vectorController() const;

  bool itemsDraggable() const;
  void setItemsDraggable(bool itemsDraggable);

  bool itemsRemoveable() const;
  void setItemsRemoveable(bool itemsRemoveable);

  OSItemType itemsType() const;
  void setItemsType(OSItemType type);

public slots:

  // adds an item, optionally selects it
  void addItem(OSItem* item, bool selectItem = true);

  // selects an item, emits objectSelected if object is selected, emits selectionCleared if item is null or not found
  void selectItem(OSItem* item);

  // selects an item by id
  void selectItemId(const OSItemId& itemId);

  // clears the selection, does not emit selectionCleared
  void clearSelection();

private slots:

  void setItemIds(const std::vector<OSItemId>& itemIds);

  void refresh();

signals:

  void itemsRequested();

protected:

  void paintEvent( QPaintEvent * event );

private:

  OSVectorController* m_vectorController;
  QVBoxLayout * m_vLayout;
  OSItem * m_selectedItem;
  bool m_itemsDraggable;
  bool m_itemsRemoveable;
  OSItemType m_type;
  bool m_dirty;

};

} // openstudio

#endif // OPENSTUDIO_OSITEMLIST_HPP

