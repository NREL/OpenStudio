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

#include "RefrigerationGraphicsItems.hpp"
#include "../shared_gui_components/OSListController.hpp"
#include "../shared_gui_components/GraphicsItems.hpp"
#include "../utilities/core/Assert.hpp"
#include <QPainter>
#include <utility>

namespace openstudio {

const int RefrigerationSystemItem::verticalSpacing = 50;

RefrigerationSystemItemDelegate::RefrigerationSystemItemDelegate()
{
}

QGraphicsObject * RefrigerationSystemItemDelegate::view(QSharedPointer<OSListItem> dataSource)
{
  RefrigerationSystemGridCellItem * item = NULL;

  if( dataSource )
  {
    item = new RefrigerationSystemGridCellItem();

    bool bingo;
    bingo = connect(item->removeButtonItem,SIGNAL(mouseClicked()),dataSource.data(),SLOT(remove()));
    OS_ASSERT(bingo);
  }

  return item;
}

RefrigerationSystemGridItem::RefrigerationSystemGridItem()
{
}

QRectF RefrigerationSystemGridItem::boundingRect() const
{
  int x = columns() * (RefrigerationSystemGridCellItem::cellSize().width() + spacing()) - spacing();
  int y = rows() * (RefrigerationSystemGridCellItem::cellSize().height() + spacing()) - spacing();

  return QRectF(0,0,x,y);
}

void RefrigerationSystemGridItem::setDelegate(QSharedPointer<RefrigerationSystemItemDelegate> delegate)
{
  if( delegate )
  { 
    m_delegate = delegate;

    refreshAllItemViews();
  }
}

void RefrigerationSystemGridItem::setListController(QSharedPointer<OSListController> listController)
{
  if( m_listController )
  {
    m_listController->disconnect(this);
  }

  m_listController = listController;

  connect(m_listController.data(),SIGNAL(itemInserted(int)),this,SLOT(insertItemView(int)));
  connect(m_listController.data(),SIGNAL(itemRemoved(int)),this,SLOT(removeItemView(int)));
  connect(m_listController.data(),SIGNAL(itemChanged(int)),this,SLOT(refreshItemView(int)));
  connect(m_listController.data(),SIGNAL(modelReset()),this,SLOT(refreshAllItemViews()));

  refreshAllItemViews();
}

QSharedPointer<OSListController> RefrigerationSystemGridItem::listController() const
{
  return m_listController;
}

void RefrigerationSystemGridItem::refreshAllItemViews()
{
  prepareGeometryChange();

  QList<QGraphicsItem *> itemList = childItems();
  for( QList<QGraphicsItem *>::iterator it = itemList.begin(); 
       it < itemList.end(); 
       it++ )
  {
    delete *it;
  }

  if( m_listController )
  {
    for( int i = 0; i < m_listController->count(); i++ )
    {
      insertItemView(i);
    }
  }
}

QGraphicsItem * RefrigerationSystemGridItem::createNewItemView(int i)
{
  if( m_listController && m_delegate )
  {
    QSharedPointer<OSListItem> itemData = m_listController->itemAt(i);

    OS_ASSERT(itemData);

    QGraphicsObject * graphicsItem = m_delegate->view(itemData);

    OS_ASSERT(graphicsItem);

    graphicsItem->setParentItem(this);

    //m_widgetItemPairs.insert( std::make_pair<QGraphicsObject *,QSharedPointer<OSListItem> >(graphicsItem,itemData) );

    //bool bingo = connect(graphicsItem,SIGNAL(destroyed(QObject *)),this,SLOT(removePair(QObject *)));

    //OS_ASSERT(bingo);

    return graphicsItem;
  }
  else
  {
    return NULL;
  }
}

void RefrigerationSystemGridItem::setItemViewGridPos(QGraphicsItem * item,std::pair<int,int> gridPos)
{
  if( item )
  {
    int x = gridPos.second * (RefrigerationSystemGridCellItem::cellSize().width() + spacing());
    int y = gridPos.first * (RefrigerationSystemGridCellItem::cellSize().height() + spacing());

    item->setPos(x,y);
  }
}

void RefrigerationSystemGridItem::insertItemView(int index)
{
  if( m_listController )
  {
    // Move Everything after index forward one grid position
    for( int i = index + 1; i < m_listController->count(); i++ )
    {
      std::pair<int,int> oldPos = gridPos(i - 1);

      QGraphicsItem * item = viewFromGridPos(oldPos);

      std::pair<int,int> newPos = gridPos(i);

      setItemViewGridPos(item,newPos);
    }

    // Create new item and position it at index grid position
    if( QGraphicsItem * item = createNewItemView(index) )
    {
      std::pair<int,int> pos = gridPos(index);

      setItemViewGridPos(item,pos);
    }
  }
}

void RefrigerationSystemGridItem::removeItemView(int index)
{
  if( m_listController )
  {
    prepareGeometryChange();

    // Remove Item
    if(QGraphicsItem * item = viewFromGridPos(gridPos(index)))
    {
      delete item;
    }

    // Move Everything after index back one grid position
    for( int i = index + 1; i < m_listController->count() + 1; i++ )
    {
      std::pair<int,int> oldPos = gridPos(i);

      QGraphicsItem * item = viewFromGridPos(oldPos);

      std::pair<int,int> newPos = gridPos(i - 1);

      setItemViewGridPos(item,newPos);
    }
  }
}

void RefrigerationSystemGridItem::removePair(QObject * object)
{
  //if( object )
  //{
  //  std::map<QObject *,QSharedPointer<OSListItem> >::iterator it = m_widgetItemPairs.find(object);

  //  if( it != m_widgetItemPairs.end() )
  //  {
  //    m_widgetItemPairs.erase(it);
  //  }
  //}
}

void RefrigerationSystemGridItem::refreshItemView(int i)
{
}

int RefrigerationSystemGridItem::spacing() const
{
  return 25;
}

int RefrigerationSystemGridItem::rows() const
{
  int result = 0;

  if( m_listController )
  {
    int count = m_listController->count();

    int _columns = columns();

    result = count / _columns;

    if( (count % _columns) != 0 )
    {
      result++;
    }
  }

  return result;
}

int RefrigerationSystemGridItem::columns() const
{
  return 2;
}

std::pair<int,int> RefrigerationSystemGridItem::gridPos(int index) 
{
  int row = 0;
  int column = 0;

  if( m_listController )
  {
    int _columns = columns();

    row = index / _columns;

    column = index % _columns;  
  }

  return std::pair<int,int>(row,column);
}

QGraphicsItem * RefrigerationSystemGridItem::viewFromGridPos(std::pair<int,int> location)
{
  QGraphicsItem * result = NULL;

  int x = location.second * (RefrigerationSystemGridCellItem::cellSize().width() + spacing()) + RefrigerationSystemGridCellItem::cellSize().width() / 2;
  int y = location.first * (RefrigerationSystemGridCellItem::cellSize().height() + spacing()) + RefrigerationSystemGridCellItem::cellSize().height() / 2;

  QPoint point(x,y);

  QList<QGraphicsItem *>  items = childItems();

  for(QList<QGraphicsItem *>::iterator it = items.begin();
      it != items.end();
      it++)
  {
    QRect rect((*it)->pos().x(),(*it)->pos().y(),RefrigerationSystemGridCellItem::cellSize().width(),RefrigerationSystemGridCellItem::cellSize().height());

    if( (*it)->contains(point) )
    {
      result = *it;
    }
  }

  return result;
}

RefrigerationSystemGridCellItem::RefrigerationSystemGridCellItem()
{
  refrigerationSystemItem = new RefrigerationSystemItem();
  refrigerationSystemItem->setParentItem(this);

  refrigerationSystemItem->setTransform(QTransform((contentRect().width() - 20) / refrigerationSystemItem->boundingRect().width(),
                                        0,0,
                                        (contentRect().height() - 20)  / refrigerationSystemItem->boundingRect().height(),
                                        0,0));

  refrigerationSystemItem->setPos(contentRect().x() + 10,contentRect().y() + 10);

  removeButtonItem = new RemoveButtonItem();
  removeButtonItem->setParentItem(this);
  removeButtonItem->setPos(cellWidth() - removeButtonItem->boundingRect().width() - 10,10);
}

QRectF RefrigerationSystemGridCellItem::boundingRect() const
{
  return QRectF(QPoint(0,0),cellSize());
}

int RefrigerationSystemGridCellItem::cellWidth()
{
  return 450;
}

int RefrigerationSystemGridCellItem::headerHeight()
{
  return 75;
}

QSize RefrigerationSystemGridCellItem::cellSize()
{
  return QSize(cellWidth(),cellWidth() + headerHeight());
}

QRectF RefrigerationSystemGridCellItem::contentRect() const
{
  return QRectF(0,headerHeight(),cellWidth(),cellWidth());
}

QRectF RefrigerationSystemGridCellItem::headerRect() const
{
  return QRectF(0,0,cellWidth(),headerHeight());
}

void RefrigerationSystemGridCellItem::paint( QPainter *painter, 
                                             const QStyleOptionGraphicsItem *option, 
                                             QWidget *widget )
{
  // Background and Border

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);

