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
#include <project/Test/ProjectFixture.hpp>

#include <project/DiscreteVariableRecord.hpp>
#include <project/DiscreteVariableRecord_Impl.hpp>
#include <project/DiscretePerturbationRecord.hpp>
#include <project/FileReferenceRecord.hpp>
#include <project/OSArgumentRecord.hpp>
#include <project/ProblemRecord.hpp>
#include <project/ProjectDatabase.hpp>
#include <project/RubyPerturbationRecord.hpp>
#include <project/RubyPerturbationRecord_Impl.hpp>

#include <analysis/DiscretePerturbation.hpp>
#include <analysis/DiscreteVariable.hpp>
#include <analysis/DiscreteVariable_Impl.hpp>
#include <analysis/NullPerturbation.hpp>
#include <analysis/Problem.hpp>
#include <analysis/RubyPerturbation.hpp>
#include <analysis/RubyPerturbation_Impl.hpp>
#include <analysis/WorkflowStep.hpp>

#include <runmanager/lib/Workflow.hpp>

#include <ruleset/OSArgument.hpp>

#include <utilities/bcl/BCLMeasure.hpp>

#include <OpenStudio.hxx>

#include <boost/filesystem.hpp>

using namespace openstudio;
using namespace openstudio::ruleset;
using namespace openstudio::analysis;
using namespace openstudio::project;

