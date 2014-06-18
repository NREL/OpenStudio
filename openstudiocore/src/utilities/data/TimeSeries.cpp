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

#include "TimeSeries.hpp"
#include "../core/Assert.hpp"

#include <exception>
#include <set>

using namespace std;
using namespace boost;

namespace openstudio{

  namespace detail{

    /// default constructor
    TimeSeries_Impl::TimeSeries_Impl():m_outOfRangeValue(0.0)
    {}

    /// constructor from start date, interval length, and values
    /// first reporting interval ends at Date + Time(0) + intervalLength
    TimeSeries_Impl::TimeSeries_Impl(const Date& startDate, const Time& intervalLength, const Vector& values, const std::string& units)
      : m_secondsFromFirstReport(values.size()), m_values(values), m_units(units), m_intervalLength(intervalLength), m_outOfRangeValue(0.0), m_wrapAround(false)
    {
      if (values.empty()){
        LOG(Warn, "Creating empty timeseries");
      }

      // length of interval in seconds
      int secondsPerInterval = intervalLength.totalSeconds();

      // date and time of first report, end of the first reporting interval
      // DLM: startDate may or may not have baseYear defined
      m_firstReportDateTime=DateTime(startDate,intervalLength); 

      for (unsigned i = 0; i < values.size(); ++i){
        m_secondsFromFirstReport[i] = i*secondsPerInterval;
      }
      m_secondsFromFirstReportAsVector = createVector(m_secondsFromFirstReport);

      long durationSeconds = 0;
      if (!m_secondsFromFirstReport.empty()){
        durationSeconds = m_secondsFromFirstReport.back();
      }

      // check for wrap around
      boost::optional<int> calendarYear = m_firstReportDateTime.date().baseYear();
      if (!calendarYear){
        DateTime lastDateTime = m_firstReportDateTime.date() + Time(0,0,0,durationSeconds);
        Date lastDate(lastDateTime.date().monthOfYear(), lastDateTime.date().dayOfMonth());
        if ((durationSeconds > Time(366.0).totalSeconds()) || (lastDate < m_firstReportDateTime.date())){
          m_wrapAround = true;
        }
      }
    }

    /// constructor from start date and time, interval length, and values
    /// first reporting interval ends at startDateTime
    TimeSeries_Impl::TimeSeries_Impl(const DateTime& startDateTime, const Time& intervalLength, const Vector& values, const std::string& units)
      :  m_secondsFromFirstReport(values.size()), m_values(values), m_units(units), m_intervalLength(intervalLength), m_outOfRangeValue(0.0), m_wrapAround(false)
    {
      if (values.empty()){
        LOG(Warn, "Creating empty timeseries");
      }

      // length of interval in seconds
      int secondsPerInterval = intervalLength.totalSeconds();

      // DLM: startDate may or may not have baseYear defined
      m_firstReportDateTime = DateTime(startDateTime.date(), startDateTime.time());

      for (unsigned i = 0; i < values.size(); ++i){
        m_secondsFromFirstReport[i] = i*secondsPerInterval;
      }
      m_secondsFromFirstReportAsVector = createVector(m_secondsFromFirstReport);

      long durationSeconds = 0;
      if (!m_secondsFromFirstReport.empty()){
        durationSeconds = m_secondsFromFirstReport.back();
      }

      // check for wrap around
      boost::optional<int> calendarYear = m_firstReportDateTime.date().baseYear();
      if (!calendarYear){
        DateTime lastDateTime = m_firstReportDateTime.date() +  Time(0,0,0,durationSeconds);
        Date lastDate(lastDateTime.date().monthOfYear(), lastDateTime.date().dayOfMonth());
        if ((durationSeconds > Time(366.0).totalSeconds()) || (lastDate < m_firstReportDateTime.date())){
          m_wrapAround = true;
        }
      }
    }


