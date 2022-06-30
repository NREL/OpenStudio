/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

  StandardOpaqueMaterial material(model);

  EXPECT_EQ(0.1, material.thermalConductivity());
  EXPECT_EQ(0.1 / 0.1, material.thermalConductance());
  EXPECT_EQ(1.0 / 0.1, material.thermalResistivity());
  EXPECT_EQ((1.0 / 0.1) * 0.1, material.thermalResistance());
  ASSERT_TRUE(material.thermalReflectance());
  EXPECT_EQ(0.1, material.thermalReflectance().get());
  ASSERT_TRUE(material.solarReflectance());
  EXPECT_EQ(0.3, material.solarReflectance().get());
  ASSERT_TRUE(material.visibleReflectance());
  EXPECT_EQ(0.3, material.visibleReflectance().get());
  EXPECT_EQ("Smooth", material.roughness());
  EXPECT_EQ(0.1, material.thickness());
  EXPECT_EQ(0.1, material.conductivity());
  EXPECT_EQ(0.1, material.density());
  EXPECT_EQ(1400.0, material.specificHeat());
  EXPECT_EQ(0.9, material.thermalAbsorptance());
  EXPECT_TRUE(material.isThermalAbsorptanceDefaulted());
  EXPECT_EQ(0.7, material.solarAbsorptance());
  EXPECT_TRUE(material.isSolarAbsorptanceDefaulted());
  EXPECT_EQ(0.7, material.visibleAbsorptance());
  EXPECT_TRUE(material.isVisibleAbsorptanceDefaulted());
  EXPECT_FALSE(material.heatCapacity());
}

TEST_F(ModelFixture, StandardOpaqueMaterial_Ctor) {
  Model model;

  {
    StandardOpaqueMaterial material(model, "VerySmooth", 1, 2, 3, 900);

    EXPECT_EQ(2, material.thermalConductivity());
    EXPECT_EQ(2 / 1, material.thermalConductance());
    EXPECT_EQ(1.0 / 2, material.thermalResistivity());
    EXPECT_EQ((1.0 / 2) * 1, material.thermalResistance());
    EXPECT_EQ("Smooth", material.roughness());
    EXPECT_EQ(1, material.thickness());
    EXPECT_EQ(2, material.conductivity());
    EXPECT_EQ(3, material.density());
    EXPECT_EQ(900, material.specificHeat());
  }
}

TEST_F(ModelFixture, StandardOpaqueMaterial_SetGetFields) {
  Model model;

  StandardOpaqueMaterial material(model);

  EXPECT_TRUE(material.setThermalConductivity(3));
  EXPECT_EQ(3, material.thermalConductivity());
  EXPECT_EQ(3 / 0.1, material.thermalConductance());
  EXPECT_EQ(1.0 / 3, material.thermalResistivity());
  EXPECT_EQ((1.0 / 3) * 0.1, material.thermalResistance());
  EXPECT_EQ(3, material.conductivity());
  EXPECT_EQ(0.1, material.thickness());

  EXPECT_TRUE(material.setThermalConductance(4));
  EXPECT_EQ(3, material.thermalConductivity());
  EXPECT_EQ(3 / 0.1, material.thermalConductance());
  EXPECT_EQ(1.0 / 3, material.thermalResistivity());
  EXPECT_EQ((1.0 / 3) * (3 / 4), material.thermalResistance());
  EXPECT_EQ(3, material.conductivity());
  EXPECT_EQ(3 / 4, material.thickness());

  EXPECT_TRUE(material.setThermalResistivity(5));
  EXPECT_EQ(1.0 / 5, material.thermalConductivity());
  EXPECT_EQ((1.0 / 5) / (3 / 4), material.thermalConductance());
  EXPECT_EQ(1.0 / (1.0 / 5), material.thermalResistivity());
  EXPECT_EQ((1.0 / (1.0 / 5)) * (3 / 4), material.thermaResistance());
  EXPECT_EQ(1.0 / 5, material.conductivity());
  EXPECT_EQ(3 / 4, material.thickness());

  EXPECT_TRUE(material.setThermalResistance(6));
  EXPECT_EQ(1.0 / 5, material.thermalConductivity());
  EXPECT_EQ((1.0 / 5) / (6 / (1.0 / (1.0 / 5))), material.thermalConductance());
  EXPECT_EQ(1.0 / (1.0 / 5), material.thermalResistivity());
  EXPECT_EQ((1.0 / (1.0 / 5)) * (3 / 4), material.thermaResistance());
  EXPECT_EQ(1.0 / 5, material.conductivity());
  EXPECT_EQ(6 / (1.0 / (1.0 / 5)), material.thickness());

  EXPECT_TRUE(material.setThermalAbsorptance(0.2));
  EXPECT_FALSE(material.isThermalAbsorptanceDefaulted());
  EXPECT_EQ(0.2, material.thermalAbsorptance());
  EXPECT_EQ(0.8, material.thermalReflectance());

  EXPECT_TRUE(material.setThermalReflectance(0.3));
  EXPECT_EQ(0.7, material.thermalAbsorptance());
  EXPECT_EQ(0.3, material.thermalReflectance());

  EXPECT_TRUE(material.setSolarAbsorptance(0.4));
  EXPECT_FALSE(material.isSolarAbsorptanceDefaulted());
  EXPECT_EQ(0.4, material.solarAbsorptance());
  EXPECT_EQ(0.6, material.solarReflectance());

  EXPECT_TRUE(material.setSolarReflectance(0.6));
  EXPECT_EQ(0.4, material.solarAbsorptance());
  EXPECT_EQ(0.6, material.solarReflectance());

  EXPECT_TRUE(material.setVisibleAbsorptance(0.7));
  EXPECT_FALSE(material.isVisibleAbsorptanceDefaulted());
  EXPECT_EQ(0.7, material.visibleAbsorptance());
  EXPECT_EQ(0.3, material.visibleReflectance());

  EXPECT_TRUE(material.setVisibleReflectance(0.8));
  EXPECT_EQ(0.2, material.visibleAbsorptance());
  EXPECT_EQ(0.8, material.visibleReflectance());

  EXPECT_TRUE(material.setRoughness("Rough"));
  EXPECT_EQ("Rough", material.roughness());

  EXPECT_TRUE(material.setThickness(4));
  EXPECT_EQ(4, material.thickness());

  EXPECT_TRUE(material.setConductivity(5));
  EXPECT_EQ(5, material.conductivity());

  EXPECT_TRUE(material.setDensity(6));
  EXPECT_EQ(6, material.density());

  EXPECT_TRUE(material.setSpecificHeat(700));
  EXPECT_EQ(700, material.specificHeat());

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
    EXPECT_EQ(1.5, materialClone.thickness());
  }
}

TEST_F(ModelFixture, StandardOpaqueMaterial_Remove) {
  Model model;

  StandardOpaqueMaterial material(model);
  auto size = model.modelObjects().size();
  EXPECT_FALSE(material.remove().empt());
  EXPECT_EQ(size - 1, model.modelObjects().size());
  EXPECT_EQ(0u, model.getConcreteModelObjects<StandardOpaqueMaterial>().size());
}
