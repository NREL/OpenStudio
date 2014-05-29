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

#include "ServiceWaterGridItems.hpp"
#include "ServiceWaterScene.hpp"
#include "IconLibrary.hpp"
#include "../model/WaterUseConnections.hpp"
#include "../model/WaterUseConnections_Impl.hpp"
#include "../model/WaterUseEquipment.hpp"
#include "../model/WaterUseEquipment_Impl.hpp"
#include "../model/PlantLoop.hpp"
#include "../model/PlantLoop_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include <QPainter>
#include <QMimeData>
#include <QGraphicsSceneDragDropEvent>
#include <QKeyEvent>
#include <QKeySequence>
#include <QGraphicsScene>
#include <QApplication>

using namespace openstudio::model;

namespace openstudio {

WaterUseConnectionsDetailItem::WaterUseConnectionsDetailItem(WaterUseConnectionsDetailScene * waterUseConnectionsDetailScene)
  : GridItem()
{
  waterUseConnectionsDetailScene->addItem(this);

  model::WaterUseConnections waterUseConnections = waterUseConnectionsDetailScene->waterUseConnections();

  std::vector<model::WaterUseEquipment> waterEquipmentObjects = waterUseConnections.waterUseEquipment();

  int branchCount = waterEquipmentObjects.size();

  setHGridLength(12);

  setVGridLength(6);

  int i = 1;
  int j = 1;

  // Sewer

  SewerItem * sewerItem = new SewerItem(this);

  sewerItem->sewerButton()->setToolTip("Go back to water mains editor");

  connect(sewerItem->sewerButton(),SIGNAL(mouseClicked()),waterUseConnectionsDetailScene,SIGNAL(goToServiceWaterSceneClicked()));

  sewerItem->setGridPos(i,j + 3);

  i = i + sewerItem->getHGridLength();

  // Left Vertical

  TwoFourStraightItem * leftVItem1 = new TwoFourStraightItem(this);

  leftVItem1->setEnableHighlight(false);

  leftVItem1->setGridPos(i,j + 1);

  if( branchCount > 0 )
  {
    OneTwoFourStraightItem * leftVItem2 = new OneTwoFourStraightItem(this);

    leftVItem2->setEnableHighlight(false);
    
    leftVItem2->setGridPos(i,j + 2);
  }
  else
  {
    TwoFourStraightItem * leftVItem2 = new TwoFourStraightItem(this);

    leftVItem2->setEnableHighlight(false);

    leftVItem2->setGridPos(i,j + 2);
  }

  if( branchCount > 1 )
  {
    TwoThreeFourStraightItem * leftVItem3 = new TwoThreeFourStraightItem(this);

    leftVItem3->setEnableHighlight(false);

    leftVItem3->setGridPos(i,j + 3);
  }
  else
  {
    TwoThreeStraightItem * leftVItem3 = new TwoThreeStraightItem(this);

    leftVItem3->setEnableHighlight(false);

    leftVItem3->setGridPos(i,j + 3);
  }

  // Left top elbow

  OneFourStraightItem * leftTopElbow = new OneFourStraightItem(this);

  leftTopElbow->setEnableHighlight(false);

  leftTopElbow->setGridPos(i,j);

  i = i + leftTopElbow->getHGridLength();

  // Outlet straight

  OneThreeStraightItem * straightItem = new OneThreeStraightItem(this);

  straightItem->setEnableHighlight(false);

  straightItem->setGridPos(i,j);

  i = i + straightItem->getHGridLength();

  // Drop Zone

  WaterUseEquipmentDropZoneItem * dropBranchItem = new WaterUseEquipmentDropZoneItem(this);

  dropBranchItem->setModelObject(waterUseConnections);

  dropBranchItem->setGridPos(i,j);

  i = i + dropBranchItem->getHGridLength();

  // Inlet straight

  DoubleOneThreeStraightItem * straightItem2 = new DoubleOneThreeStraightItem(this);

  straightItem2->setEnableHighlight(false);

  straightItem2->setGridPos(i,j);

  i = i + straightItem2->getHGridLength();

  // Right Top elbow

  DoubleThreeFourStraightItem * rightTopElbow = new DoubleThreeFourStraightItem(this);

  rightTopElbow->setEnableHighlight(false);

  rightTopElbow->setGridPos(i,j);

  // Hot water supply

  HotWaterSupplyItem * hotWaterSupplyItem = new HotWaterSupplyItem(this);

  hotWaterSupplyItem->setGridPos(i + 1,j + 1);

  // Right Vertical

  HotWaterJunctionItem * rightVItem1 = new HotWaterJunctionItem(this);

  rightVItem1->setEnableHighlight(false);

  rightVItem1->setGridPos(i,j + 1);

  if( branchCount > 0 )
  {
    DoubleTwoThreeFourStraightItem * rightVItem2 = new DoubleTwoThreeFourStraightItem(this);

    rightVItem2->setEnableHighlight(false);

    rightVItem2->setGridPos(i,j + 2);
  }
  else
  {
    DoubleTwoFourStraightItem * rightVItem2 = new DoubleTwoFourStraightItem(this);

    rightVItem2->setEnableHighlight(false);

    rightVItem2->setGridPos(i,j + 2);
  }

  if( branchCount > 1)
  {
    ColdWaterJunctionItem * rightVItem3 = new ColdWaterJunctionItem(false,this);

    rightVItem3->setGridPos(i,j + 3);

    i = i + rightVItem1->getHGridLength();
  }
  else
  {
    ColdWaterJunctionItem * rightVItem3 = new ColdWaterJunctionItem(true,this);

    rightVItem3->setGridPos(i,j + 3);

    i = i + rightVItem1->getHGridLength();
  }

  // Mains supply

  MainsSupplyItem * mainsSupplyItem = new MainsSupplyItem(this);

  mainsSupplyItem->mainsSupplyButton()->setToolTip("Go back to water mains editor");

  connect(mainsSupplyItem->mainsSupplyButton(),SIGNAL(mouseClicked()),waterUseConnectionsDetailScene,SIGNAL(goToServiceWaterSceneClicked()));

  mainsSupplyItem->setGridPos(i,j + 3);

  i = i + mainsSupplyItem->getHGridLength();

  // Add branches

  j = 3;

  for( int b = 0; b < branchCount; b++ )
  {
    i = 4; 

    OneThreeStraightItem * outletItem = new OneThreeStraightItem(this);

    outletItem->setEnableHighlight(false);

    outletItem->setGridPos(i,j);

    i = i + outletItem->getHGridLength();

    WaterUseEquipmentItem * waterUseEquipmentItem = new WaterUseEquipmentItem(this);

    model::WaterUseEquipment waterUseEquipment = waterEquipmentObjects[b];

    waterUseEquipmentItem->setModelObject(waterUseEquipment);

    waterUseEquipmentItem->setGridPos(i,j);

    i = i + waterUseEquipmentItem->getHGridLength();

    DoubleOneThreeStraightItem * inletItem = new DoubleOneThreeStraightItem(this);

    inletItem->setEnableHighlight(false);

    inletItem->setGridPos(i,j);

    if( b != 0 && b < branchCount - 1 )
    {
      DoubleTwoThreeFourStraightItem * inletJunctionItem = new DoubleTwoThreeFourStraightItem(this);

      inletJunctionItem->setEnableHighlight(false);

      inletJunctionItem->setGridPos(8,j);

      OneTwoFourStraightItem * outletJunctionItem = new OneTwoFourStraightItem(this);

      outletJunctionItem->setEnableHighlight(false);

      outletJunctionItem->setGridPos(3,j);
    }

    if( b > 1 )
    {
      TwoFourStraightItem * leftStraight = new TwoFourStraightItem(this);

      leftStraight->setEnableHighlight(false);

      leftStraight->setGridPos(3,j - 1);

      DoubleTwoFourStraightItem * rightStraight = new DoubleTwoFourStraightItem(this);

      rightStraight->setEnableHighlight(false);

      rightStraight->setGridPos(8,j - 1);
    }

    if( b == branchCount - 1 && branchCount > 1 )
    {
      OneTwoStraightItem * bottomLeftCorner = new OneTwoStraightItem(this);

      bottomLeftCorner->setEnableHighlight(false);

      bottomLeftCorner->setGridPos(3,j);

      DoubleTwoThreeStraightItem * bottomRightCorner = new DoubleTwoThreeStraightItem(this);

      bottomRightCorner->setEnableHighlight(false);

      bottomRightCorner->setGridPos(8,j);
    }

    j = j + 2;
  }

  if( j > 6 )
  {
    setVGridLength(j);
  }

  // Makeup Water

  MakeupWaterItem * makeupWaterItem = new MakeupWaterItem(this);
  makeupWaterItem->setGridPos(1,1);

  connect(makeupWaterItem->mainsSupplyButton(),SIGNAL(mouseClicked()),waterUseConnectionsDetailScene,SIGNAL(goToServiceWaterSceneClicked()));

}

void WaterUseConnectionsDetailItem::paint( QPainter *painter, 
                           const QStyleOptionGraphicsItem *option, 
                           QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(QBrush(QColor(206,206,206),Qt::SolidPattern));
  painter->setPen(Qt::NoPen);

  painter->drawRoundedRect(10,10,boundingRect().width() - 20, boundingRect().height() - 20,8,8);
}

ServiceWaterItem::ServiceWaterItem(ServiceWaterScene * serviceWaterScene)
  : GridItem()
{
  serviceWaterScene->addItem(this);

  std::vector<model::WaterUseConnections> waterConnectionsObjects = serviceWaterScene->model().getConcreteModelObjects<model::WaterUseConnections>();

  int branchCount = waterConnectionsObjects.size();

  setHGridLength(12);

  //setVGridLength(6);

  int i = 1;
  int j = 1;

  // Sewer

  SewerItem * sewerItem = new SewerItem(this);

  sewerItem->setGridPos(i,j + 1);

  i = i + sewerItem->getHGridLength();

  // Left Vertical

  if( branchCount > 0 )
  {
    TwoThreeFourStraightItem * leftVItem1 = new TwoThreeFourStraightItem(this);

    leftVItem1->setEnableHighlight(false);
    
    leftVItem1->setGridPos(i,j + 1);
  }
  else
  {
    TwoThreeStraightItem * leftVItem1 = new TwoThreeStraightItem(this);

    leftVItem1->setEnableHighlight(false);

    leftVItem1->setGridPos(i,j + 1);
  }

  // Left top elbow

  OneFourStraightItem * leftTopElbow = new OneFourStraightItem(this);

  leftTopElbow->setEnableHighlight(false);

  leftTopElbow->setGridPos(i,j);

  i = i + leftTopElbow->getHGridLength();

  // Outlet straight

  OneThreeStraightItem * straightItem = new OneThreeStraightItem(this);

  straightItem->setEnableHighlight(false);

  straightItem->setGridPos(i,j);

  i = i + straightItem->getHGridLength();

  // Drop Zone

  WaterUseConnectionsDropZoneItem * dropBranchItem = new WaterUseConnectionsDropZoneItem(this);

  dropBranchItem->setGridPos(i,j);

  i = i + dropBranchItem->getHGridLength();

  // Inlet straight

  OneThreeStraightItem * straightItem2 = new OneThreeStraightItem(this);

  straightItem2->setEnableHighlight(false);

  straightItem2->setGridPos(i,j);

  i = i + straightItem2->getHGridLength();

  // Right Top elbow

  ThreeFourStraightItem * rightTopElbow = new ThreeFourStraightItem(this);

  rightTopElbow->setEnableHighlight(false);

  rightTopElbow->setGridPos(i,j);

  // Right Vertical

  if( branchCount > 0 )
  {
    OneTwoFourStraightItem * rightVItem1 = new OneTwoFourStraightItem(this);

    rightVItem1->setEnableHighlight(false);

    rightVItem1->setGridPos(i,j + 1);

    i = i + rightVItem1->getHGridLength();
  }
  else
  {
    OneTwoStraightItem * rightVItem1 = new OneTwoStraightItem(this);

    rightVItem1->setEnableHighlight(false);

    rightVItem1->setGridPos(i,j + 1);

    i = i + rightVItem1->getHGridLength();
  }

  // Mains supply

  MainsSupplyItem * mainsSupplyItem = new MainsSupplyItem(this);

  mainsSupplyItem->setGridPos(i,j + 1);

  i = i + mainsSupplyItem->getHGridLength();

  // Add branches

  j = 3;

  for( int b = 0; b < branchCount; b++ )
  {
    i = 4; 

    OneThreeStraightItem * outletItem = new OneThreeStraightItem(this);

    outletItem->setEnableHighlight(false);

    outletItem->setGridPos(i,j);

    i = i + outletItem->getHGridLength();

    WaterUseConnectionsItem * waterUseConnectionsItem = new WaterUseConnectionsItem(this);

    waterUseConnectionsItem->setModelObject(waterConnectionsObjects[b]);

    waterUseConnectionsItem->setGridPos(i,j);

    i = i + waterUseConnectionsItem->getHGridLength();

    OneThreeStraightItem * inletItem = new OneThreeStraightItem(this);

    inletItem->setEnableHighlight(false);

    inletItem->setGridPos(i,j);

    if( b > 0 )
    {
      TwoFourStraightItem * leftStraight = new TwoFourStraightItem(this);

      leftStraight->setEnableHighlight(false);

      leftStraight->setGridPos(3,j - 1);

      TwoFourStraightItem * rightStraight = new TwoFourStraightItem(this);

      rightStraight->setEnableHighlight(false);

      rightStraight->setGridPos(8,j - 1);
    }

    if( b == branchCount - 1 )
    {
      OneTwoStraightItem * bottomLeftCorner = new OneTwoStraightItem(this);

      bottomLeftCorner->setEnableHighlight(false);

      bottomLeftCorner->setGridPos(3,j);

      TwoThreeStraightItem * bottomRightCorner = new TwoThreeStraightItem(this);

      bottomRightCorner->setEnableHighlight(false);

      bottomRightCorner->setGridPos(8,j);
    }
    else
    {
      TwoThreeFourStraightItem * inletJunctionItem = new TwoThreeFourStraightItem(this);

      inletJunctionItem->setEnableHighlight(false);

      inletJunctionItem->setGridPos(8,j);

      OneTwoFourStraightItem * outletJunctionItem = new OneTwoFourStraightItem(this);

      outletJunctionItem->setEnableHighlight(false);

      outletJunctionItem->setGridPos(3,j);
    }

    j = j + 2;
  }

  if( j < 4 )
  {
    j = 4;
  }

  setVGridLength(j);
}

void ServiceWaterItem::paint( QPainter *painter, 
                           const QStyleOptionGraphicsItem *option, 
                           QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(QBrush(QColor(206,206,206),Qt::SolidPattern));
  painter->setPen(Qt::NoPen);

  painter->drawRoundedRect(10,10,boundingRect().width() - 20, boundingRect().height() - 20,8,8);
}

WaterUseEquipmentDropZoneItem::WaterUseEquipmentDropZoneItem(QGraphicsItem * parent)
  : HorizontalBranchItem(std::vector<model::ModelObject>(),parent)
{
  setIsDropZone(true);

  setHasTwoRightSidePipes(true);

  setHGridLength(2);

  setText("Drag Water Use Equipment from Library");
}

WaterUseConnectionsDropZoneItem::WaterUseConnectionsDropZoneItem(QGraphicsItem * parent)
  : HorizontalBranchItem(std::vector<model::ModelObject>(),parent)
{
  setIsDropZone(true);

  setHGridLength(2);

  setText("Drag Water Use Connections from Library");
}

SewerItem::SewerItem(QGraphicsItem * parent)
  : GridItem(parent)
{
  setHGridLength(2);

  setEnableHighlight(false);

  QPixmap sewerPixmap(":/images/sewer_alone.png");
  QPixmap sewerDownPixmap(":/images/sewer_press.png");
  QPixmap sewerHoverPixmap(":/images/sewer_over.png");

  m_sewerButton = new ButtonItem(sewerPixmap,sewerDownPixmap,sewerHoverPixmap,this);
}

ButtonItem * SewerItem::sewerButton() const
{
  return m_sewerButton;
}

void SewerItem::paint( QPainter *painter, 
                       const QStyleOptionGraphicsItem *option, 
                       QWidget *widget )
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));

  painter->drawLine( 50,50,200,50 );

  QTransform transform;
  transform = transform.rotate(90);

  painter->drawPixmap(137,37,25,25,QPixmap(":/images/arrow.png").transformed(transform));
}

