/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "../EpwFile.hpp"
#include "../../time/Time.hpp"
#include "../../time/Date.hpp"
#include "../../core/Checksum.hpp"

#include <resources.hxx>

using namespace openstudio;

TEST(Filetypes, EpwFile)
{
  try{
    // LOCATION,Climate Zone 1,CA,USA,CTZRV2,725945,40.80,-124.20,-8.0,13.0
    // DESIGN CONDITIONS,1,Climate Design Data 2009 ASHRAE Handbook,,Heating,12,-0.6,0.6,-4.1,2.7,5.3,-2.2,3.2,3.8,11.1,12.3,9.7,11.3,2,90,Cooling,8,6.5,21.6,15.2,19.9,15.1,18.7,14.6,16.8,19.5,16.1,18.6,15.5,17.6,4.3,320,15.9,11.4,17.5,15.1,10.8,16.8,14.4,10.3,16.2,47.3,19.8,45,18.5,43.3,17.6,1804,Extremes,9,8.2,7.4,20.8,-2.5,28.3,1.7,2.2,-3.7,29.8,-4.6,31.1,-5.6,32.3,-6.8,33.9
    // TYPICAL/EXTREME PERIODS,6,Summer - Week Nearest Max Temperature For Period,Extreme,7/ 8,7/14,Summer - Week Nearest Average Temperature For Period,Typical,8/12,8/18,Winter - Week Nearest Min Temperature For Period,Extreme,1/15,1/21,Winter - Week Nearest Average Temperature For Period,Typical,1/22,1/28,Autumn - Week Nearest Average Temperature For Period,Typical,11/26,12/ 2,Spring - Week Nearest Average Temperature For Period,Typical,5/27,6/ 2
    // GROUND TEMPERATURES,3,.5,,,,9.66,10.10,10.96,11.78,13.32,14.13,14.35,13.94,12.99,11.81,10.64,9.87,2,,,,10.27,10.38,10.87,11.41,12.60,13.35,13.73,13.65,13.10,12.29,11.37,10.64,4,,,,10.90,10.82,11.04,11.35,12.13,12.72,13.10,13.19,12.95,12.47,11.85,11.28
    // HOLIDAYS/DAYLIGHT SAVINGS,No,0,0,0
    // COMMENTS 1,California Climate Zone 01 Version 2;
    // COMMENTS 2, -- Ground temps produced with a standard soil diffusivity of 2.3225760E-03 {m**2/day}
    // DATA PERIODS,1,1,Data,Sunday, 1/ 1,12/31
    path p = resourcesPath() / toPath("utilities/Filetypes/USA_CO_Golden-NREL.724666_TMY3.epw");
    EpwFile epwFile(p);
    EXPECT_EQ(p, epwFile.path());
    EXPECT_EQ("BDF687C1", epwFile.checksum());
    EXPECT_EQ(openstudio::checksum(epwFile.path()), epwFile.checksum());
    EXPECT_EQ("Denver Centennial  Golden   Nr", epwFile.city());
    EXPECT_EQ("CO", epwFile.stateProvinceRegion());
    EXPECT_EQ("USA", epwFile.country());
    EXPECT_EQ("TMY3", epwFile.dataSource());
    EXPECT_EQ("724666", epwFile.wmoNumber());
    EXPECT_EQ(39.74, epwFile.latitude());
    EXPECT_EQ(-105.18, epwFile.longitude());
    EXPECT_EQ(-7, epwFile.timeZone());
    EXPECT_EQ(1829, epwFile.elevation());
    EXPECT_EQ(Time(0,1,0,0), epwFile.timeStep());
    EXPECT_EQ(DayOfWeek(DayOfWeek::Sunday), epwFile.startDayOfWeek());
    EXPECT_EQ(Date(MonthOfYear::Jan, 1), epwFile.startDate());
    EXPECT_EQ(Date(MonthOfYear::Dec, 31), epwFile.endDate());
    EXPECT_FALSE(epwFile.isActual());
  } catch (...) {
    ASSERT_TRUE(false);
  }
}

