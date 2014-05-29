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

#include "WindowMaterialBlindInspectorView.hpp"

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
    m_slatOrientation(NULL),
    m_nameEdit(NULL),
    m_slatWidth(NULL),
    m_slatSeparation(NULL),
    m_slatThickness(NULL),
    m_slatAngle(NULL),
    m_slatConductivity(NULL),
    m_slatBeamSolarTransmittance(NULL),
    m_frontSideSlatBeamSolarReflectance(NULL),
    m_backSideSlatBeamSolarReflectance(NULL),
    m_slatDiffuseSolarTransmittance(NULL),
    m_frontSideSlatDiffuseSolarReflectance(NULL),
    m_backSideSlatDiffuseSolarReflectance(NULL),
    m_slatBeamVisibleTransmittance(NULL),
    m_frontSideSlatBeamVisibleReflectance(NULL),
    m_backSideSlatBeamVisibleReflectance(NULL),
    m_slatDiffuseVisibleTransmittance(NULL),
    m_frontSideSlatDiffuseVisibleReflectance(NULL),
    m_backSideSlatDiffuseVisibleReflectance(NULL),
    m_slatInfraredHemisphericalTransmittance(NULL),
    m_frontSideSlatInfraredHemisphericalEmissivity(NULL),
    m_backSideSlatInfraredHemisphericalEmissivity(NULL),
    m_blindToGlassDistance(NULL),
    m_blindTopOpeningMultiplier(NULL),
    m_blindBottomOpeningMultiplier(NULL),
    m_blindLeftSideOpeningMultiplier(NULL),
    m_blindRightSideOpeningMultiplier(NULL),
    m_minimumSlatAngle(NULL),
    m_maximumSlatAngle(NULL),
    m_isIP(isIP)
{
  createLayout();
}

