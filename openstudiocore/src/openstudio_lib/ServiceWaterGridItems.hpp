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

#ifndef OPENSTUDIO_SERVICEWATERGRIDITEMS_HPP
#define OPENSTUDIO_SERVICEWATERGRIDITEMS_HPP

#include <QGraphicsObject>
#include <QCoreApplication>
#include <QPixmap>
#include "../model/ModelObject.hpp"
#include "OSItem.hpp"
#include "GridItem.hpp"

namespace openstudio {

class ServiceWaterScene;

class WaterUseConnectionsDetailScene;

class ServiceWaterItem : public GridItem
{
  public:

  ServiceWaterItem(ServiceWaterScene * serviceWaterScene);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

class WaterUseConnectionsDetailItem : public GridItem
{
  public:

  WaterUseConnectionsDetailItem(WaterUseConnectionsDetailScene * waterUseConnectionsDetailScene);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

class WaterUseConnectionsDropZoneItem : public HorizontalBranchItem
{
  public:

  WaterUseConnectionsDropZoneItem(QGraphicsItem * parent = 0);
};

class WaterUseEquipmentDropZoneItem : public HorizontalBranchItem
{
  public:

  WaterUseEquipmentDropZoneItem(QGraphicsItem * parent = 0);
};

class SewerItem : public GridItem
{
  public:

  SewerItem(QGraphicsItem * parent = 0);

  ButtonItem * sewerButton() const;

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);

  ButtonItem * m_sewerButton;
};

class MainsSupplyItem : public GridItem
{
  public:

  MainsSupplyItem(QGraphicsItem * parent = 0);

  ButtonItem * mainsSupplyButton() const;

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);

  ButtonItem * m_mainsSupplyButton;
};

class HotWaterSupplyItem : public GridItem
{
  Q_OBJECT;

  public:

  HotWaterSupplyItem(QGraphicsItem * parent = 0);

  virtual ~HotWaterSupplyItem() {}

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);

  ButtonItem * m_hotWaterSupplyButton;

  private slots:

  void onHotWaterSupplyButtonClicked();
};

class DoubleOneThreeStraightItem : public GridItem
{
  public:

  DoubleOneThreeStraightItem(QGraphicsItem * parent = 0);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

class DoubleThreeFourStraightItem : public GridItem
{
  public:

  DoubleThreeFourStraightItem(QGraphicsItem * parent = 0);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

class DoubleTwoFourStraightItem : public GridItem
{
  public:

  DoubleTwoFourStraightItem(QGraphicsItem * parent = 0);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

class DoubleTwoThreeStraightItem : public GridItem
{
  public:

  DoubleTwoThreeStraightItem(QGraphicsItem * parent = 0);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

class DoubleTwoThreeFourStraightItem : public GridItem
{
  public:

  DoubleTwoThreeFourStraightItem(QGraphicsItem * parent = 0);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

class HotWaterJunctionItem : public DoubleTwoFourStraightItem
{
  public:

  HotWaterJunctionItem(QGraphicsItem * parent = 0);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

class ColdWaterJunctionItem : public GridItem
{
  public:

  ColdWaterJunctionItem(int isElbow,QGraphicsItem * parent = 0);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);

  private:

  bool m_isElbow;
};

class WaterUseConnectionsItem : public GridItem
{
  public:

  WaterUseConnectionsItem(QGraphicsItem * parent = 0);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

class WaterUseEquipmentItem : public GridItem
{
  public:

  WaterUseEquipmentItem(QGraphicsItem * parent = 0);

  void setModelObject( model::OptionalModelObject modelObject );

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

class MakeupWaterItem : public GridItem
{
  Q_OBJECT;

  public:

  MakeupWaterItem(QGraphicsItem * parent = 0);

  virtual ~MakeupWaterItem() {}

  ButtonItem * mainsSupplyButton() const;

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);

  private slots:

  void onHotWaterSupplyButtonClicked();

  private:

  ButtonItem * m_mainsSupplyButton;

  ButtonItem * m_hotWaterSupplyButton;
};

} // openstudio

#endif // OPENSTUDIO_SERVICEWATERGRIDITEMS_HPP

