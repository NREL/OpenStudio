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
    model::Model model =fastExampleModel();
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
