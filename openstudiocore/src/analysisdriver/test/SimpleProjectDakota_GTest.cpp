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

#include <analysisdriver/SimpleProject.hpp>
#include <analysisdriver/CurrentAnalysis.hpp>
#include <analysisdriver/AnalysisRunOptions.hpp>

#include <analysis/Analysis.hpp>
#include <analysis/Problem.hpp>
#include <analysis/Algorithm.hpp>
#include <analysis/DataPoint.hpp>
#include <analysis/DDACEAlgorithmOptions.hpp>
#include <analysis/DDACEAlgorithm.hpp>
#include <analysis/DesignOfExperimentsOptions.hpp>
#include <analysis/DesignOfExperiments.hpp>
#include <analysis/Measure.hpp>
#include <analysis/MeasureGroup.hpp>
#include <analysis/NullMeasure.hpp>
#include <analysis/RubyMeasure.hpp>
#include <analysis/WorkflowStep.hpp>

#include <project/ProjectDatabase.hpp>
#include <project/AnalysisRecord.hpp>
#include <project/ProblemRecord.hpp>
#include <project/DataPointRecord.hpp>
#include <project/InputVariableRecord.hpp>
#include <project/DakotaAlgorithmRecord.hpp>
#include <project/DakotaAlgorithmRecord_Impl.hpp>
#include <project/FileReferenceRecord.hpp>
#include <project/WorkflowRecord.hpp>

#include <runmanager/lib/Job.hpp>
#include <runmanager/lib/RunManager.hpp>
#include <runmanager/Test/ToolBin.hxx>
#include <runmanager/lib/Workflow.hpp>
#include <runmanager/lib/WorkItem.hpp>

#include <ruleset/OSArgument.hpp>

#include <model/Model.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>

#include <utilities/bcl/BCLMeasure.hpp>
#include <utilities/core/FileReference.hpp>
#include <utilities/core/PathHelpers.hpp>
#include <utilities/sql/SqlFile.hpp>

#include <OpenStudio.hxx>
#include <resources.hxx>

#include <boost/foreach.hpp>

using namespace openstudio;
using namespace openstudio::model;
using namespace openstudio::ruleset;
using namespace openstudio::runmanager;
using namespace openstudio::analysis;
using namespace openstudio::project;
using namespace openstudio::analysisdriver;

