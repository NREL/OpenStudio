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
                       QWidget * parent)
  : QSplitter(parent),
    m_itemSelector(itemSelector),
    m_inspectorView(inspectorView)
{
  this->setObjectName("GrayWidgetWithLeftTopBorders");

  //QHBoxLayout * mainHLayout = new QHBoxLayout();
  //mainHLayout->setContentsMargins(1,1,0,0);
  //mainHLayout->setSpacing(0);
  //this->setLayout(mainHLayout);

  QWidget * leftWidget = new QWidget();
  //leftWidget->setFixedWidth(190);
  //mainHLayout->addWidget(leftWidget);
  addWidget(leftWidget);
  
  QVBoxLayout * outerLeftVLayout = new QVBoxLayout();
  outerLeftVLayout->setContentsMargins(0,0,0,0);
  outerLeftVLayout->setSpacing(0);
  leftWidget->setLayout(outerLeftVLayout);
  //mainHLayout->addLayout(outerLeftVLayout);

  // Item Selector
  outerLeftVLayout->addWidget(m_itemSelector, 10);

  bool isConnected = false;
  isConnected = QObject::connect(m_itemSelector,
                                 SIGNAL(itemSelected(OSItem*)),
                                 this,
                                 SIGNAL(itemSelected(OSItem*)));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_itemSelector,
                                 SIGNAL(itemRemoveClicked(OSItem*)),
                                 this,
                                 SIGNAL(itemRemoveClicked(OSItem*)));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_itemSelector,
                                 SIGNAL(itemReplacementDropped(OSItem*, const OSItemId&)),
                                 this,
                                 SIGNAL(itemReplacementDropped(OSItem*, const OSItemId&)));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_itemSelector,
                                 SIGNAL(selectionCleared()),
                                 this,
                                 SIGNAL(selectionCleared()));
  OS_ASSERT(isConnected);


  // Item Selector Buttons
  m_itemSelectorButtons = new OSItemSelectorButtons();
  outerLeftVLayout->addWidget(m_itemSelectorButtons);
  
  isConnected = QObject::connect(m_itemSelectorButtons,
                                 SIGNAL(itemDropped(const OSItemId&)),
                                 this,
                                 SIGNAL(itemDropped(const OSItemId&)));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_itemSelectorButtons,
                                 SIGNAL(addClicked()),
                                 this,
                                 SIGNAL(addClicked()));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_itemSelectorButtons,
                                 SIGNAL(copyClicked()),
                                 this,
                                 SIGNAL(copyClicked()));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_itemSelectorButtons,
                                 SIGNAL(removeClicked()),
                                 this,
                                 SIGNAL(removeClicked()));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_itemSelectorButtons,
                                 SIGNAL(purgeClicked()),
                                 this,
                                 SIGNAL(purgeClicked()));
  OS_ASSERT(isConnected);

  isConnected = QObject::connect(m_itemSelectorButtons,
                                 SIGNAL(downloadComponentsClicked()),
                                 this,
                                 SIGNAL(downloadComponentsClicked()));
  OS_ASSERT(isConnected);

  //isConnected = QObject::connect(m_itemSelectorButtons,
  //                               SIGNAL(openLibDlgClicked()),
  //                               this,
  //                               SIGNAL(openLibDlgClicked()));
  //OS_ASSERT(isConnected); TODO

  // vertical separator
  //QWidget * vLine = new QWidget();
  //vLine->setObjectName("VLine");
  //vLine->setStyleSheet("QWidget#VLine { background: #445051;}");
  //vLine->setFixedWidth(2);
  //mainHLayout->addWidget(vLine);

  // Inspector View
  //mainHLayout->addWidget(m_inspectorView, 10);
  addWidget(m_inspectorView);
  setStretchFactor(1,100000);

  isConnected = QObject::connect(m_inspectorView,
                                 SIGNAL(dropZoneItemClicked(OSItem*)),
                                 this,
                                 SIGNAL(dropZoneItemClicked(OSItem*)));
  OS_ASSERT(isConnected);
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

/*


void SubTabView::addClicked()
{
  ModelObjectListView* modelObjectListView = qobject_cast<ModelObjectListView*>(m_itemSelector);
  if (modelObjectListView){
    IddObjectType iddObjectType = modelObjectListView->iddObjectType();
    emit addObjectClicked(iddObjectType);
  }

  ModelObjectTypeListView* modelObjectTypeListView = qobject_cast<ModelObjectTypeListView*>(m_itemSelector);
  if (modelObjectTypeListView){
    IddObjectType iddObjectType = modelObjectTypeListView->currentIddObjectType();
    emit addObjectClicked(iddObjectType);
  }
}

void SubTabView::copyClicked()
{
  ModelObjectListView* modelObjectListView = qobject_cast<ModelObjectListView*>(m_itemSelector);
  if (modelObjectListView){
    boost::optional<openstudio::model::ModelObject> selectedModelObject = modelObjectListView->selectedModelObject();
    if (selectedModelObject){
      emit copyObjectClicked(*selectedModelObject);
    }
  }

  ModelObjectTypeListView* modelObjectTypeListView = qobject_cast<ModelObjectTypeListView*>(m_itemSelector);
  if (modelObjectTypeListView){
    boost::optional<openstudio::model::ModelObject> selectedModelObject = modelObjectTypeListView->selectedModelObject();
    if (selectedModelObject){
      emit copyObjectClicked(*selectedModelObject);
    }
  }

}

void SubTabView::removeClicked()
{
  ModelObjectListView* modelObjectListView = qobject_cast<ModelObjectListView*>(m_itemSelector);
  if (modelObjectListView){
    boost::optional<openstudio::model::ModelObject> selectedModelObject = modelObjectListView->selectedModelObject();
    if (selectedModelObject){
      emit removeObjectClicked(*selectedModelObject);
    }
  }

  ModelObjectTypeListView* modelObjectTypeListView = qobject_cast<ModelObjectTypeListView*>(m_itemSelector);
  if (modelObjectTypeListView){
    boost::optional<openstudio::model::ModelObject> selectedModelObject = modelObjectTypeListView->selectedModelObject();
    if (selectedModelObject){
      emit removeObjectClicked(*selectedModelObject);
    }
  }

}

void SubTabView::purgeClicked()
{
  ModelObjectListView* modelObjectListView = qobject_cast<ModelObjectListView*>(m_itemSelector);
  if (modelObjectListView){
    IddObjectType iddObjectType = modelObjectListView->iddObjectType();
    emit purgeObjectsClicked(iddObjectType);
  }

  ModelObjectTypeListView* modelObjectTypeListView = qobject_cast<ModelObjectTypeListView*>(m_itemSelector);
  if (modelObjectTypeListView){
    IddObjectType iddObjectType = modelObjectTypeListView->currentIddObjectType();
    emit purgeObjectsClicked(iddObjectType);
  }
}
*/

} // openstudio

