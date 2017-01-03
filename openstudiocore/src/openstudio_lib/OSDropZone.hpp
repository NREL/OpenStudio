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

#ifndef OPENSTUDIO_OSDROPZONE_HPP
#define OPENSTUDIO_OSDROPZONE_HPP

#include "OSItem.hpp"
#include <nano/nano_signal_slot.hpp> // Signal-Slot replacement

#include "../shared_gui_components/FieldMethodTypedefs.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsItem>

class QBoxLayout;
class QDropEvent;
class QDragEnterEvent;
class QDragLeaveEvent;
class QFocusEvent;
class QPushButton;
class QScrollArea;
class QLabel;

namespace openstudio {

class OSItem;
class OSVectorController;

// This is a "bindable" version of drop zone
// that looks different because it is initially designed to go 
// in a grid.  It also works differently because it binds to function pointers.
// There is no use of VectorController like in OSDropZone.
class OSDropZone2 : public QWidget, public Nano::Observer
{
  Q_OBJECT

public:

  OSDropZone2();

  ~OSDropZone2();

  void enableClickFocus() { this->setFocusPolicy(Qt::ClickFocus); }
  bool hasData() { return !this->m_label->text().isEmpty(); }
  void setDeleteObject(bool deleteObject) { m_deleteObject = deleteObject; }
  bool deleteObject() { return m_deleteObject; }
  void setIsDefaulted(bool defaulted);
  bool isDefaulted();
  void bind(model::ModelObject & modelObject,
            OptionalModelObjectGetter get,
            ModelObjectSetter set,
            boost::optional<NoFailAction> reset = boost::none);

  void unbind();

signals:

  void itemClicked(OSItem* item);
  void objectRemoved(boost::optional<model::ParentObject> parent);
  void inFocus(bool inFocus, bool hasData);

protected:

  void paintEvent ( QPaintEvent * event ) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  virtual void focusInEvent(QFocusEvent * e) override;
  virtual void focusOutEvent(QFocusEvent * e) override;

public slots:

  void onItemRemoveClicked();

private slots:

  void refresh();
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dropEvent(QDropEvent *event) override;

private:

  void makeItem();

  boost::optional<OptionalModelObjectGetter> m_get;
  boost::optional<ModelObjectSetter> m_set;
  boost::optional<NoFailAction> m_reset;
  boost::optional<model::ModelObject> m_modelObject;
  //QString m_text;
  OSItem * m_item = nullptr;
  bool m_deleteObject = false;
  QLabel * m_label;
};

class OSDropZone : public QWidget, public Nano::Observer
{
  Q_OBJECT

public:

  OSDropZone(OSVectorController* vectorController,
             const QString & text = "Drag From Library",
             const QSize & size = QSize(0,0),
             bool m_growsHorizontally = true,
             QWidget * parent = nullptr );

  virtual ~OSDropZone() {}

  int maxItems() const;
  bool setMaxItems(int max);

  int minItems() const;
  bool setMinItems(int min);

  bool itemsDraggable() const;
  void setItemsDraggable(bool itemsDraggable);

  bool itemsAcceptDrops() const;
  void setItemsAcceptDrops(bool itemsAcceptDrops);

  bool itemsRemoveable() const;
  void setItemsRemoveable(bool itemsRemoveable);

  void showAddButton();
  void hideAddButton();

  bool useLargeIcon();
  void setUseLargeIcon(bool userLargeIcon);

signals:

  // request items from vector controller
  void itemsRequested();

  // emitted in onDrop if item is created
  void itemDropped(const OSItemId& itemId);

  // emitted if item is clicked
  void itemClicked(OSItem* item);

  // emitted if item remove is clicked
  void itemRemoveClicked(OSItem* item);

  // emitted if item is dropped on existing item
  void itemReplacementDropped(OSItem * currentItem, const OSItemId& replacementItemId);

  void addButtonClicked();

protected:

  // called when drop occurs, emit onDrop here if needed
  virtual void onDrop(const OSItemId& itemId);

  void paintEvent ( QPaintEvent * event ) override;

  //void resizeEvent(QResizeEvent * event);

private slots:

  // set this objects item ids
  void setItemIds(const std::vector<OSItemId>& itemIds);

  // called on drop
  void handleDrop( QDropEvent * event );

private:

  OSVectorController* m_vectorController;
  int m_minItems;
  int m_maxItems;
  bool m_itemsDraggable;
  bool m_itemsAcceptDrops;
  bool m_itemsRemoveable;
  bool m_allowAdd;
  QBoxLayout * m_mainBoxLayout;
  QScrollArea* m_scrollArea;
  QPushButton * m_addButton;
  bool m_growsHorizontally;
  bool m_useLargeIcon;
  QString m_text;
  QSize m_size;
};

class OSItemDropZone : public QWidget
{
  Q_OBJECT

public:
  OSItemDropZone(bool growsHorizontally,
                 const QString & text,
                 const QSize & size,
                 QWidget * parent = nullptr);

  virtual ~OSItemDropZone() {}

  void setExtensible(bool extensible);

signals:
  void dropped( QDropEvent * event );

protected:
  void paintEvent ( QPaintEvent * event ) override;
  void dragEnterEvent( QDragEnterEvent * event ) override;
  void dropEvent( QDropEvent * event ) override;

private:
  QSize m_size;
  bool m_growsHorizontally;

};

// This is a version of OSDropZone that works with QGraphicsScene
// as opposed to QWidget.  Much of the custom drawing in the HVAC 
// interface is done with QGraphicsScene.
class OSDropZoneItem : public QGraphicsObject, public Nano::Observer
{
  Q_OBJECT;

  public:

  OSDropZoneItem();

  virtual ~OSDropZoneItem() {}

  QRectF boundingRect() const override;

  void setSize(double width, double height);

  void setText(const QString & text);

  signals:

  void mouseClicked();

  void componentDropped(const OSItemId & itemid);

  protected:

  void paint( QPainter *painter, 
              const QStyleOptionGraphicsItem *option, 
              QWidget *widget ) override;

  void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
  void dropEvent(QGraphicsSceneDragDropEvent *event) override;

  QString m_text;

  private:

  bool m_mouseDown;

  double m_width;
  double m_height;
};

} // openstudio

#endif // OPENSTUDIO_OSDROPZONE_HPP
