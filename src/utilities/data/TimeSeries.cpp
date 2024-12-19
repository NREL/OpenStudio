/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "TimeSeries.hpp"
#include "../core/Assert.hpp"

using namespace std;
using namespace boost;

namespace openstudio {

namespace detail {

  TimeSeries_Impl::TimeSeries_Impl() : m_outOfRangeValue(0.0), m_wrapAround(false) {}

  TimeSeries_Impl::TimeSeries_Impl(const Date& startDate, const Time& intervalLength, const Vector& values, const std::string& units)
    : m_secondsFromFirstReport(values.size()),
      m_secondsFromStart(values.size()),
      m_values(values),
      m_units(units),
      m_intervalLength(intervalLength),
      m_outOfRangeValue(0.0),
      m_wrapAround(false) {
    if (values.empty()) {
      LOG(Warn, "Creating empty timeseries");
    }

    // length of interval in seconds
    int secondsPerInterval = intervalLength.totalSeconds();

    // date and time of first report, end of the first reporting interval
    // DLM: startDate may or may not have baseYear defined
    m_firstReportDateTime = DateTime(startDate, intervalLength);

    m_startDateTime = DateTime(startDate, Time(0));

    for (size_t i = 0; i < values.size(); ++i) {
      m_secondsFromFirstReport[i] = i * secondsPerInterval;
      m_secondsFromStart[i] = (i + 1) * secondsPerInterval;
    }
    m_secondsFromFirstReportAsVector = createVector(m_secondsFromFirstReport);

    long durationSeconds = 0;
    if (!m_secondsFromFirstReport.empty()) {
      durationSeconds = m_secondsFromFirstReport.back();
    }

    // check for wrap around
    boost::optional<int> calendarYear = m_firstReportDateTime.date().baseYear();
    if (!calendarYear) {
      DateTime lastDateTime = m_firstReportDateTime + Time(0, 0, 0, durationSeconds);
      Date lastDate(lastDateTime.date().monthOfYear(), lastDateTime.date().dayOfMonth());
      if ((durationSeconds > Time(366.0).totalSeconds()) || (lastDate < m_firstReportDateTime.date())) {
        m_wrapAround = true;
      }
    }
  }

  TimeSeries_Impl::TimeSeries_Impl(const DateTime& firstReportDateTime, const Time& intervalLength, const Vector& values, const std::string& units)
    : m_secondsFromFirstReport(values.size()),
      m_secondsFromStart(values.size()),
      m_values(values),
      m_units(units),
      m_intervalLength(intervalLength),
      m_outOfRangeValue(0.0),
      m_wrapAround(false) {
    if (values.empty()) {
      LOG(Warn, "Creating empty timeseries");
    }

    // length of interval in seconds
    int secondsPerInterval = intervalLength.totalSeconds();

    // DLM: startDate may or may not have baseYear defined
    m_firstReportDateTime = DateTime(firstReportDateTime.date(), firstReportDateTime.time());

    m_startDateTime = m_firstReportDateTime - intervalLength;

    for (size_t i = 0; i < values.size(); ++i) {
      m_secondsFromFirstReport[i] = i * secondsPerInterval;
      m_secondsFromStart[i] = (i + 1) * secondsPerInterval;
    }
    m_secondsFromFirstReportAsVector = createVector(m_secondsFromFirstReport);

    long durationSeconds = 0;
    if (!m_secondsFromFirstReport.empty()) {
      durationSeconds = m_secondsFromFirstReport.back();
    }

    // check for wrap around
    boost::optional<int> calendarYear = m_firstReportDateTime.date().baseYear();
    if (!calendarYear) {
      DateTime lastDateTime = m_firstReportDateTime + Time(0, 0, 0, durationSeconds);
      Date lastDate(lastDateTime.date().monthOfYear(), lastDateTime.date().dayOfMonth());
      if ((durationSeconds > Time(366.0).totalSeconds()) || (lastDate < m_firstReportDateTime.date())) {
        m_wrapAround = true;
      }
    }
  }

