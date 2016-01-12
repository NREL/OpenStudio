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

#include "ScheduleWeek.hpp"
#include "ScheduleWeek_Impl.hpp"
#include "ScheduleDay.hpp"
#include "ScheduleDay_Impl.hpp"
#include "Model.hpp"

#include <utilities/idd/OS_Schedule_Week_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ScheduleWeek_Impl::ScheduleWeek_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ResourceObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ScheduleWeek::iddObjectType());
  }

  ScheduleWeek_Impl::ScheduleWeek_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ScheduleWeek::iddObjectType());
  }

  ScheduleWeek_Impl::ScheduleWeek_Impl(const ScheduleWeek_Impl& other,
                                       Model_Impl* model,
                                       bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ScheduleWeek_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ScheduleWeek_Impl::iddObjectType() const {
    return ScheduleWeek::iddObjectType();
  }

  boost::optional<ScheduleDay> ScheduleWeek_Impl::sundaySchedule() const
  {
    boost::optional<ScheduleDay> result;
    OptionalWorkspaceObject schedule = getTarget(OS_Schedule_WeekFields::SundaySchedule_DayName);
    if (schedule){
      result = schedule->optionalCast<ScheduleDay>();
    }
    return result;
  }

  boost::optional<ScheduleDay> ScheduleWeek_Impl::mondaySchedule() const
  {
    boost::optional<ScheduleDay> result;
    OptionalWorkspaceObject schedule = getTarget(OS_Schedule_WeekFields::MondaySchedule_DayName);
    if (schedule){
      result = schedule->optionalCast<ScheduleDay>();
    }
    return result;
  }

  boost::optional<ScheduleDay> ScheduleWeek_Impl::tuesdaySchedule() const
  {
    boost::optional<ScheduleDay> result;
    OptionalWorkspaceObject schedule = getTarget(OS_Schedule_WeekFields::TuesdaySchedule_DayName);
    if (schedule){
      result = schedule->optionalCast<ScheduleDay>();
    }
    return result;
  }

  boost::optional<ScheduleDay> ScheduleWeek_Impl::wednesdaySchedule() const
  {
    boost::optional<ScheduleDay> result;
    OptionalWorkspaceObject schedule = getTarget(OS_Schedule_WeekFields::WednesdaySchedule_DayName);
    if (schedule){
      result = schedule->optionalCast<ScheduleDay>();
    }
    return result;
  }

  boost::optional<ScheduleDay> ScheduleWeek_Impl::thursdaySchedule() const
  {
    boost::optional<ScheduleDay> result;
    OptionalWorkspaceObject schedule = getTarget(OS_Schedule_WeekFields::ThursdaySchedule_DayName);
    if (schedule){
      result = schedule->optionalCast<ScheduleDay>();
    }
    return result;
  }

  boost::optional<ScheduleDay> ScheduleWeek_Impl::fridaySchedule() const
  {
    boost::optional<ScheduleDay> result;
    OptionalWorkspaceObject schedule = getTarget(OS_Schedule_WeekFields::FridaySchedule_DayName);
    if (schedule){
      result = schedule->optionalCast<ScheduleDay>();
    }
    return result;
  }

  boost::optional<ScheduleDay> ScheduleWeek_Impl::saturdaySchedule() const
  {
    boost::optional<ScheduleDay> result;
    OptionalWorkspaceObject schedule = getTarget(OS_Schedule_WeekFields::SaturdaySchedule_DayName);
    if (schedule){
      result = schedule->optionalCast<ScheduleDay>();
    }
    return result;
  }

  boost::optional<ScheduleDay> ScheduleWeek_Impl::holidaySchedule() const
  {
    boost::optional<ScheduleDay> result;
    OptionalWorkspaceObject schedule = getTarget(OS_Schedule_WeekFields::HolidaySchedule_DayName);
    if (schedule){
      result = schedule->optionalCast<ScheduleDay>();
    }
    return result;
  }

  boost::optional<ScheduleDay> ScheduleWeek_Impl::summerDesignDaySchedule() const
  {
    boost::optional<ScheduleDay> result;
    OptionalWorkspaceObject schedule = getTarget(OS_Schedule_WeekFields::SummerDesignDaySchedule_DayName);
    if (schedule){
      result = schedule->optionalCast<ScheduleDay>();
    }
    return result;
  }

  boost::optional<ScheduleDay> ScheduleWeek_Impl::winterDesignDaySchedule() const
  {
    boost::optional<ScheduleDay> result;
    OptionalWorkspaceObject schedule = getTarget(OS_Schedule_WeekFields::WinterDesignDaySchedule_DayName);
    if (schedule){
      result = schedule->optionalCast<ScheduleDay>();
    }
    return result;
  }

  boost::optional<ScheduleDay> ScheduleWeek_Impl::customDay1Schedule() const
  {
    boost::optional<ScheduleDay> result;
    OptionalWorkspaceObject schedule = getTarget(OS_Schedule_WeekFields::CustomDay1Schedule_DayName);
    if (schedule){
      result = schedule->optionalCast<ScheduleDay>();
    }
    return result;
  }

  boost::optional<ScheduleDay> ScheduleWeek_Impl::customDay2Schedule() const
  {
    boost::optional<ScheduleDay> result;
    OptionalWorkspaceObject schedule = getTarget(OS_Schedule_WeekFields::CustomDay2Schedule_DayName);
    if (schedule){
      result = schedule->optionalCast<ScheduleDay>();
    }
    return result;
  }

  bool ScheduleWeek_Impl::setSundaySchedule(const ScheduleDay& schedule)
  {
    return setPointer(OS_Schedule_WeekFields::SundaySchedule_DayName, schedule.handle());
  }

  bool ScheduleWeek_Impl::setMondaySchedule(const ScheduleDay& schedule)
  {
    return setPointer(OS_Schedule_WeekFields::MondaySchedule_DayName, schedule.handle());
  }

  bool ScheduleWeek_Impl::setTuesdaySchedule(const ScheduleDay& schedule)
  {
    return setPointer(OS_Schedule_WeekFields::TuesdaySchedule_DayName, schedule.handle());
  }

  bool ScheduleWeek_Impl::setWednesdaySchedule(const ScheduleDay& schedule)
  {
    return setPointer(OS_Schedule_WeekFields::WednesdaySchedule_DayName, schedule.handle());
  }

  bool ScheduleWeek_Impl::setThursdaySchedule(const ScheduleDay& schedule)
  {
    return setPointer(OS_Schedule_WeekFields::ThursdaySchedule_DayName, schedule.handle());
  }

  bool ScheduleWeek_Impl::setFridaySchedule(const ScheduleDay& schedule)
  {
    return setPointer(OS_Schedule_WeekFields::FridaySchedule_DayName, schedule.handle());
  }

  bool ScheduleWeek_Impl::setSaturdaySchedule(const ScheduleDay& schedule)
  {
    return setPointer(OS_Schedule_WeekFields::SaturdaySchedule_DayName, schedule.handle());
  }

  bool ScheduleWeek_Impl::setHolidaySchedule(const ScheduleDay& schedule)
  {
    return setPointer(OS_Schedule_WeekFields::HolidaySchedule_DayName, schedule.handle());
  }

  bool ScheduleWeek_Impl::setSummerDesignDaySchedule(const ScheduleDay& schedule)
  {
    return setPointer(OS_Schedule_WeekFields::SummerDesignDaySchedule_DayName, schedule.handle());
  }

  bool ScheduleWeek_Impl::setWinterDesignDaySchedule(const ScheduleDay& schedule)
  {
    return setPointer(OS_Schedule_WeekFields::WinterDesignDaySchedule_DayName, schedule.handle());
  }

  bool ScheduleWeek_Impl::setCustomDay1Schedule(const ScheduleDay& schedule)
  {
    return setPointer(OS_Schedule_WeekFields::CustomDay1Schedule_DayName, schedule.handle());
  }

  bool ScheduleWeek_Impl::setCustomDay2Schedule(const ScheduleDay& schedule)
  {
    return setPointer(OS_Schedule_WeekFields::CustomDay2Schedule_DayName, schedule.handle());
  }

  bool ScheduleWeek_Impl::setAllSchedules(const ScheduleDay& schedule)
  {
    bool result = true;

    result = result && setSundaySchedule(schedule);
    result = result && setMondaySchedule(schedule);
    result = result && setTuesdaySchedule(schedule);
    result = result && setWednesdaySchedule(schedule);
    result = result && setThursdaySchedule(schedule);
    result = result && setFridaySchedule(schedule);
    result = result && setSaturdaySchedule(schedule);
    result = result && setHolidaySchedule(schedule);
    result = result && setSummerDesignDaySchedule(schedule);
    result = result && setWinterDesignDaySchedule(schedule);
    result = result && setCustomDay1Schedule(schedule);
    result = result && setCustomDay2Schedule(schedule);

    return result;
  }

  bool ScheduleWeek_Impl::setWeekdaySchedule(const ScheduleDay& schedule)
  {
    bool result = true;

    result = result && setMondaySchedule(schedule);
    result = result && setTuesdaySchedule(schedule);
    result = result && setWednesdaySchedule(schedule);
    result = result && setThursdaySchedule(schedule);
    result = result && setFridaySchedule(schedule);

    return result;
  }

  bool ScheduleWeek_Impl::setWeekendSchedule(const ScheduleDay& schedule)
  {
    bool result = true;

    result = result && setSundaySchedule(schedule);
    result = result && setSaturdaySchedule(schedule);

    return result;
  }

} // detail

