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
    m_height(0)
{
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

  terminalDropZone->setPos(x,y);

  x = x + terminalDropZone->boundingRect().width() + margin;
  zoneDropZone->setPos(x,y);

  x = x + zoneDropZone->boundingRect().width() + margin;
  y = y + zoneDropZone->boundingRect().height() + margin;

  for(std::vector<QGraphicsObject *>::iterator it = m_terminalViews.begin();
      it != m_terminalViews.end();
      ++it)
  {
    (*it)->setPos(margin,y); 
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
  bool bingo;
  double x = VRFSystemView::margin;

  terminalIconButton = new ButtonItem(m_terminalPixmap,m_terminalPixmap,m_terminalPixmap);
  terminalIconButton->setParentItem(this);
  terminalIconButton->setPos(x,(VRFSystemView::terminalViewHeight - terminalIconButton->boundingRect().height()) / 2.0);
  bingo = connect(terminalIconButton,SIGNAL(mouseClicked()),this,SLOT(onTerminalIconClicked()));
  OS_ASSERT(bingo);

  x = x + terminalIconButton->boundingRect().width() + VRFSystemView::margin;

  removeButtonItem = new RemoveButtonItem();
  removeButtonItem->setParentItem(this);
  removeButtonItem->setPos(x,terminalIconButton->y());
  bingo = connect(removeButtonItem,SIGNAL(mouseClicked()),this,SLOT(onRemoveTerminalClicked()));
  OS_ASSERT(bingo);

  x = x + removeButtonItem->boundingRect().width() + VRFSystemView::margin;

  zoneDropZone = new VRFThermalZoneDropZoneView();
  zoneDropZone->setParentItem(this);
  zoneDropZone->setPos(x,(VRFSystemView::terminalViewHeight - zoneDropZone->boundingRect().height()) / 2.0);
  bingo = connect(zoneDropZone,SIGNAL(componentDropped(const OSItemId &)),this,SLOT(onComponenDroppedOnZone(const OSItemId &)));
  OS_ASSERT(bingo);

  x = x + zoneDropZone->boundingRect().width() + VRFSystemView::margin;

  removeZoneButtonItem = new RemoveButtonItem();
  removeZoneButtonItem->setParentItem(this);
  removeZoneButtonItem->setPos(x,zoneDropZone->y());
  bingo = connect(removeZoneButtonItem,SIGNAL(mouseClicked()),this,SLOT(onRemoveZoneClicked()));
  OS_ASSERT(bingo);
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

  QRectF t_terminalPixmapRect = terminalPixmapRect();
  painter->drawPixmap(t_terminalPixmapRect.x(),
                      t_terminalPixmapRect.y(),
                      m_terminalPixmap);
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
    font.setPointSize(30);
    painter->setFont(font);
    painter->setPen(QPen(QColor(109,109,109),2,Qt::DashLine, Qt::RoundCap));
    painter->drawText(boundingRect(),Qt::AlignCenter | Qt::TextWordWrap,m_text);
  }
  else
  {
    OSDropZoneItem::paint(painter,option,widget);
  }
}

VRFSystemMiniView::VRFSystemMiniView()
{
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
  return QSize(cellWidth(),cellWidth() + headerHeight());
}

int VRFSystemMiniView::cellWidth()
{
  return 350;
}

int VRFSystemMiniView::headerHeight()
{
  return 50;
}

QRectF VRFSystemMiniView::contentRect() const
{
  return QRectF(0,headerHeight(),cellWidth(),cellWidth());
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

void VRFSystemMiniView::paint( QPainter *painter, 
                           const QStyleOptionGraphicsItem *option, 
                           QWidget *widget )
{
  // Background and Border

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);

  painter->setPen(QPen(Qt::black,1,Qt::SolidLine, Qt::RoundCap));
  painter->drawRect(boundingRect());

  painter->setPen(QPen(Qt::black,2,Qt::SolidLine, Qt::RoundCap));
  QRectF _headerRect = headerRect();
  painter->drawRect(_headerRect);

  painter->drawText(QRectF(_headerRect.x() + 5, _headerRect.y() + 5, _headerRect.width() - 10, _headerRect.height() - 10),Qt::AlignVCenter | Qt::AlignLeft,m_name);
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

