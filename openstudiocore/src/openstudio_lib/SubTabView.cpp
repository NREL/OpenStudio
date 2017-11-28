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

