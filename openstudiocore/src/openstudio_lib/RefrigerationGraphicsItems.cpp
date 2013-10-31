/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include "RefrigerationGraphicsItems.hpp"
#include <QPainter>

namespace openstudio {

const int RefrigerationSystemItem::verticalSpacing = 50;

RefrigerationSystemItem::RefrigerationSystemItem()
  : QGraphicsObject()
{
  // Condenser Item

  refrigerationCondenserItem = new RefrigerationCondenserItem();
  refrigerationCondenserItem->setParentItem(this);

  // Sub Cooler Item

  refrigerationSubCoolerItem = new RefrigerationSubCoolerItem();
  refrigerationSubCoolerItem->setParentItem(this);

  // Heat Reclaim Item

  refrigerationHeatReclaimItem = new RefrigerationHeatReclaimItem();
  refrigerationHeatReclaimItem->setParentItem(this);

  // Compressor Item

  refrigerationCompressorItem = new RefrigerationCompressorItem();
  refrigerationCompressorItem->setParentItem(this);

  // Liquid Suction HX Item

  refrigerationSHXItem = new RefrigerationSHXItem();
  refrigerationSHXItem->setParentItem(this);

  // Cases Item

  refrigerationCasesItem = new RefrigerationCasesItem();
  refrigerationCasesItem->setParentItem(this);

  // Secondary Item

  refrigerationSecondaryItem = new RefrigerationSecondaryItem();
  refrigerationSecondaryItem->setParentItem(this);

  adjustLayout();
}

void RefrigerationSystemItem::adjustLayout()
{
  prepareGeometryChange();

  // Condenser

  refrigerationCondenserItem->setPos(centerXPos() - refrigerationCondenserItem->boundingRect().width() / 2, 
                                     verticalSpacing);

  // Mechanical Sub Cooler

  refrigerationSubCoolerItem->setPos(leftXPos() - refrigerationSubCoolerItem->boundingRect().width() / 2, 
                                     refrigerationCondenserItem->y() + refrigerationCondenserItem->boundingRect().height() + verticalSpacing);

  // Heat Reclaim

  refrigerationHeatReclaimItem->setPos(rightXPos() - refrigerationHeatReclaimItem->boundingRect().width() / 2, 
                                       refrigerationCondenserItem->y() + refrigerationCondenserItem->boundingRect().height() + verticalSpacing);
  
  // Compressor

  refrigerationCompressorItem->setPos(rightXPos() - refrigerationCompressorItem->boundingRect().width() / 2, 
                                      refrigerationHeatReclaimItem->y() + refrigerationHeatReclaimItem->boundingRect().height() + verticalSpacing / 2);

  // SHX

  refrigerationSHXItem->setPos(leftXPos() - refrigerationSHXItem->boundingRect().width() / 2, 
                               refrigerationSubCoolerItem->y() + refrigerationSubCoolerItem->boundingRect().height() + verticalSpacing);

  // Cases

  refrigerationCasesItem->setPos(centerXPos() - refrigerationCasesItem->boundingRect().width() / 2, 
                                 refrigerationSHXItem->y() + refrigerationSHXItem->boundingRect().height() + verticalSpacing);

  // Cascade or Secondary

  refrigerationSecondaryItem->setPos(centerXPos() - refrigerationSecondaryItem->boundingRect().width() / 2, 
                                 refrigerationCasesItem->y() + refrigerationCasesItem->boundingRect().height() + verticalSpacing);
}

void RefrigerationSystemItem::paint( QPainter *painter, 
                                     const QStyleOptionGraphicsItem *option, 
                                     QWidget *widget )
{
  // Background and Border

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::red,2,Qt::SolidLine, Qt::RoundCap));

  painter->drawRoundedRect(10,10,boundingRect().width() - 20, boundingRect().height() - 20,8,8);
}

QRectF RefrigerationSystemItem::boundingRect() const
{
  return QRectF(0,0,800,refrigerationSecondaryItem->y() + refrigerationSecondaryItem->boundingRect().height() + verticalSpacing);
}

int RefrigerationSystemItem::leftXPos() const
{
  return boundingRect().width() * 1 / 4;
}

int RefrigerationSystemItem::centerXPos() const
{
  return boundingRect().width() * 2 / 4;
}

int RefrigerationSystemItem::rightXPos() const
{
  return boundingRect().width() * 3 / 4;
}

RefrigerationCasesItem::RefrigerationCasesItem()
  : QGraphicsObject()
{
}

void RefrigerationCasesItem::paint( QPainter *painter, 
                                    const QStyleOptionGraphicsItem *option, 
                                    QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::red,2,Qt::SolidLine, Qt::RoundCap));

  painter->drawRoundedRect(10,10,boundingRect().width() - 20, boundingRect().height() - 20,8,8);

  painter->drawText(boundingRect(),Qt::AlignCenter,"Cases");
}

QRectF RefrigerationCasesItem::boundingRect() const
{
  return QRectF(0,0,200,100);
}

void RefrigerationCondenserItem::paint( QPainter *painter, 
                                        const QStyleOptionGraphicsItem *option, 
                                        QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::red,2,Qt::SolidLine, Qt::RoundCap));

  painter->drawRoundedRect(10,10,boundingRect().width() - 20, boundingRect().height() - 20,8,8);

  painter->drawText(boundingRect(),Qt::AlignCenter,"Condenser");
}

QRectF RefrigerationCondenserItem::boundingRect() const
{
  return QRectF(0,0,200,100);
}

void RefrigerationCompressorItem::paint( QPainter *painter, 
                                         const QStyleOptionGraphicsItem *option, 
                                         QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::red,2,Qt::SolidLine, Qt::RoundCap));

  painter->drawRoundedRect(10,10,boundingRect().width() - 20, boundingRect().height() - 20,8,8);

  painter->drawText(boundingRect(),Qt::AlignCenter,"Compressor");
}

QRectF RefrigerationCompressorItem::boundingRect() const
{
  return QRectF(0,0,200,100);
}

void RefrigerationSubCoolerItem::paint( QPainter *painter, 
                                         const QStyleOptionGraphicsItem *option, 
                                         QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::red,2,Qt::SolidLine, Qt::RoundCap));

  painter->drawRoundedRect(10,10,boundingRect().width() - 20, boundingRect().height() - 20,8,8);

  painter->drawText(boundingRect(),Qt::AlignCenter,"Sub Cooler");
}

QRectF RefrigerationSubCoolerItem::boundingRect() const
{
  return QRectF(0,0,200,100);
}

void RefrigerationHeatReclaimItem::paint( QPainter *painter, 
                                         const QStyleOptionGraphicsItem *option, 
                                         QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::red,2,Qt::SolidLine, Qt::RoundCap));

  painter->drawRoundedRect(10,10,boundingRect().width() - 20, boundingRect().height() - 20,8,8);

  painter->drawText(boundingRect(),Qt::AlignCenter,"Heat Reclaim");
}

QRectF RefrigerationHeatReclaimItem::boundingRect() const
{
  return QRectF(0,0,200,100);
}

void RefrigerationSHXItem::paint( QPainter *painter, 
                                         const QStyleOptionGraphicsItem *option, 
                                         QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::red,2,Qt::SolidLine, Qt::RoundCap));

  painter->drawRoundedRect(10,10,boundingRect().width() - 20, boundingRect().height() - 20,8,8);

  painter->drawText(boundingRect(),Qt::AlignCenter,"Liquid Suction HX");
}

QRectF RefrigerationSHXItem::boundingRect() const
{
  return QRectF(0,0,200,100);
}

void RefrigerationSecondaryItem::paint( QPainter *painter, 
                                        const QStyleOptionGraphicsItem *option, 
                                        QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(Qt::red,2,Qt::SolidLine, Qt::RoundCap));

  painter->drawRoundedRect(10,10,boundingRect().width() - 20, boundingRect().height() - 20,8,8);

  painter->drawText(boundingRect(),Qt::AlignCenter,"Secondary System");
}

QRectF RefrigerationSecondaryItem::boundingRect() const
{
  return QRectF(0,0,200,100);
}

} // openstudio

