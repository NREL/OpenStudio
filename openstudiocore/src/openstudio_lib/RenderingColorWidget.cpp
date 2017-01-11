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

#include "RenderingColorWidget.hpp"

#include "../model/RenderingColor_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include "../model/BuildingStory.hpp"
#include "../model/BuildingStory_Impl.hpp"
#include "../model/ConstructionBase.hpp"
#include "../model/ConstructionBase_Impl.hpp"
#include "../model/LightingSimulationZone.hpp"
#include "../model/LightingSimulationZone_Impl.hpp"
#include "../model/SpaceType.hpp"
#include "../model/SpaceType_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"

#include <QBoxLayout>
#include <QColor>
#include <QColorDialog>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

namespace openstudio {

  RenderingColorWidget2::RenderingColorWidget2(QWidget * parent)
    : QWidget(parent)
  {
    auto layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);

    m_renderColorButton = new QPushButton();
    m_renderColorButton->setFixedSize(QSize(20, 20));
    layout->addWidget(m_renderColorButton);

    bool isConnected = connect(m_renderColorButton, SIGNAL(clicked()), this, SLOT(renderColorButtonClicked()));
    OS_ASSERT(isConnected);
  }

  void RenderingColorWidget2::bind(model::ModelObject & modelObject,
    OptionalModelObjectGetter get,
    ModelObjectSetter set)
  {
    m_get = get;
    m_set = set;
    m_modelObject = modelObject;    

    setRenderingColor();
  }

  void RenderingColorWidget2::setRenderingColor()
  {
    if (m_modelObject) {
      if (boost::optional<model::BuildingStory> bs = m_modelObject->optionalCast<model::BuildingStory>())
      {
        m_renderingColor = bs->renderingColor();
      }
      else if (boost::optional<model::ConstructionBase> cb = m_modelObject->optionalCast<model::ConstructionBase>())
      {
        m_renderingColor = cb->renderingColor();
      }
      else if (boost::optional<model::LightingSimulationZone> lsz = m_modelObject->optionalCast<model::LightingSimulationZone>())
      {
        m_renderingColor = lsz->renderingColor();
      }
      else if (boost::optional<model::SpaceType> st = m_modelObject->optionalCast<model::SpaceType>())
      {
        m_renderingColor = st->renderingColor();
      }
      else if (boost::optional<model::ThermalZone> tz = m_modelObject->optionalCast<model::ThermalZone>())
      {
        m_renderingColor = tz->renderingColor();
      }
      else
      {
        // Should never get here
        OS_ASSERT(false);
      }
    }

    if (!m_renderingColor){
      // app->processEvents not sufficient; must use singleShot
      // to prevent race condition stack overflow
      QTimer::singleShot(0, this, SLOT(getRenderingColor()));
    }
    else{
      refresh();
    }
  }

  void RenderingColorWidget2::getRenderingColor()
  {
    bool isConnected = false;

    if (m_modelObject) {
      if (boost::optional<model::BuildingStory> bs = m_modelObject->optionalCast<model::BuildingStory>())
      {
        m_renderingColor = model::RenderingColor(bs->model());
        bs->setRenderingColor(*m_renderingColor);
        bs->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.connect<RenderingColorWidget2, &RenderingColorWidget2::refresh>(this);
      }
      else if (boost::optional<model::ConstructionBase> cb = m_modelObject->optionalCast<model::ConstructionBase>())
      {
        m_renderingColor = model::RenderingColor(cb->model());
        cb->setRenderingColor(*m_renderingColor);
        cb->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.connect<RenderingColorWidget2, &RenderingColorWidget2::refresh>(this);
      }
      else if (boost::optional<model::LightingSimulationZone> lsz = m_modelObject->optionalCast<model::LightingSimulationZone>())
      {
        m_renderingColor = model::RenderingColor(lsz->model());
        lsz->setRenderingColor(*m_renderingColor);
        lsz->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.connect<RenderingColorWidget2, &RenderingColorWidget2::refresh>(this);
      }
      else if (boost::optional<model::SpaceType> st = m_modelObject->optionalCast<model::SpaceType>())
      {
        m_renderingColor = model::RenderingColor(st->model());
        st->setRenderingColor(*m_renderingColor);
        st->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.connect<RenderingColorWidget2, &RenderingColorWidget2::refresh>(this);
      }
      else if (boost::optional<model::ThermalZone> tz = m_modelObject->optionalCast<model::ThermalZone>())
      {
        m_renderingColor = model::RenderingColor(tz->model());
        tz->setRenderingColor(*m_renderingColor);
        tz->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.connect<RenderingColorWidget2, &RenderingColorWidget2::refresh>(this);
      }
      else
      {
        // Should never get here
        OS_ASSERT(false);
      }

      //isConnected = connect(m_modelObject->getImpl<openstudio::model::detail::ModelObject_Impl>().get(), SIGNAL(onChange()), this, SLOT(refresh()));
      //OS_ASSERT(isConnected);

      refresh();
    }
  }

  void RenderingColorWidget2::unbind()
  {
    this->blockSignals(true); // Incompatible with Nano Signal Slot
    clear();
    this->blockSignals(false); // Incompatible with Nano Signal Slot

    if (m_modelObject) {
      if (boost::optional<model::BuildingStory> bs = m_modelObject->optionalCast<model::BuildingStory>())
      {
        bs->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.disconnect<RenderingColorWidget2, &RenderingColorWidget2::refresh>(this);
      }
      else if (boost::optional<model::ConstructionBase> cb = m_modelObject->optionalCast<model::ConstructionBase>())
      {
        cb->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.disconnect<RenderingColorWidget2, &RenderingColorWidget2::refresh>(this);
      }
      else if (boost::optional<model::LightingSimulationZone> lsz = m_modelObject->optionalCast<model::LightingSimulationZone>())
      {
        lsz->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.disconnect<RenderingColorWidget2, &RenderingColorWidget2::refresh>(this);
      }
      else if (boost::optional<model::SpaceType> st = m_modelObject->optionalCast<model::SpaceType>())
      {
        st->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.disconnect<RenderingColorWidget2, &RenderingColorWidget2::refresh>(this);
      }
      else if (boost::optional<model::ThermalZone> tz = m_modelObject->optionalCast<model::ThermalZone>())
      {
        tz->getImpl<openstudio::model::detail::ModelObject_Impl>().get()->onChange.disconnect<RenderingColorWidget2, &RenderingColorWidget2::refresh>(this);
      }
    }

    m_modelObject.reset();
    m_get.reset();
    m_set.reset();

    refresh();
  }

  void RenderingColorWidget2::clear()
  {
    QString style;
    style.append("QPushButton { ");
    style.append("              border-radius: 0px; ");
    style.append("              border: 1px solid #949393; ");
    style.append("              background : rgba(255,255,255,255); ");
    style.append("              margin: 0; ");
    style.append("              padding-left: 0px; ");
    style.append("              padding-right: 0px; ");
    style.append("              padding-top: 0px; ");
    style.append("              padding-bottom: 0px; ");
    style.append("            } ");

    m_renderColorButton->setStyleSheet(style);
  }

  void RenderingColorWidget2::refresh()
  {
    this->blockSignals(true);
    clear();
    this->blockSignals(false);

    if (m_renderingColor){

      int r = m_renderingColor->renderingRedValue();
      int g = m_renderingColor->renderingGreenValue();
      int b = m_renderingColor->renderingBlueValue();
      int a = m_renderingColor->renderingAlphaValue();

      QString style;
      style.append("QPushButton { ");
      style.append("              border-radius: 0px; ");
      style.append("              border: 1px solid #949393; ");
      style.append("              background : rgba(" + QString::number(r) + "," + QString::number(g) + ", " + QString::number(b) + ", " + QString::number(a) + "); ");
      style.append("              margin: 0; ");
      style.append("              padding-left: 0px; ");
      style.append("              padding-right: 0px; ");
      style.append("              padding-top: 0px; ");
      style.append("              padding-bottom: 0px; ");
      style.append("            } ");

      m_renderColorButton->setStyleSheet(style);
    }
  }

  void RenderingColorWidget2::renderColorButtonClicked()
  {
    if (m_renderingColor){
      int r = m_renderingColor->renderingRedValue();
      int g = m_renderingColor->renderingGreenValue();
      int b = m_renderingColor->renderingBlueValue();
      int a = m_renderingColor->renderingAlphaValue();
      QColor initialColor = QColor(r, g, b, a);

      QColor color = QColorDialog::getColor(initialColor, this, "Choose Rendering Color", QColorDialog::ShowAlphaChannel);

      if (color.isValid() && color != initialColor){
        m_renderingColor->setRenderingRedValue(color.red());
        m_renderingColor->setRenderingGreenValue(color.green());
        m_renderingColor->setRenderingBlueValue(color.blue());
        m_renderingColor->setRenderingAlphaValue(color.alpha());

        refresh();
      }
    }
  }

