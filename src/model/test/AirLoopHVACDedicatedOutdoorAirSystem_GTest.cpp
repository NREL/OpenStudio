/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../AirLoopHVACDedicatedOutdoorAirSystem.hpp"
#include "../AirLoopHVACDedicatedOutdoorAirSystem_Impl.hpp"
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../ControllerOutdoorAir_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../CoilCoolingDXSingleSpeed.hpp"
#include "../CoilCoolingDXTwoSpeed.hpp"
#include "../CoilCoolingDXTwoStageWithHumidityControlMode.hpp"
#include "../CoilCoolingDXVariableSpeed.hpp"
#include "../CoilCoolingWater.hpp"
#include "../CoilHeatingDXMultiSpeed.hpp"
#include "../CoilHeatingDXSingleSpeed.hpp"
#include "../CoilHeatingDXVariableSpeed.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CoilHeatingGas.hpp"
#include "../CoilHeatingWater.hpp"
#include "../CoilSystemCoolingDXHeatExchangerAssisted.hpp"
#include "../CoilSystemCoolingWaterHeatExchangerAssisted.hpp"
#include "../CoilSystemIntegratedHeatPumpAirSource.hpp"
#include "../FanComponentModel.hpp"
#include "../FanConstantVolume.hpp"
#include "../FanOnOff.hpp"
#include "../FanSystemModel.hpp"
#include "../FanVariableVolume.hpp"
#include "../HumidifierSteamElectric.hpp"
#include "../HumidifierSteamGas.hpp"
#include "../HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "../SetpointManagerOutdoorAirReset.hpp"
#include "../SetpointManagerOutdoorAirReset_Impl.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

TEST_F(ModelFixture, AirLoopHVACDedicatedOutdoorAirSystem_AirLoopHVACDedicatedOutdoorAirSystem) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;

      ControllerOutdoorAir controller(model);
      AirLoopHVACOutdoorAirSystem oaSystem(model, controller);
      AirLoopHVACDedicatedOutdoorAirSystem doaSystem(oaSystem);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model model;

  ControllerOutdoorAir controller(model);
  AirLoopHVACOutdoorAirSystem oaSystem(model, controller);
  AirLoopHVACDedicatedOutdoorAirSystem doaSystem(oaSystem);

  ASSERT_TRUE(doaSystem.airLoopHVACOutdoorAirSystem().optionalCast<AirLoopHVACOutdoorAirSystem>());
  ASSERT_TRUE(doaSystem.availabilitySchedule().optionalCast<Schedule>());
  EXPECT_EQ(4.5, doaSystem.preheatDesignTemperature());
  EXPECT_EQ(0.004, doaSystem.preheatDesignHumidityRatio());
  EXPECT_EQ(17.5, doaSystem.precoolDesignTemperature());
  EXPECT_EQ(0.012, doaSystem.precoolDesignHumidityRatio());
  EXPECT_EQ(0, doaSystem.numberofAirLoops());
  EXPECT_EQ(0, doaSystem.airLoops().size());
}

