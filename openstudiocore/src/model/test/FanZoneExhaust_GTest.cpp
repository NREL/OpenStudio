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
#include "../FanZoneExhaust.hpp"
#include "../FanZoneExhaust_Impl.hpp"
#include "../Model.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../AirflowNetworkZoneExhaustFan.hpp"
#include "../AirflowNetworkZoneExhaustFan_Impl.hpp"
#include "../AirflowNetworkCrack.hpp"
#include "../AirflowNetworkCrack_Impl.hpp"
#include "../AirflowNetworkReferenceCrackConditions.hpp"
#include "../AirflowNetworkReferenceCrackConditions_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, FanZoneExhaust_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    Model model;
    FanZoneExhaust testObject = FanZoneExhaust(model);

    exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, FanZoneExhaust_AddToAndRemoveFromThermalZone)
{
  Model model;
  FanZoneExhaust testObject = FanZoneExhaust(model);
  ThermalZone thermalZone(model);

  // Add to thermal zone
  EXPECT_TRUE(testObject.addToThermalZone(thermalZone));
  boost::optional<ThermalZone> testThermalZone = testObject.thermalZone();
  ASSERT_TRUE(testThermalZone);
  EXPECT_EQ(*(testThermalZone),testObject.thermalZone());
  EXPECT_EQ(1u, thermalZone.equipment().size());

  // Check inlet and outlet nodes
  EXPECT_TRUE(testObject.inletNode());
  EXPECT_TRUE(testObject.outletNode());

  // Remove from thermal zone
  testObject.removeFromThermalZone();
  EXPECT_FALSE(testObject.thermalZone());
  EXPECT_EQ(0u, thermalZone.equipment().size());
}

TEST_F(ModelFixture, FanZoneExhaust_AddAFNZoneExhaustFan)
{
  Model model;
  FanZoneExhaust testObject = FanZoneExhaust(model);
  ThermalZone thermalZone(model);

  EXPECT_FALSE(testObject.airflowNetworkZoneExhaustFan());

  AirflowNetworkCrack crack(model, 1.0, 0.5);
  EXPECT_EQ(1, crack.airMassFlowCoefficient());
  EXPECT_EQ(0.5, crack.airMassFlowExponent());
  EXPECT_FALSE(crack.referenceCrackConditions());

  auto afnobject = testObject.getAirflowNetworkZoneExhaustFan(crack);

  ASSERT_TRUE(afnobject.crack());
  EXPECT_EQ(crack, afnobject.crack().get());
}

