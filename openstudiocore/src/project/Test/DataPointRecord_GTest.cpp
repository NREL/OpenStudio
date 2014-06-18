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
