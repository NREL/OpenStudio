/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

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

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, Site_Outputvariables) {
  Model model;
  EXPECT_FALSE(model.getOptionalUniqueModelObject<Site>());
  Site site = model.getUniqueModelObject<Site>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<Site>());
  std::vector<std::string> variables = site.outputVariableNames();
  EXPECT_FALSE(variables.empty());
  std::vector<std::string> variables2 = site.outputVariableNames();
  ASSERT_EQ(variables.size(), variables2.size());
  for (size_t i = 0; i < variables.size(); ++i) {
    EXPECT_EQ(variables[i], variables2[i]);
  }
  EXPECT_EQ(site.outputVariableNames(), site.outputVariableNames());
  EXPECT_EQ(site.outputVariableNames().front(), site.outputVariableNames().front());
}
