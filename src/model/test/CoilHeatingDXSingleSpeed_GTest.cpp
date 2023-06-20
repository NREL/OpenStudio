/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../PlantLoop.hpp"
#include "../Model.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../CoilHeatingDXSingleSpeed.hpp"
#include "../CoilHeatingDXSingleSpeed_Impl.hpp"
#include "../Schedule.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveQuadratic.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, CoilHeatingDXSingleSpeed_CoilHeatingDXSingleSpeed) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      Schedule s = m.alwaysOnDiscreteSchedule();
      CurveBiquadratic totalHeatingCapacityFunctionofTemperatureCurve(m);
      CurveQuadratic totalHeatingCapacityFunctionofFlowFractionCurve(m);
      CurveBiquadratic energyInputRatioFunctionofTemperatureCurve(m);
      CurveQuadratic energyInputRatioFunctionofFlowFractionCurve(m);
      CurveQuadratic partLoadFractionCorrelationCurve(m);

      CoilHeatingDXSingleSpeed coil(m, s, totalHeatingCapacityFunctionofTemperatureCurve, totalHeatingCapacityFunctionofFlowFractionCurve,
                                    energyInputRatioFunctionofTemperatureCurve, energyInputRatioFunctionofFlowFractionCurve,
                                    partLoadFractionCorrelationCurve);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CoilHeatingDXSingleSpeed_addToNode) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  CurveBiquadratic totalHeatingCapacityFunctionofTemperatureCurve(m);
  CurveQuadratic totalHeatingCapacityFunctionofFlowFractionCurve(m);
  CurveBiquadratic energyInputRatioFunctionofTemperatureCurve(m);
  CurveQuadratic energyInputRatioFunctionofFlowFractionCurve(m);
  CurveQuadratic partLoadFractionCorrelationCurve(m);

  CoilHeatingDXSingleSpeed testObject(m, s, totalHeatingCapacityFunctionofTemperatureCurve, totalHeatingCapacityFunctionofFlowFractionCurve,
                                      energyInputRatioFunctionofTemperatureCurve, energyInputRatioFunctionofFlowFractionCurve,
                                      partLoadFractionCorrelationCurve);

  AirLoopHVAC airLoop(m);
  ControllerOutdoorAir controllerOutdoorAir(m);
  AirLoopHVACOutdoorAirSystem outdoorAirSystem(m, controllerOutdoorAir);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  outdoorAirSystem.addToNode(supplyOutletNode);

  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  CurveBiquadratic c1(m);
  CurveQuadratic c2(m);
  CurveBiquadratic c3(m);
  CurveQuadratic c4(m);
  CurveQuadratic c5(m);

  CoilHeatingDXSingleSpeed testObject2(m, s, c1, c2, c3, c4, c5);

  if (boost::optional<Node> OANode = outdoorAirSystem.outboardOANode()) {
    EXPECT_TRUE(testObject2.addToNode(*OANode));
    EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());
    EXPECT_EQ((unsigned)3, outdoorAirSystem.oaComponents().size());
  }

  CurveBiquadratic c1_2(m);
  CurveQuadratic c2_2(m);
  CurveBiquadratic c3_2(m);
  CurveQuadratic c4_2(m);
  CurveQuadratic c5_2(m);

  CoilHeatingDXSingleSpeed testObject3(m, s, c1_2, c2_2, c3_2, c4_2, c5_2);

  if (boost::optional<Node> reliefNode = outdoorAirSystem.outboardReliefNode()) {
    EXPECT_TRUE(testObject3.addToNode(*reliefNode));
    EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());
    EXPECT_EQ((unsigned)3, outdoorAirSystem.reliefComponents().size());
  }

  auto testObjectClone = testObject.clone(m).cast<CoilHeatingDXSingleSpeed>();
  supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, airLoop.supplyComponents().size());
}

TEST_F(ModelFixture, CoilHeatingDXSingleSpeed_2017and2023) {
  Model model;

  CoilHeatingDXSingleSpeed speed(model);

  EXPECT_EQ(773.3, speed.ratedSupplyFanPowerPerVolumeFlowRate2017());
  EXPECT_EQ(934.4, speed.ratedSupplyFanPowerPerVolumeFlowRate2023());

  EXPECT_TRUE(speed.setRatedSupplyFanPowerPerVolumeFlowRate2017(8.0));
  EXPECT_TRUE(speed.setRatedSupplyFanPowerPerVolumeFlowRate2023(9.0));

  EXPECT_EQ(8.0, speed.ratedSupplyFanPowerPerVolumeFlowRate2017());
  EXPECT_EQ(9.0, speed.ratedSupplyFanPowerPerVolumeFlowRate2023());
}
