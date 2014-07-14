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

#ifndef SHAREDGUICOMPONENTS_GRAPHICSITEMS_HPP
#define SHAREDGUICOMPONENTS_GRAPHICSITEMS_HPP

#include <QGraphicsObject>
#include <QSizeF>

namespace openstudio {

class OSListItem;
class OSListController;
class OSGraphicsItemDelegate;

// Button functionality without any visual elements
// ButtonItem already existed when AbstractButtonItem was factored out.
// Ideally ButtonItem derives from AbstractButtonItem.
class AbstractButtonItem : public QGraphicsObject
{
  Q_OBJECT;

  public:

  AbstractButtonItem(QGraphicsItem * parent = nullptr);

  virtual ~AbstractButtonItem() {}

  void setChecked(bool checked);
  
  signals:

  void mouseClicked(bool checked = false);

  void toggled(bool checked);

  protected:

  void mousePressEvent(QGraphicsSceneMouseEvent * event);

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

  bool m_checked;

  private:

  bool m_mouseDown;
};

class ButtonItem : public QGraphicsObject
{
  Q_OBJECT;

  public:

  ButtonItem(const QPixmap & image, const QPixmap & downImage, 
             const QPixmap & hoverImage, QGraphicsItem * parent = nullptr);

  virtual ~ButtonItem() {}

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
             QWidget *widget = nullptr);

  private:

  bool m_mouseDown;

  bool m_isHovering;

  QPixmap m_image;

  QPixmap m_downImage;

  QPixmap m_hoverImage;
};

class RemoveButtonItem : public ButtonItem
{
  public:

  RemoveButtonItem(QGraphicsItem * parent = nullptr);
};

class ZoomInButtonItem : public ButtonItem
{
  public:

  ZoomInButtonItem(QGraphicsItem * parent = nullptr);
};

class ZoomOutButtonItem : public ButtonItem
{
  public:

  ZoomOutButtonItem(QGraphicsItem * parent = nullptr);
};

class GridLayoutItem : public QGraphicsObject
{
  Q_OBJECT;

  public:

  GridLayoutItem();

  virtual ~GridLayoutItem();

  void setDelegate(QSharedPointer<OSGraphicsItemDelegate> delegate);

  void setListController(QSharedPointer<OSListController> listController);

  QSharedPointer<OSListController> listController() const;

  QRectF boundingRect() const;

  QSizeF cellSize() const;

  void setCellSize(const QSizeF & size);

  int margin() const;

  void setMargin(int margin);

  public slots:

  void refreshAllItemViews();

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = nullptr ) {}

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

  QSizeF m_cellSize;

  int m_margin;
};

} // openstudio

#endif // SHAREDGUICOMPONENTS_GRAPHICSITEMS_HPP