TEST(Filetypes, EpwFile_Data)
{
  try{
    path p = resourcesPath() / toPath("utilities/Filetypes/USA_CO_Golden-NREL.724666_TMY3.epw");
    EpwFile epwFile(p);
    EXPECT_EQ(p, epwFile.path());
    EXPECT_EQ("BDF687C1", epwFile.checksum());
    EXPECT_EQ(openstudio::checksum(epwFile.path()), epwFile.checksum());
    EXPECT_EQ("Denver Centennial  Golden   Nr", epwFile.city());
    EXPECT_EQ("CO", epwFile.stateProvinceRegion());
    EXPECT_EQ("USA", epwFile.country());
    EXPECT_EQ("TMY3", epwFile.dataSource());
    EXPECT_EQ("724666", epwFile.wmoNumber());
    EXPECT_EQ(39.74, epwFile.latitude());
    EXPECT_EQ(-105.18, epwFile.longitude());
    EXPECT_EQ(-7, epwFile.timeZone());
    EXPECT_EQ(1829, epwFile.elevation());
    EXPECT_EQ(Time(0,1,0,0), epwFile.timeStep());
    EXPECT_EQ(DayOfWeek(DayOfWeek::Sunday), epwFile.startDayOfWeek());
    EXPECT_EQ(Date(MonthOfYear::Jan, 1), epwFile.startDate());
    EXPECT_EQ(Date(MonthOfYear::Dec, 31), epwFile.endDate());
    // Up to here, everything should be the same as the first test. Now ask for the data
    std::vector<EpwDataPoint> data = epwFile.data();
    EXPECT_EQ(8760,data.size());
    // The last data point should be for the last hour 12/31/1996, with a dry bulb temp of 4C and pressure 81100
    openstudio::DateTime dateTime = data[8759].dateTime();
    EXPECT_EQ(1, dateTime.date().monthOfYear().value());
    EXPECT_EQ(1, dateTime.date().dayOfMonth());
    // Stopgap test until things are straightened out with years
    EXPECT_EQ(2010, dateTime.date().year());
    //EXPECT_EQ(1997, dateTime.date().year());
    EXPECT_EQ(0, dateTime.time().hours());
    EXPECT_EQ(0, dateTime.time().minutes());
    EXPECT_EQ(0, dateTime.time().seconds());
    EXPECT_EQ(4.0,data[8759].dryBulbTemperature().get());
    EXPECT_EQ(81100,data[8759].atmosphericStationPressure().get());
    // Try out the alternate access functions, dew point temperature should be -1C
    EXPECT_EQ(-1.0,data[8759].getFieldByName("Dew Point Temperature").get());
    EXPECT_EQ(-1.0,data[8759].getField(EpwDataField("Dew Point Temperature")).get());
    // The last data point should not have a liquid precipitation depth
    EXPECT_FALSE(data[8759].getFieldByName("Liquid Precipitation Depth"));
    // Get the data as strings
    std::vector<std::string> epwStrings = data[8759].toEpwStrings();
    ASSERT_EQ(35, epwStrings.size());
    std::vector<std::string> known = { "1996", "12", "31", "24", "0",
      "?9?9?9?9E0?9?9?9?9?9?9?9?9?9?9?9?9?9?9?9*9*9?9*9*9", "4.0", "-1.0",
      "69", "81100", "0", "0", "294", "0.000000", "0", "0", "0", "0", "0",
      "0", "130", "6.200000", "9", "9", "48.3", "7500", "9", "999999999",
      "60", "0.0310", "0", "88", "0.210", "999", "99" };
    for (unsigned i = 0; i < 35; i++) {
      EXPECT_EQ(known[i], epwStrings[i]);
    }
    // Get a time series
    boost::optional<openstudio::TimeSeries> series = epwFile.getTimeSeries("Wind Speed");
    ASSERT_TRUE(series);
    ASSERT_EQ(8760,series->values().size());
    DateTimeVector seriesTimes = series->dateTimes();
    ASSERT_EQ(8760,seriesTimes.size());
    // Check the times in the data and the time series
    DateTime current(Date(1,1,1999),Time(0,1)); // Use 1999 to avoid leap years
    Time delta(0,1);
    for(unsigned i=0;i<8760;i++) {
      // This is a lot more complicated that it probably should be to avoid the year being a problem
      DateTime datatime = data[i].dateTime();
      EXPECT_EQ(datatime.date().monthOfYear(), current.date().monthOfYear());
      EXPECT_EQ(datatime.date().dayOfMonth(), current.date().dayOfMonth());
      EXPECT_EQ(datatime.time().hours(), current.time().hours());
      EXPECT_EQ(datatime.time().minutes(), current.time().minutes());
      DateTime seriestime = seriesTimes[i];
      EXPECT_EQ(seriestime.date().monthOfYear(), current.date().monthOfYear());
      EXPECT_EQ(seriestime.date().dayOfMonth(), current.date().dayOfMonth());
      EXPECT_EQ(seriestime.time().hours(), current.time().hours());
      EXPECT_EQ(seriestime.time().minutes(), current.time().minutes());
      current += delta;
    }
    // We should redo the original tests because we have reparsed the entire file
    EXPECT_EQ(p, epwFile.path());
    EXPECT_EQ("BDF687C1", epwFile.checksum());
    EXPECT_EQ(openstudio::checksum(epwFile.path()), epwFile.checksum());
    EXPECT_EQ("Denver Centennial  Golden   Nr", epwFile.city());
    EXPECT_EQ("CO", epwFile.stateProvinceRegion());
    EXPECT_EQ("USA", epwFile.country());
    EXPECT_EQ("TMY3", epwFile.dataSource());
    EXPECT_EQ("724666", epwFile.wmoNumber());
    EXPECT_EQ(39.74, epwFile.latitude());
    EXPECT_EQ(-105.18, epwFile.longitude());
    EXPECT_EQ(-7, epwFile.timeZone());
    EXPECT_EQ(1829, epwFile.elevation());
    EXPECT_EQ(Time(0,1,0,0), epwFile.timeStep());
    EXPECT_EQ(DayOfWeek(DayOfWeek::Sunday), epwFile.startDayOfWeek());
    EXPECT_EQ(Date(MonthOfYear::Jan, 1), epwFile.startDate());
    EXPECT_EQ(Date(MonthOfYear::Dec, 31), epwFile.endDate());
    EXPECT_FALSE(epwFile.isActual());
  } catch (...) {
    ASSERT_TRUE(false);
  }
}

