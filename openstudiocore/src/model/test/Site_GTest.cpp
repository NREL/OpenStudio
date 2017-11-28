/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>
/*
#include "ModelFixture.hpp"
#include "../Site.hpp"
#include "../Site_Impl.hpp"

#include "../Model_Impl.hpp"
#include "../DesignDay.hpp"
#include "../DesignDay_Impl.hpp"
#include "../SkyTemperature.hpp"
#include "../SkyTemperature_Impl.hpp"
#include "../WeatherFile.hpp"
#include "../WeatherFile_Impl.hpp"
#include "../WeatherFileConditionType.hpp"
#include "../WeatherFileConditionType_Impl.hpp"
#include "../WeatherFileDays.hpp"
#include "../WeatherFileDays_Impl.hpp"

#include "../../energyplus/ReverseTranslator.hpp"

using namespace openstudio;
using namespace openstudio::model;

void testSiteObjectInRealFile(Model& model,bool childrenExpected) {

  OptionalSite oSite = model.getOptionalUniqueModelObject<Site>();
  EXPECT_TRUE(oSite);
  Site site = *oSite;
  unsigned numSiteChildren = site.children().size();
  if (childrenExpected) { EXPECT_TRUE(numSiteChildren > 0); }

  DesignDay designDay(model);
  EXPECT_EQ(++numSiteChildren,site.children().size());

  SkyTemperature skyTemperature(model);
  EXPECT_EQ(++numSiteChildren,site.children().size());

  OptionalWeatherFile owf = model.getOptionalUniqueModelObject<WeatherFile>();
  if (!owf) {
    WeatherFile weatherFile = model.getUniqueModelObject<WeatherFile>();
    EXPECT_EQ(++numSiteChildren,site.children().size());
  }

  WeatherFileConditionType weatherFileConditionType(model);
  EXPECT_EQ(++numSiteChildren,site.children().size());

  WeatherFileDays weatherFileDays(model);
  EXPECT_EQ(++numSiteChildren,site.children().size());

}

TEST_F(ModelFixture, Site_5ZoneAirCooled) {
  Workspace workspace(idfFile);
  energyplus::ReverseTranslator translator(workspace);
  OptionalModel oModel = translator.convert();
  ASSERT_TRUE(oModel);
  Model model = *oModel;

  SCOPED_TRACE("5ZoneAirCooled");
  testSiteObjectInRealFile(model,true);
}

TEST_F(ModelFixture, Site_Daylighting) {
  Workspace workspace(daylightingIdfFile);
  energyplus::ReverseTranslator translator(workspace);
  OptionalModel oModel = translator.convert();
  ASSERT_TRUE(oModel);
  Model model = *oModel;

  SCOPED_TRACE("Daylighting");
  testSiteObjectInRealFile(model,true);
}
*/
