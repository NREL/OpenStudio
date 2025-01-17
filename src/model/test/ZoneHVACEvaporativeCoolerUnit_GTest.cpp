/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ModelFixture.hpp"

#include "../ZoneHVACEvaporativeCoolerUnit.hpp"
#include "../ZoneHVACEvaporativeCoolerUnit_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../FanSystemModel.hpp"
#include "../FanSystemModel_Impl.hpp"
#include "../EvaporativeCoolerDirectResearchSpecial.hpp"
#include "../EvaporativeCoolerDirectResearchSpecial_Impl.hpp"
#include "../EvaporativeCoolerIndirectResearchSpecial.hpp"
#include "../EvaporativeCoolerIndirectResearchSpecial_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../AirLoopHVACZoneSplitter_Impl.hpp"
#include "../PlantLoop.hpp"
#include "../PlantLoop_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneHVACEvaporativeCoolerUnit_GettersSetters) {
  Model m;

  ZoneHVACEvaporativeCoolerUnit zoneHVACEvaporativeCoolerUnit(m);

  zoneHVACEvaporativeCoolerUnit.setName("My ZoneHVACEvaporativeCoolerUnit");

  // Availability Schedule Name: Required Object
  Schedule availabilitySchedule = m.alwaysOnDiscreteSchedule();
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setAvailabilitySchedule(availabilitySchedule));
  EXPECT_EQ(availabilitySchedule, zoneHVACEvaporativeCoolerUnit.availabilitySchedule());

  // Supply Air Fan Name: Required Object
  FanSystemModel supplyAirFan(m);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setSupplyAirFan(supplyAirFan));
  EXPECT_EQ(supplyAirFan, zoneHVACEvaporativeCoolerUnit.supplyAirFan());

  // Design Supply Air Flow Rate: Required Double
  // Autosize
  zoneHVACEvaporativeCoolerUnit.autosizeDesignSupplyAirFlowRate();
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.isDesignSupplyAirFlowRateAutosized());
  // Set
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setDesignSupplyAirFlowRate(0.9));
  ASSERT_TRUE(zoneHVACEvaporativeCoolerUnit.designSupplyAirFlowRate());
  EXPECT_EQ(0.9, zoneHVACEvaporativeCoolerUnit.designSupplyAirFlowRate().get());
  // Bad Value
  EXPECT_FALSE(zoneHVACEvaporativeCoolerUnit.setDesignSupplyAirFlowRate(-10.0));
  ASSERT_TRUE(zoneHVACEvaporativeCoolerUnit.designSupplyAirFlowRate());
  EXPECT_EQ(0.9, zoneHVACEvaporativeCoolerUnit.designSupplyAirFlowRate().get());
  EXPECT_FALSE(zoneHVACEvaporativeCoolerUnit.isDesignSupplyAirFlowRateAutosized());

  // Fan Placement: Required String
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setFanPlacement("BlowThrough"));
  EXPECT_EQ("BlowThrough", zoneHVACEvaporativeCoolerUnit.fanPlacement());
  // Bad Value
  EXPECT_FALSE(zoneHVACEvaporativeCoolerUnit.setFanPlacement("BADENUM"));
  EXPECT_EQ("BlowThrough", zoneHVACEvaporativeCoolerUnit.fanPlacement());

  // Cooler Unit Control Method: Required String
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setCoolerUnitControlMethod("ZoneTemperatureDeadbandOnOffCycling"));
  EXPECT_EQ("ZoneTemperatureDeadbandOnOffCycling", zoneHVACEvaporativeCoolerUnit.coolerUnitControlMethod());
  // Bad Value
  EXPECT_FALSE(zoneHVACEvaporativeCoolerUnit.setCoolerUnitControlMethod("BADENUM"));
  EXPECT_EQ("ZoneTemperatureDeadbandOnOffCycling", zoneHVACEvaporativeCoolerUnit.coolerUnitControlMethod());

  // Throttling Range Temperature Difference: Required Double
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setThrottlingRangeTemperatureDifference(1.2));
  EXPECT_EQ(1.2, zoneHVACEvaporativeCoolerUnit.throttlingRangeTemperatureDifference());
  // Bad Value
  EXPECT_FALSE(zoneHVACEvaporativeCoolerUnit.setThrottlingRangeTemperatureDifference(-10.0));
  EXPECT_EQ(1.2, zoneHVACEvaporativeCoolerUnit.throttlingRangeTemperatureDifference());

  // Cooling Load Control Threshold Heat Transfer Rate: Required Double
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setCoolingLoadControlThresholdHeatTransferRate(1.3));
  EXPECT_EQ(1.3, zoneHVACEvaporativeCoolerUnit.coolingLoadControlThresholdHeatTransferRate());
  // Bad Value
  EXPECT_FALSE(zoneHVACEvaporativeCoolerUnit.setCoolingLoadControlThresholdHeatTransferRate(-10.0));
  EXPECT_EQ(1.3, zoneHVACEvaporativeCoolerUnit.coolingLoadControlThresholdHeatTransferRate());

  // First Evaporative Cooler: Required Object
  EvaporativeCoolerDirectResearchSpecial firstEvaporativeCooler(m, availabilitySchedule);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setFirstEvaporativeCooler(firstEvaporativeCooler));
  EXPECT_EQ(firstEvaporativeCooler, zoneHVACEvaporativeCoolerUnit.firstEvaporativeCooler());

  // Second Evaporative Cooler Name: Optional Object
  EXPECT_FALSE(zoneHVACEvaporativeCoolerUnit.secondEvaporativeCooler());
  EvaporativeCoolerIndirectResearchSpecial secondEvaporativeCooler(m);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setSecondEvaporativeCooler(secondEvaporativeCooler));
  ASSERT_TRUE(zoneHVACEvaporativeCoolerUnit.secondEvaporativeCooler());
  EXPECT_EQ(secondEvaporativeCooler, zoneHVACEvaporativeCoolerUnit.secondEvaporativeCooler().get());

  // Shut Off Relative Humidity: Required Double
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setShutOffRelativeHumidity(94.444));
  EXPECT_EQ(94.444, zoneHVACEvaporativeCoolerUnit.shutOffRelativeHumidity());
  // Bad Value
  EXPECT_FALSE(zoneHVACEvaporativeCoolerUnit.setShutOffRelativeHumidity(-10.0));
  EXPECT_EQ(94.444, zoneHVACEvaporativeCoolerUnit.shutOffRelativeHumidity());
}

