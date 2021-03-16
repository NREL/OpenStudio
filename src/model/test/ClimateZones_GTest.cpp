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
#include "../ClimateZones.hpp"
#include "../ClimateZones_Impl.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../Site.hpp"
#include "../Site_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ClimateZones) {
  // construct directly
  Model model;
  EXPECT_FALSE(model.getOptionalUniqueModelObject<ClimateZones>());
  ClimateZones czs = model.getUniqueModelObject<ClimateZones>();

  // default
  ASSERT_EQ(0u, czs.numClimateZones());
  ASSERT_EQ(0u, czs.climateZones().size());

  // after clear
  czs.clear();
  EXPECT_EQ(0u, czs.numClimateZones());
  EXPECT_EQ(0, czs.climateZones().size());

  // append a climate zone
  ClimateZone cz = czs.appendClimateZone(ClimateZones::cecInstitutionName(),
                                         ClimateZones::validClimateZoneValues(ClimateZones::cecInstitutionName(), ClimateZones::cecDefaultYear())[0]);
  ASSERT_FALSE(cz.empty());
  EXPECT_EQ(ClimateZones::cecInstitutionName(), cz.institution());
  EXPECT_EQ(ClimateZones::cecDocumentName(), cz.documentName());
  EXPECT_EQ(ClimateZones::cecDefaultYear(), cz.year());
  EXPECT_EQ("1", cz.value());
}

TEST_F(ModelFixture, ClimateZones_Site) {
  // construct Site
  Model model;
  Site site = model.getUniqueModelObject<Site>();
  EXPECT_FALSE(site.climateZones());

  // set value creates ClimateZones object
  StringVector validValues = ClimateZones::validClimateZoneValues(ClimateZones::ashraeInstitutionName(), ClimateZones::ashraeDefaultYear());
  ASSERT_EQ(17u, validValues.size());
}
