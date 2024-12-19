/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <iomanip>
#include "SqlFileTimeSeriesQuery.hpp"

#include "../core/Assert.hpp"
#include "../core/Optional.hpp"
#include "../core/Containers.hpp"

namespace openstudio {

// ENVIRONMENT IDENTIFIER

EnvironmentIdentifier::EnvironmentIdentifier(const EnvironmentType& type) : m_type(type) {}

EnvironmentIdentifier::EnvironmentIdentifier(const std::string& name) : m_name(name) {}

OptionalEnvironmentType EnvironmentIdentifier::type() const {
  return m_type;
}

OptionalString EnvironmentIdentifier::name() const {
  return m_name;
}

// TIMESERIES IDENTIFIER

TimeSeriesIdentifier::TimeSeriesIdentifier(const std::string& name) : m_name(name) {}

TimeSeriesIdentifier::TimeSeriesIdentifier(const boost::regex& re) : m_re(re) {}

OptionalString TimeSeriesIdentifier::name() const {
  return m_name;
}

boost::optional<boost::regex> TimeSeriesIdentifier::regex() const {
  return m_re;
}

// KEYVALUE IDENTIFIER

KeyValueIdentifier::KeyValueIdentifier(const std::string& name) : m_names(1, name) {}

KeyValueIdentifier::KeyValueIdentifier(const std::vector<std::string>& names) : m_names(names) {
  if (m_names.empty()) {
    LOG_AND_THROW("KeyValueIdentifier cannot be constructed from an empty string vector. " << "Please use an OptionalKeyValueIdentifier instead.");
  }
}

KeyValueIdentifier::KeyValueIdentifier(const boost::regex& re) : m_re(re) {}

StringVector KeyValueIdentifier::names() const {
  return m_names;
}

boost::optional<boost::regex> KeyValueIdentifier::regex() const {
  if (!m_names.empty()) {
    return boost::none;
  }
  return m_re;
}

// SQLFILE TIMESERIES QUERY

SqlFileTimeSeriesQuery::SqlFileTimeSeriesQuery(const boost::optional<EnvironmentIdentifier>& oEnvId, const boost::optional<ReportingFrequency>& oRF,
                                               const boost::optional<TimeSeriesIdentifier>& oTsId, const boost::optional<KeyValueIdentifier>& oKvId)
  : m_vetted(false), m_environment(oEnvId), m_reportingFrequency(oRF), m_timeSeries(oTsId), m_keyValues(oKvId) {}

SqlFileTimeSeriesQuery::SqlFileTimeSeriesQuery(const EnvironmentIdentifier& envId) : m_vetted(false), m_environment(envId) {}

SqlFileTimeSeriesQuery::SqlFileTimeSeriesQuery(const std::string& environmentPeriod, const ReportingFrequency& reportingFrequency,
                                               const std::string& timeSeriesName, const std::string& keyValue)
  : m_vetted(false),
    m_environment(EnvironmentIdentifier(environmentPeriod)),
    m_reportingFrequency(reportingFrequency),
    m_timeSeries(TimeSeriesIdentifier(timeSeriesName)),
    m_keyValues(KeyValueIdentifier(keyValue)) {}

// GETTERS

OptionalEnvironmentIdentifier SqlFileTimeSeriesQuery::environment() const {
  return m_environment;
}

OptionalReportingFrequency SqlFileTimeSeriesQuery::reportingFrequency() const {
  return m_reportingFrequency;
}

OptionalTimeSeriesIdentifier SqlFileTimeSeriesQuery::timeSeries() const {
  return m_timeSeries;
}

OptionalKeyValueIdentifier SqlFileTimeSeriesQuery::keyValues() const {
  return m_keyValues;
}

bool SqlFileTimeSeriesQuery::vetted() const {
  return m_vetted;
}

// SETTERS

void SqlFileTimeSeriesQuery::setEnvironment(const EnvironmentIdentifier& envId) {
  m_environment = envId;
  m_vetted = false;
}

void SqlFileTimeSeriesQuery::setReportingFrequency(const ReportingFrequency& rf) {
  m_reportingFrequency = rf;
  m_vetted = false;
}

void SqlFileTimeSeriesQuery::setTimeSeries(const TimeSeriesIdentifier& tsId) {
  m_timeSeries = tsId;
  m_vetted = false;
}

void SqlFileTimeSeriesQuery::setKeyValues(const KeyValueIdentifier& kvId) {
  m_keyValues = kvId;
  m_vetted = false;
}

void SqlFileTimeSeriesQuery::clearEnvironment() {
  m_environment = boost::none;
  m_vetted = false;
}

void SqlFileTimeSeriesQuery::clearReportingFrequency() {
  m_reportingFrequency = boost::none;
  m_vetted = false;
}

void SqlFileTimeSeriesQuery::clearTimeSeries() {
  m_timeSeries = boost::none;
  m_vetted = false;
}

void SqlFileTimeSeriesQuery::clearKeyValues() {
  m_keyValues = boost::none;
  m_vetted = false;
}

// NON-MEMBER FUNCTIONS

std::ostream& operator<<(std::ostream& os, const SqlFileTimeSeriesQuery& query) {
  os << '\n';
  if (query.environment()) {
    EnvironmentIdentifier envId = query.environment().get();
    if (envId.type()) {
      os << "Environment Period:  Of type '" << envId.type().get().valueDescription() << "'" << '\n';
    } else {
      os << "Environment Period:  " << envId.name().get() << '\n';
    }
  } else {
    os << "Environment Period:  Not Specified" << '\n';
  }
  if (query.reportingFrequency()) {
    os << "Reporting Frequency: " << query.reportingFrequency().get().valueDescription() << '\n';
  } else {
    os << "Reporting Frequency: Not Specified" << '\n';
  }
  if (query.timeSeries()) {
    TimeSeriesIdentifier tsId = query.timeSeries().get();
    if (tsId.regex()) {
      os << "Time Series:         Match regex '" << tsId.regex().get() << "'" << '\n';
    } else {
      os << "Time Series:         " << tsId.name().get() << '\n';
    }
  } else {
    os << "Time Series:         Not Specified" << '\n';
  }
  StringVector names;
  if (query.keyValues()) {
    KeyValueIdentifier kvId = query.keyValues().get();
    if (kvId.regex()) {
      os << "Key Values:          Match regex '" << kvId.regex().get() << "'" << '\n';
    } else {
      names = kvId.names();
      if (names.size() == 1) {
        os << "Key Value:           " << names[0] << '\n';
      } else {
        OS_ASSERT(names.size() > 1);
        os << "Key Values:          " << names[0] << '\n';
        for (unsigned i = 1, n = names.size(); i < n; ++i) {
          os << std::setw(21) << " " << names[i] << '\n';
        }
      }
    }
  } else {
    os << "Key Value:           Not Specified" << '\n';
  }
  os << '\n';

  return os;
}

StringSet environmentPeriods(const SqlFileTimeSeriesQueryVector& queries) {
  StringSet result;
  for (const SqlFileTimeSeriesQuery& q : queries) {
    if (!q.vetted()) {
      return {};
    }
    OS_ASSERT(q.environment().get().name());
    result.insert(*(q.environment().get().name()));
  }
  return result;
}

ReportingFrequencySet reportingFrequencies(const SqlFileTimeSeriesQueryVector& queries) {
  ReportingFrequencySet result;
  for (const SqlFileTimeSeriesQuery& q : queries) {
    if (!q.vetted()) {
      return {};
    }
    OS_ASSERT(q.reportingFrequency());
    result.insert(*(q.reportingFrequency()));
  }
  return result;
}

StringSet timeSeriesNames(const SqlFileTimeSeriesQueryVector& queries) {
  StringSet result;
  for (const SqlFileTimeSeriesQuery& q : queries) {
    if (!q.vetted()) {
      return {};
    }
    OS_ASSERT(q.timeSeries().get().name());
    result.insert(*(q.timeSeries().get().name()));
  }
  return result;
}

}  // namespace openstudio