    /// constructor from first report date and time, days from first report vector, values, and units
    TimeSeries_Impl::TimeSeries_Impl(const DateTime& firstReportDateTime, const Vector& daysFromFirstReport, const Vector& values, const std::string& units)
      :  m_secondsFromFirstReport(values.size()), m_values(values), m_units(units), m_outOfRangeValue(0.0), m_wrapAround(false)
    {
      if (values.empty()){
        LOG(Warn, "Creating empty timeseries");
      }

      if (daysFromFirstReport.size() != values.size()){
        LOG_AND_THROW("Length of values must match length of times");
      }

      // DLM: firstReportDateTime may or may not have baseYear defined
      m_firstReportDateTime = firstReportDateTime;

      for (unsigned i = 0; i < values.size(); ++i){
        m_secondsFromFirstReport[i] = Time(daysFromFirstReport(i)).totalSeconds(); 
        if (i > 0){
          if (m_secondsFromFirstReport[i] < m_secondsFromFirstReport[i-1]){
            LOG_AND_THROW("Days from first report must be monotonically increasing");
          }
        }
      }
      m_secondsFromFirstReportAsVector = createVector(m_secondsFromFirstReport);

      long durationSeconds = 0;
      if (!m_secondsFromFirstReport.empty()){
        durationSeconds = m_secondsFromFirstReport.back();
      }

      // check for wrap around
      boost::optional<int> calendarYear = m_firstReportDateTime.date().baseYear();
      if (!calendarYear){
        DateTime lastDateTime = m_firstReportDateTime.date() + Time(0,0,0,durationSeconds);
        Date lastDate(lastDateTime.date().monthOfYear(), lastDateTime.date().dayOfMonth());
        if ((durationSeconds > Time(366.0).totalSeconds()) || (lastDate < m_firstReportDateTime.date())){
          m_wrapAround = true;
        }
      }
    }

    TimeSeries_Impl::TimeSeries_Impl(const DateTime& firstReportDateTime, const std::vector<double>& daysFromFirstReport, const std::vector<double>& values, const std::string& units) 
      : m_secondsFromFirstReport(daysFromFirstReport.size()), m_values(values.size()), m_units(units), m_outOfRangeValue(0.0), m_wrapAround(false)
    {
      if (values.empty()){
        LOG(Warn, "Creating empty timeseries");
      }

      if (daysFromFirstReport.size() != values.size()){
        LOG_AND_THROW("Length of values must match length of times");
      }

      // DLM: firstReportDateTime may or may not have baseYear defined
      m_firstReportDateTime = firstReportDateTime;

      //        for (unsigned i = 0; i < values.size(); i++) m_values(i) = values[i];
      std::copy(values.begin(), values.end(), m_values.begin());

      for (unsigned i = 0; i < values.size(); ++i){
        m_secondsFromFirstReport[i] = Time(daysFromFirstReport[i]).totalSeconds();
        if (i > 0){
          if (m_secondsFromFirstReport[i] < m_secondsFromFirstReport[i-1]){
            LOG_AND_THROW("Days from first report must be monotonically increasing");
          }
        }
      }
      m_secondsFromFirstReportAsVector = createVector(m_secondsFromFirstReport);

      long durationSeconds = 0;
      if (!m_secondsFromFirstReport.empty()){
        durationSeconds = m_secondsFromFirstReport.back();
      }
    
      // check for wrap around
      boost::optional<int> calendarYear = m_firstReportDateTime.date().baseYear();
      if (!calendarYear){
        DateTime lastDateTime = m_firstReportDateTime.date() + Time(0,0,0,durationSeconds);
        Date lastDate(lastDateTime.date().monthOfYear(), lastDateTime.date().dayOfMonth());
        if ((durationSeconds > Time(366.0).totalSeconds()) || (lastDate < m_firstReportDateTime.date())){
          m_wrapAround = true;
        }
      }
    }

