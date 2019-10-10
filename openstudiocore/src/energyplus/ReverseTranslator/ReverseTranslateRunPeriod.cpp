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

#include "../ReverseTranslator.hpp"

#include "../../model/RunPeriod.hpp"
#include "../../model/RunPeriod_Impl.hpp"
#include "../../model/YearDescription.hpp"
#include "../../model/YearDescription_Impl.hpp"

#include <utilities/idd/RunPeriod_FieldEnums.hxx>

#include "../../utilities/time/Date.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateRunPeriod( const WorkspaceObject & workspaceObject )
{
  OptionalModelObject result;
  openstudio::model::RunPeriod runPeriod = m_model.getUniqueModelObject<openstudio::model::RunPeriod>();
  OptionalString optS = workspaceObject.name();
  if(optS)
  {
    runPeriod.setName(*optS);
  }
  OptionalInt i;

  i = workspaceObject.getInt(openstudio::RunPeriodFields::BeginMonth);
  if( i )
  {
    runPeriod.setBeginMonth( *i );
  }

  i = workspaceObject.getInt(openstudio::RunPeriodFields::BeginDayofMonth);
  if( i )
  {
    runPeriod.setBeginDayOfMonth( *i );
  }

  i = workspaceObject.getInt(openstudio::RunPeriodFields::EndMonth);
  if( i )
  {
    runPeriod.setEndMonth( *i );
  }

  i = workspaceObject.getInt(openstudio::RunPeriodFields::EndDayofMonth);
  if( i )
  {
    runPeriod.setEndDayOfMonth( *i );
  }

  optS = workspaceObject.getString(RunPeriodFields::DayofWeekforStartDay);
  if(optS)
  {
    boost::optional<model::YearDescription> yd = runPeriod.model().getOptionalUniqueModelObject<model::YearDescription>();
    if (yd){
      if (!istringEqual(*optS, yd->dayofWeekforStartDay())){
        LOG(Warn, "Multiple values detected for dayofWeekforStartDay, using " << yd->dayofWeekforStartDay());
      }
    }else{
      // create a year description
      yd = runPeriod.model().getUniqueModelObject<model::YearDescription>();
      yd->setDayofWeekforStartDay(*optS);
    }

  }//if(optS)

  optS = workspaceObject.getString(RunPeriodFields::UseWeatherFileHolidaysandSpecialDays);
  if(optS)
  {
    std::string temp=*optS;
    boost::to_lower(temp);
    if( temp == "no")
    {
      runPeriod.setUseWeatherFileHolidays(false);
    }
    else
    {runPeriod.setUseWeatherFileHolidays(true);
    }
  }
  optS = workspaceObject.getString(RunPeriodFields::UseWeatherFileDaylightSavingPeriod);
  if(optS)
  {
    std::string temp=*optS;
    boost::to_lower(temp);
    if( temp == "no")
    {
      runPeriod.setUseWeatherFileDaylightSavings(false);
    }
    else
    {
      runPeriod.setUseWeatherFileDaylightSavings(true);
    }
  }
  optS = workspaceObject.getString(RunPeriodFields::ApplyWeekendHolidayRule);
  if(optS)
  {
    std::string temp=*optS;
    boost::to_lower(temp);
    if( temp == "no")
    {
      runPeriod.setApplyWeekendHolidayRule(false);
    }
    else
    {
      runPeriod.setApplyWeekendHolidayRule(true);
    }
  }
  optS = workspaceObject.getString(RunPeriodFields::UseWeatherFileRainIndicators);
  if(optS)
  {
    std::string temp=*optS;
    boost::to_lower(temp);
    if( temp == "no")
    {
      runPeriod.setUseWeatherFileRainInd(false);
    }
    else
    {
      runPeriod.setUseWeatherFileRainInd(true);
    }
  }
  optS = workspaceObject.getString(RunPeriodFields::UseWeatherFileSnowIndicators);
  if(optS)
  {
    std::string temp=*optS;
    boost::to_lower(temp);
    if( temp == "no")
    {
      runPeriod.setUseWeatherFileSnowInd(false);
    }
    else
    {
      runPeriod.setUseWeatherFileSnowInd(true);
    }
  }

  auto beginYear = workspaceObject.getInt(openstudio::RunPeriodFields::BeginYear);
  auto endYear = workspaceObject.getInt(openstudio::RunPeriodFields::EndYear);

  if ( beginYear ) {
    auto yd = runPeriod.model().getUniqueModelObject<model::YearDescription>();
    yd.setCalendarYear(beginYear.get());

    if ( endYear ) {
      runPeriod.setNumTimePeriodRepeats(endYear.get() - beginYear.get());
    }
  }

  result = runPeriod;
  return result;
}

} // energyplus

} // openstudio

