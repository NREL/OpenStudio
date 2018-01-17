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