    /// constructor from date times, values, and units
    TimeSeries_Impl::TimeSeries_Impl(const DateTimeVector& dateTimes, const Vector& values, const std::string& units)
      : m_secondsFromFirstReport(dateTimes.size()), m_values(values), m_units(units), m_outOfRangeValue(0.0), m_wrapAround(false)
    {
      if (values.empty()){
        LOG(Warn, "Creating empty timeseries");
      }

      if (dateTimes.size() != values.size()){
        LOG_AND_THROW("Length of values must match length of times");
      }

      // DLM: startDate may or may not have baseYear defined
      m_firstReportDateTime = dateTimes.front();
      unsigned numDateTimes = dateTimes.size();
      boost::optional<int> calendarYear = m_firstReportDateTime.date().baseYear();

      DateTime firstReportDateTimeWithYear = m_firstReportDateTime;
      if (!calendarYear){
        // add year
        firstReportDateTimeWithYear = DateTime(Date(m_firstReportDateTime.date().monthOfYear(), m_firstReportDateTime.date().dayOfMonth(), m_firstReportDateTime.date().year()), m_firstReportDateTime.time());
      }
      
      for (unsigned i = 0; i < numDateTimes; ++i){
      
        DateTime dateTime = dateTimes[i];
        if (!calendarYear && dateTime.date().baseYear()){
          // remove year for comparison with m_firstReportDateTime
          dateTime = DateTime(Date(dateTime.date().monthOfYear(), dateTime.date().dayOfMonth()), dateTime.time());
        }

        // check for wrap around
        if (!calendarYear && (dateTime < m_firstReportDateTime)){
          m_wrapAround = true;
          DateTime wrappedDateTime = DateTime(Date(dateTime.date().monthOfYear(), dateTime.date().dayOfMonth(), m_firstReportDateTime.date().year() + 1), dateTime.time());
          m_secondsFromFirstReport[i] = (wrappedDateTime-firstReportDateTimeWithYear).totalSeconds();
        }else{
          m_secondsFromFirstReport[i] = (dateTime-m_firstReportDateTime).totalSeconds();
        }

        if (i > 0){
          if (m_secondsFromFirstReport[i] < m_secondsFromFirstReport[i-1]){
            LOG_AND_THROW("Days from first report must be monotonically increasing");
          }
        }
      }
      m_secondsFromFirstReportAsVector = createVector(m_secondsFromFirstReport);
    }

    /// constructor from first report date and time, seconds from first report vector, values, and units
    TimeSeries_Impl::TimeSeries_Impl(const DateTime& firstReportDateTime, const std::vector<long>& secondsFromFirstReport, const Vector& values, const std::string& units)
      :  m_secondsFromFirstReport(values.size()), m_values(values), m_units(units), m_outOfRangeValue(0.0), m_wrapAround(false)
    {
      if (values.empty()){
        LOG(Warn, "Creating empty timeseries");
      }

      if (secondsFromFirstReport.size() != values.size()){
        LOG_AND_THROW("Length of values must match length of times");
      }

      // DLM: firstReportDateTime may or may not have baseYear defined
      m_firstReportDateTime = firstReportDateTime;

      for (unsigned i = 0; i < values.size(); ++i){
        m_secondsFromFirstReport[i] = secondsFromFirstReport[i];
        if (i > 0){
          if (m_secondsFromFirstReport[i] < m_secondsFromFirstReport[i-1]){
            LOG_AND_THROW("Days from first report must be monotonically increasing");
          }
        }
      }
      m_secondsFromFirstReportAsVector = createVector(m_secondsFromFirstReport);

      long durationSeconds = 0;
      if (!m_secondsFromFirstReport.empty()){
        durationSeconds = m_secondsFromFirstReport.back();
      }

      // check for wrap around
      boost::optional<int> calendarYear = m_firstReportDateTime.date().baseYear();
      if (!calendarYear){
        DateTime lastDateTime = m_firstReportDateTime.date() + Time(0,0,0,durationSeconds);
        Date lastDate(lastDateTime.date().monthOfYear(), lastDateTime.date().dayOfMonth());
        if ((durationSeconds > Time(366.0).totalSeconds()) || (lastDate < m_firstReportDateTime.date())){
          m_wrapAround = true;
        }
      }
    }

    /// interval length if any
    OptionalTime TimeSeries_Impl::intervalLength() const
    {
      return m_intervalLength;
    }

