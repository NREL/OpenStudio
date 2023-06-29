/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../OutputDiagnostics.hpp"
#include "../OutputDiagnostics_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, OutputDiagnostics_GettersSetters) {
  Model m;
  auto mo = m.getUniqueModelObject<OutputDiagnostics>();

  EXPECT_TRUE(mo.keys().empty());

  // Test convenience function
  EXPECT_TRUE(mo.enableDisplayExtraWarnings());
  ASSERT_EQ(1u, mo.keys().size());
  EXPECT_EQ("DisplayExtraWarnings", mo.keys()[0]);

  EXPECT_TRUE(mo.addKey("DisplayZoneAirHeatBalanceOffBalance"));
  ASSERT_EQ(2u, mo.keys().size());
  EXPECT_EQ("DisplayExtraWarnings", mo.keys()[0]);
  EXPECT_EQ("DisplayZoneAirHeatBalanceOffBalance", mo.keys()[1]);

  // Adding a key that already exists returns true, but doesn't duplicate it
  EXPECT_TRUE(mo.addKey("DisplayZoneAirHeatBalanceOffBalance"));
  ASSERT_EQ(2u, mo.keys().size());
  EXPECT_EQ("DisplayExtraWarnings", mo.keys()[0]);
  EXPECT_EQ("DisplayZoneAirHeatBalanceOffBalance", mo.keys()[1]);

  // Bad enum
  EXPECT_FALSE(mo.addKey("BADENUM"));
  ASSERT_EQ(2u, mo.keys().size());
  EXPECT_EQ("DisplayExtraWarnings", mo.keys()[0]);
  EXPECT_EQ("DisplayZoneAirHeatBalanceOffBalance", mo.keys()[1]);

  // Set keys clears, then adds, perserving unicity
  std::vector<std::string> keys({"ReportDuringWarmup", "ReportDetailedWarmupConvergence",
                                 // Oops, this one is twice
                                 "ReportDuringHVACSizingSimulation", "ReportDuringHVACSizingSimulation"});
  EXPECT_TRUE(mo.setKeys(keys));
  ASSERT_EQ(3u, mo.keys().size());
  EXPECT_EQ("ReportDuringWarmup", mo.keys()[0]);
  EXPECT_EQ("ReportDetailedWarmupConvergence", mo.keys()[1]);
  EXPECT_EQ("ReportDuringHVACSizingSimulation", mo.keys()[2]);

  mo.clearKeys();
  EXPECT_EQ(0u, mo.keys().size());

  // Set Keys with a Bad enum: will return false, but still add the valid ones
  std::vector<std::string> keysWithBadEnum({"BAD ENUM", "DisplayUnusedSchedules"});
  EXPECT_FALSE(mo.setKeys(keysWithBadEnum));
  ASSERT_EQ(1u, mo.keys().size());
  EXPECT_EQ("DisplayUnusedSchedules", mo.keys()[0]);

}  // End of Getter_Setters test
