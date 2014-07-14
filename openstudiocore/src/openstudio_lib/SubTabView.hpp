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

#ifndef OPENSTUDIO_SUBTABVIEW_HPP
#define OPENSTUDIO_SUBTABVIEW_HPP

#include "MainTabView.hpp"
#include "OSInspectorView.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"

//#include <QWidget>
#include <QSplitter>

class QPushButton;
class QScrollArea;

namespace openstudio {

class OSItemSelector;
class OSItemSelectorButtons;
class OSItem;
class OSItemId;

class SubTabView : public QSplitter
{
  Q_OBJECT

  public:

    SubTabView(OSItemSelector* itemSelector,
               OSInspectorView* inspectorView,
               QWidget* parent = 0);

    virtual ~SubTabView() {}

    OSItemSelector* itemSelector();

    OSItemSelectorButtons* itemSelectorButtons();

    OSInspectorView* inspectorView();

    const OSItemSelector* itemSelector() const;

    const OSItemSelectorButtons* itemSelectorButtons() const;

    const OSInspectorView* inspectorView() const;

  signals:

    void itemSelected(OSItem* item);

    void itemRemoveClicked(OSItem* item);

    void itemReplacementDropped(OSItem * currentItem, const OSItemId& replacementItemId);

    void selectionCleared();

    void itemDropped(const OSItemId& itemId);

    void addClicked();

    void copyClicked();

    void removeClicked();

    void purgeClicked();
    
    void dropZoneItemClicked(OSItem* item);

    void downloadComponentsClicked();

    void openLibDlgClicked();

  protected:

    void paintEvent ( QPaintEvent * event );

  private:

    QScrollArea* m_selectorScrollArea;

    OSItemSelector* m_itemSelector;

    OSItemSelectorButtons* m_itemSelectorButtons;

    OSInspectorView* m_inspectorView;
};


} // openstudio

#endif // OPENSTUDIO_SUBTABVIEW_HPP

