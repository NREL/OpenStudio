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

#include "../DDACEAlgorithm.hpp"
#include "../DDACEAlgorithm_Impl.hpp"
#include "../DDACEAlgorithmOptions.hpp"
#include "../DDACEAlgorithmOptions_Impl.hpp"
#include "../Problem.hpp"
#include "../MeasureGroup.hpp"
#include "../RubyMeasure.hpp"
#include "../RubyMeasure_Impl.hpp"
#include "../NullMeasure.hpp"
#include "../RubyContinuousVariable.hpp"

#include "../../ruleset/OSArgument.hpp"

#include "../../runmanager/lib/Workflow.hpp"

#include "../../utilities/data/Attribute.hpp"

#include <resources.hxx>

using namespace openstudio;
using namespace openstudio::analysis;
using namespace openstudio::ruleset;

TEST_F(AnalysisFixture, DDACEAlgorithmOptions) {

  // problem with three variables
  VariableVector variables;
  BCLMeasure bclMeasure(resourcesPath() / toPath("utilities/BCL/Measures/SetWindowToWallRatioByFacade"));
  RubyMeasure measure(bclMeasure);
  variables.push_back(RubyContinuousVariable("Var 1",OSArgument::makeDoubleArgument("wwr1"),measure));
  variables.push_back(RubyContinuousVariable("Var 2",OSArgument::makeDoubleArgument("wwr2"),measure));
  variables.push_back(RubyContinuousVariable("Var 3",OSArgument::makeDoubleArgument("wwr3"),measure));
  Problem problem("Null Problem",variables,runmanager::Workflow());

  DDACEAlgorithmOptions options(DDACEAlgorithmType::grid);
  EXPECT_EQ(DDACEAlgorithmType(DDACEAlgorithmType::grid),options.algorithmType());
  EXPECT_FALSE(options.samples());
  options.setSamplesForGrid(2,problem);
  ASSERT_TRUE(options.samples());
  EXPECT_EQ(8,options.samples().get());

  options.setSamplesForGrid(5,problem);
  ASSERT_TRUE(options.samples());
  EXPECT_EQ(125,options.samples().get());

  DDACEAlgorithmOptions optionsClone = options.clone().cast<DDACEAlgorithmOptions>();
  // all Attributes should be equivalent but have different UUIDs
  AttributeVector attributes = options.options();
  AttributeVector attributeClones = optionsClone.options();
  ASSERT_EQ(attributes.size(),attributeClones.size());
  for (unsigned i = 0, n = attributes.size(); i < n; ++i) {
    EXPECT_TRUE(attributes[i] == attributeClones[i]);
    EXPECT_FALSE(attributes[i].uuid() == attributeClones[i].uuid());
    EXPECT_FALSE(attributes[i].versionUUID() == attributeClones[i].versionUUID());
  }

  options = DDACEAlgorithmOptions(DDACEAlgorithmType::lhs);
  options.setSeed(891678);
  options.setSamples(62);
  ASSERT_TRUE(options.seed());
  EXPECT_EQ(891678,options.seed().get());
  ASSERT_TRUE(options.samples());
  EXPECT_EQ(62,options.samples().get());
  EXPECT_FALSE(options.symbols());
  options.setSeed(1);
  ASSERT_TRUE(options.seed());
  EXPECT_EQ(1,options.seed().get());
  options.clearSeed();
  EXPECT_FALSE(options.seed());

  options = DDACEAlgorithmOptions(DDACEAlgorithmType::oas);
  options.setSamplesAndSymbolsForOrthogonalArray(13,1);
  EXPECT_TRUE(options.seed()); // default is to generate a fixed seed
  ASSERT_TRUE(options.symbols());
  EXPECT_EQ(13,options.symbols().get());
  ASSERT_TRUE(options.samples());
  EXPECT_EQ(169,options.samples().get());

  optionsClone = options.clone().cast<DDACEAlgorithmOptions>();
  // all Attributes should be equivalent but have different UUIDs
  attributes = options.options();
  attributeClones = optionsClone.options();
  ASSERT_EQ(attributes.size(),attributeClones.size());
  for (unsigned i = 0, n = attributes.size(); i < n; ++i) {
    EXPECT_TRUE(attributes[i] == attributeClones[i]);
    EXPECT_FALSE(attributes[i].uuid() == attributeClones[i].uuid());
    EXPECT_FALSE(attributes[i].versionUUID() == attributeClones[i].versionUUID());
  }

  options.clearSymbols();
  EXPECT_FALSE(options.symbols());
  EXPECT_TRUE(options.samples());
  options.clearSamples();
  EXPECT_FALSE(options.samples());

  int n = DDACEAlgorithmOptions::samplesForCentralComposite(problem);
  EXPECT_EQ(15,n);
  n = DDACEAlgorithmOptions::samplesForBoxBehnken(problem);
  EXPECT_EQ(13,n);
}

