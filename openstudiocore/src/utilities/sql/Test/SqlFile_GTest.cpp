/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "SqlFileFixture.hpp"

#include "../../time/Date.hpp"
#include "../../time/Calendar.hpp"
#include "../../core/Optional.hpp"
#include "../../data/DataEnums.hpp"
#include "../../data/TimeSeries.hpp"
#include "../../filetypes/EpwFile.hpp"
#include "../../units/UnitFactory.hpp"
#include "../../core/Application.hpp"

#include <QRegularExpression>

#include <resources.hxx>

#include <iostream>

using namespace std;
using namespace boost;
using namespace openstudio;

TEST_F(SqlFileFixture, SummaryValues)
{
  // check values
  ASSERT_TRUE(sqlFile.netSiteEnergy());
  ASSERT_TRUE(sqlFile.totalSiteEnergy());
  ASSERT_TRUE(sqlFile.netSourceEnergy());
  ASSERT_TRUE(sqlFile.totalSourceEnergy());

  EXPECT_NEAR(224.91, *(sqlFile.netSiteEnergy()), 2);     // for 6.0, was 222.69
  EXPECT_NEAR(224.91, *(sqlFile.totalSiteEnergy()), 2);   // for 6.0, was 222.69
  EXPECT_NEAR(570.38, *(sqlFile.netSourceEnergy()), 2);   // for 6.0, was 564.41
  EXPECT_NEAR(570.38, *(sqlFile.totalSourceEnergy()), 2); // for 6.0, was 564.41
}


TEST_F(SqlFileFixture, EnvPeriods)
{
  std::vector<std::string> availableEnvPeriods = sqlFile.availableEnvPeriods();
  ASSERT_FALSE(availableEnvPeriods.empty());
  EXPECT_EQ(static_cast<unsigned>(1), availableEnvPeriods.size());
  //EXPECT_EQ("Chicago Ohare Intl Ap IL USA TMY3 WMO#=725300", availableEnvPeriods[0]);
  EXPECT_EQ("CHICAGO OHARE INTL AP IL USA TMY3 WMO#=725300", availableEnvPeriods[0]);
}

