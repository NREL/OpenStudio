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

#include "../Problem.hpp"
#include "../Variable.hpp"
#include "../Function.hpp"
#include "../Measure.hpp"
#include "../MeasureGroup.hpp"
#include "../MeasureGroup_Impl.hpp"
#include "../NullMeasure.hpp"
#include "../RubyMeasure.hpp"
#include "../RubyMeasure_Impl.hpp"
#include "../WorkflowStep.hpp"

#include "../../runmanager/lib/Workflow.hpp"
#include "../../runmanager/lib/WorkItem.hpp"

#include "../../ruleset/OSArgument.hpp"

#include "../../utilities/bcl/BCLMeasure.hpp"
#include "../../utilities/core/Containers.hpp"

#include <OpenStudio.hxx>
#include <resources.hxx>

using namespace openstudio;
using namespace openstudio::ruleset;
using namespace openstudio::analysis;

TEST_F(AnalysisFixture, Problem_Constructors) {
  VariableVector variables;
  MeasureVector measures;
  runmanager::Workflow workflow;

  // almost-default constructor
  Problem problem("Problem",variables,workflow);
  EXPECT_EQ(0,problem.numVariables());
  OptionalInt combinatorialSize = problem.combinatorialSize(true);
  ASSERT_TRUE(combinatorialSize);
  EXPECT_EQ(0,*combinatorialSize);

  // variables with consistent file types
  variables.clear();
  measures.clear();
  measures.push_back(NullMeasure());
  openstudio::path rubyScriptPath = toPath(rubyLibDir()) /
                                    toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");
  measures.push_back(RubyMeasure(rubyScriptPath,
                                           FileReferenceType::OSM,
                                           FileReferenceType::OSM));
  measures.push_back(RubyMeasure(rubyScriptPath,
                                           FileReferenceType::OSM,
                                           FileReferenceType::OSM));
  variables.push_back(MeasureGroup("Variable 1",measures));
  measures.clear();
  measures.push_back(RubyMeasure(rubyScriptPath,
                                           FileReferenceType::OSM,
                                           FileReferenceType::IDF));
  variables.push_back(MeasureGroup("Variable 2",measures));
  measures.clear();
  measures.push_back(NullMeasure());
  measures.push_back(RubyMeasure(rubyScriptPath,
                                           FileReferenceType::IDF,
                                           FileReferenceType::IDF));
  variables.push_back(MeasureGroup("Variable 3",measures));
  problem = Problem("Problem",variables,workflow);
  EXPECT_EQ(3,problem.numVariables());
  EXPECT_EQ(6,problem.combinatorialSize(true).get());

  // variables with inconistent file types
  variables.clear();
  measures.clear();
  measures.push_back(NullMeasure());
  measures.push_back(RubyMeasure(rubyScriptPath,
                                           FileReferenceType::OSM,
                                           FileReferenceType::OSM));
  measures.push_back(RubyMeasure(rubyScriptPath,
                                           FileReferenceType::OSM,
                                           FileReferenceType::OSM));
  variables.push_back(MeasureGroup("Variable 1",measures));
  measures.clear();
  measures.push_back(NullMeasure());
  measures.push_back(RubyMeasure(rubyScriptPath,
                                           FileReferenceType::IDF,
                                           FileReferenceType::IDF));
  variables.push_back(MeasureGroup("Variable 2",measures));
  EXPECT_THROW(Problem("Problem",variables,workflow),std::exception);

  // variables and non-null workflow with consistent file types
  variables.clear();
  measures.clear();
  measures.push_back(NullMeasure());
  measures.push_back(RubyMeasure(rubyScriptPath,
                                           FileReferenceType::IDF,
                                           FileReferenceType::IDF));
  variables.push_back(MeasureGroup("Variable 1",measures));
  workflow = runmanager::Workflow();
  workflow.addJob(openstudio::runmanager::JobType::EnergyPlus);
  problem = Problem("Problem",variables,workflow);
  problem.setDisplayName("Display Name");
  problem.setDescription("long winded description");
  EXPECT_EQ(1,problem.numVariables());
  EXPECT_EQ(2,problem.combinatorialSize(true).get());

  // deserialization
  Problem problemCopy(problem.uuid(),
                      problem.versionUUID(),
                      problem.name(),
                      problem.displayName(),
                      problem.description(),
                      problem.workflow(),
                      problem.responses());
  EXPECT_FALSE(problem == problemCopy); // different impls
  EXPECT_TRUE(problem.uuid() == problemCopy.uuid());
  EXPECT_TRUE(problem.versionUUID() == problemCopy.versionUUID());
  EXPECT_EQ(problem.name(),problemCopy.name());
  EXPECT_EQ(problem.displayName(),problemCopy.displayName());
  EXPECT_EQ(problem.description(),problemCopy.description());
  EXPECT_TRUE(problem.workflow() == problemCopy.workflow());

  // variables and non-null workflow with inconsistent file types
  workflow = runmanager::Workflow();
  workflow.addJob(openstudio::runmanager::JobType::ModelToIdf);
  workflow.addJob(openstudio::runmanager::JobType::EnergyPlus);
  EXPECT_THROW(Problem("Problem",variables,workflow),std::exception);
}

