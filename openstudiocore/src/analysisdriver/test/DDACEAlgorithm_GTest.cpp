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
#include <analysisdriver/SimpleProject.hpp>

#include <project/ProjectDatabase.hpp>
#include <project/AnalysisRecord.hpp>

#include <analysis/Analysis.hpp>
#include <analysis/Analysis_Impl.hpp>
#include <analysis/Problem.hpp>
#include <analysis/DataPoint.hpp>
#include <analysis/DDACEAlgorithmOptions.hpp>
#include <analysis/DDACEAlgorithmOptions_Impl.hpp>
#include <analysis/DDACEAlgorithm.hpp>
#include <analysis/DDACEAlgorithm_Impl.hpp>

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

#include <QFileInfo>
#include <QDateTime>

using namespace openstudio;
using namespace openstudio::model;
using namespace openstudio::ruleset;
using namespace openstudio::analysis;
using namespace openstudio::project;
using namespace openstudio::analysisdriver;

TEST_F(AnalysisDriverFixture, DDACE_CentralComposite_Continuous) {

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("Continuous",true,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  DDACEAlgorithmOptions algOptions(DDACEAlgorithmType::central_composite);
  Analysis analysis("DDACE Central Composite",
                    problem,
                    DDACEAlgorithm(algOptions),
                    seedModel);

  // RUN ANALYSIS
  ProjectDatabase database = getCleanDatabase("DDACECentralComposite");
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
  EXPECT_EQ(DDACEAlgorithmOptions::samplesForCentralComposite(problem),int(summary.nRows()-1));

  BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
    EXPECT_TRUE(dataPoint.isComplete());
    EXPECT_FALSE(dataPoint.failed());
    // EXPECT_FALSE(dataPoint.responseValues().empty());
  }
}