HotWaterSupplyItem::HotWaterSupplyItem(QGraphicsItem * parent)
  : GridItem(parent)
{
  setHGridLength(2);

  setEnableHighlight(false);

  QPixmap hotWaterSupplyPixmap(":/images/loop_button.png");
  QPixmap hotWaterSupplyOverPixmap(":/images/loop_button_over.png");
  QPixmap hotWaterSupplyPressPixmap(":/images/loop_button_press.png");

  m_hotWaterSupplyButton = new ButtonItem(hotWaterSupplyPixmap,hotWaterSupplyPressPixmap,
                                          hotWaterSupplyOverPixmap,this);

  m_hotWaterSupplyButton->setToolTip("Go back to hot water supply system");

  connect(m_hotWaterSupplyButton,SIGNAL(mouseClicked()),this,SLOT(onHotWaterSupplyButtonClicked()));

  m_hotWaterSupplyButton->setPos(105.9,37);
}

void HotWaterSupplyItem::onHotWaterSupplyButtonClicked()
{
  if( WaterUseConnectionsDetailScene * detailScene = qobject_cast<WaterUseConnectionsDetailScene *>(scene()) )
  {
    model::WaterUseConnections waterUseConnections = detailScene->waterUseConnections();

    emit innerNodeClicked( waterUseConnections );
  }
}

