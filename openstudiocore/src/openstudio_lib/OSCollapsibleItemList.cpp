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
  m_vLayout(nullptr),
  m_contentLayout(nullptr),
  m_selectedCollapsibleItem(nullptr),
  m_itemsDraggable(false),
  m_itemsRemoveable(false),
  m_showFilterLayout(false),
  m_itemsType(OSItemType::ListItem)
{ 
  this->setObjectName("GrayWidget"); 

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
  m_vLayout->addStretch(10);
}

OSCollapsibleItem* OSCollapsibleItemList::selectedCollapsibleItem() const
{
  return m_selectedCollapsibleItem;
}

OSItem* OSCollapsibleItemList::selectedItem() const
{
  OSItem* result = nullptr;
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

  connect(collapsibleItem, &OSCollapsibleItem::collapsableItemSelected, this, &OSCollapsibleItemList::onCollapsableItemSelected);

  connect(collapsibleItem, &OSCollapsibleItem::itemSelected, this, &OSCollapsibleItemList::onItemSelected);

  connect(collapsibleItem, &OSCollapsibleItem::itemSelected, this, &OSCollapsibleItemList::itemSelected);

  connect(collapsibleItem, &OSCollapsibleItem::itemReplacementDropped, this, &OSCollapsibleItemList::itemReplacementDropped);

  connect(collapsibleItem, &OSCollapsibleItem::itemRemoveClicked, this, &OSCollapsibleItemList::itemRemoveClicked);

  connect(collapsibleItem, &OSCollapsibleItem::selectionCleared, this, &OSCollapsibleItemList::selectionCleared);

  connect(collapsibleItem, &OSCollapsibleItem::openLibDlgClicked, this, &OSCollapsibleItemList::openLibDlgClicked);

  if (!selectedItem()){
    collapsibleItem->itemList()->selectItem(collapsibleItem->itemList()->firstItem());
  }

  collapsibleItem->setSelected(true);
}

void OSCollapsibleItemList::onCollapsableItemSelected(OSCollapsibleItem* selectedItem)
{
  QLayoutItem * layoutItem = nullptr;
  OSCollapsibleItem* collapsibleItem = nullptr;
  OSItem* newSelectedItem = nullptr;

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
  QLayoutItem * layoutItem = nullptr;
  OSCollapsibleItem* collapsibleItem = nullptr;

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

