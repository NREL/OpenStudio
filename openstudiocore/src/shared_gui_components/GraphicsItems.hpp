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

#ifndef SHAREDGUICOMPONENTS_GRAPHICSITEMS_HPP
#define SHAREDGUICOMPONENTS_GRAPHICSITEMS_HPP

#include <nano/nano_signal_slot.hpp> // Signal-Slot replacement
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

  void mousePressEvent(QGraphicsSceneMouseEvent * event) override;

  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;

  bool m_checked;

  private:

  bool m_mouseDown;
};

class ButtonItem : public QGraphicsObject, public Nano::Observer
{
  Q_OBJECT;

  public:

  ButtonItem(const QPixmap & image, const QPixmap & downImage, 
             const QPixmap & hoverImage, QGraphicsItem * parent = nullptr);

  virtual ~ButtonItem() {}

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

  QRectF boundingRect() const override;

  QSizeF cellSize() const;

  void setCellSize(const QSizeF & size);

  int margin() const;

  void setMargin(int margin);

  public slots:

  void refreshAllItemViews();

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget = nullptr ) override {}

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