TEST_F(ModelFixture, ZoneHVACEvaporativeCoolerUnit_HeatCoolFuelTypes) {
  Model m;

  ZoneHVACEvaporativeCoolerUnit zoneHVACEvaporativeCoolerUnit(m);

  EXPECT_EQ(ComponentType(ComponentType::Cooling), zoneHVACEvaporativeCoolerUnit.componentType());
  testFuelTypeEquality({FuelType::Electricity}, zoneHVACEvaporativeCoolerUnit.coolingFuelTypes());
  testFuelTypeEquality({}, zoneHVACEvaporativeCoolerUnit.heatingFuelTypes());
  testAppGFuelTypeEquality({}, zoneHVACEvaporativeCoolerUnit.appGHeatingFuelTypes());
}

TEST_F(ModelFixture, ZoneHVACEvaporativeCoolerUnit_addToThermalZone) {
  Model m;
  ZoneHVACEvaporativeCoolerUnit zonehvac(m);

  ThermalZone tz(m);
  ASSERT_TRUE(zonehvac.addToThermalZone(tz));
  EXPECT_TRUE(zonehvac.inletNode());
  EXPECT_TRUE(zonehvac.outletNode());
  ASSERT_TRUE(zonehvac.thermalZone());
  ASSERT_EQ(tz, zonehvac.thermalZone().get());
  ASSERT_EQ(1u, tz.equipment().size());
  zonehvac.removeFromThermalZone();
  ASSERT_EQ(0u, tz.equipment().size());

  ZoneHVACEvaporativeCoolerUnit zonehvac2(m);
  zonehvac2.addToThermalZone(tz);
  zonehvac2.remove();
  ASSERT_EQ(0u, tz.equipment().size());
}

