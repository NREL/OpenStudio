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
#include <fmt/printf.h>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

static unsigned startNewDay(IdfObject &idfObject,unsigned fieldIndex,Date date)
{
  std::string string = fmt::sprintf("Through: %02d/%02d",date.monthOfYear().value(),date.dayOfMonth());
  idfObject.setString(fieldIndex, string);
  ++fieldIndex;
  idfObject.setString(fieldIndex, "For: AllDays");
  ++fieldIndex;
  return fieldIndex;
}

static unsigned addUntil(IdfObject &idfObject,unsigned fieldIndex,int hours,int minutes,double value)
{
  std::string string = fmt::sprintf("Until: %02d:%02d",hours,minutes);
  idfObject.setString(fieldIndex, string);
  ++fieldIndex;
  idfObject.setDouble(fieldIndex, value);
  ++fieldIndex;
  return fieldIndex;
}

boost::optional<IdfObject> ForwardTranslator::translateScheduleFixedInterval( ScheduleFixedInterval & modelObject )
{
  std::string name = modelObject.name().get();
  boost::optional<ScheduleTypeLimits> scheduleTypeLimits;

  TimeSeries timeseries = modelObject.timeSeries();
  // Check that the time series has at least one point
  if(timeseries.values().size() == 0)
  {
    LOG(Error,"Time series in schedule '" << modelObject.name().get() << "' has no values, schedule will not be translated");
    return boost::optional<IdfObject>();
  }

  if (modelObject.translatetoScheduleFile()) { // create a ScheduleFile

    std::string fileName = name + ".csv";
    path filePath;
    std::vector<path> absoluteFilePaths = modelObject.model().workflowJSON().absoluteFilePaths();
    if (absoluteFilePaths.empty()) {
      filePath = modelObject.model().workflowJSON().absoluteRootDir() / toPath(fileName);
    } else {
      filePath = absoluteFilePaths[0] / toPath(fileName);
    }

    CSVFile csvFile;
    csvFile.addColumn(timeseries.dateTimes());
    csvFile.addColumn(timeseries.values());
    csvFile.saveAs(filePath);

    boost::optional<ExternalFile> externalFile = ExternalFile::getExternalFile(modelObject.model(), toString(filePath));
    if (!externalFile) {
      LOG(Error,"Cannot find file at '" << filePath << ", schedule will not be translated");
      return boost::optional<IdfObject>();
    }

    // create ScheduleFile object pointing to ExternalFile
    ScheduleFile scheduleFile = ScheduleFile(*externalFile, 2);
    modelObject.setName("object will not be forward translated"); // otherwise you'd have two model objects with the same name
    scheduleFile.setName(name);
    scheduleTypeLimits = modelObject.scheduleTypeLimits();
    if (scheduleTypeLimits){
      scheduleFile.setScheduleTypeLimits(*scheduleTypeLimits);
    }
    scheduleFile.setInterpolatetoTimestep(modelObject.interpolatetoTimestep());

    return translateAndMapModelObject(scheduleFile);

  } else { // create a ScheduleCompact
    IdfObject idfObject( openstudio::IddObjectType::Schedule_Compact );

    m_idfObjects.push_back(idfObject);

    idfObject.setName(name);

    scheduleTypeLimits = modelObject.scheduleTypeLimits();
    if (scheduleTypeLimits){
      boost::optional<IdfObject> idfScheduleTypeLimits = translateAndMapModelObject(*scheduleTypeLimits);
      if (idfScheduleTypeLimits){
        idfObject.setString(Schedule_CompactFields::ScheduleTypeLimitsName, idfScheduleTypeLimits->name().get());
      }
    }

    DateTime firstReportDateTime = timeseries.firstReportDateTime();
    Vector daysFromFirst = timeseries.daysFromFirstReport();
    std::vector<long> secondsFromFirst = timeseries.secondsFromFirstReport();
    Vector values = timeseries.values();

    // We aren't using this - should we?
    std::string interpolateField = "Interpolate:No";
    if (modelObject.interpolatetoTimestep()){
      interpolateField = "Interpolate:Yes";
    }

    // New version assumes that the interval is less than one day.
    // The original version did not, so it was a bit more complicated.
    // The last date data was written
    Date lastDate = firstReportDateTime.date();
    Time dayDelta = Time(1.0);
    // The day number of the date that data was last written relative to the first date
    //double lastDay = 0.0;
    int lastDay = 0;
    // Adjust the floating point day delta to be relative to the beginning of the first day and
    // shift the start of the loop if needed
    int secondShift = firstReportDateTime.time().totalSeconds();
    unsigned int start = 0;
    if(secondShift == 0) {
      start = 1;
    } else {
      for(unsigned int i=0;i<secondsFromFirst.size();i++) {
        secondsFromFirst[i] += secondShift;
      }
    }

    // Start the input into the schedule object
    unsigned fieldIndex = Schedule_CompactFields::ScheduleTypeLimitsName + 1;
    //idfObject.setString(fieldIndex, interpolateField);
    //++fieldIndex;
    fieldIndex = startNewDay(idfObject,fieldIndex,lastDate);

    for(unsigned int i=start; i < values.size()-1; i++)
    {
      // Loop over the time series values and write out values to the
      // schedule. This version is based on the seconds from the start
      // of the time series, so should not be vulnerable to round-off.
      // It was translated from the day version, so there could be
      // issues associated with that.
      //
      // We still have a potential aliasing problem unless the API has
      // enforced that the times in the time series are all distinct when
      // rounded to the minute. Is that happening?
      int secondsFromStartOfDay = secondsFromFirst[i] % 86400;
      int today = (secondsFromFirst[i]-secondsFromStartOfDay)/86400;
      // Check to see if we are at the end of a day.
      if(secondsFromStartOfDay==0 || secondsFromStartOfDay==86400) {
        // This value is an end of day value, so end the day and set up the next
        // Note that 00:00:00 counts as the end of the previous day - we only write
        // out the 24:00:00 value and not both.
        fieldIndex = addUntil(idfObject,fieldIndex,24,0,values[i]);
        lastDate += dayDelta;
        fieldIndex = startNewDay(idfObject,fieldIndex,lastDate);
      } else {
        // This still could be on a different day
        if(today != lastDay) {
          // We're on a new day, need a 24:00:00 value and set up the next day
          fieldIndex = addUntil(idfObject,fieldIndex,24,0,values[i]);
          lastDate += dayDelta;
          fieldIndex = startNewDay(idfObject,fieldIndex,lastDate);
        }
        if(values[i] == values[i+1]){
          // Bail on values that match the next value
          continue;
        }
        // Write out the current entry
        Time time(0,0,0,secondsFromStartOfDay);
        int hours = time.hours();
        int minutes = time.minutes() + static_cast<int>(std::floor((time.seconds()/60.0) + 0.5));
        // This is a little dangerous, but all of the problematic 24:00
        // times that might need to cause a day++ should be caught above.
        if(minutes==60)
        {
          hours += 1;
          minutes = 0;
        }
        fieldIndex = addUntil(idfObject,fieldIndex,hours,minutes,values[i]);
      }
      lastDay = today;
    }
    // Handle the last point a little differently to make sure that the schedule ends exactly on the end of a day
    unsigned int i = values.size()-1;
    // We'll skip a sanity check here, but it might be a good idea to add one at some point
    fieldIndex = addUntil(idfObject,fieldIndex,24,0,values[i]);

    return boost::optional<IdfObject>(idfObject);
  }

}

} // energyplus

} // openstudio

