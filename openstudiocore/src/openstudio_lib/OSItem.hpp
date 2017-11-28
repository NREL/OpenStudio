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

#ifndef OPENSTUDIO_OSITEM_HPP
#define OPENSTUDIO_OSITEM_HPP

#include "../shared_gui_components/LocalLibrary.hpp"

#include <QVariant>
#include <QWidget>
#include <vector>

#include <nano/nano_signal_slot.hpp> // Signal-Slot replacement

class QDragEnterEvent;
class QDropEvent;
class QLabel;
class QMimeData;
class QPushButton;

namespace openstudio {

class MeasureBadge;

class OSItemId
{
public:
  static const QString BCL_SOURCE_ID;
  OSItemId();
  OSItemId(const QString& itemId, const QString& sourceId, bool isDefaulted, const QString& otherData = "");
  OSItemId(const QMimeData* mimeData);
  QString itemId() const;
  QString sourceId() const;
  QString otherData() const;
  QString mimeDataText() const;
  bool isDefaulted() const;
  void setIsDefaulted(bool isDefaulted);
  bool operator==(const OSItemId& other) const;
private:
  QString m_itemId;
  QString m_sourceId;
  QString m_otherData;
  bool m_isDefaulted;
};

class OSItem : public QWidget, public Nano::Observer
{
  Q_OBJECT

  public:

    // creates a new OSItem if possible
    static OSItem* makeItem(const OSItemId& itemId, OSItemType osItemType = OSItemType::ListItem);

    OSItem(const OSItemId& itemId, OSItemType osItemType = OSItemType::ListItem, QWidget * parent = nullptr);

    virtual ~OSItem() {}

    OSItemId itemId() const;

    bool isDefaulted() const;
    void setIsDefaulted(bool isDefaulted);

    bool selected() const;
    void setSelected(bool selected);

    ///! is the remove button visible
    bool removeable() const;
    ///! show / hide the remove button
    void setRemoveable(bool removeable);

    bool draggable() const;
    void setDraggable(bool draggable);

    bool inspectable() const;
    void setInspectable(bool inspectable);

    QString text() const;
    void setText(const QString & text);

    QPixmap leftPixmap() const;
    void setLeftPixmap(const QPixmap & pixmap);

    QPixmap rightPixmap() const;
    void setRightPixmap(const QPixmap & pixmap);

    void setAspectRatio(AspectRatio aspectRatio);

    OSItemType osItemType() const;
    void setOSItemType(OSItemType osItemType);

    bool useLargeIcon();
    void setUseLargeIcon(bool userLargeIcon);

    virtual bool equal(const OSItem* other) const = 0;

    ///! The side length, if a square
    static const int ITEM_SIDE = 80;
    ///! The height, if a rectangle
    static const int ITEM_HEIGHT = 60;
    ///! The width, if a rectangle
    static const int ITEM_WIDTH = 150;
    ///! The height, if a header
    static const int HEADER_HEIGHT = 40;
    ///! The width, if a header
    static const int HEADER_WIDTH = ITEM_WIDTH;

    static const int IMAGE_HEIGHT = 30;
    static const int IMAGE_WIDTH = 30;
    static const int BTN_HEIGHT = 20;
    static const int BTN_WIDTH = 20;

  signals:
    void itemClicked(OSItem *);
    void itemRemoveClicked(OSItem *);
    void itemReplacementDropped(OSItem * currentItem, const OSItemId& replacementItemId);

  public slots:
    void onRemoveClicked();

  protected:
    void paintEvent(QPaintEvent * event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent*event) override;
    void leaveEvent(QEvent* event) override;
    //void resizeEvent(QResizeEvent* event);
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;

    bool isBold();
    void setBold(bool isBold);

    QColor textColor();
    void setTextColor(QColor color = Qt::black);
    MeasureBadge * m_measureBadge;

  private:
    void setFixedSize(const QSize & size);
    void setFixedWidth(int width);
    void setFixedHeight(int height);
    void setAttributes(OSItemType osItemType);
    void createLayout();
    //void setItemSize();
    void setLabelPixmap(QLabel * label, const QPixmap & pixmap);

    OSItemId m_itemId;
    QWidget * m_selectionWidget;
    QWidget * m_borderWidget;
    QPushButton * m_removeButton;
    QLabel * m_textLbl;
    QLabel * m_imageLeftLbl;
    QLabel * m_imageRightLbl;
    QPoint m_dragStartPosition;
    bool m_mouseDown;
    bool m_selected;
    bool m_draggable;
    bool m_inspectable;
    bool m_acceptsDrops;
    QSize m_size;
    OSItemType m_osItemType;
    QColor m_borderColor;
    bool m_useLargeIcon;
    // Large icon used behind everything else
    // For items that map to model objects, this will be set to the icon as opposed to the mini icon mapped to the type.
    QPixmap m_largePixmap;
};

} // openstudio

Q_DECLARE_METATYPE(openstudio::OSItemId)
Q_DECLARE_METATYPE(std::vector<openstudio::OSItemId>)

#endif // OPENSTUDIO_OSITEM_HPP
