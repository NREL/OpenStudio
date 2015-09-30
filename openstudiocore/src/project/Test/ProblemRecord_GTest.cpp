/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "../../analysis/MeasureGroup.hpp"
#include "../../analysis/MeasureGroup_Impl.hpp"
#include "../../analysis/NullMeasure.hpp"
#include "../../analysis/Problem.hpp"
#include "../../analysis/Problem_Impl.hpp"
#include "../../analysis/RubyMeasure.hpp"
#include "../../analysis/RubyMeasure_Impl.hpp"
#include "../../analysis/RubyContinuousVariable.hpp"
#include "../../analysis/RubyContinuousVariable_Impl.hpp"

#include "../MeasureGroupRecord.hpp"
#include "../MeasureGroupRecord_Impl.hpp"
#include "../NullMeasureRecord.hpp"
#include "../ProblemRecord.hpp"
#include "../ProjectDatabase.hpp"
#include "../RubyMeasureRecord.hpp"
#include "ProjectFixture.hpp"

#include "../../runmanager/lib/Workflow.hpp"

#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Path.hpp"

#include <resources.hxx>
#include <OpenStudio.hxx>

using namespace openstudio;
using namespace openstudio::analysis;
using namespace openstudio::project;
using namespace openstudio::runmanager;

TEST_F(ProjectFixture, ProblemRecord)
{
  //Logger::instance().logLevel(Debug);

  openstudio::path rubyLibDirPath = openstudio::toPath(rubyLibDir());
  openstudio::path perturbScript = rubyLibDirPath/openstudio::toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");

  // Workflow
  Workflow workflow;

  // Variables
  VariableVector variables;

  // Measures
  MeasureVector perturbations1;
  MeasureVector perturbations2;

  // Discrete Variable Records
  InputVariableRecordVector measureGroupRecords;

  {
    // Problem
    Problem problem("0 Variables",variables,workflow);

    // Project Database
    ProjectDatabase database = getCleanDatabase("ProblemRecord");

    // Problem Record
    ProblemRecord problemRecord = ProblemRecord::factoryFromProblem(problem,database);

    InputVariableRecordVector measureGroupRecords = problemRecord.inputVariableRecords();

    EXPECT_EQ(0u,measureGroupRecords.size());
  }

  {
    perturbations1.push_back(NullMeasure());

    variables.push_back(analysis::MeasureGroup("",perturbations1));

    // Problem
    Problem problem("perturbations1",variables,workflow);

    // Project Database
    ProjectDatabase database = getCleanDatabase("ProblemRecord");

    // Problem Record
    ProblemRecord problemRecord = ProblemRecord::factoryFromProblem(problem,database);

    measureGroupRecords = problemRecord.inputVariableRecords();

    EXPECT_EQ(1u,measureGroupRecords.size());

    // Discrete Variable Record
    MeasureGroupRecord measureGroupRecord = measureGroupRecords.at(0).cast<MeasureGroupRecord>();

    EXPECT_EQ(1u,measureGroupRecord.measureRecordIds(true).size());
    EXPECT_EQ(1u,measureGroupRecord.measureRecords(true).size());
  }

  {
    variables.push_back(MeasureGroup("Wall Construction",perturbations1));

    // Problem
    Problem problem("perturbations1",variables,workflow);

    // Project Database
    ProjectDatabase database = getCleanDatabase("ProblemRecord");

    // Problem Record
    ProblemRecord problemRecord = ProblemRecord::factoryFromProblem(problem,database);

    measureGroupRecords = problemRecord.inputVariableRecords();

    EXPECT_EQ(2u,measureGroupRecords.size());

    // Discrete Variable Record
    MeasureGroupRecord variable1 = measureGroupRecords.at(0).cast<MeasureGroupRecord>();

    EXPECT_EQ(static_cast<unsigned>(1), variable1.measureRecords(true).size());
    EXPECT_EQ(static_cast<unsigned>(1), variable1.measureRecords(false).size());
    EXPECT_EQ(static_cast<unsigned>(2), problemRecord.inputVariableRecords().size());
  }

  {
    perturbations2.push_back(NullMeasure());

    variables.push_back(MeasureGroup("Roof Construction",perturbations2));

    // Problem
    Problem problem("perturbations2",variables,workflow);

    // Project Database
    ProjectDatabase database = getCleanDatabase("ProblemRecord");

    // Problem Record
    ProblemRecord problemRecord = ProblemRecord::factoryFromProblem(problem,database);

    measureGroupRecords = problemRecord.inputVariableRecords();

    EXPECT_EQ(3u,measureGroupRecords.size());

    // Discrete Variable Record
    MeasureGroupRecord variable1 = measureGroupRecords.at(0).cast<MeasureGroupRecord>();
    MeasureGroupRecord variable2 = measureGroupRecords.at(0).cast<MeasureGroupRecord>();
    MeasureGroupRecord variable3 = measureGroupRecords.at(0).cast<MeasureGroupRecord>();

    EXPECT_EQ(static_cast<size_t>(1), variable1.measureRecords(true).size());
    EXPECT_EQ(static_cast<size_t>(1), variable1.measureRecords(false).size());
    EXPECT_EQ(static_cast<size_t>(3), problemRecord.inputVariableRecords().size());

    UUID uuid1;
    UUID versionUUID1;
    NullMeasure nullMeasure1(uuid1,versionUUID1,"","","",true);
    NullMeasureRecord nullMeasureRecord1(nullMeasure1,variable1,0);

    EXPECT_EQ(static_cast<unsigned>(2), variable1.measureRecords(true).size());
    EXPECT_EQ(static_cast<unsigned>(2), variable1.measureRecords(false).size());
    EXPECT_EQ(static_cast<unsigned>(2), variable2.measureRecords(true).size());
    EXPECT_EQ(static_cast<unsigned>(2), variable2.measureRecords(false).size());

    UUID uuid2;
    UUID versionUUID2;
    NullMeasure nullMeasure2(uuid2,versionUUID2,"","","",true);
    NullMeasureRecord nullMeasureRecord2(nullMeasure2,variable1,0);

    EXPECT_EQ(static_cast<unsigned>(2), variable1.measureRecords(true).size());
    EXPECT_EQ(static_cast<unsigned>(2), variable1.measureRecords(false).size());
    EXPECT_EQ(static_cast<unsigned>(2), variable2.measureRecords(true).size());
    EXPECT_EQ(static_cast<unsigned>(2), variable2.measureRecords(false).size());

    MeasureVector perturbations;
    MeasureGroup measureGroup("Wall Construction",perturbations);
    MeasureGroupRecord measureGroupRecord(measureGroup, problemRecord,0);

    openstudio::path rubyPath = resourcesPath() / openstudio::toPath("project/rubyscripts/*.rb");
    openstudio::path perturbScript = rubyPath/openstudio::toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");
    RubyMeasure rubyMeasure(perturbScript,FileReferenceType::OSM,FileReferenceType::OSM);
    RubyMeasureRecord rubyMeasureRecord1(rubyMeasure,measureGroupRecord,0);

    EXPECT_EQ(static_cast<unsigned>(1), measureGroupRecord.numMeasures(true));
    EXPECT_EQ(static_cast<unsigned>(1), measureGroupRecord.measureRecordIds(true).size());
    EXPECT_EQ(static_cast<unsigned>(1), measureGroupRecord.measureRecords(true).size());

    RubyMeasureRecord rubyMeasureRecord(rubyMeasure,measureGroupRecord,0);

    EXPECT_EQ(static_cast<unsigned>(1), measureGroupRecord.numMeasures(true));
    EXPECT_EQ(static_cast<unsigned>(1), measureGroupRecord.measureRecordIds(true).size());
    EXPECT_EQ(rubyMeasureRecord.id(), measureGroupRecord.measureRecordIds(true)[0]);
    EXPECT_EQ(static_cast<unsigned>(1), measureGroupRecord.measureRecords(true).size());
    EXPECT_EQ(rubyMeasureRecord.handle(), measureGroupRecord.measureRecords(true)[0].handle());
  }
}

TEST_F(ProjectFixture, ProblemRecord_RubyContinuousVariables)
{
  // Create a database containing a ProblemRecord with a chain of 
  // RubyContinuousVariables that reference the same RubyMeasure
  analysis::Problem problem("Contains Linked RubyContinuousVariables");

  // Measure with two RubyContinuousVariables  
  analysis::RubyMeasure wwrScript(openstudio::toPath(rubyLibDir()) / 
                                  openstudio::toPath("openstudio/sketchup_plugin/user_scripts/Alter or Add Model Elements/Set_Window_to_Wall_Ratio.rb"),
                                  FileReferenceType::OSM,
                                  FileReferenceType::OSM,
                                  true);
  ruleset::OSArgument arg = ruleset::OSArgument::makeDoubleArgument("wwr");
  analysis::RubyContinuousVariable wwr("Window to Wall Ratio",arg,wwrScript);
  wwr.setMinimum(0.15);
  wwr.setMaximum(0.60);
  problem.push(wwr);
  arg = ruleset::OSArgument::makeDoubleArgument("offset");
  analysis::RubyContinuousVariable offset("Window Offset from Floor",arg,wwrScript);
  offset.setMinimum(0.5);
  offset.setMaximum(2.0);
  problem.push(offset);
  EXPECT_EQ(wwr.measure(), offset.measure());
  
  // Project Database
  ProjectDatabase database = getCleanDatabase("ProblemRecord_RubyContinuousVariables");

  // Problem Record
  ProblemRecord problemRecord = ProblemRecord::factoryFromProblem(problem,database);
  database.save();

  // Deserialize the problem and verify that the deserialized RubyMeasures are 
  // all equal.  
  problemRecord = ProblemRecord::getProblemRecords(database)[0];
  problem = problemRecord.problem();
  ASSERT_EQ(2u,problem.variables().size());
  ASSERT_TRUE(problem.variables()[0].optionalCast<RubyContinuousVariable>());
  ASSERT_TRUE(problem.variables()[1].optionalCast<RubyContinuousVariable>());
  EXPECT_EQ(problem.variables()[0].cast<RubyContinuousVariable>().measure(), 
            problem.variables()[1].cast<RubyContinuousVariable>().measure());
}

