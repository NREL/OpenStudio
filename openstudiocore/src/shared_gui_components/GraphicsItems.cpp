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

#include <shared_gui_components/GraphicsItems.hpp>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QApplication>

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

    QApplication::processEvents();

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

    QApplication::processEvents();

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

} // openstudio

