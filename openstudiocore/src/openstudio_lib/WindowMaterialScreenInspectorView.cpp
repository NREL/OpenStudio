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

#include "WindowMaterialScreenInspectorView.hpp"

#include "StandardsInformationMaterialWidget.hpp"

#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"

#include "../model/Screen.hpp"
#include "../model/Screen_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

// WindowMaterialScreenInspectorView

WindowMaterialScreenInspectorView::WindowMaterialScreenInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
    m_isIP(isIP)
{
  createLayout();
}

void WindowMaterialScreenInspectorView::createLayout()
{
  auto hiddenWidget = new QWidget();
  this->stackedWidget()->addWidget(hiddenWidget);

  auto visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  auto mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7, 7, 7, 7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  int row = mainGridLayout->rowCount();

  QLabel * label = nullptr;

  // Name

  label = new QLabel("Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label, row, 0);

  ++row;

  m_nameEdit = new OSLineEdit2();
  mainGridLayout->addWidget(m_nameEdit, row, 0, 1, 3);

  ++row;

  // Standards Information

  m_standardsInformationWidget = new StandardsInformationMaterialWidget(m_isIP, mainGridLayout, row);

  ++row;

  // Reflected Beam Transmittance Accounting Method

  label = new QLabel("Reflected Beam Transmittance Accounting Method: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_reflectedBeamTransmittanceAccountingMethod = new OSComboBox2();
  m_reflectedBeamTransmittanceAccountingMethod->addItem("Do Not Model");
  m_reflectedBeamTransmittanceAccountingMethod->addItem("Model As Direct Beam");
  m_reflectedBeamTransmittanceAccountingMethod->addItem("Model As Diffuse");
  mainGridLayout->addWidget(m_reflectedBeamTransmittanceAccountingMethod,row++,0,1,3);

  // Diffuse Solar Reflectance

  label = new QLabel("Diffuse Solar Reflectance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_diffuseSolarReflectance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialScreenInspectorView::toggleUnitsClicked, m_diffuseSolarReflectance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_diffuseSolarReflectance,row++,0,1,3);

  // Diffuse Visible Reflectance

  label = new QLabel("Diffuse Visible Reflectance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_diffuseVisibleReflectance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialScreenInspectorView::toggleUnitsClicked, m_diffuseVisibleReflectance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_diffuseVisibleReflectance,row++,0,1,3);

  // Thermal Hemispherical Emissivity

  label = new QLabel("Thermal Hemispherical Emissivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_thermalHemisphericalEmissivity = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialScreenInspectorView::toggleUnitsClicked, m_thermalHemisphericalEmissivity, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_thermalHemisphericalEmissivity,row++,0,1,3);

  // Conductivity

  label = new QLabel("Conductivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_conductivity = new OSQuantityEdit2("W/m*K", "W/m*K", "Btu*in/hr*ft^2*R", m_isIP);
  connect(this, &WindowMaterialScreenInspectorView::toggleUnitsClicked, m_conductivity, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_conductivity,row++,0,1,3);

  // Screen Material Spacing

  label = new QLabel("Screen Material Spacing: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_screenMaterialSpacing = new OSQuantityEdit2("m","m","in", m_isIP);
  connect(this, &WindowMaterialScreenInspectorView::toggleUnitsClicked, m_screenMaterialSpacing, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_screenMaterialSpacing,row++,0,1,3);

  // Screen Material Diameter

  label = new QLabel("Screen Material Diameter: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_screenMaterialDiameter = new OSQuantityEdit2("m","m","in", m_isIP);
  connect(this, &WindowMaterialScreenInspectorView::toggleUnitsClicked, m_screenMaterialDiameter, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_screenMaterialDiameter,row++,0,1,3);

  // Screen To Glass Distance

  label = new QLabel("Screen To Glass Distance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_screenToGlassDistance = new OSQuantityEdit2("m","m","in", m_isIP);
  connect(this, &WindowMaterialScreenInspectorView::toggleUnitsClicked, m_screenToGlassDistance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_screenToGlassDistance,row++,0,1,3);

  // Top Opening Multiplier

  label = new QLabel("Top Opening Multiplier: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_topOpeningMultiplier = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialScreenInspectorView::toggleUnitsClicked, m_topOpeningMultiplier, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_topOpeningMultiplier,row++,0,1,3);

  // Bottom Opening Multiplier

  label = new QLabel("Bottom Opening Multiplier: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_bottomOpeningMultiplier = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialScreenInspectorView::toggleUnitsClicked, m_bottomOpeningMultiplier, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_bottomOpeningMultiplier,row++,0,1,3);

  // Left Side Opening Multiplier

  label = new QLabel("Left Side Opening Multiplier: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_leftSideOpeningMultiplier = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialScreenInspectorView::toggleUnitsClicked, m_leftSideOpeningMultiplier, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_leftSideOpeningMultiplier,row++,0,1,3);

  // Right Side Opening Multiplier

  label = new QLabel("Right Side Opening Multiplier: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_rightSideOpeningMultiplier = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialScreenInspectorView::toggleUnitsClicked, m_rightSideOpeningMultiplier, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_rightSideOpeningMultiplier,row++,0,1,3);

  // Angle Of Resolution For Screen Transmittance Output Map

  label = new QLabel("Angle Of Resolution For Screen Transmittance Output Map: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_angleOfResolutionForScreenTransmittanceOutputMap = new OSComboBox2();
  m_angleOfResolutionForScreenTransmittanceOutputMap->addItem("0"); // TODO needs OS to bind to model
  m_angleOfResolutionForScreenTransmittanceOutputMap->addItem("2");
  m_angleOfResolutionForScreenTransmittanceOutputMap->addItem("3");
  m_angleOfResolutionForScreenTransmittanceOutputMap->addItem("4");
  m_angleOfResolutionForScreenTransmittanceOutputMap->addItem("5");
  mainGridLayout->addWidget(m_angleOfResolutionForScreenTransmittanceOutputMap,row++,0,1,3);

  // Stretch

  mainGridLayout->setRowStretch(100,100);

  mainGridLayout->setColumnStretch(100,100);
}

void WindowMaterialScreenInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void WindowMaterialScreenInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::Screen screen = modelObject.cast<model::Screen>();
  attach(screen);
  refresh();
}

void WindowMaterialScreenInspectorView::onUpdate()
{
  refresh();
}

void WindowMaterialScreenInspectorView::attach(openstudio::model::Screen & screen)
{
  m_screen = screen;
  // m_reflectedBeamTransmittanceAccountingMethod->bind(screen,"reflectedBeamTransmittanceAccountingMethod");
  m_reflectedBeamTransmittanceAccountingMethod->bind<std::string>(
      *m_screen,
      static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
      &model::Screen::reflectedBeamTransmittanceAccountingMethodValues,
      std::bind(&model::Screen::reflectedBeamTransmittanceAccountingMethod, m_screen.get_ptr()),
      std::bind(&model::Screen::setReflectedBeamTransmittanceAccountingMethod, m_screen.get_ptr(), std::placeholders::_1),
      boost::optional<NoFailAction>(std::bind(&model::Screen::resetReflectedBeamTransmittanceAccountingMethod, m_screen.get_ptr())),
      boost::optional<BasicQuery>(std::bind(&model::Screen::isReflectedBeamTransmittanceAccountingMethodDefaulted, m_screen.get_ptr())));

  // m_angleOfResolutionForScreenTransmittanceOutputMap->bind(screen,"angleofResolutionforScreenTransmittanceOutputMap");
  m_angleOfResolutionForScreenTransmittanceOutputMap->bind<std::string>(
      *m_screen,
      static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
      &model::Screen::angleofResolutionforScreenTransmittanceOutputMapValues,
      std::bind(&model::Screen::angleofResolutionforScreenTransmittanceOutputMap, m_screen.get_ptr()),
      std::bind(&model::Screen::setAngleofResolutionforScreenTransmittanceOutputMap, m_screen.get_ptr(), std::placeholders::_1),
      boost::optional<NoFailAction>(std::bind(&model::Screen::resetAngleofResolutionforScreenTransmittanceOutputMap, m_screen.get_ptr())),
      boost::optional<BasicQuery>(std::bind(&model::Screen::isAngleofResolutionforScreenTransmittanceOutputMapDefaulted, m_screen.get_ptr())));

  // m_nameEdit->bind(screen,"name");
  m_nameEdit->bind(
    *m_screen,
    OptionalStringGetter(std::bind(&model::Screen::name, m_screen.get_ptr(),true)),
    boost::optional<StringSetter>(std::bind(&model::Screen::setName, m_screen.get_ptr(),std::placeholders::_1))
  );

  // m_diffuseSolarReflectance->bind(screen,"diffuseSolarReflectance",m_isIP);
  m_diffuseSolarReflectance->bind(
    m_isIP,
    *m_screen,
    DoubleGetter(std::bind(&model::Screen::diffuseSolarReflectance, m_screen.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Screen::*)(double)>(&model::Screen::setDiffuseSolarReflectance), m_screen.get_ptr(), std::placeholders::_1))
  );


  // m_diffuseVisibleReflectance->bind(screen,"diffuseVisibleReflectance",m_isIP);
  m_diffuseVisibleReflectance->bind(
    m_isIP,
    *m_screen,
    DoubleGetter(std::bind(&model::Screen::diffuseVisibleReflectance, m_screen.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Screen::*)(double)>(&model::Screen::setDiffuseVisibleReflectance), m_screen.get_ptr(), std::placeholders::_1))
  );

  // m_thermalHemisphericalEmissivity->bind(screen,"thermalHemisphericalEmissivity",m_isIP);
  m_thermalHemisphericalEmissivity->bind(
    m_isIP,
    *m_screen,
    DoubleGetter(std::bind(&model::Screen::thermalHemisphericalEmissivity, m_screen.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Screen::*)(double)>(&model::Screen::setThermalHemisphericalEmissivity), m_screen.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Screen::resetThermalHemisphericalEmissivity, m_screen.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Screen::isThermalHemisphericalEmissivityDefaulted, m_screen.get_ptr()))
  );

  // m_conductivity->bind(screen,"conductivity",m_isIP);
  m_conductivity->bind(
    m_isIP,
    *m_screen,
    DoubleGetter(std::bind(&model::Screen::conductivity, m_screen.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Screen::*)(double)>(&model::Screen::setConductivity), m_screen.get_ptr(), std::placeholders::_1))
  );

  // m_screenMaterialSpacing->bind(screen,"screenMaterialSpacing",m_isIP);
  m_screenMaterialSpacing->bind(
    m_isIP,
    *m_screen,
    DoubleGetter(std::bind(&model::Screen::screenMaterialSpacing, m_screen.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Screen::*)(double)>(&model::Screen::setScreenMaterialSpacing), m_screen.get_ptr(), std::placeholders::_1))
  );

  // m_screenMaterialDiameter->bind(screen,"screenMaterialDiameter",m_isIP);
  m_screenMaterialDiameter->bind(
    m_isIP,
    *m_screen,
    DoubleGetter(std::bind(&model::Screen::screenMaterialDiameter, m_screen.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Screen::*)(double)>(&model::Screen::setScreenMaterialDiameter), m_screen.get_ptr(), std::placeholders::_1))
  );

  // m_screenToGlassDistance->bind(screen,"screentoGlassDistance",m_isIP);
  m_screenToGlassDistance->bind(
    m_isIP,
    *m_screen,
    DoubleGetter(std::bind(&model::Screen::screentoGlassDistance, m_screen.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Screen::*)(double)>(&model::Screen::setScreentoGlassDistance), m_screen.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Screen::resetScreentoGlassDistance, m_screen.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Screen::isScreentoGlassDistanceDefaulted, m_screen.get_ptr()))
  );

  // m_topOpeningMultiplier->bind(screen,"topOpeningMultiplier",m_isIP);
  m_topOpeningMultiplier->bind(
    m_isIP,
    *m_screen,
    DoubleGetter(std::bind(&model::Screen::topOpeningMultiplier, m_screen.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Screen::*)(double)>(&model::Screen::setTopOpeningMultiplier), m_screen.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Screen::resetTopOpeningMultiplier, m_screen.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Screen::isTopOpeningMultiplierDefaulted, m_screen.get_ptr()))
  );

  // m_bottomOpeningMultiplier->bind(screen,"bottomOpeningMultiplier",m_isIP);
  m_bottomOpeningMultiplier->bind(
    m_isIP,
    *m_screen,
    DoubleGetter(std::bind(&model::Screen::bottomOpeningMultiplier, m_screen.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Screen::*)(double)>(&model::Screen::setBottomOpeningMultiplier), m_screen.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Screen::resetBottomOpeningMultiplier, m_screen.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Screen::isBottomOpeningMultiplierDefaulted, m_screen.get_ptr()))
  );

  // m_leftSideOpeningMultiplier->bind(screen,"leftSideOpeningMultiplier",m_isIP);
  m_leftSideOpeningMultiplier->bind(
    m_isIP,
    *m_screen,
    DoubleGetter(std::bind(&model::Screen::leftSideOpeningMultiplier, m_screen.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Screen::*)(double)>(&model::Screen::setLeftSideOpeningMultiplier), m_screen.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Screen::resetLeftSideOpeningMultiplier, m_screen.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Screen::isLeftSideOpeningMultiplierDefaulted, m_screen.get_ptr()))
  );

  // m_rightSideOpeningMultiplier->bind(screen,"rightSideOpeningMultiplier",m_isIP);
  m_rightSideOpeningMultiplier->bind(
    m_isIP,
    *m_screen,
    DoubleGetter(std::bind(&model::Screen::rightSideOpeningMultiplier, m_screen.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Screen::*)(double)>(&model::Screen::setRightSideOpeningMultiplier), m_screen.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Screen::resetRightSideOpeningMultiplier, m_screen.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Screen::isRightSideOpeningMultiplierDefaulted, m_screen.get_ptr()))
  );

  m_standardsInformationWidget->attach(m_screen.get());

  this->stackedWidget()->setCurrentIndex(1);
}

void WindowMaterialScreenInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_reflectedBeamTransmittanceAccountingMethod->unbind();
  m_angleOfResolutionForScreenTransmittanceOutputMap->unbind();

  m_nameEdit->unbind();
  m_diffuseSolarReflectance->unbind();
  m_diffuseVisibleReflectance->unbind();
  m_thermalHemisphericalEmissivity->unbind();
  m_conductivity->unbind();
  m_screenMaterialSpacing->unbind();
  m_screenMaterialDiameter->unbind();
  m_screenToGlassDistance->unbind();
  m_topOpeningMultiplier->unbind();
  m_bottomOpeningMultiplier->unbind();
  m_leftSideOpeningMultiplier->unbind();
  m_rightSideOpeningMultiplier->unbind();

  m_screen = boost::none;

  m_standardsInformationWidget->detach();
}

void WindowMaterialScreenInspectorView::refresh()
{
}

} // openstudio
