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

#include "ModelFixture.hpp"

#include "../CoilHeatingFourPipeBeam.hpp"
#include "../CoilHeatingFourPipeBeam_Impl.hpp"
#include "../CurveLinear.hpp"
#include "../TableMultiVariableLookup.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilHeatingFourPipeBeam_DefaultConstructors)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    Model m;
    CoilHeatingFourPipeBeam coil(m);

    exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, CoilHeatingFourPipeBeam_Getters_Setters)
{
  Model m;
  CoilHeatingFourPipeBeam coil(m);

  // N1: Beam Rated Heating Capacity per Beam Length (default 1500 W/m)
  coil.setBeamRatedHeatingCapacityperBeamLength(2000.05);
  EXPECT_FALSE(coil.isBeamRatedHeatingCapacityperBeamLengthDefaulted());
  EXPECT_EQ(2000.05, coil.beamRatedHeatingCapacityperBeamLength());
  coil.resetBeamRatedHeatingCapacityperBeamLength();
  EXPECT_TRUE(coil.isBeamRatedHeatingCapacityperBeamLengthDefaulted());

  // N2: Beam Rated Heating Room Air Hot Water Temperature Difference (default 27.8C)
  coil.setBeamRatedHeatingRoomAirHotWaterTemperatureDifference(30.05);
  EXPECT_FALSE(coil.isBeamRatedHeatingRoomAirHotWaterTemperatureDifferenceDefaulted());
  EXPECT_EQ(30.05, coil.beamRatedHeatingRoomAirHotWaterTemperatureDifference());
  coil.resetBeamRatedHeatingRoomAirHotWaterTemperatureDifference();
  EXPECT_TRUE(coil.isBeamRatedHeatingRoomAirHotWaterTemperatureDifferenceDefaulted());

  // N3: Beam Rated Hot Water Volume Flow Rate per Beam Length (default 0.00005 m3/s-m)
  coil.setBeamRatedHotWaterVolumeFlowRateperBeamLength(0.0001);
  EXPECT_FALSE(coil.isBeamRatedHotWaterVolumeFlowRateperBeamLengthDefaulted());
  EXPECT_EQ(0.0001, coil.beamRatedHotWaterVolumeFlowRateperBeamLength());
  coil.resetBeamRatedHotWaterVolumeFlowRateperBeamLength();
  EXPECT_TRUE(coil.isBeamRatedHotWaterVolumeFlowRateperBeamLengthDefaulted());

  // A5: Beam Heating Capacity Temperature Difference Modification Factor Curve Name (UniVariateCurves // UniVariateTables)
  CurveLinear capModFuncOfTempDiff(m);
  EXPECT_TRUE(coil.setBeamHeatingCapacityTemperatureDifferenceModificationFactorCurve(capModFuncOfTempDiff));
  EXPECT_EQ(capModFuncOfTempDiff.handle(), coil.beamHeatingCapacityTemperatureDifferenceModificationFactorCurve().handle());

  // A6: Beam Heating Capacity Air Flow Modification Factor Curve Name (UniVariateCurves // UniVariateTables)
  TableMultiVariableLookup heatCapModFuncOfSAFlow(m, 1);
  EXPECT_TRUE(coil.setBeamHeatingCapacityAirFlowModificationFactorCurve(heatCapModFuncOfSAFlow));
  EXPECT_EQ(heatCapModFuncOfSAFlow.handle(), coil.beamHeatingCapacityAirFlowModificationFactorCurve().handle());

  // A7: Beam Heating Capacity Hot Water Flow Modification Factor Curve Name (UniVariateCurves // UniVariateTables)
  TableMultiVariableLookup capModFuncOfWaterFlow(m, 1);
  EXPECT_TRUE(coil.setBeamHeatingCapacityHotWaterFlowModificationFactorCurve(capModFuncOfWaterFlow));
  EXPECT_EQ(capModFuncOfWaterFlow.handle(), coil.beamHeatingCapacityHotWaterFlowModificationFactorCurve().handle());

}


TEST_F(ModelFixture, CoilHeatingFourPipeBeam_LoopConnections)
{

  Model m;
  CoilHeatingFourPipeBeam coil(m);
  PlantLoop p(m);

  // A3: Hot Water Inlet Node Name
  // A4: Hot Water Outlet Node Name

  // Connection should only be allowed on the demand side of the plantLoop
  EXPECT_TRUE(p.addDemandBranchForComponent(coil));
  ASSERT_TRUE(coil.plantLoop());
  EXPECT_EQ(p.handle(), coil.plantLoop()->handle());
  EXPECT_EQ(7u, p.demandComponents().size());
  EXPECT_TRUE(coil.hotWaterInletNode());
  EXPECT_TRUE(coil.hotWaterOutletNode());

  // Shouldn't work
  EXPECT_FALSE(p.addSupplyBranchForComponent(coil));

  // Disconnect the coil
  coil.disconnect();
  EXPECT_EQ(0u, p.demandComponents().size());
  EXPECT_FALSE(coil.plantLoop());
  EXPECT_FALSE(coil.hotWaterInletNode());
  EXPECT_FALSE(coil.hotWaterOutletNode());

}

TEST_F(ModelFixture, CoilHeatingFourPipeBeam_Clone)
{

  Model m;
  CoilHeatingFourPipeBeam coil(m);

  CoilHeatingFourPipeBeam coilClone = coil.clone(m).cast<CoilHeatingFourPipeBeam>();


  // Are curves shared now?
  EXPECT_EQ(     coil.beamHeatingCapacityTemperatureDifferenceModificationFactorCurve(),
            coilClone.beamHeatingCapacityTemperatureDifferenceModificationFactorCurve() );

  EXPECT_EQ(     coil.beamHeatingCapacityAirFlowModificationFactorCurve(),
            coilClone.beamHeatingCapacityAirFlowModificationFactorCurve() );

  EXPECT_EQ(     coil.beamHeatingCapacityHotWaterFlowModificationFactorCurve(),
            coilClone.beamHeatingCapacityHotWaterFlowModificationFactorCurve() );

}
