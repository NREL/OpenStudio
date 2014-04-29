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

#include "GridItem.hpp"
#include "ServiceWaterGridItems.hpp"
#include "IconLibrary.hpp"
#include "LoopScene.hpp"
#include "SchedulesView.hpp"
#include "OSDocument.hpp"
#include "OSAppBase.hpp"
#include "MainRightColumnController.hpp"

#include <utilities/core/Assert.hpp>

#include <QPainter>
#include <QMimeData>
#include <QGraphicsSceneDragDropEvent>
#include <QKeyEvent>
#include <QKeySequence>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QApplication>
#include <QMenu>
#include <model/HVACComponent.hpp>
#include <model/HVACComponent_Impl.hpp>
#include <model/WaterUseConnections.hpp>
#include <model/WaterUseConnections_Impl.hpp>
#include <model/WaterToAirComponent.hpp>
#include <model/WaterToAirComponent_Impl.hpp>
#include <model/WaterToWaterComponent.hpp>
#include <model/WaterToWaterComponent_Impl.hpp>
#include <model/AirLoopHVACOutdoorAirSystem.hpp>
#include <model/AirLoopHVACOutdoorAirSystem_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/AirLoopHVACSupplyPlenum.hpp>
#include <model/AirLoopHVACSupplyPlenum_Impl.hpp>
#include <model/AirLoopHVACReturnPlenum.hpp>
#include <model/AirLoopHVACReturnPlenum_Impl.hpp>
#include <model/AirToAirComponent.hpp>
#include <model/AirToAirComponent_Impl.hpp>
#include <model/PlantLoop.hpp>
#include <model/PlantLoop_Impl.hpp>
#include <model/SetpointManagerMixedAir.hpp>
#include <model/SetpointManagerMixedAir_Impl.hpp>
#include <model/SetpointManagerOutdoorAirReset.hpp>
#include <model/SetpointManagerOutdoorAirReset_Impl.hpp>
#include <model/SetpointManagerSingleZoneReheat.hpp>
#include <model/SetpointManagerSingleZoneReheat_Impl.hpp>
#include <model/SetpointManagerScheduled.hpp>
#include <model/SetpointManagerScheduled_Impl.hpp>
#include <model/SetpointManagerFollowOutdoorAirTemperature.hpp>
#include <model/SetpointManagerFollowOutdoorAirTemperature_Impl.hpp>
#include <model/SetpointManagerWarmest.hpp>
#include <model/SetpointManagerWarmest_Impl.hpp>
#include <model/RenderingColor.hpp>
#include <model/RenderingColor_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/Splitter.hpp>
#include <model/Splitter_Impl.hpp>
#include <model/Mixer.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <algorithm>

using namespace openstudio::model;

namespace openstudio {

bool hasSPM(model::Node & node)
{
  if( node.getSetpointManagerSingleZoneReheat()
      || node.getSetpointManagerMixedAir()
      || node.setpointManagerOutdoorAirReset()
      || node.setpointManagerScheduled()
      || node.setpointManagerWarmest()
      || node.setpointManagerFollowOutdoorAirTemperature() )
  {
    return true;
  }
  else
  {
    return false;
  }
}

GridItem::GridItem( QGraphicsItem * parent ):
  QGraphicsObject( parent ),
  m_deleteAble(false),
  m_highlight(false),
  m_hLength(1),
  m_vLength(1),
  m_removeButtonItem(NULL),
  m_enableHighlight(true)
{
  setAcceptHoverEvents(true);
  setAcceptDrops(false);
  setFlag(QGraphicsItem::ItemIsFocusable);
  setFlag(QGraphicsItem::ItemIsSelectable,false);
  if( QGraphicsScene * _scene = scene() )
  {
    connect( this, 
             SIGNAL(modelObjectSelected( model::OptionalModelObject &, bool ) ),
             _scene,
             SIGNAL(modelObjectSelected( model::OptionalModelObject &, bool ) ) );

    connect( this, 
             SIGNAL(removeModelObjectClicked( model::ModelObject & ) ),
             _scene,
             SIGNAL(removeModelObjectClicked( model::ModelObject & ) ) );

    connect( this, 
             SIGNAL( hvacComponentDropped(OSItemId, model::HVACComponent &) ),
             _scene,
             SIGNAL( hvacComponentDropped(OSItemId, model::HVACComponent &) ) );

    connect( this, 
             SIGNAL( hvacComponentDropped(OSItemId) ),
             _scene,
             SIGNAL( hvacComponentDropped(OSItemId) ) );

    connect( this, 
             SIGNAL( innerNodeClicked(model::ModelObject &) ),
             _scene,
             SIGNAL( innerNodeClicked(model::ModelObject & ) ) );
  }
}

void GridItem::setEnableHighlight(bool highlight)
{
  m_enableHighlight = highlight;
}

void GridItem::setDeletable(bool deletable)
{
  if( deletable )
  {
    m_removeButtonItem = new RemoveButtonItem(this);

    m_removeButtonItem->setPos(boundingRect().width() - 30, boundingRect().height() - 30);
  
    connect(m_removeButtonItem,SIGNAL(mouseClicked()),this,SLOT(onRemoveButtonClicked()));
  }
  else
  {
    if( m_removeButtonItem )
    {
      m_removeButtonItem->deleteLater();

      m_removeButtonItem = NULL; 
    }
  }
}

void GridItem::onRemoveButtonClicked()
{
  if( m_modelObject )
  {
    emit removeModelObjectClicked(m_modelObject.get());
  }
}

QRectF GridItem::boundingRect() const
{
  return QRectF(0,0,m_hLength * 100,m_vLength * 100);
}

void GridItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
  event->accept();
  m_highlight = true;
  update();
}

void GridItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
  event->accept();
  m_highlight = false;
  update();
}

void GridItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  painter->setPen(QPen(Qt::black,4,Qt::NoPen, Qt::RoundCap));
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->drawRect(0,0,m_hLength * 100,m_vLength * 100);

  if(isSelected())
  {
    painter->setBrush(QBrush(QColor(128,128,128),Qt::SolidPattern));
    painter->drawRect(0,0,m_hLength * 100,m_vLength * 100);
  }

  if(m_highlight && m_enableHighlight)
  {
    painter->setPen(QPen(Qt::black,4,Qt::NoPen, Qt::RoundCap));
    painter->setBrush(QBrush(QColor(127, 127, 127, 127)));
    painter->drawRect(0,0,m_hLength * 100,m_vLength * 100);
  }
}

QVariant GridItem::itemChange( GraphicsItemChange change, const QVariant & value )
{
  if( change == QGraphicsItem::ItemSelectedHasChanged )
  {
    if(m_modelObject)
    {
      if(value.toBool())
      {
        emit modelObjectSelected( m_modelObject, false );
      }
      else
      {
        boost::optional<model::ModelObject> mo;

        emit modelObjectSelected(mo, false);
      }
    }
  }

  return QGraphicsItem::itemChange(change,value);
}

void GridItem::setGridPos(int x, int y)
{
  setPos(x * 100, y * 100);
}

int GridItem::getXGridPos()
{
  return x() / 100;
}

int GridItem::getYGridPos()
{
  return y() / 100;
}

void GridItem::setHGridLength(int l)
{
  m_hLength = l;
}

void GridItem::setVGridLength(int l)
{
  m_vLength = l;
}

void GridItem::setModelObject( model::OptionalModelObject modelObject )
{
  m_modelObject = modelObject;

  if( m_modelObject && m_modelObject->optionalCast<model::HVACComponent>() )
  {
    setAcceptDrops(true);

    setFlag(QGraphicsItem::ItemIsSelectable);

    if( m_modelObject->cast<model::HVACComponent>().isRemovable() )
    {
      setDeletable(true);
    }
    else
    {
      setDeletable(false);
    }
  }

  if( m_modelObject )
  {
    connect(m_modelObject->getImpl<detail::IdfObject_Impl>().get(),SIGNAL(onNameChange()),
            this, SLOT(onNameChange()));

    setFlag(QGraphicsItem::ItemIsSelectable);

    this->onNameChange();
  }
}

void GridItem::onNameChange()
{
  if( m_modelObject )
  {
    if( boost::optional<std::string> name = m_modelObject->name() )
    {
      setToolTip(QString::fromStdString(name.get()));
    }
  }
}

model::OptionalModelObject GridItem::modelObject()
{
  return m_modelObject;
}

int GridItem::getHGridLength()
{
  return m_hLength;
}

int GridItem::getVGridLength()
{
  return m_vLength;
}

void GridItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)  
{
  m_highlight = true;
  update();
}

void GridItem::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)  
{
  m_highlight = false;
  update();
}

void GridItem::dropEvent(QGraphicsSceneDragDropEvent *event)
{
  event->accept();

  if(event->proposedAction() == Qt::CopyAction)
  {
    if( !m_modelObject )
    {
      OSItemId id = OSItemId(event->mimeData());

      emit hvacComponentDropped(id);
    }
    else
    {
      try
      {
        Node node = m_modelObject->cast<Node>();
          
        event->setDropAction(Qt::CopyAction);

        OSItemId id = OSItemId(event->mimeData());

        emit hvacComponentDropped(id,node);
      }
      catch(std::bad_cast&)
      {
        return;
      }
    }
  }
}

LinkItem::LinkItem(QGraphicsItem * parent)
  : QGraphicsObject(parent),
    m_mouseDown(false),
    m_isHovering(false)
{
  this->setToolTip("Go to attached Loop");

  this->setAcceptHoverEvents(true);
}

void LinkItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  m_mouseDown = true;

  event->accept();
}

void LinkItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
  if( m_mouseDown )
  {
    if( this->shape().contains(event->pos()) )
    {
      event->accept();

      emit mouseClicked();
    }
  }

  m_mouseDown = false;
}

void LinkItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
  m_isHovering = true;

  this->update();

  event->accept();
}

void LinkItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
  m_isHovering = false;

  this->update();

  event->accept();
}

void LinkItem::paint( QPainter *painter, 
                      const QStyleOptionGraphicsItem *option, 
                      QWidget *widget )
{
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  if( m_isHovering )
  {
    painter->setBrush(QBrush(QColor(44,50,51),Qt::SolidPattern));
  }
  else
  {
    painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  }
  setZValue(1);

  painter->drawEllipse(3,3,14,14);
}

QRectF LinkItem::boundingRect() const
{
  return QRectF(0.0,0.0,20.0,20.0);
}

