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
#include "../CoilSystemIntegratedHeatPumpAirSource.hpp"
#include "../CoilSystemIntegratedHeatPumpAirSource_Impl.hpp"
#include "../CoilCoolingDXVariableSpeed.hpp"
#include "../CoilCoolingDXVariableSpeed_Impl.hpp"
#include "../CoilHeatingDXVariableSpeed.hpp"
#include "../CoilHeatingDXVariableSpeed_Impl.hpp"
#include "../CoilChillerAirSourceVariableSpeed.hpp"
#include "../CoilChillerAirSourceVariableSpeed_Impl.hpp"
#include "../CoilCoolingWater.hpp"
#include "../CoilCoolingWater_Impl.hpp"
#include "../ThermalStorageIceDetailed.hpp"
#include "../ThermalStorageIceDetailed_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilSystemIntegratedHeatPumpAirSource_CoilSystemIntegratedHeatPumpAirSource) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CoilSystemIntegratedHeatPumpAirSource coil(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;

  CoilSystemIntegratedHeatPumpAirSource coilSystem(m);

  boost::optional<StraightComponent> coolingCoil = coilSystem.coolingCoil().optionalCast<StraightComponent>();
  EXPECT_TRUE(coolingCoil);
  EXPECT_FALSE(coilSystem.heatingCoil());
  EXPECT_FALSE(coilSystem.chillingCoil());
  EXPECT_FALSE(coilSystem.supplementalChillingCoil());
  EXPECT_FALSE(coilSystem.storageTank());
}

TEST_F(ModelFixture, CoilSystemIntegratedHeatPumpAirSource_SetGetFields) {
  Model m;

  CoilSystemIntegratedHeatPumpAirSource coilSystem(m);

  CoilCoolingDXVariableSpeed coolingCoil(m);
  CoilHeatingDXVariableSpeed heatingCoil(m);
  CoilChillerAirSourceVariableSpeed chillingCoil(m);
  CoilCoolingWater supplementalChillingCoil(m);
  ThermalStorageIceDetailed ts(m);

  coilSystem.setCoolingCoil(coolingCoil);
  coilSystem.setHeatingCoil(heatingCoil);
  coilSystem.setChillingCoil(chillingCoil);
  coilSystem.setSupplementalChillingCoil(supplementalChillingCoil);
  coilSystem.setStorageTank(ts);

  EXPECT_EQ(coolingCoil.name().get(), coilSystem.coolingCoil().name().get());
  EXPECT_TRUE(coilSystem.heatingCoil());
  EXPECT_TRUE(coilSystem.chillingCoil());
  EXPECT_TRUE(coilSystem.supplementalChillingCoil());
  EXPECT_TRUE(coilSystem.storageTank());

  coilSystem.resetHeatingCoil();
  coilSystem.resetChillingCoil();
  coilSystem.resetSupplementalChillingCoil();
  coilSystem.resetStorageTank();

  EXPECT_FALSE(coilSystem.heatingCoil());
  EXPECT_FALSE(coilSystem.chillingCoil());
  EXPECT_FALSE(coilSystem.supplementalChillingCoil());
  EXPECT_FALSE(coilSystem.storageTank());
}

TEST_F(ModelFixture, CoilSystemIntegratedHeatPumpAirSource_Clone) {
  Model m;

  CoilSystemIntegratedHeatPumpAirSource coilSystem(m);

  CoilHeatingDXVariableSpeed heatingCoil(m);
  coilSystem.setHeatingCoil(heatingCoil);

  CoilSystemIntegratedHeatPumpAirSource coilSystemClone = coilSystem.clone(m).cast<CoilSystemIntegratedHeatPumpAirSource>();
  ASSERT_TRUE(coilSystemClone.heatingCoil());

  Model m2;
  CoilSystemIntegratedHeatPumpAirSource coilSystemClone2 = coilSystem.clone(m2).cast<CoilSystemIntegratedHeatPumpAirSource>();
  ASSERT_TRUE(coilSystemClone2.heatingCoil());
}

TEST_F(ModelFixture, CoilSystemIntegratedHeatPumpAirSource_Remove) {
  Model m;

  auto size = m.modelObjects().size();

  CoilSystemIntegratedHeatPumpAirSource coilSystem(m);

  EXPECT_EQ(size + 4, m.modelObjects().size());  // 4: CoilSystem, Coil, Curve, SpeedData
  EXPECT_FALSE(coilSystem.remove().empty());
  EXPECT_EQ(size + 1, m.modelObjects().size());  // 1: Curve
}