ScheduleWeek::ScheduleWeek(const Model& model)
  : ResourceObject(ScheduleWeek::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ScheduleWeek_Impl>());
}

IddObjectType ScheduleWeek::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Schedule_Week);
  return result;
}

/// @cond
ScheduleWeek::ScheduleWeek(std::shared_ptr<detail::ScheduleWeek_Impl> impl)
  : ResourceObject(impl)
{}
/// @endcond

boost::optional<ScheduleDay> ScheduleWeek::sundaySchedule() const
{
  return getImpl<detail::ScheduleWeek_Impl>()->sundaySchedule();
}

boost::optional<ScheduleDay> ScheduleWeek::mondaySchedule() const
{
  return getImpl<detail::ScheduleWeek_Impl>()->mondaySchedule();
}

boost::optional<ScheduleDay> ScheduleWeek::tuesdaySchedule() const
{
  return getImpl<detail::ScheduleWeek_Impl>()->tuesdaySchedule();
}

boost::optional<ScheduleDay> ScheduleWeek::wednesdaySchedule() const
{
  return getImpl<detail::ScheduleWeek_Impl>()->wednesdaySchedule();
}

boost::optional<ScheduleDay> ScheduleWeek::thursdaySchedule() const
{
  return getImpl<detail::ScheduleWeek_Impl>()->thursdaySchedule();
}