HorizontalBranchItem::HorizontalBranchItem( std::vector<model::ModelObject> modelObjects,
                                            QGraphicsItem * parent)
  : GridItem( parent ),
    m_isDropZone(false),
    m_text("Drag From Library"),
    m_hasDualTwoRightSidePipes(false)
{
  for( std::vector<model::ModelObject>::iterator it = modelObjects.begin();
       it < modelObjects.end(); ++it )
  {
    if(model::OptionalNode comp = it->optionalCast<model::Node>())
    {
      GridItem * gridItem = new OneThreeNodeItem(this); 
      gridItem->setModelObject( comp->optionalCast<model::ModelObject>() );
      if( comp->isRemovable() )
      {
        gridItem->setDeletable(true);
      }
      m_gridItems.push_back(gridItem);
    }
    else if(model::OptionalAirLoopHVACOutdoorAirSystem comp = it->optionalCast<model::AirLoopHVACOutdoorAirSystem>())
    {
      GridItem * gridItem = new OAMixerItem(this); 
      gridItem->setModelObject( comp->optionalCast<model::ModelObject>() );
      if( comp->isRemovable() )
      {
        gridItem->setDeletable(true);
      }
      m_gridItems.push_back(gridItem);
    }
    else if(model::OptionalThermalZone comp = it->optionalCast<model::ThermalZone>())
    {
      GridItem * gridItem = new OneThreeStraightItem(this); 
      gridItem->setModelObject( comp->optionalCast<model::ModelObject>() );
      if( comp->isRemovable() )
      {
        gridItem->setDeletable(true);
      }
      m_gridItems.push_back(gridItem);
    }
    else if(model::OptionalAirLoopHVACSupplyPlenum comp = it->optionalCast<model::AirLoopHVACSupplyPlenum>())
    {
      GridItem * gridItem = new SupplyPlenumItem(comp.get(),this); 
      if( comp->isRemovable() )
      {
        gridItem->setDeletable(true);
      }
      m_gridItems.push_back(gridItem);
    }
    else if(model::OptionalAirLoopHVACReturnPlenum comp = it->optionalCast<model::AirLoopHVACReturnPlenum>())
    {
      GridItem * gridItem = new ReturnPlenumItem(comp.get(),this); 
      if( comp->isRemovable() )
      {
        gridItem->setDeletable(true);
      }
      m_gridItems.push_back(gridItem);
    }
    else if(model::OptionalWaterUseConnections comp = it->optionalCast<model::WaterUseConnections>())
    {
      WaterUseConnectionsItem * gridItem = new WaterUseConnectionsItem(this); 
      gridItem->setModelObject( comp->optionalCast<model::ModelObject>() );
      m_gridItems.push_back(gridItem);
    }
    else if(model::OptionalStraightComponent comp = it->optionalCast<model::StraightComponent>())
    {
      GridItem * gridItem = new OneThreeStraightItem(this); 
      gridItem->setModelObject( comp->optionalCast<model::ModelObject>() );
      if( comp->isRemovable() )
      {
        gridItem->setDeletable(true);
      }
      m_gridItems.push_back(gridItem);
    }
    else if(boost::optional<model::WaterToAirComponent> comp = it->optionalCast<model::WaterToAirComponent>())
    {
      GridItem * gridItem = new OneThreeWaterToAirItem(this); 
      gridItem->setModelObject( comp->optionalCast<model::ModelObject>() );
      if( comp->isRemovable() )
      {
        gridItem->setDeletable(true);
      }
      m_gridItems.push_back(gridItem);
    }
    else if(boost::optional<model::WaterToWaterComponent> comp = it->optionalCast<model::WaterToWaterComponent>())
    {
      GridItem * gridItem = new OneThreeWaterToWaterItem(this); 
      gridItem->setModelObject( comp->optionalCast<model::ModelObject>() );
      if( comp->isRemovable() )
      {
        gridItem->setDeletable(true);
      }
      m_gridItems.push_back(gridItem);
    }
  }

  layout();
}

void HorizontalBranchItem::setModelObject( model::OptionalModelObject modelObject )
{
  m_modelObject = modelObject;
}

void HorizontalBranchItem::dropEvent(QGraphicsSceneDragDropEvent *event)
{
  event->accept();
  if(event->proposedAction() == Qt::CopyAction)
  {
    if( !m_modelObject )
    {
      OSItemId id = OSItemId(event->mimeData());

      emit hvacComponentDropped(id);
    }
    else if( boost::optional<HVACComponent> hvacComponent = m_modelObject->optionalCast<HVACComponent>() )
    {
      event->setDropAction(Qt::CopyAction);

      OSItemId id = OSItemId(event->mimeData());

      emit hvacComponentDropped(id,hvacComponent.get());
    }
  }
}

void HorizontalBranchItem::setIsDropZone(bool isDropZone)
{
  m_isDropZone = isDropZone;

  setAcceptDrops(m_isDropZone);

  layout();
}

void HorizontalBranchItem::setPadding( unsigned padding )
{
  if( m_paddingItems.size() > padding )
  {
    for( std::vector<OneThreeStraightItem *>::iterator it = m_paddingItems.begin() + padding;
         it < m_paddingItems.end(); 
         ++it )
    {
      (*it)->scene()->removeItem(*it);
    } 
    m_paddingItems.erase(m_paddingItems.begin() + padding, m_paddingItems.end());
  }
  else if( m_paddingItems.size() < padding )
  {
    for( unsigned i = m_paddingItems.size(); i < padding; i++ )
    {
      OneThreeStraightItem * straightItem = new OneThreeStraightItem();
      straightItem->setParentItem(this);
      m_paddingItems.push_back( straightItem );
    }
  }

  layout();
}

unsigned HorizontalBranchItem::padding()
{
  return m_paddingItems.size();
}

void HorizontalBranchItem::layout()
{
  int i = 0;

  for( std::vector<GridItem *>::iterator it = m_gridItems.begin();
       it < m_gridItems.end(); ++it )
  {
    if( m_isDropZone )
    {
      (*it)->hide();
    }
    else
    {
      (*it)->setGridPos( i, 0 );
      i = i + (*it)->getHGridLength();
      (*it)->show();
    }
  }
  for( std::vector<OneThreeStraightItem *>::iterator it = m_paddingItems.begin();
       it < m_paddingItems.end(); ++it )
  {
    if( m_isDropZone )
    {
      i = i + (*it)->getHGridLength();
      (*it)->hide();
    }
    else
    {
      (*it)->setGridPos( i, 0 );
      i = i + (*it)->getHGridLength();
      (*it)->show();
    }
  }

  setHGridLength( i );
}

void HorizontalBranchItem::setHasTwoRightSidePipes( bool dualRight )
{
  m_hasDualTwoRightSidePipes = dualRight;
}

void HorizontalBranchItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{ 
  if( m_isDropZone )
  {
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
    painter->drawRect(0,0,m_hLength * 100,m_vLength * 100);

    GridItem::paint(painter,option,widget);

    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QColor(128,128,128),2,Qt::DashLine, Qt::RoundCap));
    painter->drawRoundedRect(7,7,m_hLength * 100 - 14,m_vLength * 100 - 14,5.0,5.0);

    painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::FlatCap));
    painter->drawLine(0,m_vLength * 100 / 2.0,7,m_vLength * 100 / 2.0);
    if( m_hasDualTwoRightSidePipes )
    {
      painter->drawLine(m_hLength * 100 - 7,m_vLength * 100 / 3.0,m_hLength * 100 ,m_vLength * 100 / 3.0);
      painter->drawLine(m_hLength * 100 - 7,m_vLength * 100 * 2.0 / 3.0,m_hLength * 100 ,m_vLength * 100 * 2.0 / 3.0);
    }
    else
    {
      painter->drawLine(m_hLength * 100 - 7,m_vLength * 100 / 2.0,m_hLength * 100 ,m_vLength * 100 / 2.0);
    }

    painter->drawText(9,9,m_hLength * 100 - 18,m_vLength * 100 - 18,Qt::AlignCenter | Qt::TextWordWrap,m_text);
  }
}

void HorizontalBranchItem::setText( const QString & text )
{
  m_text = text;

  update();
}

VerticalBranchItem::VerticalBranchItem( std::vector<model::ModelObject> modelObjects,
                                        QGraphicsItem * parent )
  : GridItem( parent )
{
  for( std::vector<model::ModelObject>::iterator it = modelObjects.begin();
       it < modelObjects.end(); ++it )
  {
    if(model::OptionalNode comp = it->optionalCast<model::Node>())
    {
      GridItem * gridItem = new TwoFourNodeItem(this); 
      gridItem->setModelObject( comp->optionalCast<model::ModelObject>() );
      if( comp->isRemovable() )
      {
        gridItem->setDeletable(true);
      }
      m_gridItems.push_back(gridItem);
    }
    else if(model::OptionalStraightComponent comp = it->optionalCast<model::StraightComponent>())
    {
      GridItem * gridItem = new TwoFourStraightItem(this); 
      gridItem->setModelObject( comp->optionalCast<model::ModelObject>() );
      if( comp->isRemovable() )
      {
        gridItem->setDeletable(true);
      }
      m_gridItems.push_back(gridItem);
    }
  }

  layout();
}

void VerticalBranchItem::setPadding( unsigned padding )
{
  if( m_paddingItems.size() > padding )
  {
    for( std::vector<TwoFourStraightItem *>::iterator it = m_paddingItems.begin() + padding;
         it < m_paddingItems.end(); 
         ++it )
    {
      (*it)->scene()->removeItem(*it);
    } 
    m_paddingItems.erase(m_paddingItems.begin() + padding, m_paddingItems.end());
  }
  else if( m_paddingItems.size() < padding)
  {
    for( unsigned i = m_paddingItems.size(); i < padding; i++ )
    {
      TwoFourStraightItem * straightItem = new TwoFourStraightItem();
      straightItem->setParentItem(this);
      m_paddingItems.push_back( straightItem );
    }
  }

  layout();
}

void VerticalBranchItem::layout()
{
  int j = 0;
  for( std::vector<GridItem *>::iterator it = m_gridItems.begin();
       it < m_gridItems.end(); ++it )
  {
    (*it)->setGridPos( 0, j );
    j = j + (*it)->getVGridLength();
  }
  for( std::vector<TwoFourStraightItem *>::iterator it = m_paddingItems.begin();
       it < m_paddingItems.end(); ++it )
  {
    (*it)->setGridPos( 0, j );
    j = j + (*it)->getVGridLength();
  }
  setVGridLength( j );
}

void VerticalBranchItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{ 
}

ReverseVerticalBranchItem::ReverseVerticalBranchItem( std::vector<model::ModelObject> modelObjects,
                                        QGraphicsItem * parent )
  : GridItem(parent)
{
  for( std::vector<model::ModelObject>::iterator it = modelObjects.begin();
       it < modelObjects.end(); ++it )
  {
    if(model::OptionalNode comp = it->optionalCast<model::Node>())
    {
      GridItem * gridItem = new TwoFourNodeItem(this); 
      gridItem->setModelObject( comp->optionalCast<model::ModelObject>() );
      if( comp->isRemovable() )
      {
        gridItem->setDeletable(true);
      }
      m_gridItems.push_back(gridItem);
    }
    else if(model::OptionalStraightComponent comp = it->optionalCast<model::StraightComponent>())
    {
      GridItem * gridItem = new FourTwoStraightItem(this); 
      gridItem->setModelObject( comp->optionalCast<model::ModelObject>() );
      if( comp->isRemovable() )
      {
        gridItem->setDeletable(true);
      }
      m_gridItems.push_back(gridItem);
    }
  }

  layout();
}

void ReverseVerticalBranchItem::setPadding( unsigned padding )
{
  if( m_paddingItems.size() > padding )
  {
    for( std::vector<FourTwoStraightItem *>::iterator it = m_paddingItems.begin() + padding;
         it < m_paddingItems.end(); 
         ++it )
    {
      (*it)->scene()->removeItem(*it);
    } 
    m_paddingItems.erase(m_paddingItems.begin() + padding, m_paddingItems.end());
  }
  else if( m_paddingItems.size() < padding)
  {
    for( unsigned i = m_paddingItems.size(); i < padding; i++ )
    {
      FourTwoStraightItem * straightItem = new FourTwoStraightItem();
      straightItem->setParentItem(this);
      m_paddingItems.push_back( straightItem );
    }
  }

  layout();
}

