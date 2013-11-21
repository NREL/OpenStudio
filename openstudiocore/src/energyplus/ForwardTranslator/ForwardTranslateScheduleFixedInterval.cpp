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
  QString string = QString().sprintf("Until: %02d:%02d",hours,minutes);
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
  Vector daysFromFirst = timeseries.daysFromFirstReport();
  Vector values = timeseries.values();

  std::string interpolateField;
  if (modelObject.interpolatetoTimestep()){
    interpolateField = "Interpolate:Yes";
  }else{
    interpolateField = "Interpolate:No";
  }

  // New version starts here
  Date lastDate = firstReportDateTime.date(); // The last date data was written
  Time dayDelta = Time(1.0);
  double lastDay = 0.0; // The day number of the date that data was last written relative to the first date
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

  //unsigned N = daysFromFirst.size();
  //std::cout << N << " " << start << std::endl;
  //std::cout << daysFromFirst[0] << " " << values[0] << std::endl;
  //std::cout << daysFromFirst[1] << " " << values[1] << std::endl;
  //std::cout << daysFromFirst[N-2] << " " << values[N-2] <<  std::endl;
  //std::cout << daysFromFirst[N-1] << " " << values[N-1] <<  std::endl;

  // Start the input into the schedule object
  unsigned fieldIndex = Schedule_CompactFields::ScheduleTypeLimitsName + 1;
  fieldIndex = startNewDay(idfObject,fieldIndex,lastDate);
  //QString string = QString().sprintf("Through: %02d/%02d",lastDate.monthOfYear().value(),lastDate.dayOfMonth());
  //idfObject.setString(fieldIndex, string.toStdString());
  //++fieldIndex;
  //idfObject.setString(fieldIndex, "For: AllDays");
  //++fieldIndex;

  for(unsigned int i=start; i < values.size()-1; i++)
  {
    // We could loop over the entire array and use the fact that the
    // last entry in the daysFromFirstReport vector should be a round
    // number to avoid logic. However, this whole thing is very, very
    // sensitive to round off issues. We still have a HUGE aliasing
    // problem unless the API has enforced that the times in the 
    // time series are all distinct when rounded to the minute. Is that
    // happening?
    //std::cout << daysFromFirstReport[i] << std::endl;
    double today = floor(daysFromFirst[i]);
    double hms = daysFromFirst[i]-today;
    // Here, we need to make sure that we aren't nearly the end of a day
    // 5.787x10^-6 days is a little less than half a second.
    if(1-hms < 5.787e-6)
    {
      today += 1;
      hms = 0.0;
    }
    //std::cout << daysFromFirst[i] << " " << today << " " << hms << std::endl;
    if(hms < 5.787e-6)
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
      //std::cout << today << " " << hours << " " << minutes << " " << daysFromFirst[i] << " " 
      //  << QString().sprintf("%.15e",hms).toStdString() << std::endl;
      // This is a little dangerous, but all of the problematic 24:00 
      // times that might need to cause a day++ should be caught above.
      if(minutes==60)
      {
        hours += 1;
        minutes = 0;
      }
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
  unsigned int i = values.size()-1;
  // Could skip this, but better to be safe. Again allow up to a half second or so of error
  double today = floor(daysFromFirst[i]);
  double hms = daysFromFirst[i]-today;
  if(1-hms < 5.787e-6)
  {
    today += 1;
    hms = 0.0;
  }
  // Did we miss any days?
  int diff = today-lastDay;
  //std::cout << diff << " " << today << " " << lastDay << std::endl;
  for(int j=diff; j>1; j--)
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
  // Old version starts here
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