  painter->setPen(QPen(Qt::black,1,Qt::SolidLine, Qt::RoundCap));
  painter->drawRect(boundingRect());

  painter->setPen(QPen(Qt::black,2,Qt::SolidLine, Qt::RoundCap));
  painter->drawRect(headerRect());
}

RefrigerationSystemItem::RefrigerationSystemItem()
  : QGraphicsObject()
{
  // Condenser Item

  refrigerationCondenserItem = new RefrigerationCondenserItem();
  refrigerationCondenserItem->setParentItem(this);

  // Sub Cooler Item

  refrigerationSubCoolerItem = new RefrigerationSubCoolerItem();
  refrigerationSubCoolerItem->setParentItem(this);

  // Heat Reclaim Item

  refrigerationHeatReclaimItem = new RefrigerationHeatReclaimItem();
  refrigerationHeatReclaimItem->setParentItem(this);

  // Compressor Item

  refrigerationCompressorItem = new RefrigerationCompressorItem();
  refrigerationCompressorItem->setParentItem(this);

  // Liquid Suction HX Item

  refrigerationSHXItem = new RefrigerationSHXItem();
  refrigerationSHXItem->setParentItem(this);

  // Cases Item

  refrigerationCasesItem = new RefrigerationCasesItem();
  refrigerationCasesItem->setParentItem(this);

  // Secondary Item

  refrigerationSecondaryItem = new RefrigerationSecondaryItem();
  refrigerationSecondaryItem->setParentItem(this);

  adjustLayout();
}

