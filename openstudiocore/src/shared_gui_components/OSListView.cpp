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

#include "OSListView.hpp"
#include "OSListController.hpp"

#include "../utilities/core/Assert.hpp"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QStyleOption>
#include <QPainter>
#include <QGraphicsObject>

namespace openstudio {

OSListView::OSListView(bool scrollable, QWidget * parent)
  : QWidget(parent),
  m_scrollable(scrollable),
  m_scrollArea(nullptr)
{
  m_delegate = QSharedPointer<OSItemDelegate>(new OSItemDelegate());
  m_mainVLayout = new QVBoxLayout();
  m_mainVLayout->setSizeConstraint(QLayout::SetMinimumSize);
  m_mainVLayout->setAlignment(Qt::AlignTop);

  if( scrollable )
  {
    auto scrollWidget = new QWidget();
    scrollWidget->setObjectName("ScrollWidget");
    scrollWidget->setStyleSheet("QWidget#ScrollWidget { background: transparent; }");
    scrollWidget->setLayout(m_mainVLayout);

    m_scrollArea = new QScrollArea();
    m_scrollArea->setFrameStyle(QFrame::NoFrame);
    m_scrollArea->setWidget(scrollWidget);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setBackgroundRole(QPalette::NoRole);

    auto scrollLayout = new QVBoxLayout();
    scrollLayout->setContentsMargins(0,0,0,0);
    scrollLayout->addWidget(m_scrollArea);

    setLayout(scrollLayout);
  }
  else
  {
    setLayout(m_mainVLayout);
  }

  setContentsMargins(5,5,5,5);
  setSpacing(5);
}

void OSListView::setHorizontalScrollBarAlwaysOn(bool alwaysOn)
{
  if(!m_scrollable && !m_scrollArea) return;

  if(alwaysOn){
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  }
  else{
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  }
}

void OSListView::setVerticalScrollBarAlwaysOn(bool alwaysOn)
{
  if(!m_scrollable && !m_scrollArea) return;

  if(alwaysOn){
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  }
  else{
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  }
}

void OSListView::paintEvent(QPaintEvent *)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void OSListView::setDelegate(QSharedPointer<OSItemDelegate> delegate)
{
  if( delegate )
  { 
    m_delegate = delegate;

    refreshAllViews();
  }
}

void OSListView::setListController(QSharedPointer<OSListController> listController)
{
  if( m_listController )
  {
    m_listController->disconnect(this);
  }

  m_listController = listController;

  connect(m_listController.data(),SIGNAL(itemInserted(int)),this,SLOT(insertItemView(int)));
  connect(m_listController.data(),SIGNAL(itemRemoved(int)),this,SLOT(removeItemView(int)));
  connect(m_listController.data(),SIGNAL(itemChanged(int)),this,SLOT(refreshItemView(int)));
  connect(m_listController.data(),SIGNAL(modelReset()),this,SLOT(refreshAllViews()));

  refreshAllViews();
}

QSharedPointer<OSListController> OSListView::listController() const
{
  return m_listController;
}

void OSListView::setSpacing(int spacing)
{
  m_mainVLayout->setSpacing(spacing);
}

void OSListView::setContentsMargins(int left,int top,int right,int bottom)
{
  m_mainVLayout->setContentsMargins(left,top,right,bottom);
}

void OSListView::refreshAllViews()
{
  QLayoutItem *child;
  while((child = m_mainVLayout->takeAt(0)) != nullptr)
  {
    QWidget * widget = child->widget();

    OS_ASSERT(widget);

    delete widget;

    delete child;
  }

  if( m_listController )
  {
    for( int i = 0, n = m_listController->count(); i < n; i++ )
    {
      insertItemView(i);
    }
  }
}

void OSListView::insertItemView(int i)
{
  OS_ASSERT(m_listController);

  QSharedPointer<OSListItem> itemData = m_listController->itemAt(i);

  OS_ASSERT(itemData);

  QWidget * itemView = m_delegate->view(itemData);

  itemView->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Fixed);

  m_mainVLayout->insertWidget(i,itemView);

  m_widgetItemPairs.insert( std::make_pair(itemView,itemData) );

  bool bingo = connect(itemView,SIGNAL(destroyed(QObject *)),this,SLOT(removePair(QObject *)));

  OS_ASSERT(bingo);
}

void OSListView::removeItemView(int i)
{
  QLayoutItem * item = m_mainVLayout->takeAt(i);

  OS_ASSERT(item);

  QWidget * widget = item->widget();

  OS_ASSERT(widget);

  delete widget;

  delete item;
}

void OSListView::removePair(QObject * object)
{
  m_widgetItemPairs.erase(m_widgetItemPairs.find(object));
}

void OSListView::refreshItemView(int i)
{
  if (i < int(m_widgetItemPairs.size())) {
    removeItemView(i);
  }
  else {
    LOG(Trace,"Not calling removeItemView(" << i << "), because the list is not that long.");
  }

  insertItemView(i);
}

} // openstudio

