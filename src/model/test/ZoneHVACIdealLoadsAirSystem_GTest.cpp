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

#include "../ZoneHVACIdealLoadsAirSystem.hpp"
#include "../ZoneHVACIdealLoadsAirSystem_Impl.hpp"
#include "../AirLoopHVACReturnPlenum.hpp"
#include "../AirLoopHVACReturnPlenum_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ScheduleCompact.hpp"
#include "../DesignSpecificationOutdoorAir.hpp"

#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneHVACIdealLoadsAirSystem_GettersSetters) {

  Model m;
  ZoneHVACIdealLoadsAirSystem zv_ideal(m);

  // Availability Schedule Name: Optional Object
  // No Default
  EXPECT_FALSE(zv_ideal.availabilitySchedule());
  ScheduleCompact sch(m);
  EXPECT_TRUE(zv_ideal.setAvailabilitySchedule(sch));
  ASSERT_TRUE(zv_ideal.availabilitySchedule());
  EXPECT_EQ(sch, zv_ideal.availabilitySchedule().get());

  // Zone Supply Air Node Name:  Object
  // Zone Exhaust Air Node Name: Optional Object

  // Maximum Heating Supply Air Temperature:  Double
  // Check Idd default: 50.0
  EXPECT_EQ(50.0, zv_ideal.maximumHeatingSupplyAirTemperature());
  EXPECT_TRUE(zv_ideal.setMaximumHeatingSupplyAirTemperature(25.0));
  EXPECT_EQ(25.0, zv_ideal.maximumHeatingSupplyAirTemperature());
  zv_ideal.resetMaximumHeatingSupplyAirTemperature();
  EXPECT_EQ(50.0, zv_ideal.maximumHeatingSupplyAirTemperature());

  // Minimum Cooling Supply Air Temperature:  Double
  // Check Idd default: 13.0
  EXPECT_EQ(13.0, zv_ideal.minimumCoolingSupplyAirTemperature());
  EXPECT_TRUE(zv_ideal.setMinimumCoolingSupplyAirTemperature(-43.5));
  EXPECT_EQ(-43.5, zv_ideal.minimumCoolingSupplyAirTemperature());
  zv_ideal.resetMinimumCoolingSupplyAirTemperature();
  EXPECT_EQ(13.0, zv_ideal.minimumCoolingSupplyAirTemperature());

  // Maximum Heating Supply Air Humidity Ratio:  Double
  // Check Idd default: 0.0156
  EXPECT_EQ(0.0156, zv_ideal.maximumHeatingSupplyAirHumidityRatio());
  EXPECT_TRUE(zv_ideal.setMaximumHeatingSupplyAirHumidityRatio(0.0078));
  EXPECT_EQ(0.0078, zv_ideal.maximumHeatingSupplyAirHumidityRatio());
  zv_ideal.resetMaximumHeatingSupplyAirHumidityRatio();
  EXPECT_EQ(0.0156, zv_ideal.maximumHeatingSupplyAirHumidityRatio());

  // Minimum Cooling Supply Air Humidity Ratio:  Double
  // Check Idd default: 0.0077
  EXPECT_EQ(0.0077, zv_ideal.minimumCoolingSupplyAirHumidityRatio());
  EXPECT_TRUE(zv_ideal.setMinimumCoolingSupplyAirHumidityRatio(0.00385));
  EXPECT_EQ(0.00385, zv_ideal.minimumCoolingSupplyAirHumidityRatio());
  zv_ideal.resetMinimumCoolingSupplyAirHumidityRatio();
  EXPECT_EQ(0.0077, zv_ideal.minimumCoolingSupplyAirHumidityRatio());

  // Heating Limit:  String
  // Check Idd default: "NoLimit"
  EXPECT_EQ("NoLimit", zv_ideal.heatingLimit());
  // Test a valid choice
  EXPECT_TRUE(zv_ideal.setHeatingLimit("LimitFlowRate"));
  EXPECT_EQ("LimitFlowRate", zv_ideal.heatingLimit());
  // Test an invalid choice
  EXPECT_FALSE(zv_ideal.setHeatingLimit("BadChoice"));
  EXPECT_EQ("LimitFlowRate", zv_ideal.heatingLimit());

  // Maximum Heating Air Flow Rate: Optional Double
  // No Default
  EXPECT_TRUE(zv_ideal.setMaximumHeatingAirFlowRate(1.0));
  ASSERT_TRUE(zv_ideal.maximumHeatingAirFlowRate());
  EXPECT_EQ(1.0, zv_ideal.maximumHeatingAirFlowRate().get());
  zv_ideal.resetMaximumHeatingAirFlowRate();
  EXPECT_FALSE(zv_ideal.maximumHeatingAirFlowRate());

  // Maximum Sensible Heating Capacity: Optional Double
  // No Default
  EXPECT_TRUE(zv_ideal.setMaximumSensibleHeatingCapacity(1.0));
  ASSERT_TRUE(zv_ideal.maximumSensibleHeatingCapacity());
  EXPECT_EQ(1.0, zv_ideal.maximumSensibleHeatingCapacity().get());
  zv_ideal.resetMaximumSensibleHeatingCapacity();
  EXPECT_FALSE(zv_ideal.maximumSensibleHeatingCapacity());

  // Cooling Limit:  String
  // Check Idd default: "NoLimit"
  EXPECT_EQ("NoLimit", zv_ideal.coolingLimit());
  // Test a valid choice
  EXPECT_TRUE(zv_ideal.setCoolingLimit("LimitFlowRate"));
  EXPECT_EQ("LimitFlowRate", zv_ideal.coolingLimit());
  // Test an invalid choice
  EXPECT_FALSE(zv_ideal.setCoolingLimit("BadChoice"));
  EXPECT_EQ("LimitFlowRate", zv_ideal.coolingLimit());

  // Maximum Cooling Air Flow Rate: Optional Double
  // No Default
  EXPECT_TRUE(zv_ideal.setMaximumCoolingAirFlowRate(1.0));
  ASSERT_TRUE(zv_ideal.maximumCoolingAirFlowRate());
  EXPECT_EQ(1.0, zv_ideal.maximumCoolingAirFlowRate().get());
  zv_ideal.resetMaximumCoolingAirFlowRate();
  EXPECT_FALSE(zv_ideal.maximumCoolingAirFlowRate());

  // Maximum Total Cooling Capacity: Optional Double
  // No Default
  EXPECT_TRUE(zv_ideal.setMaximumTotalCoolingCapacity(1.0));
  ASSERT_TRUE(zv_ideal.maximumTotalCoolingCapacity());
  EXPECT_EQ(1.0, zv_ideal.maximumTotalCoolingCapacity().get());
  zv_ideal.resetMaximumTotalCoolingCapacity();
  EXPECT_FALSE(zv_ideal.maximumTotalCoolingCapacity());

  // Heating Availability Schedule Name: Optional Object
  // No Default
  ScheduleCompact sch2(m);
  EXPECT_TRUE(zv_ideal.setHeatingAvailabilitySchedule(sch2));
  ASSERT_TRUE(zv_ideal.heatingAvailabilitySchedule());
  EXPECT_EQ(sch2, zv_ideal.heatingAvailabilitySchedule().get());

  // Cooling Availability Schedule Name: Optional Object
  // No Default
  ScheduleCompact sch3(m);
  EXPECT_TRUE(zv_ideal.setCoolingAvailabilitySchedule(sch3));
  ASSERT_TRUE(zv_ideal.coolingAvailabilitySchedule());
  EXPECT_EQ(sch3, zv_ideal.coolingAvailabilitySchedule().get());

  // Dehumidification Control Type:  String
  // Check Idd default: "ConstantSensibleHeatRatio"
  EXPECT_EQ("ConstantSensibleHeatRatio", zv_ideal.dehumidificationControlType());
  // Test a valid choice
  EXPECT_TRUE(zv_ideal.setDehumidificationControlType("Humidistat"));
  EXPECT_EQ("Humidistat", zv_ideal.dehumidificationControlType());
  // Test an invalid choice
  EXPECT_FALSE(zv_ideal.setDehumidificationControlType("BadChoice"));
  EXPECT_EQ("Humidistat", zv_ideal.dehumidificationControlType());

  // Cooling Sensible Heat Ratio:  Double
  // Check Idd default: 0.7
  EXPECT_EQ(0.7, zv_ideal.coolingSensibleHeatRatio());
  EXPECT_TRUE(zv_ideal.setCoolingSensibleHeatRatio(0.35));
  EXPECT_EQ(0.35, zv_ideal.coolingSensibleHeatRatio());
  zv_ideal.resetCoolingSensibleHeatRatio();
  EXPECT_EQ(0.7, zv_ideal.coolingSensibleHeatRatio());

  // Humidification Control Type:  String
  // Check Idd default: "None"
  EXPECT_EQ("None", zv_ideal.humidificationControlType());
  // Test a valid choice
  EXPECT_TRUE(zv_ideal.setHumidificationControlType("Humidistat"));
  EXPECT_EQ("Humidistat", zv_ideal.humidificationControlType());
  // Test an invalid choice
  EXPECT_FALSE(zv_ideal.setHumidificationControlType("BadChoice"));
  EXPECT_EQ("Humidistat", zv_ideal.humidificationControlType());

  // Design Specification Outdoor Air Object Name: Optional Object
  // No Default
  EXPECT_FALSE(zv_ideal.designSpecificationOutdoorAirObject());
  DesignSpecificationOutdoorAir dsoa(m);
  EXPECT_TRUE(zv_ideal.setDesignSpecificationOutdoorAirObject(dsoa));
  ASSERT_TRUE(zv_ideal.designSpecificationOutdoorAirObject());
  EXPECT_EQ(dsoa, zv_ideal.designSpecificationOutdoorAirObject().get());

  // Demand Controlled Ventilation Type:  String
  // Check Idd default: "None"
  EXPECT_EQ("None", zv_ideal.demandControlledVentilationType());
  // Test a valid choice
  EXPECT_TRUE(zv_ideal.setDemandControlledVentilationType("OccupancySchedule"));
  EXPECT_EQ("OccupancySchedule", zv_ideal.demandControlledVentilationType());
  // Test an invalid choice
  EXPECT_FALSE(zv_ideal.setDemandControlledVentilationType("BadChoice"));
  EXPECT_EQ("OccupancySchedule", zv_ideal.demandControlledVentilationType());

  // Outdoor Air Economizer Type:  String
  // Check Idd default: "NoEconomizer"
  EXPECT_EQ("NoEconomizer", zv_ideal.outdoorAirEconomizerType());
  // Test a valid choice
  EXPECT_TRUE(zv_ideal.setOutdoorAirEconomizerType("DifferentialDryBulb"));
  EXPECT_EQ("DifferentialDryBulb", zv_ideal.outdoorAirEconomizerType());
  // Test an invalid choice
  EXPECT_FALSE(zv_ideal.setOutdoorAirEconomizerType("BadChoice"));
  EXPECT_EQ("DifferentialDryBulb", zv_ideal.outdoorAirEconomizerType());

  // Heat Recovery Type:  String
  // Check Idd default: "None"
  EXPECT_EQ("None", zv_ideal.heatRecoveryType());
  // Test a valid choice
  EXPECT_TRUE(zv_ideal.setHeatRecoveryType("Sensible"));
  EXPECT_EQ("Sensible", zv_ideal.heatRecoveryType());
  // Test an invalid choice
  EXPECT_FALSE(zv_ideal.setHeatRecoveryType("BadChoice"));
  EXPECT_EQ("Sensible", zv_ideal.heatRecoveryType());

  // Sensible Heat Recovery Effectiveness:  Double
  // Check Idd default: 0.70
  EXPECT_EQ(0.70, zv_ideal.sensibleHeatRecoveryEffectiveness());
  EXPECT_TRUE(zv_ideal.setSensibleHeatRecoveryEffectiveness(0.35));
  EXPECT_EQ(0.35, zv_ideal.sensibleHeatRecoveryEffectiveness());
  zv_ideal.resetSensibleHeatRecoveryEffectiveness();
  EXPECT_EQ(0.70, zv_ideal.sensibleHeatRecoveryEffectiveness());

  // Latent Heat Recovery Effectiveness:  Double
  // Check Idd default: 0.65
  EXPECT_EQ(0.65, zv_ideal.latentHeatRecoveryEffectiveness());
  EXPECT_TRUE(zv_ideal.setLatentHeatRecoveryEffectiveness(0.325));
  EXPECT_EQ(0.325, zv_ideal.latentHeatRecoveryEffectiveness());
  zv_ideal.resetLatentHeatRecoveryEffectiveness();
  EXPECT_EQ(0.65, zv_ideal.latentHeatRecoveryEffectiveness());
}

TEST_F(ModelFixture, ZoneHVACIdealLoadsAirSystem_ReturnPlenum) {
  Model model;
  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(model);

  ThermalZone zone(model);
  ThermalZone plenumzone(model);

  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.addToThermalZone(zone));
  auto z = zoneHVACIdealLoadsAirSystem.thermalZone();
  ASSERT_TRUE(z);
  EXPECT_EQ(z.get(), zone);

  EXPECT_TRUE(zoneHVACIdealLoadsAirSystem.setReturnPlenum(plenumzone));
  auto p = zoneHVACIdealLoadsAirSystem.returnPlenum();
  ASSERT_TRUE(p);

  auto pz = p->thermalZone();
  ASSERT_TRUE(pz);
  EXPECT_EQ(pz.get(), plenumzone);
}
