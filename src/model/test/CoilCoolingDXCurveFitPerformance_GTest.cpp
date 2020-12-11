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

#include <algorithm>

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
    ::testing::ExitedWithCode(0), "");

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

  EXPECT_EQ(performance.crankcaseHeaterCapacity(), 0.0);
  EXPECT_EQ(performance.minimumOutdoorDryBulbTemperatureforCompressorOperation(), -25.0);
  EXPECT_EQ(performance.maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(), 10.0);
  EXPECT_EQ(performance.unitInternalStaticAirPressure(), 773.3);
  EXPECT_EQ(performance.capacityControlMethod(), "Discrete");
  EXPECT_EQ(performance.evaporativeCondenserBasinHeaterCapacity(), 0.0);
  EXPECT_EQ(performance.evaporativeCondenserBasinHeaterSetpointTemperature(), 2.0);
  Schedule schedule = performance.evaporativeCondenserBasinHeaterOperatingSchedule();
  boost::optional<ScheduleConstant> scheduleConstant = schedule.optionalCast<ScheduleConstant>();
  ASSERT_TRUE(scheduleConstant);
  EXPECT_EQ((*scheduleConstant).value(), 1.0);
  EXPECT_EQ(performance.compressorFuelType(), "Electricity");
  EXPECT_EQ(performance.baseOperatingMode(), operatingMode);
  ASSERT_FALSE(performance.alternativeOperatingMode1());
  ASSERT_FALSE(performance.alternativeOperatingMode2());
  EXPECT_EQ(performance.coilCoolingDXs().size(), 0u);

  performance.setCrankcaseHeaterCapacity(1.0);
  performance.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-30.0);
  performance.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(15.0);
  performance.setUnitInternalStaticAirPressure(800.0);
  performance.setCapacityControlMethod("Continuous");
  performance.setEvaporativeCondenserBasinHeaterCapacity(1.0);
  performance.setEvaporativeCondenserBasinHeaterSetpointTemperature(3.0);
  ScheduleConstant scheduleConstant2(model);
  scheduleConstant2.setValue(0.5);
  performance.setEvaporativeCondenserBasinHeaterOperatingSchedule(scheduleConstant2);
  performance.setCompressorFuelType("NaturalGas");
  CoilCoolingDXCurveFitOperatingMode operatingMode2(model);
  performance.setBaseOperatingMode(operatingMode2);
  CoilCoolingDXCurveFitOperatingMode alternative1(model);
  performance.setAlternativeOperatingMode1(alternative1);
  CoilCoolingDXCurveFitOperatingMode alternative2(model);
  performance.setAlternativeOperatingMode2(alternative2);

  EXPECT_EQ(performance.crankcaseHeaterCapacity(), 1.0);
  EXPECT_EQ(performance.minimumOutdoorDryBulbTemperatureforCompressorOperation(), -30.0);
  EXPECT_EQ(performance.maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(), 15.0);
  EXPECT_EQ(performance.unitInternalStaticAirPressure(), 800.0);
  EXPECT_EQ(performance.capacityControlMethod(), "Continuous");
  EXPECT_EQ(performance.evaporativeCondenserBasinHeaterCapacity(), 1.0);
  EXPECT_EQ(performance.evaporativeCondenserBasinHeaterSetpointTemperature(), 3.0);
  Schedule schedule2 = performance.evaporativeCondenserBasinHeaterOperatingSchedule();
  boost::optional<ScheduleConstant> scheduleConstant3 = schedule2.optionalCast<ScheduleConstant>();
  ASSERT_TRUE(scheduleConstant3);
  EXPECT_EQ((*scheduleConstant3).value(), 0.5);
  EXPECT_EQ(performance.compressorFuelType(), "NaturalGas");
  EXPECT_EQ(performance.baseOperatingMode(), operatingMode2);
  ASSERT_TRUE(performance.alternativeOperatingMode1());
  ASSERT_TRUE(performance.alternativeOperatingMode2());
}

TEST_F(ModelFixture, CoilCoolingDXCurveFitPerformance_coilCoolingDXs) {
  Model model;

  CoilCoolingDXCurveFitOperatingMode operatingMode(model);
  CoilCoolingDXCurveFitPerformance performance(model, operatingMode);
  CoilCoolingDX dx1(model, performance);

  EXPECT_EQ(1u, performance.directUseCount());
  auto coilCoolingDXs = performance.coilCoolingDXs();
  ASSERT_EQ(1u, coilCoolingDXs.size());
  EXPECT_EQ(dx1, coilCoolingDXs[0]);

  CoilCoolingDX dx2(model, performance);
  EXPECT_EQ(2u, performance.directUseCount());
  coilCoolingDXs = performance.coilCoolingDXs();
  ASSERT_EQ(2u, coilCoolingDXs.size());
  EXPECT_TRUE(std::find(coilCoolingDXs.begin(), coilCoolingDXs.end(), dx1) != coilCoolingDXs.end());
  EXPECT_TRUE(std::find(coilCoolingDXs.begin(), coilCoolingDXs.end(), dx2) != coilCoolingDXs.end());

  CoilCoolingDXCurveFitPerformance performance2(model, operatingMode);
  EXPECT_TRUE(dx1.setPerformanceObject(performance2));
  EXPECT_EQ(1u, performance2.directUseCount());
  coilCoolingDXs = performance2.coilCoolingDXs();
  ASSERT_EQ(1u, coilCoolingDXs.size());
  EXPECT_EQ(dx1, coilCoolingDXs[0]);

  EXPECT_EQ(1u, performance.directUseCount());
  coilCoolingDXs = performance.coilCoolingDXs();
  ASSERT_EQ(1u, coilCoolingDXs.size());
  EXPECT_EQ(dx2, coilCoolingDXs[0]);
}

