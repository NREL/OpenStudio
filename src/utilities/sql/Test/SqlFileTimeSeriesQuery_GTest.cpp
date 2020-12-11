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

void environmentPeriodGeneralTests(const SqlFile& file, const SqlFileTimeSeriesQueryVector& allQueries) {
  // list of unique periods from vector should match that from file
  StringSet envPeriodsFromVector = environmentPeriods(allQueries);
  StringVector envPeriodsFromFile = file.availableEnvPeriods();
  StringSet envSetFromFile(envPeriodsFromFile.begin(), envPeriodsFromFile.end());
  EXPECT_TRUE(envSetFromFile == envPeriodsFromVector);
}

void reportingFrequencyGeneralTests(SqlFile& file, const SqlFileTimeSeriesQueryVector& allQueries) {
  // list of reporting frequencies from file should match .valueDescriptions() from vector
  StringSet rfStrs;
  for (const std::string& envPeriod : file.availableEnvPeriods()) {
    StringVector rfStrsForEnv = file.availableReportingFrequencies(envPeriod);
    rfStrs.insert(rfStrsForEnv.begin(), rfStrsForEnv.end());
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
  StringSet tsSetFromFile(tsNamesFromFile.begin(), tsNamesFromFile.end());
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

  environmentPeriodGeneralTests(file, allQueries);
  reportingFrequencyGeneralTests(file, allQueries);
  timeSeriesGeneralTests(file, allQueries);
  keyValuesGeneralTests(file, allQueries);
}

TEST_F(SqlFileFixture, SqlFileTimeSeriesQuery_GeneralTests) {
  SCOPED_TRACE("SqlFileTimeSeriesQuery_GeneralTests");
  sqlFileTimeSeriesQueryGeneralTests(sqlFile);
}
