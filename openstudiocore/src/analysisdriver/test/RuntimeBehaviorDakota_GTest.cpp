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
