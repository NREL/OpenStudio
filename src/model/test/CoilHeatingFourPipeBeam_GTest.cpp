/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../CoilHeatingFourPipeBeam.hpp"
#include "../CoilHeatingFourPipeBeam_Impl.hpp"
#include "../CurveLinear.hpp"
#include "../TableLookup.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilHeatingFourPipeBeam_DefaultConstructors) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CoilHeatingFourPipeBeam coil(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CoilHeatingFourPipeBeam_Getters_Setters) {
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
  TableLookup heatCapModFuncOfSAFlow(m);
  EXPECT_TRUE(coil.setBeamHeatingCapacityAirFlowModificationFactorCurve(heatCapModFuncOfSAFlow));
  EXPECT_EQ(heatCapModFuncOfSAFlow.handle(), coil.beamHeatingCapacityAirFlowModificationFactorCurve().handle());

  // A7: Beam Heating Capacity Hot Water Flow Modification Factor Curve Name (UniVariateCurves // UniVariateTables)
  TableLookup capModFuncOfWaterFlow(m);
  EXPECT_TRUE(coil.setBeamHeatingCapacityHotWaterFlowModificationFactorCurve(capModFuncOfWaterFlow));
  EXPECT_EQ(capModFuncOfWaterFlow.handle(), coil.beamHeatingCapacityHotWaterFlowModificationFactorCurve().handle());
}

TEST_F(ModelFixture, CoilHeatingFourPipeBeam_LoopConnections) {

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

TEST_F(ModelFixture, CoilHeatingFourPipeBeam_Clone) {

  Model m;
  CoilHeatingFourPipeBeam coil(m);

  auto coilClone = coil.clone(m).cast<CoilHeatingFourPipeBeam>();

  // Are curves shared now?
  EXPECT_EQ(coil.beamHeatingCapacityTemperatureDifferenceModificationFactorCurve(),
            coilClone.beamHeatingCapacityTemperatureDifferenceModificationFactorCurve());

  EXPECT_EQ(coil.beamHeatingCapacityAirFlowModificationFactorCurve(), coilClone.beamHeatingCapacityAirFlowModificationFactorCurve());

  EXPECT_EQ(coil.beamHeatingCapacityHotWaterFlowModificationFactorCurve(), coilClone.beamHeatingCapacityHotWaterFlowModificationFactorCurve());
}
