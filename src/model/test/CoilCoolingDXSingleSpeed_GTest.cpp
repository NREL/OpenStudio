/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../CoilCoolingDXSingleSpeed.hpp"
#include "../CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../Schedule.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveQuadratic.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilCoolingDXSingleSpeed_RatedTotalCoolingCapacity_GettersSetters) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  CurveBiquadratic totalCoolingCapacityFunctionofTemperatureCurve(m);
  CurveQuadratic totalCoolingCapacityFunctionofFlowFractionCurve(m);
  CurveBiquadratic energyInputRatioFunctionofTemperatureCurve(m);
  CurveQuadratic energyInputRatioFunctionofFlowFractionCurve(m);
  CurveQuadratic partLoadFractionCorrelationCurve(m);

  CoilCoolingDXSingleSpeed coil(m, s, totalCoolingCapacityFunctionofTemperatureCurve, totalCoolingCapacityFunctionofFlowFractionCurve,
                                energyInputRatioFunctionofTemperatureCurve, energyInputRatioFunctionofFlowFractionCurve,
                                partLoadFractionCorrelationCurve);

  EXPECT_TRUE(coil.isRatedTotalCoolingCapacityAutosized());
  EXPECT_TRUE(coil.isRatedSensibleHeatRatioAutosized());
  EXPECT_TRUE(coil.isRatedAirFlowRateAutosized());
  EXPECT_TRUE(coil.isEvaporativeCondenserAirFlowRateAutosized());
  EXPECT_TRUE(coil.isEvaporativeCondenserPumpRatedPowerConsumptionAutosized());

  coil.setRatedTotalCoolingCapacity(1.0);
  coil.setRatedSensibleHeatRatio(0.6);
  coil.setRatedCOP(3.6);
  coil.setRatedAirFlowRate(1.1);
  coil.setRatedEvaporatorFanPowerPerVolumeFlowRate(600.0);
  coil.setNominalTimeForCondensateRemovalToBegin(50.0);
  coil.setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(3.0);
  coil.setMaximumCyclingRate(3.2);
  coil.setLatentCapacityTimeConstant(499.8);
  coil.setEvaporativeCondenserEffectiveness(0.8);
  coil.setEvaporativeCondenserAirFlowRate(1.2);
  coil.setEvaporativeCondenserPumpRatedPowerConsumption(1.3);
  coil.setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(1.4);
  coil.setBasinHeaterCapacity(1.5);
  coil.setBasinHeaterSetpointTemperature(2.5);

  EXPECT_EQ(coil.ratedTotalCoolingCapacity().get(), 1.0);
  EXPECT_EQ(coil.ratedSensibleHeatRatio().get(), 0.6);
  EXPECT_EQ(coil.ratedCOP(), 3.6);
  EXPECT_EQ(coil.ratedAirFlowRate().get(), 1.1);
  EXPECT_EQ(coil.ratedEvaporatorFanPowerPerVolumeFlowRate(), 600.0);
  EXPECT_EQ(coil.nominalTimeForCondensateRemovalToBegin(), 50.0);
  EXPECT_EQ(coil.ratioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(), 3.0);
  EXPECT_EQ(coil.maximumCyclingRate(), 3.2);
  EXPECT_EQ(coil.latentCapacityTimeConstant(), 499.8);
  EXPECT_EQ(coil.evaporativeCondenserEffectiveness(), 0.8);
  EXPECT_EQ(coil.evaporativeCondenserAirFlowRate().get(), 1.2);
  EXPECT_EQ(coil.evaporativeCondenserPumpRatedPowerConsumption().get(), 1.3);
  EXPECT_EQ(coil.maximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(), 1.4);
  EXPECT_EQ(coil.basinHeaterCapacity(), 1.5);
  EXPECT_EQ(coil.basinHeaterSetpointTemperature(), 2.5);

  // #3976 - Minimum Outdoor Dry-Bulb Temperature for Compressor Operation
  // IDD Default
  EXPECT_EQ(-25.0, coil.minimumOutdoorDryBulbTemperatureforCompressorOperation());
  // There are no IDD limits, so everything should work
  EXPECT_TRUE(coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-5));
  EXPECT_EQ(-5, coil.minimumOutdoorDryBulbTemperatureforCompressorOperation());
}

TEST_F(ModelFixture, CoilCoolingDXSingleSpeed_addToNode) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  CurveBiquadratic totalCoolingCapacityFunctionofTemperatureCurve(m);
  CurveQuadratic totalCoolingCapacityFunctionofFlowFractionCurve(m);
  CurveBiquadratic energyInputRatioFunctionofTemperatureCurve(m);
  CurveQuadratic energyInputRatioFunctionofFlowFractionCurve(m);
  CurveQuadratic partLoadFractionCorrelationCurve(m);

  CoilCoolingDXSingleSpeed testObject(m, s, totalCoolingCapacityFunctionofTemperatureCurve, totalCoolingCapacityFunctionofFlowFractionCurve,
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

  CoilCoolingDXSingleSpeed testObject2(m, s, c1, c2, c3, c4, c5);

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

  CoilCoolingDXSingleSpeed testObject3(m, s, c1_2, c2_2, c3_2, c4_2, c5_2);

  if (boost::optional<Node> reliefNode = outdoorAirSystem.outboardReliefNode()) {
    EXPECT_TRUE(testObject3.addToNode(*reliefNode));
    EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());
    EXPECT_EQ((unsigned)3, outdoorAirSystem.reliefComponents().size());
  }

  // tests and checks to figure out clone bug
  // resolution: Due to using ModelObject::clone instead of StraightComponent::clone in reimplementation
  AirLoopHVAC airLoop2(m);
  EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());
  auto testObjectClone = testObject.clone(m).cast<CoilCoolingDXSingleSpeed>();
  auto testObjectClone2 = testObject.clone(m).cast<CoilCoolingDXSingleSpeed>();
  EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());
  Node supplyOutletNode2 = airLoop2.supplyOutletNode();
  supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_FALSE(testObjectClone.loop());
  EXPECT_FALSE(testObjectClone.airLoopHVAC());
  EXPECT_EQ(testObject.totalCoolingCapacityFunctionOfTemperatureCurve(), testObjectClone.totalCoolingCapacityFunctionOfTemperatureCurve());
  EXPECT_EQ(testObject.totalCoolingCapacityFunctionOfFlowFractionCurve(), testObjectClone.totalCoolingCapacityFunctionOfFlowFractionCurve());
  EXPECT_EQ(testObject.energyInputRatioFunctionOfTemperatureCurve(), testObjectClone.energyInputRatioFunctionOfTemperatureCurve());
  EXPECT_EQ(testObject.energyInputRatioFunctionOfFlowFractionCurve(), testObjectClone.energyInputRatioFunctionOfFlowFractionCurve());
  EXPECT_EQ(testObject.partLoadFractionCorrelationCurve(), testObjectClone.partLoadFractionCorrelationCurve());
  EXPECT_TRUE(testObject.inletModelObject());
  EXPECT_TRUE(testObject.outletModelObject());
  EXPECT_FALSE(testObjectClone.inletModelObject());
  EXPECT_FALSE(testObjectClone.outletModelObject());

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode2));
  EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());
  EXPECT_EQ((unsigned)3, airLoop2.supplyComponents().size());
  EXPECT_TRUE(testObjectClone2.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, airLoop.supplyComponents().size());
}

TEST_F(ModelFixture, CoilCoolingDXSingleSpeed_2017and2023) {
  Model model;

  CoilCoolingDXSingleSpeed speed(model);

  EXPECT_EQ(773.3, speed.ratedEvaporatorFanPowerPerVolumeFlowRate2017());
  EXPECT_EQ(934.4, speed.ratedEvaporatorFanPowerPerVolumeFlowRate2023());

  EXPECT_TRUE(speed.setRatedEvaporatorFanPowerPerVolumeFlowRate2017(8.0));
  EXPECT_TRUE(speed.setRatedEvaporatorFanPowerPerVolumeFlowRate2023(9.0));

  EXPECT_EQ(8.0, speed.ratedEvaporatorFanPowerPerVolumeFlowRate2017());
  EXPECT_EQ(9.0, speed.ratedEvaporatorFanPowerPerVolumeFlowRate2023());
}
