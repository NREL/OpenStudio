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

#include "../SimpleProject.hpp"
#include "../AnalysisDriver.hpp"
#include "../CurrentAnalysis.hpp"
#include "../AnalysisRunOptions.hpp"

#include "../../project/ProjectDatabase.hpp"
#include "../../project/AnalysisRecord.hpp"
#include "../../project/ProblemRecord.hpp"
#include "../../project/DataPointRecord.hpp"

#include "../../analysis/Analysis.hpp"
#include "../../analysis/Problem.hpp"
#include "../../analysis/DesignOfExperiments.hpp"
#include "../../analysis/DesignOfExperimentsOptions.hpp"
#include "../../analysis/DataPoint.hpp"
#include "../../analysis/MeasureGroup.hpp"
#include "../../analysis/MeasureGroup_Impl.hpp"
#include "../../analysis/RubyMeasure.hpp"
#include "../../analysis/RubyMeasure_Impl.hpp"

#include "../../model/Model.hpp"

#include "../../runmanager/lib/RunManager.hpp"
#include "../../runmanager/lib/Workflow.hpp"

#include "../../utilities/data/Attribute.hpp"
#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/Path.hpp"
#include "../../utilities/core/Checksum.hpp"
#include "../../utilities/core/FileReference.hpp"

#include <resources.hxx>
#include <OpenStudio.hxx>

using namespace openstudio;
using namespace openstudio::model;
using namespace openstudio::ruleset;
using namespace openstudio::analysis;
using namespace openstudio::project;
using namespace openstudio::analysisdriver;

TEST_F(AnalysisDriverFixture, DesignOfExperiments_MeshAnalysis) {

  openstudio::path rubyLibDirPath = openstudio::toPath(rubyLibDir());

  // GET SIMPLE PROJECT
  SimpleProject project = getCleanSimpleProject("DesignOfExperiments_MeshAnalysis");
  Analysis analysis = project.analysis();

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
  DesignOfExperimentsOptions algOptions(DesignOfExperimentsType::FullFactorial);
  DesignOfExperiments algorithm(algOptions);
  analysis.setAlgorithm(algorithm);

  // RUN ANALYSIS
  AnalysisDriver driver = project.analysisDriver();
  AnalysisRunOptions runOptions = standardRunOptions(project.projectDir());
  driver.run(analysis,runOptions);
  EXPECT_TRUE(driver.waitForFinished());

  // CHECK RESULTS
  AnalysisRecord analysisRecord = project.analysisRecord();
  EXPECT_EQ(4,analysisRecord.problemRecord().combinatorialSize(true).get());
  EXPECT_EQ(4u, analysisRecord.dataPointRecords().size());
  for (const DataPointRecord& dataPointRecord : analysisRecord.dataPointRecords()) {
    EXPECT_TRUE(dataPointRecord.isComplete());
    EXPECT_FALSE(dataPointRecord.failed());
  }

  // get data points by measures and vice versa
  std::vector<DataPointRecord> testDataPoints;
  std::vector<QVariant> testVariableValues;

  // all data points are successful
  testDataPoints = analysisRecord.successfulDataPointRecords();
  EXPECT_EQ(4u,testDataPoints.size());

  // empty variableValues returns all data points
  testDataPoints = analysisRecord.getDataPointRecords(testVariableValues);
  EXPECT_EQ(4u, testDataPoints.size());

  // find the baseline
  testVariableValues.clear();
  testVariableValues.push_back(0);
  testVariableValues.push_back(0);
  EXPECT_EQ(2u, problem.variables().size());
  testDataPoints = analysisRecord.getDataPointRecords(testVariableValues);
  ASSERT_EQ(1u, testDataPoints.size());

  // find model with improved wall and roof
  testVariableValues.clear();
  testVariableValues.push_back(1);
  testVariableValues.push_back(1);
  testDataPoints = analysisRecord.getDataPointRecords(testVariableValues);
  ASSERT_EQ(1u, testDataPoints.size());
  DataPoint testDataPoint = testDataPoints[0].dataPoint();
  std::vector<OptionalMeasure> measures = analysis.problem().getMeasures(testVariableValues);
  ASSERT_EQ(2u,measures.size());
  ASSERT_TRUE(measures[0] && measures[1]);
  EXPECT_TRUE(measures[0]->uuid() == problem.variables()[0].cast<MeasureGroup>().measures(false)[1].uuid());
  EXPECT_TRUE(measures[1]->uuid() == problem.variables()[1].cast<MeasureGroup>().measures(false)[1].uuid());
  EXPECT_TRUE(measures[0]->optionalCast<RubyPerturbation>());
  EXPECT_TRUE(measures[1]->optionalCast<RubyPerturbation>());

  // find models with improved wall
  testVariableValues.clear();
  testVariableValues.push_back(1);
  testDataPoints = analysisRecord.getDataPointRecords(testVariableValues);
  ASSERT_EQ(2u, testDataPoints.size());

  // infeasible
  testVariableValues.clear();
  testVariableValues.push_back(0);
  testVariableValues.push_back(0);
  testVariableValues.push_back(0);
  testDataPoints = analysisRecord.getDataPointRecords(testVariableValues);
  ASSERT_EQ(0u, testDataPoints.size());
}