TEST_F(AnalysisDriverFixture,SimpleProject_DakotaClearAllResults) {
  // March 12, 2013 - ETH
  // Writing test because clearing results mostly working, but there seem to be some residual
  // FileReferenceRecord ids left in the DataPointRecord.
  // Test passes without making any changes, however.

  // ETH@20130701 - Thought I would remove the use of DakotaAlgorithm, but upon closer inspection,
  // it is good that this is testing serialization of the restart file, etc.

  // Create and run a project with a single LHS point
  SimpleProject project = getCleanPATProject("SimpleProject_DakotaClearAllResults");
  Analysis analysis = project.analysis();
  Problem problem = retrieveProblem(AnalysisDriverFixtureProblem::Continuous,true,false);
  analysis.setProblem(problem);
  openstudio::path p = toPath("./example.osm");
  model::exampleModel().save(p,true);
  FileReference seedModel(p);
  project.setSeed(seedModel);
  DDACEAlgorithmOptions algopts(DDACEAlgorithmType::lhs);
  algopts.setSamples(1);
  DDACEAlgorithm algorithm(algopts);
  analysis.setAlgorithm(algorithm);

  AnalysisDriver analysisDriver = project.analysisDriver();
  AnalysisRunOptions runOptions = standardRunOptions(project.projectDir());
  CurrentAnalysis currentAnalysis = analysisDriver.run(analysis,runOptions);
  analysisDriver.waitForFinished();

  // Check DakotaAlgorithm, DataPoint, and records for expected state
  EXPECT_TRUE(algorithm.isComplete());
  EXPECT_FALSE(algorithm.failed());
  ASSERT_TRUE(algorithm.restartFileReference());
  EXPECT_TRUE(boost::filesystem::exists(algorithm.restartFileReference().get().path()));
  ASSERT_TRUE(algorithm.outFileReference());
  EXPECT_TRUE(boost::filesystem::exists(algorithm.outFileReference().get().path()));
  EXPECT_TRUE(algorithm.job());

  ASSERT_EQ(1u,analysis.dataPoints().size());
  DataPoint dataPoint = analysis.dataPoints()[0];
  EXPECT_TRUE(dataPoint.isComplete());
  EXPECT_FALSE(dataPoint.failed());
  EXPECT_TRUE(dataPoint.responseValues().empty());
  EXPECT_FALSE(dataPoint.directory().empty());
  EXPECT_TRUE(boost::filesystem::exists(dataPoint.directory()));
  ASSERT_TRUE(dataPoint.osmInputData());
  EXPECT_TRUE(boost::filesystem::exists(dataPoint.osmInputData().get().path()));
  ASSERT_TRUE(dataPoint.idfInputData());
  EXPECT_TRUE(boost::filesystem::exists(dataPoint.idfInputData().get().path()));
  EXPECT_FALSE(dataPoint.sqlOutputData());
  EXPECT_TRUE(dataPoint.xmlOutputData().empty());
  EXPECT_TRUE(dataPoint.model());
  EXPECT_TRUE(dataPoint.workspace());
  EXPECT_FALSE(dataPoint.sqlFile());
  EXPECT_TRUE(dataPoint.outputAttributes().empty());
  EXPECT_TRUE(dataPoint.topLevelJob());

  openstudio::UUID dakotaJobUUID, dataPointJobUUID;
  {
    AnalysisRecord analysisRecord = project.analysisRecord();
    RunManager runManager = project.runManager();

    OptionalAlgorithmRecord algorithmRecord = analysisRecord.algorithmRecord();
    ASSERT_TRUE(algorithmRecord);
    ASSERT_TRUE(algorithmRecord.get().optionalCast<DakotaAlgorithmRecord>());
    DakotaAlgorithmRecord dakotaAlgorithmRecord = algorithmRecord->cast<DakotaAlgorithmRecord>();

    EXPECT_TRUE(dakotaAlgorithmRecord.isComplete());
    OptionalFileReferenceRecord frr = dakotaAlgorithmRecord.restartFileReferenceRecord();
    ASSERT_TRUE(frr);
    EXPECT_TRUE(boost::filesystem::exists(frr->path()));
    frr = dakotaAlgorithmRecord.outFileReferenceRecord();
    ASSERT_TRUE(frr);
    EXPECT_TRUE(boost::filesystem::exists(frr->path()));
    ASSERT_TRUE(dakotaAlgorithmRecord.jobUUID());
    dakotaJobUUID = dakotaAlgorithmRecord.jobUUID().get();
    EXPECT_NO_THROW(runManager.getJob(dakotaJobUUID));

    DataPointRecordVector dataPointRecords = analysisRecord.dataPointRecords();
    ASSERT_EQ(1u,dataPointRecords.size());
    DataPointRecord dataPointRecord = dataPointRecords[0];

    EXPECT_TRUE(dataPointRecord.isComplete());
    EXPECT_FALSE(dataPointRecord.failed());
    EXPECT_FALSE(dataPointRecord.directory().empty());
    EXPECT_TRUE(boost::filesystem::exists(dataPointRecord.directory()));
    EXPECT_TRUE(dataPointRecord.responseValues().empty());
    frr = dataPointRecord.osmInputDataRecord();
    ASSERT_TRUE(frr);
    EXPECT_TRUE(boost::filesystem::exists(frr->path()));
    frr = dataPointRecord.idfInputDataRecord();
    ASSERT_TRUE(frr);
    EXPECT_TRUE(boost::filesystem::exists(frr->path()));
    frr = dataPointRecord.sqlOutputDataRecord();
    EXPECT_FALSE(frr);
    FileReferenceRecordVector frrs = dataPointRecord.xmlOutputDataRecords();
    EXPECT_TRUE(frrs.empty());
    ASSERT_TRUE(dataPointRecord.topLevelJobUUID());
    dataPointJobUUID = dataPointRecord.topLevelJobUUID().get();
    EXPECT_NO_THROW(runManager.getJob(dataPointJobUUID));

  }

  // Clear all results
  project.clearAllResults();
  project.save();

  // Check DakotaAlgorithm, DataPoint, and records for expected state
  EXPECT_FALSE(algorithm.isComplete());
  EXPECT_EQ(-1,algorithm.iter());
  EXPECT_FALSE(algorithm.failed());
  EXPECT_FALSE(algorithm.restartFileReference());
  EXPECT_FALSE(algorithm.outFileReference());
  EXPECT_FALSE(algorithm.job());

  ASSERT_EQ(1u,analysis.dataPoints().size());
  EXPECT_FALSE(dataPoint.isComplete());
  EXPECT_FALSE(dataPoint.failed());
  EXPECT_TRUE(dataPoint.responseValues().empty());
  EXPECT_TRUE(dataPoint.directory().empty());
  EXPECT_FALSE(dataPoint.osmInputData());
  EXPECT_FALSE(dataPoint.idfInputData());
  EXPECT_FALSE(dataPoint.sqlOutputData());
  EXPECT_TRUE(dataPoint.xmlOutputData().empty());
  EXPECT_FALSE(dataPoint.model());
  EXPECT_FALSE(dataPoint.workspace());
  EXPECT_FALSE(dataPoint.sqlFile());
  EXPECT_TRUE(dataPoint.outputAttributes().empty());
  EXPECT_FALSE(dataPoint.topLevelJob());

  {
    AnalysisRecord analysisRecord = project.analysisRecord();
    RunManager runManager = project.runManager();

    OptionalAlgorithmRecord algorithmRecord = analysisRecord.algorithmRecord();
    ASSERT_TRUE(algorithmRecord);
    ASSERT_TRUE(algorithmRecord.get().optionalCast<DakotaAlgorithmRecord>());
    DakotaAlgorithmRecord dakotaAlgorithmRecord = algorithmRecord->cast<DakotaAlgorithmRecord>();

    EXPECT_FALSE(dakotaAlgorithmRecord.isComplete());
    OptionalFileReferenceRecord frr = dakotaAlgorithmRecord.restartFileReferenceRecord();
    EXPECT_FALSE(frr);
    frr = dakotaAlgorithmRecord.outFileReferenceRecord();
    EXPECT_FALSE(frr);
    EXPECT_FALSE(dakotaAlgorithmRecord.jobUUID());
    EXPECT_ANY_THROW(runManager.getJob(dakotaJobUUID));

    DataPointRecordVector dataPointRecords = analysisRecord.dataPointRecords();
    ASSERT_EQ(1u,dataPointRecords.size());
    DataPointRecord dataPointRecord = dataPointRecords[0];

    EXPECT_FALSE(dataPointRecord.isComplete());
    EXPECT_FALSE(dataPointRecord.failed());
    EXPECT_TRUE(dataPointRecord.directory().empty());
    EXPECT_TRUE(dataPointRecord.responseValues().empty());
    frr = dataPointRecord.osmInputDataRecord();
    EXPECT_FALSE(frr);
    frr = dataPointRecord.idfInputDataRecord();
    EXPECT_FALSE(frr);
    frr = dataPointRecord.sqlOutputDataRecord();
    EXPECT_FALSE(frr);
    FileReferenceRecordVector frrs = dataPointRecord.xmlOutputDataRecords();
    EXPECT_TRUE(frrs.empty());
    EXPECT_FALSE(dataPointRecord.topLevelJobUUID());
    EXPECT_ANY_THROW(runManager.getJob(dataPointJobUUID));
  }
}