RenderingColorWidget::RenderingColorWidget(QWidget * parent )
  : QWidget(parent)
{
  this->setObjectName("GrayWidget");

  auto hLayout = new QHBoxLayout();
  hLayout->setContentsMargins(0,0,0,0);
  hLayout->setSpacing(10);
  this->setLayout(hLayout);

  auto renderingColorLabel = new QLabel();
  renderingColorLabel->setText("Rendering Color: ");
  renderingColorLabel->setStyleSheet("QLabel { font: bold; }");
  hLayout->addWidget(renderingColorLabel);

  m_renderColorWidget = new QWidget();
  m_renderColorWidget->setFixedHeight(30);
  m_renderColorWidget->setFixedWidth(30);
  hLayout->addWidget(m_renderColorWidget);

  m_renderColorButton = new QPushButton();
  m_renderColorButton->setFlat(true); 
  m_renderColorButton->setText("Select Color");
  m_renderColorButton->setObjectName("StandardGrayButton");
  hLayout->addWidget(m_renderColorButton);
  hLayout->addStretch();

  connect(m_renderColorButton, &QPushButton::clicked, this, &RenderingColorWidget::renderColorButtonClicked);
}

void RenderingColorWidget::attach(const openstudio::model::RenderingColor& renderingColor)
{
  detach();

  m_renderingColor = renderingColor;

  m_renderingColor->getImpl<model::detail::ModelObject_Impl>().get()->onChange.connect<RenderingColorWidget, &RenderingColorWidget::refresh>(this);

  refresh();
}