TEST_F(ModelFixture, AirLoopHVACDedicatedOutdoorAirSystem_SetGetFields) {
  Model model;

  ControllerOutdoorAir controller(model);
  AirLoopHVACOutdoorAirSystem oaSystem(model, controller);
  EXPECT_FALSE(oaSystem.airLoopHVACDedicatedOutdoorAirSystem());
  AirLoopHVACDedicatedOutdoorAirSystem doaSystem(oaSystem);
  EXPECT_TRUE(oaSystem.airLoopHVACDedicatedOutdoorAirSystem());

  ControllerOutdoorAir controller2(model);
  AirLoopHVACOutdoorAirSystem oaSystem2(model, controller2);
  oaSystem2.setName("New System");
  ScheduleConstant sched(model);
  sched.setValue(0.5);
  AirLoopHVAC airLoop(model);

  EXPECT_TRUE(doaSystem.setAirLoopHVACOutdoorAirSystem(oaSystem2));
  EXPECT_TRUE(doaSystem.setAvailabilitySchedule(sched));
  EXPECT_TRUE(doaSystem.setPreheatDesignTemperature(5.0));
  EXPECT_TRUE(doaSystem.setPreheatDesignHumidityRatio(0.0035));
  EXPECT_TRUE(doaSystem.setPrecoolDesignTemperature(18.2));
  EXPECT_TRUE(doaSystem.setPrecoolDesignHumidityRatio(0.0112));
  EXPECT_TRUE(doaSystem.addAirLoop(airLoop));

  ASSERT_TRUE(doaSystem.airLoopHVACOutdoorAirSystem().optionalCast<AirLoopHVACOutdoorAirSystem>());
  EXPECT_EQ("New System", doaSystem.airLoopHVACOutdoorAirSystem().nameString());
  Schedule schedule = doaSystem.availabilitySchedule();
  boost::optional<ScheduleConstant> scheduleConstant = schedule.optionalCast<ScheduleConstant>();
  ASSERT_TRUE(scheduleConstant);
  EXPECT_EQ((*scheduleConstant).value(), 0.5);
  EXPECT_EQ(5.0, doaSystem.preheatDesignTemperature());
  EXPECT_EQ(0.0035, doaSystem.preheatDesignHumidityRatio());
  EXPECT_EQ(18.2, doaSystem.precoolDesignTemperature());
  EXPECT_EQ(0.0112, doaSystem.precoolDesignHumidityRatio());
  EXPECT_EQ(1, doaSystem.numberofAirLoops());
  EXPECT_EQ(1, doaSystem.airLoops().size());
  EXPECT_TRUE(doaSystem.removeAirLoop(airLoop));
  EXPECT_EQ(0, doaSystem.numberofAirLoops());
  EXPECT_EQ(0, doaSystem.airLoops().size());

  std::vector<AirLoopHVAC> airLoopHVACs;
  EXPECT_FALSE(airLoop.airLoopHVACDedicatedOutdoorAirSystem());
  airLoopHVACs.push_back(airLoop);
  EXPECT_TRUE(doaSystem.addAirLoops(airLoopHVACs));
  EXPECT_EQ(1, doaSystem.numberofAirLoops());
  EXPECT_EQ(1, doaSystem.airLoops().size());
  EXPECT_TRUE(airLoop.airLoopHVACDedicatedOutdoorAirSystem());
  doaSystem.removeAllAirLoops();
  EXPECT_EQ(0, doaSystem.numberofAirLoops());
  EXPECT_EQ(0, doaSystem.airLoops().size());
  EXPECT_FALSE(doaSystem.removeAirLoop(0));

  AirLoopHVAC airLoop2(model);
  airLoopHVACs.push_back(airLoop2);

  EXPECT_TRUE(doaSystem.addAirLoops(airLoopHVACs));
  EXPECT_EQ(2, doaSystem.numberofAirLoops());
  EXPECT_EQ(2, doaSystem.airLoops().size());
  EXPECT_TRUE(doaSystem.removeAirLoop(1));
  EXPECT_EQ(1, doaSystem.numberofAirLoops());
  EXPECT_EQ(1, doaSystem.airLoops().size());
}

