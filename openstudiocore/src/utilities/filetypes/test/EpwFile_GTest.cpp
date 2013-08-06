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
#include <utilities/filetypes/EpwFile.hpp>
#include <utilities/time/Time.hpp>
#include <utilities/time/Date.hpp>

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
    path p = resourcesPath() / toPath("runmanager/USA_CO_Golden-NREL.724666_TMY3.epw");
    EpwFile epwFile(p);
    EXPECT_EQ(p, epwFile.path());
    EXPECT_EQ("E2EFCD8E", epwFile.checksum());
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
  }catch(...){
    ASSERT_TRUE(false);
  }
}

TEST(Filetypes, EpwFile_TMY)
{
  try{
    path p = resourcesPath() / toPath("utilities/Filetypes/USA_CO_Golden-NREL.724666_TMY3.epw");
    EpwFile epwFile(p);
    EXPECT_EQ(p, epwFile.path());
    EXPECT_EQ("3CF64E88", epwFile.checksum());
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
  }catch(...){
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
    EXPECT_EQ("1B0D9577", epwFile.checksum());
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
    EXPECT_EQ("7E3B5B40", epwFile.checksum());
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
  }catch(...){
    ASSERT_TRUE(false);
  }
}