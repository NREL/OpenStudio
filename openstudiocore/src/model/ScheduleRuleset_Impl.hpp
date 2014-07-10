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

#ifndef MODEL_SCHEDULERULESET_IMPL_HPP
#define MODEL_SCHEDULERULESET_IMPL_HPP

#include "ModelAPI.hpp"
#include "Schedule_Impl.hpp"

namespace openstudio {

class Date;

namespace model {

class ScheduleDay;
class ScheduleRule;

namespace detail {

  /** ScheduleRuleset_Impl is a Schedule_Impl that is the implementation class for ScheduleRuleset.*/
  class MODEL_API ScheduleRuleset_Impl : public Schedule_Impl {
   public:

    /** @name Constructors and Destructors */
    //@{

    ScheduleRuleset_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    ScheduleRuleset_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                      Model_Impl* model,
                      bool keepHandle);

    ScheduleRuleset_Impl(const ScheduleRuleset_Impl& other,
                      Model_Impl* model,
                      bool keepHandle);

    virtual ~ScheduleRuleset_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual ModelObject clone(Model model) const;

    virtual std::vector<ResourceObject> resources() const;

    virtual std::vector<ModelObject> children() const;

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

    /// Returns the default day schedule.
    ScheduleDay defaultDaySchedule() const;

    /// Returns the summer design day schedule.
    ScheduleDay summerDesignDaySchedule() const;

    /// Returns true if the summer design day is defaulted.
    bool isSummerDesignDayScheduleDefaulted() const;

    /// Returns the winter design day schedule.
    ScheduleDay winterDesignDaySchedule() const;

    /// Returns true if the winter design day is defaulted.
    bool isWinterDesignDayScheduleDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    /// Sets the summer design day schedule.
    bool setSummerDesignDaySchedule(const ScheduleDay& schedule);
    
    /// Resets the summer design day schedule.
    void resetSummerDesignDaySchedule();

    /// Sets the winter design day schedule.
    bool setWinterDesignDaySchedule(const ScheduleDay& schedule);
    
    /// Resets the winter design day schedule.
    void resetWinterDesignDaySchedule();
    
    //@}
    /** @name Other */
    //@{

    /// Returns a vector of the rules associated with this schedule, this may be empty.
    /// The rules returned are in order from highest priority to lowest priority.
    std::vector<ScheduleRule> scheduleRules() const;

    /// Sets the index of the given rule, 0 is the highest priority.
    /// Returns false if this rule is not a child of this rule set or if index > number of rules - 1.
    bool setScheduleRuleIndex(ScheduleRule& scheduleRule, unsigned index);

    /// Returns a vector of indices into the result of scheduleRules() between start date (inclusive) and end date (inclusive).
    /// If no rule is in place on a given day then -1 is returned.
    std::vector<int> getActiveRuleIndices(const openstudio::Date& startDate, const openstudio::Date& endDate) const;

    /// Returns a vector of day schedules between start date (inclusive) and end date (inclusive).
    std::vector<ScheduleDay> getDaySchedules(const openstudio::Date& startDate, const openstudio::Date& endDate) const;
    
    // Moves this rule to the last position. Called in ScheduleRule remove.
    bool moveToEnd(ScheduleRule& scheduleRule);

    // ensure that this object does not contain the date 2/29
    virtual void ensureNoLeapDays();

    //@}
   private:
    REGISTER_LOGGER("openstudio.model.ScheduleRuleset");

    boost::optional<ScheduleDay> optionalDefaultDaySchedule() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SCHEDULERULESET_IMPL_HPP

