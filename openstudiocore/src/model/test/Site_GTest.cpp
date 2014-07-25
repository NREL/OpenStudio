/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

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
#include "../TimeDependentValuation.hpp"
#include "../TimeDependentValuation_Impl.hpp"
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

  OptionalTimeDependentValuation otdv = model.getOptionalUniqueModelObject<TimeDependentValuation>();
  if (!otdv) {
    TimeDependentValuation tdv = model.getUniqueModelObject<TimeDependentValuation>();
    EXPECT_EQ(++numSiteChildren,site.children().size());
  }

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
