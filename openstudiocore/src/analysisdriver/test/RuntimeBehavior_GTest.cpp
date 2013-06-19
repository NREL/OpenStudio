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
#include <analysisdriver/test/AnalysisDriverFixture.hpp>

#include <analysisdriver/AnalysisDriver.hpp>
#include <analysisdriver/CurrentAnalysis.hpp>
#include <analysisdriver/AnalysisRunOptions.hpp>
#include <analysisdriver/AnalysisDriverWatcher.hpp>

#include <project/ProjectDatabase.hpp>
#include <project/AnalysisRecord.hpp>

#include <analysis/Analysis.hpp>
#include <analysis/Problem.hpp>
#include <analysis/DataPoint.hpp>
#include <analysis/DesignOfExperiments.hpp>
#include <analysis/DesignOfExperiments_Impl.hpp>
#include <analysis/DesignOfExperimentsOptions.hpp>
#include <analysis/DesignOfExperimentsOptions_Impl.hpp>
#include <analysis/SamplingAlgorithmOptions.hpp>
#include <analysis/SamplingAlgorithmOptions_Impl.hpp>
#include <analysis/SamplingAlgorithm.hpp>
#include <analysis/ContinuousVariable.hpp>
#include <analysis/ContinuousVariable_Impl.hpp>

#include <runmanager/Test/ToolBin.hxx>
#include <runmanager/lib/JobErrors.hpp>
#include <runmanager/lib/RunManager.hpp>
#include <runmanager/lib/Job.hpp>

#include <model/Model.hpp>

#include <utilities/core/FileReference.hpp>

#include <boost/foreach.hpp>
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

class StopWatcher : public AnalysisDriverWatcher {
 public:
  StopWatcher(const AnalysisDriver& analysisDriver, int stopNum=2)
    : AnalysisDriverWatcher(analysisDriver),
      m_stopNum(stopNum),
      m_nComplete(0),
      m_stoppingTime(0,0,0,0)
  {}

  virtual ~StopWatcher() {}

  virtual void onDataPointComplete(const UUID &dataPoint) {
    ++m_nComplete;
    if (m_nComplete == m_stopNum) {
      AnalysisDriver driver = analysisDriver();
      CurrentAnalysisVector currentAnalyses = driver.currentAnalyses();
      ASSERT_FALSE(currentAnalyses.empty());
      EXPECT_EQ(1u,currentAnalyses.size());
      CurrentAnalysis currentAnalysis = currentAnalyses[0];
      openstudio::Time start = openstudio::Time::currentTime();
      driver.stop(currentAnalysis);
      m_stoppingTime = openstudio::Time::currentTime() - start;
    }
  }

  int stopNum() const {
    return m_stopNum;
  }

  int nComplete() const {
    return m_nComplete;
  }

  openstudio::Time stoppingTime() const {
    return m_stoppingTime;
  }

