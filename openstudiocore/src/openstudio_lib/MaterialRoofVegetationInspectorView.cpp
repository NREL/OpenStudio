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
  : ModelObjectInspectorView(model, true, parent),
  m_isIP(isIP)
{
  createLayout();
}

void MaterialRoofVegetationInspectorView::createLayout()
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

  // Height Of Plants

  label = new QLabel("Height Of Plants: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_heightOfPlants = new OSQuantityEdit2("m","m","in", m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_heightOfPlants, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_heightOfPlants,row++,0,1,3);

  // Leaf Area Index

  label = new QLabel("Leaf Area Index: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_leafAreaIndex = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_leafAreaIndex, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_leafAreaIndex,row++,0,1,3);

  // Leaf Reflectivity

  label = new QLabel("Leaf Reflectivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_leafReflectivity = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_leafReflectivity, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_leafReflectivity,row++,0,1,3);

  // Leaf Emissivity

  label = new QLabel("Leaf Emissivity: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_leafEmissivity = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_leafEmissivity, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_leafEmissivity,row++,0,1,3);

  // Minimum Stomatal Resistance

  label = new QLabel("Minimum Stomatal Resistance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_minimumStomatalResistance = new OSQuantityEdit2("s/m","s/m","min/ft", m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_minimumStomatalResistance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_minimumStomatalResistance,row++,0,1,3);

  // Soil Layer Name

  label = new QLabel("Soil Layer Name: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_soilLayerName = new OSLineEdit2();
  mainGridLayout->addWidget(m_soilLayerName,row++,0,1,3);

 // Roughness

  label = new QLabel("Roughness: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_roughness = new OSComboBox2();
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

  m_thickness = new OSQuantityEdit2("m","m","in", m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_thickness, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_thickness,row++,0,1,3);

  // Conductivity Of Dry Soil

  label = new QLabel("Conductivity Of Dry Soil: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_conductivityOfDrySoil = new OSQuantityEdit2("W/m*K", "W/m*K", "Btu*in/hr*ft^2*R", m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_conductivityOfDrySoil, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_conductivityOfDrySoil,row++,0,1,3);

  // Density Of Dry Soil

  label = new QLabel("Density Of Dry Soil: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_densityOfDrySoil = new OSQuantityEdit2("kg/m^3","kg/m^3","lb/ft^3", m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_densityOfDrySoil, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_densityOfDrySoil,row++,0,1,3);

  // Specific Heat Of Dry Soil

  label = new QLabel("Specific Heat Of Dry Soil: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_specificHeatOfDrySoil = new OSQuantityEdit2("J/kg*K","J/kg*K","Btu/lb*R", m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_specificHeatOfDrySoil, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_specificHeatOfDrySoil,row++,0,1,3);

  // Thermal Absorptance

  label = new QLabel("Thermal Absorptance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_thermalAbsorptance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_thermalAbsorptance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_thermalAbsorptance,row++,0,1,3);

  // Solar Absorptance

  label = new QLabel("Solar Absorptance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_solarAbsorptance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_solarAbsorptance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_solarAbsorptance,row++,0,1,3);

  // Visible Absorptance

  label = new QLabel("Visible Absorptance: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_visibleAbsorptance = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_visibleAbsorptance, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_visibleAbsorptance,row++,0,1,3);

  // Saturation Volumetric Moisture Content Of The Soil Layer

  label = new QLabel("Saturation Volumetric Moisture Content Of The Soil Layer: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_saturationVolumetricMoistureContentOfTheSoilLayer = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_saturationVolumetricMoistureContentOfTheSoilLayer, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_saturationVolumetricMoistureContentOfTheSoilLayer,row++,0,1,3);

  // Residual Volumetric Moisture Content Of The Soil Layer

  label = new QLabel("Residual Volumetric Moisture Content Of The Soil Layer: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_residualVolumetricMoistureContentOfTheSoilLayer = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_residualVolumetricMoistureContentOfTheSoilLayer, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_residualVolumetricMoistureContentOfTheSoilLayer,row++,0,1,3);

  // Initial Volumetric Moisture Content Of The Soil Layer

  label = new QLabel("Initial Volumetric Moisture Content Of The Soil Layer: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_initialVolumetricMoistureContentOfTheSoilLayer = new OSQuantityEdit2("","","", m_isIP);
  connect(this, &MaterialRoofVegetationInspectorView::toggleUnitsClicked, m_initialVolumetricMoistureContentOfTheSoilLayer, &OSQuantityEdit2::onUnitSystemChange);
  mainGridLayout->addWidget(m_initialVolumetricMoistureContentOfTheSoilLayer,row++,0,1,3);

  // Moisture Diffusion Calculation Method

  label = new QLabel("Moisture Diffusion Calculation Method: ");
  label->setObjectName("H2");
  mainGridLayout->addWidget(label,row++,0);

  m_moistureDiffusionCalculationMethod = new OSLineEdit2();
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
  m_roofVegetation = roofVegetation;

  // m_roughness->bind(roofVegetation,"roughness");
  if(m_roughness){
    m_roughness->bind<std::string>(
      *m_roofVegetation,
      static_cast<std::string (*)(const std::string&)>(&openstudio::toString),
      &model::RoofVegetation::roughnessValues,
      std::bind(&model::RoofVegetation::roughness, m_roofVegetation.get_ptr()),
      std::bind(&model::RoofVegetation::setRoughness, m_roofVegetation.get_ptr(), std::placeholders::_1),
      boost::optional<NoFailAction>(std::bind(&model::RoofVegetation::resetRoughness, m_roofVegetation.get_ptr())),
      boost::optional<BasicQuery>(std::bind(&model::RoofVegetation::isRoughnessDefaulted, m_roofVegetation.get_ptr())));
  }

  // m_nameEdit->bind(roofVegetation,"name");
  m_nameEdit->bind(
    *m_roofVegetation,
    OptionalStringGetter(std::bind(&model::RoofVegetation::name, m_roofVegetation.get_ptr(),true)),
    boost::optional<StringSetter>(std::bind(&model::RoofVegetation::setName, m_roofVegetation.get_ptr(),std::placeholders::_1))
  );

  // m_soilLayerName->bind(roofVegetation,"soilLayerName");
  m_soilLayerName->bind(
    *m_roofVegetation,
    StringGetter(std::bind(&model::RoofVegetation::soilLayerName, m_roofVegetation.get_ptr())),
    boost::optional<StringSetter>(std::bind(&model::RoofVegetation::setSoilLayerName, m_roofVegetation.get_ptr(),std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::RoofVegetation::resetSoilLayerName, m_roofVegetation.get_ptr())),
    boost::optional<BasicQuery>(std::bind(&model::RoofVegetation::isSoilLayerNameDefaulted, m_roofVegetation.get_ptr()))
  );

  // m_moistureDiffusionCalculationMethod->bind(roofVegetation,"moistureDiffusionCalculationMethod");
  m_moistureDiffusionCalculationMethod->bind(
    *m_roofVegetation,
    StringGetter(std::bind(&model::RoofVegetation::moistureDiffusionCalculationMethod, m_roofVegetation.get_ptr())),
    boost::optional<StringSetter>(std::bind(&model::RoofVegetation::setMoistureDiffusionCalculationMethod, m_roofVegetation.get_ptr(),std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::RoofVegetation::resetMoistureDiffusionCalculationMethod, m_roofVegetation.get_ptr())),
    boost::optional<BasicQuery>(std::bind(&model::RoofVegetation::isMoistureDiffusionCalculationMethodDefaulted, m_roofVegetation.get_ptr()))
  );

  // m_heightOfPlants->bind(roofVegetation,"heightofPlants",m_isIP);
  m_heightOfPlants->bind(
    m_isIP,
    *m_roofVegetation,
    DoubleGetter(std::bind(&model::RoofVegetation::heightofPlants, m_roofVegetation.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RoofVegetation::*)(double)>(&model::RoofVegetation::setHeightofPlants), m_roofVegetation.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::RoofVegetation::resetHeightofPlants, m_roofVegetation.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::RoofVegetation::isHeightofPlantsDefaulted, m_roofVegetation.get_ptr()))
  );

  // m_leafAreaIndex->bind(roofVegetation,"leafAreaIndex",m_isIP);
  m_leafAreaIndex->bind(
    m_isIP,
    *m_roofVegetation,
    DoubleGetter(std::bind(&model::RoofVegetation::leafAreaIndex, m_roofVegetation.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RoofVegetation::*)(double)>(&model::RoofVegetation::setLeafAreaIndex), m_roofVegetation.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::RoofVegetation::resetLeafAreaIndex, m_roofVegetation.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::RoofVegetation::isLeafAreaIndexDefaulted, m_roofVegetation.get_ptr()))
  );

  // m_leafReflectivity->bind(roofVegetation,"leafReflectivity",m_isIP);
  m_leafReflectivity->bind(
    m_isIP,
    *m_roofVegetation,
    DoubleGetter(std::bind(&model::RoofVegetation::leafReflectivity, m_roofVegetation.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RoofVegetation::*)(double)>(&model::RoofVegetation::setLeafReflectivity), m_roofVegetation.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::RoofVegetation::resetLeafReflectivity, m_roofVegetation.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::RoofVegetation::isLeafReflectivityDefaulted, m_roofVegetation.get_ptr()))
  );

  // m_leafEmissivity->bind(roofVegetation,"leafEmissivity",m_isIP);
  m_leafEmissivity->bind(
    m_isIP,
    *m_roofVegetation,
    DoubleGetter(std::bind(&model::RoofVegetation::leafEmissivity, m_roofVegetation.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RoofVegetation::*)(double)>(&model::RoofVegetation::setLeafEmissivity), m_roofVegetation.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::RoofVegetation::resetLeafEmissivity, m_roofVegetation.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::RoofVegetation::isLeafEmissivityDefaulted, m_roofVegetation.get_ptr()))
  );

  // m_minimumStomatalResistance->bind(roofVegetation,"minimumStomatalResistance",m_isIP);
  m_minimumStomatalResistance->bind(
    m_isIP,
    *m_roofVegetation,
    DoubleGetter(std::bind(&model::RoofVegetation::minimumStomatalResistance, m_roofVegetation.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RoofVegetation::*)(double)>(&model::RoofVegetation::setMinimumStomatalResistance), m_roofVegetation.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::RoofVegetation::resetMinimumStomatalResistance, m_roofVegetation.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::RoofVegetation::isMinimumStomatalResistanceDefaulted, m_roofVegetation.get_ptr()))
  );

  // m_thickness->bind(roofVegetation,"thickness",m_isIP);
  m_thickness->bind(
    m_isIP,
    *m_roofVegetation,
    DoubleGetter(std::bind(&model::RoofVegetation::thickness, m_roofVegetation.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RoofVegetation::*)(double)>(&model::RoofVegetation::setThickness), m_roofVegetation.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::RoofVegetation::resetThickness, m_roofVegetation.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::RoofVegetation::isThicknessDefaulted, m_roofVegetation.get_ptr()))
  );

  // m_conductivityOfDrySoil->bind(roofVegetation,"conductivityofDrySoil",m_isIP);
  m_conductivityOfDrySoil->bind(
    m_isIP,
    *m_roofVegetation,
    DoubleGetter(std::bind(&model::RoofVegetation::conductivityofDrySoil, m_roofVegetation.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RoofVegetation::*)(double)>(&model::RoofVegetation::setConductivityofDrySoil), m_roofVegetation.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::RoofVegetation::resetConductivityofDrySoil, m_roofVegetation.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::RoofVegetation::isConductivityofDrySoilDefaulted, m_roofVegetation.get_ptr()))
  );

  // m_densityOfDrySoil->bind(roofVegetation,"densityofDrySoil",m_isIP);
  m_densityOfDrySoil->bind(
    m_isIP,
    *m_roofVegetation,
    DoubleGetter(std::bind(&model::RoofVegetation::densityofDrySoil, m_roofVegetation.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RoofVegetation::*)(double)>(&model::RoofVegetation::setDensityofDrySoil), m_roofVegetation.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::RoofVegetation::resetDensityofDrySoil, m_roofVegetation.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::RoofVegetation::isDensityofDrySoilDefaulted, m_roofVegetation.get_ptr()))
  );

  // m_specificHeatOfDrySoil->bind(roofVegetation,"specificHeatofDrySoil",m_isIP);
  m_specificHeatOfDrySoil->bind(
    m_isIP,
    *m_roofVegetation,
    DoubleGetter(std::bind(&model::RoofVegetation::specificHeatofDrySoil, m_roofVegetation.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RoofVegetation::*)(double)>(&model::RoofVegetation::setSpecificHeatofDrySoil), m_roofVegetation.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::RoofVegetation::resetSpecificHeatofDrySoil, m_roofVegetation.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::RoofVegetation::isSpecificHeatofDrySoilDefaulted, m_roofVegetation.get_ptr()))
  );

  // m_thermalAbsorptance->bind(roofVegetation,"thermalAbsorptance",m_isIP);
  m_thermalAbsorptance->bind(
    m_isIP,
    *m_roofVegetation,
    OptionalDoubleGetter(std::bind(&model::RoofVegetation::thermalAbsorptance, m_roofVegetation.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RoofVegetation::*)(double)>(&model::RoofVegetation::setThermalAbsorptance), m_roofVegetation.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::RoofVegetation::resetThermalAbsorptance, m_roofVegetation.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::RoofVegetation::isThermalAbsorptanceDefaulted, m_roofVegetation.get_ptr()))
  );

  // m_visibleAbsorptance->bind(roofVegetation,"visibleAbsorptance",m_isIP);
  m_visibleAbsorptance->bind(
    m_isIP,
    *m_roofVegetation,
    OptionalDoubleGetter(std::bind(&model::RoofVegetation::visibleAbsorptance, m_roofVegetation.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RoofVegetation::*)(double)>(&model::RoofVegetation::setVisibleAbsorptance), m_roofVegetation.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::RoofVegetation::resetVisibleAbsorptance, m_roofVegetation.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::RoofVegetation::isVisibleAbsorptanceDefaulted, m_roofVegetation.get_ptr()))
  );

  // m_saturationVolumetricMoistureContentOfTheSoilLayer->bind(roofVegetation,"saturationVolumetricMoistureContentoftheSoilLayer",m_isIP);
  m_saturationVolumetricMoistureContentOfTheSoilLayer->bind(
    m_isIP,
    *m_roofVegetation,
    DoubleGetter(std::bind(&model::RoofVegetation::saturationVolumetricMoistureContentoftheSoilLayer, m_roofVegetation.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RoofVegetation::*)(double)>(&model::RoofVegetation::setSaturationVolumetricMoistureContentoftheSoilLayer), m_roofVegetation.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::RoofVegetation::resetSaturationVolumetricMoistureContentoftheSoilLayer, m_roofVegetation.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::RoofVegetation::isSaturationVolumetricMoistureContentoftheSoilLayerDefaulted, m_roofVegetation.get_ptr()))
  );

  // m_residualVolumetricMoistureContentOfTheSoilLayer->bind(roofVegetation,"residualVolumetricMoistureContentoftheSoilLayer",m_isIP);
  m_residualVolumetricMoistureContentOfTheSoilLayer->bind(
    m_isIP,
    *m_roofVegetation,
    DoubleGetter(std::bind(&model::RoofVegetation::residualVolumetricMoistureContentoftheSoilLayer, m_roofVegetation.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RoofVegetation::*)(double)>(&model::RoofVegetation::setResidualVolumetricMoistureContentoftheSoilLayer), m_roofVegetation.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::RoofVegetation::resetResidualVolumetricMoistureContentoftheSoilLayer, m_roofVegetation.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::RoofVegetation::isResidualVolumetricMoistureContentoftheSoilLayerDefaulted, m_roofVegetation.get_ptr()))
  );

  // m_initialVolumetricMoistureContentOfTheSoilLayer->bind(roofVegetation,"initialVolumetricMoistureContentoftheSoilLayer",m_isIP);
  m_initialVolumetricMoistureContentOfTheSoilLayer->bind(
    m_isIP,
    *m_roofVegetation,
    DoubleGetter(std::bind(&model::RoofVegetation::initialVolumetricMoistureContentoftheSoilLayer, m_roofVegetation.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RoofVegetation::*)(double)>(&model::RoofVegetation::setInitialVolumetricMoistureContentoftheSoilLayer), m_roofVegetation.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::RoofVegetation::resetInitialVolumetricMoistureContentoftheSoilLayer, m_roofVegetation.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::RoofVegetation::isInitialVolumetricMoistureContentoftheSoilLayerDefaulted, m_roofVegetation.get_ptr()))
  );

  // m_solarAbsorptance->bind(roofVegetation,"solarAbsorptance",m_isIP);
  m_solarAbsorptance->bind(
    m_isIP,
    *m_roofVegetation,
    OptionalDoubleGetter(std::bind(&model::RoofVegetation::solarAbsorptance, m_roofVegetation.get_ptr())),
    boost::optional<DoubleSetter>(std::bind(static_cast<bool(model::RoofVegetation::*)(double)>(&model::RoofVegetation::setSolarAbsorptance), m_roofVegetation.get_ptr(), std::placeholders::_1)),
    boost::optional<NoFailAction>(std::bind(&model::RoofVegetation::resetSolarAbsorptance, m_roofVegetation.get_ptr())),
    boost::none,
    boost::none,
    boost::optional<BasicQuery>(std::bind(&model::RoofVegetation::isSolarAbsorptanceDefaulted, m_roofVegetation.get_ptr()))
  );

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

  m_roofVegetation = boost::none;

  m_standardsInformationWidget->detach();
}

void MaterialRoofVegetationInspectorView::refresh()
{
}

} // openstudio