TEST_F(AnalysisDriverFixture, DDACE_CentralComposite_MixedOsmIdf) {

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("MixedOsmIdf",false,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  DDACEAlgorithmOptions algOptions(DDACEAlgorithmType::central_composite);
  // central_composite not compatible with discrete variables
  EXPECT_ANY_THROW(Analysis analysis("DDACE Central Composite",problem,DDACEAlgorithm(algOptions),seedModel));

}

TEST_F(AnalysisDriverFixture, DDACE_BoxBehnken_Continuous) {

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("Continuous",true,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  DDACEAlgorithmOptions algOptions(DDACEAlgorithmType::box_behnken);
  Analysis analysis("DDACE Box-Behnken",
                    problem,
                    DDACEAlgorithm(algOptions),
                    seedModel);

  // RUN ANALYSIS
  ProjectDatabase database = getCleanDatabase("DDACEBoxBehnken");
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
  EXPECT_EQ(DDACEAlgorithmOptions::samplesForBoxBehnken(problem),int(summary.nRows()-1));

  BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
    EXPECT_TRUE(dataPoint.isComplete());
    EXPECT_FALSE(dataPoint.failed());
    // EXPECT_FALSE(dataPoint.responseValues().empty());
  }
}

TEST_F(AnalysisDriverFixture, DDACE_BoxBehnken_MixedOsmIdf) {

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("MixedOsmIdf",false,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  DDACEAlgorithmOptions algOptions(DDACEAlgorithmType::box_behnken);
  // box_behnken not compatible with discrete variables
  EXPECT_ANY_THROW(Analysis analysis("DDACE Box-Behnken",problem,DDACEAlgorithm(algOptions),seedModel));

}

TEST_F(AnalysisDriverFixture, DDACE_OrthogonalArray_Continuous) {
  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("Continuous",true,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  DDACEAlgorithmOptions algOptions(DDACEAlgorithmType::oas);
  Analysis analysis("DDACE Orthogonal Array Sampling",
                    problem,
                    DDACEAlgorithm(algOptions),
                    seedModel);

  // RUN ANALYSIS
  {
    ProjectDatabase database = getCleanDatabase("DDACEOrthogonalArray_NoSamples");
    AnalysisDriver analysisDriver(database);
    AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
    CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
    EXPECT_TRUE(analysisDriver.waitForFinished());
    boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
    ASSERT_TRUE(jobErrors);
    EXPECT_FALSE(jobErrors->errors().empty()); // require specification of number of samples
    EXPECT_TRUE(analysisDriver.currentAnalyses().empty());
    Table summary = currentAnalysis.analysis().summaryTable();
    EXPECT_EQ(1u,summary.nRows()); // no points
  }

  {
    algOptions.setSamples(6); // symbols = 0
    analysis = Analysis("DDACE Orthogonal Array Sampling - Wrong Samples",
                        problem,
                        DDACEAlgorithm(algOptions),
                        seedModel);
    ProjectDatabase database = getCleanDatabase("DDACEOrthogonalArray_WrongSamples");
    AnalysisDriver analysisDriver = AnalysisDriver(database);
    AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
    CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
    EXPECT_TRUE(analysisDriver.waitForFinished());
    boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
    ASSERT_TRUE(jobErrors);
    EXPECT_TRUE(jobErrors->errors().empty());
    EXPECT_TRUE(analysisDriver.currentAnalyses().empty());
    Table summary = currentAnalysis.analysis().summaryTable();
    EXPECT_EQ(10u,summary.nRows()); // 9 points
    summary.save(analysisDriver.database().path().parent_path() / toPath("summary.csv"));

    BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
      EXPECT_TRUE(dataPoint.isComplete());
      EXPECT_FALSE(dataPoint.failed());
      // EXPECT_FALSE(dataPoint.responseValues().empty());
    }
  }

  {
    bool ok = algOptions.setSamplesAndSymbolsForOrthogonalArray(6,1);
    EXPECT_FALSE(ok);
    ok = algOptions.setSamplesAndSymbolsForOrthogonalArray(2,2); // 2,3 not ok for no apparent reason
    EXPECT_TRUE(ok);
    ASSERT_TRUE(algOptions.symbols());
    EXPECT_EQ(2,algOptions.symbols().get());
    ASSERT_TRUE(algOptions.samples());
    EXPECT_EQ(8,algOptions.samples().get());
    analysis = Analysis("DDACE Orthogonal Array Sampling - Correct Samples",
                        problem,
                        DDACEAlgorithm(algOptions),
                        seedModel);
    ProjectDatabase database = getCleanDatabase("DDACEOrthogonalArray_CorrectSamples");
    AnalysisDriver analysisDriver = AnalysisDriver(database);
    AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
    CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
    EXPECT_TRUE(analysisDriver.waitForFinished());
    boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
    ASSERT_TRUE(jobErrors);
    EXPECT_TRUE(jobErrors->errors().empty());
    EXPECT_TRUE(analysisDriver.currentAnalyses().empty());
    Table summary = currentAnalysis.analysis().summaryTable();
    EXPECT_EQ(9u,summary.nRows());
    summary.save(analysisDriver.database().path().parent_path() / toPath("summary.csv"));

    BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
      EXPECT_TRUE(dataPoint.isComplete());
      EXPECT_FALSE(dataPoint.failed());
    }
  }
}

TEST_F(AnalysisDriverFixture, DDACE_OrthogonalArray_MixedOsmIdf) {

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("MixedOsmIdf",false,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  DDACEAlgorithmOptions algOptions(DDACEAlgorithmType::oas);
  algOptions.setSamples(4);
  Analysis analysis("DDACE Orthogonal Array Sampling",
                    problem,
                    DDACEAlgorithm(algOptions),
                    seedModel);

  // RUN ANALYSIS
  analysis = Analysis("DDACE Orthogonal Array Sampling - MixedOsmIdf",
                      problem,
                      DDACEAlgorithm(algOptions),
                      seedModel);
  ProjectDatabase database = getCleanDatabase("DDACEOrthogonalArray_MixedOsmIdf");
  AnalysisDriver analysisDriver = AnalysisDriver(database);
  AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
  CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
  EXPECT_TRUE(analysisDriver.waitForFinished());
  boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
  ASSERT_TRUE(jobErrors);
  EXPECT_TRUE(jobErrors->errors().empty());
  EXPECT_TRUE(analysisDriver.currentAnalyses().empty());
  Table summary = currentAnalysis.analysis().summaryTable();
  // EXPECT_EQ(10u,summary.nRows()); // 9 points
  summary.save(analysisDriver.database().path().parent_path() / toPath("summary.csv"));

  BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
    EXPECT_TRUE(dataPoint.isComplete());
    EXPECT_FALSE(dataPoint.failed());
  }

}

TEST_F(AnalysisDriverFixture, DDACE_Grid_Continuous) {
  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("Continuous",true,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  DDACEAlgorithmOptions algOptions(DDACEAlgorithmType::grid);
  Analysis analysis("DDACE Grid Sampling",
                    problem,
                    DDACEAlgorithm(algOptions),
                    seedModel);

  // RUN ANALYSIS
  {
    ProjectDatabase database = getCleanDatabase("DDACEGridSampling_NoSamples");
    AnalysisDriver analysisDriver(database);
    AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
    runOptions.setQueueSize(4);
    CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
    EXPECT_TRUE(analysisDriver.waitForFinished());
    boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
    ASSERT_TRUE(jobErrors);
    EXPECT_FALSE(jobErrors->errors().empty()); // require specification of number of samples
    EXPECT_TRUE(analysisDriver.currentAnalyses().empty());
    Table summary = currentAnalysis.analysis().summaryTable();
    EXPECT_EQ(1u,summary.nRows()); // no points
  }

  // ETH@20120120 At first tried just using the same database, analysisdriver, etc., but
  // it did not go well. Restarting an initially failed Dakota analysis should be part of
  // enabling Dakota restart more generally.

  {
    // algorithm rounds samples up to next one that fits n**(problem.numVariables())
    algOptions.setSamples(6);
    analysis = Analysis("DDACE Grid Sampling - Wrong Samples",
                        problem,
                        DDACEAlgorithm(algOptions),
                        seedModel);
    ProjectDatabase database = getCleanDatabase("DDACEGridSampling_WrongSamples");
    AnalysisDriver analysisDriver = AnalysisDriver(database);
    AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
    CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
    EXPECT_TRUE(analysisDriver.waitForFinished());
    boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
    ASSERT_TRUE(jobErrors);
    EXPECT_TRUE(jobErrors->errors().empty());
    EXPECT_TRUE(analysisDriver.currentAnalyses().empty());
    Table summary = currentAnalysis.analysis().summaryTable();
    EXPECT_EQ(10u,summary.nRows()); // 9 points
    summary.save(analysisDriver.database().path().parent_path() / toPath("summary.csv"));

    BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
      EXPECT_TRUE(dataPoint.isComplete());
      EXPECT_FALSE(dataPoint.failed());
      // EXPECT_FALSE(dataPoint.responseValues().empty());
    }
  }

  {
    // algorithm rounds samples up to next one that fits n**(problem.numVariables())
    algOptions.setSamplesForGrid(2,problem);
    analysis = Analysis("DDACE Grid Sampling - Correct Samples",
                        problem,
                        DDACEAlgorithm(algOptions),
                        seedModel);
    ProjectDatabase database = getCleanDatabase("DDACEGridSampling_CorrectSamples");
    AnalysisDriver analysisDriver = AnalysisDriver(database);
    AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
    CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
    EXPECT_TRUE(analysisDriver.waitForFinished());
    boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
    ASSERT_TRUE(jobErrors);
    EXPECT_TRUE(jobErrors->errors().empty());
    EXPECT_TRUE(analysisDriver.currentAnalyses().empty());
    Table summary = currentAnalysis.analysis().summaryTable();
    EXPECT_EQ(5u,summary.nRows()); // 4 points
    summary.save(analysisDriver.database().path().parent_path() / toPath("summary.csv"));

    BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
      EXPECT_TRUE(dataPoint.isComplete());
      EXPECT_FALSE(dataPoint.failed());
      // EXPECT_FALSE(dataPoint.responseValues().empty());
    }
  }
}

