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

#include "../Model.hpp"
#include "../FanConstantVolume.hpp"
#include "../FanConstantVolume_Impl.hpp"
#include "../AirTerminalSingleDuctInletSideMixer.hpp"
#include "../AirTerminalSingleDuctInletSideMixer_Impl.hpp"
#include "../CoilCoolingWater.hpp"
#include "../CoilCoolingWater_Impl.hpp"
#include "../CoilHeatingWater.hpp"
#include "../CoilHeatingWater_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ZoneHVACFourPipeFanCoil.hpp"
#include "../ZoneHVACFourPipeFanCoil_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../ScheduleCompact.hpp"

#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneHVACFourPipeFanCoil_WrongOrderConstructor) {
  model::Model m;
  model::Schedule availabilitySchedule = m.alwaysOnDiscreteSchedule();
  model::FanConstantVolume fan(m, availabilitySchedule);
  model::CoilHeatingWater heatingCoil(m, availabilitySchedule);
  CoilCoolingWater coolingCoil(m, availabilitySchedule);

  // Takes in cooling coil THEN heating coil
  ASSERT_THROW(model::ZoneHVACFourPipeFanCoil fpfc(m, availabilitySchedule, fan, heatingCoil, coolingCoil), openstudio::Exception);
}

TEST_F(ModelFixture, ZoneHVACFourPipeFanCoil_GettersSetters) {
  model::Model m;
  model::Schedule availabilitySchedule = m.alwaysOnDiscreteSchedule();
  model::FanConstantVolume fan(m, availabilitySchedule);
  model::CoilHeatingWater heatingCoil(m, availabilitySchedule);
  CoilCoolingWater coolingCoil(m, availabilitySchedule);

  model::ZoneHVACFourPipeFanCoil fpfc(m, availabilitySchedule, fan, coolingCoil, heatingCoil);

  // Availability Schedule Name:  Object
  EXPECT_EQ(availabilitySchedule, fpfc.availabilitySchedule());
  ScheduleCompact sch(m);
  EXPECT_TRUE(fpfc.setAvailabilitySchedule(sch));
  EXPECT_EQ(sch, fpfc.availabilitySchedule());

  // Capacity Control Method:  String
  // No Default
  // Test a valid choice
  EXPECT_TRUE(fpfc.setCapacityControlMethod("ConstantFanVariableFlow"));
  EXPECT_EQ("ConstantFanVariableFlow", fpfc.capacityControlMethod());
  // Test an invalid choice
  EXPECT_FALSE(fpfc.setCapacityControlMethod("BadChoice"));
  EXPECT_EQ("ConstantFanVariableFlow", fpfc.capacityControlMethod());

  // Maximum Supply Air Flow Rate: Optional Double
  // No Default
  EXPECT_TRUE(fpfc.setMaximumSupplyAirFlowRate(10.03));
  ASSERT_TRUE(fpfc.maximumSupplyAirFlowRate());
  EXPECT_EQ(10.03, fpfc.maximumSupplyAirFlowRate().get());

  // Low Speed Supply Air Flow Ratio:  Double
  // Check Idd default: 0.33
  EXPECT_EQ(0.33, fpfc.lowSpeedSupplyAirFlowRatio());
  EXPECT_TRUE(fpfc.setLowSpeedSupplyAirFlowRatio(0.165));
  EXPECT_EQ(0.165, fpfc.lowSpeedSupplyAirFlowRatio());
  fpfc.resetLowSpeedSupplyAirFlowRatio();
  EXPECT_EQ(0.33, fpfc.lowSpeedSupplyAirFlowRatio());

  // Medium Speed Supply Air Flow Ratio:  Double
  // Check Idd default: 0.66
  EXPECT_EQ(0.66, fpfc.mediumSpeedSupplyAirFlowRatio());
  EXPECT_TRUE(fpfc.setMediumSpeedSupplyAirFlowRatio(0.33));
  EXPECT_EQ(0.33, fpfc.mediumSpeedSupplyAirFlowRatio());
  fpfc.resetMediumSpeedSupplyAirFlowRatio();
  EXPECT_EQ(0.66, fpfc.mediumSpeedSupplyAirFlowRatio());

  // Maximum Outdoor Air Flow Rate: Optional Double
  // No Default
  EXPECT_TRUE(fpfc.setMaximumOutdoorAirFlowRate(10.03));
  ASSERT_TRUE(fpfc.maximumOutdoorAirFlowRate());
  EXPECT_EQ(10.03, fpfc.maximumOutdoorAirFlowRate().get());

  // Outdoor Air Schedule Name: Optional Object
  // No Default
  EXPECT_FALSE(fpfc.outdoorAirSchedule());
  ScheduleCompact sch_oa(m);
  EXPECT_TRUE(fpfc.setOutdoorAirSchedule(sch_oa));
  ASSERT_TRUE(fpfc.outdoorAirSchedule());
  EXPECT_EQ(sch_oa, fpfc.outdoorAirSchedule().get());

  // Air Inlet Node Name:  Object
  // Air Outlet Node Name:  Object

  // Outdoor Air Mixer Object Type:  String
  // No Default
  // Test a valid choice
  EXPECT_TRUE(fpfc.setOutdoorAirMixerObjectType("OutdoorAir:Mixer"));
  EXPECT_EQ("OutdoorAir:Mixer", fpfc.outdoorAirMixerObjectType());
  // Test an invalid choice
  EXPECT_FALSE(fpfc.setOutdoorAirMixerObjectType("BadChoice"));
  EXPECT_EQ("OutdoorAir:Mixer", fpfc.outdoorAirMixerObjectType());

  // Outdoor Air Mixer Name:  Object

  // Supply Air Fan Name:  Object
  EXPECT_EQ(fan, fpfc.supplyAirFan());
  FanConstantVolume fan2(m);
  EXPECT_TRUE(fpfc.setSupplyAirFan(fan2));
  EXPECT_EQ(fan2, fpfc.supplyAirFan());

  // Cooling Coil Name:  Object
  // No Default
  EXPECT_EQ(coolingCoil, fpfc.coolingCoil());
  CoilCoolingWater coolingCoil2(m);
  EXPECT_TRUE(fpfc.setCoolingCoil(coolingCoil2));
  EXPECT_EQ(coolingCoil2.handle(), fpfc.coolingCoil().handle());

  // Maximum Cold Water Flow Rate: Optional Double
  // No Default
  EXPECT_TRUE(fpfc.setMaximumColdWaterFlowRate(10.03));
  ASSERT_TRUE(fpfc.maximumColdWaterFlowRate());
  EXPECT_EQ(10.03, fpfc.maximumColdWaterFlowRate().get());

  // Minimum Cold Water Flow Rate:  Double
  // Check Idd default: 0.0
  EXPECT_EQ(0.0, fpfc.minimumColdWaterFlowRate());
  EXPECT_TRUE(fpfc.setMinimumColdWaterFlowRate(-1.0));
  EXPECT_EQ(-1.0, fpfc.minimumColdWaterFlowRate());
  fpfc.resetMinimumColdWaterFlowRate();
  EXPECT_EQ(0.0, fpfc.minimumColdWaterFlowRate());

  // Cooling Convergence Tolerance:  Double
  // Check Idd default: 0.001
  EXPECT_EQ(0.001, fpfc.coolingConvergenceTolerance());
  EXPECT_TRUE(fpfc.setCoolingConvergenceTolerance(0.0005));
  EXPECT_EQ(0.0005, fpfc.coolingConvergenceTolerance());
  fpfc.resetCoolingConvergenceTolerance();
  EXPECT_EQ(0.001, fpfc.coolingConvergenceTolerance());

  // Heating Coil Name:  Object
  // No Default
  EXPECT_EQ(heatingCoil, fpfc.heatingCoil());
  CoilHeatingWater heatingCoil2(m);
  EXPECT_TRUE(fpfc.setHeatingCoil(heatingCoil2));
  EXPECT_EQ(heatingCoil2, fpfc.heatingCoil());

  // Maximum Hot Water Flow Rate: Optional Double
  // No Default
  EXPECT_TRUE(fpfc.setMaximumHotWaterFlowRate(10.03));
  ASSERT_TRUE(fpfc.maximumHotWaterFlowRate());
  EXPECT_EQ(10.03, fpfc.maximumHotWaterFlowRate().get());

  // Minimum Hot Water Flow Rate:  Double
  // Check Idd default: 0.0
  EXPECT_EQ(0.0, fpfc.minimumHotWaterFlowRate());
  EXPECT_TRUE(fpfc.setMinimumHotWaterFlowRate(-1.0));
  EXPECT_EQ(-1.0, fpfc.minimumHotWaterFlowRate());
  fpfc.resetMinimumHotWaterFlowRate();
  EXPECT_EQ(0.0, fpfc.minimumHotWaterFlowRate());

  // Heating Convergence Tolerance:  Double
  // Check Idd default: 0.001
  EXPECT_EQ(0.001, fpfc.heatingConvergenceTolerance());
  EXPECT_TRUE(fpfc.setHeatingConvergenceTolerance(0.0005));
  EXPECT_EQ(0.0005, fpfc.heatingConvergenceTolerance());
  fpfc.resetHeatingConvergenceTolerance();
  EXPECT_EQ(0.001, fpfc.heatingConvergenceTolerance());

  // Supply Air Fan Operating Mode Schedule Name: Optional Object
  // No Default
  EXPECT_FALSE(fpfc.supplyAirFanOperatingModeSchedule());
  ScheduleCompact sch_op(m);
  EXPECT_TRUE(fpfc.setSupplyAirFanOperatingModeSchedule(sch_op));
  ASSERT_TRUE(fpfc.supplyAirFanOperatingModeSchedule());
  EXPECT_EQ(sch_op, fpfc.supplyAirFanOperatingModeSchedule().get());

  // Minimum Supply Air Temperature in Cooling Mode: Optional Double
  // No Default
  EXPECT_TRUE(fpfc.setMinimumSupplyAirTemperatureInCoolingMode(1.0));
  ASSERT_TRUE(fpfc.minimumSupplyAirTemperatureInCoolingMode());
  EXPECT_EQ(1.0, fpfc.minimumSupplyAirTemperatureInCoolingMode().get());

  // Maximum Supply Air Temperature in Heating Mode: Optional Double
  // No Default
  EXPECT_TRUE(fpfc.setMaximumSupplyAirTemperatureInHeatingMode(1.0));
  ASSERT_TRUE(fpfc.maximumSupplyAirTemperatureInHeatingMode());
  EXPECT_EQ(1.0, fpfc.maximumSupplyAirTemperatureInHeatingMode().get());
}

