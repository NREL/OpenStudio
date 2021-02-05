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
#include <string>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../ThermalStoragePcmSimple.hpp"
#include "../ThermalStoragePcmSimple_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ThermalStoragePcmSimple_ThermalStoragePcmSimple) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      ThermalStoragePcmSimple ts(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;

  ThermalStoragePcmSimple ts(m);

  EXPECT_EQ("IceOnCoilInternal", ts.iceStorageType());
  EXPECT_EQ(1.0, ts.capacity());
  EXPECT_TRUE(ts.isOnsetTemperatureOfPhaseChangeDefaulted());
  EXPECT_EQ(5.5, ts.onsetTemperatureOfPhaseChange());
  EXPECT_TRUE(ts.isFinishTemperatureOfPhaseChangeDefaulted());
  EXPECT_EQ(7.0, ts.finishTemperatureOfPhaseChange());
  EXPECT_TRUE(ts.isUAAtSolidPhaseOfPhaseChangeMaterialDefaulted());
  EXPECT_EQ(20000, ts.uaAtSolidPhaseOfPhaseChangeMaterial());
  EXPECT_TRUE(ts.isUAAtLiquidPhaseOfPhaseChangeMaterialDefaulted());
  EXPECT_EQ(20000, ts.uaAtLiquidPhaseOfPhaseChangeMaterial());

  EXPECT_FALSE(ts.containingHVACComponent());
}

TEST_F(ModelFixture, ThermalStoragePcmSimple_SetGetFields) {
  Model m;

  ThermalStoragePcmSimple ts(m);

  EXPECT_TRUE(ts.setIceStorageType("IceOnCoilExternal"));
  EXPECT_TRUE(ts.setCapacity(1.5));
  EXPECT_TRUE(ts.setOnsetTemperatureOfPhaseChange(25.0));
  EXPECT_TRUE(ts.setFinishTemperatureOfPhaseChange(30.1));
  EXPECT_TRUE(ts.setUAAtSolidPhaseOfPhaseChangeMaterial(10001));
  EXPECT_TRUE(ts.setUAAtLiquidPhaseOfPhaseChangeMaterial(25234));

  EXPECT_EQ("IceOnCoilExternal", ts.iceStorageType());
  EXPECT_EQ(1.5, ts.capacity());
  EXPECT_FALSE(ts.isOnsetTemperatureOfPhaseChangeDefaulted());
  EXPECT_EQ(25.0, ts.onsetTemperatureOfPhaseChange());
  EXPECT_FALSE(ts.isFinishTemperatureOfPhaseChangeDefaulted());
  EXPECT_EQ(30.1, ts.finishTemperatureOfPhaseChange());
  EXPECT_FALSE(ts.isUAAtSolidPhaseOfPhaseChangeMaterialDefaulted());
  EXPECT_EQ(10001, ts.uaAtSolidPhaseOfPhaseChangeMaterial());
  EXPECT_FALSE(ts.isUAAtLiquidPhaseOfPhaseChangeMaterialDefaulted());
  EXPECT_EQ(25234, ts.uaAtLiquidPhaseOfPhaseChangeMaterial());

  ts.resetOnsetTemperatureOfPhaseChange();
  ts.resetFinishTemperatureOfPhaseChange();
  ts.resetUAAtSolidPhaseOfPhaseChangeMaterial();
  ts.resetUAAtLiquidPhaseOfPhaseChangeMaterial();

  EXPECT_TRUE(ts.isOnsetTemperatureOfPhaseChangeDefaulted());
  EXPECT_EQ(5.5, ts.onsetTemperatureOfPhaseChange());
  EXPECT_TRUE(ts.isFinishTemperatureOfPhaseChangeDefaulted());
  EXPECT_EQ(7.0, ts.finishTemperatureOfPhaseChange());
  EXPECT_TRUE(ts.isUAAtSolidPhaseOfPhaseChangeMaterialDefaulted());
  EXPECT_EQ(20000, ts.uaAtSolidPhaseOfPhaseChangeMaterial());
  EXPECT_TRUE(ts.isUAAtLiquidPhaseOfPhaseChangeMaterialDefaulted());
  EXPECT_EQ(20000, ts.uaAtLiquidPhaseOfPhaseChangeMaterial());
}

TEST_F(ModelFixture, ThermalStoragePcmSimple_Clone) {
  Model m;

  ThermalStoragePcmSimple ts(m);

  ts.setOnsetTemperatureOfPhaseChange(6.25);

  ThermalStoragePcmSimple tsClone = ts.clone(m).cast<ThermalStoragePcmSimple>();
  ASSERT_EQ(1.0, tsClone.capacity());
  ASSERT_FALSE(tsClone.isOnsetTemperatureOfPhaseChangeDefaulted());
  ASSERT_TRUE(tsClone.isFinishTemperatureOfPhaseChangeDefaulted());

  Model m2;
  ThermalStoragePcmSimple tsClone2 = ts.clone(m2).cast<ThermalStoragePcmSimple>();
  ASSERT_EQ(1.0, tsClone2.capacity());
  ASSERT_FALSE(tsClone2.isOnsetTemperatureOfPhaseChangeDefaulted());
  ASSERT_TRUE(tsClone2.isFinishTemperatureOfPhaseChangeDefaulted());
}

TEST_F(ModelFixture, ThermalStoragePcmSimple_Remove) {
  Model m;

  auto size = m.modelObjects().size();

  ThermalStoragePcmSimple ts(m);

  EXPECT_EQ(size + 1, m.modelObjects().size());
  EXPECT_FALSE(ts.remove().empty());
  EXPECT_EQ(size, m.modelObjects().size());
}

TEST_F(ModelFixture, ThermalStoragePcmSimple_addToNode) {
  Model m;

  ThermalStoragePcmSimple ts(m);

  // TODO
}
