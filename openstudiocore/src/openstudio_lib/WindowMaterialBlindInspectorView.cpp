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

#include "WindowMaterialBlindInspectorView.hpp"

#include "StandardsInformationMaterialWidget.hpp"

#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"

#include "../model/Blind.hpp"
#include "../model/Blind_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

// WindowMaterialBlindInspectorView

WindowMaterialBlindInspectorView::WindowMaterialBlindInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
    m_isIP(isIP)
{
  createLayout();
}

void WindowMaterialBlindInspectorView::createLayout()
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

  // Slat Orientation

  label = new QLabel("Slat Orientation: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_slatOrientation = new OSComboBox2();
  m_slatOrientation->addItem("Horizontal");
  m_slatOrientation->addItem("Vertical");
  mainGridLayout->addWidget(m_slatOrientation,row++,0,1,3);

  // Slat Width

  label = new QLabel("Slat Width: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_slatWidth = new OSQuantityEdit2("m","m","in", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_slatWidth, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_slatWidth,row++,0,1,3);

  // Slat Separation

  label = new QLabel("Slat Separation: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_slatSeparation = new OSQuantityEdit2("m","m","in", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_slatSeparation, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_slatSeparation,row++,0,1,3);

  // Slat Thickness

  label = new QLabel("Slat Thickness: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_slatThickness = new OSQuantityEdit2("m","m","in", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_slatThickness, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_slatThickness,row++,0,1,3);

  // Slat Angle

  label = new QLabel("Slat Angle: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_slatAngle = new OSQuantityEdit2("deg","deg","deg", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_slatAngle, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_slatAngle,row++,0,1,3);

  // Slat Conductivity

  label = new QLabel("Slat Conductivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_slatConductivity = new OSQuantityEdit2("W/m*K", "W/m*K", "Btu*in/hr*ft^2*R", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_slatConductivity, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_slatConductivity,row++,0,1,3);

  // Slat Beam Solar Transmittance

  label = new QLabel("Slat Beam Solar Transmittance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_slatBeamSolarTransmittance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_slatBeamSolarTransmittance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_slatBeamSolarTransmittance,row++,0,1,3);

  // Front Side Slat Beam Solar Reflectance

  label = new QLabel("Front Side Slat Beam Solar Reflectance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_frontSideSlatBeamSolarReflectance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_frontSideSlatBeamSolarReflectance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_frontSideSlatBeamSolarReflectance,row++,0,1,3);

  // Back Side Slat Beam Solar Reflectance

  label = new QLabel("Back Side Slat Beam Solar Reflectance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_backSideSlatBeamSolarReflectance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_backSideSlatBeamSolarReflectance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_backSideSlatBeamSolarReflectance,row++,0,1,3);

  // Slat Diffuse Solar Transmittance

  label = new QLabel("Slat Diffuse Solar Transmittance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_slatDiffuseSolarTransmittance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_slatDiffuseSolarTransmittance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_slatDiffuseSolarTransmittance,row++,0,1,3);

  // Front Side Slat Diffuse Solar Reflectance

  label = new QLabel("Front Side Slat Diffuse Solar Reflectance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_frontSideSlatDiffuseSolarReflectance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_frontSideSlatDiffuseSolarReflectance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_frontSideSlatDiffuseSolarReflectance,row++,0,1,3);

  // Back Side Slat Diffuse Solar Reflectance

  label = new QLabel("Back Side Slat Diffuse Solar Reflectance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_backSideSlatDiffuseSolarReflectance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_backSideSlatDiffuseSolarReflectance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_backSideSlatDiffuseSolarReflectance,row++,0,1,3);

  // Slat Beam Visible Transmittance

  label = new QLabel("Slat Beam Visible Transmittance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_slatBeamVisibleTransmittance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_slatBeamVisibleTransmittance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_slatBeamVisibleTransmittance,row++,0,1,3);

  // Front Side Slat Beam Visible Reflectance

  label = new QLabel("Front Side Slat Beam Visible Reflectance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_frontSideSlatBeamVisibleReflectance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_frontSideSlatBeamVisibleReflectance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_frontSideSlatBeamVisibleReflectance,row++,0,1,3);

  // Back Side Slat Beam Visible Reflectance

  label = new QLabel("Back Side Slat Beam Visible Reflectance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_backSideSlatBeamVisibleReflectance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_frontSideSlatBeamVisibleReflectance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_backSideSlatBeamVisibleReflectance,row++,0,1,3);

  // Slat Diffuse Visible Transmittance

  label = new QLabel("Slat Diffuse Visible Transmittance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_slatDiffuseVisibleTransmittance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_slatDiffuseVisibleTransmittance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_slatDiffuseVisibleTransmittance,row++,0,1,3);

  // Front Side Slat Diffuse Visible Reflectance

  label = new QLabel("Front Side Slat Diffuse Visible Reflectance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_frontSideSlatDiffuseVisibleReflectance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_frontSideSlatDiffuseVisibleReflectance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_frontSideSlatDiffuseVisibleReflectance,row++,0,1,3);

  // Back Side Slat Diffuse Visible Reflectance

  label = new QLabel("Back Side Slat Diffuse Visible Reflectance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_backSideSlatDiffuseVisibleReflectance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_backSideSlatDiffuseVisibleReflectance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_backSideSlatDiffuseVisibleReflectance,row++,0,1,3);

  // Slat Infrared Hemispherical Transmittance

  label = new QLabel("Slat Infrared Hemispherical Transmittance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_slatInfraredHemisphericalTransmittance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_slatInfraredHemisphericalTransmittance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_slatInfraredHemisphericalTransmittance,row++,0,1,3);

  // Front Side Slat Infrared Hemispherical Emissivity

  label = new QLabel("Front Side Slat Infrared Hemispherical Emissivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_frontSideSlatInfraredHemisphericalEmissivity = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_frontSideSlatInfraredHemisphericalEmissivity, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_frontSideSlatInfraredHemisphericalEmissivity,row++,0,1,3);

  // Back Side Slat Infrared Hemispherical Emissivity

  label = new QLabel("Back Side Slat Infrared Hemispherical Emissivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_backSideSlatInfraredHemisphericalEmissivity = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_backSideSlatInfraredHemisphericalEmissivity, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_backSideSlatInfraredHemisphericalEmissivity,row++,0,1,3);

  // Blind To Glass Distance

  label = new QLabel("Blind To Glass Distance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_blindToGlassDistance = new OSQuantityEdit2("m","m","in", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_blindToGlassDistance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_blindToGlassDistance,row++,0,1,3);

  // Blind Top Opening Multiplier

  label = new QLabel("Blind Top Opening Multiplier: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_blindTopOpeningMultiplier = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_blindTopOpeningMultiplier, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_blindTopOpeningMultiplier,row++,0,1,3);

  // Blind Bottom Opening Multiplier

  label = new QLabel("Blind Bottom Opening Multiplier: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_blindBottomOpeningMultiplier = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_blindBottomOpeningMultiplier, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_blindBottomOpeningMultiplier,row++,0,1,3);

  // Blind Left Side Opening Multiplier

  label = new QLabel("Blind Left Side Opening Multiplier: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_blindLeftSideOpeningMultiplier = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_blindLeftSideOpeningMultiplier, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_blindLeftSideOpeningMultiplier,row++,0,1,3);

  // Blind Right Side Opening Multiplier

  label = new QLabel("Blind Right Side Opening Multiplier: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_blindRightSideOpeningMultiplier = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_blindRightSideOpeningMultiplier, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_blindRightSideOpeningMultiplier,row++,0,1,3);

  // Minimum Slat Angle

  label = new QLabel("Minimum Slat Angle: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_minimumSlatAngle = new OSQuantityEdit2("deg","deg","deg", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_minimumSlatAngle, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_minimumSlatAngle,row++,0,1,3);

  // Maximum Slat Angle

  label = new QLabel("Maximum Slat Angle: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_maximumSlatAngle = new OSQuantityEdit2("deg","deg","deg", m_isIP);
  connect(this, &WindowMaterialBlindInspectorView::toggleUnitsClicked, m_maximumSlatAngle, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_maximumSlatAngle,row++,0,1,3);

  // Stretch

  mainGridLayout->setRowStretch(100,100);

  mainGridLayout->setColumnStretch(100,100);
}

void WindowMaterialBlindInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void WindowMaterialBlindInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::Blind material = modelObject.cast<model::Blind>();
  attach(material);
  refresh();
}

void WindowMaterialBlindInspectorView::onUpdate()
{
  refresh();
}

void WindowMaterialBlindInspectorView::attach(openstudio::model::Blind & material)
{
  m_material = material;

  // m_slatOrientation->bind(material,"slatOrientation");
  m_slatOrientation->bind<std::string>(
      *m_material,
      static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
      &model::Blind::slatOrientationValues,
      std::bind(&model::Blind::slatOrientation, m_material.get_ptr()),
      std::bind(&model::Blind::setSlatOrientation, m_material.get_ptr(), std::placeholders::_1),
      boost::optional<NoFailAction>(std::bind(&model::Blind::resetSlatOrientation, m_material.get_ptr())),
      boost::optional<BasicQuery>(std::bind(&model::Blind::isSlatOrientationDefaulted, m_material.get_ptr())));

  // m_nameEdit->bind(material,"name");
  m_nameEdit->bind(
    *m_material,
    OptionalStringGetter(std::bind(&model::Blind::name, m_material.get_ptr(),true)),
    boost::optional<StringSetter>(std::bind(&model::Blind::setName, m_material.get_ptr(),std::placeholders::_1))
  );

  // m_slatWidth->bind(material,"slatWidth",m_isIP);
  m_slatWidth->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::slatWidth, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setSlatWidth), m_material.get_ptr(), std::placeholders::_1))
  );

  // m_slatSeparation->bind(material,"slatSeparation",m_isIP);
  m_slatSeparation->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::slatSeparation, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setSlatSeparation), m_material.get_ptr(), std::placeholders::_1))
  );

  // m_slatThickness->bind(material,"slatThickness",m_isIP);
  m_slatThickness->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::slatThickness, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setSlatThickness), m_material.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Blind::resetSlatThickness, m_material.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Blind::isSlatThicknessDefaulted, m_material.get_ptr()))
  );

  // m_slatAngle->bind(material,"slatAngle",m_isIP);
  m_slatAngle->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::slatAngle, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setSlatAngle), m_material.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Blind::resetSlatAngle, m_material.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Blind::isSlatAngleDefaulted, m_material.get_ptr()))
  );

  // m_slatConductivity->bind(material,"slatConductivity",m_isIP);
  m_slatConductivity->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::slatConductivity, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setSlatConductivity), m_material.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Blind::resetSlatConductivity, m_material.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Blind::isSlatConductivityDefaulted, m_material.get_ptr()))
  );

  // m_slatBeamSolarTransmittance->bind(material,"slatBeamSolarTransmittance",m_isIP);
  m_slatBeamSolarTransmittance->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::slatBeamSolarTransmittance, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setSlatBeamSolarTransmittance), m_material.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Blind::resetSlatBeamSolarTransmittance, m_material.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Blind::isSlatBeamSolarTransmittanceDefaulted, m_material.get_ptr()))
  );

  // m_frontSideSlatBeamSolarReflectance->bind(material,"frontSideSlatBeamSolarReflectance",m_isIP);
  m_frontSideSlatBeamSolarReflectance->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::frontSideSlatBeamSolarReflectance, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setFrontSideSlatBeamSolarReflectance), m_material.get_ptr(), std::placeholders::_1))
  );

  // m_backSideSlatBeamSolarReflectance->bind(material,"backSideSlatBeamSolarReflectance",m_isIP);
  m_backSideSlatBeamSolarReflectance->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::backSideSlatBeamSolarReflectance, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setBackSideSlatBeamSolarReflectance), m_material.get_ptr(), std::placeholders::_1))
  );

  // m_slatDiffuseSolarTransmittance->bind(material,"slatDiffuseSolarTransmittance",m_isIP);
  m_slatDiffuseSolarTransmittance->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::slatDiffuseSolarTransmittance, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setSlatDiffuseSolarTransmittance), m_material.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Blind::resetSlatDiffuseSolarTransmittance, m_material.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Blind::isSlatDiffuseSolarTransmittanceDefaulted, m_material.get_ptr()))
  );

  // m_frontSideSlatDiffuseSolarReflectance->bind(material,"frontSideSlatDiffuseSolarReflectance",m_isIP);
  m_frontSideSlatDiffuseSolarReflectance->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::frontSideSlatDiffuseSolarReflectance, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setFrontSideSlatDiffuseSolarReflectance), m_material.get_ptr(), std::placeholders::_1))
  );

  // m_backSideSlatDiffuseSolarReflectance->bind(material,"backSideSlatDiffuseSolarReflectance",m_isIP);
  m_backSideSlatDiffuseSolarReflectance->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::backSideSlatDiffuseSolarReflectance, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setBackSideSlatDiffuseSolarReflectance), m_material.get_ptr(), std::placeholders::_1))
  );

  // m_slatBeamVisibleTransmittance->bind(material,"slatBeamVisibleTransmittance",m_isIP);
  m_slatBeamVisibleTransmittance->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::slatBeamVisibleTransmittance, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setSlatBeamVisibleTransmittance), m_material.get_ptr(), std::placeholders::_1))
  );

  // m_frontSideSlatBeamVisibleReflectance->bind(material,"frontSideSlatBeamVisibleReflectance",m_isIP);
  m_frontSideSlatBeamVisibleReflectance->bind(
    m_isIP,
    *m_material,
    OptionalDoubleGetter(std::bind(&model::Blind::frontSideSlatBeamVisibleReflectance, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setFrontSideSlatBeamVisibleReflectance), m_material.get_ptr(), std::placeholders::_1))
  );

  // m_backSideSlatBeamVisibleReflectance->bind(material,"backSideSlatBeamVisibleReflectance",m_isIP);
  m_backSideSlatBeamVisibleReflectance->bind(
    m_isIP,
    *m_material,
    OptionalDoubleGetter(std::bind(&model::Blind::backSideSlatBeamVisibleReflectance, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setBackSideSlatBeamVisibleReflectance), m_material.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Blind::resetBackSideSlatBeamVisibleReflectance, m_material.get_ptr()))
  );

  // m_slatDiffuseVisibleTransmittance->bind(material,"slatDiffuseVisibleTransmittance",m_isIP);
  m_slatDiffuseVisibleTransmittance->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::slatDiffuseVisibleTransmittance, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setSlatDiffuseVisibleTransmittance), m_material.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Blind::resetSlatDiffuseVisibleTransmittance, m_material.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Blind::isSlatDiffuseVisibleTransmittanceDefaulted, m_material.get_ptr()))
  );

  // m_frontSideSlatDiffuseVisibleReflectance->bind(material,"frontSideSlatDiffuseVisibleReflectance",m_isIP);
  m_frontSideSlatDiffuseVisibleReflectance->bind(
    m_isIP,
    *m_material,
    OptionalDoubleGetter(std::bind(&model::Blind::frontSideSlatDiffuseVisibleReflectance, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setFrontSideSlatDiffuseVisibleReflectance), m_material.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Blind::resetFrontSideSlatDiffuseVisibleReflectance, m_material.get_ptr()))
  );

  // m_backSideSlatDiffuseVisibleReflectance->bind(material,"backSideSlatDiffuseVisibleReflectance",m_isIP);
  m_backSideSlatDiffuseVisibleReflectance->bind(
    m_isIP,
    *m_material,
    OptionalDoubleGetter(std::bind(&model::Blind::backSideSlatDiffuseVisibleReflectance, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setBackSideSlatDiffuseVisibleReflectance), m_material.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Blind::resetBackSideSlatDiffuseVisibleReflectance, m_material.get_ptr()))
  );

  // m_slatInfraredHemisphericalTransmittance->bind(material,"slatInfraredHemisphericalTransmittance",m_isIP);
  m_slatInfraredHemisphericalTransmittance->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::slatInfraredHemisphericalTransmittance, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setSlatInfraredHemisphericalTransmittance), m_material.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Blind::resetSlatInfraredHemisphericalTransmittance, m_material.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Blind::isSlatInfraredHemisphericalTransmittanceDefaulted, m_material.get_ptr()))
  );

  // m_frontSideSlatInfraredHemisphericalEmissivity->bind(material,"frontSideSlatInfraredHemisphericalEmissivity",m_isIP);
  m_frontSideSlatInfraredHemisphericalEmissivity->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::frontSideSlatInfraredHemisphericalEmissivity, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setFrontSideSlatInfraredHemisphericalEmissivity), m_material.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Blind::resetFrontSideSlatInfraredHemisphericalEmissivity, m_material.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Blind::isFrontSideSlatInfraredHemisphericalEmissivityDefaulted, m_material.get_ptr()))
  );

  // m_backSideSlatInfraredHemisphericalEmissivity->bind(material,"backSideSlatInfraredHemisphericalEmissivity",m_isIP);
  m_backSideSlatInfraredHemisphericalEmissivity->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::backSideSlatInfraredHemisphericalEmissivity, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setBackSideSlatInfraredHemisphericalEmissivity), m_material.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Blind::resetBackSideSlatInfraredHemisphericalEmissivity, m_material.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Blind::isBackSideSlatInfraredHemisphericalEmissivityDefaulted, m_material.get_ptr()))
  );

  // m_blindToGlassDistance->bind(material,"blindtoGlassDistance",m_isIP);
  m_blindToGlassDistance->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::blindtoGlassDistance, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setBlindtoGlassDistance), m_material.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Blind::resetBlindtoGlassDistance, m_material.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Blind::isBlindtoGlassDistanceDefaulted, m_material.get_ptr()))
  );

  // m_blindTopOpeningMultiplier->bind(material,"blindTopOpeningMultiplier",m_isIP);
  m_blindTopOpeningMultiplier->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::blindTopOpeningMultiplier, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setBlindTopOpeningMultiplier), m_material.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Blind::resetBlindTopOpeningMultiplier, m_material.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Blind::isBlindTopOpeningMultiplierDefaulted, m_material.get_ptr()))
  );

  // m_blindBottomOpeningMultiplier->bind(material,"blindBottomOpeningMultiplier",m_isIP);
  m_blindTopOpeningMultiplier->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::blindBottomOpeningMultiplier, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setBlindBottomOpeningMultiplier), m_material.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Blind::resetBlindBottomOpeningMultiplier, m_material.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Blind::isBlindBottomOpeningMultiplierDefaulted, m_material.get_ptr()))
  );

  // m_blindLeftSideOpeningMultiplier->bind(material,"blindLeftSideOpeningMultiplier",m_isIP);
  m_blindLeftSideOpeningMultiplier->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::blindLeftSideOpeningMultiplier, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setBlindLeftSideOpeningMultiplier), m_material.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Blind::resetBlindLeftSideOpeningMultiplier, m_material.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Blind::isBlindLeftSideOpeningMultiplierDefaulted, m_material.get_ptr()))
  );

  // m_blindRightSideOpeningMultiplier->bind(material,"blindRightSideOpeningMultiplier",m_isIP);
  m_blindRightSideOpeningMultiplier->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::blindRightSideOpeningMultiplier, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setBlindRightSideOpeningMultiplier), m_material.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Blind::resetBlindRightSideOpeningMultiplier, m_material.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Blind::isBlindRightSideOpeningMultiplierDefaulted, m_material.get_ptr()))
  );

  // m_minimumSlatAngle->bind(material,"minimumSlatAngle",m_isIP);
  m_minimumSlatAngle->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::minimumSlatAngle, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setMinimumSlatAngle), m_material.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Blind::resetMinimumSlatAngle, m_material.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Blind::isMinimumSlatAngleDefaulted, m_material.get_ptr()))
  );

  // m_maximumSlatAngle->bind(material,"maximumSlatAngle",m_isIP);
  m_maximumSlatAngle->bind(
    m_isIP,
    *m_material,
    DoubleGetter(std::bind(&model::Blind::maximumSlatAngle, m_material.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::Blind::*)(double)>(&model::Blind::setMaximumSlatAngle), m_material.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::Blind::resetMaximumSlatAngle, m_material.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::Blind::isMaximumSlatAngleDefaulted, m_material.get_ptr()))
  );

  m_standardsInformationWidget->attach(material);

  this->stackedWidget()->setCurrentIndex(1);
}

void WindowMaterialBlindInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_slatOrientation->unbind();

  m_nameEdit->unbind();
  m_slatWidth->unbind();
  m_slatSeparation->unbind();
  m_slatThickness->unbind();
  m_slatAngle->unbind();
  m_slatConductivity->unbind();
  m_slatBeamSolarTransmittance->unbind();
  m_frontSideSlatBeamSolarReflectance->unbind();
  m_backSideSlatBeamSolarReflectance->unbind();
  m_slatDiffuseSolarTransmittance->unbind();
  m_frontSideSlatDiffuseSolarReflectance->unbind();
  m_backSideSlatDiffuseSolarReflectance->unbind();
  m_slatBeamVisibleTransmittance->unbind();
  m_frontSideSlatBeamVisibleReflectance->unbind();
  m_backSideSlatBeamVisibleReflectance->unbind();
  m_slatDiffuseVisibleTransmittance->unbind();
  m_frontSideSlatDiffuseVisibleReflectance->unbind();
  m_backSideSlatDiffuseVisibleReflectance->unbind();
  m_slatInfraredHemisphericalTransmittance->unbind();
  m_frontSideSlatInfraredHemisphericalEmissivity->unbind();
  m_backSideSlatInfraredHemisphericalEmissivity->unbind();
  m_blindToGlassDistance->unbind();
  m_blindTopOpeningMultiplier->unbind();
  m_blindBottomOpeningMultiplier->unbind();
  m_blindLeftSideOpeningMultiplier->unbind();
  m_blindRightSideOpeningMultiplier->unbind();
  m_minimumSlatAngle->unbind();
  m_maximumSlatAngle->unbind();

  m_material = boost::none;

  m_standardsInformationWidget->detach();
}

void WindowMaterialBlindInspectorView::refresh()
{
}

} // openstudio