void ReverseVerticalBranchItem::layout()
{
  int j = 0;
  for( std::vector<FourTwoStraightItem *>::iterator it = m_paddingItems.begin();
       it < m_paddingItems.end(); ++it )
  {
    (*it)->setGridPos( 0, j );
    j = j + (*it)->getVGridLength();
  }

  for( std::vector<GridItem *>::reverse_iterator it = m_gridItems.rbegin();
       it < m_gridItems.rend(); ++it )
  {
    (*it)->setGridPos( 0, j );
    j = j + (*it)->getVGridLength();
  }
  setVGridLength( j );
}

bool sortBranches(std::vector<ModelObject> i, std::vector<ModelObject> j)
{
  OS_ASSERT(! i.empty());
  OS_ASSERT(! j.empty());

  std::vector<ThermalZone> iZones = subsetCastVector<ThermalZone>(i);
  std::vector<ThermalZone> jZones = subsetCastVector<ThermalZone>(j);

  boost::optional<ModelObject> iModelObject;
  boost::optional<ModelObject> jModelObject;

  if( ! iZones.empty() )
  {
    iModelObject = iZones.front();
  }
  else if( i.size() > 1u )
  {
    iModelObject = i[1];
  }
  else
  {
    iModelObject = i[0];
  }

  if( ! jZones.empty() )
  {
    jModelObject = jZones.front();
  }
  else if( j.size() > 1u )
  {
    jModelObject = j[1];
  }
  else
  {
    jModelObject = j[0];
  }

  OS_ASSERT(iModelObject);
  OS_ASSERT(jModelObject);

  return iModelObject->name().get() < jModelObject->name().get();
}

HorizontalBranchGroupItem::HorizontalBranchGroupItem( model::Splitter & splitter,
                                                      model::Mixer & mixer,
                                                      QGraphicsItem * parent ) 
  : GridItem(parent),
    m_splitter(splitter),
    m_mixer(mixer),
    m_dropZoneBranchItem(NULL)
{
  boost::optional<model::Loop> optionalLoop = splitter.loop();
  OS_ASSERT( optionalLoop );
  model::Loop loop = optionalLoop.get(); 

  std::vector<model::ModelObject> splitterOutletObjects = splitter.outletModelObjects();
  bool isSupplySide = loop.supplyComponent(splitter.handle());

  std::vector<model::ModelObject> branchComponents;
  std::vector< std::vector<model::ModelObject> > allBranchComponents;

  if( ! (splitterOutletObjects.front() == mixer) )
  {
    for( std::vector<model::ModelObject>::iterator it1 = splitterOutletObjects.begin();
         it1 != splitterOutletObjects.end();
         ++it1 )
    {
      bool isSupplyPlenum = false;
      if( boost::optional<model::Node> node = it1->optionalCast<model::Node>() )
      {
        boost::optional<model::ModelObject> outletMo = node->outletModelObject();
        OS_ASSERT(outletMo);
        if(boost::optional<model::Splitter> plenumSplitter = outletMo->optionalCast<model::Splitter>())
        {
          isSupplyPlenum = true;
          std::vector<model::ModelObject> plenumOutletObjects = plenumSplitter->outletModelObjects();
          for( std::vector<model::ModelObject>::iterator it2 = plenumOutletObjects.begin();
               it2 != plenumOutletObjects.end();
               ++it2 )
          {
            boost::optional<model::HVACComponent> comp1 = it2->optionalCast<model::HVACComponent>();
            OS_ASSERT(comp1);
            branchComponents = loop.components(comp1.get(),mixer);
            branchComponents.erase(branchComponents.end() - 1);
            branchComponents.insert(branchComponents.begin(),plenumSplitter.get());
            branchComponents.insert(branchComponents.begin(),*it1);

            std::vector<model::ModelObject> rBranchComponents;
            for( std::vector<model::ModelObject>::reverse_iterator rit = branchComponents.rbegin();
                 rit < branchComponents.rend(); ++rit )
            {
              rBranchComponents.push_back( *rit );
            }
            allBranchComponents.push_back(rBranchComponents);
          }
        }
      }
      if( ! isSupplyPlenum )
      {
        boost::optional<model::HVACComponent> comp1 = it1->optionalCast<model::HVACComponent>();
        OS_ASSERT(comp1);
        branchComponents = loop.components(comp1.get(),mixer);
        branchComponents.erase(branchComponents.end() - 1);

        if( isSupplySide )
        {
          allBranchComponents.push_back(branchComponents);
        }
        else
        {
          std::vector<model::ModelObject> rBranchComponents;
          for( std::vector<model::ModelObject>::reverse_iterator rit = branchComponents.rbegin();
               rit < branchComponents.rend(); ++rit )
          {
            rBranchComponents.push_back( *rit );
          }
          allBranchComponents.push_back(rBranchComponents);
        }
      }
    }
  }

  std::sort(allBranchComponents.begin(),allBranchComponents.end(),sortBranches);
  for(std::vector< std::vector<ModelObject> >::iterator it = allBranchComponents.begin();
      it != allBranchComponents.end();
      ++it)
  {
    m_branchItems.push_back(new HorizontalBranchItem(*it,this));
  }

  layout();
}

unsigned HorizontalBranchGroupItem::numberOfBranches() const
{
  return m_branchItems.size();  
}

void HorizontalBranchGroupItem::setShowDropZone(bool showDropZone)
{
  if( m_dropZoneBranchItem )
  {
    std::vector<HorizontalBranchItem *>::iterator it =
      std::find(m_branchItems.begin(),m_branchItems.end(),m_dropZoneBranchItem);

    if( it != m_branchItems.end() )
    {
      m_branchItems.erase(it);

      m_dropZoneBranchItem = NULL;
    }
  }

  if( showDropZone )
  {
    m_dropZoneBranchItem = new HorizontalBranchItem(std::vector<model::ModelObject>(),this);
    m_dropZoneBranchItem->setIsDropZone(true);
    m_branchItems.push_back(m_dropZoneBranchItem);

    m_dropZoneBranchItem->setModelObject(m_splitter);
  }

  layout();
}

void HorizontalBranchGroupItem::layout()
{
  if( m_branchItems.size() == 0 )
  {
    HorizontalBranchItem * branchItem = new HorizontalBranchItem(std::vector<model::ModelObject>(),this);

    branchItem->setPadding(3);
    
    m_branchItems.push_back(branchItem);
  }

  int longestBranch = 0;
  for( std::vector<HorizontalBranchItem *>::iterator it = m_branchItems.begin();
       it < m_branchItems.end(); ++it )
  {
    if( (*it)->getHGridLength() > longestBranch )
    {
      longestBranch = (*it)->getHGridLength();
    }
  } 

  setHGridLength( longestBranch );

  for( std::vector<HorizontalBranchItem *>::iterator it = m_branchItems.begin();
       it < m_branchItems.end(); ++it )
  {
    unsigned padding = longestBranch - (*it)->getHGridLength(); 
    if( padding > 0 )
    {
      (*it)->setPadding( padding );
    }
  } 

  int j = 0;
  int lastj = 0;
  for( std::vector<HorizontalBranchItem *>::iterator it = m_branchItems.begin();
       it < m_branchItems.end(); ++it )
  {
    (*it)->setGridPos(0,j);
    lastj = j + (*it)->getVGridLength();
    j = lastj + 1;
  }
  
  if( lastj == 0 )
  {
    lastj = 1;
  }

  setVGridLength(lastj);
}

void HorizontalBranchGroupItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{ 
}

SystemItem::SystemItem( model::Loop loop, LoopScene * loopScene )
  : GridItem(),
    m_loop(loop),
    m_loopScene(loopScene)
{
  boost::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();
  boost::shared_ptr<MainRightColumnController> mrc = doc->mainRightColumnController(); 
  mrc->registerSystemItem(m_loop.handle(),this);

  m_loopScene->addItem(this);

  model::Node supplyInletNode = m_loop.supplyInletNode();
  model::Node supplyOutletNode = m_loop.supplyOutletNode();

  std::vector<model::AirLoopHVACSupplyPlenum> supplyPlenums = subsetCastVector<model::AirLoopHVACSupplyPlenum>(loop.demandComponents());
  std::vector<model::AirLoopHVACReturnPlenum> returnPlenums = subsetCastVector<model::AirLoopHVACReturnPlenum>(loop.demandComponents());

  int i = 0;

  for(std::vector<model::AirLoopHVACSupplyPlenum>::iterator it = supplyPlenums.begin();
      it != supplyPlenums.end();
      ++it)
  {
    if( boost::optional<model::ThermalZone> tz = it->thermalZone() )
    {
      if( boost::optional<model::RenderingColor> rc = tz->renderingColor() )
      {
        QColor color;
        color.setRed(rc->renderingRedValue());
        color.setBlue(rc->renderingBlueValue());
        color.setGreen(rc->renderingGreenValue());
        m_plenumColorMap.insert(std::make_pair(it->handle(),color));
      }
    }
    m_plenumIndexMap.insert(std::make_pair(it->handle(),i));
    i++;
  }

  for(std::vector<model::AirLoopHVACReturnPlenum>::iterator it = returnPlenums.begin();
      it != returnPlenums.end();
      ++it)
  {
    if( boost::optional<model::ThermalZone> tz = it->thermalZone() )
    {
      if( boost::optional<model::RenderingColor> rc = tz->renderingColor() )
      {
        QColor color;
        color.setRed(rc->renderingRedValue());
        color.setBlue(rc->renderingBlueValue());
        color.setGreen(rc->renderingGreenValue());
        m_plenumColorMap.insert(std::make_pair(it->handle(),color));
      }
    }
    m_plenumIndexMap.insert(std::make_pair(it->handle(),i));
    i++;
  }

  m_supplySideItem = new SupplySideItem( this,
                                         supplyInletNode,
                                         supplyOutletNode);

  m_supplySideItem->setGridPos(0,0);

  model::Node demandInletNode = m_loop.demandInletNode();
  model::Node demandOutletNode = m_loop.demandOutletNode();

  m_demandSideItem = new DemandSideItem( this,
                                         demandInletNode,
                                         demandOutletNode );

  m_demandSideItem->setGridPos(0,m_supplySideItem->getVGridLength() + 1);
  
  int defaultDemandPadding = 0;
  int defaultSupplyPadding = 0;

  m_supplySideItem->setPadding(defaultSupplyPadding);
  m_demandSideItem->setPadding(defaultDemandPadding);

  int supplyHGridLength = m_supplySideItem->getHGridLength();
  int demandHGridLength = m_demandSideItem->getHGridLength();

  int diff  = supplyHGridLength - demandHGridLength;

  if( diff >= 0 )
  {
    m_demandSideItem->setPadding( diff + defaultDemandPadding );
  }
  else if( diff < 0 )
  {
    m_supplySideItem->setPadding( (diff * -1) + defaultSupplyPadding );
  }

  m_systemCenterItem = new SystemCenterItem(this,m_loop);

  m_systemCenterItem->setHGridLength(m_supplySideItem->getHGridLength());

  m_systemCenterItem->setGridPos(0,m_supplySideItem->getVGridLength());

  setVGridLength( m_supplySideItem->getVGridLength() + m_demandSideItem->getVGridLength() + m_systemCenterItem->getVGridLength() );

  setHGridLength( m_supplySideItem->getHGridLength() );
}

