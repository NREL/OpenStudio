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

#include "ScheduleRuleset.hpp"
#include "ScheduleRuleset_Impl.hpp"
#include "ScheduleRule.hpp"
#include "ScheduleRule_Impl.hpp"
#include "ScheduleDay.hpp"
#include "ScheduleDay_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeLimits_Impl.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "YearDescription.hpp"
#include "YearDescription_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ModelExtensibleGroup.hpp"

#include <utilities/idd/OS_Schedule_Ruleset_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/time/Date.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ScheduleRuleset_Impl::ScheduleRuleset_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Schedule_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ScheduleRuleset::iddObjectType());
  }

  ScheduleRuleset_Impl::ScheduleRuleset_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : Schedule_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ScheduleRuleset::iddObjectType());
  }

  ScheduleRuleset_Impl::ScheduleRuleset_Impl(const ScheduleRuleset_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : Schedule_Impl(other,model,keepHandle)
  {}

  ModelObject ScheduleRuleset_Impl::clone(Model model) const {
    ModelObject newScheduleRulesetAsModelObject = ModelObject_Impl::clone(model);
    ScheduleRuleset newScheduleRuleset = newScheduleRulesetAsModelObject.cast<ScheduleRuleset>();

    ModelObject newDefaultDaySchedule = defaultDaySchedule().clone(model);
    bool test = newScheduleRuleset.setPointer(OS_Schedule_RulesetFields::DefaultDayScheduleName,newDefaultDaySchedule.handle());
    OS_ASSERT(test);

    if (!isSummerDesignDayScheduleDefaulted()) {
      ModelObject newSummerDesignDaySchedule = summerDesignDaySchedule().clone(model);
      test = newScheduleRuleset.setPointer(OS_Schedule_RulesetFields::SummerDesignDayScheduleName,newSummerDesignDaySchedule.handle());
      OS_ASSERT(test);
    }

    if (!isWinterDesignDayScheduleDefaulted()) {
      ModelObject newWinterDesignDaySchedule = winterDesignDaySchedule().clone(model);
      test = newScheduleRuleset.setPointer(OS_Schedule_RulesetFields::WinterDesignDayScheduleName,newWinterDesignDaySchedule.handle());
      OS_ASSERT(test);
    }

    for (ScheduleRule scheduleRule : scheduleRules()) {
      ModelObject newScheduleRule = scheduleRule.clone(model);
      test = newScheduleRule.setParent(newScheduleRuleset);
      OS_ASSERT(test);
    }

    return newScheduleRulesetAsModelObject;
  }

  std::vector<ResourceObject> ScheduleRuleset_Impl::resources() const {
    // remove daySchedule as a resource so it can be a child
    ResourceObjectVector result = ModelObject_Impl::resources();
    ScheduleDayVector daySchedules(1u,defaultDaySchedule());
    if (!isSummerDesignDayScheduleDefaulted()) {
      daySchedules.push_back(summerDesignDaySchedule());
    }
    if (!isWinterDesignDayScheduleDefaulted()) {
      daySchedules.push_back(winterDesignDaySchedule());
    }
    auto it = result.begin();
    while (it != result.end()) {
      auto jit = std::find(daySchedules.begin(),daySchedules.end(),*it);
      if (jit != daySchedules.end()) {
        it = result.erase(it);
        daySchedules.erase(jit);
      }
      else {
        ++it;
      }
    }
    return result;
  }

  std::vector<ModelObject> ScheduleRuleset_Impl::children() const
  {
    std::vector<ModelObject> result;

    // can add the ScheduleDays as children because removed from resource list
    if (OptionalScheduleDay schedule = optionalDefaultDaySchedule()) {
      result.push_back(*schedule);
    }

    if (!this->isSummerDesignDayScheduleDefaulted()){
      result.push_back(this->summerDesignDaySchedule());
    }

    if (!this->isWinterDesignDayScheduleDefaulted()){
      result.push_back(this->winterDesignDaySchedule());
    }

    for (ScheduleRule scheduleRule : this->scheduleRules()){
      result.push_back(scheduleRule);
    }

    return result;
  }

  const std::vector<std::string>& ScheduleRuleset_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ScheduleRuleset_Impl::iddObjectType() const {
    return ScheduleRuleset::iddObjectType();
  }

  boost::optional<ScheduleTypeLimits> ScheduleRuleset_Impl::scheduleTypeLimits() const {
    return getObject<ModelObject>().getModelObjectTarget<ScheduleTypeLimits>(OS_Schedule_RulesetFields::ScheduleTypeLimitsName);
  }

  std::vector<double> ScheduleRuleset_Impl::values() const {
    return DoubleVector();
  }

  bool ScheduleRuleset_Impl::setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) {
    if (scheduleTypeLimits.model() != model()) {
      return false;
    }
    if (!candidateIsCompatibleWithCurrentUse(scheduleTypeLimits)) {
      return false;
    }
    bool result = setPointer(OS_Schedule_RulesetFields::ScheduleTypeLimitsName, scheduleTypeLimits.handle());
    if (result) {
      result = defaultDaySchedule().setScheduleTypeLimits(scheduleTypeLimits);
      OS_ASSERT(result);
      if (!isSummerDesignDayScheduleDefaulted()) {
        result = summerDesignDaySchedule().setScheduleTypeLimits(scheduleTypeLimits);
        OS_ASSERT(result);
      }
      if (!isWinterDesignDayScheduleDefaulted()) {
        result = winterDesignDaySchedule().setScheduleTypeLimits(scheduleTypeLimits);
        OS_ASSERT(result);
      }
      ScheduleRuleVector rules = scheduleRules();
      for (const ScheduleRule& rule : rules) {
        result = rule.daySchedule().setScheduleTypeLimits(scheduleTypeLimits);
        OS_ASSERT(result);
      }
    }
    return result;
  }

  bool ScheduleRuleset_Impl::resetScheduleTypeLimits() {
    bool result(false);
    if (okToResetScheduleTypeLimits()) {
      result = setString(OS_Schedule_RulesetFields::ScheduleTypeLimitsName,"");
      OS_ASSERT(result);
      result = defaultDaySchedule().resetScheduleTypeLimits();
      OS_ASSERT(result);
      if (!isSummerDesignDayScheduleDefaulted()) {
        result = summerDesignDaySchedule().resetScheduleTypeLimits();
        OS_ASSERT(result);
      }
      if (!isWinterDesignDayScheduleDefaulted()) {
        result = winterDesignDaySchedule().resetScheduleTypeLimits();
        OS_ASSERT(result);
      }
      ScheduleRuleVector rules = scheduleRules();
      for (ScheduleRule& rule : rules) {
        result = rule.daySchedule().resetScheduleTypeLimits();
        OS_ASSERT(result);
      }
    }
    return result;
  }

  ScheduleDay ScheduleRuleset_Impl::defaultDaySchedule() const
  {
    OptionalScheduleDay result = optionalDefaultDaySchedule();
    if (!result) {
      LOG_AND_THROW(briefDescription() << " has been corrupted. It no longer points to a default "
                    << "OS:Schedule:Day object.");
    }
    return *result;
  }

  ScheduleDay ScheduleRuleset_Impl::summerDesignDaySchedule() const
  {
    OptionalScheduleDay result = getObject<ScheduleRuleset>().getModelObjectTarget<ScheduleDay>(OS_Schedule_RulesetFields::SummerDesignDayScheduleName);
    if (result){
      return *result;
    }
    return this->defaultDaySchedule();
  }

  bool ScheduleRuleset_Impl::isSummerDesignDayScheduleDefaulted() const
  {
    return this->isEmpty(OS_Schedule_RulesetFields::SummerDesignDayScheduleName);
  }

  ScheduleDay ScheduleRuleset_Impl::winterDesignDaySchedule() const
  {
    OptionalScheduleDay result = getObject<ScheduleRuleset>().getModelObjectTarget<ScheduleDay>(OS_Schedule_RulesetFields::WinterDesignDayScheduleName);
    if (result){
      return *result;
    }
    return this->defaultDaySchedule();
  }

  bool ScheduleRuleset_Impl::isWinterDesignDayScheduleDefaulted() const
  {
    return this->isEmpty(OS_Schedule_RulesetFields::WinterDesignDayScheduleName);
  }

  bool ScheduleRuleset_Impl::setSummerDesignDaySchedule(const ScheduleDay& schedule)
  {
    if (OptionalScheduleTypeLimits candidateLimits = schedule.scheduleTypeLimits()) {
      if (OptionalScheduleTypeLimits parentLimits = scheduleTypeLimits()) {
        if (!isCompatible(*parentLimits,*candidateLimits)) {
          return false;
        }
      }
      else {
        return false;
      }
    }

    if (!this->isSummerDesignDayScheduleDefaulted()){
      ScheduleDay summerDesignDaySchedule = this->summerDesignDaySchedule();
      summerDesignDaySchedule.remove();
    }
    ModelObject clone = schedule.clone();
    bool result = setPointer(OS_Schedule_RulesetFields::SummerDesignDayScheduleName, clone.handle());
    OS_ASSERT(result);
    if (OptionalScheduleTypeLimits limits = scheduleTypeLimits()) {
      result = summerDesignDaySchedule().setScheduleTypeLimits(*limits);
      OS_ASSERT(result);
    }
    return result;
  }

  void ScheduleRuleset_Impl::resetSummerDesignDaySchedule()
  {
    if (!this->isSummerDesignDayScheduleDefaulted()){
      ScheduleDay summerDesignDaySchedule = this->summerDesignDaySchedule();
      summerDesignDaySchedule.remove();
    }
    bool test = this->setString(OS_Schedule_RulesetFields::SummerDesignDayScheduleName, "");
    OS_ASSERT(test);
  }

  bool ScheduleRuleset_Impl::setWinterDesignDaySchedule(const ScheduleDay& schedule)
  {
    if (OptionalScheduleTypeLimits candidateLimits = schedule.scheduleTypeLimits()) {
      if (OptionalScheduleTypeLimits parentLimits = scheduleTypeLimits()) {
        if (!isCompatible(*parentLimits,*candidateLimits)) {
          return false;
        }
      }
      else {
        return false;
      }
    }

    if (!this->isWinterDesignDayScheduleDefaulted()){
      ScheduleDay winterDesignDaySchedule = this->winterDesignDaySchedule();
      winterDesignDaySchedule.remove();
    }
    ModelObject clone = schedule.clone();
    bool result = setPointer(OS_Schedule_RulesetFields::WinterDesignDayScheduleName, clone.handle());
    OS_ASSERT(result);
    if (OptionalScheduleTypeLimits limits = scheduleTypeLimits()) {
      result = winterDesignDaySchedule().setScheduleTypeLimits(*limits);
      OS_ASSERT(result);
    }
    return result;
  }

  void ScheduleRuleset_Impl::resetWinterDesignDaySchedule()
  {
    if (!this->isWinterDesignDayScheduleDefaulted()){
      ScheduleDay winterDesignDaySchedule = this->winterDesignDaySchedule();
      winterDesignDaySchedule.remove();
    }
    bool test = this->setString(OS_Schedule_RulesetFields::WinterDesignDayScheduleName, "");
    OS_ASSERT(test);
  }

  struct ScheduleRuleIndexCompare {
    bool operator()(const ScheduleRule& left, const ScheduleRule& right){
      return (left.ruleIndex() < right.ruleIndex());
    }
  };

  std::vector<ScheduleRule> ScheduleRuleset_Impl::scheduleRules() const
  {
    std::vector<ScheduleRule> result = this->getObject<ModelObject>().getModelObjectSources<ScheduleRule>(ScheduleRule::iddObjectType());
    std::sort(result.begin(), result.end(), ScheduleRuleIndexCompare());
    return result;
  }

  bool ScheduleRuleset_Impl::setScheduleRuleIndex(ScheduleRule& scheduleRule, unsigned index)
  {
    std::vector<ScheduleRule> scheduleRules = this->scheduleRules();
    unsigned N = scheduleRules.size();

    boost::optional<unsigned> indexFound;
    for (unsigned i = 0; i < N; ++i){
      if (scheduleRule.handle() == scheduleRules[i].handle()){
        indexFound = i;

        if (indexFound == index){
          if(scheduleRule.ruleIndex() == (int)index){
            return true;
          }
        }
        // set index for target rule, when called in constructor this rule has no index
        scheduleRule.setRuleIndex(index);
        break;
      }
    }

    if (!indexFound){
      return false;
    }

    int increment = 0;
    for (unsigned i = 0; i < N; ++i){

      // have to move remaining rules up to fill hole
      if (i == indexFound){
        increment--;
        continue;
      }

      // have to move this and remaining cells down
      if (i == index){
        if (index < indexFound){
          increment++;
        }
      }

      if (increment != 0){
        OS_ASSERT(i + increment >= 0);
        unsigned newIndex = i + increment;
        OS_ASSERT(newIndex < N);
        scheduleRules[i].setRuleIndex(newIndex);
      }

      // have to move this and remaining cells down
      if (i == index){
        if (index > indexFound){
          increment++;
        }
      }

    }

    return true;
  }

  std::vector<int> ScheduleRuleset_Impl::getActiveRuleIndices(const openstudio::Date& startDate, const openstudio::Date& endDate) const
  {

    // need to check or adjust assumed base year on input date?

    // populate dates to check
    std::vector<openstudio::Date> dates;
    if (startDate <= endDate){
      openstudio::Date date = startDate;
      while (date <= endDate){
        dates.push_back(date);
        date += Time(1);
      }
    }else{
      openstudio::Date date = startDate;
      openstudio::Date endOfYear(MonthOfYear::Dec, 31);
      while (date <= endOfYear){
        dates.push_back(date);
        date += Time(1);
      }
      date = openstudio::Date(MonthOfYear::Jan, 1);
      while (date <= endDate){
        dates.push_back(date);
        date += Time(1);
      }
    }

    unsigned numDates = dates.size();

    // check if each rule contains each date
    std::vector<ScheduleRule> scheduleRules = this->scheduleRules();
    unsigned numRules = scheduleRules.size();
    std::vector<std::vector<bool> > test;
    for(unsigned i = 0; i < numRules; ++i){
      test.push_back(scheduleRules[i].containsDates(dates));
    }

    // now create result
    std::vector<int> result(numDates, -1);
    for(unsigned j = 0; j < numDates; ++j){
      for(unsigned i = 0; i < numRules; ++i){
        if (test[i][j]){
          result[j] = i;
          break;
        }
      }
    }

    return result;
  }

  std::vector<ScheduleDay> ScheduleRuleset_Impl::getDaySchedules(const openstudio::Date& startDate, const openstudio::Date& endDate) const
  {
    std::vector<ScheduleDay> result;
    ScheduleDay defaultDaySchedule = this->defaultDaySchedule();
    std::vector<ScheduleRule> scheduleRules = this->scheduleRules();
    std::vector<int> activeRuleIndices = this->getActiveRuleIndices(startDate, endDate);
    for (int i : activeRuleIndices){
      if (i == -1){
        result.push_back(defaultDaySchedule);
      }else{
        result.push_back(scheduleRules[i].daySchedule());
      }
    }

    return result;
  }

  bool ScheduleRuleset_Impl::moveToEnd(ScheduleRule& scheduleRule)
  {
    std::vector<ScheduleRule> scheduleRules = this->scheduleRules();
    return setScheduleRuleIndex(scheduleRule, scheduleRules.size() - 1);
  }

  void ScheduleRuleset_Impl::ensureNoLeapDays()
  {
    for (ScheduleRule scheduleRule : this->scheduleRules()){
      scheduleRule.ensureNoLeapDays();
    }
  }

  boost::optional<ScheduleDay> ScheduleRuleset_Impl::optionalDefaultDaySchedule() const {
    return getObject<ScheduleRuleset>().getModelObjectTarget<ScheduleDay>(OS_Schedule_RulesetFields::DefaultDayScheduleName);
  }

} // detail

