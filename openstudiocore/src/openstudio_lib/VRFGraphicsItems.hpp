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

#ifndef OPENSTUDIO_VRFGRAPHICSITEMS_HPP
#define OPENSTUDIO_VRFGRAPHICSITEMS_HPP

#include <QGraphicsObject>
#include "OSItem.hpp"
#include "OSDropZone.hpp"
#include "../shared_gui_components/OSListController.hpp"
#include "../shared_gui_components/OSListView.hpp"
#include "../shared_gui_components/GraphicsItems.hpp"

class QGraphicsView;
class QPushButton;
class QLabel;

namespace openstudio {

class VRFTerminalView;
class RemoveButtonItem;
class VRFThermalZoneDropZoneView;

class VRFView : public QWidget
{
  Q_OBJECT;

  public:

  VRFView();

  virtual ~VRFView() {}

  QWidget * header;

  QGraphicsView * graphicsView;

  QPushButton * zoomOutButton;

  QLabel * nameLabel;
};

class VRFSystemMiniView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  VRFSystemMiniView();

  virtual ~VRFSystemMiniView() {}

  RemoveButtonItem * removeButtonItem;
  ZoomInButtonItem * zoomInButtonItem;

  QRectF boundingRect() const;
  static QSize cellSize();

  void setNumberOfZones(int zones);
  void setNumberOfTerminals(int terminals);

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

  static int cellHeight();

  static int headerHeight();

  QString m_name;

  QPixmap m_vrfOutdoorPix;
  QPixmap m_vrfTransferPix;
  QPixmap m_vrfTerminalPix;
  QPixmap m_vrfZonePix;

  double m_length;

  int m_zones;
  int m_terminals;
};

class VRFSystemView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  VRFSystemView();

  virtual ~VRFSystemView() {}

  QRectF boundingRect() const;

  void setId(const OSItemId & id);

  OSDropZoneItem * terminalDropZone;

  OSDropZoneItem * zoneDropZone;

  ButtonItem * vrfIconButton;

  void adjustLayout();

  static const int margin;
  static const int terminalDropZoneWidth;
  static const int zoneDropZoneWidth;
  static const int dropZoneHeight;
  static const int terminalViewHeight;

  void addVRFTerminalView(VRFTerminalView * view);
  void removeAllVRFTerminalViews();

  signals:

  void inspectClicked(const OSItemId & id);

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );

  void mousePressEvent(QGraphicsSceneMouseEvent * event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
  bool m_mouseDown;

  private slots:

  void onVRFIconClicked();

  private:

  double m_width;
  double m_height;

  std::vector<QGraphicsObject *> m_terminalViews;

  OSItemId m_id;
  QPixmap m_vrfPixmap;
};

class VRFTerminalView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  VRFTerminalView();

  virtual ~VRFTerminalView() {}

  QRectF boundingRect() const;

  VRFThermalZoneDropZoneView * zoneDropZone;

  ButtonItem * terminalIconButton;

  RemoveButtonItem * removeButtonItem;

  RemoveButtonItem * removeZoneButtonItem;

  VRFThermalZoneDropZoneView * vrfThermalZoneDropZoneView;

  void setId(const OSItemId & id);

  signals:

  void componentDroppedOnZone(const OSItemId & zoneHVACTerminalID, const OSItemId & dropComponentID); 

  void removeZoneClicked(const OSItemId & zoneHVACTerminalID); 

  void terminalIconClicked(const OSItemId & zoneHVACTerminalID); 

  void removeTerminalClicked(const OSItemId & zoneHVACTerminalID); 

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget );

  private slots:

  void onComponenDroppedOnZone(const OSItemId & dropComponentID);

  void onRemoveZoneClicked();

  void onRemoveTerminalClicked();

  void onTerminalIconClicked();

  private:

  QRectF terminalPixmapRect() const;

  QPixmap m_terminalPixmap;

  OSItemId m_id;
};

class VRFThermalZoneDropZoneView : public OSDropZoneItem
{
  Q_OBJECT;

  public:

  VRFThermalZoneDropZoneView();

  ~VRFThermalZoneDropZoneView() {}

  void setHasZone(bool hasZone);

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget );

  private:

  bool m_hasZone;
};

class VRFSystemDropZoneView : public OSDropZoneItem
{
  Q_OBJECT;

  public:

  QRectF boundingRect() const;

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget );
};

} // openstudio

#endif // OPENSTUDIO_VRFGRAPHICSITEMS_HPP

