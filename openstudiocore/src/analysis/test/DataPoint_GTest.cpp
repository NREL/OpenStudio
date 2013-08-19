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
#include <analysis/test/AnalysisFixture.hpp>

#include <analysis/Analysis.hpp>
#include <analysis/DataPoint.hpp>
#include <analysis/DataPoint_Impl.hpp>

#include <runmanager/lib/WorkItem.hpp>

#include <utilities/data/Attribute.hpp>

#include <utilities/time/Date.hpp>

#include <utilities/core/Containers.hpp>

#include <resources.hxx>
#include <OpenStudio.hxx>

using namespace openstudio;
using namespace openstudio::analysis;
using namespace openstudio::runmanager;
using namespace openstudio::ruleset;

TEST_F(AnalysisFixture, DataPoint_JSONSerialization_PreRun_Roundtrip) {
  // Right now, round-tripping only works with no server view attached.
  // Should be able to relax this once Analysis has a method to update its
  // DataPoints from JSON.
  DataPointSerializationOptions options(openstudio::path(),false);

  Analysis analysis = analysis1(PreRun);

  // Retrieve data point
  ASSERT_FALSE(analysis.dataPoints().empty());
  DataPoint dataPoint = analysis.dataPoints()[0];

  // Serialize data point
  std::string json = dataPoint.toJSON(options);
  EXPECT_FALSE(json.empty());

  // Deserialize and check results
  OptionalAnalysisObject copyAsAnalysisObject = loadJSON(json);
  ASSERT_TRUE(copyAsAnalysisObject);
  ASSERT_TRUE(copyAsAnalysisObject->optionalCast<DataPoint>());
  DataPoint copy = copyAsAnalysisObject->cast<DataPoint>();
  EXPECT_EQ(json,copy.toJSON(options));

  // Save data point
  openstudio::path p = toPath("AnalysisFixtureData/data_point_pre_run.json");
  EXPECT_TRUE(dataPoint.saveJSON(p,options,true));

  // Load and check results
  copyAsAnalysisObject = loadJSON(p);
  ASSERT_TRUE(copyAsAnalysisObject);
  ASSERT_TRUE(copyAsAnalysisObject->optionalCast<DataPoint>());
  copy = copyAsAnalysisObject->cast<DataPoint>();
  EXPECT_EQ(json,copy.toJSON(options));

}

TEST_F(AnalysisFixture, DataPoint_JSONSerialization_PostRun_Roundtrip) {
  // Right now, round-tripping only works with no server view attached.
  // Should be able to relax this once Analysis has a method to update its
  // DataPoints from JSON.
  DataPointSerializationOptions options(openstudio::path(),false);

  // Create analysis
  Analysis analysis = analysis1(PostRun);

  // Retrieve "simulated" data point
  ASSERT_FALSE(analysis.dataPoints().empty());
  DataPoint dataPoint = analysis.dataPoints()[0];

  // Serialize data point
  std::string json = dataPoint.toJSON(options);
  EXPECT_FALSE(json.empty());

  // Deserialize and check results
  OptionalAnalysisObject copyAsAnalysisObject = loadJSON(json);
  ASSERT_TRUE(copyAsAnalysisObject);
  ASSERT_TRUE(copyAsAnalysisObject->optionalCast<DataPoint>());
  DataPoint copy = copyAsAnalysisObject->cast<DataPoint>();
  EXPECT_EQ(json,copy.toJSON(options));

  // Save data point
  openstudio::path p = toPath("AnalysisFixtureData/data_point_post_run.json");
  EXPECT_TRUE(dataPoint.saveJSON(p,options,true));

  // Load and check results
  copyAsAnalysisObject = loadJSON(p);
  ASSERT_TRUE(copyAsAnalysisObject);
  ASSERT_TRUE(copyAsAnalysisObject->optionalCast<DataPoint>());
  copy = copyAsAnalysisObject->cast<DataPoint>();
  EXPECT_EQ(json,copy.toJSON(options));
}

TEST_F(AnalysisFixture, DataPoint_JSONSerialization_Versioning) {
  openstudio::path dir = resourcesPath() / toPath("analysis/version");
  
  // create this version's json file
  DataPoint dataPoint = analysis1(PostRun).dataPoints()[0];
  std::string dashVersionString = boost::regex_replace(openStudioVersion(),boost::regex("\\."),"-");
  openstudio::path p = dir / toPath("data_point_" + dashVersionString + ".json");
  bool ok = dataPoint.saveJSON(p,DataPointSerializationOptions(),true);
  EXPECT_TRUE(ok);

  // loop through all versions' json files
  for (openstudio::directory_iterator it(dir); it != openstudio::directory_iterator(); ++it) {
    if (boost::regex_match(toString(it->path().stem()),boost::regex("data_point_.*"))) {
      LOG(Debug,"Loading " << toString(it->filename()) << ".");

      // open and check results
      OptionalAnalysisObject analysisObject = loadJSON(it->path());
      ASSERT_TRUE(analysisObject);
      ASSERT_TRUE(analysisObject->optionalCast<DataPoint>());
      DataPoint loaded = analysisObject->cast<DataPoint>();

      EXPECT_TRUE(loaded.topLevelJob());
    }
  }
}