TEST_F(ModelFixture, CoilCoolingDXCurveFitPerformance_clone) {
  Model model;

  CoilCoolingDXCurveFitOperatingMode baseOperatingMode(model);
  CoilCoolingDXCurveFitOperatingMode alternativeOperatingMode1(model);
  CoilCoolingDXCurveFitOperatingMode alternativeOperatingMode2(model);
  CoilCoolingDXCurveFitPerformance performance(model, baseOperatingMode);
  CoilCoolingDX dx(model, performance);

  EXPECT_EQ(performance, dx.performanceObject());
  EXPECT_EQ(baseOperatingMode, performance.baseOperatingMode());
  EXPECT_FALSE(performance.alternativeOperatingMode1());
  EXPECT_FALSE(performance.alternativeOperatingMode2());

  EXPECT_TRUE(performance.setAlternativeOperatingMode1(alternativeOperatingMode1));
  EXPECT_EQ(performance, dx.performanceObject());
  EXPECT_EQ(baseOperatingMode, performance.baseOperatingMode());
  ASSERT_TRUE(performance.alternativeOperatingMode1());
  EXPECT_EQ(alternativeOperatingMode1, performance.alternativeOperatingMode1());
  EXPECT_FALSE(performance.alternativeOperatingMode2());

  EXPECT_TRUE(performance.setAlternativeOperatingMode2(alternativeOperatingMode2));
  EXPECT_EQ(performance, dx.performanceObject());
  EXPECT_EQ(baseOperatingMode, performance.baseOperatingMode());
  ASSERT_TRUE(performance.alternativeOperatingMode1());
  EXPECT_EQ(alternativeOperatingMode1, performance.alternativeOperatingMode1());
  ASSERT_TRUE(performance.alternativeOperatingMode2());
  EXPECT_EQ(alternativeOperatingMode2, performance.alternativeOperatingMode2());

  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDX>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitPerformance>().size());
  EXPECT_EQ(3u, model.getConcreteModelObjects<CoilCoolingDXCurveFitOperatingMode>().size());

  auto performanceClone = performance.clone(model).cast<CoilCoolingDXCurveFitPerformance>();
  EXPECT_EQ(performance, dx.performanceObject());
  EXPECT_EQ(baseOperatingMode, performance.baseOperatingMode());
  EXPECT_EQ(alternativeOperatingMode1, performance.alternativeOperatingMode1());
  EXPECT_EQ(alternativeOperatingMode2, performance.alternativeOperatingMode2());
  EXPECT_EQ(baseOperatingMode, performanceClone.baseOperatingMode());
  EXPECT_EQ(alternativeOperatingMode1, performanceClone.alternativeOperatingMode1());
  EXPECT_EQ(alternativeOperatingMode2, performanceClone.alternativeOperatingMode2());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDX>().size());
  EXPECT_EQ(2u, model.getConcreteModelObjects<CoilCoolingDXCurveFitPerformance>().size());
  EXPECT_EQ(3u, model.getConcreteModelObjects<CoilCoolingDXCurveFitOperatingMode>().size());

  // This shouldn't work, it's going to put the CoilCoolingDX in a broken state
  auto rmed = performance.remove();
  EXPECT_EQ(0u, rmed.size());
  EXPECT_NO_THROW(dx.performanceObject());
  CoilCoolingDXCurveFitOperatingMode anotherOperatingMode(model);
  CoilCoolingDXCurveFitPerformance anotherPerformance(model, anotherOperatingMode);
  EXPECT_TRUE(dx.setPerformanceObject(anotherPerformance));
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDX>().size());
  EXPECT_EQ(3u, model.getConcreteModelObjects<CoilCoolingDXCurveFitPerformance>().size());
  EXPECT_EQ(4u, model.getConcreteModelObjects<CoilCoolingDXCurveFitOperatingMode>().size());

  rmed = performance.remove();
  EXPECT_EQ(1u, rmed.size());
  EXPECT_EQ(baseOperatingMode, performanceClone.baseOperatingMode());
  EXPECT_EQ(alternativeOperatingMode1, performanceClone.alternativeOperatingMode1());
  EXPECT_EQ(alternativeOperatingMode2, performanceClone.alternativeOperatingMode2());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDX>().size());
  EXPECT_EQ(2u, model.getConcreteModelObjects<CoilCoolingDXCurveFitPerformance>().size());
  EXPECT_EQ(4u, model.getConcreteModelObjects<CoilCoolingDXCurveFitOperatingMode>().size());

  rmed = performanceClone.remove();
  EXPECT_EQ(4u, rmed.size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDX>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitPerformance>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<CoilCoolingDXCurveFitOperatingMode>().size());
}
