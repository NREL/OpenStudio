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
  Model model = model::exampleModel();
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

