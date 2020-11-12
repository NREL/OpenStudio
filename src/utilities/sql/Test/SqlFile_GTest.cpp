/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "SqlFileFixture.hpp"

#include "../../time/Date.hpp"
#include "../../time/Calendar.hpp"
#include "../../core/Optional.hpp"
#include "../../data/DataEnums.hpp"
#include "../../data/TimeSeries.hpp"
#include "../../filetypes/EpwFile.hpp"
#include "../../units/UnitFactory.hpp"
#include "../../idf/Workspace.hpp"
#include "../../idf/WorkspaceObject.hpp"
#include "../../idd/IddEnums.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/Exterior_FuelEquipment_FieldEnums.hxx>
#include <utilities/idd/Exterior_WaterEquipment_FieldEnums.hxx>

#include <iostream>
#include <boost/regex.hpp>
#include <resources.hxx>
#include <stdexcept>

using namespace std;
using namespace boost;
using namespace openstudio;

TEST_F(SqlFileFixture, SummaryValues) {
  // check values
  ASSERT_TRUE(sqlFile.netSiteEnergy());
  ASSERT_TRUE(sqlFile.totalSiteEnergy());
  ASSERT_TRUE(sqlFile.netSourceEnergy());
  ASSERT_TRUE(sqlFile.totalSourceEnergy());

  EXPECT_NEAR(224.91, *(sqlFile.netSiteEnergy()), 2);      // for 6.0, was 222.69
  EXPECT_NEAR(224.91, *(sqlFile.totalSiteEnergy()), 2);    // for 6.0, was 222.69
  EXPECT_NEAR(570.38, *(sqlFile.netSourceEnergy()), 2);    // for 6.0, was 564.41
  EXPECT_NEAR(570.38, *(sqlFile.totalSourceEnergy()), 2);  // for 6.0, was 564.41
}

TEST_F(SqlFileFixture, EnvPeriods) {
  std::vector<std::string> availableEnvPeriods = sqlFile.availableEnvPeriods();
  ASSERT_FALSE(availableEnvPeriods.empty());
  EXPECT_EQ(static_cast<unsigned>(1), availableEnvPeriods.size());
  //EXPECT_EQ("Chicago Ohare Intl Ap IL USA TMY3 WMO#=725300", availableEnvPeriods[0]);
  EXPECT_EQ("RUNPERIOD 1", availableEnvPeriods[0]);
}

