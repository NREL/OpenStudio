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
#include <model/ScheduleVariableInterval.hpp>
#include <model/ScheduleVariableInterval_Impl.hpp>

#include <utilities/data/TimeSeries.hpp>

#include <utilities/idd/Schedule_Compact_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateScheduleVariableInterval( ScheduleVariableInterval & modelObject )
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

  return idfObject;
}

} // energyplus

} // openstudio

