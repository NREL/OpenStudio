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

#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../ScheduleCompact.hpp"
#include "../ScheduleCompact_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"

#include "../CoilLiquidDesiccantSimple.hpp"
#include "../CoilLiquidDesiccantSimple_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilLiquidDesiccantSimple_CoilLiquidDesiccantSimple)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
    {
      Model m;

      ScheduleCompact s(m);

      CoilLiquidDesiccantSimple coil(m, s);

      exit(0);
    },
    ::testing::ExitedWithCode(0),
    ""
  );

  Model m;

  ScheduleCompact s(m);

  CoilLiquidDesiccantSimple coil(m, s);

  Schedule schedule = coil.availabilitySchedule();
  boost::optional<ScheduleCompact> scheduleCompact = schedule.optionalCast<ScheduleCompact>();
  ASSERT_TRUE(scheduleCompact);
  EXPECT_TRUE(coil.isDesignWaterFlowRateAutosized());
  EXPECT_TRUE(coil.isDesignAirFlowRateAutosized());
  EXPECT_TRUE(coil.isDesignInletWaterTemperatureAutosized());
  EXPECT_TRUE(coil.isDesignInletAirTemperatureAutosized());
  EXPECT_TRUE(coil.isDesignOutletAirTemperatureAutosized());
  EXPECT_TRUE(coil.isDesignInletAirHumidityRatioAutosized());
  EXPECT_TRUE(coil.isDesignOutletAirHumidityRatioAutosized());
  EXPECT_TRUE(coil.isDesignInletSolutionConcentrationAutosized());
  EXPECT_FALSE(coil.isDesignFanPowerPerUnitMassFlowRateAutosized());
  EXPECT_TRUE(coil.designFanPowerPerUnitMassFlowRate());
  EXPECT_TRUE(coil.isOutdoorAirFlowRatesAutosized());
  EXPECT_FALSE(coil.isDesignPumpPowerAutosized());
  EXPECT_TRUE(coil.designPumpPower());
  EXPECT_FALSE(coil.isDesignEffectivenessAtNormalConditionAutosized());
  EXPECT_TRUE(coil.designEffectivenessAtNormalCondition());
  EXPECT_TRUE(coil.isTypeOfOperationModeDefaulted());
  EXPECT_TRUE(coil.isAirSourceDefaulted());
  EXPECT_TRUE(coil.isMaterialDefaulted());
  EXPECT_FALSE(coil.designLiquidDesiccantConcentrationDifference());

  ASSERT_EQ((*coil.designFanPowerPerUnitMassFlowRate()), 50.0);
  ASSERT_EQ((*coil.designPumpPower()), 100.0);
  ASSERT_EQ((*coil.designEffectivenessAtNormalCondition()), 1.0);
  ASSERT_EQ("DehumidificationMode", coil.typeOfOperationMode());
  ASSERT_EQ("ZoneAirSource", coil.airSource());
  ASSERT_EQ("LiCl", coil.material());
}

