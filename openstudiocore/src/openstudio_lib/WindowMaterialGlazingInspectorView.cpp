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

#include "WindowMaterialGlazingInspectorView.hpp"

#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"
#include "../shared_gui_components/OSSwitch.hpp"

#include "../model/Glazing.hpp"
#include "../model/Glazing_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

// WindowMaterialGlazingInspectorView

WindowMaterialGlazingInspectorView::WindowMaterialGlazingInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
    m_opticalDataType(NULL),
    m_solarDiffusing(NULL),
    m_nameEdit(NULL),
    m_windowGlassSpectralDataSetName(NULL),
    m_thickness(NULL),
    m_solarTransmittanceAtNormalIncidence(NULL),
    m_frontSideSolarReflectanceAtNormalIncidence(NULL),
    m_backSideSolarReflectanceAtNormalIncidence(NULL),
    m_visibleTransmittanceAtNormalIncidence(NULL),
    m_frontSideVisibleReflectanceAtNormalIncidence(NULL),
    m_backSideVisibleReflectanceAtNormalIncidence(NULL),
    m_infraredTransmittanceAtNormalIncidence(NULL),
    m_frontSideInfraredHemisphericalEmissivity(NULL),
    m_backSideInfraredHemisphericalEmissivity(NULL),
    m_conductivity(NULL),
    m_dirtCorrectionFactorForSolarAndVisibleTransmittance(NULL),
    m_isIP(isIP)
{
  createLayout();
}

