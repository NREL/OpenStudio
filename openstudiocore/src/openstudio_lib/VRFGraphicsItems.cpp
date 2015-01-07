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

#include "OSItem.hpp"
#include "OSDropZone.hpp"
#include "VRFGraphicsItems.hpp"
#include "../utilities/core/Assert.hpp"
#include "../shared_gui_components/Buttons.hpp"
#include <QPainter>
#include <utility>
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QLabel>

namespace openstudio {

const int VRFSystemView::margin = 20;
const int VRFSystemView::terminalDropZoneWidth = 200;
const int VRFSystemView::zoneDropZoneWidth = 300;
const int VRFSystemView::dropZoneHeight = 150;
const int VRFSystemView::terminalViewHeight = 100;

VRFView::VRFView()
  : QWidget()
{
  QVBoxLayout * mainVLayout = new QVBoxLayout();
  mainVLayout->setSpacing(0);
  mainVLayout->setContentsMargins(0,0,0,0);
  mainVLayout->setAlignment(Qt::AlignTop);
  setLayout(mainVLayout);

  header = new QWidget();
  header->setObjectName("VRFHeader");
  header->setStyleSheet("QWidget#VRFHeader { background: #C3C3C3; }");
  header->setFixedHeight(35);
  mainVLayout->addWidget(header);

  QHBoxLayout * headerLayout = new QHBoxLayout();
  headerLayout->setContentsMargins(5,5,5,5);
  headerLayout->setSpacing(0);
  header->setLayout(headerLayout);

  nameLabel = new QLabel();
  headerLayout->addWidget(nameLabel);

  zoomOutButton = new ZoomOutButton();
  headerLayout->addStretch();
  headerLayout->addWidget(zoomOutButton);

  graphicsView = new QGraphicsView();
  graphicsView->setObjectName("GrayWidget");
  mainVLayout->addWidget(graphicsView);
}

VRFSystemView::VRFSystemView()
  : m_mouseDown(false),
    m_width(0),
    m_height(0),
    m_vrfPixmap(":images/vrf_outdoor.png")
{
  vrfIconButton = new ButtonItem(m_vrfPixmap,m_vrfPixmap,m_vrfPixmap);
  vrfIconButton->setParentItem(this);
  connect(vrfIconButton, &ButtonItem::mouseClicked, this, &VRFSystemView::onVRFIconClicked);

  terminalDropZone = new OSDropZoneItem();
  terminalDropZone->setParentItem(this);
  terminalDropZone->setSize(terminalDropZoneWidth,dropZoneHeight);
  terminalDropZone->setText("Drop VRF Terminal");

  zoneDropZone = new OSDropZoneItem();
  zoneDropZone->setParentItem(this);
  zoneDropZone->setSize(zoneDropZoneWidth,dropZoneHeight);
  zoneDropZone->setText("Drop Thermal Zone");

  adjustLayout();
}

void VRFSystemView::adjustLayout()
{
  prepareGeometryChange();

  double x = margin;
  double y = margin;

  vrfIconButton->setPos(x,y + ((VRFSystemView::dropZoneHeight - vrfIconButton->boundingRect().height()) / 2.0));
  x = x + vrfIconButton->boundingRect().width() + margin;

  terminalDropZone->setPos(x,y);
  double terminalX = x;

  x = x + terminalDropZone->boundingRect().width() + margin;
  zoneDropZone->setPos(x,y);

  x = x + zoneDropZone->boundingRect().width() + margin;
  y = y + zoneDropZone->boundingRect().height() + margin;

  for(std::vector<QGraphicsObject *>::iterator it = m_terminalViews.begin();
      it != m_terminalViews.end();
      ++it)
  {
    (*it)->setPos(terminalX,y); 
    y = y + (*it)->boundingRect().height() + margin;
  }

  m_width = x;
  m_height = y;

  if( QGraphicsScene * _scene = scene() )
  {
    _scene->setSceneRect(boundingRect());
  }
}

void VRFSystemView::setId(const OSItemId & id)
{
  m_id = id;
}

QRectF VRFSystemView::boundingRect() const
{
  return QRectF(0,0,m_width,m_height);
}

void VRFSystemView::onVRFIconClicked()
{
  emit inspectClicked(m_id);
}

void VRFSystemView::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  m_mouseDown = true;
  update();
  event->accept();
}