TEST_F(ProjectFixture, RubyPerturbationRecord_RubyScript) {
  // Perturbations
  DiscretePerturbationVector perturbations;

  // Null Perturbation
  perturbations.push_back(NullPerturbation());

  openstudio::path rubyLibDirPath = openstudio::toPath(rubyLibDir());
  openstudio::path perturbScript = rubyLibDirPath/openstudio::toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");
  RubyPerturbation rubyPerturbation(perturbScript,
                                    FileReferenceType::OSM,
                                    FileReferenceType::OSM);
  rubyPerturbation.addArgument("inputPath", "in.osm");
  rubyPerturbation.addArgument("outputPath", "out.osm");
  rubyPerturbation.addArgument("objectType", "OS:Material");
  rubyPerturbation.addArgument("nameRegex", "I02 50mm insulation board");
  rubyPerturbation.addArgument("field", "3");
  rubyPerturbation.addArgument("value", "0.10");

  // RubyPerturbation
  perturbations.push_back(rubyPerturbation);

  // Variables
  VariableVector variables;

  variables.push_back(DiscreteVariable("Wall Construction",perturbations));

  // Workflow
  openstudio::runmanager::Workflow workflow;

  // Problem
  Problem problem("Variable",variables,workflow);

  // Save to database
  {
    ProjectDatabase database = getCleanDatabase("RubyPerturbationRecord_RubyScript");

    bool didStartTransaction = database.startTransaction();
    EXPECT_TRUE(didStartTransaction);

    // Problem Record
    ProblemRecord problemRecord = ProblemRecord::factoryFromProblem(problem,database);

    database.save();
    if (didStartTransaction) {
      EXPECT_TRUE(database.commitTransaction());
    }

    // Variable Records
    InputVariableRecordVector discreteVariableRecords = problemRecord.inputVariableRecords();
    EXPECT_EQ(1u,discreteVariableRecords.size());

    // Discrete Variable Record
    DiscreteVariableRecord discreteVariableRecord = discreteVariableRecords.at(0).cast<DiscreteVariableRecord>();
    EXPECT_EQ(2u,discreteVariableRecord.discretePerturbationRecordIds(true).size());
    EXPECT_EQ(2u,discreteVariableRecord.discretePerturbationRecords(true).size());
    RubyPerturbationRecord rubyPerturbationRecord(rubyPerturbation,discreteVariableRecord,0);
    EXPECT_EQ("DiscretePerturbationRecords",rubyPerturbationRecord.databaseTableName());
    ObjectRecordVector objectRecordVector = rubyPerturbationRecord.children();
    EXPECT_EQ(6u,objectRecordVector.size()); // arguments
    objectRecordVector = rubyPerturbationRecord.resources();
    EXPECT_EQ(1u,objectRecordVector.size()); // script
    FileReferenceRecord scriptRecord = rubyPerturbationRecord.scriptOrBCLMeasureRecord();
    EXPECT_EQ("FileReferenceRecords",scriptRecord.databaseTableName());

    DiscretePerturbation discretePerturbation = rubyPerturbationRecord.discretePerturbation();
    EXPECT_EQ(true,discretePerturbation.isSelected());
    ASSERT_TRUE(discretePerturbation.optionalCast<RubyPerturbation>());
    RubyPerturbation rubyPerturbationCopy = discretePerturbation.cast<RubyPerturbation>();
    EXPECT_FALSE(rubyPerturbationCopy.usesBCLMeasure());
    EXPECT_FALSE(rubyPerturbationCopy.isUserScript());
    EXPECT_EQ(6u,rubyPerturbationCopy.arguments().size());

    DiscreteVariableRecord discreteVariableRecordFromRuby = rubyPerturbationRecord.discreteVariableRecord().get();
    EXPECT_EQ(discreteVariableRecord.databaseTableName(),discreteVariableRecordFromRuby.databaseTableName());
    EXPECT_EQ(discreteVariableRecord.id(),discreteVariableRecordFromRuby.id());
  }

  // Reopen database
  {
    ProjectDatabase database = getExistingDatabase("RubyPerturbationRecord_RubyScript");

    ProblemRecordVector problemRecords = ProblemRecord::getProblemRecords(database);
    ASSERT_FALSE(problemRecords.empty());
    EXPECT_EQ(1u,problemRecords.size());
    ProblemRecord problemRecord = problemRecords[0];

    // COPY-PASTED FROM ABOVE

    // Variable Records
    InputVariableRecordVector discreteVariableRecords = problemRecord.inputVariableRecords();
    EXPECT_EQ(1u,discreteVariableRecords.size());

    // Discrete Variable Record
    DiscreteVariableRecord discreteVariableRecord = discreteVariableRecords.at(0).cast<DiscreteVariableRecord>();
    EXPECT_EQ(2u,discreteVariableRecord.discretePerturbationRecordIds(true).size());
    EXPECT_EQ(2u,discreteVariableRecord.discretePerturbationRecords(true).size());
    RubyPerturbationRecord rubyPerturbationRecord(rubyPerturbation,discreteVariableRecord,0);
    EXPECT_EQ("DiscretePerturbationRecords",rubyPerturbationRecord.databaseTableName());
    ObjectRecordVector objectRecordVector = rubyPerturbationRecord.children();
    EXPECT_EQ(6u,objectRecordVector.size()); // arguments
    objectRecordVector = rubyPerturbationRecord.resources();
    EXPECT_EQ(1u,objectRecordVector.size()); // script
    FileReferenceRecord scriptRecord = rubyPerturbationRecord.scriptOrBCLMeasureRecord();
    EXPECT_EQ("FileReferenceRecords",scriptRecord.databaseTableName());

    DiscretePerturbation discretePerturbation = rubyPerturbationRecord.discretePerturbation();
    EXPECT_EQ(true,discretePerturbation.isSelected());
    ASSERT_TRUE(discretePerturbation.optionalCast<RubyPerturbation>());
    RubyPerturbation rubyPerturbationCopy = discretePerturbation.cast<RubyPerturbation>();
    EXPECT_FALSE(rubyPerturbationCopy.usesBCLMeasure());
    EXPECT_FALSE(rubyPerturbationCopy.isUserScript());
    EXPECT_EQ(6u,rubyPerturbationCopy.arguments().size());

    DiscreteVariableRecord discreteVariableRecordFromRuby = rubyPerturbationRecord.discreteVariableRecord().get();
    EXPECT_EQ(discreteVariableRecord.databaseTableName(),discreteVariableRecordFromRuby.databaseTableName());
    EXPECT_EQ(discreteVariableRecord.id(),discreteVariableRecordFromRuby.id());
  }
}

