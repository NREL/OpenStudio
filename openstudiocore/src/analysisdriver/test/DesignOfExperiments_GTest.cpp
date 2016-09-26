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
  Model model = fastExampleModel();
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

