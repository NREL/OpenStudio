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
#include "../WaterHeaterHeatPump.hpp"
#include "../WaterHeaterHeatPump_Impl.hpp"
#include "../FanOnOff.hpp"
#include "../FanOnOff_Impl.hpp"
#include "../CoilWaterHeatingAirToWaterHeatPump.hpp"
#include "../CoilWaterHeatingAirToWaterHeatPump_Impl.hpp"
#include "../HVACComponent.hpp"
#include "../HVACComponent_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"
#include "../WaterHeaterMixed.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, WaterHeaterHeatPump) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  // Create
  ASSERT_EXIT(
    {
      Model m;
      WaterHeaterHeatPump hpwh(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, WaterHeaterHeatPump_Relationships) {
  Model m;
  WaterHeaterHeatPump hpwh(m);

  auto dxCoil = hpwh.dXCoil();
  ASSERT_FALSE(dxCoil.handle().isNull());
  auto fan = hpwh.fan();
  ASSERT_FALSE(fan.handle().isNull());
  auto tank = hpwh.tank();
  ASSERT_FALSE(tank.handle().isNull());

  auto zoneHVAC = tank.containingZoneHVACComponent();
  ASSERT_TRUE(zoneHVAC);
  EXPECT_EQ(hpwh, zoneHVAC.get());
}

TEST_F(ModelFixture, WaterHeaterHeatPump_Remove) {
  Model m;
  WaterHeaterHeatPump hpwh(m);

  auto dxCoil = hpwh.dXCoil();
  auto fan = hpwh.fan();
  auto tank = hpwh.tank();

  hpwh.remove();

  EXPECT_TRUE(hpwh.handle().isNull());
  EXPECT_TRUE(dxCoil.handle().isNull());
  EXPECT_TRUE(fan.handle().isNull());
  EXPECT_TRUE(tank.handle().isNull());
}

TEST_F(ModelFixture, WaterHeaterHeatPump_ThermalZone) {
  Model m;
  WaterHeaterHeatPump hpwh(m);

  ThermalZone zone(m);
  EXPECT_TRUE(hpwh.addToThermalZone(zone));
  EXPECT_EQ(1u, zone.equipment().size());

  hpwh.remove();
  EXPECT_TRUE(hpwh.handle().isNull());
  EXPECT_TRUE(zone.equipment().empty());
}

TEST_F(ModelFixture, WaterHeaterHeatPump_SystemConnections) {
  Model m;
  WaterHeaterHeatPump hpwh(m);

  PlantLoop plant(m);
  auto demandCount = plant.supplyComponents().size();
  auto tank = hpwh.tank();
  EXPECT_TRUE(plant.addSupplyBranchForComponent(tank));
  EXPECT_TRUE(plant.supplyComponent(tank.handle()));

  hpwh.remove();

  EXPECT_EQ(demandCount, plant.supplyComponents().size());
  EXPECT_TRUE(hpwh.handle().isNull());
  EXPECT_TRUE(tank.handle().isNull());
}

TEST_F(ModelFixture, WaterHeaterHeatPump_GettersSetters) {
  Model m;

  WaterHeaterHeatPump hpwh(m);
  hpwh.setName("My WaterHeaterHeatPump");

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

  // Condenser Water Flow Rate: Optional Double
  hpwh.autosizeCondenserWaterFlowRate();
  EXPECT_TRUE(hpwh.isCondenserWaterFlowRateAutosized());
  EXPECT_TRUE(hpwh.setCondenserWaterFlowRate(0.1));
  ASSERT_TRUE(hpwh.condenserWaterFlowRate());
  EXPECT_EQ(0.1, hpwh.condenserWaterFlowRate().get());
  // Bad Value
  EXPECT_FALSE(hpwh.setCondenserWaterFlowRate(-10.0));
  ASSERT_TRUE(hpwh.condenserWaterFlowRate());
  EXPECT_EQ(0.1, hpwh.condenserWaterFlowRate().get());
  EXPECT_FALSE(hpwh.isCondenserWaterFlowRateAutosized());

  // Evaporator Air Flow Rate: Optional Double
  hpwh.autosizeEvaporatorAirFlowRate();
  EXPECT_TRUE(hpwh.isEvaporatorAirFlowRateAutosized());
  EXPECT_TRUE(hpwh.setEvaporatorAirFlowRate(0.1));
  ASSERT_TRUE(hpwh.evaporatorAirFlowRate());
  EXPECT_EQ(0.1, hpwh.evaporatorAirFlowRate().get());
  // Bad Value
  EXPECT_FALSE(hpwh.setEvaporatorAirFlowRate(-10.0));
  ASSERT_TRUE(hpwh.evaporatorAirFlowRate());
  EXPECT_EQ(0.1, hpwh.evaporatorAirFlowRate().get());
  EXPECT_FALSE(hpwh.isEvaporatorAirFlowRateAutosized());

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
  WaterHeaterMixed wh(m);
  EXPECT_TRUE(hpwh.setTank(wh));
  EXPECT_EQ(wh, hpwh.tank());

  // DX Coil: Required Object
  CoilWaterHeatingAirToWaterHeatPump coil(m);
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
  EXPECT_FALSE(hpwh.setMaximumInletAirTemperatureforCompressorOperation(0.0));
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

  // Fan: Required Object
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

  // Inlet Air Mixer Schedule: Required Object
  {
    ScheduleConstant obj(m);
    EXPECT_TRUE(hpwh.setInletAirMixerSchedule(obj));
    EXPECT_EQ(obj, hpwh.inletAirMixerSchedule());
  }

  // Control Sensor Location In Stratified Tank: Required String
  EXPECT_TRUE(hpwh.setControlSensorLocationInStratifiedTank("Heater1"));
  EXPECT_EQ("Heater1", hpwh.controlSensorLocationInStratifiedTank());
  // Bad Value
  EXPECT_FALSE(hpwh.setControlSensorLocationInStratifiedTank("BADENUM"));
  EXPECT_EQ("Heater1", hpwh.controlSensorLocationInStratifiedTank());
}

TEST_F(ModelFixture, WaterHeaterHeatPump_InletAirTemperatureforCompressorOperation) {
  // Test for #4053
  Model m;
  WaterHeaterHeatPump hpwh(m);

  EXPECT_TRUE(hpwh.setMinimumInletAirTemperatureforCompressorOperation(-10.0));
  EXPECT_TRUE(hpwh.setMaximumInletAirTemperatureforCompressorOperation(40.0));
  EXPECT_EQ(-10.0, hpwh.minimumInletAirTemperatureforCompressorOperation());
  EXPECT_EQ(40.0, hpwh.maximumInletAirTemperatureforCompressorOperation());
}