TEST_F(AnalysisFixture,DDACEAlgorithm_CompatibleProblemType) {
  // continuous problem with five variables
  VariableVector variables;
  BCLMeasure bclMeasure(resourcesPath() / toPath("utilities/BCL/Measures/SetWindowToWallRatioByFacade"));
  RubyMeasure measure(bclMeasure);
  variables.push_back(RubyContinuousVariable("Var 1",OSArgument::makeDoubleArgument("wwr1"),measure));
  variables.push_back(RubyContinuousVariable("Var 2",OSArgument::makeDoubleArgument("wwr2"),measure));
  variables.push_back(RubyContinuousVariable("Var 3",OSArgument::makeDoubleArgument("wwr3"),measure));
  variables.push_back(RubyContinuousVariable("Var 4",OSArgument::makeDoubleArgument("wwr4"),measure));
  variables.push_back(RubyContinuousVariable("Var 5",OSArgument::makeDoubleArgument("wwr5"),measure));
  Problem cProblem("Continuous Problem",variables,runmanager::Workflow());
  EXPECT_EQ(5,cProblem.numVariables());
  EXPECT_EQ(5,cProblem.numContinuousVariables());
  variables.clear();

  // mixed problem with three variables, ignorable discrete variable
  MeasureVector measures;
  variables.push_back(RubyContinuousVariable("Var 1",OSArgument::makeDoubleArgument("wwr1"),measure));
  measures.push_back(RubyMeasure(toPath("script.rb"),FileReferenceType::OSM,FileReferenceType::OSM));
  variables.push_back(MeasureGroup("Var 2",measures));
  measures.clear();
  variables.push_back(RubyContinuousVariable("Var 3",OSArgument::makeDoubleArgument("wwr3"),measure));
  Problem mProblem("Mixed Problem",variables,runmanager::Workflow());
  EXPECT_EQ(3,mProblem.numVariables());
  EXPECT_EQ(2,mProblem.numContinuousVariables());
  EXPECT_EQ(1,mProblem.numDiscreteVariables());
  EXPECT_EQ(1,mProblem.numStaticTransformations());
  variables.clear();

  // discrete problem
  measures.push_back(NullMeasure());
  measures.push_back(RubyMeasure(toPath("script1.rb"),FileReferenceType::OSM,FileReferenceType::OSM));
  measures.back().cast<RubyMeasure>().addArgument("wwr","0.2");
  measures.push_back(RubyMeasure(toPath("script1.rb"),FileReferenceType::OSM,FileReferenceType::OSM));
  measures.back().cast<RubyMeasure>().addArgument("wwr","0.4");
  variables.push_back(MeasureGroup("Var 1",measures));
  measures.clear();
  measures.push_back(NullMeasure());
  measures.push_back(RubyMeasure(toPath("script2.rb"),FileReferenceType::OSM,FileReferenceType::OSM));
  measures.back().cast<RubyMeasure>().addArgument("cop","3.0");
  measures.back().cast<RubyMeasure>().addArgument("fan_eff","0.3");
  measures.push_back(RubyMeasure(toPath("script2.rb"),FileReferenceType::OSM,FileReferenceType::OSM));
  measures.back().cast<RubyMeasure>().addArgument("cop","3.5");
  measures.back().cast<RubyMeasure>().addArgument("fan_eff","0.3");
  measures.push_back(RubyMeasure(toPath("script2.rb"),FileReferenceType::OSM,FileReferenceType::OSM));
  measures.back().cast<RubyMeasure>().addArgument("cop","3.0");
  measures.back().cast<RubyMeasure>().addArgument("fan_eff","0.5");
  measures.push_back(RubyMeasure(toPath("script2.rb"),FileReferenceType::OSM,FileReferenceType::OSM));
  measures.back().cast<RubyMeasure>().addArgument("cop","3.5");
  measures.back().cast<RubyMeasure>().addArgument("fan_eff","0.5");
  variables.push_back(MeasureGroup("Var 2",measures));
  measures.clear();
  Problem dProblem("Discrete Problem",variables,runmanager::Workflow());
  EXPECT_EQ(2,dProblem.numVariables());
  EXPECT_EQ(2,dProblem.numDiscreteVariables());
  EXPECT_EQ(0,dProblem.numStaticTransformations());
  variables.clear();

  // box-behnken
  DDACEAlgorithmOptions options(DDACEAlgorithmType::box_behnken);
  DDACEAlgorithm algorithm(options);
  EXPECT_TRUE(algorithm.isCompatibleProblemType(cProblem));
  EXPECT_TRUE(algorithm.isCompatibleProblemType(mProblem));
  EXPECT_FALSE(algorithm.isCompatibleProblemType(dProblem));
  options.setSamples(DDACEAlgorithmOptions::samplesForBoxBehnken(cProblem));
  ASSERT_TRUE(algorithm.ddaceAlgorithmOptions().samples());
  EXPECT_EQ(81,algorithm.ddaceAlgorithmOptions().samples().get());
  EXPECT_EQ(5,DDACEAlgorithmOptions::samplesForBoxBehnken(mProblem));
  EXPECT_EQ(0,DDACEAlgorithmOptions::samplesForBoxBehnken(dProblem));

  // central-composite
  options = DDACEAlgorithmOptions(DDACEAlgorithmType::central_composite);
  algorithm = DDACEAlgorithm(options);
  EXPECT_TRUE(algorithm.isCompatibleProblemType(cProblem));
  EXPECT_TRUE(algorithm.isCompatibleProblemType(mProblem));
  EXPECT_FALSE(algorithm.isCompatibleProblemType(dProblem));
  options.setSamples(DDACEAlgorithmOptions::samplesForCentralComposite(cProblem));
  ASSERT_TRUE(algorithm.ddaceAlgorithmOptions().samples());
  EXPECT_EQ(43,algorithm.ddaceAlgorithmOptions().samples().get());
  EXPECT_EQ(9,DDACEAlgorithmOptions::samplesForCentralComposite(mProblem));
  EXPECT_EQ(0,DDACEAlgorithmOptions::samplesForCentralComposite(dProblem));

  // grid
  options = DDACEAlgorithmOptions(DDACEAlgorithmType::grid);
  algorithm = DDACEAlgorithm(options);
  EXPECT_TRUE(algorithm.isCompatibleProblemType(cProblem));
  EXPECT_TRUE(algorithm.isCompatibleProblemType(mProblem));
  EXPECT_FALSE(algorithm.isCompatibleProblemType(dProblem));
  EXPECT_TRUE(options.setSamplesForGrid(3,cProblem));
  ASSERT_TRUE(algorithm.ddaceAlgorithmOptions().symbols());
  ASSERT_TRUE(algorithm.ddaceAlgorithmOptions().samples());
  EXPECT_EQ(3,algorithm.ddaceAlgorithmOptions().symbols().get());
  EXPECT_EQ(243,algorithm.ddaceAlgorithmOptions().samples().get());
  EXPECT_TRUE(algorithm.ddaceAlgorithmOptions().setSamplesForGrid(5,mProblem));
  ASSERT_TRUE(algorithm.ddaceAlgorithmOptions().symbols());
  ASSERT_TRUE(algorithm.ddaceAlgorithmOptions().samples());
  EXPECT_EQ(5,algorithm.ddaceAlgorithmOptions().symbols().get());
  EXPECT_EQ(25,algorithm.ddaceAlgorithmOptions().samples().get());
  EXPECT_FALSE(options.setSamplesForGrid(2,dProblem));

  // lhs
  options = DDACEAlgorithmOptions(DDACEAlgorithmType::lhs);
  algorithm = DDACEAlgorithm(options);
  EXPECT_TRUE(algorithm.isCompatibleProblemType(cProblem));
  EXPECT_TRUE(algorithm.isCompatibleProblemType(mProblem));
  EXPECT_TRUE(algorithm.isCompatibleProblemType(dProblem));

  // oa_lhs
  options = DDACEAlgorithmOptions(DDACEAlgorithmType::oa_lhs);
  algorithm = DDACEAlgorithm(options);
  EXPECT_TRUE(algorithm.isCompatibleProblemType(cProblem));
  EXPECT_TRUE(algorithm.isCompatibleProblemType(mProblem));
  EXPECT_TRUE(algorithm.isCompatibleProblemType(dProblem));

  // oas
  options = DDACEAlgorithmOptions(DDACEAlgorithmType::oas);
  algorithm = DDACEAlgorithm(options);
  EXPECT_TRUE(algorithm.isCompatibleProblemType(cProblem));
  EXPECT_TRUE(algorithm.isCompatibleProblemType(mProblem));
  EXPECT_TRUE(algorithm.isCompatibleProblemType(dProblem));

  // random
  options = DDACEAlgorithmOptions(DDACEAlgorithmType::random);
  algorithm = DDACEAlgorithm(options);
  EXPECT_TRUE(algorithm.isCompatibleProblemType(cProblem));
  EXPECT_TRUE(algorithm.isCompatibleProblemType(mProblem));
  EXPECT_TRUE(algorithm.isCompatibleProblemType(dProblem));
}

