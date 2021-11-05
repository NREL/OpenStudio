/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef UTILITIES_DATA_TIMESERIES_HPP
#define UTILITIES_DATA_TIMESERIES_HPP

#include "../UtilitiesAPI.hpp"

#include "Vector.hpp"
#include "../time/Date.hpp"
#include "../time/Time.hpp"
#include "../time/DateTime.hpp"

#include <boost/optional.hpp>
#include <boost/function.hpp>

#include <vector>

namespace openstudio {

namespace detail {

  class UTILITIES_API TimeSeries_Impl
  {
   public:
    TimeSeries_Impl();

    TimeSeries_Impl(const Date& startDate, const Time& intervalLength, const Vector& values, const std::string& units);

    TimeSeries_Impl(const DateTime& firstReportDateTime, const Time& intervalLength, const Vector& values, const std::string& units);

    TimeSeries_Impl(const DateTime& firstReportDateTime, const Vector& timeInDays, const Vector& values, const std::string& units);

    TimeSeries_Impl(const DateTime& firstReportDateTime, const std::vector<double>& timeInDays, const std::vector<double>& values,
                    const std::string& units);

    TimeSeries_Impl(const DateTimeVector& inDateTimes, const Vector& values, const std::string& units);

    TimeSeries_Impl(const DateTime& firstReportDateTime, const std::vector<long>& timeInSeconds, const Vector& values, const std::string& units);

    ~TimeSeries_Impl() {}

    openstudio::OptionalTime intervalLength() const;

    openstudio::DateTime firstReportDateTime() const;

    DateTimeVector dateTimes() const;

    openstudio::Vector daysFromFirstReport() const;

    double daysFromFirstReport(unsigned int i) const;

    std::vector<long> secondsFromFirstReport() const;

    long secondsFromFirstReport(unsigned int i) const;

    openstudio::Vector values() const;

    double values(unsigned int i) const;

    const std::string units() const;

    double valueAtSecondsFromFirstReport(long secondsFromFirstReport) const;

    double value(double daysFromFirstReport) const;

    double value(const Time& timeFromFirstReport) const;

    double value(const DateTime& dateTime) const;

    Vector values(const DateTime& startDateTime, const DateTime& endDateTime) const;

    double outOfRangeValue() const;

    void setOutOfRangeValue(double value);

    std::shared_ptr<TimeSeries_Impl> operator+(const TimeSeries_Impl& other) const;

    std::shared_ptr<TimeSeries_Impl> operator-(const TimeSeries_Impl& other) const;

    std::shared_ptr<TimeSeries_Impl> operator*(double d) const;

    double integrate() const;

    double averageValue() const;

   private:
    REGISTER_LOGGER("utilities.TimeSeries_Impl");
    // fully qualified first report date
    DateTime m_firstReportDateTime;

    // start date and time of time series
    DateTime m_startDateTime;

    // integer seconds from first report date time, used for quick interpolation
    std::vector<long> m_secondsFromFirstReport;
    Vector m_secondsFromFirstReportAsVector;  // same as m_secondsFromFirstReport but stored as Vector
    std::vector<long> m_secondsFromStart;

    // values reported at m_dateTimes
    Vector m_values;

    // units of the values
    std::string m_units;

    // length of the reporting interval if known, can be used to speed up interpolation
    OptionalTime m_intervalLength;

    // value used for out of range data
    double m_outOfRangeValue;

    // true if the time series should support wrap around dates, e.g. 4/11-4/10 without specific year
    bool m_wrapAround;
  };
}  // namespace detail

/** TimeSeries is a series of values each reported at a single time.  We follow the EnergyPlus
 *   convention that the time reported for each value is at the end of the reporting interval.  For example, if a value
 *   is measured over the interval from hour 1 (non-inclusive) to hour 2 (inclusive), that is 1 < t <= 2, and the reported
 *   value (either the average, median, min, max, etc) is determined to be 7, then the value 7 is reported at hour 2.  One
 *   of the primary tasks of the TimeSeries class is to get the value of the series at an arbitrary time.  For the example above,
 *   any time in the interval 1 < t <= 2 will return 7.  We refer to the beginning of the first reporting interval as the
 *   startDateTime.  The end of the first interval is referred to as the firstReportDateTime.
 *
 *   Todo: add method to mark TimeSeries that represent point in time measurements rather than interval measurements
 *
 *   Todo: add calendar to deal with Daylight Savings
 **/
class UTILITIES_API TimeSeries
{
 public:
  /** @name Constructors */
  //@{

  /// Default constructor with no data.
  TimeSeries();

  /** Constructor from start date, interval length, values, and units.
   *  First reporting interval starts at 0 hours on startDate and ends at startDate + intervalLength. */
  TimeSeries(const Date& startDate, const Time& intervalLength, const Vector& values, const std::string& units);

  /** Constructor from first report date and time, interval length, values, and units.
   *  First reporting interval starts at firstReportDateTime - intervalLength and ends at firstReportDateTime. */
  TimeSeries(const DateTime& firstReportDateTime, const Time& intervalLength, const Vector& values, const std::string& units);

  /** Constructor from first report date and time, time in days, values, and units.
   *  The treatment of the time vector depends upon the value of the first element:
   *    - If timeInDays[0] == 0 then the first reporting interval starts at 0 hours on the firstReportDateTime's date and ends at firstReportDateTime.
   *    - If timeInDays[0] > 0 then the first reporting interval starts at firstReportDateTime - timeInDays[0] and ends at firstReportDateTime.
   *
   *  An exception is thrown if:
   *    - timeInDays.size != values.size
   *    - timeInDays is not monotonically increasing
   *    - the start date and time of first reporting interval cannot be determined */
  TimeSeries(const DateTime& firstReportDateTime, const Vector& timeInDays, const Vector& values, const std::string& units);

  /** Constructor from first report date and time, time in days, values, and units.
   *  The treatment of the time vector depends upon the value of the first element:
   *    - If timeInDays[0] == 0 then the first reporting interval starts at 0 hours on the firstReportDateTime's date and ends at firstReportDateTime.
   *    - If timeInDays[0] > 0 then the first reporting interval starts at firstReportDateTime - timeInDays[0] and ends at firstReportDateTime.
   *
   *  An exception is thrown if:
   *    - timeInDays.size != values.size
   *    - timeInDays is not monotonically increasing
   *    - the start date and time of first reporting interval cannot be determined */
  TimeSeries(const DateTime& firstReportDateTime, const std::vector<double>& timeInDays, const std::vector<double>& values, const std::string& units);

  /** Constructor from date times, values, and units.
   *  The size of the dateTimes vector determines how it is handled:
   *    - If dateTimes.size == values.size then the first reporting interval starts at 0 hours on dateTimes[0]'s date and ends at dateTimes[0].
   *    - If dateTimes.size == values.size + 1 then the first reporting interval starts at dateTimes[0] and ends at dateTimes[1].
   *
   * An exception is thrown if:
   *   - (timeInDays.size != values.size) and (timeInDays.size != values.size + 1)
   *   - the start date and time of first reporting interval cannot be determined */
  // DLM: this is the constructor that would be used for point in time measurements
  TimeSeries(const DateTimeVector& dateTimes, const Vector& values, const std::string& units);

  /** Constructor from first report date and time, time in seconds, values, and units.
   *  The treatment of the time vector depends upon the value of the first element:
   *    - If timeInSeconds[0] == 0 then the first reporting interval starts at 0 hours on the firstReportDateTime's date and ends at firstReportDateTime.
   *    - If timeInSeconds[0] > 0 then the first reporting interval starts at firstReportDateTime - timeInSeconds[0] and ends at firstReportDateTime.
   *
   * An exception is thrown if:
   *   - timeInSeconds.size != values.size
   *   - timeInSeconds is not monotonically increasing
   *   - start date and time of first reporting interval cannot be determined */
  TimeSeries(const DateTime& firstReportDateTime, const std::vector<long>& timeInSeconds, const Vector& values, const std::string& units);