  TimeSeries_Impl::TimeSeries_Impl(const DateTime& firstReportDateTime, const Vector& timeInDays, const Vector& values, const std::string& units)
    : m_secondsFromFirstReport(values.size()),
      m_secondsFromStart(values.size()),
      m_values(values),
      m_units(units),
      m_outOfRangeValue(0.0),
      m_wrapAround(false) {
    if (timeInDays.size() != values.size()) {
      LOG_AND_THROW("Length of values (" << values.size() << ") must match length of times (" << timeInDays.size() << ")");
    }

    if (values.empty() || timeInDays.empty()) {
      LOG(Warn, "Creating empty timeseries");
      m_startDateTime = firstReportDateTime;
      m_firstReportDateTime = firstReportDateTime;
    } else {

      // DLM: firstReportDateTime may or may not have baseYear defined
      m_firstReportDateTime = firstReportDateTime;

      std::copy(values.begin(), values.end(), m_values.begin());

      if (timeInDays[0] == 0) {  // This is the old, BROKEN way
        int firstIntervalSeconds = firstReportDateTime.time().totalSeconds();
        if (firstIntervalSeconds == 0) {
          LOG_AND_THROW("Cannot calculate the series start date for first report at the beginning of a day");
        }
        LOG(Warn, "Assuming time series begins at the start of the day of first report. This behavior is deprecated and will instead be an error in "
                  "the future.");
        m_startDateTime = DateTime(m_firstReportDateTime.date());

        for (unsigned i = 0; i < values.size(); ++i) {
          m_secondsFromFirstReport[i] = Time(timeInDays[i]).totalSeconds();
          m_secondsFromStart[i] = m_secondsFromFirstReport[i] + firstIntervalSeconds;
          if (i > 0) {
            if (m_secondsFromFirstReport[i] < m_secondsFromFirstReport[i - 1]) {
              LOG_AND_THROW("Days from first report must be monotonically increasing");
            }
          }
        }
      } else {  // This is the new way
        m_startDateTime = m_firstReportDateTime - Time(timeInDays[0]);
        for (unsigned i = 0; i < values.size(); ++i) {
          m_secondsFromStart[i] = Time(timeInDays[i]).totalSeconds();
          m_secondsFromFirstReport[i] = m_secondsFromStart[i] - m_secondsFromStart[0];
          if (i > 0) {
            if (m_secondsFromFirstReport[i] < m_secondsFromFirstReport[i - 1]) {
              LOG_AND_THROW("Days from first report must be monotonically increasing");
            }
          }
        }
      }

      m_secondsFromFirstReportAsVector = createVector(m_secondsFromFirstReport);

      long durationSeconds = 0;
      if (!m_secondsFromFirstReport.empty()) {
        durationSeconds = m_secondsFromFirstReport.back();
      }

      // check for wrap around
      boost::optional<int> calendarYear = m_firstReportDateTime.date().baseYear();
      if (!calendarYear) {
        DateTime lastDateTime = m_firstReportDateTime + Time(0, 0, 0, durationSeconds);
        Date lastDate(lastDateTime.date().monthOfYear(), lastDateTime.date().dayOfMonth());
        if ((durationSeconds > Time(366.0).totalSeconds()) || (lastDate < m_firstReportDateTime.date())) {
          m_wrapAround = true;
        }
      }
    }
  }

  TimeSeries_Impl::TimeSeries_Impl(const DateTime& firstReportDateTime, const std::vector<double>& timeInDays, const std::vector<double>& values,
                                   const std::string& units)
    : m_secondsFromFirstReport(timeInDays.size()),
      m_secondsFromStart(values.size()),
      m_values(values.size()),
      m_units(units),
      m_outOfRangeValue(0.0),
      m_wrapAround(false) {

    if (timeInDays.size() != values.size()) {
      LOG_AND_THROW("Length of values (" << values.size() << ") must match length of times (" << timeInDays.size() << ")");
    }

    if (values.empty() || timeInDays.empty()) {
      LOG(Warn, "Creating empty timeseries");
      m_startDateTime = firstReportDateTime;
      m_firstReportDateTime = firstReportDateTime;
    } else {

      // DLM: firstReportDateTime may or may not have baseYear defined
      m_firstReportDateTime = firstReportDateTime;

      std::copy(values.begin(), values.end(), m_values.begin());

      if (timeInDays[0] == 0) {  // This is the old, BROKEN way
        int firstIntervalSeconds = firstReportDateTime.time().totalSeconds();
        if (firstIntervalSeconds == 0) {
          LOG_AND_THROW("Cannot calculate the series start date for first report at the beginning of a day");
        }
        LOG(Warn, "Assuming time series begins at the start of the day of first report. This behavior is deprecated and will instead be an error in "
                  "the future.");
        m_startDateTime = DateTime(m_firstReportDateTime.date());

        for (unsigned i = 0; i < values.size(); ++i) {
          m_secondsFromFirstReport[i] = Time(timeInDays[i]).totalSeconds();
          m_secondsFromStart[i] = m_secondsFromFirstReport[i] + firstIntervalSeconds;
          if (i > 0) {
            if (m_secondsFromFirstReport[i] < m_secondsFromFirstReport[i - 1]) {
              LOG_AND_THROW("Days from first report must be monotonically increasing");
            }
          }
        }
      } else {  // This is the new way
        m_startDateTime = m_firstReportDateTime - Time(timeInDays[0]);
        for (unsigned i = 0; i < values.size(); ++i) {
          m_secondsFromStart[i] = Time(timeInDays[i]).totalSeconds();
          m_secondsFromFirstReport[i] = m_secondsFromStart[i] - m_secondsFromStart[0];
          if (i > 0) {
            if (m_secondsFromFirstReport[i] < m_secondsFromFirstReport[i - 1]) {
              LOG_AND_THROW("Days from first report must be monotonically increasing");
            }
          }
        }
      }

      m_secondsFromFirstReportAsVector = createVector(m_secondsFromFirstReport);

      long durationSeconds = 0;
      if (!m_secondsFromFirstReport.empty()) {
        durationSeconds = m_secondsFromFirstReport.back();
      }

      // check for wrap around
      boost::optional<int> calendarYear = m_firstReportDateTime.date().baseYear();
      if (!calendarYear) {
        DateTime lastDateTime = m_firstReportDateTime + Time(0, 0, 0, durationSeconds);
        Date lastDate(lastDateTime.date().monthOfYear(), lastDateTime.date().dayOfMonth());
        if ((durationSeconds > Time(366.0).totalSeconds()) || (lastDate < m_firstReportDateTime.date())) {
          m_wrapAround = true;
        }
      }
    }
  }

  TimeSeries_Impl::TimeSeries_Impl(const DateTimeVector& inDateTimes, const Vector& values, const std::string& units)
    : m_secondsFromFirstReport(values.size()),
      m_secondsFromStart(values.size()),
      m_values(values),
      m_units(units),
      m_outOfRangeValue(0.0),
      m_wrapAround(false) {
    // DLM: this seems to be a pretty fragile constructor with a lot going on

    if (values.empty() || inDateTimes.empty()) {
      LOG(Warn, "Creating empty timeseries");
    } else {

      bool extraTime(false);
      DateTimeVector dateTimes = inDateTimes;
      if (dateTimes.size() != values.size()) {
        if (dateTimes.size() - 1 == values.size()) {
          extraTime = true;
          m_startDateTime = inDateTimes.front();
          dateTimes = DateTimeVector(inDateTimes.cbegin() + 1, inDateTimes.cend());
        } else {
          LOG_AND_THROW("Length of values (" << values.size() << ") must match length of times (" << dateTimes.size() << ")");
        }
      }

      // DLM: startDate may or may not have baseYear defined
      m_firstReportDateTime = dateTimes.front();
      // unsigned numDateTimes = dateTimes.size();
      boost::optional<int> calendarYear = m_firstReportDateTime.date().baseYear();

      // Check for wrap around
      m_wrapAround = false;
      if (!calendarYear) {
        for (unsigned i = 1; i < dateTimes.size(); i++) {
          if (dateTimes[i] < dateTimes[i - 1]) {
            m_wrapAround = true;
            break;
          }
        }
      }

      // Compute the seconds from first report
      if (m_wrapAround) {
        m_secondsFromFirstReport[0] = 0;
        m_secondsFromStart[0] = 0;
        int delta = 0;
        DateTime firstReportDateTimeWithYear =
          DateTime(Date(m_firstReportDateTime.date().monthOfYear(), m_firstReportDateTime.date().dayOfMonth(), m_firstReportDateTime.date().year()),
                   m_firstReportDateTime.time());
        for (unsigned i = 1; i < dateTimes.size(); i++) {
          DateTime wrappedDateTime =
            DateTime(Date(dateTimes[i].date().monthOfYear(), dateTimes[i].date().dayOfMonth(), m_firstReportDateTime.date().year() + delta),
                     dateTimes[i].time());
          if (wrappedDateTime < dateTimes[i - 1]) {
            ++delta;
            wrappedDateTime =
              DateTime(Date(dateTimes[i].date().monthOfYear(), dateTimes[i].date().dayOfMonth(), m_firstReportDateTime.date().year() + delta),
                       dateTimes[i].time());
          }
          m_secondsFromFirstReport[i] = (wrappedDateTime - firstReportDateTimeWithYear).totalSeconds();
          m_secondsFromStart[i] = (wrappedDateTime - firstReportDateTimeWithYear).totalSeconds();
        }
      } else {
        m_secondsFromFirstReport[0] = 0;
        m_secondsFromStart[0] = 0;
        for (unsigned i = 1; i < dateTimes.size(); i++) {
          m_secondsFromFirstReport[i] = (dateTimes[i] - m_firstReportDateTime).totalSeconds();
          m_secondsFromStart[i] = (dateTimes[i] - m_firstReportDateTime).totalSeconds();
        }
      }

      for (unsigned i = 1; i < dateTimes.size(); i++) {
        if (m_secondsFromStart[i] < m_secondsFromStart[i - 1]) {
          LOG_AND_THROW("Dates from first report must be monotonically increasing");
        }
      }

      // Try to get a start time if we don't have one already
      if (!extraTime) {
        int delta;
        bool foundInterval = false;
        if (m_secondsFromStart.size() > 1) {
          // check if all data is reported at a constant interval
          delta = m_secondsFromStart[1] - m_secondsFromStart[0];
          foundInterval = true;
          for (unsigned i = 2; i < m_secondsFromStart.size(); i++) {
            if (delta != m_secondsFromStart[i] - m_secondsFromStart[i - 1]) {
              foundInterval = false;
            }
            break;
          }
        }
        if (foundInterval) {
          // DLM: shouldn't we also set m_intervalLength here?
          // DLM: we could but some behavior seems to expect this to stay unset, TEST_F(DataFixture,TimeSeries_DetailedConstructor_WrapAroundDates)
          //m_intervalLength = Time(0, 0, 0, delta);
          m_startDateTime = m_firstReportDateTime - Time(0, 0, 0, delta);
        } else {
          if (m_firstReportDateTime.time().totalSeconds() == 0) {
            LOG_AND_THROW("Cannot calculate the series start date for first report at the beginning of a day");
          }
          LOG(Warn, "Assuming time series begins at the start of the day of first report. This behavior is deprecated and will instead be an error "
                    "in the future.");
          m_startDateTime = DateTime(m_firstReportDateTime.date());
        }
      }

      int firstIntervalSeconds = (m_firstReportDateTime - m_startDateTime).totalSeconds();

      // Compute the seconds from start
      m_secondsFromStart[0] = firstIntervalSeconds;
      for (unsigned i = 1; i < dateTimes.size(); i++) {
        m_secondsFromStart[i] = m_secondsFromStart[i] + firstIntervalSeconds;
      }

      m_secondsFromFirstReportAsVector = createVector(m_secondsFromFirstReport);
    }
  }

  TimeSeries_Impl::TimeSeries_Impl(const DateTime& firstReportDateTime, const std::vector<long>& timeInSeconds, const Vector& values,
                                   const std::string& units)
    : m_secondsFromFirstReport(values.size()),
      m_secondsFromStart(values.size()),
      m_values(values),
      m_units(units),
      m_outOfRangeValue(0.0),
      m_wrapAround(false) {
    if (timeInSeconds.size() != values.size()) {
      LOG_AND_THROW("Length of values (" << values.size() << ") must match length of times (" << timeInSeconds.size() << ")");
    }
    if (values.empty() || timeInSeconds.empty()) {
      LOG(Warn, "Creating empty timeseries");
      m_startDateTime = firstReportDateTime;
      m_firstReportDateTime = firstReportDateTime;
    } else {
      // Check that seconds are monotonic
      for (unsigned i = 1; i < timeInSeconds.size(); ++i) {
        if (timeInSeconds[i] < timeInSeconds[i - 1]) {
          LOG_AND_THROW("Seconds from first report/start of series must be monotonically increasing");
        }
      }

      if (timeInSeconds[0] == 0) {  // This is the old, BROKEN way
        if (firstReportDateTime.time().totalSeconds() == 0) {
          LOG_AND_THROW("Cannot calculate the series start date for first report at the beginning of a day");
        }
        LOG(Warn, "Assuming time series begins at the start of the day of first report. This behavior is deprecated and will instead be an error in "
                  "the future.");
        m_startDateTime = DateTime(firstReportDateTime.date());
        m_firstReportDateTime = firstReportDateTime;
        int firstIntervalSeconds = m_firstReportDateTime.time().totalSeconds();
        m_secondsFromStart = timeInSeconds;
        for (unsigned i = 0; i < m_secondsFromStart.size(); i++) {
          m_secondsFromStart[i] += firstIntervalSeconds;
        }
        m_secondsFromFirstReport = timeInSeconds;

      } else {  // This is the new behavior
        m_startDateTime = firstReportDateTime - Time(0, 0, 0, timeInSeconds[0]);
        m_firstReportDateTime = firstReportDateTime;
        m_secondsFromStart = timeInSeconds;

        // Get rid of this later
        m_secondsFromFirstReport[0] = 0;
        for (unsigned i = 1; i < values.size(); ++i) {
          m_secondsFromFirstReport[i] = timeInSeconds[i] - timeInSeconds[0];
        }
      }
    }

    m_secondsFromFirstReportAsVector = createVector(m_secondsFromFirstReport);

    long durationSeconds = 0;
    if (!m_secondsFromFirstReport.empty()) {
      durationSeconds = m_secondsFromFirstReport.back();
    }

    // check for wrap around
    boost::optional<int> calendarYear = m_firstReportDateTime.date().baseYear();
    if (!calendarYear) {
      DateTime lastDateTime = m_firstReportDateTime + Time(0, 0, 0, durationSeconds);
      Date lastDate(lastDateTime.date().monthOfYear(), lastDateTime.date().dayOfMonth());
      if ((durationSeconds > Time(366.0).totalSeconds()) || (lastDate < m_firstReportDateTime.date())) {
        m_wrapAround = true;
      }
    }
  }

  /// interval length if any
  OptionalTime TimeSeries_Impl::intervalLength() const {
    return m_intervalLength;
  }

  DateTimeVector TimeSeries_Impl::dateTimes() const {
    DateTimeVector dateTimeObjs(m_secondsFromFirstReport.size());
    for (unsigned i = 0; i < m_secondsFromFirstReport.size(); i++) {
      dateTimeObjs[i] = m_firstReportDateTime + openstudio::Time(0, 0, 0, m_secondsFromFirstReport[i]);
    }
    return dateTimeObjs;
  }

  /// time in days from end of the first reporting interval
  Vector TimeSeries_Impl::daysFromFirstReport() const {
    Vector daysFromFirstReport(m_secondsFromFirstReport.size());
    for (unsigned i = 0; i < m_secondsFromFirstReport.size(); i++) {
      daysFromFirstReport[i] = Time(0, 0, 0, m_secondsFromFirstReport[i]).totalDays();
    }
    return daysFromFirstReport;
  }

  /// time in days from end of the first reporting interval at index i
  double TimeSeries_Impl::daysFromFirstReport(unsigned int i) const {
    double value = m_outOfRangeValue;
    if (i < m_secondsFromFirstReport.size()) {
      value = Time(0, 0, 0, m_secondsFromFirstReport[i]).totalDays();
    }
    return value;
  }

  /// time in seconds from end of the first reporting interval
  std::vector<long> TimeSeries_Impl::secondsFromFirstReport() const {
    return m_secondsFromFirstReport;
  }

  /// time in seconds from end of the first reporting interval at index i
  long TimeSeries_Impl::secondsFromFirstReport(unsigned int i) const {
    //double value = m_outOfRangeValue; // JWD: Shouldn't the out of range value be for values only?
    long value = 0;
    if (i < m_secondsFromFirstReport.size()) {
      value = m_secondsFromFirstReport[i];
    }
    return value;
  }

  /// values
  Vector TimeSeries_Impl::values() const {
    return m_values;
  }

  /// values at index i
  double TimeSeries_Impl::values(unsigned int i) const {
    double value = m_outOfRangeValue;
    if (i < m_values.size()) {
      value = m_values[i];
    }
    return value;
  }

  /// units
  const std::string TimeSeries_Impl::units() const {
    return m_units;
  }

  /// first report date
  openstudio::DateTime TimeSeries_Impl::firstReportDateTime() const {
    return m_firstReportDateTime;
  }

  /// start date
  openstudio::DateTime TimeSeries_Impl::startDateTime() const {
    return m_startDateTime;
  }

  /// get value at number of seconds from start date and time
  double TimeSeries_Impl::valueAtSecondsFromFirstReport(long secondsFromFirstReport) const {
    double result = m_outOfRangeValue;

    if (m_secondsFromFirstReport.empty()) {
      LOG(Debug, "Cannot compute value because timeseries is empty");
      return result;
    }

    long duration = m_secondsFromFirstReport.back();

    if (m_intervalLength) {

      // before the start of the first interval
      if (secondsFromFirstReport <= -m_intervalLength->totalSeconds()) {
        LOG(Debug, "Cannot compute value " << secondsFromFirstReport << " seconds before first reporting time when interval length is "
                                           << *m_intervalLength);
      } else if (secondsFromFirstReport > duration) {
        // after end of time series
        LOG(Debug,
            "Cannot compute value " << secondsFromFirstReport << " seconds after first reporting time when duration is " << duration << " seconds");
      } else {
        // faster look up if know we have fixed interval
        unsigned numIntervals = secondsFromFirstReport / m_intervalLength->totalSeconds();
        unsigned remainder = secondsFromFirstReport % m_intervalLength->totalSeconds();
        unsigned index;
        if (secondsFromFirstReport < 0) {
          OS_ASSERT(numIntervals == 0);
          index = numIntervals;
        } else if (remainder > 0) {
          index = numIntervals + 1;
        } else {
          index = numIntervals;
        }

        // issue with daily, hourly flood plots when index == m_values.size()
        if (index >= m_values.size()) {
          LOG(Warn,
              "Timeseries index " << index << " is greater than or equal to values size " << m_values.size() << " and has been set to size - 1.");
          index = index - 1;
        }
        result = m_values(index);
      }

    } else {

      // before the end of the first interval, do not know when start is
      if (secondsFromFirstReport < 0) {
        LOG(Debug, "Cannot compute value " << secondsFromFirstReport << " seconds before first reporting time when interval length is unknown");

      } else if (secondsFromFirstReport > duration) {
        // after end of time series
        LOG(Debug,
            "Cannot compute value " << secondsFromFirstReport << " seconds after first reporting time when duration is " << duration << " seconds");
      } else {
        // normal interpolation
        result = interp(m_secondsFromFirstReportAsVector, m_values, secondsFromFirstReport, HoldNextInterp, NoneExtrap);
      }
    }

    return result;
  }

  /// get value at number of days from start date
  double TimeSeries_Impl::value(double daysFromFirstReport) const {
    return valueAtSecondsFromFirstReport(Time(daysFromFirstReport).totalSeconds());
  }

  /// get value at time from first report date and time
  double TimeSeries_Impl::value(const Time& timeFromFirstReport) const {
    return valueAtSecondsFromFirstReport(timeFromFirstReport.totalSeconds());
  }

  /// get value at date and time
  double TimeSeries_Impl::value(const DateTime& dateTime) const {
    boost::optional<int> calendarYear = m_firstReportDateTime.date().baseYear();

    // If our timeseries doesn't have a year, we force it to the assumed one
    DateTime firstReportDateTimeWithYear = m_firstReportDateTime;
    int timeSeriesYear = m_firstReportDateTime.date().year();
    if (!calendarYear) {
      firstReportDateTimeWithYear = DateTime(
        Date(m_firstReportDateTime.date().monthOfYear(), m_firstReportDateTime.date().dayOfMonth(), timeSeriesYear), m_firstReportDateTime.time());
    }

    // If our requested datetime doesn't have an assigned year, we default to the one of the **TimeSeries** (whether hard assigned or not)
    // Or the one right after in case it ends up before the start (try wrap-around)
    DateTime dateTimeWithYear = dateTime;
    if (!dateTimeWithYear.date().baseYear()) {
      // Starts by assuming same year as timeseries, then use that to compare, and if before, then assume wrap-around
      // Even if it doesn't really wrap-around, it'll just return nothing so it's fine
      // (and valueAtSecondsFromFirstReport will also add Debug message)
      dateTimeWithYear = DateTime(Date(dateTime.date().monthOfYear(), dateTime.date().dayOfMonth(), timeSeriesYear), dateTime.time());

      int secondsFromFirstReport = (dateTimeWithYear - firstReportDateTimeWithYear).totalSeconds();

      // If it's negative, then: if m_intervalLength exist we check that it's even bigger than intervalLength,
      // in which case we do shift to next year, otherwise we do nothing
      // This allows passing "2005-01-01 00:01:00" to report at "2005-01-01 01:00:00" (historical behavior)
      // cf valueAtSecondsFromFirstReport which will allow it
      if (secondsFromFirstReport < 0 && (!m_intervalLength || secondsFromFirstReport <= -m_intervalLength->totalSeconds())) {
        dateTimeWithYear = DateTime(Date(dateTime.date().monthOfYear(), dateTime.date().dayOfMonth(), timeSeriesYear + 1), dateTime.time());
      }
    }

    LOG(Debug, "Initial: dateTime=" << dateTime << ", m_firstReportDateTime=" << m_firstReportDateTime);
    LOG(Debug, "Querying with dateTimeWithYear=" << dateTimeWithYear << ", firstReportDateTimeWithYear=" << firstReportDateTimeWithYear);

    return value(dateTimeWithYear - firstReportDateTimeWithYear);
  }

  /// get values between start and end date times
  Vector TimeSeries_Impl::values(const DateTime& startDateTime, const DateTime& endDateTime) const {
    boost::optional<int> calendarYear = m_firstReportDateTime.date().baseYear();

    // If our timeseries doesn't have a year, we force it to the assumed one
    DateTime firstReportDateTimeWithYear = m_firstReportDateTime;
    int timeSeriesYear = m_firstReportDateTime.date().year();
    if (!calendarYear) {
      firstReportDateTimeWithYear = DateTime(
        Date(m_firstReportDateTime.date().monthOfYear(), m_firstReportDateTime.date().dayOfMonth(), timeSeriesYear), m_firstReportDateTime.time());
    }

    // If our requested start doen't have an assigned year, we default to the one of the **TimeSeries** (whether hard assigned or not)
    DateTime startDateTimeWithYear = startDateTime;
    if (!startDateTime.date().baseYear()) {
      startDateTimeWithYear =
        DateTime(Date(startDateTime.date().monthOfYear(), startDateTime.date().dayOfMonth(), timeSeriesYear), startDateTime.time());
    }

    // Then if our end doesn't have an assigned year, we default it based on the start one
    DateTime endDateTimeWithYear = endDateTime;
    if (!endDateTime.date().baseYear()) {

      // Compare on a Month/Day basis only
      DateTime startDateTimeCompare = DateTime(Date(startDateTime.date().monthOfYear(), startDateTime.date().dayOfMonth()), startDateTime.time());
      DateTime endDateTimeCompare = DateTime(Date(endDateTime.date().monthOfYear(), endDateTime.date().dayOfMonth()), endDateTime.time());

      if (endDateTimeCompare <= startDateTimeCompare) {
        endDateTimeWithYear = DateTime(
          Date(endDateTime.date().monthOfYear(), endDateTime.date().dayOfMonth(), startDateTimeWithYear.date().year() + 1), endDateTime.time());
      } else {
        int totalSeconds = (endDateTimeCompare - startDateTimeCompare).totalSeconds();
        endDateTimeWithYear = startDateTimeWithYear + Time(0, 0, 0, totalSeconds);
        // endDateTimeWithYear = DateTime(Date(endDateTime.date().monthOfYear(), endDateTime.date().dayOfMonth(), timeSeriesYear), endDateTime.time());
      }
    }

    // After our thing, we ensure that we do end up with dates that make sense
    if (endDateTimeWithYear < startDateTimeWithYear) {
      LOG(Warn, "Incorrect DateTimes passed, ensure that the start and end dates are coherent");
    }

    LOG(Debug, "Initial: startDateTime=" << startDateTime << ", endDateTime=" << endDateTime << ", m_firstReportDateTime=" << m_firstReportDateTime);
    LOG(Debug, "Querying with startDateTimeWithYear=" << startDateTimeWithYear << ", endDateTimeWithYear=" << endDateTimeWithYear
                                                      << ", firstReportDateTimeWithYear=" << firstReportDateTimeWithYear);

    double startSecondsFromFirstReport = (startDateTimeWithYear - firstReportDateTimeWithYear).totalSeconds();
    double endSecondsFromFirstReport = (endDateTimeWithYear - firstReportDateTimeWithYear).totalSeconds();

    unsigned numValues = m_values.size();
    OS_ASSERT(numValues == m_secondsFromFirstReport.size());

    Vector result(numValues);
    unsigned resultSize = 0;
    for (unsigned i = 0; i < numValues; ++i) {
      if ((m_secondsFromFirstReport[i] >= startSecondsFromFirstReport) && (m_secondsFromFirstReport[i] <= endSecondsFromFirstReport)) {
        result[resultSize] = m_values[i];
        ++resultSize;
      }
    }

    result.resize(resultSize, true);
    // Warn if empty
    if (resultSize == 0) {
      LOG(Warn, "The combination of start and end DateTimes you passed resulted in zero values");
    }

    return result;
  }

  /// get the value used for out of range data
  double TimeSeries_Impl::outOfRangeValue() const {
    return m_outOfRangeValue;
  }

  /// set the value used for out of range data, defaults to 0
  void TimeSeries_Impl::setOutOfRangeValue(double value) {
    m_outOfRangeValue = value;
  }

  /// add timeseries
  std::shared_ptr<TimeSeries_Impl> TimeSeries_Impl::operator+(const TimeSeries_Impl& other) const {
    std::shared_ptr<TimeSeries_Impl> result(new TimeSeries_Impl());

    // if same units
    if (m_units == other.units()) {

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
      for (const DateTime& dt : dateTimes) {
        values[valueIndex] = value(dt) + other.value(dt);

        LOG(Debug, "At '" << dt << "' " << value(dt) << " + " << other.value(dt) << " = " << values[valueIndex]);

        ++valueIndex;
      }

      // make new result
      result = std::shared_ptr<TimeSeries_Impl>(new TimeSeries_Impl(dateTimes, values, m_units));

    } else {
      LOG(Warn, "Adding timeseries with different units returns an empty timeseries");
    }

    return result;
  }

  /// subtract timeseries
  std::shared_ptr<TimeSeries_Impl> TimeSeries_Impl::operator-(const TimeSeries_Impl& other) const {
    std::shared_ptr<TimeSeries_Impl> result(new TimeSeries_Impl());

    // if same units
    if (m_units == other.units()) {

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
      for (const DateTime& dt : dateTimes) {
        values[valueIndex] = value(dt) - other.value(dt);

        LOG(Debug, "At '" << dt << "' " << value(dt) << " - " << other.value(dt) << " = " << values[valueIndex]);

        ++valueIndex;
      }

      // make new result
      result = std::shared_ptr<TimeSeries_Impl>(new TimeSeries_Impl(dateTimes, values, m_units));

    } else {
      LOG(Warn, "Subtracting timeseries with different units returns an empty timeseries");
    }

    return result;
  }

  std::shared_ptr<TimeSeries_Impl> TimeSeries_Impl::operator*(double d) const {
    if (m_intervalLength) {
      return std::shared_ptr<TimeSeries_Impl>(new TimeSeries_Impl(m_firstReportDateTime, m_intervalLength.get(), m_values * d, m_units));
    }
    return std::shared_ptr<TimeSeries_Impl>(new TimeSeries_Impl(m_firstReportDateTime, m_secondsFromFirstReport, m_values * d, m_units));
  }

  double TimeSeries_Impl::integrate() const {
    double result = 0;
    if (m_intervalLength) {
      int secs = m_intervalLength.get().totalSeconds();
      // Use a Riemann sum to integrate under the curve
      for (unsigned i = 0; i < m_values.size(); i++) {
        result += secs * m_values[i];
      }
    } else {
      double lastTime = 0;
      // Use a Riemann sum to integrate under the curve
      for (unsigned i = 0; i < m_values.size(); i++) {
        result += (m_secondsFromStart[i] - lastTime) * m_values[i];
        lastTime = m_secondsFromStart[i];
      }
    }
    return result;
  }

  double TimeSeries_Impl::averageValue() const {
    if (!m_secondsFromStart.empty()) {
      return integrate() / m_secondsFromStart.back();
    }
    return 0;
  }

}  // namespace detail

TimeSeries::TimeSeries() : m_impl(std::shared_ptr<detail::TimeSeries_Impl>(new detail::TimeSeries_Impl())) {}

TimeSeries::TimeSeries(const Date& startDate, const Time& intervalLength, const Vector& values, const std::string& units)
  : m_impl(std::shared_ptr<detail::TimeSeries_Impl>(new detail::TimeSeries_Impl(startDate, intervalLength, values, units))) {}

TimeSeries::TimeSeries(const DateTime& firstReportDateTime, const Time& intervalLength, const Vector& values, const std::string& units)
  : m_impl(std::shared_ptr<detail::TimeSeries_Impl>(new detail::TimeSeries_Impl(firstReportDateTime, intervalLength, values, units))) {}

TimeSeries::TimeSeries(const DateTime& firstReportDateTime, const Vector& timeInDays, const Vector& values, const std::string& units)
  : m_impl(std::shared_ptr<detail::TimeSeries_Impl>(new detail::TimeSeries_Impl(firstReportDateTime, timeInDays, values, units))) {}

TimeSeries::TimeSeries(const DateTime& firstReportDateTime, const std::vector<double>& timeInDays, const std::vector<double>& values,
                       const std::string& units)
  : m_impl(std::shared_ptr<detail::TimeSeries_Impl>(new detail::TimeSeries_Impl(firstReportDateTime, timeInDays, values, units))) {}

TimeSeries::TimeSeries(const DateTimeVector& dateTimes, const Vector& values, const std::string& units)
  : m_impl(std::shared_ptr<detail::TimeSeries_Impl>(new detail::TimeSeries_Impl(dateTimes, values, units))) {}

TimeSeries::TimeSeries(const DateTime& firstReportDateTime, const std::vector<long>& timeInSeconds, const Vector& values, const std::string& units)
  : m_impl(std::shared_ptr<detail::TimeSeries_Impl>(new detail::TimeSeries_Impl(firstReportDateTime, timeInSeconds, values, units))) {}

openstudio::OptionalTime TimeSeries::intervalLength() const {
  return m_impl->intervalLength();
}

openstudio::DateTimeVector TimeSeries::dateTimes() const {
  return m_impl->dateTimes();
}

openstudio::DateTime TimeSeries::firstReportDateTime() const {
  return m_impl->firstReportDateTime();
}

openstudio::DateTime TimeSeries::startDateTime() const {
  return m_impl->startDateTime();
}

openstudio::Vector TimeSeries::daysFromFirstReport() const {
  return m_impl->daysFromFirstReport();
}

double TimeSeries::daysFromFirstReport(unsigned int i) const {
  return m_impl->daysFromFirstReport(i);
}

std::vector<long> TimeSeries::secondsFromFirstReport() const {
  return m_impl->secondsFromFirstReport();
}

long TimeSeries::secondsFromFirstReport(unsigned int i) const {
  return m_impl->secondsFromFirstReport(i);
}

openstudio::Vector TimeSeries::values() const {
  return m_impl->values();
}

double TimeSeries::values(unsigned int i) const {
  return m_impl->values(i);
}

const std::string TimeSeries::units() const {
  return m_impl->units();
}

double TimeSeries::value(double daysFromFirstReport) const {
  return m_impl->value(daysFromFirstReport);
}

double TimeSeries::value(const Time& timeFromFirstReport) const {
  return m_impl->value(timeFromFirstReport);
}

double TimeSeries::value(const DateTime& dateTime) const {
  return m_impl->value(dateTime);
}

Vector TimeSeries::values(const DateTime& startDateTime, const DateTime& endDateTime) const {
  return m_impl->values(startDateTime, endDateTime);
}

double TimeSeries::outOfRangeValue() const {
  return m_impl->outOfRangeValue();
}

void TimeSeries::setOutOfRangeValue(double value) {
  m_impl->setOutOfRangeValue(value);
}

TimeSeries TimeSeries::operator+(const TimeSeries& other) const {
  std::shared_ptr<detail::TimeSeries_Impl> impl = (*m_impl) + *(other.m_impl);
  return {impl};
}

TimeSeries TimeSeries::operator-(const TimeSeries& other) const {
  std::shared_ptr<detail::TimeSeries_Impl> impl = (*m_impl) - *(other.m_impl);
  return {impl};
}

TimeSeries TimeSeries::operator*(double d) const {
  std::shared_ptr<detail::TimeSeries_Impl> impl = (*m_impl) * d;
  return {impl};
}

TimeSeries TimeSeries::operator/(double d) const {
  std::shared_ptr<detail::TimeSeries_Impl> impl = (*m_impl) * (1.0 / d);
  return {impl};
}

double TimeSeries::integrate() const {
  return m_impl->integrate();
}

double TimeSeries::averageValue() const {
  return m_impl->averageValue();
}

TimeSeries::TimeSeries(std::shared_ptr<detail::TimeSeries_Impl> impl) : m_impl(impl) {}

TimeSeries operator*(double d, const TimeSeries& series) {
  return series * d;
}

TimeSeries sum(const std::vector<TimeSeries>& timeSeriesVector) {
  TimeSeries result;
  bool first = true;
  for (const TimeSeries& ts : timeSeriesVector) {
    if (first) {
      result = ts;
    } else {
      result = result + ts;
    }
    if (result.values().empty()) {
      LOG_FREE(Info, "zero.sum", "Could not sum the timeSeriesVector. Either the first series is empty, or the " << "units are incompatible.");
      break;
    }
    first = false;
  }
  return result;
}

boost::function1<TimeSeries, const std::vector<TimeSeries>&> sumTimeSeriesFunctor() {
  using functype = TimeSeries (*)(const std::vector<TimeSeries>&);
  return std::function<TimeSeries(const std::vector<TimeSeries>&)>(functype(&sum));
}

TimeSeries evaluateTimeSeriesFromTimeSeriesVectorFunctor(const boost::function1<TimeSeries, const std::vector<TimeSeries>&>& functor,
                                                         const std::vector<TimeSeries>& timeSeriesVector) {
  return functor(timeSeriesVector);
}

}  // namespace openstudio
