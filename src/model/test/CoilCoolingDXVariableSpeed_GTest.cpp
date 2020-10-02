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
#include "../CoilCoolingDXVariableSpeed.hpp"
#include "../CoilCoolingDXVariableSpeed_Impl.hpp"
#include "../Curve.hpp"
#include "../Curve_Impl.hpp"
#include "../CoilCoolingDXVariableSpeedSpeedData.hpp"
#include "../CoilCoolingDXVariableSpeedSpeedData_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,CoilCoolingDXVariableSpeed)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
     Model m;
     CoilCoolingDXVariableSpeed coil(m);

     exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, CoilCoolingDXVariableSpeed_Speeds)
{
  Model m;
  CoilCoolingDXVariableSpeed coil(m);

  CoilCoolingDXVariableSpeedSpeedData speed1( m );
  coil.addSpeed(speed1);

  CoilCoolingDXVariableSpeedSpeedData speed2( m );
  coil.addSpeed(speed2);

  ASSERT_EQ(2u,coil.speeds().size());

}

TEST_F(ModelFixture,CoilCoolingDXVariableSpeed_Remove)
{
  Model m;
  auto count = m.modelObjects().size();

  CoilCoolingDXVariableSpeed coil(m);
  CoilCoolingDXVariableSpeedSpeedData speed1( m );
  coil.addSpeed(speed1);
  coil.remove();

  auto curves = m.getModelObjects<model::Curve>();

  EXPECT_EQ(count,m.modelObjects().size() - curves.size());
}

TEST_F(ModelFixture,CoilCoolingDXVariableSpeed_MinOATCompressor)
{
  Model m;

  CoilCoolingDXVariableSpeed coil(m);

  // #3976 - Minimum Outdoor Dry-Bulb Temperature for Compressor Operation
  // IDD Default
  EXPECT_EQ(-25.0, coil.minimumOutdoorDryBulbTemperatureforCompressorOperation());
  // There are no IDD limits, so everything should work
  EXPECT_TRUE(coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-5));
  EXPECT_EQ(-5, coil.minimumOutdoorDryBulbTemperatureforCompressorOperation());
}

TEST_F(ModelFixture,CoilCoolingDXVariableSpeed_Grid)
{
  Model m;

  CoilCoolingDXVariableSpeed coil(m);

  EXPECT_FALSE(coil.gridSignalSchedule());
  ScheduleConstant schedule(m);
  schedule.setValue(0.5);
  coil.setGridSignalSchedule(schedule);
  EXPECT_TRUE(coil.gridSignalSchedule());
  coil.resetGridSignalSchedule();
  EXPECT_FALSE(coil.gridSignalSchedule());

  EXPECT_EQ(100.0, coil.lowerBoundToApplyGridResponsiveControl());
  EXPECT_TRUE(coil.isLowerBoundToApplyGridResponsiveControlDefaulted());
  EXPECT_TRUE(coil.setLowerBoundToApplyGridResponsiveControl(50.0));
  EXPECT_EQ(50.0, coil.lowerBoundToApplyGridResponsiveControl());
  EXPECT_FALSE(coil.isLowerBoundToApplyGridResponsiveControlDefaulted());
  coil.resetLowerBoundToApplyGridResponsiveControl();
  EXPECT_TRUE(coil.isLowerBoundToApplyGridResponsiveControlDefaulted());

  EXPECT_EQ(-100.0, coil.upperBoundToApplyGridResponsiveControl());
  EXPECT_TRUE(coil.isUpperBoundToApplyGridResponsiveControlDefaulted());
  EXPECT_TRUE(coil.setUpperBoundToApplyGridResponsiveControl(50.0));
  EXPECT_EQ(-50.0, coil.upperBoundToApplyGridResponsiveControl());
  EXPECT_FALSE(coil.isUpperBoundToApplyGridResponsiveControlDefaulted());
  coil.resetUpperBoundToApplyGridResponsiveControl();
  EXPECT_TRUE(coil.isUpperBoundToApplyGridResponsiveControlDefaulted());

  EXPECT_EQ(10, coil.maxSpeedLevelDuringGridResponsiveControl());
  EXPECT_TRUE(coil.isMaxSpeedLevelDuringGridResponsiveControlDefaulted());
  EXPECT_TRUE(coil.setMaxSpeedLevelDuringGridResponsiveControl(5));
  EXPECT_EQ(5, coil.maxSpeedLevelDuringGridResponsiveControl());
  EXPECT_FALSE(coil.isMaxSpeedLevelDuringGridResponsiveControlDefaulted());
  coil.resetMaxSpeedLevelDuringGridResponsiveControl();
  EXPECT_TRUE(coil.isMaxSpeedLevelDuringGridResponsiveControlDefaulted());

  EXPECT_EQ("SenLat", coil.loadControlDuringGridResponsiveControl());
  EXPECT_TRUE(coil.isLoadControlDuringGridResponsiveControlDefaulted());
  EXPECT_TRUE(coil.setLoadControlDuringGridResponsiveControl("Sensible"));
  EXPECT_EQ("Sensible", coil.loadControlDuringGridResponsiveControl());
  EXPECT_FALSE(coil.isLoadControlDuringGridResponsiveControlDefaulted());
  coil.resetLoadControlDuringGridResponsiveControl();
  EXPECT_TRUE(coil.isLoadControlDuringGridResponsiveControlDefaulted());
}