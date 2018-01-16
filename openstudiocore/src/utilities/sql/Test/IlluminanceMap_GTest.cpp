/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "../SqlFile.hpp"
#include "../../core/Path.hpp"
#include "../../core/Application.hpp"
#include "../../core/FileLogSink.hpp"

#include <resources.hxx>


#include <boost/algorithm/string/predicate.hpp>

using namespace std;
using namespace boost;
using namespace openstudio;

///////////////////////////////////////////////////////////////////////////////
// *** BEGIN FIXTURE ***
///////////////////////////////////////////////////////////////////////////////

class IlluminanceMapFixture : public ::testing::Test {
protected:

  // initialize for each test
  virtual void SetUp() override {}

  // tear down after for each test
  virtual void TearDown() override {}

  // initialize static members
  static void SetUpTestCase()
  {
    logFile = FileLogSink(toPath("./IlluminanceMapFixture.log"));
    logFile->setLogLevel(Info);

    openstudio::path path;
    path = resourcesPath() / toPath("energyplus/Daylighting_School/eplusout.sql");
    sqlFile = SqlFile(path);
    ASSERT_TRUE(sqlFile.connectionOpen());
  }

  // tear down static members
  static void TearDownTestCase()
  {
    logFile->disable();
  }

  // set up logging
  REGISTER_LOGGER("IlluminanceMapFixture");

public:

  // sql files
  static SqlFile sqlFile;
  static boost::optional<openstudio::FileLogSink> logFile;
};

// define static storage
SqlFile IlluminanceMapFixture::sqlFile;
boost::optional<openstudio::FileLogSink> IlluminanceMapFixture::logFile;

///////////////////////////////////////////////////////////////////////////////
// *** BEGIN TESTS ***
///////////////////////////////////////////////////////////////////////////////

TEST_F(IlluminanceMapFixture, IlluminanceMapPlot)
{
  if (!Application::instance().hasApplication()){
    Application::instance().application(true);
  }
  if (Application::instance().hasGUI()){

    openstudio::DateTime dateTime(Date(MonthOfYear::Jul, 21), Time(0.5));

    const std::string& mapName = "CLASSROOM ILLUMINANCE MAP";
    // MapName in EnergyPlus 7.0: CLASSROOM ILLUMINANCE MAP at 0.76m Illuminance [lux] (Hourly)
    // MapName in EnergyPlus 7.1: CLASSROOM:Denver Centennial  Golden   Nr CO USA TMY3 WMO#=724666:CLASSROOM ILLUMINANCE MAP at 0.76m Illuminance [lux] (Hourly)

    std::vector<string> illuminanceMapNames = sqlFile.illuminanceMapNames();
    ASSERT_EQ(static_cast<unsigned>(1), illuminanceMapNames.size());
    ASSERT_TRUE(boost::algorithm::contains(illuminanceMapNames[0], ":" + mapName));

    Vector x = sqlFile.illuminanceMapX(mapName, dateTime);
    ASSERT_EQ(static_cast<unsigned>(9), x.size());

    Vector y = sqlFile.illuminanceMapY(mapName, dateTime);
    ASSERT_EQ(static_cast<unsigned>(9), y.size());

    // eventually will want to check these points against input file
    // these are currently negative from input file because
    // classroom zone has angle of relative north = 180 degrees
    EXPECT_NEAR(-8.2296, x[0], 0.00001);
    EXPECT_NEAR(-0.9144, x[8], 0.00001);
    EXPECT_NEAR(-8.2296, y[0], 0.00001);
    EXPECT_NEAR(-0.9144, y[8], 0.00001);

    Matrix v = sqlFile.illuminanceMap(mapName, dateTime);
    ASSERT_EQ(x.size(), v.size1());
    ASSERT_EQ(y.size(), v.size2());
  }
}

TEST_F(IlluminanceMapFixture, IlluminanceMapPlotMin)
{
  const std::string& mapName = "CLASSROOM ILLUMINANCE MAP";
  // yearly min and max
  boost::optional<double> minValue;
  minValue = sqlFile.illuminanceMapMinValue(mapName);
  ASSERT_TRUE(minValue);
  ASSERT_EQ(0,*minValue);
}

