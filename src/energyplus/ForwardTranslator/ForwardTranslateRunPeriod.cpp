/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
  Date firstDay = yd.makeDate(modelObject.getBeginMonth(), modelObject.getBeginDayOfMonth());
  Date endDay = yd.makeDate(modelObject.getEndMonth(), modelObject.getEndDayOfMonth());

  int beginYear = firstDay.year();
  int endYear = 0;
  if (endDay < firstDay) {
    // e.g. 4/1 - 3/31
    endYear = beginYear + modelObject.getNumTimePeriodRepeats();
  } else {
    // e.g. 1/1 - 12/31
    endYear = beginYear + modelObject.getNumTimePeriodRepeats() - 1;
  }

  runPeriod.setInt(openstudio::RunPeriodFields::BeginYear, beginYear);
  runPeriod.setInt(openstudio::RunPeriodFields::EndYear, endYear);

  // ETH@20121219 - This always hard codes a day of the week to start on, even if the user
  // specified "UseWeatherFile". It is important to keep it this way for now, since we are
  // not parsing the start day out of the weather file, and we are hard-setting the day schedules
  // based on the settings/assumptions of YearDescription (assumes 2009 if 'UseWeatherFile').
  switch (firstDay.dayOfWeek().value()) {
  case DayOfWeek::Sunday:
    runPeriod.setString(openstudio::RunPeriodFields::DayofWeekforStartDay, "Sunday");
    break;
  case DayOfWeek::Monday:
    runPeriod.setString(openstudio::RunPeriodFields::DayofWeekforStartDay, "Monday");
    break;
  case DayOfWeek::Tuesday:
    runPeriod.setString(openstudio::RunPeriodFields::DayofWeekforStartDay, "Tuesday");
    break;
  case DayOfWeek::Wednesday:
    runPeriod.setString(openstudio::RunPeriodFields::DayofWeekforStartDay, "Wednesday");
    break;
  case DayOfWeek::Thursday:
    runPeriod.setString(openstudio::RunPeriodFields::DayofWeekforStartDay, "Thursday");
    break;
  case DayOfWeek::Friday:
    runPeriod.setString(openstudio::RunPeriodFields::DayofWeekforStartDay, "Friday");
    break;
  case DayOfWeek::Saturday:
    runPeriod.setString(openstudio::RunPeriodFields::DayofWeekforStartDay, "Saturday");
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

  //runPeriod.setInt(openstudio::RunPeriodFields::NumberofTimesRunperiodtobeRepeated,modelObject.getNumTimePeriodRepeats());

  return boost::optional<IdfObject>(runPeriod);
}

} // energyplus

} // openstudio

