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

#ifndef OPENSTUDIO_GRIDITEM_HPP
#define OPENSTUDIO_GRIDITEM_HPP

#include <QGraphicsObject>
#include <QCoreApplication>
#include <QPixmap>
#include <QPointer>
#include "../model/ModelObject.hpp"
#include "../model/Node.hpp"
#include "../model/Mixer.hpp"
#include "../model/Splitter.hpp"
#include "../model/Loop.hpp"
#include "OSItem.hpp"
#include "shared_gui_components/GraphicsItems.hpp"

class QMenu;

namespace openstudio {

class LoopScene;

class RemoveButtonItem;

class NodeContextButtonItem;

class GridItem : public QGraphicsObject
{
  Q_OBJECT;

  public:

  GridItem(QGraphicsItem * parent = 0 );

  virtual ~GridItem() {}

  void setEnableHighlight(bool highlight);

  virtual QRectF boundingRect() const;

  void setGridPos(int x, int y);

  int getXGridPos();

  int getYGridPos();

  void setHGridLength(int l);

  void setVGridLength(int l);

  int getHGridLength();

  int getVGridLength();

  void dragEnterEvent(QGraphicsSceneDragDropEvent *event);

  void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);

  void dropEvent(QGraphicsSceneDragDropEvent *event);

  void hoverEnterEvent(QGraphicsSceneHoverEvent * event);

  void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

  //virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );

  virtual void setModelObject( model::OptionalModelObject modelObject );

  model::OptionalModelObject modelObject();

  void setDeletable(bool deletable);

  signals:

  void modelObjectSelected( model::OptionalModelObject &, bool readOnly );

  void removeModelObjectClicked( model::ModelObject & );

  void hvacComponentDropped(OSItemId itemid, model::HVACComponent & contextComp);

  void hvacComponentDropped(OSItemId itemid);

  void innerNodeClicked( model::ModelObject & );

  protected:

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

  QVariant itemChange(GraphicsItemChange change, const QVariant & value);

  bool m_deleteAble;

  bool m_highlight;

  int  m_hLength;

  int  m_vLength;

  model::OptionalModelObject m_modelObject;

  RemoveButtonItem * m_removeButtonItem;

  bool m_enableHighlight;

  private slots:

  void onNameChange(); 

  void onRemoveButtonClicked();

  private:
};

class NodeContextButtonItem : public ButtonItem
{
  Q_OBJECT;

  public:

  NodeContextButtonItem(GridItem * parent);

  signals:

  void removeModelObjectClicked( model::ModelObject & );

  private slots:

  void showContextMenu();

  void onRemoveSPMActionTriggered();

  private:

  QMenu * m_menu;
};

class SplitterItem : public GridItem
{
  public:

  SplitterItem(QGraphicsItem * parent = 0);

  void setNumberBranches( int branches );

  int numberBranches();

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

  private:

  int m_numberBranches;
};

class MixerItem : public GridItem
{
  public:

  MixerItem(QGraphicsItem * parent = 0);

  void setNumberBranches( int branches );

  int numberBranches();

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

  private:

  int m_numberBranches;
};

class SupplySplitterItem : public GridItem
{
  public:

  SupplySplitterItem(QGraphicsItem * parent = 0);

  void setNumberBranches( int branches );

  int numberBranches();

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

  private:

  int m_numberBranches;
};

class SupplyMixerItem : public GridItem
{
  public:

  SupplyMixerItem(QGraphicsItem * parent = 0);

  void setNumberBranches( int branches );

  int numberBranches();

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

  private:

  int m_numberBranches;
};

class OneThreeNodeItem : public GridItem
{
  public:

  OneThreeNodeItem(QGraphicsItem * parent = 0 );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0); 

  void setModelObject( model::OptionalModelObject modelObject );

  private:

  QPointer<NodeContextButtonItem> m_contextButton;
};

class FourFiveNodeItem : public GridItem
{
  public:

  FourFiveNodeItem(QGraphicsItem * parent = 0 );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

class TwoFiveNodeItem : public GridItem
{
  public:

  TwoFiveNodeItem(QGraphicsItem * parent = 0 );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

class OneThreeStraightItem : public GridItem
{
  public:

  OneThreeStraightItem(QGraphicsItem * parent = 0 );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);

  protected:

  void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );
};

class SupplyPlenumItem : public GridItem
{
  public:

  SupplyPlenumItem(const model::ModelObject & modelObject, QGraphicsItem * parent = 0);

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);

  void setModelObject( model::OptionalModelObject modelObject );

  private:

  QColor m_color;
};

class ReturnPlenumItem : public GridItem
{
  public:

  ReturnPlenumItem(const model::ModelObject & modelObject, QGraphicsItem * parent = 0);

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);

  void setModelObject( model::OptionalModelObject modelObject );

  private:

  QColor m_color;
};

class LinkItem : public QGraphicsObject
{
  Q_OBJECT;

  public:

  LinkItem(QGraphicsItem * parent = 0);

  virtual ~LinkItem() {}

  QRectF boundingRect() const;

  signals:

  void mouseClicked();

  protected:

  void mousePressEvent(QGraphicsSceneMouseEvent * event);

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

  void hoverEnterEvent(QGraphicsSceneHoverEvent * event);

  void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);

  private:

  bool m_mouseDown;

  bool m_isHovering;
};

class OneThreeWaterToAirItem : public GridItem
{
  Q_OBJECT;

  public:

  OneThreeWaterToAirItem(QGraphicsItem * parent = 0 );

  virtual ~OneThreeWaterToAirItem() {}

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);

  void setModelObject( model::OptionalModelObject modelObject );

  private slots:

  void onLinkItemClicked();

  protected:

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

  private:

  bool m_showLinks;
};

class OneThreeWaterToWaterItem : public GridItem
{
  Q_OBJECT;

  public:

  OneThreeWaterToWaterItem(QGraphicsItem * parent = 0 );

  virtual ~OneThreeWaterToWaterItem() {}

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);

  void setModelObject( model::OptionalModelObject modelObject );

  private slots:

  void onLinkItemClicked();

  protected:

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

  private:

  bool m_showLinks;
};

//class OneThreeWaterToAirItem : public GridItem
//{
//  Q_OBJECT;
//
//  public:
//
//  OneThreeWaterToAirItem(QGraphicsItem * parent = 0 );
//
//  void paint(QPainter *painter, 
//             const QStyleOptionGraphicsItem *option, 
//             QWidget *widget = 0);
//
//  void setModelObject( model::OptionalModelObject modelObject );
//
//  private slots:
//
//  void onLinkItemClicked();
//
//  protected:
//
//  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
//
//  private:
//
//  bool m_showLinks;
//};

class TwoFourStraightItem : public GridItem
{
  public:

  TwoFourStraightItem(QGraphicsItem * parent = 0 );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

class OASupplyStraightItem : public GridItem
{
  public:

  OASupplyStraightItem(QGraphicsItem * parent = 0 );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);

  protected:

  void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );
};

class OAReliefStraightItem : public GridItem
{
  public:

  OAReliefStraightItem(QGraphicsItem * parent = 0 );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);

  protected:

  void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );
};

class OAStraightNodeItem : public GridItem
{
  public:

  OAStraightNodeItem(QGraphicsItem * parent = 0 );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);

  void setModelObject( model::OptionalModelObject modelObject );

  private:

  QPointer<NodeContextButtonItem> m_contextButton;
};

class OASupplyBranchItem : public GridItem
{
  public:

  OASupplyBranchItem( std::vector<model::ModelObject> supplyModelObjects,
                      std::vector<model::ModelObject> reliefModelObjects,
                      QGraphicsItem * parent = 0 );

  void layout();

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0);

  private:

  std::vector<GridItem *> m_gridItems;

  std::vector<OASupplyStraightItem *> m_paddingItems;
};

class OAReliefBranchItem : public GridItem
{
  public:

  OAReliefBranchItem( std::vector<model::ModelObject> reliefModelObjects,
                      std::vector<model::ModelObject> supplyModelObjects,
                      QGraphicsItem * parent = 0 );

