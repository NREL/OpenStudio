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

#ifndef SHAREDGUICOMPONENTS_OSDRAGABLEVIEW_HPP
#define SHAREDGUICOMPONENTS_OSDRAGABLEVIEW_HPP

#include <QWidget>

namespace openstudio {

class OSDragPixmapData
{
  public:

  OSDragPixmapData(const QPixmap & pixmap, const QPoint & hotSpot);

  virtual ~OSDragPixmapData() {}

  QPixmap pixmap;

  QPoint hotSpot;
};

class OSDragableView : public QWidget
{
  Q_OBJECT

  public:

  OSDragableView(QWidget * parent = nullptr);

  virtual ~OSDragableView() {}

  signals:

  void dragRequested(const OSDragPixmapData & dragPixmapData);

  void clicked();

  protected:

  void mouseReleaseEvent(QMouseEvent* event);

  void mousePressEvent(QMouseEvent* event);

  void mouseMoveEvent(QMouseEvent*event);

  void leaveEvent(QEvent* event);

  private:

  bool m_mouseDown;

  QPoint m_dragStartPosition;
};

} // openstudio

#endif // SHAREDGUICOMPONENTS_OSDRAGABLEVIEW_HPP

