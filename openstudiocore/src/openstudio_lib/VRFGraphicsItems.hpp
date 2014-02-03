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

#ifndef OPENSTUDIO_VRFGRAPHICSITEMS_H
#define OPENSTUDIO_VRFGRAPHICSITEMS_H

#include <QGraphicsObject>
#include "OSItem.hpp"
#include "../shared_gui_components/OSListController.hpp"
#include "../shared_gui_components/OSListView.hpp"
#include "../shared_gui_components/GraphicsItems.hpp"

class QGraphicsView;
class QPushButton;
class QLabel;

namespace openstudio {

class VRFSystemView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  VRFSystemView();

  virtual ~VRFSystemView() {}

  RemoveButtonItem * removeButtonItem;

  QRectF boundingRect() const;

  static QSize cellSize();

  public slots:

  void setName( const QString & name); 

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget );

  private:

  QRectF contentRect() const;

  QRectF headerRect() const;

  static int cellWidth();

  static int headerHeight();

  QString m_name;
};

class VRFSystemDropZoneView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  VRFSystemDropZoneView();

  virtual ~VRFSystemDropZoneView() {}

  QRectF boundingRect() const;

  signals:

  void mouseClicked();

  void componentDropped(const OSItemId & itemid);

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget );

  void mousePressEvent(QGraphicsSceneMouseEvent * event);

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

  void dropEvent(QGraphicsSceneDragDropEvent *event);

  private:

  bool m_mouseDown;
};

} // openstudio

#endif // OPENSTUDIO_VRFGRAPHICSITEMS_H