TEST_F(ModelFixture, CoilLiquidDesiccantSimple_SetGetFields) {
  Model m;

  CoilLiquidDesiccantSimple coil(m);

  Schedule schedule = coil.availabilitySchedule();
  boost::optional<ScheduleConstant> scheduleConstant = schedule.optionalCast<ScheduleConstant>();
  ASSERT_TRUE(scheduleConstant);
  EXPECT_EQ((*scheduleConstant).value(), 1.0);
  EXPECT_EQ(m.alwaysOnDiscreteScheduleName(), schedule.nameString());

  ScheduleConstant sched(m);
  sched.setValue(0.6);

  EXPECT_TRUE(coil.setAvailabilitySchedule(sched));
  EXPECT_TRUE(coil.setDesignWaterFlowRate(0.5));
  EXPECT_TRUE(coil.setDesignAirFlowRate(1.0));
  EXPECT_TRUE(coil.setDesignInletWaterTemperature(1.5));
  EXPECT_TRUE(coil.setDesignInletAirTemperature(2.0));
  EXPECT_TRUE(coil.setDesignOutletAirTemperature(2.5));
  EXPECT_TRUE(coil.setDesignInletAirHumidityRatio(3.0));
  EXPECT_TRUE(coil.setDesignOutletAirHumidityRatio(3.5));
  EXPECT_TRUE(coil.setDesignInletSolutionConcentration(4.0));
  EXPECT_TRUE(coil.setDesignFanPowerPerUnitMassFlowRate(4.5));
  EXPECT_TRUE(coil.setOutdoorAirFlowRates(5.0));
  EXPECT_TRUE(coil.setDesignPumpPower(5.5));
  EXPECT_TRUE(coil.setDesignEffectivenessAtNormalCondition(6.0));
  EXPECT_TRUE(coil.setTypeOfOperationMode("RegenerationMode"));
  EXPECT_TRUE(coil.setAirSource("OutdoorAirSource"));
  EXPECT_TRUE(coil.setMaterial("CaCl2"));
  EXPECT_TRUE(coil.setDesignLiquidDesiccantConcentrationDifference(6.5));

  Schedule sch = coil.availabilitySchedule();
  boost::optional<ScheduleConstant> schConst = sch.optionalCast<ScheduleConstant>();
  ASSERT_TRUE(schConst);
  EXPECT_EQ((*schConst).value(), 0.6);

  EXPECT_FALSE(coil.isDesignWaterFlowRateAutosized());
  EXPECT_FALSE(coil.isDesignAirFlowRateAutosized());
  EXPECT_FALSE(coil.isDesignInletWaterTemperatureAutosized());
  EXPECT_FALSE(coil.isDesignInletAirTemperatureAutosized());
  EXPECT_FALSE(coil.isDesignOutletAirTemperatureAutosized());
  EXPECT_FALSE(coil.isDesignInletAirHumidityRatioAutosized());
  EXPECT_FALSE(coil.isDesignOutletAirHumidityRatioAutosized());
  EXPECT_FALSE(coil.isDesignInletSolutionConcentrationAutosized());
  EXPECT_FALSE(coil.isDesignFanPowerPerUnitMassFlowRateAutosized());
  EXPECT_FALSE(coil.isOutdoorAirFlowRatesAutosized());
  EXPECT_FALSE(coil.isDesignPumpPowerAutosized());
  EXPECT_FALSE(coil.isDesignEffectivenessAtNormalConditionAutosized());
  EXPECT_FALSE(coil.isTypeOfOperationModeDefaulted());
  EXPECT_FALSE(coil.isAirSourceDefaulted());
  EXPECT_FALSE(coil.isMaterialDefaulted());
  EXPECT_TRUE(coil.designLiquidDesiccantConcentrationDifference());

  EXPECT_EQ((*coil.designWaterFlowRate()), 0.5);
  EXPECT_EQ((*coil.designAirFlowRate()), 1.0);
  EXPECT_EQ((*coil.designInletWaterTemperature()), 1.5);
  EXPECT_EQ((*coil.designInletAirTemperature()), 2.0);
  EXPECT_EQ((*coil.designOutletAirTemperature()), 2.5);
  EXPECT_EQ((*coil.designInletAirHumidityRatio()), 3.0);
  EXPECT_EQ((*coil.designOutletAirHumidityRatio()), 3.5);
  EXPECT_EQ((*coil.designInletSolutionConcentration()), 4.0);
  EXPECT_EQ((*coil.designFanPowerPerUnitMassFlowRate()), 4.5);
  EXPECT_EQ((*coil.outdoorAirFlowRates()), 5.0);
  EXPECT_EQ((*coil.designPumpPower()), 5.5);
  EXPECT_EQ((*coil.designEffectivenessAtNormalCondition()), 6.0);
  EXPECT_EQ(coil.typeOfOperationMode(), "RegenerationMode");
  EXPECT_EQ(coil.airSource(), "OutdoorAirSource");
  EXPECT_EQ(coil.material(), "CaCl2");
  EXPECT_EQ((*coil.designLiquidDesiccantConcentrationDifference()), 6.5);

  coil.autosizeDesignWaterFlowRate();
  coil.autosizeDesignAirFlowRate();
  coil.autosizeDesignInletWaterTemperature();
  coil.autosizeDesignInletAirTemperature();
  coil.autosizeDesignOutletAirTemperature();
  coil.autosizeDesignInletAirHumidityRatio();
  coil.autosizeDesignOutletAirHumidityRatio();
  coil.autosizeDesignInletSolutionConcentration();
  coil.autosizeDesignFanPowerPerUnitMassFlowRate();
  coil.autosizeOutdoorAirFlowRates();
  coil.autosizeDesignPumpPower();
  coil.autosizeDesignEffectivenessAtNormalCondition();
  coil.resetTypeOfOperationMode();
  coil.resetAirSource();
  coil.resetMaterial();
  coil.resetDesignLiquidDesiccantConcentrationDifference();

  EXPECT_TRUE(coil.isDesignWaterFlowRateAutosized());
  EXPECT_TRUE(coil.isDesignAirFlowRateAutosized());
  EXPECT_TRUE(coil.isDesignInletWaterTemperatureAutosized());
  EXPECT_TRUE(coil.isDesignInletAirTemperatureAutosized());
  EXPECT_TRUE(coil.isDesignOutletAirTemperatureAutosized());
  EXPECT_TRUE(coil.isDesignInletAirHumidityRatioAutosized());
  EXPECT_TRUE(coil.isDesignOutletAirHumidityRatioAutosized());
  EXPECT_TRUE(coil.isDesignInletSolutionConcentrationAutosized());
  EXPECT_TRUE(coil.isDesignFanPowerPerUnitMassFlowRateAutosized());
  EXPECT_TRUE(coil.isOutdoorAirFlowRatesAutosized());
  EXPECT_TRUE(coil.isDesignPumpPowerAutosized());
  EXPECT_TRUE(coil.isDesignEffectivenessAtNormalConditionAutosized());
  EXPECT_TRUE(coil.isTypeOfOperationModeDefaulted());
  EXPECT_TRUE(coil.isAirSourceDefaulted());
  EXPECT_TRUE(coil.isMaterialDefaulted());
  EXPECT_FALSE(coil.designLiquidDesiccantConcentrationDifference());

  EXPECT_TRUE(coil.setAvailabilitySchedule(sched));
  EXPECT_TRUE(coil.setDesignWaterFlowRate(0.5));
  EXPECT_TRUE(coil.setDesignAirFlowRate(1.0));
  EXPECT_TRUE(coil.setDesignInletWaterTemperature(1.5));
  EXPECT_TRUE(coil.setDesignInletAirTemperature(2.0));
  EXPECT_TRUE(coil.setDesignOutletAirTemperature(2.5));
  EXPECT_TRUE(coil.setDesignInletAirHumidityRatio(3.0));
  EXPECT_TRUE(coil.setDesignOutletAirHumidityRatio(3.5));
  EXPECT_TRUE(coil.setDesignInletSolutionConcentration(4.0));
  EXPECT_TRUE(coil.setDesignFanPowerPerUnitMassFlowRate(4.5));
  EXPECT_TRUE(coil.setOutdoorAirFlowRates(5.0));
  EXPECT_TRUE(coil.setDesignPumpPower(5.5));
  EXPECT_TRUE(coil.setDesignEffectivenessAtNormalCondition(6.0));
  EXPECT_TRUE(coil.setTypeOfOperationMode("RegenerationMode"));
  EXPECT_TRUE(coil.setAirSource("OutdoorAirSource"));
  EXPECT_TRUE(coil.setMaterial("CaCl2"));
  EXPECT_TRUE(coil.setDesignLiquidDesiccantConcentrationDifference(6.5));

  coil.autosize();

  EXPECT_FALSE(coil.autosizedDesignWaterFlowRate());
  EXPECT_FALSE(coil.autosizedDesignAirFlowRate());
  EXPECT_FALSE(coil.autosizedDesignInletWaterTemperature());
  EXPECT_FALSE(coil.autosizedDesignInletAirTemperature());
  EXPECT_FALSE(coil.autosizedDesignOutletAirTemperature());
  EXPECT_FALSE(coil.autosizedDesignInletAirHumidityRatio());
  EXPECT_FALSE(coil.autosizedDesignOutletAirHumidityRatio());
  EXPECT_FALSE(coil.autosizedDesignInletSolutionConcentration());
  EXPECT_FALSE(coil.autosizedDesignFanPowerPerUnitMassFlowRate());
  EXPECT_FALSE(coil.autosizedOutdoorAirFlowRates());
  EXPECT_FALSE(coil.autosizedDesignPumpPower());
  EXPECT_FALSE(coil.autosizedDesignEffectivenessAtNormalCondition());
}

