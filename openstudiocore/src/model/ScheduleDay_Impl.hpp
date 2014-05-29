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

#ifndef MODEL_SCHEDULEDAY_IMPL_HPP
#define MODEL_SCHEDULEDAY_IMPL_HPP

#include "ModelAPI.hpp"
#include "ScheduleBase_Impl.hpp"

#include "../utilities/time/Time.hpp"

namespace openstudio {

namespace model {

class ScheduleTypeLimits;

namespace detail {

  /** ScheduleDay_Impl is a ResourceObject_Impl that is the implementation class for ScheduleDay.*/
  class MODEL_API ScheduleDay_Impl : public ScheduleBase_Impl {
    Q_OBJECT;
    Q_PROPERTY(bool interpolatetoTimestep READ interpolatetoTimestep WRITE setInterpolatetoTimestep RESET resetInterpolatetoTimestep);
    Q_PROPERTY(bool isInterpolatetoTimestepDefaulted READ isInterpolatetoTimestepDefaulted);
   public:
    /** @name Constructors and Destructors */
    //@{

    ScheduleDay_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    ScheduleDay_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                     Model_Impl* model,
                     bool keepHandle);

    ScheduleDay_Impl(const ScheduleDay_Impl& other,
                     Model_Impl* model,
                     bool keepHandle);

    virtual ~ScheduleDay_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual std::vector<IdfObject> remove();

    virtual boost::optional<ParentObject> parent() const;

    virtual bool setParent(ParentObject& newParent);

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    virtual boost::optional<ScheduleTypeLimits> scheduleTypeLimits() const;

    bool isScheduleTypeLimitsDefaulted() const;

    bool interpolatetoTimestep() const;

    bool isInterpolatetoTimestepDefaulted() const;

    /// Returns a vector of times marking the end value intervals.
    /// These times will be in order and have the same number of elements as values.
    /// All times will be less than or equal to 1 day.
    std::vector<openstudio::Time> times() const;

    /// Returns a vector of values in the same order and with the same number of elements as times.
    virtual std::vector<double> values() const;

    /// Returns the value in effect at the given time.  If time is less than 0 days or greater than 1 day, 0 is returned.
    double getValue(const openstudio::Time& time) const;

    boost::optional<Quantity> getValueAsQuantity(const openstudio::Time& time, bool returnIP=false) const;

    //@}
    /** @name Setters */
    //@{

    virtual bool setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits);

    virtual bool resetScheduleTypeLimits();

    void setInterpolatetoTimestep(bool interpolatetoTimestep);

    void resetInterpolatetoTimestep();

    /// Sets the given week schedule to be in effect until (inclusive) the date given starting the day after the
    /// previous date marker or January 1st if no previous date marker exists.
    /// Returns false if time is less than 0 days or greater than 1 day.  Replaces existing value
    /// for same time if it exists.
    bool addValue(const openstudio::Time& untilTime, double value);

    bool addValue(const openstudio::Time& untilTime, const Quantity& value);

    boost::optional<double> removeValue(const openstudio::Time& time);

    /// Clear all values from this schedule.
    void clearValues();

    // ensure that this object does not contain the date 2/29
    virtual void ensureNoLeapDays();

    //@}
   protected:
    virtual bool candidateIsCompatibleWithCurrentUse(const ScheduleTypeLimits& candidate) const;

    virtual bool okToResetScheduleTypeLimits() const;

   private slots:

    void clearCachedVariables();

   private:
    REGISTER_LOGGER("openstudio.model.ScheduleDay");

    mutable boost::optional<std::vector<openstudio::Time> > m_cachedTimes;
    mutable boost::optional<std::vector<double> > m_cachedValues;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SCHEDULEDAY_IMPL_HPP

