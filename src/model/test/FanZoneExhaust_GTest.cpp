/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

TEST_F(ModelFixture, FanZoneExhaust_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;
      FanZoneExhaust testObject = FanZoneExhaust(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, FanZoneExhaust_AddToAndRemoveFromThermalZone) {
  Model model;
  FanZoneExhaust testObject = FanZoneExhaust(model);
  ThermalZone thermalZone(model);

  // Add to thermal zone
  EXPECT_TRUE(testObject.addToThermalZone(thermalZone));
  boost::optional<ThermalZone> testThermalZone = testObject.thermalZone();
  ASSERT_TRUE(testThermalZone);
  EXPECT_EQ(*(testThermalZone), testObject.thermalZone().get());
  EXPECT_EQ(1u, thermalZone.equipment().size());

  // Check inlet and outlet nodes
  EXPECT_TRUE(testObject.inletNode());
  EXPECT_TRUE(testObject.outletNode());

  // Remove from thermal zone
  testObject.removeFromThermalZone();
  EXPECT_FALSE(testObject.thermalZone());
  EXPECT_EQ(0u, thermalZone.equipment().size());
}

TEST_F(ModelFixture, FanZoneExhaust_AddAFNZoneExhaustFan) {
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
