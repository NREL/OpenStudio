/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "RunPeriod.hpp"
#include "RunPeriod_Impl.hpp"
#include "Model.hpp"

#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"
#include "SkyTemperature.hpp"
#include "SkyTemperature_Impl.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/OS_RunPeriod_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <boost/algorithm/string.hpp>


namespace openstudio {
namespace model  {

namespace detail {

  RunPeriod_Impl::RunPeriod_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ParentObject_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RunPeriod::iddObjectType());
  }

  RunPeriod_Impl::RunPeriod_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                 Model_Impl* model,
                                 bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RunPeriod::iddObjectType());
  }

  RunPeriod_Impl::RunPeriod_Impl(const RunPeriod_Impl& other,Model_Impl* model,bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
  }

  IddObjectType RunPeriod_Impl::iddObjectType() const {
    return RunPeriod::iddObjectType();
  }

  int RunPeriod_Impl::getBeginMonth() const
  {
    //seems safe since this can't be autocalculate
    return getInt(OS_RunPeriodFields::BeginMonth, true).get();
  }

  int RunPeriod_Impl::getBeginDayOfMonth() const
  {
    return getInt(OS_RunPeriodFields::BeginDayofMonth, true).get();
  }

  int RunPeriod_Impl::getEndMonth() const
  {
    return getInt(OS_RunPeriodFields::EndMonth, true).get();
  }

  int RunPeriod_Impl::getEndDayOfMonth() const
  {
    return getInt(OS_RunPeriodFields::EndDayofMonth, true).get();
  }

  bool RunPeriod_Impl::getUseWeatherFileHolidays() const
  {
    std::string temp=getString(OS_RunPeriodFields::UseWeatherFileHolidaysandSpecialDays, true).get();
    boost::to_lower(temp);
    if( temp == "no")
    {
      return false;
    }
    return true;
  }

  bool RunPeriod_Impl::getUseWeatherFileDaylightSavings() const
  {
    std::string temp=getString(OS_RunPeriodFields::UseWeatherFileDaylightSavingPeriod, true).get();
    boost::to_lower(temp);
    if( temp == "no")
    {
      return false;
    }
    return true;
  }

  bool RunPeriod_Impl::getApplyWeekendHolidayRule() const
  {
    std::string temp=getString(OS_RunPeriodFields::ApplyWeekendHolidayRule, true).get();
    boost::to_lower(temp);
    if( temp == "no")
    {
      return false;
    }
    return true;
  }

  bool RunPeriod_Impl::getUseWeatherFileRainInd() const
  {
    std::string temp=getString(OS_RunPeriodFields::UseWeatherFileRainIndicators, true).get();
    boost::to_lower(temp);
    if( temp == "no")
    {
      return false;
    }
    return true;
  }

  bool RunPeriod_Impl::getUseWeatherFileSnowInd() const
  {
    std::string temp=getString(OS_RunPeriodFields::UseWeatherFileSnowIndicators, true).get();
    boost::to_lower(temp);
    if( temp == "no")
    {
      return false;
    }
    return true;
  }

  int RunPeriod_Impl::getNumTimePeriodRepeats() const
  {
    return getInt(OS_RunPeriodFields::NumberofTimesRunperiodtobeRepeated, true).get();
  }

  void RunPeriod_Impl::setBeginMonth(int month)
  {
    setInt(OS_RunPeriodFields::BeginMonth,month);
  }

  void RunPeriod_Impl::setBeginDayOfMonth(int day)
  {
    setInt(OS_RunPeriodFields::BeginDayofMonth,day);
  }

  void RunPeriod_Impl::setEndMonth(int month)
  {
    setInt(OS_RunPeriodFields::EndMonth,month);
  }

  void RunPeriod_Impl::setEndDayOfMonth(int day)
  {
    setInt(OS_RunPeriodFields::EndDayofMonth,day);
  }

  void RunPeriod_Impl::setUseWeatherFileHolidays(bool use)
  {
    if(use)
    {
      setString(OS_RunPeriodFields::UseWeatherFileHolidaysandSpecialDays,"Yes");
      return;
    }
    setString(OS_RunPeriodFields::UseWeatherFileHolidaysandSpecialDays,"No");
  }
  void RunPeriod_Impl::setUseWeatherFileDaylightSavings(bool use)
  {
    if(use)
    {
      setString(OS_RunPeriodFields::UseWeatherFileDaylightSavingPeriod,"Yes");
      return;
    }
    setString(OS_RunPeriodFields::UseWeatherFileDaylightSavingPeriod,"No");
  }
  void RunPeriod_Impl::setApplyWeekendHolidayRule(bool apply)
  {
    if(apply)
    {
      setString(OS_RunPeriodFields::ApplyWeekendHolidayRule,"Yes");
      return;
    }
    setString(OS_RunPeriodFields::ApplyWeekendHolidayRule,"No");
  }
  void RunPeriod_Impl::setUseWeatherFileRainInd(bool rainInd)
  {
    if(rainInd)
    {
      setString(OS_RunPeriodFields::UseWeatherFileRainIndicators,"Yes");
      return;
    }
    setString(OS_RunPeriodFields::UseWeatherFileRainIndicators,"No");
  }
  void RunPeriod_Impl::setUseWeatherFileSnowInd(bool snowInd)
  {
    if(snowInd)
    {
      setString(OS_RunPeriodFields::UseWeatherFileSnowIndicators,"Yes");
      return;
    }
    setString(OS_RunPeriodFields::UseWeatherFileSnowIndicators,"No");
  }

  void RunPeriod_Impl::setNumTimePeriodRepeats(int numRepeats)
  {
    setInt(OS_RunPeriodFields::NumberofTimesRunperiodtobeRepeated,numRepeats);
  }

  void RunPeriod_Impl::ensureNoLeapDays()
  {
    boost::optional<int> month;
    boost::optional<int> day;

    month = getInt(OS_RunPeriodFields::BeginMonth);
    if (month && (month.get() == 2)){
      day = this->getInt(OS_RunPeriodFields::BeginDayofMonth);
      if (day && (day.get() == 29)){
        this->setInt(OS_RunPeriodFields::BeginDayofMonth, 28);
      }
    }

    month = getInt(OS_RunPeriodFields::EndMonth);
    if (month && (month.get() == 2)){
      day = this->getInt(OS_RunPeriodFields::EndDayofMonth);
      if (day && (day.get() == 29)){
        this->setInt(OS_RunPeriodFields::EndDayofMonth, 28);
      }
    }
  }

  // return the parent object in the hierarchy
  boost::optional<ParentObject> RunPeriod_Impl::parent() const
  {
    boost::optional<ParentObject> result(model().getUniqueModelObject<SimulationControl>());
    return result;
  }

  // set the parent, child may have to call methods on the parent
  bool RunPeriod_Impl::setParent(ParentObject& newParent)
  {
    if (newParent.optionalCast<SimulationControl>() && (newParent.model() == model())) {
      return true;
    }
    return false;
  }

  // return any children objects in the hierarchy
  std::vector<ModelObject> RunPeriod_Impl::children() const
  {
    std::vector<ModelObject> result;
    SkyTemperatureVector sts = getObject<ModelObject>().getModelObjectSources<SkyTemperature>();
    result.insert(result.end(),sts.begin(),sts.end());
    return result;
  }

  std::vector<IddObjectType> RunPeriod_Impl::allowableChildTypes() const {
    IddObjectTypeVector result;
    result.push_back(SkyTemperature::iddObjectType());
    return result;
  }

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& RunPeriod_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  bool RunPeriod_Impl::isAnnual() const {
    if (getBeginMonth() != 1u) { return false; }
    if (getBeginDayOfMonth() != 1u) { return false; }
    if (getEndMonth() != 12u) { return false; }
    if (getEndDayOfMonth() != 31u) { return false; }
    return true;
  }

  bool RunPeriod_Impl::isPartialYear() const {
    return !isAnnual();
  }

  bool RunPeriod_Impl::isRepeated() const {
    return (getNumTimePeriodRepeats() > 1);
  }

} // detail

/// constructor
RunPeriod::RunPeriod(const Model& model)
  : ParentObject(RunPeriod::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RunPeriod_Impl>());
  setBeginMonth(1);
  setBeginDayOfMonth(1);
  setEndMonth(12);
  setEndDayOfMonth(31);
}

// constructor
RunPeriod::RunPeriod(std::shared_ptr<detail::RunPeriod_Impl> impl)
  : ParentObject(impl)
{}

int RunPeriod::getBeginMonth() const
{
  return getImpl<detail::RunPeriod_Impl>()->getBeginMonth();
}

int RunPeriod::getBeginDayOfMonth() const
{
  return getImpl<detail::RunPeriod_Impl>()->getBeginDayOfMonth();
}

int RunPeriod::getEndMonth() const
{
  return getImpl<detail::RunPeriod_Impl>()->getEndMonth();
}

int RunPeriod::getEndDayOfMonth() const
{
  return getImpl<detail::RunPeriod_Impl>()->getEndDayOfMonth();
}

bool RunPeriod::getUseWeatherFileHolidays() const
{
  return getImpl<detail::RunPeriod_Impl>()->getUseWeatherFileHolidays();
}

bool RunPeriod::getUseWeatherFileDaylightSavings() const
{
  return getImpl<detail::RunPeriod_Impl>()->getUseWeatherFileDaylightSavings();
}

bool RunPeriod::getApplyWeekendHolidayRule() const
{
  return getImpl<detail::RunPeriod_Impl>()->getApplyWeekendHolidayRule();
}

bool RunPeriod::getUseWeatherFileRainInd() const
{
  return getImpl<detail::RunPeriod_Impl>()->getUseWeatherFileRainInd();
}

bool RunPeriod::getUseWeatherFileSnowInd() const
{
  return getImpl<detail::RunPeriod_Impl>()->getUseWeatherFileSnowInd();
}

int RunPeriod::getNumTimePeriodRepeats() const
{
  return getImpl<detail::RunPeriod_Impl>()->getNumTimePeriodRepeats();
}

void RunPeriod::setBeginMonth(int month)
{
  getImpl<detail::RunPeriod_Impl>()->setBeginMonth(month);
}

void RunPeriod::setBeginDayOfMonth(int day)
{
  getImpl<detail::RunPeriod_Impl>()->setBeginDayOfMonth(day);
}

void RunPeriod::setEndMonth(int month)
{
  getImpl<detail::RunPeriod_Impl>()->setEndMonth(month);
}

void RunPeriod::setEndDayOfMonth(int day)
{
  getImpl<detail::RunPeriod_Impl>()->setEndDayOfMonth(day);
}

void RunPeriod::setUseWeatherFileHolidays(bool use)
{
  getImpl<detail::RunPeriod_Impl>()->setUseWeatherFileHolidays(use);
}

void RunPeriod::setUseWeatherFileDaylightSavings(bool use)
{
  getImpl<detail::RunPeriod_Impl>()->setUseWeatherFileDaylightSavings(use);
}

void RunPeriod::setApplyWeekendHolidayRule(bool apply)
{
  getImpl<detail::RunPeriod_Impl>()->setApplyWeekendHolidayRule(apply);
}

void RunPeriod::setUseWeatherFileRainInd(bool rainInd)
{
  getImpl<detail::RunPeriod_Impl>()->setUseWeatherFileRainInd(rainInd);
}

void RunPeriod::setUseWeatherFileSnowInd(bool snowInd)
{
  getImpl<detail::RunPeriod_Impl>()->setUseWeatherFileSnowInd(snowInd);
}

void RunPeriod::setNumTimePeriodRepeats(int numRepeats)
{
  getImpl<detail::RunPeriod_Impl>()->setNumTimePeriodRepeats(numRepeats);
}

void RunPeriod::ensureNoLeapDays()
{
  getImpl<detail::RunPeriod_Impl>()->ensureNoLeapDays();
}

bool RunPeriod::isAnnual() const {
  return getImpl<detail::RunPeriod_Impl>()->isAnnual();
}

bool RunPeriod::isPartialYear() const {
  return getImpl<detail::RunPeriod_Impl>()->isPartialYear();
}

bool RunPeriod::isRepeated() const {
  return getImpl<detail::RunPeriod_Impl>()->isRepeated();
}

IddObjectType RunPeriod::iddObjectType() {
  IddObjectType result(IddObjectType::OS_RunPeriod);
  return result;
}

} // model
} // openstudio
