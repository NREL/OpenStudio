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

#include "../MeasureGroupRecord.hpp"
#include "../MeasureGroupRecord_Impl.hpp"
#include "../MeasureRecord.hpp"
#include "../FileReferenceRecord.hpp"
#include "../OSArgumentRecord.hpp"
#include "../ProblemRecord.hpp"
#include "../ProjectDatabase.hpp"
#include "../RubyMeasureRecord.hpp"
#include "../RubyMeasureRecord_Impl.hpp"

#include "../../analysis/Measure.hpp"
#include "../../analysis/MeasureGroup.hpp"
#include "../../analysis/MeasureGroup_Impl.hpp"
#include "../../analysis/NullMeasure.hpp"
#include "../../analysis/Problem.hpp"
#include "../../analysis/RubyMeasure.hpp"
#include "../../analysis/RubyMeasure_Impl.hpp"
#include "../../analysis/WorkflowStep.hpp"

#include "../../runmanager/lib/Workflow.hpp"

#include "../../ruleset/OSArgument.hpp"

#include "../../utilities/bcl/BCLMeasure.hpp"

#include <OpenStudio.hxx>

#include <boost/filesystem.hpp>

using namespace openstudio;
using namespace openstudio::ruleset;
using namespace openstudio::analysis;
using namespace openstudio::project;

