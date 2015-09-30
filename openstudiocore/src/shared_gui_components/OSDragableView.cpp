/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.  
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

#include "OSDragableView.hpp"
#include <QMouseEvent>
#include <QApplication>

namespace openstudio {

OSDragPixmapData::OSDragPixmapData(const QPixmap & t_pixmap, const QPoint & t_hotSpot)
  : pixmap(t_pixmap),
    hotSpot(t_hotSpot)
{
}

OSDragableView::OSDragableView(QWidget * parent)
  : QWidget(parent),
    m_mouseDown(false)
{
}

void OSDragableView::mouseReleaseEvent(QMouseEvent* event)
{
  if(event->button() == Qt::LeftButton)
  {
    if( m_mouseDown )
    { 
      emit clicked();
      m_mouseDown = false;
    }
  }
}

void OSDragableView::mousePressEvent(QMouseEvent * event)
{
  if(event->button() == Qt::LeftButton)
  {
    m_mouseDown = true;
    m_dragStartPosition = event->pos();
  }
}

void OSDragableView::mouseMoveEvent(QMouseEvent * event)
{
  if(!m_mouseDown)
  {
    return;
  }

  if(!(event->buttons() && Qt::LeftButton))
  {
    return;
  }

  if((event->pos() - m_dragStartPosition).manhattanLength() < QApplication::startDragDistance())
  {
    return;
  }

  m_mouseDown = false;
  
  QPixmap pixmap(size());
  pixmap.fill(Qt::transparent);

  render(&pixmap,QPoint(),QRegion(),QWidget::RenderFlags(QWidget::DrawChildren));

  OSDragPixmapData dragPixmapData(pixmap,event->pos());

  emit dragRequested(dragPixmapData);
}

void OSDragableView::leaveEvent(QEvent* event)
{
  m_mouseDown = false;
}

} // openstudio