TEST_F(AnalysisDriverFixture, DDACE_Grid_MixedOsmIdf) {

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("MixedOsmIdf",false,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  DDACEAlgorithmOptions algOptions(DDACEAlgorithmType::grid);
  algOptions.setSamplesForGrid(2,problem);
  EXPECT_ANY_THROW(Analysis analysis("DDACE Grid Sampling",problem,DDACEAlgorithm(algOptions),seedModel));

}

TEST_F(AnalysisDriverFixture, DDACE_MonteCarlo_Continuous) {
  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("Continuous",true,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  DDACEAlgorithmOptions algOptions(DDACEAlgorithmType::random);
  Analysis analysis("DDACE Monte Carlo Sampling",
                    problem,
                    DDACEAlgorithm(algOptions),
                    seedModel);

  // RUN ANALYSIS
  {
    ProjectDatabase database = getCleanDatabase("DDACEMonteCarlo_Continuous_NoSamples");
    AnalysisDriver analysisDriver(database);
    AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
    CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
    EXPECT_TRUE(analysisDriver.waitForFinished());
    boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
    ASSERT_TRUE(jobErrors);
    EXPECT_FALSE(jobErrors->errors().empty()); // require specification of number of samples
    EXPECT_TRUE(analysisDriver.currentAnalyses().empty());
    Table summary = currentAnalysis.analysis().summaryTable();
    EXPECT_EQ(1u,summary.nRows()); // no points
  }

  {
    algOptions.setSamples(6);
    analysis = Analysis("DDACE MonteCarlo Sampling",
                        problem,
                        DDACEAlgorithm(algOptions),
                        seedModel);
    ProjectDatabase database = getCleanDatabase("DDACEMonteCarlo_Continuous");
    AnalysisDriver analysisDriver = AnalysisDriver(database);
    AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
    runOptions.setQueueSize(4);
    CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
    EXPECT_TRUE(analysisDriver.waitForFinished());
    boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
    ASSERT_TRUE(jobErrors);
    EXPECT_TRUE(jobErrors->errors().empty());
    EXPECT_TRUE(analysisDriver.currentAnalyses().empty());
    Table summary = currentAnalysis.analysis().summaryTable();
    EXPECT_EQ(7u,summary.nRows()); // 6 points
    summary.save(analysisDriver.database().path().parent_path() / toPath("summary.csv"));

    BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
      EXPECT_TRUE(dataPoint.isComplete());
      EXPECT_FALSE(dataPoint.failed());
      // EXPECT_FALSE(dataPoint.responseValues().empty());
    }
  }
}

TEST_F(AnalysisDriverFixture, DDACE_MonteCarlo_MixedOsmIdf_ProjectDatabaseOpen) {

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("MixedOsmIdf",false,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  DDACEAlgorithmOptions algOptions(DDACEAlgorithmType::oas);
  algOptions.setSamples(4);
  Analysis analysis("DDACE Monte Carlo Sampling",
                    problem,
                    DDACEAlgorithm(algOptions),
                    seedModel);

  // RUN ANALYSIS
  {
    analysis = Analysis("DDACE Monte Carlo Sampling - MixedOsmIdf",
                        problem,
                        DDACEAlgorithm(algOptions),
                        seedModel);
    ProjectDatabase database = getCleanDatabase("DDACEMonteCarlo_MixedOsmIdf");
    AnalysisDriver analysisDriver = AnalysisDriver(database);
    AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
    CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
    EXPECT_TRUE(analysisDriver.waitForFinished());
    boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
    ASSERT_TRUE(jobErrors);
    EXPECT_TRUE(jobErrors->errors().empty());
    EXPECT_TRUE(analysisDriver.currentAnalyses().empty());
    Table summary = currentAnalysis.analysis().summaryTable();
    EXPECT_EQ(5u,summary.nRows()); // 4 points
    summary.save(analysisDriver.database().path().parent_path() / toPath("summary.csv"));

    BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
      EXPECT_TRUE(dataPoint.isComplete());
      EXPECT_FALSE(dataPoint.failed());
    }
  }
  {
    project::OptionalProjectDatabase oDatabase = project::ProjectDatabase::open(toPath("AnalysisDriverFixtureData/DDACEMonteCarloMixedOsmIdf/DDACEMonteCarlo_MixedOsmIdf.osp"));
    ASSERT_TRUE(oDatabase);
    project::AnalysisRecordVector analysisRecords = project::AnalysisRecord::getAnalysisRecords(*oDatabase);
    EXPECT_EQ(1u,analysisRecords.size());
    if (!analysisRecords.empty()) {
      EXPECT_NO_THROW(analysisRecords[0].analysis());
    }
  }

}

TEST_F(AnalysisDriverFixture, DDACE_LatinHypercube_Continuous) {
  {
    // GET SIMPLE PROJECT
    SimpleProject project = getCleanSimpleProject("DDACE_LatinHypercube_Continuous");
    Analysis analysis = project.analysis();

    // SET PROBLEM
    Problem problem = retrieveProblem("Continuous",true,false);
    analysis.setProblem(problem);

    // DEFINE SEED
    Model model = model::exampleModel();
    openstudio::path p = toPath("./example.osm");
    model.save(p,true);
    FileReference seedModel(p);
    analysis.setSeed(seedModel);

    // CREATE ANALYSIS
    DDACEAlgorithmOptions algOptions(DDACEAlgorithmType::lhs);
    DDACEAlgorithm algorithm(algOptions);
    analysis.setAlgorithm(algorithm);

    // RUN ANALYSIS
    AnalysisDriver driver = project.analysisDriver();
    AnalysisRunOptions runOptions = standardRunOptions(project.projectDir());
    CurrentAnalysis currentAnalysis = driver.run(analysis,runOptions);
    EXPECT_TRUE(driver.waitForFinished());
    boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
    ASSERT_TRUE(jobErrors);
    EXPECT_FALSE(jobErrors->errors().empty()); // require specification of number of samples
    EXPECT_TRUE(driver.currentAnalyses().empty());
    Table summary = currentAnalysis.analysis().summaryTable();
    EXPECT_EQ(1u,summary.nRows()); // no points

    project.clearAllResults();
    algOptions.setSamples(4);
    EXPECT_EQ(4,analysis.algorithm()->cast<DDACEAlgorithm>().ddaceAlgorithmOptions().samples());
    currentAnalysis = driver.run(analysis,runOptions);
    EXPECT_TRUE(driver.waitForFinished());
    jobErrors = currentAnalysis.dakotaJobErrors();
    ASSERT_TRUE(jobErrors);
    EXPECT_TRUE(jobErrors->errors().empty());
    EXPECT_TRUE(driver.currentAnalyses().empty());
    summary = currentAnalysis.analysis().summaryTable();
    EXPECT_EQ(5u,summary.nRows());
    summary.save(project.projectDir() / toPath("summary.csv"));

    BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
      EXPECT_TRUE(dataPoint.isComplete());
      EXPECT_FALSE(dataPoint.failed());
      // EXPECT_FALSE(dataPoint.responseValues().empty());
    }

    ASSERT_TRUE(analysis.algorithm());
    EXPECT_TRUE(analysis.algorithm()->isComplete());
    EXPECT_FALSE(analysis.algorithm()->failed());

    {
      AnalysisRecord analysisRecord = project.analysisRecord();
      Analysis analysisCopy = analysisRecord.analysis();
      ASSERT_TRUE(analysisCopy.algorithm());
      EXPECT_TRUE(analysisCopy.algorithm()->isComplete());
      EXPECT_FALSE(analysisCopy.algorithm()->failed());
    }
  }

  LOG(Info,"Restart from existing project.");

  // Get existing project
  SimpleProject project = getSimpleProject("DDACE_LatinHypercube_Continuous");
  EXPECT_FALSE(project.analysisIsLoaded()); // make sure starting fresh
  Analysis analysis = project.analysis();
  EXPECT_FALSE(analysis.isDirty());

  // Add custom data point
  std::vector<QVariant> values;
  values.push_back(0.0);
  values.push_back(0.8);
  values.push_back(int(0));
  OptionalDataPoint dataPoint = analysis.problem().createDataPoint(values);
  ASSERT_TRUE(dataPoint);
  analysis.addDataPoint(*dataPoint);
  EXPECT_EQ(1u,analysis.dataPointsToQueue().size());
  ASSERT_TRUE(analysis.algorithm());
  EXPECT_TRUE(analysis.algorithm()->isComplete());
  EXPECT_FALSE(analysis.algorithm()->failed());
  EXPECT_TRUE(analysis.isDirty());
  EXPECT_FALSE(analysis.resultsAreInvalid());
  EXPECT_FALSE(analysis.dataPointsAreInvalid());

  // get last modified time of a file in a completed data point to make sure nothing is re-run
  DataPointVector completePoints = analysis.completeDataPoints();
  ASSERT_FALSE(completePoints.empty());
  OptionalFileReference inputFileRef = completePoints[0].osmInputData();
  ASSERT_TRUE(inputFileRef);
  QFileInfo inputFileInfo(toQString(inputFileRef->path()));
  QDateTime inputFileModifiedTestTime = inputFileInfo.lastModified();
  EXPECT_EQ(1u,analysis.dataPointsToQueue().size());

  AnalysisDriver driver = project.analysisDriver();
  CurrentAnalysis currentAnalysis = driver.run(
        analysis,
        standardRunOptions(project.projectDir()));
  EXPECT_TRUE(driver.waitForFinished());
  boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
  EXPECT_FALSE(jobErrors); // should not try to re-run DakotaAlgorithm
  EXPECT_TRUE(driver.currentAnalyses().empty());
  EXPECT_TRUE(analysis.dataPointsToQueue().empty());
  Table summary = currentAnalysis.analysis().summaryTable();
  EXPECT_EQ(6u,summary.nRows());
  summary.save(project.projectDir() / toPath("summary_post_restart.csv"));
  // RunManager should not re-run any data points
  EXPECT_EQ(inputFileModifiedTestTime,inputFileInfo.lastModified());
}

