/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/ModelObject.hpp"

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
#include "../../utilities/core/ASCIIStrings.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  struct SpecialDayScheduleNames
  {
    std::set<std::string> holidayScheduleDayNames;
    std::set<std::string> summerDesignDayScheduleDayNames;
    std::set<std::string> winterDesignDayScheduleDayNames;
    std::set<std::string> customDay1ScheduleDayNames;
    std::set<std::string> customDay2ScheduleDayNames;

    [[nodiscard]] bool hasMoreThanOneSpecialDaysInEachType() const {
      return (holidayScheduleDayNames.size() > 1 || summerDesignDayScheduleDayNames.size() > 1 || winterDesignDayScheduleDayNames.size() > 1
              || customDay1ScheduleDayNames.size() > 1 || customDay2ScheduleDayNames.size() > 1);
    }
  };

  SpecialDayScheduleNames getSpecialDayScheduleNames(const WorkspaceObject& workspaceObject) {

    SpecialDayScheduleNames specialDayScheduleNames;
    std::vector<IdfExtensibleGroup> extensibleGroups = workspaceObject.extensibleGroups();
    for (const IdfExtensibleGroup& idfGroup : extensibleGroups) {
      auto workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();

      // Schedule:Week:Daily, Schedule:Week:Compact
      OptionalWorkspaceObject scheduleWeek = workspaceGroup.getTarget(Schedule_YearExtensibleFields::Schedule_WeekName);

      if (scheduleWeek->iddObject().type() == IddObjectType::Schedule_Week_Daily) {
        if (OptionalString holidayScheduleDayName = scheduleWeek->getString(Schedule_Week_DailyFields::HolidaySchedule_DayName)) {
          specialDayScheduleNames.holidayScheduleDayNames.emplace(*holidayScheduleDayName);
        }
        if (OptionalString summerDesignDayScheduleDayName = scheduleWeek->getString(Schedule_Week_DailyFields::SummerDesignDaySchedule_DayName)) {
          specialDayScheduleNames.summerDesignDayScheduleDayNames.emplace(*summerDesignDayScheduleDayName);
        }
        if (OptionalString winterDesignDayScheduleDayName = scheduleWeek->getString(Schedule_Week_DailyFields::WinterDesignDaySchedule_DayName)) {
          specialDayScheduleNames.winterDesignDayScheduleDayNames.emplace(*winterDesignDayScheduleDayName);
        }
        if (OptionalString customDay1ScheduleDayName = scheduleWeek->getString(Schedule_Week_DailyFields::CustomDay1Schedule_DayName)) {
          specialDayScheduleNames.customDay1ScheduleDayNames.emplace(*customDay1ScheduleDayName);
        }
        if (OptionalString customDay2ScheduleDayName = scheduleWeek->getString(Schedule_Week_DailyFields::CustomDay2Schedule_DayName)) {
          specialDayScheduleNames.customDay2ScheduleDayNames.emplace(*customDay2ScheduleDayName);
        }
      } else if (scheduleWeek->iddObject().type() == IddObjectType::Schedule_Week_Compact) {

        for (const IdfExtensibleGroup& dayTypeIdfGroup : scheduleWeek->extensibleGroups()) {
          auto dayTypeWorkspaceGroup = dayTypeIdfGroup.cast<WorkspaceExtensibleGroup>();

          boost::optional<std::string> dayType_ = dayTypeWorkspaceGroup.getString(Schedule_Week_CompactExtensibleFields::DayTypeList);
          OS_ASSERT(dayType_);
          std::string dayTypeLower = openstudio::ascii_to_lower_copy(dayType_.get());
          boost::optional<std::string> scheduleDayName = dayTypeWorkspaceGroup.getString(Schedule_Week_CompactExtensibleFields::Schedule_DayName);

          if (dayTypeLower == "holiday") {
            specialDayScheduleNames.holidayScheduleDayNames.emplace(*scheduleDayName);
          }
          if (dayTypeLower == "summerdesignDay") {
            specialDayScheduleNames.summerDesignDayScheduleDayNames.emplace(*scheduleDayName);
          }
          if (dayTypeLower == "winterdesignDay") {
            specialDayScheduleNames.winterDesignDayScheduleDayNames.emplace(*scheduleDayName);
          }
          if (dayTypeLower == "customday1") {
            specialDayScheduleNames.customDay1ScheduleDayNames.emplace(*scheduleDayName);
          }
          if (dayTypeLower == "customday2") {
            specialDayScheduleNames.customDay2ScheduleDayNames.emplace(*scheduleDayName);
          }
        }
      }
    }
    return specialDayScheduleNames;
  }

  OptionalModelObject ReverseTranslator::translateScheduleYear(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Schedule_Year) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Schedule:Year");
      return boost::none;
    }

    std::vector<IdfExtensibleGroup> extensibleGroups = workspaceObject.extensibleGroups();
    SpecialDayScheduleNames specialDayScheduleNames = getSpecialDayScheduleNames(workspaceObject);

    if (specialDayScheduleNames.hasMoreThanOneSpecialDaysInEachType()) {
      ScheduleYear scheduleYear(m_model);

      //translate name
      scheduleYear.setName(workspaceObject.nameString());

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
      // Special Days: for each type, not more than one exists (ScheduleRuleset can only accept a single one for each type)

      ScheduleRuleset scheduleRuleset(m_model);

      // Name
      scheduleRuleset.setName(workspaceObject.nameString());

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
        auto workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();

        OptionalWorkspaceObject scheduleWeek =
          workspaceGroup.getTarget(Schedule_YearExtensibleFields::Schedule_WeekName);  // Schedule:Week:Daily, Schedule:Week:Compact
        OptionalInt startMonth = workspaceGroup.getInt(Schedule_YearExtensibleFields::StartMonth);
        OptionalInt startDay = workspaceGroup.getInt(Schedule_YearExtensibleFields::StartDay);
        OptionalInt endMonth = workspaceGroup.getInt(Schedule_YearExtensibleFields::EndMonth);
        OptionalInt endDay = workspaceGroup.getInt(Schedule_YearExtensibleFields::EndDay);

        Date startDate(*startMonth, *startDay);
        Date endDate(*endMonth, *endDay);

        std::vector<std::string> dayScheduleNames;
        std::vector<ScheduleRule> scheduleRules;

        // Lambda helper, not making it into a function since I need the ReverseTranslator context to call translateAndMapWorkspaceObject and I don't
        // want to define it into the ReverseTranslator.hpp
        auto getOrCreateRule = [this, &scheduleRuleset, &startDate, &endDate, &dayScheduleNames,
                                &scheduleRules](const boost::optional<WorkspaceObject>& scheduleDay, const std::string& scheduleWeekName) {
          boost::optional<ScheduleRule> scheduleRule;

          if (scheduleDay) {
            std::string scheduleDayName = scheduleDay->nameString();

            auto it = std::find(dayScheduleNames.begin(), dayScheduleNames.end(), scheduleDayName);
            if (it != dayScheduleNames.end()) {
              scheduleRule = scheduleRules[it - dayScheduleNames.begin()];
            } else {
              if (OptionalModelObject mo_daySchedule = translateAndMapWorkspaceObject(*scheduleDay)) {
                auto daySchedule = mo_daySchedule->cast<ScheduleDay>();
                // TODO: this clones daySchedule and sets the clone with a different name, which is annoying

                scheduleRule = ScheduleRule(scheduleRuleset, daySchedule);
                // LOG(Debug, "Creating a new rule named " << scheduleRule->nameString() << " for " << scheduleWeekName << " with daySchedule"
                //                                         << daySchedule.nameString() << ", startDate=" << startDate << ", endDate=" << endDate);
                scheduleRule->setName(scheduleWeekName);
                scheduleRule->setStartDate(startDate);
                scheduleRule->setEndDate(endDate);

                dayScheduleNames.push_back(scheduleDayName);
                scheduleRules.push_back(*scheduleRule);
              }
            }
          }

          return scheduleRule;
        };

        if (scheduleWeek->iddObject().type() == IddObjectType::Schedule_Week_Daily) {

          // Handled at the end (common path)
          holidayScheduleDay = scheduleWeek->getTarget(Schedule_Week_DailyFields::HolidaySchedule_DayName);
          summerDesignDayScheduleDay = scheduleWeek->getTarget(Schedule_Week_DailyFields::SummerDesignDaySchedule_DayName);
          winterDesignDayScheduleDay = scheduleWeek->getTarget(Schedule_Week_DailyFields::WinterDesignDaySchedule_DayName);
          customDay1ScheduleDay = scheduleWeek->getTarget(Schedule_Week_DailyFields::CustomDay1Schedule_DayName);
          customDay2ScheduleDay = scheduleWeek->getTarget(Schedule_Week_DailyFields::CustomDay2Schedule_DayName);

          std::string scheduleWeekDailyName = scheduleWeek->getString(Schedule_Week_DailyFields::Name).get();
          if (auto schRule_ = getOrCreateRule(scheduleWeek->getTarget(Schedule_Week_DailyFields::SundaySchedule_DayName), scheduleWeekDailyName)) {
            schRule_->setApplySunday(true);
          }
          if (auto schRule_ = getOrCreateRule(scheduleWeek->getTarget(Schedule_Week_DailyFields::MondaySchedule_DayName), scheduleWeekDailyName)) {
            schRule_->setApplyMonday(true);
          }
          if (auto schRule_ = getOrCreateRule(scheduleWeek->getTarget(Schedule_Week_DailyFields::TuesdaySchedule_DayName), scheduleWeekDailyName)) {
            schRule_->setApplyTuesday(true);
          }
          if (auto schRule_ = getOrCreateRule(scheduleWeek->getTarget(Schedule_Week_DailyFields::WednesdaySchedule_DayName), scheduleWeekDailyName)) {
            schRule_->setApplyWednesday(true);
          }
          if (auto schRule_ = getOrCreateRule(scheduleWeek->getTarget(Schedule_Week_DailyFields::ThursdaySchedule_DayName), scheduleWeekDailyName)) {
            schRule_->setApplyThursday(true);
          }
          if (auto schRule_ = getOrCreateRule(scheduleWeek->getTarget(Schedule_Week_DailyFields::FridaySchedule_DayName), scheduleWeekDailyName)) {
            schRule_->setApplyFriday(true);
          }
          if (auto schRule_ = getOrCreateRule(scheduleWeek->getTarget(Schedule_Week_DailyFields::SaturdaySchedule_DayName), scheduleWeekDailyName)) {
            schRule_->setApplySaturday(true);
          }

        } else if (scheduleWeek->iddObject().type() == IddObjectType::Schedule_Week_Compact) {

          std::string scheduleWeekCompactName = scheduleWeek->getString(Schedule_Week_CompactFields::Name).get();

          for (const IdfExtensibleGroup& dayTypeIdfGroup : scheduleWeek->extensibleGroups()) {
            auto dayTypeWorkspaceGroup = dayTypeIdfGroup.cast<WorkspaceExtensibleGroup>();

            std::string dayType = dayTypeWorkspaceGroup.getString(Schedule_Week_CompactExtensibleFields::DayTypeList).get();
            std::string dayTypeLower = openstudio::ascii_to_lower_copy(dayType);
            OptionalWorkspaceObject scheduleDay = dayTypeWorkspaceGroup.getTarget(Schedule_Week_CompactExtensibleFields::Schedule_DayName);

            if (dayTypeLower == "holiday") {
              holidayScheduleDay = scheduleDay;
            } else if (dayTypeLower == "summerdesignday") {
              summerDesignDayScheduleDay = scheduleDay;
            } else if (dayTypeLower == "winterdesignday") {
              winterDesignDayScheduleDay = scheduleDay;
            } else if (dayTypeLower == "customday1") {
              customDay1ScheduleDay = scheduleDay;
            } else if (dayTypeLower == "customday2") {
              customDay2ScheduleDay = scheduleDay;
            } else if (auto schRule_ = getOrCreateRule(scheduleDay, scheduleWeekCompactName)) {
              // TODO: allotherdays not correctly handled here
              if (dayTypeLower == "alldays" || dayTypeLower == "allotherdays") {
                schRule_->setApplySunday(true);
                schRule_->setApplyMonday(true);
                schRule_->setApplyTuesday(true);
                schRule_->setApplyWednesday(true);
                schRule_->setApplyThursday(true);
                schRule_->setApplyFriday(true);
                schRule_->setApplySaturday(true);
              } else if (dayTypeLower == "weekdays") {
                schRule_->setApplyMonday(true);
                schRule_->setApplyTuesday(true);
                schRule_->setApplyWednesday(true);
                schRule_->setApplyThursday(true);
                schRule_->setApplyFriday(true);
              } else if (dayTypeLower == "weekends") {
                schRule_->setApplySunday(true);
                schRule_->setApplySaturday(true);
              } else if (dayTypeLower == "sunday") {
                schRule_->setApplySunday(true);
              } else if (dayTypeLower == "monday") {
                schRule_->setApplyMonday(true);
              } else if (dayTypeLower == "tuesday") {
                schRule_->setApplyTuesday(true);
              } else if (dayTypeLower == "wednesday") {
                schRule_->setApplyWednesday(true);
              } else if (dayTypeLower == "thursday") {
                schRule_->setApplyThursday(true);
              } else if (dayTypeLower == "friday") {
                schRule_->setApplyFriday(true);
              } else if (dayTypeLower == "saturday") {
                schRule_->setApplySaturday(true);
              }
            }
          }
        }
      }  // End loop on extensible groups

      // Common path to both Schedule_Week_Daily and Schedule_Week_Compact
      if (holidayScheduleDay) {
        if (auto mo_ = translateAndMapWorkspaceObject(*holidayScheduleDay)) {
          auto scheduleDay = mo_->cast<ScheduleDay>();
          scheduleRuleset.setHolidaySchedule(scheduleDay);
        }
      }
      if (summerDesignDayScheduleDay) {
        if (auto mo_ = translateAndMapWorkspaceObject(*summerDesignDayScheduleDay)) {
          auto scheduleDay = mo_->cast<ScheduleDay>();
          scheduleRuleset.setSummerDesignDaySchedule(scheduleDay);
        }
      }
      if (winterDesignDayScheduleDay) {
        if (auto mo_ = translateAndMapWorkspaceObject(*winterDesignDayScheduleDay)) {
          auto scheduleDay = mo_->cast<ScheduleDay>();
          scheduleRuleset.setWinterDesignDaySchedule(scheduleDay);
        }
      }
      if (customDay1ScheduleDay) {
        if (auto mo_ = translateAndMapWorkspaceObject(*customDay1ScheduleDay)) {
          auto scheduleDay = mo_->cast<ScheduleDay>();
          scheduleRuleset.setCustomDay1Schedule(scheduleDay);
        }
      }
      if (customDay2ScheduleDay) {
        if (auto mo_ = translateAndMapWorkspaceObject(*customDay2ScheduleDay)) {
          auto scheduleDay = mo_->cast<ScheduleDay>();
          scheduleRuleset.setCustomDay2Schedule(scheduleDay);
        }
      }
      return scheduleRuleset;
    }  // end if hasMoreThanOneSpecialDaysInEachType()
  }
}  // namespace energyplus

}  // namespace openstudio