TEST_F(IlluminanceMapFixture, IlluminanceMapPlotMax)
{
  const std::string& mapName = "CLASSROOM ILLUMINANCE MAP";
  // yearly min and max
  boost::optional<double> maxValue;
  maxValue = sqlFile.illuminanceMapMaxValue(mapName);
  ASSERT_TRUE(maxValue);
  ASSERT_EQ(3648,*maxValue);
}

TEST_F(IlluminanceMapFixture, IlluminanceMapPlotMinMax)
{
  const std::string& mapName = "CLASSROOM ILLUMINANCE MAP";
  // yearly min and max
  double minValue = std::numeric_limits<double>::min();
  double maxValue = std::numeric_limits<double>::max();
  sqlFile.illuminanceMapMaxValue(mapName,minValue,maxValue);
  ASSERT_EQ(0,minValue);
  ASSERT_EQ(3648,maxValue);
}


TEST_F(IlluminanceMapFixture, IlluminanceMapPlotRefPts)
{
    const std::string& mapName = "CLASSROOM ILLUMINANCE MAP";

  // reference points
  boost::optional<std::string> refPt1, refPt2;
  refPt1 = sqlFile.illuminanceMapRefPt(mapName,1);
  EXPECT_EQ(::std::string("RefPt1=(-4.57:-2.29:0.76)"), *refPt1);
  refPt2 = sqlFile.illuminanceMapRefPt(mapName,2);
  EXPECT_EQ(::std::string("RefPt2=(-4.57:-6.86:0.76)"), *refPt2);
}


TEST_F(IlluminanceMapFixture, IlluminanceMapPlotSeriesCount)
{
  const std::string& mapName = "CLASSROOM ILLUMINANCE MAP";

  std::vector< std::pair<int, DateTime> > illuminanceMapReportIndicesDates;
  // list of hourly reports for the illuminance map
  illuminanceMapReportIndicesDates = sqlFile.illuminanceMapHourlyReportIndicesDates(mapName);

  EXPECT_EQ(4760u,illuminanceMapReportIndicesDates.size());
}


TEST_F(IlluminanceMapFixture, IlluminanceMapPlotSeries)
{
  if (!Application::instance().hasApplication()){
    Application::instance().application(true);
  }
  if (Application::instance().hasGUI()){

    const std::string& mapName = "CLASSROOM ILLUMINANCE MAP";

    std::vector< std::pair<int, DateTime> > illuminanceMapReportIndicesDates;
    // list of hourly reports for the illuminance map
    illuminanceMapReportIndicesDates = sqlFile.illuminanceMapHourlyReportIndicesDates(mapName);

    ASSERT_FALSE(illuminanceMapReportIndicesDates.empty());
  }
}

TEST_F(IlluminanceMapFixture, IlluminanceMapPlotSeriesOpt)
{
  if (!Application::instance().hasApplication()){
    Application::instance().application(true);
  }
  if (Application::instance().hasGUI()){

    const std::string& mapName = "CLASSROOM ILLUMINANCE MAP";

    std::vector< std::pair<int, DateTime> > illuminanceMapReportIndicesDates;
    // list of hourly reports for the illuminance map
    illuminanceMapReportIndicesDates = sqlFile.illuminanceMapHourlyReportIndicesDates(mapName);

    ASSERT_FALSE(illuminanceMapReportIndicesDates.empty());

    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> illuminance;

    sqlFile.illuminanceMap(illuminanceMapReportIndicesDates[0].first, x, y, illuminance);
  }
}

TEST_F(IlluminanceMapFixture, IlluminanceMapMatrixBaseline)
{
  if (!Application::instance().hasApplication()){
    Application::instance().application(true);
  }
  if (Application::instance().hasGUI()){

    Vector x(9);
    Vector y(9);

    Matrix m(9, 9);

    for (unsigned i = 0; i < 9; i++)
    {
      x(i) = i;
      y(i) = i;
    }
    for (unsigned i = 0; i < 9; i++){
      for (unsigned j = 0; j < 9; j++){
        m(i, j) = x(i)*y(8 - j);
      }
    }
  }
}