TEST_F(AnalysisDriverFixture, DDACE_LatinHypercube_MixedOsmIdf_MoveProjectDatabase) {
  openstudio::path oldDir, newDir;
  {
    // GET SIMPLE PROJECT
    SimpleProject project = getCleanSimpleProject("DDACE_LatinHypercube_MixedOsmIdf");
    Analysis analysis = project.analysis();
    analysis.setName("DDACE Latin Hypercube Sampling - MixedOsmIdf");

    // SET PROBLEM
    Problem problem = retrieveProblem("MixedOsmIdf",false,false);
    analysis.setProblem(problem);

    // SET SEED
    Model model = model::exampleModel();
    openstudio::path p = toPath("./example.osm");
    model.save(p,true);
    FileReference seedModel(p);
    analysis.setSeed(seedModel);

    // SET ALGORITHM
    DDACEAlgorithmOptions algOptions(DDACEAlgorithmType::lhs);
    algOptions.setSamples(12); // test reprinting results.out for copies of same point
    DDACEAlgorithm algorithm(algOptions);
    analysis.setAlgorithm(algorithm);

    // RUN ANALYSIS
    AnalysisDriver driver = project.analysisDriver();
    AnalysisRunOptions runOptions = standardRunOptions(project.projectDir());
    CurrentAnalysis currentAnalysis = driver.run(analysis,runOptions);
    EXPECT_TRUE(driver.waitForFinished());
    boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
    ASSERT_TRUE(jobErrors);
    EXPECT_TRUE(jobErrors->errors().empty());
    EXPECT_TRUE(driver.currentAnalyses().empty());
    Table summary = analysis.summaryTable();
    EXPECT_EQ(5u,summary.nRows()); // 4 points (all combinations)
    summary.save(project.projectDir() / toPath("summary.csv"));

    EXPECT_EQ(4u,analysis.dataPoints().size());
    BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
      EXPECT_TRUE(dataPoint.isComplete());
      EXPECT_FALSE(dataPoint.failed());
      EXPECT_TRUE(dataPoint.workspace()); // should be able to load data from disk
    }

    oldDir = project.projectDir();
    newDir = project.projectDir().parent_path() / toPath("DDACELatinHypercubeMixedOsmIdfCopy");
    // Make copy of project
    boost::filesystem::remove_all(newDir);
    ASSERT_TRUE(project.saveAs(newDir));
  }
  // Blow away old project.
  // TODO: Reinstate. This was failing on Windows and isn't absolutely necessary.