 private:
  int m_stopNum;
  int m_nComplete;
  openstudio::Time m_stoppingTime;
};

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
  BOOST_FOREACH(const Job& job,runManager.getJobs()) {
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

TEST_F(AnalysisDriverFixture,RuntimeBehavior_StopDakotaAnalysis) {
  // Tests for stopping time < 20s.

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("SimpleHistogramBinUQ",true,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  SamplingAlgorithmOptions algOptions;
  algOptions.setSamples(100);
  Analysis analysis("Stop Dakota Analysis",
                    problem,
                    SamplingAlgorithm(algOptions),
                    seedModel);

  // RUN ANALYSIS
  if (!dakotaExePath().empty()) {
    ProjectDatabase database = getCleanDatabase("StopDakotaAnalysis");
    AnalysisDriver analysisDriver(database);
    AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
    StopWatcher watcher(analysisDriver);
    watcher.watch(analysis.uuid());
    CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
    analysisDriver.waitForFinished();
    EXPECT_FALSE(analysisDriver.isRunning());
    EXPECT_GE(watcher.nComplete(),watcher.stopNum());
    EXPECT_LE(watcher.stoppingTime(),openstudio::Time(0,0,0,20));

    // check conditions afterward
    boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
    ASSERT_TRUE(jobErrors);
    EXPECT_FALSE(jobErrors->errors().empty());
    EXPECT_FALSE(currentAnalysis.analysis().dataPoints().empty());
    EXPECT_FALSE(currentAnalysis.analysis().algorithm()->isComplete());
    EXPECT_FALSE(currentAnalysis.analysis().algorithm()->failed());
    EXPECT_EQ(0u,analysisDriver.currentAnalyses().size());
  }
}

TEST_F(AnalysisDriverFixture,RuntimeBehavior_StopAndRestartCustomAnalysis) {
}

TEST_F(AnalysisDriverFixture,RuntimeBehavior_StopAndRestartOpenStudioAnalysis) {
}

TEST_F(AnalysisDriverFixture,RuntimeBehavior_StopAndRestartDakotaAnalysis) {

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("SimpleHistogramBinUQ",true,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  SamplingAlgorithmOptions algOptions;
  algOptions.setSamples(10);
  Analysis analysis("Stop and Restart Dakota Analysis",
                    problem,
                    SamplingAlgorithm(algOptions),
                    seedModel);

  // RUN ANALYSIS
  if (!dakotaExePath().empty()) {
    ProjectDatabase database = getCleanDatabase("StopAndRestartDakotaAnalysis");
    AnalysisDriver analysisDriver(database);
    AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
    StopWatcher watcher(analysisDriver);
    watcher.watch(analysis.uuid());
    CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
    analysisDriver.waitForFinished();
    EXPECT_FALSE(analysisDriver.isRunning());

    // check conditions afterward
    boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
    ASSERT_TRUE(jobErrors);
    EXPECT_FALSE(jobErrors->errors().empty());
    EXPECT_FALSE(currentAnalysis.analysis().dataPoints().empty());
    EXPECT_FALSE(currentAnalysis.analysis().dataPointsToQueue().empty());
    EXPECT_FALSE(currentAnalysis.analysis().completeDataPoints().empty());
    EXPECT_FALSE(currentAnalysis.analysis().successfulDataPoints().empty());
    EXPECT_TRUE(currentAnalysis.analysis().failedDataPoints().empty());
    EXPECT_FALSE(currentAnalysis.analysis().algorithm()->isComplete());
    EXPECT_FALSE(currentAnalysis.analysis().algorithm()->failed());
    EXPECT_EQ(0u,analysisDriver.currentAnalyses().size());
    LOG(Debug,"After initial stop, there are " << currentAnalysis.analysis().dataPoints().size()
        << " data points, of which " << currentAnalysis.analysis().completeDataPoints().size()
        << " are complete.");

    // try to restart from database contents
    Analysis analysis = AnalysisRecord::getAnalysisRecords(database)[0].analysis();
    ASSERT_TRUE(analysis.algorithm());
    EXPECT_FALSE(analysis.algorithm()->isComplete());
    EXPECT_FALSE(analysis.algorithm()->failed());
    currentAnalysis = analysisDriver.run(analysis,runOptions);
    analysisDriver.waitForFinished();
    EXPECT_EQ(10u,analysis.dataPoints().size());
    EXPECT_EQ(0u,analysis.dataPointsToQueue().size());
    EXPECT_EQ(10u,analysis.completeDataPoints().size());
    EXPECT_EQ(10u,analysis.successfulDataPoints().size());
    EXPECT_EQ(0u,analysis.failedDataPoints().size());
  }
}

TEST_F(AnalysisDriverFixture,RuntimeBehavior_ClearAndRerunCustomAnalysis) {
}

TEST_F(AnalysisDriverFixture,RuntimeBehavior_ClearAndRerunOpenStudioAnalysis) {
}

TEST_F(AnalysisDriverFixture,RuntimeBehavior_ClearAndRerunDakotaAnalysis) {
}