ScheduleRuleset::ScheduleRuleset(const Model& model)
  : Schedule(ScheduleRuleset::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ScheduleRuleset_Impl>());
  ScheduleDay defaultDaySchedule(model);
  getImpl<detail::ScheduleRuleset_Impl>()->setPointer(OS_Schedule_RulesetFields::DefaultDayScheduleName, defaultDaySchedule.handle());
}

ScheduleRuleset::ScheduleRuleset(const Model& model, double value)
  : Schedule(ScheduleRuleset::iddObjectType(), model)
{
  OS_ASSERT(getImpl<detail::ScheduleRuleset_Impl>());
  ScheduleDay defaultDaySchedule(model, value);
  getImpl<detail::ScheduleRuleset_Impl>()->setPointer(OS_Schedule_RulesetFields::DefaultDayScheduleName, defaultDaySchedule.handle());
}

IddObjectType ScheduleRuleset::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Schedule_Ruleset);
  return result;
}

/// @cond
ScheduleRuleset::ScheduleRuleset(std::shared_ptr<detail::ScheduleRuleset_Impl> impl)
  : Schedule(std::move(impl))
{}
/// @endcond

ScheduleDay ScheduleRuleset::defaultDaySchedule() const
{
  return getImpl<detail::ScheduleRuleset_Impl>()->defaultDaySchedule();
}