SystemItem::~SystemItem()
{
  boost::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();
  boost::shared_ptr<MainRightColumnController> mrc = doc->mainRightColumnController(); 
  mrc->unregisterSystemItem(m_loop.handle());
}

int SystemItem::plenumIndex(const Handle & plenumHandle)
{
  std::map<Handle,int>::iterator it = m_plenumIndexMap.find(plenumHandle);
  if( it != m_plenumIndexMap.end() )
  {
    return it->second;
  }
  else
  {
    return -1;
  }
}

QColor SystemItem::plenumColor(const Handle & plenumHandle)
{
  QColor color;

  std::map<Handle,QColor>::iterator it = m_plenumColorMap.find(plenumHandle);
  if( it != m_plenumColorMap.end() )
  {
    color = it->second;
  }
  else
  {
    int index = plenumIndex(plenumHandle);
    if( index < 0 )
    {
      color = SchedulesView::colors[0];
    }
    else if( index > 12 )
    {
      color = SchedulesView::colors[12];
    }
    else
    {
      color = SchedulesView::colors[index];
    }

    // DLM: Create a RenderingColor and associate it with the thermal zone?
  }

  return color;
}

void SystemItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
} 

SystemCenterItem::SystemCenterItem( QGraphicsItem * parent, model::Loop loop )
  : GridItem(parent)
{
  this->setModelObject(loop);
}

void SystemCenterItem::paint( QPainter *painter, 
                              const QStyleOptionGraphicsItem *option, 
                              QWidget *widget )
{
  int yOrigin = 0;

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::NoPen, Qt::RoundCap));
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->drawRect(0,yOrigin,100,100);
  painter->drawRect((m_hLength - 1) * 100,yOrigin,100,100);

  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->drawLine(50,yOrigin,50,yOrigin + 100);
  painter->drawLine( (m_hLength - 1) * 100 + 50,
                     yOrigin,
                     (m_hLength - 1) * 100 + 50,
                     yOrigin + 100
                     );

  painter->setPen(QPen(Qt::black,1,Qt::DashLine, Qt::RoundCap));
  painter->drawLine( 0,yOrigin + 50,(m_hLength) * 100,yOrigin + 50 );

  painter->drawPixmap((m_hLength - 1) * 100 + 37.5,yOrigin + 25,25,25,QPixmap(":/images/arrow.png"));

  painter->rotate(180);
  painter->drawPixmap(-62,-(yOrigin + 75),25,25,QPixmap(":/images/arrow.png"));

  painter->rotate(-180);

  if(m_highlight)
  {
    painter->setPen(QPen(Qt::black,4,Qt::NoPen, Qt::RoundCap));
    painter->setBrush(QBrush(QColor(127, 127, 127, 127)));
    painter->drawRect(0,yOrigin,m_hLength * 100,100);
  }

  QFont f;
  f.setPixelSize(20);
  painter->setFont(f);
  painter->setPen(QPen(Qt::black,1,Qt::SolidLine, Qt::RoundCap));
  painter->setBrush(QBrush(Qt::black,Qt::SolidPattern));
  painter->drawText(QRect(110,21,200,25),Qt::AlignBottom,"Supply Equipment");
  painter->drawText(QRect(110,52,200,25),Qt::AlignTop,"Demand Equipment");
}

SupplyPlenumItem::SupplyPlenumItem(const model::ModelObject & mo, QGraphicsItem * parent)
  : GridItem(parent)
{
  setModelObject(mo);

  // HorizontalBranchItem -> BranchGroupItem -> DemandSideItem -> SystemItem
  m_color = static_cast<SystemItem *>(parentItem()->parentItem()->parentItem()->parentItem())->plenumColor(mo.handle());
}

void SupplyPlenumItem::setModelObject( model::OptionalModelObject modelObject )
{
  if( modelObject )
  {
    boost::optional<model::AirLoopHVACSupplyPlenum> plenum = modelObject->optionalCast<model::AirLoopHVACSupplyPlenum>();
    OS_ASSERT(plenum);

    GridItem::setModelObject(modelObject);

    if( boost::optional<model::ThermalZone> tz = plenum->thermalZone() )
    {
      setToolTip(QString::fromStdString(tz->name().get()));
    }
  }
}

void SupplyPlenumItem::paint(QPainter *painter, 
                             const QStyleOptionGraphicsItem *option, 
                             QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->setBrush(QBrush(m_color,Qt::SolidPattern));

  painter->drawLine(0,50,25,50);
  painter->drawLine(75,50,100,50);

  QPointF points[4] = {
    QPointF(25,25),
    QPointF(75,40),
    QPointF(75,60),
    QPointF(25,75),
  };
  painter->drawPolygon(points,4);
}

ReturnPlenumItem::ReturnPlenumItem(const model::ModelObject & mo, QGraphicsItem * parent)
  : GridItem(parent)
{
  setModelObject(mo);

  // HorizontalBranchItem -> BranchGroupItem -> DemandSideItem -> SystemItem
  m_color = static_cast<SystemItem *>(parentItem()->parentItem()->parentItem()->parentItem())->plenumColor(mo.handle());
}

void ReturnPlenumItem::setModelObject( model::OptionalModelObject modelObject )
{
  if( modelObject )
  {
    boost::optional<model::AirLoopHVACReturnPlenum> plenum = modelObject->optionalCast<model::AirLoopHVACReturnPlenum>();
    OS_ASSERT(plenum);

    GridItem::setModelObject(modelObject);

    if( boost::optional<model::ThermalZone> tz = plenum->thermalZone() )
    {
      setToolTip(QString::fromStdString(tz->name().get()));
    }
  }
}

void ReturnPlenumItem::paint(QPainter *painter, 
                             const QStyleOptionGraphicsItem *option, 
                             QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->setBrush(QBrush(m_color,Qt::SolidPattern));

  painter->drawLine(0,50,25,50);
  painter->drawLine(75,50,100,50);

  QPointF points[4] = {
    QPointF(25,40),
    QPointF(75,25),
    QPointF(75,75),
    QPointF(25,60),
  };
  painter->drawPolygon(points,4);
}

OneThreeStraightItem::OneThreeStraightItem( QGraphicsItem * parent )
  : GridItem(parent)
{
}

void OneThreeStraightItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->drawLine(0,50,100,50);

  if( modelObject() )
  {
    const QPixmap * qPixmap = IconLibrary::Instance().findIcon( modelObject()->iddObject().type().value() );
    painter->drawPixmap(12,12,75,75,*qPixmap);

    //if(m_deleteAble)
    //{
    //  painter->drawPixmap(70,70,25,25,QPixmap(":/images/delete-icon.png"));
    //}
  }
}

void OneThreeStraightItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
  //if(QRect(70,70,25,25).contains(event->pos().toPoint()))
  //{
  //  if(m_modelObject)
  //  {
  //    event->accept();
  //    
  //    QApplication::instance()->processEvents();

  //    emit removeModelObjectClicked( m_modelObject.get() );
  //  }
  //}
  //else
  //{
    GridItem::mouseReleaseEvent(event);
  //}
}

OneThreeWaterToAirItem::OneThreeWaterToAirItem( QGraphicsItem * parent )
  : GridItem( parent )
{
}

void OneThreeWaterToAirItem::onLinkItemClicked()
{
  if( m_modelObject )
  {
    emit innerNodeClicked( m_modelObject.get() );
  }
}

void OneThreeWaterToAirItem::setModelObject( model::OptionalModelObject modelObject )
{
  GridItem::setModelObject(modelObject);

  m_showLinks = false;

  if( m_modelObject )
  {
    if( boost::optional<WaterToAirComponent> waterToAirComponent = m_modelObject->optionalCast<WaterToAirComponent>() )
    {
      if( waterToAirComponent->airLoopHVAC() && waterToAirComponent->plantLoop() )
      {
        LinkItem * linkItem1 = new LinkItem(this);
        linkItem1->setPos(40,5); 
        connect(linkItem1,SIGNAL(mouseClicked()),this,SLOT(onLinkItemClicked()));
        
        LinkItem * linkItem2 = new LinkItem(this);
        linkItem2->setPos(40,75); 
        connect(linkItem2,SIGNAL(mouseClicked()),this,SLOT(onLinkItemClicked()));

        m_showLinks = true;
      }
    }
  }
}

void OneThreeWaterToAirItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::NoPen, Qt::RoundCap));
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->drawLine(0,50,100,50);

  if( m_showLinks )
  {
    painter->drawLine(50,15,50,85);
  }

  if( modelObject() )
  {
    const QPixmap * qPixmap = IconLibrary::Instance().findIcon( modelObject()->iddObject().type().value() );
    painter->drawPixmap(12,12,75,75,*qPixmap);

    //if(m_deleteAble)
    //{
    //  painter->drawPixmap(70,70,25,25,QPixmap(":/images/delete-icon.png"));
    //}
  }
}

void OneThreeWaterToAirItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
  //if(QRect(70,70,25,25).contains(event->pos().toPoint()))
  //{
  //  if(m_modelObject)
  //  {
  //    event->accept();
  //    
  //    QApplication::instance()->processEvents();

  //    emit removeModelObjectClicked( m_modelObject.get() );
  //  }
  //}
  //else
  //{
    GridItem::mouseReleaseEvent(event);
  //}
}

OneThreeWaterToWaterItem::OneThreeWaterToWaterItem( QGraphicsItem * parent )
  : GridItem( parent )
{
}

void OneThreeWaterToWaterItem::onLinkItemClicked()
{
  if( m_modelObject )
  {
    emit innerNodeClicked( m_modelObject.get() );
  }
}

void OneThreeWaterToWaterItem::setModelObject( model::OptionalModelObject modelObject )
{
  GridItem::setModelObject(modelObject);

  m_showLinks = false;

  if( m_modelObject )
  {
    if( boost::optional<WaterToWaterComponent> waterToWaterComponent = m_modelObject->optionalCast<WaterToWaterComponent>() )
    {
      if( waterToWaterComponent->plantLoop() && waterToWaterComponent->secondaryPlantLoop() )
      {
        LinkItem * linkItem1 = new LinkItem(this);
        linkItem1->setPos(40,5); 
        connect(linkItem1,SIGNAL(mouseClicked()),this,SLOT(onLinkItemClicked()));
        
        LinkItem * linkItem2 = new LinkItem(this);
        linkItem2->setPos(40,75); 
        connect(linkItem2,SIGNAL(mouseClicked()),this,SLOT(onLinkItemClicked()));

        m_showLinks = true;
      }
    }
  }
}

void OneThreeWaterToWaterItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::NoPen, Qt::RoundCap));
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->drawLine(0,50,100,50);

  if( m_showLinks )
  {
    painter->drawLine(50,15,50,85);
  }

  if( modelObject() )
  {
    const QPixmap * qPixmap = IconLibrary::Instance().findIcon( modelObject()->iddObject().type().value() );
    painter->drawPixmap(12,12,75,75,*qPixmap);

    //if(m_deleteAble)
    //{
    //  painter->drawPixmap(70,70,25,25,QPixmap(":/images/delete-icon.png"));
    //}
  }
}

void OneThreeWaterToWaterItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
  //if(QRect(70,70,25,25).contains(event->pos().toPoint()))
  //{
  //  if(m_modelObject)
  //  {
  //    event->accept();
  //    
  //    QApplication::instance()->processEvents();

  //    emit removeModelObjectClicked( m_modelObject.get() );
  //  }
  //}
  //else
  //{
    GridItem::mouseReleaseEvent(event);
  //}
}

TwoFourStraightItem::TwoFourStraightItem( QGraphicsItem * parent )
  : GridItem( parent )
{
}

void TwoFourStraightItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->drawLine(50,0,50,100);

  if( modelObject() )
  {
    painter->translate(100,0);
    painter->rotate(90);

    const QPixmap * qPixmap = IconLibrary::Instance().findIcon( modelObject()->iddObject().type().value() );
    painter->drawPixmap(12,12,75,75,*qPixmap);

    painter->rotate(-90);
    painter->translate(-100,0);

    //if(m_deleteAble)
    //{
    //  painter->drawPixmap(70,70,25,25,QPixmap(":/images/delete-icon.png"));
    //}
  }
}

OASupplyStraightItem::OASupplyStraightItem( QGraphicsItem * parent )
  : GridItem( parent )
{
}

void OASupplyStraightItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  painter->setPen(QPen(Qt::black,4,Qt::NoPen, Qt::RoundCap));
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->drawRect(5,0,90,100);

  if(isSelected())
  {
    painter->setBrush(QBrush(QColor(128,128,128),Qt::SolidPattern));
    painter->drawRect(5,0,90,100);
  }

  if(m_highlight)
  {
    painter->setPen(QPen(Qt::black,4,Qt::NoPen, Qt::RoundCap));
    painter->setBrush(QBrush(QColor(127, 127, 127, 127)));
    painter->drawRect(5,0,90,100);
  }

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->drawLine(50,0,50,100);

  if( modelObject() )
  {
    painter->translate(100,0);
    painter->rotate(90);

    const QPixmap * qPixmap = IconLibrary::Instance().findIcon( modelObject()->iddObject().type().value() );
    painter->drawPixmap(12,12,75,75,*qPixmap);

    painter->rotate(-90);
    painter->translate(-100,0);

    //if(m_deleteAble)
    //{
    //  painter->drawPixmap(68,70,25,25,QPixmap(":/images/delete-icon.png"));
    //}
  }
}

OAReliefStraightItem::OAReliefStraightItem( QGraphicsItem * parent )
  : GridItem( parent )
{
}

void OASupplyStraightItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
  //if(QRect(68,70,25,25).contains(event->pos().toPoint()))
  //{
  //  if(m_modelObject)
  //  {
  //    event->accept();
  //    
  //    QApplication::instance()->processEvents();

  //    emit removeModelObjectClicked( m_modelObject.get() );
  //  }
  //}
  //else
  //{
    GridItem::mouseReleaseEvent(event);
  //}
}

void OAReliefStraightItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  painter->setPen(QPen(Qt::black,4,Qt::NoPen, Qt::RoundCap));
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->drawRect(5,0,90,100);

  if(m_highlight)
  {
    painter->setPen(QPen(Qt::black,4,Qt::NoPen, Qt::RoundCap));
    painter->setBrush(QBrush(QColor(127, 127, 127, 127)));
    painter->drawRect(5,0,90,100);
  }

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->drawLine(50,0,50,100);

  if( modelObject() )
  {
    painter->translate(0,100);
    painter->rotate(-90);

    const QPixmap * qPixmap = IconLibrary::Instance().findIcon( modelObject()->iddObject().type().value() );
    painter->drawPixmap(12,12,75,75,*qPixmap);

    painter->rotate(90);
    painter->translate(0,-100);

    //if(m_deleteAble)
    //{
    //  painter->drawPixmap(68,70,25,25,QPixmap(":/images/delete-icon.png"));
    //}
  }
}

void OAReliefStraightItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
  //if(QRect(68,70,25,25).contains(event->pos().toPoint()))
  //{
  //  if(m_modelObject)
  //  {
  //    event->accept();
  //    
  //    QApplication::instance()->processEvents();

  //    emit removeModelObjectClicked( m_modelObject.get() );
  //  }
  //}
  //else
  //{
    GridItem::mouseReleaseEvent(event);
  //}
}

OAAirToAirItem::OAAirToAirItem(QGraphicsItem * parent )
  : GridItem(parent)
{
  setHGridLength(2);
  setVGridLength(1);
}

void OAAirToAirItem::paint(QPainter *painter, 
                           const QStyleOptionGraphicsItem *option, 
                           QWidget *widget)
{
  painter->setPen(QPen(Qt::black,4,Qt::NoPen, Qt::RoundCap));
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->drawRect(5,0,m_hLength * 100 - 10,m_vLength * 100);

  if(isSelected())
  {
    painter->setBrush(QBrush(QColor(128,128,128),Qt::SolidPattern));
    painter->drawRect(5,0,m_hLength * 100 - 10,m_vLength * 100);
  }

  if(m_highlight && m_enableHighlight)
  {
    painter->setPen(QPen(Qt::black,4,Qt::NoPen, Qt::RoundCap));
    painter->setBrush(QBrush(QColor(127, 127, 127, 127)));
    painter->drawRect(5,0,m_hLength * 100 - 10,m_vLength * 100);
  }

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->drawLine(50,0,50,100);
  painter->drawLine(150,0,150,100);

  if( modelObject() )
  {
    const QPixmap * qPixmap = IconLibrary::Instance().findIcon( modelObject()->iddObject().type().value() );
    painter->drawPixmap(0,0,200,100,*qPixmap);
  }
}

OASupplyBranchItem::OASupplyBranchItem( std::vector<model::ModelObject> supplyModelObjects,
                                        std::vector<model::ModelObject> reliefModelObjects,
                                        QGraphicsItem * parent )
  : GridItem( parent )
{
  setAcceptHoverEvents(false);

  std::vector<model::ModelObject>::reverse_iterator it2 = reliefModelObjects.rbegin();

  for( std::vector<model::ModelObject>::iterator it = supplyModelObjects.begin();
       it < supplyModelObjects.end(); ++it )
  {
    if(model::OptionalNode comp = it->optionalCast<model::Node>())
    {
      GridItem * gridItem = new OAStraightNodeItem(this); 
      gridItem->setModelObject( comp->optionalCast<model::ModelObject>() );
      if( comp->isRemovable() )
      {
        gridItem->setDeletable(true);
      }
      m_gridItems.push_back(gridItem);
    }
    else if(model::OptionalStraightComponent comp = it->optionalCast<model::StraightComponent>())
    {
      GridItem * gridItem = new OASupplyStraightItem(this); 
      gridItem->setModelObject( comp->optionalCast<model::ModelObject>() );
      if( comp->isRemovable() )
      {
        gridItem->setDeletable(true);
      }
      m_gridItems.push_back(gridItem);
    }
    else if(boost::optional<model::AirToAirComponent> comp = it->optionalCast<model::AirToAirComponent>())
    {
      while( it2 != reliefModelObjects.rend() )
      {
        if( boost::optional<model::AirToAirComponent> comp2 = it2->optionalCast<model::AirToAirComponent>() )
        {
          break;
        }
        else
        {
          GridItem * gridItem = new OASupplyStraightItem(this); 
          m_gridItems.push_back(gridItem);
          ++it2;
        }
      }
      m_gridItems.push_back(NULL);
    }

    if( it2 != reliefModelObjects.rend() )
    {
      ++it2;
    }
  }

  while( it2 != reliefModelObjects.rend() )
  {
    GridItem * gridItem = new OASupplyStraightItem(this); 
    m_gridItems.insert(m_gridItems.begin(),gridItem);
    ++it2;
  }

  layout();
}

void OASupplyBranchItem::layout()
{
  int j = 0;
  for( std::vector<GridItem *>::iterator it = m_gridItems.begin();
       it < m_gridItems.end(); ++it )
  {
    if( *it )
    {
      (*it)->setGridPos( 0, j );
      j = j + (*it)->getVGridLength();
    }
    else
    {
      j = j + 1;
    }
  }
  setVGridLength( j );
}

void OASupplyBranchItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{ 
}

OAReliefBranchItem::OAReliefBranchItem( std::vector<model::ModelObject> reliefModelObjects,
                                        std::vector<model::ModelObject> supplyModelObjects,
                                        QGraphicsItem * parent )
  : GridItem( parent )
{
  setAcceptHoverEvents(false);

  std::vector<model::ModelObject>::reverse_iterator it2 = supplyModelObjects.rbegin();

  for( std::vector<model::ModelObject>::iterator it = reliefModelObjects.begin();
       it < reliefModelObjects.end(); ++it )
  {
    if(model::OptionalNode comp = it->optionalCast<model::Node>())
    {
      GridItem * gridItem = new OAStraightNodeItem(this); 
      gridItem->setModelObject( comp->optionalCast<model::ModelObject>() );
      if( comp->isRemovable() )
      {
        gridItem->setDeletable(true);
      }
      m_gridItems.push_back(gridItem);
    }
    else if(model::OptionalStraightComponent comp = it->optionalCast<model::StraightComponent>())
    {
      GridItem * gridItem = new OAReliefStraightItem(this); 
      gridItem->setModelObject( comp->optionalCast<model::ModelObject>() );
      if( comp->isRemovable() )
      {
        gridItem->setDeletable(true);
      }
      m_gridItems.push_back(gridItem);
    }
    else if(boost::optional<model::AirToAirComponent> comp = it->optionalCast<model::AirToAirComponent>())
    {
      while( it2 != supplyModelObjects.rend() )
      {
        if( boost::optional<model::AirToAirComponent> comp2 = it2->optionalCast<model::AirToAirComponent>() )
        {
          break;
        }
        else
        {
          GridItem * gridItem = new OAReliefStraightItem(this); 
          m_gridItems.push_back(gridItem);
          ++it2;
        }
      }
      GridItem * gridItem = new OAAirToAirItem(this); 
      gridItem->setModelObject( comp->optionalCast<model::ModelObject>() );
      if( comp->isRemovable() )
      {
        gridItem->setDeletable(true);
      }
      m_gridItems.push_back(gridItem);
    }

    if( it2 != supplyModelObjects.rend() )
    {
      ++it2;
    }
  }

  while( it2 != supplyModelObjects.rend() )
  {
    GridItem * gridItem = new OAReliefStraightItem(this); 
    m_gridItems.push_back(gridItem);
    ++it2;
  }

  layout();
}

void OAReliefBranchItem::layout()
{
  int j = 0;
  for( std::vector<GridItem *>::reverse_iterator it = m_gridItems.rbegin();
       it < m_gridItems.rend(); ++it )
  {
    (*it)->setGridPos( 0, j );
    j = j + (*it)->getVGridLength();
  }
  setVGridLength( j );
}

void OAReliefBranchItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{ 
}

FourTwoStraightItem::FourTwoStraightItem( QGraphicsItem * parent )
  : GridItem( parent )
{
}

void FourTwoStraightItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->drawLine(50,0,50,100);

  if( modelObject() )
  {
    painter->translate(0,100);
    painter->rotate(-90);

    const QPixmap * qPixmap = IconLibrary::Instance().findIcon( modelObject()->iddObject().type().value() );
    painter->drawPixmap(12,12,75,75,*qPixmap);

    painter->rotate(90);
    painter->translate(0,-100);

    //if(m_deleteAble)
    //{
    //  painter->drawPixmap(70,70,25,25,QPixmap(":/images/delete-icon.png"));
    //}
  }
}

