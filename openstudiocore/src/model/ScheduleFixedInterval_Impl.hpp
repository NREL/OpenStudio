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

#ifndef MODEL_SCHEDULEFIXEDINTERVAL_IMPL_HPP
#define MODEL_SCHEDULEFIXEDINTERVAL_IMPL_HPP

#include "ModelAPI.hpp"
#include "ScheduleInterval_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** ScheduleFixedInterval_Impl is a ScheduleInterval_Impl that is the implementation class for ScheduleFixedInterval.*/
  class MODEL_API ScheduleFixedInterval_Impl : public ScheduleInterval_Impl {
    Q_OBJECT;
    Q_PROPERTY(bool interpolatetoTimestep READ interpolatetoTimestep WRITE setInterpolatetoTimestep RESET resetInterpolatetoTimestep);
    Q_PROPERTY(bool isInterpolatetoTimestepDefaulted READ isInterpolatetoTimestepDefaulted);
    Q_PROPERTY(double intervalLength READ intervalLength WRITE setIntervalLength);
    Q_PROPERTY(double outOfRangeValue READ outOfRangeValue WRITE setOutOfRangeValue RESET resetOutOfRangeValue);
    Q_PROPERTY(bool isOutOfRangeValueDefaulted READ isOutOfRangeValueDefaulted);
    Q_PROPERTY(int startMonth READ startMonth WRITE setStartMonth);
    Q_PROPERTY(int startDay READ startDay WRITE setStartDay);
   public:

    /** @name Constructors and Destructors */
    //@{

    ScheduleFixedInterval_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    ScheduleFixedInterval_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                               Model_Impl* model,
                               bool keepHandle);

    ScheduleFixedInterval_Impl(const ScheduleFixedInterval_Impl& other,
                               Model_Impl* model,
                               bool keepHandle);

    virtual ~ScheduleFixedInterval_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual IddObjectType iddObjectType() const;

    virtual boost::optional<ScheduleTypeLimits> scheduleTypeLimits() const;

    virtual bool setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits);

    virtual bool resetScheduleTypeLimits();

    virtual openstudio::TimeSeries timeSeries() const;

    virtual bool setTimeSeries(const openstudio::TimeSeries& timeSeries);

    //@}
    /** @name Getters */
    //@{

    bool interpolatetoTimestep() const;

    bool isInterpolatetoTimestepDefaulted() const;

    double intervalLength() const;

    double outOfRangeValue() const;

    bool isOutOfRangeValueDefaulted() const;

    int startMonth() const;

    int startDay() const;

    //@}
    /** @name Setters */
    //@{

    void setInterpolatetoTimestep(bool interpolatetoTimestep, bool driverMethod = true);

    void resetInterpolatetoTimestep(bool driverMethod = true);

    bool setIntervalLength(double intervalLength, bool driverMethod = true);

    void setOutOfRangeValue(double outOfRangeValue, bool driverMethod = true);

    void resetOutOfRangeValue(bool driverMethod = true);

    bool setStartMonth(int startMonth, bool driverMethod = true);

    bool setStartDay(int startDay, bool driverMethod = true);

    // ensure that this object does not contain the date 2/29
    virtual void ensureNoLeapDays();

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.ScheduleFixedInterval");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SCHEDULEFIXEDINTERVAL_IMPL_HPP