void HotWaterSupplyItem::paint( QPainter *painter, 
                           const QStyleOptionGraphicsItem *option, 
                           QWidget *widget )
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));

  painter->drawLine( 0,50,150,50 );

  QTransform transform;
  transform = transform.rotate(90);

  painter->drawPixmap(37,37,25,25,QPixmap(":/images/arrow.png").transformed(transform));
}

MainsSupplyItem::MainsSupplyItem(QGraphicsItem * parent)
  : GridItem(parent)
{
  setHGridLength(2);

  setEnableHighlight(false);

  QPixmap waterMainPixmap(":/images/water_main_alone.png");
  QPixmap waterMainPressPixmap(":/images/water_main_alone_press.png");
  QPixmap waterMainOverPixmap(":/images/water_main_alone_over.png");

  m_mainsSupplyButton = new ButtonItem(waterMainPixmap,waterMainPressPixmap,waterMainOverPixmap,this);

  m_mainsSupplyButton->setPos(100,0);
}

ButtonItem * MainsSupplyItem::mainsSupplyButton() const
{
  return m_mainsSupplyButton;
}

void MainsSupplyItem::paint( QPainter *painter, 
                           const QStyleOptionGraphicsItem *option, 
                           QWidget *widget )
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));

  painter->drawLine( 0,50,150,50 );

  QTransform transform;
  transform = transform.rotate(90);

  painter->drawPixmap(37,37,25,25,QPixmap(":/images/arrow.png").transformed(transform));
}