OneFourStraightItem::OneFourStraightItem( QGraphicsItem * parent )
  : GridItem( parent )
{
}

void OneFourStraightItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->drawLine(50,50,100,50);
  painter->drawLine(50,50,50,100);
}

ThreeFourStraightItem::ThreeFourStraightItem( QGraphicsItem * parent  )
  : GridItem( parent )
{
}

void ThreeFourStraightItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->drawLine(50,50,0,50);
  painter->drawLine(50,50,50,100);
}

TwoThreeStraightItem::TwoThreeStraightItem( QGraphicsItem * parent )
  : GridItem( parent )
{
}

void TwoThreeStraightItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->drawLine(50,50,0,50);
  painter->drawLine(50,50,50,0);
}

OneTwoStraightItem::OneTwoStraightItem(QGraphicsItem * parent  )
  : GridItem( parent )
{
}

void OneTwoStraightItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->drawLine(50,50,100,50);
  painter->drawLine(50,50,50,0);
}

OneThreeNodeItem::OneThreeNodeItem( QGraphicsItem * parent )
  : GridItem( parent )
{
}

void OneThreeNodeItem::setModelObject( model::OptionalModelObject mo )
{
  GridItem::setModelObject(mo);

  if( m_contextButton )
  {
    delete m_contextButton;
  }

  if( mo )
  {
    if( boost::optional<model::Node> node = mo->optionalCast<model::Node>() )
    {
      if( hasSPM(node.get()) )
      {
        m_contextButton = new NodeContextButtonItem(this); 

        m_contextButton->setPos(70,15);
      }
    }
  }
}

void OneThreeNodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->drawLine(0,50,100,50);
  setZValue(1);
  painter->drawEllipse(43,43,15,15);

  if(m_modelObject)
  {
    if( model::OptionalNode node = m_modelObject->optionalCast<model::Node>() )
    {
      if( node->getSetpointManagerMixedAir() )
      {
        painter->drawPixmap(37,13,25,25,QPixmap(":/images/setpoint_mixed.png"));
      }
      else if( node->getSetpointManagerSingleZoneReheat() )
      {
        painter->drawPixmap(37,13,25,25,QPixmap(":/images/setpoint_singlezone.png"));
      }  
      else if( node->setpointManagerScheduled() )
      {
        painter->drawPixmap(37,13,25,25,QPixmap(":/images/setpoint_scheduled.png"));
      }  
      else if( node->setpointManagerFollowOutdoorAirTemperature() )
      {
        painter->drawPixmap(37,13,25,25,QPixmap(":/images/setpoint_follow_outdoorair.png"));
      }  
      else if( node->setpointManagerOutdoorAirReset() )
      {
        painter->drawPixmap(37,13,25,25,QPixmap(":/images/setpoint_outdoorair.png"));
      }  
      else if( node->setpointManagerWarmest() )
      {
        painter->drawPixmap(37,13,25,25,QPixmap(":/images/setpoint_warmest.png"));
      }  
    }  
  }
}

FourFiveNodeItem::FourFiveNodeItem( QGraphicsItem * parent )
  : GridItem( parent )
{
}

void FourFiveNodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->drawLine(50,50,50,100);
  setZValue(1);
  painter->drawEllipse(43,43,15,15);
}

OAEndNodeItem::OAEndNodeItem( QGraphicsItem * parent )
  : GridItem( parent )
{
}

void OAEndNodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  painter->setPen(QPen(Qt::black,4,Qt::NoPen, Qt::RoundCap));
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->drawRect(5,0,90,100);

  if(isSelected())
  {
    painter->setBrush(QBrush(QColor(128,128,128),Qt::SolidPattern));
    painter->drawRect(5,0,90,100);
  }

  if(m_highlight)
  {
    painter->setPen(QPen(Qt::black,4,Qt::NoPen, Qt::RoundCap));
    painter->setBrush(QBrush(QColor(127, 127, 127, 127)));
    painter->drawRect(5,0,90,100);
  }

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->drawLine(50,50,50,100);
  setZValue(1);
  painter->drawEllipse(43,43,15,15);
}

TwoFiveNodeItem::TwoFiveNodeItem( QGraphicsItem * parent )
  : GridItem( parent )
{
}

void TwoFiveNodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->drawLine(50,0,50,50);
  setZValue(1);
  painter->drawEllipse(43,43,15,15);
}

TwoFourNodeItem::TwoFourNodeItem( QGraphicsItem * parent )
  : GridItem( parent )
{
}

void TwoFourNodeItem::setModelObject( model::OptionalModelObject mo )
{
  GridItem::setModelObject(mo);

  if( m_contextButton )
  {
    delete m_contextButton;
  }

  if( mo )
  {
    if( boost::optional<model::Node> node = mo->optionalCast<model::Node>() )
    {
      if( hasSPM(node.get()) )
      {
        m_contextButton = new NodeContextButtonItem(this); 

        m_contextButton->setPos(72,65);
      }
    }
  }
}

void TwoFourNodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->drawLine(50,0,50,100);
  setZValue(1);
  painter->drawEllipse(43,43,15,15);

  if(m_modelObject)
  {
    if( model::OptionalNode node = m_modelObject->optionalCast<model::Node>() )
    {
      if( node->getSetpointManagerMixedAir() )
      {
        painter->drawPixmap(62,37,25,25,QPixmap(":/images/setpoint_mixed_right.png"));
      }
      else if( node->getSetpointManagerSingleZoneReheat() )
      {
        painter->drawPixmap(62,37,25,25,QPixmap(":/images/setpoint_singlezone_right.png"));
      }  
      else if( node->setpointManagerScheduled() )
      {
        painter->drawPixmap(62,37,25,25,QPixmap(":/images/setpoint_scheduled_right.png"));
      }  
      else if( node->setpointManagerFollowOutdoorAirTemperature() )
      {
        painter->drawPixmap(62,37,25,25,QPixmap(":/images/setpoint_follow_outdoorair_right.png"));
      }  
      else if( node->setpointManagerOutdoorAirReset() )
      {
        painter->drawPixmap(62,37,25,25,QPixmap(":/images/setpoint_outdoorair_right.png"));
      }  
      else if( node->setpointManagerWarmest() )
      {
        painter->drawPixmap(62,37,25,25,QPixmap(":/images/setpoint_warmest_right.png"));
      }  
    }  
  }
}

OAStraightNodeItem::OAStraightNodeItem( QGraphicsItem * parent )
  : GridItem( parent )
{
}

void OAStraightNodeItem::setModelObject( model::OptionalModelObject mo )
{
  GridItem::setModelObject(mo);

  if( m_contextButton )
  {
    delete m_contextButton;
  }

  if( mo )
  {
    if( boost::optional<model::Node> node = mo->optionalCast<model::Node>() )
    {
      if( hasSPM(node.get()) )
      {
        m_contextButton = new NodeContextButtonItem(this); 

        m_contextButton->setPos(70,65);
      }
    }
  }
}

void OAStraightNodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  painter->setPen(QPen(Qt::black,4,Qt::NoPen, Qt::RoundCap));
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->drawRect(5,0,90,100);

  if(isSelected())
  {
    painter->setBrush(QBrush(QColor(128,128,128),Qt::SolidPattern));
    painter->drawRect(5,0,90,100);
  }

  if(m_highlight)
  {
    painter->setPen(QPen(Qt::black,4,Qt::NoPen, Qt::RoundCap));
    painter->setBrush(QBrush(QColor(127, 127, 127, 127)));
    painter->drawRect(5,0,90,100);
  }

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->drawLine(50,0,50,100);
  setZValue(1);
  painter->drawEllipse(43,43,15,15);

  if(m_modelObject)
  {
    if( model::OptionalNode node = m_modelObject->optionalCast<model::Node>() )
    {
      if( node->getSetpointManagerMixedAir() )
      {
        painter->drawPixmap(62,37,25,25,QPixmap(":/images/setpoint_mixed.png"));
      }
      else if( node->getSetpointManagerSingleZoneReheat() )
      {
        painter->drawPixmap(62,37,25,25,QPixmap(":/images/setpoint_singlezone.png"));
      }  
      else if( node->setpointManagerScheduled() )
      {
        painter->drawPixmap(62,37,25,25,QPixmap(":/images/setpoint_scheduled.png"));
      }  
      else if( node->setpointManagerFollowOutdoorAirTemperature() )
      {
        painter->drawPixmap(62,37,25,25,QPixmap(":/images/setpoint_follow_outdoorair.png"));
      }  
      else if( node->setpointManagerOutdoorAirReset() )
      {
        painter->drawPixmap(62,37,25,25,QPixmap(":/images/setpoint_outdoorair.png"));
      }  
      else if( node->setpointManagerWarmest() )
      {
        painter->drawPixmap(62,37,25,25,QPixmap(":/images/setpoint_warmest.png"));
      }  
    }  
  }
}

OAMixerItem::OAMixerItem( QGraphicsItem * parent )
  : GridItem( parent )
{
  setHGridLength(2);
}

void OAMixerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::NoPen, Qt::RoundCap));
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->drawLine(0,50,200,50);
  painter->drawLine(50,50,50,0);
  setZValue(1);
  painter->drawLine(150,50,150,0);

  if( modelObject() )
  {
    const QPixmap * qPixmap = IconLibrary::Instance().findIcon( modelObject()->iddObject().type().value() );
    painter->drawPixmap(0,0,200,100,*qPixmap);
  }
}

void OAMixerItem::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
  GridItem::mouseReleaseEvent(event);
}

OASystemItem::OASystemItem( model::AirLoopHVACOutdoorAirSystem & oaSystem,
                            QGraphicsItem * parent )
  : GridItem( parent ),
    m_oaMixerItem(NULL), 
    m_oaBranch(NULL), 
    m_reliefBranch(NULL),
    m_oaNodeItem(NULL),
    m_reliefNodeItem(NULL)
{
  m_oaMixerItem = new OAMixerItem(this);
  m_oaMixerItem->setModelObject( oaSystem );

  std::vector<model::ModelObject> oaComponents = oaSystem.oaComponents();
  std::vector<model::ModelObject> oaBranchComponents( oaComponents.begin() + 1, oaComponents.end() );

  std::vector<model::ModelObject> reliefComponents = oaSystem.reliefComponents();
  std::vector<model::ModelObject> reliefBranchComponents( reliefComponents.begin(), reliefComponents.end() - 1 );

  m_reliefBranch = new OAReliefBranchItem( reliefBranchComponents,oaBranchComponents,this );
  m_oaBranch = new OASupplyBranchItem( oaBranchComponents,reliefBranchComponents,this );

  m_oaNodeItem = new OAEndNodeItem(this);
  m_oaNodeItem->setModelObject(oaSystem.outboardOANode().get());

  m_reliefNodeItem = new OAEndNodeItem(this);
  m_reliefNodeItem->setModelObject(oaSystem.outboardReliefNode().get());

  layout();
}

void OASystemItem::layout()
{
  setHGridLength(2);

  m_reliefNodeItem->setGridPos(0,0);
  m_reliefBranch->setGridPos(0,1);
  m_oaNodeItem->setGridPos(1,0);
  m_oaBranch->setGridPos(1,1);

  unsigned oaVLength = m_oaBranch->getVGridLength();
  setVGridLength( oaVLength + 2 );
  m_oaMixerItem->setGridPos(0, oaVLength + 1);
}