void RenderingColorWidget::detach()
{
  clear();

  if (m_renderingColor){
    m_renderingColor->getImpl<model::detail::ModelObject_Impl>().get()->onChange.disconnect<RenderingColorWidget, &RenderingColorWidget::refresh>(this);
    m_renderingColor.reset();
  }
}

void RenderingColorWidget::clear()
{
  QString style = "QWidget { background-color : rgba(255,255,255,255);}";
  m_renderColorWidget->setStyleSheet(style);
  m_renderColorButton->setEnabled(false);
}

void RenderingColorWidget::refresh()
{
  clear();

  if (m_renderingColor){

    int r = m_renderingColor->renderingRedValue();
    int g = m_renderingColor->renderingGreenValue();
    int b = m_renderingColor->renderingBlueValue();
    int a = m_renderingColor->renderingAlphaValue();
    QString style = "QWidget { background-color : rgba(" + QString::number(r) + "," + QString::number(g) + ", " + QString::number(b) + ", " + QString::number(a) + ");}";
    m_renderColorWidget->setStyleSheet(style);
    m_renderColorButton->setEnabled(true);
  }
}

void RenderingColorWidget::renderColorButtonClicked()
{
  if(m_renderingColor){
    int r = m_renderingColor->renderingRedValue();
    int g = m_renderingColor->renderingGreenValue();
    int b = m_renderingColor->renderingBlueValue();
    int a = m_renderingColor->renderingAlphaValue();
    QColor initialColor = QColor(r, g, b, a);
  
    QColor color = QColorDialog::getColor(initialColor, this, "Choose Rendering Color", QColorDialog::ShowAlphaChannel);

    if (color.isValid()){
      m_renderingColor->setRenderingRedValue(color.red());
      m_renderingColor->setRenderingGreenValue(color.green());
      m_renderingColor->setRenderingBlueValue(color.blue());
      m_renderingColor->setRenderingAlphaValue(color.alpha());

      refresh();
    }
  }
}

} // openstudio