    DateTimeVector TimeSeries_Impl::dateTimes() const
    {
      DateTimeVector dateTimeObjs(m_secondsFromFirstReport.size());
      for(unsigned i=0;i<m_secondsFromFirstReport.size();i++)
      {
        dateTimeObjs[i] = m_firstReportDateTime + openstudio::Time(0,0,0,m_secondsFromFirstReport[i]);
      }
      return dateTimeObjs;
    }


    /// time in days from end of the first reporting interval
    Vector TimeSeries_Impl::daysFromFirstReport() const 
    {
      Vector daysFromFirstReport(m_secondsFromFirstReport.size());
      for(unsigned i=0;i<m_secondsFromFirstReport.size();i++)
      {
        daysFromFirstReport[i] = Time(0,0,0,m_secondsFromFirstReport[i]).totalDays();
      }
      return daysFromFirstReport;
    }

    /// time in days from end of the first reporting interval at index i
    double TimeSeries_Impl::daysFromFirstReport(const unsigned& i) const 
    {
      double value = m_outOfRangeValue;
      if (i<m_secondsFromFirstReport.size()){
        value = Time(0,0,0,m_secondsFromFirstReport[i]).totalDays();
      }
      return value;
    }

    /// time in seconds from end of the first reporting interval
    std::vector<long> TimeSeries_Impl::secondsFromFirstReport() const 
    {
      return m_secondsFromFirstReport;
    }

    /// time in seconds from end of the first reporting interval at index i
    long TimeSeries_Impl::secondsFromFirstReport(const unsigned& i) const 
    {
      //double value = m_outOfRangeValue; // JWD: Shouldn't the out of range value be for values only?
      long value = 0;
      if (i<m_secondsFromFirstReport.size())
      {
        value = m_secondsFromFirstReport[i];
      }
      return value;
    }

    /// values
    Vector TimeSeries_Impl::values() const 
    {
      return m_values;
    }

    /// values at index i
    double TimeSeries_Impl::values(const unsigned& i) const 
    {
      double value = m_outOfRangeValue;
      if (i<m_values.size()) 
      {
        value = m_values[i];
      }
      return value;
    }

    /// units
    const std::string TimeSeries_Impl::units() const 
    {
      return m_units;
    }

    /// first report date
    openstudio::DateTime TimeSeries_Impl::firstReportDateTime() const
    {
      return m_firstReportDateTime;
    }

    /// get value at number of seconds from start date and time
    double TimeSeries_Impl::valueAtSecondsFromFirstReport(long secondsFromFirstReport) const
    {
      double result = m_outOfRangeValue;

      if (m_secondsFromFirstReport.empty()){
        LOG(Debug, "Cannot compute value because timeseries is empty");
        return result;
      }

      long duration = m_secondsFromFirstReport.back();

      if (m_intervalLength){

        // before the start of the first interval 
        if (secondsFromFirstReport <= -m_intervalLength->totalSeconds()){
          LOG(Debug, "Cannot compute value " << secondsFromFirstReport << " seconds before first reporting time when interval length is " << *m_intervalLength);
        }else if(secondsFromFirstReport > duration){
          // after end of time series
          LOG(Debug, "Cannot compute value " << secondsFromFirstReport << " seconds after first reporting time when duration is " << duration << " seconds");
        }else{
          // faster look up if know we have fixed interval
          unsigned numIntervals = secondsFromFirstReport / m_intervalLength->totalSeconds();
          unsigned remainder = secondsFromFirstReport % m_intervalLength->totalSeconds();
          unsigned index;
          if (secondsFromFirstReport < 0){
            OS_ASSERT(numIntervals == 0);
            index = numIntervals;
          }else if (remainder > 0){
            index = numIntervals+1;
          }else{
            index = numIntervals;
          }

          // issue with daily, hourly flood plots when index == m_values.size()
          if (index >= m_values.size()) {
            LOG(Warn, "Timeseries index " << index << " is greater than or equal to values size " << m_values.size() << " and has been set to size - 1.");
            index = index - 1;
          }
          result = m_values(index);
        }

      }else{

        // before the end of the first interval, do not know when start is
        if (secondsFromFirstReport < 0){
          LOG(Debug, "Cannot compute value " << secondsFromFirstReport << " seconds before first reporting time when interval length is unknown");

        }else if(secondsFromFirstReport > duration){
          // after end of time series
          LOG(Debug, "Cannot compute value " << secondsFromFirstReport << " seconds after first reporting time when duration is " << duration << " seconds");
        }else{
          // normal interpolation
          result = interp(m_secondsFromFirstReportAsVector, m_values, secondsFromFirstReport, HoldNextInterp, NoneExtrap);
        }
      }

      return result;
    }

