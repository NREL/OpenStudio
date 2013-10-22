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

#include <project/ProjectDatabase.hpp>
#include <project/AnalysisRecord.hpp>

#include <analysis/Analysis.hpp>
#include <analysis/Problem.hpp>
#include <analysis/DataPoint.hpp>
#include <analysis/ParameterStudyAlgorithmOptions.hpp>
#include <analysis/ParameterStudyAlgorithmOptions_Impl.hpp>
#include <analysis/ParameterStudyAlgorithm.hpp>

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

TEST_F(AnalysisDriverFixture, VectorParameterStudy) {

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("Continuous",true,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  ParameterStudyAlgorithmOptions algOptions(ParameterStudyAlgorithmType::vector_parameter_study);

  std::vector<double> vectorOne(problem.numContinuousVariables(), 0.8);

  algOptions.setFinalPoint(vectorOne);
  algOptions.setNumSteps(2);

  Analysis analysis("VectorParameterStudy",
                    problem,
                    ParameterStudyAlgorithm(algOptions),
                    seedModel);

  // RUN ANALYSIS
  ProjectDatabase database = getCleanDatabase("VectorParameterStudy");
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
    // EXPECT_FALSE(dataPoint.responseValues().empty());
  }
}


TEST_F(AnalysisDriverFixture, VectorParameterStudy_MixedOsmIdf) {

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("MixedOsmIdf",false,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  ParameterStudyAlgorithmOptions algOptions(ParameterStudyAlgorithmType::vector_parameter_study);
  std::vector<double> vectorOne(2);
  vectorOne[0]=1.5;
  vectorOne[1]=1.5;

  algOptions.setFinalPoint(vectorOne);
  algOptions.setNumSteps(2);
  Analysis analysis("VectorParameterStudy Sampling",
                    problem,
                    ParameterStudyAlgorithm(algOptions),
                    seedModel);

  // RUN ANALYSIS
  analysis = Analysis("VectorParameterStudy Sampling - MixedOsmIdf",
                      problem,
                      ParameterStudyAlgorithm(algOptions),
                      seedModel);
  ProjectDatabase database = getCleanDatabase("VectorParameterStudy_MixedOsmIdf");
  AnalysisDriver analysisDriver = AnalysisDriver(database);
  AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
  CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
  EXPECT_TRUE(analysisDriver.waitForFinished());
  boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
  ASSERT_TRUE(jobErrors);
  EXPECT_TRUE(jobErrors->errors().empty());
  EXPECT_TRUE(analysisDriver.currentAnalyses().empty());
  Table summary = currentAnalysis.analysis().summaryTable();
  summary.save(analysisDriver.database().path().parent_path() / toPath("summary.csv"));

  BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
    EXPECT_TRUE(dataPoint.isComplete());
    EXPECT_FALSE(dataPoint.failed());
  }

}

TEST_F(AnalysisDriverFixture, ListParameterStudy) {

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("Continuous",true,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  ParameterStudyAlgorithmOptions algOptions(ParameterStudyAlgorithmType::list_parameter_study);

  std::vector<double> vectorOne(4);
  vectorOne[0]=0.7;
  vectorOne[1]=0.7;
  vectorOne[2]=0.8;
  vectorOne[3]=0.8;

  algOptions.setListOfPoints(vectorOne);

  Analysis analysis("ListParameterStudy",
                    problem,
                    ParameterStudyAlgorithm(algOptions),
                    seedModel);

  // RUN ANALYSIS
  ProjectDatabase database = getCleanDatabase("ListParameterStudy");
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
    // EXPECT_FALSE(dataPoint.responseValues().empty());
  }
}


TEST_F(AnalysisDriverFixture, ListParameterStudy_MixedOsmIdf) {

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("MixedOsmIdf",false,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  ParameterStudyAlgorithmOptions algOptions(ParameterStudyAlgorithmType::list_parameter_study);

  std::vector<double> vectorOne(4);
  vectorOne[0]=0.0;
  vectorOne[1]=0.5;
  vectorOne[2]=1.0;
  vectorOne[3]=1.5;

  algOptions.setListOfPoints(vectorOne);
  Analysis analysis("ListParameterStudy Sampling",
                    problem,
                    ParameterStudyAlgorithm(algOptions),
                    seedModel);

  // RUN ANALYSIS
  analysis = Analysis("ListParameterStudy Sampling - MixedOsmIdf",
                      problem,
                      ParameterStudyAlgorithm(algOptions),
                      seedModel);
  ProjectDatabase database = getCleanDatabase("ListParameterStudy_MixedOsmIdf");
  AnalysisDriver analysisDriver = AnalysisDriver(database);
  AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
  CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
  EXPECT_TRUE(analysisDriver.waitForFinished());
  boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
  ASSERT_TRUE(jobErrors);
  EXPECT_TRUE(jobErrors->errors().empty());
  EXPECT_TRUE(analysisDriver.currentAnalyses().empty());
  Table summary = currentAnalysis.analysis().summaryTable();
  summary.save(analysisDriver.database().path().parent_path() / toPath("summary.csv"));

  BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
    EXPECT_TRUE(dataPoint.isComplete());
    EXPECT_FALSE(dataPoint.failed());
  }

}

