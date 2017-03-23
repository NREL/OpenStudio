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

#include "WindowMaterialShadeInspectorView.hpp"

#include "StandardsInformationMaterialWidget.hpp"

#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"

#include "../model/Shade.hpp"
#include "../model/Shade_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

// WindowMaterialShadeInspectorView

WindowMaterialShadeInspectorView::WindowMaterialShadeInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
    m_isIP(isIP)
{
  createLayout();
}

void WindowMaterialShadeInspectorView::createLayout()
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

  // Solar Transmittance

  label = new QLabel("Solar Transmittance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_solarTransmittance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialShadeInspectorView::toggleUnitsClicked, m_solarTransmittance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_solarTransmittance,row++,0,1,3);

  // Solar Reflectance

  label = new QLabel("Solar Reflectance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_solarReflectance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialShadeInspectorView::toggleUnitsClicked, m_solarReflectance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_solarReflectance,row++,0,1,3);

  // Visible Transmittance

  label = new QLabel("Visible Transmittance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_visibleTransmittance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialShadeInspectorView::toggleUnitsClicked, m_visibleTransmittance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_visibleTransmittance,row++,0,1,3);

  // Visible Reflectance

  label = new QLabel("Visible Reflectance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_visibleReflectance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialShadeInspectorView::toggleUnitsClicked, m_visibleReflectance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_visibleReflectance,row++,0,1,3);

  // Thermal Hemispherical Emissivity

  label = new QLabel("Thermal Hemispherical Emissivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_thermalHemisphericalEmissivity = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialShadeInspectorView::toggleUnitsClicked, m_thermalHemisphericalEmissivity, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_thermalHemisphericalEmissivity,row++,0,1,3);

  // Thermal Transmittance

  label = new QLabel("Thermal Transmittance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_thermalTransmittance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialShadeInspectorView::toggleUnitsClicked, m_thermalTransmittance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_thermalTransmittance,row++,0,1,3);

  // Thickness

  label = new QLabel("Thickness: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_thickness = new OSQuantityEdit2("m","m","in", m_isIP);
  connect(this, &WindowMaterialShadeInspectorView::toggleUnitsClicked, m_thickness, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_thickness,row++,0,1,3);

  // Conductivity

  label = new QLabel("Conductivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_conductivity = new OSQuantityEdit2("W/m*K", "W/m*K", "Btu*in/hr*ft^2*R", m_isIP);
  connect(this, &WindowMaterialShadeInspectorView::toggleUnitsClicked, m_conductivity, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_conductivity,row++,0,1,3);

  // Shade To Glass Distance

  label = new QLabel("Shade To Glass Distance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_shadeToGlassDistance = new OSQuantityEdit2("m","m","in", m_isIP);
  connect(this, &WindowMaterialShadeInspectorView::toggleUnitsClicked, m_shadeToGlassDistance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_shadeToGlassDistance,row++,0,1,3);

  // Top Opening Multiplier

  label = new QLabel("Top Opening Multiplier: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_topOpeningMultiplier = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialShadeInspectorView::toggleUnitsClicked, m_topOpeningMultiplier, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_topOpeningMultiplier,row++,0,1,3);

  // Bottom Opening Multiplier

  label = new QLabel("Bottom Opening Multiplier: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_bottomOpeningMultiplier = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialShadeInspectorView::toggleUnitsClicked, m_bottomOpeningMultiplier, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_bottomOpeningMultiplier,row++,0,1,3);

  // Left-Side Opening Multiplier

  label = new QLabel("Left-Side Opening Multiplier: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_leftSideOpeningMultiplier = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialShadeInspectorView::toggleUnitsClicked, m_leftSideOpeningMultiplier, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_leftSideOpeningMultiplier,row++,0,1,3);

  // Right-Side Opening Multiplier

  label = new QLabel("Right-Side Opening Multiplier: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_rightSideOpeningMultiplier = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialShadeInspectorView::toggleUnitsClicked, m_rightSideOpeningMultiplier, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_rightSideOpeningMultiplier,row++,0,1,3);

  // Airflow Permeability

  label = new QLabel("Airflow Permeability: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_airflowPermeability = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialShadeInspectorView::toggleUnitsClicked, m_airflowPermeability, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_airflowPermeability,row++,0,1,3);

  // Stretch

  mainGridLayout->setRowStretch(100,100);

  mainGridLayout->setColumnStretch(100,100);
}

void WindowMaterialShadeInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void WindowMaterialShadeInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::Shade shade = modelObject.cast<model::Shade>();
  attach(shade);
  refresh();
}

void WindowMaterialShadeInspectorView::onUpdate()
{
  refresh();
}

void WindowMaterialShadeInspectorView::attach(openstudio::model::Shade & shade)
{
  // m_nameEdit->bind(shade,"name");
  m_shade = shade;
  m_nameEdit->bind(
    *m_shade,
    OptionalStringGetter(std::bind(&model::Shade::name, m_shade.get_ptr(),true)),
    boost::optional<StringSetter>(std::bind(&model::Shade::setName, m_shade.get_ptr(),std::placeholders::_1))
  );

  // m_solarTransmittance->bind(shade,"solarTransmittance",m_isIP);
  m_solarTransmittance->bind(
    m_isIP,
    *m_shade,
    DoubleGetter(std::bind(&model::Shade::solarTransmittance, m_shade.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Shade::*)(double)>(&model::Shade::setSolarTransmittance), m_shade.get_ptr(), std::placeholders::_1))
  );

  // m_solarReflectance->bind(shade,"solarReflectance",m_isIP);
  m_solarReflectance->bind(
    m_isIP,
    *m_shade,
    DoubleGetter(std::bind(&model::Shade::solarReflectance, m_shade.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Shade::*)(double)>(&model::Shade::setSolarReflectance), m_shade.get_ptr(), std::placeholders::_1))
  );

  // m_visibleTransmittance->bind(shade,"visibleTransmittance",m_isIP);
  m_visibleTransmittance->bind(
    m_isIP,
    *m_shade,
    DoubleGetter(std::bind(&model::Shade::visibleTransmittance, m_shade.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Shade::*)(double)>(&model::Shade::setVisibleTransmittance), m_shade.get_ptr(), std::placeholders::_1))
  );

  // m_visibleReflectance->bind(shade,"visibleReflectance",m_isIP);
  m_visibleReflectance->bind(
    m_isIP,
    *m_shade,
    DoubleGetter(std::bind(&model::Shade::visibleReflectance, m_shade.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Shade::*)(double)>(&model::Shade::setVisibleReflectance), m_shade.get_ptr(), std::placeholders::_1))
  );

  // m_thermalHemisphericalEmissivity->bind(shade,"thermalHemisphericalEmissivity",m_isIP);
  m_thermalHemisphericalEmissivity->bind(
    m_isIP,
    *m_shade,
    DoubleGetter(std::bind(&model::Shade::thermalHemisphericalEmissivity, m_shade.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Shade::*)(double)>(&model::Shade::setThermalHemisphericalEmissivity), m_shade.get_ptr(), std::placeholders::_1))
  );

  // m_thermalTransmittance->bind(shade,"thermalTransmittance",m_isIP);
  m_thermalTransmittance->bind(
    m_isIP,
    *m_shade,
    DoubleGetter(std::bind(&model::Shade::thermalTransmittance, m_shade.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Shade::*)(double)>(&model::Shade::setThermalTransmittance), m_shade.get_ptr(), std::placeholders::_1))
  );

  // m_thickness->bind(shade,"thickness",m_isIP);
  m_thickness->bind(
    m_isIP,
    *m_shade,
    DoubleGetter(std::bind(&model::Shade::thickness, m_shade.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Shade::*)(double)>(&model::Shade::setThickness), m_shade.get_ptr(), std::placeholders::_1))
  );

  // m_conductivity->bind(shade,"conductivity",m_isIP);
  m_conductivity->bind(
    m_isIP,
    *m_shade,
    DoubleGetter(std::bind(&model::Shade::conductivity, m_shade.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Shade::*)(double)>(&model::Shade::setConductivity), m_shade.get_ptr(), std::placeholders::_1))
  );

  // m_shadeToGlassDistance->bind(shade,"shadetoGlassDistance",m_isIP);
  m_shadeToGlassDistance->bind(
    m_isIP,
    *m_shade,
    DoubleGetter(std::bind(&model::Shade::shadetoGlassDistance, m_shade.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Shade::*)(double)>(&model::Shade::setShadetoGlassDistance), m_shade.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Shade::resetShadetoGlassDistance, m_shade.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Shade::isShadetoGlassDistanceDefaulted, m_shade.get_ptr()))
  );

  // m_topOpeningMultiplier->bind(shade,"topOpeningMultiplier",m_isIP);
  m_topOpeningMultiplier->bind(
    m_isIP,
    *m_shade,
    DoubleGetter(std::bind(&model::Shade::topOpeningMultiplier, m_shade.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Shade::*)(double)>(&model::Shade::setTopOpeningMultiplier), m_shade.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Shade::resetTopOpeningMultiplier, m_shade.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Shade::isTopOpeningMultiplierDefaulted, m_shade.get_ptr()))
  );

  // m_bottomOpeningMultiplier->bind(shade,"bottomOpeningMultiplier",m_isIP);
  m_bottomOpeningMultiplier->bind(
    m_isIP,
    *m_shade,
    DoubleGetter(std::bind(&model::Shade::bottomOpeningMultiplier, m_shade.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Shade::*)(double)>(&model::Shade::setBottomOpeningMultiplier), m_shade.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Shade::resetBottomOpeningMultiplier, m_shade.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Shade::isBottomOpeningMultiplierDefaulted, m_shade.get_ptr()))
  );

  // m_leftSideOpeningMultiplier->bind(shade,"leftSideOpeningMultiplier",m_isIP);
  m_leftSideOpeningMultiplier->bind(
    m_isIP,
    *m_shade,
    DoubleGetter(std::bind(&model::Shade::leftSideOpeningMultiplier, m_shade.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Shade::*)(double)>(&model::Shade::setLeftSideOpeningMultiplier), m_shade.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Shade::resetLeftSideOpeningMultiplier, m_shade.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Shade::isLeftSideOpeningMultiplierDefaulted, m_shade.get_ptr()))
  );

  // m_rightSideOpeningMultiplier->bind(shade,"rightSideOpeningMultiplier",m_isIP);
  m_rightSideOpeningMultiplier->bind(
    m_isIP,
    *m_shade,
    DoubleGetter(std::bind(&model::Shade::rightSideOpeningMultiplier, m_shade.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Shade::*)(double)>(&model::Shade::setRightSideOpeningMultiplier), m_shade.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Shade::resetRightSideOpeningMultiplier, m_shade.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Shade::isRightSideOpeningMultiplierDefaulted, m_shade.get_ptr()))
  );

  // m_airflowPermeability->bind(shade,"airflowPermeability",m_isIP);
  m_airflowPermeability->bind(
    m_isIP,
    *m_shade,
    DoubleGetter(std::bind(&model::Shade::airflowPermeability, m_shade.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Shade::*)(double)>(&model::Shade::setAirflowPermeability), m_shade.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Shade::resetAirflowPermeability, m_shade.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Shade::isAirflowPermeabilityDefaulted, m_shade.get_ptr()))
  );

  m_standardsInformationWidget->attach(shade);

  this->stackedWidget()->setCurrentIndex(1);
}

void WindowMaterialShadeInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();
  m_solarTransmittance->unbind();
  m_solarReflectance->unbind();
  m_visibleTransmittance->unbind();
  m_visibleReflectance->unbind();
  m_thermalHemisphericalEmissivity->unbind();
  m_thermalTransmittance->unbind();
  m_thickness->unbind();
  m_conductivity->unbind();
  m_shadeToGlassDistance->unbind();
  m_topOpeningMultiplier->unbind();
  m_bottomOpeningMultiplier->unbind();
  m_leftSideOpeningMultiplier->unbind();
  m_rightSideOpeningMultiplier->unbind();
  m_airflowPermeability->unbind();

  m_shade = boost::none;

  m_standardsInformationWidget->detach();
}

void WindowMaterialShadeInspectorView::refresh()
{
}

} // openstudio
