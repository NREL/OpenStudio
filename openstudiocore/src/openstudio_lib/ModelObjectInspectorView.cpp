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

#include "ModelObjectInspectorView.hpp"
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

ModelObjectInspectorView::ModelObjectInspectorView(const openstudio::model::Model& model, bool addScrollArea, QWidget * parent)
  : OSInspectorView(addScrollArea, parent),
    m_model(model)
{
  bool isConnected = connect(this, 
                             SIGNAL(toggleUnitsClicked(bool)),
                             this, 
                             SLOT(toggleUnits(bool)));
  OS_ASSERT(isConnected);
}

void ModelObjectInspectorView::selectModelObject(const openstudio::model::ModelObject& modelObject)
{
  if (m_modelObject){
    this->disconnect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get());
  }

  m_modelObject = modelObject;

  bool isConnected = connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(),
                             SIGNAL(onChange()),
                             this,
                             SLOT(update()));
  OS_ASSERT(isConnected);

  onSelectModelObject(*m_modelObject);
}

void ModelObjectInspectorView::onClearSelection()
{
  if (m_modelObject){
    this->disconnect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get());
  }

  m_modelObject.reset();
}

void ModelObjectInspectorView::onSelectItem(OSItem *item)
{
  ModelObjectItem* modelObjectItem = qobject_cast<ModelObjectItem*>(item);
  OS_ASSERT(modelObjectItem);
  selectModelObject(modelObjectItem->modelObject());
}

boost::optional<openstudio::model::ModelObject> ModelObjectInspectorView::modelObject() const
{
  return m_modelObject;
}

void ModelObjectInspectorView::toggleUnits(bool displayIP)
{
}

DefaultInspectorView::DefaultInspectorView(const model::Model& model,
                                           QWidget * parent )
  : ModelObjectInspectorView(model, true, parent)
{
  QWidget* hiddenWidget = new QWidget();
  this->stackedWidget()->insertWidget(0, hiddenWidget);

  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->insertWidget(1, visibleWidget);

  this->stackedWidget()->setCurrentIndex(0);

  QVBoxLayout* mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(7,7,7,7);
  mainVLayout->setSpacing(7);

  QLabel * underConstructionLabel = new QLabel();
  underConstructionLabel->setPixmap(QPixmap(":/images/coming_soon_building_summary.png"));
  underConstructionLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  mainVLayout->addWidget(underConstructionLabel);

  visibleWidget->setLayout(mainVLayout);

  underConstructionLabel = new QLabel();
  underConstructionLabel->setPixmap(QPixmap(":/images/coming_soon_building_summary.png"));
  underConstructionLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  this->stackedWidget()->addWidget(underConstructionLabel);

  m_mainLabel = new QLabel(this);
  m_mainLabel->setWordWrap(true);
  m_mainLabel->setText("Initial");
  m_mainLabel->setAlignment(Qt::AlignHCenter);
  m_mainLabel->setAlignment(Qt::AlignVCenter);
  
  mainVLayout->addWidget(underConstructionLabel);
  mainVLayout->addWidget(m_mainLabel);
}

void DefaultInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  this->stackedWidget()->setCurrentIndex(0);
}

void DefaultInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  std::stringstream ss;
  ss << modelObject;
  m_mainLabel->setText(toQString(ss.str()));

  this->stackedWidget()->setCurrentIndex(1);
}

void DefaultInspectorView::onUpdate()
{
  boost::optional<openstudio::model::ModelObject> modelObject = this->modelObject();
  OS_ASSERT(modelObject);

  std::stringstream ss;
  ss << *modelObject;
  m_mainLabel->setText(toQString(ss.str()));
}

} // openstudio

