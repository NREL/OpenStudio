/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "../AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"
#include "../Schedule.hpp"
#include "../FanConstantVolume.hpp"
#include "../CoilHeatingDXSingleSpeed.hpp"
#include "../CoilCoolingDXSingleSpeed.hpp"
#include "../CoilHeatingDXVariableSpeed.hpp"
#include "../CoilCoolingDXVariableSpeed.hpp"
#include "../CoilWaterHeatingAirToWaterHeatPumpVariableSpeed.hpp"
#include "../CoilSystemIntegratedHeatPumpAirSource.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveQuadratic.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, AirLoopHVACUnitaryHeatPumpAirToAir_AirLoopHVACUnitaryHeatPumpAirToAir) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      Schedule s = m.alwaysOnDiscreteSchedule();
      FanConstantVolume supplyFan(m, s);

      CurveBiquadratic ctotalHeatingCapacityFunctionofTemperatureCurve(m);
      CurveQuadratic ctotalHeatingCapacityFunctionofFlowFractionCurve(m);
      CurveBiquadratic cenergyInputRatioFunctionofTemperatureCurve(m);
      CurveQuadratic cenergyInputRatioFunctionofFlowFractionCurve(m);
      CurveQuadratic cpartLoadFractionCorrelationCurve(m);

      CoilCoolingDXSingleSpeed coolingCoil(m, s, ctotalHeatingCapacityFunctionofTemperatureCurve, ctotalHeatingCapacityFunctionofFlowFractionCurve,
                                           cenergyInputRatioFunctionofTemperatureCurve, cenergyInputRatioFunctionofFlowFractionCurve,
                                           cpartLoadFractionCorrelationCurve);

      CurveBiquadratic totalHeatingCapacityFunctionofTemperatureCurve(m);
      CurveQuadratic totalHeatingCapacityFunctionofFlowFractionCurve(m);
      CurveBiquadratic energyInputRatioFunctionofTemperatureCurve(m);
      CurveQuadratic energyInputRatioFunctionofFlowFractionCurve(m);
      CurveQuadratic partLoadFractionCorrelationCurve(m);

      CoilHeatingDXSingleSpeed heatingCoil(m, s, totalHeatingCapacityFunctionofTemperatureCurve, totalHeatingCapacityFunctionofFlowFractionCurve,
                                           energyInputRatioFunctionofTemperatureCurve, energyInputRatioFunctionofFlowFractionCurve,
                                           partLoadFractionCorrelationCurve);

      CoilHeatingElectric coilHeatingElectric(m, s);

      AirLoopHVACUnitaryHeatPumpAirToAir coil(m, s, supplyFan, heatingCoil, coolingCoil, coilHeatingElectric);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, AirLoopHVACUnitaryHeatPumpAirToAir_addToNode) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanConstantVolume supplyFan(m, s);

  CurveBiquadratic ctotalHeatingCapacityFunctionofTemperatureCurve(m);
  CurveQuadratic ctotalHeatingCapacityFunctionofFlowFractionCurve(m);
  CurveBiquadratic cenergyInputRatioFunctionofTemperatureCurve(m);
  CurveQuadratic cenergyInputRatioFunctionofFlowFractionCurve(m);
  CurveQuadratic cpartLoadFractionCorrelationCurve(m);

  CoilCoolingDXSingleSpeed coolingCoil(m, s, ctotalHeatingCapacityFunctionofTemperatureCurve, ctotalHeatingCapacityFunctionofFlowFractionCurve,
                                       cenergyInputRatioFunctionofTemperatureCurve, cenergyInputRatioFunctionofFlowFractionCurve,
                                       cpartLoadFractionCorrelationCurve);

  CurveBiquadratic totalHeatingCapacityFunctionofTemperatureCurve(m);
  CurveQuadratic totalHeatingCapacityFunctionofFlowFractionCurve(m);
  CurveBiquadratic energyInputRatioFunctionofTemperatureCurve(m);
  CurveQuadratic energyInputRatioFunctionofFlowFractionCurve(m);
  CurveQuadratic partLoadFractionCorrelationCurve(m);

  CoilHeatingDXSingleSpeed heatingCoil(m, s, totalHeatingCapacityFunctionofTemperatureCurve, totalHeatingCapacityFunctionofFlowFractionCurve,
                                       energyInputRatioFunctionofTemperatureCurve, energyInputRatioFunctionofFlowFractionCurve,
                                       partLoadFractionCorrelationCurve);

  CoilHeatingElectric coilHeatingElectric(m, s);

  AirLoopHVACUnitaryHeatPumpAirToAir testObject(m, s, supplyFan, heatingCoil, coolingCoil, coilHeatingElectric);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)3, airLoop.supplyComponents().size());

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

  auto testObjectClone = testObject.clone(m).cast<AirLoopHVACUnitaryHeatPumpAirToAir>();
  // Clone should reset the inlet/outlet nodes
  ASSERT_FALSE(testObjectClone.inletModelObject());
  ASSERT_FALSE(testObjectClone.outletModelObject());

  supplyOutletNode = airLoop.supplyOutletNode();
  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());
}

TEST_F(ModelFixture, AirLoopHVACUnitaryHeatPumpAirToAir_VariableSpeedCoils) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanConstantVolume supplyFan(m, s);

  CoilCoolingDXVariableSpeed coolingCoil(m);
  CoilHeatingDXVariableSpeed heatingCoil(m);

  CoilHeatingElectric coilHeatingElectric(m, s);

  AirLoopHVACUnitaryHeatPumpAirToAir testObject(m, s, supplyFan, heatingCoil, coolingCoil, coilHeatingElectric);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)3, airLoop.supplyComponents().size());

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

  auto testObjectClone = testObject.clone(m).cast<AirLoopHVACUnitaryHeatPumpAirToAir>();
  supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());
}

TEST_F(ModelFixture, AirLoopHVACUnitaryHeatPumpAirToAir_CoilSystemIntegratedHeatPumpAirSource) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanConstantVolume supplyFan(m, s);

  CoilCoolingDXVariableSpeed spaceCoolingCoil(m);
  CoilHeatingDXVariableSpeed spaceHeatingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed dedicatedWaterHeatingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed scwhCoil(m);
  CoilCoolingDXVariableSpeed scdwhCoolingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed scdwhWaterHeatingCoil(m);
  CoilHeatingDXVariableSpeed shdwhHeatingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed shdwhWaterHeatingCoil(m);

  CoilSystemIntegratedHeatPumpAirSource coilSystem(m, spaceCoolingCoil, spaceHeatingCoil, dedicatedWaterHeatingCoil, scwhCoil, scdwhCoolingCoil,
                                                   scdwhWaterHeatingCoil, shdwhHeatingCoil, shdwhWaterHeatingCoil);

  CoilHeatingElectric coilHeatingElectric(m, s);

  AirLoopHVACUnitaryHeatPumpAirToAir testObject(m, s, supplyFan, coilSystem, coilSystem, coilHeatingElectric);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)3, airLoop.supplyComponents().size());

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

  auto testObjectClone = testObject.clone(m).cast<AirLoopHVACUnitaryHeatPumpAirToAir>();
  supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());
}

TEST_F(ModelFixture, AirLoopHVACUnitaryHeatPumpAirToAir_Ctor_WrongChildType) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanConstantVolume supplyFan(m, s);

  CoilCoolingDXVariableSpeed cc(m);
  CoilHeatingDXVariableSpeed hc(m);

  // Wrong supplemental heating coil type, I expect a graceful failure, not a segfault
  CoilHeatingDXVariableSpeed suppHC(m);

  // The segfault is caught here, but if graceful, the remove() was called!
  EXPECT_THROW(AirLoopHVACUnitaryHeatPumpAirToAir(m, s, supplyFan, hc, cc, suppHC), openstudio::Exception);
  EXPECT_EQ(0, m.getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAir>().size());
}