TEST_F(SqlFileFixture, TimeSeriesValues)
{
  std::vector<std::string> availableEnvPeriods = sqlFile.availableEnvPeriods();
  ASSERT_FALSE(availableEnvPeriods.empty());
  EXPECT_EQ(static_cast<unsigned>(1), availableEnvPeriods.size());
  //EXPECT_EQ("Chicago Ohare Intl Ap IL USA TMY3 WMO#=725300", availableEnvPeriods[0]);
  EXPECT_EQ("CHICAGO OHARE INTL AP IL USA TMY3 WMO#=725300", availableEnvPeriods[0]);

  openstudio::OptionalTimeSeries ts = sqlFile.timeSeries(availableEnvPeriods[0], "Hourly", "Site Outdoor Air Drybulb Temperature",  "Environment");
  ASSERT_TRUE(ts);
//  ASSERT_EQ(static_cast<unsigned>(8760), ts->dateTimes().size());
  ASSERT_EQ(static_cast<unsigned>(8760), ts->daysFromFirstReport().size());
  ASSERT_EQ(static_cast<unsigned>(8760), ts->values().size());
//  EXPECT_EQ(DateTime(Date(MonthOfYear::Jan, 1), Time(0,1,0,0)), ts->dateTimes().front());
  EXPECT_EQ(DateTime(Date(MonthOfYear::Jan, 1), Time(0,1,0,0)), ts->firstReportDateTime());
  EXPECT_DOUBLE_EQ(-8.2625, ts->values()[0]);
  EXPECT_DOUBLE_EQ(ts->outOfRangeValue(), ts->value(DateTime(Date(MonthOfYear::Jan, 1), Time(0,0,0,0))));
  // DLM: there is commented out code in openstudio::OptionalTime SqlFile_Impl::timeSeriesInterval(const DataDictionaryItem& dataDictionary)
  // that does not recognize hourly as an interval type, so the following line is not expected to work
  //EXPECT_DOUBLE_EQ(-8.2625, ts->value(DateTime(Date(MonthOfYear::Jan, 1), Time(0,0,0,1))));
  EXPECT_DOUBLE_EQ(-8.2625, ts->value(DateTime(Date(MonthOfYear::Jan, 1), Time(0,1,0,0))));
  EXPECT_DOUBLE_EQ(-11.8875, ts->value(DateTime(Date(MonthOfYear::Jan, 1), Time(0,1,0,1))));
//  EXPECT_EQ(DateTime(Date(MonthOfYear::Dec, 31), Time(0,24,0,0)), ts->dateTimes().back());
  EXPECT_EQ(DateTime(Date(MonthOfYear::Dec, 31), Time(0,24,0,0)), ts->firstReportDateTime()+Time(ts->daysFromFirstReport(ts->daysFromFirstReport().size()-1)));
  EXPECT_DOUBLE_EQ(-5.6875, ts->values()[8759]);
  EXPECT_DOUBLE_EQ(-4.775, ts->value(DateTime(Date(MonthOfYear::Dec, 31), Time(0,22,59,59))));
  EXPECT_DOUBLE_EQ(-4.775, ts->value(DateTime(Date(MonthOfYear::Dec, 31), Time(0,23,0,0))));
  EXPECT_DOUBLE_EQ(-5.6875, ts->value(DateTime(Date(MonthOfYear::Dec, 31), Time(0,23,0,1))));
  EXPECT_DOUBLE_EQ(-5.6875, ts->value(DateTime(Date(MonthOfYear::Dec, 31), Time(0,24,0,0))));
  EXPECT_DOUBLE_EQ(ts->outOfRangeValue(), ts->value(DateTime(Date(MonthOfYear::Dec, 31), Time(0,24,0,1))));

  ts = sqlFile.timeSeries(availableEnvPeriods[0], "HVAC System Timestep", "Site Outdoor Air Drybulb Temperature",  "Environment");
  ASSERT_TRUE(ts);
//  EXPECT_EQ(DateTime(Date(MonthOfYear::Jan, 1), Time(0,0,15,0)), ts->dateTimes().front());
  EXPECT_EQ(DateTime(Date(MonthOfYear::Jan, 1), Time(0,0,15,0)), ts->firstReportDateTime());
  EXPECT_DOUBLE_EQ(ts->outOfRangeValue(), ts->value(DateTime(Date(MonthOfYear::Jan, 1), Time(0,0,0,0))));
  EXPECT_DOUBLE_EQ(ts->outOfRangeValue(), ts->value(DateTime(Date(MonthOfYear::Jan, 1), Time(0,0,0,1))));
  EXPECT_DOUBLE_EQ(ts->outOfRangeValue(), ts->value(DateTime(Date(MonthOfYear::Jan, 1), Time(0,0,0,14))));
  EXPECT_DOUBLE_EQ(-4.325, ts->value(DateTime(Date(MonthOfYear::Jan, 1), Time(0,0,15,0))));
  EXPECT_DOUBLE_EQ(-6.95, ts->value(DateTime(Date(MonthOfYear::Jan, 1), Time(0,0,15,1))));
  EXPECT_DOUBLE_EQ(-6.95, ts->value(DateTime(Date(MonthOfYear::Jan, 1), Time(0,0,30,0))));
  EXPECT_DOUBLE_EQ(-12.2 , ts->value(DateTime(Date(MonthOfYear::Jan, 1), Time(0,1,0,0))));
//  EXPECT_EQ(DateTime(Date(MonthOfYear::Dec, 31), Time(0,24,0,0)), ts->dateTimes().back());
  EXPECT_EQ(DateTime(Date(MonthOfYear::Dec, 31), Time(0,24,0,0)), ts->firstReportDateTime()+Time(ts->daysFromFirstReport(ts->daysFromFirstReport().size()-1)));
  EXPECT_DOUBLE_EQ(-5.0  , ts->value(DateTime(Date(MonthOfYear::Dec, 31), Time(0,22,59,59))));
  EXPECT_DOUBLE_EQ(-5.0  , ts->value(DateTime(Date(MonthOfYear::Dec, 31), Time(0,23,0,0))));
  EXPECT_DOUBLE_EQ(-5.275, ts->value(DateTime(Date(MonthOfYear::Dec, 31), Time(0,23,0,1))));
  EXPECT_DOUBLE_EQ(-5.275, ts->value(DateTime(Date(MonthOfYear::Dec, 31), Time(0,23,15,0))));
  EXPECT_DOUBLE_EQ(-6.1  , ts->value(DateTime(Date(MonthOfYear::Dec, 31), Time(0,24,0,0))));
  EXPECT_DOUBLE_EQ(ts->outOfRangeValue(), ts->value(DateTime(Date(MonthOfYear::Dec, 31), Time(0,24,0,1))));
}