//     try {
//       boost::filesystem::remove_all(oldDir);
//     }
//     catch (std::exception& e) {
//       EXPECT_TRUE(false) << "Boost filesystem was unable to delete the old folder, because " << e.what();
//     }

  // Open new project
  SimpleProject project = getSimpleProject("DDACE_LatinHypercube_MixedOsmIdf_Copy");
  EXPECT_TRUE(project.projectDir() == newDir);
  EXPECT_EQ(toString(newDir),toString(project.projectDir()));

  // After move, should be able to retrieve results.
  EXPECT_FALSE(project.analysisIsLoaded());
  Analysis analysis = project.analysis();
  EXPECT_TRUE(project.analysisIsLoaded());
  EXPECT_EQ(4u,analysis.dataPoints().size());
  BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
    EXPECT_TRUE(dataPoint.isComplete());
    EXPECT_FALSE(dataPoint.failed());
    LOG(Debug,"Attempting to load workspace for data point at '" << dataPoint.directory() << "'.");
    if (dataPoint.idfInputData()) {
      LOG(Debug,"Says there should be input data at " << toString(dataPoint.idfInputData()->path()));
    }
    EXPECT_TRUE(dataPoint.workspace()); // should be able to load data from disk
    if (!dataPoint.workspace()) {
      LOG(Debug,"Unsuccessful.")
    }
  }

  // Should be able to blow away results and run again
  project.removeAllDataPoints();
  EXPECT_EQ(0u,analysis.dataPoints().size());
  EXPECT_FALSE(analysis.algorithm()->isComplete());
  EXPECT_FALSE(analysis.algorithm()->failed());
  EXPECT_EQ(-1,analysis.algorithm()->iter());
  EXPECT_FALSE(analysis.algorithm()->cast<DakotaAlgorithm>().restartFileReference());
  EXPECT_FALSE(analysis.algorithm()->cast<DakotaAlgorithm>().outFileReference());
  AnalysisRunOptions runOptions = standardRunOptions(project.projectDir());
  AnalysisDriver driver = project.analysisDriver();
  CurrentAnalysis currentAnalysis = driver.run(analysis,runOptions);
  EXPECT_TRUE(driver.waitForFinished());
  boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
  ASSERT_TRUE(jobErrors);
  EXPECT_TRUE(jobErrors->errors().empty());
  EXPECT_TRUE(driver.currentAnalyses().empty());
  Table summary = analysis.summaryTable();
  EXPECT_EQ(5u,summary.nRows()); // 4 points (all combinations)
  summary.save(project.projectDir() / toPath("summary.csv"));

  BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
    EXPECT_TRUE(dataPoint.isComplete());
    EXPECT_FALSE(dataPoint.failed());
    EXPECT_TRUE(dataPoint.workspace()); // should be able to load data from disk
  }
}