TEST_F(SqlFileFixture, TimeSeriesValues) {
  std::vector<std::string> availableEnvPeriods = sqlFile.availableEnvPeriods();
  ASSERT_FALSE(availableEnvPeriods.empty());
  EXPECT_EQ(static_cast<unsigned>(1), availableEnvPeriods.size());
  //EXPECT_EQ("Chicago Ohare Intl Ap IL USA TMY3 WMO#=725300", availableEnvPeriods[0]);
  EXPECT_EQ("RUNPERIOD 1", availableEnvPeriods[0]);

  // Since v8.9.0, E+ includes the Year into the SQL. 5ZoneAirCooled has 2013
  unsigned year = 2013;
  Date jan1 = Date(MonthOfYear::Jan, 1, year);
  Date dec31 = Date(MonthOfYear::Dec, 31, year);

  openstudio::OptionalTimeSeries ts = sqlFile.timeSeries(availableEnvPeriods[0], "Hourly", "Site Outdoor Air Drybulb Temperature", "Environment");
  ASSERT_TRUE(ts);
  //  ASSERT_EQ(static_cast<unsigned>(8760), ts->dateTimes().size());
  ASSERT_EQ(static_cast<unsigned>(8760), ts->daysFromFirstReport().size());
  ASSERT_EQ(static_cast<unsigned>(8760), ts->values().size());
  //  EXPECT_EQ(DateTime(jan1, Time(0,1,0,0)), ts->dateTimes().front());
  EXPECT_EQ(DateTime(jan1, Time(0, 1, 0, 0)), ts->firstReportDateTime());
  EXPECT_DOUBLE_EQ(-8.2625, ts->values()[0]);
  EXPECT_DOUBLE_EQ(ts->outOfRangeValue(), ts->value(DateTime(jan1, Time(0, 0, 0, 0))));
  // DLM: there is commented out code in openstudio::OptionalTime SqlFile_Impl::timeSeriesInterval(const DataDictionaryItem& dataDictionary)
  // that does not recognize hourly as an interval type, so the following line is not expected to work
  //EXPECT_DOUBLE_EQ(-8.2625, ts->value(DateTime(jan1, Time(0,0,0,1))));
  EXPECT_DOUBLE_EQ(-8.2625, ts->value(DateTime(jan1, Time(0, 1, 0, 0))));
  EXPECT_DOUBLE_EQ(-11.8875, ts->value(DateTime(jan1, Time(0, 1, 0, 1))));
  //  EXPECT_EQ(DateTime(dec31, Time(0,24,0,0)), ts->dateTimes().back());
  EXPECT_EQ(DateTime(dec31, Time(0, 24, 0, 0)), ts->firstReportDateTime() + Time(ts->daysFromFirstReport(ts->daysFromFirstReport().size() - 1)));
  EXPECT_DOUBLE_EQ(-5.6875, ts->values()[8759]);
  EXPECT_DOUBLE_EQ(-4.775, ts->value(DateTime(dec31, Time(0, 22, 59, 59))));
  EXPECT_DOUBLE_EQ(-4.775, ts->value(DateTime(dec31, Time(0, 23, 0, 0))));
  EXPECT_DOUBLE_EQ(-5.6875, ts->value(DateTime(dec31, Time(0, 23, 0, 1))));
  EXPECT_DOUBLE_EQ(-5.6875, ts->value(DateTime(dec31, Time(0, 24, 0, 0))));
  EXPECT_DOUBLE_EQ(ts->outOfRangeValue(), ts->value(DateTime(dec31, Time(0, 24, 0, 1))));

  ts = sqlFile.timeSeries(availableEnvPeriods[0], "HVAC System Timestep", "Site Outdoor Air Drybulb Temperature", "Environment");
  ASSERT_TRUE(ts);
  //  EXPECT_EQ(DateTime(jan1, Time(0,0,15,0)), ts->dateTimes().front());
  EXPECT_EQ(DateTime(jan1, Time(0, 0, 15, 0)), ts->firstReportDateTime());
  EXPECT_DOUBLE_EQ(ts->outOfRangeValue(), ts->value(DateTime(jan1, Time(0, 0, 0, 0))));
  EXPECT_DOUBLE_EQ(ts->outOfRangeValue(), ts->value(DateTime(jan1, Time(0, 0, 0, 1))));
  EXPECT_DOUBLE_EQ(ts->outOfRangeValue(), ts->value(DateTime(jan1, Time(0, 0, 0, 14))));
  EXPECT_DOUBLE_EQ(-4.325, ts->value(DateTime(jan1, Time(0, 0, 15, 0))));
  EXPECT_DOUBLE_EQ(-6.95, ts->value(DateTime(jan1, Time(0, 0, 15, 1))));
  EXPECT_DOUBLE_EQ(-6.95, ts->value(DateTime(jan1, Time(0, 0, 30, 0))));
  EXPECT_DOUBLE_EQ(-12.2, ts->value(DateTime(jan1, Time(0, 1, 0, 0))));
  //  EXPECT_EQ(DateTime(dec31, Time(0,24,0,0)), ts->dateTimes().back());
  EXPECT_EQ(DateTime(dec31, Time(0, 24, 0, 0)), ts->firstReportDateTime() + Time(ts->daysFromFirstReport(ts->daysFromFirstReport().size() - 1)));
  EXPECT_DOUBLE_EQ(-5.0, ts->value(DateTime(dec31, Time(0, 22, 59, 59))));
  EXPECT_DOUBLE_EQ(-5.0, ts->value(DateTime(dec31, Time(0, 23, 0, 0))));
  EXPECT_DOUBLE_EQ(-5.275, ts->value(DateTime(dec31, Time(0, 23, 0, 1))));
  EXPECT_DOUBLE_EQ(-5.275, ts->value(DateTime(dec31, Time(0, 23, 15, 0))));
  EXPECT_DOUBLE_EQ(-6.1, ts->value(DateTime(dec31, Time(0, 24, 0, 0))));
  EXPECT_DOUBLE_EQ(ts->outOfRangeValue(), ts->value(DateTime(dec31, Time(0, 24, 0, 1))));
}

TEST_F(SqlFileFixture, TimeSeriesCount) {
  std::vector<std::string> availableTimeSeries = sqlFile.availableTimeSeries();
  ASSERT_FALSE(availableTimeSeries.empty());

  std::vector<std::string> availableEnvPeriods = sqlFile.availableEnvPeriods();
  ASSERT_FALSE(availableEnvPeriods.empty());

  // check that "Electricity:Facility" is available
  ASSERT_EQ(1, std::count(availableTimeSeries.begin(), availableTimeSeries.end(), "Electricity:Facility"));
  openstudio::OptionalTimeSeries ts = sqlFile.timeSeries(availableEnvPeriods[0], "Hourly", "Electricity:Facility", "");
  ASSERT_TRUE(ts);

  // check that "NaturalGas:Facility" is available (prior to 9.4.0, it was 'Gas:Facility')
  ASSERT_EQ(1, std::count(availableTimeSeries.begin(), availableTimeSeries.end(), "NaturalGas:Facility"));
  ts = sqlFile.timeSeries(availableEnvPeriods[0], "Hourly", "NaturalGas:Facility", "");
  ASSERT_TRUE(ts);

  // check that "NotAVariable:Facility" is not available
  EXPECT_TRUE(availableTimeSeries.end() == std::find(availableTimeSeries.begin(), availableTimeSeries.end(), "NotAVariable:Facility"));
  ts = sqlFile.timeSeries(availableEnvPeriods[0], "Hourly", "NotAVariable:Facility", "");
  EXPECT_FALSE(ts);

  // check that "Electricity:Facility" is available
  ASSERT_EQ(1, std::count(availableTimeSeries.begin(), availableTimeSeries.end(), "Electricity:Facility"));
  ts = sqlFile.timeSeries(availableEnvPeriods[0], "Run Period", "Electricity:Facility", "");
  ASSERT_TRUE(ts);

  // check that "NaturalGas:Facility" is available
  ASSERT_EQ(1, std::count(availableTimeSeries.begin(), availableTimeSeries.end(), "NaturalGas:Facility"));
  ts = sqlFile.timeSeries(availableEnvPeriods[0], "Run Period", "NaturalGas:Facility", "");
  ASSERT_TRUE(ts);

  // check that "NotAVariable:Facility" is not available
  EXPECT_TRUE(availableTimeSeries.end() == std::find(availableTimeSeries.begin(), availableTimeSeries.end(), "NotAVariable:Facility"));
  ts = sqlFile.timeSeries(availableEnvPeriods[0], "Run Period", "NotAVariable:Facility", "");
  EXPECT_FALSE(ts);
}