ScheduleDay ScheduleRuleset::summerDesignDaySchedule() const
{
  return getImpl<detail::ScheduleRuleset_Impl>()->summerDesignDaySchedule();
}

bool ScheduleRuleset::isSummerDesignDayScheduleDefaulted() const
{
  return getImpl<detail::ScheduleRuleset_Impl>()->isSummerDesignDayScheduleDefaulted();
}

ScheduleDay ScheduleRuleset::winterDesignDaySchedule() const
{
  return getImpl<detail::ScheduleRuleset_Impl>()->winterDesignDaySchedule();
}

bool ScheduleRuleset::isWinterDesignDayScheduleDefaulted() const
{
  return getImpl<detail::ScheduleRuleset_Impl>()->isWinterDesignDayScheduleDefaulted();
}

bool ScheduleRuleset::setSummerDesignDaySchedule(const ScheduleDay& schedule)
{
  return getImpl<detail::ScheduleRuleset_Impl>()->setSummerDesignDaySchedule(schedule);
}

void ScheduleRuleset::resetSummerDesignDaySchedule()
{
  return getImpl<detail::ScheduleRuleset_Impl>()->resetSummerDesignDaySchedule();
}

bool ScheduleRuleset::setWinterDesignDaySchedule(const ScheduleDay& schedule)
{
  return getImpl<detail::ScheduleRuleset_Impl>()->setWinterDesignDaySchedule(schedule);
}

