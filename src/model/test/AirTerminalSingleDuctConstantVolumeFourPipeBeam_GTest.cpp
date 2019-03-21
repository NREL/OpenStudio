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

#include "../AirTerminalSingleDuctConstantVolumeFourPipeBeam.hpp"
#include "../AirTerminalSingleDuctConstantVolumeFourPipeBeam_Impl.hpp"

#include "../CoilCoolingFourPipeBeam.hpp"
#include "../CoilHeatingFourPipeBeam.hpp"
#include "../PlantLoop.hpp"
#include "../AirLoopHVAC.hpp"
#include "../ScheduleConstant.hpp"
#include "../Node.hpp"

// Needed for getConcreteModelObjects
#include "../CoilCoolingFourPipeBeam_Impl.hpp"
#include "../CoilHeatingFourPipeBeam_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;


TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeFourPipeBeam_DefaultConstructors)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    Model m;
    AirTerminalSingleDuctConstantVolumeFourPipeBeam atu(m);

    exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeFourPipeBeam_OverloadedCtor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    Model m;
    CoilCoolingFourPipeBeam cc(m);
    CoilHeatingFourPipeBeam hc(m);
    AirTerminalSingleDuctConstantVolumeFourPipeBeam atu(m, cc, hc);

    exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeFourPipeBeam_BasicGettersSetters)
{
  Model m;
  AirTerminalSingleDuctConstantVolumeFourPipeBeam atu(m);

}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeFourPipeBeam_Coils)
{

  Model m;
  CoilCoolingFourPipeBeam cc(m);
  CoilHeatingFourPipeBeam hc(m);
  AirTerminalSingleDuctConstantVolumeFourPipeBeam atu(m, cc, hc);

  ASSERT_TRUE(atu.coolingCoil());
  EXPECT_EQ(cc.handle(), atu.coolingCoil()->handle());

  ASSERT_TRUE(atu.heatingCoil());
  EXPECT_EQ(hc.handle(), atu.heatingCoil()->handle());

  // Test setting the coils
  CoilCoolingFourPipeBeam cc2(m);
  EXPECT_TRUE(atu.setCoolingCoil(cc2));
  EXPECT_EQ(cc2.handle(), atu.coolingCoil()->handle());

  CoilHeatingFourPipeBeam hc2(m);
  EXPECT_TRUE(atu.setHeatingCoil(hc2));
  EXPECT_EQ(hc2.handle(), atu.heatingCoil()->handle());

  PlantLoop hw_p(m);
  PlantLoop chw_p(m);

  EXPECT_TRUE(chw_p.addDemandBranchForComponent(cc2));
  EXPECT_TRUE(hw_p.addDemandBranchForComponent(hc2));

  /* Test the provided convenience functions */
  ASSERT_TRUE(atu.chilledWaterPlantLoop());
  EXPECT_EQ(chw_p.handle(), atu.chilledWaterPlantLoop()->handle());

  ASSERT_TRUE(atu.chilledWaterInletNode());
  ASSERT_TRUE(cc2.chilledWaterInletNode());
  EXPECT_EQ(cc2.chilledWaterInletNode()->handle(), atu.chilledWaterInletNode()->handle());

  ASSERT_TRUE(atu.chilledWaterOutletNode());
  ASSERT_TRUE(cc2.chilledWaterOutletNode());
  EXPECT_EQ(cc2.chilledWaterOutletNode()->handle(), atu.chilledWaterOutletNode()->handle());


  ASSERT_TRUE(atu.hotWaterPlantLoop());
  EXPECT_EQ(hw_p.handle(), atu.hotWaterPlantLoop()->handle());

  ASSERT_TRUE(atu.hotWaterInletNode());
  ASSERT_TRUE(hc2.hotWaterInletNode());
  EXPECT_EQ(hc2.hotWaterInletNode()->handle(), atu.hotWaterInletNode()->handle());

  ASSERT_TRUE(atu.hotWaterOutletNode());
  ASSERT_TRUE(hc2.hotWaterOutletNode());
  EXPECT_EQ(hc2.hotWaterOutletNode()->handle(), atu.hotWaterOutletNode()->handle());

}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeFourPipeBeam_CloneAndRemove)
{

  Model m;
  CoilCoolingFourPipeBeam cc(m);
  CoilHeatingFourPipeBeam hc(m);
  AirTerminalSingleDuctConstantVolumeFourPipeBeam atu(m, cc, hc);

  AirTerminalSingleDuctConstantVolumeFourPipeBeam atuClone = atu.clone(m).cast<AirTerminalSingleDuctConstantVolumeFourPipeBeam>();

  // I expect the cooling/heating coils to have been cloned too
  EXPECT_EQ(2u, m.getConcreteModelObjects<AirTerminalSingleDuctConstantVolumeFourPipeBeam>().size());
  EXPECT_EQ(2u, m.getConcreteModelObjects<CoilCoolingFourPipeBeam>().size());
  EXPECT_EQ(2u, m.getConcreteModelObjects<CoilHeatingFourPipeBeam>().size());

  // And they should have been reassigned to the clone too
  ASSERT_TRUE(atuClone.coolingCoil());
  ASSERT_TRUE(atuClone.heatingCoil());

  // test remove method
  atuClone.remove();
  EXPECT_EQ(1u, m.getConcreteModelObjects<AirTerminalSingleDuctConstantVolumeFourPipeBeam>().size());
  EXPECT_EQ(1u, m.getConcreteModelObjects<CoilCoolingFourPipeBeam>().size());
  EXPECT_EQ(1u, m.getConcreteModelObjects<CoilHeatingFourPipeBeam>().size());


}

