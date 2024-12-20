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

// TODO: Check the following class names against object getters and setters.
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"

#include "../SystemAvailabilityManagerLists.hpp"
#include "../SystemAvailabilityManagerLists_Impl.hpp"

#include "../Connection.hpp"
#include "../Connection_Impl.hpp"

#include "../Fans.hpp"
#include "../Fans_Impl.hpp"

#include "../EvapCooler.hpp"
#include "../EvapCooler_Impl.hpp"

#include "../DesignSpecificationZoneHVACSizingName.hpp"
#include "../DesignSpecificationZoneHVACSizingName_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneHVACEvaporativeCoolerUnit_GettersSetters) {
  Model m;
  // TODO: Check regular Ctor arguments
  ZoneHVACEvaporativeCoolerUnit zoneHVACEvaporativeCoolerUnit(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // ZoneHVACEvaporativeCoolerUnit zoneHVACEvaporativeCoolerUnit = m.getUniqueModelObject<ZoneHVACEvaporativeCoolerUnit>();

  zoneHVACEvaporativeCoolerUnit.setName("My ZoneHVACEvaporativeCoolerUnit");

  // Availability Schedule Name: Required Object
  Schedule availabilitySchedule(m);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setAvailabilitySchedule(availabilitySchedule));
  EXPECT_EQ(availabilitySchedule, zoneHVACEvaporativeCoolerUnit.availabilitySchedule());

  // Availability Manager List Name: Optional Object
  boost::optional<SystemAvailabilityManagerLists> availabilityManagerList(m);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setAvailabilityManagerList(availabilityManagerList));
  ASSERT_TRUE(zoneHVACEvaporativeCoolerUnit.availabilityManagerList());
  EXPECT_EQ(availabilityManagerList, zoneHVACEvaporativeCoolerUnit.availabilityManagerList().get());

  // Outdoor Air Inlet Node Name: Required Object
  Connection outdoorAirInletNode(m);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setOutdoorAirInletNode(outdoorAirInletNode));
  EXPECT_EQ(outdoorAirInletNode, zoneHVACEvaporativeCoolerUnit.outdoorAirInletNode());

  // Cooler Outlet Node Name: Required Object
  Connection coolerOutletNode(m);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setCoolerOutletNode(coolerOutletNode));
  EXPECT_EQ(coolerOutletNode, zoneHVACEvaporativeCoolerUnit.coolerOutletNode());

  // Zone Relief Air Node Name: Optional Object
  boost::optional<Connection> zoneReliefAirNode(m);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setZoneReliefAirNode(zoneReliefAirNode));
  ASSERT_TRUE(zoneHVACEvaporativeCoolerUnit.zoneReliefAirNode());
  EXPECT_EQ(zoneReliefAirNode, zoneHVACEvaporativeCoolerUnit.zoneReliefAirNode().get());

  // Supply Air Fan Name: Required Object
  Fans supplyAirFan(m);
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
  EvapCooler firstEvaporativeCooler(m);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setFirstEvaporativeCooler(firstEvaporativeCooler));
  EXPECT_EQ(firstEvaporativeCooler, zoneHVACEvaporativeCoolerUnit.firstEvaporativeCooler());

  // Second Evaporative Cooler Name: Optional Object
  boost::optional<EvapCooler> secondEvaporativeCooler(m);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setSecondEvaporativeCooler(secondEvaporativeCooler));
  ASSERT_TRUE(zoneHVACEvaporativeCoolerUnit.secondEvaporativeCooler());
  EXPECT_EQ(secondEvaporativeCooler, zoneHVACEvaporativeCoolerUnit.secondEvaporativeCooler().get());

  // Design Specification ZoneHVAC Sizing: Optional Object
  boost::optional<DesignSpecificationZoneHVACSizingName> designSpecificationZoneHVACSizing(m);
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setDesignSpecificationZoneHVACSizing(designSpecificationZoneHVACSizing));
  ASSERT_TRUE(zoneHVACEvaporativeCoolerUnit.designSpecificationZoneHVACSizing());
  EXPECT_EQ(designSpecificationZoneHVACSizing, zoneHVACEvaporativeCoolerUnit.designSpecificationZoneHVACSizing().get());

  // Shut Off Relative Humidity: Required Double
  EXPECT_TRUE(zoneHVACEvaporativeCoolerUnit.setShutOffRelativeHumidity(94.444));
  EXPECT_EQ(94.444, zoneHVACEvaporativeCoolerUnit.shutOffRelativeHumidity());
  // Bad Value
  EXPECT_FALSE(zoneHVACEvaporativeCoolerUnit.setShutOffRelativeHumidity(-10.0));
  EXPECT_EQ(94.444, zoneHVACEvaporativeCoolerUnit.shutOffRelativeHumidity());
}
TEST_F(ModelFixture, ZoneHVACEvaporativeCoolerUnit_HeatCoolFuelTypes) {
  Model m;
  // TODO: Check regular Ctor arguments
  ZoneHVACEvaporativeCoolerUnit zoneHVACEvaporativeCoolerUnit(m);
  // TODO: Or if a UniqueModelObject (and make sure _Impl is included)
  // ZoneHVACEvaporativeCoolerUnit zoneHVACEvaporativeCoolerUnit = m.getUniqueModelObject<ZoneHVACEvaporativeCoolerUnit>();

  EXPECT_EQ(ComponentType(ComponentType::Both), zoneHVACEvaporativeCoolerUnit.componentType());
  testFuelTypeEquality({FuelType::Electricity}, zoneHVACEvaporativeCoolerUnit.coolingFuelTypes());
  testFuelTypeEquality({FuelType::Electricity, FuelType::Propane}, zoneHVACEvaporativeCoolerUnit.heatingFuelTypes());
  testAppGFuelTypeEquality({AppGFuelType::Fuel, AppGFuelType::HeatPump}, zoneHVACEvaporativeCoolerUnit.appGHeatingFuelTypes());
}
