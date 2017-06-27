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

#include "RunPeriodControlDaylightSavingTime.hpp"
#include "RunPeriodControlDaylightSavingTime_Impl.hpp"
#include "YearDescription.hpp"
#include "YearDescription_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/OS_RunPeriodControl_DaylightSavingTime_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/time/Date.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  RunPeriodControlDaylightSavingTime_Impl::RunPeriodControlDaylightSavingTime_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RunPeriodControlDaylightSavingTime::iddObjectType());

    if (isEmpty(OS_RunPeriodControl_DaylightSavingTimeFields::StartDate)){
      bool result = setString(OS_RunPeriodControl_DaylightSavingTimeFields::StartDate, "2nd Sunday in March");
      OS_ASSERT(result);
    }
    if (isEmpty(OS_RunPeriodControl_DaylightSavingTimeFields::EndDate)){
      bool result = setString(OS_RunPeriodControl_DaylightSavingTimeFields::EndDate, "1st Sunday in November");
      OS_ASSERT(result);
    }
  }

  RunPeriodControlDaylightSavingTime_Impl::RunPeriodControlDaylightSavingTime_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RunPeriodControlDaylightSavingTime::iddObjectType());
  }

  RunPeriodControlDaylightSavingTime_Impl::RunPeriodControlDaylightSavingTime_Impl(const RunPeriodControlDaylightSavingTime_Impl& other,
                                                     Model_Impl* model,
                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  boost::optional<ParentObject> RunPeriodControlDaylightSavingTime_Impl::parent() const
  {
    boost::optional<ParentObject> result(model().getOptionalUniqueModelObject<YearDescription>());
    return result;
  }

  bool RunPeriodControlDaylightSavingTime_Impl::setParent(ParentObject& newParent)
  {
    if (newParent.optionalCast<YearDescription>() && (newParent.model() == model())) {
      return true;
    }
    return false;
  }

  const std::vector<std::string>& RunPeriodControlDaylightSavingTime_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RunPeriodControlDaylightSavingTime_Impl::iddObjectType() const {
    return RunPeriodControlDaylightSavingTime::iddObjectType();
  }

  openstudio::Date RunPeriodControlDaylightSavingTime_Impl::startDate() const {
    boost::optional<std::string> text = getString(OS_RunPeriodControl_DaylightSavingTimeFields::StartDate);
    OS_ASSERT(text);
    return getDate(*text);
  }

  boost::optional<openstudio::NthDayOfWeekInMonth> RunPeriodControlDaylightSavingTime_Impl::startNthDayOfWeekInMonth() const {
    boost::optional<std::string> text = getString(OS_RunPeriodControl_DaylightSavingTimeFields::StartDate);
    OS_ASSERT(text);
    return getNthDayOfWeekInMonth(*text);
  }

  openstudio::Date RunPeriodControlDaylightSavingTime_Impl::endDate() const {
    boost::optional<std::string> text = getString(OS_RunPeriodControl_DaylightSavingTimeFields::EndDate);
    OS_ASSERT(text);
    return getDate(*text);
  }

  boost::optional<openstudio::NthDayOfWeekInMonth> RunPeriodControlDaylightSavingTime_Impl::endNthDayOfWeekInMonth() const {
    boost::optional<std::string> text = getString(OS_RunPeriodControl_DaylightSavingTimeFields::EndDate);
    OS_ASSERT(text);
    return getNthDayOfWeekInMonth(*text);
  }

  bool RunPeriodControlDaylightSavingTime_Impl::setStartDate(const std::string& startDate)
  {
    bool result = false;
    try{
      getDate(startDate);
      result = setString(OS_RunPeriodControl_DaylightSavingTimeFields::StartDate, startDate);
    }catch(...){
    }
    return result;
  }

  bool RunPeriodControlDaylightSavingTime_Impl::setStartDate(const openstudio::MonthOfYear& monthOfYear, unsigned day)
  {
    std::stringstream ss;
    ss << monthOfYear.value() << "/" << day;
    return setString(OS_RunPeriodControl_DaylightSavingTimeFields::StartDate, ss.str());
  }

  bool RunPeriodControlDaylightSavingTime_Impl::setStartDate(const openstudio::NthDayOfWeekInMonth& nth, const openstudio::DayOfWeek& dayOfWeek, const openstudio::MonthOfYear& monthOfYear)
  {
    std::stringstream ss;
    switch (nth.value()){
      case NthDayOfWeekInMonth::first:
        ss << "1st " << dayOfWeek.valueName() << " in " << monthOfYear.valueDescription();
        break;
      case NthDayOfWeekInMonth::second:
        ss << "2nd " << dayOfWeek.valueName() << " in " << monthOfYear.valueDescription();
        break;
      case NthDayOfWeekInMonth::third:
        ss << "3rd " << dayOfWeek.valueName() << " in " << monthOfYear.valueDescription();
        break;
      case NthDayOfWeekInMonth::fourth:
        ss << "4th " << dayOfWeek.valueName() << " in " << monthOfYear.valueDescription();
        break;
      case NthDayOfWeekInMonth::fifth:
        ss << "Last " << dayOfWeek.valueName() << " in " << monthOfYear.valueDescription();
        break;
      default:
        OS_ASSERT(false);
    }

    return setString(OS_RunPeriodControl_DaylightSavingTimeFields::StartDate, ss.str());
  }

  bool RunPeriodControlDaylightSavingTime_Impl::setEndDate(const std::string& endDate)
  {
    bool result = false;
    try{
      getDate(endDate);
      result = setString(OS_RunPeriodControl_DaylightSavingTimeFields::EndDate, endDate);
    }catch(...){
    }
    return result;
  }

  bool RunPeriodControlDaylightSavingTime_Impl::setEndDate(const openstudio::MonthOfYear& monthOfYear, unsigned day)
  {
    std::stringstream ss;
    ss << monthOfYear.value() << "/" << day;
    return setString(OS_RunPeriodControl_DaylightSavingTimeFields::EndDate, ss.str());
  }

  bool RunPeriodControlDaylightSavingTime_Impl::setEndDate(const openstudio::NthDayOfWeekInMonth& nth, const openstudio::DayOfWeek& dayOfWeek, const openstudio::MonthOfYear& monthOfYear)
  {
    std::stringstream ss;
    switch (nth.value()){
      case NthDayOfWeekInMonth::first:
        ss << "1st " << dayOfWeek.valueName() << " in " << monthOfYear.valueDescription();
        break;
      case NthDayOfWeekInMonth::second:
        ss << "2nd " << dayOfWeek.valueName() << " in " << monthOfYear.valueDescription();
        break;
      case NthDayOfWeekInMonth::third:
        ss << "3rd " << dayOfWeek.valueName() << " in " << monthOfYear.valueDescription();
        break;
      case NthDayOfWeekInMonth::fourth:
        ss << "4th " << dayOfWeek.valueName() << " in " << monthOfYear.valueDescription();
        break;
      case NthDayOfWeekInMonth::fifth:
        ss << "Last " << dayOfWeek.valueName() << " in " << monthOfYear.valueDescription();
        break;
      default:
        OS_ASSERT(false);
    }

    return setString(OS_RunPeriodControl_DaylightSavingTimeFields::EndDate, ss.str());
  }

  void RunPeriodControlDaylightSavingTime_Impl::ensureNoLeapDays()
  {
    LOG(Warn, "Ensure no leap days is not yet implemented for run control daylight saving time");
  }

  openstudio::Date RunPeriodControlDaylightSavingTime_Impl::getDate(const std::string& text) const
  {
    Date result;
    YearDescription yd = this->model().getUniqueModelObject<YearDescription>();

    /*
     \note  <number>/<number>  (month/day)
     \note  <number> <Month>
     \note  <Month> <number>
     \note <Nth> <Weekday> in <Month)
     \note Last <WeekDay> in <Month>
     \note <Month> can be January, February, March, April, May, June, July, August, September, October, November, December
     \note Months can be the first 3 letters of the month
     \note <Weekday> can be Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday
     \note <Nth> can be 1 or 1st, 2 or 2nd, etc. up to 5(?)
    */
    boost::smatch matches;
    if (boost::regex_search(text, matches, boost::regex("(\\d+)\\s?\\/\\s?(\\d+)"))){

      std::string monthString(matches[1].first, matches[1].second);
      MonthOfYear monthOfYear(boost::lexical_cast<unsigned>(monthString));

      std::string dayOfMonthString(matches[2].first, matches[2].second);
      unsigned dayOfMonth = boost::lexical_cast<unsigned>(dayOfMonthString);

      result = yd.makeDate(monthOfYear, dayOfMonth);
      return result;
    }else if (boost::regex_search(text, matches, boost::regex("(\\d+)\\s+(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec|January|February|March|April|May|June|July|August|September|October|November|December)", boost::regex::icase))){

      std::string dayOfMonthString(matches[1].first, matches[1].second);
      unsigned dayOfMonth = boost::lexical_cast<unsigned>(dayOfMonthString);
      std::string monthString(matches[2].first, matches[2].second);

      result = yd.makeDate(monthOfYear(monthString), dayOfMonth);
      return result;
    }else if (boost::regex_search(text, matches, boost::regex("(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec|January|February|March|April|May|June|July|August|September|October|November|December)\\s+(\\d+)", boost::regex::icase))){

      std::string monthString(matches[1].first, matches[1].second);
      std::string dayOfMonthString(matches[2].first, matches[2].second);
      unsigned dayOfMonth = boost::lexical_cast<unsigned>(dayOfMonthString);

      result = yd.makeDate(monthOfYear(monthString), dayOfMonth);
      return result;
    }else if (boost::regex_search(text, matches, boost::regex("(1|2|3|4|5|1st|2nd|3rd|4th|5th|Last)\\s+(Sun|Mon|Tue|Wed|Thu|Fri|Sat|Sunday|Monday|Tuesday|Wednesday|Thursday|Friday|Saturday)\\s+in\\s+(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec|January|February|March|April|May|June|July|August|September|October|November|December)", boost::regex::icase))){

      std::string nthString(matches[1].first, matches[1].second);
      std::string dayOfWeekString(matches[2].first, matches[2].second);
      std::string monthString(matches[3].first, matches[3].second);

      result = yd.makeDate(nthDayOfWeekInMonth(nthString), dayOfWeek(dayOfWeekString), monthOfYear(monthString));
      return result;
    }

    LOG_AND_THROW("Could not determine date for startDate = '" << text << "'");
    return Date();
  }

  boost::optional<openstudio::NthDayOfWeekInMonth> RunPeriodControlDaylightSavingTime_Impl::getNthDayOfWeekInMonth(const std::string& text) const
  {
    boost::optional<openstudio::NthDayOfWeekInMonth> result;
    boost::smatch matches;
    if (boost::regex_search(text, matches, boost::regex("(1|2|3|4|5|1st|2nd|3rd|4th|5th|Last)\\s+(Sun|Mon|Tue|Wed|Thu|Fri|Sat|Sunday|Monday|Tuesday|Wednesday|Thursday|Friday|Saturday)\\s+in\\s+(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec|January|February|March|April|May|June|July|August|September|October|November|December)", boost::regex::icase))){
      std::string nthString(matches[1].first, matches[1].second);
      result = nthDayOfWeekInMonth(nthString);
    }
    return result;
  }

} // detail

