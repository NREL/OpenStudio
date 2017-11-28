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

#include "OSItemList.hpp"
#include "OSVectorController.hpp"

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
    m_vLayout(nullptr),
    m_selectedItem(nullptr),
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

  auto outerVLayout = new QVBoxLayout();
  outerVLayout->setContentsMargins(0,0,0,0);
  this->setLayout(outerVLayout);

  auto outerWidget = new QWidget();

  if (addScrollArea){
    auto scrollArea = new QScrollArea();
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

  connect(this, &OSItemList::itemsRequested, vectorController, &OSVectorController::reportItems);

  /* Vector controller does not handle removing items in list from model
  *
  connect(this, &OSItemList::itemRemoveClicked, vectorController, &OSVectorController::removeItem);
  */

  connect(vectorController, &OSVectorController::itemIds, this, &OSItemList::setItemIds);

  connect(vectorController, &OSVectorController::selectedItemId, this, &OSItemList::selectItemId);

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
  return nullptr;
}

OSItem* OSItemList::lastItem()
{
  std::vector<OSItem*> items = this->items();
  if (!items.empty()){
    return items.back();
  }
  return nullptr;
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
  /* DLM: ScriptsListView was removed
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
  */

  QLayoutItem* child;
  while( (child = m_vLayout->takeAt(0)) != nullptr ){
    QWidget* widget = child->widget();
    if (widget){
      delete widget;
    }
    delete child;
  }
  m_vLayout->addStretch();

  m_selectedItem = nullptr;
  
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

  connect(item, &OSItem::itemClicked, this, &OSItemList::selectItem);

  connect(item, &OSItem::itemRemoveClicked, this, &OSItemList::itemRemoveClicked);

  connect(item, &OSItem::itemReplacementDropped, this, &OSItemList::itemReplacementDropped);

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
    m_selectedItem = nullptr;
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
  m_selectedItem = nullptr;
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
  m_selectedItem = nullptr;

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

