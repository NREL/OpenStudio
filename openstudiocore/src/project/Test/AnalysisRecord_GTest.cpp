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

#include "../AnalysisRecord.hpp"
#include "ProjectFixture.hpp"

#include "../ProblemRecord.hpp"
#include "../MeasureGroupRecord.hpp"
#include "../MeasureGroupRecord_Impl.hpp"
#include "../NullMeasureRecord.hpp"
#include "../NullMeasureRecord_Impl.hpp"
#include "../RubyMeasureRecord.hpp"
#include "../RubyMeasureRecord_Impl.hpp"
#include "../LinearFunctionRecord.hpp"
#include "../LinearFunctionRecord_Impl.hpp"
#include "../DataPointRecord.hpp"
#include "../DataPointRecord_Impl.hpp"

#include "../../analysis/Analysis.hpp"
#include "../../analysis/Problem.hpp"
#include "../../analysis/Problem_Impl.hpp"
#include "../../analysis/DataPoint.hpp"
#include "../../analysis/MeasureGroup.hpp"
#include "../../analysis/MeasureGroup_Impl.hpp"
#include "../../analysis/NullMeasure.hpp"
#include "../../analysis/RubyMeasure.hpp"
#include "../../analysis/RubyContinuousVariable.hpp"
#include "../../analysis/LinearFunction.hpp"
#include "../../analysis/OutputAttributeVariable.hpp"

#include "../../runmanager/lib/Workflow.hpp"

#include "../../ruleset/OSArgument.hpp"

#include "../../utilities/core/FileReference.hpp"

using namespace openstudio;
using namespace openstudio::analysis;
using namespace openstudio::project;

