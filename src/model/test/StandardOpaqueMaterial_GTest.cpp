/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../StandardOpaqueMaterial.hpp"
#include "../StandardOpaqueMaterial_Impl.hpp"
#include "../StandardsInformationMaterial.hpp"
#include "../StandardsInformationMaterial_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, StandardOpaqueMaterial_StandardOpaqueMaterial) {
  Model model;

  std::string roughness = "Smooth";
  double thickness = 0.1;
  double conductivity = 0.1;
  double density = 0.1;
  double specificHeat = 1400.0;

  double conductance = conductivity / thickness;
  double resistivity = 1.0 / conductivity;
  double resistance = resistivity * thickness;

  double thermalAbsorptance = 0.9;
  double solarAbsorptance = 0.7;
  double visibleAbsorptance = 0.7;

  double thermalReflectance = 1.0 - thermalAbsorptance;
  double solarReflectance = 1.0 - solarAbsorptance;
  double visibleReflectance = 1.0 - visibleAbsorptance;

  StandardOpaqueMaterial material(model);

  EXPECT_EQ(conductivity, material.thermalConductivity());
  EXPECT_EQ(conductance, material.thermalConductance());
  EXPECT_EQ(resistivity, material.thermalResistivity());
  EXPECT_EQ(resistance, material.thermalResistance());
  ASSERT_TRUE(material.thermalReflectance());
  EXPECT_EQ(thermalReflectance, material.thermalReflectance().get());
  ASSERT_TRUE(material.solarReflectance());
  EXPECT_EQ(solarReflectance, material.solarReflectance().get());
  ASSERT_TRUE(material.visibleReflectance());
  EXPECT_EQ(visibleReflectance, material.visibleReflectance().get());
  EXPECT_EQ(roughness, material.roughness());
  EXPECT_EQ(thickness, material.thickness());
  EXPECT_EQ(conductivity, material.conductivity());
  EXPECT_EQ(density, material.density());
  EXPECT_EQ(specificHeat, material.specificHeat());
  EXPECT_EQ(thermalAbsorptance, material.thermalAbsorptance());
  EXPECT_TRUE(material.isThermalAbsorptanceDefaulted());
  EXPECT_EQ(solarAbsorptance, material.solarAbsorptance());
  EXPECT_TRUE(material.isSolarAbsorptanceDefaulted());
  EXPECT_EQ(visibleAbsorptance, material.visibleAbsorptance());
  EXPECT_TRUE(material.isVisibleAbsorptanceDefaulted());
  EXPECT_FALSE(material.heatCapacity());
}

TEST_F(ModelFixture, StandardOpaqueMaterial_Ctor) {
  Model model;

  std::string roughness = "VerySmooth";
  double thickness = 1.0;
  double conductivity = 2.0;
  double density = 3.0;
  double specificHeat = 900.0;

  double conductance = conductivity / thickness;
  double resistivity = 1.0 / conductivity;
  double resistance = resistivity * thickness;

  StandardOpaqueMaterial material(model, roughness, thickness, conductivity, density, specificHeat);

  EXPECT_EQ(conductivity, material.thermalConductivity());
  EXPECT_EQ(conductance, material.thermalConductance());
  EXPECT_EQ(resistivity, material.thermalResistivity());
  EXPECT_EQ(resistance, material.thermalResistance());
  EXPECT_EQ(roughness, material.roughness());
  EXPECT_EQ(thickness, material.thickness());
  EXPECT_EQ(conductivity, material.conductivity());
  EXPECT_EQ(density, material.density());
  EXPECT_EQ(specificHeat, material.specificHeat());
}

