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

#ifndef OPENSTUDIO_REFRIGERATIONGRAPHICSITEMS_HPP
#define OPENSTUDIO_REFRIGERATIONGRAPHICSITEMS_HPP

#include <QGraphicsObject>
#include "OSItem.hpp"
#include "OSDropZone.hpp"
#include "../shared_gui_components/OSListController.hpp"
#include "../shared_gui_components/OSListView.hpp"
#include "../shared_gui_components/GraphicsItems.hpp"
#include "../utilities/idf/Handle.hpp"

class QGraphicsView;
class QPushButton;
class QLabel;

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

class RefrigerationView : public QWidget
{
  Q_OBJECT;

  public:

  RefrigerationView();

  QWidget * header;

  QGraphicsView * graphicsView;

  QPushButton * zoomOutButton;

  QLabel * nameLabel;
};

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

  void adjustLayout();

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

class RefrigerationSystemDropZoneView : public OSDropZoneItem 
{
  Q_OBJECT;

  public:

  QRectF boundingRect() const;

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget );
};

class RefrigerationSystemView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  RefrigerationSystemView();

  virtual ~RefrigerationSystemView() {}

  static const int verticalSpacing; 

  static const int margin;

  static const double componentHeight;

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

  void setId(const OSItemId & id);

  signals:

  void inspectClicked(OSItemId id);

  public slots:

  void setCasesExpanded(bool expanded);

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );

  void mousePressEvent(QGraphicsSceneMouseEvent * event);

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

  bool m_mouseDown;

  private:

  OSItemId m_id;
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

  void setIcon(const QPixmap & pixmap);

  static QSizeF size();

  signals:

  void removeClicked(OSItemId id);

  void inspectClicked(OSItemId id);

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );

  void mousePressEvent(QGraphicsSceneMouseEvent * event);

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

  bool m_mouseDown;

  private slots:

  void onRemoveButtonClicked();

  private:

  OSItemId m_id;

  QString m_name;

  QPixmap m_pixmap;
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

  void inspectClicked(OSItemId id);

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );

  void mousePressEvent(QGraphicsSceneMouseEvent * event);

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

  bool m_mouseDown;

  private slots:

  void onRemoveButtonClicked();
  
  private:

  QString m_label;

  OSItemId m_id;

  QPixmap m_pixmap;
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

  static double height();

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

  static QSizeF size();

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

  static QRectF nameRect();

  static QRectF iconRect();

  QRectF boundingRect() const;

  void setName(const QString & name);

  void setId(const OSItemId & id);

  signals:

  void removeClicked(OSItemId id);

  void inspectClicked(OSItemId id);

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );

  void mousePressEvent(QGraphicsSceneMouseEvent * event);

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

  bool m_mouseDown;

  private slots:

  void onRemoveButtonClicked();

  private:

  QString m_name;

  OSItemId m_id;

  QPixmap m_displayCasesPixmap;

  QPixmap m_walkinPixmap;
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

  static QRectF displayCasesRect();

  static QRectF walkinCasesRect();

  static QRectF summaryRect();

  public slots:

  void setExpanded(bool exapanded);

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );

  private:

  QPointF casePos(int index) const;

  int m_numberOfDisplayCases;

  int m_numberOfWalkinCases;

  QSizeF size() const;

  bool m_expanded;

  std::vector<QGraphicsObject *> m_caseDetailViews;

  QPixmap m_displayCasesPixmap;

  QPixmap m_walkinPixmap;
};

class RefrigerationSubCoolerView : public RefrigerationSystemDropZoneView
{
  Q_OBJECT;

  public:

  RefrigerationSubCoolerView();

  virtual ~RefrigerationSubCoolerView() {}

  RemoveButtonItem * removeButtonItem;

  QRectF boundingRect() const;

  void setId(const OSItemId & id);

  void setName(const QString & name);

  static QSizeF size();

  signals:

  void removeClicked(OSItemId id);

  void inspectClicked(OSItemId id);

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );

  void mousePressEvent(QGraphicsSceneMouseEvent * event);

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

  bool m_mouseDown;

  private slots:

  void onRemoveButtonClicked();

  private:

  OSItemId m_id;

  QString m_name;

  QPixmap m_pixmap;
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

  RefrigerationSHXView();

  virtual ~RefrigerationSHXView() {}

  RemoveButtonItem * removeButtonItem;

  QRectF boundingRect() const;

  void setId(const OSItemId & id);

  void setName(const QString & name);

  static QSizeF size();

  signals:

  void removeClicked(OSItemId id);

  void inspectClicked(OSItemId id);

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );

  void mousePressEvent(QGraphicsSceneMouseEvent * event);

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

  bool m_mouseDown;

  private slots:

  void onRemoveButtonClicked();

  private:

  OSItemId m_id;

  QString m_name;

  QPixmap m_pixmap;
};

class SecondaryDropZoneView : public RefrigerationSystemDropZoneView
{
  Q_OBJECT;

  public:

  QRectF boundingRect() const;

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget );
};

class SecondaryDetailView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  SecondaryDetailView();

  virtual ~SecondaryDetailView() {}

  ZoomInButtonItem * zoomInButtonItem;
  RemoveButtonItem * removeButtonItem;

  QRectF boundingRect() const;
  QRectF nameRect();

  void setName(const QString & name);
  void setHandle(const Handle & handle);

  static double width();
  static double height();

  signals:

  void zoomInOnSystemClicked(const Handle & handle);
  void removeClicked(const Handle & handle);

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget );

  private slots:

  void onZoomButtonClicked();
  void onRemoveButtonClicked();

  private:

  QString m_name;
  Handle m_handle;
};

class RefrigerationSecondaryView : public QGraphicsObject
{
  Q_OBJECT;

  public:

  RefrigerationSecondaryView();

  virtual ~RefrigerationSecondaryView() {}

  QRectF boundingRect() const;

  SecondaryDropZoneView * secondaryDropZoneView;

  void insertSecondaryDetailView(int index, QGraphicsObject * object);

  void removeAllSecondaryDetailViews();

  void adjustLayout();

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );

  private:

  std::vector<QGraphicsObject *> m_secondaryDetailViews;
  int m_height;
};

} // openstudio

#endif // OPENSTUDIO_REFRIGERATIONGRAPHICSITEMS_HPP

