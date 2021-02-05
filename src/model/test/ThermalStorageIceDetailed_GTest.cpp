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

#include "../ThermalStorageIceDetailed.hpp"
#include "../ThermalStorageIceDetailed_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ThermalStorageIceDetailed_ThermalStorageIceDetailed) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      ThermalStorageIceDetailed ts(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;

  ThermalStorageIceDetailed ts(m);

  EXPECT_FALSE(ts.availabilitySchedule());
  EXPECT_EQ(0.5, ts.capacity());
  EXPECT_EQ("FractionDischargedLMTD", ts.dischargingCurveVariableSpecifications());
  EXPECT_TRUE(ts.dischargingCurve().optionalCast<CurveQuadraticLinear>());
  EXPECT_EQ("FractionChargedLMTD", ts.chargingCurveVariableSpecifications());
  EXPECT_TRUE(ts.chargingCurve().optionalCast<CurveQuadraticLinear>());
  EXPECT_EQ(1.0, ts.timestepoftheCurveData());
  EXPECT_EQ(0.0001, ts.parasiticElectricLoadDuringDischarging());
  EXPECT_EQ(0.0002, ts.parasiticElectricLoadDuringCharging());
  EXPECT_EQ(0.0003, ts.tankLossCoefficient());
  EXPECT_EQ(0.0, ts.freezingTemperatureofStorageMedium());
  EXPECT_EQ("OutsideMelt", ts.thawProcessIndicator());

  EXPECT_FALSE(ts.containingHVACComponent());
}

TEST_F(ModelFixture, ThermalStorageIceDetailed_SetGetFields) {
  Model m;

  ThermalStorageIceDetailed ts(m);

  ScheduleConstant schedule(m);
  CurveQuadraticLinear dischargingCurve(m);
  CurveQuadraticLinear chargingCurve(m);

  EXPECT_TRUE(ts.setAvailabilitySchedule(schedule));
  EXPECT_TRUE(ts.setCapacity(0.6));
  EXPECT_TRUE(ts.setDischargingCurveVariableSpecifications("LMTDMassFlow"));
  EXPECT_TRUE(ts.setDischargingCurve(dischargingCurve));
  EXPECT_TRUE(ts.setChargingCurveVariableSpecifications("LMTDFractionCharged"));
  EXPECT_TRUE(ts.setChargingCurve(chargingCurve));
  EXPECT_TRUE(ts.setTimestepoftheCurveData(15.0));
  EXPECT_TRUE(ts.setParasiticElectricLoadDuringDischarging(0.005));
  EXPECT_TRUE(ts.setParasiticElectricLoadDuringCharging(0.0025));
  EXPECT_TRUE(ts.setTankLossCoefficient(0.0055));
  EXPECT_TRUE(ts.setFreezingTemperatureofStorageMedium(0.3));
  EXPECT_TRUE(ts.setThawProcessIndicator("InsideMelt"));

  EXPECT_TRUE(ts.availabilitySchedule());
  EXPECT_EQ(0.6, ts.capacity());
  EXPECT_EQ("LMTDMassFlow", ts.dischargingCurveVariableSpecifications());
  EXPECT_TRUE(ts.dischargingCurve().optionalCast<CurveQuadraticLinear>());
  EXPECT_EQ("LMTDFractionCharged", ts.chargingCurveVariableSpecifications());
  EXPECT_TRUE(ts.chargingCurve().optionalCast<CurveQuadraticLinear>());
  EXPECT_EQ(15.0, ts.timestepoftheCurveData());
  EXPECT_EQ(0.005, ts.parasiticElectricLoadDuringDischarging());
  EXPECT_EQ(0.0025, ts.parasiticElectricLoadDuringCharging());
  EXPECT_EQ(0.0055, ts.tankLossCoefficient());
  EXPECT_EQ(0.3, ts.freezingTemperatureofStorageMedium());
  EXPECT_EQ("InsideMelt", ts.thawProcessIndicator());

  ts.resetAvailabilitySchedule();

  EXPECT_FALSE(ts.availabilitySchedule());
}

TEST_F(ModelFixture, ThermalStorageIceDetailed_Clone) {
  Model m;

  ThermalStorageIceDetailed ts(m);

  ts.setCapacity(0.7);

  ThermalStorageIceDetailed tsClone = ts.clone(m).cast<ThermalStorageIceDetailed>();
  ASSERT_EQ(0.7, tsClone.capacity());

  Model m2;
  ThermalStorageIceDetailed tsClone2 = ts.clone(m2).cast<ThermalStorageIceDetailed>();
  ASSERT_EQ(0.7, tsClone2.capacity());
}

TEST_F(ModelFixture, ThermalStorageIceDetailed_Remove) {
  Model m;

  auto size = m.modelObjects().size();

  ThermalStorageIceDetailed ts(m);

  EXPECT_EQ(size + 1, m.modelObjects().size());
  EXPECT_FALSE(ts.remove().empty());
  EXPECT_EQ(size, m.modelObjects().size());
}

TEST_F(ModelFixture, ThermalStorageIceDetailed_addToNode) {
  Model m;

  ThermalStorageIceDetailed ts(m);

  // TODO
}
