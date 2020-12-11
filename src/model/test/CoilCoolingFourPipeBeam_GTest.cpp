/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../CoilCoolingFourPipeBeam.hpp"
#include "../CoilCoolingFourPipeBeam_Impl.hpp"
#include "../CurveLinear.hpp"
#include "../TableMultiVariableLookup.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilCoolingFourPipeBeam_DefaultConstructors) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CoilCoolingFourPipeBeam coil(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CoilCoolingFourPipeBeam_Getters_Setters) {
  Model m;
  CoilCoolingFourPipeBeam coil(m);

  // N1: Beam Rated Cooling Capacity per Beam Length (default 600 W/m)
  coil.setBeamRatedCoolingCapacityperBeamLength(1000.05);
  EXPECT_FALSE(coil.isBeamRatedCoolingCapacityperBeamLengthDefaulted());
  EXPECT_EQ(1000.05, coil.beamRatedCoolingCapacityperBeamLength());
  coil.resetBeamRatedCoolingCapacityperBeamLength();
  EXPECT_TRUE(coil.isBeamRatedCoolingCapacityperBeamLengthDefaulted());

  // N2: Beam Rated Cooling Room Air Chilled Water Temperature Difference (default 10.0C)
  coil.setBeamRatedCoolingRoomAirChilledWaterTemperatureDifference(12.05);
  EXPECT_FALSE(coil.isBeamRatedCoolingRoomAirChilledWaterTemperatureDifferenceDefaulted());
  EXPECT_EQ(12.05, coil.beamRatedCoolingRoomAirChilledWaterTemperatureDifference());
  coil.resetBeamRatedCoolingRoomAirChilledWaterTemperatureDifference();
  EXPECT_TRUE(coil.isBeamRatedCoolingRoomAirChilledWaterTemperatureDifferenceDefaulted());

  // N3: Beam Rated Chilled Water Volume Flow Rate per Beam Length (default 0.00005 m3/s-m)
  coil.setBeamRatedChilledWaterVolumeFlowRateperBeamLength(0.0001);
  EXPECT_FALSE(coil.isBeamRatedChilledWaterVolumeFlowRateperBeamLengthDefaulted());
  EXPECT_EQ(0.0001, coil.beamRatedChilledWaterVolumeFlowRateperBeamLength());
  coil.resetBeamRatedChilledWaterVolumeFlowRateperBeamLength();
  EXPECT_TRUE(coil.isBeamRatedChilledWaterVolumeFlowRateperBeamLengthDefaulted());

  // A5: Beam Cooling Capacity Temperature Difference Modification Factor Curve Name (UniVariateCurves // UniVariateTables)
  CurveLinear capModFuncOfTempDiff(m);
  EXPECT_TRUE(coil.setBeamCoolingCapacityTemperatureDifferenceModificationFactorCurve(capModFuncOfTempDiff));
  EXPECT_EQ(capModFuncOfTempDiff.handle(), coil.beamCoolingCapacityTemperatureDifferenceModificationFactorCurve().handle());

  // A6: Beam Cooling Capacity Air Flow Modification Factor Curve Name (UniVariateCurves // UniVariateTables)
  TableMultiVariableLookup coolCapModFuncOfSAFlow(m, 1);
  EXPECT_TRUE(coil.setBeamCoolingCapacityAirFlowModificationFactorCurve(coolCapModFuncOfSAFlow));
  EXPECT_EQ(coolCapModFuncOfSAFlow.handle(), coil.beamCoolingCapacityAirFlowModificationFactorCurve().handle());

  // A7: Beam Cooling Capacity Chilled Water Flow Modification Factor Curve Name (UniVariateCurves // UniVariateTables)
  TableMultiVariableLookup capModFuncOfWaterFlow(m, 1);
  EXPECT_TRUE(coil.setBeamCoolingCapacityChilledWaterFlowModificationFactorCurve(capModFuncOfWaterFlow));
  EXPECT_EQ(capModFuncOfWaterFlow.handle(), coil.beamCoolingCapacityChilledWaterFlowModificationFactorCurve().handle());
}

TEST_F(ModelFixture, CoilCoolingFourPipeBeam_LoopConnections) {

  Model m;
  CoilCoolingFourPipeBeam coil(m);
  PlantLoop p(m);

  // A3: Chilled Water Inlet Node Name
  // A4: Chilled Water Outlet Node Name

  // Connection should only be allowed on the demand side of the plantLoop
  EXPECT_TRUE(p.addDemandBranchForComponent(coil));
  ASSERT_TRUE(coil.plantLoop());
  EXPECT_EQ(p.handle(), coil.plantLoop()->handle());
  EXPECT_EQ(7u, p.demandComponents().size());
  EXPECT_TRUE(coil.chilledWaterInletNode());
  EXPECT_TRUE(coil.chilledWaterOutletNode());

  // Shouldn't work
  EXPECT_FALSE(p.addSupplyBranchForComponent(coil));

  // Disconnect the coil
  coil.disconnect();
  EXPECT_EQ(0u, p.demandComponents().size());
  EXPECT_FALSE(coil.plantLoop());
  EXPECT_FALSE(coil.chilledWaterInletNode());
  EXPECT_FALSE(coil.chilledWaterOutletNode());
}

TEST_F(ModelFixture, CoilCoolingFourPipeBeam_Clone) {

  Model m;
  CoilCoolingFourPipeBeam coil(m);

  CoilCoolingFourPipeBeam coilClone = coil.clone(m).cast<CoilCoolingFourPipeBeam>();

  // Are curves shared now?
  EXPECT_EQ(coil.beamCoolingCapacityTemperatureDifferenceModificationFactorCurve(),
            coilClone.beamCoolingCapacityTemperatureDifferenceModificationFactorCurve());

  EXPECT_EQ(coil.beamCoolingCapacityAirFlowModificationFactorCurve(), coilClone.beamCoolingCapacityAirFlowModificationFactorCurve());

  EXPECT_EQ(coil.beamCoolingCapacityChilledWaterFlowModificationFactorCurve(),
            coilClone.beamCoolingCapacityChilledWaterFlowModificationFactorCurve());
}
