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

#include "WindowMaterialScreenInspectorView.hpp"

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
    m_reflectedBeamTransmittanceAccountingMethod(NULL),
    m_angleOfResolutionForScreenTransmittanceOutputMap(NULL),
    m_nameEdit(NULL),
    m_diffuseSolarReflectance(NULL),
    m_diffuseVisibleReflectance(NULL),
    m_thermalHemisphericalEmissivity(NULL),
    m_conductivity(NULL),
    m_screenMaterialSpacing(NULL),
    m_screenMaterialDiameter(NULL),
    m_screenToGlassDistance(NULL),
    m_topOpeningMultiplier(NULL),
    m_bottomOpeningMultiplier(NULL),
    m_leftSideOpeningMultiplier(NULL),
    m_rightSideOpeningMultiplier(NULL),
    m_isIP(isIP)
{
  createLayout();
}

void WindowMaterialScreenInspectorView::createLayout()
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

  //// Reflected Beam Transmittance Accounting Method

  label = new QLabel("Reflected Beam Transmittance Accounting Method: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_reflectedBeamTransmittanceAccountingMethod = new OSComboBox();
  m_reflectedBeamTransmittanceAccountingMethod->addItem("Do Not Model");
  m_reflectedBeamTransmittanceAccountingMethod->addItem("Model As Direct Beam");
  m_reflectedBeamTransmittanceAccountingMethod->addItem("Model As Diffuse");
  mainGridLayout->addWidget(m_reflectedBeamTransmittanceAccountingMethod,row++,0,1,3);

  // Diffuse Solar Reflectance

  label = new QLabel("Diffuse Solar Reflectance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_diffuseSolarReflectance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_diffuseSolarReflectance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_diffuseSolarReflectance,row++,0,1,3);

  // Diffuse Visible Reflectance

  label = new QLabel("Diffuse Visible Reflectance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_diffuseVisibleReflectance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_diffuseVisibleReflectance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_diffuseVisibleReflectance,row++,0,1,3);

  // Thermal Hemispherical Emissivity

  label = new QLabel("Thermal Hemispherical Emissivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_thermalHemisphericalEmissivity = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_thermalHemisphericalEmissivity, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_thermalHemisphericalEmissivity,row++,0,1,3);

  // Conductivity

  label = new QLabel("Conductivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_conductivity = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_conductivity, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_conductivity,row++,0,1,3);

  // Screen Material Spacing

  label = new QLabel("Screen Material Spacing: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_screenMaterialSpacing = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_screenMaterialSpacing, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_screenMaterialSpacing,row++,0,1,3);

  // Screen Material Diameter

  label = new QLabel("Screen Material Diameter: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_screenMaterialDiameter = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_screenMaterialDiameter, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_screenMaterialDiameter,row++,0,1,3);

  // Screen To Glass Distance

  label = new QLabel("Screen To Glass Distance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_screenToGlassDistance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_screenToGlassDistance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_screenToGlassDistance,row++,0,1,3);

  // Top Opening Multiplier

  label = new QLabel("Top Opening Multiplier: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_topOpeningMultiplier = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_topOpeningMultiplier, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_topOpeningMultiplier,row++,0,1,3);

  // Bottom Opening Multiplier

  label = new QLabel("Bottom Opening Multiplier: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_bottomOpeningMultiplier = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_bottomOpeningMultiplier, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_bottomOpeningMultiplier,row++,0,1,3);

  // Left Side Opening Multiplier

  label = new QLabel("Left Side Opening Multiplier: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_leftSideOpeningMultiplier = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_leftSideOpeningMultiplier, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_leftSideOpeningMultiplier,row++,0,1,3);

  // Right Side Opening Multiplier

  label = new QLabel("Right Side Opening Multiplier: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_rightSideOpeningMultiplier = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_rightSideOpeningMultiplier, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_rightSideOpeningMultiplier,row++,0,1,3);  

  // Angle Of Resolution For Screen Transmittance Output Map

  label = new QLabel("Angle Of Resolution For Screen Transmittance Output Map: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_angleOfResolutionForScreenTransmittanceOutputMap = new OSComboBox();
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
  m_reflectedBeamTransmittanceAccountingMethod->bind(screen,"reflectedBeamTransmittanceAccountingMethod");
  m_angleOfResolutionForScreenTransmittanceOutputMap->bind(screen,"angleofResolutionforScreenTransmittanceOutputMap");

  m_nameEdit->bind(screen,"name");
  m_diffuseSolarReflectance->bind(screen,"diffuseSolarReflectance",m_isIP);
  m_diffuseVisibleReflectance->bind(screen,"diffuseVisibleReflectance",m_isIP);
  m_thermalHemisphericalEmissivity->bind(screen,"thermalHemisphericalEmissivity",m_isIP);
  m_conductivity->bind(screen,"conductivity",m_isIP);
  m_screenMaterialSpacing->bind(screen,"screenMaterialSpacing",m_isIP);
  m_screenMaterialDiameter->bind(screen,"screenMaterialDiameter",m_isIP);
  m_screenToGlassDistance->bind(screen,"screentoGlassDistance",m_isIP);
  m_topOpeningMultiplier->bind(screen,"topOpeningMultiplier",m_isIP);
  m_bottomOpeningMultiplier->bind(screen,"bottomOpeningMultiplier",m_isIP);
  m_leftSideOpeningMultiplier->bind(screen,"leftSideOpeningMultiplier",m_isIP);
  m_rightSideOpeningMultiplier->bind(screen,"rightSideOpeningMultiplier",m_isIP);

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
}

void WindowMaterialScreenInspectorView::refresh()
{
}

void WindowMaterialScreenInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio
