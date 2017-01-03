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

#ifndef OPENSTUDIO_GRIDITEM_HPP
#define OPENSTUDIO_GRIDITEM_HPP

#include <QGraphicsObject>
#include <QCoreApplication>
#include <QPixmap>
#include <QPointer>
#include <nano/nano_signal_slot.hpp> // Signal-Slot replacement
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

class ModelObjectGraphicsItem : public QGraphicsObject, public Nano::Observer
{
  Q_OBJECT;

  public:

  ModelObjectGraphicsItem(QGraphicsItem * parent = nullptr);

  ~ModelObjectGraphicsItem() {}

  void setEnableHighlight(bool highlight);

  void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;

  void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;

  void dropEvent(QGraphicsSceneDragDropEvent *event) override;

  void hoverEnterEvent(QGraphicsSceneHoverEvent * event) override;

  void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) override;

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

  QVariant itemChange(GraphicsItemChange change, const QVariant & value) override;

  bool m_deleteAble;

  bool m_highlight;

  model::OptionalModelObject m_modelObject;

  RemoveButtonItem * m_removeButtonItem;

  bool m_enableHighlight;

  private slots:

  void onNameChange(); 

  void onRemoveButtonClicked();
};

class GridItem : public ModelObjectGraphicsItem
{
  Q_OBJECT;

  public:

  GridItem(QGraphicsItem * parent = nullptr );

  virtual ~GridItem() {}

  virtual QRectF boundingRect() const override;

  void setGridPos(int x, int y);

  int getXGridPos();

  int getYGridPos();

  void setHGridLength(int l);

  void setVGridLength(int l);

  int getHGridLength();

  int getVGridLength();

  protected:

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

  int  m_hLength;

  int  m_vLength;
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

  SplitterItem(QGraphicsItem * parent = nullptr);

  void setNumberBranches( int branches );

  int numberBranches();

  // SingleDuct1 means a terminal is attached to the demand inlet node 1 branch
  // SingleDuct2 means a terminal is attached to the demand inlet node 2 branch
  // DualDuct means there is a dual duct terminal
  enum TerminalType { SingleDuct1, SingleDuct2, DualDuct, None };

  // The size of the types vector is equal to the number of zones connected to the system
  // This is also expected to be equal to the number of branches
  void setTerminalTypes( std::vector< TerminalType > types );

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

  private:

  std::vector<TerminalType> m_terminalTypes;
  int m_numberBranches;
  int m_firstDuct1Index;
  int m_firstDuct2Index;
};

class MixerItem : public GridItem
{
  public:

  MixerItem(QGraphicsItem * parent = nullptr);

  void setNumberBranches( int branches );

  int numberBranches();

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

  private:

  int m_numberBranches;
};

class SupplySplitterItem : public GridItem
{
  public:

  SupplySplitterItem(QGraphicsItem * parent = nullptr);

  void setNumberBranches( int branches );

  int numberBranches();

  // See related method HorizontalBranchGroupItem::branchBaselineGridPositions
  // This method is used to get the splitter to line up with the horizontal lines in 
  // the HorizontalBranchGroupItem.
  // SupplySideItem will coordinate this
  // The size should be equal to numberBranches
  void setBranchGridPositions( const std::vector<int> & branchGridPositions );

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

  private:

  std::vector<int> m_baselineBranchPositions; 
};

class SupplyMixerItem : public GridItem
{
  public:

  SupplyMixerItem(QGraphicsItem * parent = nullptr);

  void setNumberBranches( int branches );

  int numberBranches();

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

  private:

  int m_numberBranches;
};

class OneThreeNodeItem : public GridItem
{
  public:

  OneThreeNodeItem(QGraphicsItem * parent = nullptr );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override; 

  void setModelObject( model::OptionalModelObject modelObject ) override;

  private:

  QPointer<NodeContextButtonItem> m_contextButton;
};

// This is a "dual path" squeezed into the space of a single GridItem
// Used after the zone splitter, going into the zone terminal
class OneThreeDualDuctItem : public GridItem
{
  public:

  OneThreeDualDuctItem(QGraphicsItem * parent = nullptr );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override; 

  void setModelObject( model::OptionalModelObject modelObject ) override;

  void setModelObject2( model::OptionalModelObject modelObject );

  private:

  boost::optional<model::ModelObject> m_modelObject2;
};

class FourFiveNodeItem : public GridItem
{
  public:

  FourFiveNodeItem(QGraphicsItem * parent = nullptr );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

class TwoFiveNodeItem : public GridItem
{
  public:

  TwoFiveNodeItem(QGraphicsItem * parent = nullptr );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

class OneThreeStraightItem : public GridItem
{
  public:

  // if dual is true there are two parallel lines
  OneThreeStraightItem(QGraphicsItem * parent = nullptr, bool dualDuct = false);

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;

  protected:

  void mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) override;

  private:

  bool m_dualDuct;
};

class OneThreeDualDuctMixerItem : public GridItem
{
  public:

  // if dual is true there are two parallel lines
  OneThreeDualDuctMixerItem(QGraphicsItem * parent = nullptr, bool dualDuct = false);

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

class SupplyPlenumItem : public GridItem
{
  public:

  SupplyPlenumItem(const model::ModelObject & modelObject, QGraphicsItem * parent = nullptr);

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;

  void setModelObject( model::OptionalModelObject modelObject ) override;

  private:

  QColor m_color;
};

class ReturnPlenumItem : public GridItem
{
  public:

  ReturnPlenumItem(const model::ModelObject & modelObject, QGraphicsItem * parent = nullptr);

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;

  void setModelObject( model::OptionalModelObject modelObject ) override;

  private:

  QColor m_color;
};

class LinkItem : public QGraphicsObject
{
  Q_OBJECT;

  public:

  LinkItem(QGraphicsItem * parent = nullptr);

  virtual ~LinkItem() {}

  QRectF boundingRect() const override;

  signals:

  void mouseClicked();

  protected:

  void mousePressEvent(QGraphicsSceneMouseEvent * event) override;

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

  void hoverEnterEvent(QGraphicsSceneHoverEvent * event) override;

  void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) override;

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;

  private:

  bool m_mouseDown;

  bool m_isHovering;
};

class OneThreeWaterToAirItem : public GridItem
{
  Q_OBJECT;

  public:

  OneThreeWaterToAirItem(QGraphicsItem * parent = nullptr );

  virtual ~OneThreeWaterToAirItem() {}

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;

  void setModelObject( model::OptionalModelObject modelObject ) override;

  private slots:

  void onLinkItemClicked();

  protected:

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

  private:

  bool m_showLinks;
};

class OneThreeWaterToWaterItem : public GridItem
{
  Q_OBJECT;

  public:

  OneThreeWaterToWaterItem(QGraphicsItem * parent = nullptr );

  virtual ~OneThreeWaterToWaterItem() {}

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;

  void setModelObject( model::OptionalModelObject modelObject ) override;

  private slots:

  void onLinkItemClicked();

  protected:

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

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

  TwoFourStraightItem(QGraphicsItem * parent = nullptr );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

class OASupplyStraightItem : public GridItem
{
  public:

  OASupplyStraightItem(QGraphicsItem * parent = nullptr );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;

  protected:

  void mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) override;
};

class OAReliefStraightItem : public GridItem
{
  public:

  OAReliefStraightItem(QGraphicsItem * parent = nullptr );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;

  protected:

  void mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) override;
};

class OAStraightNodeItem : public GridItem
{
  public:

  OAStraightNodeItem(QGraphicsItem * parent = nullptr );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;

  void setModelObject( model::OptionalModelObject modelObject ) override;

  private:

  QPointer<NodeContextButtonItem> m_contextButton;
};

class OASupplyBranchItem : public GridItem
{
  public:

  OASupplyBranchItem( std::vector<model::ModelObject> supplyModelObjects,
                      std::vector<model::ModelObject> reliefModelObjects,
                      QGraphicsItem * parent = nullptr );

  void layout();

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = nullptr) override;

  private:

  std::vector<GridItem *> m_gridItems;

  std::vector<OASupplyStraightItem *> m_paddingItems;
};

class OAReliefBranchItem : public GridItem
{
  public:

  OAReliefBranchItem( std::vector<model::ModelObject> reliefModelObjects,
                      std::vector<model::ModelObject> supplyModelObjects,
                      QGraphicsItem * parent = nullptr );

  void layout();

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = nullptr) override;

  private:

  std::vector<GridItem *> m_gridItems;

  std::vector<OASupplyStraightItem *> m_paddingItems;
};

class OAEndNodeItem : public GridItem
{
  public:

  OAEndNodeItem(QGraphicsItem * parent = nullptr );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

class OAAirToAirItem : public GridItem
{
  public:

  OAAirToAirItem(QGraphicsItem * parent = nullptr );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

class FourTwoStraightItem : public GridItem
{
  public:

  FourTwoStraightItem(QGraphicsItem * parent = nullptr );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

class TwoFourNodeItem : public GridItem
{
  public:

  TwoFourNodeItem(QGraphicsItem * parent = nullptr );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;

  void setModelObject( model::OptionalModelObject modelObject ) override;

  private:

  QPointer<NodeContextButtonItem> m_contextButton;
};

class OneFourStraightItem : public GridItem
{
  public:

  OneFourStraightItem(QGraphicsItem * parent = nullptr );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

class ThreeFourStraightItem : public GridItem
{
  public:

  ThreeFourStraightItem( QGraphicsItem * parent = nullptr );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

class TwoThreeStraightItem : public GridItem
{
  public:

  TwoThreeStraightItem(QGraphicsItem * parent = nullptr );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

class OneTwoStraightItem : public GridItem
{
  public:

  OneTwoStraightItem(QGraphicsItem * parent = nullptr );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

class OAMixerItem : public GridItem
{
  public:

  OAMixerItem(QGraphicsItem * parent = nullptr );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;

  protected:

  void mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) override;
};

// These are used inside dual duct items that combine two paths inside one GridItem
class HalfHeightItem : public ModelObjectGraphicsItem
{
  public:

  HalfHeightItem(QGraphicsItem * parent = nullptr );

  virtual ~HalfHeightItem() {}

  virtual QRectF boundingRect() const override;
};

class HalfHeightOneThreeStraightItem : public HalfHeightItem
{
  public:

  HalfHeightOneThreeStraightItem(QGraphicsItem * parent = nullptr);

  virtual ~HalfHeightOneThreeStraightItem() {}

  protected:

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

class HalfHeightOneThreeNodeItem : public HalfHeightItem
{
  public:

  HalfHeightOneThreeNodeItem(QGraphicsItem * parent = nullptr);

  virtual ~HalfHeightOneThreeNodeItem() {}

  protected:

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

class HorizontalBranchItem : public GridItem
{
  public:

  // Dual duct means there will be two parallel paths within one item
  HorizontalBranchItem( std::vector<model::ModelObject> modelObjects,
                        QGraphicsItem * parent = nullptr,
                        bool dualDuct = false );

  // This signature is always used on a dual duct
  // modelObjectsBeforeTerminal is what it says, the outer pair for the two parallel paths
  // After the terminal the paths come togethor (the dual duct terminal is a mixer) and we 
  // have a single linear series of components from then on to the zone mixer
  HorizontalBranchItem( std::pair< std::vector<model::ModelObject>, std::vector<model::ModelObject> > modelObjectsBeforeTerminal,
                        std::vector<model::ModelObject> modelObjectsAfterTerminal,
                        QGraphicsItem * parent = nullptr);

  static std::vector<GridItem *> itemFactory(std::vector<model::ModelObject> modelObjects, QGraphicsItem * parent);

  void setPadding( unsigned );

  void setText( const QString & text );

  void setHasTwoRightSidePipes( bool dualRight );

  unsigned padding();

  void layout();

  void setIsDropZone(bool isDropZone);

  void setModelObject( model::OptionalModelObject modelObject ) override;

  protected:

  virtual void paint(QPainter *painter, 
                     const QStyleOptionGraphicsItem *option, 
                     QWidget *widget = nullptr) override;

  void dropEvent(QGraphicsSceneDragDropEvent *event) override;

  private:

  std::vector<GridItem *> m_gridItems;

  std::vector<OneThreeStraightItem *> m_paddingItems;

  bool m_isDropZone;

  QString m_text;

  bool m_hasDualTwoRightSidePipes;

  bool m_dualDuct;
};

class HorizontalBranchGroupItem : public GridItem
{
  public:

  HorizontalBranchGroupItem( model::Splitter & splitter,
                             model::Mixer & mixer,
                             QGraphicsItem * parent = nullptr );

  HorizontalBranchGroupItem( model::Splitter & splitter,
                             std::vector<model::Node> & supplyOutletNodes,
                             QGraphicsItem * parent ); 

  void layout();

  void setShowDropZone(bool showDropZone);

  unsigned numberOfBranches() const;

  // HorizontalBranchGroupItem instances normally have a series of parallel branches
  // that are each 1 grid unit in height. If we have a dual duct there might be oa systems on the branches
  // and this messes up the assumption about each branch being 1 grid unit high.
  // This method will return the "baseline" (ie where the horizontal black line is) grid pos
  // for each branch.
  std::vector<int> branchBaselineGridPositions() const;

  protected:

  virtual void paint(QPainter *painter, 
                     const QStyleOptionGraphicsItem *option, 
                     QWidget *widget = nullptr) override;

  private:

  model::Splitter m_splitter;
  std::vector<HorizontalBranchItem *> m_branchItems;
  HorizontalBranchItem * m_dropZoneBranchItem;
};

class VerticalBranchItem : public GridItem
{
  public:

  VerticalBranchItem( std::vector<model::ModelObject> modelObjects,
                      QGraphicsItem * parent = nullptr );

  void setPadding( unsigned );

  void layout();

  protected:

  virtual void paint(QPainter *painter, 
                     const QStyleOptionGraphicsItem *option, 
                     QWidget *widget = nullptr) override;

  private:

  std::vector<GridItem *> m_gridItems;

  std::vector<TwoFourStraightItem *> m_paddingItems;
};

class ReverseVerticalBranchItem : public GridItem
{
  public:

  ReverseVerticalBranchItem( std::vector<model::ModelObject> modelObjects,
                      QGraphicsItem * parent = nullptr );

  void setPadding( unsigned );

  void layout();

  private:

  std::vector<GridItem *> m_gridItems;

  std::vector<FourTwoStraightItem *> m_paddingItems;
};

// This is the thing on the demand side of a dual duct
// where the second stream intersects the first forming a tee.
// The streams are not physically combined but visual they combine,
// into the same grid items (ie one grid item show both streams).
class DualDuctTee : public GridItem
{
  public:

  DualDuctTee(QGraphicsItem * parent = nullptr);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

// This is the elbow used on the demand side of a dual duct
class TwoThreeStraightItem2 : public GridItem
{
  public:

  TwoThreeStraightItem2(QGraphicsItem * parent = nullptr);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

class DemandSideItem : public GridItem
{
  public:

  DemandSideItem( QGraphicsItem * parent,
                  std::vector<model::Node> demandInletNodes, 
                  model::Node demandOutletNode );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;

  void layout();

  void setPadding( unsigned padding );

  unsigned padding();

  int numberBranches();

  private:

  std::vector<model::Node> m_demandInletNodes;

  model::Node m_demandOutletNode;

  HorizontalBranchItem * m_inletBranch;

  HorizontalBranchItem * m_outletBranch;

  HorizontalBranchGroupItem * m_zoneBranches;

  SplitterItem * m_splitterItem;

  MixerItem * m_mixerItem;

  // A TwoFourStraightItem if single duct
  // or a TwoFourStraightItem2 if a dual duct
  GridItem * m_rightElbow;

  DualDuctTee * m_rightElbow2;

  OneTwoStraightItem * m_leftElbow;

  VerticalBranchItem * m_leftVertical;

  VerticalBranchItem * m_rightVertical;

  VerticalBranchItem * m_rightVertical2;

  TwoFourNodeItem * m_inletNode;

  TwoFourNodeItem * m_inletNode2;

  TwoFourNodeItem * m_outletNode;

  unsigned m_padding;

  OneThreeStraightItem * m_inletSpacer;

  OneThreeStraightItem * m_outletSpacer;
};

class OASystemItem : public GridItem
{
  public:

  OASystemItem( model::AirLoopHVACOutdoorAirSystem & oaSystem,
                QGraphicsItem * parent = nullptr );

  protected:

  void layout();

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;

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
                  std::vector<model::Node> supplyOutletNodes );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;

  void layout();

  void setPadding( unsigned padding );

  unsigned padding();

  int numberBranches();

  private:

  model::Node m_supplyInletNode;

  std::vector<model::Node> m_supplyOutletNodes;

  HorizontalBranchItem * m_outletBranchItem;

  HorizontalBranchItem * m_inletBranchItem;

  HorizontalBranchGroupItem * m_mainBranchGroupItem;

  TwoFourNodeItem * m_inletNodeItem;

  TwoFourNodeItem * m_outletNodeItem;  

  TwoFourNodeItem * m_outletNodeItem2;  

  OneFourStraightItem * m_leftElbowItem;

  VerticalBranchItem * m_leftVerticalItem;

  ThreeFourStraightItem * m_rightElbowItem;

  ThreeFourStraightItem * m_rightElbowItem2;

  VerticalBranchItem * m_rightVerticalItem;

  VerticalBranchItem * m_rightVerticalItem2;

  SupplySplitterItem * m_splitterItem;
  
  SupplyMixerItem * m_mixerItem;

  unsigned m_padding;

  OneThreeStraightItem * m_inletSpacer;

  OneThreeStraightItem * m_outletSpacer;

  HorizontalBranchItem * m_dualDuctHorizontalSpace;
};

class SystemCenterItem : public GridItem
{
  public:

  SystemCenterItem( QGraphicsItem * parent, model::Loop loop );

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;

  private:

  bool m_supplyDualDuct;
  bool m_demandDualDuct;
};

class SystemItem : public GridItem
{
  public:

  SystemItem( model::Loop loop, LoopScene * loopScene );

  virtual ~SystemItem();

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;

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

  OneTwoFourStraightItem(QGraphicsItem * parent = nullptr);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

class TwoThreeFourStraightItem : public GridItem
{
  public:

  TwoThreeFourStraightItem(QGraphicsItem * parent = nullptr);

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = nullptr) override;
};

} // openstudio

#endif // OPENSTUDIO_GRIDITEM_HPP