TEST(Filetypes, EpwFile_Design)
{
  try{
    path p = resourcesPath() / toPath("utilities/Filetypes/USA_CO_Golden-NREL.724666_TMY3.epw");
    EpwFile epwFile(p);
    EXPECT_EQ(p, epwFile.path());
    EXPECT_EQ("BDF687C1", epwFile.checksum());
    EXPECT_EQ(openstudio::checksum(epwFile.path()), epwFile.checksum());
    EXPECT_EQ("Denver Centennial  Golden   Nr", epwFile.city());
    EXPECT_EQ("CO", epwFile.stateProvinceRegion());
    EXPECT_EQ("USA", epwFile.country());
    EXPECT_EQ("TMY3", epwFile.dataSource());
    EXPECT_EQ("724666", epwFile.wmoNumber());
    EXPECT_EQ(39.74, epwFile.latitude());
    EXPECT_EQ(-105.18, epwFile.longitude());
    EXPECT_EQ(-7, epwFile.timeZone());
    EXPECT_EQ(1829, epwFile.elevation());
    EXPECT_EQ(Time(0, 1, 0, 0), epwFile.timeStep());
    EXPECT_EQ(DayOfWeek(DayOfWeek::Sunday), epwFile.startDayOfWeek());
    EXPECT_EQ(Date(MonthOfYear::Jan, 1), epwFile.startDate());
    EXPECT_EQ(Date(MonthOfYear::Dec, 31), epwFile.endDate());
    // Up to here, everything should be the same as the first test. Now ask for the design conditions
    std::vector<EpwDesignCondition> designs = epwFile.designConditions();
    EXPECT_EQ(1, designs.size());
    EXPECT_EQ("Climate Design Data 2009 ASHRAE Handbook", designs[0].titleOfDesignCondition());
    EXPECT_EQ(12, designs[0].heatingColdestMonth());
    EXPECT_EQ(-18.8, designs[0].heatingDryBulb99pt6());
    EXPECT_EQ(-18.8, designs[0].getFieldByName("Heating Dry Bulb Temperature 99.6%").get());
    EXPECT_EQ(-18.8, designs[0].getField(EpwDesignField("Heating Dry Bulb Temperature 99.6%")).get());
    EXPECT_EQ("C", designs[0].getUnitsByName("Heating Dry Bulb Temperature 99.6%").get());
    EXPECT_EQ("C", designs[0].getUnits(EpwDesignField("Heating Dry Bulb Temperature 99.6%")));
    EXPECT_EQ(-15.5, designs[0].heatingDryBulb99());
    EXPECT_EQ(-21.6, designs[0].heatingHumidificationDewPoint99pt6());
    EXPECT_EQ(0.7, designs[0].heatingHumidificationHumidityRatio99pt6());
    EXPECT_EQ(-10.9, designs[0].heatingHumidificationMeanCoincidentDryBulb99pt6());
    EXPECT_EQ(-18.8, designs[0].heatingHumidificationDewPoint99());
    EXPECT_EQ(0.9, designs[0].heatingHumidificationHumidityRatio99());
    EXPECT_EQ(-7.5, designs[0].heatingHumidificationMeanCoincidentDryBulb99());
    EXPECT_EQ(12.2, designs[0].heatingColdestMonthWindSpeed0pt4());
    EXPECT_EQ(3.9, designs[0].heatingColdestMonthMeanCoincidentDryBulb0pt4());
    EXPECT_EQ(10.9, designs[0].heatingColdestMonthWindSpeed1());
    EXPECT_EQ(3.8, designs[0].heatingColdestMonthMeanCoincidentDryBulb1());
    EXPECT_EQ(3.0, designs[0].heatingMeanCoincidentWindSpeed99pt6());
    EXPECT_EQ(340, designs[0].heatingPrevailingCoincidentWindDirection99pt6());
    EXPECT_EQ(7, designs[0].coolingHottestMonth());
    EXPECT_EQ(15.2, designs[0].coolingDryBulbRange());
    EXPECT_EQ(33.0, designs[0].coolingDryBulb0pt4());
    EXPECT_EQ(15.7, designs[0].coolingMeanCoincidentWetBulb0pt4());
    EXPECT_EQ(32.0, designs[0].coolingDryBulb1());
    EXPECT_EQ(15.5, designs[0].coolingMeanCoincidentWetBulb1());
    EXPECT_EQ(30.2, designs[0].coolingDryBulb2());
    EXPECT_EQ(15.3, designs[0].coolingMeanCoincidentWetBulb2());
    EXPECT_EQ(18.4, designs[0].coolingEvaporationWetBulb0pt4());
    EXPECT_EQ(27.3, designs[0].coolingEvaporationMeanCoincidentDryBulb0pt4());
    EXPECT_EQ(17.5, designs[0].coolingEvaporationWetBulb1());
    EXPECT_EQ(26.4, designs[0].coolingEvaporationMeanCoincidentDryBulb1());
    EXPECT_EQ(16.8, designs[0].coolingEvaporationWetBulb2());
    EXPECT_EQ(25.6, designs[0].coolingEvaporationMeanCoincidentDryBulb2());
    EXPECT_EQ(4.9, designs[0].coolingMeanCoincidentWindSpeed0pt4());
    EXPECT_EQ(0, designs[0].coolingPrevailingCoincidentWindDirection0pt4());
    EXPECT_EQ(16.1, designs[0].coolingDehumidificationDewPoint0pt4());
    EXPECT_EQ(14.3, designs[0].coolingDehumidificationHumidityRatio0pt4());
    EXPECT_EQ(20.2, designs[0].coolingDehumidificationMeanCoincidentDryBulb0pt4());
    EXPECT_EQ(14.9, designs[0].coolingDehumidificationDewPoint1());
    EXPECT_EQ(13.2, designs[0].coolingDehumidificationHumidityRatio1());
    EXPECT_EQ(19.9, designs[0].coolingDehumidificationMeanCoincidentDryBulb1());
    EXPECT_EQ(13.9, designs[0].coolingDehumidificationDewPoint2());
    EXPECT_EQ(12.3, designs[0].coolingDehumidificationHumidityRatio2());
    EXPECT_EQ(19.6, designs[0].coolingDehumidificationMeanCoincidentDryBulb2());
    EXPECT_EQ(59.7, designs[0].coolingEnthalpy0pt4());
    EXPECT_EQ(27.3, designs[0].coolingEnthalpyMeanCoincidentDryBulb0pt4());
    EXPECT_EQ(56.6, designs[0].coolingEnthalpy1());
    EXPECT_EQ(26.6, designs[0].coolingEnthalpyMeanCoincidentDryBulb1());
    EXPECT_EQ(54.0, designs[0].coolingEnthalpy2());
    EXPECT_EQ(25.7, designs[0].coolingEnthalpyMeanCoincidentDryBulb2());
    EXPECT_EQ(760, designs[0].coolingHours8To4AndDryBulb12pt8To20pt6());
    EXPECT_EQ(11.1, designs[0].extremeWindSpeed1());
    EXPECT_EQ(9.5, designs[0].extremeWindSpeed2pt5());
    EXPECT_EQ(8.4, designs[0].extremeWindSpeed5());
    EXPECT_EQ(22.9, designs[0].extremeMaxWetBulb());
    EXPECT_EQ(-22.9, designs[0].extremeMeanMinDryBulb());
    EXPECT_EQ(36.1, designs[0].extremeMeanMaxDryBulb());
    EXPECT_EQ(3.8, designs[0].extremeStdDevMinDryBulb());
    EXPECT_EQ(1.2, designs[0].extremeStdDevMaxDryBulb());
    EXPECT_EQ(-25.7, designs[0].extremeN5YearsMinDryBulb());
    EXPECT_EQ(37.0, designs[0].extremeN5YearsMaxDryBulb());
    EXPECT_EQ(-27.9, designs[0].extremeN10YearsMinDryBulb());
    EXPECT_EQ(37.7, designs[0].extremeN10YearsMaxDryBulb());
    EXPECT_EQ(-30.1, designs[0].extremeN20YearsMinDryBulb());
    EXPECT_EQ(38.3, designs[0].extremeN20YearsMaxDryBulb());
    EXPECT_EQ(-32.8, designs[0].extremeN50YearsMinDryBulb());
    EXPECT_EQ(39.2, designs[0].extremeN50YearsMaxDryBulb());
  }
  catch (...) {
    ASSERT_TRUE(false);
  }
}

