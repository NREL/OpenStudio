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
#include "../OutputDiagnostics.hpp"
#include "../OutputDiagnostics_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, OutputDiagnostics_GettersSetters) {
  Model m;
  OutputDiagnostics mo = m.getUniqueModelObject<OutputDiagnostics>();

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
