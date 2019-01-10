/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include <string>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../FoundationKivaSettings.hpp"
#include "../FoundationKivaSettings_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, FoundationKivaSettings_FoundationKivaSettings) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      // create a model to use
      Model model;
      
      // create a foundation kiva settings object to use
      FoundationKivaSettings kivaSettings = model.getUniqueModelObject<FoundationKivaSettings>();
      
      exit(0);
    },
    ::testing::ExitedWithCode(0),
    ""
  );
    
  // create a model to use
  Model model;
  
  // create a foundation kiva settings object to use
  FoundationKivaSettings kivaSettings = model.getUniqueModelObject<FoundationKivaSettings>();
  
  EXPECT_TRUE(kivaSettings.isSoilConductivityDefaulted());
  EXPECT_TRUE(kivaSettings.isSoilDensityDefaulted());
  EXPECT_TRUE(kivaSettings.isSoilSpecificHeatDefaulted());
  EXPECT_TRUE(kivaSettings.isGroundSolarAbsorptivityDefaulted());
  EXPECT_TRUE(kivaSettings.isGroundThermalAbsorptivityDefaulted());
  EXPECT_TRUE(kivaSettings.isGroundSurfaceRoughnessDefaulted());
  EXPECT_TRUE(kivaSettings.isFarFieldWidthDefaulted());
  EXPECT_TRUE(kivaSettings.isDeepGroundBoundaryConditionAutoselected());
  EXPECT_TRUE(kivaSettings.isDeepGroundDepthAutocalculated());
  EXPECT_TRUE(kivaSettings.isMinimumCellDimensionDefaulted());
  EXPECT_TRUE(kivaSettings.isMaximumCellGrowthCoefficientDefaulted());
  EXPECT_TRUE(kivaSettings.isSimulationTimestepDefaulted());

  ASSERT_EQ(1.73, kivaSettings.soilConductivity());
  ASSERT_EQ(1842, kivaSettings.soilDensity());
  ASSERT_EQ(419, kivaSettings.soilSpecificHeat());
  ASSERT_EQ(0.9, kivaSettings.groundSolarAbsorptivity());
  ASSERT_EQ(0.9, kivaSettings.groundThermalAbsorptivity());
  ASSERT_EQ(0.03, kivaSettings.groundSurfaceRoughness());
  ASSERT_EQ(40, kivaSettings.farFieldWidth());
  ASSERT_EQ("Autoselect", kivaSettings.deepGroundBoundaryCondition());
  ASSERT_FALSE(kivaSettings.deepGroundDepth());
  ASSERT_EQ(0.02, kivaSettings.minimumCellDimension());
  ASSERT_EQ(1.5, kivaSettings.maximumCellGrowthCoefficient());
  ASSERT_EQ("Hourly", kivaSettings.simulationTimestep());
}

