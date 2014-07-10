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
  analysis = AnalysisRecord::getAnalysisRecords(database)[0].analysis();
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

TEST_F(AnalysisDriverFixture,RuntimeBehavior_ClearAndRerunDakotaAnalysis) {
}