TEST_F(AnalysisFixture, Problem_FileTypesAreCompatible_VariableNotInProblem) {
  // fileTypesAreCompatible only works with variables that are in the problem
  MeasureGroup notInProblem("Not in Problem",MeasureVector());

  Problem problem("My Problem, Not Yours",VariableVector(),runmanager::Workflow());
  EXPECT_FALSE(problem.fileTypesAreCompatible(notInProblem,
                                              FileReferenceType(FileReferenceType::IDF),
                                              FileReferenceType(FileReferenceType::IDF)));
  EXPECT_FALSE(problem.fileTypesAreCompatible(notInProblem,
                                              FileReferenceType(FileReferenceType::OSM),
                                              FileReferenceType(FileReferenceType::OSM)));
  EXPECT_FALSE(problem.fileTypesAreCompatible(notInProblem,
                                              FileReferenceType(FileReferenceType::OSM),
                                              boost::none));

  problem.push(MeasureGroup("Variable 1",MeasureVector(1u,NullMeasure())));
  problem.push(MeasureGroup("Variable 2",MeasureVector(1u,NullMeasure())));
  problem.push(MeasureGroup("Variable 3",MeasureVector(1u,NullMeasure())));
  problem.push(runmanager::WorkItem(runmanager::JobType::ModelToIdf));
  problem.push(runmanager::WorkItem(runmanager::JobType::EnergyPlus));
  EXPECT_EQ(3u,problem.variables().size());

  EXPECT_FALSE(problem.fileTypesAreCompatible(notInProblem,
                                              FileReferenceType(FileReferenceType::IDF),
                                              FileReferenceType(FileReferenceType::IDF)));
  EXPECT_FALSE(problem.fileTypesAreCompatible(notInProblem,
                                              FileReferenceType(FileReferenceType::OSM),
                                              FileReferenceType(FileReferenceType::OSM)));
  EXPECT_FALSE(problem.fileTypesAreCompatible(notInProblem,
                                              FileReferenceType(FileReferenceType::OSM),
                                              boost::none));
}

