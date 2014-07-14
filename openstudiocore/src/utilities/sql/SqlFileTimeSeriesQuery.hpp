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