TEST(Filetypes, EpwFile_Design_DoubleRead)
{
  try {
    path p = resourcesPath() / toPath("utilities/Filetypes/USA_CO_Golden-NREL.724666_TMY3.epw");
    EpwFile epwFile(p);
    EXPECT_EQ(p, epwFile.path());
    EXPECT_EQ("BDF687C1", epwFile.checksum());
    EXPECT_EQ(openstudio::checksum(epwFile.path()), epwFile.checksum());
    EXPECT_EQ("Denver Centennial  Golden   Nr", epwFile.city());
    EXPECT_EQ("CO", epwFile.stateProvinceRegion());
    EXPECT_EQ("USA", epwFile.country());
    EXPECT_EQ("TMY3", epwFile.dataSource());
    EXPECT_EQ("724666", epwFile.wmoNumber());
    EXPECT_EQ(39.74, epwFile.latitude());
    EXPECT_EQ(-105.18, epwFile.longitude());
    EXPECT_EQ(-7, epwFile.timeZone());
    EXPECT_EQ(1829, epwFile.elevation());
    EXPECT_EQ(Time(0, 1, 0, 0), epwFile.timeStep());
    EXPECT_EQ(DayOfWeek(DayOfWeek::Sunday), epwFile.startDayOfWeek());
    EXPECT_EQ(Date(MonthOfYear::Jan, 1), epwFile.startDate());
    EXPECT_EQ(Date(MonthOfYear::Dec, 31), epwFile.endDate());
    // Ask for data
    std::vector<EpwDataPoint> data = epwFile.data();
    EXPECT_EQ(8760, data.size());
    // The last data point should be for the last hour 12/31/1996, with a dry bulb temp of 4C and pressure 81100
    openstudio::DateTime dateTime = data[8759].dateTime();
    EXPECT_EQ(1, dateTime.date().monthOfYear().value());
    EXPECT_EQ(1, dateTime.date().dayOfMonth());
    // Stopgap test until things are straightened out with years
    EXPECT_EQ(2010, dateTime.date().year());
    //EXPECT_EQ(1997, dateTime.date().year());
    EXPECT_EQ(0, dateTime.time().hours());
    EXPECT_EQ(0, dateTime.time().minutes());
    EXPECT_EQ(0, dateTime.time().seconds());
    EXPECT_EQ(4.0, data[8759].dryBulbTemperature().get());
    EXPECT_EQ(81100, data[8759].atmosphericStationPressure().get());
    // Try out the alternate access functions, dew point temperature should be -1C
    EXPECT_EQ(-1.0, data[8759].getFieldByName("Dew Point Temperature").get());
    EXPECT_EQ(-1.0, data[8759].getField(EpwDataField("Dew Point Temperature")).get());
    // The last data point should not have a liquid precipitation depth
    EXPECT_FALSE(data[8759].getFieldByName("Liquid Precipitation Depth"));
    // Ask for the design conditions
    std::vector<EpwDesignCondition> designs = epwFile.designConditions();
    EXPECT_EQ(1, designs.size());
    EXPECT_EQ("Climate Design Data 2009 ASHRAE Handbook", designs[0].titleOfDesignCondition());
    EXPECT_EQ(12, designs[0].heatingColdestMonth());
    EXPECT_EQ(-18.8, designs[0].heatingDryBulb99pt6());
    EXPECT_EQ(-18.8, designs[0].getFieldByName("Heating Dry Bulb Temperature 99.6%").get());
    EXPECT_EQ(-18.8, designs[0].getField(EpwDesignField("Heating Dry Bulb Temperature 99.6%")).get());
    EXPECT_EQ("C", designs[0].getUnitsByName("Heating Dry Bulb Temperature 99.6%").get());
    EXPECT_EQ("C", designs[0].getUnits(EpwDesignField("Heating Dry Bulb Temperature 99.6%")));
    EXPECT_EQ(-15.5, designs[0].heatingDryBulb99());
    EXPECT_EQ(-21.6, designs[0].heatingHumidificationDewPoint99pt6());
    EXPECT_EQ(0.7, designs[0].heatingHumidificationHumidityRatio99pt6());
    EXPECT_EQ(-10.9, designs[0].heatingHumidificationMeanCoincidentDryBulb99pt6());
    EXPECT_EQ(-18.8, designs[0].heatingHumidificationDewPoint99());
    EXPECT_EQ(0.9, designs[0].heatingHumidificationHumidityRatio99());
    EXPECT_EQ(-7.5, designs[0].heatingHumidificationMeanCoincidentDryBulb99());
    EXPECT_EQ(12.2, designs[0].heatingColdestMonthWindSpeed0pt4());
    EXPECT_EQ(3.9, designs[0].heatingColdestMonthMeanCoincidentDryBulb0pt4());
    EXPECT_EQ(10.9, designs[0].heatingColdestMonthWindSpeed1());
    EXPECT_EQ(3.8, designs[0].heatingColdestMonthMeanCoincidentDryBulb1());
    EXPECT_EQ(3.0, designs[0].heatingMeanCoincidentWindSpeed99pt6());
    EXPECT_EQ(340, designs[0].heatingPrevailingCoincidentWindDirection99pt6());
    EXPECT_EQ(7, designs[0].coolingHottestMonth());
    EXPECT_EQ(15.2, designs[0].coolingDryBulbRange());
    EXPECT_EQ(33.0, designs[0].coolingDryBulb0pt4());
    EXPECT_EQ(15.7, designs[0].coolingMeanCoincidentWetBulb0pt4());
    EXPECT_EQ(32.0, designs[0].coolingDryBulb1());
    EXPECT_EQ(15.5, designs[0].coolingMeanCoincidentWetBulb1());
    EXPECT_EQ(30.2, designs[0].coolingDryBulb2());
    EXPECT_EQ(15.3, designs[0].coolingMeanCoincidentWetBulb2());
    EXPECT_EQ(18.4, designs[0].coolingEvaporationWetBulb0pt4());
    EXPECT_EQ(27.3, designs[0].coolingEvaporationMeanCoincidentDryBulb0pt4());
    EXPECT_EQ(17.5, designs[0].coolingEvaporationWetBulb1());
    EXPECT_EQ(26.4, designs[0].coolingEvaporationMeanCoincidentDryBulb1());
    EXPECT_EQ(16.8, designs[0].coolingEvaporationWetBulb2());
    EXPECT_EQ(25.6, designs[0].coolingEvaporationMeanCoincidentDryBulb2());
    EXPECT_EQ(4.9, designs[0].coolingMeanCoincidentWindSpeed0pt4());
    EXPECT_EQ(0, designs[0].coolingPrevailingCoincidentWindDirection0pt4());
    EXPECT_EQ(16.1, designs[0].coolingDehumidificationDewPoint0pt4());
    EXPECT_EQ(14.3, designs[0].coolingDehumidificationHumidityRatio0pt4());
    EXPECT_EQ(20.2, designs[0].coolingDehumidificationMeanCoincidentDryBulb0pt4());
    EXPECT_EQ(14.9, designs[0].coolingDehumidificationDewPoint1());
    EXPECT_EQ(13.2, designs[0].coolingDehumidificationHumidityRatio1());
    EXPECT_EQ(19.9, designs[0].coolingDehumidificationMeanCoincidentDryBulb1());
    EXPECT_EQ(13.9, designs[0].coolingDehumidificationDewPoint2());
    EXPECT_EQ(12.3, designs[0].coolingDehumidificationHumidityRatio2());
    EXPECT_EQ(19.6, designs[0].coolingDehumidificationMeanCoincidentDryBulb2());
    EXPECT_EQ(59.7, designs[0].coolingEnthalpy0pt4());
    EXPECT_EQ(27.3, designs[0].coolingEnthalpyMeanCoincidentDryBulb0pt4());
    EXPECT_EQ(56.6, designs[0].coolingEnthalpy1());
    EXPECT_EQ(26.6, designs[0].coolingEnthalpyMeanCoincidentDryBulb1());
    EXPECT_EQ(54.0, designs[0].coolingEnthalpy2());
    EXPECT_EQ(25.7, designs[0].coolingEnthalpyMeanCoincidentDryBulb2());
    EXPECT_EQ(760, designs[0].coolingHours8To4AndDryBulb12pt8To20pt6());
    EXPECT_EQ(11.1, designs[0].extremeWindSpeed1());
    EXPECT_EQ(9.5, designs[0].extremeWindSpeed2pt5());
    EXPECT_EQ(8.4, designs[0].extremeWindSpeed5());
    EXPECT_EQ(22.9, designs[0].extremeMaxWetBulb());
    EXPECT_EQ(-22.9, designs[0].extremeMeanMinDryBulb());
    EXPECT_EQ(36.1, designs[0].extremeMeanMaxDryBulb());
    EXPECT_EQ(3.8, designs[0].extremeStdDevMinDryBulb());
    EXPECT_EQ(1.2, designs[0].extremeStdDevMaxDryBulb());
    EXPECT_EQ(-25.7, designs[0].extremeN5YearsMinDryBulb());
    EXPECT_EQ(37.0, designs[0].extremeN5YearsMaxDryBulb());
    EXPECT_EQ(-27.9, designs[0].extremeN10YearsMinDryBulb());
    EXPECT_EQ(37.7, designs[0].extremeN10YearsMaxDryBulb());
    EXPECT_EQ(-30.1, designs[0].extremeN20YearsMinDryBulb());
    EXPECT_EQ(38.3, designs[0].extremeN20YearsMaxDryBulb());
    EXPECT_EQ(-32.8, designs[0].extremeN50YearsMinDryBulb());
    EXPECT_EQ(39.2, designs[0].extremeN50YearsMaxDryBulb());
    // Check that the data is the right size
    data = epwFile.data();
    EXPECT_EQ(8760, data.size());
  } catch (...) {
    ASSERT_TRUE(false);
  }
}