TEST_F(SqlFileFixture, TimeSeries) {
  std::vector<std::string> availableEnvPeriods = sqlFile.availableEnvPeriods();
  ASSERT_FALSE(availableEnvPeriods.empty());

  // make a timeseries
  openstudio::OptionalTimeSeries ts = sqlFile.timeSeries(availableEnvPeriods[0], "Hourly", "Electricity:Facility", "");
  ASSERT_TRUE(ts);

  // should have 365 days minus 1 hour
  //  Time duration = ts->dateTimes().back() - ts->dateTimes().front();
  Time duration = ts->firstReportDateTime() + Time(ts->daysFromFirstReport(ts->daysFromFirstReport().size() - 1)) - ts->firstReportDateTime();
  EXPECT_DOUBLE_EQ(365 - 1.0 / 24.0, duration.totalDays());
}

TEST_F(SqlFileFixture, BadStatement) {
  const std::string query = "SELECT * FROM NonExistantTable;";
  try {
    sqlFile.execAndReturnFirstDouble(query);
    FAIL() << "BadStatement should fail";
  } catch (std::runtime_error& e) {
    std::string expectedError("Error creating prepared statement: SELECT * FROM NonExistantTable; with error code 1, extended code 1, errmsg: no "
                              "such table: NonExistantTable");
    EXPECT_EQ(expectedError, std::string{e.what()});
  }
}

