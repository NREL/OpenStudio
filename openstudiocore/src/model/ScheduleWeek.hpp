/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_SCHEDULEWEEK_HPP
#define MODEL_SCHEDULEWEEK_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

class ScheduleDay;

namespace detail {

  class ScheduleWeek_Impl;

} // detail

/** ScheduleWeek is a ResourceObject that wraps the OpenStudio IDD object 
 *  'OS_Schedule_Week'. */
class MODEL_API ScheduleWeek : public ResourceObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ScheduleWeek(const Model& model);

  virtual ~ScheduleWeek() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<ScheduleDay> sundaySchedule() const;

  boost::optional<ScheduleDay> mondaySchedule() const;

  boost::optional<ScheduleDay> tuesdaySchedule() const;

  boost::optional<ScheduleDay> wednesdaySchedule() const;

  boost::optional<ScheduleDay> thursdaySchedule() const;

  boost::optional<ScheduleDay> fridaySchedule() const;

  boost::optional<ScheduleDay> saturdaySchedule() const;

  boost::optional<ScheduleDay> holidaySchedule() const;

  boost::optional<ScheduleDay> summerDesignDaySchedule() const;

  boost::optional<ScheduleDay> winterDesignDaySchedule() const;

  boost::optional<ScheduleDay> customDay1Schedule() const;

  boost::optional<ScheduleDay> customDay2Schedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setSundaySchedule(const ScheduleDay& schedule);

  bool setMondaySchedule(const ScheduleDay& schedule);

  bool setTuesdaySchedule(const ScheduleDay& schedule);

  bool setWednesdaySchedule(const ScheduleDay& schedule);

  bool setThursdaySchedule(const ScheduleDay& schedule);

  bool setFridaySchedule(const ScheduleDay& schedule);

  bool setSaturdaySchedule(const ScheduleDay& schedule);

  bool setHolidaySchedule(const ScheduleDay& schedule);

  bool setSummerDesignDaySchedule(const ScheduleDay& schedule);

  bool setWinterDesignDaySchedule(const ScheduleDay& schedule);

  bool setCustomDay1Schedule(const ScheduleDay& schedule);

  bool setCustomDay2Schedule(const ScheduleDay& schedule);

  //@}
  /** @name Other */
  //@{

  /// Set schedules for all days.
  bool setAllSchedules(const ScheduleDay& schedule);

  /// Set schedules for all weekdays.
  bool setWeekdaySchedule(const ScheduleDay& schedule);

  /// Set schedules for all weekends.
  bool setWeekendSchedule(const ScheduleDay& schedule);
  
  //@}
 protected:
  /// @cond
  typedef detail::ScheduleWeek_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;
  friend class detail::ScheduleWeek_Impl;

  explicit ScheduleWeek(std::shared_ptr<detail::ScheduleWeek_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ScheduleWeek");
};

/** \relates ScheduleWeek*/
typedef boost::optional<ScheduleWeek> OptionalScheduleWeek;

/** \relates ScheduleWeek*/
typedef std::vector<ScheduleWeek> ScheduleWeekVector;

} // model
} // openstudio

#endif // MODEL_SCHEDULEWEEK_HPP