void RefrigerationSystemItem::adjustLayout()
{
  prepareGeometryChange();

  // Condenser

  refrigerationCondenserItem->setPos(centerXPos() - refrigerationCondenserItem->boundingRect().width() / 2, 
                                     verticalSpacing);

  // Mechanical Sub Cooler

  refrigerationSubCoolerItem->setPos(leftXPos() - refrigerationSubCoolerItem->boundingRect().width() / 2, 
                                     refrigerationCondenserItem->y() + refrigerationCondenserItem->boundingRect().height() + verticalSpacing);

  // Heat Reclaim

  refrigerationHeatReclaimItem->setPos(rightXPos() - refrigerationHeatReclaimItem->boundingRect().width() / 2, 
                                       refrigerationCondenserItem->y() + refrigerationCondenserItem->boundingRect().height() + verticalSpacing);
  
  // Compressor

  refrigerationCompressorItem->setPos(rightXPos() - refrigerationCompressorItem->boundingRect().width() / 2, 
                                      refrigerationHeatReclaimItem->y() + refrigerationHeatReclaimItem->boundingRect().height() + verticalSpacing / 2);

  // SHX

  refrigerationSHXItem->setPos(leftXPos() - refrigerationSHXItem->boundingRect().width() / 2, 
                               refrigerationSubCoolerItem->y() + refrigerationSubCoolerItem->boundingRect().height() + verticalSpacing);

  // Cases

  refrigerationCasesItem->setPos(centerXPos() - refrigerationCasesItem->boundingRect().width() / 2, 
                                 refrigerationSHXItem->y() + refrigerationSHXItem->boundingRect().height() + verticalSpacing);

  // Cascade or Secondary

  refrigerationSecondaryItem->setPos(centerXPos() - refrigerationSecondaryItem->boundingRect().width() / 2, 
                                 refrigerationCasesItem->y() + refrigerationCasesItem->boundingRect().height() + verticalSpacing);
}

void RefrigerationSystemItem::paint( QPainter *painter, 
                                     const QStyleOptionGraphicsItem *option, 
                                     QWidget *widget )
{
  // Background and Border

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::red,2,Qt::SolidLine, Qt::RoundCap));

  painter->drawRoundedRect(10,10,boundingRect().width() - 20, boundingRect().height() - 20,8,8);
}