TEST_F(SqlFileFixture, CreateSqlFile) {
  openstudio::path outfile = openstudio::tempDir() / openstudio::toPath("OpenStudioSqlFileTest.sql");
  if (openstudio::filesystem::exists(outfile)) {
    openstudio::filesystem::remove(outfile);
  }

  openstudio::Calendar c(2012);
  c.standardHolidays();

  std::vector<double> values;
  values.push_back(100);
  values.push_back(10);
  values.push_back(1);
  values.push_back(100.5);

  TimeSeries timeSeries(c.startDate(), openstudio::Time(0, 1), openstudio::createVector(values), "lux");

  {
    openstudio::SqlFile sql(outfile, openstudio::EpwFile(resourcesPath() / toPath("utilities/Filetypes/USA_CO_Golden-NREL.724666_TMY3.epw")),
                            openstudio::DateTime::now(), c);

    EXPECT_TRUE(sql.connectionOpen());

    sql.insertTimeSeriesData("Sum", "Zone", "Zone", "DAYLIGHTING WINDOW", "Daylight Luminance", openstudio::ReportingFrequency::Hourly,
                             boost::optional<std::string>(), "lux", timeSeries);

    sql.insertZone("CLASSROOM", 0, 0, 0, 0, 1, 1, 1, 3, 1, 1, 0, 2, 0, 2, 0, 2, 2, 8, 3, 3, 4, 4, 2, 2, true);

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
    m1(0, 0) = 0;
    m1(1, 0) = 0.1;
    m1(2, 0) = 0.2;
    m1(0, 1) = 0.3;
    m1(1, 1) = 0.4;
    m1(2, 1) = 0.5;

    maps.push_back(m1);
    Matrix m2(3, 2);
    m1(0, 0) = 2;
    m1(1, 0) = 2.1;
    m1(2, 0) = 2.2;
    m1(0, 1) = 2.3;
    m1(1, 1) = 2.4;
    m1(2, 1) = 2.5;
    maps.push_back(m2);

    sql.insertIlluminanceMap("CLASSROOM", "CLASSROOM DAYLIGHT MAP", "GOLDEN", dates, xs, ys, 3.8, maps);
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

  struct SqlResults
  {

    // Total annual costs for all fuel types
    double annualTotalUtilityCost;

    // Costs by fuel type
    double annualTotalCost_Electricity;
    double annualTotalCost_Gas;
    double annualTotalCost_DistrictCooling;
    double annualTotalCost_DistrictHeating;
    double annualTotalCost_Water;
    double annualTotalCost_FuelOil_1;
  };

  // Actual values:   { Total      , Elec    ,  Gas    , DC    , DH    , Water   , FuelOil_1  }
  // SqlResults ep_910 = {195052539.91, 27600.69, 427.17, 324.04, 782.87, 3256405.15, 191767000.0};
  // SqlResults ep_920 = {194898706.43, 27595.94, 426.75, 324.25, 782.28, 3256577.21, 191613000.0};
  // SqlResults ep_930 = {194906985.51, 27596.57, 426.75, 324.25, 782.28, 3262855.66, 191615000.0};
  SqlResults ep_940 = {191927299.41, 27898.69, 407.55, 361.09, 776.63, 3322855.45, 188575000.0};

  // =========== Check that you are within relatively normal ranges compared to previous versions  =================

  // TODO: probably remplement after 9.4+. 9.4 had such a huge variation (as high as 11.36% for district cooling) that bumping the tolerance that high
  // would make that test pointless
  //for (auto& ep: {ep_910, ep_920, ep_930}) {
  //// Total annual costs for all fuel types. Here I'm explicitly passing a tolerance of 0.1% (which is the default really, so omitting it after)
  //EXPECT_TRUE(IsWithinRelativeTolerance(ep.annualTotalUtilityCost, sqlFile2.annualTotalUtilityCost().get(), 0.001));

  //// Costs by fuel type
  //EXPECT_TRUE(IsWithinRelativeTolerance(ep.annualTotalCost_Electricity,     sqlFile2.annualTotalCost(FuelType::Electricity).get()));
  //EXPECT_TRUE(IsWithinRelativeTolerance(ep.annualTotalCost_Gas,             sqlFile2.annualTotalCost(FuelType::Gas).get()));
  //EXPECT_TRUE(IsWithinRelativeTolerance(ep.annualTotalCost_DistrictCooling, sqlFile2.annualTotalCost(FuelType::DistrictCooling).get(), 0.002));
  //EXPECT_TRUE(IsWithinRelativeTolerance(ep.annualTotalCost_DistrictHeating, sqlFile2.annualTotalCost(FuelType::DistrictHeating).get()));
  //EXPECT_TRUE(IsWithinRelativeTolerance(ep.annualTotalCost_Water,           sqlFile2.annualTotalCost(FuelType::Water).get(), 0.003));
  //EXPECT_TRUE(IsWithinRelativeTolerance(ep.annualTotalCost_FuelOil_1,       sqlFile2.annualTotalCost(FuelType::FuelOil_1).get()));
  //}

  // =========== Check that within our development based on the current E+ version we do not make the results vary (at all)  =================

  // Total annual costs for all fuel types
  EXPECT_DOUBLE_EQ(ep_940.annualTotalUtilityCost, sqlFile2.annualTotalUtilityCost().get());

  // Costs by fuel type
  EXPECT_DOUBLE_EQ(ep_940.annualTotalCost_Electricity, sqlFile2.annualTotalCost(FuelType::Electricity).get());
  EXPECT_DOUBLE_EQ(ep_940.annualTotalCost_Gas, sqlFile2.annualTotalCost(FuelType::Gas).get());
  EXPECT_DOUBLE_EQ(ep_940.annualTotalCost_DistrictCooling, sqlFile2.annualTotalCost(FuelType::DistrictCooling).get());
  EXPECT_DOUBLE_EQ(ep_940.annualTotalCost_DistrictHeating, sqlFile2.annualTotalCost(FuelType::DistrictHeating).get());
  EXPECT_DOUBLE_EQ(ep_940.annualTotalCost_Water, sqlFile2.annualTotalCost(FuelType::Water).get());
  EXPECT_DOUBLE_EQ(ep_940.annualTotalCost_FuelOil_1, sqlFile2.annualTotalCost(FuelType::FuelOil_1).get());

  // These have a relatively high tolerance and shouldn't fail, and they depend on the above values divided by square footage which shouldn't vary
  // So it's fine to keep it as is
  // Costs by total building area by fuel type
  EXPECT_NEAR(11.62, *(sqlFile2.annualTotalCostPerBldgArea(FuelType::Electricity)), 0.1);  // (E+ 9.2.0 = 11.498308333333332)
  EXPECT_NEAR(0.18, *(sqlFile2.annualTotalCostPerBldgArea(FuelType::Gas)), 0.1);           // (E+ 9.2.0 = 0.1778125)

  // Costs by conditioned building area by fuel type
  EXPECT_NEAR(11.62, *(sqlFile2.annualTotalCostPerNetConditionedBldgArea(FuelType::Electricity)), 0.1);
  EXPECT_NEAR(0.18, *(sqlFile2.annualTotalCostPerNetConditionedBldgArea(FuelType::Gas)), 0.1);
}

void regressionTestSqlFile(const std::string& name, double netSiteEnergy, double firstVal, double lastVal) {
  openstudio::path fromPath = resourcesPath() / toPath("utilities/SqlFile") / toPath(name);
  openstudio::path path = toPath(name);

  if (openstudio::filesystem::exists(path)) {
    openstudio::filesystem::remove(path);
  }
  ASSERT_FALSE(openstudio::filesystem::exists(path));
  ASSERT_TRUE(openstudio::filesystem::exists(fromPath)) << toString(fromPath);
  openstudio::filesystem::copy(fromPath, path);
  ASSERT_TRUE(openstudio::filesystem::exists(path));

  boost::optional<SqlFile> sqlFile;
  EXPECT_NO_THROW(sqlFile = SqlFile(path));
  ASSERT_TRUE(sqlFile);

  // Capture the version (1st group)
  boost::regex re("1ZoneEvapCooler-V(.*)\\.sql");
  boost::smatch m;
  ASSERT_TRUE(boost::regex_match(name, m, re));
  VersionString expected(m[1]);
  VersionString actual(sqlFile->energyPlusVersion());
  EXPECT_EQ(expected.major(), actual.major());
  EXPECT_EQ(expected.minor(), actual.minor());
  EXPECT_EQ(expected.patch(), actual.patch());

  // Check if the SqlFile has the 'Year' field
  if (expected >= VersionString(9, 0)) {
    EXPECT_TRUE(sqlFile->hasYear());
  } else if (expected < VersionString(8, 9)) {
    EXPECT_FALSE(sqlFile->hasYear());
  } else {
    // For 8.9.0, seems that it has the 'Year' field but it's always zero...
    EXPECT_FALSE(sqlFile->hasYear()) << "Failed for " << name;
  }

  // Check if the SqlFile has the 'Year' field for IlluminanceMap, which was added later in 9.2.0
  if (expected < VersionString(9, 2)) {
    EXPECT_FALSE(sqlFile->hasIlluminanceMapYear());
  } else {
    EXPECT_TRUE(sqlFile->hasIlluminanceMapYear());
  }

  ASSERT_TRUE(sqlFile->hoursSimulated());
  EXPECT_EQ(8760.0, sqlFile->hoursSimulated().get()) << name;
  ASSERT_TRUE(sqlFile->netSiteEnergy());
  EXPECT_NEAR(netSiteEnergy, sqlFile->netSiteEnergy().get(), 0.1) << name;

  std::vector<std::string> availableEnvPeriods = sqlFile->availableEnvPeriods();
  ASSERT_FALSE(availableEnvPeriods.empty());
  EXPECT_EQ(static_cast<unsigned>(3), availableEnvPeriods.size());
  std::string availableEnvPeriod;
  for (size_t i = 0; i < availableEnvPeriods.size(); i++) {
    if (availableEnvPeriods[i].find("CONDNS") == std::string::npos) {
      availableEnvPeriod = availableEnvPeriods[i];
    }
  }

  // Since v8.9.0, E+ includes the Year into the SQL. 5ZoneAirCooled has 2013
  unsigned year = 2013;
  Date jan1 = sqlFile->hasYear() ? Date(MonthOfYear::Jan, 1, year) : Date(MonthOfYear::Jan, 1);
  Date dec31 = sqlFile->hasYear() ? Date(MonthOfYear::Dec, 31, year) : Date(MonthOfYear::Dec, 31);
  Date jan31 = sqlFile->hasYear() ? Date(MonthOfYear::Jan, 31, year) : Date(MonthOfYear::Jan, 31);

  {  // Detailed
    openstudio::OptionalTimeSeries ts = sqlFile->timeSeries(availableEnvPeriod, "Detailed", "Zone Mean Air Temperature", "Main Zone");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriod, "HVAC System Timestep", "Zone Mean Air Temperature", "Main Zone");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriod, "HVAC System Timestep", "Zone Mean Air Temperature", "MAIN ZONE");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriod, "Detailed", "Zone Mean Air Temperature", "MAIN ZONE");
    ASSERT_TRUE(ts);

    unsigned N = ts->daysFromFirstReport().size();

    ASSERT_GT(N, 0u);
    ASSERT_EQ(N, ts->values().size());
    EXPECT_GE(N, 8760u * 6u);
    // DLM: can't expect these to always be at timestep
    //EXPECT_EQ(DateTime(jan1, Time(0, 0, 10, 0)), ts->firstReportDateTime());
    //EXPECT_EQ(DateTime(jan1, Time(0, 0, 10, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[0]);
    // DLM: some sort of rounding error is occurring here
    //EXPECT_EQ(DateTime(dec31, Time(1, 0, 0, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[N - 1]);
    EXPECT_EQ(DateTime(dec31, Time(0, 23, 59, 59)), ts->firstReportDateTime() + ts->daysFromFirstReport()[N - 1]);
  }

  {  // Timestep
    openstudio::OptionalTimeSeries ts = sqlFile->timeSeries(availableEnvPeriod, "Timestep", "Zone Mean Air Temperature", "Main Zone");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriod, "Zone Timestep", "Zone Mean Air Temperature", "Main Zone");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriod, "Zone Timestep", "Zone Mean Air Temperature", "MAIN ZONE");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriod, "Timestep", "Zone Mean Air Temperature", "MAIN ZONE");
    ASSERT_TRUE(ts);

    unsigned N = ts->daysFromFirstReport().size();

    ASSERT_GT(N, 0u);
    ASSERT_EQ(N, ts->values().size());
    EXPECT_EQ(N, 8760u * 6u);
    EXPECT_EQ(DateTime(jan1, Time(0, 0, 10, 0)), ts->firstReportDateTime());
    EXPECT_EQ(DateTime(jan1, Time(0, 0, 10, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[0]);
    // DLM: some sort of rounding error is occurring here
    //EXPECT_EQ(DateTime(dec31, Time(1, 0, 0, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[N - 1]);
    EXPECT_EQ(DateTime(dec31, Time(0, 23, 59, 59)), ts->firstReportDateTime() + ts->daysFromFirstReport()[N - 1]);
  }

  {  // Hourly
    openstudio::OptionalTimeSeries ts = sqlFile->timeSeries(availableEnvPeriod, "Hourly", "Zone Mean Air Temperature", "Main Zone");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriod, "Hourly", "Zone Mean Air Temperature", "MAIN ZONE");
    ASSERT_TRUE(ts);

    unsigned N = ts->daysFromFirstReport().size();

    ASSERT_GT(N, 0u);
    ASSERT_EQ(N, ts->values().size());
    EXPECT_EQ(N, 8760u);
    EXPECT_EQ(DateTime(jan1, Time(0, 1, 0, 0)), ts->firstReportDateTime());
    EXPECT_EQ(DateTime(jan1, Time(0, 1, 0, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[0]);
    EXPECT_EQ(DateTime(dec31, Time(1, 0, 0, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[N - 1]);
    EXPECT_DOUBLE_EQ(firstVal, ts->values()[0]) << name;
    EXPECT_DOUBLE_EQ(lastVal, ts->values()[N - 1]) << name;
  }

  {  // Daily
    openstudio::OptionalTimeSeries ts = sqlFile->timeSeries(availableEnvPeriod, "Daily", "Zone Mean Air Temperature", "Main Zone");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriod, "Daily", "Zone Mean Air Temperature", "MAIN ZONE");
    ASSERT_TRUE(ts);

    unsigned N = ts->daysFromFirstReport().size();

    ASSERT_GT(N, 0u);
    ASSERT_EQ(N, ts->values().size());
    EXPECT_EQ(N, 365u);
    EXPECT_EQ(DateTime(jan1, Time(1, 0, 0, 0)), ts->firstReportDateTime());
    EXPECT_EQ(DateTime(jan1, Time(1, 0, 0, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[0]);
    EXPECT_EQ(DateTime(dec31, Time(1, 0, 0, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[N - 1]);
  }

  {  // Monthly
    openstudio::OptionalTimeSeries ts = sqlFile->timeSeries(availableEnvPeriod, "Monthly", "Zone Mean Air Temperature", "Main Zone");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriod, "Monthly", "Zone Mean Air Temperature", "MAIN ZONE");
    ASSERT_TRUE(ts);

    unsigned N = ts->daysFromFirstReport().size();

    ASSERT_GT(N, 0u);
    ASSERT_EQ(N, ts->values().size());
    EXPECT_EQ(N, 12u);
    EXPECT_EQ(DateTime(jan31, Time(1, 0, 0, 0)), ts->firstReportDateTime());
    EXPECT_EQ(DateTime(jan31, Time(1, 0, 0, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[0]);
    EXPECT_EQ(DateTime(dec31, Time(1, 0, 0, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[N - 1]);
  }

  {  // RunPeriod - synonymous with Environment
    openstudio::OptionalTimeSeries ts = sqlFile->timeSeries(availableEnvPeriod, "RunPeriod", "Zone Mean Air Temperature", "Main Zone");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriod, "Run Period", "Zone Mean Air Temperature", "Main Zone");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriod, "Run Period", "Zone Mean Air Temperature", "MAIN ZONE");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriod, "Environment", "Zone Mean Air Temperature", "Main Zone");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriod, "Environment", "Zone Mean Air Temperature", "MAIN ZONE");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriod, "RunPeriod", "Zone Mean Air Temperature", "MAIN ZONE");
    ASSERT_TRUE(ts);

    unsigned N = ts->daysFromFirstReport().size();

    ASSERT_GT(N, 0u);
    ASSERT_EQ(N, ts->values().size());
    EXPECT_EQ(N, 1u);
    EXPECT_EQ(DateTime(dec31, Time(1, 0, 0, 0)), ts->firstReportDateTime());
    EXPECT_EQ(DateTime(dec31, Time(1, 0, 0, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[0]);
    EXPECT_EQ(DateTime(dec31, Time(1, 0, 0, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[N - 1]);
  }

  {  // Annual
    openstudio::OptionalTimeSeries ts = sqlFile->timeSeries(availableEnvPeriod, "Annual", "Zone Mean Air Temperature", "Main Zone");
    EXPECT_TRUE(ts);
    ts = sqlFile->timeSeries(availableEnvPeriod, "Annual", "Zone Mean Air Temperature", "MAIN ZONE");
    EXPECT_TRUE(ts);

    unsigned N = ts->daysFromFirstReport().size();

    ASSERT_GT(N, 0u);
    ASSERT_EQ(N, ts->values().size());
    EXPECT_EQ(N, 1u);
    EXPECT_EQ(DateTime(dec31, Time(1, 0, 0, 0)), ts->firstReportDateTime());
    EXPECT_EQ(DateTime(dec31, Time(1, 0, 0, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[0]);
    EXPECT_EQ(DateTime(dec31, Time(1, 0, 0, 0)), ts->firstReportDateTime() + ts->daysFromFirstReport()[N - 1]);
  }

  {  // Bad key name
    openstudio::OptionalTimeSeries ts = sqlFile->timeSeries(availableEnvPeriod, "Hourly", "Zone Mean Air Temperature", "Zone that does not exist");
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
  regressionTestSqlFile("1ZoneEvapCooler-V8-5-0.sql", 43.28, 20, 20);
  regressionTestSqlFile("1ZoneEvapCooler-V8-6-0.sql", 43.28, 20, 20);
  regressionTestSqlFile("1ZoneEvapCooler-V8-7-0.sql", 43.28, 20, 20);
  regressionTestSqlFile("1ZoneEvapCooler-V8-8-0.sql", 43.28, 20, 20);
  regressionTestSqlFile("1ZoneEvapCooler-V8-9-0.sql", 43.28, 20, 20);
  regressionTestSqlFile("1ZoneEvapCooler-V9-0-1.sql", 43.28, 20, 20);
  regressionTestSqlFile("1ZoneEvapCooler-V9-1-0.sql", 43.28, 20, 20);
  regressionTestSqlFile("1ZoneEvapCooler-V9-2-0.sql", 43.28, 20, 20);
  regressionTestSqlFile("1ZoneEvapCooler-V9-3-0.sql", 43.28, 20, 20);
  regressionTestSqlFile("1ZoneEvapCooler-V9-4-0.sql", 43.28, 20, 20);
}

TEST_F(SqlFileFixture, SqlFile_LeapYear) {
  openstudio::path outfile = openstudio::tempDir() / openstudio::toPath("OpenStudioSqlFileTestLeapYear.sql");
  if (openstudio::filesystem::exists(outfile)) {
    openstudio::filesystem::remove(outfile);
  }

  // picking 2012 because it's a Leap Year
  int year = 2012;

  openstudio::Calendar c(year);
  c.standardHolidays();

  // 31 days in january, 29 in feb
  int n_vals = 24 * (31 + 29);
  std::vector<double> values;
  values.resize(n_vals, 100);

  TimeSeries timeSeries(c.startDate(), openstudio::Time(0, 1), openstudio::createVector(values), "lux");

  // Inject this timeseries into the sql
  {
    openstudio::SqlFile sql(outfile, openstudio::EpwFile(resourcesPath() / toPath("utilities/Filetypes/leapday-test.epw")),
                            openstudio::DateTime::now(), c);

    EXPECT_TRUE(sql.connectionOpen());

    sql.insertTimeSeriesData("Sum", "Zone", "Zone", "DAYLIGHTING WINDOW", "Daylight Luminance", openstudio::ReportingFrequency::Hourly,
                             boost::optional<std::string>(), "lux", timeSeries);
  }

  // Retrieve and check
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
    // That's the key part!
    EXPECT_EQ(year, ts->firstReportDateTime().date().year());

    std::vector<DateTime> original_datetimes = ts->dateTimes();
    std::vector<DateTime> reloaded_datetimes = timeSeries.dateTimes();

    EXPECT_EQ(DateTime(Date(MonthOfYear::Feb, 29, year), Time(0, 24, 0, 0)), reloaded_datetimes.end()[-1]);
    EXPECT_EQ(DateTime(Date(MonthOfYear::Feb, 29, year), Time(0, 23, 0, 0)), reloaded_datetimes.end()[-2]);
    EXPECT_EQ(original_datetimes, reloaded_datetimes);
  }
}

TEST_F(SqlFileFixture, SqlFile_Escapes_injection) {
  openstudio::path outfile = openstudio::tempDir() / openstudio::toPath("OpenStudioSqlFileTest_escapes.sql");
  if (openstudio::filesystem::exists(outfile)) {
    openstudio::filesystem::remove(outfile);
  }

  openstudio::Calendar c(2012);
  c.standardHolidays();

  std::vector<double> values;
  values.push_back(100);
  values.push_back(10);
  values.push_back(1);
  values.push_back(100.5);

  TimeSeries timeSeries(c.startDate(), openstudio::Time(0, 1), openstudio::createVector(values), "lux");

  {
    openstudio::SqlFile sql(outfile, openstudio::EpwFile(resourcesPath() / toPath("utilities/Filetypes/USA_CO_Golden-NREL.724666_TMY3.epw")),
                            openstudio::DateTime::now(), c);

    ASSERT_TRUE(sql.connectionOpen());

    sql.insertTimeSeriesData("Sum", "Zone", "Zone", "DAYLIGHTING WINDOW", "Daylight Luminance", openstudio::ReportingFrequency::Hourly,
                             boost::optional<std::string>(), "lux", timeSeries);

    sql.insertZone("CLASSROOM", 0, 0, 0, 0, 1, 1, 1, 3, 1, 1, 0, 2, 0, 2, 0, 2, 2, 8, 3, 3, 4, 4, 2, 2, true);

    auto zoneNames = sql.execAndReturnVectorOfString("SELECT ZoneName from Zones");
    ASSERT_TRUE(zoneNames);
    EXPECT_EQ(1u, zoneNames->size());
  }

  {
    openstudio::SqlFile sql(outfile);
    EXPECT_TRUE(sql.connectionOpen());

    std::vector<std::string> envPeriods = sql.availableEnvPeriods();
    EXPECT_EQ(1u, envPeriods.size());

    auto _s = sql.execAndReturnFirstString("SELECT name FROM sqlite_master WHERE type='table' AND name='EnvironmentPeriods';");
    EXPECT_TRUE(_s) << "Table EnvironmentPeriods doesn't exists";

    //  This finishes the 'insert into zones' statement; calls drop table; then adds a comment so the rest is ignored. Classic SQL Injection
    std::string zoneName{
      "Little Bobby Tables', 0, 0, 0, 0, 1, 1, 1, 3, 1, 1, 0, 2, 0, 2, 0, 2, 2, 8, 3, 3, 4, 4, 2, 2, 1); DROP TABLE EnvironmentPeriods;--"};
    EXPECT_NO_THROW(sql.insertZone(zoneName, 0, 0, 0, 0, 1, 1, 1, 3, 1, 1, 0, 2, 0, 2, 0, 2, 2, 8, 3, 3, 4, 4, 2, 2, true));
  }

  {
    openstudio::SqlFile sql(outfile);
    EXPECT_TRUE(sql.connectionOpen());

    std::vector<std::string> envPeriods = sql.availableEnvPeriods();
    EXPECT_EQ(1u, envPeriods.size());

    auto _s = sql.execAndReturnFirstString("SELECT name FROM sqlite_master WHERE type='table' AND name='EnvironmentPeriods';");
    EXPECT_TRUE(_s) << "Table EnvironmentPeriods doesn't exists";

    auto zoneNames = sql.execAndReturnVectorOfString("SELECT ZoneName from Zones");
    ASSERT_TRUE(zoneNames);
    EXPECT_EQ(2u, zoneNames->size());
  }
}

TEST_F(SqlFileFixture, EndUseFuelTypes_test) {

  openstudio::path idfPath = resourcesPath() / toPath("energyplus/AllFuelTypes/in.idf");
  auto _w = openstudio::Workspace::load(idfPath, openstudio::IddFileType::EnergyPlus);
  ASSERT_TRUE(_w);
  Workspace w = _w.get();

  openstudio::path path = resourcesPath() / toPath("energyplus/AllFuelTypes/eplusout.sql");
  openstudio::SqlFile sqlFile(path);

  auto get_design_value = [&w](const openstudio::EndUseFuelType& end_use_fuel_type) -> double {
    double result = 0;
    // valueNames match between FuelType and EndUseFuelType...
    openstudio::FuelType fuel_type(end_use_fuel_type.valueName());

    if (end_use_fuel_type == openstudio::EndUseFuelType::Water) {
      auto idf_objects = w.getObjectsByType(openstudio::IddObjectType::Exterior_WaterEquipment);
      result = idf_objects[0].getDouble(openstudio::Exterior_WaterEquipmentFields::DesignLevel).get();
    } else {
      auto idf_objects = w.getObjectsByType(openstudio::IddObjectType::Exterior_FuelEquipment);
      auto it = std::find_if(idf_objects.begin(), idf_objects.end(), [&fuel_type](const auto& idf_object) {
        return (idf_object.getString(Exterior_FuelEquipmentFields::FuelUseType).get() == fuel_type.valueDescription());
      });
      result = it->getDouble(Exterior_FuelEquipmentFields::DesignLevel).get();
    }

    return result;
  };

  std::map<openstudio::MonthOfYear, int> daysInMonth = {
    {MonthOfYear::Jan, 31}, {MonthOfYear::Feb, 28}, {MonthOfYear::Mar, 31}, {MonthOfYear::Apr, 30}, {MonthOfYear::May, 31}, {MonthOfYear::Jun, 30},
    {MonthOfYear::Jul, 31}, {MonthOfYear::Aug, 31}, {MonthOfYear::Sep, 30}, {MonthOfYear::Oct, 31}, {MonthOfYear::Nov, 30}, {MonthOfYear::Dec, 31},
  };

  // This will test that queries do not throw at least
  for (const auto& [i_month, monthDescription] : openstudio::MonthOfYear::getDescriptions()) {
    if (i_month > 12) {
      break;
    }
    openstudio::MonthOfYear monthOfYear(i_month);
    // for (const auto& [i_category, categoryDescription] : openstudio::EndUseCategoryType::getDescriptions()) {
    //openstudio::EndUseCategoryType category(i_category);
    openstudio::EndUseCategoryType category = openstudio::EndUseCategoryType::ExteriorEquipment;
    std::string categoryDescription = category.valueDescription();

    for (const auto& [i_fuelType, fuelTypeDescription] : openstudio::EndUseFuelType::getDescriptions()) {
      openstudio::EndUseFuelType fuelType(i_fuelType);

      double designLevel = get_design_value(fuelType);

      auto _v = sqlFile.energyConsumptionByMonth(fuelType, category, monthOfYear);
      ASSERT_TRUE(_v) << "energyConsumptionByMonth failed for " << fuelTypeDescription << ", " << categoryDescription << ", " << monthDescription;
      EXPECT_DOUBLE_EQ(designLevel * 24 * 3600 * daysInMonth[monthOfYear], _v.get())
        << "energyConsumptionByMonth value failed for " << fuelTypeDescription << ", " << categoryDescription << ", " << monthDescription;
      auto _v_peak = sqlFile.peakEnergyDemandByMonth(fuelType, category, monthOfYear);
      ASSERT_TRUE(_v_peak) << "peakEnergyDemandByMonth failed for " << fuelTypeDescription << ", " << categoryDescription << ", " << monthDescription;
      EXPECT_EQ(designLevel, _v_peak.get());
    }
    //}
  }
}