TEST(Filetypes, EpwFile_Data_DoubleRead)
{
  try {
    path p = resourcesPath() / toPath("utilities/Filetypes/USA_CO_Golden-NREL.724666_TMY3.epw");
    EpwFile epwFile(p);
    EXPECT_EQ(p, epwFile.path());
    EXPECT_EQ("BDF687C1", epwFile.checksum());
    EXPECT_EQ(openstudio::checksum(epwFile.path()), epwFile.checksum());
    EXPECT_EQ("Denver Centennial  Golden   Nr", epwFile.city());
    EXPECT_EQ("CO", epwFile.stateProvinceRegion());
    EXPECT_EQ("USA", epwFile.country());
    EXPECT_EQ("TMY3", epwFile.dataSource());
    EXPECT_EQ("724666", epwFile.wmoNumber());
    EXPECT_EQ(39.74, epwFile.latitude());
    EXPECT_EQ(-105.18, epwFile.longitude());
    EXPECT_EQ(-7, epwFile.timeZone());
    EXPECT_EQ(1829, epwFile.elevation());
    EXPECT_EQ(Time(0, 1, 0, 0), epwFile.timeStep());
    EXPECT_EQ(DayOfWeek(DayOfWeek::Sunday), epwFile.startDayOfWeek());
    EXPECT_EQ(Date(MonthOfYear::Jan, 1), epwFile.startDate());
    EXPECT_EQ(Date(MonthOfYear::Dec, 31), epwFile.endDate());
    // Ask for the design conditions
    std::vector<EpwDesignCondition> designs = epwFile.designConditions();
    EXPECT_EQ(1, designs.size());
    EXPECT_EQ("Climate Design Data 2009 ASHRAE Handbook", designs[0].titleOfDesignCondition());
    EXPECT_EQ(12, designs[0].heatingColdestMonth());
    EXPECT_EQ(-18.8, designs[0].heatingDryBulb99pt6());
    EXPECT_EQ(-18.8, designs[0].getFieldByName("Heating Dry Bulb Temperature 99.6%").get());
    EXPECT_EQ(-18.8, designs[0].getField(EpwDesignField("Heating Dry Bulb Temperature 99.6%")).get());
    EXPECT_EQ("C", designs[0].getUnitsByName("Heating Dry Bulb Temperature 99.6%").get());
    EXPECT_EQ("C", designs[0].getUnits(EpwDesignField("Heating Dry Bulb Temperature 99.6%")));
    EXPECT_EQ(-15.5, designs[0].heatingDryBulb99());
    EXPECT_EQ(-21.6, designs[0].heatingHumidificationDewPoint99pt6());
    EXPECT_EQ(0.7, designs[0].heatingHumidificationHumidityRatio99pt6());
    EXPECT_EQ(-10.9, designs[0].heatingHumidificationMeanCoincidentDryBulb99pt6());
    EXPECT_EQ(-18.8, designs[0].heatingHumidificationDewPoint99());
    EXPECT_EQ(0.9, designs[0].heatingHumidificationHumidityRatio99());
    EXPECT_EQ(-7.5, designs[0].heatingHumidificationMeanCoincidentDryBulb99());
    EXPECT_EQ(12.2, designs[0].heatingColdestMonthWindSpeed0pt4());
    EXPECT_EQ(3.9, designs[0].heatingColdestMonthMeanCoincidentDryBulb0pt4());
    EXPECT_EQ(10.9, designs[0].heatingColdestMonthWindSpeed1());
    EXPECT_EQ(3.8, designs[0].heatingColdestMonthMeanCoincidentDryBulb1());
    EXPECT_EQ(3.0, designs[0].heatingMeanCoincidentWindSpeed99pt6());
    EXPECT_EQ(340, designs[0].heatingPrevailingCoincidentWindDirection99pt6());
    EXPECT_EQ(7, designs[0].coolingHottestMonth());
    EXPECT_EQ(15.2, designs[0].coolingDryBulbRange());
    EXPECT_EQ(33.0, designs[0].coolingDryBulb0pt4());
    EXPECT_EQ(15.7, designs[0].coolingMeanCoincidentWetBulb0pt4());
    EXPECT_EQ(32.0, designs[0].coolingDryBulb1());
    EXPECT_EQ(15.5, designs[0].coolingMeanCoincidentWetBulb1());
    EXPECT_EQ(30.2, designs[0].coolingDryBulb2());
    EXPECT_EQ(15.3, designs[0].coolingMeanCoincidentWetBulb2());
    EXPECT_EQ(18.4, designs[0].coolingEvaporationWetBulb0pt4());
    EXPECT_EQ(27.3, designs[0].coolingEvaporationMeanCoincidentDryBulb0pt4());
    EXPECT_EQ(17.5, designs[0].coolingEvaporationWetBulb1());
    EXPECT_EQ(26.4, designs[0].coolingEvaporationMeanCoincidentDryBulb1());
    EXPECT_EQ(16.8, designs[0].coolingEvaporationWetBulb2());
    EXPECT_EQ(25.6, designs[0].coolingEvaporationMeanCoincidentDryBulb2());
    EXPECT_EQ(4.9, designs[0].coolingMeanCoincidentWindSpeed0pt4());
    EXPECT_EQ(0, designs[0].coolingPrevailingCoincidentWindDirection0pt4());
    EXPECT_EQ(16.1, designs[0].coolingDehumidificationDewPoint0pt4());
    EXPECT_EQ(14.3, designs[0].coolingDehumidificationHumidityRatio0pt4());
    EXPECT_EQ(20.2, designs[0].coolingDehumidificationMeanCoincidentDryBulb0pt4());
    EXPECT_EQ(14.9, designs[0].coolingDehumidificationDewPoint1());
    EXPECT_EQ(13.2, designs[0].coolingDehumidificationHumidityRatio1());
    EXPECT_EQ(19.9, designs[0].coolingDehumidificationMeanCoincidentDryBulb1());
    EXPECT_EQ(13.9, designs[0].coolingDehumidificationDewPoint2());
    EXPECT_EQ(12.3, designs[0].coolingDehumidificationHumidityRatio2());
    EXPECT_EQ(19.6, designs[0].coolingDehumidificationMeanCoincidentDryBulb2());
    EXPECT_EQ(59.7, designs[0].coolingEnthalpy0pt4());
    EXPECT_EQ(27.3, designs[0].coolingEnthalpyMeanCoincidentDryBulb0pt4());
    EXPECT_EQ(56.6, designs[0].coolingEnthalpy1());
    EXPECT_EQ(26.6, designs[0].coolingEnthalpyMeanCoincidentDryBulb1());
    EXPECT_EQ(54.0, designs[0].coolingEnthalpy2());
    EXPECT_EQ(25.7, designs[0].coolingEnthalpyMeanCoincidentDryBulb2());
    EXPECT_EQ(760, designs[0].coolingHours8To4AndDryBulb12pt8To20pt6());
    EXPECT_EQ(11.1, designs[0].extremeWindSpeed1());
    EXPECT_EQ(9.5, designs[0].extremeWindSpeed2pt5());
    EXPECT_EQ(8.4, designs[0].extremeWindSpeed5());
    EXPECT_EQ(22.9, designs[0].extremeMaxWetBulb());
    EXPECT_EQ(-22.9, designs[0].extremeMeanMinDryBulb());
    EXPECT_EQ(36.1, designs[0].extremeMeanMaxDryBulb());
    EXPECT_EQ(3.8, designs[0].extremeStdDevMinDryBulb());
    EXPECT_EQ(1.2, designs[0].extremeStdDevMaxDryBulb());
    EXPECT_EQ(-25.7, designs[0].extremeN5YearsMinDryBulb());
    EXPECT_EQ(37.0, designs[0].extremeN5YearsMaxDryBulb());
    EXPECT_EQ(-27.9, designs[0].extremeN10YearsMinDryBulb());
    EXPECT_EQ(37.7, designs[0].extremeN10YearsMaxDryBulb());
    EXPECT_EQ(-30.1, designs[0].extremeN20YearsMinDryBulb());
    EXPECT_EQ(38.3, designs[0].extremeN20YearsMaxDryBulb());
    EXPECT_EQ(-32.8, designs[0].extremeN50YearsMinDryBulb());
    EXPECT_EQ(39.2, designs[0].extremeN50YearsMaxDryBulb());
    // Ask for data
    std::vector<EpwDataPoint> data = epwFile.data();
    EXPECT_EQ(8760, data.size());
    // The last data point should be for the last hour 12/31/1996, with a dry bulb temp of 4C and pressure 81100
    openstudio::DateTime dateTime = data[8759].dateTime();
    EXPECT_EQ(1, dateTime.date().monthOfYear().value());
    EXPECT_EQ(1, dateTime.date().dayOfMonth());
    // Stopgap test until things are straightened out with years
    EXPECT_EQ(2010, dateTime.date().year());
    //EXPECT_EQ(1997, dateTime.date().year());
    EXPECT_EQ(0, dateTime.time().hours());
    EXPECT_EQ(0, dateTime.time().minutes());
    EXPECT_EQ(0, dateTime.time().seconds());
    EXPECT_EQ(4.0, data[8759].dryBulbTemperature().get());
    EXPECT_EQ(81100, data[8759].atmosphericStationPressure().get());
    // Try out the alternate access functions, dew point temperature should be -1C
    EXPECT_EQ(-1.0, data[8759].getFieldByName("Dew Point Temperature").get());
    EXPECT_EQ(-1.0, data[8759].getField(EpwDataField("Dew Point Temperature")).get());
    // The last data point should not have a liquid precipitation depth
    EXPECT_FALSE(data[8759].getFieldByName("Liquid Precipitation Depth"));
    // Check that the design data is the right size
    designs = epwFile.designConditions();
    EXPECT_EQ(1, designs.size());
  } catch (...) {
    ASSERT_TRUE(false);
  }
}

