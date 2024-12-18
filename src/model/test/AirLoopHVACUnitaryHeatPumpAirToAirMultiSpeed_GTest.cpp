/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
#include "../AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl.hpp"

#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../CoilHeatingElectricMultiStage.hpp"
#include "../CoilCoolingDXMultiSpeed.hpp"
#include "../CoilCoolingDXSingleSpeed.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CoilHeatingGasMultiStage.hpp"
#include "../FanConstantVolume.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../PlantLoop.hpp"
#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"
#include "../ThermalZone.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_GettersSetters) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanConstantVolume supplyFan(m, s);

  CoilCoolingDXMultiSpeed cc(m);
  CoilHeatingElectricMultiStage hc(m);

  CoilHeatingElectric suppHC(m);

  AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed hp(m, supplyFan, hc, cc, suppHC);

  hp.setName("My AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed");

  // Availability Schedule: Optional Object
  ScheduleConstant availabilitySchedule(m);
  EXPECT_TRUE(hp.setAvailabilitySchedule(availabilitySchedule));
  ASSERT_TRUE(hp.availabilitySchedule());
  EXPECT_EQ(availabilitySchedule, hp.availabilitySchedule().get());

  // Air Inlet Node: Required Object
  // Air Outlet Node: Required Object

  ThermalZone controllingZoneorThermostatLocation(m);
  // Controlling Zone or Thermostat Location: Optional Object
  EXPECT_TRUE(hp.setControllingZoneorThermostatLocation(controllingZoneorThermostatLocation));
  ASSERT_TRUE(hp.controllingZoneorThermostatLocation());
  EXPECT_EQ(controllingZoneorThermostatLocation, hp.controllingZoneorThermostatLocation().get());

  // Supply Air Fan: Required Object
  FanConstantVolume supplyAirFan(m, s);
  EXPECT_TRUE(hp.setSupplyAirFan(supplyAirFan));
  EXPECT_EQ(supplyAirFan, hp.supplyAirFan());

  // Supply Air Fan Placement: Required String
  EXPECT_TRUE(hp.setSupplyAirFanPlacement("BlowThrough"));
  EXPECT_EQ("BlowThrough", hp.supplyAirFanPlacement());
  // Bad Value
  EXPECT_FALSE(hp.setSupplyAirFanPlacement("BADENUM"));
  EXPECT_EQ("BlowThrough", hp.supplyAirFanPlacement());

  // Supply Air Fan Operating Mode Schedule: Optional Object
  ScheduleConstant supplyAirFanOperatingModeSchedule(m);
  EXPECT_TRUE(hp.setSupplyAirFanOperatingModeSchedule(supplyAirFanOperatingModeSchedule));
  EXPECT_EQ(supplyAirFanOperatingModeSchedule, hp.supplyAirFanOperatingModeSchedule());

  // Heating Coil: Required Object
  CoilHeatingGasMultiStage heatingCoil(m);
  EXPECT_TRUE(hp.setHeatingCoil(heatingCoil));
  EXPECT_EQ(heatingCoil, hp.heatingCoil());

  // Minimum Outdoor Dry-Bulb Temperature for Compressor Operation: Required Double
  EXPECT_TRUE(hp.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-18.9));
  EXPECT_EQ(-18.9, hp.minimumOutdoorDryBulbTemperatureforCompressorOperation());
  // Bad Value
  EXPECT_FALSE(hp.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-30.0));
  EXPECT_EQ(-18.9, hp.minimumOutdoorDryBulbTemperatureforCompressorOperation());

  // Cooling Coil: Required Object
  CoilCoolingDXMultiSpeed coolingCoil(m);
  EXPECT_TRUE(hp.setCoolingCoil(coolingCoil));
  EXPECT_EQ(coolingCoil, hp.coolingCoil());

  // Supplemental Heating Coil: Optional Object
  CoilHeatingElectric supplementalHeatingCoil(m);
  EXPECT_TRUE(hp.setSupplementalHeatingCoil(supplementalHeatingCoil));
  EXPECT_EQ(supplementalHeatingCoil, hp.supplementalHeatingCoil());

  // Maximum Supply Air Temperature from Supplemental Heater: Required Double
  // Autosize
  hp.autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();
  EXPECT_TRUE(hp.isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized());
  // Set
  EXPECT_TRUE(hp.setMaximumSupplyAirTemperaturefromSupplementalHeater(1.4));
  ASSERT_TRUE(hp.maximumSupplyAirTemperaturefromSupplementalHeater());
  EXPECT_EQ(1.4, hp.maximumSupplyAirTemperaturefromSupplementalHeater().get());
  EXPECT_FALSE(hp.isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized());

  // Maximum Outdoor Dry-Bulb Temperature for Supplemental Heater Operation: Required Double
  EXPECT_TRUE(hp.setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(18.2));
  EXPECT_EQ(18.2, hp.maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation());
  // Bad Value
  EXPECT_FALSE(hp.setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(31.0));
  EXPECT_EQ(18.2, hp.maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation());

  // Auxiliary On-Cycle Electric Power: Required Double
  EXPECT_TRUE(hp.setAuxiliaryOnCycleElectricPower(1.6));
  EXPECT_EQ(1.6, hp.auxiliaryOnCycleElectricPower());
  // Bad Value
  EXPECT_FALSE(hp.setAuxiliaryOnCycleElectricPower(-10.0));
  EXPECT_EQ(1.6, hp.auxiliaryOnCycleElectricPower());

  // Auxiliary Off-Cycle Electric Power: Required Double
  EXPECT_TRUE(hp.setAuxiliaryOffCycleElectricPower(1.7));
  EXPECT_EQ(1.7, hp.auxiliaryOffCycleElectricPower());
  // Bad Value
  EXPECT_FALSE(hp.setAuxiliaryOffCycleElectricPower(-10.0));
  EXPECT_EQ(1.7, hp.auxiliaryOffCycleElectricPower());

  // Design Heat Recovery Water Flow Rate: Required Double
  EXPECT_TRUE(hp.setDesignHeatRecoveryWaterFlowRate(1.8));
  EXPECT_EQ(1.8, hp.designHeatRecoveryWaterFlowRate());
  // Bad Value
  EXPECT_FALSE(hp.setDesignHeatRecoveryWaterFlowRate(-10.0));
  EXPECT_EQ(1.8, hp.designHeatRecoveryWaterFlowRate());

  // Maximum Temperature for Heat Recovery: Required Double
  EXPECT_TRUE(hp.setMaximumTemperatureforHeatRecovery(95.0));
  EXPECT_EQ(95.0, hp.maximumTemperatureforHeatRecovery());
  // Bad Value
  EXPECT_FALSE(hp.setMaximumTemperatureforHeatRecovery(-10.0));
  EXPECT_EQ(95.0, hp.maximumTemperatureforHeatRecovery());

  // Heat Recovery Water Inlet Node: Required Object

  // Heat Recovery Water Outlet Node: Required Object

  // Supply Air Flow Rate When No Cooling or Heating is Needed: Required Double
  // Autosize
  hp.autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
  EXPECT_TRUE(hp.isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized());
  // Set
  EXPECT_TRUE(hp.setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(2.2));
  ASSERT_TRUE(hp.supplyAirFlowRateWhenNoCoolingorHeatingisNeeded());
  EXPECT_EQ(2.2, hp.supplyAirFlowRateWhenNoCoolingorHeatingisNeeded().get());
  // Bad Value
  EXPECT_FALSE(hp.setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(-10.0));
  ASSERT_TRUE(hp.supplyAirFlowRateWhenNoCoolingorHeatingisNeeded());
  EXPECT_EQ(2.2, hp.supplyAirFlowRateWhenNoCoolingorHeatingisNeeded().get());
  EXPECT_FALSE(hp.isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized());

  // Number of Speeds for Heating: Required Integer
  EXPECT_TRUE(hp.setNumberofSpeedsforHeating(3));
  EXPECT_EQ(3, hp.numberofSpeedsforHeating());
  // Bad Value
  EXPECT_FALSE(hp.setNumberofSpeedsforHeating(-9));
  EXPECT_EQ(3, hp.numberofSpeedsforHeating());

  // Number of Speeds for Cooling: Required Integer
  EXPECT_TRUE(hp.setNumberofSpeedsforCooling(3));
  EXPECT_EQ(3, hp.numberofSpeedsforCooling());
  // Bad Value
  EXPECT_FALSE(hp.setNumberofSpeedsforCooling(-8));
  EXPECT_EQ(3, hp.numberofSpeedsforCooling());

  // Speed 1 Supply Air Flow Rate During Heating Operation: Required Double
  // Autosize
  hp.autosizeSpeed1SupplyAirFlowRateDuringHeatingOperation();
  EXPECT_TRUE(hp.isSpeed1SupplyAirFlowRateDuringHeatingOperationAutosized());
  // Set
  EXPECT_TRUE(hp.setSpeed1SupplyAirFlowRateDuringHeatingOperation(2.5));
  ASSERT_TRUE(hp.speed1SupplyAirFlowRateDuringHeatingOperation());
  EXPECT_EQ(2.5, hp.speed1SupplyAirFlowRateDuringHeatingOperation().get());
  // Bad Value
  EXPECT_FALSE(hp.setSpeed1SupplyAirFlowRateDuringHeatingOperation(-10.0));
  ASSERT_TRUE(hp.speed1SupplyAirFlowRateDuringHeatingOperation());
  EXPECT_EQ(2.5, hp.speed1SupplyAirFlowRateDuringHeatingOperation().get());
  EXPECT_FALSE(hp.isSpeed1SupplyAirFlowRateDuringHeatingOperationAutosized());

  // Speed 2 Supply Air Flow Rate During Heating Operation: Required Double
  // Autosize
  hp.autosizeSpeed2SupplyAirFlowRateDuringHeatingOperation();
  EXPECT_TRUE(hp.isSpeed2SupplyAirFlowRateDuringHeatingOperationAutosized());
  // Set
  EXPECT_TRUE(hp.setSpeed2SupplyAirFlowRateDuringHeatingOperation(2.6));
  ASSERT_TRUE(hp.speed2SupplyAirFlowRateDuringHeatingOperation());
  EXPECT_EQ(2.6, hp.speed2SupplyAirFlowRateDuringHeatingOperation().get());
  // Bad Value
  EXPECT_FALSE(hp.setSpeed2SupplyAirFlowRateDuringHeatingOperation(-10.0));
  ASSERT_TRUE(hp.speed2SupplyAirFlowRateDuringHeatingOperation());
  EXPECT_EQ(2.6, hp.speed2SupplyAirFlowRateDuringHeatingOperation().get());
  EXPECT_FALSE(hp.isSpeed2SupplyAirFlowRateDuringHeatingOperationAutosized());

  // Speed 3 Supply Air Flow Rate During Heating Operation: Required Double
  // Autosize
  hp.autosizeSpeed3SupplyAirFlowRateDuringHeatingOperation();
  EXPECT_TRUE(hp.isSpeed3SupplyAirFlowRateDuringHeatingOperationAutosized());
  // Set
  EXPECT_TRUE(hp.setSpeed3SupplyAirFlowRateDuringHeatingOperation(2.7));
  ASSERT_TRUE(hp.speed3SupplyAirFlowRateDuringHeatingOperation());
  EXPECT_EQ(2.7, hp.speed3SupplyAirFlowRateDuringHeatingOperation().get());
  // Bad Value
  EXPECT_FALSE(hp.setSpeed3SupplyAirFlowRateDuringHeatingOperation(-10.0));
  ASSERT_TRUE(hp.speed3SupplyAirFlowRateDuringHeatingOperation());
  EXPECT_EQ(2.7, hp.speed3SupplyAirFlowRateDuringHeatingOperation().get());
  EXPECT_FALSE(hp.isSpeed3SupplyAirFlowRateDuringHeatingOperationAutosized());

  // Speed 4 Supply Air Flow Rate During Heating Operation: Required Double
  // Autosize
  hp.autosizeSpeed4SupplyAirFlowRateDuringHeatingOperation();
  EXPECT_TRUE(hp.isSpeed4SupplyAirFlowRateDuringHeatingOperationAutosized());
  // Set
  EXPECT_TRUE(hp.setSpeed4SupplyAirFlowRateDuringHeatingOperation(2.8));
  ASSERT_TRUE(hp.speed4SupplyAirFlowRateDuringHeatingOperation());
  EXPECT_EQ(2.8, hp.speed4SupplyAirFlowRateDuringHeatingOperation().get());
  // Bad Value
  EXPECT_FALSE(hp.setSpeed4SupplyAirFlowRateDuringHeatingOperation(-10.0));
  ASSERT_TRUE(hp.speed4SupplyAirFlowRateDuringHeatingOperation());
  EXPECT_EQ(2.8, hp.speed4SupplyAirFlowRateDuringHeatingOperation().get());
  EXPECT_FALSE(hp.isSpeed4SupplyAirFlowRateDuringHeatingOperationAutosized());

  // Speed 1 Supply Air Flow Rate During Cooling Operation: Required Double
  // Autosize
  hp.autosizeSpeed1SupplyAirFlowRateDuringCoolingOperation();
  EXPECT_TRUE(hp.isSpeed1SupplyAirFlowRateDuringCoolingOperationAutosized());
  // Set
  EXPECT_TRUE(hp.setSpeed1SupplyAirFlowRateDuringCoolingOperation(2.9));
  ASSERT_TRUE(hp.speed1SupplyAirFlowRateDuringCoolingOperation());
  EXPECT_EQ(2.9, hp.speed1SupplyAirFlowRateDuringCoolingOperation().get());
  // Bad Value
  EXPECT_FALSE(hp.setSpeed1SupplyAirFlowRateDuringCoolingOperation(-10.0));
  ASSERT_TRUE(hp.speed1SupplyAirFlowRateDuringCoolingOperation());
  EXPECT_EQ(2.9, hp.speed1SupplyAirFlowRateDuringCoolingOperation().get());
  EXPECT_FALSE(hp.isSpeed1SupplyAirFlowRateDuringCoolingOperationAutosized());

  // Speed 2 Supply Air Flow Rate During Cooling Operation: Required Double
  // Autosize
  hp.autosizeSpeed2SupplyAirFlowRateDuringCoolingOperation();
  EXPECT_TRUE(hp.isSpeed2SupplyAirFlowRateDuringCoolingOperationAutosized());
  // Set
  EXPECT_TRUE(hp.setSpeed2SupplyAirFlowRateDuringCoolingOperation(3.0));
  ASSERT_TRUE(hp.speed2SupplyAirFlowRateDuringCoolingOperation());
  EXPECT_EQ(3.0, hp.speed2SupplyAirFlowRateDuringCoolingOperation().get());
  // Bad Value
  EXPECT_FALSE(hp.setSpeed2SupplyAirFlowRateDuringCoolingOperation(-10.0));
  ASSERT_TRUE(hp.speed2SupplyAirFlowRateDuringCoolingOperation());
  EXPECT_EQ(3.0, hp.speed2SupplyAirFlowRateDuringCoolingOperation().get());
  EXPECT_FALSE(hp.isSpeed2SupplyAirFlowRateDuringCoolingOperationAutosized());

  // Speed 3 Supply Air Flow Rate During Cooling Operation: Required Double
  // Autosize
  hp.autosizeSpeed3SupplyAirFlowRateDuringCoolingOperation();
  EXPECT_TRUE(hp.isSpeed3SupplyAirFlowRateDuringCoolingOperationAutosized());
  // Set
  EXPECT_TRUE(hp.setSpeed3SupplyAirFlowRateDuringCoolingOperation(3.1));
  ASSERT_TRUE(hp.speed3SupplyAirFlowRateDuringCoolingOperation());
  EXPECT_EQ(3.1, hp.speed3SupplyAirFlowRateDuringCoolingOperation().get());
  // Bad Value
  EXPECT_FALSE(hp.setSpeed3SupplyAirFlowRateDuringCoolingOperation(-10.0));
  ASSERT_TRUE(hp.speed3SupplyAirFlowRateDuringCoolingOperation());
  EXPECT_EQ(3.1, hp.speed3SupplyAirFlowRateDuringCoolingOperation().get());
  EXPECT_FALSE(hp.isSpeed3SupplyAirFlowRateDuringCoolingOperationAutosized());

  // Speed 4 Supply Air Flow Rate During Cooling Operation: Required Double
  // Autosize
  hp.autosizeSpeed4SupplyAirFlowRateDuringCoolingOperation();
  EXPECT_TRUE(hp.isSpeed4SupplyAirFlowRateDuringCoolingOperationAutosized());
  // Set
  EXPECT_TRUE(hp.setSpeed4SupplyAirFlowRateDuringCoolingOperation(3.2));
  ASSERT_TRUE(hp.speed4SupplyAirFlowRateDuringCoolingOperation());
  EXPECT_EQ(3.2, hp.speed4SupplyAirFlowRateDuringCoolingOperation().get());
  // Bad Value
  EXPECT_FALSE(hp.setSpeed4SupplyAirFlowRateDuringCoolingOperation(-10.0));
  ASSERT_TRUE(hp.speed4SupplyAirFlowRateDuringCoolingOperation());
  EXPECT_EQ(3.2, hp.speed4SupplyAirFlowRateDuringCoolingOperation().get());
  EXPECT_FALSE(hp.isSpeed4SupplyAirFlowRateDuringCoolingOperationAutosized());
}

