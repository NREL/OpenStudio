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

#include "../shared_gui_components/GraphicsItems.hpp"
#include "../shared_gui_components/OSListController.hpp"
#include "../utilities/core/Assert.hpp"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QApplication>
#include <QGraphicsScene>

namespace openstudio {

AbstractButtonItem::AbstractButtonItem(QGraphicsItem * parent)
  : QGraphicsObject(parent),
    m_checked(false),
    m_mouseDown(false)
{
}

void AbstractButtonItem::setChecked(bool checked)
{
  m_checked = checked;

  update();

  emit toggled(m_checked);
}

void AbstractButtonItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  m_mouseDown = true;

  update();

  event->accept();
}

void AbstractButtonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
  if( m_mouseDown )
  {
    m_mouseDown = false;

    this->update();

    if( shape().contains(event->pos()) )
    {
      event->accept();

      m_checked = ! m_checked;

      update();

      emit mouseClicked(m_checked);

      emit toggled(m_checked);
    }
  }
}

ButtonItem::ButtonItem(const QPixmap & image, const QPixmap & downImage, 
                       const QPixmap & hoverImage, QGraphicsItem * parent)
  : QGraphicsObject(parent),
    m_mouseDown(false),
    m_isHovering(false),
    m_image(image),
    m_downImage(downImage),
    m_hoverImage(hoverImage)
{
  setAcceptHoverEvents(true);
}

QRectF ButtonItem::boundingRect() const
{
  return m_image.rect();
}

void ButtonItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  m_mouseDown = true;

  this->update();

  event->accept();
}

void ButtonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
  if( m_mouseDown )
  {
    m_mouseDown = false;

    this->update();

    if( shape().contains(event->pos()) )
    {
      event->accept();

      emit mouseClicked();
    }
  }
}

void ButtonItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
  m_isHovering = true;

  this->update();

  event->accept();
}

void ButtonItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
  m_isHovering = false;

  this->update();

  event->accept();
}

void ButtonItem::paint(QPainter *painter, 
           const QStyleOptionGraphicsItem *option, 
           QWidget *widget)
{
  if( m_mouseDown )
  {
    painter->drawPixmap(m_downImage.rect(),m_downImage);
  }
  else if( m_isHovering )
  {
    painter->drawPixmap(m_hoverImage.rect(),m_hoverImage);
  }
  else
  {
    painter->drawPixmap(m_image.rect(),m_image);
  }
}

RemoveButtonItem::RemoveButtonItem(QGraphicsItem * parent)
  : ButtonItem(QPixmap(":/images/delete.png"),
               QPixmap(":/images/delete.png"),
               QPixmap(":/images/delete.png"),
               parent)
{
}

ZoomInButtonItem::ZoomInButtonItem(QGraphicsItem * parent)
  : ButtonItem(QPixmap(":/images/zoom-in.png"),
               QPixmap(":/images/zoom-in-press.png"),
               QPixmap(":/images/zoom-in-over.png"),
               parent)
{
}

ZoomOutButtonItem::ZoomOutButtonItem(QGraphicsItem * parent)
  : ButtonItem(QPixmap(":/images/zoom-out.png"),
               QPixmap(":/images/zoom-out-press.png"),
               QPixmap(":/images/zoom-out-over.png"),
               parent)
{
}

GridLayoutItem::GridLayoutItem()
  : m_cellSize(100,100),
    m_margin(10)
{
}

GridLayoutItem::~GridLayoutItem()
{
  QList<QGraphicsItem *> itemList = childItems();
  for( QList<QGraphicsItem *>::iterator it = itemList.begin(); 
       it < itemList.end(); 
       ++it )
  {
    delete *it;
  }
}

QSizeF GridLayoutItem::cellSize() const
{
  return m_cellSize;
}

void GridLayoutItem::setCellSize(const QSizeF & size)
{
  m_cellSize = size;
}

int GridLayoutItem::margin() const
{
  return m_margin;
}

void GridLayoutItem::setMargin(int margin)
{
  m_margin = margin;
}

QRectF GridLayoutItem::boundingRect() const
{
  int x = columns() * (cellSize().width() + spacing()) - spacing() + margin() * 2.0;
  int y = rows() * (cellSize().height() + spacing()) - spacing() + margin() * 2.0;

  return QRectF(0,0,x,y);
}

void GridLayoutItem::setDelegate(QSharedPointer<OSGraphicsItemDelegate> delegate)
{
  m_delegate = delegate;

  refreshAllItemViews();
}

void GridLayoutItem::setListController(QSharedPointer<OSListController> listController)
{
  if( m_listController )
  {
    m_listController->disconnect(this);
  }

  m_listController = listController;

  connect(m_listController.data(), &OSListController::itemInserted, this, &GridLayoutItem::insertItemView);
  connect(m_listController.data(), &OSListController::itemRemoved, this, &GridLayoutItem::removeItemView);
  connect(m_listController.data(), &OSListController::itemChanged, this, &GridLayoutItem::refreshItemView);
  connect(m_listController.data(), &OSListController::modelReset, this, &GridLayoutItem::refreshAllItemViews);
  refreshAllItemViews();
}

