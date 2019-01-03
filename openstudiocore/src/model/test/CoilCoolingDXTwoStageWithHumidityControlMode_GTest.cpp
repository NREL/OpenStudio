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
#include "../CoilCoolingDXTwoStageWithHumidityControlMode.hpp"
#include "../CoilCoolingDXTwoStageWithHumidityControlMode_Impl.hpp"
#include "../CoilPerformanceDXCooling.hpp"
#include "../CoilPerformanceDXCooling_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;


TEST_F(ModelFixture,CoilCoolingDXTwoStageWithHumidityControlMode)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
     Model m;
     CoilCoolingDXTwoStageWithHumidityControlMode coil(m);

     exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

/* Verifies that the CoilPerformanceDXCooling objects are indeed cloned too
 * Address https://github.com/NREL/OpenStudio/issues/2253
 */
TEST_F(ModelFixture,CoilCoolingDXTwoStageWithHumidityControlMode_CloneCoilPerf)
{
  Model m;
  Model m2;

  CoilCoolingDXTwoStageWithHumidityControlMode coil(m);

  // The Ctor should have assigned the CoilPerformanceDXCooling Objects
  ASSERT_TRUE(coil.normalModeStage1CoilPerformance());
  ASSERT_TRUE(coil.normalModeStage1Plus2CoilPerformance());
  ASSERT_TRUE(coil.dehumidificationMode1Stage1CoilPerformance());
  ASSERT_TRUE(coil.dehumidificationMode1Stage1Plus2CoilPerformance());
  EXPECT_EQ(4u, m.getModelObjects<CoilPerformanceDXCooling>().size());

  // Clone in same model
  CoilCoolingDXTwoStageWithHumidityControlMode coilClone = coil.clone(m).cast<CoilCoolingDXTwoStageWithHumidityControlMode>();
  ASSERT_TRUE(coilClone.normalModeStage1CoilPerformance());
  ASSERT_TRUE(coilClone.normalModeStage1Plus2CoilPerformance());
  ASSERT_TRUE(coilClone.dehumidificationMode1Stage1CoilPerformance());
  ASSERT_TRUE(coilClone.dehumidificationMode1Stage1Plus2CoilPerformance());
  // These are "resources", so they should point to the same as the ori, not cloned
  EXPECT_EQ(4u, m.getModelObjects<CoilPerformanceDXCooling>().size());

  ASSERT_EQ(coil.normalModeStage1CoilPerformance().get().handle(), coilClone.normalModeStage1CoilPerformance().get().handle());
  ASSERT_EQ(coil.normalModeStage1Plus2CoilPerformance().get().handle(), coilClone.normalModeStage1Plus2CoilPerformance().get().handle());
  ASSERT_EQ(coil.dehumidificationMode1Stage1CoilPerformance().get().handle(), coilClone.dehumidificationMode1Stage1CoilPerformance().get().handle());
  ASSERT_EQ(coil.dehumidificationMode1Stage1Plus2CoilPerformance().get().handle(), coilClone.dehumidificationMode1Stage1Plus2CoilPerformance().get().handle());

  // Clone in other model
  //
  // This line triggers an OS_ASSERT exception ModelObject_Impl::clone (868)
  CoilCoolingDXTwoStageWithHumidityControlMode coilClone2 = coil.clone(m2).cast<CoilCoolingDXTwoStageWithHumidityControlMode>();
  ASSERT_TRUE(coilClone2.normalModeStage1CoilPerformance());
  ASSERT_TRUE(coilClone2.normalModeStage1Plus2CoilPerformance());
  ASSERT_TRUE(coilClone2.dehumidificationMode1Stage1CoilPerformance());
  ASSERT_TRUE(coilClone2.dehumidificationMode1Stage1Plus2CoilPerformance());
  // They should have been cloned to the new model too
  EXPECT_EQ(4u, m2.getModelObjects<CoilPerformanceDXCooling>().size());

  // Name comparison (can't do handle, not the same model, and actual clone)
  ASSERT_EQ(coil.normalModeStage1CoilPerformance().get().name(), coilClone2.normalModeStage1CoilPerformance().get().name());
  ASSERT_EQ(coil.normalModeStage1Plus2CoilPerformance().get().name(), coilClone2.normalModeStage1Plus2CoilPerformance().get().name());
  ASSERT_EQ(coil.dehumidificationMode1Stage1CoilPerformance().get().name(), coilClone2.dehumidificationMode1Stage1CoilPerformance().get().name());
  ASSERT_EQ(coil.dehumidificationMode1Stage1Plus2CoilPerformance().get().name(), coilClone2.dehumidificationMode1Stage1Plus2CoilPerformance().get().name());

}
