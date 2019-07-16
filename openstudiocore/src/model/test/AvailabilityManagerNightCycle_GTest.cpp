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
#include "../AvailabilityManagerNightCycle.hpp"
#include "../AvailabilityManagerNightCycle_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,AvailabilityManagerNightCycle) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
    {
      model::Model m;

      model::AvailabilityManagerNightCycle availabilityManagerNightCycle(m);

      exit(0);
    } ,
    ::testing::ExitedWithCode(0), "" );
}


TEST_F(ModelFixture, AvailabilityManagerNightCycle_zoneLists)
{
  Model m;

  AvailabilityManagerNightCycle avm(m);

  ThermalZone z1(m);
  ThermalZone z2(m);
  ThermalZone z3(m);
  ThermalZone z4(m);
  ThermalZone z5(m);

  // Deprecated
  // Cannot run this on Windows, treats warnings as errors
  // EXPECT_TRUE(avm.setControlThermalZone(z1));
  // EXPECT_TRUE(avm.controlThermalZone());
  // EXPECT_EQ(avm.controlThermalZone().get(), z1);
  // EXPECT_EQ(1, avm.controlThermalZones().size());
  // EXPECT_EQ(z1, avm.controlThermalZones()[0]);

  std::vector<ThermalZone> v;
  v.push_back(z1);
  v.push_back(z2);
  EXPECT_TRUE(avm.setControlThermalZones(v));
  EXPECT_EQ(2, avm.controlThermalZones().size());
  EXPECT_EQ(0, avm.coolingControlThermalZones().size());
  EXPECT_EQ(0, avm.heatingControlThermalZones().size());
  EXPECT_EQ(0, avm.heatingZoneFansOnlyThermalZones().size());

  // Deprecated method should return the first zone if more than one
  // Cannot run this on Windows, treats warnings as errors
  // EXPECT_TRUE(avm.controlThermalZone());
  // EXPECT_EQ(avm.controlThermalZone().get(), z1);

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
