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

#include "../SqlFile.hpp"
#include "../../core/Path.hpp"
#include "../../plot/FloodPlot.hpp"
#include "../../core/FileLogSink.hpp"

#include <resources.hxx>

#include <boost/filesystem.hpp>
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
  virtual void SetUp() {}

  // tear down after for each test
  virtual void TearDown() {}

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

  MatrixFloodPlotData::Ptr data = MatrixFloodPlotData::create(x,y,v);
  data->interpMethod(LinearInterp);

  FloodPlot::Ptr fp = FloodPlot::create();
  fp->floodPlotData(data);
  fp->showContour(true);
  fp->generateImage(toPath("testIlluminanceMapPlot.png"));

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
    const std::string& mapName = "CLASSROOM ILLUMINANCE MAP";

  std::vector< std::pair<int, DateTime> > illuminanceMapReportIndicesDates;
  // list of hourly reports for the illuminance map
  illuminanceMapReportIndicesDates = sqlFile.illuminanceMapHourlyReportIndicesDates(mapName);

  ASSERT_FALSE(illuminanceMapReportIndicesDates.empty());

  openstudio::MatrixFloodPlotData::Ptr data = openstudio::MatrixFloodPlotData::create(
    sqlFile.illuminanceMapX(illuminanceMapReportIndicesDates[0].first),
    sqlFile.illuminanceMapY(illuminanceMapReportIndicesDates[0].first),
    sqlFile.illuminanceMap(illuminanceMapReportIndicesDates[0].first),
    openstudio::LinearInterp);

  FloodPlot::Ptr fp = FloodPlot::create();
  fp->floodPlotData(data);
  fp->showContour(true);
  fp->generateImage(toPath("testIlluminanceMapPlotSeries.png"));
}

TEST_F(IlluminanceMapFixture, IlluminanceMapPlotSeriesOpt)
{
    const std::string& mapName = "CLASSROOM ILLUMINANCE MAP";

  std::vector< std::pair<int, DateTime> > illuminanceMapReportIndicesDates;
  // list of hourly reports for the illuminance map
  illuminanceMapReportIndicesDates = sqlFile.illuminanceMapHourlyReportIndicesDates(mapName);
  
  ASSERT_FALSE(illuminanceMapReportIndicesDates.empty());

  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> illuminance;

  sqlFile.illuminanceMap(illuminanceMapReportIndicesDates[0].first,x,y,illuminance);

  openstudio::MatrixFloodPlotData::Ptr data = openstudio::MatrixFloodPlotData::create(x,y,illuminance,openstudio::LinearInterp);

  FloodPlot::Ptr fp = FloodPlot::create();
  fp->floodPlotData(data);
  fp->showContour(true);
  fp->generateImage(toPath("testIlluminanceMapPlotSeriesOpt.png"));
}

TEST_F(IlluminanceMapFixture, IlluminanceMapMatrixBaseline)
{
  Vector x(9); 
  Vector y(9); 

  Matrix m(9,9);

  for (unsigned i=0;i<9;i++)
  {
    x(i)=i;
    y(i)=i;
  }
  for (unsigned i=0;i<9;i++)
    for (unsigned j=0;j<9;j++)
      m(i,j) = x(i)*y(8-j);

  openstudio::MatrixFloodPlotData::Ptr data = openstudio::MatrixFloodPlotData::create(
    x,y,m,openstudio::LinearInterp);

  FloodPlot::Ptr fp = FloodPlot::create();
  fp->floodPlotData(data);
  fp->showContour(true);
  fp->generateImage(toPath("testIlluminanceMapMatrixBaseline.png"));
}


