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
#include "ProjectFixture.hpp"

#include "../ProjectDatabase.hpp"
#include "../ProblemRecord.hpp"
#include "../ProblemRecord_Impl.hpp"

#include "../../analysis/Problem.hpp"
#include "../../analysis/Problem_Impl.hpp"
#include "../../analysis/RubyMeasure.hpp"
#include "../../analysis/RubyMeasure_Impl.hpp"
#include "../../analysis/RubyContinuousVariable.hpp"
#include "../../analysis/RubyContinuousVariable_Impl.hpp"

#include "../../ruleset/OSArgument.hpp"

#include "../../runmanager/lib/RunManager.hpp"
#include "../../runmanager/lib/Workflow.hpp"

#include <resources.hxx>
#include <OpenStudio.hxx>

using namespace openstudio;
using namespace openstudio::analysis;
using namespace openstudio::project;

TEST_F(ProjectFixture, RubyContinuousVariableRecord_SaveAndRetrieve)
{
  openstudio::path perturbScript = openstudio::toPath(rubyLibDir()) /
                                   openstudio::toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");
  RubyMeasure rubyMeasure(perturbScript,FileReferenceType::OSM,FileReferenceType::OSM);
  rubyMeasure.addArgument("objectType","OS:Material");
  rubyMeasure.addArgument("nameRegex","Insulation");
  rubyMeasure.addArgument("field","2");
  ruleset::OSArgument thickness = ruleset::OSArgument::makeDoubleArgument("Thickness");
  RubyContinuousVariable insulationThickness("Insulation Thickness",thickness,rubyMeasure);

  Problem problem("Trivial Problem",VariableVector(1u,insulationThickness),runmanager::Workflow());


  int id(0);
  {
    ProjectDatabase database = getCleanDatabase("RubyContinuousVariableRecord_SaveAndRetrieve");
    ProblemRecord problemRecord = ProblemRecord::factoryFromProblem(problem,database);
    id = problemRecord.id();
    database.save();
  }

  {
    ProjectDatabase database = getExistingDatabase("RubyContinuousVariableRecord_SaveAndRetrieve");
    OptionalProblemRecord oProblemRecord = ProblemRecord::getProblemRecord(id,database);
    ASSERT_TRUE(oProblemRecord);
    problem = oProblemRecord->problem();
  }

  EXPECT_EQ(1,problem.numVariables());
  ASSERT_TRUE(problem.variables()[0].optionalCast<RubyContinuousVariable>());
  insulationThickness = problem.variables()[0].cast<RubyContinuousVariable>();

  EXPECT_EQ("Thickness",insulationThickness.argument().name());
  EXPECT_EQ(3u,insulationThickness.perturbation().arguments().size());
}
