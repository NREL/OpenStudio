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

#include "../CoilCoolingDXCurveFitPerformance.hpp"
#include "../CoilCoolingDXCurveFitPerformance_Impl.hpp"

#include "../CoilCoolingDX.hpp"
#include "../CoilCoolingDX_Impl.hpp"
#include "../CoilCoolingDXCurveFitOperatingMode.hpp"
#include "../CoilCoolingDXCurveFitOperatingMode_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilCoolingDXCurveFitPerformance_CoilCoolingDXCurveFitPerformance) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      // create a model to use
      Model model;

      // create a coil cooling dx curve fit operating mode object to use
      CoilCoolingDXCurveFitOperatingMode operatingMode(model);

      // create a coil cooling dx curve fit performance object to use
      CoilCoolingDXCurveFitPerformance performance(model, operatingMode);

      exit(0);
    },
    ::testing::ExitedWithCode(0),
    ""
  );

  // create a model to use
  Model model;

  // create a coil cooling dx curve fit operating mode object to use
  CoilCoolingDXCurveFitOperatingMode operatingMode(model);

  // create a coil cooling dx curve fit performance object to use
  CoilCoolingDXCurveFitPerformance performance(model, operatingMode);

  EXPECT_EQ(0.0, performance.crankcaseHeaterCapacity());
  EXPECT_EQ(-25.0, performance.minimumOutdoorDryBulbTemperatureforCompressorOperation());
  EXPECT_EQ(10.0, performance.maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation());
  EXPECT_EQ(773.3, performance.unitInternalStaticAirPressure());
  EXPECT_EQ("Discrete", performance.capacityControlMethod());
  EXPECT_EQ(0.0, performance.evaporativeCondenserBasinHeaterCapacity());
  EXPECT_EQ(2.0, performance.evaporativeCondenserBasinHeaterSetpointTemperature());
  ASSERT_TRUE(performance.evaporativeCondenserBasinHeaterOperatingSchedule().optionalCast<Schedule>());
  ASSERT_TRUE(performance.evaporativeCondenserBasinHeaterOperatingSchedule().optionalCast<ScheduleConstant>());
  ScheduleConstant schedule = performance.evaporativeCondenserBasinHeaterOperatingSchedule().cast<ScheduleConstant>();
  EXPECT_EQ(1.0, schedule.value());
  ASSERT_TRUE(performance.baseOperatingMode().optionalCast<CoilCoolingDXCurveFitOperatingMode>());
  ASSERT_FALSE(performance.alternativeOperatingMode1());
  ASSERT_FALSE(performance.alternativeOperatingMode2());
  EXPECT_EQ(0, performance.coilCoolingDXs().size());
}

TEST_F(ModelFixture, CoilCoolingDXCurveFitPerformance_GettersSetters) {
  // create a model to use
  Model model;

  // create a coil cooling dx curve fit operating mode object to use
  CoilCoolingDXCurveFitOperatingMode operatingMode(model);

  // create a coil cooling dx curve fit performance object to use
  CoilCoolingDXCurveFitPerformance performance(model, operatingMode);
}