TEST_F(ModelFixture, AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_addToNode) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanConstantVolume supplyFan(m, s);

  CoilCoolingDXMultiSpeed cc(m);
  CoilHeatingElectricMultiStage hc(m);

  CoilHeatingElectric suppHC(m);

  AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed testObject(m, supplyFan, hc, cc, suppHC);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  EXPECT_EQ(2, airLoop.supplyComponents().size());
  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ(3, airLoop.supplyComponents().size());

  EXPECT_EQ(5, airLoop.demandComponents().size());
  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ(5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_EQ(5, plantLoop.supplyComponents().size());
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ(5, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_EQ(5, plantLoop.demandComponents().size());
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ(5, plantLoop.demandComponents().size());

  auto testObjectClone = testObject.clone(m).cast<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed>();
  // Clone should reset the inlet/outlet nodes
  ASSERT_FALSE(testObjectClone.inletModelObject());
  ASSERT_FALSE(testObjectClone.outletModelObject());

  supplyOutletNode = airLoop.supplyOutletNode();
  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ(5, airLoop.supplyComponents().size());
}

TEST_F(ModelFixture, AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Ctor_WrongChildType) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanConstantVolume supplyFan(m, s);

  // Wrong cooling type, I expect a graceful failure, not a segfault
  CoilCoolingDXSingleSpeed cc(m);
  CoilHeatingElectricMultiStage hc(m);
  CoilHeatingElectric suppHC(m);

  // The segfault is caught here, but if graceful, the remove() was called!
  EXPECT_THROW(AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed(m, supplyFan, hc, cc, suppHC), openstudio::Exception);
  EXPECT_EQ(0, m.getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed>().size());
}
