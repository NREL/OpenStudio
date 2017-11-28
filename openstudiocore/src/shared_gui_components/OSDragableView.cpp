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

