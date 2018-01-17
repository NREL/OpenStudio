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
  BCLMeasure bclMeasure(resourcesPath() / toPath("utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade"));
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
  BCLMeasure bclMeasure(resourcesPath() / toPath("utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade"));
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
