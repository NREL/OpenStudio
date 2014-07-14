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

#ifndef OPENSTUDIO_OSCOLLAPSIBLEITEMLIST_HPP
#define OPENSTUDIO_OSCOLLAPSIBLEITEMLIST_HPP

#include "OSItemSelector.hpp"
#include "OSItem.hpp"

class QVBoxLayout;
class QHBoxLayout;

namespace openstudio {

class OSItem;
class OSCollapsibleItem;
class OSCollapsibleItemHeader;
class OSVectorController;

class OSCollapsibleItemList : public OSItemSelector
{
  Q_OBJECT

public:

  OSCollapsibleItemList(bool addScrollArea, QWidget * parent = 0);

  virtual ~OSCollapsibleItemList() {}

  void addCollapsibleItem(OSCollapsibleItem* collapsibleItem);

  OSCollapsibleItem* selectedCollapsibleItem() const;

  OSItem* selectedItem() const;

  bool itemsDraggable() const;
  void setItemsDraggable(bool itemsDraggable);

  bool itemsRemoveable() const;
  void setItemsRemoveable(bool itemsRemoveable);

  OSItemType itemsType() const;
  void setItemsType(OSItemType type);

  bool showFilterLayout() const;
  void setShowFilterLayout(const bool showFilterLayout);

signals:

    void openLibDlgClicked();

private slots:

  void onCollapsableItemSelected(OSCollapsibleItem* selectedItem);

  void onItemSelected(OSItem* item);

protected:

  void paintEvent( QPaintEvent * event );

private:

  QVBoxLayout * m_vLayout;
  QHBoxLayout * m_contentLayout;
  OSCollapsibleItem * m_selectedCollapsibleItem;
  std::vector<OSCollapsibleItem *> m_collapsibleItems;
  bool m_itemsDraggable;
  bool m_itemsRemoveable;
  bool m_showFilterLayout;
  OSItemType m_itemsType;
};

} // openstudio

#endif // OPENSTUDIO_OSCOLLAPSIBLEITEMLIST_HPP