  void layout();

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0);

  private:

  std::vector<GridItem *> m_gridItems;

  std::vector<OASupplyStraightItem *> m_paddingItems;
};

class OAEndNodeItem : public GridItem
{
  public:

  OAEndNodeItem(QGraphicsItem * parent = 0 );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

class OAAirToAirItem : public GridItem
{
  public:

  OAAirToAirItem(QGraphicsItem * parent = 0 );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

class FourTwoStraightItem : public GridItem
{
  public:

  FourTwoStraightItem(QGraphicsItem * parent = 0 );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

class TwoFourNodeItem : public GridItem
{
  public:

  TwoFourNodeItem(QGraphicsItem * parent = 0 );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);

  void setModelObject( model::OptionalModelObject modelObject );

  private:

  QPointer<NodeContextButtonItem> m_contextButton;
};

class OneFourStraightItem : public GridItem
{
  public:

  OneFourStraightItem(QGraphicsItem * parent = 0 );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

class ThreeFourStraightItem : public GridItem
{
  public:

  ThreeFourStraightItem( QGraphicsItem * parent = 0 );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

class TwoThreeStraightItem : public GridItem
{
  public:

  TwoThreeStraightItem(QGraphicsItem * parent = 0 );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

class OneTwoStraightItem : public GridItem
{
  public:

  OneTwoStraightItem(QGraphicsItem * parent = 0 );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

class OAMixerItem : public GridItem
{
  public:

  OAMixerItem(QGraphicsItem * parent = 0 );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);

  protected:

  void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );
};

class HorizontalBranchItem : public GridItem
{
  public:

  HorizontalBranchItem( std::vector<model::ModelObject> modelObjects,
                        QGraphicsItem * parent = 0 );

  void setPadding( unsigned );

  void setText( const QString & text );

  void setHasTwoRightSidePipes( bool dualRight );

  unsigned padding();

  void layout();

  void setIsDropZone(bool isDropZone);

  void setModelObject( model::OptionalModelObject modelObject );

  protected:

  virtual void paint(QPainter *painter, 
                     const QStyleOptionGraphicsItem *option, 
                     QWidget *widget = 0);

  void dropEvent(QGraphicsSceneDragDropEvent *event);

  private:

  std::vector<GridItem *> m_gridItems;

  std::vector<OneThreeStraightItem *> m_paddingItems;

  bool m_isDropZone;

  QString m_text;

  bool m_hasDualTwoRightSidePipes;
};

class HorizontalBranchGroupItem : public GridItem
{
  public:

  HorizontalBranchGroupItem( model::Splitter & splitter,
                             model::Mixer & mixer,
                             QGraphicsItem * parent = 0 );

  void layout();

  void setShowDropZone(bool showDropZone);

  unsigned numberOfBranches() const;

  protected:

  virtual void paint(QPainter *painter, 
                     const QStyleOptionGraphicsItem *option, 
                     QWidget *widget = 0);

  private:

  model::Splitter m_splitter;

  model::Mixer m_mixer;

  std::vector<HorizontalBranchItem *> m_branchItems;

  HorizontalBranchItem * m_dropZoneBranchItem;
};

class VerticalBranchItem : public GridItem
{
  public:

  VerticalBranchItem( std::vector<model::ModelObject> modelObjects,
                      QGraphicsItem * parent = 0 );

  void setPadding( unsigned );

  void layout();

  protected:

  virtual void paint(QPainter *painter, 
                     const QStyleOptionGraphicsItem *option, 
                     QWidget *widget = 0);

  private:

  std::vector<GridItem *> m_gridItems;

  std::vector<TwoFourStraightItem *> m_paddingItems;
};

class ReverseVerticalBranchItem : public GridItem
{
  public:

  ReverseVerticalBranchItem( std::vector<model::ModelObject> modelObjects,
                      QGraphicsItem * parent = 0 );

  void setPadding( unsigned );

  void layout();

  private:

  std::vector<GridItem *> m_gridItems;