TEST_F(AnalysisDriverFixture, DDACE_LatinHypercube_UserScriptContinuous) {

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("UserScriptContinuous",true,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  DDACEAlgorithmOptions algOptions(DDACEAlgorithmType::lhs);
  algOptions.setSamples(10);

  // RUN ANALYSIS
  Analysis analysis("DDACE Latin Hypercube Sampling - UserScriptContinuous",
                    problem,
                    DDACEAlgorithm(algOptions),
                    seedModel);
  ProjectDatabase database = getCleanDatabase("DDACELatinHypercube_UserScriptContinuous");
  AnalysisDriver analysisDriver = AnalysisDriver(database);
  AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
  CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
  EXPECT_TRUE(analysisDriver.waitForFinished());
  boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
  ASSERT_TRUE(jobErrors);
  EXPECT_TRUE(jobErrors->errors().empty());
  EXPECT_TRUE(analysisDriver.currentAnalyses().empty());
  Table summary = currentAnalysis.analysis().summaryTable();
  EXPECT_EQ(11u,summary.nRows());
  summary.save(analysisDriver.database().path().parent_path() / toPath("summary.csv"));

  BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
    EXPECT_TRUE(dataPoint.isComplete());
    EXPECT_FALSE(dataPoint.failed());
  }

}

