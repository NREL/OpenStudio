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

#include "OSCollapsibleItemList.hpp"
#include "OSItem.hpp"
#include "OSItemList.hpp"
#include "OSCollapsibleItem.hpp"
#include "OSCollapsibleItemHeader.hpp"

#include "../utilities/core/Assert.hpp"

#include <QHBoxLayout>
#include <QPainter>
#include <QPushButton>
#include <QResizeEvent>
#include <QScrollArea>
#include <QStyleOption>
#include <QVBoxLayout>

namespace openstudio {

OSCollapsibleItemList::OSCollapsibleItemList(bool addScrollArea, QWidget * parent)
  : OSItemSelector(parent),
  m_vLayout(NULL),
  m_contentLayout(NULL),
  m_selectedCollapsibleItem(NULL),
  m_itemsDraggable(false),
  m_itemsRemoveable(false),
  m_showFilterLayout(false),
  m_itemsType(OSItemType::ListItem)
{ 
  this->setObjectName("GrayWidget"); 

  QVBoxLayout* outerVLayout = new QVBoxLayout();
  outerVLayout->setContentsMargins(0,0,0,0);
  this->setLayout(outerVLayout);

  QWidget* outerWidget = new QWidget();

  if (addScrollArea){
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setFrameStyle(QFrame::NoFrame);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    outerVLayout->addWidget(scrollArea);
    scrollArea->setWidget(outerWidget);
    scrollArea->setWidgetResizable(true);
  }else{
    outerVLayout->addWidget(outerWidget);
  }

  m_vLayout = new QVBoxLayout();
  outerWidget->setLayout(m_vLayout);
  m_vLayout->setContentsMargins(0,0,0,0);
  m_vLayout->setSpacing(0);
  m_vLayout->addStretch(10);
}

OSCollapsibleItem* OSCollapsibleItemList::selectedCollapsibleItem() const
{
  return m_selectedCollapsibleItem;
}

OSItem* OSCollapsibleItemList::selectedItem() const
{
  OSItem* result = NULL;
  if (m_selectedCollapsibleItem){
    result = m_selectedCollapsibleItem->itemList()->selectedItem();
  }
  return result;
}

bool OSCollapsibleItemList::itemsDraggable() const
{
  return m_itemsDraggable;
}

void OSCollapsibleItemList::setItemsDraggable(bool itemsDraggable)
{
  m_itemsDraggable = itemsDraggable;
  for (OSCollapsibleItem* collapsibleItem : m_collapsibleItems){
    collapsibleItem->setItemsDraggable(itemsDraggable);
  }
}

bool OSCollapsibleItemList::itemsRemoveable() const
{
  return m_itemsRemoveable;
}

void OSCollapsibleItemList::setItemsRemoveable(bool itemsRemoveable)
{
  m_itemsRemoveable = itemsRemoveable;
  for (OSCollapsibleItem* collapsibleItem : m_collapsibleItems){
    collapsibleItem->setItemsRemoveable(itemsRemoveable);
  }
}

OSItemType OSCollapsibleItemList::itemsType() const
{
  return m_itemsType;
}

void OSCollapsibleItemList::setItemsType(OSItemType type)
{
  m_itemsType = type;
  for (OSCollapsibleItem* collapsibleItem : m_collapsibleItems){
    collapsibleItem->setItemsType(type);
  }
}

bool OSCollapsibleItemList::showFilterLayout() const
{
  return m_showFilterLayout;
}

void OSCollapsibleItemList::setShowFilterLayout(const bool showFilterLayout)
{
  m_showFilterLayout = showFilterLayout;
  for (OSCollapsibleItem* collapsibleItem : m_collapsibleItems){
    collapsibleItem->setShowFilterLayout(showFilterLayout);
  }
}

void OSCollapsibleItemList::addCollapsibleItem(OSCollapsibleItem * collapsibleItem)
{
  OS_ASSERT(collapsibleItem);

  collapsibleItem->setItemsDraggable(m_itemsDraggable);

  collapsibleItem->setItemsRemoveable(m_itemsRemoveable);

  collapsibleItem->setShowFilterLayout(m_showFilterLayout);

  collapsibleItem->setItemsType(m_itemsType);

  m_vLayout->insertWidget(0, collapsibleItem);

  QString style;
  style.append( "QWidget#OSCollapsibleItemList {" );
  style.append( "background: #F2F2F2; ");
  style.append( "border-bottom: 1px solid black; " );
  style.append( "}" );
  style.append( "QWidget#SideBar {background: #EEDEDE;}" );

  collapsibleItem->setStyleSheet(style);

  bool isConnected = false;

  isConnected = connect(collapsibleItem, SIGNAL(collapsableItemSelected(OSCollapsibleItem *)),
                        this, SLOT(onCollapsableItemSelected(OSCollapsibleItem *)));
  OS_ASSERT(isConnected);

  isConnected = connect(collapsibleItem, SIGNAL(itemSelected(OSItem*)),
                        this, SLOT(onItemSelected(OSItem*)));
  OS_ASSERT(isConnected);

  isConnected = connect(collapsibleItem, SIGNAL(itemSelected(OSItem*)),
                        this, SIGNAL(itemSelected(OSItem*)));
  OS_ASSERT(isConnected);

  isConnected = connect(collapsibleItem, SIGNAL(itemReplacementDropped(OSItem*, const OSItemId&)),
                        this, SIGNAL(itemReplacementDropped(OSItem*, const OSItemId&)));
  OS_ASSERT(isConnected);

  isConnected = connect(collapsibleItem, SIGNAL(itemRemoveClicked(OSItem*)),
                        this, SIGNAL(itemRemoveClicked(OSItem*)));
  OS_ASSERT(isConnected);

  isConnected = connect(collapsibleItem, SIGNAL(selectionCleared()),
                        this, SIGNAL(selectionCleared()));
  OS_ASSERT(isConnected);

  isConnected = connect(collapsibleItem,SIGNAL(openLibDlgClicked()),
                        this,SIGNAL(openLibDlgClicked()));
  OS_ASSERT(isConnected);

  if (!selectedItem()){
    collapsibleItem->itemList()->selectItem(collapsibleItem->itemList()->firstItem());
  }

  collapsibleItem->setSelected(true);
}

void OSCollapsibleItemList::onCollapsableItemSelected(OSCollapsibleItem* selectedItem)
{
  QLayoutItem * layoutItem = NULL;
  OSCollapsibleItem* collapsibleItem = NULL;
  OSItem* newSelectedItem = NULL;

  for (int i = 0; i < m_vLayout->count(); ++i){

    layoutItem = m_vLayout->itemAt(i);
    QWidget * widget = layoutItem->widget();

    collapsibleItem = qobject_cast<OSCollapsibleItem*>(widget);
    if (collapsibleItem){
      if (collapsibleItem == selectedItem){

        if (m_selectedCollapsibleItem != collapsibleItem){
          // select new item
          m_selectedCollapsibleItem = collapsibleItem;
          m_selectedCollapsibleItem->setExpanded(true);
          newSelectedItem = m_selectedCollapsibleItem->itemList()->firstItem();
          m_selectedCollapsibleItem->itemList()->selectItem(newSelectedItem);
        }

      }else{
        // deselect
        collapsibleItem->setSelected(false);
        collapsibleItem->setExpanded(false);
        collapsibleItem->itemList()->clearSelection();
      }
    }
  }
}

void OSCollapsibleItemList::onItemSelected(OSItem* item)
{
  QLayoutItem * layoutItem = NULL;
  OSCollapsibleItem* collapsibleItem = NULL;

  for (int i = 0; i < m_vLayout->count(); ++i){

    layoutItem = m_vLayout->itemAt(i);
    QWidget * widget = layoutItem->widget();

    collapsibleItem = qobject_cast<OSCollapsibleItem*>(widget);
    if (collapsibleItem){
      std::vector<OSItem*> items = collapsibleItem->itemList()->items();
      if (std::find(items.begin(), items.end(), item) != items.end()){

        // select item
        m_selectedCollapsibleItem = collapsibleItem;
        m_selectedCollapsibleItem->setSelected(true);

      }else{
        // deselect
        collapsibleItem->setSelected(false);
        collapsibleItem->itemList()->clearSelection();
      }
    }
  }
}

void OSCollapsibleItemList::paintEvent(QPaintEvent * event)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


} // openstudio