TEST_F(ModelFixture, ZoneHVACFourPipeFanCoil_addToThermalZone) {
  Model model;

  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u, model.objects().size());
  sched.setValue(1.0);  // Always on

  // need fan
  FanConstantVolume fan(model, sched);

  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model, sched);
  CoilHeatingWater heatingCoil(model, sched);

  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model, sched, fan, coolingCoil, heatingCoil);

  // Is it hooked up to the inside components?
  EXPECT_EQ(fan.handle(), zoneHVACFourPipeFanCoil.supplyAirFan().handle());
  EXPECT_EQ(coolingCoil.handle(), zoneHVACFourPipeFanCoil.coolingCoil().handle());
  EXPECT_EQ(heatingCoil.handle(), zoneHVACFourPipeFanCoil.heatingCoil().handle());

  // construct a ThermalZone

  ThermalZone thermalZone(model);

  EXPECT_EQ(0u, thermalZone.equipment().size());

  EXPECT_TRUE(zoneHVACFourPipeFanCoil.addToThermalZone(thermalZone));

  EXPECT_TRUE(zoneHVACFourPipeFanCoil.inletNode());
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.outletNode());

  EXPECT_EQ(1u, thermalZone.equipment().size());

  EXPECT_TRUE(fan.containingZoneHVACComponent());
  EXPECT_TRUE(coolingCoil.containingZoneHVACComponent());
  EXPECT_TRUE(heatingCoil.containingZoneHVACComponent());
}

