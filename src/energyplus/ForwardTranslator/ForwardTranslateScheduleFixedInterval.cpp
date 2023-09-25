/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleFixedInterval.hpp"
#include "../../model/ScheduleFixedInterval_Impl.hpp"
#include "../../model/ScheduleFile.hpp"
#include "../../model/ScheduleFile_Impl.hpp"
#include "../../model/ExternalFile.hpp"
#include "../../model/ExternalFile_Impl.hpp"

#include "../../utilities/data/TimeSeries.hpp"
#include "../../utilities/filetypes/CSVFile.hpp"

#include <utilities/idd/Schedule_Compact_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include <fmt/format.h>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  static unsigned startNewDay(IdfObject& idfObject, unsigned fieldIndex, const Date& date) {
    const std::string s = fmt::format("Through: {:02d}/{:02d}", month(date.monthOfYear()), date.dayOfMonth());
    idfObject.setString(fieldIndex, s);
    ++fieldIndex;
    idfObject.setString(fieldIndex, "For: AllDays");
    ++fieldIndex;
    return fieldIndex;
  }

  static unsigned addUntil(IdfObject& idfObject, unsigned fieldIndex, int hours, int minutes, double value) {
    const std::string s = fmt::format("Until: {:02d}:{:02d}", hours, minutes);
    idfObject.setString(fieldIndex, s);
    ++fieldIndex;
    idfObject.setDouble(fieldIndex, value);
    ++fieldIndex;
    return fieldIndex;
  }

  boost::optional<IdfObject> ForwardTranslator::translateScheduleFixedInterval(ScheduleFixedInterval& modelObject) {
    const std::string name = modelObject.nameString();
    boost::optional<ScheduleTypeLimits> scheduleTypeLimits;

    const TimeSeries timeseries = modelObject.timeSeries();
    // Check that the time series has at least one point
    if (timeseries.values().empty()) {
      LOG(Error, "Time series in schedule '" << modelObject.name().get() << "' has no values, schedule will not be translated");
      return boost::none;
    }

    if (modelObject.translatetoScheduleFile()) {  // create a ScheduleFile

      const openstudio::path fileNamePath = toPath(name + ".csv");
      openstudio::path filePath;
      std::vector<openstudio::path> absoluteFilePaths = modelObject.model().workflowJSON().absoluteFilePaths();
      if (absoluteFilePaths.empty()) {
        filePath = modelObject.model().workflowJSON().absoluteRootDir() / fileNamePath;
      } else {
        filePath = absoluteFilePaths[0] / fileNamePath;
      }

      CSVFile csvFile;
      csvFile.addColumn(timeseries.dateTimes());
      csvFile.addColumn(timeseries.values());
      csvFile.saveAs(filePath);

      boost::optional<ExternalFile> externalFile = ExternalFile::getExternalFile(modelObject.model(), toString(filePath));
      if (!externalFile) {
        LOG(Error, "Cannot find file at '" << filePath << ", schedule will not be translated");
        return boost::none;
      }

      // create ScheduleFile object pointing to ExternalFile
      ScheduleFile scheduleFile = ScheduleFile(*externalFile, 2);
      modelObject.setName("object will not be forward translated");  // otherwise you'd have two model objects with the same name
      scheduleFile.setName(name);
      scheduleTypeLimits = modelObject.scheduleTypeLimits();
      if (scheduleTypeLimits) {
        scheduleFile.setScheduleTypeLimits(*scheduleTypeLimits);
      }
      scheduleFile.setInterpolatetoTimestep(modelObject.interpolatetoTimestep());

      return translateAndMapModelObject(scheduleFile);

    } else {  // create a ScheduleCompact
      IdfObject idfObject(openstudio::IddObjectType::Schedule_Compact);

      m_idfObjects.push_back(idfObject);

      idfObject.setName(name);

      scheduleTypeLimits = modelObject.scheduleTypeLimits();
      if (scheduleTypeLimits) {
        boost::optional<IdfObject> idfScheduleTypeLimits = translateAndMapModelObject(*scheduleTypeLimits);
        if (idfScheduleTypeLimits) {
          idfObject.setString(Schedule_CompactFields::ScheduleTypeLimitsName, idfScheduleTypeLimits->name().get());
        }
      }

      const DateTime firstReportDateTime = timeseries.firstReportDateTime();
      const Vector daysFromFirst = timeseries.daysFromFirstReport();
      std::vector<long> secondsFromFirst = timeseries.secondsFromFirstReport();
      Vector values = timeseries.values();

      // We aren't using this - should we?
      std::string interpolateField = "Interpolate:No";
      if (modelObject.interpolatetoTimestep()) {
        interpolateField = "Interpolate:Yes";
      }

      // New version assumes that the interval is less than one day.
      // The original version did not, so it was a bit more complicated.
      // The last date data was written
      Date lastDate = firstReportDateTime.date();
      const Time dayDelta = Time(1.0);
      // The day number of the date that data was last written relative to the first date
      //double lastDay = 0.0;
      int lastDay = 0;
      // Adjust the floating point day delta to be relative to the beginning of the first day and
      // shift the start of the loop if needed
      const int secondShift = firstReportDateTime.time().totalSeconds();
      unsigned int start = 0;
      unsigned int nDays = 1;
      if (secondShift == 0) {
        start = 1;
        // JJR: interval lengths of at least one day shouldn't shift the start of the loop, right? why would we ever start with the second element of the values vector?
        const int intervalLength = modelObject.intervalLength();

        // If this is an interval representing one or more days
        if (intervalLength % 1440 == 0) {
          start = 0;
          nDays = intervalLength / 1440;
          lastDate -= dayDelta;
        }
      } else {
        for (auto& i : secondsFromFirst) {
          i += secondShift;
        }
      }

      // Start the input into the schedule object
      unsigned fieldIndex = Schedule_CompactFields::ScheduleTypeLimitsName + 1;
      //idfObject.setString(fieldIndex, interpolateField);
      //++fieldIndex;
      fieldIndex = startNewDay(idfObject, fieldIndex, lastDate);

      for (unsigned int i = start; i < values.size() - 1; i++) {
        // Loop over the time series values and write out values to the
        // schedule. This version is based on the seconds from the start
        // of the time series, so should not be vulnerable to round-off.
        // It was translated from the day version, so there could be
        // issues associated with that.
        //
        // We still have a potential aliasing problem unless the API has
        // enforced that the times in the time series are all distinct when
        // rounded to the minute. Is that happening?
        const int secondsFromStartOfDay = secondsFromFirst[i] % 86400;
        const int today = (secondsFromFirst[i] - secondsFromStartOfDay) / 86400;
        // Check to see if we are at the end of a day.
        if (secondsFromStartOfDay == 0 || secondsFromStartOfDay == 86400) {
          // This value is an end of day value, so end the day and set up the next
          // Note that 00:00:00 counts as the end of the previous day - we only write
          // out the 24:00:00 value and not both.
          fieldIndex = addUntil(idfObject, fieldIndex, 24, 0, values[i]);
          lastDate += dayDelta * nDays;
          fieldIndex = startNewDay(idfObject, fieldIndex, lastDate);
        } else {
          // This still could be on a different day
          if (today != lastDay) {
            // We're on a new day, need a 24:00:00 value and set up the next day
            fieldIndex = addUntil(idfObject, fieldIndex, 24, 0, values[i]);
            lastDate += dayDelta * nDays;
            fieldIndex = startNewDay(idfObject, fieldIndex, lastDate);
          }
          if (values[i] == values[i + 1]) {
            // Bail on values that match the next value
            continue;
          }
          // Write out the current entry
          const Time time(0, 0, 0, secondsFromStartOfDay);
          int hours = time.hours();
          int minutes = time.minutes() + static_cast<int>(std::floor((time.seconds() / 60.0) + 0.5));
          // This is a little dangerous, but all of the problematic 24:00
          // times that might need to cause a day++ should be caught above.
          if (minutes == 60) {
            hours += 1;
            minutes = 0;
          }
          fieldIndex = addUntil(idfObject, fieldIndex, hours, minutes, values[i]);
        }
        lastDay = today;
      }
      // Handle the last point a little differently to make sure that the schedule ends exactly on the end of a day
      const unsigned int i = values.size() - 1;
      // We'll skip a sanity check here, but it might be a good idea to add one at some point
      fieldIndex = addUntil(idfObject, fieldIndex, 24, 0, values[i]);

      return idfObject;
    }
  }

}  // namespace energyplus

}  // namespace openstudio