// test setting and getting
TEST_F(ModelFixture, FoundationKivaSettings_SetGetFields) {    
  // create a model to use
  Model model;
  
  // create a foundation kiva settings object to use
  FoundationKivaSettings kivaSettings = model.getUniqueModelObject<FoundationKivaSettings>();
  
  // set the fields
  kivaSettings.setSoilConductivity(1.75);
  kivaSettings.setSoilDensity(1850);
  kivaSettings.setSoilSpecificHeat(400);
  kivaSettings.setGroundSolarAbsorptivity(0.95);
  kivaSettings.setGroundThermalAbsorptivity(0.95);
  kivaSettings.setGroundSurfaceRoughness(0.05);
  kivaSettings.setFarFieldWidth(50);
  kivaSettings.setDeepGroundBoundaryCondition("GroundWater");
  kivaSettings.setDeepGroundDepth(50);
  kivaSettings.setMinimumCellDimension(0.02);
  kivaSettings.setMaximumCellGrowthCoefficient(1.2);
  kivaSettings.setSimulationTimestep("Timestep");
  
  // check the fields
  ASSERT_FALSE(kivaSettings.isSoilConductivityDefaulted());
  ASSERT_EQ(1.75, kivaSettings.soilConductivity());
  ASSERT_FALSE(kivaSettings.isSoilDensityDefaulted());
  ASSERT_EQ(1850, kivaSettings.soilDensity());
  ASSERT_FALSE(kivaSettings.isSoilSpecificHeatDefaulted());
  ASSERT_EQ(400, kivaSettings.soilSpecificHeat());
  ASSERT_FALSE(kivaSettings.isGroundSolarAbsorptivityDefaulted());
  ASSERT_EQ(0.95, kivaSettings.groundSolarAbsorptivity());
  ASSERT_FALSE(kivaSettings.isGroundThermalAbsorptivityDefaulted());
  ASSERT_EQ(0.95, kivaSettings.groundThermalAbsorptivity());
  ASSERT_FALSE(kivaSettings.isGroundSurfaceRoughnessDefaulted());
  ASSERT_EQ(0.05, kivaSettings.groundSurfaceRoughness());
  ASSERT_FALSE(kivaSettings.isFarFieldWidthDefaulted());
  ASSERT_EQ(50, kivaSettings.farFieldWidth());
  ASSERT_FALSE(kivaSettings.isDeepGroundBoundaryConditionAutoselected());
  ASSERT_EQ("GroundWater", kivaSettings.deepGroundBoundaryCondition());
  ASSERT_FALSE(kivaSettings.isDeepGroundDepthAutocalculated());
  boost::optional<double> optdeepgrounddepth = kivaSettings.deepGroundDepth();
  EXPECT_TRUE(optdeepgrounddepth);
  auto deepgrounddepth = optdeepgrounddepth.get();
  ASSERT_EQ(50, deepgrounddepth);
  ASSERT_FALSE(kivaSettings.isMinimumCellDimensionDefaulted());
  ASSERT_EQ(0.02, kivaSettings.minimumCellDimension());
  ASSERT_FALSE(kivaSettings.isMaximumCellGrowthCoefficientDefaulted());
  ASSERT_EQ(1.2, kivaSettings.maximumCellGrowthCoefficient());
  ASSERT_FALSE(kivaSettings.isSimulationTimestepDefaulted());
  ASSERT_EQ("Timestep", kivaSettings.simulationTimestep());
  
  // reset them one by one
  kivaSettings.resetSoilConductivity();
  kivaSettings.resetSoilDensity();
  kivaSettings.resetSoilSpecificHeat();
  kivaSettings.resetGroundSolarAbsorptivity();
  kivaSettings.resetGroundThermalAbsorptivity();
  kivaSettings.resetGroundSurfaceRoughness();
  kivaSettings.resetFarFieldWidth();
  kivaSettings.resetDeepGroundBoundaryCondition();
  kivaSettings.autocalculateDeepGroundDepth();
  kivaSettings.resetMinimumCellDimension();
  kivaSettings.resetMaximumCellGrowthCoefficient();
  kivaSettings.resetSimulationTimestep();
  
  EXPECT_TRUE(kivaSettings.isSoilConductivityDefaulted());
  EXPECT_TRUE(kivaSettings.isSoilDensityDefaulted());
  EXPECT_TRUE(kivaSettings.isSoilSpecificHeatDefaulted());
  EXPECT_TRUE(kivaSettings.isGroundSolarAbsorptivityDefaulted());
  EXPECT_TRUE(kivaSettings.isGroundThermalAbsorptivityDefaulted());
  EXPECT_TRUE(kivaSettings.isGroundSurfaceRoughnessDefaulted());
  EXPECT_TRUE(kivaSettings.isFarFieldWidthDefaulted());
  EXPECT_TRUE(kivaSettings.isDeepGroundBoundaryConditionAutoselected());
  EXPECT_TRUE(kivaSettings.isDeepGroundDepthAutocalculated());
  EXPECT_TRUE(kivaSettings.isMinimumCellDimensionDefaulted());
  EXPECT_TRUE(kivaSettings.isMaximumCellGrowthCoefficientDefaulted());
  EXPECT_TRUE(kivaSettings.isSimulationTimestepDefaulted());
}

// test cloning it
TEST_F(ModelFixture, FoundationKivaSettings_Clone)
{
  // create a model to use
  Model model;

  // create a foundation kiva settings object to use
  FoundationKivaSettings kivaSettings = model.getUniqueModelObject<FoundationKivaSettings>();

  // change some of the fields
  kivaSettings.setSoilConductivity(1.75);

  // clone it into the same model
  FoundationKivaSettings kivaSettingsClone = kivaSettings.clone(model).cast<FoundationKivaSettings>();
  ASSERT_FALSE(kivaSettingsClone.isSoilConductivityDefaulted());
  ASSERT_EQ(1.75, kivaSettingsClone.soilConductivity());
  ASSERT_TRUE(kivaSettingsClone.isSoilDensityDefaulted());

  // clone it into a different model
  Model model2;
  FoundationKivaSettings kivaSettingsClone2 = kivaSettings.clone(model2).cast<FoundationKivaSettings>();
  ASSERT_FALSE(kivaSettingsClone2.isSoilConductivityDefaulted());
  ASSERT_EQ(1.75, kivaSettingsClone2.soilConductivity());
  ASSERT_TRUE(kivaSettingsClone2.isSoilDensityDefaulted());
}

// check that remove works
TEST_F(ModelFixture, FoundationKivaSettings_Remove) {
  Model model;
  auto size = model.modelObjects().size();
  FoundationKivaSettings kivaSettings = model.getUniqueModelObject<FoundationKivaSettings>();
  EXPECT_FALSE(kivaSettings.remove().empty());
  EXPECT_EQ(size, model.modelObjects().size());
}