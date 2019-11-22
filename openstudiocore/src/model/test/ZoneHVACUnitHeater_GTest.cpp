/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../FanConstantVolume.hpp"
#include "../FanConstantVolume_Impl.hpp"
#include "../FanVariableVolume.hpp"
#include "../FanVariableVolume_Impl.hpp"
#include "../CoilHeatingWater.hpp"
#include "../CoilHeatingWater_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ZoneHVACUnitHeater.hpp"
#include "../ZoneHVACUnitHeater_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"
#include "../LifeCycleCost.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;


TEST_F(ModelFixture,ZoneHVACUnitHeater_Check_Constructor) {

  Model model;
  ScheduleConstant sched(model);
  sched.setValue(1.0); // Always on
  FanConstantVolume fan(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  ZoneHVACUnitHeater zoneHVACUnitHeater(model,sched,fan,heatingCoil);

  // Testing .idd object type

  EXPECT_EQ(openstudio::IddObjectType::OS_ZoneHVAC_UnitHeater,zoneHVACUnitHeater.iddObjectType().value());

  // Test set and get availability schedule

  ScheduleConstant test_sched(model);
  test_sched.setValue(1.0);
  EXPECT_TRUE(zoneHVACUnitHeater.setAvailabilitySchedule(test_sched));
  EXPECT_EQ(zoneHVACUnitHeater.availabilitySchedule(),test_sched);

}

// Test add to thermal zone

TEST_F(ModelFixture,ZoneHVACUnitHeater_addToThermalZone) {

  Model model;
  ScheduleConstant sched(model);
  sched.setValue(1.0); // Always on
  FanConstantVolume fan(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  ZoneHVACUnitHeater zoneHVACUnitHeater(model,sched,fan,heatingCoil);

  //test add to and remove from Thermal zone

  ThermalZone thermalZone(model);
  EXPECT_TRUE(zoneHVACUnitHeater.addToThermalZone(thermalZone));
  boost::optional<ThermalZone> testThermalZone = zoneHVACUnitHeater.thermalZone();
  EXPECT_EQ(*(testThermalZone),zoneHVACUnitHeater.thermalZone());

  // Check inlet and outlet nodes
  EXPECT_TRUE(zoneHVACUnitHeater.inletNode());
  EXPECT_TRUE(zoneHVACUnitHeater.outletNode());

  zoneHVACUnitHeater.removeFromThermalZone();
  EXPECT_FALSE(zoneHVACUnitHeater.thermalZone());

}
  //Test set and get supply air fan

TEST_F(ModelFixture,ZoneHVACUnitHeater_Check_Supply_Air_Fan) {

  Model model;
  ScheduleConstant sched(model);
  sched.setValue(1.0); // Always on
  FanConstantVolume fan(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  ZoneHVACUnitHeater zoneHVACUnitHeater(model,sched,fan,heatingCoil);
  FanVariableVolume testfan(model,sched);
  EXPECT_TRUE(zoneHVACUnitHeater.setSupplyAirFan(testfan));
  EXPECT_EQ(zoneHVACUnitHeater.supplyAirFan(),testfan);

  // Test set and get max supply air flow rate, and test fan autosize functions

  EXPECT_TRUE(zoneHVACUnitHeater.setMaximumSupplyAirFlowRate(500));
  boost::optional<double> testmaxSAFR = zoneHVACUnitHeater.maximumSupplyAirFlowRate();
  EXPECT_EQ((*testmaxSAFR),500);


  EXPECT_FALSE(zoneHVACUnitHeater.isMaximumSupplyAirFlowRateAutosized());
  zoneHVACUnitHeater.autosizeMaximumSupplyAirFlowRate();
  EXPECT_TRUE(zoneHVACUnitHeater.isMaximumSupplyAirFlowRateAutosized());

  // Test set and get fan control type

  EXPECT_FALSE(zoneHVACUnitHeater.setFanControlType("wrong fan"));
  EXPECT_TRUE(zoneHVACUnitHeater.setFanControlType("OnOff"));
  EXPECT_TRUE(zoneHVACUnitHeater.setFanControlType("Continuous"));
  EXPECT_EQ(zoneHVACUnitHeater.fanControlType(),"Continuous");

}

// Test set and get heating coil

TEST_F(ModelFixture,ZoneHVACUnitHeater_Check_Heating_Coil) {

  Model model;
  ScheduleConstant sched(model);
  sched.setValue(1.0); // Always on
  FanConstantVolume fan(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  ZoneHVACUnitHeater zoneHVACUnitHeater(model,sched,fan,heatingCoil);

  CoilHeatingWater testheatingCoil(model,sched);
  EXPECT_TRUE(zoneHVACUnitHeater.setHeatingCoil(testheatingCoil));
  EXPECT_EQ(zoneHVACUnitHeater.heatingCoil(),testheatingCoil);

  // Test set and get max hot water flow rate, and test max hot water flow autosize functions

  EXPECT_TRUE(zoneHVACUnitHeater.setMaximumHotWaterFlowRate(500));
  boost::optional<double> testmaxHWMFR = zoneHVACUnitHeater.maximumHotWaterFlowRate();
  EXPECT_EQ((*testmaxHWMFR),500);

  EXPECT_FALSE(zoneHVACUnitHeater.isMaximumHotWaterFlowRateAutosized());
  zoneHVACUnitHeater.resetMaximumHotWaterFlowRate();
  EXPECT_FALSE(zoneHVACUnitHeater.maximumHotWaterFlowRate());
  zoneHVACUnitHeater.autosizeMaximumHotWaterFlowRate();
  EXPECT_TRUE(zoneHVACUnitHeater.isMaximumHotWaterFlowRateAutosized());

  // Test set and get min hot water flow rate, and test min hot water flow autosize functions

  EXPECT_TRUE(zoneHVACUnitHeater.setMinimumHotWaterFlowRate(500));
  boost::optional<double> testminHWMFR = zoneHVACUnitHeater.minimumHotWaterFlowRate();
  EXPECT_EQ((*testminHWMFR),500);

  EXPECT_FALSE(zoneHVACUnitHeater.isMinimumHotWaterFlowRateDefaulted());
  zoneHVACUnitHeater.resetMinimumHotWaterFlowRate();
  EXPECT_FALSE(zoneHVACUnitHeater.minimumHotWaterFlowRate());
  EXPECT_TRUE(zoneHVACUnitHeater.isMinimumHotWaterFlowRateDefaulted());

  // Test set and get min hot water flow rate, and test min hot water flow autosize functions

  EXPECT_TRUE(zoneHVACUnitHeater.setHeatingConvergenceTolerance(0.002));
  boost::optional<double> testHCT = zoneHVACUnitHeater.heatingConvergenceTolerance();
  EXPECT_EQ((*testHCT),0.002);

  EXPECT_FALSE(zoneHVACUnitHeater.isHeatingConvergenceToleranceDefaulted());
  zoneHVACUnitHeater.resetHeatingConvergenceTolerance();
  EXPECT_EQ(zoneHVACUnitHeater.heatingConvergenceTolerance(),0.001);
  EXPECT_TRUE(zoneHVACUnitHeater.isHeatingConvergenceToleranceDefaulted());
}

// Test add Life Cycle Costs

TEST_F(ModelFixture,ZoneHVACUnitHeater_addLifeCycleCosts) {

  Model model;
  ScheduleConstant sched(model);
  sched.setValue(1.0); // Always on
  FanConstantVolume fan(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  ZoneHVACUnitHeater zoneHVACUnitHeater(model,sched,fan,heatingCoil);

  boost::optional<openstudio::model::LifeCycleCost> cost1 = openstudio::model::LifeCycleCost::createLifeCycleCost("Install", zoneHVACUnitHeater, 1000.0, "CostPerEach", "Construction");
  ASSERT_TRUE(cost1);

  EXPECT_DOUBLE_EQ(1000.0, cost1->totalCost());
}
