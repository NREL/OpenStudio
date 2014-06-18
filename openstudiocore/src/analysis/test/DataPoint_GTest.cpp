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
#include "AnalysisFixture.hpp"

#include "../Analysis.hpp"
#include "../DataPoint.hpp"
#include "../DataPoint_Impl.hpp"

#include "../../runmanager/lib/WorkItem.hpp"

#include "../../utilities/data/Attribute.hpp"

#include "../../utilities/time/Date.hpp"

#include "../../utilities/core/Containers.hpp"

#include <resources.hxx>
#include <OpenStudio.hxx>

using namespace openstudio;
using namespace openstudio::analysis;
using namespace openstudio::runmanager;
using namespace openstudio::ruleset;

TEST_F(AnalysisFixture, DataPoint_JSONSerialization_PreRun_Roundtrip) {
  Analysis analysis = analysis1(PreRun);

  // Retrieve data point
  ASSERT_FALSE(analysis.dataPoints().empty());
  DataPoint dataPoint = analysis.dataPoints()[0];

  // Serialize data point
  std::string json = dataPoint.toJSON();
  EXPECT_FALSE(json.empty());

  // Deserialize and check results
  AnalysisJSONLoadResult loadResult = loadJSON(json);
  ASSERT_TRUE(loadResult.analysisObject);
  ASSERT_TRUE(loadResult.analysisObject->optionalCast<DataPoint>());
  DataPoint copy = loadResult.analysisObject->cast<DataPoint>();
  EXPECT_EQ(json,copy.toJSON());

  // Save data point
  openstudio::path p = toPath("AnalysisFixtureData/data_point_pre_run.json");
  EXPECT_TRUE(dataPoint.saveJSON(p,true));

  // Load and check results
  loadResult = loadJSON(json);
  ASSERT_TRUE(loadResult.analysisObject);
  ASSERT_TRUE(loadResult.analysisObject->optionalCast<DataPoint>());
  copy = loadResult.analysisObject->cast<DataPoint>();
  EXPECT_EQ(json,copy.toJSON());
}

TEST_F(AnalysisFixture, DataPoint_JSONSerialization_PostRun_Roundtrip) {
  // Create analysis
  Analysis analysis = analysis1(PostRun);

  // Retrieve "simulated" data point
  ASSERT_FALSE(analysis.dataPoints().empty());
  DataPoint dataPoint = analysis.dataPoints()[0];

  // Serialize data point
  std::string json = dataPoint.toJSON();
  EXPECT_FALSE(json.empty());

  // Deserialize and check results
  AnalysisJSONLoadResult loadResult = loadJSON(json);
  ASSERT_TRUE(loadResult.analysisObject);
  ASSERT_TRUE(loadResult.analysisObject->optionalCast<DataPoint>());
  DataPoint copy = loadResult.analysisObject->cast<DataPoint>();
  EXPECT_EQ(json,copy.toJSON());

  // Save data point
  openstudio::path p = toPath("AnalysisFixtureData/data_point_post_run.json");
  EXPECT_TRUE(dataPoint.saveJSON(p,true));

  // Load and check results
  loadResult = loadJSON(json);
  ASSERT_TRUE(loadResult.analysisObject);
  ASSERT_TRUE(loadResult.analysisObject->optionalCast<DataPoint>());
  copy = loadResult.analysisObject->cast<DataPoint>();
  EXPECT_EQ(json,copy.toJSON());
  if (copy.toJSON() != json) {
    p = toPath("AnalysisFixtureData/data_point_post_run_roundtripped.json");
    copy.saveJSON(p,true);
  }

}

TEST_F(AnalysisFixture, DataPoint_JSONSerialization_Versioning) {
  openstudio::path dir = resourcesPath() / toPath("analysis/version");
  
  // create this version's json file
  DataPoint dataPoint = analysis1(PostRun).dataPoints()[0];
  std::string dashVersionString = boost::regex_replace(openStudioVersion(),boost::regex("\\."),"-");
  openstudio::path p = dir / toPath("data_point_" + dashVersionString + ".json");
  bool ok = dataPoint.saveJSON(p,true);
  EXPECT_TRUE(ok);

  // loop through all versions' json files
  for (boost::filesystem::directory_iterator it(dir); it != boost::filesystem::directory_iterator(); ++it) {
    if (boost::regex_match(toString(it->path().stem()),boost::regex("data_point_.*"))) {
      LOG(Debug,"Loading " << toString(it->path().filename()) << ".");

      // open and check results
      AnalysisJSONLoadResult loadResult = loadJSON(it->path());
      ASSERT_TRUE(loadResult.analysisObject);
      ASSERT_TRUE(loadResult.analysisObject->optionalCast<DataPoint>());
      DataPoint loaded = loadResult.analysisObject->cast<DataPoint>();

      EXPECT_TRUE(loaded.topLevelJob());
    }
  }
}

TEST_F(AnalysisFixture, DataPoint_Selected) {
  // Create analysis
  Analysis analysis = analysis1(PreRun);

  // See how many to queue
  unsigned totalToRun = analysis.dataPointsToQueue().size();
  ASSERT_LT(0u,totalToRun);

  // Turn one off
  ASSERT_FALSE(analysis.dataPoints().empty());
  EXPECT_EQ(totalToRun,analysis.dataPoints().size());
  DataPoint dataPoint = analysis.dataPoints()[0];
  dataPoint.setSelected(false);
  EXPECT_FALSE(dataPoint.selected());

  // Make sure shows up in "ToQueue"
  EXPECT_EQ(totalToRun - 1u,analysis.dataPointsToQueue().size());
}
