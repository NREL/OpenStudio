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

#ifndef OPENSTUDIO_OSITEM_HPP
#define OPENSTUDIO_OSITEM_HPP

#include <shared_gui_components/LocalLibrary.hpp>

#include <QVariant>
#include <QWidget>
#include <vector>

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

class OSItem : public QWidget
{
  Q_OBJECT

  public:

    // creates a new OSItem if possible
    static OSItem* makeItem(const OSItemId& itemId, OSItemType osItemType = OSItemType::ListItem);

    OSItem(const OSItemId& itemId, OSItemType osItemType = OSItemType::ListItem, QWidget * parent = 0);

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

  private slots:
    void onRemoveClicked();

  protected:
    void paintEvent(QPaintEvent * event);

    void mouseReleaseEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent*event);
    void leaveEvent(QEvent* event);
    //void resizeEvent(QResizeEvent* event);
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

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
