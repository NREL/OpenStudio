/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeLimits_Impl.hpp"
#include "../../model/ScheduleRuleset.hpp"
#include "../../model/ScheduleRuleset_Impl.hpp"
#include "../../model/ScheduleRule.hpp"
#include "../../model/ScheduleRule_Impl.hpp"
#include "../../model/ScheduleDay.hpp"
#include "../../model/ScheduleDay_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/time/Date.hpp"
#include "../../utilities/time/Time.hpp"

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Schedule_Week_Rule_FieldEnums.hxx>
#include <utilities/idd/Schedule_Year_Rules_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateScheduleRuleset(ScheduleRuleset& modelObject) {

    IdfObject scheduleYearRules(openstudio::IddObjectType::Schedule_Year_Rules);

    std::string scheduleYearRulesName = modelObject.name().get();
    scheduleYearRules.setName(scheduleYearRulesName);

    boost::optional<ScheduleTypeLimits> scheduleTypeLimits = modelObject.scheduleTypeLimits();
    if (scheduleTypeLimits) {
      boost::optional<IdfObject> idfObject = translateAndMapModelObject(*scheduleTypeLimits);
      if (idfObject) {
        scheduleYearRules.setString(Schedule_Year_RulesFields::ScheduleTypeLimitsName, idfObject->name().get());
      }
    }

    ScheduleDay defaultDaySchedule = modelObject.defaultDaySchedule();
    scheduleYearRules.setString(Schedule_Year_RulesFields::DefaultDayScheduleName, defaultDaySchedule.nameString());

    ScheduleDay summerDesignDaySchedule = modelObject.summerDesignDaySchedule();
    scheduleYearRules.setString(Schedule_Year_RulesFields::SummerDesignDayScheduleName, summerDesignDaySchedule.nameString());

    ScheduleDay winterDesignDaySchedule = modelObject.winterDesignDaySchedule();
    scheduleYearRules.setString(Schedule_Year_RulesFields::WinterDesignDayScheduleName, winterDesignDaySchedule.nameString());

    ScheduleDay holidaySchedule = modelObject.holidaySchedule();
    scheduleYearRules.setString(Schedule_Year_RulesFields::HolidayScheduleName, holidaySchedule.nameString());

    ScheduleDay customDay1Schedule = modelObject.customDay1Schedule();
    scheduleYearRules.setString(Schedule_Year_RulesFields::CustomDay1ScheduleName, customDay1Schedule.nameString());

    ScheduleDay customDay2Schedule = modelObject.customDay2Schedule();
    scheduleYearRules.setString(Schedule_Year_RulesFields::CustomDay2ScheduleName, customDay2Schedule.nameString());

    translateAndMapModelObject(defaultDaySchedule);
    translateAndMapModelObject(summerDesignDaySchedule);
    translateAndMapModelObject(winterDesignDaySchedule);
    translateAndMapModelObject(holidaySchedule);
    translateAndMapModelObject(customDay1Schedule);
    translateAndMapModelObject(customDay2Schedule);

    for (const ScheduleRule& scheduleRule : modelObject.scheduleRules()) {
      IdfObject scheduleWeekRule(openstudio::IddObjectType::Schedule_Week_Rule);

      std::string scheduleWeekRuleName = scheduleRule.name().get();
      scheduleWeekRule.setName(scheduleWeekRuleName);

      scheduleWeekRule.setString(Schedule_Week_RuleFields::ScheduleYearRulesName, scheduleYearRulesName);

      scheduleWeekRule.setInt(Schedule_Week_RuleFields::RulePriorityOrder, scheduleRule.ruleIndex());

      ScheduleDay daySchedule = scheduleRule.daySchedule();
      scheduleWeekRule.setString(Schedule_Week_RuleFields::DayScheduleName, daySchedule.nameString());

      translateAndMapModelObject(daySchedule);

      if (scheduleRule.applySunday()) {
        scheduleWeekRule.setString(Schedule_Week_RuleFields::ApplySunday, "Yes");
      } else {
        scheduleWeekRule.setString(Schedule_Week_RuleFields::ApplySunday, "No");
      }
      if (scheduleRule.applyMonday()) {
        scheduleWeekRule.setString(Schedule_Week_RuleFields::ApplyMonday, "Yes");
      } else {
        scheduleWeekRule.setString(Schedule_Week_RuleFields::ApplyMonday, "No");
      }
      if (scheduleRule.applyTuesday()) {
        scheduleWeekRule.setString(Schedule_Week_RuleFields::ApplyTuesday, "Yes");
      } else {
        scheduleWeekRule.setString(Schedule_Week_RuleFields::ApplyTuesday, "No");
      }
      if (scheduleRule.applyWednesday()) {
        scheduleWeekRule.setString(Schedule_Week_RuleFields::ApplyWednesday, "Yes");
      } else {
        scheduleWeekRule.setString(Schedule_Week_RuleFields::ApplyWednesday, "No");
      }
      if (scheduleRule.applyThursday()) {
        scheduleWeekRule.setString(Schedule_Week_RuleFields::ApplyThursday, "Yes");
      } else {
        scheduleWeekRule.setString(Schedule_Week_RuleFields::ApplyThursday, "No");
      }
      if (scheduleRule.applyFriday()) {
        scheduleWeekRule.setString(Schedule_Week_RuleFields::ApplyFriday, "Yes");
      } else {
        scheduleWeekRule.setString(Schedule_Week_RuleFields::ApplyFriday, "No");
      }
      if (scheduleRule.applySaturday()) {
        scheduleWeekRule.setString(Schedule_Week_RuleFields::ApplySaturday, "Yes");
      } else {
        scheduleWeekRule.setString(Schedule_Week_RuleFields::ApplySaturday, "No");
      }

      if (istringEqual("DateRange", scheduleRule.dateSpecificationType())) {
        IdfExtensibleGroup group = scheduleWeekRule.pushExtensibleGroup();
        if (boost::optional<openstudio::Date> _startDate = scheduleRule.startDate()) {
          group.setInt(Schedule_Week_RuleExtensibleFields::StartMonth, _startDate->monthOfYear().value());
          group.setInt(Schedule_Week_RuleExtensibleFields::StartDay, _startDate->dayOfMonth());
        }
        if (boost::optional<openstudio::Date> _endDate = scheduleRule.endDate()) {
          group.setInt(Schedule_Week_RuleExtensibleFields::EndMonth, _endDate->monthOfYear().value());
          group.setInt(Schedule_Week_RuleExtensibleFields::EndDay, _endDate->dayOfMonth());
        }
      } else { // SpecificDates
        for (const openstudio::Date& specificDate : scheduleRule.specificDates()) {
          IdfExtensibleGroup group = scheduleWeekRule.pushExtensibleGroup();
          group.setInt(Schedule_Week_RuleExtensibleFields::StartMonth, specificDate.monthOfYear().value());
          group.setInt(Schedule_Week_RuleExtensibleFields::StartDay, specificDate.dayOfMonth());
          group.setInt(Schedule_Week_RuleExtensibleFields::EndMonth, specificDate.monthOfYear().value());
          group.setInt(Schedule_Week_RuleExtensibleFields::EndDay, specificDate.dayOfMonth());
        }
      }

       m_idfObjects.push_back(scheduleWeekRule);
    }

    m_idfObjects.push_back(scheduleYearRules);

    m_map.insert(std::make_pair(modelObject.handle(), scheduleYearRules));

    return scheduleYearRules;
  }

}  // namespace energyplus

}  // namespace openstudio