void WindowMaterialBlindInspectorView::createLayout()
{
  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  QGridLayout* mainGridLayout = new QGridLayout();
  mainGridLayout->setContentsMargins(7,7,7,7);
  mainGridLayout->setSpacing(14);
  visibleWidget->setLayout(mainGridLayout);

  unsigned row = 0;
  unsigned col = 0;

  bool isConnected = false;

  // Name

  QLabel * label = new QLabel("Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_nameEdit = new OSLineEdit();
  mainGridLayout->addWidget(m_nameEdit,row++,0,1,3);

  // Slat Orientation

  label = new QLabel("Slat Orientation: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_slatOrientation = new OSComboBox();
  m_slatOrientation->addItem("Horizontal");
  m_slatOrientation->addItem("Vertical");
  mainGridLayout->addWidget(m_slatOrientation,row++,0,1,3);

  // Slat Width

  label = new QLabel("Slat Width: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_slatWidth = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_slatWidth, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_slatWidth,row++,0,1,3);

  // Slat Separation

  label = new QLabel("Slat Separation: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_slatSeparation = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_slatSeparation, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_slatSeparation,row++,0,1,3);

  // Slat Thickness

  label = new QLabel("Slat Thickness: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_slatThickness = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_slatThickness, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_slatThickness,row++,0,1,3);

  // Slat Angle

  label = new QLabel("Slat Angle: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_slatAngle = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_slatAngle, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_slatAngle,row++,0,1,3);

  // Slat Conductivity

  label = new QLabel("Slat Conductivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_slatConductivity = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_slatConductivity, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_slatConductivity,row++,0,1,3);

  // Slat Beam Solar Transmittance

  label = new QLabel("Slat Beam Solar Transmittance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_slatBeamSolarTransmittance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_slatBeamSolarTransmittance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_slatBeamSolarTransmittance,row++,0,1,3);

  // Front Side Slat Beam Solar Reflectance

  label = new QLabel("Front Side Slat Beam Solar Reflectance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_frontSideSlatBeamSolarReflectance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_frontSideSlatBeamSolarReflectance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_frontSideSlatBeamSolarReflectance,row++,0,1,3);

  // Back Side Slat Beam Solar Reflectance

  label = new QLabel("Back Side Slat Beam Solar Reflectance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_backSideSlatBeamSolarReflectance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_backSideSlatBeamSolarReflectance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_backSideSlatBeamSolarReflectance,row++,0,1,3);

  // Slat Diffuse Solar Transmittance

  label = new QLabel("Slat Diffuse Solar Transmittance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_slatDiffuseSolarTransmittance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_slatDiffuseSolarTransmittance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_slatDiffuseSolarTransmittance,row++,0,1,3);

  // Front Side Slat Diffuse Solar Reflectance

  label = new QLabel("Front Side Slat Diffuse Solar Reflectance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_frontSideSlatDiffuseSolarReflectance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_frontSideSlatDiffuseSolarReflectance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_frontSideSlatDiffuseSolarReflectance,row++,0,1,3);

  // Back Side Slat Diffuse Solar Reflectance

  label = new QLabel("Back Side Slat Diffuse Solar Reflectance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_backSideSlatDiffuseSolarReflectance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_backSideSlatDiffuseSolarReflectance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_backSideSlatDiffuseSolarReflectance,row++,0,1,3);

  // Slat Beam Visible Transmittance

  label = new QLabel("Slat Beam Visible Transmittance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_slatBeamVisibleTransmittance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_slatBeamVisibleTransmittance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_slatBeamVisibleTransmittance,row++,0,1,3);

  // Front Side Slat Beam Visible Reflectance

  label = new QLabel("Front Side Slat Beam Visible Reflectance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_frontSideSlatBeamVisibleReflectance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_frontSideSlatBeamVisibleReflectance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_frontSideSlatBeamVisibleReflectance,row++,0,1,3);

  // Back Side Slat Beam Visible Reflectance

  label = new QLabel("Back Side Slat Beam Visible Reflectance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_backSideSlatBeamVisibleReflectance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_frontSideSlatBeamVisibleReflectance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_backSideSlatBeamVisibleReflectance,row++,0,1,3);

  // Slat Diffuse Visible Transmittance

  label = new QLabel("Slat Diffuse Visible Transmittance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_slatDiffuseVisibleTransmittance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_slatDiffuseVisibleTransmittance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_slatDiffuseVisibleTransmittance,row++,0,1,3);

  // Front Side Slat Diffuse Visible Reflectance

  label = new QLabel("Front Side Slat Diffuse Visible Reflectance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_frontSideSlatDiffuseVisibleReflectance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_frontSideSlatDiffuseVisibleReflectance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_frontSideSlatDiffuseVisibleReflectance,row++,0,1,3);

  // Back Side Slat Diffuse Visible Reflectance

  label = new QLabel("Back Side Slat Diffuse Visible Reflectance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_backSideSlatDiffuseVisibleReflectance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_backSideSlatDiffuseVisibleReflectance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_backSideSlatDiffuseVisibleReflectance,row++,0,1,3);

  // Slat Infrared Hemispherical Transmittance

  label = new QLabel("Slat Infrared Hemispherical Transmittance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_slatInfraredHemisphericalTransmittance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_slatInfraredHemisphericalTransmittance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_slatInfraredHemisphericalTransmittance,row++,0,1,3);

  // Front Side Slat Infrared Hemispherical Emissivity

  label = new QLabel("Front Side Slat Infrared Hemispherical Emissivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_frontSideSlatInfraredHemisphericalEmissivity = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_frontSideSlatInfraredHemisphericalEmissivity, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_frontSideSlatInfraredHemisphericalEmissivity,row++,0,1,3);

  // Back Side Slat Infrared Hemispherical Emissivity

  label = new QLabel("Back Side Slat Infrared Hemispherical Emissivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_backSideSlatInfraredHemisphericalEmissivity = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_backSideSlatInfraredHemisphericalEmissivity, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_backSideSlatInfraredHemisphericalEmissivity,row++,0,1,3);

  // Blind To Glass Distance

  label = new QLabel("Blind To Glass Distance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_blindToGlassDistance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_blindToGlassDistance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_blindToGlassDistance,row++,0,1,3);

  // Blind Top Opening Multiplier

  label = new QLabel("Blind Top Opening Multiplier: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_blindTopOpeningMultiplier = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_blindTopOpeningMultiplier, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_blindTopOpeningMultiplier,row++,0,1,3);

  // Blind Bottom Opening Multiplier

  label = new QLabel("Blind Bottom Opening Multiplier: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_blindBottomOpeningMultiplier = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_blindBottomOpeningMultiplier, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_blindBottomOpeningMultiplier,row++,0,1,3);

  // Blind Left Side Opening Multiplier

  label = new QLabel("Blind Left Side Opening Multiplier: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_blindLeftSideOpeningMultiplier = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_blindLeftSideOpeningMultiplier, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_blindLeftSideOpeningMultiplier,row++,0,1,3);

  // Blind Right Side Opening Multiplier

  label = new QLabel("Blind Right Side Opening Multiplier: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_blindRightSideOpeningMultiplier = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_blindRightSideOpeningMultiplier, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_blindRightSideOpeningMultiplier,row++,0,1,3);

  // Minimum Slat Angle

  label = new QLabel("Minimum Slat Angle: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_minimumSlatAngle = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_minimumSlatAngle, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_minimumSlatAngle,row++,0,1,3);

  // Maximum Slat Angle

  label = new QLabel("Maximum Slat Angle: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_maximumSlatAngle = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_maximumSlatAngle, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
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
  m_slatOrientation->bind(material,"slatOrientation");

  m_nameEdit->bind(material,"name");
  m_slatWidth->bind(material,"slatWidth",m_isIP);
  m_slatSeparation->bind(material,"slatSeparation",m_isIP);
  m_slatThickness->bind(material,"slatThickness",m_isIP);
  m_slatAngle->bind(material,"slatAngle",m_isIP);
  m_slatConductivity->bind(material,"slatConductivity",m_isIP);
  m_slatBeamSolarTransmittance->bind(material,"slatBeamSolarTransmittance",m_isIP);
  m_frontSideSlatBeamSolarReflectance->bind(material,"frontSideSlatBeamSolarReflectance",m_isIP);
  m_backSideSlatBeamSolarReflectance->bind(material,"backSideSlatBeamSolarReflectance",m_isIP);
  m_slatDiffuseSolarTransmittance->bind(material,"slatDiffuseSolarTransmittance",m_isIP);
  m_frontSideSlatDiffuseSolarReflectance->bind(material,"frontSideSlatDiffuseSolarReflectance",m_isIP);
  m_backSideSlatDiffuseSolarReflectance->bind(material,"backSideSlatDiffuseSolarReflectance",m_isIP);
  m_slatBeamVisibleTransmittance->bind(material,"slatBeamVisibleTransmittance",m_isIP);
  m_frontSideSlatBeamVisibleReflectance->bind(material,"frontSideSlatBeamVisibleReflectance",m_isIP);
  m_backSideSlatBeamVisibleReflectance->bind(material,"backSideSlatBeamVisibleReflectance",m_isIP);
  m_slatDiffuseVisibleTransmittance->bind(material,"slatDiffuseVisibleTransmittance",m_isIP);
  m_frontSideSlatDiffuseVisibleReflectance->bind(material,"frontSideSlatDiffuseVisibleReflectance",m_isIP);
  m_backSideSlatDiffuseVisibleReflectance->bind(material,"backSideSlatDiffuseVisibleReflectance",m_isIP);
  m_slatInfraredHemisphericalTransmittance->bind(material,"slatInfraredHemisphericalTransmittance",m_isIP);
  m_frontSideSlatInfraredHemisphericalEmissivity->bind(material,"frontSideSlatInfraredHemisphericalEmissivity",m_isIP);
  m_backSideSlatInfraredHemisphericalEmissivity->bind(material,"backSideSlatInfraredHemisphericalEmissivity",m_isIP);
  m_blindToGlassDistance->bind(material,"blindtoGlassDistance",m_isIP);
  m_blindTopOpeningMultiplier->bind(material,"blindTopOpeningMultiplier",m_isIP);
  m_blindBottomOpeningMultiplier->bind(material,"blindBottomOpeningMultiplier",m_isIP);
  m_blindLeftSideOpeningMultiplier->bind(material,"blindLeftSideOpeningMultiplier",m_isIP);
  m_blindRightSideOpeningMultiplier->bind(material,"blindRightSideOpeningMultiplier",m_isIP);
  m_minimumSlatAngle->bind(material,"minimumSlatAngle",m_isIP);
  m_maximumSlatAngle->bind(material,"maximumSlatAngle",m_isIP);

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
}

void WindowMaterialBlindInspectorView::refresh()
{
}

void WindowMaterialBlindInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio
