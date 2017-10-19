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

#include "RunPeriodControlSpecialDays.hpp"
#include "RunPeriodControlSpecialDays_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "YearDescription.hpp"
#include "YearDescription_Impl.hpp"

#include <utilities/idd/OS_RunPeriodControl_SpecialDays_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/time/Date.hpp"
#include "../utilities/core/Assert.hpp"

#include <boost/regex.hpp>

namespace openstudio {
namespace model  {

namespace detail {

  RunPeriodControlSpecialDays_Impl::RunPeriodControlSpecialDays_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RunPeriodControlSpecialDays::iddObjectType());
  }

  RunPeriodControlSpecialDays_Impl::RunPeriodControlSpecialDays_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RunPeriodControlSpecialDays::iddObjectType());
  }

  RunPeriodControlSpecialDays_Impl::RunPeriodControlSpecialDays_Impl(const RunPeriodControlSpecialDays_Impl& other,Model_Impl* model,bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
  }

  Date RunPeriodControlSpecialDays_Impl::startDate() const
  {
    Date result;
    YearDescription yd = this->model().getUniqueModelObject<YearDescription>();
    boost::optional<std::string> text = getString(OS_RunPeriodControl_SpecialDaysFields::StartDate);
    OS_ASSERT(text);
    return getDate(*text);
  }

  unsigned RunPeriodControlSpecialDays_Impl::duration() const
  {
    boost::optional<int> result = getInt(OS_RunPeriodControl_SpecialDaysFields::Duration, true);
    OS_ASSERT(result);
    return *result;
  }

  std::string RunPeriodControlSpecialDays_Impl::specialDayType() const
  {
    boost::optional<std::string> result = getString(OS_RunPeriodControl_SpecialDaysFields::SpecialDayType, true);
    OS_ASSERT(result);
    return *result;
  }

  bool RunPeriodControlSpecialDays_Impl::setStartDate(const std::string& startDate)
  {
    bool result = false;
    try{
      getDate(startDate);
      result = setString(OS_RunPeriodControl_SpecialDaysFields::StartDate, startDate);
    }catch(...){
    }
    return result;
  }

  bool RunPeriodControlSpecialDays_Impl::setStartDate(const openstudio::MonthOfYear& monthOfYear, unsigned day)
  {
    std::stringstream ss;
    ss << monthOfYear.value() << "/" << day;
    return setString(OS_RunPeriodControl_SpecialDaysFields::StartDate, ss.str());
  }

  bool RunPeriodControlSpecialDays_Impl::setStartDate(const openstudio::NthDayOfWeekInMonth& nth, const openstudio::DayOfWeek& dayOfWeek, const openstudio::MonthOfYear& monthOfYear)
  {
    std::stringstream ss;
    switch (nth.value()){
      case NthDayOfWeekInMonth::first:
        ss << "1st " << dayOfWeek.valueName() << " in " << monthOfYear.valueName();
        break;
      case NthDayOfWeekInMonth::second:
        ss << "2nd " << dayOfWeek.valueName() << " in " << monthOfYear.valueName();
        break;
      case NthDayOfWeekInMonth::third:
        ss << "3rd " << dayOfWeek.valueName() << " in " << monthOfYear.valueName();
        break;
      case NthDayOfWeekInMonth::fourth:
        ss << "4th " << dayOfWeek.valueName() << " in " << monthOfYear.valueName();
        break;
      case NthDayOfWeekInMonth::fifth:
        ss << "5th " << dayOfWeek.valueName() << " in " << monthOfYear.valueName();
        break;
      default:
        OS_ASSERT(false);
    }

    return setString(OS_RunPeriodControl_SpecialDaysFields::StartDate, ss.str());
  }

  bool RunPeriodControlSpecialDays_Impl::setDuration(unsigned duration)
  {
    return setUnsigned(OS_RunPeriodControl_SpecialDaysFields::Duration, duration);
  }

  bool RunPeriodControlSpecialDays_Impl::setSpecialDayType(const std::string& specialDayType)
  {
    return setString(OS_RunPeriodControl_SpecialDaysFields::SpecialDayType, specialDayType);
  }

  void RunPeriodControlSpecialDays_Impl::ensureNoLeapDays()
  {
    LOG(Warn, "Ensure no leap days is not yet implemented for run control special days");
  }

  // return the parent object in the hierarchy
  boost::optional<ParentObject> RunPeriodControlSpecialDays_Impl::parent() const
  {
    boost::optional<ParentObject> result(model().getOptionalUniqueModelObject<YearDescription>());
    return result;
  }

  // set the parent, child may have to call methods on the parent
  bool RunPeriodControlSpecialDays_Impl::setParent(ParentObject& newParent)
  {
    if (newParent.optionalCast<YearDescription>() && (newParent.model() == model())) {
      return true;
    }
    return false;
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& RunPeriodControlSpecialDays_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RunPeriodControlSpecialDays_Impl::iddObjectType() const
  {
    return RunPeriodControlSpecialDays::iddObjectType();
  }

  openstudio::Date RunPeriodControlSpecialDays_Impl::getDate(const std::string& text) const
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

} // detail

RunPeriodControlSpecialDays::RunPeriodControlSpecialDays(const std::string& startDate, Model& model)
  : ModelObject(RunPeriodControlSpecialDays::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RunPeriodControlSpecialDays_Impl>());
  bool test = getImpl<detail::RunPeriodControlSpecialDays_Impl>()->setStartDate(startDate);
  if (!test){
    LOG_AND_THROW("'" << startDate << "' is not correctly formatted");
  }
}

RunPeriodControlSpecialDays::RunPeriodControlSpecialDays(const openstudio::MonthOfYear& monthOfYear, unsigned day, Model& model)
  : ModelObject(RunPeriodControlSpecialDays::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RunPeriodControlSpecialDays_Impl>());
  getImpl<detail::RunPeriodControlSpecialDays_Impl>()->setStartDate(monthOfYear, day);
  Date test = this->startDate();
}

RunPeriodControlSpecialDays::RunPeriodControlSpecialDays(const openstudio::NthDayOfWeekInMonth& nth, const openstudio::DayOfWeek& dayOfWeek, const openstudio::MonthOfYear& monthOfYear, Model& model)
  : ModelObject(RunPeriodControlSpecialDays::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RunPeriodControlSpecialDays_Impl>());
  getImpl<detail::RunPeriodControlSpecialDays_Impl>()->setStartDate(nth, dayOfWeek, monthOfYear);
  Date test = this->startDate();
}

Date RunPeriodControlSpecialDays::startDate() const
{
  return getImpl<detail::RunPeriodControlSpecialDays_Impl>()->startDate();
}

unsigned RunPeriodControlSpecialDays::duration() const
{
  return getImpl<detail::RunPeriodControlSpecialDays_Impl>()->duration();
}

std::string RunPeriodControlSpecialDays::specialDayType() const
{
  return getImpl<detail::RunPeriodControlSpecialDays_Impl>()->specialDayType();
}

bool RunPeriodControlSpecialDays::setStartDate(const std::string& startDate)
{
  return getImpl<detail::RunPeriodControlSpecialDays_Impl>()->setStartDate(startDate);
}

bool RunPeriodControlSpecialDays::setStartDate(const openstudio::MonthOfYear& monthOfYear, unsigned day)
{
  return getImpl<detail::RunPeriodControlSpecialDays_Impl>()->setStartDate(monthOfYear, day);
}

bool RunPeriodControlSpecialDays::setStartDate(const openstudio::NthDayOfWeekInMonth& nth, const openstudio::DayOfWeek& dayOfWeek, const openstudio::MonthOfYear& monthOfYear)
{
  return getImpl<detail::RunPeriodControlSpecialDays_Impl>()->setStartDate(nth, dayOfWeek, monthOfYear);
}

bool RunPeriodControlSpecialDays::setDuration(unsigned duration)
{
  return getImpl<detail::RunPeriodControlSpecialDays_Impl>()->setDuration(duration);
}

bool RunPeriodControlSpecialDays::setSpecialDayType(const std::string& specialDayType)
{
  return getImpl<detail::RunPeriodControlSpecialDays_Impl>()->setSpecialDayType(specialDayType);
}

void RunPeriodControlSpecialDays::ensureNoLeapDays()
{
  getImpl<detail::RunPeriodControlSpecialDays_Impl>()->ensureNoLeapDays();
}

RunPeriodControlSpecialDays::RunPeriodControlSpecialDays(std::shared_ptr<detail::RunPeriodControlSpecialDays_Impl> impl)
  : ModelObject(std::move(impl))
{}

IddObjectType RunPeriodControlSpecialDays::iddObjectType() {
  IddObjectType result(IddObjectType::OS_RunPeriodControl_SpecialDays);
  return result;
}

} // model
} // openstudio
