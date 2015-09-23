/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "SubTabView.hpp"
#include "OSItemSelector.hpp"
#include "OSItemSelectorButtons.hpp"
#include "ModelObjectListView.hpp"
#include "ModelObjectTypeListView.hpp"
#include "ModelObjectItem.hpp"
#include "ModelObjectInspectorView.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../utilities/core/Assert.hpp"

#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QButtonGroup>
#include <QResizeEvent>
#include <QTimer>
#include <QMouseEvent>
#include <QCalendarWidget>
#include <QGridLayout>
#include <QRadioButton>

#include <iostream>

namespace openstudio {

  SubTabView::SubTabView(OSItemSelector* itemSelector,
    OSInspectorView *inspectorView,
    bool showGridViewLayout,
    QWidget * parent)
  : QSplitter(parent),
  m_itemSelector(itemSelector),
  m_inspectorView(inspectorView)
{
  this->setObjectName("GrayWidgetWithLeftTopBorders");
  connectItemSelector();
  connectInspectorView();
  connectItemSelectorButtons();
  if (showGridViewLayout){
    createGridViewLayout();
  }
  else {
    createLayout();
  }
}

void SubTabView::connectItemSelector()
{
  // Item Selector
  connect(m_itemSelector, &OSItemSelector::itemSelected, this, &SubTabView::itemSelected);

  connect(m_itemSelector, &OSItemSelector::itemRemoveClicked, this, &SubTabView::itemRemoveClicked);

  connect(m_itemSelector, &OSItemSelector::itemReplacementDropped, this, &SubTabView::itemReplacementDropped);

  connect(m_itemSelector, &OSItemSelector::selectionCleared, this, &SubTabView::selectionCleared);
}

void SubTabView::connectInspectorView()
{
  // Inspector View
  connect(m_inspectorView, &OSInspectorView::dropZoneItemClicked, this, &SubTabView::dropZoneItemClicked);

  auto isConnected = connect(this, SIGNAL(dropZoneItemClicked(OSItem*)), this, SLOT(onDropZoneItemClicked(OSItem*)));
  OS_ASSERT(isConnected);
}

void SubTabView::connectItemSelectorButtons()
{
  // Item Selector Buttons
  m_itemSelectorButtons = new OSItemSelectorButtons();

  connect(m_itemSelectorButtons, &OSItemSelectorButtons::itemDropped, this, &SubTabView::itemDropped);

  connect(m_itemSelectorButtons, &OSItemSelectorButtons::addClicked, this, &SubTabView::addClicked);

  connect(m_itemSelectorButtons, &OSItemSelectorButtons::copyClicked, this, &SubTabView::copyClicked);

  connect(m_itemSelectorButtons, &OSItemSelectorButtons::removeClicked, this, &SubTabView::removeClicked);

  connect(m_itemSelectorButtons, &OSItemSelectorButtons::purgeClicked, this, &SubTabView::purgeClicked);

  connect(m_itemSelectorButtons, &OSItemSelectorButtons::downloadComponentsClicked, this, &SubTabView::downloadComponentsClicked);
}

void SubTabView::createLayout()
{
  auto leftWidget = new QWidget();
  addWidget(leftWidget);

  auto outerLeftVLayout = new QVBoxLayout();
  outerLeftVLayout->setContentsMargins(0, 0, 0, 0);
  outerLeftVLayout->setSpacing(0);
  leftWidget->setLayout(outerLeftVLayout);

  outerLeftVLayout->addWidget(m_itemSelector, 10);

  outerLeftVLayout->addWidget(m_itemSelectorButtons);

  addWidget(m_inspectorView);
  setStretchFactor(1, 100000);
}

void SubTabView::createGridViewLayout()
{
  auto widget = new QWidget();
  addWidget(widget);

  auto layout = new QVBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  widget->setLayout(layout);

  layout->addWidget(m_inspectorView);

  m_itemSelectorButtons->hideDropZone();
  layout->addWidget(m_itemSelectorButtons);

  //setStretchFactor(1, 100000);
}

OSItemSelector* SubTabView::itemSelector()
{
  return m_itemSelector;
}

OSItemSelectorButtons* SubTabView::itemSelectorButtons()
{
  return m_itemSelectorButtons;
}

OSInspectorView* SubTabView::inspectorView()
{
  return m_inspectorView;
}

const OSItemSelector* SubTabView::itemSelector() const
{
  return m_itemSelector;
}

const OSItemSelectorButtons* SubTabView::itemSelectorButtons() const
{
  return m_itemSelectorButtons;
}

const OSInspectorView* SubTabView::inspectorView() const
{
  return m_inspectorView;
}

void SubTabView::paintEvent ( QPaintEvent * event )
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void SubTabView::onDropZoneItemClicked(OSItem* item)
{
}

} // openstudio