TEST(Filetypes, EpwFile_NoDesign)
{
  try {
    path p = resourcesPath() / toPath("utilities/Filetypes/leapday-test.epw");
    EpwFile epwFile(p);
    EXPECT_EQ(p, epwFile.path());
    // Ask for data
    std::vector<EpwDataPoint> data = epwFile.data();
    EXPECT_EQ(8784, data.size());
    // Ask for the design conditions, but there aren't any
    std::vector<EpwDesignCondition> designs = epwFile.designConditions();
    EXPECT_EQ(0, designs.size());
    data = epwFile.data();
    EXPECT_EQ(8784, data.size());
  } catch (...) {
    ASSERT_TRUE(false);
  }
}

TEST(Filetypes, EpwFile_International_Data)
{
  try{
    path p = resourcesPath() / toPath("utilities/Filetypes/CHN_Guangdong.Shaoguan.590820_CSWD.epw");
    EpwFile epwFile(p,true);
    EXPECT_EQ(p, epwFile.path());
    EXPECT_EQ("B68C068B", epwFile.checksum());
    EXPECT_EQ("Shaoguan", epwFile.city());
    EXPECT_EQ("Guangdong", epwFile.stateProvinceRegion());
    EXPECT_EQ("CHN", epwFile.country());
    EXPECT_EQ("CSWD", epwFile.dataSource());
    EXPECT_EQ("590820", epwFile.wmoNumber());
    EXPECT_EQ(24.68, epwFile.latitude());
    EXPECT_EQ(113.6, epwFile.longitude());
    EXPECT_EQ(8, epwFile.timeZone());
    EXPECT_EQ(61, epwFile.elevation());
    EXPECT_EQ(Time(0,1,0,0), epwFile.timeStep());
    EXPECT_EQ(DayOfWeek(DayOfWeek::Sunday), epwFile.startDayOfWeek());
    EXPECT_EQ(Date(MonthOfYear::Jan, 1), epwFile.startDate());
    EXPECT_EQ(Date(MonthOfYear::Dec, 31), epwFile.endDate());
    // Up to here, everything should be the same as the first test. Now ask for the data
    std::vector<EpwDataPoint> data = epwFile.data();
    EXPECT_EQ(8760,data.size());
    // The last data point check
    EXPECT_EQ(14.7,data[8759].dryBulbTemperature().get());
    EXPECT_EQ(101100,data[8759].atmosphericStationPressure().get());
    // Try out the alternate access functions, dew point temperature should be -1C
    EXPECT_EQ(11.7,data[8759].getFieldByName("Dew Point Temperature").get());
    EXPECT_EQ(11.7,data[8759].getField(EpwDataField("Dew Point Temperature")).get());
    // The last data point should not have a liquid precipitation depth
    EXPECT_FALSE(data[8759].getFieldByName("Liquid Precipitation Depth"));
    // Get a time series
    boost::optional<openstudio::TimeSeries> series = epwFile.getTimeSeries("Wind Speed");
    ASSERT_TRUE(series);
    ASSERT_EQ(8760,series->values().size());
    DateTimeVector seriesTimes = series->dateTimes();
    ASSERT_EQ(8760,seriesTimes.size());
    // Check the times in the data and the time series
    DateTime current(Date(1,1,1999),Time(0,1)); // Use 1999 to avoid leap years
    Time delta(0,1);
    for(unsigned i=0;i<8760;i++) {
      // This is a lot more complicated that it probably should be to avoid the year being a problem
      DateTime datatime = data[i].dateTime();
      EXPECT_EQ(datatime.date().monthOfYear(), current.date().monthOfYear());
      EXPECT_EQ(datatime.date().dayOfMonth(), current.date().dayOfMonth());
      EXPECT_EQ(datatime.time().hours(), current.time().hours());
      EXPECT_EQ(datatime.time().minutes(), current.time().minutes());
      DateTime seriestime = seriesTimes[i];
      EXPECT_EQ(seriestime.date().monthOfYear(), current.date().monthOfYear());
      EXPECT_EQ(seriestime.date().dayOfMonth(), current.date().dayOfMonth());
      EXPECT_EQ(seriestime.time().hours(), current.time().hours());
      EXPECT_EQ(seriestime.time().minutes(), current.time().minutes());
      current += delta;
    }
    // No need to redo the original tests here since the data should have been loaded in the constructor
  }catch(...){
    ASSERT_TRUE(false);
  }
}