void OASystemItem::paint(QPainter *painter, 
                         const QStyleOptionGraphicsItem *option, 
                         QWidget *widget)
{
}

SplitterItem::SplitterItem( QGraphicsItem * parent )
  : GridItem( parent ),
    m_numberBranches(1)
{
}

void SplitterItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));

  painter->drawLine(50,50,50,(((m_numberBranches * 2) - 1) * 100) - 50);

  int midpointIndex;
  if( m_numberBranches == 1 )
  {
    midpointIndex = 0;
    const QPixmap * qPixmap = IconLibrary::Instance().findIcon( 
                              modelObject()->iddObject().type().value() );
    painter->drawPixmap(12,(midpointIndex * 100) + 12,75,75,*qPixmap);
  }
  else
  {
    midpointIndex = m_numberBranches - 1;
  }
  painter->drawLine(50,(midpointIndex * 100) + 50,100,(midpointIndex * 100) + 50);

  int j = 50;
  for( int branchIndex = 1; branchIndex <= m_numberBranches; branchIndex++ )
  {
    painter->drawLine(0,j,50,j);
    j = j + 200;
  }
}


void SplitterItem::setNumberBranches( int branches )
{
  m_numberBranches = branches;
  m_vLength = (branches * 2) - 1;
}

int SplitterItem::numberBranches()
{
  return m_numberBranches;
}

SupplySplitterItem::SupplySplitterItem( QGraphicsItem * parent )
  : GridItem( parent ),
    m_numberBranches(1)
{
}

void SupplySplitterItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));

  painter->drawLine(50,50,50,(((m_numberBranches * 2) - 1) * 100) - 50);

  int midpointIndex;
  if( m_numberBranches == 1 )
  {
    midpointIndex = 0;
    QPixmap qPixmap(":images/supply_splitter.png");
    painter->drawPixmap(12,(midpointIndex * 100) + 12,75,75,qPixmap);
  }
  else
  {
    midpointIndex = m_numberBranches - 1;
  }
  painter->drawLine(0,(midpointIndex * 100) + 50,50,(midpointIndex * 100) + 50);

  int j = 50;
  for( int branchIndex = 1; branchIndex <= m_numberBranches; branchIndex++ )
  {
    painter->drawLine(50,j,100,j);
    j = j + 200;
  }
}

void SupplySplitterItem::setNumberBranches( int branches )
{
  m_numberBranches = branches;
  m_vLength = (branches * 2) - 1;
}

int SupplySplitterItem::numberBranches()
{
  return m_numberBranches;
}

MixerItem::MixerItem( QGraphicsItem * parent )
  : GridItem( parent ),
    m_numberBranches( 1 )
{
}

void MixerItem::setNumberBranches( int branches )
{
  m_numberBranches = branches;
  m_vLength = (branches * 2) - 1;
}

int MixerItem::numberBranches()
{
  return m_numberBranches;
}

void MixerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));

  painter->drawLine(50,50,50,(((m_numberBranches * 2) - 1) * 100) - 50);

  int midpointIndex;
  if( m_numberBranches == 1 )
  {
    midpointIndex = 0;
    const QPixmap * qPixmap = IconLibrary::Instance().findIcon( 
                              modelObject()->iddObject().type().value() );
    painter->drawPixmap(12,(midpointIndex * 100) + 12,75,75,*qPixmap);
  }
  else
  {
    midpointIndex = m_numberBranches - 1;
  }
  painter->drawLine(0,(midpointIndex * 100) + 50,50,(midpointIndex * 100) + 50);

  int j = 50;
  for( int branchIndex = 1; branchIndex <= m_numberBranches; branchIndex++ )
  {
    painter->drawLine(50,j,100,j);
    j = j + 200;
  }
}

SupplyMixerItem::SupplyMixerItem( QGraphicsItem * parent )
  : GridItem( parent ),
    m_numberBranches( 1 )
{
}

void SupplyMixerItem::setNumberBranches( int branches )
{
  m_numberBranches = branches;
  m_vLength = (branches * 2) - 1;
}

int SupplyMixerItem::numberBranches()
{
  return m_numberBranches;
}

void SupplyMixerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));

  painter->drawLine(50,50,50,(((m_numberBranches * 2) - 1) * 100) - 50);

  int midpointIndex;
  if( m_numberBranches == 1 )
  {
    midpointIndex = 0;
    QPixmap qPixmap(":/images/supply_mixer.png");
    painter->drawPixmap(12,(midpointIndex * 100) + 12,75,75,qPixmap);
  }
  else
  {
    midpointIndex = m_numberBranches - 1;
  }
  painter->drawLine(50,(midpointIndex * 100) + 50,100,(midpointIndex * 100) + 50);

  int j = 50;
  for( int branchIndex = 1; branchIndex <= m_numberBranches; branchIndex++ )
  {
    painter->drawLine(0,j,50,j);
    j = j + 200;
  }
}

DemandSideItem::DemandSideItem( QGraphicsItem * parent,
                                model::Node demandInletNode, 
                                model::Node demandOutletNode )
  : GridItem( parent ),
    m_demandInletNode(demandInletNode),
    m_demandOutletNode(demandOutletNode),
    m_inletBranch(NULL), 
    m_outletBranch(NULL),
    m_zoneBranches(NULL),
    m_splitterItem(NULL),
    m_mixerItem(NULL),
    m_inletNode(NULL),
    m_outletNode(NULL),
    m_padding(0),
    m_inletSpacer(NULL),
    m_outletSpacer(NULL)
{
  model::Loop loop = m_demandInletNode.loop().get(); 

  model::Splitter splitter = loop.demandSplitter();

  model::Mixer mixer = loop.demandMixer();

  m_zoneBranches = new HorizontalBranchGroupItem( splitter,
                                                  mixer,
                                                  this );

  m_zoneBranches->setShowDropZone(true);

  m_splitterItem = new SplitterItem(this);
  m_splitterItem->setModelObject(splitter);

  m_mixerItem = new MixerItem(this);
  m_mixerItem->setModelObject(mixer);

  m_mixerItem->setNumberBranches( m_zoneBranches->numberOfBranches() );
  m_splitterItem->setNumberBranches( m_zoneBranches->numberOfBranches() );

  std::vector<model::ModelObject> inletComponents; 
  inletComponents = loop.demandComponents(m_demandInletNode,splitter);
  inletComponents.erase( inletComponents.begin() );
  inletComponents.erase( inletComponents.end() - 1 );
  std::vector<model::ModelObject> rInletComponents;
  for( std::vector<model::ModelObject>::reverse_iterator rit = inletComponents.rbegin();
       rit < inletComponents.rend(); ++rit )
  {
    rInletComponents.push_back( *rit );
  }
  m_inletBranch = new HorizontalBranchItem(rInletComponents,this);

  std::vector<model::ModelObject> outletComponents; 
  outletComponents = loop.demandComponents(mixer,m_demandOutletNode);
  outletComponents.erase( outletComponents.begin() );
  outletComponents.erase( outletComponents.end() - 1 );
  std::vector<model::ModelObject> rOutletComponents;
  for( std::vector<model::ModelObject>::reverse_iterator rit = outletComponents.rbegin();
       rit < outletComponents.rend(); ++rit )
  {
    rOutletComponents.push_back( *rit );
  }
  m_outletBranch = new HorizontalBranchItem(rOutletComponents,this);

  m_rightElbow = new TwoThreeStraightItem(this);
  m_leftElbow = new OneTwoStraightItem(this);
  std::vector<model::ModelObject> vertComps;
  m_leftVertical = new VerticalBranchItem(vertComps,this);
  m_rightVertical = new VerticalBranchItem(vertComps,this);
  m_outletNode = new TwoFourNodeItem(this);
  m_outletNode->setModelObject(demandOutletNode);
  m_inletNode = new TwoFourNodeItem(this);
  m_inletNode->setModelObject(demandInletNode);

  if( rInletComponents.size() == 0 )
  {
    m_inletSpacer = new OneThreeStraightItem(this);
  }

  if( rOutletComponents.size() == 0 )
  {
    m_outletSpacer = new OneThreeStraightItem(this);
  }

  layout();
}

int DemandSideItem::numberBranches()
{
  return m_mixerItem->numberBranches();
}

void DemandSideItem::setPadding( unsigned padding )
{
  m_padding = padding;
  layout();
}

unsigned DemandSideItem::padding()
{
  return m_padding;
}

void DemandSideItem::layout()
{
  int midpoint = 0;

  if( m_zoneBranches )
  {
    midpoint = (m_zoneBranches->getVGridLength() / 2);
  }

  m_outletNode->setGridPos(0,0);

  m_leftVertical->setPadding(midpoint);  
  m_leftVertical->setGridPos(0,1);

  int i = 0;

  m_leftElbow->setGridPos(i,midpoint + 1);

  i = i + 1;

  if( m_outletSpacer )
  {
    m_outletSpacer->setGridPos(i,midpoint + 1);

    i = i + m_outletSpacer->getHGridLength();
  }

  if( m_padding % 2 == 0)
  {
    m_outletBranch->setPadding( m_padding / 2 );
  }
  else
  {
    m_outletBranch->setPadding( (m_padding / 2) + 1 );
  }

  m_outletBranch->setGridPos(i,midpoint + 1);

  i = i + m_outletBranch->getHGridLength();

  m_mixerItem->setGridPos(i,1);

  i = i + 1;

  m_zoneBranches->show();

  m_zoneBranches->setGridPos(i,1); 

  i = i + m_zoneBranches->getHGridLength();

  m_splitterItem->setGridPos(i,1);

  i = i + 1;

  m_inletBranch->setPadding( m_padding / 2 );

  m_inletBranch->setGridPos(i,midpoint + 1);

  i = i + m_inletBranch->getHGridLength();

  if( m_inletSpacer )
  {
    m_inletSpacer->setGridPos(i,midpoint + 1);

    i = i + m_inletSpacer->getHGridLength();
  }

  m_rightElbow->setGridPos(i,midpoint + 1);

  m_rightVertical->setPadding(midpoint);  
  m_rightVertical->setGridPos(i,1);

  m_inletNode->setGridPos(i,0);

  setHGridLength(i + 1);

  setVGridLength(m_zoneBranches->getVGridLength() + 1);
}

void DemandSideItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}

