/**********************************************************************
*  Copyright (c) 2008-2010, Alliance for Sustainable Energy.
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

#include <project/ProjectDatabase.hpp>
#include <project/AnalysisRecord.hpp>

#include <analysis/Analysis.hpp>
#include <analysis/Problem.hpp>
#include <analysis/DataPoint.hpp>
#include <analysis/SamplingAlgorithmOptions.hpp>
#include <analysis/SamplingAlgorithmOptions_Impl.hpp>
#include <analysis/SamplingAlgorithm.hpp>

#include <model/Model.hpp>

#include <runmanager/lib/RunManager.hpp>
#include <runmanager/lib/Workflow.hpp>
#include <runmanager/Test/ToolBin.hxx>

#include <utilities/document/Table.hpp>
#include <utilities/core/Optional.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/core/PathHelpers.hpp>
#include <utilities/core/Checksum.hpp>
#include <utilities/core/FileReference.hpp>

#include <resources.hxx>
#include <OpenStudio.hxx>

#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

using namespace openstudio;
using namespace openstudio::model;
using namespace openstudio::ruleset;
using namespace openstudio::analysis;
using namespace openstudio::project;
using namespace openstudio::analysisdriver;

TEST_F(AnalysisDriverFixture, Sampling_SimpleUQ) {

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("SimpleUQ",true,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  SamplingAlgorithmOptions algOptions;
  algOptions.setSamples(5);
  Analysis analysis("Sampling Algorithm - SimpleUQ",
                    problem,
                    SamplingAlgorithm(algOptions),
                    seedModel);

  // RUN ANALYSIS
  ProjectDatabase database = getCleanDatabase("SamplingAlgorithm_SimpleUQ");
  AnalysisDriver analysisDriver(database);
  AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
  CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
  EXPECT_TRUE(analysisDriver.waitForFinished());
  boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
  ASSERT_TRUE(jobErrors);
  EXPECT_TRUE(jobErrors->errors().empty());

  // output csv summary of data points
  Table summary = currentAnalysis.analysis().summaryTable();
  summary.save(analysisDriver.database().path().parent_path() / toPath("summary.csv"));

  BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
    EXPECT_TRUE(dataPoint.isComplete());
    EXPECT_FALSE(dataPoint.failed());
  }
}

TEST_F(AnalysisDriverFixture, Sampling_MixedContinuousAndDiscreteUQ) {

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("MixedContinuousAndDiscreteUQ",true,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  SamplingAlgorithmOptions algOptions;
  algOptions.setSamples(5);
  Analysis analysis("Sampling Algorithm - MixedContinuousAndDiscreteUQ",
                    problem,
                    SamplingAlgorithm(algOptions),
                    seedModel);

  // RUN ANALYSIS
  ProjectDatabase database = getCleanDatabase("SamplingAlgorithm_MixedContinuousAndDiscreteUQ");
  AnalysisDriver analysisDriver(database);
  AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
  CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
  EXPECT_TRUE(analysisDriver.waitForFinished());
  boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
  ASSERT_TRUE(jobErrors);
  EXPECT_TRUE(jobErrors->errors().empty());

  // output csv summary of data points
  Table summary = currentAnalysis.analysis().summaryTable();
  summary.save(analysisDriver.database().path().parent_path() / toPath("summary.csv"));

  BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
    EXPECT_TRUE(dataPoint.isComplete());
    EXPECT_FALSE(dataPoint.failed());
  }
}

TEST_F(AnalysisDriverFixture, Sampling_SimpleLognormalUQ) {

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("SimpleLognormalUQ",true,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  SamplingAlgorithmOptions algOptions;
  algOptions.setSamples(5);
  Analysis analysis("Sampling Algorithm - SimpleLognormalUQ",
                    problem,
                    SamplingAlgorithm(algOptions),
                    seedModel);

  // RUN ANALYSIS
  ProjectDatabase database = getCleanDatabase("SamplingAlgorithm_SimpleLognormalUQ");
  AnalysisDriver analysisDriver(database);
  AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
  CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
  EXPECT_TRUE(analysisDriver.waitForFinished());
  boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
  ASSERT_TRUE(jobErrors);
  EXPECT_TRUE(jobErrors->errors().empty());

  // output csv summary of data points
  Table summary = currentAnalysis.analysis().summaryTable();
  summary.save(analysisDriver.database().path().parent_path() / toPath("summary.csv"));

  BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
    EXPECT_TRUE(dataPoint.isComplete());
    EXPECT_FALSE(dataPoint.failed());
  }
}

TEST_F(AnalysisDriverFixture, Sampling_SimpleHistogramBinUQ) {

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("SimpleHistogramBinUQ",true,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  SamplingAlgorithmOptions algOptions;
  algOptions.setSamples(5);
  Analysis analysis("Sampling Algorithm - SimpleHistogramBinUQ",
                    problem,
                    SamplingAlgorithm(algOptions),
                    seedModel);

  // RUN ANALYSIS
  ProjectDatabase database = getCleanDatabase("SamplingAlgorithm_SimpleHistogramBinUQ");
  AnalysisDriver analysisDriver(database);
  AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
  CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
  EXPECT_TRUE(analysisDriver.waitForFinished());
  boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
  ASSERT_TRUE(jobErrors);
  EXPECT_TRUE(jobErrors->errors().empty());

  // output csv summary of data points
  Table summary = currentAnalysis.analysis().summaryTable();
  summary.save(analysisDriver.database().path().parent_path() / toPath("summary.csv"));

  BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
    EXPECT_TRUE(dataPoint.isComplete());
    EXPECT_FALSE(dataPoint.failed());
  }
}