TEST_F(ModelFixture, AirLoopHVACDedicatedOutdoorAirSystem_AirLoops) {
  Model model;
  ControllerOutdoorAir controller(model);
  AirLoopHVACOutdoorAirSystem oaSystem(model, controller);
  AirLoopHVACDedicatedOutdoorAirSystem doaSystem(oaSystem);

  AirLoopHVAC airLoop1(model);
  EXPECT_EQ(0, doaSystem.airLoops().size());
  EXPECT_TRUE(doaSystem.addAirLoop(airLoop1));
  EXPECT_EQ(1, doaSystem.airLoops().size());
  EXPECT_TRUE(doaSystem.addAirLoop(airLoop1));
  EXPECT_EQ(1, doaSystem.airLoops().size());

  ControllerOutdoorAir controller2(model);
  AirLoopHVACOutdoorAirSystem oaSystem2(model, controller2);
  AirLoopHVACDedicatedOutdoorAirSystem doaSystem2(oaSystem2);

  AirLoopHVAC airLoop2(model);
  EXPECT_EQ(0, doaSystem2.airLoops().size());
  EXPECT_TRUE(doaSystem2.addAirLoop(airLoop2));
  EXPECT_EQ(1, doaSystem2.airLoops().size());
  EXPECT_TRUE(doaSystem.addAirLoop(airLoop1));
  EXPECT_EQ(1, doaSystem2.airLoops().size());

  doaSystem2.removeAllAirLoops();
  EXPECT_EQ(0, doaSystem2.airLoops().size());
  std::vector<AirLoopHVAC> airLoopHVACs;
  airLoopHVACs.push_back(airLoop2);
  airLoopHVACs.push_back(airLoop2);

  EXPECT_TRUE(doaSystem2.addAirLoops(airLoopHVACs));
  EXPECT_EQ(1, doaSystem2.airLoops().size());
}

TEST_F(ModelFixture, AirLoopHVACDedicatedOutdoorAirSystem_Clone) {
  Model model;
  ControllerOutdoorAir controller(model);
  AirLoopHVACOutdoorAirSystem oaSystem(model, controller);
  AirLoopHVAC airLoop(model);
  AirLoopHVAC airLoop2(model);
  std::vector<AirLoopHVAC> airLoopHVACs;
  airLoopHVACs.push_back(airLoop);
  airLoopHVACs.push_back(airLoop2);

  AirLoopHVACDedicatedOutdoorAirSystem doaSystem(oaSystem);
  doaSystem.addAirLoops(airLoopHVACs);

  EXPECT_EQ(2, doaSystem.numberofAirLoops());
  EXPECT_EQ(2, doaSystem.airLoops().size());
  EXPECT_EQ(2, model.getConcreteModelObjects<AirLoopHVAC>().size());
  EXPECT_EQ(1, model.getConcreteModelObjects<AirLoopHVACDedicatedOutdoorAirSystem>().size());

  auto doaSystemClone = doaSystem.clone(model).cast<AirLoopHVACDedicatedOutdoorAirSystem>();

  EXPECT_EQ(2, doaSystemClone.numberofAirLoops());
  EXPECT_EQ(2, doaSystemClone.airLoops().size());
  EXPECT_EQ(2, model.getConcreteModelObjects<AirLoopHVAC>().size());
  EXPECT_EQ(2, model.getConcreteModelObjects<AirLoopHVACDedicatedOutdoorAirSystem>().size());
}

TEST_F(ModelFixture, AirLoopHVACDedicatedOutdoorAirSystem_Remove) {
  Model model;
  std::vector<AirLoopHVACDedicatedOutdoorAirSystem> doaSystems = model.getConcreteModelObjects<AirLoopHVACDedicatedOutdoorAirSystem>();
  EXPECT_EQ(0u, doaSystems.size());

  ControllerOutdoorAir controller(model);
  AirLoopHVACOutdoorAirSystem oaSystem(model, controller);
  AirLoopHVAC airLoop(model);
  AirLoopHVAC airLoop2(model);
  std::vector<AirLoopHVAC> airLoopHVACs;
  airLoopHVACs.push_back(airLoop);
  airLoopHVACs.push_back(airLoop2);

  AirLoopHVACDedicatedOutdoorAirSystem doaSystem(oaSystem);
  doaSystem.addAirLoops(airLoopHVACs);

  doaSystems = model.getConcreteModelObjects<AirLoopHVACDedicatedOutdoorAirSystem>();
  EXPECT_EQ(1u, doaSystems.size());

  EXPECT_FALSE(doaSystem.remove().empty());
  doaSystems = model.getConcreteModelObjects<AirLoopHVACDedicatedOutdoorAirSystem>();
  EXPECT_EQ(0u, doaSystems.size());
  EXPECT_FALSE(oaSystem.airLoopHVACDedicatedOutdoorAirSystem());
}

