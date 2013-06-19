/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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
#include <model/test/ModelFixture.hpp>
#include <model/Site.hpp>
#include <model/Site_Impl.hpp>

#include <model/Model_Impl.hpp>
#include <model/DesignDay.hpp>
#include <model/DesignDay_Impl.hpp>
#include <model/SkyTemperature.hpp>
#include <model/SkyTemperature_Impl.hpp>
#include <model/TimeDependentValuation.hpp>
#include <model/TimeDependentValuation_Impl.hpp>
#include <model/WeatherFile.hpp>
#include <model/WeatherFile_Impl.hpp>
#include <model/WeatherFileConditionType.hpp>
#include <model/WeatherFileConditionType_Impl.hpp>
#include <model/WeatherFileDays.hpp>
#include <model/WeatherFileDays_Impl.hpp>

#include <energyplus/ReverseTranslator.hpp>

#include <boost/foreach.hpp>

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