void VRFSystemView::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
  if( m_mouseDown )
  {
    m_mouseDown = false;
    this->update();
    QApplication::processEvents();

    if( shape().contains(event->pos()) )
    {
      event->accept();
      update();
      emit inspectClicked(m_id);
    }
  }
}

void VRFSystemView::paint( QPainter *painter, 
                           const QStyleOptionGraphicsItem *option, 
                           QWidget *widget )
{
  // Background and Border

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(QColor(206,206,206));
  painter->setPen(Qt::NoPen);

  QRectF _boudingRect = boundingRect();
  painter->drawRoundedRect(_boudingRect.x(),_boudingRect.y() + 5,_boudingRect.width(),_boudingRect.height() - 10,5,5);

  if( ! m_terminalViews.empty() )
  {
    painter->setPen(QPen(Qt::black,1,Qt::SolidLine, Qt::RoundCap));
    double line1X = vrfIconButton->x() + vrfIconButton->boundingRect().width() / 2.0;
    double line1Y1 = vrfIconButton->y() + vrfIconButton->boundingRect().height() / 2.0;
    QGraphicsObject * lastView = m_terminalViews.back();
    double line1Y2 = lastView->y() + lastView->boundingRect().height() / 2.0;
    painter->drawLine(line1X,line1Y1,
                      line1X,line1Y2);

    for(std::vector<QGraphicsObject *>::iterator it = m_terminalViews.begin();
        it != m_terminalViews.end();
        ++it)
    {
      double line2Y1 = (*it)->y() + (*it)->boundingRect().height() / 2.0;
      double line2X2 = (*it)->x();
      painter->drawLine(line1X,line2Y1,
                        line2X2,line2Y1);
    }
  }
}

void VRFSystemView::addVRFTerminalView(VRFTerminalView * view)
{
  m_terminalViews.push_back(view);
  view->setParentItem(this);
  adjustLayout();
}

void VRFSystemView::removeAllVRFTerminalViews()
{
  prepareGeometryChange();

  for( std::vector<QGraphicsObject *>::iterator it = m_terminalViews.begin();
       it != m_terminalViews.end(); )
  {
    delete * it;
    it = m_terminalViews.erase(it);
  }

  adjustLayout();
}

VRFTerminalView::VRFTerminalView()
  : m_terminalPixmap(QPixmap(":images/vrf_unit.png"))
{
  double x = VRFSystemView::margin;

  removeButtonItem = new RemoveButtonItem();
  removeButtonItem->setParentItem(this);
  removeButtonItem->setPos(x,(VRFSystemView::terminalViewHeight - removeButtonItem->boundingRect().height()) / 2.0);
  connect(removeButtonItem, &RemoveButtonItem::mouseClicked, this, &VRFTerminalView::onRemoveTerminalClicked);

  x = x + removeButtonItem->boundingRect().width() + VRFSystemView::margin;

  terminalIconButton = new ButtonItem(m_terminalPixmap,m_terminalPixmap,m_terminalPixmap);
  terminalIconButton->setParentItem(this);
  terminalIconButton->setPos(x,(VRFSystemView::terminalViewHeight - terminalIconButton->boundingRect().height()) / 2.0);
  connect(terminalIconButton, &ButtonItem::mouseClicked, this, &VRFTerminalView::onTerminalIconClicked);

  x = x + terminalIconButton->boundingRect().width() + VRFSystemView::margin;

  zoneDropZone = new VRFThermalZoneDropZoneView();
  zoneDropZone->setParentItem(this);
  zoneDropZone->setPos(x,(VRFSystemView::terminalViewHeight - zoneDropZone->boundingRect().height()) / 2.0);
  connect(zoneDropZone, &OSDropZoneItem::componentDropped, this, &VRFTerminalView::onComponenDroppedOnZone);

  x = x + zoneDropZone->boundingRect().width() + VRFSystemView::margin;

  removeZoneButtonItem = new RemoveButtonItem();
  removeZoneButtonItem->setParentItem(this);
  removeZoneButtonItem->setPos(zoneDropZone->x() + zoneDropZone->boundingRect().width() - 10,zoneDropZone->y() - 10);
  removeZoneButtonItem->setVisible(false);
  connect(removeZoneButtonItem, &RemoveButtonItem::mouseClicked, this, &VRFTerminalView::onRemoveZoneClicked);
}