TEST_F(ProjectFixture, RubyPerturbationRecord_BCLMeasure) {
  // Construct problem with RubyPerturbation that points to BCLMeasure
  Problem problem("Problem",VariableVector(),runmanager::Workflow());
  DiscreteVariable dvar("Variable",DiscretePerturbationVector());
  problem.push(dvar);
  openstudio::path measuresPath = resourcesPath() / toPath("/utilities/BCL/Measures");
  openstudio::path dir = measuresPath / toPath("SetWindowToWallRatioByFacade");
  BCLMeasure measure = BCLMeasure::load(dir).get();
  RubyPerturbation rpert(measure);
  dvar.push(rpert);
  OSArgument arg = OSArgument::makeDoubleArgument("wwr");
  arg.setValue(0.4);
  rpert.setArgument(arg);
  arg = OSArgument::makeIntegerArgument("typo_arg");
  arg.setDefaultValue(1);
  rpert.setArgument(arg);

  // Serialize to database
  {
    ProjectDatabase database = getCleanDatabase("RubyPerturbationRecord_BCLMeasure");

    bool didStartTransaction = database.startTransaction();
    EXPECT_TRUE(didStartTransaction);

    // Problem Record
    ProblemRecord problemRecord = ProblemRecord::factoryFromProblem(problem,database);

    database.save();
    if (didStartTransaction) {
      EXPECT_TRUE(database.commitTransaction());
    }
  }

  // Re-open database, de-serialize, verify that RubyPerturbation is intact.
  openstudio::path tempDir1 = measuresPath / toPath(toString(createUUID()));
  {
    ProjectDatabase database = getExistingDatabase("RubyPerturbationRecord_BCLMeasure");
    ASSERT_EQ(1u,ProblemRecord::getProblemRecords(database).size());
    ASSERT_EQ(1u,DiscreteVariableRecord::getDiscreteVariableRecords(database).size());
    EXPECT_EQ(1u,RubyPerturbationRecord::getRubyPerturbationRecords(database).size());

    DiscretePerturbationRecordVector dprs = DiscreteVariableRecord::getDiscreteVariableRecords(database)[0].discretePerturbationRecords(false);
    ASSERT_EQ(1u,dprs.size());
    ASSERT_TRUE(dprs[0].optionalCast<RubyPerturbationRecord>());
    RubyPerturbationRecord rpr = dprs[0].cast<RubyPerturbationRecord>();
    RubyPerturbation rp = rpr.rubyPerturbation();
    EXPECT_TRUE(rp.usesBCLMeasure());
    EXPECT_TRUE(rp.measure());
    EXPECT_EQ(dir,rp.measureDirectory());
    EXPECT_EQ(measure.uuid(),rp.measureUUID());
    EXPECT_EQ(measure.versionUUID(),rp.measureVersionUUID());
    EXPECT_ANY_THROW(rp.perturbationScript());
    EXPECT_EQ(2u,rp.arguments().size());
    EXPECT_FALSE(rp.hasIncompleteArguments());

    // Update measure and save
    BCLMeasure newVersion = measure.clone(tempDir1).get();
    newVersion.setDescription("Window to wall ratio with sill height configurable.");
    newVersion.save();
    EXPECT_NE(measure.versionUUID(),newVersion.versionUUID());
    OSArgumentVector args;
    args.push_back(OSArgument::makeDoubleArgument("wwr"));
    args.push_back(OSArgument::makeDoubleArgument("sillHeight"));

    Problem problemCopy = ProblemRecord::getProblemRecords(database)[0].problem();
    problemCopy.updateMeasure(newVersion,args,false);

    bool didStartTransaction = database.startTransaction();
    EXPECT_TRUE(didStartTransaction);

    // Problem Record
    ProblemRecord problemRecord = ProblemRecord::factoryFromProblem(problemCopy,database);

    database.save();
    if (didStartTransaction) {
      EXPECT_TRUE(database.commitTransaction());
    }
  }

  // Re-open database, check that old argument records are gone, check that de-serialized object ok
  openstudio::path tempDir2 = measuresPath / toPath(toString(createUUID()));
  {
    ProjectDatabase database = getExistingDatabase("RubyPerturbationRecord_BCLMeasure");
    ASSERT_EQ(1u,ProblemRecord::getProblemRecords(database).size());
    EXPECT_EQ(1u,DiscreteVariableRecord::getDiscreteVariableRecords(database).size());
    EXPECT_EQ(1u,RubyPerturbationRecord::getRubyPerturbationRecords(database).size());
    EXPECT_EQ(1u,FileReferenceRecord::getFileReferenceRecords(database).size());
    EXPECT_EQ(2u,OSArgumentRecord::getOSArgumentRecords(database).size());

    Problem problemCopy = ProblemRecord::getProblemRecords(database)[0].problem();
    InputVariableVector vars = problemCopy.variables();
    ASSERT_FALSE(vars.empty());
    ASSERT_TRUE(vars[0].optionalCast<DiscreteVariable>());
    DiscretePerturbationVector dps = vars[0].cast<DiscreteVariable>().perturbations(false);
    ASSERT_FALSE(dps.empty());
    ASSERT_TRUE(dps[0].optionalCast<RubyPerturbation>());
    RubyPerturbation rp = dps[0].cast<RubyPerturbation>();
    EXPECT_TRUE(rp.usesBCLMeasure());
    EXPECT_TRUE(rp.measure());
    EXPECT_EQ(tempDir1,rp.measureDirectory());
    EXPECT_EQ(measure.uuid(),rp.measureUUID());
    EXPECT_NE(measure.versionUUID(),rp.measureVersionUUID());
    EXPECT_ANY_THROW(rp.perturbationScript());
    ASSERT_EQ(2u,rp.arguments().size());
    EXPECT_TRUE(rp.hasIncompleteArguments());
    EXPECT_EQ("wwr",rp.arguments()[0].name());
    ASSERT_EQ(1u,rp.incompleteArguments().size());
    EXPECT_EQ("sillHeight",rp.incompleteArguments()[0].name());

    // Set to different measure
    BCLMeasure measure2 = measure.clone(tempDir2).get();
    measure2.changeUID();
    measure2.incrementVersionId();
    measure2.save();
    measure2 = BCLMeasure::load(tempDir2).get();
    EXPECT_NE(measure.uuid(),measure2.uuid());
    EXPECT_NE(measure.versionUUID(),measure2.versionUUID());
    rp.setMeasure(measure2);
    EXPECT_TRUE(rp.isDirty());
    EXPECT_TRUE(problemCopy.isDirty());

    bool didStartTransaction = database.startTransaction();
    EXPECT_TRUE(didStartTransaction);

    // Problem Record
    ProblemRecord problemRecord = ProblemRecord::factoryFromProblem(problemCopy,database);

    database.save();
    if (didStartTransaction) {
      EXPECT_TRUE(database.commitTransaction());
    }
  }

  // Re-open database, check that old measure and all argument records are gone
  {
    ProjectDatabase database = getExistingDatabase("RubyPerturbationRecord_BCLMeasure");
    ASSERT_EQ(1u,ProblemRecord::getProblemRecords(database).size());
    EXPECT_EQ(1u,DiscreteVariableRecord::getDiscreteVariableRecords(database).size());
    EXPECT_EQ(1u,RubyPerturbationRecord::getRubyPerturbationRecords(database).size());
    EXPECT_EQ(1u,FileReferenceRecord::getFileReferenceRecords(database).size());
    EXPECT_EQ(0u,OSArgumentRecord::getOSArgumentRecords(database).size());

    Problem problemCopy = ProblemRecord::getProblemRecords(database)[0].problem();
    InputVariableVector vars = problemCopy.variables();
    ASSERT_FALSE(vars.empty());
    ASSERT_TRUE(vars[0].optionalCast<DiscreteVariable>());
    DiscretePerturbationVector dps = vars[0].cast<DiscreteVariable>().perturbations(false);
    ASSERT_FALSE(dps.empty());
    ASSERT_TRUE(dps[0].optionalCast<RubyPerturbation>());
    RubyPerturbation rp = dps[0].cast<RubyPerturbation>();
    EXPECT_TRUE(rp.usesBCLMeasure());
    EXPECT_TRUE(rp.measure());
    EXPECT_EQ(tempDir2,rp.measureDirectory());
    EXPECT_NE(measure.uuid(),rp.measureUUID());
    EXPECT_NE(measure.versionUUID(),rp.measureVersionUUID());
    EXPECT_ANY_THROW(rp.perturbationScript());
    ASSERT_EQ(0u,rp.arguments().size());
    EXPECT_FALSE(rp.hasIncompleteArguments());
  }

  boost::filesystem::remove_all(tempDir1);
  boost::filesystem::remove_all(tempDir2);
}

