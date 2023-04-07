/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ModelFixture.hpp"

#include "../SpaceInfiltrationFlowCoefficient.hpp"
#include "../SpaceInfiltrationFlowCoefficient_Impl.hpp"

#include "../Space.hpp"
#include "../SpaceType.hpp"
#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../DefaultScheduleSet.hpp"

#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SpaceInfiltrationFlowCoefficient_GettersSetters) {
  Model m;
  SpaceInfiltrationFlowCoefficient spaceInfiltrationFlowCoefficient(m);

  spaceInfiltrationFlowCoefficient.setName("My SpaceInfiltrationFlowCoefficient");

  // Schedule Name: Object
  ScheduleConstant sch(m);
  EXPECT_FALSE(spaceInfiltrationFlowCoefficient.schedule());
  EXPECT_FALSE(sch.scheduleTypeLimits());
  EXPECT_TRUE(spaceInfiltrationFlowCoefficient.setSchedule(sch));
  auto schTypeLim_ = sch.scheduleTypeLimits();
  ASSERT_TRUE(schTypeLim_);
  EXPECT_EQ(0.0, schTypeLim_->lowerLimitValue().get());
  EXPECT_EQ(1.0, schTypeLim_->upperLimitValue().get());
  EXPECT_EQ("Continuous", schTypeLim_->numericType().get());

  EXPECT_EQ(sch, spaceInfiltrationFlowCoefficient.schedule().get());
  spaceInfiltrationFlowCoefficient.resetSchedule();
  EXPECT_FALSE(spaceInfiltrationFlowCoefficient.schedule());

  // Flow Coefficient: Required Double
  EXPECT_TRUE(spaceInfiltrationFlowCoefficient.setFlowCoefficient(0.1));
  EXPECT_EQ(0.1, spaceInfiltrationFlowCoefficient.flowCoefficient());
  // Bad Value
  EXPECT_FALSE(spaceInfiltrationFlowCoefficient.setFlowCoefficient(-10.0));
  EXPECT_EQ(0.1, spaceInfiltrationFlowCoefficient.flowCoefficient());

  // Stack Coefficient: Required Double
  EXPECT_TRUE(spaceInfiltrationFlowCoefficient.setStackCoefficient(0.2));
  EXPECT_EQ(0.2, spaceInfiltrationFlowCoefficient.stackCoefficient());
  // Bad Value
  EXPECT_FALSE(spaceInfiltrationFlowCoefficient.setStackCoefficient(-10.0));
  EXPECT_EQ(0.2, spaceInfiltrationFlowCoefficient.stackCoefficient());

  // Pressure Exponent: Required Double
  EXPECT_TRUE(spaceInfiltrationFlowCoefficient.setPressureExponent(0.3));
  EXPECT_EQ(0.3, spaceInfiltrationFlowCoefficient.pressureExponent());
  // Bad Value
  EXPECT_FALSE(spaceInfiltrationFlowCoefficient.setPressureExponent(-10.0));
  EXPECT_EQ(0.3, spaceInfiltrationFlowCoefficient.pressureExponent());

  // Wind Coefficient: Required Double
  EXPECT_TRUE(spaceInfiltrationFlowCoefficient.setWindCoefficient(0.4));
  EXPECT_EQ(0.4, spaceInfiltrationFlowCoefficient.windCoefficient());
  // Bad Value
  EXPECT_FALSE(spaceInfiltrationFlowCoefficient.setWindCoefficient(-10.0));
  EXPECT_EQ(0.4, spaceInfiltrationFlowCoefficient.windCoefficient());

  // Shelter Factor: Required Double
  EXPECT_TRUE(spaceInfiltrationFlowCoefficient.setShelterFactor(0.5));
  EXPECT_EQ(0.5, spaceInfiltrationFlowCoefficient.shelterFactor());
  // Bad Value
  EXPECT_FALSE(spaceInfiltrationFlowCoefficient.setShelterFactor(-10.0));
  EXPECT_EQ(0.5, spaceInfiltrationFlowCoefficient.shelterFactor());
}

TEST_F(ModelFixture, SpaceInfiltrationFlowCoefficient_Clone) {

  Model m;
  SpaceInfiltrationFlowCoefficient spaceInfiltrationFlowCoefficient(m);

  EXPECT_EQ(1u, m.numObjects());

  auto clone1 = spaceInfiltrationFlowCoefficient.clone(m).cast<SpaceInfiltrationFlowCoefficient>();
  EXPECT_EQ(2u, m.numObjects());

  auto clone2 = clone1.clone(m).cast<SpaceInfiltrationFlowCoefficient>();
  EXPECT_EQ(3u, m.numObjects());

  auto spaceType1 = SpaceType(m);
  auto success = clone1.setSpaceType(spaceType1);
  ASSERT_TRUE(success);

  auto spaceType2 = SpaceType(m);
  success = clone2.setSpaceType(spaceType2);
  ASSERT_TRUE(success);

  success = clone2.setSpaceType(spaceType1);
  ASSERT_TRUE(success);

  EXPECT_EQ(2, m.getObjectsByType(IddObjectType::OS_SpaceType).size());

  EXPECT_EQ(3, m.getObjectsByType(IddObjectType::OS_SpaceInfiltration_FlowCoefficient).size());
}

