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

#include "MaterialRoofVegetationInspectorView.hpp"

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
  : ModelObjectInspectorView(model, true, parent),
    m_roughness(NULL),
    m_nameEdit(NULL),
    m_soilLayerName(NULL),
    m_moistureDiffusionCalculationMethod(NULL),
    m_heightOfPlants(NULL),
    m_leafAreaIndex(NULL),
    m_leafReflectivity(NULL),
    m_leafEmissivity(NULL),
    m_minimumStomatalResistance(NULL),
    m_thickness(NULL),
    m_conductivityOfDrySoil(NULL),
    m_densityOfDrySoil(NULL),
    m_specificHeatOfDrySoil(NULL),
    m_thermalAbsorptance(NULL),
    m_visibleAbsorptance(NULL),
    m_saturationVolumetricMoistureContentOfTheSoilLayer(NULL),
    m_residualVolumetricMoistureContentOfTheSoilLayer(NULL),
    m_initialVolumetricMoistureContentOfTheSoilLayer(NULL),
    m_solarAbsorptance(NULL),
    m_isIP(isIP)
{
  createLayout();
}

void MaterialRoofVegetationInspectorView::createLayout()
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

  // Height Of Plants

  label = new QLabel("Height Of Plants: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_heightOfPlants = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_heightOfPlants, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_heightOfPlants,row++,0,1,3);

  // Leaf Area Index

  label = new QLabel("Leaf Area Index: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_leafAreaIndex = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_leafAreaIndex, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_leafAreaIndex,row++,0,1,3);

  // Leaf Reflectivity

  label = new QLabel("Leaf Reflectivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_leafReflectivity = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_leafReflectivity, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_leafReflectivity,row++,0,1,3);

  // Leaf Emissivity

  label = new QLabel("Leaf Emissivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_leafEmissivity = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_leafEmissivity, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_leafEmissivity,row++,0,1,3);

  // Minimum Stomatal Resistance

  label = new QLabel("Minimum Stomatal Resistance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_minimumStomatalResistance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_minimumStomatalResistance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_minimumStomatalResistance,row++,0,1,3);

  // Soil Layer Name

  label = new QLabel("Soil Layer Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_soilLayerName = new OSLineEdit();
  mainGridLayout->addWidget(m_soilLayerName,row++,0,1,3);

 // Roughness

  label = new QLabel("Roughness: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

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
  mainGridLayout->addWidget(label,row++,col);

  m_thickness = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_thickness, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_thickness,row++,0,1,3);

  // Conductivity Of Dry Soil

  label = new QLabel("Conductivity Of Dry Soil: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_conductivityOfDrySoil = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_conductivityOfDrySoil, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_conductivityOfDrySoil,row++,0,1,3);

  // Density Of Dry Soil

  label = new QLabel("Density Of Dry Soil: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_densityOfDrySoil = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_densityOfDrySoil, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_densityOfDrySoil,row++,0,1,3);

  // Specific Heat Of Dry Soil

  label = new QLabel("Specific Heat Of Dry Soil: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_specificHeatOfDrySoil = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_specificHeatOfDrySoil, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_specificHeatOfDrySoil,row++,0,1,3);

  // Thermal Absorptance

  label = new QLabel("Thermal Absorptance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_thermalAbsorptance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_thermalAbsorptance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_thermalAbsorptance,row++,0,1,3);

  // Solar Absorptance

  label = new QLabel("Solar Absorptance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_solarAbsorptance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_solarAbsorptance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_solarAbsorptance,row++,0,1,3);

  // Visible Absorptance

  label = new QLabel("Visible Absorptance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_visibleAbsorptance = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_visibleAbsorptance, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_visibleAbsorptance,row++,0,1,3);

  // Saturation Volumetric Moisture Content Of The Soil Layer

  label = new QLabel("Saturation Volumetric Moisture Content Of The Soil Layer: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_saturationVolumetricMoistureContentOfTheSoilLayer = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_saturationVolumetricMoistureContentOfTheSoilLayer, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_saturationVolumetricMoistureContentOfTheSoilLayer,row++,0,1,3);

  // Residual Volumetric Moisture Content Of The Soil Layer

  label = new QLabel("Residual Volumetric Moisture Content Of The Soil Layer: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_residualVolumetricMoistureContentOfTheSoilLayer = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_residualVolumetricMoistureContentOfTheSoilLayer, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_residualVolumetricMoistureContentOfTheSoilLayer,row++,0,1,3);

  // Initial Volumetric Moisture Content Of The Soil Layer

  label = new QLabel("Initial Volumetric Moisture Content Of The Soil Layer: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

  m_initialVolumetricMoistureContentOfTheSoilLayer = new OSQuantityEdit(m_isIP);
  isConnected = connect(this, SIGNAL(toggleUnitsClicked(bool)), m_initialVolumetricMoistureContentOfTheSoilLayer, SLOT(onUnitSystemChange(bool)));
  OS_ASSERT(isConnected);
  mainGridLayout->addWidget(m_initialVolumetricMoistureContentOfTheSoilLayer,row++,0,1,3);

  // Moisture Diffusion Calculation Method

  label = new QLabel("Moisture Diffusion Calculation Method: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,col);

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
}

void MaterialRoofVegetationInspectorView::refresh()
{
}

void MaterialRoofVegetationInspectorView::toggleUnits(bool displayIP)
{
  m_isIP = displayIP;
}

} // openstudio