IddObjectType RunPeriodControlDaylightSavingTime::iddObjectType() {
  IddObjectType result(IddObjectType::OS_RunPeriodControl_DaylightSavingTime);
  return result;
}

openstudio::Date RunPeriodControlDaylightSavingTime::startDate() const {
  return getImpl<detail::RunPeriodControlDaylightSavingTime_Impl>()->startDate();
}

boost::optional<openstudio::NthDayOfWeekInMonth> RunPeriodControlDaylightSavingTime::startNthDayOfWeekInMonth() const {
  return getImpl<detail::RunPeriodControlDaylightSavingTime_Impl>()->startNthDayOfWeekInMonth();
}

openstudio::Date RunPeriodControlDaylightSavingTime::endDate() const {
  return getImpl<detail::RunPeriodControlDaylightSavingTime_Impl>()->endDate();
}

boost::optional<openstudio::NthDayOfWeekInMonth> RunPeriodControlDaylightSavingTime::endNthDayOfWeekInMonth() const {
  return getImpl<detail::RunPeriodControlDaylightSavingTime_Impl>()->endNthDayOfWeekInMonth();
}

bool RunPeriodControlDaylightSavingTime::setStartDate(const std::string& startDate){
  return getImpl<detail::RunPeriodControlDaylightSavingTime_Impl>()->setStartDate(startDate);
}

