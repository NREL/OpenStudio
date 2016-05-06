/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
#include "../SiteGroundTemperatureDeep.hpp"
#include "../SiteGroundTemperatureDeep_Impl.hpp"
#include "../Model.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, SiteGroundTemperatureDeep_SiteGroundTemperatureDeep)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
    {
      // create a model to use
      Model m;

      // create a SiteGroundTemperatureDeep object
      SiteGroundTemperatureDeep ground_temp(m);
      exit(0);
    },
    ::testing::ExitedWithCode(0),
    ""
  );

  // create a model to use
  Model m;

  // create a SiteGroundTemperatureDeep object
  SiteGroundTemperatureDeep ground_temp(m);

  // Check to make sure all the fields are defaulted as expected
  ASSERT_TRUE(ground_temp.isJanuaryDeepGroundTemperatureDefaulted());
  ASSERT_TRUE(ground_temp.isFebruaryDeepGroundTemperatureDefaulted());
  ASSERT_TRUE(ground_temp.isMarchDeepGroundTemperatureDefaulted());
  ASSERT_TRUE(ground_temp.isAprilDeepGroundTemperatureDefaulted());
  ASSERT_TRUE(ground_temp.isMayDeepGroundTemperatureDefaulted());
  ASSERT_TRUE(ground_temp.isJuneDeepGroundTemperatureDefaulted());
  ASSERT_TRUE(ground_temp.isJulyDeepGroundTemperatureDefaulted());
  ASSERT_TRUE(ground_temp.isAugustDeepGroundTemperatureDefaulted());
  ASSERT_TRUE(ground_temp.isSeptemberDeepGroundTemperatureDefaulted());
  ASSERT_TRUE(ground_temp.isOctoberDeepGroundTemperatureDefaulted());
  ASSERT_TRUE(ground_temp.isNovemberDeepGroundTemperatureDefaulted());
  ASSERT_TRUE(ground_temp.isDecemberDeepGroundTemperatureDefaulted());

}

// Test cloning it
TEST_F(ModelFixture, SiteGroundTemperatureDeep_Clone)
{
  // Create a model
  Model m;

  // Create the object
  SiteGroundTemperatureDeep ground_temp(m);

  // Change some of the fields
  ground_temp.setJanuaryDeepGroundTemperature(25.0);

  // Clone it into the same model
  SiteGroundTemperatureDeep ground_temp_clone = ground_temp.clone(m).cast<SiteGroundTemperatureDeep>();
  ASSERT_FALSE(ground_temp_clone.isJanuaryDeepGroundTemperatureDefaulted());
  ASSERT_EQ(25.0, ground_temp_clone.januaryDeepGroundTemperature());
  ASSERT_TRUE(ground_temp_clone.isFebruaryDeepGroundTemperatureDefaulted());

  // Clone it into a different model
  Model m2;
  SiteGroundTemperatureDeep ground_temp_clone2 = ground_temp.clone(m2).cast<SiteGroundTemperatureDeep>();
  ASSERT_FALSE(ground_temp_clone2.isJanuaryDeepGroundTemperatureDefaulted());
  ASSERT_EQ(25.0, ground_temp_clone2.januaryDeepGroundTemperature());
  ASSERT_TRUE(ground_temp_clone2.isFebruaryDeepGroundTemperatureDefaulted());

}

// Test setting and getting fields
TEST_F(ModelFixture, SiteGroundTemperatureDeep_SetGetFields)
{
  // Create model
  Model m;

  // Create the object
  SiteGroundTemperatureDeep ground_temp(m);

  // Set the fields
  ground_temp.setJanuaryDeepGroundTemperature(20.0);
  ground_temp.setFebruaryDeepGroundTemperature(21.0);
  ground_temp.setMarchDeepGroundTemperature(22.0);
  ground_temp.setAprilDeepGroundTemperature(23.0);
  ground_temp.setMayDeepGroundTemperature(24.0);
  ground_temp.setJuneDeepGroundTemperature(25.0);
  ground_temp.setJulyDeepGroundTemperature(25.0);
  ground_temp.setAugustDeepGroundTemperature(24.0);
  ground_temp.setSeptemberDeepGroundTemperature(23.0);
  ground_temp.setOctoberDeepGroundTemperature(22.0);
  ground_temp.setNovemberDeepGroundTemperature(21.0);
  ground_temp.setDecemberDeepGroundTemperature(20.0);

  // Check the fields
  ASSERT_FALSE(ground_temp.isJanuaryDeepGroundTemperatureDefaulted());
  ASSERT_EQ(20.0, ground_temp.januaryDeepGroundTemperature());
  ASSERT_FALSE(ground_temp.isFebruaryDeepGroundTemperatureDefaulted());
  ASSERT_EQ(21.0, ground_temp.februaryDeepGroundTemperature());
  ASSERT_FALSE(ground_temp.isMarchDeepGroundTemperatureDefaulted());
  ASSERT_EQ(22.0, ground_temp.marchDeepGroundTemperature());
  ASSERT_FALSE(ground_temp.isAprilDeepGroundTemperatureDefaulted());
  ASSERT_EQ(23.0, ground_temp.aprilDeepGroundTemperature());
  ASSERT_FALSE(ground_temp.isMayDeepGroundTemperatureDefaulted());
  ASSERT_EQ(24.0, ground_temp.mayDeepGroundTemperature());
  ASSERT_FALSE(ground_temp.isJuneDeepGroundTemperatureDefaulted());
  ASSERT_EQ(25.0, ground_temp.juneDeepGroundTemperature());
  ASSERT_FALSE(ground_temp.isJulyDeepGroundTemperatureDefaulted());
  ASSERT_EQ(25.0, ground_temp.julyDeepGroundTemperature());
  ASSERT_FALSE(ground_temp.isAugustDeepGroundTemperatureDefaulted());
  ASSERT_EQ(24.0, ground_temp.augustDeepGroundTemperature());
  ASSERT_FALSE(ground_temp.isSeptemberDeepGroundTemperatureDefaulted());
  ASSERT_EQ(23.0, ground_temp.septemberDeepGroundTemperature());
  ASSERT_FALSE(ground_temp.isOctoberDeepGroundTemperatureDefaulted());
  ASSERT_EQ(22.0, ground_temp.octoberDeepGroundTemperature());
  ASSERT_FALSE(ground_temp.isNovemberDeepGroundTemperatureDefaulted());
  ASSERT_EQ(21.0, ground_temp.novemberDeepGroundTemperature());
  ASSERT_FALSE(ground_temp.isDecemberDeepGroundTemperatureDefaulted());
  ASSERT_EQ(20.0, ground_temp.decemberDeepGroundTemperature());

  // Check the new getter
  ASSERT_EQ(24.0, ground_temp.getTemperatureByMonth(8));

}