TEST_F(AnalysisFixture, Problem_FileTypesAreCompatible_NewMeasureGroup) {
  // ok to insert null-only discrete variable anywhere in the chain
  // (between null-model, model-model, idf-idf, idf-workflow)
  VariableVector variables;
  // 0
  variables.push_back(
        MeasureGroup("Null Variable 1",
                     MeasureVector(1u,NullMeasure())));
  // 1
  variables.push_back(
        MeasureGroup("Model Variable 1",
                         MeasureVector(1u,RubyMeasure(toPath("modelUserScript.rb"),
                                                                        FileReferenceType::OSM,
                                                                        FileReferenceType::OSM,
                                                                        true))));
  // 2
  variables.push_back(
        MeasureGroup("Translation Variable 1",
                         MeasureVector(1u,RubyMeasure(toPath("translationUserScript.rb"),
                                                                        FileReferenceType::OSM,
                                                                        FileReferenceType::IDF,
                                                                        true))));
  // 3
  variables.push_back(
        MeasureGroup("Workspace Variable 1",
                         MeasureVector(1u,RubyMeasure(toPath("workspaceUserScript.rb"),
                                                                        FileReferenceType::IDF,
                                                                        FileReferenceType::IDF,
                                                                        true))));
  runmanager::Workflow workflow;
  workflow.addJob(runmanager::JobType::EnergyPlus);
  Problem problem("Problem 1",variables,workflow);
  MeasureGroup newVar("New Discrete Variable",
                          MeasureVector(1u,NullMeasure()));
  EXPECT_TRUE(problem.insert(4,newVar.clone().cast<InputVariable>()));
  ASSERT_EQ(5,problem.numVariables());
  EXPECT_EQ("New Discrete Variable",problem.variables()[4].name());
  EXPECT_TRUE(problem.insert(3,newVar.clone().cast<InputVariable>()));
  EXPECT_EQ(6,problem.numVariables());
  ASSERT_GE(problem.numVariables(),5);
  EXPECT_EQ("Translation Variable 1",problem.variables()[2].name());
  EXPECT_EQ("New Discrete Variable",problem.variables()[3].name());
  EXPECT_EQ("Workspace Variable 1",problem.variables()[4].name());
  EXPECT_TRUE(problem.insert(2,newVar.clone().cast<InputVariable>()));
  EXPECT_TRUE(problem.insert(1,newVar.clone().cast<InputVariable>()));
  EXPECT_TRUE(problem.insert(0,newVar.clone().cast<InputVariable>()));
  EXPECT_EQ(9,problem.numVariables());
  variables = castVector<Variable>(problem.variables());
  for (int i = 0, n = problem.numVariables(); i < n; ++i) {
    if (i % 2 == 0) {
      EXPECT_EQ("New Discrete Variable",variables[i].name());
    }
    else {
      EXPECT_NE("New Discrete Variable",variables[i].name());
    }
  }

  // (between null-idf, idf-idf, idf-workflow)
  variables.clear();
  // 0
  variables.push_back(
        MeasureGroup("Null Variable 1",
                         MeasureVector(1u,NullMeasure())));
  // 1
  variables.push_back(
        MeasureGroup("Workspace Variable 1",
                         MeasureVector(1u,RubyMeasure(toPath("workspaceUserScript.rb"),
                                                                        FileReferenceType::IDF,
                                                                        FileReferenceType::IDF,
                                                                        true))));
  // 2
  variables.push_back(
        MeasureGroup("Workspace Variable 2",
                         MeasureVector(1u,RubyMeasure(toPath("workspaceUserScript.rb"),
                                                                        FileReferenceType::IDF,
                                                                        FileReferenceType::IDF,
                                                                        true))));
  problem = Problem("Problem 2",variables,workflow);
  EXPECT_TRUE(problem.insert(3,newVar.clone().cast<InputVariable>()));
  EXPECT_TRUE(problem.insert(2,newVar.clone().cast<InputVariable>()));
  EXPECT_TRUE(problem.insert(1,newVar.clone().cast<InputVariable>()));
  EXPECT_TRUE(problem.insert(0,newVar.clone().cast<InputVariable>()));
  EXPECT_EQ(7,problem.numVariables());
  variables = castVector<Variable>(problem.variables());
  for (int i = 0, n = problem.numVariables(); i < n; ++i) {
    if (i % 2 == 0) {
      EXPECT_EQ("New Discrete Variable",variables[i].name());
    }
    else {
      EXPECT_NE("New Discrete Variable",variables[i].name());
    }
  }

  // expected behavior for then adding first measure
  // test with BCLMeasure first. verify with RubyMeasure.
  openstudio::path dir = resourcesPath() / toPath("/utilities/BCL/Measures/SetWindowToWallRatioByFacade");
  BCLMeasure bclMeasure = BCLMeasure::load(dir).get();
  EXPECT_EQ(FileReferenceType(FileReferenceType::OSM),bclMeasure.inputFileType());
  EXPECT_EQ(FileReferenceType(FileReferenceType::OSM),bclMeasure.outputFileType());
  RubyMeasure measure(bclMeasure);

  // have idf-only problem with two null variables at the top
  // try to add measures to first variable
  MeasureGroup firstVariable = problem.variables()[0].cast<MeasureGroup>();
  ASSERT_TRUE(firstVariable.parent());
  WorkflowStep step0 = problem.workflow()[0];
  EXPECT_TRUE(firstVariable.parent().get() == step0);
  EXPECT_EQ(1u,firstVariable.numMeasures(false));
  EXPECT_FALSE(problem.fileTypesAreCompatible(firstVariable,
                                              measure.inputFileType(),
                                              measure.outputFileType()));
  ASSERT_TRUE(firstVariable.parent());
  EXPECT_TRUE(firstVariable.parent().get() == problem.workflow()[0]);
  EXPECT_TRUE(problem.workflow()[0] == step0);
  EXPECT_FALSE(firstVariable.push(measure));
  EXPECT_EQ(1u,firstVariable.numMeasures(false));

  // make second variable a translation variable
  MeasureGroup secondVariable = problem.variables()[1].cast<MeasureGroup>();
  EXPECT_FALSE(secondVariable.push(RubyMeasure(toPath("myTranslationScript.rb"),
                                                    FileReferenceType::OSM,
                                                    FileReferenceType::IDF)));
  secondVariable.erase(secondVariable.measures(false)[0]);
  EXPECT_TRUE(secondVariable.push(RubyMeasure(toPath("myTranslationScript.rb"),
                                                   FileReferenceType::OSM,
                                                   FileReferenceType::IDF)));

  // now should be able to add OSM measure
  EXPECT_TRUE(problem.fileTypesAreCompatible(firstVariable,
                                             measure.inputFileType(),
                                             measure.outputFileType()));
  EXPECT_TRUE(firstVariable.push(measure));
  EXPECT_EQ(2u,firstVariable.numMeasures(false));
}

