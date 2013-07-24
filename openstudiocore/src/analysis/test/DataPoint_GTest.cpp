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
#include <analysis/Problem.hpp>

#include <resources.hxx>

using namespace openstudio;
using namespace openstudio::analysis;

TEST_F(AnalysisFixture, DataPoint_JSONSerialization_PreRun) {
  Analysis analysis = analysis1();
  Problem problem = analysis.problem();

  // Create data point and add to analysis
  std::vector<QVariant> values;
  values.push_back(1);
  values.push_back(0.3);
  values.push_back(0.9);
  OptionalDataPoint dataPoint = problem.createDataPoint(values);
  ASSERT_TRUE(dataPoint);
  EXPECT_TRUE(analysis.addDataPoint(*dataPoint));

  // Serialize data point
  std::string json = dataPoint->toJSON();
  EXPECT_FALSE(json.empty());

  // Deserialize and check results

  // Save data point
  openstudio::path p = toPath("AnalysisFixtureData/data_point_pre_run.json");
  EXPECT_TRUE(dataPoint->saveJSON(p,true));

  // Load and check results

}

TEST_F(AnalysisFixture, DataPoint_JSONSerialization_PostRun) {
  // Create problem

  // Create "simulated" data point

  // Create analysis with this "simulated" data point

  // Serialize data point

  // Deserialize and check results

  // Save data point
  openstudio::path p = toPath("AnalysisFixtureData/data_point_post_run.json");
  // EXPECT_TRUE(dataPoint->saveJSON(p,true));

  // Load and check results

}
