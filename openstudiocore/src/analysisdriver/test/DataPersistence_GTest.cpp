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
#include "AnalysisDriverFixture.hpp"

#include "../SimpleProject.hpp"
#include "../CurrentAnalysis.hpp"
#include "../AnalysisRunOptions.hpp"

#include "../../analysis/Analysis.hpp"
#include "../../analysis/Problem.hpp"
#include "../../analysis/DataPoint.hpp"
#include "../../analysis/DDACEAlgorithm.hpp"
#include "../../analysis/DDACEAlgorithm_Impl.hpp"
#include "../../analysis/DDACEAlgorithmOptions.hpp"

#include "../../model/Model.hpp"

#include "../../utilities/bcl/BCLMeasure.hpp"
#include "../../utilities/core/FileReference.hpp"

using namespace openstudio;
using namespace openstudio::ruleset;
using namespace openstudio::runmanager;
using namespace openstudio::analysis;
using namespace openstudio::project;
using namespace openstudio::analysisdriver;

TEST_F(AnalysisDriverFixture,DataPersistence_DataPointErrors) {
  {
    // Create and populate project
    SimpleProject project = getCleanSimpleProject("DataPersistence_DataPointErrors");
    Analysis analysis = project.analysis();
    Problem problem = retrieveProblem(AnalysisDriverFixtureProblem::BuggyBCLMeasure,
                                      true,
                                      false);
    EXPECT_EQ(5u,problem.workflow().size());
    analysis.setProblem(problem);
    model::Model model = model::exampleModel();
    openstudio::path p = toPath("./example.osm");
    model.save(p,true);
    FileReference seedModel(p);
    project.setSeed(seedModel);
    DataPoint dataPoint = problem.createDataPoint(std::vector<QVariant>(problem.numVariables(),0)).get();
    analysis.addDataPoint(dataPoint);

    // Run analysis
    AnalysisRunOptions runOptions = standardRunOptions(project.projectDir());
    project.analysisDriver().run(analysis,runOptions);
    project.analysisDriver().waitForFinished();

    // Check DataPoint job and error information
    ASSERT_EQ(1u,analysis.dataPoints().size());
    dataPoint = analysis.dataPoints()[0];
    EXPECT_TRUE(dataPoint.isComplete());
    EXPECT_TRUE(dataPoint.failed());
    EXPECT_TRUE(dataPoint.topLevelJob());
    WorkflowStepJobVector jobResults = problem.getJobsByWorkflowStep(dataPoint);
    EXPECT_EQ(problem.workflow().size(),jobResults.size());
    ASSERT_EQ(5u,jobResults.size());

    WorkflowStepJob jobResult = jobResults[0];
    ASSERT_TRUE(jobResult.job);
    EXPECT_TRUE(jobResult.measure);
    Job job = jobResult.job.get();
    ASSERT_TRUE(jobResult.mergedJobIndex);
    EXPECT_EQ(0u,jobResult.mergedJobIndex.get());
    EXPECT_FALSE(job.running());
    EXPECT_FALSE(job.outOfDate());
    EXPECT_FALSE(job.canceled());
    EXPECT_TRUE(job.lastRun());
    JobErrors treeErrors = job.treeErrors(); // get all tree errors now, test later
    JobErrors errors = jobResult.errors().get();
    EXPECT_EQ(OSResultValue(OSResultValue::NA),errors.result);
    EXPECT_TRUE(errors.succeeded());
    EXPECT_TRUE(errors.errors().empty());
    EXPECT_TRUE(errors.warnings().empty());
    EXPECT_FALSE(errors.infos().empty());

    jobResult = jobResults[1];
    ASSERT_TRUE(jobResult.job);
    EXPECT_FALSE(jobResult.measure);
    ASSERT_TRUE(jobResult.step.isWorkItem());
    EXPECT_EQ(JobType(JobType::UserScript),jobResult.step.workItemType());
    job = jobResult.job.get();
    ASSERT_TRUE(jobResult.mergedJobIndex);
    EXPECT_EQ(1u,jobResult.mergedJobIndex.get());
    EXPECT_FALSE(job.running());
    EXPECT_FALSE(job.outOfDate());
    EXPECT_FALSE(job.canceled());
    EXPECT_TRUE(job.lastRun());
    errors = jobResult.errors().get();
    EXPECT_EQ(OSResultValue(OSResultValue::Success),errors.result);
    EXPECT_TRUE(errors.succeeded());
    EXPECT_TRUE(errors.errors().empty());
    EXPECT_FALSE(errors.warnings().empty());
    EXPECT_TRUE(errors.infos().empty());

    jobResult = jobResults[2];
    ASSERT_TRUE(jobResult.job);
    EXPECT_TRUE(jobResult.measure);
    job = jobResult.job.get();
    ASSERT_TRUE(jobResult.mergedJobIndex);
    EXPECT_EQ(2u,jobResult.mergedJobIndex.get());
    EXPECT_FALSE(job.running());
    EXPECT_FALSE(job.outOfDate());
    EXPECT_FALSE(job.canceled());
    EXPECT_TRUE(job.lastRun());
    errors = jobResult.errors().get();
    EXPECT_EQ(OSResultValue(OSResultValue::Fail),errors.result);
    EXPECT_FALSE(errors.succeeded());
    EXPECT_FALSE(errors.errors().empty());
    EXPECT_TRUE(errors.warnings().empty());
    EXPECT_TRUE(errors.infos().empty());

    jobResult = jobResults[3];
    ASSERT_TRUE(jobResult.job);
    EXPECT_FALSE(jobResult.measure);
    ASSERT_TRUE(jobResult.step.isWorkItem());
    EXPECT_EQ(JobType(JobType::UserScript),jobResult.step.workItemType());
    job = jobResult.job.get();
    ASSERT_TRUE(jobResult.mergedJobIndex);
    EXPECT_EQ(3u,jobResult.mergedJobIndex.get());
    EXPECT_FALSE(job.running());
    EXPECT_FALSE(job.outOfDate()); // now all four scripts are in same job
    EXPECT_FALSE(job.canceled());
    EXPECT_TRUE(job.lastRun());    // now all four scripts are in same job
    errors = jobResult.errors().get();
    // this script not actually run, so result in default state
    EXPECT_EQ(OSResultValue(OSResultValue::Fail),errors.result);
    EXPECT_FALSE(errors.succeeded());
    EXPECT_EQ(1u, errors.errors().size());
    EXPECT_TRUE(errors.warnings().empty());
    EXPECT_TRUE(errors.infos().empty());

    jobResult = jobResults[4];
    ASSERT_TRUE(jobResult.job);
    EXPECT_FALSE(jobResult.measure);
    ASSERT_TRUE(jobResult.step.isWorkItem());
    EXPECT_EQ(JobType(JobType::ModelToIdf),jobResult.step.workItemType());
    job = jobResult.job.get();
    EXPECT_FALSE(jobResult.mergedJobIndex);
    EXPECT_TRUE(job.outOfDate()); // never run
    EXPECT_FALSE(job.canceled());
    EXPECT_FALSE(job.lastRun());
    errors = jobResult.errors().get();
    EXPECT_EQ(OSResultValue(OSResultValue::Fail),errors.result);
    EXPECT_FALSE(errors.succeeded());
    EXPECT_TRUE(errors.errors().empty());
    EXPECT_TRUE(errors.warnings().empty());
    EXPECT_TRUE(errors.infos().empty());

    EXPECT_EQ(OSResultValue(OSResultValue::Fail),treeErrors.result);
    EXPECT_FALSE(treeErrors.succeeded());
    EXPECT_FALSE(treeErrors.errors().empty());
    EXPECT_FALSE(treeErrors.warnings().empty());
    EXPECT_FALSE(treeErrors.infos().empty());
  }

  {
    // Re-open project
    SimpleProject project = getSimpleProject("DataPersistence_DataPointErrors");
    Analysis analysis = project.analysis();
    Problem problem = analysis.problem();

    // Verify job and error information still there
    // Check DataPoint job and error information
    ASSERT_EQ(1u,analysis.dataPoints().size());
    DataPoint dataPoint = analysis.dataPoints()[0];
    EXPECT_TRUE(dataPoint.isComplete());
    EXPECT_TRUE(dataPoint.failed());
    EXPECT_TRUE(dataPoint.topLevelJob());
    WorkflowStepJobVector jobResults = problem.getJobsByWorkflowStep(dataPoint);
    EXPECT_EQ(problem.workflow().size(),jobResults.size());
    ASSERT_EQ(5u,jobResults.size());

    WorkflowStepJob jobResult = jobResults[0];
    ASSERT_TRUE(jobResult.job);
    EXPECT_TRUE(jobResult.measure);
    Job job = jobResult.job.get();
    EXPECT_FALSE(job.running());
    EXPECT_FALSE(job.outOfDate());
    EXPECT_FALSE(job.canceled());
    EXPECT_TRUE(job.lastRun());
    JobErrors treeErrors = job.treeErrors(); // get all tree errors now, test later
    JobErrors errors = jobResult.errors().get();
    EXPECT_EQ(OSResultValue(OSResultValue::NA),errors.result);
    EXPECT_TRUE(errors.succeeded());
    EXPECT_TRUE(errors.errors().empty());
    EXPECT_TRUE(errors.warnings().empty());
    EXPECT_FALSE(errors.infos().empty());

    jobResult = jobResults[1];
    ASSERT_TRUE(jobResult.job);
    EXPECT_FALSE(jobResult.measure);
    ASSERT_TRUE(jobResult.step.isWorkItem());
    EXPECT_EQ(JobType(JobType::UserScript),jobResult.step.workItemType());
    job = jobResult.job.get();
    EXPECT_FALSE(job.running());
    EXPECT_FALSE(job.outOfDate());
    EXPECT_FALSE(job.canceled());
    EXPECT_TRUE(job.lastRun());
    errors = jobResult.errors().get();
    EXPECT_EQ(OSResultValue(OSResultValue::Success),errors.result);
    EXPECT_TRUE(errors.succeeded());
    EXPECT_TRUE(errors.errors().empty());
    EXPECT_FALSE(errors.warnings().empty());
    EXPECT_TRUE(errors.infos().empty());

    jobResult = jobResults[2];
    ASSERT_TRUE(jobResult.job);
    EXPECT_TRUE(jobResult.measure);
    job = jobResult.job.get();
    EXPECT_FALSE(job.running());
    EXPECT_FALSE(job.outOfDate());
    EXPECT_FALSE(job.canceled());
    EXPECT_TRUE(job.lastRun());
    errors = jobResult.errors().get();
    EXPECT_EQ(OSResultValue(OSResultValue::Fail),errors.result);
    EXPECT_FALSE(errors.succeeded());
    EXPECT_FALSE(errors.errors().empty());
    EXPECT_TRUE(errors.warnings().empty());
    EXPECT_TRUE(errors.infos().empty());

    jobResult = jobResults[3];
    ASSERT_TRUE(jobResult.job);
    EXPECT_FALSE(jobResult.measure);
    ASSERT_TRUE(jobResult.step.isWorkItem());
    EXPECT_EQ(JobType(JobType::UserScript),jobResult.step.workItemType());
    job = jobResult.job.get();
    EXPECT_FALSE(job.running());
    EXPECT_FALSE(job.outOfDate()); // now all four scripts are in same job
    EXPECT_FALSE(job.canceled());
    EXPECT_TRUE(job.lastRun());    // now all four scripts are in same job
    errors = jobResult.errors().get();
    EXPECT_EQ(OSResultValue(OSResultValue::Fail),errors.result);
    EXPECT_FALSE(errors.succeeded());
    EXPECT_EQ(1u, errors.errors().size());
    EXPECT_TRUE(errors.warnings().empty());
    EXPECT_TRUE(errors.infos().empty());

    jobResult = jobResults[4];
    ASSERT_TRUE(jobResult.job);
    EXPECT_FALSE(jobResult.measure);
    ASSERT_TRUE(jobResult.step.isWorkItem());
    EXPECT_EQ(JobType(JobType::ModelToIdf),jobResult.step.workItemType());
    job = jobResult.job.get();
    EXPECT_TRUE(job.outOfDate()); // never run
    EXPECT_FALSE(job.canceled());
    EXPECT_FALSE(job.lastRun());
    errors = jobResult.errors().get();
    EXPECT_EQ(OSResultValue(OSResultValue::Fail),errors.result);
    EXPECT_FALSE(errors.succeeded());
    EXPECT_TRUE(errors.errors().empty());
    EXPECT_TRUE(errors.warnings().empty());
    EXPECT_TRUE(errors.infos().empty());

    EXPECT_EQ(OSResultValue(OSResultValue::Fail),treeErrors.result);
    EXPECT_FALSE(treeErrors.succeeded());
    EXPECT_FALSE(treeErrors.errors().empty());
    EXPECT_FALSE(treeErrors.warnings().empty());
    EXPECT_FALSE(treeErrors.infos().empty());
  }

}

TEST_F(AnalysisDriverFixture,DataPersistence_DakotaErrors) {

  {
    // Create and populate project
    SimpleProject project = getCleanSimpleProject("DataPersistence_DakotaErrors");
    Analysis analysis = project.analysis();
    Problem problem = retrieveProblem(AnalysisDriverFixtureProblem::BuggyBCLMeasure,
                                      true,
                                      false);
    analysis.setProblem(problem);
    model::Model model = model::exampleModel();
    openstudio::path p = toPath("./example.osm");
    model.save(p,true);
    FileReference seedModel(p);
    project.setSeed(seedModel);
    DDACEAlgorithmOptions algOpts(DDACEAlgorithmType::lhs);
    // do not set samples so Dakota job will have errors
    DDACEAlgorithm alg(algOpts);
    analysis.setAlgorithm(alg);

    // Run analysis
    AnalysisRunOptions runOptions = standardRunOptions(project.projectDir());
    project.analysisDriver().run(analysis,runOptions);
    project.analysisDriver().waitForFinished();

    // Check Dakota job and error information
    ASSERT_TRUE(alg.job());
    Job job = alg.job().get();
    EXPECT_FALSE(job.running());
    EXPECT_FALSE(job.outOfDate());
    EXPECT_FALSE(job.canceled());
    EXPECT_TRUE(job.lastRun());
    JobErrors errors = job.errors();
    EXPECT_EQ(OSResultValue(OSResultValue::Fail),errors.result);
    EXPECT_FALSE(errors.succeeded());
    EXPECT_FALSE(errors.errors().empty());
    EXPECT_TRUE(errors.warnings().empty());
    EXPECT_TRUE(errors.infos().empty());
  }

  {
    // Re-open project
    SimpleProject project = getSimpleProject("DataPersistence_DakotaErrors");
    Analysis analysis = project.analysis();
    DDACEAlgorithm alg = analysis.algorithm()->cast<DDACEAlgorithm>();

    // Verify job and error information still there
    ASSERT_TRUE(alg.job());
    Job job = alg.job().get();
    EXPECT_FALSE(job.running());
    EXPECT_FALSE(job.outOfDate());
    EXPECT_FALSE(job.canceled());
    EXPECT_TRUE(job.lastRun());
    JobErrors errors = job.errors();
    EXPECT_EQ(OSResultValue(OSResultValue::Fail),errors.result);
    EXPECT_FALSE(errors.succeeded());
    EXPECT_FALSE(errors.errors().empty());
    EXPECT_TRUE(errors.warnings().empty());
    EXPECT_TRUE(errors.infos().empty());
  }

}
