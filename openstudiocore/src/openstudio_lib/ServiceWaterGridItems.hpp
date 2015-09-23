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
             QWidget *widget = nullptr) override;
};

class WaterUseConnectionsDetailItem : public GridItem
{
  public:

  WaterUseConnectionsDetailItem(WaterUseConnectionsDetailScene * waterUseConnectionsDetailScene);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

class WaterUseConnectionsDropZoneItem : public HorizontalBranchItem
{
  public:

  WaterUseConnectionsDropZoneItem(QGraphicsItem * parent = nullptr);
};

class WaterUseEquipmentDropZoneItem : public HorizontalBranchItem
{
  public:

  WaterUseEquipmentDropZoneItem(QGraphicsItem * parent = nullptr);
};

class SewerItem : public GridItem
{
  public:

  SewerItem(QGraphicsItem * parent = nullptr);

  ButtonItem * sewerButton() const;

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;

  ButtonItem * m_sewerButton;
};

class MainsSupplyItem : public GridItem
{
  public:

  MainsSupplyItem(QGraphicsItem * parent = nullptr);

  ButtonItem * mainsSupplyButton() const;

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;

  ButtonItem * m_mainsSupplyButton;
};

class HotWaterSupplyItem : public GridItem
{
  Q_OBJECT;

  public:

  HotWaterSupplyItem(QGraphicsItem * parent = nullptr);

  virtual ~HotWaterSupplyItem() {}

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;

  ButtonItem * m_hotWaterSupplyButton;

  private slots:

  void onHotWaterSupplyButtonClicked();
};

class DoubleOneThreeStraightItem : public GridItem
{
  public:

  DoubleOneThreeStraightItem(QGraphicsItem * parent = nullptr);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

class DoubleThreeFourStraightItem : public GridItem
{
  public:

  DoubleThreeFourStraightItem(QGraphicsItem * parent = nullptr);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

class DoubleTwoFourStraightItem : public GridItem
{
  public:

  DoubleTwoFourStraightItem(QGraphicsItem * parent = nullptr);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

class DoubleTwoThreeStraightItem : public GridItem
{
  public:

  DoubleTwoThreeStraightItem(QGraphicsItem * parent = nullptr);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

class DoubleTwoThreeFourStraightItem : public GridItem
{
  public:

  DoubleTwoThreeFourStraightItem(QGraphicsItem * parent = nullptr);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

class HotWaterJunctionItem : public DoubleTwoFourStraightItem
{
  public:

  HotWaterJunctionItem(QGraphicsItem * parent = nullptr);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

class ColdWaterJunctionItem : public GridItem
{
  public:

  ColdWaterJunctionItem(int isElbow,QGraphicsItem * parent = nullptr);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;

  private:

  bool m_isElbow;
};

class WaterUseConnectionsItem : public GridItem
{
  public:

  WaterUseConnectionsItem(QGraphicsItem * parent = nullptr);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

class WaterUseEquipmentItem : public GridItem
{
  public:

  WaterUseEquipmentItem(QGraphicsItem * parent = nullptr);

  void setModelObject( model::OptionalModelObject modelObject ) override;

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

class MakeupWaterItem : public GridItem
{
  Q_OBJECT;

  public:

  MakeupWaterItem(QGraphicsItem * parent = nullptr);

  virtual ~MakeupWaterItem() {}

  ButtonItem * mainsSupplyButton() const;

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;

  private slots:

  void onHotWaterSupplyButtonClicked();

  private:

  ButtonItem * m_mainsSupplyButton;

  ButtonItem * m_hotWaterSupplyButton;
};

} // openstudio

#endif // OPENSTUDIO_SERVICEWATERGRIDITEMS_HPP