TEST_F(AnalysisFixture, Problem_FileTypesAreCompatible_ExistingMeasureGroup) {
  // expected behavior when discrete variable already contains measures
  // test with BCLMeasure first. verify with RubyMeasure.

  // create problem
  VariableVector variables;
  variables.push_back(
        MeasureGroup("Model Variable 1",
                         MeasureVector(1u,RubyMeasure(toPath("modelUserScript.rb"),
                                                                        FileReferenceType::OSM,
                                                                        FileReferenceType::OSM,
                                                                        true))));
  variables.back().cast<MeasureGroup>().insert(0,NullMeasure());
  // 2
  variables.push_back(
        MeasureGroup("Translation Variable 1",
                         MeasureVector(1u,RubyMeasure(toPath("translationUserScript.rb"),
                                                                        FileReferenceType::OSM,
                                                                        FileReferenceType::IDF,
                                                                        true))));
  // 3
  variables.push_back(
        MeasureGroup("Workspace Variable 1",
                         MeasureVector(1u,RubyMeasure(toPath("workspaceUserScript.rb"),
                                                                        FileReferenceType::IDF,
                                                                        FileReferenceType::IDF,
                                                                        true))));
  variables.back().cast<MeasureGroup>().insert(0,NullMeasure());
  runmanager::Workflow workflow;
  workflow.addJob(runmanager::JobType::EnergyPlus);
  Problem problem("Problem",variables,workflow);

  // create measure to be pushed onto a variable
  openstudio::path dir = resourcesPath() / toPath("/utilities/BCL/Measures/SetWindowToWallRatioByFacade");
  BCLMeasure bclMeasure = BCLMeasure::load(dir).get();
  EXPECT_EQ(FileReferenceType(FileReferenceType::OSM),bclMeasure.inputFileType());
  EXPECT_EQ(FileReferenceType(FileReferenceType::OSM),bclMeasure.outputFileType());
  RubyMeasure measure(bclMeasure);

  ASSERT_EQ(3,problem.numVariables());

  // can be added to variable 0 (OSM)
  WorkflowStep step = problem.workflow()[0];
  InputVariable var = step.inputVariable();
  EXPECT_EQ(2u,var.cast<MeasureGroup>().numMeasures(false));
  EXPECT_TRUE(problem.fileTypesAreCompatible(step,
                                             measure.inputFileType(),
                                             measure.outputFileType()));
  EXPECT_TRUE(var.cast<MeasureGroup>().push(measure));
  EXPECT_EQ(3u,var.cast<MeasureGroup>().numMeasures(false));

  // cannot be added to variable 1 (translation)
  step = problem.workflow()[1];
  var = step.inputVariable();
  EXPECT_EQ(1u,var.cast<MeasureGroup>().numMeasures(false));
  EXPECT_FALSE(problem.fileTypesAreCompatible(step,
                                              measure.inputFileType(),
                                              measure.outputFileType()));
  EXPECT_FALSE(var.cast<MeasureGroup>().push(measure));
  EXPECT_EQ(1u,var.cast<MeasureGroup>().numMeasures(false));

  // cannot be added to variable 2 (IDF)
  step = problem.variables()[2];
  var = step.inputVariable();
  EXPECT_EQ(2u,var.cast<MeasureGroup>().numMeasures(false));
  EXPECT_FALSE(problem.fileTypesAreCompatible(step,
                                              measure.inputFileType(),
                                              measure.outputFileType()));
  EXPECT_FALSE(var.cast<MeasureGroup>().push(measure));
  EXPECT_EQ(2u,var.cast<MeasureGroup>().numMeasures(false));
}

