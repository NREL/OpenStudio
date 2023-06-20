/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

  OptionalModelObject ReverseTranslator::translateRunPeriod(const WorkspaceObject& workspaceObject) {
    OptionalModelObject result;
    openstudio::model::RunPeriod runPeriod = m_model.getUniqueModelObject<openstudio::model::RunPeriod>();
    OptionalString optS = workspaceObject.name();
    if (optS) {
      runPeriod.setName(*optS);
    }
    OptionalInt i;

    i = workspaceObject.getInt(openstudio::RunPeriodFields::BeginMonth);
    if (i) {
      runPeriod.setBeginMonth(*i);
    }

    i = workspaceObject.getInt(openstudio::RunPeriodFields::BeginDayofMonth);
    if (i) {
      runPeriod.setBeginDayOfMonth(*i);
    }

    i = workspaceObject.getInt(openstudio::RunPeriodFields::EndMonth);
    if (i) {
      runPeriod.setEndMonth(*i);
    }

    i = workspaceObject.getInt(openstudio::RunPeriodFields::EndDayofMonth);
    if (i) {
      runPeriod.setEndDayOfMonth(*i);
    }

    optS = workspaceObject.getString(RunPeriodFields::DayofWeekforStartDay);
    if (optS) {
      boost::optional<model::YearDescription> yd = runPeriod.model().getOptionalUniqueModelObject<model::YearDescription>();
      if (yd) {
        if (!istringEqual(*optS, yd->dayofWeekforStartDay())) {
          LOG(Warn, "Multiple values detected for dayofWeekforStartDay, using " << yd->dayofWeekforStartDay());
        }
      } else {
        // create a year description
        yd = runPeriod.model().getUniqueModelObject<model::YearDescription>();
        yd->setDayofWeekforStartDay(*optS);
      }

    }  //if(optS)

    optS = workspaceObject.getString(RunPeriodFields::UseWeatherFileHolidaysandSpecialDays);
    if (optS) {
      std::string temp = *optS;
      boost::to_lower(temp);
      if (temp == "no") {
        runPeriod.setUseWeatherFileHolidays(false);
      } else {
        runPeriod.setUseWeatherFileHolidays(true);
      }
    }
    optS = workspaceObject.getString(RunPeriodFields::UseWeatherFileDaylightSavingPeriod);
    if (optS) {
      std::string temp = *optS;
      boost::to_lower(temp);
      if (temp == "no") {
        runPeriod.setUseWeatherFileDaylightSavings(false);
      } else {
        runPeriod.setUseWeatherFileDaylightSavings(true);
      }
    }
    optS = workspaceObject.getString(RunPeriodFields::ApplyWeekendHolidayRule);
    if (optS) {
      std::string temp = *optS;
      boost::to_lower(temp);
      if (temp == "no") {
        runPeriod.setApplyWeekendHolidayRule(false);
      } else {
        runPeriod.setApplyWeekendHolidayRule(true);
      }
    }
    optS = workspaceObject.getString(RunPeriodFields::UseWeatherFileRainIndicators);
    if (optS) {
      std::string temp = *optS;
      boost::to_lower(temp);
      if (temp == "no") {
        runPeriod.setUseWeatherFileRainInd(false);
      } else {
        runPeriod.setUseWeatherFileRainInd(true);
      }
    }
    optS = workspaceObject.getString(RunPeriodFields::UseWeatherFileSnowIndicators);
    if (optS) {
      std::string temp = *optS;
      boost::to_lower(temp);
      if (temp == "no") {
        runPeriod.setUseWeatherFileSnowInd(false);
      } else {
        runPeriod.setUseWeatherFileSnowInd(true);
      }
    }

    auto beginYear = workspaceObject.getInt(openstudio::RunPeriodFields::BeginYear);
    auto endYear = workspaceObject.getInt(openstudio::RunPeriodFields::EndYear);

    if (beginYear) {
      auto yd = runPeriod.model().getUniqueModelObject<model::YearDescription>();
      yd.setCalendarYear(beginYear.get());

      if (endYear) {
        runPeriod.setNumTimePeriodRepeats(endYear.get() - beginYear.get());
      }
    }

    result = runPeriod;
    return result;
  }

}  // namespace energyplus

}  // namespace openstudio
