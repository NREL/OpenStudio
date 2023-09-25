/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../WaterHeaterHeatPumpWrappedCondenser.hpp"
#include "../WaterHeaterHeatPumpWrappedCondenser_Impl.hpp"
#include "../FanOnOff.hpp"
#include "../FanOnOff_Impl.hpp"
#include "../CoilWaterHeatingAirToWaterHeatPumpWrapped.hpp"
#include "../CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl.hpp"
#include "../HVACComponent.hpp"
#include "../HVACComponent_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"
#include "../WaterHeaterStratified.hpp"
#include "../WaterHeaterStratified_Impl.hpp"
#include "../PlantLoop.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, WaterHeaterHeatPumpWrappedCondenser_GettersSetters) {
  Model m;

  WaterHeaterHeatPumpWrappedCondenser hpwh(m);
  hpwh.setName("My WaterHeaterHeatPumpWrappedCondenser");

  // Availability Schedule: Optional Object
  {
    ScheduleConstant obj(m);
    EXPECT_TRUE(hpwh.setAvailabilitySchedule(obj));
    ASSERT_TRUE(hpwh.availabilitySchedule());
    EXPECT_EQ(obj, hpwh.availabilitySchedule().get());
  }

  // Compressor Setpoint Temperature Schedule: Required Object
  {
    ScheduleConstant obj(m);
    EXPECT_TRUE(hpwh.setCompressorSetpointTemperatureSchedule(obj));
    EXPECT_EQ(obj, hpwh.compressorSetpointTemperatureSchedule());
  }

  // Dead Band Temperature Difference: Required Double
  EXPECT_TRUE(hpwh.setDeadBandTemperatureDifference(10.0));
  EXPECT_EQ(10.0, hpwh.deadBandTemperatureDifference());
  // Bad Value
  EXPECT_FALSE(hpwh.setDeadBandTemperatureDifference(-10.0));
  EXPECT_EQ(10.0, hpwh.deadBandTemperatureDifference());

  // Condenser Bottom Location: Required Double
  EXPECT_TRUE(hpwh.setCondenserBottomLocation(0.1));
  EXPECT_EQ(0.1, hpwh.condenserBottomLocation());
  // Bad Value
  EXPECT_FALSE(hpwh.setCondenserBottomLocation(-10.0));
  EXPECT_EQ(0.1, hpwh.condenserBottomLocation());

  // Condenser Top Location: Required Double
  EXPECT_TRUE(hpwh.setCondenserTopLocation(0.1));
  EXPECT_EQ(0.1, hpwh.condenserTopLocation());
  // Bad Value
  EXPECT_FALSE(hpwh.setCondenserTopLocation(-10.0));
  EXPECT_EQ(0.1, hpwh.condenserTopLocation());

  // Evaporator Air Flow Rate: Required Double
  hpwh.autocalculateEvaporatorAirFlowRate();
  EXPECT_TRUE(hpwh.isEvaporatorAirFlowRateAutocalculated());
  EXPECT_TRUE(hpwh.setEvaporatorAirFlowRate(0.1));
  ASSERT_TRUE(hpwh.evaporatorAirFlowRate());
  EXPECT_EQ(0.1, hpwh.evaporatorAirFlowRate().get());
  // Bad Value
  EXPECT_FALSE(hpwh.setEvaporatorAirFlowRate(-10.0));
  ASSERT_TRUE(hpwh.evaporatorAirFlowRate());
  EXPECT_EQ(0.1, hpwh.evaporatorAirFlowRate().get());
  EXPECT_FALSE(hpwh.isEvaporatorAirFlowRateAutocalculated());

  // Inlet Air Configuration: Required String
  EXPECT_TRUE(hpwh.setInletAirConfiguration("OutdoorAirOnly"));
  EXPECT_EQ("OutdoorAirOnly", hpwh.inletAirConfiguration());
  // Bad Value
  EXPECT_FALSE(hpwh.setInletAirConfiguration("BADENUM"));
  EXPECT_EQ("OutdoorAirOnly", hpwh.inletAirConfiguration());

  // Inlet Air Temperature Schedule: Optional Object but set in Ctor
  {
    ScheduleConstant obj(m);
    EXPECT_TRUE(hpwh.setInletAirTemperatureSchedule(obj));
    ASSERT_TRUE(hpwh.inletAirTemperatureSchedule());
    EXPECT_EQ(obj, hpwh.inletAirTemperatureSchedule().get());
  }

  // Inlet Air Humidity Schedule: Optional Object but set in Ctor
  {
    ScheduleConstant obj(m);
    EXPECT_TRUE(hpwh.setInletAirHumiditySchedule(obj));
    ASSERT_TRUE(hpwh.inletAirHumiditySchedule());
    EXPECT_EQ(obj, hpwh.inletAirHumiditySchedule().get());
  }

  // Tank: Required Object
  WaterHeaterStratified wh(m);
  EXPECT_TRUE(hpwh.setTank(wh));
  EXPECT_EQ(wh, hpwh.tank());

  // DX Coil Name: Required Object
  CoilWaterHeatingAirToWaterHeatPumpWrapped coil(m);
  EXPECT_TRUE(hpwh.setDXCoil(coil));
  EXPECT_EQ(coil, hpwh.dXCoil());

  // Minimum Inlet Air Temperature for Compressor Operation: Required Double
  EXPECT_TRUE(hpwh.setMinimumInletAirTemperatureforCompressorOperation(3));
  EXPECT_EQ(3, hpwh.minimumInletAirTemperatureforCompressorOperation());
  // Test for #4053 specifically
  EXPECT_TRUE(hpwh.setMinimumInletAirTemperatureforCompressorOperation(-10.0));
  EXPECT_EQ(-10.0, hpwh.minimumInletAirTemperatureforCompressorOperation());

  // Maximum Inlet Air Temperature for Compressor Operation: Required Double
  EXPECT_TRUE(hpwh.setMaximumInletAirTemperatureforCompressorOperation(60.0));
  EXPECT_EQ(60.0, hpwh.maximumInletAirTemperatureforCompressorOperation());
  // Bad Value
  EXPECT_FALSE(hpwh.setMaximumInletAirTemperatureforCompressorOperation(16.0));
  EXPECT_EQ(60.0, hpwh.maximumInletAirTemperatureforCompressorOperation());

  // Compressor Location: Required String
  EXPECT_TRUE(hpwh.setCompressorLocation("Schedule"));
  EXPECT_EQ("Schedule", hpwh.compressorLocation());
  // Bad Value
  EXPECT_FALSE(hpwh.setCompressorLocation("BADENUM"));
  EXPECT_EQ("Schedule", hpwh.compressorLocation());

  // Compressor Ambient Temperature Schedule: Optional Object
  {
    ScheduleConstant obj(m);
    EXPECT_TRUE(hpwh.setCompressorAmbientTemperatureSchedule(obj));
    ASSERT_TRUE(hpwh.compressorAmbientTemperatureSchedule());
    EXPECT_EQ(obj, hpwh.compressorAmbientTemperatureSchedule().get());
  }

  // Fan Name: Required Object
  FanOnOff fan(m);
  EXPECT_TRUE(hpwh.setFan(fan));
  EXPECT_EQ(fan, hpwh.fan());

  // Fan Placement: Required String
  EXPECT_TRUE(hpwh.setFanPlacement("BlowThrough"));
  EXPECT_EQ("BlowThrough", hpwh.fanPlacement());
  // Bad Value
  EXPECT_FALSE(hpwh.setFanPlacement("BADENUM"));
  EXPECT_EQ("BlowThrough", hpwh.fanPlacement());

  // On Cycle Parasitic Electric Load: Required Double
  EXPECT_TRUE(hpwh.setOnCycleParasiticElectricLoad(0.1));
  EXPECT_EQ(0.1, hpwh.onCycleParasiticElectricLoad());
  // Bad Value
  EXPECT_FALSE(hpwh.setOnCycleParasiticElectricLoad(-10.0));
  EXPECT_EQ(0.1, hpwh.onCycleParasiticElectricLoad());

  // Off Cycle Parasitic Electric Load: Required Double
  EXPECT_TRUE(hpwh.setOffCycleParasiticElectricLoad(0.1));
  EXPECT_EQ(0.1, hpwh.offCycleParasiticElectricLoad());
  // Bad Value
  EXPECT_FALSE(hpwh.setOffCycleParasiticElectricLoad(-10.0));
  EXPECT_EQ(0.1, hpwh.offCycleParasiticElectricLoad());

  // Parasitic Heat Rejection Location: Required String
  EXPECT_TRUE(hpwh.setParasiticHeatRejectionLocation("Zone"));
  EXPECT_EQ("Zone", hpwh.parasiticHeatRejectionLocation());
  // Bad Value
  EXPECT_FALSE(hpwh.setParasiticHeatRejectionLocation("BADENUM"));
  EXPECT_EQ("Zone", hpwh.parasiticHeatRejectionLocation());

  // Inlet Air Mixer Schedule: Optional Object (for some reason unline the WaterHeaterHeatPump(PumpedCondenser) object...)
  {
    ScheduleConstant obj(m);
    EXPECT_TRUE(hpwh.setInletAirMixerSchedule(obj));
    ASSERT_TRUE(hpwh.inletAirMixerSchedule());
    EXPECT_EQ(obj, hpwh.inletAirMixerSchedule().get());
  }

  // Tank Element Control Logic: Required String
  EXPECT_TRUE(hpwh.setTankElementControlLogic("MutuallyExclusive"));
  EXPECT_EQ("MutuallyExclusive", hpwh.tankElementControlLogic());
  // Bad Value
  EXPECT_FALSE(hpwh.setTankElementControlLogic("BADENUM"));
  EXPECT_EQ("MutuallyExclusive", hpwh.tankElementControlLogic());

  // Control Sensor 1 Height In Stratified Tank: Optional Double
  EXPECT_TRUE(hpwh.setControlSensor1HeightInStratifiedTank(0.1));
  ASSERT_TRUE(hpwh.controlSensor1HeightInStratifiedTank());
  EXPECT_EQ(0.1, hpwh.controlSensor1HeightInStratifiedTank().get());
  // Bad Value
  EXPECT_FALSE(hpwh.setControlSensor1HeightInStratifiedTank(-10.0));
  ASSERT_TRUE(hpwh.controlSensor1HeightInStratifiedTank());
  EXPECT_EQ(0.1, hpwh.controlSensor1HeightInStratifiedTank().get());

  // Control Sensor 1 Weight: Required Double
  EXPECT_TRUE(hpwh.setControlSensor1Weight(0.5));
  EXPECT_EQ(0.5, hpwh.controlSensor1Weight());
  // Bad Value
  EXPECT_FALSE(hpwh.setControlSensor1Weight(-10.0));
  EXPECT_EQ(0.5, hpwh.controlSensor1Weight());

  // Control Sensor 2 Height In Stratified Tank: Optional Double
  EXPECT_TRUE(hpwh.setControlSensor2HeightInStratifiedTank(0.1));
  ASSERT_TRUE(hpwh.controlSensor2HeightInStratifiedTank());
  EXPECT_EQ(0.1, hpwh.controlSensor2HeightInStratifiedTank().get());
  // Bad Value
  EXPECT_FALSE(hpwh.setControlSensor2HeightInStratifiedTank(-10.0));
  ASSERT_TRUE(hpwh.controlSensor2HeightInStratifiedTank());
  EXPECT_EQ(0.1, hpwh.controlSensor2HeightInStratifiedTank().get());
}