TEST_F(ModelFixture, AirLoopHVACDedicatedOutdoorAirSystem_Remove2) {
  Model model;
  std::vector<AirLoopHVACDedicatedOutdoorAirSystem> doaSystems = model.getConcreteModelObjects<AirLoopHVACDedicatedOutdoorAirSystem>();
  EXPECT_EQ(0u, doaSystems.size());

  ControllerOutdoorAir controller(model);
  AirLoopHVACOutdoorAirSystem oaSystem(model, controller);
  AirLoopHVAC airLoop(model);
  AirLoopHVAC airLoop2(model);
  std::vector<AirLoopHVAC> airLoopHVACs;
  airLoopHVACs.push_back(airLoop);
  airLoopHVACs.push_back(airLoop2);

  AirLoopHVACDedicatedOutdoorAirSystem doaSystem(oaSystem);
  doaSystem.addAirLoops(airLoopHVACs);

  doaSystems = model.getConcreteModelObjects<AirLoopHVACDedicatedOutdoorAirSystem>();
  EXPECT_EQ(1u, doaSystems.size());

  EXPECT_FALSE(doaSystem.remove().empty());
  doaSystems = model.getConcreteModelObjects<AirLoopHVACDedicatedOutdoorAirSystem>();
  EXPECT_EQ(0u, doaSystems.size());
  EXPECT_FALSE(oaSystem.airLoopHVACDedicatedOutdoorAirSystem());
}

TEST_F(ModelFixture, AirLoopHVACDedicatedOutdoorAirSystem_Remove3) {
  Model model;
  std::vector<AirLoopHVACDedicatedOutdoorAirSystem> doaSystems = model.getModelObjects<AirLoopHVACDedicatedOutdoorAirSystem>();
  EXPECT_EQ(0u, doaSystems.size());
  std::vector<SetpointManagerOutdoorAirReset> spms = model.getModelObjects<SetpointManagerOutdoorAirReset>();

  ControllerOutdoorAir controller(model);
  AirLoopHVACOutdoorAirSystem oaSystem(model, controller);
  AirLoopHVAC airLoop(model);
  AirLoopHVAC airLoop2(model);
  std::vector<AirLoopHVAC> airLoopHVACs;
  airLoopHVACs.push_back(airLoop);
  airLoopHVACs.push_back(airLoop2);

  AirLoopHVACDedicatedOutdoorAirSystem doaSystem(oaSystem);
  doaSystem.addAirLoops(airLoopHVACs);

  CoilHeatingElectric coil(model);
  coil.addToNode(oaSystem.outboardOANode().get());

  SetpointManagerOutdoorAirReset spm(model);
  spm.addToNode(coil.outletModelObject()->optionalCast<Node>().get());

  doaSystems = model.getModelObjects<AirLoopHVACDedicatedOutdoorAirSystem>();
  EXPECT_EQ(1u, doaSystems.size());

  // make sure there are no traces of the outdoorAirSystem leftover after DOAS is removed
  EXPECT_FALSE(doaSystem.remove().empty());
  doaSystems = model.getModelObjects<AirLoopHVACDedicatedOutdoorAirSystem>();
  EXPECT_EQ(0u, doaSystems.size());
  spms = model.getModelObjects<SetpointManagerOutdoorAirReset>();
  EXPECT_EQ(0u, spms.size());
}

