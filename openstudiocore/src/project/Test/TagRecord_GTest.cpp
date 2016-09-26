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