TEST_F(AnalysisDriverFixture, CenteredParameterStudy) {

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("Continuous",true,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  ParameterStudyAlgorithmOptions algOptions(ParameterStudyAlgorithmType::centered_parameter_study);

  std::vector<double> vectorOne(2);
  vectorOne[0]=0.8;
  vectorOne[1]=0.8;

  std::vector<int> vectorTwo(2);
  vectorTwo[0]=0;
  vectorTwo[1]=0;

  algOptions.setStepVector(vectorOne);
  algOptions.setStepsPerVariable(vectorTwo);

  Analysis analysis("CenteredParameterStudy",
                    problem,
                    ParameterStudyAlgorithm(algOptions),
                    seedModel);

  // RUN ANALYSIS
  ProjectDatabase database = getCleanDatabase("CenteredParameterStudy");
  AnalysisDriver analysisDriver(database);
  AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
  runOptions.setQueueSize(4);
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
    // EXPECT_FALSE(dataPoint.responseValues().empty());
  }
}


TEST_F(AnalysisDriverFixture, CenteredParameterStudy_MixedOsmIdf) {

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("MixedOsmIdf",false,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  ParameterStudyAlgorithmOptions algOptions(ParameterStudyAlgorithmType::centered_parameter_study);

  std::vector<double> vectorOne(2);
  vectorOne[0]=1;
  vectorOne[1]=0.5;

  std::vector<int> vectorTwo(2);
  vectorTwo[0]=1;
  vectorTwo[1]=2;

  algOptions.setStepVector(vectorOne);
  algOptions.setStepsPerVariable(vectorTwo);
  Analysis analysis("CenteredParameterStudy Sampling",
                    problem,
                    ParameterStudyAlgorithm(algOptions),
                    seedModel);

  // RUN ANALYSIS
  analysis = Analysis("CenteredParameterStudy Sampling - MixedOsmIdf",
                      problem,
                      ParameterStudyAlgorithm(algOptions),
                      seedModel);
  ProjectDatabase database = getCleanDatabase("CenteredParameterStudy_MixedOsmIdf");
  AnalysisDriver analysisDriver = AnalysisDriver(database);
  AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
  CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
  EXPECT_TRUE(analysisDriver.waitForFinished());
  boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
  ASSERT_TRUE(jobErrors);
  EXPECT_TRUE(jobErrors->errors().empty());
  EXPECT_TRUE(analysisDriver.currentAnalyses().empty());
  Table summary = currentAnalysis.analysis().summaryTable();
  summary.save(analysisDriver.database().path().parent_path() / toPath("summary.csv"));

  BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
    EXPECT_TRUE(dataPoint.isComplete());
    EXPECT_FALSE(dataPoint.failed());
  }

}

TEST_F(AnalysisDriverFixture, MultidimParameterStudy) {

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("Continuous",true,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  ParameterStudyAlgorithmOptions algOptions(ParameterStudyAlgorithmType::multidim_parameter_study);

  std::vector<int> vectorOne(2);
  vectorOne[0]=1;
  vectorOne[1]=2;

  algOptions.setPartitions(vectorOne);

  Analysis analysis("MultidimParameterStudy",
                    problem,
                    ParameterStudyAlgorithm(algOptions),
                    seedModel);

  // RUN ANALYSIS
  ProjectDatabase database = getCleanDatabase("MultidimParameterStudy");
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
    // EXPECT_FALSE(dataPoint.responseValues().empty());
  }
}


TEST_F(AnalysisDriverFixture, MultidimParameterStudy_MixedOsmIdf) {

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("MixedOsmIdf",false,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  ParameterStudyAlgorithmOptions algOptions(ParameterStudyAlgorithmType::multidim_parameter_study);

  std::vector<int> vectorOne(2);
  vectorOne[0]=1;
  vectorOne[1]=2;

  algOptions.setPartitions(vectorOne);
  Analysis analysis("MultidimParameterStudy Sampling",
                    problem,
                    ParameterStudyAlgorithm(algOptions),
                    seedModel);
  analysis = Analysis("MultidimParameterStudy Sampling - MixedOsmIdf",
                      problem,
                      ParameterStudyAlgorithm(algOptions),
                      seedModel);
  ProjectDatabase database = getCleanDatabase("MultidimParameterStudy_MixedOsmIdf");
  AnalysisDriver analysisDriver = AnalysisDriver(database);
  AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
  CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
  EXPECT_TRUE(analysisDriver.waitForFinished());
  boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
  ASSERT_TRUE(jobErrors);
  EXPECT_TRUE(jobErrors->errors().empty());
  EXPECT_TRUE(analysisDriver.currentAnalyses().empty());
  Table summary = currentAnalysis.analysis().summaryTable();
  summary.save(analysisDriver.database().path().parent_path() / toPath("summary.csv"));

  BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
    EXPECT_TRUE(dataPoint.isComplete());
    EXPECT_FALSE(dataPoint.failed());
  }

}
