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
#include <string>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../PerformancePrecisionTradeoffs.hpp"
#include "../PerformancePrecisionTradeoffs_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, PerformancePrecisionTradeoffs_PerformancePrecisionTradeoffs) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      // create a model to use
      Model model;
      
      // create a performance precision tradeoffs object to use
      PerformancePrecisionTradeoffs performancePrecisionTradeoffs = model.getUniqueModelObject<PerformancePrecisionTradeoffs>();
      
      exit(0);
    },
    ::testing::ExitedWithCode(0),
    ""
  );
    
  // create a model to use
  Model model;
  
  // create a performance precision tradeoffs object to use
  PerformancePrecisionTradeoffs performancePrecisionTradeoffs = model.getUniqueModelObject<PerformancePrecisionTradeoffs>();


}

// test setting and getting
TEST_F(ModelFixture, PerformancePrecisionTradeoffs_SetGetFields) {    
  // create a model to use
  Model model;
  
  // create a performance precision tradeoffs object to use
  PerformancePrecisionTradeoffs performancePrecisionTradeoffs = model.getUniqueModelObject<PerformancePrecisionTradeoffs>();
  
  // set the fields
  
  // check the fields
  
  // reset them one by one


}

// test cloning it
TEST_F(ModelFixture, PerformancePrecisionTradeoffs_Clone)
{
  // create a model to use
  Model model;

  // create a performance precision tradeoffs object to use
  PerformancePrecisionTradeoffs performancePrecisionTradeoffs = model.getUniqueModelObject<PerformancePrecisionTradeoffs>();

  // change some of the fields

  // clone it into the same model

  // clone it into a different model


}

// check that remove works
TEST_F(ModelFixture, PerformancePrecisionTradeoffs_Remove) {
  Model model;
  auto size = model.modelObjects().size();
  PerformancePrecisionTradeoffs performancePrecisionTradeoffs = model.getUniqueModelObject<PerformancePrecisionTradeoffs>();
  EXPECT_FALSE(performancePrecisionTradeoffs.remove().empty());
  EXPECT_EQ(size, model.modelObjects().size());
}