TEST_F(ModelFixture, AirLoopHVACDedicatedOutdoorAirSystem_SupportedComponents) {
  Model m;
  ControllerOutdoorAir controller(m);
  AirLoopHVACOutdoorAirSystem oas(m, controller);
  AirLoopHVACDedicatedOutdoorAirSystem doas(oas);

  // CoilCoolingDXSingleSpeed
  {
    CoilCoolingDXSingleSpeed coil(m);
    EXPECT_TRUE(coil.addToNode(oas.outboardOANode().get()));
  }

  // CoilCoolingDXTwoSpeed
  {
    CoilCoolingDXTwoSpeed coil(m);
    EXPECT_TRUE(coil.addToNode(oas.outboardOANode().get()));
  }

  // CoilCoolingDXTwoStageWithHumidityControlMode
  {
    CoilCoolingDXTwoStageWithHumidityControlMode coil(m);
    EXPECT_TRUE(coil.addToNode(oas.outboardOANode().get()));
  }

  // CoilCoolingDXVariableSpeed
  {
    CoilCoolingDXVariableSpeed coil(m);
    EXPECT_TRUE(coil.addToNode(oas.outboardOANode().get()));
  }

  // CoilCoolingWater
  {
    CoilCoolingWater coil(m);
    EXPECT_TRUE(coil.addToNode(oas.outboardOANode().get()));
  }

  // CoilHeatingDXSingleSpeed
  {
    CoilHeatingDXSingleSpeed coil(m);
    EXPECT_TRUE(coil.addToNode(oas.outboardOANode().get()));
  }

  // CoilHeatingDXVariableSpeed
  {
    CoilHeatingDXVariableSpeed coil(m);
    EXPECT_TRUE(coil.addToNode(oas.outboardOANode().get()));
  }

  // CoilHeatingElectric
  {
    CoilHeatingElectric coil(m);
    EXPECT_TRUE(coil.addToNode(oas.outboardOANode().get()));
  }

  // CoilHeatingGas
  {
    CoilHeatingGas coil(m);
    EXPECT_TRUE(coil.addToNode(oas.outboardOANode().get()));
  }

  // CoilHeatingWater
  {
    CoilHeatingWater coil(m);
    EXPECT_TRUE(coil.addToNode(oas.outboardOANode().get()));
  }

  // CoilSystemCoolingDXHeatExchangerAssisted
  {
    CoilSystemCoolingDXHeatExchangerAssisted coil(m);
    EXPECT_FALSE(coil.addToNode(oas.outboardOANode().get()));
  }

  // CoilSystemCoolingWaterHeatExchangerAssisted
  {
    CoilSystemCoolingWaterHeatExchangerAssisted coil(m);
    EXPECT_TRUE(coil.addToNode(oas.outboardOANode().get()));
  }

  // CoilSystemIntegratedHeatPumpAirSource
  {
    CoilSystemIntegratedHeatPumpAirSource coil(m);
    EXPECT_FALSE(coil.addToNode(oas.outboardOANode().get()));
  }

  // FanComponentModel
  {
    FanComponentModel fan(m);
    EXPECT_TRUE(fan.addToNode(oas.outboardOANode().get()));
  }

  // FanConstantVolume
  {
    FanConstantVolume fan(m);
    EXPECT_TRUE(fan.addToNode(oas.outboardOANode().get()));
  }

  // FanOnOff
  {
    FanOnOff fan(m);
    EXPECT_FALSE(fan.addToNode(oas.outboardOANode().get()));
  }

  // FanSystemModel
  {
    FanSystemModel fan(m);
    EXPECT_TRUE(fan.addToNode(oas.outboardOANode().get()));
  }

  // FanVariableVolume
  {
    FanVariableVolume fan(m);
    EXPECT_TRUE(fan.addToNode(oas.outboardOANode().get()));
  }

  // HumidifierSteamElectric
  {
    HumidifierSteamElectric hum(m);
    EXPECT_TRUE(hum.addToNode(oas.outboardOANode().get()));
  }

  // HumidifierSteamGas
  {
    HumidifierSteamGas hum(m);
    EXPECT_TRUE(hum.addToNode(oas.outboardOANode().get()));
  }

  // HeatExchangerAirToAirSensibleAndLatent
  {
    HeatExchangerAirToAirSensibleAndLatent hx(m);
    EXPECT_TRUE(hx.addToNode(oas.outboardOANode().get()));
  }
}
