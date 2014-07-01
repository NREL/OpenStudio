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

#include "OSItemList.hpp"
#include "OSVectorController.hpp"
#include "ScriptsListView.hpp"

#include "../utilities/core/Assert.hpp"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QStyleOption>
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>

namespace openstudio {


OSItemList::OSItemList(OSVectorController* vectorController,
                       bool addScrollArea,
                       QWidget * parent)
  : OSItemSelector(parent),
    m_vectorController(vectorController),
    m_vLayout(NULL),
    m_selectedItem(NULL),
    m_itemsDraggable(false),
    m_itemsRemoveable(false),
    m_type(OSItemType::ListItem),
    m_dirty(false)
{
  // for now we will allow this item list to manage memory of 
  OS_ASSERT(!m_vectorController->parent());
  m_vectorController->setParent(this);

  this->setObjectName("GrayWidget"); 

  QString style;

  style.append("QWidget#GrayWidget {");
  style.append(" background: #E6E6E6;");
  style.append(" border-bottom: 1px solid black;");
  style.append("}");

  setStyleSheet(style);

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
  m_vLayout->addStretch();

  bool isConnected = false;
  isConnected = connect(this, SIGNAL(itemsRequested()),
                        vectorController, SLOT(reportItems())); 
  OS_ASSERT(isConnected);

  /* Vector controller does not handle removing items in list from model
  *
  isConnected = connect(this, SIGNAL(itemRemoveClicked(OSItem*)),
                        vectorController, SLOT(removeItem(OSItem*)));
  OS_ASSERT(isConnected);
  */

  isConnected = connect(vectorController, SIGNAL(itemIds(const std::vector<OSItemId>&)),
                        this, SLOT(setItemIds(const std::vector<OSItemId>&))); 
  OS_ASSERT(isConnected);

  isConnected = connect(vectorController, SIGNAL(selectedItemId(const OSItemId&)),
                        this, SLOT(selectItemId(const OSItemId&))); 
  OS_ASSERT(isConnected);

  // allow time for OSDocument to finish constructing
  QTimer::singleShot(0, vectorController, SLOT(reportItems()));
}

OSItem* OSItemList::selectedItem() const
{
  return m_selectedItem;
}

OSItem* OSItemList::firstItem()
{
  std::vector<OSItem*> items = this->items();
  if (!items.empty()){
    return items.front();
  }
  return NULL;
}

OSItem* OSItemList::lastItem()
{
  std::vector<OSItem*> items = this->items();
  if (!items.empty()){
    return items.back();
  }
  return NULL;
}

std::vector<OSItem *> OSItemList::items()
{
  std::vector<OSItem *> result;

  for (int i = 0; i < m_vLayout->count(); ++i){
    QLayoutItem * layoutItem = m_vLayout->itemAt(i);
    QWidget * widget = layoutItem->widget();
    OSItem * item = qobject_cast<OSItem*>(widget);

    if (item){
      result.push_back(item);
    }
  }

  return result;
}

OSVectorController* OSItemList::vectorController() const
{
  return m_vectorController;
}

bool OSItemList::itemsDraggable() const
{
  return m_itemsDraggable;
}

void OSItemList::setItemsDraggable(bool itemsDraggable)
{
  m_itemsDraggable = itemsDraggable;
  for (OSItem* item : this->items()){
    item->setDraggable(itemsDraggable);
  }
}

bool OSItemList::itemsRemoveable() const
{
  return m_itemsRemoveable;
}

void OSItemList::setItemsRemoveable(bool itemsRemoveable)
{
  m_itemsRemoveable = itemsRemoveable;
  for (OSItem* item : this->items()){
    item->setRemoveable(itemsRemoveable);
  }
}

void OSItemList::setItemIds(const std::vector<OSItemId>& itemIds)
{
  if (qobject_cast<ScriptsListView*>(this)) {
    std::vector<OSItem*> myItems = items();
    size_t n = myItems.size();
    if ((n > 0) && (itemIds.size() == n)) {
      bool doNothing = true;
      for (size_t i = 0; i < n; ++i) {
        if (std::find(itemIds.begin(),itemIds.end(),myItems[i]->itemId()) == itemIds.end()) {
          doNothing = false;
          break;
        }
      }
      if (doNothing) {
        return;
      }
    }
  }

  QLayoutItem* child;
  while( (child = m_vLayout->takeAt(0)) != 0 ){
    QWidget* widget = child->widget();
    if (widget){
      delete widget;
    }
    delete child;
  }
  m_vLayout->addStretch();

  m_selectedItem = NULL;
  
  for (const OSItemId& itemId : itemIds){
    OSItem* item = OSItem::makeItem(itemId, OSItemType::ListItem);
    if (item){
      addItem(item, false);
    }
  }
  selectItem(firstItem());

  m_dirty = true;
  QTimer::singleShot(0, this, SLOT(refresh()));
}

void OSItemList::refresh()
{
  if (m_dirty){
    m_dirty = false;
  }
}

void OSItemList::addItem(OSItem* item, bool selectItem)
{
  OS_ASSERT(item);

  item->setDraggable(m_itemsDraggable);

  item->setRemoveable(m_itemsRemoveable);

  item->setOSItemType(m_type);

  bool isConnected = false;
  isConnected = connect(item, SIGNAL(itemClicked(OSItem*)),
                        this, SLOT(selectItem(OSItem*)));
  OS_ASSERT(isConnected);

  isConnected = connect(item, SIGNAL(itemRemoveClicked(OSItem*)),
                        this, SIGNAL(itemRemoveClicked(OSItem*)));
  OS_ASSERT(isConnected);

  isConnected = connect(item, SIGNAL(itemReplacementDropped(OSItem*, const OSItemId&)),
                        this, SIGNAL(itemReplacementDropped(OSItem*, const OSItemId&)));
  OS_ASSERT(isConnected);

  m_vLayout->insertWidget(0, item);

  if (selectItem){
    this->selectItem(item);
  }

  m_dirty = true;
  QTimer::singleShot(0, this, SLOT(refresh()));
}

void OSItemList::selectItem(OSItem* selectItem)
{
  if(!selectItem){
    if(m_selectedItem){
      // deselect
      m_selectedItem->setSelected(false);
    }
    m_selectedItem = NULL;
    emit selectionCleared();
    return;
  }

  for (int i = 0; i < m_vLayout->count(); ++i){
    QLayoutItem * layoutItem = m_vLayout->itemAt(i);
    QWidget * widget = layoutItem->widget();
    OSItem * item = qobject_cast<OSItem*>(widget);

    if (item){
      if (selectItem->equal(item)){

        if(m_selectedItem){
          if (m_selectedItem->equal(item)){
            // already selected
            return;
          }else{
            // deselect
            m_selectedItem->setSelected(false);
          }
        }

        m_selectedItem = item;
        m_selectedItem->setSelected(true);
        emit itemSelected(m_selectedItem);
        return;
      }
    }
  }

  // selected object was not found
  if(m_selectedItem){
    // deselect
    m_selectedItem->setSelected(false);
  }
  m_selectedItem = NULL;
  emit selectionCleared();
}

void OSItemList::selectItemId(const OSItemId& itemId)
{
  for (int i = 0; i < m_vLayout->count(); ++i){
    QLayoutItem* layoutItem = m_vLayout->itemAt(i);
    QWidget* widget = layoutItem->widget();
    OSItem* item = qobject_cast<OSItem*>(widget);

    if (item){
      if (item->itemId() == itemId){
        selectItem(item);
        break;
      }
    }
  }
}

void OSItemList::clearSelection()
{
  m_selectedItem = NULL;

  for (int i = 0; i < m_vLayout->count(); ++i){
    QLayoutItem * layoutItem = m_vLayout->itemAt(i);
    QWidget * widget = layoutItem->widget();
    OSItem * item = qobject_cast<OSItem*>(widget);
    if (item){
      item->setSelected(false);
    }
  }
}

void OSItemList::paintEvent( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

OSItemType OSItemList::itemsType() const
{
  return m_type;
}

void OSItemList::setItemsType(OSItemType type)
{
  m_type = type;
  for (OSItem* item : this->items()){
    item->setOSItemType(type);
  }
}

} // openstudio

