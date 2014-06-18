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

#include "BusyWidget.hpp"

#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QSize>

/********************************************************************************/
// TODO this class exists in PAT.  Once PAT is in the OpenStudio project,
// the PAT implementation of BusyWidget should be used.
// Note that QPixmap pixmap = QPixmap(":/shared_gui_components/images/rotating_arrow.png").scaled(25,25)
// should not scale-down the image for the OS App application
/********************************************************************************/

#include <QStyleOption>

namespace openstudio {

BusyWidget::BusyWidget(QWidget * parent)
  : QWidget(parent)
{
  //QPixmap pixmap = QPixmap(":/shared_gui_components/images/rotating_arrow.png").scaled(25,25);
  QPixmap pixmap = QPixmap(":/shared_gui_components/images/rotating_arrow.png");

  m_pixmap = std::shared_ptr<QPixmap>(new QPixmap(pixmap));

  m_rotation = 0.0;

  setMinimumSize(m_pixmap->width(),m_pixmap->height());
}

void BusyWidget::rotate()
{
  m_rotation = m_rotation + 5.0;

  update();
}

void BusyWidget::paintEvent ( QPaintEvent * event )
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
  painter.setRenderHint(QPainter::Antialiasing,true);

  int transX = m_pixmap->width() / 2;
  int transY = m_pixmap->height() / 2;

  painter.translate(transX,transY);

  painter.rotate(m_rotation);

  painter.drawPixmap(-transX,-transY,*m_pixmap);
}

const QSize BusyWidget::sizeHint()
{
  return QSize(m_pixmap->width(),m_pixmap->height());
}

} // namespace openstudio
