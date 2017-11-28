/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ScheduleRule::iddObjectType());
  }

  ScheduleRule_Impl::ScheduleRule_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ScheduleRule::iddObjectType());
  }

  ScheduleRule_Impl::ScheduleRule_Impl(const ScheduleRule_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  ScheduleRule_Impl::~ScheduleRule_Impl()
  {}

  boost::optional<ParentObject> ScheduleRule_Impl::parent() const
  {
    return this->scheduleRuleset();
  }

  bool ScheduleRule_Impl::setParent(ParentObject& newParent)
  {
    bool result = false;
    if (newParent.optionalCast<ScheduleRuleset>()){
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

  std::vector<ModelObject> ScheduleRule_Impl::children() const
  {
    std::vector<ModelObject> result;
    // daySchedule can be a child (appropriate for .clone()), because removed from .resources() list.
    if (OptionalScheduleDay schedule = optionalDaySchedule()) {
      result.push_back(*schedule);
    }
    return result;
  }

  std::vector<IdfObject> ScheduleRule_Impl::remove()
  {
    ScheduleRule self = this->getObject<ScheduleRule>();
    ScheduleRuleset scheduleRuleset = this->scheduleRuleset();
    scheduleRuleset.moveToEnd(self);

    return ParentObject_Impl::remove();
  }

  const std::vector<std::string>& ScheduleRule_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ScheduleRule_Impl::iddObjectType() const {
    return ScheduleRule::iddObjectType();
  }

  ScheduleRuleset ScheduleRule_Impl::scheduleRuleset() const
  {
    OptionalScheduleRuleset result = getObject<ScheduleRule>().getModelObjectTarget<ScheduleRuleset>(OS_Schedule_RuleFields::ScheduleRulesetName);
    OS_ASSERT(result);
    return *result;
  }

  int ScheduleRule_Impl::ruleIndex() const {
    boost::optional<int> value = getInt(OS_Schedule_RuleFields::RuleOrder,true);
    OS_ASSERT(value);
    return value.get();
  }

  ScheduleDay ScheduleRule_Impl::daySchedule() const
  {
    OptionalScheduleDay result = optionalDaySchedule();
    if (!result) {
      LOG_AND_THROW(briefDescription() << " has been corrupted. It no longer points to an "
                    << "OS:Schedule:Day object.");
    }
    return *result;
  }

  bool ScheduleRule_Impl::applySunday() const {
    boost::optional<std::string> value = getString(OS_Schedule_RuleFields::ApplySunday,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool ScheduleRule_Impl::applyMonday() const {
    boost::optional<std::string> value = getString(OS_Schedule_RuleFields::ApplyMonday,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool ScheduleRule_Impl::applyTuesday() const {
    boost::optional<std::string> value = getString(OS_Schedule_RuleFields::ApplyTuesday,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool ScheduleRule_Impl::applyWednesday() const {
    boost::optional<std::string> value = getString(OS_Schedule_RuleFields::ApplyWednesday,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool ScheduleRule_Impl::applyThursday() const {
    boost::optional<std::string> value = getString(OS_Schedule_RuleFields::ApplyThursday,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool ScheduleRule_Impl::applyFriday() const {
    boost::optional<std::string> value = getString(OS_Schedule_RuleFields::ApplyFriday,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool ScheduleRule_Impl::applySaturday() const {
    boost::optional<std::string> value = getString(OS_Schedule_RuleFields::ApplySaturday,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }
/*
  bool ScheduleRule_Impl::applyHoliday() const {
    boost::optional<std::string> value = getString(OS_Schedule_RuleFields::ApplyHoliday,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }
*/

  std::string ScheduleRule_Impl::dateSpecificationType() const {
    boost::optional<std::string> value = getString(OS_Schedule_RuleFields::DateSpecificationType,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<openstudio::Date> ScheduleRule_Impl::startDate() const
  {
    boost::optional<openstudio::Date> result;
    boost::optional<std::string> dateSpecificationType = this->getString(OS_Schedule_RuleFields::DateSpecificationType, true);
    OS_ASSERT(dateSpecificationType);
    if (istringEqual("DateRange", *dateSpecificationType)){
      boost::optional<int> startMonth = this->getInt(OS_Schedule_RuleFields::StartMonth, true);
      OS_ASSERT(startMonth);
      boost::optional<int> startDay = this->getInt(OS_Schedule_RuleFields::StartDay, true);
      OS_ASSERT(startDay);
      YearDescription yd = this->model().getUniqueModelObject<model::YearDescription>();
      result = yd.makeDate(MonthOfYear(*startMonth), *startDay);
    }

    return result;
  }

  boost::optional<openstudio::Date> ScheduleRule_Impl::endDate() const
  {
    boost::optional<openstudio::Date> result;
    boost::optional<std::string> dateSpecificationType = this->getString(OS_Schedule_RuleFields::DateSpecificationType, true);
    OS_ASSERT(dateSpecificationType);
    if (istringEqual("DateRange", *dateSpecificationType)){
      boost::optional<int> endMonth = this->getInt(OS_Schedule_RuleFields::EndMonth, true);
      OS_ASSERT(endMonth);
      boost::optional<int> endDay = this->getInt(OS_Schedule_RuleFields::EndDay, true);
      OS_ASSERT(endDay);
      YearDescription yd = this->model().getUniqueModelObject<model::YearDescription>();
      result = yd.makeDate(MonthOfYear(*endMonth), *endDay);
    }

    return result;
  }

  std::vector<openstudio::Date> ScheduleRule_Impl::specificDates() const
  {
    YearDescription yd = this->model().getUniqueModelObject<model::YearDescription>();

    std::vector<openstudio::Date> result;
    for (const ModelExtensibleGroup& group : castVector<ModelExtensibleGroup>(extensibleGroups()))
    {
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

  void ScheduleRule_Impl::setApplySunday(bool applySunday) {
    bool result = false;
    if (applySunday) {
      result = setString(OS_Schedule_RuleFields::ApplySunday, "Yes");
    } else {
      result = setString(OS_Schedule_RuleFields::ApplySunday, "No");
    }
    OS_ASSERT(result);
  }

  void ScheduleRule_Impl::setApplyMonday(bool applyMonday) {
    bool result = false;
    if (applyMonday) {
      result = setString(OS_Schedule_RuleFields::ApplyMonday, "Yes");
    } else {
      result = setString(OS_Schedule_RuleFields::ApplyMonday, "No");
    }
    OS_ASSERT(result);
  }

  void ScheduleRule_Impl::setApplyTuesday(bool applyTuesday) {
    bool result = false;
    if (applyTuesday) {
      result = setString(OS_Schedule_RuleFields::ApplyTuesday, "Yes");
    } else {
      result = setString(OS_Schedule_RuleFields::ApplyTuesday, "No");
    }
    OS_ASSERT(result);
  }

  void ScheduleRule_Impl::setApplyWednesday(bool applyWednesday) {
    bool result = false;
    if (applyWednesday) {
      result = setString(OS_Schedule_RuleFields::ApplyWednesday, "Yes");
    } else {
      result = setString(OS_Schedule_RuleFields::ApplyWednesday, "No");
    }
    OS_ASSERT(result);
  }

  void ScheduleRule_Impl::setApplyThursday(bool applyThursday) {
    bool result = false;
    if (applyThursday) {
      result = setString(OS_Schedule_RuleFields::ApplyThursday, "Yes");
    } else {
      result = setString(OS_Schedule_RuleFields::ApplyThursday, "No");
    }
    OS_ASSERT(result);
  }

  void ScheduleRule_Impl::setApplyFriday(bool applyFriday) {
    bool result = false;
    if (applyFriday) {
      result = setString(OS_Schedule_RuleFields::ApplyFriday, "Yes");
    } else {
      result = setString(OS_Schedule_RuleFields::ApplyFriday, "No");
    }
    OS_ASSERT(result);
  }

  void ScheduleRule_Impl::setApplySaturday(bool applySaturday) {
    bool result = false;
    if (applySaturday) {
      result = setString(OS_Schedule_RuleFields::ApplySaturday, "Yes");
    } else {
      result = setString(OS_Schedule_RuleFields::ApplySaturday, "No");
    }
    OS_ASSERT(result);
  }
/*
  void ScheduleRule_Impl::setApplyHoliday(bool applyHoliday) {
    bool result = false;
    if (applyHoliday) {
      result = setString(OS_Schedule_RuleFields::ApplyHoliday, "Yes");
    } else {
      result = setString(OS_Schedule_RuleFields::ApplyHoliday, "No");
    }
    OS_ASSERT(result);
  }

  bool ScheduleRule_Impl::setDateSpecificationType(std::string dateSpecificationType) {
    bool result = setString(OS_Schedule_RuleFields::DateSpecificationType, dateSpecificationType);
    return result;
  }
  */

  bool ScheduleRule_Impl::setStartDate(const openstudio::Date& date) {
    bool result = setString(OS_Schedule_RuleFields::DateSpecificationType, "DateRange");
    OS_ASSERT(result);
    result = this->setInt(OS_Schedule_RuleFields::StartMonth, date.monthOfYear().value());
    OS_ASSERT(result);
    result = this->setInt(OS_Schedule_RuleFields::StartDay, date.dayOfMonth());
    OS_ASSERT(result);

    // set end date if need to
    if (this->isEmpty(OS_Schedule_RuleFields::EndMonth) || this->isEmpty(OS_Schedule_RuleFields::EndDay)){
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
    if (this->isEmpty(OS_Schedule_RuleFields::StartMonth) || this->isEmpty(OS_Schedule_RuleFields::StartDay)){
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

    std::vector<std::string> values;
    values.push_back(boost::lexical_cast<std::string>(date.monthOfYear().value()));
    values.push_back(boost::lexical_cast<std::string>(date.dayOfMonth()));

    ModelExtensibleGroup group = pushExtensibleGroup(values, true).cast<ModelExtensibleGroup>();
    OS_ASSERT(!group.empty());

    return true;
  }

  void ScheduleRule_Impl::ensureNoLeapDays(){
    boost::optional<int> month;
    boost::optional<int> day;

    month = getInt(OS_Schedule_RuleFields::StartMonth);
    if (month && (month.get() == 2)){
      day = this->getInt(OS_Schedule_RuleFields::StartDay);
      if (day && (day.get() == 29)){
        this->setInt(OS_Schedule_RuleFields::StartDay, 28);
      }
    }

    month = getInt(OS_Schedule_RuleFields::EndMonth);
    if (month && (month.get() == 2)){
      day = this->getInt(OS_Schedule_RuleFields::EndDay);
      if (day && (day.get() == 29)){
        this->setInt(OS_Schedule_RuleFields::EndDay, 28);
      }
    }

    for (IdfExtensibleGroup group : this->extensibleGroups()){
      boost::optional<int> month;
      boost::optional<int> day;

      month = group.getInt(OS_Schedule_RuleExtensibleFields::SpecificMonth);
      if (month && (month.get() == 2)){
        day = group.getInt(OS_Schedule_RuleExtensibleFields::SpecificDay);
        if (day && (day.get() == 29)){
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
    if (istringEqual("DateRange", *dateSpecificationType)){
      boost::optional<openstudio::Date> startDate = this->startDate();
      OS_ASSERT(startDate);
      boost::optional<openstudio::Date> endDate = this->endDate();
      OS_ASSERT(endDate);
      if (*startDate <= *endDate){
        result = ((date >= *startDate) && (date <= *endDate));
      }else{
        result = ((date >= *startDate) || (date <= *endDate));
      }
    }else{
      std::vector<openstudio::Date> specificDates = this->specificDates();
      for (const openstudio::Date& specificDate : specificDates){
        if (specificDate == date){
          result = true;
          break;
        }
      }
    }

    if (result){
      switch (date.dayOfWeek().value()){
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
    if (istringEqual("DateRange", *dateSpecificationType)){
      boost::optional<openstudio::Date> startDate = this->startDate();
      OS_ASSERT(startDate);
      boost::optional<openstudio::Date> endDate = this->endDate();
      OS_ASSERT(endDate);
      if (*startDate <= *endDate){
        for (unsigned i = 0; i < N; ++i){
          result[i] = ((dates[i] >= *startDate) && (dates[i] <= *endDate));
        }
      }else{
        for (unsigned i = 0; i < N; ++i){
          result[i] = ((dates[i] >= *startDate) || (dates[i] <= *endDate));
        }
      }
    }else{
      std::vector<openstudio::Date> specificDates = this->specificDates();
      for (unsigned i = 0; i < N; ++i){
        for (const openstudio::Date& specificDate : specificDates){
          if (specificDate == dates[i]){
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

    for (unsigned i = 0; i < N; ++i){
      if (result[i]){
        switch (dates[i].dayOfWeek().value()){
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

} // detail

ScheduleRule::ScheduleRule(ScheduleRuleset& scheduleRuleset)
  : ParentObject(ScheduleRule::iddObjectType(), scheduleRuleset.model())
{
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
  : ParentObject(ScheduleRule::iddObjectType(), scheduleRuleset.model())
{
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
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Schedule_RuleFields::DateSpecificationType);
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
/*
bool ScheduleRule::applyHoliday() const {
  return getImpl<detail::ScheduleRule_Impl>()->applyHoliday();
}
*/

boost::optional<openstudio::Date> ScheduleRule::startDate() const
{
  return getImpl<detail::ScheduleRule_Impl>()->startDate();
}

boost::optional<openstudio::Date> ScheduleRule::endDate() const
{
  return getImpl<detail::ScheduleRule_Impl>()->endDate();
}

std::vector<openstudio::Date> ScheduleRule::specificDates() const
{
  return getImpl<detail::ScheduleRule_Impl>()->specificDates();
}

std::string ScheduleRule::dateSpecificationType() const {
  return getImpl<detail::ScheduleRule_Impl>()->dateSpecificationType();
}

bool ScheduleRule::setRuleIndex(int index) {
  return getImpl<detail::ScheduleRule_Impl>()->setRuleIndex(index);
}

void ScheduleRule::setApplySunday(bool applySunday) {
  getImpl<detail::ScheduleRule_Impl>()->setApplySunday(applySunday);
}

void ScheduleRule::setApplyMonday(bool applyMonday) {
  getImpl<detail::ScheduleRule_Impl>()->setApplyMonday(applyMonday);
}

void ScheduleRule::setApplyTuesday(bool applyTuesday) {
  getImpl<detail::ScheduleRule_Impl>()->setApplyTuesday(applyTuesday);
}

void ScheduleRule::setApplyWednesday(bool applyWednesday) {
  getImpl<detail::ScheduleRule_Impl>()->setApplyWednesday(applyWednesday);
}

void ScheduleRule::setApplyThursday(bool applyThursday) {
  getImpl<detail::ScheduleRule_Impl>()->setApplyThursday(applyThursday);
}

void ScheduleRule::setApplyFriday(bool applyFriday) {
  getImpl<detail::ScheduleRule_Impl>()->setApplyFriday(applyFriday);
}

void ScheduleRule::setApplySaturday(bool applySaturday) {
  getImpl<detail::ScheduleRule_Impl>()->setApplySaturday(applySaturday);
}

/*
void ScheduleRule::setApplyHoliday(bool applyHoliday) {
  getImpl<detail::ScheduleRule_Impl>()->setApplyHoliday(applyHoliday);
}
*/

bool ScheduleRule::setStartDate(const openstudio::Date& date) {
  return getImpl<detail::ScheduleRule_Impl>()->setStartDate(date);
}


bool ScheduleRule::setEndDate(const openstudio::Date& date) {
  return getImpl<detail::ScheduleRule_Impl>()->setEndDate(date);
}


bool ScheduleRule::addSpecificDate(const openstudio::Date& date) {
  return getImpl<detail::ScheduleRule_Impl>()->addSpecificDate(date);
}

void ScheduleRule::ensureNoLeapDays(){
  return getImpl<detail::ScheduleRule_Impl>()->ensureNoLeapDays();
}

bool ScheduleRule::containsDate(const openstudio::Date& date) {
  return getImpl<detail::ScheduleRule_Impl>()->containsDate(date);
}

std::vector<bool> ScheduleRule::containsDates(const std::vector<openstudio::Date>& dates) {
  return getImpl<detail::ScheduleRule_Impl>()->containsDates(dates);
}


/// @cond
ScheduleRule::ScheduleRule(std::shared_ptr<detail::ScheduleRule_Impl> impl)
  : ParentObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