TEST_F(AnalysisDriverFixture, DDACE_OALHS_Continuous) {
  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("Continuous",true,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  DDACEAlgorithmOptions algOptions(DDACEAlgorithmType::oa_lhs);
  Analysis analysis("DDACE OA-LHS",
                    problem,
                    DDACEAlgorithm(algOptions),
                    seedModel);

  // RUN ANALYSIS
  {
    ProjectDatabase database = getCleanDatabase("DDACEOA-LHS_Continuous_NoSamples");
    AnalysisDriver analysisDriver(database);
    AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
    CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
    EXPECT_TRUE(analysisDriver.waitForFinished());
    boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
    ASSERT_TRUE(jobErrors);
    EXPECT_FALSE(jobErrors->errors().empty()); // require specification of number of samples
    EXPECT_TRUE(analysisDriver.currentAnalyses().empty());
    Table summary = currentAnalysis.analysis().summaryTable();
    EXPECT_EQ(1u,summary.nRows()); // no points
  }

  {
    algOptions.setSamples(5);
    analysis = Analysis("DDACE OA-LHS",
                        problem,
                        DDACEAlgorithm(algOptions),
                        seedModel);
    ProjectDatabase database = getCleanDatabase("DDACEOA-LHS_Continuous");
    AnalysisDriver analysisDriver = AnalysisDriver(database);
    AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
    CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
    EXPECT_TRUE(analysisDriver.waitForFinished());
    boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
    ASSERT_TRUE(jobErrors);
    EXPECT_TRUE(jobErrors->errors().empty());
    EXPECT_TRUE(analysisDriver.currentAnalyses().empty());
    Table summary = currentAnalysis.analysis().summaryTable();
    EXPECT_EQ(10u,summary.nRows()); // ups to 9 samples
    summary.save(analysisDriver.database().path().parent_path() / toPath("summary.csv"));

    BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
      EXPECT_TRUE(dataPoint.isComplete());
      EXPECT_FALSE(dataPoint.failed());
      // EXPECT_FALSE(dataPoint.responseValues().empty());
    }
  }
}

