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

#ifndef UTILITIES_SQL_SQLFILETIMESERIESQUERY_HPP
#define UTILITIES_SQL_SQLFILETIMESERIESQUERY_HPP

#include "../UtilitiesAPI.hpp"
#include "SqlFileEnums.hpp"

#include "../core/Logger.hpp"

#include <boost/optional.hpp>
#include <boost/regex.hpp>

#include <string>
#include <vector>
#include <ostream>

namespace openstudio {

// forward declarations
namespace detail {
  class SqlFile_Impl;
}

/** Small class to let users identify a SqlFile environment period either by type or by name.
 *  If type is specified, there is a chance the environment name will not be unique. */
class UTILITIES_API EnvironmentIdentifier {
 public:

   /** Construct from EnvironmentType enumeration. */
  EnvironmentIdentifier(const openstudio::EnvironmentType& type=EnvironmentType());
  /** Construct from name. */
  EnvironmentIdentifier(const std::string& name);

  /** Return the EnvironmentType, if it is specified. */
  boost::optional<openstudio::EnvironmentType> type() const;
  /** Return the environment period name, if it is specified. */
  boost::optional<std::string> name() const;

 private:
  boost::optional<EnvironmentType> m_type;
  boost::optional<std::string> m_name;
};

typedef boost::optional<openstudio::EnvironmentIdentifier> OptionalEnvironmentIdentifier;

/** Small class to let users identify a time series by name, or a set of time
 *  series by regex. */
class UTILITIES_API TimeSeriesIdentifier {
 public:

  /** Construct from name. */
  TimeSeriesIdentifier(const std::string& name);
  /** Construct from regex. */
  TimeSeriesIdentifier(const boost::regex& re);

  /** Get name, if specified. */
  boost::optional<std::string> name() const;
  /** Get regex, if specified. */
  boost::optional<boost::regex> regex() const;

 private:
  boost::optional<std::string> m_name;
  boost::optional<boost::regex> m_re;
};

typedef boost::optional<openstudio::TimeSeriesIdentifier> OptionalTimeSeriesIdentifier;

/** Small class to let users identify one or more key values by name or regex. */
class UTILITIES_API KeyValueIdentifier {
 public:

  /** Construct from single name. */
  KeyValueIdentifier(const std::string& name);
  /** Construct from multiple names. Throws if names.size() == 0. */
  KeyValueIdentifier(const std::vector<std::string>& names);
  /** Construct from regex. */
  KeyValueIdentifier(const boost::regex& re);

  /** Get name(s). */
  std::vector<std::string> names() const;
  /** Get regex, if it exists. */
  boost::optional<boost::regex> regex() const;

 private:
  std::vector<std::string> m_names;
  boost::regex m_re;

  REGISTER_LOGGER("openstudio.KeyValueIdentifier");
};

typedef boost::optional<openstudio::KeyValueIdentifier> OptionalKeyValueIdentifier;

/** Class to hold information about one or more SqlFile time series of interest. The query is
 *  limited to one ReportingFrequency so that the resulting TimeSeries.values() Vectors are of the
 *  same length. This class is intended to serve as a way to streamline the SqlFile interface, and
 *  also as data storage for classes like analysis::SqlTimeSeriesVariableAccessor. */
class UTILITIES_API SqlFileTimeSeriesQuery {
 public:

  /** @name Constructors */
  //@{

  /** Constructor from optionals. Environment period, reporting frequency, time series, and key
   *  values can all be specified (or not). */
  SqlFileTimeSeriesQuery(const boost::optional<openstudio::EnvironmentIdentifier>& oEnvId = boost::none,
                         const boost::optional<openstudio::ReportingFrequency>& oRF = boost::none,
                         const boost::optional<openstudio::TimeSeriesIdentifier>& oTsId = boost::none,
                         const boost::optional<openstudio::KeyValueIdentifier>& oKvId = boost::none);

  /** Constructor from EnvironmentIdentifier. */
  SqlFileTimeSeriesQuery(const EnvironmentIdentifier& envId);

  /** Construct from fully specified query. */
  SqlFileTimeSeriesQuery(const std::string& environmentPeriod,
                         const openstudio::ReportingFrequency& reportingFrequency,
                         const std::string& timeSeriesName,
                         const std::string& keyValue);

  //@}
  /** @name Getters */
  //@{

  /** Get the EnvironmentIdentifier. */
  boost::optional<openstudio::EnvironmentIdentifier> environment() const;

  /** Get the ReportingFrequency. */
  boost::optional<openstudio::ReportingFrequency> reportingFrequency() const;

  /** Get the time series identifier. */
  boost::optional<openstudio::TimeSeriesIdentifier> timeSeries() const;

  /** Get the key value identifier. */
  boost::optional<openstudio::KeyValueIdentifier> keyValues() const;

  /** Returns true if this query has been vetted (expended) by a SqlFile. */
  bool vetted() const;

  //@}
  /** @name Setters */
  //@{

  /** Set the EnvironmentIdentifier. */
  void setEnvironment(const EnvironmentIdentifier& envId);

  /** Set the ReportingFrequency. */
  void setReportingFrequency(const openstudio::ReportingFrequency& rf);

  /** Set the TimeSeries NameIdentifier. */
  void setTimeSeries(const TimeSeriesIdentifier& tsId);

  /** Set the KeyValues NameIdentifier. */
  void setKeyValues(const KeyValueIdentifier& kvId);

  /** Remove any EnvironmentIdentifier. */
  void clearEnvironment();

  /** Remove any ReportingFrequency. */
  void clearReportingFrequency();

  /** Remove any TimeSeries. */
  void clearTimeSeries();

  /** Remove any KeyValues. */
  void clearKeyValues();

  //@}

 protected:
  // flag that is flipped by setters and SqlFile::expandQuery
  friend class detail::SqlFile_Impl;
  bool m_vetted;

 private:
  boost::optional<EnvironmentIdentifier> m_environment;
  boost::optional<ReportingFrequency> m_reportingFrequency;
  boost::optional<TimeSeriesIdentifier> m_timeSeries;
  boost::optional<KeyValueIdentifier> m_keyValues;
};

UTILITIES_API std::ostream& operator<<(std::ostream& os,const SqlFileTimeSeriesQuery& query);

typedef std::vector<SqlFileTimeSeriesQuery> SqlFileTimeSeriesQueryVector;

/** If all queries have been vetted, returns the set of unique environment names. Otherwise,
 *  returns an empty set. */
UTILITIES_API std::set<std::string> environmentPeriods(const std::vector<SqlFileTimeSeriesQuery>& queries);

/** If all queries have been vetted, returns the set of unique \link ReportingFrequency
 *  ReportingFrequencies \endlink. Otherwise, returns an empty set. */
UTILITIES_API std::set<openstudio::ReportingFrequency> reportingFrequencies(const std::vector<openstudio::SqlFileTimeSeriesQuery>& queries);

/** If all queries have been vetted, returns the set of unique time series names. Otherwise,
 *  returns an empty set. */
UTILITIES_API std::set<std::string> timeSeriesNames(const std::vector<SqlFileTimeSeriesQuery>& queries);

} // openstudio

#endif // UTILITIES_SQL_SQLFILETIMESERIESQUERY_HPP