TEST_F(ModelFixture, ZoneHVACEvaporativeCoolerUnit_clone) {
  Model m;
  FanSystemModel fan(m);
  Schedule availabilitySchedule = m.alwaysOnDiscreteSchedule();
  EvaporativeCoolerDirectResearchSpecial firstEvaporativeCooler(m, availabilitySchedule);
  ZoneHVACEvaporativeCoolerUnit zonehvac(m, availabilitySchedule, fan, firstEvaporativeCooler);
  EvaporativeCoolerIndirectResearchSpecial secondEvaporativeCooler(m);
  EXPECT_TRUE(zonehvac.setSecondEvaporativeCooler(secondEvaporativeCooler));
  EXPECT_EQ(1u, m.getConcreteModelObjects<FanSystemModel>().size());
  EXPECT_EQ(1u, m.getConcreteModelObjects<EvaporativeCoolerDirectResearchSpecial>().size());
  EXPECT_EQ(1u, m.getConcreteModelObjects<EvaporativeCoolerIndirectResearchSpecial>().size());

  ThermalZone tz(m);
  EXPECT_TRUE(zonehvac.addToThermalZone(tz));
  EXPECT_TRUE(zonehvac.inletNode());
  EXPECT_TRUE(zonehvac.outletNode());
  EXPECT_TRUE(zonehvac.thermalZone());
  {
    Model m2;
    auto zonehvacClone = zonehvac.clone(m2).cast<ZoneHVACEvaporativeCoolerUnit>();
    EXPECT_FALSE(zonehvacClone.inletNode());
    EXPECT_FALSE(zonehvacClone.outletNode());
    EXPECT_FALSE(zonehvacClone.thermalZone());
    EXPECT_NE(fan.handle(), zonehvacClone.supplyAirFan().handle());
    EXPECT_NE(firstEvaporativeCooler.handle(), zonehvacClone.firstEvaporativeCooler().handle());
    ASSERT_TRUE(zonehvacClone.secondEvaporativeCooler());
    EXPECT_NE(secondEvaporativeCooler.handle(), zonehvacClone.secondEvaporativeCooler().get().handle());
    EXPECT_EQ(1u, m2.getConcreteModelObjects<FanSystemModel>().size());
    EXPECT_EQ(1u, m2.getConcreteModelObjects<EvaporativeCoolerDirectResearchSpecial>().size());
    EXPECT_EQ(1u, m2.getConcreteModelObjects<EvaporativeCoolerIndirectResearchSpecial>().size());
  }

  {
    auto zonehvacClone = zonehvac.clone(m).cast<ZoneHVACEvaporativeCoolerUnit>();
    EXPECT_FALSE(zonehvacClone.inletNode());
    EXPECT_FALSE(zonehvacClone.outletNode());
    EXPECT_FALSE(zonehvacClone.thermalZone());
    EXPECT_NE(fan.handle(), zonehvacClone.supplyAirFan().handle());
    EXPECT_NE(firstEvaporativeCooler.handle(), zonehvacClone.firstEvaporativeCooler().handle());
    ASSERT_TRUE(zonehvacClone.secondEvaporativeCooler());
    EXPECT_NE(secondEvaporativeCooler.handle(), zonehvacClone.secondEvaporativeCooler().get().handle());
    EXPECT_EQ(2u, m.getConcreteModelObjects<FanSystemModel>().size());
    EXPECT_EQ(2u, m.getConcreteModelObjects<EvaporativeCoolerDirectResearchSpecial>().size());
    EXPECT_EQ(2u, m.getConcreteModelObjects<EvaporativeCoolerIndirectResearchSpecial>().size());
  }
}

TEST_F(ModelFixture, ZoneHVACEvaporativeCoolerUnit_remove) {
  Model m;
  ZoneHVACEvaporativeCoolerUnit zonehvac(m);

  auto fanSystem = zonehvac.supplyAirFan();
  ASSERT_TRUE(fanSystem.optionalCast<FanSystemModel>());
  EXPECT_TRUE(fanSystem.containingZoneHVACComponent());
  EXPECT_EQ(zonehvac.handle(), fanSystem.containingZoneHVACComponent().get().handle());
  EXPECT_FALSE(fanSystem.isRemovable());

  auto evapCoolerComponent = zonehvac.firstEvaporativeCooler();
  ASSERT_TRUE(evapCoolerComponent.optionalCast<EvaporativeCoolerDirectResearchSpecial>());
  EXPECT_TRUE(evapCoolerComponent.containingZoneHVACComponent());
  EXPECT_EQ(zonehvac.handle(), evapCoolerComponent.containingZoneHVACComponent().get().handle());
  EXPECT_FALSE(evapCoolerComponent.isRemovable());

  auto size = m.modelObjects().size();
  EXPECT_EQ(1, m.getConcreteModelObjects<ZoneHVACEvaporativeCoolerUnit>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<FanSystemModel>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<EvaporativeCoolerDirectResearchSpecial>().size());
  EXPECT_EQ(0, m.getConcreteModelObjects<EvaporativeCoolerIndirectResearchSpecial>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<ScheduleConstant>().size());
  // remove returns: ["Zone HVAC Evaporative Cooler Unit 1", "Fan Component Model 1", "Evaporative Cooler Direct Research Special 1"]
  EXPECT_FALSE(zonehvac.remove().empty());
  EXPECT_EQ(size - 3, m.modelObjects().size());
  EXPECT_EQ(0, m.getConcreteModelObjects<ZoneHVACEvaporativeCoolerUnit>().size());
  EXPECT_EQ(0, m.getConcreteModelObjects<FanSystemModel>().size());
  EXPECT_EQ(0, m.getConcreteModelObjects<EvaporativeCoolerDirectResearchSpecial>().size());
  EXPECT_EQ(0, m.getConcreteModelObjects<EvaporativeCoolerIndirectResearchSpecial>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<ScheduleConstant>().size());
}

TEST_F(ModelFixture, ZoneHVACEvaporativeCoolerUnit_addToNode) {
  Model m;
  ZoneHVACEvaporativeCoolerUnit zonehvac(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(zonehvac.addToNode(supplyOutletNode));
  EXPECT_EQ(2, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(zonehvac.addToNode(inletNode));
  EXPECT_EQ(5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(zonehvac.addToNode(supplyOutletNode));
  EXPECT_EQ(5, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(zonehvac.addToNode(demandOutletNode));
  EXPECT_EQ(5, plantLoop.demandComponents().size());

  auto zonehvacClone = zonehvac.clone(m).cast<ZoneHVACEvaporativeCoolerUnit>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_FALSE(zonehvacClone.addToNode(supplyOutletNode));
  EXPECT_EQ(5, plantLoop.supplyComponents().size());
}
