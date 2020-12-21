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
#include "../CoilChillerAirSourceVariableSpeed.hpp"
#include "../CoilChillerAirSourceVariableSpeed_Impl.hpp"
#include "../CoilChillerAirSourceVariableSpeedSpeedData.hpp"
#include "../CoilChillerAirSourceVariableSpeedSpeedData_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilChillerAirSourceVariableSpeed_CoilChillerAirSourceVariableSpeed) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CoilChillerAirSourceVariableSpeed coil(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;

  CoilChillerAirSourceVariableSpeed coil(m);

  EXPECT_EQ(1, coil.nominalSpeedLevel());
  EXPECT_FALSE(coil.ratedChilledWaterCapacity());
  EXPECT_TRUE(coil.isRatedChilledWaterCapacityAutosized());
  EXPECT_EQ(8.0, coil.ratedEvaporatorInletWaterTemperature());
  EXPECT_EQ(35.0, coil.ratedCondenserInletAirTemperature());
  EXPECT_FALSE(coil.ratedEvaporatorWaterFlowRate());
  EXPECT_TRUE(coil.isRatedEvaporatorWaterFlowRateAutocalculated());
  EXPECT_EQ("No", coil.evaporatorPumpPowerIncludedinRatedCOP());
  EXPECT_EQ("No", coil.evaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP());
  EXPECT_EQ(0.2, coil.fractionofEvaporatorPumpHeattoWater());
  EXPECT_EQ(0, coil.crankcaseHeaterCapacity());
  EXPECT_EQ(10, coil.maximumAmbientTemperatureforCrankcaseHeaterOperation());
  EXPECT_FALSE(coil.partLoadFractionCorrelationCurve());
  EXPECT_FALSE(coil.gridSignalSchedule());
  EXPECT_EQ(100, coil.lowerBoundToApplyGridResponsiveControl());
  EXPECT_TRUE(coil.isLowerBoundToApplyGridResponsiveControlDefaulted());
  EXPECT_EQ(-100.0, coil.upperBoundToApplyGridResponsiveControl());
  EXPECT_TRUE(coil.isUpperBoundToApplyGridResponsiveControlDefaulted());
  EXPECT_EQ(10, coil.maxSpeedLevelDuringGridResponsiveControl());
  EXPECT_TRUE(coil.isMaxSpeedLevelDuringGridResponsiveControlDefaulted());
}

