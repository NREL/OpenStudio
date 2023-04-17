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

#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../MaterialPropertyPhaseChangeHysteresis.hpp"
#include "../MaterialPropertyPhaseChangeHysteresis_Impl.hpp"

#include "../StandardOpaqueMaterial.hpp"
#include "../StandardOpaqueMaterial_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, MaterialPropertyPhaseChangeHysteresis_MaterialPropertyPhaseChangeHysteresis) {
  // create a model to use
  Model model;

  // create a material object to use
  StandardOpaqueMaterial material(model);
  EXPECT_EQ(1, model.modelObjects().size());

  // new material does not have material property moisture penetration depth settings yet
  EXPECT_FALSE(material.materialPropertyPhaseChangeHysteresis());

  // create a material property phase change hysteresis object to use
  boost::optional<MaterialPropertyPhaseChangeHysteresis> optphaseChangeHysteresis = material.createMaterialPropertyPhaseChangeHysteresis();
  ASSERT_TRUE(optphaseChangeHysteresis);
  EXPECT_EQ(2, model.modelObjects().size());

  // now the material has material property phase change
  EXPECT_TRUE(material.materialPropertyPhaseChangeHysteresis());

  // check to make sure the liquid state density is as expected
  auto phaseChangeHysteresis = optphaseChangeHysteresis.get();
  EXPECT_EQ(material.nameString(), phaseChangeHysteresis.materialName());
  EXPECT_EQ(material.handle(), phaseChangeHysteresis.material().handle());
  EXPECT_EQ(2200.0, phaseChangeHysteresis.liquidStateDensity());

  // check that creating the material property phase change hysteresis when they already exists does nothing and returns nil
  boost::optional<MaterialPropertyPhaseChangeHysteresis> optphaseChangeHysteresis2 = material.createMaterialPropertyPhaseChangeHysteresis();
  ASSERT_FALSE(optphaseChangeHysteresis2);
  EXPECT_EQ(2, model.modelObjects().size());
}

// test setting and getting
TEST_F(ModelFixture, MaterialPropertyPhaseChangeHysteresis_SetGetFields) {
  Model model;
  StandardOpaqueMaterial material(model);
  boost::optional<MaterialPropertyPhaseChangeHysteresis> optphaseChangeHysteresis = material.createMaterialPropertyPhaseChangeHysteresis();
  auto phaseChangeHysteresis = optphaseChangeHysteresis.get();

  // check that the properties were set properly
  EXPECT_EQ(10000.0, phaseChangeHysteresis.latentHeatduringtheEntirePhaseChangeProcess());
  EXPECT_EQ(1.5, phaseChangeHysteresis.liquidStateThermalConductivity());
  EXPECT_EQ(2200.0, phaseChangeHysteresis.liquidStateDensity());
  EXPECT_EQ(2000.0, phaseChangeHysteresis.liquidStateSpecificHeat());
  EXPECT_EQ(1.0, phaseChangeHysteresis.highTemperatureDifferenceofMeltingCurve());
  EXPECT_EQ(23.0, phaseChangeHysteresis.peakMeltingTemperature());
  EXPECT_EQ(1.0, phaseChangeHysteresis.lowTemperatureDifferenceofMeltingCurve());
  EXPECT_EQ(1.8, phaseChangeHysteresis.solidStateThermalConductivity());
  EXPECT_EQ(2300.0, phaseChangeHysteresis.solidStateDensity());
  EXPECT_EQ(2000.0, phaseChangeHysteresis.solidStateSpecificHeat());
  EXPECT_EQ(1.0, phaseChangeHysteresis.highTemperatureDifferenceofFreezingCurve());
  EXPECT_EQ(20.0, phaseChangeHysteresis.peakFreezingTemperature());
  EXPECT_EQ(1.0, phaseChangeHysteresis.lowTemperatureDifferenceofFreezingCurve());

  // now override the defaults with explicit values
  phaseChangeHysteresis.setLatentHeatduringtheEntirePhaseChangeProcess(10500.0);
  phaseChangeHysteresis.setLiquidStateThermalConductivity(1.6);
  phaseChangeHysteresis.setLiquidStateDensity(1900.0);
  phaseChangeHysteresis.setLiquidStateSpecificHeat(1800.0);
  phaseChangeHysteresis.setHighTemperatureDifferenceofMeltingCurve(2.0);
  phaseChangeHysteresis.setPeakMeltingTemperature(24.0);
  phaseChangeHysteresis.setLowTemperatureDifferenceofMeltingCurve(3.0);
  phaseChangeHysteresis.setSolidStateThermalConductivity(1.9);
  phaseChangeHysteresis.setSolidStateDensity(2500.0);
  phaseChangeHysteresis.setSolidStateSpecificHeat(1750.0);
  phaseChangeHysteresis.setHighTemperatureDifferenceofFreezingCurve(1.3);
  phaseChangeHysteresis.setPeakFreezingTemperature(22.0);
  phaseChangeHysteresis.setLowTemperatureDifferenceofFreezingCurve(2.0);

  EXPECT_EQ(10500.0, phaseChangeHysteresis.latentHeatduringtheEntirePhaseChangeProcess());
  EXPECT_EQ(1.6, phaseChangeHysteresis.liquidStateThermalConductivity());
  EXPECT_EQ(1900.0, phaseChangeHysteresis.liquidStateDensity());
  EXPECT_EQ(1800.0, phaseChangeHysteresis.liquidStateSpecificHeat());
  EXPECT_EQ(2.0, phaseChangeHysteresis.highTemperatureDifferenceofMeltingCurve());
  EXPECT_EQ(24.0, phaseChangeHysteresis.peakMeltingTemperature());
  EXPECT_EQ(3.0, phaseChangeHysteresis.lowTemperatureDifferenceofMeltingCurve());
  EXPECT_EQ(1.9, phaseChangeHysteresis.solidStateThermalConductivity());
  EXPECT_EQ(2500.0, phaseChangeHysteresis.solidStateDensity());
  EXPECT_EQ(1750.0, phaseChangeHysteresis.solidStateSpecificHeat());
  EXPECT_EQ(1.3, phaseChangeHysteresis.highTemperatureDifferenceofFreezingCurve());
  EXPECT_EQ(22.0, phaseChangeHysteresis.peakFreezingTemperature());
  EXPECT_EQ(2.0, phaseChangeHysteresis.lowTemperatureDifferenceofFreezingCurve());
}

TEST_F(ModelFixture, MaterialPropertyPhaseChangeHysteresis_CtorThrow) {
  Model model;
  StandardOpaqueMaterial material(model);

  EXPECT_NO_THROW(MaterialPropertyPhaseChangeHysteresis phaseChangeHysteresis(material));
  EXPECT_TRUE(material.materialPropertyPhaseChangeHysteresis());
  EXPECT_THROW(MaterialPropertyPhaseChangeHysteresis phaseChangeHysteresis(material), openstudio::Exception);
  EXPECT_TRUE(material.materialPropertyPhaseChangeHysteresis());
}