TEST_F(SqlFileFixture, TimeSeriesCount)
{
  std::vector<std::string> availableTimeSeries = sqlFile.availableTimeSeries();
  ASSERT_FALSE(availableTimeSeries.empty());

  std::vector<std::string> availableEnvPeriods = sqlFile.availableEnvPeriods();
  ASSERT_FALSE(availableEnvPeriods.empty());

  // check that "Electricity:Facility" is available
  ASSERT_EQ(1, std::count(availableTimeSeries.begin(), availableTimeSeries.end(), "Electricity:Facility"));
  openstudio::OptionalTimeSeries ts = sqlFile.timeSeries(availableEnvPeriods[0], "Hourly", "Electricity:Facility",  "");
  ASSERT_TRUE(ts);

  // check that "Gas:Facility" is available
  ASSERT_EQ(1, std::count(availableTimeSeries.begin(), availableTimeSeries.end(), "Gas:Facility"));
  ts = sqlFile.timeSeries(availableEnvPeriods[0], "Hourly", "Gas:Facility",  "");
  ASSERT_TRUE(ts);

  // check that "NotAVariable:Facility" is not available
  EXPECT_TRUE(availableTimeSeries.end() == std::find(availableTimeSeries.begin(), availableTimeSeries.end(), "NotAVariable:Facility"));
  ts = sqlFile.timeSeries(availableEnvPeriods[0], "Hourly", "NotAVariable:Facility",  "");
  EXPECT_FALSE(ts);

  // check that "Electricity:Facility" is available
  ASSERT_EQ(1, std::count(availableTimeSeries.begin(), availableTimeSeries.end(), "Electricity:Facility"));
  ts = sqlFile.timeSeries(availableEnvPeriods[0], "Run Period", "Electricity:Facility",  "");
  ASSERT_TRUE(ts);

  // check that "Gas:Facility" is available
  ASSERT_EQ(1, std::count(availableTimeSeries.begin(), availableTimeSeries.end(), "Gas:Facility"));
  ts = sqlFile.timeSeries(availableEnvPeriods[0], "Run Period", "Gas:Facility",  "");
  ASSERT_TRUE(ts);

  // check that "NotAVariable:Facility" is not available
  EXPECT_TRUE(availableTimeSeries.end() == std::find(availableTimeSeries.begin(), availableTimeSeries.end(), "NotAVariable:Facility"));
  ts = sqlFile.timeSeries(availableEnvPeriods[0], "Run Period", "NotAVariable:Facility",  "");
  EXPECT_FALSE(ts);
}

TEST_F(SqlFileFixture, TimeSeries)
{
  std::vector<std::string> availableEnvPeriods = sqlFile.availableEnvPeriods();
  ASSERT_FALSE(availableEnvPeriods.empty());

  // make a timeseries
  openstudio::OptionalTimeSeries ts = sqlFile.timeSeries(availableEnvPeriods[0], "Hourly", "Electricity:Facility",  "");
  ASSERT_TRUE(ts);

  // should have 365 days minus 1 hour
//  Time duration = ts->dateTimes().back() - ts->dateTimes().front();
  Time duration = ts->firstReportDateTime() + Time(ts->daysFromFirstReport(ts->daysFromFirstReport().size()-1)) - ts->firstReportDateTime();
  EXPECT_DOUBLE_EQ(365-1.0/24.0, duration.totalDays());
}

TEST_F(SqlFileFixture, BadStatement)
{
  OptionalDouble result = sqlFile.execAndReturnFirstDouble("SELECT * FROM NonExistantTable");
  EXPECT_FALSE(result);
}

