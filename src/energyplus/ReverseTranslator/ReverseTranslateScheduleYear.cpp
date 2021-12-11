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

#include <utilities/idd/OS_Schedule_Year_FieldEnums.hxx>
#include <utilities/idd/OS_Schedule_Ruleset_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/time/Date.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  ScheduleRule createNewRule(ScheduleRuleset& scheduleRuleset, OptionalModelObject& modelObject, std::string name, const Date& startDate,
                             const Date& endDate) {
    ScheduleDay daySchedule = modelObject->cast<ScheduleDay>();
    ScheduleRule scheduleRule(scheduleRuleset, daySchedule);  // this clones daySchedule and sets the clone with a different name
    scheduleRule.setName(name);
    scheduleRule.setStartDate(startDate);
    scheduleRule.setEndDate(endDate);

    return scheduleRule;
  }

  OptionalModelObject ReverseTranslator::translateScheduleYear(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Schedule_Year) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Schedule:Year");
      return boost::none;
    }

    std::vector<std::string> holidayScheduleDayNames;
    std::vector<std::string> summerDesignDayScheduleDayNames;
    std::vector<std::string> winterDesignDayScheduleDayNames;
    std::vector<std::string> customDay1ScheduleDayNames;
    std::vector<std::string> customDay2ScheduleDayNames;

    std::vector<IdfExtensibleGroup> extensibleGroups = workspaceObject.extensibleGroups();
    for (const IdfExtensibleGroup& idfGroup : extensibleGroups) {
      WorkspaceExtensibleGroup workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();

      OptionalWorkspaceObject scheduleWeek =
        workspaceGroup.getTarget(Schedule_YearExtensibleFields::Schedule_WeekName);  // Schedule:Week:Daily, Schedule:Week:Compact

      if (scheduleWeek->iddObject().type() == IddObjectType::Schedule_Week_Daily) {
        if (OptionalString holidayScheduleDayName = scheduleWeek->getString(Schedule_Week_DailyFields::HolidaySchedule_DayName)) {
          if (std::find(holidayScheduleDayNames.begin(), holidayScheduleDayNames.end(), *holidayScheduleDayName) == holidayScheduleDayNames.end()) {
            holidayScheduleDayNames.push_back(*holidayScheduleDayName);
          }
        }
        if (OptionalString summerDesignDayScheduleDayName = scheduleWeek->getString(Schedule_Week_DailyFields::SummerDesignDaySchedule_DayName)) {
          if (std::find(summerDesignDayScheduleDayNames.begin(), summerDesignDayScheduleDayNames.end(), *summerDesignDayScheduleDayName)
              == summerDesignDayScheduleDayNames.end()) {
            summerDesignDayScheduleDayNames.push_back(*summerDesignDayScheduleDayName);
          }
        }
        if (OptionalString winterDesignDayScheduleDayName = scheduleWeek->getString(Schedule_Week_DailyFields::WinterDesignDaySchedule_DayName)) {
          if (std::find(winterDesignDayScheduleDayNames.begin(), winterDesignDayScheduleDayNames.end(), *winterDesignDayScheduleDayName)
              == winterDesignDayScheduleDayNames.end()) {
            winterDesignDayScheduleDayNames.push_back(*winterDesignDayScheduleDayName);
          }
        }
        if (OptionalString customDay1ScheduleDayName = scheduleWeek->getString(Schedule_Week_DailyFields::CustomDay1Schedule_DayName)) {
          if (std::find(customDay1ScheduleDayNames.begin(), customDay1ScheduleDayNames.end(), *customDay1ScheduleDayName)
              == customDay1ScheduleDayNames.end()) {
            customDay1ScheduleDayNames.push_back(*customDay1ScheduleDayName);
          }
        }
        if (OptionalString customDay2ScheduleDayName = scheduleWeek->getString(Schedule_Week_DailyFields::CustomDay2Schedule_DayName)) {
          if (std::find(customDay2ScheduleDayNames.begin(), customDay2ScheduleDayNames.end(), *customDay2ScheduleDayName)
              == customDay2ScheduleDayNames.end()) {
            customDay2ScheduleDayNames.push_back(*customDay2ScheduleDayName);
          }
        }
      } else if (scheduleWeek->iddObject().type() == IddObjectType::Schedule_Week_Compact) {

        for (const IdfExtensibleGroup& dayTypeIdfGroup : scheduleWeek->extensibleGroups()) {
          WorkspaceExtensibleGroup dayTypeWorkspaceGroup = dayTypeIdfGroup.cast<WorkspaceExtensibleGroup>();

          boost::optional<std::string> dayType = dayTypeWorkspaceGroup.getString(Schedule_Week_CompactExtensibleFields::DayTypeList);
          boost::optional<std::string> scheduleDayName = dayTypeWorkspaceGroup.getString(Schedule_Week_CompactExtensibleFields::Schedule_DayName);

          if (dayType.get() == "Holiday") {
            if (std::find(holidayScheduleDayNames.begin(), holidayScheduleDayNames.end(), *scheduleDayName) == holidayScheduleDayNames.end()) {
              holidayScheduleDayNames.push_back(*scheduleDayName);
            }
          }
          if (dayType.get() == "SummerDesignDay") {
            if (std::find(summerDesignDayScheduleDayNames.begin(), summerDesignDayScheduleDayNames.end(), *scheduleDayName)
                == summerDesignDayScheduleDayNames.end()) {
              summerDesignDayScheduleDayNames.push_back(*scheduleDayName);
            }
          }
          if (dayType.get() == "WinterDesignDay") {
            if (std::find(winterDesignDayScheduleDayNames.begin(), winterDesignDayScheduleDayNames.end(), *scheduleDayName)
                == winterDesignDayScheduleDayNames.end()) {
              winterDesignDayScheduleDayNames.push_back(*scheduleDayName);
            }
          }
          if (dayType.get() == "CustomDay1") {
            if (std::find(customDay1ScheduleDayNames.begin(), customDay1ScheduleDayNames.end(), *scheduleDayName)
                == customDay1ScheduleDayNames.end()) {
              customDay1ScheduleDayNames.push_back(*scheduleDayName);
            }
          }
          if (dayType.get() == "CustomDay2") {
            if (std::find(customDay2ScheduleDayNames.begin(), customDay2ScheduleDayNames.end(), *scheduleDayName)
                == customDay2ScheduleDayNames.end()) {
              customDay2ScheduleDayNames.push_back(*scheduleDayName);
            }
          }
        }
      }
    }

    bool translateToScheduleYear = true;
    if (holidayScheduleDayNames.size() <= 1 && summerDesignDayScheduleDayNames.size() <= 1 && winterDesignDayScheduleDayNames.size() <= 1
        && customDay1ScheduleDayNames.size() <= 1 && customDay2ScheduleDayNames.size() <= 1) {
      translateToScheduleYear = false;
    }

    if (translateToScheduleYear) {

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

    } else {

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

      OptionalWorkspaceObject holidayScheduleDay;
      OptionalWorkspaceObject summerDesignDayScheduleDay;
      OptionalWorkspaceObject winterDesignDayScheduleDay;
      OptionalWorkspaceObject customDay1ScheduleDay;
      OptionalWorkspaceObject customDay2ScheduleDay;

      for (const IdfExtensibleGroup& idfGroup : extensibleGroups) {
        WorkspaceExtensibleGroup workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();

        OptionalWorkspaceObject scheduleWeek =
          workspaceGroup.getTarget(Schedule_YearExtensibleFields::Schedule_WeekName);  // Schedule:Week:Daily, Schedule:Week:Compact
        OptionalInt startMonth = workspaceGroup.getInt(Schedule_YearExtensibleFields::StartMonth);
        OptionalInt startDay = workspaceGroup.getInt(Schedule_YearExtensibleFields::StartDay);
        OptionalInt endMonth = workspaceGroup.getInt(Schedule_YearExtensibleFields::EndMonth);
        OptionalInt endDay = workspaceGroup.getInt(Schedule_YearExtensibleFields::EndDay);

        std::vector<std::string> dayScheduleNames;
        std::vector<ScheduleRule> scheduleRules;

        if (scheduleWeek->iddObject().type() == IddObjectType::Schedule_Week_Daily) {
          std::string sundayScheduleDayName;
          std::string mondayScheduleDayName;
          std::string tuesdayScheduleDayName;
          std::string wednesdayScheduleDayName;
          std::string thursdayScheduleDayName;
          std::string fridayScheduleDayName;
          std::string saturdayScheduleDayName;

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
            sundayScheduleDayName = sundayScheduleDay->getString(0).get();

            boost::optional<ScheduleRule> scheduleRule;
            auto itr = std::find(dayScheduleNames.begin(), dayScheduleNames.end(), sundayScheduleDayName);
            if (itr != dayScheduleNames.end()) {
              scheduleRule = scheduleRules[itr - dayScheduleNames.begin()];
            }

            if (!scheduleRule) {
              if (OptionalModelObject modelObject = translateAndMapWorkspaceObject(*sundayScheduleDay)) {
                scheduleRule =
                  createNewRule(scheduleRuleset, modelObject, *scheduleWeekDailyName, Date(*startMonth, *startDay), Date(*endMonth, *endDay));
                dayScheduleNames.push_back(sundayScheduleDayName);
                scheduleRules.push_back(*scheduleRule);
              }
            }

            scheduleRule->setApplySunday(true);
          }
          if (mondayScheduleDay) {
            mondayScheduleDayName = mondayScheduleDay->getString(0).get();

            boost::optional<ScheduleRule> scheduleRule;
            auto itr = std::find(dayScheduleNames.begin(), dayScheduleNames.end(), mondayScheduleDayName);
            if (itr != dayScheduleNames.end()) {
              scheduleRule = scheduleRules[itr - dayScheduleNames.begin()];
            }

            if (!scheduleRule) {
              if (OptionalModelObject modelObject = translateAndMapWorkspaceObject(*mondayScheduleDay)) {
                scheduleRule =
                  createNewRule(scheduleRuleset, modelObject, *scheduleWeekDailyName, Date(*startMonth, *startDay), Date(*endMonth, *endDay));
                dayScheduleNames.push_back(mondayScheduleDayName);
                scheduleRules.push_back(*scheduleRule);
              }
            }

            scheduleRule->setApplyMonday(true);
          }
          if (tuesdayScheduleDay) {
            tuesdayScheduleDayName = tuesdayScheduleDay->getString(0).get();

            boost::optional<ScheduleRule> scheduleRule;
            auto itr = std::find(dayScheduleNames.begin(), dayScheduleNames.end(), tuesdayScheduleDayName);
            if (itr != dayScheduleNames.end()) {
              scheduleRule = scheduleRules[itr - dayScheduleNames.begin()];
            }

            if (!scheduleRule) {
              if (OptionalModelObject modelObject = translateAndMapWorkspaceObject(*tuesdayScheduleDay)) {
                scheduleRule =
                  createNewRule(scheduleRuleset, modelObject, *scheduleWeekDailyName, Date(*startMonth, *startDay), Date(*endMonth, *endDay));
                dayScheduleNames.push_back(tuesdayScheduleDayName);
                scheduleRules.push_back(*scheduleRule);
              }
            }

            scheduleRule->setApplyTuesday(true);
          }
          if (wednesdayScheduleDay) {
            wednesdayScheduleDayName = wednesdayScheduleDay->getString(0).get();

            boost::optional<ScheduleRule> scheduleRule;
            auto itr = std::find(dayScheduleNames.begin(), dayScheduleNames.end(), wednesdayScheduleDayName);
            if (itr != dayScheduleNames.end()) {
              scheduleRule = scheduleRules[itr - dayScheduleNames.begin()];
            }

            if (!scheduleRule) {
              if (OptionalModelObject modelObject = translateAndMapWorkspaceObject(*wednesdayScheduleDay)) {
                scheduleRule =
                  createNewRule(scheduleRuleset, modelObject, *scheduleWeekDailyName, Date(*startMonth, *startDay), Date(*endMonth, *endDay));
                dayScheduleNames.push_back(wednesdayScheduleDayName);
                scheduleRules.push_back(*scheduleRule);
              }
            }

            scheduleRule->setApplyWednesday(true);
          }
          if (thursdayScheduleDay) {
            thursdayScheduleDayName = thursdayScheduleDay->getString(0).get();

            boost::optional<ScheduleRule> scheduleRule;
            auto itr = std::find(dayScheduleNames.begin(), dayScheduleNames.end(), thursdayScheduleDayName);
            if (itr != dayScheduleNames.end()) {
              scheduleRule = scheduleRules[itr - dayScheduleNames.begin()];
            }

            if (!scheduleRule) {
              if (OptionalModelObject modelObject = translateAndMapWorkspaceObject(*thursdayScheduleDay)) {
                scheduleRule =
                  createNewRule(scheduleRuleset, modelObject, *scheduleWeekDailyName, Date(*startMonth, *startDay), Date(*endMonth, *endDay));
                dayScheduleNames.push_back(thursdayScheduleDayName);
                scheduleRules.push_back(*scheduleRule);
              }
            }

            scheduleRule->setApplyThursday(true);
          }
          if (fridayScheduleDay) {
            fridayScheduleDayName = fridayScheduleDay->getString(0).get();

            boost::optional<ScheduleRule> scheduleRule;
            auto itr = std::find(dayScheduleNames.begin(), dayScheduleNames.end(), fridayScheduleDayName);
            if (itr != dayScheduleNames.end()) {
              scheduleRule = scheduleRules[itr - dayScheduleNames.begin()];
            }

            if (!scheduleRule) {
              if (OptionalModelObject modelObject = translateAndMapWorkspaceObject(*fridayScheduleDay)) {
                scheduleRule =
                  createNewRule(scheduleRuleset, modelObject, *scheduleWeekDailyName, Date(*startMonth, *startDay), Date(*endMonth, *endDay));
                dayScheduleNames.push_back(fridayScheduleDayName);
                scheduleRules.push_back(*scheduleRule);
              }
            }

            scheduleRule->setApplyFriday(true);
          }
          if (saturdayScheduleDay) {
            saturdayScheduleDayName = saturdayScheduleDay->getString(0).get();

            boost::optional<ScheduleRule> scheduleRule;
            auto itr = std::find(dayScheduleNames.begin(), dayScheduleNames.end(), saturdayScheduleDayName);
            if (itr != dayScheduleNames.end()) {
              scheduleRule = scheduleRules[itr - dayScheduleNames.begin()];
            }

            if (!scheduleRule) {
              if (OptionalModelObject modelObject = translateAndMapWorkspaceObject(*saturdayScheduleDay)) {
                scheduleRule =
                  createNewRule(scheduleRuleset, modelObject, *scheduleWeekDailyName, Date(*startMonth, *startDay), Date(*endMonth, *endDay));
                dayScheduleNames.push_back(saturdayScheduleDayName);
                scheduleRules.push_back(*scheduleRule);
              }
            }

            scheduleRule->setApplySaturday(true);
          }
        } else if (scheduleWeek->iddObject().type() == IddObjectType::Schedule_Week_Compact) {
          std::string scheduleDayName;

          boost::optional<std::string> scheduleWeekCompactName = scheduleWeek->getString(Schedule_Week_CompactFields::Name);

          for (const IdfExtensibleGroup& dayTypeIdfGroup : scheduleWeek->extensibleGroups()) {
            WorkspaceExtensibleGroup dayTypeWorkspaceGroup = dayTypeIdfGroup.cast<WorkspaceExtensibleGroup>();

            boost::optional<std::string> dayType = dayTypeWorkspaceGroup.getString(Schedule_Week_CompactExtensibleFields::DayTypeList);
            OptionalWorkspaceObject scheduleDay = dayTypeWorkspaceGroup.getTarget(Schedule_Week_CompactExtensibleFields::Schedule_DayName);

            if (scheduleDay && dayType.get() != "Holiday" && dayType.get() != "SummerDesignDay" && dayType.get() != "WinterDesignDay"
                && dayType.get() != "CustomDay1" && dayType.get() != "CustomDay2") {
              scheduleDayName = scheduleDay->getString(0).get();

              boost::optional<ScheduleRule> scheduleRule;
              auto itr = std::find(dayScheduleNames.begin(), dayScheduleNames.end(), scheduleDayName);
              if (itr != dayScheduleNames.end()) {
                scheduleRule = scheduleRules[itr - dayScheduleNames.begin()];
              }

              if (!scheduleRule) {
                if (OptionalModelObject modelObject = translateAndMapWorkspaceObject(*scheduleDay)) {
                  scheduleRule =
                    createNewRule(scheduleRuleset, modelObject, *scheduleWeekCompactName, Date(*startMonth, *startDay), Date(*endMonth, *endDay));
                  dayScheduleNames.push_back(scheduleDayName);
                  scheduleRules.push_back(*scheduleRule);
                }
              }

              if (dayType.get() == "AllDays" || dayType.get() == "AllOtherDays") {
                scheduleRule->setApplySunday(true);
                scheduleRule->setApplyMonday(true);
                scheduleRule->setApplyTuesday(true);
                scheduleRule->setApplyWednesday(true);
                scheduleRule->setApplyThursday(true);
                scheduleRule->setApplyFriday(true);
                scheduleRule->setApplySaturday(true);
              } else if (dayType.get() == "Weekdays") {
                scheduleRule->setApplyMonday(true);
                scheduleRule->setApplyTuesday(true);
                scheduleRule->setApplyWednesday(true);
                scheduleRule->setApplyThursday(true);
                scheduleRule->setApplyFriday(true);
              } else if (dayType.get() == "Weekends") {
                scheduleRule->setApplySunday(true);
                scheduleRule->setApplySaturday(true);
              } else if (dayType.get() == "Sunday") {
                scheduleRule->setApplySunday(true);
              } else if (dayType.get() == "Monday") {
                scheduleRule->setApplyMonday(true);
              } else if (dayType.get() == "Tuesday") {
                scheduleRule->setApplyTuesday(true);
              } else if (dayType.get() == "Wednesday") {
                scheduleRule->setApplyWednesday(true);
              } else if (dayType.get() == "Thursday") {
                scheduleRule->setApplyThursday(true);
              } else if (dayType.get() == "Friday") {
                scheduleRule->setApplyFriday(true);
              } else if (dayType.get() == "Saturday") {
                scheduleRule->setApplySaturday(true);
              }
            }

            if (dayType.get() == "Holiday") {
              holidayScheduleDay = scheduleDay;
            } else if (dayType.get() == "SummerDesignDay") {
              summerDesignDayScheduleDay = scheduleDay;
            } else if (dayType.get() == "WinterDesignDay") {
              winterDesignDayScheduleDay = scheduleDay;
            } else if (dayType.get() == "CustomDay1") {
              customDay1ScheduleDay = scheduleDay;
            } else if (dayType.get() == "CustomDay2") {
              customDay2ScheduleDay = scheduleDay;
            }
          }
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
  }

}  // namespace energyplus

}  // namespace openstudio
