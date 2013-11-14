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
  //LOG_FREE(Info,"openstudio.model.ScheduleFixedInterval","Day input field index is " << fieldIndex);
  idfObject.setString(fieldIndex, string.toStdString());
  ++fieldIndex;
  //LOG_FREE(Info,"openstudio.model.ScheduleFixedInterval","Field index is " << fieldIndex);
  idfObject.setString(fieldIndex, "For: AllDays");
  ++fieldIndex;
  return fieldIndex;
}

static unsigned addUntil(IdfObject &idfObject,unsigned fieldIndex,int hours,int minutes,double value)
{
  QString string = QString().sprintf("Until: %02d/%02d",hours,minutes);
  //LOG_FREE(Info,"openstudio.model.ScheduleFixedInterval","Until input field index is " << fieldIndex);
  idfObject.setString(fieldIndex, string.toStdString());
  ++fieldIndex;
  //LOG_FREE(Info,"openstudio.model.ScheduleFixedInterval","Field index is " << fieldIndex);
  idfObject.setDouble(fieldIndex, value);
  ++fieldIndex;
  //LOG_FREE(Info,"openstudio.model.ScheduleFixedInterval","Until complete");
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
  DateTime firstReportDateTime = timeseries.firstReportDateTime();
  Vector daysFromFirstReport = timeseries.daysFromFirstReport();
  Vector values = timeseries.values();

  std::string interpolateField;
  if (modelObject.interpolatetoTimestep()){
    interpolateField = "Interpolate:Yes";
  }else{
    interpolateField = "Interpolate:No";
  }
 
  Date lastDate = firstReportDateTime.date(); // The last date data was written
  Time dayDelta = Time(1.0);
  double lastDay = 0.0; // The day number of the date that data was last written relative to the first date
  unsigned int start = 0;
  if(daysFromFirstReport[0] == 0.0)
  {
    // Skip the first value if it is at 00:00:00
    unsigned start=1;
  }

  // Start the input into the schedule object
  unsigned fieldIndex = Schedule_CompactFields::ScheduleTypeLimitsName + 1;
  fieldIndex = startNewDay(idfObject,fieldIndex,lastDate);
  //QString string = QString().sprintf("Through: %02d/%02d",lastDate.monthOfYear().value(),lastDate.dayOfMonth());
  //idfObject.setString(fieldIndex, string.toStdString());
  //++fieldIndex;
  //idfObject.setString(fieldIndex, "For: AllDays");
  //++fieldIndex;

  for(unsigned int i=start; i < values.size(); i++)
  {
    // We could loop over the entire array and use the fact that the
    // last entry in the daysFromFirstReport vector should be a round
    // number to avoid logic. That is probably too tricky and could be
    // vulnerable to rounding issues. It still might be worth it to allow
    // for a margin of error (say a half second or something) to avoid
    // problems in the calculation of the fractional part (hms) below.
    double today = floor(daysFromFirstReport[i]);
    double hms = daysFromFirstReport[i]-today;
    if(hms == 0)
    {
      // This value is an end of day value, but we could have skipped multiple days
      int diff = today-lastDay;
      for(int j=diff; j>0; j--)
      {
        fieldIndex = addUntil(idfObject,fieldIndex,24,0,values[i]);
        //idfObject.setString(fieldIndex, "Until: 24:00");
        //++fieldIndex;
        //idfObject.setDouble(fieldIndex, values[i]);
        //++fieldIndex;

        lastDate += dayDelta;
        fieldIndex = startNewDay(idfObject,fieldIndex,lastDate);
        //QString string = QString().sprintf("Through: %02d/%02d",lastDate.monthOfYear().value(),lastDate.dayOfMonth());
        //idfObject.setString(fieldIndex, string.toStdString());
        //++fieldIndex;
        //idfObject.setString(fieldIndex, "For: AllDays");
        //++fieldIndex;
      }
    }
    else
    {
      if(values[i] == values[i+1])
      {
        // Bail on values that match the next value
        continue;
      }
      if(today != lastDay)
      {
        // We're on a new day, need 24:00:00 value(s), how many days did we miss?
        int diff = today-lastDay;
        for(int j=diff; j>0; j--)
        {
          fieldIndex = addUntil(idfObject,fieldIndex,24,0,values[i]);
          //idfObject.setString(fieldIndex, "Until: 24:00");
          //++fieldIndex;
          //idfObject.setDouble(fieldIndex, values[i]);
          //++fieldIndex;

          lastDate += dayDelta;
          fieldIndex = startNewDay(idfObject,fieldIndex,lastDate);
          //QString string = QString().sprintf("Through: %02d/%02d",lastDate.monthOfYear().value(),lastDate.dayOfMonth());
          //idfObject.setString(fieldIndex, string.toStdString());
          //++fieldIndex;
          //idfObject.setString(fieldIndex, "For: AllDays");
          //++fieldIndex;
        }
      }
      Time time(hms);
      int hours = time.hours();
      int minutes = time.minutes() + floor((time.seconds()/60.0) + 0.5);
      fieldIndex = addUntil(idfObject,fieldIndex,hours,minutes,values[i]);
      //QString string = QString().sprintf("Until: %02d/%02d",hours,minutes);
      //idfObject.setString(fieldIndex, string.toStdString());
      //++fieldIndex;
      //idfObject.setDouble(fieldIndex, values[i]);
      //++fieldIndex;
    }
    lastDay = today;
  }
  // Handle the last point a little differently to make sure that the schedule ends exactly on the end of a day
  int i = values.size()-1;
  double today = floor(daysFromFirstReport[i]); // Could skip this, but better to be safe
  // Did we miss any days?
  int diff = today-lastDay;
  for(int j=diff; j>0; j--)
  {
    fieldIndex = addUntil(idfObject,fieldIndex,24,0,values[i]);
    //idfObject.setString(fieldIndex, "Until: 24:00");
    //++fieldIndex;
    //idfObject.setDouble(fieldIndex, values[i]);
    //++fieldIndex;

    lastDate += dayDelta;
    fieldIndex = startNewDay(idfObject,fieldIndex,lastDate);
    //QString string = QString().sprintf("Through: %02d/%02d",lastDate.monthOfYear().value(),lastDate.dayOfMonth());
    //idfObject.setString(fieldIndex, string.toStdString());
    //++fieldIndex;
    //idfObject.setString(fieldIndex, "For: AllDays");
    //++fieldIndex;
  }

  fieldIndex = addUntil(idfObject,fieldIndex,24,0,values[i]);
  //idfObject.setString(fieldIndex, "Until: 24:00");
  //++fieldIndex;
  //idfObject.setDouble(fieldIndex, values[i]);
  //++fieldIndex;
  /*
  boost::optional<Date> lastDate;

  unsigned fieldIndex = Schedule_CompactFields::ScheduleTypeLimitsName + 1;
  unsigned N = values.size();
  for (unsigned i = 0; i < N; ++i){
    DateTime dateTime = firstReportDateTime + Time(daysFromFirstReport[i]);
    Date date = dateTime.date();
    Time time = dateTime.time();

    int hours = time.hours();
    int minutes = time.minutes() + floor((time.seconds()/60.0) + 0.5);

    if ((i < (N-1)) && (hours == 0) && (minutes == 0)){
      continue;
    }
    
    if ((i < (N-1)) && (values[i] == values[i+1])){
      DateTime dateTime2 = firstReportDateTime + Time(daysFromFirstReport[i+1]);
      Date date2 = dateTime2.date();
      if (date == date2){
        continue;
      }
    }
    
    if (minutes == 60){
      hours += 1;
      minutes = 0;
    }

    int month = date.monthOfYear().value();
    int day = date.dayOfMonth();

    if (!lastDate || (date > *lastDate)){
      
      if (lastDate){
        // we know that date > *currentDate
        if ((hours != 24) || (minutes != 0)){
          idfObject.setString(fieldIndex, "Until: 24:00");
          ++fieldIndex;
          
          idfObject.setDouble(fieldIndex, values[i-1]);
          ++fieldIndex;
        }
      }

      lastDate = date;

      std::stringstream ss;

      ss << "Through: " << setfill('0') << setw(2) << month << "/" << setfill('0') << setw(2) << day;
      idfObject.setString(fieldIndex, ss.str());
      ++fieldIndex;

      idfObject.setString(fieldIndex, "For: AllDays");
      ++fieldIndex;

      idfObject.setString(fieldIndex, interpolateField);
      ++fieldIndex;
    }

    std::stringstream ss;

    ss << "Until: " << setfill('0') << setw(2) << hours << ":" << setfill('0') << setw(2) << minutes;
    idfObject.setString(fieldIndex, ss.str());
    ++fieldIndex;

    idfObject.setDouble(fieldIndex, values[i]);
    ++fieldIndex;
    
    if (i == (N-1)){
      if ((hours != 24) || (minutes != 0)){
        idfObject.setString(fieldIndex, "Until: 24:00");
        ++fieldIndex;
        
        idfObject.setDouble(fieldIndex, values[i]);
        ++fieldIndex;
      }    
    }
  }
  */

  return idfObject;
}

} // energyplus

} // openstudio