DoubleOneThreeStraightItem::DoubleOneThreeStraightItem( QGraphicsItem * parent )
  : GridItem(parent)
{
}

void DoubleOneThreeStraightItem::paint(QPainter *painter, 
                                       const QStyleOptionGraphicsItem *option, 
                                       QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));

  painter->drawLine( 0,33,100,33 );
  painter->drawLine( 0,66,100,66 );
}

DoubleThreeFourStraightItem::DoubleThreeFourStraightItem( QGraphicsItem * parent )
  : GridItem(parent)
{
}

void DoubleThreeFourStraightItem::paint(QPainter *painter, 
                                       const QStyleOptionGraphicsItem *option, 
                                       QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));

  painter->drawLine( 0,33,66,33 );
  painter->drawLine( 66,33,66,100 );

  painter->drawLine( 0,66,33,66 );
  painter->drawLine( 33,66,33,100 );
}

DoubleTwoFourStraightItem::DoubleTwoFourStraightItem( QGraphicsItem * parent )
  : GridItem(parent)
{
}

void DoubleTwoFourStraightItem::paint(QPainter *painter, 
                                       const QStyleOptionGraphicsItem *option, 
                                       QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));

  painter->drawLine( 33,0,33,100 );
  painter->drawLine( 66,0,66,100 );
}

