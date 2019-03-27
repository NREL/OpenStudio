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

#include "../ElectricLoadCenterInverterLookUpTable.hpp"
#include "../Schedule.hpp"
#include "../ScheduleCompact.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ElectricLoadCenterInverterLookUpTable_Instantiate) {
  auto result = false;
  auto value = 0.1;

  Model model;
  ThermalZone thermalZone(model);

  ElectricLoadCenterInverterLookUpTable electricLoadCenterInverterLookUpTable(model);

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.thermalZone());
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.setThermalZone(thermalZone));
  ASSERT_TRUE(electricLoadCenterInverterLookUpTable.thermalZone());

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.availabilitySchedule());
  ScheduleCompact scheduleCompact(model);
  result = electricLoadCenterInverterLookUpTable.setAvailabilitySchedule(scheduleCompact);
  EXPECT_TRUE(result);
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.availabilitySchedule());
  electricLoadCenterInverterLookUpTable.resetAvailabilitySchedule();
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.availabilitySchedule());

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.radiativeFraction());
  result = electricLoadCenterInverterLookUpTable.setRadiativeFraction(value);
  EXPECT_TRUE(result);
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.radiativeFraction());
  EXPECT_EQ(electricLoadCenterInverterLookUpTable.radiativeFraction().get(), value);
  electricLoadCenterInverterLookUpTable.resetRadiativeFraction();
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.radiativeFraction());

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.ratedMaximumContinuousOutputPower());
  electricLoadCenterInverterLookUpTable.setRatedMaximumContinuousOutputPower(value);
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.ratedMaximumContinuousOutputPower());
  EXPECT_EQ(electricLoadCenterInverterLookUpTable.ratedMaximumContinuousOutputPower().get(), value);
  electricLoadCenterInverterLookUpTable.resetRatedMaximumContinuousOutputPower();
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.ratedMaximumContinuousOutputPower());

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.nightTareLossPower());
  electricLoadCenterInverterLookUpTable.setNightTareLossPower(value);
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.nightTareLossPower());
  EXPECT_EQ(electricLoadCenterInverterLookUpTable.nightTareLossPower().get(), value);
  electricLoadCenterInverterLookUpTable.resetNightTareLossPower();
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.nightTareLossPower());

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.nominalVoltageInput());
  electricLoadCenterInverterLookUpTable.setNominalVoltageInput(value);
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.nominalVoltageInput());
  EXPECT_EQ(electricLoadCenterInverterLookUpTable.nominalVoltageInput().get(), value);
  electricLoadCenterInverterLookUpTable.resetNominalVoltageInput();
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.nominalVoltageInput());

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt10PowerAndNominalVoltage());
  result = electricLoadCenterInverterLookUpTable.setEfficiencyAt10PowerAndNominalVoltage(value);
  EXPECT_TRUE(result);
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.efficiencyAt10PowerAndNominalVoltage());
  EXPECT_EQ(electricLoadCenterInverterLookUpTable.efficiencyAt10PowerAndNominalVoltage().get(), value);
  electricLoadCenterInverterLookUpTable.resetEfficiencyAt10PowerAndNominalVoltage();
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt10PowerAndNominalVoltage());

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt20PowerAndNominalVoltage());
  result = electricLoadCenterInverterLookUpTable.setEfficiencyAt20PowerAndNominalVoltage(value);
  EXPECT_TRUE(result);
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.efficiencyAt20PowerAndNominalVoltage());
  EXPECT_EQ(electricLoadCenterInverterLookUpTable.efficiencyAt20PowerAndNominalVoltage().get(), value);
  electricLoadCenterInverterLookUpTable.resetEfficiencyAt20PowerAndNominalVoltage();
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt20PowerAndNominalVoltage());

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt30PowerAndNominalVoltage());
  result = electricLoadCenterInverterLookUpTable.setEfficiencyAt30PowerAndNominalVoltage(value);
  EXPECT_TRUE(result);
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.efficiencyAt30PowerAndNominalVoltage());
  EXPECT_EQ(electricLoadCenterInverterLookUpTable.efficiencyAt30PowerAndNominalVoltage().get(), value);
  electricLoadCenterInverterLookUpTable.resetEfficiencyAt30PowerAndNominalVoltage();
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt30PowerAndNominalVoltage());

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt50PowerAndNominalVoltage());
  result = electricLoadCenterInverterLookUpTable.setEfficiencyAt50PowerAndNominalVoltage(value);
  EXPECT_TRUE(result);
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.efficiencyAt50PowerAndNominalVoltage());
  EXPECT_EQ(electricLoadCenterInverterLookUpTable.efficiencyAt50PowerAndNominalVoltage().get(), value);
  electricLoadCenterInverterLookUpTable.resetEfficiencyAt50PowerAndNominalVoltage();
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt50PowerAndNominalVoltage());

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt75PowerAndNominalVoltage());
  result = electricLoadCenterInverterLookUpTable.setEfficiencyAt75PowerAndNominalVoltage(value);
  EXPECT_TRUE(result);
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.efficiencyAt75PowerAndNominalVoltage());
  EXPECT_EQ(electricLoadCenterInverterLookUpTable.efficiencyAt75PowerAndNominalVoltage().get(), value);
  electricLoadCenterInverterLookUpTable.resetEfficiencyAt75PowerAndNominalVoltage();
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt75PowerAndNominalVoltage());

  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt100PowerAndNominalVoltage());
  result = electricLoadCenterInverterLookUpTable.setEfficiencyAt100PowerAndNominalVoltage(value);
  EXPECT_TRUE(result);
  EXPECT_TRUE(electricLoadCenterInverterLookUpTable.efficiencyAt100PowerAndNominalVoltage());
  EXPECT_EQ(electricLoadCenterInverterLookUpTable.efficiencyAt100PowerAndNominalVoltage().get(), value);
  electricLoadCenterInverterLookUpTable.resetEfficiencyAt100PowerAndNominalVoltage();
  EXPECT_FALSE(electricLoadCenterInverterLookUpTable.efficiencyAt100PowerAndNominalVoltage());

}

