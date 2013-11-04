/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef MODEL_SCHEDULEWEEK_IMPL_HPP
#define MODEL_SCHEDULEWEEK_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/ResourceObject_Impl.hpp>

namespace openstudio {
namespace model {

class ScheduleDay;

namespace detail {

  /** ScheduleWeek_Impl is a ResourceObject_Impl that is the implementation class for ScheduleWeek.*/
  class MODEL_API ScheduleWeek_Impl : public ResourceObject_Impl {
   public:

    /** @name Constructors and Destructors */
    //@{

    ScheduleWeek_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    ScheduleWeek_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                      Model_Impl* model,
                      bool keepHandle);

    ScheduleWeek_Impl(const ScheduleWeek_Impl& other,
                      Model_Impl* model,
                      bool keepHandle);

    virtual ~ScheduleWeek_Impl() {}

    //@}
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
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

    /// Set schedules for all days.
    bool setAllSchedules(const ScheduleDay& schedule);

    /// Set schedules for all weekdays.
    bool setWeekdaySchedule(const ScheduleDay& schedule);

    /// Set schedules for all weekends.
    bool setWeekendSchedule(const ScheduleDay& schedule);

   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ScheduleWeek");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SCHEDULEWEEK_IMPL_HPP

