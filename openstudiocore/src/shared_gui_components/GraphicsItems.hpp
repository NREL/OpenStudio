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

#ifndef OPENSTUDIO_GRAPHICSITEMS_H
#define OPENSTUDIO_GRAPHICSITEMS_H

#include <QGraphicsObject>

namespace openstudio {

// Button functionality without any visual elements
// ButtonItem already existed when AbstractButtonItem was factored out.
// Ideally ButtonItem derives from AbstractButtonItem.
class AbstractButtonItem : public QGraphicsObject
{
  Q_OBJECT;

  public:

  AbstractButtonItem(QGraphicsItem * parent = 0);

  virtual ~AbstractButtonItem() {}

  void setChecked(bool checked);
  
  signals:

  void mouseClicked(bool checked = false);

  void toggled(bool checked);

  protected:

  void mousePressEvent(QGraphicsSceneMouseEvent * event);

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

  bool m_checked;

  private:

  bool m_mouseDown;
};

class ButtonItem : public QGraphicsObject
{
  Q_OBJECT;

  public:

  ButtonItem(const QPixmap & image, const QPixmap & downImage, 
             const QPixmap & hoverImage, QGraphicsItem * parent = 0);

  virtual ~ButtonItem() {}

  QRectF boundingRect() const;

  signals:

  void mouseClicked();

  protected:

  void mousePressEvent(QGraphicsSceneMouseEvent * event);

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

  void hoverEnterEvent(QGraphicsSceneHoverEvent * event);

  void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);

  private:

  bool m_mouseDown;

  bool m_isHovering;

  QPixmap m_image;

  QPixmap m_downImage;

  QPixmap m_hoverImage;
};

class RemoveButtonItem : public ButtonItem
{
  public:

  RemoveButtonItem(QGraphicsItem * parent = 0);
};

class ZoomInButtonItem : public ButtonItem
{
  public:

  ZoomInButtonItem(QGraphicsItem * parent = 0);
};

class ZoomOutButtonItem : public ButtonItem
{
  public:

  ZoomOutButtonItem(QGraphicsItem * parent = 0);
};

} // openstudio

#endif // OPENSTUDIO_GRAPHICSITEMS_H

