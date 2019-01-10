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

#include <vector>
#include <gtest/gtest.h>
#include <string>
#include "ModelFixture.hpp"
#include "../SiteGroundTemperatureBuildingSurface.hpp"
#include "../SiteGroundTemperatureBuildingSurface_Impl.hpp"
#include "../Model.hpp"
#include "../../utilities/core/Exception.hpp"
#include "../../utilities/time/Date.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, SiteGroundTemperatureBuildingSurface_SiteGroundTemperatureBuildingSurface)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
    {
      // create a model to use
      Model m;

      // create a SiteGroundTemperatureBuildingSurface object
      SiteGroundTemperatureBuildingSurface groundTemp(m);
      exit(0);
    },
    ::testing::ExitedWithCode(0),
    ""
  );

  // create a model to use
  Model m;

  // create a SiteGroundTemperatureBuildingSurface object
  SiteGroundTemperatureBuildingSurface groundTemp(m);

  // Check to make sure all the fields are defaulted as expected
  EXPECT_TRUE(groundTemp.isJanuaryGroundTemperatureDefaulted());
  EXPECT_TRUE(groundTemp.isFebruaryGroundTemperatureDefaulted());
  EXPECT_TRUE(groundTemp.isMarchGroundTemperatureDefaulted());
  EXPECT_TRUE(groundTemp.isAprilGroundTemperatureDefaulted());
  EXPECT_TRUE(groundTemp.isMayGroundTemperatureDefaulted());
  EXPECT_TRUE(groundTemp.isJuneGroundTemperatureDefaulted());
  EXPECT_TRUE(groundTemp.isJulyGroundTemperatureDefaulted());
  EXPECT_TRUE(groundTemp.isAugustGroundTemperatureDefaulted());
  EXPECT_TRUE(groundTemp.isSeptemberGroundTemperatureDefaulted());
  EXPECT_TRUE(groundTemp.isOctoberGroundTemperatureDefaulted());
  EXPECT_TRUE(groundTemp.isNovemberGroundTemperatureDefaulted());
  EXPECT_TRUE(groundTemp.isDecemberGroundTemperatureDefaulted());

}

// Test cloning it
TEST_F(ModelFixture, SiteGroundTemperatureBuildingSurface_Clone)
{
  // Create a model
  Model m;

  // Create the object
  SiteGroundTemperatureBuildingSurface groundTemp(m);

  // Change some of the fields
  groundTemp.setJanuaryGroundTemperature(25.0);

  // Clone it into the same model
  SiteGroundTemperatureBuildingSurface groundTempClone = groundTemp.clone(m).cast<SiteGroundTemperatureBuildingSurface>();
  ASSERT_FALSE(groundTempClone.isJanuaryGroundTemperatureDefaulted());
  ASSERT_EQ(25.0, groundTempClone.januaryGroundTemperature());
  ASSERT_TRUE(groundTempClone.isFebruaryGroundTemperatureDefaulted());

  // Clone it into a different model
  Model m2;
  SiteGroundTemperatureBuildingSurface groundTempClone2 = groundTemp.clone(m2).cast<SiteGroundTemperatureBuildingSurface>();
  ASSERT_FALSE(groundTempClone2.isJanuaryGroundTemperatureDefaulted());
  ASSERT_EQ(25.0, groundTempClone2.januaryGroundTemperature());
  ASSERT_TRUE(groundTempClone2.isFebruaryGroundTemperatureDefaulted());

}

