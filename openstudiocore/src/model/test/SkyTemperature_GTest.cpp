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

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../SkyTemperature.hpp"
#include "../SkyTemperature_Impl.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../Site.hpp"
#include "../Site_Impl.hpp"
#include "../RunPeriod.hpp"
#include "../RunPeriod_Impl.hpp"
#include "../DesignDay.hpp"
#include "../DesignDay_Impl.hpp"
#include "../WeatherFileDays.hpp"
#include "../WeatherFileDays_Impl.hpp"
#include "../WeatherFileConditionType.hpp"
#include "../WeatherFileConditionType_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SkyTemperature_Hierarchy)
{
  Model model;

  SkyTemperature skyTemperature(model);
  // default parent is Site, but not made by parent()
  OptionalParentObject opo = skyTemperature.parent();
  EXPECT_FALSE(opo);
  // construct Site
  Site site = model.getUniqueModelObject<Site>();
  opo = skyTemperature.parent();
  ASSERT_TRUE(opo);
  ParentObject skyTempParent = *opo;
  EXPECT_TRUE(skyTempParent == site);

  // Make skyTemperature apply to a RunPeriod
  RunPeriod runPeriod = model.getUniqueModelObject<RunPeriod>();
  EXPECT_TRUE(skyTemperature.setParent(runPeriod));
  opo = skyTemperature.parent();
  ASSERT_TRUE(opo);
  skyTempParent = *opo;
  EXPECT_TRUE(skyTempParent == runPeriod);
  EXPECT_EQ(static_cast<unsigned>(0),site.children().size());

  SkyTemperature globalSkyTemperature(model);
  opo = globalSkyTemperature.parent();
  ASSERT_TRUE(opo);
  ParentObject globalSkyTempParent = *opo;
  EXPECT_TRUE(globalSkyTempParent == site);
  EXPECT_EQ(static_cast<unsigned>(1),site.children().size());
  EXPECT_EQ(static_cast<unsigned>(1),runPeriod.children().size());
  EXPECT_EQ(static_cast<unsigned>(4),model.numObjects());

  // skyTemperature can also be parented by SizingPeriod objects
  DesignDay designDay(model);
  EXPECT_TRUE(skyTemperature.setParent(designDay));
  WeatherFileDays weatherFileDays(model);
  EXPECT_TRUE(skyTemperature.setParent(weatherFileDays));
  WeatherFileConditionType weatherFileConditionType(model);
  EXPECT_TRUE(skyTemperature.setParent(weatherFileConditionType));
}