void ScheduleRuleset::resetWinterDesignDaySchedule()
{
  return getImpl<detail::ScheduleRuleset_Impl>()->resetWinterDesignDaySchedule();
}

std::vector<ScheduleRule> ScheduleRuleset::scheduleRules() const
{
  return getImpl<detail::ScheduleRuleset_Impl>()->scheduleRules();
}

bool ScheduleRuleset::setScheduleRuleIndex(ScheduleRule& scheduleRule, unsigned index)
{
  return getImpl<detail::ScheduleRuleset_Impl>()->setScheduleRuleIndex(scheduleRule, index);
}

std::vector<int> ScheduleRuleset::getActiveRuleIndices(const openstudio::Date& startDate, const openstudio::Date& endDate) const
{
  return getImpl<detail::ScheduleRuleset_Impl>()->getActiveRuleIndices(startDate, endDate);
}

std::vector<ScheduleDay> ScheduleRuleset::getDaySchedules(const openstudio::Date& startDate, const openstudio::Date& endDate) const
{
  return getImpl<detail::ScheduleRuleset_Impl>()->getDaySchedules(startDate, endDate);
}

bool ScheduleRuleset::moveToEnd(ScheduleRule& scheduleRule)
{
  return getImpl<detail::ScheduleRuleset_Impl>()->moveToEnd(scheduleRule);
}

} // model
} // openstudio