void VRFTerminalView::setId(const OSItemId & id)
{
  m_id = id;
}

void VRFTerminalView::onComponenDroppedOnZone(const OSItemId & dropComponentID)
{
  emit componentDroppedOnZone(m_id,dropComponentID);
}

void VRFTerminalView::onTerminalIconClicked()
{
  emit terminalIconClicked(m_id);
}

void VRFTerminalView::onRemoveZoneClicked()
{
  emit removeZoneClicked(m_id);
}

void VRFTerminalView::onRemoveTerminalClicked()
{
  emit removeTerminalClicked(m_id);
}

QRectF VRFTerminalView::boundingRect() const
{
  return QRectF(0,0,
                VRFSystemView::terminalDropZoneWidth + VRFSystemView::margin + VRFSystemView::zoneDropZoneWidth,
                VRFSystemView::terminalViewHeight);
}

QRectF VRFTerminalView::terminalPixmapRect() const
{
  return QRectF(VRFSystemView::margin,(boundingRect().height() - m_terminalPixmap.height()) / 2.0,
                m_terminalPixmap.width(),m_terminalPixmap.height());
}

void VRFTerminalView::paint( QPainter *painter, 
                           const QStyleOptionGraphicsItem *option, 
                           QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);

  painter->setPen(QPen(Qt::black,1,Qt::SolidLine, Qt::RoundCap));
  painter->drawRect(boundingRect());
}

VRFThermalZoneDropZoneView::VRFThermalZoneDropZoneView()
  : m_hasZone(false)
{
  setSize(300,50);
  setText("Drop Thermal Zone");
}

void VRFThermalZoneDropZoneView::setHasZone(bool hasZone)
{
  m_hasZone = hasZone;
  update();
}

void VRFThermalZoneDropZoneView::paint( QPainter *painter, 
                                        const QStyleOptionGraphicsItem *option, 
                                        QWidget *widget )
{
  if( m_hasZone )
  {
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QColor(109,109,109),2,Qt::SolidLine, Qt::RoundCap));

    painter->drawRect(boundingRect());

    QFont font = painter->font();
    font.setPointSize(25);
    painter->setFont(font);
    painter->setPen(QPen(QColor(109,109,109),2,Qt::DashLine, Qt::RoundCap));
    QRectF t_rec = boundingRect();
    QFontMetrics m(font);
    QRectF t_textRec = m.boundingRect(m_text);
    if( t_textRec.width() > (t_rec.width() - 10) )
    {
      painter->drawText(QRectF(5,5,t_rec.width() - 10,t_rec.height() - 10),Qt::AlignLeft | Qt::AlignVCenter,m_text);
    }
    else
    {
      painter->drawText(QRectF(5,5,t_rec.width() - 10,t_rec.height() - 10),Qt::AlignCenter,m_text);
    }
  }
  else
  {
    OSDropZoneItem::paint(painter,option,widget);
  }
}

VRFSystemMiniView::VRFSystemMiniView()
{
  m_length = 75;
  m_zones = 0;
  m_terminals = 0;

  m_vrfOutdoorPix = QPixmap(":images/vrf_outdoor.png").scaled(m_length,m_length);
  m_vrfTransferPix = QPixmap(":images/vrf_transfer.png").scaled(m_length,m_length);
  m_vrfTerminalPix = QPixmap(":images/vrf_unit.png").scaled(m_length,m_length);
  m_vrfZonePix = QPixmap(":images/zone.png").scaled(m_length,m_length);

  removeButtonItem = new RemoveButtonItem();
  removeButtonItem->setParentItem(this);
  removeButtonItem->setPos(cellWidth() - removeButtonItem->boundingRect().width() - 10,headerHeight() / 2.0 - removeButtonItem->boundingRect().height() / 2.0);

  zoomInButtonItem = new ZoomInButtonItem();
  zoomInButtonItem->setParentItem(this);
  zoomInButtonItem->setPos(removeButtonItem->pos().x() - 10 - zoomInButtonItem->boundingRect().width(),headerHeight() / 2.0 - zoomInButtonItem->boundingRect().height() / 2.0);
}

