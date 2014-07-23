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

#include "OSInspectorView.hpp"
#include "ModelObjectItem.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../utilities/core/Assert.hpp"

#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QScrollArea>
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

OSInspectorView::OSInspectorView(bool addScrollArea, QWidget * parent)
  : QWidget(parent)
{
  this->setObjectName("GrayWidget");

  QVBoxLayout* outerVLayout = new QVBoxLayout();
  outerVLayout->setContentsMargins(0,0,0,0);
  this->setLayout(outerVLayout);

  m_stackedWidget = new QStackedWidget();

  if (addScrollArea){
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setFrameStyle(QFrame::NoFrame);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    outerVLayout->addWidget(scrollArea);
    scrollArea->setWidget(m_stackedWidget);
    scrollArea->setWidgetResizable(true);
  }else{
    outerVLayout->addWidget(m_stackedWidget);
  }

}

void OSInspectorView::clearSelection()
{
  onClearSelection();
}

void OSInspectorView::selectItem(OSItem* item)
{
  onSelectItem(item);
}


QStackedWidget* OSInspectorView::stackedWidget() const
{
  return m_stackedWidget;
}

void OSInspectorView::update()
{
  onUpdate();
}


} // openstudio

