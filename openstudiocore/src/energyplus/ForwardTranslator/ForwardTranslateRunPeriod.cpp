/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "../ForwardTranslator.hpp"
#include "../../model/RunPeriod.hpp"
#include "../../model/YearDescription.hpp"
#include "../../model/YearDescription_Impl.hpp"
#include <utilities/idd/RunPeriod_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/time/Date.hpp"

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateRunPeriod( RunPeriod& modelObject )
{
  IdfObject runPeriod( openstudio::IddObjectType::RunPeriod );

  m_idfObjects.push_back(runPeriod);

  OptionalString s;

  s = modelObject.name();
  if( s )
  {
    runPeriod.setName(*s);
  }
  runPeriod.setInt(openstudio::RunPeriodFields::BeginMonth,modelObject.getBeginMonth());
  runPeriod.setInt(openstudio::RunPeriodFields::BeginDayofMonth,modelObject.getBeginDayOfMonth());
  runPeriod.setInt(openstudio::RunPeriodFields::EndMonth,modelObject.getEndMonth());
  runPeriod.setInt(openstudio::RunPeriodFields::EndDayofMonth,modelObject.getEndDayOfMonth());

  model::YearDescription yd = modelObject.model().getUniqueModelObject<model::YearDescription>();
  Date jan1 = yd.makeDate(1,1);

  // ETH@20121219 - This always hard codes a day of the week to start on, even if the user 
  // specified "UseWeatherFile". It is important to keep it this way for now, since we are
  // not parsing the start day out of the weather file, and we are hard-setting the day schedules
  // based on the settings/assumptions of YearDescription (assumes 2009 if 'UseWeatherFile').
  switch(jan1.dayOfWeek().value()){
    case DayOfWeek::Sunday:
      runPeriod.setString(openstudio::RunPeriodFields::DayofWeekforStartDay,"Sunday");
      break;
    case DayOfWeek::Monday:
      runPeriod.setString(openstudio::RunPeriodFields::DayofWeekforStartDay,"Monday");
      break;
    case DayOfWeek::Tuesday:
      runPeriod.setString(openstudio::RunPeriodFields::DayofWeekforStartDay,"Tuesday");
      break;
    case DayOfWeek::Wednesday:
      runPeriod.setString(openstudio::RunPeriodFields::DayofWeekforStartDay,"Wednesday");
      break;
    case DayOfWeek::Thursday:
      runPeriod.setString(openstudio::RunPeriodFields::DayofWeekforStartDay,"Thursday");
      break;
    case DayOfWeek::Friday:
      runPeriod.setString(openstudio::RunPeriodFields::DayofWeekforStartDay,"Friday");
      break;
    case DayOfWeek::Saturday:
      runPeriod.setString(openstudio::RunPeriodFields::DayofWeekforStartDay,"Saturday");
      break;
    default:
      ;
  }

  // ETH@20121219 - We don't currently support holidays. This makes sure that default day schedules 
  // do not get used unintentionally.
  // if( modelObject.getUseWeatherFileHolidays() )
  // {
  //   runPeriod.setString(openstudio::RunPeriodFields::UseWeatherFileHolidaysandSpecialDays,"Yes");
  // }
  // else
  // {
    runPeriod.setString(openstudio::RunPeriodFields::UseWeatherFileHolidaysandSpecialDays,"No");
  // }

  // ETH@20121219 - We also do not support pulling the daylight savings period in from the weather
  // file. Explicit RunPeriodControl:DaylightSavingTime objects already override this field, so 
  // making this change only affects files without such objects (and likely doesn't change results
  // for those since weather files from the E+ website have no such designations).
  // if( modelObject.getUseWeatherFileDaylightSavings() )
  // {
  //   runPeriod.setString(openstudio::RunPeriodFields::UseWeatherFileDaylightSavingPeriod,"Yes");
  // }
  // else
  // {
    runPeriod.setString(openstudio::RunPeriodFields::UseWeatherFileDaylightSavingPeriod,"No");
  // }

  if( modelObject.getApplyWeekendHolidayRule() )
  {
    runPeriod.setString(openstudio::RunPeriodFields::ApplyWeekendHolidayRule,"Yes");
  }
  else
  {
    runPeriod.setString(openstudio::RunPeriodFields::ApplyWeekendHolidayRule,"No");
  }

  if( modelObject.getUseWeatherFileRainInd() )
  {
    runPeriod.setString(openstudio::RunPeriodFields::UseWeatherFileRainIndicators,"Yes");
  }
  else
  {
    runPeriod.setString(openstudio::RunPeriodFields::UseWeatherFileRainIndicators,"No");
  }

  if( modelObject.getUseWeatherFileSnowInd() )
  {
    runPeriod.setString(openstudio::RunPeriodFields::UseWeatherFileSnowIndicators,"Yes");
  }
  else
  {
    runPeriod.setString(openstudio::RunPeriodFields::UseWeatherFileSnowIndicators,"No");
  }

  runPeriod.setInt(openstudio::RunPeriodFields::NumberofTimesRunperiodtobeRepeated,modelObject.getNumTimePeriodRepeats());

  return boost::optional<IdfObject>(runPeriod);
}

} // energyplus

} // openstudio

