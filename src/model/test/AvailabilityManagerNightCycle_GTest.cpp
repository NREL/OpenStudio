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
#include "../AvailabilityManagerNightCycle.hpp"
#include "../AvailabilityManagerNightCycle_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"
#include "../AirLoopHVAC.hpp"
#include "../ModelObjectList.hpp"
#include "../ModelObjectList_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AvailabilityManagerNightCycle) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      model::Model m;

      model::AvailabilityManagerNightCycle availabilityManagerNightCycle(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, AvailabilityManagerNightCycle_zoneLists) {
  Model m;

  AvailabilityManagerNightCycle avm(m);

  ThermalZone z1(m);
  ThermalZone z2(m);
  ThermalZone z3(m);
  ThermalZone z4(m);
  ThermalZone z5(m);

  std::vector<ThermalZone> v;
  v.push_back(z1);
  v.push_back(z2);
  EXPECT_TRUE(avm.setControlThermalZones(v));
  EXPECT_EQ(2, avm.controlThermalZones().size());
  EXPECT_EQ(0, avm.coolingControlThermalZones().size());
  EXPECT_EQ(0, avm.heatingControlThermalZones().size());
  EXPECT_EQ(0, avm.heatingZoneFansOnlyThermalZones().size());

  avm.resetControlThermalZones();
  EXPECT_EQ(0, avm.controlThermalZones().size());

  std::vector<ThermalZone> v2;
  v2.push_back(z1);
  v2.push_back(z3);
  EXPECT_TRUE(avm.setCoolingControlThermalZones(v2));
  EXPECT_EQ(0, avm.controlThermalZones().size());
  EXPECT_EQ(2, avm.coolingControlThermalZones().size());
  EXPECT_EQ(0, avm.heatingControlThermalZones().size());
  EXPECT_EQ(0, avm.heatingZoneFansOnlyThermalZones().size());

  std::vector<ThermalZone> v3;
  v3.push_back(z1);
  v3.push_back(z3);
  v3.push_back(z5);
  EXPECT_TRUE(avm.setHeatingControlThermalZones(v3));
  EXPECT_EQ(0, avm.controlThermalZones().size());
  EXPECT_EQ(2, avm.coolingControlThermalZones().size());
  EXPECT_EQ(3, avm.heatingControlThermalZones().size());
  EXPECT_EQ(0, avm.heatingZoneFansOnlyThermalZones().size());

  std::vector<ThermalZone> v4;
  v4.push_back(z4);
  EXPECT_TRUE(avm.setHeatingZoneFansOnlyThermalZones(v4));
  EXPECT_EQ(0, avm.controlThermalZones().size());
  EXPECT_EQ(2, avm.coolingControlThermalZones().size());
  EXPECT_EQ(3, avm.heatingControlThermalZones().size());
  EXPECT_EQ(1, avm.heatingZoneFansOnlyThermalZones().size());

  avm.resetCoolingControlThermalZones();
  EXPECT_EQ(0, avm.controlThermalZones().size());
  EXPECT_EQ(0, avm.coolingControlThermalZones().size());
  EXPECT_EQ(3, avm.heatingControlThermalZones().size());
  EXPECT_EQ(1, avm.heatingZoneFansOnlyThermalZones().size());

  avm.resetHeatingZoneFansOnlyThermalZones();
  EXPECT_EQ(0, avm.controlThermalZones().size());
  EXPECT_EQ(0, avm.coolingControlThermalZones().size());
  EXPECT_EQ(3, avm.heatingControlThermalZones().size());
  EXPECT_EQ(0, avm.heatingZoneFansOnlyThermalZones().size());

  avm.resetHeatingControlThermalZones();
  EXPECT_EQ(0, avm.controlThermalZones().size());
  EXPECT_EQ(0, avm.coolingControlThermalZones().size());
  EXPECT_EQ(0, avm.heatingControlThermalZones().size());
  EXPECT_EQ(0, avm.heatingZoneFansOnlyThermalZones().size());
}

TEST_F(ModelFixture, AvailabilityManagerNightCycle_Schedules) {
  Model m;

  Schedule alwaysOnDiscreteSchedule = m.alwaysOnDiscreteSchedule();
  AvailabilityManagerNightCycle avm(m);
  EXPECT_EQ(alwaysOnDiscreteSchedule, avm.applicabilitySchedule());

  ScheduleConstant sch_applicability(m);
  sch_applicability.setName("AVM NightCycle Applicability Schedule");
  sch_applicability.setValue(1.0);
  EXPECT_TRUE(avm.setApplicabilitySchedule(sch_applicability));
  EXPECT_EQ(sch_applicability, avm.applicabilitySchedule());

  EXPECT_FALSE(avm.airLoopHVAC());
  EXPECT_FALSE(avm.fanSchedule());

  AirLoopHVAC a(m);
  ScheduleConstant sch_op(m);
  sch_op.setName("HVAC Operation");
  sch_op.setValue(1.0);
  EXPECT_TRUE(a.setAvailabilitySchedule(sch_op));

  EXPECT_TRUE(a.addAvailabilityManager(avm));
  boost::optional<AirLoopHVAC> _a = avm.airLoopHVAC();
  ASSERT_TRUE(_a);
  EXPECT_EQ(a, _a.get());
  boost::optional<Schedule> _sch = avm.fanSchedule();
  ASSERT_TRUE(_sch);
  EXPECT_EQ(sch_op, _sch.get());
}

