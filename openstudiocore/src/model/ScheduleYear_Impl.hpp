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

#ifndef MODEL_SCHEDULEYEAR_IMPL_HPP
#define MODEL_SCHEDULEYEAR_IMPL_HPP

#include "ModelAPI.hpp"
#include "Schedule_Impl.hpp"

namespace openstudio {

class Date;

namespace model {

class ScheduleWeek;

namespace detail {

  /** ScheduleYear_Impl is a Schedule_Impl that is the implementation class for ScheduleYear.*/
  class MODEL_API ScheduleYear_Impl : public Schedule_Impl {
   public:

    /** @name Constructors and Destructors */
    //@{

    ScheduleYear_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    ScheduleYear_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                      Model_Impl* model,
                      bool keepHandle);

    ScheduleYear_Impl(const ScheduleYear_Impl& other,
                      Model_Impl* model,
                      bool keepHandle);

    virtual ~ScheduleYear_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual boost::optional<ScheduleTypeLimits> scheduleTypeLimits() const;

    /** Returns empty vector, because individual ScheduleDay objects will check values() against
     *  bounds. */
    virtual std::vector<double> values() const;

    virtual bool setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits);

    virtual bool resetScheduleTypeLimits();

    //@}
    /** @name Getters */
    //@{

    /// Returns a vector of dates marking the end of week schedules.
    /// These dates will be in order and have the same number of elements as scheduleWeeks.
    // DLM@20111018: This almost needs a RunPeriod as an argument to know the year description?
    std::vector<openstudio::Date> dates() const;

    /// Returns a vector of week schedules in the same order and with the same number of elements as dates.
    std::vector<ScheduleWeek> scheduleWeeks() const;

    //@}
    /** @name Setters */
    //@{

    /// Sets the given week schedule to be in effect until (inclusive) the date given starting the day after the
    /// previous date marker or January 1st if no previous date marker exists.  Replaces existing scheduleWeek
    /// for same date if it exists.
    bool addScheduleWeek(const openstudio::Date& untilDate, const ScheduleWeek& scheduleWeek);
  
    /// Clear all week schedules from this schedule.
    void clearScheduleWeeks();

    // ensure that this object does not contain the date 2/29
    virtual void ensureNoLeapDays();

    //@}
    /** @name Other */
    //@{

    /// Returns the week schedule in effect at the given date.
    // DLM@20111018: Eventually allow "Nth day in Month" and other descriptors
    boost::optional<ScheduleWeek> getScheduleWeek(const openstudio::Date& date) const;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ScheduleYear");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SCHEDULEYEAR_IMPL_HPP

