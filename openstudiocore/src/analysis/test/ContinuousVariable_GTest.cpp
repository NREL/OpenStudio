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
