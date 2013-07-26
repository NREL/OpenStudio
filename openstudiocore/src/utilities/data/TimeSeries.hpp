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

#ifndef UTILITIES_DATA_TIMESERIES_HPP
#define UTILITIES_DATA_TIMESERIES_HPP

#include <utilities/UtilitiesAPI.hpp>

#include <utilities/data/Vector.hpp>
#include <utilities/time/Date.hpp>
#include <utilities/time/Time.hpp>
#include <utilities/time/DateTime.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/function.hpp>

#include <vector>

namespace openstudio{

  namespace detail{

    class UTILITIES_API TimeSeries_Impl
    {
      public:

        /// default constructor
        TimeSeries_Impl();

        /// constructor from start date, interval length, values, and units
        /// first reporting interval ends at Date + Time(0) + intervalLength
        TimeSeries_Impl(const Date& startDate, const Time& intervalLength, const Vector& values, const std::string& units);

        /// constructor from start date and time, interval length, values, and units
        TimeSeries_Impl(const DateTime& startDateTime, const Time& intervalLength, const Vector& values, const std::string& units);

        /// constructor from first report date and time, days from first report vector, values, and units
        TimeSeries_Impl(const DateTime& firstReportDateTime, const Vector& daysFromFirstReport, const Vector& values, const std::string& units);

        /// constructor from first report date and time, days from first report , values, and units
        TimeSeries_Impl(const DateTime& firstReportDateTime, const std::vector<double>& daysFromFirstReport, const std::vector<double>& values, const std::string& units);

        /// constructor from date times, values, and units
        TimeSeries_Impl(const DateTimeVector& dateTimes, const Vector& values, const std::string& units);

        // virtual destructor
        ~TimeSeries_Impl() {}

        /// interval length if any
        openstudio::OptionalTime intervalLength() const;

        /// date and time of first report value
        openstudio::DateTime firstReportDateTime() const;

        /// time in days from end of the first reporting interval
        openstudio::Vector daysFromFirstReport() const;
        /// time in days from end of the first reporting interval at index i to prevent inplicit vector copy for single value
        double daysFromFirstReport(const unsigned& i) const;

        /// values
        openstudio::Vector values() const;
        /// values at index i to prevent inplicit vector copy for single value
        double values(const unsigned& i) const;

        /// units
        const std::string units() const;

        /// get value at number of days from start date and time
        /// Note that rounding errors may occur - see trac 1380 and 
        /// TEST(FloodPlot, TimeSeriesFloodPlot_Detailed)
        /// TEST(LinePlot, TimeSeriesLinePlot_Detailed)
        double value(double daysFromFirstReport) const;

        /// get value at time from first report date and time
        double value(const Time& timeFromFirstReport) const;

        /// get value at date and time
        double value(const DateTime& dateTime) const;

        /// get values between start and end date times
        Vector values(const DateTime& startDateTime, const DateTime& endDateTime) const;

        /// get the value used for out of range data
        double outOfRangeValue() const;

        /// set the value used for out of range data, defaults to 0
        void setOutOfRangeValue(double value);

        /// add timeseries
        boost::shared_ptr<TimeSeries_Impl> operator+(const TimeSeries_Impl& other) const;

        /// subtract timeseries
        boost::shared_ptr<TimeSeries_Impl> operator-(const TimeSeries_Impl& other) const;

        /** TimeSereis * double */
        boost::shared_ptr<TimeSeries_Impl> operator*(double d) const;

      private:

        REGISTER_LOGGER("utilities.TimeSeries_Impl");
        // fully qualified first report date
        DateTime m_firstReportDateTime;

        // fractional days from first report date time, used for quick interpolation
        Vector m_daysFromFirstReport; 

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
  } // detail

  /** TimeSeries is a series of values each reported at a single time.  We follow the EnergyPlus
   *   convention that the time reported for each value is at the end of the reporting interval.  For example, if a value 
   *   is measured over the interval from hour 1 (non-inclusive) to hour 2 (inclusive), that is 1 < t <= 2, and the reported
   *   value (either the average, median, min, max, etc) is determined to be 7, then the value 7 is reported at hour 2.  One 
   *   of the primary tasks of the TimeSeries class is to get the value of the series at an arbitrary time.  For the example above, 
   *   any time in the interval 1 < t <= 2 will return 7.
   *
   *   Todo: add calendar to deal with Daylight Savings
   **/
  class UTILITIES_API TimeSeries
  {
    public:
      /** @name Constructors */
      //@{

      /// default constructor 
      TimeSeries();

      /// constructor from start date, interval length, values, and units
      /// first reporting interval ends at Date + Time(0) + intervalLength
      TimeSeries(const Date& startDate, const Time& intervalLength, const Vector& values, const std::string& units);

      /// constructor from start date and time, interval length, values, and units
      TimeSeries(const DateTime& startDateTime, const Time& intervalLength, const Vector& values, const std::string& units);

      /// constructor from first report date and time, days from first report vector, values, and units
      TimeSeries(const DateTime& firstReportDateTime, const Vector& daysFromFirstReport, const Vector& values, const std::string& units);

      /// constructor from first report date and time, days from first report , values, and units
      TimeSeries(const DateTime& firstReportDateTime, const std::vector<double>& daysFromFirstReport, const std::vector<double>& values, const std::string& units);

      /// constructor from date times, values, and units
      TimeSeries(const DateTimeVector& dateTimes, const Vector& values, const std::string& units);

      /// virtual destructor
      ~TimeSeries() {}

      //@}
      /** @name Getters */
      //@{

      /// interval length if any
      openstudio::OptionalTime intervalLength() const;

      /// date and time of first report value
      openstudio::DateTime firstReportDateTime() const;

      /// time in days from end of the first reporting interval
      openstudio::Vector daysFromFirstReport() const;

      /// time in days from end of the first reporting interval at index i to prevent inplicit vector copy for single value
      double daysFromFirstReport(const unsigned& i) const;

      /// values
      openstudio::Vector values() const;
      
      /// values at index i to prevent inplicit vector copy for single value
      double values(const unsigned& i) const;

      /// units
      const std::string units() const;

      /// get value at number of days from first report date and time
      double value(double daysFromFirstReport) const;

      /// get value at time from first report date and time
      double value(const Time& timeFromFirstReport) const;

      /// get value at date and time
      double value(const DateTime& dateTime) const;

      /// get values between start and end date times
      Vector values(const DateTime& startDateTime, const DateTime& endDateTime) const;

      /// get the value used for out of range data
      double outOfRangeValue() const;

      //@}
      /** @name Setters */
      //@{

      /// set the value used for out of range data, defaults to 0
      void setOutOfRangeValue(double value);

      //@}
      /** @name Operators */
      //@{

      /// add timeseries
      TimeSeries operator+(const TimeSeries& other) const;

      /// subtract timeseries
      TimeSeries operator-(const TimeSeries& other) const;

      /** TimeSereis * double */
      TimeSeries operator*(double d) const;

      /** TimeSeries / double */
      TimeSeries operator/(double d) const;

      //@}
    private:

      REGISTER_LOGGER("utilities.TimeSeries");
      // constructor from impl
      TimeSeries(boost::shared_ptr<detail::TimeSeries_Impl> impl);

      // pointer to impl
      boost::shared_ptr<detail::TimeSeries_Impl> m_impl;
  };

  // optional TimeSeries
  typedef boost::optional<TimeSeries> OptionalTimeSeries;

  // vector of TimeSeries
  typedef std::vector<TimeSeries> TimeSeriesVector;

  /** double * TimeSeries */
  UTILITIES_API TimeSeries operator*(double d,const TimeSeries& series);

  // ETH@20100910 No implementation of double/TimeSeries yet because that would change the units.
  // We should be able to tackle double/TimeSeries after adding get/setQuantity to 
  // IdfObject.

  // Helper function to add up all the TimeSeries in timeSeriesVector.
  UTILITIES_API TimeSeries sum(const std::vector<TimeSeries>& timeSeriesVector);

  /** Returns boost::function pointer to sum(const std::vector<TimeSeries>&). */
  UTILITIES_API boost::function1<TimeSeries, const std::vector<TimeSeries>&> sumTimeSeriesFunctor();

  /** Evaluates functor(timeSeriesVector). For use in SWIG bindings. */
  UTILITIES_API TimeSeries evaluateTimeSeriesFromTimeSeriesVectorFunctor(
      const boost::function1<TimeSeries, const std::vector<TimeSeries>&>& functor,
      const std::vector<TimeSeries>& timeSeriesVector);

} // openstudio

#endif // UTILITIES_DATA_TIMESERIES_HPP