    /// get value at number of days from start date
    double TimeSeries_Impl::value(double daysFromFirstReport) const
    {
      return valueAtSecondsFromFirstReport(Time(daysFromFirstReport).totalSeconds());
    }

    /// get value at time from first report date and time
    double TimeSeries_Impl::value(const Time& timeFromFirstReport) const
    {
      return valueAtSecondsFromFirstReport(timeFromFirstReport.totalSeconds());
    }

    /// get value at date and time
    double TimeSeries_Impl::value(const DateTime& dateTime) const
    {
      boost::optional<int> calendarYear = m_firstReportDateTime.date().baseYear();

      DateTime firstReportDateTimeWithYear = m_firstReportDateTime;
      if (!calendarYear){
        firstReportDateTimeWithYear = DateTime(Date(m_firstReportDateTime.date().monthOfYear(), m_firstReportDateTime.date().dayOfMonth(), m_firstReportDateTime.date().year()), m_firstReportDateTime.time());
      }

      DateTime dateTimeWithYear = dateTime;
      if (!dateTimeWithYear.date().baseYear()){
        dateTimeWithYear = DateTime(Date(dateTime.date().monthOfYear(), dateTime.date().dayOfMonth(), dateTime.date().year()), dateTime.time());
      }

      DateTime dateTimeCompare = dateTime;
      if (!calendarYear && dateTime.date().baseYear()){
        // remove year for comparison with m_firstReportDateTime
        dateTimeCompare = DateTime(Date(dateTime.date().monthOfYear(), dateTime.date().dayOfMonth()), dateTime.time());
      }

      // check for wrap around
      if (m_wrapAround){
        if (dateTimeCompare < m_firstReportDateTime){
          DateTime wrappedDateTime = DateTime(Date(dateTime.date().monthOfYear(), dateTime.date().dayOfMonth(), m_firstReportDateTime.date().year() + 1), dateTime.time());

          // wrapped by less than one year, use wrapped value
          return value(wrappedDateTime-firstReportDateTimeWithYear);
        }
      }

      return value(dateTimeWithYear-firstReportDateTimeWithYear);
    }

    /// get values between start and end date times
    Vector TimeSeries_Impl::values(const DateTime& startDateTime, const DateTime& endDateTime) const
    {
      boost::optional<int> calendarYear = m_firstReportDateTime.date().baseYear();

      DateTime startDateTimeCompare = startDateTime;
      if (!calendarYear && startDateTime.date().baseYear()){
        startDateTimeCompare = DateTime(Date(startDateTime.date().monthOfYear(), startDateTime.date().dayOfMonth()), startDateTime.time());
      }

      DateTime endDateTimeCompare = endDateTime;
      if (!calendarYear && endDateTime.date().baseYear()){
        endDateTimeCompare = DateTime(Date(endDateTime.date().monthOfYear(), endDateTime.date().dayOfMonth()), endDateTime.time());
      }

      DateTime firstReportDateTimeWithYear = m_firstReportDateTime;
      if (!calendarYear){
        firstReportDateTimeWithYear = DateTime(Date(m_firstReportDateTime.date().monthOfYear(), m_firstReportDateTime.date().dayOfMonth(), m_firstReportDateTime.date().year()), m_firstReportDateTime.time());
      }

      DateTime startDateTimeWithYear = startDateTime;
      if (!startDateTime.date().baseYear()){
        startDateTimeWithYear = DateTime(Date(startDateTime.date().monthOfYear(), startDateTime.date().dayOfMonth(), startDateTime.date().year()), startDateTime.time());
      }

      DateTime endDateTimeWithYear = endDateTime;
      if (!endDateTime.date().baseYear()){
        if (m_wrapAround){
          if (endDateTimeCompare < startDateTimeCompare){
            endDateTimeWithYear = DateTime(Date(endDateTime.date().monthOfYear(), endDateTime.date().dayOfMonth(), endDateTime.date().year() + 1), endDateTime.time());
          }else{
            endDateTimeWithYear = DateTime(Date(endDateTime.date().monthOfYear(), endDateTime.date().dayOfMonth(), endDateTime.date().year()), endDateTime.time());
          }
        }else{
          endDateTimeWithYear = DateTime(Date(endDateTime.date().monthOfYear(), endDateTime.date().dayOfMonth(), endDateTime.date().year()), endDateTime.time());
        }
      }

      double startSecondsFromFirstReport = (startDateTimeWithYear - firstReportDateTimeWithYear).totalSeconds();
      double endSecondsFromFirstReport = (endDateTimeWithYear - firstReportDateTimeWithYear).totalSeconds();

      unsigned numValues = m_values.size();
      OS_ASSERT(numValues == m_secondsFromFirstReport.size());

      Vector result(numValues);
      unsigned resultSize = 0;
      for (unsigned i = 0; i < numValues; ++i){
        if ((m_secondsFromFirstReport[i] >= startSecondsFromFirstReport) &&
            (m_secondsFromFirstReport[i] <= endSecondsFromFirstReport)){
          result[resultSize] = m_values[i];
          ++resultSize;
        }
      }

      result.resize(resultSize, true);

      return result;
    }

    /// get the value used for out of range data
    double TimeSeries_Impl::outOfRangeValue() const
    {
      return m_outOfRangeValue;
    }

    /// set the value used for out of range data, defaults to 0
    void TimeSeries_Impl::setOutOfRangeValue(double value)
    {
      m_outOfRangeValue = value;
    }

    /// add timeseries
    std::shared_ptr<TimeSeries_Impl> TimeSeries_Impl::operator+(const TimeSeries_Impl& other) const
    {
      std::shared_ptr<TimeSeries_Impl> result(new TimeSeries_Impl());

      // if same units
      if (m_units == other.units()){

        // make unique, ordered set of all date times
        std::set<DateTime> dateTimesSet;
        DateTimeVector dateTimes1 = dateTimes();
        DateTimeVector dateTimes2 = other.dateTimes();
        dateTimesSet.insert(dateTimes1.begin(), dateTimes1.end());
        dateTimesSet.insert(dateTimes2.begin(), dateTimes2.end());

        // create vector out of set
        DateTimeVector dateTimes(dateTimesSet.begin(), dateTimesSet.end());

        // compute value at each date time
        Vector values(dateTimesSet.size());
        unsigned valueIndex = 0;
        for (const DateTime& dt : dateTimes){
          values[valueIndex] = value(dt) + other.value(dt);

          LOG(Debug, "At '" << dt << "' " << value(dt) << " + " << other.value(dt) << " = " << values[valueIndex]);

          ++valueIndex;
        }

        // make new result
        result = std::shared_ptr<TimeSeries_Impl>(new TimeSeries_Impl(dateTimes, values, m_units));

      }
      else{
        LOG(Warn, "Adding timeseries with different units returns an empty timeseries");
      }

      return result;
    }

    /// subtract timeseries
    std::shared_ptr<TimeSeries_Impl> TimeSeries_Impl::operator-(const TimeSeries_Impl& other) const
    {
      std::shared_ptr<TimeSeries_Impl> result(new TimeSeries_Impl());

      // if same units
      if (m_units == other.units()){

        // make unique, ordered set of all date times
        std::set<DateTime> dateTimesSet;
        DateTimeVector dateTimes1 = dateTimes();
        DateTimeVector dateTimes2 = other.dateTimes();
        dateTimesSet.insert(dateTimes1.begin(), dateTimes1.end());
        dateTimesSet.insert(dateTimes2.begin(), dateTimes2.end());

        // create vector out of set
        DateTimeVector dateTimes(dateTimesSet.begin(), dateTimesSet.end());

        // compute value at each date time
        Vector values(dateTimesSet.size());
        unsigned valueIndex = 0;
        for (const DateTime& dt : dateTimes){
          values[valueIndex] = value(dt) - other.value(dt);

          LOG(Debug, "At '" << dt << "' " << value(dt) << " - " << other.value(dt) << " = " << values[valueIndex]);

          ++valueIndex;
        }

        // make new result
        result = std::shared_ptr<TimeSeries_Impl>(new TimeSeries_Impl(dateTimes, values, m_units));

      }
      else{
        LOG(Warn, "Subtracting timeseries with different units returns an empty timeseries");
      }

      return result;
    }