QSharedPointer<OSListController> GridLayoutItem::listController() const
{
  return m_listController;
}

void GridLayoutItem::refreshAllItemViews()
{
  prepareGeometryChange();

  QList<QGraphicsItem *> itemList = childItems();
  for( QList<QGraphicsItem *>::iterator it = itemList.begin(); 
       it < itemList.end(); 
       ++it )
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

QGraphicsObject * GridLayoutItem::createNewItemView(int i)
{
  if( m_listController && m_delegate )
  {
    QSharedPointer<OSListItem> itemData = m_listController->itemAt(i);

    OS_ASSERT(itemData);

    QGraphicsObject * graphicsItem = m_delegate->view(itemData);

    OS_ASSERT(graphicsItem);

    graphicsItem->setParentItem(this);

    m_widgetItemPairs.insert( std::make_pair(graphicsItem,itemData) );

    connect(graphicsItem, &QGraphicsObject::destroyed, this, &GridLayoutItem::removePair);

    return graphicsItem;
  }
  else
  {
    return nullptr;
  }
}

void GridLayoutItem::setItemViewGridPos(QGraphicsObject * item,std::pair<int,int> gridPos)
{
  if( item )
  {
    int x = gridPos.second * (cellSize().width() + spacing()) + margin();
    int y = gridPos.first * (cellSize().height() + spacing()) + margin();

    item->setPos(x,y);

    m_gridPosItemViewPairs[gridPos] = item;

    m_itemViewGridPosPairs[item] = gridPos;
  }
}

void GridLayoutItem::insertItemView(int index)
{
  if( m_listController )
  {
    prepareGeometryChange();

    // Move existing views forward one grid position
    for( int i = (m_listController->count() - 2); i >= index; i-- )
    {
      std::pair<int,int> oldPos = gridPos(i);

      QGraphicsObject * item = viewFromGridPos(oldPos);

      std::pair<int,int> newPos = gridPos(i + 1);

      setItemViewGridPos(item,newPos);
    }

    // Create new item and position it at index grid position
    if( QGraphicsObject * item = createNewItemView(index) )
    {
      std::pair<int,int> pos = gridPos(index);

      setItemViewGridPos(item,pos);
    }

    if( QGraphicsScene * _scene = scene() )
    {
      _scene->setSceneRect(boundingRect());
    }
  }
}

void GridLayoutItem::removeItemView(int index)
{
  if( m_listController )
  {
    // Remove Item
    if(QGraphicsObject * itemViewToRemove = viewFromGridPos(gridPos(index)))
    {
      prepareGeometryChange();

      removePair(itemViewToRemove);

      itemViewToRemove->deleteLater();

      // Move Everything after index back one grid position
      for( int i = index + 1; i < m_listController->count() + 1; i++ )
      {
        std::pair<int,int> oldPos = gridPos(i);

        QGraphicsObject * item = viewFromGridPos(oldPos);

        OS_ASSERT(item);

        std::pair<int,int> newPos = gridPos(i - 1);

        setItemViewGridPos(item,newPos);
      }
    }

    if( QGraphicsScene * _scene = scene() )
    {
      _scene->setSceneRect(boundingRect());
    }
  }
}

void GridLayoutItem::removePair(QObject * object)
{
  if( object )
  {
    auto it = m_widgetItemPairs.find(object);

    if( it != m_widgetItemPairs.end() )
    {
      m_widgetItemPairs.erase(it);
    }

    auto it2 = m_itemViewGridPosPairs.find(object);

    std::pair<int,int> pos;

    if( it2 != m_itemViewGridPosPairs.end() )
    {
      pos = it2->second;

      m_itemViewGridPosPairs.erase(it2);

      auto it3 = m_gridPosItemViewPairs.find(pos);

      if( it3 != m_gridPosItemViewPairs.end() )
      {
        m_gridPosItemViewPairs.erase(it3);
      }
    }
  }
}

void GridLayoutItem::refreshItemView(int i)
{
}

int GridLayoutItem::spacing() const
{
  return margin();
}

int GridLayoutItem::rows() const
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

int GridLayoutItem::columns() const
{
  return 2;
}

std::pair<int,int> GridLayoutItem::gridPos(int index) 
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

QGraphicsObject * GridLayoutItem::viewFromGridPos(std::pair<int,int> location)
{
  QGraphicsObject * result = nullptr;

  auto it = m_gridPosItemViewPairs.find(location);

  if( it != m_gridPosItemViewPairs.end() )
  {
    result = qobject_cast<QGraphicsObject *>(it->second);
  }

  return result;
}

} // openstudio