TEST_F(ModelFixture, ZoneHVACFourPipeFanCoil_AddRemoveAirLoopHVAC) {
  model::Model m;
  model::Schedule availabilitySchedule = m.alwaysOnDiscreteSchedule();
  model::FanConstantVolume fan(m, availabilitySchedule);
  model::CoilHeatingWater heatingCoil(m, availabilitySchedule);
  CoilCoolingWater coolingCoil(m, availabilitySchedule);

  model::ZoneHVACFourPipeFanCoil fpfc(m, availabilitySchedule, fan, coolingCoil, heatingCoil);

  AirLoopHVAC airLoop(m);
  ThermalZone thermalZone(m);
  AirTerminalSingleDuctInletSideMixer terminal(m);
  EXPECT_TRUE(airLoop.addBranchForZone(thermalZone, terminal));
  ASSERT_EQ(9u, airLoop.demandComponents().size());
  ASSERT_FALSE(terminal.secondaryAirInletNode());

  boost::optional<ModelObject> outletModelObject = terminal.outletModelObject();
  ASSERT_TRUE(outletModelObject);
  boost::optional<Node> outletNode = outletModelObject->optionalCast<Node>();
  ASSERT_TRUE(outletNode);

  ASSERT_TRUE(fpfc.addToNode(outletNode.get()));
  ASSERT_EQ(11u, airLoop.demandComponents().size());
  ASSERT_TRUE(fpfc.airLoopHVAC());
  ASSERT_TRUE(fpfc.thermalZone());
  ASSERT_TRUE(terminal.secondaryAirInletNode());
  ASSERT_EQ(thermalZone, fpfc.thermalZone().get());
  ASSERT_EQ(2u, thermalZone.equipment().size());
  ASSERT_EQ(airLoop, fpfc.airLoopHVAC().get());

  fpfc.removeFromThermalZone();
  ASSERT_FALSE(fpfc.airLoopHVAC());
  ASSERT_FALSE(fpfc.thermalZone());
  ASSERT_EQ(1u, thermalZone.equipment().size());
  ASSERT_EQ(9u, airLoop.demandComponents().size());

  // Demonstrate that when you remove the terminal,
  // the ZoneHVAC stays attached to the zone.
  outletModelObject = terminal.outletModelObject();
  ASSERT_TRUE(outletModelObject);
  outletNode = outletModelObject->optionalCast<Node>();
  ASSERT_TRUE(outletNode);
  ASSERT_TRUE(fpfc.addToNode(outletNode.get()));
  ASSERT_EQ(11u, airLoop.demandComponents().size());
  terminal.remove();

  ASSERT_EQ(7u, airLoop.demandComponents().size());
  ASSERT_EQ(thermalZone, fpfc.thermalZone().get());
  ASSERT_EQ(1u, thermalZone.equipment().size());
}
