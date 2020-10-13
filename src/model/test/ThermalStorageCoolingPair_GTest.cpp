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

#include "../ThermalStorageCoolingPair.hpp"
#include "../ThermalStorageCoolingPair_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,ThermalStorageCoolingPair) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
    {
       Model m;
       ThermalStorageCoolingPair ts(m);

       exit(0);
    },
    ::testing::ExitedWithCode(0),
    ""
  );

  Model m;

  ThermalStorageCoolingPair ts(m);

  EXPECT_EQ(0.0, ts.maximumPeakOperationHours());
  EXPECT_EQ(0.0, ts.temperatureOrConcentrationChangeInTankThroughOperation());
  EXPECT_TRUE(ts.isLoadTypeDefaulted());
  EXPECT_EQ("Total", ts.loadType());
  EXPECT_TRUE(ts.isCapacityRatioOfRecoveryUnitToMainCoolingCoilDefaulted());
  EXPECT_EQ(1.0, ts.capacityRatioOfRecoveryUnitToMainCoolingCoil());
}

TEST_F(ModelFixture, ThermalStorageCoolingPair_SetGetFields) {
  Model m;

  ThermalStorageCoolingPair ts(m);

  EXPECT_TRUE(ts.setMaximumPeakOperationHours(2.5));
  EXPECT_TRUE(ts.setTemperatureOrConcentrationChangeInTankThroughOperation(4.5));
  EXPECT_TRUE(ts.setLoadType("Latent"));
  EXPECT_TRUE(ts.setCapacityRatioOfRecoveryUnitToMainCoolingCoil(1.5));

  EXPECT_EQ(2.5, ts.maximumPeakOperationHours());
  EXPECT_EQ(4.5, ts.temperatureOrConcentrationChangeInTankThroughOperation());
  EXPECT_FALSE(ts.isLoadTypeDefaulted());
  EXPECT_EQ("Latent", ts.loadType());
  EXPECT_FALSE(ts.isCapacityRatioOfRecoveryUnitToMainCoolingCoilDefaulted());
  EXPECT_EQ(1.5, ts.capacityRatioOfRecoveryUnitToMainCoolingCoil());

  ts.resetLoadType();
  ts.resetCapacityRatioOfRecoveryUnitToMainCoolingCoil();

  EXPECT_TRUE(ts.isLoadTypeDefaulted());
  EXPECT_EQ("Total", ts.loadType());
  EXPECT_TRUE(ts.isCapacityRatioOfRecoveryUnitToMainCoolingCoilDefaulted());
  EXPECT_EQ(1.0, ts.capacityRatioOfRecoveryUnitToMainCoolingCoil());
}

TEST_F(ModelFixture, ThermalStorageCoolingPair_Clone) {
  Model m;

  ThermalStorageCoolingPair ts(m);

  ts.setCapacityRatioOfRecoveryUnitToMainCoolingCoil(2.0);

  ThermalStorageCoolingPair tsClone = ts.clone(m).cast<ThermalStorageCoolingPair>();
  ASSERT_EQ(0.0, tsClone.maximumPeakOperationHours());
  ASSERT_FALSE(tsClone.isCapacityRatioOfRecoveryUnitToMainCoolingCoilDefaulted());

  Model m2;
  ThermalStorageCoolingPair tsClone2 = ts.clone(m2).cast<ThermalStorageCoolingPair>();
  ASSERT_EQ(0.0, tsClone2.maximumPeakOperationHours());
  ASSERT_FALSE(tsClone2.isCapacityRatioOfRecoveryUnitToMainCoolingCoilDefaulted());
}

TEST_F(ModelFixture, ThermalStorageCoolingPair_Remove) {
  Model m;

  auto size = m.modelObjects().size();

  ThermalStorageCoolingPair ts(m);

  EXPECT_EQ(size+1, m.modelObjects().size());
  EXPECT_FALSE(ts.remove().empty());
  EXPECT_EQ(size, m.modelObjects().size());
}
