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
#include "StopWatcher.hpp"

#include "../AnalysisDriver.hpp"
#include "../CurrentAnalysis.hpp"
#include "../AnalysisRunOptions.hpp"

#include "../../project/ProjectDatabase.hpp"
#include "../../project/AnalysisRecord.hpp"

#include "../../analysis/Analysis.hpp"
#include "../../analysis/Problem.hpp"
#include "../../analysis/DataPoint.hpp"
#include "../../analysis/DesignOfExperiments.hpp"
#include "../../analysis/DesignOfExperiments_Impl.hpp"
#include "../../analysis/DesignOfExperimentsOptions.hpp"
#include "../../analysis/DesignOfExperimentsOptions_Impl.hpp"
#include "../../analysis/SamplingAlgorithmOptions.hpp"
#include "../../analysis/SamplingAlgorithmOptions_Impl.hpp"
#include "../../analysis/SamplingAlgorithm.hpp"
#include "../../analysis/ContinuousVariable.hpp"
#include "../../analysis/ContinuousVariable_Impl.hpp"

#include <runmanager/Test/ToolBin.hxx>
#include "../../runmanager/lib/JobErrors.hpp"
#include "../../runmanager/lib/RunManager.hpp"
#include "../../runmanager/lib/Job.hpp"

#include "../../model/Model.hpp"

#include "../../utilities/core/FileReference.hpp"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

#include <OpenStudio.hxx>

#include <resources.hxx>

using namespace openstudio;
using namespace openstudio::model;
using namespace openstudio::runmanager;
using namespace openstudio::project;
using namespace openstudio::analysis;
using namespace openstudio::analysisdriver;

TEST_F(AnalysisDriverFixture,RuntimeBehavior_StopCustomAnalysis) {
  // Tests for stopping time < 20s.

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("UserScriptContinuous",true,false);

  // DEFINE SEED
  Model model = fastExampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  Analysis analysis("Stop Custom Analysis",
                    problem,
                    seedModel);
  // generate 100 random points
  boost::mt19937 mt;
  typedef boost::uniform_real<> dist_type;
  typedef boost::variate_generator<boost::mt19937&, dist_type > gen_type;
  InputVariableVector variables = problem.variables();
  ContinuousVariable cvar = variables[0].cast<ContinuousVariable>();
  gen_type generator0(mt,dist_type(cvar.minimum().get(),cvar.maximum().get()));
  cvar = variables[1].cast<ContinuousVariable>();
  gen_type generator1(mt,dist_type(cvar.minimum().get(),cvar.maximum().get()));
  cvar = variables[2].cast<ContinuousVariable>();
  gen_type generator2(mt,dist_type(cvar.minimum().get(),cvar.maximum().get()));

  for (int i = 0, n = 100; i < n; ++i) {
    std::vector<QVariant> values;
    double value = generator0();
    values.push_back(value);
    value = generator1();
    values.push_back(value);
    value = generator2();
    values.push_back(value);
    OptionalDataPoint dataPoint = problem.createDataPoint(values);
    ASSERT_TRUE(dataPoint);
    ASSERT_TRUE(analysis.addDataPoint(*dataPoint));
  }

  // RUN ANALYSIS
  ProjectDatabase database = getCleanDatabase("StopCustomAnalysis");
  AnalysisDriver analysisDriver(database);
  AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
  runOptions.setQueueSize(2);
  StopWatcher watcher(analysisDriver);
  watcher.watch(analysis.uuid());
  CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
  EXPECT_EQ(2,currentAnalysis.numQueuedJobs());
  EXPECT_EQ(0,currentAnalysis.numQueuedDakotaJobs());
  EXPECT_EQ(100,currentAnalysis.totalNumJobsInOSIteration());
  EXPECT_EQ(0,currentAnalysis.numCompletedJobsInOSIteration());
  analysisDriver.waitForFinished();
  EXPECT_FALSE(analysisDriver.isRunning());
  EXPECT_GE(watcher.nComplete(),watcher.stopNum());
  EXPECT_LE(watcher.stoppingTime(),openstudio::Time(0,0,0,20));

  // check conditions afterward
  RunManager runManager = analysisDriver.database().runManager();
  EXPECT_FALSE(runManager.workPending());
  for (const Job& job : runManager.getJobs()) {
    EXPECT_FALSE(job.running());
    EXPECT_FALSE(job.treeRunning());
  }
  EXPECT_TRUE(currentAnalysis.numCompletedJobsInOSIteration() > 0);
  EXPECT_TRUE(currentAnalysis.analysis().dataPointsToQueue().size() > 0u);
  EXPECT_TRUE(currentAnalysis.analysis().dataPointsToQueue().size() < 100u);
  EXPECT_EQ(0u,analysisDriver.currentAnalyses().size());
}

TEST_F(AnalysisDriverFixture,RuntimeBehavior_StopOpenStudioAnalysis) {
  // Tests for stopping time < 20s.

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("IdfOnly",false,false);

  // DEFINE SEED
  FileReference seedModel(resourcesPath() / openstudio::toPath("energyplus/5ZoneAirCooled/in.idf"));

  // CREATE ANALYSIS
  DesignOfExperimentsOptions algOptions(DesignOfExperimentsType::FullFactorial);
  Analysis analysis("Stop OpenStudio Analysis",
                    problem,
                    DesignOfExperiments(algOptions),
                    seedModel);

  // RUN ANALYSIS
  ProjectDatabase database = getCleanDatabase("StopOpenStudioAnalysis");
  AnalysisDriver analysisDriver(database);
  AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
  runOptions.setQueueSize(2);
  StopWatcher watcher(analysisDriver);
  watcher.watch(analysis.uuid());
  CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
  EXPECT_EQ(2,currentAnalysis.numQueuedJobs());
  EXPECT_EQ(0,currentAnalysis.numQueuedDakotaJobs());
  EXPECT_GE(currentAnalysis.totalNumJobsInOSIteration(),10);
  EXPECT_EQ(0,currentAnalysis.numCompletedJobsInOSIteration());
  analysisDriver.waitForFinished();
  EXPECT_FALSE(analysisDriver.isRunning());
  EXPECT_GE(watcher.nComplete(),watcher.stopNum());
  EXPECT_LE(watcher.stoppingTime(),openstudio::Time(0,0,0,20));

  // check conditions afterward
  EXPECT_TRUE(currentAnalysis.numCompletedJobsInOSIteration() > 0);
  EXPECT_TRUE(currentAnalysis.analysis().dataPointsToQueue().size() > 0u);
  EXPECT_TRUE(currentAnalysis.analysis().dataPointsToQueue().size() <
              currentAnalysis.analysis().dataPoints().size());
  EXPECT_EQ(0u,analysisDriver.currentAnalyses().size());
}