    std::shared_ptr<TimeSeries_Impl> TimeSeries_Impl::operator*(double d) const {

      return std::shared_ptr<TimeSeries_Impl>(new TimeSeries_Impl(m_firstReportDateTime, 
        m_secondsFromFirstReport,
        m_values*d, 
        m_units));
    }
  } // detail

  /// default constructor 
  TimeSeries::TimeSeries()
  {
    m_impl = std::shared_ptr<detail::TimeSeries_Impl>(new detail::TimeSeries_Impl());
  }

  /// constructor from start date, interval length, values, and units
  TimeSeries::TimeSeries(const Date& startDate, const Time& intervalLength, const Vector& values, const std::string& units)
  {
    m_impl = std::shared_ptr<detail::TimeSeries_Impl>(new detail::TimeSeries_Impl(startDate, intervalLength, values, units));
  }

  /// constructor from start date and time, interval length, values, and units
  TimeSeries::TimeSeries(const DateTime& startDateTime, const Time& intervalLength, const Vector& values, const std::string& units)
  {
    m_impl = std::shared_ptr<detail::TimeSeries_Impl>(new detail::TimeSeries_Impl(startDateTime, intervalLength, values, units));
  }

  /// constructor from first report date and time, days from first report vector, values, and units
  TimeSeries::TimeSeries(const DateTime& firstReportDateTime, const Vector& daysFromFirstReport, const Vector& values, const std::string& units)
  {
    m_impl = std::shared_ptr<detail::TimeSeries_Impl>(new detail::TimeSeries_Impl(firstReportDateTime, daysFromFirstReport, values, units));
  }

  /// constructor from first report date and time, days from first report , values, and units
  TimeSeries::TimeSeries(const DateTime& firstReportDateTime, const std::vector<double>& daysFromFirstReport, const std::vector<double>& values, const std::string& units)
  {
    m_impl = std::shared_ptr<detail::TimeSeries_Impl>(new detail::TimeSeries_Impl(firstReportDateTime, daysFromFirstReport, values, units));
  }

  /// constructor from date times, values, and units
  TimeSeries::TimeSeries(const DateTimeVector& dateTimes, const Vector& values, const std::string& units)
  {
    m_impl = std::shared_ptr<detail::TimeSeries_Impl>(new detail::TimeSeries_Impl(dateTimes, values, units));
  }

  /// constructor from first report date and time, seconds from first report vector, values, and units
  TimeSeries::TimeSeries(const DateTime& firstReportDateTime, const std::vector<long>& secondsFromFirstReport, const Vector& values, const std::string& units)
  {
    m_impl = std::shared_ptr<detail::TimeSeries_Impl>(new detail::TimeSeries_Impl(firstReportDateTime, secondsFromFirstReport, values, units));
  }

  /// interval length if any
  openstudio::OptionalTime TimeSeries::intervalLength() const
  {
    return m_impl->intervalLength();
  }

  /// date and times at which values are reported, these are the end of each reporting interval 
  openstudio::DateTimeVector TimeSeries::dateTimes() const
  {
    return m_impl->dateTimes();
  }
  openstudio::DateTime TimeSeries::firstReportDateTime() const
  {
    return m_impl->firstReportDateTime();
  }

  /// time in days from end of the first reporting interval
  openstudio::Vector TimeSeries::daysFromFirstReport() const
  {
    return m_impl->daysFromFirstReport();
  }
  /// time in days from end of the first reporting interval at index i
  double TimeSeries::daysFromFirstReport(const unsigned& i) const
  {
    return m_impl->daysFromFirstReport(i);
  }

  /// time in seconds from end of the first reporting interval
  std::vector<long> TimeSeries::secondsFromFirstReport() const
  {
    return m_impl->secondsFromFirstReport();
  }
  /// time in seconds from end of the first reporting interval at index i
  long TimeSeries::secondsFromFirstReport(const unsigned& i) const
  {
    return m_impl->secondsFromFirstReport(i);
  }

  /// values
  openstudio::Vector TimeSeries::values() const
  {
    return m_impl->values();
  }
  /// values
  double TimeSeries::values(const unsigned& i) const
  {
    return m_impl->values(i);
  }

  /// units
  const std::string TimeSeries::units() const
  {
    return m_impl->units();
  }

  /// get value at number of days from first report date and time
  double TimeSeries::value(double daysFromFirstReport) const
  {
    return m_impl->value(daysFromFirstReport);
  }

  /// get value at time from first report date and time
  double TimeSeries::value(const Time& timeFromFirstReport) const
  {
    return m_impl->value(timeFromFirstReport);
  }

  /// get value at date and time
  double TimeSeries::value(const DateTime& dateTime) const
  {
    return m_impl->value(dateTime);
  }

  /// get values between start and end date times
  Vector TimeSeries::values(const DateTime& startDateTime, const DateTime& endDateTime) const
  {
    return m_impl->values(startDateTime, endDateTime);
  }

  /// get the value used for out of range data
  double TimeSeries::outOfRangeValue() const
  {
    return m_impl->outOfRangeValue();
  }

  /// set the value used for out of range data, defaults to 0
  void TimeSeries::setOutOfRangeValue(double value)
  {
    m_impl->setOutOfRangeValue(value);
  }

  /// add timeseries
  TimeSeries TimeSeries::operator+(const TimeSeries& other) const
  {
    std::shared_ptr<detail::TimeSeries_Impl> impl = (*m_impl) + *(other.m_impl);
    return TimeSeries(impl);
  }

  /// subtract timeseries
  TimeSeries TimeSeries::operator-(const TimeSeries& other) const
  {
    std::shared_ptr<detail::TimeSeries_Impl> impl = (*m_impl) - *(other.m_impl);
    return TimeSeries(impl);
  }

  TimeSeries TimeSeries::operator*(double d) const {
    std::shared_ptr<detail::TimeSeries_Impl> impl = (*m_impl)*d;
    return TimeSeries(impl);
  }

  TimeSeries TimeSeries::operator/(double d) const {
    std::shared_ptr<detail::TimeSeries_Impl> impl = (*m_impl)*(1.0/d);
    return TimeSeries(impl);
  }

  // constructor from impl
  TimeSeries::TimeSeries(std::shared_ptr<detail::TimeSeries_Impl> impl)
    : m_impl(impl)
  {}

  TimeSeries operator*(double d,const TimeSeries& series) {
    return series*d;
  }

  TimeSeries sum(const std::vector<TimeSeries>& timeSeriesVector) {
    TimeSeries result;
    bool first = true;
    for (const TimeSeries& ts : timeSeriesVector) {
      if (first) { result = ts; }
      else { result = result + ts; }
      if (result.values().empty()) { 
        LOG_FREE(Info,"zero.sum","Could not sum the timeSeriesVector. Either the first series is empty, or the "
          << "units are incompatible.");
        break; 
      }
      first = false;
    }
    return result;
  }

  boost::function1<TimeSeries, const std::vector<TimeSeries>&> sumTimeSeriesFunctor() { 
    typedef TimeSeries (*functype)(const std::vector<TimeSeries>&);
    return std::function<TimeSeries (const std::vector<TimeSeries>&) > (functype(&sum));
  }

  TimeSeries evaluateTimeSeriesFromTimeSeriesVectorFunctor(
    const boost::function1<TimeSeries, const std::vector<TimeSeries>&>& functor,
    const std::vector<TimeSeries>& timeSeriesVector) 
  {
    return functor(timeSeriesVector);
  }

} // openstudio
