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

namespace openstudio {

class RefrigerationCondenserItem;
class RefrigerationSubCoolerItem;
class RefrigerationHeatReclaimItem;
class RefrigerationCompressorItem;
class RefrigerationSHXItem;
class RefrigerationCasesItem;
class RefrigerationSecondaryItem;
class OSListItem;
class OSListController;

class RefrigerationSystemItemDelegate : public QObject
{
  Q_OBJECT;

  public:

  RefrigerationSystemItemDelegate();

  virtual ~RefrigerationSystemItemDelegate() {}

  virtual QGraphicsItem * view(QSharedPointer<OSListItem> dataSource);
};

class RefrigerationSystemGridItem : public QGraphicsObject
{
  Q_OBJECT;

  public:

  RefrigerationSystemGridItem();

  virtual ~RefrigerationSystemGridItem() {};

  void setDelegate(QSharedPointer<RefrigerationSystemItemDelegate> delegate);

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

  QSize cellSize() const;

  std::pair<int,int> gridLocation(int index); 

  QGraphicsItem * viewFromGridLocation(std::pair<int,int>);

  QSharedPointer<RefrigerationSystemItemDelegate> m_delegate;

  QSharedPointer<OSListController> m_listController;
};

class RefrigerationSystemItem : public QGraphicsObject
{
  Q_OBJECT;

  public:

  RefrigerationSystemItem();

  virtual ~RefrigerationSystemItem() {}

  static const int verticalSpacing; 

  RefrigerationCondenserItem * refrigerationCondenserItem;

  RefrigerationSubCoolerItem * refrigerationSubCoolerItem;

  RefrigerationHeatReclaimItem * refrigerationHeatReclaimItem;

  RefrigerationCompressorItem * refrigerationCompressorItem;

  RefrigerationSHXItem * refrigerationSHXItem;

  RefrigerationCasesItem * refrigerationCasesItem;

  RefrigerationSecondaryItem * refrigerationSecondaryItem;

  void adjustLayout();

  // RefrigerationSystemItem is divided into three main colums.
  // These are the x coordinates of the left, center, and right center lines

  int leftXPos() const;

  int centerXPos() const;

  int rightXPos() const;

  QRectF boundingRect() const;

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );
};

class RefrigerationCasesItem : public QGraphicsObject
{
  Q_OBJECT;

  public:

  RefrigerationCasesItem();

  virtual ~RefrigerationCasesItem() {}

  QRectF boundingRect() const;

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );
};

class RefrigerationCondenserItem : public QGraphicsObject
{
  Q_OBJECT;

  public:

  QRectF boundingRect() const;

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );
};

class RefrigerationCompressorItem : public QGraphicsObject
{
  Q_OBJECT;

  public:

  QRectF boundingRect() const;

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );
};

class RefrigerationSubCoolerItem : public QGraphicsObject
{
  Q_OBJECT;

  public:

  QRectF boundingRect() const;

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );
};

class RefrigerationHeatReclaimItem : public QGraphicsObject
{
  Q_OBJECT;

  public:

  QRectF boundingRect() const;

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );
};

class RefrigerationSHXItem : public QGraphicsObject
{
  Q_OBJECT;

  public:

  QRectF boundingRect() const;

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = 0 );
};

class RefrigerationSecondaryItem : public QGraphicsObject
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

