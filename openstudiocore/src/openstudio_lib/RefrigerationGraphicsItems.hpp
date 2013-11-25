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

#ifndef OPENSTUDIO_REFRIGERATIONGRAPHICSITEMS_H
#define OPENSTUDIO_REFRIGERATIONGRAPHICSITEMS_H

#include <QGraphicsObject>
#include "OSItem.hpp"
#include "../shared_gui_components/OSListController.hpp"
#include "../shared_gui_components/OSListView.hpp"
#include "../shared_gui_components/GraphicsItems.hpp"

namespace openstudio {

class RefrigerationCondenserView;
class RefrigerationSubCoolerView;
class RefrigerationHeatReclaimView;
class RefrigerationCompressorView;
class RefrigerationSHXView;
class RefrigerationCasesView;
class RefrigerationCaseDetailView;
class RefrigerationCasesDropZoneView;
class RefrigerationSystemView;
class RefrigerationSecondaryView;
class RefrigerationCompressorDropZoneView;
class OSItemId;

class CaseViewExpandButton : public AbstractButtonItem
{
  Q_OBJECT;

  public:

  CaseViewExpandButton();

  virtual ~CaseViewExpandButton() {}

  QRectF boundingRect() const;

  static QSize size();

  protected:

  void paint(QPainter *painter, 
             const QStyleOptionGraphicsItem *option, 
             QWidget *widget = 0);

  private:

  QPixmap m_openImage;

  QPixmap m_closeImage;
};

// A grid layout of refrigeration systems
class RefrigerationSystemGridView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  RefrigerationSystemGridView();

  virtual ~RefrigerationSystemGridView();

  void setDelegate(QSharedPointer<OSGraphicsItemDelegate> delegate);

  void setListController(QSharedPointer<OSListController> listController);

  QSharedPointer<OSListController> listController() const;

  QRectF boundingRect() const;

  public slots:

  void refreshAllItemViews();

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 ) {}

  private slots:

  void insertItemView(int i);

  void removeItemView(int i);

  void removePair(QObject * object);

  void refreshItemView(int i);

  private:

  int spacing() const;

  int rows() const;

  int columns() const;

  std::pair<int,int> gridPos(int i); 

  QGraphicsObject * createNewItemView(int i);

  void setItemViewGridPos(QGraphicsObject * item,std::pair<int,int> gridPos);

  QGraphicsObject * viewFromGridPos(std::pair<int,int> gridPos);

  QSharedPointer<OSGraphicsItemDelegate> m_delegate;

  QSharedPointer<OSListController> m_listController;

  // Use this to keep the OSListItem classes around for the life of the widget
  std::map<QObject *,QSharedPointer<OSListItem> > m_widgetItemPairs;

  std::map<std::pair<int,int>,QObject *> m_gridPosItemViewPairs;

  std::map<QObject *,std::pair<int,int> > m_itemViewGridPosPairs;
};

// A cell of the refrigeration system grid
// This is an item that contains a RefrigerationSystemItem plus some overlays
class RefrigerationSystemMiniView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  RefrigerationSystemMiniView();

  virtual ~RefrigerationSystemMiniView() {}

  RefrigerationSystemView * refrigerationSystemView;

  RemoveButtonItem * removeButtonItem;

  ZoomInButtonItem * zoomInButtonItem;

  QRectF boundingRect() const;

  static QSize cellSize();

  public slots:

  void setName( const QString & name); 

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget );

  private:

  QRectF contentRect() const;

  QRectF headerRect() const;

  static int cellWidth();

  static int headerHeight();

  QString m_name;
};

class RefrigerationSystemDetailView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  RefrigerationSystemDetailView();

  virtual ~RefrigerationSystemDetailView() {}

  RefrigerationSystemView * refrigerationSystemView;

  ZoomOutButtonItem * zoomOutButton;

  QRectF boundingRect() const;

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget );

  private:

  // QRectF contentRect() const;

  // QRectF headerRect() const;

  // static int cellWidth();

  // static int headerHeight();

  // QString m_name;
};

class RefrigerationSystemDropZoneView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  RefrigerationSystemDropZoneView();

  virtual ~RefrigerationSystemDropZoneView() {}

  QRectF boundingRect() const;

  signals:

  void mouseClicked();

  void componentDropped(const OSItemId & itemid);

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget );

  void mousePressEvent(QGraphicsSceneMouseEvent * event);

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

  void dropEvent(QGraphicsSceneDragDropEvent *event);

  private:

  bool m_mouseDown;
};