TEST_F(AnalysisDriverFixture, DDACE_OALHS_MixedOsmIdf) {

  // RETRIEVE PROBLEM
  Problem problem = retrieveProblem("MixedOsmIdf",false,false);

  // DEFINE SEED
  Model model = model::exampleModel();
  openstudio::path p = toPath("./example.osm");
  model.save(p,true);
  FileReference seedModel(p);

  // CREATE ANALYSIS
  DDACEAlgorithmOptions algOptions(DDACEAlgorithmType::oa_lhs);
  algOptions.setSamples(4);

  // RUN ANALYSIS
  Analysis analysis("DDACE OA-LHS - MixedOsmIdf",
                    problem,
                    DDACEAlgorithm(algOptions),
                    seedModel);
  ProjectDatabase database = getCleanDatabase("DDACEOA-LHS_MixedOsmIdf");
  AnalysisDriver analysisDriver = AnalysisDriver(database);
  AnalysisRunOptions runOptions = standardRunOptions(analysisDriver.database().path().parent_path());
  CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
  EXPECT_TRUE(analysisDriver.waitForFinished());
  boost::optional<runmanager::JobErrors> jobErrors = currentAnalysis.dakotaJobErrors();
  ASSERT_TRUE(jobErrors);
  EXPECT_TRUE(jobErrors->errors().empty());
  EXPECT_TRUE(analysisDriver.currentAnalyses().empty());
  Table summary = currentAnalysis.analysis().summaryTable();
  EXPECT_EQ(5u,summary.nRows()); // 4 points (all combinations)
  summary.save(analysisDriver.database().path().parent_path() / toPath("summary.csv"));

  BOOST_FOREACH(const DataPoint& dataPoint,analysis.dataPoints()) {
    EXPECT_TRUE(dataPoint.isComplete());
    EXPECT_FALSE(dataPoint.failed());
  }

}
