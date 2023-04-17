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

#include <gtest/gtest.h>
#include "ModelFixture.hpp"

#include "../ZoneHVACCoolingPanelRadiantConvectiveWater.hpp"
#include "../ZoneHVACCoolingPanelRadiantConvectiveWater_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../CoilCoolingWaterPanelRadiant.hpp"
#include "../CoilCoolingWaterPanelRadiant_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneHVACCoolingPanelRadiantConvectiveWater) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      ZoneHVACCoolingPanelRadiantConvectiveWater zonehvac(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, ZoneHVACCoolingPanelRadiantConvectiveWater_AddAndRemove) {
  Model m;
  ZoneHVACCoolingPanelRadiantConvectiveWater zonehvac(m);

  ThermalZone tz(m);
  ASSERT_TRUE(zonehvac.addToThermalZone(tz));
  ASSERT_TRUE(zonehvac.thermalZone());
  ASSERT_EQ(tz, zonehvac.thermalZone().get());
  ASSERT_EQ(1u, tz.equipment().size());
  zonehvac.removeFromThermalZone();
  ASSERT_EQ(0u, tz.equipment().size());

  ZoneHVACCoolingPanelRadiantConvectiveWater zonehvac2(m);
  zonehvac2.addToThermalZone(tz);
  zonehvac2.remove();
  ASSERT_EQ(0u, tz.equipment().size());
}

TEST_F(ModelFixture, ZoneHVACCoolingPanelRadiantConvectiveWater_SetGetFields) {
  Model m;
  ZoneHVACCoolingPanelRadiantConvectiveWater zonehvac(m);

  Schedule schedule = zonehvac.availabilitySchedule();
  boost::optional<ScheduleConstant> scheduleConstant = schedule.optionalCast<ScheduleConstant>();
  ASSERT_TRUE(scheduleConstant);
  EXPECT_EQ((*scheduleConstant).value(), 1.0);
  EXPECT_EQ(0.65, zonehvac.fractionRadiant());
  EXPECT_EQ(0.2, zonehvac.fractionofRadiantEnergyIncidentonPeople());
  HVACComponent coil = zonehvac.coolingCoil();
  boost::optional<CoilCoolingWaterPanelRadiant> coilCooling = coil.optionalCast<CoilCoolingWaterPanelRadiant>();
  ASSERT_TRUE(coilCooling);

  ScheduleConstant sched(m);
  sched.setValue(0.5);
  EXPECT_TRUE(zonehvac.setAvailabilitySchedule(sched));
  EXPECT_TRUE(zonehvac.setFractionRadiant(0.75));
  EXPECT_TRUE(zonehvac.setFractionofRadiantEnergyIncidentonPeople(0.3));
  CoilCoolingWaterPanelRadiant coilCooling2(m);
  EXPECT_TRUE(zonehvac.setCoolingCoil(coilCooling2));

  Schedule schedule2 = zonehvac.availabilitySchedule();
  boost::optional<ScheduleConstant> scheduleConstant2 = schedule2.optionalCast<ScheduleConstant>();
  ASSERT_TRUE(scheduleConstant2);
  EXPECT_EQ((*scheduleConstant2).value(), 0.5);
  EXPECT_EQ(0.75, zonehvac.fractionRadiant());
  EXPECT_EQ(0.3, zonehvac.fractionofRadiantEnergyIncidentonPeople());
  HVACComponent coil3 = zonehvac.coolingCoil();
  boost::optional<CoilCoolingWaterPanelRadiant> coilCooling3 = coil3.optionalCast<CoilCoolingWaterPanelRadiant>();
  ASSERT_TRUE(coilCooling3);
  EXPECT_EQ(coilCooling2.nameString(), coil3.nameString());
}
