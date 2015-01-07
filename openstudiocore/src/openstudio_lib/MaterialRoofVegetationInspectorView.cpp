/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "MaterialRoofVegetationInspectorView.hpp"

#include "StandardsInformationMaterialWidget.hpp"

#include "../shared_gui_components/OSComboBox.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"
#include "../shared_gui_components/OSQuantityEdit.hpp"

#include "../model/RoofVegetation.hpp"
#include "../model/RoofVegetation_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>

namespace openstudio {

// MaterialRoofVegetationInspectorView

MaterialRoofVegetationInspectorView::MaterialRoofVegetationInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent)
  : ModelObjectInspectorView(model, true, parent)
{
  createLayout();
}

void MaterialRoofVegetationInspectorView::createLayout()
{
  QWidget* hiddenWidget = new QWidget();
  this->stackedWidget()->addWidget(hiddenWidget);

  QWidget* visibleWidget = new QWidget();
  this->stackedWidget()->addWidget(visibleWidget);

  QGridLayout* mainGridLayout = new QGridLayout();
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

  m_nameEdit = new OSLineEdit();
  mainGridLayout->addWidget(m_nameEdit, row, 0, 1, 3);

  ++row;

  // Standards Information

  m_standardsInformationWidget = new StandardsInformationMaterialWidget(m_isIP, mainGridLayout, row);

  ++row;

  // Height Of Plants

  label = new QLabel("Height Of Plants: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_heightOfPlants = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_heightOfPlants, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_heightOfPlants,row++,0,1,3);

  // Leaf Area Index

  label = new QLabel("Leaf Area Index: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_leafAreaIndex = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_leafAreaIndex, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_leafAreaIndex,row++,0,1,3);

  // Leaf Reflectivity

  label = new QLabel("Leaf Reflectivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_leafReflectivity = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_leafReflectivity, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_leafReflectivity,row++,0,1,3);

  // Leaf Emissivity

  label = new QLabel("Leaf Emissivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_leafEmissivity = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_leafEmissivity, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_leafEmissivity,row++,0,1,3);

  // Minimum Stomatal Resistance

  label = new QLabel("Minimum Stomatal Resistance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_minimumStomatalResistance = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_minimumStomatalResistance, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_minimumStomatalResistance,row++,0,1,3);

  // Soil Layer Name

  label = new QLabel("Soil Layer Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_soilLayerName = new OSLineEdit();
  mainGridLayout->addWidget(m_soilLayerName,row++,0,1,3);

 // Roughness

  label = new QLabel("Roughness: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_roughness = new OSComboBox();
  m_roughness->addItem("Very Rough");
  m_roughness->addItem("Rough");
  m_roughness->addItem("Medium Rough");
  m_roughness->addItem("Medium Smooth");
  m_roughness->addItem("Smooth");
  m_roughness->addItem("Very Smooth");
  mainGridLayout->addWidget(m_roughness,row++,0,1,3);

  // Thickness

  label = new QLabel("Thickness: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_thickness = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_thickness, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_thickness,row++,0,1,3);

  // Conductivity Of Dry Soil

  label = new QLabel("Conductivity Of Dry Soil: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_conductivityOfDrySoil = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_conductivityOfDrySoil, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_conductivityOfDrySoil,row++,0,1,3);

  // Density Of Dry Soil

  label = new QLabel("Density Of Dry Soil: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_densityOfDrySoil = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_densityOfDrySoil, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_densityOfDrySoil,row++,0,1,3);

  // Specific Heat Of Dry Soil

  label = new QLabel("Specific Heat Of Dry Soil: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_specificHeatOfDrySoil = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_specificHeatOfDrySoil, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_specificHeatOfDrySoil,row++,0,1,3);

  // Thermal Absorptance

  label = new QLabel("Thermal Absorptance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_thermalAbsorptance = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_thermalAbsorptance, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_thermalAbsorptance,row++,0,1,3);

  // Solar Absorptance

  label = new QLabel("Solar Absorptance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_solarAbsorptance = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_solarAbsorptance, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_solarAbsorptance,row++,0,1,3);

  // Visible Absorptance

  label = new QLabel("Visible Absorptance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_visibleAbsorptance = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_visibleAbsorptance, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_visibleAbsorptance,row++,0,1,3);

  // Saturation Volumetric Moisture Content Of The Soil Layer

  label = new QLabel("Saturation Volumetric Moisture Content Of The Soil Layer: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_saturationVolumetricMoistureContentOfTheSoilLayer = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_saturationVolumetricMoistureContentOfTheSoilLayer, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_saturationVolumetricMoistureContentOfTheSoilLayer,row++,0,1,3);

  // Residual Volumetric Moisture Content Of The Soil Layer

  label = new QLabel("Residual Volumetric Moisture Content Of The Soil Layer: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_residualVolumetricMoistureContentOfTheSoilLayer = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_residualVolumetricMoistureContentOfTheSoilLayer, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_residualVolumetricMoistureContentOfTheSoilLayer,row++,0,1,3);

  // Initial Volumetric Moisture Content Of The Soil Layer

  label = new QLabel("Initial Volumetric Moisture Content Of The Soil Layer: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_initialVolumetricMoistureContentOfTheSoilLayer = new OSQuantityEdit(m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_initialVolumetricMoistureContentOfTheSoilLayer, &OSQuantityEdit::onUnitSystemChange);
  mainGridLayout->addWidget(m_initialVolumetricMoistureContentOfTheSoilLayer,row++,0,1,3);

  // Moisture Diffusion Calculation Method

  label = new QLabel("Moisture Diffusion Calculation Method: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_moistureDiffusionCalculationMethod = new OSLineEdit();
  mainGridLayout->addWidget(m_moistureDiffusionCalculationMethod,row++,0,1,3);

  // Stretch

  mainGridLayout->setRowStretch(100,100);

  mainGridLayout->setColumnStretch(100,100);
}

void MaterialRoofVegetationInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  detach();
}

void MaterialRoofVegetationInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::RoofVegetation roofVegetation = modelObject.cast<model::RoofVegetation>();
  attach(roofVegetation);
  refresh();
}

void MaterialRoofVegetationInspectorView::onUpdate()
{
  refresh();
}

void MaterialRoofVegetationInspectorView::attach(openstudio::model::RoofVegetation & roofVegetation)
{
  m_roughness->bind(roofVegetation,"roughness");

  m_nameEdit->bind(roofVegetation,"name");
  m_soilLayerName->bind(roofVegetation,"soilLayerName");
  m_moistureDiffusionCalculationMethod->bind(roofVegetation,"moistureDiffusionCalculationMethod");
  m_heightOfPlants->bind(roofVegetation,"heightofPlants",m_isIP);
  m_leafAreaIndex->bind(roofVegetation,"leafAreaIndex",m_isIP);
  m_leafReflectivity->bind(roofVegetation,"leafReflectivity",m_isIP);
  m_leafEmissivity->bind(roofVegetation,"leafEmissivity",m_isIP);
  m_minimumStomatalResistance->bind(roofVegetation,"minimumStomatalResistance",m_isIP);
  m_thickness->bind(roofVegetation,"thickness",m_isIP);
  m_conductivityOfDrySoil->bind(roofVegetation,"conductivityofDrySoil",m_isIP);
  m_densityOfDrySoil->bind(roofVegetation,"densityofDrySoil",m_isIP);
  m_specificHeatOfDrySoil->bind(roofVegetation,"specificHeatofDrySoil",m_isIP);
  m_thermalAbsorptance->bind(roofVegetation,"thermalAbsorptance",m_isIP);
  m_visibleAbsorptance->bind(roofVegetation,"visibleAbsorptance",m_isIP);
  m_saturationVolumetricMoistureContentOfTheSoilLayer->bind(roofVegetation,"saturationVolumetricMoistureContentoftheSoilLayer",m_isIP);
  m_residualVolumetricMoistureContentOfTheSoilLayer->bind(roofVegetation,"residualVolumetricMoistureContentoftheSoilLayer",m_isIP);
  m_initialVolumetricMoistureContentOfTheSoilLayer->bind(roofVegetation,"initialVolumetricMoistureContentoftheSoilLayer",m_isIP);
  m_solarAbsorptance->bind(roofVegetation,"solarAbsorptance",m_isIP);

  m_standardsInformationWidget->attach(roofVegetation);

  this->stackedWidget()->setCurrentIndex(1);
}

void MaterialRoofVegetationInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_roughness->unbind();

  m_nameEdit->unbind();
  m_soilLayerName->unbind();
  m_moistureDiffusionCalculationMethod->unbind();
  m_heightOfPlants->unbind();
  m_leafAreaIndex->unbind();
  m_leafReflectivity->unbind();
  m_leafEmissivity->unbind();
  m_minimumStomatalResistance->unbind();
  m_thickness->unbind();
  m_conductivityOfDrySoil->unbind();
  m_densityOfDrySoil->unbind();
  m_specificHeatOfDrySoil->unbind();
  m_thermalAbsorptance->unbind();
  m_visibleAbsorptance->unbind();
  m_saturationVolumetricMoistureContentOfTheSoilLayer->unbind();
  m_residualVolumetricMoistureContentOfTheSoilLayer->unbind();
  m_initialVolumetricMoistureContentOfTheSoilLayer->unbind();
  m_solarAbsorptance->unbind();

  m_standardsInformationWidget->detach();
}

void MaterialRoofVegetationInspectorView::refresh()
{
}

} // openstudio