TEST_F(ModelFixture, WaterHeaterHeatPumpWrappedCondenser_HeatCoolFuelTypes) {
  // Test for #4053
  Model m;
  WaterHeaterHeatPumpWrappedCondenser hpwh(m);
  auto tank = hpwh.tank().cast<WaterHeaterStratified>();

  ThermalZone zone(m);
  EXPECT_TRUE(hpwh.addToThermalZone(zone));

  PlantLoop plantLoop(m);
  EXPECT_TRUE(plantLoop.addSupplyBranchForComponent(tank));

  // Tank has no heater capacity, so it should only return the HPWH's
  EXPECT_TRUE(tank.setHeaterFuelType("NaturalGas"));
  EXPECT_TRUE(tank.setHeater1Capacity(0.0));
  EXPECT_TRUE(tank.setHeater2Capacity(0.0));
  EXPECT_EQ(ComponentType(ComponentType::Heating), tank.componentType());
  EXPECT_EQ(0, tank.coolingFuelTypes().size());
  ASSERT_EQ(1, tank.heatingFuelTypes().size());
  EXPECT_EQ(FuelType(FuelType::Electricity), tank.heatingFuelTypes().front());
  ASSERT_EQ(1, tank.appGHeatingFuelTypes().size());
  EXPECT_EQ(AppGFuelType(AppGFuelType::HeatPump), tank.appGHeatingFuelTypes().front());

  // HPWH should not affect the zone
  EXPECT_EQ(ComponentType(ComponentType::None), zone.componentType());
  EXPECT_EQ(0, zone.coolingFuelTypes().size());
  EXPECT_EQ(0, zone.heatingFuelTypes().size());
  EXPECT_EQ(0, zone.appGHeatingFuelTypes().size());
  // And that's because we set the HPWH to be none/empty
  EXPECT_EQ(ComponentType(ComponentType::None), hpwh.componentType());
  EXPECT_EQ(0, hpwh.coolingFuelTypes().size());
  EXPECT_EQ(0, hpwh.heatingFuelTypes().size());
  EXPECT_EQ(0, hpwh.appGHeatingFuelTypes().size());

  // But the tank should affect the plantLoop
  EXPECT_EQ(ComponentType(ComponentType::Heating), plantLoop.componentType());
  EXPECT_EQ(0, plantLoop.coolingFuelTypes().size());
  ASSERT_EQ(1, plantLoop.heatingFuelTypes().size());
  EXPECT_EQ(FuelType(FuelType::Electricity), plantLoop.heatingFuelTypes().front());
  ASSERT_EQ(1, plantLoop.appGHeatingFuelTypes().size());
  EXPECT_EQ(AppGFuelType(AppGFuelType::HeatPump), plantLoop.appGHeatingFuelTypes().front());
}
