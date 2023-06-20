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
#include "../HumidifierSteamGas.hpp"
#include "../HumidifierSteamGas_Impl.hpp"
#include "../CoilCoolingWater.hpp"
#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../Curve.hpp"
#include "../CurveLinear.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, HumidifierSteamGas_HumidifierSteamGas) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      HumidifierSteamGas humidifier(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  HumidifierSteamGas humidifier(m);

  EXPECT_FALSE(humidifier.availabilitySchedule());
  EXPECT_FALSE(humidifier.ratedCapacity());
  EXPECT_TRUE(humidifier.isRatedCapacityAutosized());
  EXPECT_TRUE(humidifier.ratedGasUseRate());
  EXPECT_EQ(104000, humidifier.ratedGasUseRate().get());
  EXPECT_FALSE(humidifier.isRatedGasUseRateAutosized());
  EXPECT_EQ(0.8, humidifier.thermalEfficiency());
  EXPECT_TRUE(humidifier.isThermalEfficiencyDefaulted());
  EXPECT_FALSE(humidifier.thermalEfficiencyModifierCurve());
  EXPECT_FALSE(humidifier.ratedFanPower());
  EXPECT_EQ(0.0, humidifier.auxiliaryElectricPower());
  EXPECT_TRUE(humidifier.isAuxiliaryElectricPowerDefaulted());
  EXPECT_EQ("FixedInletWaterTemperature", humidifier.inletWaterTemperatureOption());
  EXPECT_TRUE(humidifier.isInletWaterTemperatureOptionDefaulted());

  ScheduleConstant schedule(m);
  EXPECT_TRUE(humidifier.setAvailabilitySchedule(schedule));
  EXPECT_TRUE(humidifier.availabilitySchedule());
  humidifier.resetAvailabilitySchedule();
  EXPECT_FALSE(humidifier.availabilitySchedule());

  EXPECT_TRUE(humidifier.setRatedCapacity(1000));
  EXPECT_TRUE(humidifier.ratedCapacity());
  EXPECT_EQ(1000, humidifier.ratedCapacity().get());
  EXPECT_FALSE(humidifier.isRatedCapacityAutosized());
  humidifier.autosizeRatedCapacity();
  EXPECT_TRUE(humidifier.isRatedCapacityAutosized());

  humidifier.autosizeRatedGasUseRate();
  EXPECT_TRUE(humidifier.isRatedGasUseRateAutosized());
  EXPECT_FALSE(humidifier.ratedGasUseRate());
  humidifier.resetRatedGasUseRate();
  EXPECT_FALSE(humidifier.ratedGasUseRate());
  EXPECT_FALSE(humidifier.isRatedGasUseRateAutosized());

  EXPECT_TRUE(humidifier.setThermalEfficiency(0.9));
  EXPECT_EQ(0.9, humidifier.thermalEfficiency());
  EXPECT_FALSE(humidifier.isThermalEfficiencyDefaulted());
  humidifier.resetThermalEfficiency();
  EXPECT_EQ(0.8, humidifier.thermalEfficiency());
  EXPECT_TRUE(humidifier.isThermalEfficiencyDefaulted());

  CurveLinear curve(m);
  EXPECT_TRUE(humidifier.setThermalEfficiencyModifierCurve(curve));
  EXPECT_TRUE(humidifier.thermalEfficiencyModifierCurve());
  humidifier.resetThermalEfficiencyModifierCurve();
  EXPECT_FALSE(humidifier.thermalEfficiencyModifierCurve());

  EXPECT_TRUE(humidifier.setRatedFanPower(10));
  EXPECT_TRUE(humidifier.ratedFanPower());
  humidifier.resetRatedFanPower();
  EXPECT_FALSE(humidifier.ratedFanPower());

  EXPECT_TRUE(humidifier.setAuxiliaryElectricPower(0.1));
  EXPECT_EQ(0.1, humidifier.auxiliaryElectricPower());
  EXPECT_FALSE(humidifier.isAuxiliaryElectricPowerDefaulted());
  humidifier.resetAuxiliaryElectricPower();
  EXPECT_EQ(0.0, humidifier.auxiliaryElectricPower());
  EXPECT_TRUE(humidifier.isAuxiliaryElectricPowerDefaulted());

  EXPECT_TRUE(humidifier.setInletWaterTemperatureOption("VariableInletWaterTemperature"));
  EXPECT_EQ("VariableInletWaterTemperature", humidifier.inletWaterTemperatureOption());
  EXPECT_FALSE(humidifier.isInletWaterTemperatureOptionDefaulted());
  humidifier.resetInletWaterTemperatureOption();
  EXPECT_EQ("FixedInletWaterTemperature", humidifier.inletWaterTemperatureOption());
  EXPECT_TRUE(humidifier.isInletWaterTemperatureOptionDefaulted());
}

TEST_F(ModelFixture, HumidifierSteamGas_addToNode) {
  Model m;
  HumidifierSteamGas testObject(m);

  AirLoopHVAC airLoop(m);
  ControllerOutdoorAir controllerOutdoorAir(m);
  AirLoopHVACOutdoorAirSystem outdoorAirSystem(m, controllerOutdoorAir);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  outdoorAirSystem.addToNode(supplyOutletNode);

  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_TRUE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_TRUE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.demandComponents().size());

  HumidifierSteamGas testObject2(m);
  HumidifierSteamGas testObject3(m);

  if (boost::optional<Node> OANode = outdoorAirSystem.outboardOANode()) {
    EXPECT_TRUE(testObject2.addToNode(*OANode));
    EXPECT_EQ((unsigned)3, airLoop.supplyComponents().size());
    EXPECT_EQ((unsigned)3, outdoorAirSystem.oaComponents().size());
  }

  if (boost::optional<Node> reliefNode = outdoorAirSystem.outboardReliefNode()) {
    EXPECT_TRUE(testObject3.addToNode(*reliefNode));
    EXPECT_EQ((unsigned)3, airLoop.supplyComponents().size());
    EXPECT_EQ((unsigned)3, outdoorAirSystem.reliefComponents().size());
  }

  auto testObjectClone = testObject.clone(m).cast<HumidifierSteamGas>();
  supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());
}
