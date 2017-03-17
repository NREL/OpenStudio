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

  QRectF boundingRect() const override;
  static QSize cellSize();

  void setNumberOfZones(int zones);
  void setNumberOfTerminals(int terminals);

  public slots:

  void setName( const QString & name); 

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget ) override;

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

  QRectF boundingRect() const override;

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
              QWidget *widget = nullptr ) override;

  void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
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

  QRectF boundingRect() const override;

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
              QWidget *widget ) override;

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
              QWidget *widget ) override;

  private:

  bool m_hasZone;
};

class VRFSystemDropZoneView : public OSDropZoneItem
{
  Q_OBJECT;

  public:

  QRectF boundingRect() const override;

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget ) override;
};

} // openstudio

#endif // OPENSTUDIO_VRFGRAPHICSITEMS_HPP