QRectF VRFSystemMiniView::boundingRect() const
{
  return QRectF(QPoint(0,0),cellSize());
}

QSize VRFSystemMiniView::cellSize()
{
  return QSize(cellWidth(),cellHeight() + headerHeight());
}

int VRFSystemMiniView::cellWidth()
{
  return 450;
}

int VRFSystemMiniView::cellHeight()
{
  return 100;
}

int VRFSystemMiniView::headerHeight()
{
  return 50;
}

QRectF VRFSystemMiniView::contentRect() const
{
  return QRectF(0,headerHeight(),cellWidth(),cellHeight());
}

QRectF VRFSystemMiniView::headerRect() const
{
  return QRectF(0,0,cellWidth(),headerHeight());
}

void VRFSystemMiniView::setName(const QString & name)
{
  m_name = name;
  update();
}

void VRFSystemMiniView::setNumberOfZones(int zones)
{
  m_zones = zones;
}

void VRFSystemMiniView::setNumberOfTerminals(int terminals)
{
  m_terminals = terminals;
}

void VRFSystemMiniView::paint( QPainter *painter, 
                           const QStyleOptionGraphicsItem *option, 
                           QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);

  painter->setPen(QPen(Qt::black,1,Qt::SolidLine, Qt::RoundCap));
  painter->drawRect(boundingRect());

  painter->setPen(QPen(Qt::black,2,Qt::SolidLine, Qt::RoundCap));

  QRectF _headerRect = headerRect();
  painter->drawRect(_headerRect);
  painter->drawText(QRectF(_headerRect.x() + 5, _headerRect.y() + 5, _headerRect.width() - 10, _headerRect.height() - 10),Qt::AlignVCenter | Qt::AlignLeft,m_name);

  QRectF _contentRect = contentRect();
  double x = _contentRect.x() + 5;
  double y = _contentRect.y() + 12;

  painter->drawPixmap(x,y,m_vrfOutdoorPix);
  x = x + m_vrfOutdoorPix.width() + 2;

  painter->drawPixmap(x,y,m_vrfTransferPix);
  x = x + m_vrfTerminalPix.width() + 2;

  painter->drawPixmap(x,y,m_vrfTerminalPix);
  x = x + m_vrfTerminalPix.width() + 2;

  QFont font = painter->font();

  font.setPointSize(24);
  font.setWeight(QFont::Bold);
  painter->setFont(font);
  painter->drawText(QRectF(x,y,m_length,m_length / 2.0),Qt::AlignCenter,QString::number(m_terminals));
  font.setPointSize(12);
  font.setWeight(QFont::Normal);
  painter->setFont(font);
  painter->drawText(QRectF(x,y + m_length / 2.0,m_length,m_length / 2.0),Qt::AlignCenter,QString("Terminals"));
  x = x + m_length;

  painter->drawPixmap(x,y,m_vrfZonePix); 
  x = x + m_vrfZonePix.width();

  font.setPointSize(24);
  font.setWeight(QFont::Bold);
  painter->setFont(font);
  painter->drawText(QRectF(x,y,m_length,m_length / 2.0),Qt::AlignCenter,QString::number(m_zones));
  font.setPointSize(12);
  font.setWeight(QFont::Normal);
  painter->setFont(font);
  painter->drawText(QRectF(x,y + m_length / 2.0,m_length,m_length / 2.0),Qt::AlignCenter,QString("Zones"));
  x = x + m_length;
}

QRectF VRFSystemDropZoneView::boundingRect() const
{
  return QRectF(QPoint(0,0),VRFSystemMiniView::cellSize());
}

void VRFSystemDropZoneView::paint( QPainter *painter, 
                                                 const QStyleOptionGraphicsItem *option, 
                                                 QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::black,2,Qt::DashLine, Qt::RoundCap));

  painter->drawRect(boundingRect());

  QFont font = painter->font();
  font.setPointSize(30);
  painter->setFont(font);
  painter->setPen(QPen(QColor(109,109,109),2,Qt::DashLine, Qt::RoundCap));
  painter->drawText(boundingRect(),Qt::AlignCenter | Qt::TextWordWrap,"Drop VRF System");
}

} // openstudio