SupplySideItem::SupplySideItem( QGraphicsItem * parent, 
                                model::Node supplyInletNode, 
                                model::Node supplyOutletNode )
  : GridItem( parent ),
    m_supplyInletNode(supplyInletNode),
    m_supplyOutletNode(supplyOutletNode),
    m_outletBranchItem(NULL), 
    m_inletBranchItem(NULL),
    m_mainBranchGroupItem(NULL),
    m_oaSystemItem(NULL),
    m_inletNodeItem(NULL),
    m_outletNodeItem(NULL),
    m_leftVerticalItem(NULL),
    m_rightVerticalItem(NULL),
    m_splitterItem(NULL),
    m_mixerItem(NULL),
    m_padding(0),
    m_inletSpacer(NULL),
    m_outletSpacer(NULL)
{
  model::Node _supplyInletNode = m_supplyInletNode;
  model::Node _supplyOutletNode = m_supplyOutletNode;
  model::Loop loop = m_supplyInletNode.loop().get();

  boost::optional<model::AirLoopHVAC> airLoop = loop.optionalCast<model::AirLoopHVAC>();
  boost::optional<model::AirLoopHVACOutdoorAirSystem> oaSystem;
  if( airLoop )
  {
    oaSystem = airLoop->airLoopHVACOutdoorAirSystem();
  }

  m_inletNodeItem = new TwoFourNodeItem(this);
  m_inletNodeItem->setModelObject( _supplyInletNode );

  m_outletNodeItem = new TwoFourNodeItem(this);
  m_outletNodeItem->setModelObject( _supplyOutletNode );

  if( airLoop && oaSystem )
  {
    m_oaSystemItem = new OASystemItem(oaSystem.get(),this);

    std::vector<model::ModelObject> returnComponents = 
      airLoop->supplyComponents(_supplyInletNode, oaSystem->returnAirModelObject()->cast<model::Node>());
    returnComponents.erase( returnComponents.begin() );

    m_inletBranchItem = new HorizontalBranchItem( returnComponents,this );

    std::vector<model::ModelObject> mainComponents =
      airLoop->supplyComponents(oaSystem->mixedAirModelObject()->cast<model::Node>(), _supplyOutletNode);
    mainComponents.erase( mainComponents.end() - 1 );

    m_outletBranchItem = new HorizontalBranchItem( mainComponents, this );
  }
  else
  {
    boost::optional<model::Splitter> splitter;
    boost::optional<model::Mixer> mixer;

    if( boost::optional<model::PlantLoop> plant = loop.optionalCast<model::PlantLoop>() )
    {
      splitter = plant->supplySplitter();
      mixer = plant->supplyMixer();
    }

    if( splitter && mixer )
    {
      m_splitterItem = new SupplySplitterItem(this);
      m_splitterItem->setModelObject(splitter.get());

      m_mixerItem = new SupplyMixerItem(this);
      m_mixerItem->setModelObject(mixer.get());

      m_mainBranchGroupItem = new HorizontalBranchGroupItem( splitter.get(),
                                                             mixer.get(),
                                                             this );

      m_mainBranchGroupItem->setShowDropZone(true);

      m_mixerItem->setNumberBranches( m_mainBranchGroupItem->numberOfBranches() );
      m_splitterItem->setNumberBranches( m_mainBranchGroupItem->numberOfBranches() );

      std::vector<model::ModelObject> inletComponents; 
      inletComponents = loop.supplyComponents(m_supplyInletNode,splitter.get());
      inletComponents.erase( inletComponents.begin() );
      inletComponents.erase( inletComponents.end() - 1 );
      m_inletBranchItem = new HorizontalBranchItem(inletComponents,this);

      std::vector<model::ModelObject> outletComponents; 
      outletComponents = loop.supplyComponents(mixer.get(),m_supplyOutletNode);
      outletComponents.erase( outletComponents.begin() );
      outletComponents.erase( outletComponents.end() - 1 );
      m_outletBranchItem = new HorizontalBranchItem(outletComponents,this);

      if( inletComponents.size() == 0 )
      {
        m_inletSpacer = new OneThreeStraightItem(this);
      }

      if( outletComponents.size() == 0 )
      {
        m_outletSpacer = new OneThreeStraightItem(this);
      }
    }
    else
    {
      std::vector<model::ModelObject> supplyComponents = loop.supplyComponents(); 
      supplyComponents.erase( supplyComponents.begin() );
      supplyComponents.erase( supplyComponents.end() - 1 );
      m_outletBranchItem = new HorizontalBranchItem( supplyComponents, this );
    }
  }

  m_rightElbowItem = new ThreeFourStraightItem(this);
  m_leftElbowItem = new OneFourStraightItem(this);

  std::vector<model::ModelObject> vertComps;
  m_leftVerticalItem = new VerticalBranchItem(vertComps,this);
  m_rightVerticalItem = new VerticalBranchItem(vertComps,this);

  layout();
}

int SupplySideItem::numberBranches()
{
  if( m_splitterItem )
  {
    return m_mixerItem->numberBranches();
  }
  else 
  {
    return 1;
  }
}

void SupplySideItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
}

void SupplySideItem::layout()
{
  int i = 0;
  int j = 0; // Represents the centerline 

  int halfBranchGroupHeight = 0;

  if( m_mainBranchGroupItem )
  {
    halfBranchGroupHeight = m_mainBranchGroupItem->getVGridLength() / 2;

    j = halfBranchGroupHeight;
  }

  if( m_oaSystemItem )
  {
    int j2 = m_oaSystemItem->getVGridLength() - 1;

    if( j2 > j )
    {
      j = j2;
    }
  } 

  m_inletNodeItem->setGridPos(i,j + halfBranchGroupHeight + 1);

  if( halfBranchGroupHeight > 0 )
  {
    m_leftVerticalItem->setGridPos(i,j + 1);
    m_leftVerticalItem->setPadding(halfBranchGroupHeight);
    m_leftVerticalItem->show();
  }
  else
  {
    m_leftVerticalItem->hide();
  }

  m_leftElbowItem->setGridPos(i,j);

  i++;

  if( m_inletBranchItem )
  {
    m_inletBranchItem->setGridPos(i,j);

    if( m_padding % 2 == 0)
    {
      m_inletBranchItem->setPadding( m_padding / 2 );
    }
    else
    {
      m_inletBranchItem->setPadding( (m_padding / 2) + 1 );
    }

    i = i + m_inletBranchItem->getHGridLength();
  }

  if( m_inletSpacer )
  {
    m_inletSpacer->setGridPos(i,j);

    i = i + m_inletSpacer->getHGridLength();
  }

  if( m_oaSystemItem )
  {
    m_oaSystemItem->setGridPos(i,0);

    i = i + m_oaSystemItem->getHGridLength();
  }

  if( m_splitterItem )
  {
    m_splitterItem->setGridPos(i,j - halfBranchGroupHeight);

    i++;
  }

  if( m_mainBranchGroupItem )
  {
    m_mainBranchGroupItem->setGridPos(i,j - halfBranchGroupHeight);

    i = i + m_mainBranchGroupItem->getHGridLength();
  }

  if( m_mixerItem )
  {
    m_mixerItem->setGridPos(i,j - halfBranchGroupHeight);

    i++;
  }

  if( m_outletSpacer )
  {
    m_outletSpacer->setGridPos(i,j);

    i = i + m_outletSpacer->getHGridLength();
  }

  m_outletBranchItem->setGridPos(i,j);

  if( m_inletBranchItem )
  {
    m_outletBranchItem->setPadding(m_padding / 2);
  }
  else
  {
    m_outletBranchItem->setPadding(m_padding);
  }

  i = i + m_outletBranchItem->getHGridLength();

  m_rightElbowItem->setGridPos(i,j);

  if( halfBranchGroupHeight > 0 )
  {
    m_rightVerticalItem->setGridPos(i,j + 1);
    m_rightVerticalItem->setPadding(halfBranchGroupHeight);
    m_rightVerticalItem->show();
  }
  else
  {
    m_rightVerticalItem->hide();
  }

  m_outletNodeItem->setGridPos(i,j + halfBranchGroupHeight + 1);

  setHGridLength(i + 1);
  setVGridLength(j + halfBranchGroupHeight + 2);
}

void SupplySideItem::setPadding( unsigned padding )
{
  m_padding = padding;
  layout();
}

unsigned SupplySideItem::padding()
{
  return m_padding;
}

TwoThreeFourStraightItem::TwoThreeFourStraightItem(QGraphicsItem * parent)
  : GridItem(parent)
{
}

void TwoThreeFourStraightItem::paint(QPainter *painter, 
                                   const QStyleOptionGraphicsItem *option, 
                                   QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));

  painter->drawLine( 50,0,50,100 );
  painter->drawLine( 0,50,50,50 );
}

OneTwoFourStraightItem::OneTwoFourStraightItem(QGraphicsItem * parent)
  : GridItem(parent)
{
}

void OneTwoFourStraightItem::paint(QPainter *painter, 
                                   const QStyleOptionGraphicsItem *option, 
                                   QWidget *widget)
{
  GridItem::paint(painter,option,widget);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(QBrush(Qt::lightGray,Qt::SolidPattern));
  painter->setPen(QPen(Qt::black,4,Qt::SolidLine, Qt::RoundCap));

  painter->drawLine( 50,0,50,100 );
  painter->drawLine( 50,50,100,50 );
}

NodeContextButtonItem::NodeContextButtonItem(GridItem * parent)
  : ButtonItem(QPixmap(":/images/contextual_arrow.png"),
               QPixmap(":/images/contextual_arrow.png"),
               QPixmap(":/images/contextual_arrow.png"),
               parent)
{
  bool bingo;

  bingo = connect(this,SIGNAL(mouseClicked()),this,SLOT(showContextMenu()));
  OS_ASSERT(bingo);

  bingo = connect( this, 
             SIGNAL(removeModelObjectClicked( model::ModelObject & ) ),
             parent,
             SIGNAL(removeModelObjectClicked( model::ModelObject & ) ) );
  OS_ASSERT(bingo);
}

void NodeContextButtonItem::showContextMenu()
{
  if( scene() != NULL
      && ! scene()->views().empty() 
      && scene()->views().first() != NULL
      && scene()->views().first()->viewport() != NULL )
  {

    QGraphicsView * v = scene()->views().first();
    QPointF sceneP = mapToScene(boundingRect().bottomRight());
    QPoint viewP = v->mapFromScene(sceneP);
    QPoint menuPos = v->viewport()->mapToGlobal(viewP);

    bool bingo;

    QMenu menu;
    QAction removeSPMAction(QIcon(":/images/delete-icon.png"),"Delete Setpoint Manager",&menu);
    menu.addAction(&removeSPMAction);
    bingo = connect(&removeSPMAction,SIGNAL(triggered()),this,SLOT(onRemoveSPMActionTriggered()));
    OS_ASSERT(bingo);

    menu.exec(menuPos); 
  }
}

void NodeContextButtonItem::onRemoveSPMActionTriggered()
{
  if(GridItem * gridItem = qobject_cast<GridItem *>(parentObject()))
  {
    if( gridItem != NULL
        && gridItem->modelObject()
        && gridItem->modelObject()->optionalCast<model::Node>() )
    {
      model::Node node = gridItem->modelObject()->cast<model::Node>();

      if(OptionalSetpointManagerSingleZoneReheat spm = node.getSetpointManagerSingleZoneReheat())
      {
        emit removeModelObjectClicked( spm.get() );
      }
      else if(OptionalSetpointManagerMixedAir spm = node.getSetpointManagerMixedAir())
      {
        emit removeModelObjectClicked( spm.get() );
      }
      else if(OptionalSetpointManagerOutdoorAirReset spm = node.setpointManagerOutdoorAirReset())
      {
        emit removeModelObjectClicked( spm.get() );
      }
      else if(boost::optional<SetpointManagerScheduled> spm = node.setpointManagerScheduled())
      {
        emit removeModelObjectClicked( spm.get() );
      }
      else if(boost::optional<SetpointManagerWarmest> spm = node.setpointManagerWarmest())
      {
        emit removeModelObjectClicked( spm.get() );
      }
      else if(boost::optional<SetpointManagerFollowOutdoorAirTemperature> spm = 
              node.setpointManagerFollowOutdoorAirTemperature())
      {
        emit removeModelObjectClicked( spm.get() );
      }
    }
  }
}

} // openstudio