TEST_F(AnalysisFixture, Problem_UpdateMeasure_MeasureGroups) {
  // open up example measure
  openstudio::path measuresPath = resourcesPath() / toPath("/utilities/BCL/Measures");
  openstudio::path dir = measuresPath / toPath("SetWindowToWallRatioByFacade");
  BCLMeasure measure1 = BCLMeasure::load(dir).get();
  openstudio::path tempDir1 = measuresPath / toPath(toString(createUUID()));
  openstudio::path tempDir2 = measuresPath / toPath(toString(createUUID()));
  {
    // create multiple BCLMeasures
    BCLMeasure measure1_1 = measure1.clone(tempDir1).get();
    measure1_1.setDescription("Window to wall ratio by wwr and offset.");
    measure1_1.save();
    EXPECT_TRUE(measure1_1.uuid() == measure1.uuid());
    EXPECT_FALSE(measure1_1.versionUUID() == measure1.versionUUID());
    BCLMeasure measure2 = measure1.clone(tempDir2).get();
    measure2.changeUID();
    measure2.incrementVersionId();
    measure2.save();
    EXPECT_FALSE(measure2.uuid() == measure1.uuid());
    EXPECT_FALSE(measure2.versionUUID() == measure1.versionUUID());

    // create args for those measures
    OSArgumentVector args1, args1_1, args2;
    args1.push_back(OSArgument::makeDoubleArgument("wwr"));
    args1.push_back(OSArgument::makeDoubleArgument("sillHeight"));
    args1_1.push_back(OSArgument::makeDoubleArgument("wwr"));
    args1_1.push_back(OSArgument::makeDoubleArgument("offset"));
    args1_1.push_back(OSArgument::makeDoubleArgument("vt"));
    args2.push_back(OSArgument::makeIntegerArgument("numPeople"));

    // create a problem that uses multiple BCLMeasures
    Problem problem("Problem",VariableVector(),runmanager::Workflow());
    MeasureGroup dv("South WWR",MeasureVector(1u,NullMeasure()));
    problem.push(dv);
    RubyMeasure rp(measure1);
    rp.setArguments(args1);
    dv.push(rp);
    dv.push(rp.clone().cast<RubyMeasure>());
    ASSERT_EQ(3u,dv.numMeasures(false));
    rp = dv.measures(false)[2].cast<RubyMeasure>();
    EXPECT_EQ(2u,rp.arguments().size());
    EXPECT_TRUE(rp.hasIncompleteArguments());
    dv = MeasureGroup("Occupancy",MeasureVector(1u,NullMeasure()));
    problem.push(dv);
    rp = RubyMeasure(measure2);
    rp.setArguments(args2);
    dv.push(rp);
    OSArgument arg = args2[0].clone();
    arg.setValue(100);
    rp.setArgument(arg);
    EXPECT_EQ(1u,rp.arguments().size());
    EXPECT_FALSE(rp.hasIncompleteArguments());
    dv = MeasureGroup("North WWR",MeasureVector(1u,NullMeasure()));
    problem.push(dv);
    rp = RubyMeasure(measure1);
    rp.setArguments(args1);
    arg = args1[0].clone();
    arg.setValue(0.32);
    rp.setArgument(arg);
    arg = args1[1].clone();
    arg.setValue(1.0);
    rp.setArgument(arg);
    EXPECT_EQ(2u,rp.arguments().size());
    EXPECT_FALSE(rp.hasIncompleteArguments());
    dv.push(rp);
    EXPECT_EQ(2u,dv.numMeasures(false));

    // call update
    problem.clearDirtyFlag();
    problem.updateMeasure(measure1_1,args1_1,false);

    // check state
    VariableVector vars = castVector<Variable>(problem.variables());
    ASSERT_EQ(3u,vars.size());
    EXPECT_TRUE(vars[0].isDirty());
    EXPECT_FALSE(vars[1].isDirty());
    EXPECT_TRUE(vars[2].isDirty());

    dv = vars[0].cast<MeasureGroup>();
    ASSERT_EQ(3u,dv.numMeasures(false));
    MeasureVector ps = dv.measures(false);
    EXPECT_FALSE(ps[0].isDirty());
    EXPECT_TRUE(ps[1].isDirty());
    rp = ps[1].cast<RubyMeasure>();
    EXPECT_EQ(3u,rp.arguments().size());
    EXPECT_EQ(3u,rp.incompleteArguments().size());
    EXPECT_TRUE(ps[2].isDirty());
    rp = ps[2].cast<RubyMeasure>();
    EXPECT_EQ(3u,rp.arguments().size());
    EXPECT_EQ(3u,rp.incompleteArguments().size());

    dv = vars[2].cast<MeasureGroup>();
    ASSERT_EQ(2u,dv.numMeasures(false));
    ps = dv.measures(false);
    EXPECT_FALSE(ps[0].isDirty());
    EXPECT_TRUE(ps[1].isDirty());
    rp = ps[1].cast<RubyMeasure>();
    EXPECT_EQ(3u,rp.arguments().size());
    EXPECT_EQ(2u,rp.incompleteArguments().size());
  }
  boost::filesystem::remove_all(tempDir1);
  boost::filesystem::remove_all(tempDir2);
}
