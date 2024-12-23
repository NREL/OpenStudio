/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ScheduleRule.hpp"
#include "ScheduleRule_Impl.hpp"
#include "ScheduleRuleset.hpp"
#include "ScheduleRuleset_Impl.hpp"
#include "ScheduleDay.hpp"
#include "ScheduleDay_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "YearDescription.hpp"
#include "YearDescription_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ModelExtensibleGroup.hpp"

#include "../utilities/time/Date.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Schedule_Rule_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ScheduleRule_Impl::ScheduleRule_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ScheduleRule::iddObjectType());
    }

    ScheduleRule_Impl::ScheduleRule_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ScheduleRule::iddObjectType());
    }

    ScheduleRule_Impl::ScheduleRule_Impl(const ScheduleRule_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {}

    boost::optional<ParentObject> ScheduleRule_Impl::parent() const {
      return this->scheduleRuleset();
    }

    bool ScheduleRule_Impl::setParent(ParentObject& newParent) {
      bool result = false;
      if (newParent.optionalCast<ScheduleRuleset>()) {
        result = setPointer(OS_Schedule_RuleFields::ScheduleRulesetName, newParent.handle());
      }
      return result;
    }

    std::vector<ResourceObject> ScheduleRule_Impl::resources() const {
      // remove daySchedule as a resource so it can be a child
      ResourceObjectVector result = ModelObject_Impl::resources();
      ScheduleDay daySchedule = this->daySchedule();
      auto it = result.begin();
      while (it != result.end()) {
        if ((*it) == daySchedule) {
          result.erase(it);
          break;
        }
        ++it;
      }
      return result;
    }

    std::vector<ModelObject> ScheduleRule_Impl::children() const {
      std::vector<ModelObject> result;
      // daySchedule can be a child (appropriate for .clone()), because removed from .resources() list.
      if (OptionalScheduleDay schedule = optionalDaySchedule()) {
        result.push_back(*schedule);
      }
      return result;
    }

    std::vector<IdfObject> ScheduleRule_Impl::remove() {
      auto self = this->getObject<ScheduleRule>();
      ScheduleRuleset scheduleRuleset = this->scheduleRuleset();
      scheduleRuleset.moveToEnd(self);

      return ParentObject_Impl::remove();
    }

    const std::vector<std::string>& ScheduleRule_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ScheduleRule_Impl::iddObjectType() const {
      return ScheduleRule::iddObjectType();
    }

    ScheduleRuleset ScheduleRule_Impl::scheduleRuleset() const {
      OptionalScheduleRuleset result = getObject<ScheduleRule>().getModelObjectTarget<ScheduleRuleset>(OS_Schedule_RuleFields::ScheduleRulesetName);
      OS_ASSERT(result);
      return *result;
    }

    int ScheduleRule_Impl::ruleIndex() const {
      boost::optional<int> value = getInt(OS_Schedule_RuleFields::RuleOrder, true);
      OS_ASSERT(value);
      return value.get();
    }

    ScheduleDay ScheduleRule_Impl::daySchedule() const {
      OptionalScheduleDay result = optionalDaySchedule();
      if (!result) {
        LOG_AND_THROW(briefDescription() << " has been corrupted. It no longer points to an " << "OS:Schedule:Day object.");
      }
      return *result;
    }

    bool ScheduleRule_Impl::applySunday() const {
      boost::optional<std::string> value = getString(OS_Schedule_RuleFields::ApplySunday, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool ScheduleRule_Impl::applyMonday() const {
      boost::optional<std::string> value = getString(OS_Schedule_RuleFields::ApplyMonday, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool ScheduleRule_Impl::applyTuesday() const {
      boost::optional<std::string> value = getString(OS_Schedule_RuleFields::ApplyTuesday, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool ScheduleRule_Impl::applyWednesday() const {
      boost::optional<std::string> value = getString(OS_Schedule_RuleFields::ApplyWednesday, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool ScheduleRule_Impl::applyThursday() const {
      boost::optional<std::string> value = getString(OS_Schedule_RuleFields::ApplyThursday, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool ScheduleRule_Impl::applyFriday() const {
      boost::optional<std::string> value = getString(OS_Schedule_RuleFields::ApplyFriday, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool ScheduleRule_Impl::applySaturday() const {
      boost::optional<std::string> value = getString(OS_Schedule_RuleFields::ApplySaturday, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    std::string ScheduleRule_Impl::dateSpecificationType() const {
      boost::optional<std::string> value = getString(OS_Schedule_RuleFields::DateSpecificationType, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<openstudio::Date> ScheduleRule_Impl::startDate() const {
      boost::optional<openstudio::Date> result;
      boost::optional<std::string> dateSpecificationType = this->getString(OS_Schedule_RuleFields::DateSpecificationType, true);
      OS_ASSERT(dateSpecificationType);
      if (istringEqual("DateRange", *dateSpecificationType)) {
        boost::optional<int> startMonth = this->getInt(OS_Schedule_RuleFields::StartMonth, true);
        OS_ASSERT(startMonth);
        boost::optional<int> startDay = this->getInt(OS_Schedule_RuleFields::StartDay, true);
        OS_ASSERT(startDay);
        YearDescription yd = this->model().getUniqueModelObject<model::YearDescription>();
        result = yd.makeDate(MonthOfYear(*startMonth), *startDay);
      }

      return result;
    }

    boost::optional<openstudio::Date> ScheduleRule_Impl::endDate() const {
      boost::optional<openstudio::Date> result;
      boost::optional<std::string> dateSpecificationType = this->getString(OS_Schedule_RuleFields::DateSpecificationType, true);
      OS_ASSERT(dateSpecificationType);
      if (istringEqual("DateRange", *dateSpecificationType)) {
        boost::optional<int> endMonth = this->getInt(OS_Schedule_RuleFields::EndMonth, true);
        OS_ASSERT(endMonth);
        boost::optional<int> endDay = this->getInt(OS_Schedule_RuleFields::EndDay, true);
        OS_ASSERT(endDay);
        YearDescription yd = this->model().getUniqueModelObject<model::YearDescription>();
        result = yd.makeDate(MonthOfYear(*endMonth), *endDay);
      }

      return result;
    }

    std::vector<openstudio::Date> ScheduleRule_Impl::specificDates() const {
      YearDescription yd = this->model().getUniqueModelObject<model::YearDescription>();

      std::vector<openstudio::Date> result;
      for (const ModelExtensibleGroup& group : castVector<ModelExtensibleGroup>(extensibleGroups())) {
        OptionalInt month = group.getInt(0);
        OS_ASSERT(month);
        OptionalInt day = group.getInt(1);
        OS_ASSERT(day);

        result.push_back(yd.makeDate(MonthOfYear(*month), *day));
      }

      return result;
    }

    bool ScheduleRule_Impl::setRuleIndex(int index) {
      bool result = setInt(OS_Schedule_RuleFields::RuleOrder, index);
      return result;
    }

    bool ScheduleRule_Impl::setApplySunday(bool applySunday) {
      bool result = false;
      if (applySunday) {
        result = setString(OS_Schedule_RuleFields::ApplySunday, "Yes");
      } else {
        result = setString(OS_Schedule_RuleFields::ApplySunday, "No");
      }
      OS_ASSERT(result);
      return result;
    }

    bool ScheduleRule_Impl::setApplyMonday(bool applyMonday) {
      bool result = false;
      if (applyMonday) {
        result = setString(OS_Schedule_RuleFields::ApplyMonday, "Yes");
      } else {
        result = setString(OS_Schedule_RuleFields::ApplyMonday, "No");
      }
      OS_ASSERT(result);
      return result;
    }

    bool ScheduleRule_Impl::setApplyTuesday(bool applyTuesday) {
      bool result = false;
      if (applyTuesday) {
        result = setString(OS_Schedule_RuleFields::ApplyTuesday, "Yes");
      } else {
        result = setString(OS_Schedule_RuleFields::ApplyTuesday, "No");
      }
      OS_ASSERT(result);
      return result;
    }

    bool ScheduleRule_Impl::setApplyWednesday(bool applyWednesday) {
      bool result = false;
      if (applyWednesday) {
        result = setString(OS_Schedule_RuleFields::ApplyWednesday, "Yes");
      } else {
        result = setString(OS_Schedule_RuleFields::ApplyWednesday, "No");
      }
      OS_ASSERT(result);
      return result;
    }

    bool ScheduleRule_Impl::setApplyThursday(bool applyThursday) {
      bool result = false;
      if (applyThursday) {
        result = setString(OS_Schedule_RuleFields::ApplyThursday, "Yes");
      } else {
        result = setString(OS_Schedule_RuleFields::ApplyThursday, "No");
      }
      OS_ASSERT(result);
      return result;
    }

    bool ScheduleRule_Impl::setApplyFriday(bool applyFriday) {
      bool result = false;
      if (applyFriday) {
        result = setString(OS_Schedule_RuleFields::ApplyFriday, "Yes");
      } else {
        result = setString(OS_Schedule_RuleFields::ApplyFriday, "No");
      }
      OS_ASSERT(result);
      return result;
    }

    bool ScheduleRule_Impl::setApplySaturday(bool applySaturday) {
      bool result = false;
      if (applySaturday) {
        result = setString(OS_Schedule_RuleFields::ApplySaturday, "Yes");
      } else {
        result = setString(OS_Schedule_RuleFields::ApplySaturday, "No");
      }
      OS_ASSERT(result);
      return result;
    }

    bool ScheduleRule_Impl::applyWeekdays() const {
      return (this->applyMonday() && this->applyTuesday() && this->applyWednesday() && this->applyThursday() && this->applyFriday());
    }

    bool ScheduleRule_Impl::applyWeekends() const {
      return (this->applySaturday() && this->applySunday());
    }

    bool ScheduleRule_Impl::applyAllDays() const {
      return (this->applyWeekdays() && this->applyWeekends());
    }

    bool ScheduleRule_Impl::setApplyWeekdays(bool applyWeekdays) {
      return (this->setApplyMonday(applyWeekdays) && this->setApplyTuesday(applyWeekdays) && this->setApplyWednesday(applyWeekdays)
              && this->setApplyThursday(applyWeekdays) && this->setApplyFriday(applyWeekdays));
    }

    bool ScheduleRule_Impl::setApplyWeekends(bool applyWeekends) {
      return (this->setApplySaturday(applyWeekends) && this->setApplySunday(applyWeekends));
    }

    bool ScheduleRule_Impl::setApplyAllDays(bool applyAllDays) {
      return (this->setApplyWeekdays(applyAllDays) && this->setApplyWeekends(applyAllDays));
    }

    bool ScheduleRule_Impl::setStartDate(const openstudio::Date& date) {
      bool result = setString(OS_Schedule_RuleFields::DateSpecificationType, "DateRange");
      OS_ASSERT(result);
      result = this->setInt(OS_Schedule_RuleFields::StartMonth, date.monthOfYear().value());
      OS_ASSERT(result);
      result = this->setInt(OS_Schedule_RuleFields::StartDay, date.dayOfMonth());
      OS_ASSERT(result);

      // set end date if need to
      if (this->isEmpty(OS_Schedule_RuleFields::EndMonth) || this->isEmpty(OS_Schedule_RuleFields::EndDay)) {
        this->setEndDate(openstudio::Date(MonthOfYear::Dec, 31));
      }

      // clear specific dates
      this->clearExtensibleGroups();
      return true;
    }

    bool ScheduleRule_Impl::setEndDate(const openstudio::Date& date) {
      bool result = setString(OS_Schedule_RuleFields::DateSpecificationType, "DateRange", false);
      OS_ASSERT(result);
      result = this->setInt(OS_Schedule_RuleFields::EndMonth, date.monthOfYear().value());
      OS_ASSERT(result);
      result = this->setInt(OS_Schedule_RuleFields::EndDay, date.dayOfMonth());
      OS_ASSERT(result);

      // set end date if need to
      if (this->isEmpty(OS_Schedule_RuleFields::StartMonth) || this->isEmpty(OS_Schedule_RuleFields::StartDay)) {
        this->setStartDate(openstudio::Date(MonthOfYear::Jan, 1));
      }

      // clear specific dates
      this->clearExtensibleGroups();
      return true;
    }

    bool ScheduleRule_Impl::addSpecificDate(const openstudio::Date& date) {

      bool result = setString(OS_Schedule_RuleFields::DateSpecificationType, "SpecificDates", false);
      OS_ASSERT(result);

      result = setString(OS_Schedule_RuleFields::StartMonth, "", false);
      OS_ASSERT(result);
      result = setString(OS_Schedule_RuleFields::StartDay, "", false);
      OS_ASSERT(result);
      result = setString(OS_Schedule_RuleFields::EndMonth, "", false);
      OS_ASSERT(result);
      result = setString(OS_Schedule_RuleFields::EndDay, "", false);
      OS_ASSERT(result);

      std::vector<std::string> values{std::to_string(date.monthOfYear().value()), std::to_string(date.dayOfMonth())};

      auto group = pushExtensibleGroup(values, true).cast<ModelExtensibleGroup>();
      OS_ASSERT(!group.empty());

      return true;
    }

    void ScheduleRule_Impl::ensureNoLeapDays() {
      boost::optional<int> month;
      boost::optional<int> day;

      month = getInt(OS_Schedule_RuleFields::StartMonth);
      if (month && (month.get() == 2)) {
        day = this->getInt(OS_Schedule_RuleFields::StartDay);
        if (day && (day.get() == 29)) {
          this->setInt(OS_Schedule_RuleFields::StartDay, 28);
        }
      }

      month = getInt(OS_Schedule_RuleFields::EndMonth);
      if (month && (month.get() == 2)) {
        day = this->getInt(OS_Schedule_RuleFields::EndDay);
        if (day && (day.get() == 29)) {
          this->setInt(OS_Schedule_RuleFields::EndDay, 28);
        }
      }

      for (const IdfExtensibleGroup& group : this->extensibleGroups()) {
        boost::optional<int> month;
        boost::optional<int> day;

        month = group.getInt(OS_Schedule_RuleExtensibleFields::SpecificMonth);
        if (month && (month.get() == 2)) {
          day = group.getInt(OS_Schedule_RuleExtensibleFields::SpecificDay);
          if (day && (day.get() == 29)) {
            this->setInt(OS_Schedule_RuleExtensibleFields::SpecificDay, 28);
          }
        }
      }
    }

    bool ScheduleRule_Impl::containsDate(const openstudio::Date& date) {
      bool result = false;

      // need to check or adjust assumed base year on input date?

      boost::optional<std::string> dateSpecificationType = this->getString(OS_Schedule_RuleFields::DateSpecificationType, true);
      OS_ASSERT(dateSpecificationType);
      if (istringEqual("DateRange", *dateSpecificationType)) {
        boost::optional<openstudio::Date> startDate = this->startDate();
        OS_ASSERT(startDate);
        boost::optional<openstudio::Date> endDate = this->endDate();
        OS_ASSERT(endDate);
        if (*startDate <= *endDate) {
          result = ((date >= *startDate) && (date <= *endDate));
        } else {
          result = ((date >= *startDate) || (date <= *endDate));
        }
      } else {
        std::vector<openstudio::Date> specificDates = this->specificDates();
        for (const openstudio::Date& specificDate : specificDates) {
          if (specificDate == date) {
            result = true;
            break;
          }
        }
      }

      if (result) {
        switch (date.dayOfWeek().value()) {
          case DayOfWeek::Sunday:
            result = this->applySunday();
            break;
          case DayOfWeek::Monday:
            result = this->applyMonday();
            break;
          case DayOfWeek::Tuesday:
            result = this->applyTuesday();
            break;
          case DayOfWeek::Wednesday:
            result = this->applyWednesday();
            break;
          case DayOfWeek::Thursday:
            result = this->applyThursday();
            break;
          case DayOfWeek::Friday:
            result = this->applyFriday();
            break;
          case DayOfWeek::Saturday:
            result = this->applySaturday();
            break;
          default:
            OS_ASSERT(false);
        }
      }

      return result;
    }

    std::vector<bool> ScheduleRule_Impl::containsDates(const std::vector<openstudio::Date>& dates) {
      unsigned N = dates.size();
      std::vector<bool> result(N, false);

      // need to check or adjust assumed base year on input date?

      boost::optional<std::string> dateSpecificationType = this->getString(OS_Schedule_RuleFields::DateSpecificationType, true);
      OS_ASSERT(dateSpecificationType);
      if (istringEqual("DateRange", *dateSpecificationType)) {
        boost::optional<openstudio::Date> startDate = this->startDate();
        OS_ASSERT(startDate);
        boost::optional<openstudio::Date> endDate = this->endDate();
        OS_ASSERT(endDate);
        if (*startDate <= *endDate) {
          for (unsigned i = 0; i < N; ++i) {
            result[i] = ((dates[i] >= *startDate) && (dates[i] <= *endDate));
          }
        } else {
          for (unsigned i = 0; i < N; ++i) {
            result[i] = ((dates[i] >= *startDate) || (dates[i] <= *endDate));
          }
        }
      } else {
        std::vector<openstudio::Date> specificDates = this->specificDates();
        for (unsigned i = 0; i < N; ++i) {
          for (const openstudio::Date& specificDate : specificDates) {
            if (specificDate == dates[i]) {
              result[i] = true;
              break;
            }
          }
        }
      }

      bool applySunday = this->applySunday();
      bool applyMonday = this->applyMonday();
      bool applyTuesday = this->applyTuesday();
      bool applyWednesday = this->applyWednesday();
      bool applyThursday = this->applyThursday();
      bool applyFriday = this->applyFriday();
      bool applySaturday = this->applySaturday();

      for (unsigned i = 0; i < N; ++i) {
        if (result[i]) {
          switch (dates[i].dayOfWeek().value()) {
            case DayOfWeek::Sunday:
              result[i] = applySunday;
              break;
            case DayOfWeek::Monday:
              result[i] = applyMonday;
              break;
            case DayOfWeek::Tuesday:
              result[i] = applyTuesday;
              break;
            case DayOfWeek::Wednesday:
              result[i] = applyWednesday;
              break;
            case DayOfWeek::Thursday:
              result[i] = applyThursday;
              break;
            case DayOfWeek::Friday:
              result[i] = applyFriday;
              break;
            case DayOfWeek::Saturday:
              result[i] = applySaturday;
              break;
            default:
              OS_ASSERT(false);
          }
        }
      }

      return result;
    }

    boost::optional<ScheduleDay> ScheduleRule_Impl::optionalDaySchedule() const {
      return getObject<ScheduleRule>().getModelObjectTarget<ScheduleDay>(OS_Schedule_RuleFields::DayScheduleName);
    }

  }  // namespace detail

  ScheduleRule::ScheduleRule(ScheduleRuleset& scheduleRuleset) : ParentObject(ScheduleRule::iddObjectType(), scheduleRuleset.model()) {
    OS_ASSERT(getImpl<detail::ScheduleRule_Impl>());

    bool result = setPointer(OS_Schedule_RuleFields::ScheduleRulesetName, scheduleRuleset.handle());
    OS_ASSERT(result);

    ScheduleDay daySchedule(scheduleRuleset.model());
    result = setPointer(OS_Schedule_RuleFields::DayScheduleName, daySchedule.handle());
    OS_ASSERT(result);
    if (OptionalScheduleTypeLimits limits = scheduleRuleset.scheduleTypeLimits()) {
      daySchedule.setScheduleTypeLimits(*limits);
    }

    this->setRuleIndex(std::numeric_limits<int>::max());
    result = scheduleRuleset.setScheduleRuleIndex(*this, 0);
    OS_ASSERT(result);
  }

  ScheduleRule::ScheduleRule(ScheduleRuleset& scheduleRuleset, const ScheduleDay& daySchedule)
    : ParentObject(ScheduleRule::iddObjectType(), scheduleRuleset.model()) {
    OS_ASSERT(getImpl<detail::ScheduleRule_Impl>());

    bool result = setPointer(OS_Schedule_RuleFields::ScheduleRulesetName, scheduleRuleset.handle());
    OS_ASSERT(result);

    ModelObject clone = daySchedule.clone(scheduleRuleset.model());
    result = setPointer(OS_Schedule_RuleFields::DayScheduleName, clone.handle());
    OS_ASSERT(result);
    if (OptionalScheduleTypeLimits limits = scheduleRuleset.scheduleTypeLimits()) {
      clone.cast<ScheduleDay>().setScheduleTypeLimits(*limits);
    }

    this->setRuleIndex(std::numeric_limits<int>::max());
    result = scheduleRuleset.setScheduleRuleIndex(*this, 0);
    OS_ASSERT(result);
  }

  IddObjectType ScheduleRule::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Schedule_Rule);
    return result;
  }

  std::vector<std::string> ScheduleRule::validDateSpecificationTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Schedule_RuleFields::DateSpecificationType);
  }

  ScheduleRuleset ScheduleRule::scheduleRuleset() const {
    return getImpl<detail::ScheduleRule_Impl>()->scheduleRuleset();
  }

  int ScheduleRule::ruleIndex() const {
    return getImpl<detail::ScheduleRule_Impl>()->ruleIndex();
  }

  ScheduleDay ScheduleRule::daySchedule() const {
    return getImpl<detail::ScheduleRule_Impl>()->daySchedule();
  }

  bool ScheduleRule::applySunday() const {
    return getImpl<detail::ScheduleRule_Impl>()->applySunday();
  }

  bool ScheduleRule::applyMonday() const {
    return getImpl<detail::ScheduleRule_Impl>()->applyMonday();
  }

  bool ScheduleRule::applyTuesday() const {
    return getImpl<detail::ScheduleRule_Impl>()->applyTuesday();
  }

  bool ScheduleRule::applyWednesday() const {
    return getImpl<detail::ScheduleRule_Impl>()->applyWednesday();
  }

  bool ScheduleRule::applyThursday() const {
    return getImpl<detail::ScheduleRule_Impl>()->applyThursday();
  }

  bool ScheduleRule::applyFriday() const {
    return getImpl<detail::ScheduleRule_Impl>()->applyFriday();
  }

  bool ScheduleRule::applySaturday() const {
    return getImpl<detail::ScheduleRule_Impl>()->applySaturday();
  }

  boost::optional<openstudio::Date> ScheduleRule::startDate() const {
    return getImpl<detail::ScheduleRule_Impl>()->startDate();
  }

  boost::optional<openstudio::Date> ScheduleRule::endDate() const {
    return getImpl<detail::ScheduleRule_Impl>()->endDate();
  }

  std::vector<openstudio::Date> ScheduleRule::specificDates() const {
    return getImpl<detail::ScheduleRule_Impl>()->specificDates();
  }

  std::string ScheduleRule::dateSpecificationType() const {
    return getImpl<detail::ScheduleRule_Impl>()->dateSpecificationType();
  }

  bool ScheduleRule::setRuleIndex(int index) {
    return getImpl<detail::ScheduleRule_Impl>()->setRuleIndex(index);
  }

  bool ScheduleRule::setApplySunday(bool applySunday) {
    return getImpl<detail::ScheduleRule_Impl>()->setApplySunday(applySunday);
  }

  void ScheduleRule::setApplySundayNoFail(bool applySunday) {
    bool result = getImpl<detail::ScheduleRule_Impl>()->setApplySunday(applySunday);
    OS_ASSERT(result);
  }

  bool ScheduleRule::setApplyMonday(bool applyMonday) {
    return getImpl<detail::ScheduleRule_Impl>()->setApplyMonday(applyMonday);
  }

  void ScheduleRule::setApplyMondayNoFail(bool applyMonday) {
    bool result = getImpl<detail::ScheduleRule_Impl>()->setApplyMonday(applyMonday);
    OS_ASSERT(result);
  }

  bool ScheduleRule::setApplyTuesday(bool applyTuesday) {
    return getImpl<detail::ScheduleRule_Impl>()->setApplyTuesday(applyTuesday);
  }

  void ScheduleRule::setApplyTuesdayNoFail(bool applyTuesday) {
    bool result = getImpl<detail::ScheduleRule_Impl>()->setApplyTuesday(applyTuesday);
    OS_ASSERT(result);
  }

  bool ScheduleRule::setApplyWednesday(bool applyWednesday) {
    return getImpl<detail::ScheduleRule_Impl>()->setApplyWednesday(applyWednesday);
  }

  void ScheduleRule::setApplyWednesdayNoFail(bool applyWednesday) {
    bool result = getImpl<detail::ScheduleRule_Impl>()->setApplyWednesday(applyWednesday);
    OS_ASSERT(result);
  }

  bool ScheduleRule::setApplyThursday(bool applyThursday) {
    return getImpl<detail::ScheduleRule_Impl>()->setApplyThursday(applyThursday);
  }

  void ScheduleRule::setApplyThursdayNoFail(bool applyThursday) {
    bool result = getImpl<detail::ScheduleRule_Impl>()->setApplyThursday(applyThursday);
    OS_ASSERT(result);
  }

  bool ScheduleRule::setApplyFriday(bool applyFriday) {
    return getImpl<detail::ScheduleRule_Impl>()->setApplyFriday(applyFriday);
  }

  void ScheduleRule::setApplyFridayNoFail(bool applyFriday) {
    bool result = getImpl<detail::ScheduleRule_Impl>()->setApplyFriday(applyFriday);
    OS_ASSERT(result);
  }

  bool ScheduleRule::setApplySaturday(bool applySaturday) {
    return getImpl<detail::ScheduleRule_Impl>()->setApplySaturday(applySaturday);
  }

  void ScheduleRule::setApplySaturdayNoFail(bool applySaturday) {
    bool result = getImpl<detail::ScheduleRule_Impl>()->setApplySaturday(applySaturday);
    OS_ASSERT(result);
  }

  bool ScheduleRule::applyWeekdays() const {
    return getImpl<detail::ScheduleRule_Impl>()->applyWeekdays();
  }

  bool ScheduleRule::setApplyWeekdays(bool applyWeekdays) {
    return getImpl<detail::ScheduleRule_Impl>()->setApplyWeekdays(applyWeekdays);
  }

  bool ScheduleRule::applyWeekends() const {
    return getImpl<detail::ScheduleRule_Impl>()->applyWeekends();
  }

  bool ScheduleRule::setApplyWeekends(bool applyWeekends) {
    return getImpl<detail::ScheduleRule_Impl>()->setApplyWeekends(applyWeekends);
  }

  bool ScheduleRule::applyAllDays() const {
    return getImpl<detail::ScheduleRule_Impl>()->applyAllDays();
  }

  bool ScheduleRule::setApplyAllDays(bool applyAllDays) {
    return getImpl<detail::ScheduleRule_Impl>()->setApplyAllDays(applyAllDays);
  }

  bool ScheduleRule::setStartDate(const openstudio::Date& date) {
    return getImpl<detail::ScheduleRule_Impl>()->setStartDate(date);
  }

  bool ScheduleRule::setEndDate(const openstudio::Date& date) {
    return getImpl<detail::ScheduleRule_Impl>()->setEndDate(date);
  }

  bool ScheduleRule::addSpecificDate(const openstudio::Date& date) {
    return getImpl<detail::ScheduleRule_Impl>()->addSpecificDate(date);
  }

  void ScheduleRule::ensureNoLeapDays() {
    return getImpl<detail::ScheduleRule_Impl>()->ensureNoLeapDays();
  }

  bool ScheduleRule::containsDate(const openstudio::Date& date) {
    return getImpl<detail::ScheduleRule_Impl>()->containsDate(date);
  }

  std::vector<bool> ScheduleRule::containsDates(const std::vector<openstudio::Date>& dates) {
    return getImpl<detail::ScheduleRule_Impl>()->containsDates(dates);
  }

  /// @cond
  ScheduleRule::ScheduleRule(std::shared_ptr<detail::ScheduleRule_Impl> impl) : ParentObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