boost::optional<ScheduleDay> ScheduleWeek::fridaySchedule() const
{
  return getImpl<detail::ScheduleWeek_Impl>()->fridaySchedule();
}

boost::optional<ScheduleDay> ScheduleWeek::saturdaySchedule() const
{
  return getImpl<detail::ScheduleWeek_Impl>()->saturdaySchedule();
}

boost::optional<ScheduleDay> ScheduleWeek::holidaySchedule() const
{
  return getImpl<detail::ScheduleWeek_Impl>()->holidaySchedule();
}

boost::optional<ScheduleDay> ScheduleWeek::summerDesignDaySchedule() const
{
  return getImpl<detail::ScheduleWeek_Impl>()->summerDesignDaySchedule();
}

boost::optional<ScheduleDay> ScheduleWeek::winterDesignDaySchedule() const
{
  return getImpl<detail::ScheduleWeek_Impl>()->winterDesignDaySchedule();
}

boost::optional<ScheduleDay> ScheduleWeek::customDay1Schedule() const
{
  return getImpl<detail::ScheduleWeek_Impl>()->customDay1Schedule();
}

boost::optional<ScheduleDay> ScheduleWeek::customDay2Schedule() const
{
  return getImpl<detail::ScheduleWeek_Impl>()->customDay2Schedule();
}

bool ScheduleWeek::setSundaySchedule(const ScheduleDay& schedule)
{
  return getImpl<detail::ScheduleWeek_Impl>()->setSundaySchedule(schedule);
}

bool ScheduleWeek::setMondaySchedule(const ScheduleDay& schedule)
{
  return getImpl<detail::ScheduleWeek_Impl>()->setMondaySchedule(schedule);
}

bool ScheduleWeek::setTuesdaySchedule(const ScheduleDay& schedule)
{
  return getImpl<detail::ScheduleWeek_Impl>()->setTuesdaySchedule(schedule);
}

bool ScheduleWeek::setWednesdaySchedule(const ScheduleDay& schedule)
{
  return getImpl<detail::ScheduleWeek_Impl>()->setWednesdaySchedule(schedule);
}

bool ScheduleWeek::setThursdaySchedule(const ScheduleDay& schedule)
{
  return getImpl<detail::ScheduleWeek_Impl>()->setThursdaySchedule(schedule);
}

bool ScheduleWeek::setFridaySchedule(const ScheduleDay& schedule)
{
  return getImpl<detail::ScheduleWeek_Impl>()->setFridaySchedule(schedule);
}

bool ScheduleWeek::setSaturdaySchedule(const ScheduleDay& schedule)
{
  return getImpl<detail::ScheduleWeek_Impl>()->setSaturdaySchedule(schedule);
}

bool ScheduleWeek::setHolidaySchedule(const ScheduleDay& schedule)
{
  return getImpl<detail::ScheduleWeek_Impl>()->setHolidaySchedule(schedule);
}

bool ScheduleWeek::setSummerDesignDaySchedule(const ScheduleDay& schedule)
{
  return getImpl<detail::ScheduleWeek_Impl>()->setSummerDesignDaySchedule(schedule);
}

bool ScheduleWeek::setWinterDesignDaySchedule(const ScheduleDay& schedule)
{
  return getImpl<detail::ScheduleWeek_Impl>()->setWinterDesignDaySchedule(schedule);
}

bool ScheduleWeek::setCustomDay1Schedule(const ScheduleDay& schedule)
{
  return getImpl<detail::ScheduleWeek_Impl>()->setCustomDay1Schedule(schedule);
}

bool ScheduleWeek::setCustomDay2Schedule(const ScheduleDay& schedule)
{
  return getImpl<detail::ScheduleWeek_Impl>()->setCustomDay2Schedule(schedule);
}

bool ScheduleWeek::setAllSchedules(const ScheduleDay& schedule)
{
  return getImpl<detail::ScheduleWeek_Impl>()->setAllSchedules(schedule);
}

bool ScheduleWeek::setWeekdaySchedule(const ScheduleDay& schedule)
{
  return getImpl<detail::ScheduleWeek_Impl>()->setWeekdaySchedule(schedule);
}

bool ScheduleWeek::setWeekendSchedule(const ScheduleDay& schedule)
{
  return getImpl<detail::ScheduleWeek_Impl>()->setWeekendSchedule(schedule);
}

} // model
} // openstudio

