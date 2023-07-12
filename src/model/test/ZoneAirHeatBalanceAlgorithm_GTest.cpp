/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ModelFixture.hpp"

#include "../ZoneAirHeatBalanceAlgorithm.hpp"
#include "../ZoneAirHeatBalanceAlgorithm_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneAirHeatBalanceAlgorithm_GettersSetters) {
  Model m;
  auto zoneAirHeatBalanceAlgorithm = m.getUniqueModelObject<ZoneAirHeatBalanceAlgorithm>();

  // Algorithm: Optional String
  // Default value from IDD
  EXPECT_TRUE(zoneAirHeatBalanceAlgorithm.isAlgorithmDefaulted());
  EXPECT_EQ("ThirdOrderBackwardDifference", zoneAirHeatBalanceAlgorithm.algorithm());
  // Set
  EXPECT_TRUE(zoneAirHeatBalanceAlgorithm.setAlgorithm("AnalyticalSolution"));
  EXPECT_EQ("AnalyticalSolution", zoneAirHeatBalanceAlgorithm.algorithm());
  EXPECT_FALSE(zoneAirHeatBalanceAlgorithm.isAlgorithmDefaulted());
  // Bad Value
  EXPECT_FALSE(zoneAirHeatBalanceAlgorithm.setAlgorithm("BADENUM"));
  EXPECT_EQ("AnalyticalSolution", zoneAirHeatBalanceAlgorithm.algorithm());
  // Reset
  zoneAirHeatBalanceAlgorithm.resetAlgorithm();
  EXPECT_TRUE(zoneAirHeatBalanceAlgorithm.isAlgorithmDefaulted());
  EXPECT_EQ("ThirdOrderBackwardDifference", zoneAirHeatBalanceAlgorithm.algorithm());

  // Do Space Heat Balance for Sizing: Optional Boolean
  // Default value from IDD
  EXPECT_TRUE(zoneAirHeatBalanceAlgorithm.isDoSpaceHeatBalanceforSizingDefaulted());
  EXPECT_FALSE(zoneAirHeatBalanceAlgorithm.doSpaceHeatBalanceforSizing());
  // Set
  EXPECT_TRUE(zoneAirHeatBalanceAlgorithm.setDoSpaceHeatBalanceforSizing(true));
  EXPECT_TRUE(zoneAirHeatBalanceAlgorithm.doSpaceHeatBalanceforSizing());
  EXPECT_FALSE(zoneAirHeatBalanceAlgorithm.isDoSpaceHeatBalanceforSizingDefaulted());

  EXPECT_TRUE(zoneAirHeatBalanceAlgorithm.setDoSpaceHeatBalanceforSizing(false));
  EXPECT_FALSE(zoneAirHeatBalanceAlgorithm.doSpaceHeatBalanceforSizing());
  EXPECT_FALSE(zoneAirHeatBalanceAlgorithm.isDoSpaceHeatBalanceforSizingDefaulted());

  EXPECT_TRUE(zoneAirHeatBalanceAlgorithm.setDoSpaceHeatBalanceforSizing(true));
  EXPECT_TRUE(zoneAirHeatBalanceAlgorithm.doSpaceHeatBalanceforSizing());
  EXPECT_FALSE(zoneAirHeatBalanceAlgorithm.isDoSpaceHeatBalanceforSizingDefaulted());
  // Reset
  zoneAirHeatBalanceAlgorithm.resetDoSpaceHeatBalanceforSizing();
  EXPECT_TRUE(zoneAirHeatBalanceAlgorithm.isDoSpaceHeatBalanceforSizingDefaulted());
  EXPECT_FALSE(zoneAirHeatBalanceAlgorithm.doSpaceHeatBalanceforSizing());

  // Do Space Heat Balance for Simulation: Optional Boolean
  // Default value from IDD
  EXPECT_TRUE(zoneAirHeatBalanceAlgorithm.isDoSpaceHeatBalanceforSimulationDefaulted());
  EXPECT_FALSE(zoneAirHeatBalanceAlgorithm.doSpaceHeatBalanceforSimulation());
  // Set
  EXPECT_TRUE(zoneAirHeatBalanceAlgorithm.setDoSpaceHeatBalanceforSimulation(true));
  EXPECT_TRUE(zoneAirHeatBalanceAlgorithm.doSpaceHeatBalanceforSimulation());
  EXPECT_FALSE(zoneAirHeatBalanceAlgorithm.isDoSpaceHeatBalanceforSimulationDefaulted());

  EXPECT_TRUE(zoneAirHeatBalanceAlgorithm.setDoSpaceHeatBalanceforSimulation(false));
  EXPECT_FALSE(zoneAirHeatBalanceAlgorithm.doSpaceHeatBalanceforSimulation());
  EXPECT_FALSE(zoneAirHeatBalanceAlgorithm.isDoSpaceHeatBalanceforSimulationDefaulted());

  EXPECT_TRUE(zoneAirHeatBalanceAlgorithm.setDoSpaceHeatBalanceforSimulation(true));
  EXPECT_TRUE(zoneAirHeatBalanceAlgorithm.doSpaceHeatBalanceforSimulation());
  EXPECT_FALSE(zoneAirHeatBalanceAlgorithm.isDoSpaceHeatBalanceforSimulationDefaulted());
  // Reset
  zoneAirHeatBalanceAlgorithm.resetDoSpaceHeatBalanceforSimulation();
  EXPECT_TRUE(zoneAirHeatBalanceAlgorithm.isDoSpaceHeatBalanceforSimulationDefaulted());
  EXPECT_FALSE(zoneAirHeatBalanceAlgorithm.doSpaceHeatBalanceforSimulation());
}
