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