TEST(Filetypes, EpwFile_IWEC_Data)
{
  try {
    path p = resourcesPath() / toPath("utilities/Filetypes/TUN_Tunis.607150_IWEC.epw");
    EpwFile epwFile(p, true);
    EXPECT_EQ(p, epwFile.path());
    EXPECT_EQ("FEAB878E", epwFile.checksum());
    EXPECT_EQ("TUNIS", epwFile.city());
    EXPECT_EQ("-", epwFile.stateProvinceRegion());
    EXPECT_EQ("TUN", epwFile.country());
    EXPECT_EQ("IWEC Data", epwFile.dataSource());
    EXPECT_EQ("607150", epwFile.wmoNumber());
    EXPECT_EQ(36.83, epwFile.latitude());
    EXPECT_EQ(10.23, epwFile.longitude());
    EXPECT_EQ(1, epwFile.timeZone());
    EXPECT_EQ(4, epwFile.elevation());
    EXPECT_EQ(Time(0, 1, 0, 0), epwFile.timeStep());
    EXPECT_EQ(DayOfWeek(DayOfWeek::Sunday), epwFile.startDayOfWeek());
    EXPECT_EQ(Date(MonthOfYear::Jan, 1), epwFile.startDate());
    EXPECT_EQ(Date(MonthOfYear::Dec, 31), epwFile.endDate());
    // Up to here, everything should be the same as the first test. Now ask for the data
    std::vector<EpwDataPoint> data = epwFile.data();
    EXPECT_EQ(8760, data.size());
    // The last data point check
    EXPECT_EQ(11.3, data[8759].dryBulbTemperature().get());
    EXPECT_EQ(102400, data[8759].atmosphericStationPressure().get());
    // Try out the alternate access functions
    EXPECT_EQ(9.8, data[8759].getFieldByName("Dew Point Temperature").get());
    EXPECT_EQ(9.8, data[8759].getField(EpwDataField("Dew Point Temperature")).get());
    // Get a time series
    boost::optional<openstudio::TimeSeries> series = epwFile.getTimeSeries("Wind Speed");
    ASSERT_TRUE(series);
    ASSERT_EQ(8760, series->values().size());
    DateTimeVector seriesTimes = series->dateTimes();
    ASSERT_EQ(8760, seriesTimes.size());
    // Check the times in the data and the time series
    DateTime current(Date(1, 1, 1999), Time(0, 1)); // Use 1999 to avoid leap years
    Time delta(0, 1);
    for (unsigned i = 0; i<8760; i++) {
      // This is a lot more complicated that it probably should be to avoid the year being a problem
      DateTime datatime = data[i].dateTime();
      EXPECT_EQ(datatime.date().monthOfYear(), current.date().monthOfYear());
      EXPECT_EQ(datatime.date().dayOfMonth(), current.date().dayOfMonth());
      EXPECT_EQ(datatime.time().hours(), current.time().hours());
      EXPECT_EQ(datatime.time().minutes(), current.time().minutes());
      DateTime seriestime = seriesTimes[i];
      EXPECT_EQ(seriestime.date().monthOfYear(), current.date().monthOfYear());
      EXPECT_EQ(seriestime.date().dayOfMonth(), current.date().dayOfMonth());
      EXPECT_EQ(seriestime.time().hours(), current.time().hours());
      EXPECT_EQ(seriestime.time().minutes(), current.time().minutes());
      current += delta;
    }
    // No need to redo the original tests here since the data should have been loaded in the constructor
  }
  catch (...) {
    ASSERT_TRUE(false);
  }
}