TEST_F(AnalysisFixture,DDACEAlgorithm_Clone) {
  DDACEAlgorithmOptions options(DDACEAlgorithmType::oa_lhs);
  options.setSamplesAndSymbolsForOrthogonalArray(53,1);
  EXPECT_TRUE(options.seed()); // default is to generate a fixed seed
  EXPECT_TRUE(options.symbols());
  EXPECT_TRUE(options.samples());

  DDACEAlgorithm algorithm(options);
  algorithm.setDisplayName("OA-LHS Algorithm with 53 Symbols");

  DDACEAlgorithm algorithmClone = algorithm.clone().cast<DDACEAlgorithm>();
  DDACEAlgorithmOptions optionsClone = algorithmClone.ddaceAlgorithmOptions().clone().cast<DDACEAlgorithmOptions>();
  
  EXPECT_FALSE(algorithm.uuid() == algorithmClone.uuid());
  EXPECT_FALSE(algorithm.versionUUID() == algorithmClone.versionUUID());
  EXPECT_EQ(algorithm.name(),algorithmClone.name());
  EXPECT_EQ(algorithm.displayName(),algorithmClone.displayName());
  EXPECT_EQ(algorithm.ddaceAlgorithmOptions().algorithmType(),algorithmClone.ddaceAlgorithmOptions().algorithmType());

  // all Attributes should be equivalent but have different UUIDs
  AttributeVector attributes = options.options();
  AttributeVector attributeClones = optionsClone.options();
  ASSERT_EQ(attributes.size(),attributeClones.size());
  for (unsigned i = 0, n = attributes.size(); i < n; ++i) {
    EXPECT_TRUE(attributes[i] == attributeClones[i]);
    EXPECT_FALSE(attributes[i].uuid() == attributeClones[i].uuid());
    EXPECT_FALSE(attributes[i].versionUUID() == attributeClones[i].versionUUID());
  }
}