// Test setting and getting fields
TEST_F(ModelFixture, SiteGroundTemperatureBuildingSurface_SetGetFields)
{
  // Create model
  Model m;

  // Create the object
  SiteGroundTemperatureBuildingSurface groundTemp(m);

  // Set the fields
  groundTemp.setJanuaryGroundTemperature(20.0);
  groundTemp.setFebruaryGroundTemperature(21.0);
  groundTemp.setMarchGroundTemperature(22.0);
  groundTemp.setAprilGroundTemperature(23.0);
  groundTemp.setMayGroundTemperature(24.0);
  groundTemp.setJuneGroundTemperature(25.0);
  groundTemp.setJulyGroundTemperature(25.0);
  groundTemp.setAugustGroundTemperature(24.0);
  groundTemp.setSeptemberGroundTemperature(23.0);
  groundTemp.setOctoberGroundTemperature(22.0);
  groundTemp.setNovemberGroundTemperature(21.0);
  groundTemp.setDecemberGroundTemperature(20.0);

  // Check the fields
  ASSERT_FALSE(groundTemp.isJanuaryGroundTemperatureDefaulted());
  ASSERT_EQ(20.0, groundTemp.januaryGroundTemperature());
  ASSERT_EQ(20.0, groundTemp.getTemperatureByMonth(1));
  ASSERT_FALSE(groundTemp.isFebruaryGroundTemperatureDefaulted());
  ASSERT_EQ(21.0, groundTemp.februaryGroundTemperature());
  ASSERT_EQ(21.0, groundTemp.getTemperatureByMonth(2));
  ASSERT_FALSE(groundTemp.isMarchGroundTemperatureDefaulted());
  ASSERT_EQ(22.0, groundTemp.marchGroundTemperature());
  ASSERT_EQ(22.0, groundTemp.getTemperatureByMonth(3));
  ASSERT_FALSE(groundTemp.isAprilGroundTemperatureDefaulted());
  ASSERT_EQ(23.0, groundTemp.aprilGroundTemperature());
  ASSERT_EQ(23.0, groundTemp.getTemperatureByMonth(4));
  ASSERT_FALSE(groundTemp.isMayGroundTemperatureDefaulted());
  ASSERT_EQ(24.0, groundTemp.mayGroundTemperature());
  ASSERT_EQ(24.0, groundTemp.getTemperatureByMonth(5));
  ASSERT_FALSE(groundTemp.isJuneGroundTemperatureDefaulted());
  ASSERT_EQ(25.0, groundTemp.juneGroundTemperature());
  ASSERT_EQ(25.0, groundTemp.getTemperatureByMonth(6));
  ASSERT_FALSE(groundTemp.isJulyGroundTemperatureDefaulted());
  ASSERT_EQ(25.0, groundTemp.julyGroundTemperature());
  ASSERT_EQ(25.0, groundTemp.getTemperatureByMonth(7));
  ASSERT_FALSE(groundTemp.isAugustGroundTemperatureDefaulted());
  ASSERT_EQ(24.0, groundTemp.augustGroundTemperature());
  ASSERT_EQ(24.0, groundTemp.getTemperatureByMonth(8));
  ASSERT_FALSE(groundTemp.isSeptemberGroundTemperatureDefaulted());
  ASSERT_EQ(23.0, groundTemp.septemberGroundTemperature());
  ASSERT_EQ(23.0, groundTemp.getTemperatureByMonth(9));
  ASSERT_FALSE(groundTemp.isOctoberGroundTemperatureDefaulted());
  ASSERT_EQ(22.0, groundTemp.octoberGroundTemperature());
  ASSERT_EQ(22.0, groundTemp.getTemperatureByMonth(10));
  ASSERT_FALSE(groundTemp.isNovemberGroundTemperatureDefaulted());
  ASSERT_EQ(21.0, groundTemp.novemberGroundTemperature());
  ASSERT_EQ(21.0, groundTemp.getTemperatureByMonth(11));
  ASSERT_FALSE(groundTemp.isDecemberGroundTemperatureDefaulted());
  ASSERT_EQ(20.0, groundTemp.decemberGroundTemperature());
  ASSERT_EQ(20.0, groundTemp.getTemperatureByMonth(12));

  // Reset them all and make sure they're all defaulted again.
  groundTemp.resetAllMonths();
  for (int i = 1; i <= 12; ++i) {
    ASSERT_TRUE(groundTemp.isMonthDefaulted(i));
  }

  // Test the setTemperatureByMonth and getTemperatureByMonth methods.
  std::vector<double> temperatures;
  temperatures.reserve(12);
  for (int i=0; i < 12; ++i) {
    temperatures.push_back(i + 18.0);
  }
  ASSERT_EQ(temperatures.size(), 12);
  for (int i=1; i <= 12; ++i) {
    groundTemp.setTemperatureByMonth(i, temperatures[i-1]);
    ASSERT_FALSE(groundTemp.isMonthDefaulted(i));
    ASSERT_EQ(temperatures[i-1], groundTemp.getTemperatureByMonth(i));
  }

  // Reset them one by one with the resetTemperatureByMonth method and check that it worked.
  for (int i = 1; i <= 12; ++i) {
    groundTemp.resetTemperatureByMonth(i);
    ASSERT_TRUE(groundTemp.isMonthDefaulted(i));
  }

  // Test the vector getter/setters
  bool setWorked = groundTemp.setAllMonthlyTemperatures(temperatures);
  ASSERT_TRUE(setWorked);
  std::vector<double> returned_temperatures = groundTemp.getAllMonthlyTemperatures();
  ASSERT_EQ(temperatures.size(), 12);
  for (int i=0; i < 12; ++i) {
    ASSERT_EQ(temperatures[i], returned_temperatures[i]);
    ASSERT_FALSE(groundTemp.isMonthDefaulted(i+1));
    ASSERT_EQ(temperatures[i], groundTemp.getTemperatureByMonth(i+1));
  }

  // Try to set the monthly temperatures with a vector of the wrong length
  temperatures.push_back(20.4);
  ASSERT_EQ(temperatures.size(), 13);
  setWorked = groundTemp.setAllMonthlyTemperatures(temperatures);
  ASSERT_FALSE(setWorked);

  // Test the ...byMonth getter/setters with MonthOfYear enums
  groundTemp.resetAllMonths();
  groundTemp.setTemperatureByMonth(openstudio::MonthOfYear::Jan, 15.0);
  ASSERT_EQ(groundTemp.getTemperatureByMonth(openstudio::MonthOfYear::Jan), 15.0);
  groundTemp.resetTemperatureByMonth(openstudio::MonthOfYear::Jan);
  ASSERT_TRUE(groundTemp.isMonthDefaulted(openstudio::MonthOfYear::Jan));

  // Test exceptions on an invalid month
  const std::string errorMsg("Invalid Month 13");
  try {
    groundTemp.getTemperatureByMonth(13);
  } catch (openstudio::Exception const & err) {
    // Test to make sure the error message returned ends with the value of errorMsg.
    ASSERT_EQ(err.message().rfind(errorMsg), err.message().size() - errorMsg.size());
  } catch (...) {
    FAIL();
  }
  try {
    groundTemp.isMonthDefaulted(13);
  } catch (openstudio::Exception const & err) {
    ASSERT_EQ(err.message().rfind(errorMsg), err.message().size() - errorMsg.size());
  } catch (...) {
    FAIL();
  }
  try {
    groundTemp.setTemperatureByMonth(13, 19.2);
  } catch (openstudio::Exception const & err) {
    ASSERT_EQ(err.message().rfind(errorMsg), err.message().size() - errorMsg.size());
  } catch (...) {
    FAIL();
  }
  try {
    groundTemp.resetTemperatureByMonth(13);
  } catch (openstudio::Exception const & err) {
    ASSERT_EQ(err.message().rfind(errorMsg), err.message().size() - errorMsg.size());
  } catch (...) {
    FAIL();
  }
}
