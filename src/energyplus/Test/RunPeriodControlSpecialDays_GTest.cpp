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
#include "EnergyPlusFixture.hpp"

#include "../ErrorFile.hpp"
#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/RunPeriodControlSpecialDays.hpp"
#include "../../model/RunPeriodControlSpecialDays_Impl.hpp"

#include "../../utilities/time/Date.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/OS_RunPeriodControl_SpecialDays_FieldEnums.hxx>
#include <utilities/idd/RunPeriodControl_SpecialDays_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_RunPeriodControlSpecialDays)
{
  ForwardTranslator ft;

  // Create a model
  Model m;

  RunPeriodControlSpecialDays holiday(openstudio::MonthOfYear::Jul, 4, m);
  holiday.setName("4th of July");
  holiday.setSpecialDayType("Holiday");
  holiday.setDuration(1);

  {
    Workspace w = ft.translateModel(m);

    ASSERT_EQ(1u, w.getObjectsByType(IddObjectType::RunPeriodControl_SpecialDays).size());
    WorkspaceObject idf_holiday = w.getObjectsByType(IddObjectType::RunPeriodControl_SpecialDays)[0];
    EXPECT_EQ("4th of July", idf_holiday.nameString(false));
    EXPECT_EQ("7/4", idf_holiday.getString(RunPeriodControl_SpecialDaysFields::StartDate).get());
    EXPECT_EQ(1, idf_holiday.getInt(RunPeriodControl_SpecialDaysFields::Duration).get());
    EXPECT_EQ("Holiday", idf_holiday.getString(RunPeriodControl_SpecialDaysFields::SpecialDayType).get());
  }

  holiday.setName("A week off");
  EXPECT_TRUE(holiday.setStartDate(openstudio::NthDayOfWeekInMonth::first, openstudio::DayOfWeek::Monday, openstudio::MonthOfYear::Mar));
  EXPECT_TRUE(holiday.setDuration(5));
  EXPECT_TRUE(holiday.setSpecialDayType("CustomDay1"));

  {
    Workspace w = ft.translateModel(m);

    ASSERT_EQ(1u, w.getObjectsByType(IddObjectType::RunPeriodControl_SpecialDays).size());
    WorkspaceObject idf_holiday = w.getObjectsByType(IddObjectType::RunPeriodControl_SpecialDays)[0];
    EXPECT_EQ("A week off", idf_holiday.nameString(false));
    EXPECT_EQ("1st Monday in March", idf_holiday.getString(RunPeriodControl_SpecialDaysFields::StartDate).get());
    EXPECT_EQ(5, idf_holiday.getInt(RunPeriodControl_SpecialDaysFields::Duration).get());
    EXPECT_EQ("CustomDay1", idf_holiday.getString(RunPeriodControl_SpecialDaysFields::SpecialDayType).get());
  }

  // 5th replaced by Last
  EXPECT_TRUE(holiday.setStartDate(openstudio::NthDayOfWeekInMonth::fifth, openstudio::DayOfWeek::Tuesday, openstudio::MonthOfYear::Aug));
  {
    Workspace w = ft.translateModel(m);

    ASSERT_EQ(1u, w.getObjectsByType(IddObjectType::RunPeriodControl_SpecialDays).size());
    WorkspaceObject idf_holiday = w.getObjectsByType(IddObjectType::RunPeriodControl_SpecialDays)[0];
    EXPECT_EQ("Last Tuesday in August", idf_holiday.getString(RunPeriodControl_SpecialDaysFields::StartDate).get());
  }

}