  std::vector<FourTwoStraightItem *> m_paddingItems;
};

class DemandSideItem : public GridItem
{
  public:

  DemandSideItem( QGraphicsItem * parent,
                  model::Node demandInletNode, 
                  model::Node demandOutletNode );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);

  void layout();

  void setPadding( unsigned padding );

  unsigned padding();

  int numberBranches();

  private:

  model::Node m_demandInletNode;

  model::Node m_demandOutletNode;

  HorizontalBranchItem * m_inletBranch;

  HorizontalBranchItem * m_outletBranch;

  HorizontalBranchGroupItem * m_zoneBranches;

  SplitterItem * m_splitterItem;

  MixerItem * m_mixerItem;

  TwoThreeStraightItem * m_rightElbow;

  OneTwoStraightItem * m_leftElbow;

  VerticalBranchItem * m_leftVertical;

  VerticalBranchItem * m_rightVertical;

  TwoFourNodeItem * m_inletNode;

  TwoFourNodeItem * m_outletNode;

  unsigned m_padding;

  OneThreeStraightItem * m_inletSpacer;

  OneThreeStraightItem * m_outletSpacer;
};

class OASystemItem : public GridItem
{
  public:

  OASystemItem( model::AirLoopHVACOutdoorAirSystem & oaSystem,
                QGraphicsItem * parent = 0 );

  protected:

  void layout();

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);

  private:

  OAMixerItem * m_oaMixerItem;

  OASupplyBranchItem * m_oaBranch;

  OAReliefBranchItem * m_reliefBranch;

  OAEndNodeItem * m_oaNodeItem;

  OAEndNodeItem * m_reliefNodeItem;
};

class SupplySideItem : public GridItem
{
  public:

  SupplySideItem( QGraphicsItem * parent, 
                  model::Node supplyInletNode, 
                  model::Node supplyOutletNode );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);

  void layout();

  void setPadding( unsigned padding );

  unsigned padding();

  int numberBranches();

  private:

  model::Node m_supplyInletNode;

  model::Node m_supplyOutletNode;

  HorizontalBranchItem * m_outletBranchItem;

  HorizontalBranchItem * m_inletBranchItem;

  HorizontalBranchGroupItem * m_mainBranchGroupItem;

  OASystemItem * m_oaSystemItem;

  TwoFourNodeItem * m_inletNodeItem;

  TwoFourNodeItem * m_outletNodeItem;  

  ThreeFourStraightItem * m_rightElbowItem;

  OneFourStraightItem * m_leftElbowItem;

  VerticalBranchItem * m_leftVerticalItem;

  VerticalBranchItem * m_rightVerticalItem;

  SupplySplitterItem * m_splitterItem;
  
  SupplyMixerItem * m_mixerItem;

  unsigned m_padding;

  OneThreeStraightItem * m_inletSpacer;

  OneThreeStraightItem * m_outletSpacer;
};

class SystemCenterItem : public GridItem
{
  public:

  SystemCenterItem( QGraphicsItem * parent, model::Loop loop );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

class SystemItem : public GridItem
{
  public:

  SystemItem( model::Loop loop, LoopScene * loopScene );

  virtual ~SystemItem();

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);

  QColor plenumColor(const Handle & plenumHandle);

  private:

  model::Loop m_loop;

  LoopScene * m_loopScene;

  SupplySideItem * m_supplySideItem;

  DemandSideItem * m_demandSideItem;
  
  SystemCenterItem * m_systemCenterItem;

  std::map<Handle,int> m_plenumIndexMap;
  std::map<Handle,QColor> m_plenumColorMap;

  int plenumIndex(const Handle & plenumHandle);
};

class OneTwoFourStraightItem : public GridItem
{
  public:

  OneTwoFourStraightItem(QGraphicsItem * parent = 0);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

class TwoThreeFourStraightItem : public GridItem
{
  public:

  TwoThreeFourStraightItem(QGraphicsItem * parent = 0);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);
};

} // openstudio

#endif // OPENSTUDIO_GRIDITEM_HPP

