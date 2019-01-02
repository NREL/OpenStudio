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

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_MaximumSupplyAirFlowRate_Quantity) {
  Model model;
  EXPECT_EQ(0u,model.objects().size());
  EXPECT_TRUE(model.versionObject());
  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u,model.objects().size());
  sched.setValue(1.0); // Always on
  // need fan
  FanConstantVolume fan(model,sched);
  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model,sched,fan,coolingCoil,heatingCoil);
  EXPECT_EQ(6u,model.objects().size());

  Unit units = zoneHVACFourPipeFanCoil.getMaximumSupplyAirFlowRate(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.setMaximumSupplyAirFlowRate(testQ));
  OSOptionalQuantity q = zoneHVACFourPipeFanCoil.getMaximumSupplyAirFlowRate(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_LowSpeedSupplyAirFlowRatio_Quantity) {
  Model model;
  EXPECT_EQ(0u,model.objects().size());
  EXPECT_TRUE(model.versionObject());
  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u,model.objects().size());
  sched.setValue(1.0); // Always on
  // need fan
  FanConstantVolume fan(model,sched);
  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model,sched,fan,coolingCoil,heatingCoil);
  EXPECT_EQ(6u,model.objects().size());

  Unit units = zoneHVACFourPipeFanCoil.getLowSpeedSupplyAirFlowRatio(true).units(); // Get IP units.
  double value(0.33);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.setLowSpeedSupplyAirFlowRatio(testQ));
  Quantity q = zoneHVACFourPipeFanCoil.getLowSpeedSupplyAirFlowRatio(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_MediumSpeedSupplyAirFlowRatio_Quantity) {
  Model model;
  EXPECT_EQ(0u,model.objects().size());
  EXPECT_TRUE(model.versionObject());
  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u,model.objects().size());
  sched.setValue(1.0); // Always on
  // need fan
  FanConstantVolume fan(model,sched);
  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model,sched,fan,coolingCoil,heatingCoil);
  EXPECT_EQ(6u,model.objects().size());

  Unit units = zoneHVACFourPipeFanCoil.getMediumSpeedSupplyAirFlowRatio(true).units(); // Get IP units.
  double value(0.66);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.setMediumSpeedSupplyAirFlowRatio(testQ));
  Quantity q = zoneHVACFourPipeFanCoil.getMediumSpeedSupplyAirFlowRatio(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_MaximumOutdoorAirFlowRate_Quantity) {
  Model model;
  EXPECT_EQ(0u,model.objects().size());
  EXPECT_TRUE(model.versionObject());
  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u,model.objects().size());
  sched.setValue(1.0); // Always on
  // need fan
  FanConstantVolume fan(model,sched);
  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model,sched,fan,coolingCoil,heatingCoil);
  EXPECT_EQ(6u,model.objects().size());

  Unit units = zoneHVACFourPipeFanCoil.getMaximumOutdoorAirFlowRate(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.setMaximumOutdoorAirFlowRate(testQ));
  OSOptionalQuantity q = zoneHVACFourPipeFanCoil.getMaximumOutdoorAirFlowRate(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_MaximumColdWaterFlowRate_Quantity) {
  Model model;
  EXPECT_EQ(0u,model.objects().size());
  EXPECT_TRUE(model.versionObject());
  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u,model.objects().size());
  sched.setValue(1.0); // Always on
  // need fan
  FanConstantVolume fan(model,sched);
  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model,sched,fan,coolingCoil,heatingCoil);
  EXPECT_EQ(6u,model.objects().size());

  Unit units = zoneHVACFourPipeFanCoil.getMaximumColdWaterFlowRate(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.setMaximumColdWaterFlowRate(testQ));
  OSOptionalQuantity q = zoneHVACFourPipeFanCoil.getMaximumColdWaterFlowRate(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_MinimumColdWaterFlowRate_Quantity) {
  Model model;
  EXPECT_EQ(0u,model.objects().size());
  EXPECT_TRUE(model.versionObject());
  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u,model.objects().size());
  sched.setValue(1.0); // Always on
  // need fan
  FanConstantVolume fan(model,sched);
  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model,sched,fan,coolingCoil,heatingCoil);
  EXPECT_EQ(6u,model.objects().size());

  Unit units = zoneHVACFourPipeFanCoil.getMinimumColdWaterFlowRate(true).units(); // Get IP units.
  double value(0.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.setMinimumColdWaterFlowRate(testQ));
  Quantity q = zoneHVACFourPipeFanCoil.getMinimumColdWaterFlowRate(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_CoolingConvergenceTolerance_Quantity) {
  Model model;
  EXPECT_EQ(0u,model.objects().size());
  EXPECT_TRUE(model.versionObject());
  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u,model.objects().size());
  sched.setValue(1.0); // Always on
  // need fan
  FanConstantVolume fan(model,sched);
  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model,sched,fan,coolingCoil,heatingCoil);
  EXPECT_EQ(6u,model.objects().size());

  Unit units = zoneHVACFourPipeFanCoil.getCoolingConvergenceTolerance(true).units(); // Get IP units.
  double value(0.001);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.setCoolingConvergenceTolerance(testQ));
  Quantity q = zoneHVACFourPipeFanCoil.getCoolingConvergenceTolerance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_MaximumHotWaterFlowRate_Quantity) {
  Model model;
  EXPECT_EQ(0u,model.objects().size());
  EXPECT_TRUE(model.versionObject());
  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u,model.objects().size());
  sched.setValue(1.0); // Always on
  // need fan
  FanConstantVolume fan(model,sched);
  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model,sched,fan,coolingCoil,heatingCoil);
  EXPECT_EQ(6u,model.objects().size());

  Unit units = zoneHVACFourPipeFanCoil.getMaximumHotWaterFlowRate(true).units(); // Get IP units.
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.setMaximumHotWaterFlowRate(testQ));
  OSOptionalQuantity q = zoneHVACFourPipeFanCoil.getMaximumHotWaterFlowRate(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_MinimumHotWaterFlowRate_Quantity) {
  Model model;
  EXPECT_EQ(0u,model.objects().size());
  EXPECT_TRUE(model.versionObject());
  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u,model.objects().size());
  sched.setValue(1.0); // Always on
  // need fan
  FanConstantVolume fan(model,sched);
  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model,sched,fan,coolingCoil,heatingCoil);
  EXPECT_EQ(6u,model.objects().size());

  Unit units = zoneHVACFourPipeFanCoil.getMinimumHotWaterFlowRate(true).units(); // Get IP units.
  double value(0.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.setMinimumHotWaterFlowRate(testQ));
  Quantity q = zoneHVACFourPipeFanCoil.getMinimumHotWaterFlowRate(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_HeatingConvergenceTolerance_Quantity) {
  Model model;
  EXPECT_EQ(0u,model.objects().size());
  EXPECT_TRUE(model.versionObject());
  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u,model.objects().size());
  sched.setValue(1.0); // Always on
  // need fan
  FanConstantVolume fan(model,sched);
  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model,sched);
  CoilHeatingWater heatingCoil(model,sched);
  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model,sched,fan,coolingCoil,heatingCoil);
  EXPECT_EQ(6u,model.objects().size());

  Unit units = zoneHVACFourPipeFanCoil.getHeatingConvergenceTolerance(true).units(); // Get IP units.
  double value(0.001);
  Quantity testQ(value,units);
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.setHeatingConvergenceTolerance(testQ));
  Quantity q = zoneHVACFourPipeFanCoil.getHeatingConvergenceTolerance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_addToThermalZone) {
  Model model;

  // need schedule
  ScheduleConstant sched(model);
  EXPECT_EQ(1u,model.objects().size());
  sched.setValue(1.0); // Always on

  // need fan
  FanConstantVolume fan(model,sched);

  // need cooling and heating coils
  CoilCoolingWater coolingCoil(model,sched);
  CoilHeatingWater heatingCoil(model,sched);

  // construct object
  ZoneHVACFourPipeFanCoil zoneHVACFourPipeFanCoil(model,sched,fan,coolingCoil,heatingCoil);

  // Is it hooked up to the inside components?
  EXPECT_EQ(fan.handle(),zoneHVACFourPipeFanCoil.supplyAirFan().handle());
  EXPECT_EQ(coolingCoil.handle(),zoneHVACFourPipeFanCoil.coolingCoil().handle());
  EXPECT_EQ(heatingCoil.handle(),zoneHVACFourPipeFanCoil.heatingCoil().handle());

  // construct a ThermalZone

  ThermalZone thermalZone(model);

  EXPECT_EQ(0u,thermalZone.equipment().size());

  EXPECT_TRUE(zoneHVACFourPipeFanCoil.addToThermalZone(thermalZone));

  EXPECT_TRUE(zoneHVACFourPipeFanCoil.inletNode());
  EXPECT_TRUE(zoneHVACFourPipeFanCoil.outletNode());

  EXPECT_EQ(1u,thermalZone.equipment().size());

  EXPECT_TRUE(fan.containingZoneHVACComponent());
  EXPECT_TRUE(coolingCoil.containingZoneHVACComponent());
  EXPECT_TRUE(heatingCoil.containingZoneHVACComponent());
}

TEST_F(ModelFixture,ZoneHVACFourPipeFanCoil_AddRemoveAirLoopHVAC)
{
  model::Model m;
  model::Schedule availabilitySchedule = m.alwaysOnDiscreteSchedule();
  model::FanConstantVolume fan(m,availabilitySchedule);
  model::CoilHeatingWater heatingCoil(m,availabilitySchedule);
  CoilCoolingWater coolingCoil(m,availabilitySchedule);

  model::ZoneHVACFourPipeFanCoil fpfc( m,
                                       availabilitySchedule,
                                       fan,
                                       heatingCoil,
                                       coolingCoil );

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
  ASSERT_EQ(thermalZone,fpfc.thermalZone().get());
  ASSERT_EQ(2u,thermalZone.equipment().size());
  ASSERT_EQ(airLoop,fpfc.airLoopHVAC().get());

  fpfc.removeFromThermalZone();
  ASSERT_FALSE(fpfc.airLoopHVAC());
  ASSERT_FALSE(fpfc.thermalZone());
  ASSERT_EQ(1u,thermalZone.equipment().size());
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
  ASSERT_EQ(thermalZone,fpfc.thermalZone().get());
  ASSERT_EQ(1u,thermalZone.equipment().size());
}