TEST_F(SqlFileFixture, CreateSqlFile)
{
  openstudio::path outfile = openstudio::tempDir() / openstudio::toPath("OpenStudioSqlFileTest.sql");
  if (openstudio::filesystem::exists(outfile))
  {
    openstudio::filesystem::remove(outfile);
  }

  openstudio::Calendar c(2012);
  c.standardHolidays();


  std::vector<double> values;
  values.push_back(100);
  values.push_back(10);
  values.push_back(1);
  values.push_back(100.5);

  TimeSeries timeSeries(c.startDate(), openstudio::Time(0,1), openstudio::createVector(values), "lux");

  {
    openstudio::SqlFile sql(outfile,
        openstudio::EpwFile(resourcesPath() / toPath("utilities/Filetypes/USA_CO_Golden-NREL.724666_TMY3.epw")),
        openstudio::DateTime::now(),
        c);

    EXPECT_TRUE(sql.connectionOpen());

    sql.insertTimeSeriesData("Sum", "Zone", "Zone", "DAYLIGHTING WINDOW", "Daylight Luminance", openstudio::ReportingFrequency::Hourly,
        boost::optional<std::string>(), "lux", timeSeries);

    sql.insertZone("CLASSROOM",
        0,
        0,0,0,
        1,1,1,
        3,
        1,
        1,
        0, 2,
        0, 2,
        0, 2,
        2,
        8,
        3,
        3,
        4,
        4,
        2,
        2,
        true);

    std::vector<double> xs;
    std::vector<double> ys;
    xs.push_back(1.1);
    xs.push_back(2.2);
    xs.push_back(3.3);

    ys.push_back(2.8);
    ys.push_back(3.9);

    std::vector<DateTime> dates;
    dates.push_back(DateTime("2012-09-01 1:00"));
    dates.push_back(DateTime("2012-09-01 2:00"));

    std::vector<Matrix> maps;

    Matrix m1(3, 2);
    m1(0,0) = 0;
    m1(1,0) = 0.1;
    m1(2,0) = 0.2;
    m1(0,1) = 0.3;
    m1(1,1) = 0.4;
    m1(2,1) = 0.5;

    maps.push_back(m1);
    Matrix m2(3, 2);
    m1(0,0) = 2;
    m1(1,0) = 2.1;
    m1(2,0) = 2.2;
    m1(0,1) = 2.3;
    m1(1,1) = 2.4;
    m1(2,1) = 2.5;
    maps.push_back(m2);


    sql.insertIlluminanceMap("CLASSROOM", "CLASSROOM DAYLIGHT MAP", "GOLDEN",
      dates, xs, ys, 3.8, maps);
  }


  {
    openstudio::SqlFile sql(outfile);
    EXPECT_TRUE(sql.connectionOpen());
    std::vector<std::string> envPeriods = sql.availableEnvPeriods();
    ASSERT_EQ(envPeriods.size(), 1u);
    std::vector<std::string> reportingFrequencies = sql.availableReportingFrequencies(envPeriods[0]);
    ASSERT_EQ(reportingFrequencies.size(), 1u);
    std::vector<std::string> timeSeriesNames = sql.availableTimeSeries();
    ASSERT_EQ(reportingFrequencies.size(), 1u);
    std::vector<std::string> keyValues = sql.availableKeyValues(envPeriods[0], reportingFrequencies[0], timeSeriesNames[0]);
    ASSERT_EQ(keyValues.size(), 1u);

    boost::optional<TimeSeries> ts = sql.timeSeries(envPeriods[0], reportingFrequencies[0], timeSeriesNames[0], keyValues[0]);
    ASSERT_TRUE(ts);

    EXPECT_EQ(openstudio::toStandardVector(ts->values()), openstudio::toStandardVector(timeSeries.values()));
    EXPECT_EQ(openstudio::toStandardVector(ts->daysFromFirstReport()), openstudio::toStandardVector(timeSeries.daysFromFirstReport()));
  }

}

