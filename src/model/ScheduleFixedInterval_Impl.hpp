/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SCHEDULEFIXEDINTERVAL_IMPL_HPP
#define MODEL_SCHEDULEFIXEDINTERVAL_IMPL_HPP

#include "ModelAPI.hpp"
#include "ScheduleInterval_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ScheduleFixedInterval_Impl is a ScheduleInterval_Impl that is the implementation class for ScheduleFixedInterval.*/
    class MODEL_API ScheduleFixedInterval_Impl : public ScheduleInterval_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ScheduleFixedInterval_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ScheduleFixedInterval_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ScheduleFixedInterval_Impl(const ScheduleFixedInterval_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ScheduleFixedInterval_Impl() override = default;

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

      bool translatetoScheduleFile() const;

      bool isTranslatetoScheduleFileDefaulted() const;

      int intervalLength() const;

      double outOfRangeValue() const;

      bool isOutOfRangeValueDefaulted() const;

      int startMonth() const;

      int startDay() const;

      //@}
      /** @name Setters */
      //@{

      bool setInterpolatetoTimestep(bool interpolatetoTimestep, bool driverMethod = true);

      void resetInterpolatetoTimestep(bool driverMethod = true);

      bool setTranslatetoScheduleFile(bool translatetoScheduleFile, bool driverMethod = true);

      void resetTranslatetoScheduleFile(bool driverMethod = true);

      bool setIntervalLength(int intervalLength, bool driverMethod = true);

      bool setOutOfRangeValue(double outOfRangeValue, bool driverMethod = true);

      void resetOutOfRangeValue(bool driverMethod = true);

      bool setStartMonth(int startMonth, bool driverMethod = true);

      bool setStartDay(int startDay, bool driverMethod = true);

      // ensure that this object does not contain the date 2/29
      virtual void ensureNoLeapDays() override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ScheduleFixedInterval");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULEFIXEDINTERVAL_IMPL_HPP