TEST_F(ModelFixture, AvailabilityManagerNightCycle_zoneLists_clone_Remove) {
  // Add test for #4020

  Model m;

  AirLoopHVAC a(m);
  AvailabilityManagerNightCycle avm(m);
  EXPECT_EQ(4, m.getConcreteModelObjects<ModelObjectList>().size());

  EXPECT_TRUE(a.addAvailabilityManager(avm));
  EXPECT_TRUE(avm.airLoopHVAC());

  ThermalZone z1(m);
  ThermalZone z2(m);
  ThermalZone z3(m);
  ThermalZone z4(m);
  EXPECT_EQ(4, m.getConcreteModelObjects<ThermalZone>().size());
  EXPECT_EQ(4, m.getConcreteModelObjects<ModelObjectList>().size());

  EXPECT_TRUE(avm.setControlThermalZones({z1}));
  EXPECT_TRUE(avm.setCoolingControlThermalZones({z2}));
  EXPECT_TRUE(avm.setHeatingControlThermalZones({z3}));
  EXPECT_TRUE(avm.setHeatingZoneFansOnlyThermalZones({z4}));

  ASSERT_EQ(1, avm.controlThermalZones().size());
  ASSERT_EQ(1, avm.coolingControlThermalZones().size());
  ASSERT_EQ(1, avm.heatingControlThermalZones().size());
  ASSERT_EQ(1, avm.heatingZoneFansOnlyThermalZones().size());
  ASSERT_EQ(z1, avm.controlThermalZones()[0]);
  ASSERT_EQ(z2, avm.coolingControlThermalZones()[0]);
  ASSERT_EQ(z3, avm.heatingControlThermalZones()[0]);
  ASSERT_EQ(z4, avm.heatingZoneFansOnlyThermalZones()[0]);

  // Cloning should ensure we do have modelObjectLists for control zones, but they should be empty
  // as it is not connected to an AirLoopHVAC (yet) and it wouldn't make sense to have zones there
  AvailabilityManagerNightCycle avmClone = avm.clone(m).cast<AvailabilityManagerNightCycle>();
  EXPECT_TRUE(avm.airLoopHVAC());
  EXPECT_FALSE(avmClone.airLoopHVAC());
  EXPECT_EQ(2, m.getConcreteModelObjects<AvailabilityManagerNightCycle>().size());
  EXPECT_EQ(4, m.getConcreteModelObjects<ThermalZone>().size());
  EXPECT_EQ(8, m.getConcreteModelObjects<ModelObjectList>().size());
  ASSERT_EQ(1, avm.controlThermalZones().size());
  ASSERT_EQ(1, avm.coolingControlThermalZones().size());
  ASSERT_EQ(1, avm.heatingControlThermalZones().size());
  ASSERT_EQ(1, avm.heatingZoneFansOnlyThermalZones().size());
  ASSERT_EQ(z1, avm.controlThermalZones()[0]);
  ASSERT_EQ(z2, avm.coolingControlThermalZones()[0]);
  ASSERT_EQ(z3, avm.heatingControlThermalZones()[0]);
  ASSERT_EQ(z4, avm.heatingZoneFansOnlyThermalZones()[0]);
  EXPECT_EQ(0, avmClone.controlThermalZones().size());
  EXPECT_EQ(0, avmClone.coolingControlThermalZones().size());
  EXPECT_EQ(0, avmClone.heatingControlThermalZones().size());
  EXPECT_EQ(0, avmClone.heatingZoneFansOnlyThermalZones().size());

  // Removes the object and 4 MOLists
  auto removedObjects = avmClone.remove();
  EXPECT_EQ(5, removedObjects.size());
  EXPECT_TRUE(avm.airLoopHVAC());
  EXPECT_EQ(1, m.getConcreteModelObjects<AvailabilityManagerNightCycle>().size());
  EXPECT_EQ(4, m.getConcreteModelObjects<ThermalZone>().size());
  EXPECT_EQ(4, m.getConcreteModelObjects<ModelObjectList>().size());
  ASSERT_EQ(1, avm.controlThermalZones().size());
  ASSERT_EQ(1, avm.coolingControlThermalZones().size());
  ASSERT_EQ(1, avm.heatingControlThermalZones().size());
  ASSERT_EQ(1, avm.heatingZoneFansOnlyThermalZones().size());
  ASSERT_EQ(z1, avm.controlThermalZones()[0]);
  ASSERT_EQ(z2, avm.coolingControlThermalZones()[0]);
  ASSERT_EQ(z3, avm.heatingControlThermalZones()[0]);
  ASSERT_EQ(z4, avm.heatingZoneFansOnlyThermalZones()[0]);

  // Removes the object and 4 MOLists, and must not delete the thermal zones!
  removedObjects = avm.remove();
  EXPECT_EQ(5, removedObjects.size());
  EXPECT_EQ(0, m.getConcreteModelObjects<AvailabilityManagerNightCycle>().size());
  EXPECT_EQ(4, m.getConcreteModelObjects<ThermalZone>().size());
  EXPECT_EQ(0, m.getConcreteModelObjects<ModelObjectList>().size());
}
