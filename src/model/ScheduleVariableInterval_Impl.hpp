/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SCHEDULEVARIABLEINTERVAL_IMPL_HPP
#define MODEL_SCHEDULEVARIABLEINTERVAL_IMPL_HPP

#include "ModelAPI.hpp"
#include "ScheduleInterval_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ScheduleVariableInterval_Impl is a ScheduleInterval_Impl that is the implementation class for ScheduleVariableInterval.*/
    class MODEL_API ScheduleVariableInterval_Impl : public ScheduleInterval_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ScheduleVariableInterval_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ScheduleVariableInterval_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ScheduleVariableInterval_Impl(const ScheduleVariableInterval_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ScheduleVariableInterval_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual IddObjectType iddObjectType() const override;

      virtual boost::optional<ScheduleTypeLimits> scheduleTypeLimits() const override;

      virtual bool setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) override;

      virtual bool resetScheduleTypeLimits() override;

      virtual openstudio::TimeSeries timeSeries() const override;

      virtual bool setTimeSeries(const openstudio::TimeSeries& timeSeries) override;

      //@}
      /** @name Getters */
      //@{

      bool interpolatetoTimestep() const;

      bool isInterpolatetoTimestepDefaulted() const;

      int startMonth() const;

      int startDay() const;

      double fixedIntervalLength() const;

      double outOfRangeValue() const;

      bool isOutOfRangeValueDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setInterpolatetoTimestep(bool interpolatetoTimestep, bool driverMethod = true);

      void resetInterpolatetoTimestep(bool driverMethod = true);

      bool setStartMonth(int startMonth, bool driverMethod = true);

      bool setStartDay(int startDay, bool driverMethod = true);

      bool setOutOfRangeValue(double outOfRangeValue, bool driverMethod = true);

      void resetOutOfRangeValue(bool driverMethod = true);

      // ensure that this object does not contain the date 2/29
      virtual void ensureNoLeapDays() override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ScheduleVariableInterval");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULEVARIABLEINTERVAL_IMPL_HPP
