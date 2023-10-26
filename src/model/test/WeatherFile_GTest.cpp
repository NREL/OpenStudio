/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../WeatherFile.hpp"
#include "../WeatherFile_Impl.hpp"
#include "../YearDescription.hpp"
#include "../YearDescription_Impl.hpp"
#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../../utilities/filetypes/EpwFile.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, WeatherFile) {
  Model model;
  EXPECT_FALSE(model.getOptionalUniqueModelObject<WeatherFile>());
  auto weatherFile = model.getUniqueModelObject<WeatherFile>();
  EXPECT_TRUE(model.getOptionalUniqueModelObject<WeatherFile>());
  EXPECT_FALSE(weatherFile.startDateActualYear());
  EXPECT_FALSE(weatherFile.startDayOfWeek());
}

TEST_F(ModelFixture, WeatherFile_assign_TMY) {

  Model model;
  auto yd = model.getUniqueModelObject<openstudio::model::YearDescription>();
  EXPECT_FALSE(yd.calendarYear());
  EXPECT_EQ(2009, yd.assumedYear());

  auto epwPath = resourcesPath() / "utilities/Filetypes/USA_CO_Golden-NREL.724666_TMY3.epw";
  EpwFile epwFile(epwPath);
  EXPECT_TRUE(WeatherFile::setWeatherFile(model, epwFile));

  EXPECT_FALSE(epwFile.isActual());
  EXPECT_FALSE(epwFile.startDateActualYear());
  EXPECT_EQ(DayOfWeek{"Sunday"}, epwFile.startDayOfWeek());

  EXPECT_FALSE(yd.calendarYear());
  EXPECT_EQ(2006, yd.assumedYear());
  ASSERT_TRUE(model.weatherFile());

  auto weatherFile = model.weatherFile().get();
  // This is a TMY, so no actual start date
  EXPECT_FALSE(weatherFile.startDateActualYear());
  ASSERT_TRUE(weatherFile.startDayOfWeek());
  EXPECT_EQ(epwFile.startDayOfWeek(), weatherFile.startDayOfWeek().get());
}

TEST_F(ModelFixture, WeatherFile_assign_AMY) {

  Model model;
  auto yd = model.getUniqueModelObject<openstudio::model::YearDescription>();
  EXPECT_FALSE(yd.calendarYear());
  EXPECT_EQ(2009, yd.assumedYear());

  auto epwPath = resourcesPath() / "utilities/Filetypes/USA_CO_Golden-NREL.amy";
  EpwFile epwFile(epwPath);
  EXPECT_TRUE(WeatherFile::setWeatherFile(model, epwFile));

  EXPECT_TRUE(epwFile.isActual());
  ASSERT_TRUE(epwFile.startDateActualYear());
  EXPECT_EQ(DayOfWeek{"Friday"}, epwFile.startDayOfWeek());

  EXPECT_FALSE(yd.calendarYear());
  EXPECT_EQ(1999, yd.assumedYear());
  ASSERT_TRUE(model.weatherFile());

  auto weatherFile = model.weatherFile().get();
  // This is a TMY, so no actual start date
  ASSERT_TRUE(weatherFile.startDateActualYear());
  EXPECT_EQ(1999, weatherFile.startDateActualYear().get());

  ASSERT_TRUE(weatherFile.startDayOfWeek());
  EXPECT_EQ(epwFile.startDayOfWeek(), weatherFile.startDayOfWeek().get());
}

