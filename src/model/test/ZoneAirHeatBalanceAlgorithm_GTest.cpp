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