TEST_F(ModelFixture, CoilLiquidDesiccantSimple_Clone) {
  Model m;

  ScheduleCompact s(m);

  CoilLiquidDesiccantSimple coil(m, s);

  coil.setDesignInletAirTemperature(2.0);

  CoilLiquidDesiccantSimple coilClone = coil.clone(m).cast<CoilLiquidDesiccantSimple>();
  ASSERT_FALSE(coilClone.isDesignInletAirTemperatureAutosized());
  ASSERT_TRUE(coilClone.designInletAirTemperature());
  ASSERT_EQ((*coilClone.designInletAirTemperature()), 2.0);

  Model m2;
  CoilLiquidDesiccantSimple coilClone2 = coil.clone(m2).cast<CoilLiquidDesiccantSimple>();
  ASSERT_FALSE(coilClone.isDesignInletAirTemperatureAutosized());
  ASSERT_TRUE(coilClone.designInletAirTemperature());
  ASSERT_EQ((*coilClone.designInletAirTemperature()), 2.0);
}

TEST_F(ModelFixture, CoilLiquidDesiccantSimple_Remove) {
  Model m;
  auto size = m.modelObjects().size();
  CoilLiquidDesiccantSimple coil(m);
  EXPECT_FALSE(coil.remove().empty());
  EXPECT_EQ(size+2, m.modelObjects().size());
}