DoubleTwoThreeStraightItem::DoubleTwoThreeStraightItem( QGraphicsItem * parent )
  : GridItem(parent)
{
}

void DoubleTwoThreeStraightItem::paint(QPainter *painter, 
                                       const QStyleOptionGraphicsItem *option, 
                                       QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));

  painter->drawLine( 66,0,66,33 );
  painter->drawLine( 0,33,16,33 );
  painter->drawArc( 16,16,33,33,2880,-2880 );
  painter->drawLine( 50,33,66,33 );

  painter->drawLine( 33,0,33,66 );
  painter->drawLine( 0,66,33,66 );
}

HotWaterJunctionItem::HotWaterJunctionItem( QGraphicsItem * parent )
  : DoubleTwoFourStraightItem(parent)
{
}

void HotWaterJunctionItem::paint(QPainter *painter, 
                                       const QStyleOptionGraphicsItem *option, 
                                       QWidget *widget)
{
  DoubleTwoFourStraightItem::paint(painter,option,widget);

  painter->drawLine( 33,50,50,50 );
  painter->drawArc( 50,33,33,33,2880,-2880 );
  painter->drawLine( 83,50,100,50 );
}

ColdWaterJunctionItem::ColdWaterJunctionItem( int isElbow, QGraphicsItem * parent )
  : GridItem(parent),
    m_isElbow(isElbow)
{
  setEnableHighlight(false);
}

