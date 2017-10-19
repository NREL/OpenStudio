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

#include "YearDescription.hpp"
#include "YearDescription_Impl.hpp"
#include "RunPeriod.hpp"
#include "RunPeriod_Impl.hpp"
#include "RunPeriodControlDaylightSavingTime.hpp"
#include "RunPeriodControlDaylightSavingTime_Impl.hpp"
#include "RunPeriodControlSpecialDays.hpp"
#include "RunPeriodControlSpecialDays_Impl.hpp"
#include "SizingPeriod.hpp"
#include "SizingPeriod_Impl.hpp"
#include "ScheduleBase.hpp"
#include "ScheduleBase_Impl.hpp"
#include "ScheduleRule.hpp"
#include "ScheduleRule_Impl.hpp"
#include "LightingDesignDay.hpp"
#include "LightingDesignDay_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_YearDescription_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/time/Date.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  YearDescription_Impl::YearDescription_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == YearDescription::iddObjectType());
  }

  YearDescription_Impl::YearDescription_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == YearDescription::iddObjectType());
  }

  YearDescription_Impl::YearDescription_Impl(const YearDescription_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& YearDescription_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType YearDescription_Impl::iddObjectType() const {
    return YearDescription::iddObjectType();
  }

  std::vector<ModelObject> YearDescription_Impl::children() const
  {
    std::vector<ModelObject> result;
    Model model = this->model();

    boost::optional<RunPeriodControlDaylightSavingTime> dst = model.getOptionalUniqueModelObject<RunPeriodControlDaylightSavingTime>();
    if (dst){
      result.push_back(*dst);
    }

    for (RunPeriodControlSpecialDays day : model.getConcreteModelObjects<RunPeriodControlSpecialDays>()){
      result.push_back(day);
    }

    return result;
  }

  std::vector<IddObjectType> YearDescription_Impl::allowableChildTypes() const
  {
    IddObjectTypeVector result;
    result.push_back(RunPeriodControlDaylightSavingTime::iddObjectType());
    result.push_back(RunPeriodControlSpecialDays::iddObjectType());
    return result;
  }

  boost::optional<int> YearDescription_Impl::calendarYear() const {
    return getInt(OS_YearDescriptionFields::CalendarYear,true);
  }

  std::string YearDescription_Impl::dayofWeekforStartDay() const {

    boost::optional<int> calendarYear = this->calendarYear();
    if (calendarYear){
      openstudio::Date jan1(MonthOfYear::Jan, 1, *calendarYear);
      std::string result = jan1.dayOfWeek().valueName();
      return result;
    }

    boost::optional<std::string> value = getString(OS_YearDescriptionFields::DayofWeekforStartDay,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool YearDescription_Impl::isDayofWeekforStartDayDefaulted() const {
    return isEmpty(OS_YearDescriptionFields::DayofWeekforStartDay);
  }

  bool YearDescription_Impl::isLeapYear() const {

    boost::optional<int> calendarYear = this->calendarYear();
    if (calendarYear){
      openstudio::Date jan1(MonthOfYear::Jan, 1, *calendarYear);
      bool result = jan1.isLeapYear();
      return result;
    }

    boost::optional<std::string> value = getString(OS_YearDescriptionFields::IsLeapYear,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool YearDescription_Impl::isIsLeapYearDefaulted() const {
    return isEmpty(OS_YearDescriptionFields::IsLeapYear);
  }

  void YearDescription_Impl::setCalendarYear(boost::optional<int> calendarYear) {
    bool wasLeapYear = this->isLeapYear();

    bool result = false;
    if (calendarYear) {
      result = setInt(OS_YearDescriptionFields::CalendarYear, calendarYear.get());
      this->resetDayofWeekforStartDay();
      this->resetIsLeapYear();
    } else {
      result = setString(OS_YearDescriptionFields::CalendarYear, "");
    }
    OS_ASSERT(result);

    bool isLeapYear = this->isLeapYear();
    updateModelLeapYear(wasLeapYear, isLeapYear);
  }

  void YearDescription_Impl::resetCalendarYear() {
    bool wasLeapYear = this->isLeapYear();

    bool result = setString(OS_YearDescriptionFields::CalendarYear, "");
    OS_ASSERT(result);

    bool isLeapYear = this->isLeapYear();
    updateModelLeapYear(wasLeapYear, isLeapYear);
  }

  bool YearDescription_Impl::setDayofWeekforStartDay(std::string dayofWeekforStartDay) {
    bool result = false;
    if (!this->calendarYear()){
      result = setString(OS_YearDescriptionFields::DayofWeekforStartDay, dayofWeekforStartDay);
    }
    return result;
  }

  void YearDescription_Impl::resetDayofWeekforStartDay() {
    bool result = setString(OS_YearDescriptionFields::DayofWeekforStartDay, "");
    OS_ASSERT(result);
  }

  bool YearDescription_Impl::setIsLeapYear(bool isLeapYear) {
    bool result = false;
    bool wasLeapYear = this->isLeapYear();

    if (!this->calendarYear()){
      if (isLeapYear) {
        result = setString(OS_YearDescriptionFields::IsLeapYear, "Yes");
      } else {
        result = setString(OS_YearDescriptionFields::IsLeapYear, "No");
      }
    }

    if (result){
      updateModelLeapYear(wasLeapYear, isLeapYear);
    }

    return result;
  }

  void YearDescription_Impl::resetIsLeapYear() {
    bool wasLeapYear = this->isLeapYear();

    bool result = setString(OS_YearDescriptionFields::IsLeapYear, "");
    OS_ASSERT(result);

    bool isLeapYear = this->isLeapYear();
    updateModelLeapYear(wasLeapYear, isLeapYear);
  }

  int YearDescription_Impl::assumedYear() const
  {
    boost::optional<int> calendarYear = this->calendarYear();
    if (calendarYear){
      return *calendarYear;
    }

    openstudio::YearDescription yd;

    yd.isLeapYear = this->isLeapYear();

    std::string dayofWeekforStartDay = this->dayofWeekforStartDay();
    if (!dayofWeekforStartDay.empty()){
      try{
        openstudio::DayOfWeek dow(dayofWeekforStartDay);
        yd.yearStartsOnDayOfWeek = dow;
      }catch(const std::exception& ){
        LOG(Error, "'" << dayofWeekforStartDay << "' is not yet a supported option for YearDescription");
      }
    }

    return yd.assumedYear();
  }

  openstudio::Date YearDescription_Impl::makeDate(openstudio::MonthOfYear monthOfYear, unsigned dayOfMonth)
  {
    boost::optional<int> calendarYear = this->calendarYear();
    if (calendarYear){
      return openstudio::Date(monthOfYear, dayOfMonth, *calendarYear);
    }

    openstudio::YearDescription yd;

    yd.isLeapYear = this->isLeapYear();

    std::string dayofWeekforStartDay = this->dayofWeekforStartDay();
    if (!dayofWeekforStartDay.empty()){
      if (istringEqual(dayofWeekforStartDay, "UseWeatherFile")){
        LOG(Info, "'UseWeatherFile' is not yet a supported option for YearDescription");
      }else{
        openstudio::DayOfWeek dow(dayofWeekforStartDay);
        yd.yearStartsOnDayOfWeek = dow;
      }
    }

    return openstudio::Date(monthOfYear, dayOfMonth, yd);
  }

  openstudio::Date YearDescription_Impl::makeDate(unsigned monthOfYear, unsigned dayOfMonth)
  {
    return makeDate(openstudio::MonthOfYear(monthOfYear), dayOfMonth);
  }

  openstudio::Date YearDescription_Impl::makeDate(openstudio::NthDayOfWeekInMonth n, openstudio::DayOfWeek dayOfWeek, openstudio::MonthOfYear monthOfYear)
  {
    boost::optional<int> year = this->calendarYear();
    if (!year){
      year = this->assumedYear();
    }

    return openstudio::Date::fromNthDayOfMonth(n, dayOfWeek, monthOfYear, *year);
  }

  openstudio::Date YearDescription_Impl::makeDate(unsigned dayOfYear)
  {
    boost::optional<int> year = this->calendarYear();
    if (!year){
      year = this->assumedYear();
    }

    return openstudio::Date::fromDayOfYear(dayOfYear, *year);
  }

  void YearDescription_Impl::updateModelLeapYear(bool wasLeapYear, bool isLeapYear)
  {
    if (wasLeapYear == isLeapYear){
      return;
    }

    if (!wasLeapYear && isLeapYear){
      return;
    }

    model::Model model = this->model();
    if (wasLeapYear && !isLeapYear){
      for (RunPeriod runPeriod : model.getConcreteModelObjects<RunPeriod>()){
        runPeriod.ensureNoLeapDays();
      }

      for (RunPeriodControlDaylightSavingTime runPeriodControlDaylightSavingTime : model.getConcreteModelObjects<RunPeriodControlDaylightSavingTime>()){
        runPeriodControlDaylightSavingTime.ensureNoLeapDays();
      }

      for (RunPeriodControlSpecialDays runPeriodControlSpecialDays : model.getConcreteModelObjects<RunPeriodControlSpecialDays>()){
        runPeriodControlSpecialDays.ensureNoLeapDays();
      }

      for (SizingPeriod sizingPeriod : model.getModelObjects<SizingPeriod>()){
        sizingPeriod.ensureNoLeapDays();
      }

      for (ScheduleBase scheduleBase : model.getModelObjects<ScheduleBase>()){
        scheduleBase.ensureNoLeapDays();
      }

      for (ScheduleRule scheduleRule : model.getModelObjects<ScheduleRule>()){
        scheduleRule.ensureNoLeapDays();
      }

      for (LightingDesignDay lightingDesignDay : model.getConcreteModelObjects<LightingDesignDay>()){
        lightingDesignDay.ensureNoLeapDays();
      }
    }
  }

} // detail

IddObjectType YearDescription::iddObjectType() {
  IddObjectType result(IddObjectType::OS_YearDescription);
  return result;
}

std::vector<std::string> YearDescription::validDayofWeekforStartDayValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_YearDescriptionFields::DayofWeekforStartDay);
}

boost::optional<int> YearDescription::calendarYear() const {
  return getImpl<detail::YearDescription_Impl>()->calendarYear();
}

std::string YearDescription::dayofWeekforStartDay() const {
  return getImpl<detail::YearDescription_Impl>()->dayofWeekforStartDay();
}

bool YearDescription::isDayofWeekforStartDayDefaulted() const {
  return getImpl<detail::YearDescription_Impl>()->isDayofWeekforStartDayDefaulted();
}

bool YearDescription::isLeapYear() const {
  return getImpl<detail::YearDescription_Impl>()->isLeapYear();
}

bool YearDescription::isIsLeapYearDefaulted() const {
  return getImpl<detail::YearDescription_Impl>()->isIsLeapYearDefaulted();
}

void YearDescription::setCalendarYear(int calendarYear) {
  getImpl<detail::YearDescription_Impl>()->setCalendarYear(calendarYear);
}

void YearDescription::resetCalendarYear() {
  getImpl<detail::YearDescription_Impl>()->resetCalendarYear();
}

bool YearDescription::setDayofWeekforStartDay(std::string dayofWeekforStartDay) {
  return getImpl<detail::YearDescription_Impl>()->setDayofWeekforStartDay(dayofWeekforStartDay);
}

void YearDescription::resetDayofWeekforStartDay() {
  getImpl<detail::YearDescription_Impl>()->resetDayofWeekforStartDay();
}

bool YearDescription::setIsLeapYear(bool isLeapYear) {
  return getImpl<detail::YearDescription_Impl>()->setIsLeapYear(isLeapYear);
}

void YearDescription::resetIsLeapYear() {
  getImpl<detail::YearDescription_Impl>()->resetIsLeapYear();
}

int YearDescription::assumedYear() const {
  return getImpl<detail::YearDescription_Impl>()->assumedYear();
}

openstudio::Date YearDescription::makeDate(openstudio::MonthOfYear monthOfYear, unsigned dayOfMonth)
{
  return getImpl<detail::YearDescription_Impl>()->makeDate(monthOfYear, dayOfMonth);
}

openstudio::Date YearDescription::makeDate(unsigned monthOfYear, unsigned dayOfMonth)
{
  return getImpl<detail::YearDescription_Impl>()->makeDate(monthOfYear, dayOfMonth);
}

openstudio::Date YearDescription::makeDate(openstudio::NthDayOfWeekInMonth n, openstudio::DayOfWeek dayOfWeek, openstudio::MonthOfYear monthOfYear)
{
  return getImpl<detail::YearDescription_Impl>()->makeDate(n, dayOfWeek, monthOfYear);
}

openstudio::Date YearDescription::makeDate(unsigned dayOfYear)
{
  return getImpl<detail::YearDescription_Impl>()->makeDate(dayOfYear);
}

/// @cond
YearDescription::YearDescription(std::shared_ptr<detail::YearDescription_Impl> impl)
  : ParentObject(std::move(impl))
{}
YearDescription::YearDescription(Model& model)
  : ParentObject(YearDescription::iddObjectType(),model)
{}

/// @endcond


} // model
} // openstudio