class RefrigerationSystemView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  RefrigerationSystemView();

  virtual ~RefrigerationSystemView() {}

  static const int verticalSpacing; 

  static const int margin;

  RefrigerationCondenserView * refrigerationCondenserView;

  RefrigerationSubCoolerView * refrigerationSubCoolerView;

  RefrigerationHeatReclaimView * refrigerationHeatReclaimView;

  RefrigerationCompressorView * refrigerationCompressorView;

  RefrigerationSHXView * refrigerationSHXView;

  RefrigerationCasesView * refrigerationCasesView;

  RefrigerationSecondaryView * refrigerationSecondaryView;

  void adjustLayout();

  // RefrigerationSystemItem is divided into three main colums.
  // These are the x coordinates of the left, center, and right center lines

  int leftXPos() const;

  int centerXPos() const;

  int rightXPos() const;

  QRectF boundingRect() const;

  public slots:

  void setCasesExpanded(bool expanded);

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );
};

class RefrigerationCondenserView : public RefrigerationSystemDropZoneView
{
  Q_OBJECT;

  public:

  RefrigerationCondenserView();

  virtual ~RefrigerationCondenserView() {}

  RemoveButtonItem * removeButtonItem;

  QRectF boundingRect() const;

  void setCondenserId(const OSItemId & condenserId);

  void setCondenserName(const QString & name);

  static QSizeF size();

  signals:

  void removeClicked(OSItemId id);

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );

  private slots:

  void onRemoveButtonClicked();

  private:

  OSItemId m_condenserId;

  QString m_name;
};

class RefrigerationCompressorDetailView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  RefrigerationCompressorDetailView();

  virtual ~RefrigerationCompressorDetailView() {}

  RemoveButtonItem * removeButtonItem;

  static QSizeF size();

  QRectF boundingRect() const;

  void setLabel(const QString & label);

  void setId(const OSItemId & id);

  signals:

  void removeClicked(OSItemId id);

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );

  private slots:

  void onRemoveButtonClicked();
  
  private:

  QString m_label;

  OSItemId m_id;
};

class RefrigerationCompressorDropZoneView : public RefrigerationSystemDropZoneView
{
  Q_OBJECT;

  public:

  RefrigerationCompressorDropZoneView();

  virtual ~RefrigerationCompressorDropZoneView() {}

  QRectF boundingRect() const;

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );
};

class RefrigerationCompressorView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  RefrigerationCompressorView();

  virtual ~RefrigerationCompressorView() {}

  QRectF boundingRect() const;

  RefrigerationCompressorDropZoneView * refrigerationCompressorDropZoneView;

  void insertCompressorDetailView(int index, QGraphicsObject * object);

  void removeAllCompressorDetailViews();

  void adjustLayout();

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );

  private:

  std::vector<QGraphicsObject *> m_compressorDetailViews;
};

class RefrigerationCasesDropZoneView : public RefrigerationSystemDropZoneView
{
  Q_OBJECT;

  public:

  RefrigerationCasesDropZoneView();

  virtual ~RefrigerationCasesDropZoneView() {}

  QRectF boundingRect() const;

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );
};

class RefrigerationCaseDetailView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  RefrigerationCaseDetailView();

  virtual ~RefrigerationCaseDetailView() {}

  RemoveButtonItem * removeButtonItem;

  static QSizeF size();

  QRectF boundingRect() const;

  void setName(const QString & name);

  void setId(const OSItemId & id);

  signals:

  void removeClicked(OSItemId id);

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );

  private slots:

  void onRemoveButtonClicked();

  private:

  QRectF nameRect() const;

  QRectF iconRect() const;

  QString m_name;

  OSItemId m_id;
};

class RefrigerationCasesView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  RefrigerationCasesView();

  virtual ~RefrigerationCasesView() {}

  QRectF boundingRect() const;

  RefrigerationCasesDropZoneView * refrigerationCasesDropZoneView;

  CaseViewExpandButton * expandButton;

  void setNumberOfDisplayCases(int number);

  void setNumberOfWalkinCases(int number);

  void insertCaseDetailView(int index, QGraphicsObject * object);

  void removeAllCaseDetailViews();

  void adjustLayout();

  public slots:

  void setExpanded(bool exapanded);

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );

  private:

  QPointF casePos(int index) const;

  QRectF displayCasesRect() const;

  QRectF walkinCasesRect() const;

  QRectF summaryRect() const;

  QSizeF size() const;

  int m_numberOfDisplayCases;

  int m_numberOfWalkinCases;

  bool m_expanded;

  std::vector<QGraphicsObject *> m_caseDetailViews;
};

class RefrigerationSubCoolerView : public RefrigerationSystemDropZoneView
{
  Q_OBJECT;

  public:

  QRectF boundingRect() const;

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );
};

class RefrigerationHeatReclaimView : public RefrigerationSystemDropZoneView
{
  Q_OBJECT;

  public:

  QRectF boundingRect() const;

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );
};

class RefrigerationSHXView : public RefrigerationSystemDropZoneView
{
  Q_OBJECT;

  public:

  QRectF boundingRect() const;

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );
};

class RefrigerationSecondaryView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  QRectF boundingRect() const;

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );
};

} // openstudio

#endif // OPENSTUDIO_REFRIGERATIONGRAPHICSITEMS_H