TEST(Filetypes, EpwFile_TMY)
{
  try{
    path p = resourcesPath() / toPath("utilities/Filetypes/USA_CO_Golden-NREL.724666_TMY3.epw");
    EpwFile epwFile(p);
    EXPECT_EQ(p, epwFile.path());
    EXPECT_EQ("BDF687C1", epwFile.checksum());
    EXPECT_EQ("Denver Centennial  Golden   Nr", epwFile.city());
    EXPECT_EQ("CO", epwFile.stateProvinceRegion());
    EXPECT_EQ("USA", epwFile.country());
    EXPECT_EQ("TMY3", epwFile.dataSource());
    EXPECT_EQ("724666", epwFile.wmoNumber());
    EXPECT_EQ(39.74, epwFile.latitude());
    EXPECT_EQ(-105.18, epwFile.longitude());
    EXPECT_EQ(-7, epwFile.timeZone());
    EXPECT_EQ(1829, epwFile.elevation());
    EXPECT_EQ(Time(0,1,0,0), epwFile.timeStep());
    EXPECT_EQ(DayOfWeek(DayOfWeek::Sunday), epwFile.startDayOfWeek());
    EXPECT_FALSE(epwFile.startDateActualYear());
    EXPECT_FALSE(epwFile.endDateActualYear());
    EXPECT_EQ(Date(MonthOfYear::Jan, 1), epwFile.startDate());
    EXPECT_EQ(Date(MonthOfYear::Dec, 31), epwFile.endDate());
    EXPECT_EQ(365, (epwFile.endDate() - epwFile.startDate()).totalDays() + 1);
    EXPECT_FALSE(epwFile.isActual());
  } catch (...) {
    ASSERT_TRUE(false);
  }
}

TEST(Filetypes, EpwFile_Wrap_TMY)
{
  try{
    path p = resourcesPath() / toPath("utilities/Filetypes/USA_CO_Golden-NREL.wrap.epw");
    EpwFile epwFile(p);
    EXPECT_TRUE(false);
  }catch(...){
    EXPECT_TRUE(true);
  }
}

TEST(Filetypes, EpwFile_AMY)
{
  try{
    path p = resourcesPath() / toPath("utilities/Filetypes/USA_CO_Golden-NREL.amy");
    EpwFile epwFile(p);
    EXPECT_EQ(p, epwFile.path());
    EXPECT_EQ("521A957C", epwFile.checksum());
    EXPECT_EQ("Denver Centennial  Golden   Nr", epwFile.city());
    EXPECT_EQ("CO", epwFile.stateProvinceRegion());
    EXPECT_EQ("USA", epwFile.country());
    EXPECT_EQ("TMY3", epwFile.dataSource());
    EXPECT_EQ("724666", epwFile.wmoNumber());
    EXPECT_EQ(39.74, epwFile.latitude());
    EXPECT_EQ(-105.18, epwFile.longitude());
    EXPECT_EQ(-7, epwFile.timeZone());
    EXPECT_EQ(1829, epwFile.elevation());
    EXPECT_EQ(Time(0,1,0,0), epwFile.timeStep());
    EXPECT_EQ(DayOfWeek(DayOfWeek::Friday), epwFile.startDayOfWeek());
    ASSERT_TRUE(epwFile.startDateActualYear());
    EXPECT_EQ(1999, epwFile.startDateActualYear().get());
    ASSERT_TRUE(epwFile.endDateActualYear());
    EXPECT_EQ(1999, epwFile.endDateActualYear().get());
    EXPECT_EQ(Date(MonthOfYear::Jan, 1, 1999), epwFile.startDate());
    EXPECT_EQ(Date(MonthOfYear::Dec, 31, 1999), epwFile.endDate());
    EXPECT_EQ(365, (epwFile.endDate() - epwFile.startDate()).totalDays() + 1);
    EXPECT_TRUE(epwFile.isActual());
  }catch(...){
    ASSERT_TRUE(false);
  }
}

TEST(Filetypes, EpwFile_Wrap_AMY)
{
  try{
    path p = resourcesPath() / toPath("utilities/Filetypes/USA_CO_Golden-NREL.wrap.amy");
    EpwFile epwFile(p);
    EXPECT_EQ(p, epwFile.path());
    EXPECT_EQ("8F74A20B", epwFile.checksum());
    EXPECT_EQ("Denver Centennial  Golden   Nr", epwFile.city());
    EXPECT_EQ("CO", epwFile.stateProvinceRegion());
    EXPECT_EQ("USA", epwFile.country());
    EXPECT_EQ("TMY3", epwFile.dataSource());
    EXPECT_EQ("724666", epwFile.wmoNumber());
    EXPECT_EQ(39.74, epwFile.latitude());
    EXPECT_EQ(-105.18, epwFile.longitude());
    EXPECT_EQ(-7, epwFile.timeZone());
    EXPECT_EQ(1829, epwFile.elevation());
    EXPECT_EQ(Time(0,1,0,0), epwFile.timeStep());
    EXPECT_EQ(DayOfWeek(DayOfWeek::Saturday), epwFile.startDayOfWeek());
    ASSERT_TRUE(epwFile.startDateActualYear());
    EXPECT_EQ(1999, epwFile.startDateActualYear().get());
    ASSERT_TRUE(epwFile.endDateActualYear());
    EXPECT_EQ(2000, epwFile.endDateActualYear().get());
    EXPECT_EQ(Date(MonthOfYear::Apr, 10, 1999), epwFile.startDate());
    EXPECT_EQ(Date(MonthOfYear::Apr, 8, 2000), epwFile.endDate()); // 2000 is a leap year
    EXPECT_EQ(365, (epwFile.endDate() - epwFile.startDate()).totalDays() + 1);
    EXPECT_TRUE(epwFile.isActual());
  } catch (...) {
    ASSERT_TRUE(false);
  }
}

TEST(Filetypes, EpwFile_DataPoint)
{
  try{
    // Check that we can read all timeseries wind speed values, even if they are extreme
    path p = resourcesPath() / toPath("utilities/Filetypes/USA_CT_New.Haven-Tweed.AP.725045_TMY3.epw");
    EpwFile epwFile(p);
    EXPECT_EQ(p, epwFile.path());
    std::vector<EpwDataPoint> data = epwFile.data();
    for (EpwDataPoint dataPoint : data) {
      EXPECT_NE(dataPoint.windSpeed(), boost::none);
      //EXPECT_NO_THROW(dataPoint.windSpeed().get());
    }
  } catch (...) {
    ASSERT_TRUE(false);
  }
}
