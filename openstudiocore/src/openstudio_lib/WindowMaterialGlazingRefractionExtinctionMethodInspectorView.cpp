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

#include "WindowMaterialGlazingRefractionExtinctionMethodInspectorView.hpp"

#include "StandardsInformationMaterialWidget.hpp"

#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "../shared_gui_components/OSSwitch.hpp"

#include "../model/RefractionExtinctionGlazing.hpp"
#include "../model/RefractionExtinctionGlazing_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

// WindowMaterialGlazingRefractionExtinctionMethodInspectorView

WindowMaterialGlazingRefractionExtinctionMethodInspectorView::WindowMaterialGlazingRefractionExtinctionMethodInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
    m_isIP(isIP)
{
  createLayout();
}

void WindowMaterialGlazingRefractionExtinctionMethodInspectorView::createLayout()
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

  // Thickness

  label = new QLabel("Thickness: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_thickness = new OSQuantityEdit2("m","m","in", m_isIP);
  connect(this, &WindowMaterialGlazingRefractionExtinctionMethodInspectorView::toggleUnitsClicked, m_thickness, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_thickness,row++,0,1,3);

  // Solar Index Of Refraction

  label = new QLabel("Solar Index Of Refraction: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_solarIndexOfRefraction = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialGlazingRefractionExtinctionMethodInspectorView::toggleUnitsClicked, m_solarIndexOfRefraction, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_solarIndexOfRefraction,row++,0,1,3);

  // Solar Extinction Coefficient

  label = new QLabel("Solar Extinction Coefficient: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_solarExtinctionCoefficient = new OSQuantityEdit2("1/m","1/m","1/ft", m_isIP);
  connect(this, &WindowMaterialGlazingRefractionExtinctionMethodInspectorView::toggleUnitsClicked, m_solarExtinctionCoefficient, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_solarExtinctionCoefficient,row++,0,1,3);

  // Visible Index of Refraction

  label = new QLabel("Visible Index of Refraction: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_visibleIndexOfRefraction = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialGlazingRefractionExtinctionMethodInspectorView::toggleUnitsClicked, m_visibleIndexOfRefraction, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_visibleIndexOfRefraction,row++,0,1,3);

  // Visible Extinction Coefficient

  label = new QLabel("Visible Extinction Coefficient: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_visibleExtinctionCoefficient = new OSQuantityEdit2("1/m","1/m","1/ft", m_isIP);
  connect(this, &WindowMaterialGlazingRefractionExtinctionMethodInspectorView::toggleUnitsClicked, m_visibleExtinctionCoefficient, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_visibleExtinctionCoefficient,row++,0,1,3);

  // Infrared Transmittance At Normal Incidence

  label = new QLabel("Infrared Transmittance At Normal Incidence: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_infraredTransmittanceAtNormalIncidence = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialGlazingRefractionExtinctionMethodInspectorView::toggleUnitsClicked, m_infraredTransmittanceAtNormalIncidence, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_infraredTransmittanceAtNormalIncidence,row++,0,1,3);

  // Infrared Hemispherical Emissivity

  label = new QLabel("Infrared Hemispherical Emissivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_infraredHemisphericalEmissivity = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialGlazingRefractionExtinctionMethodInspectorView::toggleUnitsClicked, m_infraredHemisphericalEmissivity, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_infraredHemisphericalEmissivity,row++,0,1,3);

  // Conductivity

  label = new QLabel("Conductivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_conductivity = new OSQuantityEdit2("W/m*K", "W/m*K", "Btu*in/hr*ft^2*R", m_isIP);
  connect(this, &WindowMaterialGlazingRefractionExtinctionMethodInspectorView::toggleUnitsClicked, m_conductivity, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_conductivity,row++,0,1,3);

  // Dirt Correction Factor For Solar And Visible Transmittance

  label = new QLabel("Dirt Correction Factor For Solar And Visible Transmittance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_dirtCorrectionFactorForSolarAndVisibleTransmittance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialGlazingRefractionExtinctionMethodInspectorView::toggleUnitsClicked, m_dirtCorrectionFactorForSolarAndVisibleTransmittance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_dirtCorrectionFactorForSolarAndVisibleTransmittance,row++,0,1,3);

  // Solar Diffusing

  label = new QLabel("Solar Diffusing: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_solarDiffusing = new OSSwitch2();
  mainGridLayout->addWidget(m_solarDiffusing,row++,0,1,3);

  // Stretch

  mainGridLayout->setRowStretch(100,100);

  mainGridLayout->setColumnStretch(100,100);
}

void WindowMaterialGlazingRefractionExtinctionMethodInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void WindowMaterialGlazingRefractionExtinctionMethodInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::RefractionExtinctionGlazing refractionExtinctionGlazing = modelObject.cast<model::RefractionExtinctionGlazing>();
  attach(refractionExtinctionGlazing);
  refresh();
}

void WindowMaterialGlazingRefractionExtinctionMethodInspectorView::onUpdate()
{
  refresh();
}

void WindowMaterialGlazingRefractionExtinctionMethodInspectorView::attach(openstudio::model::RefractionExtinctionGlazing & refractionExtinctionGlazing)
{
  m_refractionExtinctionGlazing = refractionExtinctionGlazing;

  // m_solarDiffusing->bind(refractionExtinctionGlazing,"solarDiffusing");
  m_solarDiffusing->bind(
    *m_refractionExtinctionGlazing,
    std::bind(&model::RefractionExtinctionGlazing::solarDiffusing, m_refractionExtinctionGlazing.get_ptr()),
    boost::optional<BoolSetter>(std::bind(&model::RefractionExtinctionGlazing::setSolarDiffusing, m_refractionExtinctionGlazing.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::RefractionExtinctionGlazing::resetSolarDiffusing, m_refractionExtinctionGlazing.get_ptr())),
    boost::optional<BasicQuery>(std::bind(&model::RefractionExtinctionGlazing::isSolarDiffusingDefaulted, m_refractionExtinctionGlazing.get_ptr()))
  );

  // m_nameEdit->bind(refractionExtinctionGlazing,"name");
  m_nameEdit->bind(
    *m_refractionExtinctionGlazing,
    OptionalStringGetter(std::bind(&model::RefractionExtinctionGlazing::name, m_refractionExtinctionGlazing.get_ptr(),true)),
    boost::optional<StringSetter>(std::bind(&model::RefractionExtinctionGlazing::setName, m_refractionExtinctionGlazing.get_ptr(),std::placeholders::_1))
  );

  // m_conductivity->bind(refractionExtinctionGlazing,"conductivity",m_isIP);
  m_conductivity->bind(
    m_isIP,
    *m_refractionExtinctionGlazing,
    DoubleGetter(std::bind(&model::RefractionExtinctionGlazing::conductivity, m_refractionExtinctionGlazing.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RefractionExtinctionGlazing::*)(double)>(&model::RefractionExtinctionGlazing::setConductivity), m_refractionExtinctionGlazing.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::RefractionExtinctionGlazing::resetConductivity, m_refractionExtinctionGlazing.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::RefractionExtinctionGlazing::isConductivityDefaulted, m_refractionExtinctionGlazing.get_ptr()))
  );

  // m_dirtCorrectionFactorForSolarAndVisibleTransmittance->bind(refractionExtinctionGlazing,"dirtCorrectionFactorforSolarandVisibleTransmittance",m_isIP);
  m_dirtCorrectionFactorForSolarAndVisibleTransmittance->bind(
    m_isIP,
    *m_refractionExtinctionGlazing,
    DoubleGetter(std::bind(&model::RefractionExtinctionGlazing::dirtCorrectionFactorforSolarandVisibleTransmittance, m_refractionExtinctionGlazing.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RefractionExtinctionGlazing::*)(double)>(&model::RefractionExtinctionGlazing::setDirtCorrectionFactorforSolarandVisibleTransmittance), m_refractionExtinctionGlazing.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::RefractionExtinctionGlazing::resetDirtCorrectionFactorforSolarandVisibleTransmittance, m_refractionExtinctionGlazing.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::RefractionExtinctionGlazing::isDirtCorrectionFactorforSolarandVisibleTransmittanceDefaulted, m_refractionExtinctionGlazing.get_ptr()))
  );

  // m_infraredHemisphericalEmissivity->bind(refractionExtinctionGlazing,"infraredHemisphericalEmissivity",m_isIP);
  m_infraredHemisphericalEmissivity->bind(
    m_isIP,
    *m_refractionExtinctionGlazing,
    DoubleGetter(std::bind(&model::RefractionExtinctionGlazing::infraredHemisphericalEmissivity, m_refractionExtinctionGlazing.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RefractionExtinctionGlazing::*)(double)>(&model::RefractionExtinctionGlazing::setInfraredHemisphericalEmissivity), m_refractionExtinctionGlazing.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::RefractionExtinctionGlazing::resetInfraredHemisphericalEmissivity, m_refractionExtinctionGlazing.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::RefractionExtinctionGlazing::isInfraredHemisphericalEmissivityDefaulted, m_refractionExtinctionGlazing.get_ptr()))
  );

  // m_infraredTransmittanceAtNormalIncidence->bind(refractionExtinctionGlazing,"infraredTransmittanceatNormalIncidence",m_isIP);
  m_infraredTransmittanceAtNormalIncidence->bind(
    m_isIP,
    *m_refractionExtinctionGlazing,
    DoubleGetter(std::bind(&model::RefractionExtinctionGlazing::infraredTransmittanceatNormalIncidence, m_refractionExtinctionGlazing.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RefractionExtinctionGlazing::*)(double)>(&model::RefractionExtinctionGlazing::setInfraredTransmittanceatNormalIncidence), m_refractionExtinctionGlazing.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::RefractionExtinctionGlazing::resetInfraredTransmittanceatNormalIncidence, m_refractionExtinctionGlazing.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::RefractionExtinctionGlazing::isInfraredTransmittanceatNormalIncidenceDefaulted, m_refractionExtinctionGlazing.get_ptr()))
  );

  // m_solarExtinctionCoefficient->bind(refractionExtinctionGlazing,"solarExtinctionCoefficient",m_isIP);
  m_solarExtinctionCoefficient->bind(
    m_isIP,
    *m_refractionExtinctionGlazing,
    DoubleGetter(std::bind(&model::RefractionExtinctionGlazing::solarExtinctionCoefficient, m_refractionExtinctionGlazing.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RefractionExtinctionGlazing::*)(double)>(&model::RefractionExtinctionGlazing::setSolarExtinctionCoefficient), m_refractionExtinctionGlazing.get_ptr(), std::placeholders::_1))
  );

  // m_solarIndexOfRefraction->bind(refractionExtinctionGlazing,"solarIndexofRefraction",m_isIP);
  m_solarIndexOfRefraction->bind(
    m_isIP,
    *m_refractionExtinctionGlazing,
    DoubleGetter(std::bind(&model::RefractionExtinctionGlazing::solarIndexofRefraction, m_refractionExtinctionGlazing.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RefractionExtinctionGlazing::*)(double)>(&model::RefractionExtinctionGlazing::setSolarIndexofRefraction), m_refractionExtinctionGlazing.get_ptr(), std::placeholders::_1))
  );

  // m_thickness->bind(refractionExtinctionGlazing,"thickness",m_isIP);
  m_thickness->bind(
    m_isIP,
    *m_refractionExtinctionGlazing,
    DoubleGetter(std::bind(&model::RefractionExtinctionGlazing::thickness, m_refractionExtinctionGlazing.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RefractionExtinctionGlazing::*)(double)>(&model::RefractionExtinctionGlazing::setThickness), m_refractionExtinctionGlazing.get_ptr(), std::placeholders::_1))
  );

  // m_visibleExtinctionCoefficient->bind(refractionExtinctionGlazing,"visibleExtinctionCoefficient",m_isIP);
  m_visibleExtinctionCoefficient->bind(
    m_isIP,
    *m_refractionExtinctionGlazing,
    DoubleGetter(std::bind(&model::RefractionExtinctionGlazing::visibleExtinctionCoefficient, m_refractionExtinctionGlazing.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RefractionExtinctionGlazing::*)(double)>(&model::RefractionExtinctionGlazing::setVisibleExtinctionCoefficient), m_refractionExtinctionGlazing.get_ptr(), std::placeholders::_1))
  );

  // m_visibleIndexOfRefraction->bind(refractionExtinctionGlazing,"visibleIndexofRefraction",m_isIP);
  m_visibleIndexOfRefraction->bind(
    m_isIP,
    *m_refractionExtinctionGlazing,
    DoubleGetter(std::bind(&model::RefractionExtinctionGlazing::visibleIndexofRefraction, m_refractionExtinctionGlazing.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RefractionExtinctionGlazing::*)(double)>(&model::RefractionExtinctionGlazing::setVisibleIndexofRefraction), m_refractionExtinctionGlazing.get_ptr(), std::placeholders::_1))
  );

  m_standardsInformationWidget->attach(refractionExtinctionGlazing);

  this->stackedWidget()->setCurrentIndex(1);
}

void WindowMaterialGlazingRefractionExtinctionMethodInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_solarDiffusing->unbind();

  m_nameEdit->unbind();
  m_thickness->unbind();
  m_solarIndexOfRefraction->unbind();
  m_solarExtinctionCoefficient->unbind();
  m_visibleIndexOfRefraction->unbind();
  m_visibleExtinctionCoefficient->unbind();
  m_infraredTransmittanceAtNormalIncidence->unbind();
  m_infraredHemisphericalEmissivity->unbind();
  m_conductivity->unbind();
  m_dirtCorrectionFactorForSolarAndVisibleTransmittance->unbind();

  m_refractionExtinctionGlazing = boost::none;

  m_standardsInformationWidget->detach();
}

void WindowMaterialGlazingRefractionExtinctionMethodInspectorView::refresh()
{
}

} // openstudio