TEST_F(ProjectFixture,AnalysisRecord_AddAndRemoveDataPoints) {
  // create an analysis with data points, no results
  Analysis analysis("My Analysis",
                    Problem("My Problem",VariableVector(),runmanager::Workflow()),
                    FileReferenceType::OSM);
  Problem problem = analysis.problem();

  MeasureVector measures;
  int pi = 1;
  std::stringstream ss;
  for (int i = 0; i < 3; ++i) {
    measures.push_back(NullMeasure());
    for (int j = 0; j < 4; ++j) {
      ss << "measure" << pi << ".rb";
      measures.push_back(RubyMeasure(toPath(ss.str()),
                                     FileReferenceType::OSM,
                                     FileReferenceType::OSM,true));
      ss.str("");
      ++pi;
    }
    ss << "Variable " << i+1;
    problem.push(MeasureGroup(ss.str(),measures));
    measures.clear();
    ss.str("");
  }

  EXPECT_EQ(3u,analysis.problem().variables().size());
  ASSERT_FALSE(problem.variables().empty());
  EXPECT_EQ(5u,problem.variables()[0].cast<MeasureGroup>().numMeasures(true));

  problem.pushResponse(
        LinearFunction("Energy Use",
                       VariableVector(1u,OutputAttributeVariable("Energy Use","Total.Energy.Use"))));
  EXPECT_EQ(1u,problem.responses().size());

  std::vector<QVariant> values(3u,0);
  OptionalDataPoint dataPoint = problem.createDataPoint(values);
  ASSERT_TRUE(dataPoint);
  bool test = analysis.addDataPoint(*dataPoint);
  EXPECT_TRUE(test);
  values[1] = 3;
  dataPoint = problem.createDataPoint(values);
  ASSERT_TRUE(dataPoint);
  test = analysis.addDataPoint(*dataPoint);
  EXPECT_TRUE(test);
  values[0] = 1;
  values[1] = 2;
  values[2] = 4;
  dataPoint = problem.createDataPoint(values);
  ASSERT_TRUE(dataPoint);
  test = analysis.addDataPoint(*dataPoint);
  EXPECT_TRUE(test);

  EXPECT_TRUE(analysis.isDirty());
  EXPECT_TRUE(problem.isDirty());
  for (const Variable& variable : problem.variables()) {
    EXPECT_TRUE(variable.isDirty());
    ASSERT_TRUE(variable.optionalCast<MeasureGroup>());
    for (const Measure& measure : variable.cast<MeasureGroup>().measures(false)) {
      EXPECT_TRUE(measure.isDirty());
    }
  }
  for (const Function& response : problem.responses()) {
    EXPECT_TRUE(response.isDirty());
  }
  for (const DataPoint& dataPoint : analysis.dataPoints()) {
    EXPECT_TRUE(dataPoint.isDirty());
  }

  // save to database, make sure everything is there, make sure clean signal filters down
  ProjectDatabase database = getCleanDatabase("AnalysisRecord_AddAndRemoveDataPoints");
  {
    bool transactionStarted = database.startTransaction();
    EXPECT_TRUE(transactionStarted);
    AnalysisRecord analysisRecord(analysis,database);
    database.save();
    test = database.commitTransaction();
    EXPECT_TRUE(test);
    EXPECT_EQ(analysis.uuid(),analysisRecord.handle());
    EXPECT_EQ(analysis.versionUUID(),analysisRecord.uuidLast());
    ASSERT_NO_THROW(analysisRecord.problemRecord());
    ProblemRecord problemRecord = analysisRecord.problemRecord();
    InputVariableRecordVector variableRecords = problemRecord.inputVariableRecords();
    EXPECT_EQ(3u,variableRecords.size());
    ASSERT_FALSE(variableRecords.empty());
    ASSERT_TRUE(variableRecords[0].optionalCast<MeasureGroupRecord>());
    MeasureRecordVector measureRecords = variableRecords[0].cast<MeasureGroupRecord>().measureRecords(false);
    EXPECT_EQ(5u,measureRecords.size());
    ASSERT_TRUE(measureRecords.size() > 1);
    EXPECT_TRUE(measureRecords[0].optionalCast<NullMeasureRecord>());
    EXPECT_TRUE(measureRecords[1].optionalCast<RubyMeasureRecord>());
    EXPECT_EQ(3u,analysisRecord.dataPointRecords().size());
    EXPECT_TRUE(analysisRecord.completeDataPointRecords().empty());
  }

  analysis.clearDirtyFlag();
  EXPECT_FALSE(analysis.isDirty());
  EXPECT_FALSE(problem.isDirty());
  for (const Variable& variable : problem.variables()) {
    EXPECT_FALSE(variable.isDirty());
    ASSERT_TRUE(variable.optionalCast<MeasureGroup>());
    for (const Measure& measure : variable.cast<MeasureGroup>().measures(false)) {
      EXPECT_FALSE(measure.isDirty());
    }
  }
  for (const Function& response : problem.responses()) {
    EXPECT_FALSE(response.isDirty());
  }
  for (const DataPoint& dataPoint : analysis.dataPoints()) {
    EXPECT_FALSE(dataPoint.isDirty());
  }

  // make some more data points and make sure dirty flag bubbles up
  values[0] = 3;
  values[1] = 2;
  values[2] = 2;
  dataPoint = problem.createDataPoint(values);
  ASSERT_TRUE(dataPoint);
  test = analysis.addDataPoint(*dataPoint);
  EXPECT_TRUE(test);
  values[0] = 0;
  values[1] = 4;
  values[2] = 0;
  dataPoint = problem.createDataPoint(values);
  ASSERT_TRUE(dataPoint);
  test = analysis.addDataPoint(*dataPoint);
  EXPECT_TRUE(test);
  EXPECT_EQ(5u,analysis.dataPoints().size());
  EXPECT_TRUE(analysis.isDirty());
  EXPECT_FALSE(problem.isDirty());
  for (const Variable& variable : problem.variables()) {
    EXPECT_FALSE(variable.isDirty());
    ASSERT_TRUE(variable.optionalCast<MeasureGroup>());
    for (const Measure& measure : variable.cast<MeasureGroup>().measures(false)) {
      EXPECT_FALSE(measure.isDirty());
    }
  }
  for (const Function& response : problem.responses()) {
    EXPECT_FALSE(response.isDirty());
  }
  int i = 0;
  for (const DataPoint& dataPoint : analysis.dataPoints()) {
    if (i < 3) {
      EXPECT_FALSE(dataPoint.isDirty());
    }
    else {
      EXPECT_TRUE(dataPoint.isDirty());
    }
    ++i;
  }

  // save to database, make sure changes made it in
  {
    bool transactionStarted = database.startTransaction();
    EXPECT_TRUE(transactionStarted);
    database.unloadUnusedCleanRecords(); // ETH@20130201 - Having to call this is awkward.
                                         // Where is the best place?
    AnalysisRecord analysisRecord(analysis,database);
    database.save();
    test = database.commitTransaction();
    EXPECT_TRUE(test);
    EXPECT_EQ(analysis.uuid(),analysisRecord.handle());
    EXPECT_EQ(analysis.versionUUID(),analysisRecord.uuidLast());
    ASSERT_NO_THROW(analysisRecord.problemRecord());
    ProblemRecord problemRecord = analysisRecord.problemRecord();
    EXPECT_EQ(problemRecord.handle(),analysisRecord.problemRecord().handle());
    EXPECT_EQ(problemRecord.uuidLast(),analysisRecord.problemRecord().uuidLast());
    InputVariableRecordVector variableRecords = problemRecord.inputVariableRecords();
    EXPECT_EQ(3u,variableRecords.size());
    ASSERT_FALSE(variableRecords.empty());
    ASSERT_TRUE(variableRecords[0].optionalCast<MeasureGroupRecord>());
    MeasureRecordVector measureRecords = variableRecords[0].cast<MeasureGroupRecord>().measureRecords(false);
    EXPECT_EQ(5u,measureRecords.size());
    ASSERT_TRUE(measureRecords.size() > 1);
    EXPECT_TRUE(measureRecords[0].optionalCast<NullMeasureRecord>());
    EXPECT_TRUE(measureRecords[1].optionalCast<RubyMeasureRecord>());
    EXPECT_EQ(5u,analysisRecord.dataPointRecords().size());
    EXPECT_TRUE(analysisRecord.completeDataPointRecords().empty());
  }

  analysis.clearDirtyFlag();
  EXPECT_FALSE(analysis.isDirty());
  EXPECT_FALSE(problem.isDirty());
  for (const Variable& variable : problem.variables()) {
    EXPECT_FALSE(variable.isDirty());
    ASSERT_TRUE(variable.optionalCast<MeasureGroup>());
    for (const Measure& measure : variable.cast<MeasureGroup>().measures(false)) {
      EXPECT_FALSE(measure.isDirty());
    }
  }
  for (const Function& response : problem.responses()) {
    EXPECT_FALSE(response.isDirty());
  }
  for (const DataPoint& dataPoint : analysis.dataPoints()) {
    EXPECT_FALSE(dataPoint.isDirty());
  }

  // remove a data point and save database. make sure point actually disappears
  DataPoint toRemove = analysis.dataPoints()[3];
  test = analysis.removeDataPoint(toRemove);
  EXPECT_TRUE(test);
  EXPECT_TRUE(analysis.isDirty());

  {
    bool transactionStarted = database.startTransaction();
    EXPECT_TRUE(transactionStarted);
    database.unloadUnusedCleanRecords(); // ETH@20130201 - Having to call this is awkward.
    // Where is the best place?
    AnalysisRecord analysisRecord(analysis,database);
    database.save();
    test = database.commitTransaction();
    EXPECT_TRUE(test);
    EXPECT_EQ(analysis.uuid(),analysisRecord.handle());
    EXPECT_EQ(analysis.versionUUID(),analysisRecord.uuidLast());
    ASSERT_NO_THROW(analysisRecord.problemRecord());
    ProblemRecord problemRecord = analysisRecord.problemRecord();
    EXPECT_EQ(problemRecord.handle(),analysisRecord.problemRecord().handle());
    EXPECT_EQ(problemRecord.uuidLast(),analysisRecord.problemRecord().uuidLast());
    InputVariableRecordVector variableRecords = problemRecord.inputVariableRecords();
    EXPECT_EQ(3u,variableRecords.size());
    ASSERT_FALSE(variableRecords.empty());
    ASSERT_TRUE(variableRecords[0].optionalCast<MeasureGroupRecord>());
    MeasureRecordVector measureRecords = variableRecords[0].cast<MeasureGroupRecord>().measureRecords(false);
    EXPECT_EQ(5u,measureRecords.size());
    ASSERT_TRUE(measureRecords.size() > 1);
    EXPECT_TRUE(measureRecords[0].optionalCast<NullMeasureRecord>());
    EXPECT_TRUE(measureRecords[1].optionalCast<RubyMeasureRecord>());
    EXPECT_EQ(4u,analysisRecord.dataPointRecords().size());
    OptionalDataPointRecord searchResult = database.getObjectRecordByHandle<DataPointRecord>(toRemove.uuid());
    EXPECT_FALSE(searchResult);
    EXPECT_EQ(4u,DataPointRecord::getDataPointRecords(database).size());
  }

  EXPECT_EQ(4u,analysis.dataPoints().size());
  test = analysis.clearDirtyFlag();
  EXPECT_TRUE(test);
}

TEST_F(ProjectFixture,AnalysisRecord_SetProblem) {
  // create an analysis with data points
  Problem problem1("Minimal Problem",VariableVector(),runmanager::Workflow());
  Analysis analysis("Analysis",
                    problem1,
                    FileReferenceType::OSM);
  OptionalDataPoint dataPoint = problem1.createDataPoint(std::vector<QVariant>());
  ASSERT_TRUE(dataPoint);
  bool test = analysis.addDataPoint(*dataPoint);
  EXPECT_TRUE(test);

  // save to database, make sure everything is there, make sure clean signal filters down
  ProjectDatabase database = getCleanDatabase("AnalysisRecord_SetProblem");
  {
    bool transactionStarted = database.startTransaction();
    EXPECT_TRUE(transactionStarted);
    AnalysisRecord analysisRecord(analysis,database);
    database.save();
    test = database.commitTransaction();
    EXPECT_TRUE(test);
    ASSERT_NO_THROW(analysisRecord.problemRecord());
    ProblemRecord problemRecord = analysisRecord.problemRecord();
    EXPECT_EQ(problem1.uuid(),problemRecord.handle());
    EXPECT_EQ(problem1.versionUUID(),problemRecord.uuidLast());
    EXPECT_EQ(1u,analysisRecord.dataPointRecords().size());
  }
  analysis.clearDirtyFlag();
  EXPECT_FALSE(analysis.isDirty());
  EXPECT_FALSE(problem1.isDirty());
  for (const DataPoint& dataPoint : analysis.dataPoints()) {
    EXPECT_FALSE(dataPoint.isDirty());
  }
  EXPECT_FALSE(analysis.dataPointsAreInvalid());

  // set analysis to have new problem
  // make sure dirty flag bubbles up, data points marked invalid
  Problem problem2 = problem1.clone().cast<Problem>();
  problem2.setName("Real Problem");
  EXPECT_NE(problem1.uuid(),problem2.uuid());
  analysis.setProblem(problem2);
  EXPECT_TRUE(analysis.isDirty());
  EXPECT_FALSE(problem1.isDirty());
  EXPECT_TRUE(problem2.isDirty());
  EXPECT_TRUE(analysis.dataPointsAreInvalid());
  RubyMeasure userScript(toPath("measure.rb"),
                              FileReferenceType::IDF,
                              FileReferenceType::IDF,
                              true);
  RubyContinuousVariable cvar("Script Argument Variable",
                              ruleset::OSArgument::makeDoubleArgument("COP"),
                              userScript);
  test = problem2.push(cvar);
  EXPECT_FALSE(test); // IDF not compatible with seed
  EXPECT_TRUE(problem2.variables().empty());
  test = userScript.setPerturbationScript(toPath("measure.rb"),
                                          FileReferenceType::OSM,
                                          FileReferenceType::OSM,
                                          true);
  EXPECT_TRUE(test);
  test = problem2.push(cvar);
  EXPECT_TRUE(test);
  EXPECT_EQ(1u,analysis.problem().variables().size());
  EXPECT_EQ(1u,analysis.dataPoints().size());
  dataPoint = problem2.createDataPoint(std::vector<QVariant>(1u,3.5));
  ASSERT_TRUE(dataPoint);
  test = analysis.addDataPoint(*dataPoint);
  EXPECT_FALSE(test);

  // save to database, make sure dataPointsAreInvalid flag was saved, old problem
  // should still be there, new problem there, and clean signal won't go through
  Analysis analysisCopy = analysis; // will replace with deserialized version momentarily
  {
    bool transactionStarted = database.startTransaction();
    EXPECT_TRUE(transactionStarted);
    database.unloadUnusedCleanRecords(); // ETH@20130201 - Having to call this is awkward.
                                         // (As are the brackets) Where is the best place?
    AnalysisRecord analysisRecord(analysis,database);
    database.save();
    test = database.commitTransaction();
    EXPECT_TRUE(test);
    ASSERT_NO_THROW(analysisRecord.problemRecord());
    ProblemRecord problemRecord = analysisRecord.problemRecord();
    EXPECT_EQ(problem2.uuid(),problemRecord.handle());
    EXPECT_EQ(problem2.versionUUID(),problemRecord.uuidLast());
    EXPECT_EQ(2u,ProblemRecord::getProblemRecords(database).size()); // old one still around
    EXPECT_EQ(1u,analysisRecord.dataPointRecords().size());
    test = analysis.clearDirtyFlag();
    EXPECT_FALSE(test);
    Analysis analysisCopy = analysisRecord.analysis(); // should work b/c both problems around
  }
  EXPECT_EQ(analysis.uuid(),analysisCopy.uuid());
  EXPECT_EQ(analysis.versionUUID(),analysisCopy.versionUUID());
  EXPECT_TRUE(analysisCopy.isDirty());
  EXPECT_TRUE(analysisCopy.dataPointsAreInvalid());
  ASSERT_FALSE(analysisCopy.dataPoints().empty());
  EXPECT_NE(analysisCopy.problem().uuid(),analysisCopy.dataPoints()[0].problem().uuid());
  test = analysisCopy.clearDirtyFlag();
  EXPECT_FALSE(test);

  // remove data points, save again, make sure dataPointsAreInvalid flag a-okay, new problem
  // still there and ok, old problem is going to hang around because didn't get rid of it when
  // analysisRecord still pointing to it, clean signal filters all the way down
  analysisCopy.removeAllDataPoints();
  {
    bool transactionStarted = database.startTransaction();
    EXPECT_TRUE(transactionStarted);
    database.unloadUnusedCleanRecords(); // ETH@20130201 - Having to call this is awkward.
                                         // Where is the best place?
    AnalysisRecord analysisRecord(analysisCopy,database);
    database.save();
    test = database.commitTransaction();
    EXPECT_TRUE(test);
    ASSERT_NO_THROW(analysisRecord.problemRecord());
    ProblemRecord problemRecord = analysisRecord.problemRecord();
    EXPECT_EQ(problem2.uuid(),problemRecord.handle());
    EXPECT_EQ(problem2.versionUUID(),problemRecord.uuidLast());
    // old problem record still hanging around.
    // TODO: Have use count as child, resource, and be able to (selectively) purge.
    EXPECT_EQ(2u,ProblemRecord::getProblemRecords(database).size());
    EXPECT_TRUE(analysisRecord.dataPointRecords().empty());
    analysis = analysisRecord.analysis();
  }
  test = analysis.clearDirtyFlag();
  EXPECT_TRUE(test);
  EXPECT_FALSE(analysis.isDirty());
  EXPECT_FALSE(analysis.dataPointsAreInvalid());
  EXPECT_FALSE(analysis.resultsAreInvalid());
  EXPECT_TRUE(analysis.dataPoints().empty());
}

TEST_F(ProjectFixture,AnalysisRecord_AddAndRemoveVariables) {
  // create an analysis with no variables and one data point

  // save to database, make sure everything is there, mark analysis clean and make sure
  // signal filters down

  // add variables to problem

  // make sure dirty flag bubbles up, data points marked invalid, can't add data points

  // remove data points, save, check analysis flags, check problem record, mark clean and check

  // remove a variable, check that invalidDataPoints flag is not flipped (b/c no data points),
  // then add new data points

  // save to database and make sure changes registered

}
