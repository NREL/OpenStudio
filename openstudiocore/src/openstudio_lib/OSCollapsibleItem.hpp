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

#ifndef OPENSTUDIO_OSCOLLAPSIBLEITEM_HPP
#define OPENSTUDIO_OSCOLLAPSIBLEITEM_HPP

#include <QWidget>
#include "OSItem.hpp"

class QButtonGroup;
class QComboBox;
class QLabel;
class QPushButton;
class QRadioButton;
class QVBoxLayout;

namespace openstudio {

class OSCollapsibleItemHeader;
class OSItemId;
class OSItemList;

class OSCollapsibleItem : public QWidget
{
  Q_OBJECT

  public:

    OSCollapsibleItem(OSCollapsibleItemHeader * collapsibleItemHeader,
                      OSItemList * itemList,
                      QWidget * parent = 0);

    virtual ~OSCollapsibleItem() {}

    bool isSelected() const;
    void setSelected(bool selected);

    bool expanded() const;
    void setExpanded(bool expanded);

    OSCollapsibleItemHeader* collapsibleItemHeader() const;

    OSItemList* itemList() const;

    bool itemsDraggable() const;
    void setItemsDraggable(bool itemsDraggable);

    bool itemsRemoveable() const;
    void setItemsRemoveable(bool itemsRemoveable);

    OSItemType itemsType() const;
    void setItemsType(OSItemType type);

    bool showFilterLayout();
    void setShowFilterLayout(const bool showFilterLayout);

  signals:

    void collapsableItemSelected(OSCollapsibleItem *);

    void itemSelected(OSItem* item);

    void itemRemoveClicked(OSItem* item);

    void itemReplacementDropped(OSItem * currentItem, const OSItemId& replacementItemId);

    void selectionCleared();

    void openLibDlgClicked();

  protected:

    void paintEvent(QPaintEvent * event);

  private slots:

    void onHeaderClicked(OSCollapsibleItemHeader * header);

    void filtersOnClicked();

    void filtersOffClicked();

    void comboBoxClicked(const QString & string);

  private:
    void createLayout();

    void setShowFilterWidgets(const bool showFilterWidgets);

    OSCollapsibleItemHeader * m_collapsibleItemHeader;
    OSItemList * m_itemList;
    QVBoxLayout * m_mainLayout;
    QPushButton * m_openLibDlgButton;
    QRadioButton * m_filtersOnBtn;
    QRadioButton * m_filtersOffBtn;
    QButtonGroup * m_filterBtnGroup;
    QLabel * m_sortLabel;
    QComboBox * m_sortComboBox;
    bool m_showFilterLayout;
    QWidget * m_filterWidget;
};

} // openstudio

#endif // OPENSTUDIO_OSCOLLAPSIBLEITEM_HPP
