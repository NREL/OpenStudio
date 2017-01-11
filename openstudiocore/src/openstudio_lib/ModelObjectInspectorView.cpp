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

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectItem.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../utilities/core/Assert.hpp"

#include <nano/nano_signal_slot.hpp>
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
  connect(this, &ModelObjectInspectorView::toggleUnitsClicked, this, &ModelObjectInspectorView::toggleUnits);
}

void ModelObjectInspectorView::update()
{
  /** Nano Signal onChange originally modified a slot directly inside
   *  QWidget called update(). This is fine, except for when we need
   *  to automatically disconnect. Now this nano signal calls this slot,
   *  And this slot automatically emits the QSignal that will call 
   *  call QWidget::update().
   */
  // this->onChange.nano_emit();
  //emit onChange();
}

void ModelObjectInspectorView::selectModelObject(const openstudio::model::ModelObject& modelObject)
{
  if (m_modelObject){
    m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.disconnect<ModelObjectInspectorView, &ModelObjectInspectorView::update>(this);
  }

  m_modelObject = modelObject;

  m_modelObject->getImpl<model::detail::ModelObject_Impl>().get()->onChange.connect<ModelObjectInspectorView, &ModelObjectInspectorView::update>(this);

  onSelectModelObject(*m_modelObject);
}

void ModelObjectInspectorView::onClearSelection()
{
  if (m_modelObject){
    m_modelObject->getImpl<model::detail::ModelObject_Impl>().get()->onChange.disconnect<ModelObjectInspectorView, &ModelObjectInspectorView::update>(this);
  }

  m_modelObject.reset();
}

void ModelObjectInspectorView::onSelectItem(OSItem *item)
{
  ModelObjectItem* modelObjectItem = qobject_cast<ModelObjectItem*>(item);
  OS_ASSERT(modelObjectItem);
  selectModelObject(modelObjectItem->modelObject());
}

std::vector<model::ModelObject> ModelObjectInspectorView::selectedObjects() const
{
  // Default implementation. The tabs with grid views need to do their own thing here
  return std::vector<model::ModelObject>();
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
  auto hiddenWidget = new QWidget();
  this->stackedWidget()->insertWidget(0, hiddenWidget);

  auto visibleWidget = new QWidget();
  this->stackedWidget()->insertWidget(1, visibleWidget);

  this->stackedWidget()->setCurrentIndex(0);

  auto mainVLayout = new QVBoxLayout();
  mainVLayout->setContentsMargins(7,7,7,7);
  mainVLayout->setSpacing(7);

  auto underConstructionLabel = new QLabel();
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