TEST_F(SqlFileFixture, AnnualTotalCosts) {
  
  // Total annual costs for all fuel types
  EXPECT_NEAR(194619758.82, *(sqlFile2.annualTotalUtilityCost()), 0.1);

  // Costs by fuel type
  EXPECT_NEAR(28168.44, *(sqlFile2.annualTotalCost(FuelType::Electricity)), 0.1);
  EXPECT_NEAR(428.86, *(sqlFile2.annualTotalCost(FuelType::Gas)), 0.1);
  EXPECT_NEAR(324.24, *(sqlFile2.annualTotalCost(FuelType::DistrictCooling)), 0.1);
  EXPECT_NEAR(836.37, *(sqlFile2.annualTotalCost(FuelType::DistrictHeating)), 0.1);
  EXPECT_NEAR(0.91, *(sqlFile2.annualTotalCost(FuelType::Water)), 0.1);
  EXPECT_NEAR(194590000, *(sqlFile2.annualTotalCost(FuelType::FuelOil_1)), 100);

  // Costs by total building area by fuel type
  EXPECT_NEAR(11.73, *(sqlFile2.annualTotalCostPerBldgArea(FuelType::Electricity)), 0.1);
  EXPECT_NEAR(0.18, *(sqlFile2.annualTotalCostPerBldgArea(FuelType::Gas)), 0.1);

  // Costs by conditioned building area by fuel type
  EXPECT_NEAR(11.73, *(sqlFile2.annualTotalCostPerNetConditionedBldgArea(FuelType::Electricity)), 0.1);
  EXPECT_NEAR(0.18, *(sqlFile2.annualTotalCostPerNetConditionedBldgArea(FuelType::Gas)), 0.1);

}

