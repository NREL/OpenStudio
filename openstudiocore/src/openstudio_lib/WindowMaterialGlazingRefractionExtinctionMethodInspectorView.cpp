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

#include "WindowMaterialGlazingRefractionExtinctionMethodInspectorView.hpp"

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
  : MaterialBaseInspectorView(isIP, model, parent),
    m_solarDiffusing(nullptr),
    m_thickness(nullptr),
    m_solarIndexOfRefraction(nullptr),
    m_solarExtinctionCoefficient(nullptr),
    m_visibleIndexOfRefraction(nullptr),
    m_visibleExtinctionCoefficient(nullptr),
    m_infraredTransmittanceAtNormalIncidence(nullptr),
    m_infraredHemisphericalEmissivity(nullptr),
    m_conductivity(nullptr),
    m_dirtCorrectionFactorForSolarAndVisibleTransmittance(nullptr)
{
  createLayout();
}

void WindowMaterialGlazingRefractionExtinctionMethodInspectorView::createLayout()
{
  QLabel * label = nullptr;

  unsigned row = m_mainGridLayout->rowCount();

  // Thickness

  label = new QLabel("Thickness: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row++,0);

  m_thickness = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialGlazingRefractionExtinctionMethodInspectorView::toggleUnitsClicked, m_thickness, &OSQuantityEdit::onUnitSystemChange);
  m_mainGridLayout->addWidget(m_thickness,row++,0,1,3);

  // Solar Index Of Refraction

  label = new QLabel("Solar Index Of Refraction: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row++,0);

  m_solarIndexOfRefraction = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialGlazingRefractionExtinctionMethodInspectorView::toggleUnitsClicked, m_solarIndexOfRefraction, &OSQuantityEdit::onUnitSystemChange);
  m_mainGridLayout->addWidget(m_solarIndexOfRefraction,row++,0,1,3);

  // Solar Extinction Coefficient

  label = new QLabel("Solar Extinction Coefficient: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row++,0);

  m_solarExtinctionCoefficient = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialGlazingRefractionExtinctionMethodInspectorView::toggleUnitsClicked, m_solarExtinctionCoefficient, &OSQuantityEdit::onUnitSystemChange);
  m_mainGridLayout->addWidget(m_solarExtinctionCoefficient,row++,0,1,3);

  // Visible Index of Refraction

  label = new QLabel("Visible Index of Refraction: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row++,0);

  m_visibleIndexOfRefraction = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialGlazingRefractionExtinctionMethodInspectorView::toggleUnitsClicked, m_visibleIndexOfRefraction, &OSQuantityEdit::onUnitSystemChange);
  m_mainGridLayout->addWidget(m_visibleIndexOfRefraction,row++,0,1,3);

  // Visible Extinction Coefficient

  label = new QLabel("Visible Extinction Coefficient: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row++,0);

  m_visibleExtinctionCoefficient = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialGlazingRefractionExtinctionMethodInspectorView::toggleUnitsClicked, m_visibleExtinctionCoefficient, &OSQuantityEdit::onUnitSystemChange);
  m_mainGridLayout->addWidget(m_visibleExtinctionCoefficient,row++,0,1,3);

  // Infrared Transmittance At Normal Incidence

  label = new QLabel("Infrared Transmittance At Normal Incidence: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row++,0);

  m_infraredTransmittanceAtNormalIncidence = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialGlazingRefractionExtinctionMethodInspectorView::toggleUnitsClicked, m_infraredTransmittanceAtNormalIncidence, &OSQuantityEdit::onUnitSystemChange);
  m_mainGridLayout->addWidget(m_infraredTransmittanceAtNormalIncidence,row++,0,1,3);

  // Infrared Hemispherical Emissivity

  label = new QLabel("Infrared Hemispherical Emissivity: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row++,0);

  m_infraredHemisphericalEmissivity = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialGlazingRefractionExtinctionMethodInspectorView::toggleUnitsClicked, m_infraredHemisphericalEmissivity, &OSQuantityEdit::onUnitSystemChange);
  m_mainGridLayout->addWidget(m_infraredHemisphericalEmissivity,row++,0,1,3);

  // Conductivity

  label = new QLabel("Conductivity: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row++,0);

  m_conductivity = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialGlazingRefractionExtinctionMethodInspectorView::toggleUnitsClicked, m_conductivity, &OSQuantityEdit::onUnitSystemChange);
  m_mainGridLayout->addWidget(m_conductivity,row++,0,1,3);

  // Dirt Correction Factor For Solar And Visible Transmittance

  label = new QLabel("Dirt Correction Factor For Solar And Visible Transmittance: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row++,0);

  m_dirtCorrectionFactorForSolarAndVisibleTransmittance = new OSQuantityEdit(m_isIP);
  connect(this, &WindowMaterialGlazingRefractionExtinctionMethodInspectorView::toggleUnitsClicked, m_dirtCorrectionFactorForSolarAndVisibleTransmittance, &OSQuantityEdit::onUnitSystemChange);
  m_mainGridLayout->addWidget(m_dirtCorrectionFactorForSolarAndVisibleTransmittance,row++,0,1,3);

  // Solar Diffusing

  label = new QLabel("Solar Diffusing: ");
  label->setObjectName("H2");
  m_mainGridLayout->addWidget(label,row++,0);

  m_solarDiffusing = new OSSwitch();
  m_mainGridLayout->addWidget(m_solarDiffusing,row++,0,1,3);

  // Stretch

  m_mainGridLayout->setRowStretch(100,100);

  m_mainGridLayout->setColumnStretch(100,100);
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
  m_solarDiffusing->bind(refractionExtinctionGlazing,"solarDiffusing");

  m_nameEdit->bind(refractionExtinctionGlazing,"name");
  m_conductivity->bind(refractionExtinctionGlazing,"conductivity",m_isIP);
  m_dirtCorrectionFactorForSolarAndVisibleTransmittance->bind(refractionExtinctionGlazing,"dirtCorrectionFactorforSolarandVisibleTransmittance",m_isIP);
  m_infraredHemisphericalEmissivity->bind(refractionExtinctionGlazing,"infraredHemisphericalEmissivity",m_isIP);
  m_infraredTransmittanceAtNormalIncidence->bind(refractionExtinctionGlazing,"infraredTransmittanceatNormalIncidence",m_isIP);
  m_solarExtinctionCoefficient->bind(refractionExtinctionGlazing,"solarExtinctionCoefficient",m_isIP);
  m_solarIndexOfRefraction->bind(refractionExtinctionGlazing,"solarIndexofRefraction",m_isIP);
  m_thickness->bind(refractionExtinctionGlazing,"thickness",m_isIP);
  m_visibleExtinctionCoefficient->bind(refractionExtinctionGlazing,"visibleExtinctionCoefficient",m_isIP);
  m_visibleIndexOfRefraction->bind(refractionExtinctionGlazing,"visibleIndexofRefraction",m_isIP);

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
}

void WindowMaterialGlazingRefractionExtinctionMethodInspectorView::refresh()
{
}

} // openstudio
