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

#include <energyplus/ForwardTranslator.hpp>

#include <model/Model.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleFixedInterval.hpp>
#include <model/ScheduleFixedInterval_Impl.hpp>

#include <utilities/data/TimeSeries.hpp>

#include <utilities/idd/Schedule_Compact_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

static unsigned startNewDay(IdfObject &idfObject,unsigned fieldIndex,Date date)
{
  QString string = QString().sprintf("Through: %02d/%02d",date.monthOfYear().value(),date.dayOfMonth());
  idfObject.setString(fieldIndex, string.toStdString());
  ++fieldIndex;
  idfObject.setString(fieldIndex, "For: AllDays");
  ++fieldIndex;
  return fieldIndex;
}

static unsigned addUntil(IdfObject &idfObject,unsigned fieldIndex,int hours,int minutes,double value)
{
  QString string = QString().sprintf("Until: %02d:%02d",hours,minutes);
  idfObject.setString(fieldIndex, string.toStdString());
  ++fieldIndex;
  idfObject.setDouble(fieldIndex, value);
  ++fieldIndex;
  return fieldIndex;
}

boost::optional<IdfObject> ForwardTranslator::translateScheduleFixedInterval( ScheduleFixedInterval & modelObject )
{
  IdfObject idfObject( openstudio::IddObjectType::Schedule_Compact );

  m_idfObjects.push_back(idfObject);

  idfObject.setName(modelObject.name().get());
  
  boost::optional<ScheduleTypeLimits> scheduleTypeLimits = modelObject.scheduleTypeLimits();
  if (scheduleTypeLimits){
    boost::optional<IdfObject> idfScheduleTypeLimits = translateAndMapModelObject(*scheduleTypeLimits);
    if (idfScheduleTypeLimits){
      idfObject.setString(Schedule_CompactFields::ScheduleTypeLimitsName, idfScheduleTypeLimits->name().get());
    }
  }

  TimeSeries timeseries = modelObject.timeSeries();
  // Check that the time series has at least one point
  if(timeseries.values().size() == 0)
  {
    LOG(Error,"Time series in schedule '" << modelObject.name().get() << "' has no values, schedule will not be translated");
    return boost::optional<IdfObject>();
  }
  DateTime firstReportDateTime = timeseries.firstReportDateTime();
  Vector daysFromFirst = timeseries.daysFromFirstReport();
  Vector values = timeseries.values();

  // We aren't using this - should we?
  std::string interpolateField;
  if (modelObject.interpolatetoTimestep()){
    interpolateField = "Interpolate:Yes";
  }else{
    interpolateField = "Interpolate:No";
  }

  // New version assumes that the interval is less than one day.
  // The original version did not, so it was a bit more complicated.
  // 5.787x10^-6 days is a little less than half a second
  double eps = 5.787e-6;
  double intervalDays = modelObject.intervalLength();
  // The last date data was written
  Date lastDate = firstReportDateTime.date();
  Time dayDelta = Time(1.0);
  // The day number of the date that data was last written relative to the first date
  double lastDay = 0.0; 
  // Adjust the floating point day delta to be relative to the beginning of the first day and
  // shift the start of the loop if needed
  double timeShift = firstReportDateTime.time().totalDays();
  unsigned int start = 0;
  if(timeShift == 0.0)
  {
    start = 1;
  }
  else
  {
    for(unsigned int i=0;i<daysFromFirst.size();i++)
    {
      daysFromFirst[i] += timeShift;
    }
  }

  // Start the input into the schedule object
  unsigned fieldIndex = Schedule_CompactFields::ScheduleTypeLimitsName + 1;
  fieldIndex = startNewDay(idfObject,fieldIndex,lastDate);

  for(unsigned int i=start; i < values.size()-1; i++)
  {
    // We could loop over the entire array and use the fact that the
    // last entry in the daysFromFirstReport vector should be a round
    // number to avoid logic. However, this whole thing is very, very
    // sensitive to round off issues. We still have a HUGE aliasing
    // problem unless the API has enforced that the times in the 
    // time series are all distinct when rounded to the minute. Is that
    // happening?
    double today = floor(daysFromFirst[i]);
    double hms = daysFromFirst[i]-today;
    // Here, we need to make sure that we aren't nearly the end of a day
    if(fabs(1.0-hms) < eps)
    {
      today += 1;
      hms = 0.0;
    }
    if(hms < eps)
    {
      // This value is an end of day value, so end the day and set up the next
      fieldIndex = addUntil(idfObject,fieldIndex,24,0,values[i]);
      lastDate += dayDelta;
      fieldIndex = startNewDay(idfObject,fieldIndex,lastDate);
    }
    else
    {
      if(today != lastDay)
      {
        // We're on a new day, need a 24:00:00 value and set up the next day
        fieldIndex = addUntil(idfObject,fieldIndex,24,0,values[i]);
        lastDate += dayDelta;
        fieldIndex = startNewDay(idfObject,fieldIndex,lastDate);
      }
      if(values[i] == values[i+1])
      {
        // Bail on values that match the next value
        continue;
      }
      // Write out the current entry
      Time time(hms);
      int hours = time.hours();
      int minutes = time.minutes() + floor((time.seconds()/60.0) + 0.5);
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
  
  return idfObject;
}

} // energyplus

} // openstudio