void ColdWaterJunctionItem::paint(QPainter *painter, 
                                  const QStyleOptionGraphicsItem *option, 
                                  QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(QBrush(Qt::black,Qt::SolidPattern));
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));

  if( m_isElbow )
  {
    painter->drawLine( 66,0,66,50 );
    painter->drawLine( 66,50,100,50 );
    painter->drawLine( 33,0,33,50 );

    painter->drawEllipse( 28,45,10,10 );
  }
  else
  {
    painter->drawLine( 66,0,66,100 );
    painter->drawLine( 66,50,100,50 );
    painter->drawLine( 33,0,33,100 );
  }
}

DoubleTwoThreeFourStraightItem::DoubleTwoThreeFourStraightItem(QGraphicsItem * parent)
  : GridItem(parent)
{
}

void DoubleTwoThreeFourStraightItem::paint(QPainter *painter, 
                                   const QStyleOptionGraphicsItem *option, 
                                   QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));

  painter->drawLine( 33,0,33,100 );
  painter->drawLine( 66,0,66,100 );

  painter->drawLine( 0,33,16,33 );
  painter->drawArc( 16,16,33,33,2880,-2880 );
  painter->drawLine( 50,33,66,33 );
  painter->drawLine( 0,66,33,66 );
}

WaterUseConnectionsItem::WaterUseConnectionsItem(QGraphicsItem * parent)
  : GridItem(parent)
{
  setHGridLength(2);

  setDeletable(true);
}