TEST_F(ModelFixture, CoilChillerAirSourceVariableSpeed_SetGetFields) {
  Model m;

  CoilChillerAirSourceVariableSpeed coil(m);

  EXPECT_TRUE(coil.setNominalSpeedLevel(2));
  EXPECT_TRUE(coil.setRatedChilledWaterCapacity(150));
  EXPECT_TRUE(coil.setRatedEvaporatorInletWaterTemperature(200));
  EXPECT_TRUE(coil.setRatedCondenserInletAirTemperature(250));
  EXPECT_TRUE(coil.setRatedEvaporatorWaterFlowRate(300));
  EXPECT_TRUE(coil.setEvaporatorPumpPowerIncludedinRatedCOP("Yes"));
  EXPECT_TRUE(coil.setEvaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP("Yes"));
  EXPECT_TRUE(coil.setFractionofEvaporatorPumpHeattoWater(0.5));
  EXPECT_TRUE(coil.setCrankcaseHeaterCapacity(550));
  EXPECT_TRUE(coil.setMaximumAmbientTemperatureforCrankcaseHeaterOperation(600));
  auto curve = CurveQuadratic(m);
  EXPECT_TRUE(coil.setPartLoadFractionCorrelationCurve(curve));
  ScheduleConstant sch_const(m);
  sch_const.setValue(0.5);
  EXPECT_TRUE(coil.setGridSignalSchedule(sch_const));
  EXPECT_TRUE(coil.setLowerBoundToApplyGridResponsiveControl(50.0));
  EXPECT_TRUE(coil.setUpperBoundToApplyGridResponsiveControl(-50.0));
  EXPECT_TRUE(coil.setMaxSpeedLevelDuringGridResponsiveControl(5));

  EXPECT_EQ(2, coil.nominalSpeedLevel());
  ASSERT_TRUE(coil.ratedChilledWaterCapacity());
  EXPECT_EQ(150, coil.ratedChilledWaterCapacity().get());
  EXPECT_FALSE(coil.isRatedChilledWaterCapacityAutosized());
  EXPECT_EQ(200, coil.ratedEvaporatorInletWaterTemperature());
  EXPECT_EQ(250, coil.ratedCondenserInletAirTemperature());
  ASSERT_TRUE(coil.ratedEvaporatorWaterFlowRate());
  EXPECT_EQ(300, coil.ratedEvaporatorWaterFlowRate().get());
  EXPECT_FALSE(coil.isRatedEvaporatorWaterFlowRateAutocalculated());
  EXPECT_EQ("Yes", coil.evaporatorPumpPowerIncludedinRatedCOP());
  EXPECT_EQ("Yes", coil.evaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP());
  EXPECT_EQ(0.5, coil.fractionofEvaporatorPumpHeattoWater());
  EXPECT_EQ(550, coil.crankcaseHeaterCapacity());
  EXPECT_EQ(600, coil.maximumAmbientTemperatureforCrankcaseHeaterOperation());
  EXPECT_TRUE(coil.partLoadFractionCorrelationCurve());
  EXPECT_TRUE(coil.gridSignalSchedule());
  boost::optional<Schedule> _sch = coil.gridSignalSchedule();
  ASSERT_TRUE(_sch);
  Schedule sch = _sch.get();
  boost::optional<ScheduleConstant> scheduleConstant = sch.optionalCast<ScheduleConstant>();
  ASSERT_TRUE(scheduleConstant);
  EXPECT_EQ(0.5, (*scheduleConstant).value());
  EXPECT_EQ(50.0, coil.lowerBoundToApplyGridResponsiveControl());
  EXPECT_FALSE(coil.isLowerBoundToApplyGridResponsiveControlDefaulted());
  EXPECT_EQ(-50.0, coil.upperBoundToApplyGridResponsiveControl());
  EXPECT_FALSE(coil.isUpperBoundToApplyGridResponsiveControlDefaulted());
  EXPECT_EQ(5, coil.maxSpeedLevelDuringGridResponsiveControl());
  EXPECT_FALSE(coil.isMaxSpeedLevelDuringGridResponsiveControlDefaulted());

  coil.autosizeRatedChilledWaterCapacity();
  coil.autocalculateRatedEvaporatorWaterFlowRate();
  coil.resetPartLoadFractionCorrelationCurve();
  coil.resetGridSignalSchedule();
  coil.resetLowerBoundToApplyGridResponsiveControl();
  coil.resetUpperBoundToApplyGridResponsiveControl();
  coil.resetMaxSpeedLevelDuringGridResponsiveControl();

  EXPECT_FALSE(coil.ratedChilledWaterCapacity());
  EXPECT_TRUE(coil.isRatedChilledWaterCapacityAutosized());
  EXPECT_FALSE(coil.ratedEvaporatorWaterFlowRate());
  EXPECT_TRUE(coil.isRatedEvaporatorWaterFlowRateAutocalculated());
  EXPECT_FALSE(coil.partLoadFractionCorrelationCurve());
  EXPECT_FALSE(coil.gridSignalSchedule());
  EXPECT_TRUE(coil.isLowerBoundToApplyGridResponsiveControlDefaulted());
  EXPECT_TRUE(coil.isUpperBoundToApplyGridResponsiveControlDefaulted());
  EXPECT_TRUE(coil.isMaxSpeedLevelDuringGridResponsiveControlDefaulted());
}

TEST_F(ModelFixture, CoilChillerAirSourceVariableSpeed_Clone) {
  Model m;

  CoilChillerAirSourceVariableSpeed coil(m);

  coil.setRatedChilledWaterCapacity(75);

  CoilChillerAirSourceVariableSpeed coilClone = coil.clone(m).cast<CoilChillerAirSourceVariableSpeed>();
  ASSERT_EQ(75, coilClone.ratedChilledWaterCapacity().get());

  Model m2;
  CoilChillerAirSourceVariableSpeed coilClone2 = coil.clone(m2).cast<CoilChillerAirSourceVariableSpeed>();
  ASSERT_EQ(75, coilClone2.ratedChilledWaterCapacity().get());
}

TEST_F(ModelFixture, CoilChillerAirSourceVariableSpeed_Remove) {
  Model m;
  auto count = m.modelObjects().size();

  CoilChillerAirSourceVariableSpeed coil(m);
  CoilChillerAirSourceVariableSpeedSpeedData speed1(m);
  coil.addSpeed(speed1);
  coil.remove();

  auto curves = m.getModelObjects<model::Curve>();

  EXPECT_EQ(count, m.modelObjects().size() - curves.size());
}

TEST_F(ModelFixture, CoilChillerAirSourceVariableSpeed_Speeds) {
  Model m;
  
  CoilChillerAirSourceVariableSpeed coil(m);
  
  CoilChillerAirSourceVariableSpeedSpeedData speed1(m);
  coil.addSpeed(speed1);
  
  CoilChillerAirSourceVariableSpeedSpeedData speed2(m);
  coil.addSpeed(speed2);
  
  ASSERT_EQ(2u, coil.speeds().size());
}