TEST_F(ModelFixture, prout) {

  // A TMY, with start day set to Tuesday (see DATA PERIODS line)
  std::string epwFileStringNoYear = R"(LOCATION,Denver Centennial  Golden   Nr,CO,USA,TMY3,724666,39.74,-105.18,-7.0,1829.0
DESIGN CONDITIONS,1,Climate Design Data 2009 ASHRAE Handbook,,Heating,12,-18.8,-15.5,-21.6,0.7,-10.9,-18.8,0.9,-7.5,12.2,3.9,10.9,3.8,3,340,Cooling,7,15.2,33,15.7,32,15.5,30.2,15.3,18.4,27.3,17.5,26.4,16.8,25.6,4.9,0,16.1,14.3,20.2,14.9,13.2,19.9,13.9,12.3,19.6,59.7,27.3,56.6,26.6,54,25.7,760,Extremes,11.1,9.5,8.4,22.9,-22.9,36.1,3.8,1.2,-25.7,37,-27.9,37.7,-30.1,38.3,-32.8,39.2
TYPICAL/EXTREME PERIODS,6,Summer - Week Nearest Max Temperature For Period,Extreme,7/ 6,7/12,Summer - Week Nearest Average Temperature For Period,Typical,7/27,8/ 2,Winter - Week Nearest Min Temperature For Period,Extreme,12/15,12/21,Winter - Week Nearest Average Temperature For Period,Typical,12/ 1,12/ 7,Autumn - Week Nearest Average Temperature For Period,Typical,10/20,10/26,Spring - Week Nearest Average Temperature For Period,Typical,4/26,5/ 2
GROUND TEMPERATURES,3,.5,,,,-0.60,1.34,5.12,8.69,15.46,19.02,20.00,18.20,14.02,8.83,3.71,0.32,2,,,,2.08,2.55,4.70,7.10,12.30,15.62,17.28,16.91,14.53,10.94,6.90,3.72,4,,,,4.84,4.51,5.45,6.81,10.25,12.82,14.49,14.90,13.86,11.74,9.00,6.53
HOLIDAYS/DAYLIGHT SAVINGS,No,0,0,0
COMMENTS 1,Custom/User Format -- WMO#724666; NREL TMY Data Set (2008); Period of Record 1973-2005 (Generally)
COMMENTS 2, -- Ground temps produced with a standard soil diffusivity of 2.3225760E-03 {m**2/day}
DATA PERIODS,1,1,Data,Tuesday, 1/ 1,1/1
1999,1,1,1,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9*9*9,-3.0,-4.0,92,80600,0,0,257,0,0,0,0,0,0,0,0,0.0,9,8,16.1,3300,9,999999999,89,0.0310,0,88,0.330,999.0,99.0
2005,1,1,2,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9*9*9,-3.0,-6.0,77,80600,0,0,261,0,0,0,0,0,0,0,170,2.1,10,9,16.1,3000,9,999999999,89,0.0310,0,88,0.330,999.0,99.0
1999,1,1,3,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9*9*9,-4.0,-7.0,77,80700,0,0,251,0,0,0,0,0,0,0,250,1.5,9,8,16.1,3000,9,999999999,89,0.0310,0,88,0.330,999.0,99.0
1999,1,1,4,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9*9*9,-2.0,-5.0,78,80700,0,0,273,0,0,0,0,0,0,0,330,1.5,10,10,16.1,2700,9,999999999,89,0.0310,0,88,0.330,999.0,99.0
1999,1,1,5,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9?9?9,-2.0,-4.0,85,80800,0,0,274,0,0,0,0,0,0,0,240,3.6,10,10,12.9,1080,9,999999999,100,0.0310,0,88,0.330,0.0,1.0
1999,1,1,6,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9*9*9,-1.0,-2.0,92,80900,0,0,280,0,0,0,0,0,0,0,0,0.0,10,10,1.2,150,9,999999999,89,0.0310,0,88,0.330,999.0,99.0
1999,1,1,7,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9*9*9,-1.0,-1.0,100,81000,0,0,281,0,0,0,0,0,0,0,270,2.6,10,10,1.6,480,9,999999999,89,0.0310,0,88,0.330,999.0,99.0
1999,1,1,8,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9?9?9,-1.0,-1.0,100,81100,41,861,281,4,119,2,906,6725,559,79,260,2.1,10,10,1.6,480,9,999999999,89,0.0310,0,88,0.330,0.0,1.0
1999,1,1,9,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9*9*9,0.0,-1.0,92,81300,242,1415,272,178,480,95,17449,32960,11774,1860,240,2.6,9,8,6.4,1500,9,999999999,89,0.0310,0,88,0.330,999.0,99.0
1999,1,1,10,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9*9*9,2.0,-1.0,80,81500,429,1415,294,49,0,49,5871,0,5871,2108,280,2.6,10,10,16.1,1500,9,999999999,89,0.0310,0,88,0.330,999.0,99.0
1999,1,1,11,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9?9?9,2.0,-1.0,80,81500,563,1415,294,431,654,169,44097,62425,19149,3410,10,4.6,10,10,16.1,1500,9,999999999,89,0.0310,0,88,0.330,0.0,6.0
1999,1,1,12,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9*9*9,1.0,-2.0,79,81500,635,1415,275,326,296,193,34655,30037,21116,4256,10,7.2,8,8,16.1,1500,9,999999999,80,0.0310,0,88,0.330,999.0,99.0
1999,1,1,13,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9*9*9,3.0,-3.0,63,81600,640,1415,296,255,96,212,28023,9483,23716,5980,360,6.2,10,10,16.1,810,9,999999999,80,0.0310,0,88,0.330,999.0,99.0
1999,1,1,14,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9?9?9,2.0,-4.0,62,81600,577,1415,291,160,12,155,18096,900,17727,5938,20,7.2,10,10,16.1,960,9,999999999,80,0.0310,0,88,0.330,0.0,1.0
1999,1,1,15,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9?9?9,1.0,-4.0,67,81700,452,1415,287,59,0,59,7007,0,7007,2516,20,7.2,10,10,12.9,840,9,999999999,80,0.0310,0,88,0.330,0.0,1.0
1999,1,1,16,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9?9?9,1.0,-4.0,67,81700,272,1415,287,51,0,51,5821,0,5821,1885,360,5.1,10,10,16.1,1020,9,999999999,80,0.0310,0,88,0.330,0.0,1.0
1999,1,1,17,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9*9?9?9?9*9*9?9*9*9,0.0,-3.9,72,81700,62,1073,283,18,170,12,2390,7964,1886,227,10,5.4,10,10,16.1,885,9,999999999,80,0.0310,0,88,0.330,999.0,99.0
1999,1,1,18,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9?9?9,-1.0,-4.0,78,81800,0,0,278,0,0,0,0,0,0,0,10,5.7,10,10,12.9,750,9,999999999,80,0.0310,0,88,0.330,0.0,1.0
1999,1,1,19,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9*9*9,-2.0,-6.0,71,81800,0,0,272,0,0,0,0,0,0,0,360,5.1,10,10,14.5,750,9,999999999,69,0.0310,0,88,0.330,999.0,99.0
1999,1,1,20,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9?9?9,-4.0,-8.0,71,81800,0,0,262,0,0,0,0,0,0,0,20,8.2,10,10,14.5,480,9,999999999,69,0.0310,0,88,0.330,0.0,1.0
1999,1,1,21,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9?9?9,-5.0,-7.0,84,81800,0,0,260,0,0,0,0,0,0,0,10,5.1,10,10,2.8,330,9,999999999,69,0.0310,0,88,0.330,0.0,1.0
1999,1,1,22,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9?9?9,-6.0,-6.0,100,81800,0,0,257,0,0,0,0,0,0,0,10,3.6,10,10,1.2,210,9,999999999,69,0.0310,0,88,0.330,0.0,1.0
1999,1,1,23,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9*9*9,-7.0,-9.0,84,81800,0,0,250,0,0,0,0,0,0,0,30,6.7,10,10,1.2,150,9,999999999,69,0.0310,0,88,0.330,999.0,99.0
1999,1,1,24,0,?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9*9*9,-7.0,-9.0,84,81700,0,0,250,0,0,0,0,0,0,0,30,6.7,10,10,1.2,150,9,999999999,69,0.0310,0,88,0.330,999.0,99.0
)";

  boost::optional<EpwFile> epwFile_ = EpwFile::loadFromString(epwFileStringNoYear);
  ASSERT_TRUE(epwFile_);
  auto epwFile = epwFile_.get();

  Model model;
  auto yd = model.getUniqueModelObject<openstudio::model::YearDescription>();
  EXPECT_FALSE(yd.calendarYear());
  EXPECT_EQ(2009, yd.assumedYear());

  EXPECT_TRUE(WeatherFile::setWeatherFile(model, epwFile));

  EXPECT_FALSE(epwFile.isActual());
  EXPECT_FALSE(epwFile.startDateActualYear());
  EXPECT_EQ(DayOfWeek{"Tuesday"}, epwFile.startDayOfWeek());

  EXPECT_FALSE(yd.calendarYear());
  EXPECT_EQ(2002, yd.assumedYear());
  ASSERT_TRUE(model.weatherFile());

  auto weatherFile = model.weatherFile().get();
  // This is a TMY, so no actual start date
  EXPECT_FALSE(weatherFile.startDateActualYear());

  ASSERT_TRUE(weatherFile.startDayOfWeek());
  EXPECT_EQ(epwFile.startDayOfWeek(), weatherFile.startDayOfWeek().get());
}
