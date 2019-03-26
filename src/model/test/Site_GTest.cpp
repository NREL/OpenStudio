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
  for (size_t i = 0; i < variables.size(); ++i){
    EXPECT_EQ(variables[i], variables2[i]);
  }
  EXPECT_EQ(site.outputVariableNames(), site.outputVariableNames());
  EXPECT_EQ(site.outputVariableNames().front(), site.outputVariableNames().front());
}