void regressionTestSqlFile(const std::string& name, double netSiteEnergy, double firstVal, double lastVal)
{
  openstudio::path fromPath = resourcesPath() / toPath("utilities/SqlFile") / toPath(name);
  openstudio::path path = toPath(name);

  if (openstudio::filesystem::exists(path)){
    openstudio::filesystem::remove(path);
  }
  ASSERT_FALSE(openstudio::filesystem::exists(path));
  ASSERT_TRUE(openstudio::filesystem::exists(fromPath)) << toString(fromPath);
  openstudio::filesystem::copy(fromPath, path);
  ASSERT_TRUE(openstudio::filesystem::exists(path));

  boost::optional<SqlFile> sqlFile;
  EXPECT_NO_THROW(sqlFile = SqlFile(path));
  ASSERT_TRUE(sqlFile);

  QRegularExpression re("1ZoneEvapCooler-V(.*)\\.sql");
  QRegularExpressionMatch match = re.match(toQString(name));
  ASSERT_TRUE(match.hasMatch());
  VersionString expected(toString(match.captured(1)));
  VersionString actual(sqlFile->energyPlusVersion());
  EXPECT_EQ(expected.major(), actual.major());
  EXPECT_EQ(expected.minor(), actual.minor());
  EXPECT_EQ(expected.patch(), actual.patch());
 
  ASSERT_TRUE(sqlFile->hoursSimulated());
  EXPECT_EQ(8760.0, sqlFile->hoursSimulated().get()) << name;
  ASSERT_TRUE(sqlFile->netSiteEnergy());
  EXPECT_NEAR(netSiteEnergy, sqlFile->netSiteEnergy().get(), 0.1) << name;

  std::vector<std::string> availableEnvPeriods = sqlFile->availableEnvPeriods();
  ASSERT_FALSE(availableEnvPeriods.empty());
  EXPECT_EQ(static_cast<unsigned>(3), availableEnvPeriods.size());

  { // Detailed
    openstudio::OptionalTimeSeries ts = sqlFile->timeSeries(availableEnvPeriods[0], "Detailed", "Zone Mean Air Temperature", "Main Zone");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriods[0], "HVAC System Timestep", "Zone Mean Air Temperature", "Main Zone");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriods[0], "HVAC System Timestep", "Zone Mean Air Temperature", "MAIN ZONE");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriods[0], "Detailed", "Zone Mean Air Temperature", "MAIN ZONE");
    ASSERT_TRUE(ts);

    unsigned N = ts->daysFromFirstReport().size();

    ASSERT_GT(N, 0u);
    ASSERT_EQ(N, ts->values().size());
    EXPECT_GE(N, 8760u*6u);
    // DLM: can't expect these to always be at timestep
    //EXPECT_EQ(DateTime(Date(MonthOfYear::Jan, 1), Time(0, 0, 10, 0)), ts->firstReportDateTime());
    //EXPECT_EQ(DateTime(Date(MonthOfYear::Jan, 1), Time(0, 0, 10, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[0]);
    // DLM: some sort of rounding error is occurring here
    //EXPECT_EQ(DateTime(Date(MonthOfYear::Dec, 31), Time(1, 0, 0, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[N - 1]);
    EXPECT_EQ(DateTime(Date(MonthOfYear::Dec, 31), Time(0, 23, 59, 59)), ts->firstReportDateTime() + ts->daysFromFirstReport()[N - 1]);
  }
 
  { // Timestep
    openstudio::OptionalTimeSeries ts = sqlFile->timeSeries(availableEnvPeriods[0], "Timestep", "Zone Mean Air Temperature", "Main Zone");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriods[0], "Zone Timestep", "Zone Mean Air Temperature", "Main Zone");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriods[0], "Zone Timestep", "Zone Mean Air Temperature", "MAIN ZONE");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriods[0], "Timestep", "Zone Mean Air Temperature", "MAIN ZONE");
    ASSERT_TRUE(ts);

    unsigned N = ts->daysFromFirstReport().size();

    ASSERT_GT(N, 0u);
    ASSERT_EQ(N, ts->values().size());
    EXPECT_EQ(N, 8760u * 6u);
    EXPECT_EQ(DateTime(Date(MonthOfYear::Jan, 1), Time(0, 0, 10, 0)), ts->firstReportDateTime());
    EXPECT_EQ(DateTime(Date(MonthOfYear::Jan, 1), Time(0, 0, 10, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[0]);
    // DLM: some sort of rounding error is occurring here
    //EXPECT_EQ(DateTime(Date(MonthOfYear::Dec, 31), Time(1, 0, 0, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[N - 1]);
    EXPECT_EQ(DateTime(Date(MonthOfYear::Dec, 31), Time(0, 23, 59, 59)), ts->firstReportDateTime() + ts->daysFromFirstReport()[N - 1]);
  }

  { // Hourly
    openstudio::OptionalTimeSeries ts = sqlFile->timeSeries(availableEnvPeriods[0], "Hourly", "Zone Mean Air Temperature", "Main Zone");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriods[0], "Hourly", "Zone Mean Air Temperature", "MAIN ZONE");
    ASSERT_TRUE(ts);

    unsigned N = ts->daysFromFirstReport().size();

    ASSERT_GT(N, 0u);
    ASSERT_EQ(N, ts->values().size());
    EXPECT_EQ(N, 8760u);
    EXPECT_EQ(DateTime(Date(MonthOfYear::Jan, 1), Time(0, 1, 0, 0)), ts->firstReportDateTime());
    EXPECT_EQ(DateTime(Date(MonthOfYear::Jan, 1), Time(0, 1, 0, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[0]);
    EXPECT_EQ(DateTime(Date(MonthOfYear::Dec, 31), Time(1, 0, 0, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[N - 1]);
    EXPECT_DOUBLE_EQ(firstVal, ts->values()[0]) << name;
    EXPECT_DOUBLE_EQ(lastVal, ts->values()[N-1]) << name;
  }

  { // Daily
    openstudio::OptionalTimeSeries ts = sqlFile->timeSeries(availableEnvPeriods[0], "Daily", "Zone Mean Air Temperature", "Main Zone");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriods[0], "Daily", "Zone Mean Air Temperature", "MAIN ZONE");
    ASSERT_TRUE(ts);

    unsigned N = ts->daysFromFirstReport().size();

    ASSERT_GT(N, 0u);
    ASSERT_EQ(N, ts->values().size());
    EXPECT_EQ(N, 365u);
    EXPECT_EQ(DateTime(Date(MonthOfYear::Jan, 1), Time(1, 0, 0, 0)), ts->firstReportDateTime());
    EXPECT_EQ(DateTime(Date(MonthOfYear::Jan, 1), Time(1, 0, 0, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[0]);
    EXPECT_EQ(DateTime(Date(MonthOfYear::Dec, 31), Time(1, 0, 0, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[N - 1]);
  }
 
  { // Monthly
    openstudio::OptionalTimeSeries ts = sqlFile->timeSeries(availableEnvPeriods[0], "Monthly", "Zone Mean Air Temperature", "Main Zone");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriods[0], "Monthly", "Zone Mean Air Temperature", "MAIN ZONE");
    ASSERT_TRUE(ts);

    unsigned N = ts->daysFromFirstReport().size();

    ASSERT_GT(N, 0u);
    ASSERT_EQ(N, ts->values().size());
    EXPECT_EQ(N, 12u);
    EXPECT_EQ(DateTime(Date(MonthOfYear::Jan, 31), Time(1, 0, 0, 0)), ts->firstReportDateTime());
    EXPECT_EQ(DateTime(Date(MonthOfYear::Jan, 31), Time(1, 0, 0, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[0]);
    EXPECT_EQ(DateTime(Date(MonthOfYear::Dec, 31), Time(1, 0, 0, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[N - 1]);
  }
  
  { // RunPeriod - synonymous with Environment and Annual 
    openstudio::OptionalTimeSeries ts = sqlFile->timeSeries(availableEnvPeriods[0], "RunPeriod", "Zone Mean Air Temperature", "Main Zone");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriods[0], "Run Period", "Zone Mean Air Temperature", "Main Zone");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriods[0], "Run Period", "Zone Mean Air Temperature", "MAIN ZONE");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriods[0], "Environment", "Zone Mean Air Temperature", "Main Zone");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriods[0], "Environment", "Zone Mean Air Temperature", "MAIN ZONE");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriods[0], "Annual", "Zone Mean Air Temperature", "Main Zone");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriods[0], "Annual", "Zone Mean Air Temperature", "MAIN ZONE");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriods[0], "RunPeriod", "Zone Mean Air Temperature", "MAIN ZONE");
    ASSERT_TRUE(ts);

    unsigned N = ts->daysFromFirstReport().size();

    ASSERT_GT(N, 0u);
    ASSERT_EQ(N, ts->values().size());
    EXPECT_EQ(N, 1u);
    EXPECT_EQ(DateTime(Date(MonthOfYear::Dec, 31), Time(1, 0, 0, 0)), ts->firstReportDateTime());
    EXPECT_EQ(DateTime(Date(MonthOfYear::Dec, 31), Time(1, 0, 0, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[0]);
    EXPECT_EQ(DateTime(Date(MonthOfYear::Dec, 31), Time(1, 0, 0, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[N - 1]);
  }

  { // Bad key name
    openstudio::OptionalTimeSeries ts = sqlFile->timeSeries(availableEnvPeriods[0], "Hourly", "Zone Mean Air Temperature", "Zone that does not exist");
    EXPECT_FALSE(ts);
  }
}

TEST_F(SqlFileFixture, Regressions) {
  // these files were created by running the 1ZoneEvapCooler.idf example file 
  // adding the Output:SQLite,SimpleAndTabular object as well as several output variables
  // and using the USA_CO_Golden-NREL.724666_TMY3.epw weather file
  
  regressionTestSqlFile("1ZoneEvapCooler-V7-0-0.sql", 42.25, 20, 20);
  regressionTestSqlFile("1ZoneEvapCooler-V7-1-0.sql", 42.05, 20, 20);
  regressionTestSqlFile("1ZoneEvapCooler-V7-2-0.sql", 43.28, 20, 20);
  regressionTestSqlFile("1ZoneEvapCooler-V8-0-0.sql", 43.28, 20, 20);
  regressionTestSqlFile("1ZoneEvapCooler-V8-1-0.sql", 43.28, 20, 20);
  regressionTestSqlFile("1ZoneEvapCooler-V8-2-0.sql", 43.28, 20, 20);
  regressionTestSqlFile("1ZoneEvapCooler-V8-3-0.sql", 43.28, 20, 20);
  regressionTestSqlFile("1ZoneEvapCooler-V8-4-0.sql", 43.28, 20, 20);
}
