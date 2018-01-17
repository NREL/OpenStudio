/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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
#include "AnalysisFixture.hpp"

#include "../RubyContinuousVariable.hpp"
#include "../RubyMeasure.hpp"
#include "../NormalDistribution.hpp"
#include "../WeibullDistribution.hpp"

#include "../../ruleset/OSArgument.hpp"

#include "../../utilities/core/FileReference.hpp"
#include "../../utilities/core/PathHelpers.hpp"

using namespace openstudio;
using namespace openstudio::analysis;
using namespace openstudio::ruleset;

TEST_F(AnalysisFixture, RubyContinuousVariable_UserScript) {
  RubyMeasure userScript(toPath("setGeometry.rb"),
                              FileReferenceType::OSM,
                              FileReferenceType::OSM,
                              true);
  OSArgument arg = OSArgument::makeDoubleArgument("floorToFloorHeight");
  arg.setValue(3.0);
  userScript.addArgument(arg);
  arg = OSArgument::makeIntegerArgument("numFloors");
  arg.setValue(1);
  userScript.addArgument(arg);
  EXPECT_EQ(2u,userScript.arguments().size());
  EXPECT_TRUE(userScript.isUserScript());
  EXPECT_EQ(toString(completeAndNormalize(toPath("setGeometry.rb"))),
            toString(userScript.perturbationScript().path()));

  arg = OSArgument::makeDoubleArgument("floorArea");
  RubyContinuousVariable var("Floor Area (m^2)",arg,userScript);
  var.setMinimum(500.0);
  var.setMaximum(1000.0);
  EXPECT_EQ(2u,var.measure().arguments().size());
  EXPECT_TRUE(var.measure().isUserScript());
  EXPECT_EQ(toString(completeAndNormalize(toPath("setGeometry.rb"))),
            toString(var.measure().perturbationScript().path()));
  ASSERT_TRUE(var.minimum());
  EXPECT_DOUBLE_EQ(500.0,var.minimum().get());
  ASSERT_TRUE(var.maximum());
  EXPECT_DOUBLE_EQ(1000.0,var.maximum().get());
  EXPECT_FALSE(var.increment());
  EXPECT_FALSE(var.nSteps());
  EXPECT_TRUE(var.isFeasible(500.0));
  EXPECT_FALSE(var.isFeasible(1500.0));
  ASSERT_TRUE(var.truncate(1500.0));
  EXPECT_DOUBLE_EQ(1000.0,var.truncate(1500.0).get());
  WeibullDistribution distribution(1.5,1.0);
  EXPECT_DOUBLE_EQ(1.5,distribution.alpha());
  EXPECT_DOUBLE_EQ(1.0,distribution.beta());
  EXPECT_FALSE(var.uncertaintyDescription());
  EXPECT_TRUE(var.setUncertaintyDescription(distribution));
  ASSERT_TRUE(var.uncertaintyDescription());
  ASSERT_TRUE(var.uncertaintyDescription().get().optionalCast<WeibullDistribution>());
  EXPECT_DOUBLE_EQ(1.5,var.uncertaintyDescription().get().cast<WeibullDistribution>().alpha());
  EXPECT_DOUBLE_EQ(1.0,var.uncertaintyDescription().get().cast<WeibullDistribution>().beta());
  var.resetUncertaintyDescription();
  EXPECT_FALSE(var.uncertaintyDescription());
}

TEST_F(AnalysisFixture, RubyContinuousVariable_PlainScript) {
  RubyMeasure script(toPath("addUtilityRate.rb"),
                          FileReferenceType::IDF,
                          FileReferenceType::IDF);
  script.addArgument("noDemandCharge");
  EXPECT_EQ(1u,script.arguments().size());
  EXPECT_FALSE(script.isUserScript());
  EXPECT_EQ(toString(completeAndNormalize(toPath("addUtilityRate.rb"))),
            toString(script.perturbationScript().path()));

  OSArgument arg = OSArgument::makeDoubleArgument("energyCharge");
  RubyContinuousVariable var("Energy Charge ($/kWh)",arg,script);
  var.setMinimum(0.005);
  EXPECT_EQ(1u,var.measure().arguments().size());
  EXPECT_FALSE(var.measure().isUserScript());
  EXPECT_EQ(toString(completeAndNormalize(toPath("addUtilityRate.rb"))),
            toString(var.measure().perturbationScript().path()));
  ASSERT_TRUE(var.minimum());
  EXPECT_DOUBLE_EQ(0.005,var.minimum().get());
  EXPECT_FALSE(var.maximum());
  EXPECT_FALSE(var.increment());
  EXPECT_FALSE(var.nSteps());
  NormalDistribution distribution(0.07,0.03);
  EXPECT_FALSE(var.uncertaintyDescription());
  EXPECT_TRUE(var.setUncertaintyDescription(distribution));
  ASSERT_TRUE(var.uncertaintyDescription());
  EXPECT_FALSE(var.uncertaintyDescription().get().optionalCast<WeibullDistribution>());
  ASSERT_TRUE(var.uncertaintyDescription().get().optionalCast<NormalDistribution>());
  EXPECT_DOUBLE_EQ(0.07,var.uncertaintyDescription().get().cast<NormalDistribution>().mean());
  EXPECT_DOUBLE_EQ(0.03,var.uncertaintyDescription().get().cast<NormalDistribution>().standardDeviation());
  var.resetUncertaintyDescription();
  EXPECT_FALSE(var.uncertaintyDescription());
}
