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

#include "ModelFixture.hpp"
#include "../TimeDependentValuation.hpp"
#include "../TimeDependentValuation_Impl.hpp"

#include "../Model.hpp"
#include "../Site.hpp"
#include "../Site_Impl.hpp"
#include "../Facility.hpp"
#include "../Facility_Impl.hpp"

#include "../../utilities/data/Attribute.hpp"
#include "../../utilities/math/FloatCompare.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, TimeDependentValuation_Construction) {
  // load file
  openstudio::path p = resourcesPath()/toPath("utilities/Filetypes/TDV_2008_kBtu_CZ13.csv");
  OptionalTimeDependentValuationFile oTdvFile = TimeDependentValuationFile::load(p);
  ASSERT_TRUE(oTdvFile);
  TimeDependentValuationFile tdvFile = *oTdvFile;

  // set file
  Model model;
  OptionalTimeDependentValuation oTdv = TimeDependentValuation::setTimeDependentValuation(model,tdvFile);
  ASSERT_TRUE(oTdv);
  TimeDependentValuation tdv = *oTdv;

  EXPECT_TRUE(tdv.file());
  EXPECT_FALSE(tdv.site());
  ASSERT_TRUE(tdv.url());
  ASSERT_TRUE(tdv.path());
  EXPECT_EQ(toString(p),toString(tdv.path().get()));
  EXPECT_TRUE(tdv.activeBuildingSector() == BuildingSector::Commercial);
  FuelTypeVector fuelTypes = tdv.availableFuelTypes();
  ASSERT_EQ(3u,fuelTypes.size());
  EXPECT_TRUE(fuelTypes[0] == FuelType::Electricity);
  EXPECT_TRUE(fuelTypes[1] == FuelType::Gas);
  EXPECT_TRUE(fuelTypes[2] == FuelType::Propane);
  tdv.setActiveBuildingSector(BuildingSector::Residential);
  fuelTypes = tdv.availableFuelTypes();
  ASSERT_EQ(3u,fuelTypes.size());
  EXPECT_TRUE(fuelTypes[0] == FuelType::Electricity);
  EXPECT_TRUE(fuelTypes[1] == FuelType::Gas);
  EXPECT_TRUE(fuelTypes[2] == FuelType::Propane);

  EXPECT_TRUE(tdv.makeUrlRelative());
  EXPECT_TRUE(tdv.file()); // file gets cached!
  EXPECT_TRUE(tdv.makeUrlAbsolute(resourcesPath()/toPath("utilities/Filetypes/")));
  EXPECT_TRUE(tdv.file());
}
