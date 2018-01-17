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

#include "../../analysis/Analysis.hpp"
#include "../../analysis/DataPoint.hpp"
#include "../../analysis/DataPoint_Impl.hpp"
#include "../../analysis/DesignOfExperiments.hpp"
#include "../../analysis/DesignOfExperimentsOptions.hpp"
#include "../../analysis/Problem.hpp"
#include "../../analysis/Variable.hpp"

#include "../AnalysisRecord.hpp"
#include "../DataPointRecord.hpp"
#include "../ProjectDatabase.hpp"
#include "ProjectFixture.hpp"

#include "../../runmanager/lib/Workflow.hpp"

#include "../../utilities/core/FileReference.hpp"

#include <QVariant>

using namespace openstudio;
using namespace openstudio::analysis;
using namespace openstudio::project;

TEST_F(ProjectFixture,DataPoint_DataPointRecord_Synchronization) {

  // create analysis with one data point
  ProjectDatabase database = getCleanDatabase("DataPoint_DataPointRecord_Synchronization");

  analysis::Analysis analysis(
      "Test Analysis",
      analysis::Problem("Test Problem",analysis::VariableVector(),runmanager::Workflow()),
      analysis::DesignOfExperiments(analysis::DesignOfExperimentsOptions(
          analysis::DesignOfExperimentsType::FullFactorial)),
      FileReference(toPath("in.osm")));

  std::vector<QVariant> variableValues;
  analysis::DataPoint original = analysis.problem().createDataPoint(variableValues).get();
  analysis.addDataPoint(original);

  // save analysis to database
  {
    bool didStartTransaction = database.startTransaction();
    AnalysisRecord analysisRecord(analysis,database);

    DataPointRecordVector dataPointRecords = DataPointRecord::getDataPointRecords(database);
    ASSERT_EQ(1u,dataPointRecords.size());
    DataPointRecord dataPointRecord = dataPointRecords[0];
    EXPECT_FALSE(dataPointRecord.isComplete());

    database.save();
    if (didStartTransaction) {
      database.commitTransaction();
    }
  }

  // reload analysis, change data point, save back to database
  // verify that change shows up in database
  OptionalAnalysis loadedAnalysis;
  {
    AnalysisRecordVector loadedAnalysisRecords = AnalysisRecord::getAnalysisRecords(database);
    ASSERT_EQ(1u,loadedAnalysisRecords.size());
    loadedAnalysis = loadedAnalysisRecords[0].analysis();

    analysis::DataPointVector loadedDataPoints = loadedAnalysis->dataPoints();
    ASSERT_EQ(1u,loadedDataPoints.size());
    loadedDataPoints[0].getImpl<analysis::detail::DataPoint_Impl>()->markComplete();
    EXPECT_TRUE(loadedDataPoints[0].isComplete());
  }
  ASSERT_TRUE(loadedAnalysis);

  {
    bool didStartTransaction = database.startTransaction();
    database.unloadUnusedCleanRecords();
    AnalysisRecord analysisRecord(*loadedAnalysis,database);
    database.save();
    if (didStartTransaction) {
      database.commitTransaction();
    }
    EXPECT_EQ(1u,AnalysisRecord::getAnalysisRecords(database).size());

    DataPointRecordVector dataPointRecords = DataPointRecord::getDataPointRecords(database);
    ASSERT_EQ(1u,dataPointRecords.size());
    DataPointRecord dataPointRecord = dataPointRecords[0];
    EXPECT_TRUE(dataPointRecord.isComplete());
  }

}
