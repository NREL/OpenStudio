/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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
#include <analysis/test/AnalysisFixture.hpp>

#include <analysis/DDACEAlgorithm.hpp>
#include <analysis/DDACEAlgorithm_Impl.hpp>
#include <analysis/DDACEAlgorithmOptions.hpp>
#include <analysis/DDACEAlgorithmOptions_Impl.hpp>
#include <analysis/Problem.hpp>
#include <analysis/ModelRulesetContinuousVariable.hpp>
#include <analysis/DiscreteVariable.hpp>
#include <analysis/RubyPerturbation.hpp>
#include <analysis/RubyPerturbation_Impl.hpp>
#include <analysis/NullPerturbation.hpp>

#include <ruleset/ModelObjectFilterClause.hpp>

#include <runmanager/lib/Workflow.hpp>

#include <utilities/data/Attribute.hpp>

using namespace openstudio;
using namespace openstudio::analysis;

TEST_F(AnalysisFixture, DDACEAlgorithmOptions) {

  // problem with three variables
  VariableVector variables;
  variables.push_back(ModelRulesetContinuousVariable("Var 1",ruleset::ModelObjectFilterClauseVector(),"attr 1"));
  variables.push_back(ModelRulesetContinuousVariable("Var 2",ruleset::ModelObjectFilterClauseVector(),"attr 2"));
  variables.push_back(ModelRulesetContinuousVariable("Var 3",ruleset::ModelObjectFilterClauseVector(),"attr 3"));
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
  variables.push_back(ModelRulesetContinuousVariable("Var 1",ruleset::ModelObjectFilterClauseVector(),"attr 1"));
  variables.push_back(ModelRulesetContinuousVariable("Var 2",ruleset::ModelObjectFilterClauseVector(),"attr 2"));
  variables.push_back(ModelRulesetContinuousVariable("Var 3",ruleset::ModelObjectFilterClauseVector(),"attr 3"));
  variables.push_back(ModelRulesetContinuousVariable("Var 4",ruleset::ModelObjectFilterClauseVector(),"attr 3"));
  variables.push_back(ModelRulesetContinuousVariable("Var 5",ruleset::ModelObjectFilterClauseVector(),"attr 3"));
  Problem cProblem("Continuous Problem",variables,runmanager::Workflow());
  EXPECT_EQ(5,cProblem.numVariables());
  EXPECT_EQ(5,cProblem.numContinuousVariables());
  variables.clear();

  // mixed problem with three variables, ignorable discrete variable
  DiscretePerturbationVector perturbations;
  variables.push_back(ModelRulesetContinuousVariable("Var 1",ruleset::ModelObjectFilterClauseVector(),"attr 1"));
  perturbations.push_back(RubyPerturbation(toPath("script.rb"),FileReferenceType::OSM,FileReferenceType::OSM));
  variables.push_back(DiscreteVariable("Var 2",perturbations));
  perturbations.clear();
  variables.push_back(ModelRulesetContinuousVariable("Var 3",ruleset::ModelObjectFilterClauseVector(),"attr 3"));
  Problem mProblem("Mixed Problem",variables,runmanager::Workflow());
  EXPECT_EQ(3,mProblem.numVariables());
  EXPECT_EQ(2,mProblem.numContinuousVariables());
  EXPECT_EQ(1,mProblem.numDiscreteVariables());
  EXPECT_EQ(1,mProblem.numStaticTransformations());
  variables.clear();

  // discrete problem
  perturbations.push_back(NullPerturbation());
  perturbations.push_back(RubyPerturbation(toPath("script1.rb"),FileReferenceType::OSM,FileReferenceType::OSM));
  perturbations.back().cast<RubyPerturbation>().addArgument("wwr","0.2");
  perturbations.push_back(RubyPerturbation(toPath("script1.rb"),FileReferenceType::OSM,FileReferenceType::OSM));
  perturbations.back().cast<RubyPerturbation>().addArgument("wwr","0.4");
  variables.push_back(DiscreteVariable("Var 1",perturbations));
  perturbations.clear();
  perturbations.push_back(NullPerturbation());
  perturbations.push_back(RubyPerturbation(toPath("script2.rb"),FileReferenceType::OSM,FileReferenceType::OSM));
  perturbations.back().cast<RubyPerturbation>().addArgument("cop","3.0");
  perturbations.back().cast<RubyPerturbation>().addArgument("fan_eff","0.3");
  perturbations.push_back(RubyPerturbation(toPath("script2.rb"),FileReferenceType::OSM,FileReferenceType::OSM));
  perturbations.back().cast<RubyPerturbation>().addArgument("cop","3.5");
  perturbations.back().cast<RubyPerturbation>().addArgument("fan_eff","0.3");
  perturbations.push_back(RubyPerturbation(toPath("script2.rb"),FileReferenceType::OSM,FileReferenceType::OSM));
  perturbations.back().cast<RubyPerturbation>().addArgument("cop","3.0");
  perturbations.back().cast<RubyPerturbation>().addArgument("fan_eff","0.5");
  perturbations.push_back(RubyPerturbation(toPath("script2.rb"),FileReferenceType::OSM,FileReferenceType::OSM));
  perturbations.back().cast<RubyPerturbation>().addArgument("cop","3.5");
  perturbations.back().cast<RubyPerturbation>().addArgument("fan_eff","0.5");
  variables.push_back(DiscreteVariable("Var 2",perturbations));
  perturbations.clear();
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
