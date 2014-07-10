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

#include "SqlFileFixture.hpp"

#include "../SqlFileTimeSeriesQuery.hpp"
#include "../SqlFileEnums.hpp"
#include "../../data/TimeSeries.hpp"

#include "../../core/Containers.hpp"

#include <resources.hxx>

using openstudio::SqlFile;
using openstudio::SqlFileTimeSeriesQuery;
using openstudio::SqlFileTimeSeriesQueryVector;
using openstudio::EnvironmentIdentifier;
using openstudio::ReportingFrequency;
using openstudio::ReportingFrequencySet;

using openstudio::StringVector;
using openstudio::StringSet;

void environmentPeriodGeneralTests(SqlFile& file, const SqlFileTimeSeriesQueryVector& allQueries) {
  // list of unique periods from vector should match that from file
  StringSet envPeriodsFromVector = environmentPeriods(allQueries);
  StringVector envPeriodsFromFile = file.availableEnvPeriods();
  StringSet envSetFromFile(envPeriodsFromFile.begin(),envPeriodsFromFile.end());
  EXPECT_TRUE(envSetFromFile == envPeriodsFromVector);
}

void reportingFrequencyGeneralTests(SqlFile& file, const SqlFileTimeSeriesQueryVector& allQueries) {
  // list of reporting frequencies from file should match .valueDescriptions() from vector
  StringSet rfStrs;
  for (const std::string& envPeriod : file.availableEnvPeriods()) {
    StringVector rfStrsForEnv = file.availableReportingFrequencies(envPeriod);
    rfStrs.insert(rfStrsForEnv.begin(),rfStrsForEnv.end());
  }
  ReportingFrequencySet rfSetFromVector = reportingFrequencies(allQueries);
  StringSet rfStrsFromVector;
  for (const ReportingFrequency& rf : rfSetFromVector) {
    rfStrsFromVector.insert(rf.valueDescription());
  }
  EXPECT_TRUE(rfStrs.size() >= rfStrsFromVector.size());
  for (const std::string& rfStr : rfStrsFromVector) {
    EXPECT_TRUE(rfStrs.find(rfStr) != rfStrs.end());
  }
}

void timeSeriesGeneralTests(SqlFile& file, const SqlFileTimeSeriesQueryVector& allQueries) {
  // list of time series from vector should be same as all available from file
  StringSet tsNames = timeSeriesNames(allQueries);
  StringVector tsNamesFromFile = file.availableTimeSeries();
  StringSet tsSetFromFile(tsNamesFromFile.begin(),tsNamesFromFile.end());
  EXPECT_TRUE(tsNames == tsSetFromFile);
}

void keyValuesGeneralTests(SqlFile& file, const SqlFileTimeSeriesQueryVector& allQueries) {
  // key value request should not be explicit in any of allQueries
  for (const SqlFileTimeSeriesQuery& q : allQueries) {
    EXPECT_FALSE(q.keyValues());
  }
} 

void sqlFileTimeSeriesQueryGeneralTests(SqlFile& file) {
  SqlFileTimeSeriesQuery everythingQuery;
  SqlFileTimeSeriesQueryVector allQueries = file.expandQuery(everythingQuery);

  environmentPeriodGeneralTests(file,allQueries);
  reportingFrequencyGeneralTests(file,allQueries);
  timeSeriesGeneralTests(file,allQueries);
  keyValuesGeneralTests(file,allQueries);
}

TEST_F(SqlFileFixture,SqlFileTimeSeriesQuery_GeneralTests) {
  SCOPED_TRACE("SqlFileTimeSeriesQuery_GeneralTests");
  sqlFileTimeSeriesQueryGeneralTests(sqlFile);
}