QRectF RefrigerationSystemItem::boundingRect() const
{
  return QRectF(0,0,800,refrigerationSecondaryItem->y() + refrigerationSecondaryItem->boundingRect().height() + verticalSpacing);
}

int RefrigerationSystemItem::leftXPos() const
{
  return boundingRect().width() * 1 / 4;
}

int RefrigerationSystemItem::centerXPos() const
{
  return boundingRect().width() * 2 / 4;
}

int RefrigerationSystemItem::rightXPos() const
{
  return boundingRect().width() * 3 / 4;
}

RefrigerationCasesItem::RefrigerationCasesItem()
  : QGraphicsObject()
{
}

void RefrigerationCasesItem::paint( QPainter *painter, 
                                    const QStyleOptionGraphicsItem *option, 
                                    QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::red,2,Qt::SolidLine, Qt::RoundCap));

  painter->drawRoundedRect(10,10,boundingRect().width() - 20, boundingRect().height() - 20,8,8);

  painter->drawText(boundingRect(),Qt::AlignCenter,"Cases");
}

QRectF RefrigerationCasesItem::boundingRect() const
{
  return QRectF(0,0,200,100);
}

void RefrigerationCondenserItem::paint( QPainter *painter, 
                                        const QStyleOptionGraphicsItem *option, 
                                        QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::red,2,Qt::SolidLine, Qt::RoundCap));

  painter->drawRoundedRect(10,10,boundingRect().width() - 20, boundingRect().height() - 20,8,8);

  painter->drawText(boundingRect(),Qt::AlignCenter,"Condenser");
}

QRectF RefrigerationCondenserItem::boundingRect() const
{
  return QRectF(0,0,200,100);
}

void RefrigerationCompressorItem::paint( QPainter *painter, 
                                         const QStyleOptionGraphicsItem *option, 
                                         QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::red,2,Qt::SolidLine, Qt::RoundCap));

  painter->drawRoundedRect(10,10,boundingRect().width() - 20, boundingRect().height() - 20,8,8);

  painter->drawText(boundingRect(),Qt::AlignCenter,"Compressor");
}

QRectF RefrigerationCompressorItem::boundingRect() const
{
  return QRectF(0,0,200,100);
}

void RefrigerationSubCoolerItem::paint( QPainter *painter, 
                                         const QStyleOptionGraphicsItem *option, 
                                         QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::red,2,Qt::SolidLine, Qt::RoundCap));

  painter->drawRoundedRect(10,10,boundingRect().width() - 20, boundingRect().height() - 20,8,8);

  painter->drawText(boundingRect(),Qt::AlignCenter,"Sub Cooler");
}

QRectF RefrigerationSubCoolerItem::boundingRect() const
{
  return QRectF(0,0,200,100);
}

void RefrigerationHeatReclaimItem::paint( QPainter *painter, 
                                         const QStyleOptionGraphicsItem *option, 
                                         QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::red,2,Qt::SolidLine, Qt::RoundCap));

  painter->drawRoundedRect(10,10,boundingRect().width() - 20, boundingRect().height() - 20,8,8);

  painter->drawText(boundingRect(),Qt::AlignCenter,"Heat Reclaim");
}

QRectF RefrigerationHeatReclaimItem::boundingRect() const
{
  return QRectF(0,0,200,100);
}

void RefrigerationSHXItem::paint( QPainter *painter, 
                                         const QStyleOptionGraphicsItem *option, 
                                         QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::red,2,Qt::SolidLine, Qt::RoundCap));

  painter->drawRoundedRect(10,10,boundingRect().width() - 20, boundingRect().height() - 20,8,8);

  painter->drawText(boundingRect(),Qt::AlignCenter,"Liquid Suction HX");
}

QRectF RefrigerationSHXItem::boundingRect() const
{
  return QRectF(0,0,200,100);
}

void RefrigerationSecondaryItem::paint( QPainter *painter, 
                                        const QStyleOptionGraphicsItem *option, 
                                        QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::red,2,Qt::SolidLine, Qt::RoundCap));

  painter->drawRoundedRect(10,10,boundingRect().width() - 20, boundingRect().height() - 20,8,8);

  painter->drawText(boundingRect(),Qt::AlignCenter,"Secondary System");
}

QRectF RefrigerationSecondaryItem::boundingRect() const
{
  return QRectF(0,0,200,100);
}

} // openstudio