TEST_F(ModelFixture, SpaceInfiltrationFlowCoefficient_SpaceSpaceType) {

  Model m;

  {
    SpaceInfiltrationFlowCoefficient i(m);
    EXPECT_FALSE(i.schedule());

    SpaceType spaceType(m);
    DefaultScheduleSet spaceTypeDefaultScheduleSet(m);
    EXPECT_TRUE(spaceType.setDefaultScheduleSet(spaceTypeDefaultScheduleSet));
    EXPECT_FALSE(spaceTypeDefaultScheduleSet.infiltrationSchedule());
    EXPECT_FALSE(spaceTypeDefaultScheduleSet.getDefaultSchedule(DefaultScheduleType::InfiltrationSchedule));

    EXPECT_FALSE(i.space());
    EXPECT_FALSE(i.spaceType());
    EXPECT_EQ(0, spaceType.spaceInfiltrationFlowCoefficients().size());

    EXPECT_TRUE(i.setSpaceType(spaceType));
    EXPECT_FALSE(i.space());
    EXPECT_TRUE(i.spaceType());
    ASSERT_EQ(1, spaceType.spaceInfiltrationFlowCoefficients().size());
    EXPECT_EQ(i, spaceType.spaceInfiltrationFlowCoefficients()[0]);

    EXPECT_FALSE(i.schedule());
    ScheduleConstant infilSch(m);
    EXPECT_TRUE(spaceTypeDefaultScheduleSet.setInfiltrationSchedule(infilSch));
    ASSERT_TRUE(i.schedule());
    EXPECT_EQ(infilSch, i.schedule().get());
    EXPECT_TRUE(i.isScheduleDefaulted());

    ScheduleConstant hardAssignedSch(m);
    EXPECT_TRUE(i.setSchedule(hardAssignedSch));
    ASSERT_TRUE(i.schedule());
    EXPECT_EQ(hardAssignedSch, i.schedule().get());
    EXPECT_FALSE(i.isScheduleDefaulted());

    i.resetSchedule();
    ASSERT_TRUE(i.schedule());
    EXPECT_EQ(infilSch, i.schedule().get());
    EXPECT_TRUE(i.isScheduleDefaulted());
  }

  {
    SpaceInfiltrationFlowCoefficient i(m);
    EXPECT_FALSE(i.schedule());

    Space space(m);
    DefaultScheduleSet spaceDefaultScheduleSet(m);
    EXPECT_TRUE(space.setDefaultScheduleSet(spaceDefaultScheduleSet));
    EXPECT_FALSE(spaceDefaultScheduleSet.infiltrationSchedule());
    EXPECT_FALSE(spaceDefaultScheduleSet.getDefaultSchedule(DefaultScheduleType::InfiltrationSchedule));

    EXPECT_FALSE(i.space());
    EXPECT_FALSE(i.space());
    EXPECT_EQ(0, space.spaceInfiltrationFlowCoefficients().size());

    EXPECT_TRUE(i.setSpace(space));
    EXPECT_FALSE(i.spaceType());
    EXPECT_TRUE(i.space());
    ASSERT_EQ(1, space.spaceInfiltrationFlowCoefficients().size());
    EXPECT_EQ(i, space.spaceInfiltrationFlowCoefficients()[0]);

    EXPECT_FALSE(i.schedule());
    ScheduleConstant infilSch(m);
    EXPECT_TRUE(spaceDefaultScheduleSet.setInfiltrationSchedule(infilSch));
    ASSERT_TRUE(i.schedule());
    EXPECT_EQ(infilSch, i.schedule().get());
    EXPECT_TRUE(i.isScheduleDefaulted());

    ScheduleConstant hardAssignedSch(m);
    EXPECT_TRUE(i.setSchedule(hardAssignedSch));
    ASSERT_TRUE(i.schedule());
    EXPECT_EQ(hardAssignedSch, i.schedule().get());
    EXPECT_FALSE(i.isScheduleDefaulted());

    i.resetSchedule();
    ASSERT_TRUE(i.schedule());
    EXPECT_EQ(infilSch, i.schedule().get());
    EXPECT_TRUE(i.isScheduleDefaulted());
  }
}