bool RunPeriodControlDaylightSavingTime::setStartDate(const openstudio::MonthOfYear& monthOfYear, unsigned day){
  return getImpl<detail::RunPeriodControlDaylightSavingTime_Impl>()->setStartDate(monthOfYear, day);
}

bool RunPeriodControlDaylightSavingTime::setStartDate(const openstudio::NthDayOfWeekInMonth& nth, const openstudio::DayOfWeek& dayOfWeek, const openstudio::MonthOfYear& monthOfYear){
  return getImpl<detail::RunPeriodControlDaylightSavingTime_Impl>()->setStartDate(nth, dayOfWeek, monthOfYear);
}

bool RunPeriodControlDaylightSavingTime::setEndDate(const std::string& endDate){
  return getImpl<detail::RunPeriodControlDaylightSavingTime_Impl>()->setEndDate(endDate);
}

bool RunPeriodControlDaylightSavingTime::setEndDate(const openstudio::MonthOfYear& monthOfYear, unsigned day){
  return getImpl<detail::RunPeriodControlDaylightSavingTime_Impl>()->setEndDate(monthOfYear, day);
}

bool RunPeriodControlDaylightSavingTime::setEndDate(const openstudio::NthDayOfWeekInMonth& nth, const openstudio::DayOfWeek& dayOfWeek, const openstudio::MonthOfYear& monthOfYear){
  return getImpl<detail::RunPeriodControlDaylightSavingTime_Impl>()->setEndDate(nth, dayOfWeek, monthOfYear);
}

void RunPeriodControlDaylightSavingTime::ensureNoLeapDays()
{
  getImpl<detail::RunPeriodControlDaylightSavingTime_Impl>()->ensureNoLeapDays();
}

/// @cond
RunPeriodControlDaylightSavingTime::RunPeriodControlDaylightSavingTime(std::shared_ptr<detail::RunPeriodControlDaylightSavingTime_Impl> impl)
  : ModelObject(std::move(impl))
{}

RunPeriodControlDaylightSavingTime::RunPeriodControlDaylightSavingTime(Model& model)
  : ModelObject(RunPeriodControlDaylightSavingTime::iddObjectType(),model)
{}

/// @endcond

} // model
} // openstudio

