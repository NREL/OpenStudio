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
#include "ProjectFixture.hpp"
#include "../TagRecord.hpp"
#include "../TagRecord_Impl.hpp"
#include "../AnalysisRecord.hpp"
#include "../DataPointRecord.hpp"

#include "../../analysis/Analysis.hpp"
#include "../../analysis/DataPoint.hpp"
#include "../../analysis/Problem.hpp"
#include "../../analysis/Variable.hpp"

#include "../../runmanager/lib/Workflow.hpp"

#include "../../utilities/data/Tag.hpp"
#include "../../utilities/core/Optional.hpp"
#include "../../utilities/core/FileReference.hpp"
#include "../../utilities/core/Finder.hpp"
#include "../../utilities/core/Containers.hpp"

using namespace openstudio;
using namespace openstudio::model;
using namespace openstudio::project;

TEST_F(ProjectFixture, DataPointTagRecords) {
  // create Analysis with one DataPoint with one Tag ("test tag")
  analysis::Problem problem("problem",analysis::VariableVector(),runmanager::Workflow());
  FileReference seed(toPath("in.osm"));
  analysis::Analysis analysis("analysis",problem,seed);
  analysis::DataPoint dataPoint = problem.createDataPoint(std::vector<QVariant>()).get();
  dataPoint.addTag("test tag");
  analysis.addDataPoint(dataPoint);

  // make sure tag records follow data points around.
  // trying to replicate issue where data point tag records get repointed to
  // file reference record with id 0.
  {
    // save analysis to a new database
    ProjectDatabase database = getCleanDatabase("DataPointTagRecords");
    AnalysisRecord analysisRecord(analysis,database);
    database.save();
    ASSERT_EQ(1u,analysisRecord.dataPointRecords().size());
    DataPointRecord dataPointRecord = analysisRecord.dataPointRecords()[0];
    ASSERT_EQ(1u,dataPointRecord.tagRecords().size());
    TagRecord tagRecord = dataPointRecord.tagRecords()[0];
    EXPECT_EQ("test tag",tagRecord.name());
    analysis::DataPoint dataPoint = dataPointRecord.dataPoint();
    ASSERT_EQ(1u,dataPoint.tags().size());
    EXPECT_EQ("test tag",dataPoint.tags()[0].name());
    analysis::Analysis analysis = analysisRecord.analysis();
    ASSERT_EQ(1u,analysis.dataPoints().size());
    dataPoint = analysis.dataPoints()[0];
    ASSERT_EQ(1u,dataPoint.tags().size());
    EXPECT_EQ("test tag",dataPoint.tags()[0].name());
  }

  NameFinder<TagRecord> testRecordFinder("test tag");
  NameFinder<TagRecord> anotherTestRecordFinder("another test");
  NameFinder<Tag> testTagFinder("test tag");
  NameFinder<Tag> anotherTestTagFinder("another test");

  {
    ProjectDatabase database = getExistingDatabase("DataPointTagRecords");
    ASSERT_EQ(1u,AnalysisRecord::getAnalysisRecords(database).size());
    AnalysisRecord analysisRecord = AnalysisRecord::getAnalysisRecords(database)[0];
    ASSERT_EQ(1u,analysisRecord.dataPointRecords().size());
    DataPointRecord dataPointRecord = analysisRecord.dataPointRecords()[0];
    ASSERT_EQ(1u,dataPointRecord.tagRecords().size());
    TagRecord tagRecord = dataPointRecord.tagRecords()[0];
    EXPECT_EQ("test tag",tagRecord.name());
    analysis::DataPoint dataPoint = dataPointRecord.dataPoint();
    ASSERT_EQ(1u,dataPoint.tags().size());
    EXPECT_EQ("test tag",dataPoint.tags()[0].name());
    analysis::Analysis analysis = analysisRecord.analysis();
    ASSERT_EQ(1u,analysis.dataPoints().size());
    dataPoint = analysis.dataPoints()[0];
    ASSERT_EQ(1u,dataPoint.tags().size());
    EXPECT_EQ("test tag",dataPoint.tags()[0].name());

    // adding a second tag and resaving the analysis
    dataPoint.addTag("another test");
    analysisRecord = AnalysisRecord(analysis,database);
    bool test = database.save();
    EXPECT_TRUE(test);
    ASSERT_EQ(1u,analysisRecord.dataPointRecords().size());
    dataPointRecord = analysisRecord.dataPointRecords()[0];
    TagRecordVector tagRecords = dataPointRecord.tagRecords();
    EXPECT_EQ(2u,tagRecords.size());
    EXPECT_FALSE(std::find_if(tagRecords.begin(),tagRecords.end(),testRecordFinder) == tagRecords.end());
    EXPECT_FALSE(std::find_if(tagRecords.begin(),tagRecords.end(),anotherTestRecordFinder) == tagRecords.end());
    dataPoint = dataPointRecord.dataPoint();
    TagVector tags = dataPoint.tags();
    EXPECT_EQ(2u,tags.size());
    EXPECT_FALSE(std::find_if(tags.begin(),tags.end(),testTagFinder) == tags.end());
    EXPECT_FALSE(std::find_if(tags.begin(),tags.end(),anotherTestTagFinder) == tags.end());
    analysis = analysisRecord.analysis();
    ASSERT_EQ(1u,analysis.dataPoints().size());
    dataPoint = analysis.dataPoints()[0];
    tags = dataPoint.tags();
    EXPECT_EQ(2u,tags.size());
    EXPECT_FALSE(std::find_if(tags.begin(),tags.end(),testTagFinder) == tags.end());
    EXPECT_FALSE(std::find_if(tags.begin(),tags.end(),anotherTestTagFinder) == tags.end());
    tagRecords = TagRecord::getTagRecords(database);
    EXPECT_EQ(2u,tagRecords.size());
    EXPECT_FALSE(std::find_if(tagRecords.begin(),tagRecords.end(),testRecordFinder) == tagRecords.end());
    EXPECT_FALSE(std::find_if(tagRecords.begin(),tagRecords.end(),anotherTestRecordFinder) == tagRecords.end());
  }

  {
    ProjectDatabase database = getExistingDatabase("DataPointTagRecords");
    ASSERT_EQ(1u,AnalysisRecord::getAnalysisRecords(database).size());
    AnalysisRecord analysisRecord = AnalysisRecord::getAnalysisRecords(database)[0];
    ASSERT_EQ(1u,analysisRecord.dataPointRecords().size());
    DataPointRecord dataPointRecord = analysisRecord.dataPointRecords()[0];
    TagRecordVector tagRecords = dataPointRecord.tagRecords();
    EXPECT_EQ(2u,tagRecords.size());
    EXPECT_FALSE(std::find_if(tagRecords.begin(),tagRecords.end(),testRecordFinder) == tagRecords.end());
    EXPECT_FALSE(std::find_if(tagRecords.begin(),tagRecords.end(),anotherTestRecordFinder) == tagRecords.end());
    dataPoint = dataPointRecord.dataPoint();
    TagVector tags = dataPoint.tags();
    EXPECT_FALSE(std::find_if(tags.begin(),tags.end(),testTagFinder) == tags.end());
    EXPECT_FALSE(std::find_if(tags.begin(),tags.end(),anotherTestTagFinder) == tags.end());
    analysis = analysisRecord.analysis();
    ASSERT_EQ(1u,analysis.dataPoints().size());
    dataPoint = analysis.dataPoints()[0];
    tags = dataPoint.tags();
    EXPECT_FALSE(std::find_if(tags.begin(),tags.end(),testTagFinder) == tags.end());
    EXPECT_FALSE(std::find_if(tags.begin(),tags.end(),anotherTestTagFinder) == tags.end());
  }

}
