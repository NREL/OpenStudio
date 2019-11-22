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
#include "../CoilSystemCoolingWaterHeatExchangerAssisted.hpp"
#include "../CoilSystemCoolingWaterHeatExchangerAssisted_Impl.hpp"
#include "../CoilCoolingWater.hpp"
#include "../CoilCoolingWater_Impl.hpp"
#include "../HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "../HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../AirLoopHVACUnitarySystem.hpp"
#include "../ZoneHVACUnitVentilator.hpp"
#include "../ZoneHVACFourPipeFanCoil.hpp"
#include "../FanConstantVolume.hpp"
#include "../CoilHeatingWater.hpp"
#include "../CoilCoolingWater.hpp"
#include "../Schedule.hpp"

using namespace openstudio;
using namespace openstudio::model;


TEST_F(ModelFixture,CoilSystemCoolingWaterHeatExchangerAssisted)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
     Model m;
     CoilSystemCoolingWaterHeatExchangerAssisted coilSystem(m);

     exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

// This test ensures that only the parent CoilSystem can call addToNode, the individual CoilCoolingWater and HX cannot
TEST_F(ModelFixture, CoilSystemCoolingWaterHeatExchangerAssisted_addToNode) {

  Model m;
  CoilSystemCoolingWaterHeatExchangerAssisted coilSystem(m);

  AirLoopHVAC a(m);
  Node n = a.supplyOutletNode();

  CoilCoolingWater cc = coilSystem.coolingCoil().cast<CoilCoolingWater>();
  HeatExchangerAirToAirSensibleAndLatent hx = coilSystem.heatExchanger().cast<HeatExchangerAirToAirSensibleAndLatent>();

  EXPECT_EQ(2u, a.supplyComponents().size());

  EXPECT_FALSE(cc.addToNode(n));
  EXPECT_EQ(2u, a.supplyComponents().size());

  EXPECT_FALSE(hx.addToNode(n));
  EXPECT_EQ(2u, a.supplyComponents().size());

  EXPECT_TRUE(coilSystem.addToNode(n));
  EXPECT_EQ(3u, a.supplyComponents().size());

  {
    auto containingHVACComponent = cc.containingHVACComponent();
    ASSERT_TRUE(containingHVACComponent);
    EXPECT_EQ(containingHVACComponent->handle(), coilSystem.handle());
  }

  {
    auto containingHVACComponent = hx.containingHVACComponent();
    ASSERT_TRUE(containingHVACComponent);
    EXPECT_EQ(containingHVACComponent->handle(), coilSystem.handle());
  }


  // BUT, we need to be able to connect the water side of the Coil...
  PlantLoop p(m);
  EXPECT_TRUE(p.addDemandBranchForComponent(cc));
}

TEST_F(ModelFixture, CoilSystemCoolingWaterHeatExchangerAssisted_clone) {

  Model m;

  // Create a CoilSystem, connected to an AirLoopHVAC, and with its coolingCoil connected to a PlantLoop
  CoilSystemCoolingWaterHeatExchangerAssisted coilSystem(m);
  CoilCoolingWater cc = coilSystem.coolingCoil().cast<CoilCoolingWater>();
  HeatExchangerAirToAirSensibleAndLatent hx = coilSystem.heatExchanger().cast<HeatExchangerAirToAirSensibleAndLatent>();


  AirLoopHVAC a(m);
  Node n = a.supplyOutletNode();
  EXPECT_TRUE(coilSystem.addToNode(n));

  PlantLoop p(m);
  EXPECT_TRUE(p.addDemandBranchForComponent(cc));

  EXPECT_EQ(1u, m.getModelObjects<CoilSystemCoolingWaterHeatExchangerAssisted>().size());
  EXPECT_EQ(1u, m.getModelObjects<CoilCoolingWater>().size());
  EXPECT_EQ(1u, m.getModelObjects<HeatExchangerAirToAirSensibleAndLatent>().size());

  ASSERT_TRUE(coilSystem.airLoopHVAC());
  EXPECT_EQ(coilSystem.airLoopHVAC()->handle(), a.handle());

  EXPECT_TRUE(cc.plantLoop());

  // TODO: should these work?
  //ASSERT_TRUE(coilSystem.plantLoop());
  //EXPECT_EQ(coilSystem.plantLoop()->handle(), p.handle());
  //EXPECT_TRUE(hx.airLoopHVAC());


  CoilSystemCoolingWaterHeatExchangerAssisted coilSystem2 = coilSystem.clone(m).cast<CoilSystemCoolingWaterHeatExchangerAssisted>();

  EXPECT_EQ(2u, m.getModelObjects<CoilSystemCoolingWaterHeatExchangerAssisted>().size());
  EXPECT_EQ(2u, m.getModelObjects<CoilCoolingWater>().size());
  EXPECT_EQ(2u, m.getModelObjects<HeatExchangerAirToAirSensibleAndLatent>().size());

  EXPECT_TRUE(coilSystem.airLoopHVAC());
  EXPECT_TRUE(coilSystem.inletModelObject());
  EXPECT_TRUE(coilSystem.outletModelObject());

  EXPECT_FALSE(coilSystem2.airLoopHVAC());
  EXPECT_FALSE(coilSystem2.inletModelObject());
  EXPECT_FALSE(coilSystem2.outletModelObject());

  EXPECT_NE(coilSystem2.coolingCoil().handle(), cc.handle());
  EXPECT_NE(coilSystem2.heatExchanger().handle(), hx.handle());

}

TEST_F(ModelFixture, CoilSystemCoolingWaterHeatExchangerAssisted_containingComponent) {

  Model m;

  {
    AirLoopHVACUnitarySystem unitary(m);
    CoilSystemCoolingWaterHeatExchangerAssisted coilSystem(m);

    EXPECT_TRUE(unitary.setCoolingCoil(coilSystem));
    boost::optional<HVACComponent> _c = coilSystem.containingHVACComponent();
    ASSERT_TRUE(_c);
    EXPECT_EQ(_c->handle(), unitary.handle());
  }


  {
    ZoneHVACUnitVentilator uv(m);

    CoilSystemCoolingWaterHeatExchangerAssisted coilSystem(m);
    EXPECT_TRUE(uv.setCoolingCoil(coilSystem));
    boost::optional<ZoneHVACComponent> _c = coilSystem.containingZoneHVACComponent();
    ASSERT_TRUE(_c);
    EXPECT_EQ(_c->handle(), uv.handle());
  }

  {
    Schedule s = m.alwaysOnDiscreteSchedule();
    FanConstantVolume supplyFan(m);
    CoilCoolingWater coolingCoil(m);
    CoilHeatingWater heatingCoil(m);

    ZoneHVACFourPipeFanCoil fc(m, s, supplyFan, coolingCoil, heatingCoil);

    CoilSystemCoolingWaterHeatExchangerAssisted coilSystem(m);
    EXPECT_TRUE(fc.setCoolingCoil(coilSystem));
    boost::optional<ZoneHVACComponent> _c = coilSystem.containingZoneHVACComponent();
    ASSERT_TRUE(_c);
    EXPECT_EQ(_c->handle(), fc.handle());
  }

}
