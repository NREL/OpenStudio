/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../../model/ScheduleYear.hpp"
#include "../../model/ScheduleYear_Impl.hpp"
#include "../../model/ScheduleWeek.hpp"
#include "../../model/ScheduleWeek_Impl.hpp"

#include "../../model/ScheduleRuleset.hpp"
#include "../../model/ScheduleRuleset_Impl.hpp"
#include "../../model/ScheduleRule.hpp"
#include "../../model/ScheduleRule_Impl.hpp"
#include "../../model/ScheduleDay.hpp"
#include "../../model/ScheduleDay_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Schedule_Year_FieldEnums.hxx>
#include <utilities/idd/Schedule_Week_Compact_FieldEnums.hxx>
#include <utilities/idd/Schedule_Week_Daily_FieldEnums.hxx>
#include <utilities/idd/Schedule_Day_Interval_FieldEnums.hxx>

#include <utilities/idd/OS_Schedule_Year_FieldEnums.hxx>
#include <utilities/idd/OS_Schedule_Ruleset_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/time/Date.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateScheduleYear(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Schedule_Year) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Schedule:Year");
      return boost::none;
    }

    std::vector<ScheduleDay> holidaySchedules;
    std::vector<ScheduleDay> summerDesignDaySchedules;
    std::vector<ScheduleDay> winterDesignDaySchedules;
    std::vector<ScheduleDay> customDay1Schedules;
    std::vector<ScheduleDay> customDay2Schedules;
    // FIXME: decide, based on varying design day, holiday, and custom day schedules, whether to RT as ScheduleRuleset or ScheduleYear
    // translateToScheduleYear or translateToScheduleRuleset methods

    ScheduleRuleset scheduleRuleset(m_model);

    // Name
    OptionalString s = workspaceObject.name();
    if (s) {
      scheduleRuleset.setName(*s);
    }

    // Schedule Type Limits Name
    OptionalWorkspaceObject target = workspaceObject.getTarget(Schedule_YearFields::ScheduleTypeLimitsName);
    if (target) {
      OptionalModelObject scheduleTypeLimits = translateAndMapWorkspaceObject(*target);
      if (scheduleTypeLimits) {
        scheduleRuleset.setPointer(OS_Schedule_RulesetFields::ScheduleTypeLimitsName, scheduleTypeLimits->handle());
      }
    }

    std::string sundayScheduleDayName;
    std::string mondayScheduleDayName;
    std::string tuesdayScheduleDayName;
    std::string wednesdayScheduleDayName;
    std::string thursdayScheduleDayName;
    std::string fridayScheduleDayName;
    std::string saturdayScheduleDayName;

    OptionalWorkspaceObject holidayScheduleDay;
    OptionalWorkspaceObject summerDesignDayScheduleDay;
    OptionalWorkspaceObject winterDesignDayScheduleDay;
    OptionalWorkspaceObject customDay1ScheduleDay;
    OptionalWorkspaceObject customDay2ScheduleDay;

    for (const IdfExtensibleGroup& idfGroup : workspaceObject.extensibleGroups()) {
      WorkspaceExtensibleGroup workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();

      OptionalWorkspaceObject scheduleWeek =
        workspaceGroup.getTarget(Schedule_YearExtensibleFields::Schedule_WeekName);  // Schedule:Week:Daily, Schedule:Week:Compact
      OptionalInt startMonth = workspaceGroup.getInt(Schedule_YearExtensibleFields::StartMonth);
      OptionalInt startDay = workspaceGroup.getInt(Schedule_YearExtensibleFields::StartDay);
      OptionalInt endMonth = workspaceGroup.getInt(Schedule_YearExtensibleFields::EndMonth);
      OptionalInt endDay = workspaceGroup.getInt(Schedule_YearExtensibleFields::EndDay);

      std::vector<ScheduleRule> scheduleRules;
      bool found;

      if (scheduleWeek->iddObject().type() == IddObjectType::Schedule_Week_Daily) {
        boost::optional<std::string> scheduleWeekDailyName = scheduleWeek->getString(Schedule_Week_DailyFields::Name);
        OptionalWorkspaceObject sundayScheduleDay = scheduleWeek->getTarget(Schedule_Week_DailyFields::SundaySchedule_DayName);
        OptionalWorkspaceObject mondayScheduleDay = scheduleWeek->getTarget(Schedule_Week_DailyFields::MondaySchedule_DayName);
        OptionalWorkspaceObject tuesdayScheduleDay = scheduleWeek->getTarget(Schedule_Week_DailyFields::TuesdaySchedule_DayName);
        OptionalWorkspaceObject wednesdayScheduleDay = scheduleWeek->getTarget(Schedule_Week_DailyFields::WednesdaySchedule_DayName);
        OptionalWorkspaceObject thursdayScheduleDay = scheduleWeek->getTarget(Schedule_Week_DailyFields::ThursdaySchedule_DayName);
        OptionalWorkspaceObject fridayScheduleDay = scheduleWeek->getTarget(Schedule_Week_DailyFields::FridaySchedule_DayName);
        OptionalWorkspaceObject saturdayScheduleDay = scheduleWeek->getTarget(Schedule_Week_DailyFields::SaturdaySchedule_DayName);
        holidayScheduleDay = scheduleWeek->getTarget(Schedule_Week_DailyFields::HolidaySchedule_DayName);
        summerDesignDayScheduleDay = scheduleWeek->getTarget(Schedule_Week_DailyFields::SummerDesignDaySchedule_DayName);
        winterDesignDayScheduleDay = scheduleWeek->getTarget(Schedule_Week_DailyFields::WinterDesignDaySchedule_DayName);
        customDay1ScheduleDay = scheduleWeek->getTarget(Schedule_Week_DailyFields::CustomDay1Schedule_DayName);
        customDay2ScheduleDay = scheduleWeek->getTarget(Schedule_Week_DailyFields::CustomDay2Schedule_DayName);

        if (sundayScheduleDay) {
          sundayScheduleDayName = sundayScheduleDay->getString(Schedule_Day_IntervalFields::Name).get();

          found = false;
          for (ScheduleRule scheduleRule : scheduleRules) {
            if (sundayScheduleDayName == scheduleRule.daySchedule().nameString()) {
              found = true;
              scheduleRule.setApplySunday(true);
            }
          }

          if (!found) {
            OptionalModelObject modelObject = translateAndMapWorkspaceObject(*sundayScheduleDay);
            if (modelObject) {
              ScheduleDay scheduleDay = modelObject->cast<ScheduleDay>();
              ScheduleRule scheduleRule(scheduleRuleset, scheduleDay);
              scheduleRule.setName(*scheduleWeekDailyName);
              scheduleRule.setStartDate(Date(*startMonth, *startDay));
              scheduleRule.setEndDate(Date(*endMonth, *endDay));
              scheduleRule.setApplySunday(true);
              scheduleRules.push_back(scheduleRule);
            }
          }
        }
        if (mondayScheduleDay) {
          mondayScheduleDayName = mondayScheduleDay->getString(Schedule_Day_IntervalFields::Name).get();

          found = false;
          for (ScheduleRule scheduleRule : scheduleRules) {
            if (mondayScheduleDayName == scheduleRule.daySchedule().nameString()) {
              found = true;
              scheduleRule.setApplyMonday(true);
            }
          }

          if (!found) {
            OptionalModelObject modelObject = translateAndMapWorkspaceObject(*mondayScheduleDay);
            if (modelObject) {
              ScheduleDay scheduleDay = modelObject->cast<ScheduleDay>();
              ScheduleRule scheduleRule(scheduleRuleset, scheduleDay);
              scheduleRule.setName(*scheduleWeekDailyName);
              scheduleRule.setStartDate(Date(*startMonth, *startDay));
              scheduleRule.setEndDate(Date(*endMonth, *endDay));
              scheduleRule.setApplyMonday(true);
              scheduleRules.push_back(scheduleRule);
            }
          }
        }
        if (tuesdayScheduleDay) {
          tuesdayScheduleDayName = tuesdayScheduleDay->getString(Schedule_Day_IntervalFields::Name).get();

          found = false;
          for (ScheduleRule scheduleRule : scheduleRules) {
            if (tuesdayScheduleDayName == scheduleRule.daySchedule().nameString()) {
              found = true;
              scheduleRule.setApplyTuesday(true);
            }
          }

          if (!found) {
            OptionalModelObject modelObject = translateAndMapWorkspaceObject(*tuesdayScheduleDay);
            if (modelObject) {
              ScheduleDay scheduleDay = modelObject->cast<ScheduleDay>();
              ScheduleRule scheduleRule(scheduleRuleset, scheduleDay);
              scheduleRule.setName(*scheduleWeekDailyName);
              scheduleRule.setStartDate(Date(*startMonth, *startDay));
              scheduleRule.setEndDate(Date(*endMonth, *endDay));
              scheduleRule.setApplyTuesday(true);
              scheduleRules.push_back(scheduleRule);
            }
          }
        }
        if (wednesdayScheduleDay) {
          wednesdayScheduleDayName = wednesdayScheduleDay->getString(Schedule_Day_IntervalFields::Name).get();

          found = false;
          for (ScheduleRule scheduleRule : scheduleRules) {
            if (wednesdayScheduleDayName == scheduleRule.daySchedule().nameString()) {
              found = true;
              scheduleRule.setApplyWednesday(true);
            }
          }

          if (!found) {
            OptionalModelObject modelObject = translateAndMapWorkspaceObject(*wednesdayScheduleDay);
            if (modelObject) {
              ScheduleDay scheduleDay = modelObject->cast<ScheduleDay>();
              ScheduleRule scheduleRule(scheduleRuleset, scheduleDay);
              scheduleRule.setName(*scheduleWeekDailyName);
              scheduleRule.setStartDate(Date(*startMonth, *startDay));
              scheduleRule.setEndDate(Date(*endMonth, *endDay));
              scheduleRule.setApplyWednesday(true);
              scheduleRules.push_back(scheduleRule);
            }
          }
        }
        if (thursdayScheduleDay) {
          thursdayScheduleDayName = thursdayScheduleDay->getString(Schedule_Day_IntervalFields::Name).get();

          found = false;
          for (ScheduleRule scheduleRule : scheduleRules) {
            if (thursdayScheduleDayName == scheduleRule.daySchedule().nameString()) {
              found = true;
              scheduleRule.setApplyThursday(true);
            }
          }

          if (!found) {
            OptionalModelObject modelObject = translateAndMapWorkspaceObject(*thursdayScheduleDay);
            if (modelObject) {
              ScheduleDay scheduleDay = modelObject->cast<ScheduleDay>();
              ScheduleRule scheduleRule(scheduleRuleset, scheduleDay);
              scheduleRule.setName(*scheduleWeekDailyName);
              scheduleRule.setStartDate(Date(*startMonth, *startDay));
              scheduleRule.setEndDate(Date(*endMonth, *endDay));
              scheduleRule.setApplyThursday(true);
              scheduleRules.push_back(scheduleRule);
            }
          }
        }
        if (fridayScheduleDay) {
          fridayScheduleDayName = fridayScheduleDay->getString(Schedule_Day_IntervalFields::Name).get();

          found = false;
          for (ScheduleRule scheduleRule : scheduleRules) {
            if (fridayScheduleDayName == scheduleRule.daySchedule().nameString()) {
              found = true;
              scheduleRule.setApplyFriday(true);
            }
          }

          if (!found) {
            OptionalModelObject modelObject = translateAndMapWorkspaceObject(*fridayScheduleDay);
            if (modelObject) {
              ScheduleDay scheduleDay = modelObject->cast<ScheduleDay>();
              ScheduleRule scheduleRule(scheduleRuleset, scheduleDay);
              scheduleRule.setName(*scheduleWeekDailyName);
              scheduleRule.setStartDate(Date(*startMonth, *startDay));
              scheduleRule.setEndDate(Date(*endMonth, *endDay));
              scheduleRule.setApplyFriday(true);
              scheduleRules.push_back(scheduleRule);
            }
          }
        }
        if (saturdayScheduleDay) {
          saturdayScheduleDayName = saturdayScheduleDay->getString(Schedule_Day_IntervalFields::Name).get();

          found = false;
          for (ScheduleRule scheduleRule : scheduleRules) {
            if (saturdayScheduleDayName == scheduleRule.daySchedule().nameString()) {
              found = true;
              scheduleRule.setApplySaturday(true);
            }
          }

          if (!found) {
            OptionalModelObject modelObject = translateAndMapWorkspaceObject(*saturdayScheduleDay);
            if (modelObject) {
              ScheduleDay scheduleDay = modelObject->cast<ScheduleDay>();
              ScheduleRule scheduleRule(scheduleRuleset, scheduleDay);
              scheduleRule.setName(*scheduleWeekDailyName);
              scheduleRule.setStartDate(Date(*startMonth, *startDay));
              scheduleRule.setEndDate(Date(*endMonth, *endDay));
              scheduleRule.setApplySaturday(true);
              scheduleRules.push_back(scheduleRule);
            }
          }
        }
      } else if (scheduleWeek->iddObject().type() == IddObjectType::Schedule_Week_Compact) {
        boost::optional<std::string> scheduleWeekCompactName = scheduleWeek->getString(Schedule_Week_CompactFields::Name);
        // TODO
      }
    }

    if (holidayScheduleDay) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*holidayScheduleDay);
      if (modelObject) {
        ScheduleDay scheduleDay = modelObject->cast<ScheduleDay>();
        scheduleRuleset.setHolidaySchedule(scheduleDay);
      }
    }
    if (summerDesignDayScheduleDay) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*summerDesignDayScheduleDay);
      if (modelObject) {
        ScheduleDay scheduleDay = modelObject->cast<ScheduleDay>();
        scheduleRuleset.setSummerDesignDaySchedule(scheduleDay);
      }
    }
    if (winterDesignDayScheduleDay) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*winterDesignDayScheduleDay);
      if (modelObject) {
        ScheduleDay scheduleDay = modelObject->cast<ScheduleDay>();
        scheduleRuleset.setWinterDesignDaySchedule(scheduleDay);
      }
    }
    if (customDay1ScheduleDay) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*customDay1ScheduleDay);
      if (modelObject) {
        ScheduleDay scheduleDay = modelObject->cast<ScheduleDay>();
        scheduleRuleset.setCustomDay1Schedule(scheduleDay);
      }
    }
    if (customDay2ScheduleDay) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*customDay2ScheduleDay);
      if (modelObject) {
        ScheduleDay scheduleDay = modelObject->cast<ScheduleDay>();
        scheduleRuleset.setCustomDay2Schedule(scheduleDay);
      }
    }

    return scheduleRuleset;
  }

  /*   OptionalModelObject ReverseTranslator::translateScheduleYear(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Schedule_Year) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Schedule:Year");
      return boost::none;
    }

    //create the schedule
    ScheduleYear scheduleYear(m_model);
    //translate name
    OptionalString s = workspaceObject.name();
    if (s) {
      scheduleYear.setName(*s);
    }
    //translate schedule type limits name
    OptionalWorkspaceObject target = workspaceObject.getTarget(Schedule_YearFields::ScheduleTypeLimitsName);
    if (target) {
      OptionalModelObject scheduleTypeLimits = translateAndMapWorkspaceObject(*target);
      if (scheduleTypeLimits) {
        scheduleYear.setPointer(OS_Schedule_YearFields::ScheduleTypeLimitsName, scheduleTypeLimits->handle());
      }
    }
    //get extensible groups
    std::vector<IdfExtensibleGroup> extensibleGroups = workspaceObject.extensibleGroups();
    //loop over extensible groups
    unsigned n = extensibleGroups.size();
    for (unsigned i = 0; i < n; ++i) {
      //read in extensible groups
      boost::optional<std::string> scheduleName = extensibleGroups[i].getString(Schedule_YearExtensibleFields::Schedule_WeekName);
      boost::optional<unsigned> startMonth = extensibleGroups[i].getUnsigned(Schedule_YearExtensibleFields::StartMonth);
      boost::optional<unsigned> startDay = extensibleGroups[i].getUnsigned(Schedule_YearExtensibleFields::StartDay);
      boost::optional<unsigned> endMonth = extensibleGroups[i].getUnsigned(Schedule_YearExtensibleFields::EndMonth);
      boost::optional<unsigned> endDay = extensibleGroups[i].getUnsigned(Schedule_YearExtensibleFields::EndDay);
      // check for errors, assume sorted groups
      if (i == 0) {
        if ((*startMonth != 1) && (*startDay != 1)) {
          LOG(Error, *scheduleName + " does not start on January 1. Object not translated.");
          return boost::none;
        }
      } else if (i == n - 1) {
        if ((*endMonth != 12) && (*endDay != 31)) {
          LOG(Error, *scheduleName + " does not end on December 31. Object not translated.");
          return boost::none;
        }
      }
      // reverse translate schedule week
      target = extensibleGroups[i].cast<WorkspaceExtensibleGroup>().getTarget(Schedule_YearExtensibleFields::Schedule_WeekName);
      if (target) {
        OptionalModelObject scheduleWeekName = translateAndMapWorkspaceObject(*target);
        if (scheduleWeekName) {
          scheduleYear.addScheduleWeek(Date(*endMonth, *endDay), scheduleWeekName->cast<ScheduleWeek>());
        }
      }
    }

    return scheduleYear;
  } */

}  // namespace energyplus

}  // namespace openstudio
