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

#include "OSGridView.hpp"
#include "OSGridController.hpp"

#include <QGraphicsObject>
#include <QPainter>
#include <QScrollArea>
#include <QStyleOption>
#include <QVBoxLayout>

#include <utilities/core/Assert.hpp>

namespace openstudio {

OSGridView::OSGridView(bool scrollable, QWidget * parent)
  : QWidget(parent),
  m_scrollable(scrollable),
  m_scrollArea(NULL)
{
  m_delegate = QSharedPointer<OSGridItemDelegate>(new OSGridItemDelegate());
  m_gridLayout = new QGridLayout();
  m_gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
  m_gridLayout->setAlignment(Qt::AlignTop);

  if( scrollable )
  {
    QWidget * scrollWidget = new QWidget();
    scrollWidget->setObjectName("ScrollWidget");
    scrollWidget->setStyleSheet("QWidget#ScrollWidget { background: transparent; }");
    scrollWidget->setLayout(m_gridLayout);

    m_scrollArea = new QScrollArea();
    m_scrollArea->setFrameStyle(QFrame::NoFrame);
    m_scrollArea->setWidget(scrollWidget);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setBackgroundRole(QPalette::NoRole);

    QVBoxLayout * scrollLayout = new QVBoxLayout();
    scrollLayout->setContentsMargins(0,0,0,0);
    scrollLayout->addWidget(m_scrollArea);

    setLayout(scrollLayout);
  }
  else
  {
    setLayout(m_gridLayout);
  }

  setContentsMargins(5,5,5,5);
  setSpacing(5);
}

void OSGridView::setHorizontalScrollBarAlwaysOn(bool alwaysOn)
{
  if(!m_scrollable && !m_scrollArea) return;

  if(alwaysOn){
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  }
  else{
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  }
}

void OSGridView::setVerticalScrollBarAlwaysOn(bool alwaysOn)
{
  if(!m_scrollable && !m_scrollArea) return;

  if(alwaysOn){
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  }
  else{
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  }
}

void OSGridView::paintEvent(QPaintEvent *)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void OSGridView::setDelegate(QSharedPointer<OSGridItemDelegate> delegate)
{
  if( delegate )
  { 
    m_delegate = delegate;

    refreshAllViews();
  }
}

void OSGridView::setGridController(QSharedPointer<OSGridController> gridController)
{
  if( m_gridController )
  {
    m_gridController->disconnect(this);
  }

  m_gridController = gridController;

  connect(m_gridController.data(),SIGNAL(itemInserted(int,int)),this,SLOT(insertItemView(int,int)));
  connect(m_gridController.data(),SIGNAL(itemRemoved(int,int)),this,SLOT(removeItemView(int,int)));
  connect(m_gridController.data(),SIGNAL(itemChanged(int,int)),this,SLOT(refreshItemView(int,int)));
  connect(m_gridController.data(),SIGNAL(modelReset()),this,SLOT(refreshAllViews()));

  refreshAllViews();
}

QSharedPointer<OSGridController> OSGridView::gridController() const
{
  return m_gridController;
}

void OSGridView::setSpacing(int spacing)
{
  m_gridLayout->setSpacing(spacing);
}

void OSGridView::setContentsMargins(int left,int top,int right,int bottom)
{
  m_gridLayout->setContentsMargins(left,top,right,bottom);
}

void OSGridView::refreshAllViews()
{
  QLayoutItem *child;
  while((child = m_gridLayout->takeAt(0)) != 0)
  {
      QWidget * widget = child->widget();

      OS_ASSERT(widget);

      delete widget;

      delete child;
  }

  if( m_gridController )
  {
    for( int i = 0, n = m_gridController->rowCount(); i < n; i++ )
    {
      for( int j = 0, n = m_gridController->columnCount(); i < n; i++ )
      {
        insertItemView(i,j);
      }
    }
  }
}

void OSGridView::insertItemView(int row, int column)
{
  //OS_ASSERT(m_gridController);

  //QSharedPointer<OSGridItem> itemData = m_gridController->itemAt(row,column);

  //OS_ASSERT(itemData);

  //QWidget * itemView = m_delegate->view(itemData);

  //itemView->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Fixed);

  //m_gridLayout->insertWidget(i,itemView);

  //m_widgetItemPairs.insert( std::make_pair<QObject *,QSharedPointer<OSGridItem> >(itemView,itemData) );

  //bool bingo = connect(itemView,SIGNAL(destroyed(QObject *)),this,SLOT(removePair(QObject *)));

  //OS_ASSERT(bingo);
}

void OSGridView::removeItemView(int row, int column)
{
  //QLayoutItem * item = m_gridLayout->takeAt(i);

  //OS_ASSERT(item);

  //QWidget * widget = item->widget();

  //OS_ASSERT(widget);

  //delete widget;

  //delete item;
}

void OSGridView::removePair(QObject * object)
{
  m_widgetItemPairs.erase(m_widgetItemPairs.find(object));
}

void OSGridView::refreshItemView(int row, int column)
{
  removeItemView(row, column);

  insertItemView(row, column);
}

} // openstudio