TEST_F(ModelFixture, StandardOpaqueMaterial_SetGetFields) {
  Model model;

  std::string roughness = "Smooth";
  double thickness = 0.1;
  double conductivity = 0.1;
  double density = 0.1;
  double specificHeat = 1400.0;

  double conductance = conductivity / thickness;
  double resistivity = 1.0 / conductivity;
  double resistance = resistivity * thickness;

  double thermalAbsorptance = 0.9;
  double solarAbsorptance = 0.7;
  double visibleAbsorptance = 0.7;

  double thermalReflectance = 1.0 - thermalAbsorptance;
  double solarReflectance = 1.0 - solarAbsorptance;
  double visibleReflectance = 1.0 - visibleAbsorptance;

  StandardOpaqueMaterial material(model);

  conductivity = 3.0;
  conductance = conductivity / thickness;
  resistivity = 1.0 / conductivity;
  resistance = resistivity * thickness;

  EXPECT_TRUE(material.setThermalConductivity(conductivity));
  EXPECT_EQ(conductivity, material.thermalConductivity());
  EXPECT_EQ(conductance, material.thermalConductance());
  EXPECT_EQ(resistivity, material.thermalResistivity());
  EXPECT_EQ(resistance, material.thermalResistance());
  EXPECT_EQ(conductivity, material.conductivity());
  EXPECT_EQ(thickness, material.thickness());

  conductance = 4.0;
  thickness = conductivity / conductance;
  conductance = conductivity / thickness;
  resistivity = 1.0 / conductivity;
  resistance = resistivity * thickness;

  EXPECT_TRUE(material.setThermalConductance(conductance));
  EXPECT_EQ(conductivity, material.thermalConductivity());
  EXPECT_EQ(conductance, material.thermalConductance());
  EXPECT_EQ(resistivity, material.thermalResistivity());
  EXPECT_EQ(resistance, material.thermalResistance());
  EXPECT_EQ(conductivity, material.conductivity());
  EXPECT_EQ(thickness, material.thickness());

  resistivity = 5.0;
  conductivity = 1.0 / resistivity;
  conductance = conductivity / thickness;
  resistivity = 1.0 / conductivity;
  resistance = resistivity * thickness;

  EXPECT_TRUE(material.setThermalResistivity(resistivity));
  EXPECT_EQ(conductivity, material.thermalConductivity());
  EXPECT_EQ(conductance, material.thermalConductance());
  EXPECT_EQ(resistivity, material.thermalResistivity());
  EXPECT_EQ(resistance, material.thermalResistance());
  EXPECT_EQ(conductivity, material.conductivity());
  EXPECT_EQ(thickness, material.thickness());

  resistance = 6.0;
  thickness = resistance / resistivity;
  conductance = conductivity / thickness;
  resistivity = 1.0 / conductivity;
  resistance = resistivity * thickness;

  EXPECT_TRUE(material.setThermalResistance(resistance));
  EXPECT_EQ(conductivity, material.thermalConductivity());
  EXPECT_EQ(conductance, material.thermalConductance());
  EXPECT_EQ(resistivity, material.thermalResistivity());
  EXPECT_EQ(resistance, material.thermalResistance());
  EXPECT_EQ(conductivity, material.conductivity());
  EXPECT_EQ(thickness, material.thickness());

  thermalAbsorptance = 0.2;
  thermalReflectance = 1.0 - thermalAbsorptance;

  EXPECT_TRUE(material.setThermalAbsorptance(thermalAbsorptance));
  EXPECT_FALSE(material.isThermalAbsorptanceDefaulted());
  EXPECT_EQ(thermalAbsorptance, material.thermalAbsorptance());
  ASSERT_TRUE(material.thermalReflectance());
  EXPECT_EQ(thermalReflectance, material.thermalReflectance().get());

  thermalReflectance = 0.25;
  thermalAbsorptance = 1.0 - thermalReflectance;

  EXPECT_TRUE(material.setThermalReflectance(thermalReflectance));
  EXPECT_EQ(thermalAbsorptance, material.thermalAbsorptance());
  ASSERT_TRUE(material.thermalReflectance());
  EXPECT_EQ(thermalReflectance, material.thermalReflectance().get());

  solarAbsorptance = 0.4;
  solarReflectance = 1.0 - solarAbsorptance;

  EXPECT_TRUE(material.setSolarAbsorptance(solarAbsorptance));
  EXPECT_FALSE(material.isSolarAbsorptanceDefaulted());
  EXPECT_EQ(solarAbsorptance, material.solarAbsorptance());
  ASSERT_TRUE(material.solarReflectance());
  EXPECT_EQ(solarReflectance, material.solarReflectance().get());

  solarReflectance = 0.6;
  solarAbsorptance = 1.0 - solarReflectance;

  EXPECT_TRUE(material.setSolarReflectance(solarReflectance));
  EXPECT_EQ(solarAbsorptance, material.solarAbsorptance());
  ASSERT_TRUE(material.solarReflectance());
  EXPECT_EQ(solarReflectance, material.solarReflectance().get());

  visibleAbsorptance = 0.7;
  visibleReflectance = 1.0 - visibleAbsorptance;

  EXPECT_TRUE(material.setVisibleAbsorptance(visibleAbsorptance));
  EXPECT_FALSE(material.isVisibleAbsorptanceDefaulted());
  EXPECT_EQ(visibleAbsorptance, material.visibleAbsorptance());
  ASSERT_TRUE(material.visibleReflectance());
  EXPECT_EQ(visibleReflectance, material.visibleReflectance().get());

  visibleReflectance = 0.75;
  visibleAbsorptance = 1.0 - visibleReflectance;

  EXPECT_TRUE(material.setVisibleReflectance(visibleReflectance));
  EXPECT_EQ(visibleAbsorptance, material.visibleAbsorptance());
  ASSERT_TRUE(material.visibleReflectance());
  EXPECT_EQ(visibleReflectance, material.visibleReflectance().get());

  roughness = "Rough";

  EXPECT_TRUE(material.setRoughness(roughness));
  EXPECT_EQ(roughness, material.roughness());

  thickness = 4.0;

  EXPECT_TRUE(material.setThickness(thickness));
  EXPECT_EQ(thickness, material.thickness());

  conductivity = 5.0;

  EXPECT_TRUE(material.setConductivity(conductivity));
  EXPECT_EQ(conductivity, material.conductivity());

  density = 6.0;

  EXPECT_TRUE(material.setDensity(density));
  EXPECT_EQ(density, material.density());

  specificHeat = 700.0;

  EXPECT_TRUE(material.setSpecificHeat(specificHeat));
  EXPECT_EQ(specificHeat, material.specificHeat());

  material.resetThermalAbsorptance();
  EXPECT_TRUE(material.isThermalAbsorptanceDefaulted());

  material.resetSolarAbsorptance();
  EXPECT_TRUE(material.isSolarAbsorptanceDefaulted());

  material.resetVisibleAbsorptance();
  EXPECT_TRUE(material.isVisibleAbsorptanceDefaulted());
}

TEST_F(ModelFixture, StandardOpaqueMaterial_Clone) {
  Model model;

  StandardOpaqueMaterial material(model);
  material.setThickness(1.5);
  EXPECT_EQ(1u, model.getConcreteModelObjects<StandardOpaqueMaterial>().size());

  {
    auto materialClone = material.clone(model).cast<StandardOpaqueMaterial>();
    EXPECT_EQ(2u, model.getConcreteModelObjects<StandardOpaqueMaterial>().size());
    EXPECT_EQ(1.5, materialClone.thickness());
  }

  {
    Model model2;
    auto materialClone2 = material.clone(model2).cast<StandardOpaqueMaterial>();
    EXPECT_EQ(1u, model2.getConcreteModelObjects<StandardOpaqueMaterial>().size());
    EXPECT_EQ(1.5, materialClone2.thickness());
  }
}

TEST_F(ModelFixture, StandardOpaqueMaterial_Remove) {
  Model model;

  StandardOpaqueMaterial material(model);
  auto size = model.modelObjects().size();
  EXPECT_FALSE(material.remove().empty());
  EXPECT_EQ(size - 1, model.modelObjects().size());
  EXPECT_EQ(0u, model.getConcreteModelObjects<StandardOpaqueMaterial>().size());
}