TEST_F(ProjectFixture, RubyMeasureRecord_RubyScript) {
  // Measures
  MeasureVector measures;

  // Null Measure
  measures.push_back(NullMeasure());

  openstudio::path rubyLibDirPath = openstudio::toPath(rubyLibDir());
  openstudio::path perturbScript = rubyLibDirPath/openstudio::toPath("openstudio/runmanager/rubyscripts/PerturbObject.rb");
  RubyMeasure rubyMeasure(perturbScript,
                          FileReferenceType::OSM,
                          FileReferenceType::OSM);
  rubyMeasure.addArgument("inputPath", "in.osm");
  rubyMeasure.addArgument("outputPath", "out.osm");
  rubyMeasure.addArgument("objectType", "OS:Material");
  rubyMeasure.addArgument("nameRegex", "I02 50mm insulation board");
  rubyMeasure.addArgument("field", "3");
  rubyMeasure.addArgument("value", "0.10");

  // RubyMeasure
  measures.push_back(rubyMeasure);

  // Variables
  VariableVector variables;

  variables.push_back(MeasureGroup("Wall Construction",measures));

  // Workflow
  openstudio::runmanager::Workflow workflow;

  // Problem
  Problem problem("Variable",variables,workflow);

  // Save to database
  {
    ProjectDatabase database = getCleanDatabase("RubyMeasureRecord_RubyScript");

    bool didStartTransaction = database.startTransaction();
    EXPECT_TRUE(didStartTransaction);

    // Problem Record
    ProblemRecord problemRecord = ProblemRecord::factoryFromProblem(problem,database);

    database.save();
    if (didStartTransaction) {
      EXPECT_TRUE(database.commitTransaction());
    }

    // Variable Records
    InputVariableRecordVector measureGroupRecords = problemRecord.inputVariableRecords();
    EXPECT_EQ(1u,measureGroupRecords.size());

    // Discrete Variable Record
    MeasureGroupRecord measureGroupRecord = measureGroupRecords.at(0).cast<MeasureGroupRecord>();
    EXPECT_EQ(2u,measureGroupRecord.measureRecordIds(true).size());
    EXPECT_EQ(2u,measureGroupRecord.measureRecords(true).size());
    RubyMeasureRecord rubyMeasureRecord(rubyMeasure,measureGroupRecord,0);
    EXPECT_EQ("MeasureRecords",rubyMeasureRecord.databaseTableName());
    ObjectRecordVector objectRecordVector = rubyMeasureRecord.children();
    EXPECT_EQ(6u,objectRecordVector.size()); // arguments
    objectRecordVector = rubyMeasureRecord.resources();
    EXPECT_EQ(1u,objectRecordVector.size()); // script
    FileReferenceRecord scriptRecord = rubyMeasureRecord.fileReferenceRecord();
    EXPECT_EQ("FileReferenceRecords",scriptRecord.databaseTableName());

    Measure measure = rubyMeasureRecord.measure();
    EXPECT_EQ(true,measure.isSelected());
    ASSERT_TRUE(measure.optionalCast<RubyMeasure>());
    RubyMeasure rubyMeasureCopy = measure.cast<RubyMeasure>();
    EXPECT_FALSE(rubyMeasureCopy.usesBCLMeasure());
    EXPECT_FALSE(rubyMeasureCopy.isUserScript());
    EXPECT_EQ(6u,rubyMeasureCopy.arguments().size());

    MeasureGroupRecord measureGroupRecordFromRuby = rubyMeasureRecord.measureGroupRecord().get();
    EXPECT_EQ(measureGroupRecord.databaseTableName(),measureGroupRecordFromRuby.databaseTableName());
    EXPECT_EQ(measureGroupRecord.id(),measureGroupRecordFromRuby.id());
  }

  // Reopen database
  {
    ProjectDatabase database = getExistingDatabase("RubyMeasureRecord_RubyScript");

    ProblemRecordVector problemRecords = ProblemRecord::getProblemRecords(database);
    ASSERT_FALSE(problemRecords.empty());
    EXPECT_EQ(1u,problemRecords.size());
    ProblemRecord problemRecord = problemRecords[0];

    // COPY-PASTED FROM ABOVE

    // Variable Records
    InputVariableRecordVector measureGroupRecords = problemRecord.inputVariableRecords();
    EXPECT_EQ(1u,measureGroupRecords.size());

    // Discrete Variable Record
    MeasureGroupRecord measureGroupRecord = measureGroupRecords.at(0).cast<MeasureGroupRecord>();
    EXPECT_EQ(2u,measureGroupRecord.measureRecordIds(true).size());
    EXPECT_EQ(2u,measureGroupRecord.measureRecords(true).size());
    RubyMeasureRecord rubyMeasureRecord(rubyMeasure,measureGroupRecord,0);
    EXPECT_EQ("MeasureRecords",rubyMeasureRecord.databaseTableName());
    ObjectRecordVector objectRecordVector = rubyMeasureRecord.children();
    EXPECT_EQ(6u,objectRecordVector.size()); // arguments
    objectRecordVector = rubyMeasureRecord.resources();
    EXPECT_EQ(1u,objectRecordVector.size()); // script
    FileReferenceRecord scriptRecord = rubyMeasureRecord.fileReferenceRecord();
    EXPECT_EQ("FileReferenceRecords",scriptRecord.databaseTableName());

    Measure measure = rubyMeasureRecord.measure();
    EXPECT_EQ(true,measure.isSelected());
    ASSERT_TRUE(measure.optionalCast<RubyMeasure>());
    RubyMeasure rubyMeasureCopy = measure.cast<RubyMeasure>();
    EXPECT_FALSE(rubyMeasureCopy.usesBCLMeasure());
    EXPECT_FALSE(rubyMeasureCopy.isUserScript());
    EXPECT_EQ(6u,rubyMeasureCopy.arguments().size());

    MeasureGroupRecord measureGroupRecordFromRuby = rubyMeasureRecord.measureGroupRecord().get();
    EXPECT_EQ(measureGroupRecord.databaseTableName(),measureGroupRecordFromRuby.databaseTableName());
    EXPECT_EQ(measureGroupRecord.id(),measureGroupRecordFromRuby.id());
  }
}

TEST_F(ProjectFixture, RubyMeasureRecord_BCLMeasure) {
  // Construct problem with RubyMeasure that points to BCLMeasure
  Problem problem("Problem",VariableVector(),runmanager::Workflow());
  MeasureGroup dvar("Variable",MeasureVector());
  problem.push(dvar);
  openstudio::path measuresPath = resourcesPath() / toPath("/utilities/BCL/Measures/v2");
  openstudio::path dir = measuresPath / toPath("SetWindowToWallRatioByFacade");
  ASSERT_TRUE(BCLMeasure::load(dir));
  BCLMeasure measure = BCLMeasure::load(dir).get();
  RubyMeasure rpert(measure);
  dvar.push(rpert);
  OSArgument arg = OSArgument::makeDoubleArgument("wwr");
  arg.setValue(0.4);
  rpert.setArgument(arg);
  arg = OSArgument::makeIntegerArgument("typo_arg");
  arg.setDefaultValue(1);
  rpert.setArgument(arg);

  // Serialize to database
  {
    ProjectDatabase database = getCleanDatabase("RubyMeasureRecord_BCLMeasure");

    bool didStartTransaction = database.startTransaction();
    EXPECT_TRUE(didStartTransaction);

    // Problem Record
    ProblemRecord problemRecord = ProblemRecord::factoryFromProblem(problem,database);

    database.save();
    if (didStartTransaction) {
      EXPECT_TRUE(database.commitTransaction());
    }
  }

  // Re-open database, de-serialize, verify that RubyMeasure is intact.
  openstudio::path tempDir1 = measuresPath / toPath(toString(createUUID()));
  {
    ProjectDatabase database = getExistingDatabase("RubyMeasureRecord_BCLMeasure");
    ASSERT_EQ(1u,ProblemRecord::getProblemRecords(database).size());
    ASSERT_EQ(1u,MeasureGroupRecord::getMeasureGroupRecords(database).size());
    EXPECT_EQ(1u,RubyMeasureRecord::getRubyMeasureRecords(database).size());

    MeasureRecordVector dprs = MeasureGroupRecord::getMeasureGroupRecords(database)[0].measureRecords(false);
    ASSERT_EQ(1u,dprs.size());
    ASSERT_TRUE(dprs[0].optionalCast<RubyMeasureRecord>());
    RubyMeasureRecord rpr = dprs[0].cast<RubyMeasureRecord>();
    RubyMeasure rp = rpr.rubyMeasure();
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
    ProjectDatabase database = getExistingDatabase("RubyMeasureRecord_BCLMeasure");
    ASSERT_EQ(1u,ProblemRecord::getProblemRecords(database).size());
    EXPECT_EQ(1u,MeasureGroupRecord::getMeasureGroupRecords(database).size());
    EXPECT_EQ(1u,RubyMeasureRecord::getRubyMeasureRecords(database).size());
    EXPECT_EQ(1u,FileReferenceRecord::getFileReferenceRecords(database).size());
    EXPECT_EQ(2u,OSArgumentRecord::getOSArgumentRecords(database).size());

    Problem problemCopy = ProblemRecord::getProblemRecords(database)[0].problem();
    InputVariableVector vars = problemCopy.variables();
    ASSERT_FALSE(vars.empty());
    ASSERT_TRUE(vars[0].optionalCast<MeasureGroup>());
    MeasureVector dps = vars[0].cast<MeasureGroup>().measures(false);
    ASSERT_FALSE(dps.empty());
    ASSERT_TRUE(dps[0].optionalCast<RubyMeasure>());
    RubyMeasure rp = dps[0].cast<RubyMeasure>();
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
    ProjectDatabase database = getExistingDatabase("RubyMeasureRecord_BCLMeasure");
    ASSERT_EQ(1u,ProblemRecord::getProblemRecords(database).size());
    EXPECT_EQ(1u,MeasureGroupRecord::getMeasureGroupRecords(database).size());
    EXPECT_EQ(1u,RubyMeasureRecord::getRubyMeasureRecords(database).size());
    EXPECT_EQ(1u,FileReferenceRecord::getFileReferenceRecords(database).size());
    EXPECT_EQ(0u,OSArgumentRecord::getOSArgumentRecords(database).size());

    Problem problemCopy = ProblemRecord::getProblemRecords(database)[0].problem();
    InputVariableVector vars = problemCopy.variables();
    ASSERT_FALSE(vars.empty());
    ASSERT_TRUE(vars[0].optionalCast<MeasureGroup>());
    MeasureVector dps = vars[0].cast<MeasureGroup>().measures(false);
    ASSERT_FALSE(dps.empty());
    ASSERT_TRUE(dps[0].optionalCast<RubyMeasure>());
    RubyMeasure rp = dps[0].cast<RubyMeasure>();
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