void WaterUseConnectionsItem::paint(QPainter *painter, 
                                    const QStyleOptionGraphicsItem *option, 
                                    QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->drawPixmap(0,0,200,100,QPixmap(":/images/water_connection.png"));
}

WaterUseEquipmentItem::WaterUseEquipmentItem(QGraphicsItem * parent)
  : GridItem(parent)
{
  setHGridLength(2);
}

void WaterUseEquipmentItem::setModelObject( model::OptionalModelObject modelObject )
{
  if( modelObject )
  {
    setDeletable(true);
  }
  else
  {
    setDeletable(false);
  }

  GridItem::setModelObject(modelObject);
}

void WaterUseEquipmentItem::paint(QPainter *painter, 
                                  const QStyleOptionGraphicsItem *option, 
                                  QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->drawPixmap(0,0,200,100,QPixmap(":/images/sink.png"));
}

MakeupWaterItem::MakeupWaterItem(QGraphicsItem * parent)
  : GridItem(parent)
{
  setHGridLength(2);
  setVGridLength(2);

  QPixmap waterMainPixmap(":/images/water_main_alone.png");
  QPixmap waterMainPressPixmap(":/images/water_main_alone_press.png");
  QPixmap waterMainOverPixmap(":/images/water_main_alone_over.png");

  m_mainsSupplyButton = new ButtonItem(waterMainPixmap,waterMainPressPixmap,waterMainOverPixmap,this);

  m_mainsSupplyButton->setToolTip("Go back to water mains editor");

  m_mainsSupplyButton->setPos(75,0); 

  QPixmap hotWaterSupplyPixmap(":/images/loop_button.png");
  QPixmap hotWaterSupplyPressPixmap(":/images/loop_button_press.png");
  QPixmap hotWaterSupplyOverPixmap(":/images/loop_button_over.png");

  m_hotWaterSupplyButton = new ButtonItem(hotWaterSupplyPixmap,hotWaterSupplyPressPixmap,
                                                     hotWaterSupplyOverPixmap,this);


  m_hotWaterSupplyButton->setToolTip("Go back to hot water supply system");

  connect(m_hotWaterSupplyButton,SIGNAL(mouseClicked()),this,SLOT(onHotWaterSupplyButtonClicked()));

  m_hotWaterSupplyButton->setPos(90,137);
}

void MakeupWaterItem::paint(QPainter *painter, 
                                  const QStyleOptionGraphicsItem *option, 
                                  QWidget *widget)
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));

  painter->drawLine( 125,50,125,140 );
  painter->drawLine( 50,150,100,150 );

  QTransform transform;
  transform = transform.rotate(90);

  painter->drawPixmap(37,137,25,25,QPixmap(":/images/arrow.png").transformed(transform));
}

void MakeupWaterItem::onHotWaterSupplyButtonClicked()
{
  if( WaterUseConnectionsDetailScene * detailScene = qobject_cast<WaterUseConnectionsDetailScene *>(scene()) )
  {
    model::WaterUseConnections waterUseConnections = detailScene->waterUseConnections();

    emit innerNodeClicked( waterUseConnections );
  }
}

ButtonItem * MakeupWaterItem::mainsSupplyButton() const
{
  return m_mainsSupplyButton;
}

} // openstudio