  /// Virtual destructor
  ~TimeSeries() {}

  //@}
  /** @name Getters */
  //@{

  /// Returns the interval length if any
  openstudio::OptionalTime intervalLength() const;

  /// Returns the date and times at which values are reported, these are the end of each reporting interval
  openstudio::DateTimeVector dateTimes() const;

  /// Returns the date and time of first report value
  openstudio::DateTime firstReportDateTime() const;

  /// Returns the vector of time in days from end of the first reporting interval
  openstudio::Vector daysFromFirstReport() const;

  /// Returns the time in days from end of the first reporting interval at index i to prevent implicit vector copy for single value
  double daysFromFirstReport(unsigned int i) const;

  /// Returns the time in seconds from end of the first reporting interval
  std::vector<long> secondsFromFirstReport() const;

  /// Return the time in seconds from end of the first reporting interval at index i to prevent implicit vector copy for single value
  long secondsFromFirstReport(unsigned int i) const;

  /// Returns the values vector
  openstudio::Vector values() const;

  /// Returns the value at index i to prevent implicit vector copy for single value
  double values(unsigned int i) const;

  /// Returns the series units as a standard string
  const std::string units() const;

  /// Get value at number of days from first report date and time
  double value(double daysFromFirstReport) const;

  /// Get value at time from first report date and time
  double value(const Time& timeFromFirstReport) const;

  /// Get value at date and time
  double value(const DateTime& dateTime) const;

  /// Get values between start and end date times
  Vector values(const DateTime& startDateTime, const DateTime& endDateTime) const;

  /// Get the value used for out of range data
  double outOfRangeValue() const;

  //@}
  /** @name Setters */
  //@{

  /// Set the value used for out of range data, defaults to 0
  void setOutOfRangeValue(double value);

  //@}
  /** @name Operators */
  //@{

  /// Add timeseries
  TimeSeries operator+(const TimeSeries& other) const;

  /// Subtract timeseries
  TimeSeries operator-(const TimeSeries& other) const;

  /** TimeSeries * double */
  TimeSeries operator*(double d) const;

  /** TimeSeries / double */
  TimeSeries operator/(double d) const;

  //@}
  /** @name Analysis Functions */
  //@{

  /** Integrate the time series */
  double integrate() const;

  /** Compute the time series average value */
  double averageValue() const;

  //@}
 private:
  REGISTER_LOGGER("utilities.TimeSeries");
  // constructor from impl
  TimeSeries(std::shared_ptr<detail::TimeSeries_Impl> impl);

  // pointer to impl
  std::shared_ptr<detail::TimeSeries_Impl> m_impl;
};

// optional TimeSeries
typedef boost::optional<TimeSeries> OptionalTimeSeries;

// vector of TimeSeries
typedef std::vector<TimeSeries> TimeSeriesVector;

/** double * TimeSeries */
UTILITIES_API TimeSeries operator*(double d, const TimeSeries& series);

// ETH@20100910 No implementation of double/TimeSeries yet because that would change the units.
// We should be able to tackle double/TimeSeries after adding get/setQuantity to
// IdfObject.

// Helper function to add up all the TimeSeries in timeSeriesVector.
UTILITIES_API TimeSeries sum(const std::vector<TimeSeries>& timeSeriesVector);

/** Returns std::function pointer to sum(const std::vector<TimeSeries>&). */
UTILITIES_API boost::function1<TimeSeries, const std::vector<TimeSeries>&> sumTimeSeriesFunctor();

/** Evaluates functor(timeSeriesVector). For use in SWIG bindings. */
UTILITIES_API TimeSeries evaluateTimeSeriesFromTimeSeriesVectorFunctor(const boost::function1<TimeSeries, const std::vector<TimeSeries>&>& functor,
                                                                       const std::vector<TimeSeries>& timeSeriesVector);

}  // namespace openstudio

#endif  // UTILITIES_DATA_TIMESERIES_HPP