void WindowMaterialGlazingInspectorView::createLayout()
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

  // Optical Data Type

  label = new QLabel("Optical Data Type: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_opticalDataType = new OSComboBox();
  m_opticalDataType->addItem("Spectral Average");
  m_opticalDataType->addItem("Spectral");
  mainGridLayout->addWidget(m_opticalDataType,row++,0,1,3);

  // Window Glass Spectral Data Set Name

  label = new QLabel("Window Glass Spectral Data Set Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_windowGlassSpectralDataSetName = new OSLineEdit();
  mainGridLayout->addWidget(m_windowGlassSpectralDataSetName,row++,0,1,3);

  // Thickness

  label = new QLabel("Thickness: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_thickness = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_thickness, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_thickness,row++,0,1,3);

    // Solar Transmittance At Normal Incidence

  label = new QLabel("Solar Transmittance At Normal Incidence: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_solarTransmittanceAtNormalIncidence = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_solarTransmittanceAtNormalIncidence, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_solarTransmittanceAtNormalIncidence,row++,0,1,3);

  // Front Side Solar Reflectance At Normal Incidence

  label = new QLabel("Front Side Solar Reflectance At Normal Incidence: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_frontSideSolarReflectanceAtNormalIncidence = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_frontSideSolarReflectanceAtNormalIncidence, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_frontSideSolarReflectanceAtNormalIncidence,row++,0,1,3);

  // Back Side Solar Reflectance At Normal Incidence

  label = new QLabel("Back Side Solar Reflectance At Normal Incidence: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_backSideSolarReflectanceAtNormalIncidence = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_backSideSolarReflectanceAtNormalIncidence, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_backSideSolarReflectanceAtNormalIncidence,row++,0,1,3);

  // Visible Transmittance At Normal Incidence

  label = new QLabel("Visible Transmittance At Normal Incidence: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_visibleTransmittanceAtNormalIncidence = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_visibleTransmittanceAtNormalIncidence, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_visibleTransmittanceAtNormalIncidence,row++,0,1,3);

  // Front Side Visible Reflectance At Normal Incidence

  label = new QLabel("Front Side Visible Reflectance At Normal Incidence: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_frontSideVisibleReflectanceAtNormalIncidence = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_frontSideVisibleReflectanceAtNormalIncidence, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_frontSideVisibleReflectanceAtNormalIncidence,row++,0,1,3);

  // Back Side Visible Reflectance At Normal Incidence

  label = new QLabel("Back Side Visible Reflectance At Normal Incidence: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_backSideVisibleReflectanceAtNormalIncidence = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_backSideVisibleReflectanceAtNormalIncidence, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_backSideVisibleReflectanceAtNormalIncidence,row++,0,1,3);

  // Infrared Transmittance at Normal Incidence

  label = new QLabel("Infrared Transmittance at Normal Incidence: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_infraredTransmittanceAtNormalIncidence = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_infraredTransmittanceAtNormalIncidence, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_infraredTransmittanceAtNormalIncidence,row++,0,1,3);

  // Front Side Infrared Hemispherical Emissivity

  label = new QLabel("Front Side Infrared Hemispherical Emissivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_frontSideInfraredHemisphericalEmissivity = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_frontSideInfraredHemisphericalEmissivity, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_frontSideInfraredHemisphericalEmissivity,row++,0,1,3);

  // Back Side Infrared Hemispherical Emissivity

  label = new QLabel("Back Side Infrared Hemispherical Emissivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_backSideInfraredHemisphericalEmissivity = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_backSideInfraredHemisphericalEmissivity, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_backSideInfraredHemisphericalEmissivity,row++,0,1,3);

  // Conductivity

  label = new QLabel("Conductivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_conductivity = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_conductivity, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_conductivity,row++,0,1,3);

  // Dirt Correction Factor For Solar And Visible Transmittance

  label = new QLabel("Dirt Correction Factor For Solar And Visible Transmittance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_dirtCorrectionFactorForSolarAndVisibleTransmittance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_dirtCorrectionFactorForSolarAndVisibleTransmittance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_dirtCorrectionFactorForSolarAndVisibleTransmittance,row++,0,1,3);

  // Solar Diffusing

  label = new QLabel("Solar Diffusing: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_solarDiffusing = new OSSwitch();
  mainGridLayout->addWidget(m_solarDiffusing,row++,0,1,3); 

  // Stretch

  mainGridLayout->setRowStretch(100,100);

  mainGridLayout->setColumnStretch(100,100);
}

void WindowMaterialGlazingInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void WindowMaterialGlazingInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::Glazing glazing = modelObject.cast<model::Glazing>();
  attach(glazing);
  refresh();
}

void WindowMaterialGlazingInspectorView::onUpdate()
{
  refresh();
}

void WindowMaterialGlazingInspectorView::attach(openstudio::model::Glazing & glazing)
{
  m_opticalDataType->bind(glazing,"opticalDataType");
  m_solarDiffusing->bind(glazing,"solarDiffusing");
    
  m_nameEdit->bind(glazing,"name");
  m_windowGlassSpectralDataSetName->bind(glazing,"windowGlassSpectralDataSetName");
  m_thickness->bind(glazing,"thickness",m_isIP);
  m_solarTransmittanceAtNormalIncidence->bind(glazing,"solarTransmittanceatNormalIncidence",m_isIP);
  m_frontSideSolarReflectanceAtNormalIncidence->bind(glazing,"frontSideSolarReflectanceatNormalIncidence",m_isIP);
  m_backSideSolarReflectanceAtNormalIncidence->bind(glazing,"backSideSolarReflectanceatNormalIncidence",m_isIP);
  m_visibleTransmittanceAtNormalIncidence->bind(glazing,"visibleTransmittanceatNormalIncidence",m_isIP);
  m_frontSideVisibleReflectanceAtNormalIncidence->bind(glazing,"frontSideVisibleReflectanceatNormalIncidence",m_isIP);
  m_backSideVisibleReflectanceAtNormalIncidence->bind(glazing,"backSideVisibleReflectanceatNormalIncidence",m_isIP);
  m_infraredTransmittanceAtNormalIncidence->bind(glazing,"infraredTransmittanceatNormalIncidence",m_isIP);
  m_frontSideInfraredHemisphericalEmissivity->bind(glazing,"frontSideInfraredHemisphericalEmissivity",m_isIP);
  m_backSideInfraredHemisphericalEmissivity->bind(glazing,"backSideInfraredHemisphericalEmissivity",m_isIP);
  m_conductivity->bind(glazing,"conductivity",m_isIP);
  m_dirtCorrectionFactorForSolarAndVisibleTransmittance->bind(glazing,"dirtCorrectionFactorforSolarandVisibleTransmittance",m_isIP);

  this->stackedWidget()->setCurrentIndex(1);
}

void WindowMaterialGlazingInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_opticalDataType->unbind();
  m_solarDiffusing->unbind();

  m_nameEdit->unbind();
  m_windowGlassSpectralDataSetName->unbind();
  m_thickness->unbind();
  m_solarTransmittanceAtNormalIncidence->unbind();
  m_frontSideSolarReflectanceAtNormalIncidence->unbind();
  m_backSideSolarReflectanceAtNormalIncidence->unbind();
  m_visibleTransmittanceAtNormalIncidence->unbind();
  m_frontSideVisibleReflectanceAtNormalIncidence->unbind();
  m_backSideVisibleReflectanceAtNormalIncidence->unbind();
  m_infraredTransmittanceAtNormalIncidence->unbind();
  m_frontSideInfraredHemisphericalEmissivity->unbind();
  m_backSideInfraredHemisphericalEmissivity->unbind();
  m_conductivity->unbind();
  m_dirtCorrectionFactorForSolarAndVisibleTransmittance->unbind();
}

void WindowMaterialGlazingInspectorView::refresh()
{
}

void WindowMaterialGlazingInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio
