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

RefrigerationSystemItem::RefrigerationSystemItem()
  : QGraphicsObject()
{
  // Condenser Item

  RefrigerationCondenserItem * condenserItem = new RefrigerationCondenserItem();
  condenserItem->setParentItem(this);
  condenserItem->setPos(centerXPos() - condenserItem->boundingRect().width() / 2, yPos1() - condenserItem->boundingRect().height() / 2);

  // Sub Cooler Item

  RefrigerationSubCoolerItem * subCoolerItem = new RefrigerationSubCoolerItem();
  subCoolerItem->setParentItem(this);
  subCoolerItem->setPos(leftXPos() - subCoolerItem->boundingRect().width() / 2, yPos2() - subCoolerItem->boundingRect().height() / 2);

  // Heat Reclaim Item

  RefrigerationHeatReclaimItem * reclaimItem = new RefrigerationHeatReclaimItem();
  reclaimItem->setParentItem(this);
  reclaimItem->setPos(rightXPos() - reclaimItem->boundingRect().width() / 2, yPos2() - reclaimItem->boundingRect().height() / 2);

  // Compressor Item

  RefrigerationCompressorItem * compressorItem = new RefrigerationCompressorItem();
  compressorItem->setParentItem(this);
  compressorItem->setPos(rightXPos() - compressorItem->boundingRect().width() / 2, yPos3() - compressorItem->boundingRect().height() / 2);

  // Liquid Suction HX Item

  RefrigerationSHXItem * shxItem = new RefrigerationSHXItem();
  shxItem->setParentItem(this);
  shxItem->setPos(leftXPos() - shxItem->boundingRect().width() / 2, yPos4() - shxItem->boundingRect().height() / 2);

  // Cases Item

  RefrigerationCasesItem * casesItem = new RefrigerationCasesItem();
  casesItem->setParentItem(this);
  casesItem->setPos(centerXPos() - casesItem->boundingRect().width() / 2, yPos6() - casesItem->boundingRect().height() / 2);

  // Secondary Item

  RefrigerationSecondaryItem * secondaryItem = new RefrigerationSecondaryItem();
  secondaryItem->setParentItem(this);
  secondaryItem->setPos(centerXPos() - secondaryItem->boundingRect().width() / 2, yPos7() - secondaryItem->boundingRect().height() / 2);
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
  return QRectF(0,0,800,800);
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

int RefrigerationSystemItem::yPos1() const
{
  return boundingRect().height() * 1 / 6;
}

// Mechanical Sub Cooler, Heat Reclaim
int RefrigerationSystemItem::yPos2() const
{
  return boundingRect().height() * 2 / 6;
}

// Compressor, SHX Outlet
int RefrigerationSystemItem::yPos3() const
{
  // Halfway between position 2 and 4
  return (boundingRect().height() * 3 / 6) - 50;
}

// SHX Center
int RefrigerationSystemItem::yPos4() const
{
  return boundingRect().height() * 3 / 6;
}

// SHX Inlet
int RefrigerationSystemItem::yPos5() const
{
  // Halfway between position 4 and 6
  return (boundingRect().height() * 3 / 6) + 50;
}

// Cases
int RefrigerationSystemItem::yPos6() const
{
  return boundingRect().height() * 4 / 6;
}

// Cascade or Secondary
int RefrigerationSystemItem::yPos7() const
{
  return boundingRect().height() * 5 / 6;
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

