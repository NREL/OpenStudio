/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SCHEDULEDAY_IMPL_HPP
#define MODEL_SCHEDULEDAY_IMPL_HPP

#include "ModelAPI.hpp"
#include "ScheduleBase_Impl.hpp"

#include "../utilities/time/Time.hpp"
#include "../utilities/data/TimeSeries.hpp"

namespace openstudio {

class TimeSeries;

namespace model {

  class ScheduleTypeLimits;

  namespace detail {

    class Timestep_Impl;

    /** ScheduleDay_Impl is a ResourceObject_Impl that is the implementation class for ScheduleDay.*/
    class MODEL_API ScheduleDay_Impl : public ScheduleBase_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ScheduleDay_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ScheduleDay_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ScheduleDay_Impl(const ScheduleDay_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ScheduleDay_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual std::vector<IdfObject> remove() override;

      virtual boost::optional<ParentObject> parent() const override;

      virtual bool setParent(ParentObject& newParent) override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      virtual boost::optional<ScheduleTypeLimits> scheduleTypeLimits() const override;

      bool isScheduleTypeLimitsDefaulted() const;

      std::string interpolatetoTimestep() const;

      bool isInterpolatetoTimestepDefaulted() const;

      /// Returns a vector of times marking the end value intervals.
      /// These times will be in order and have the same number of elements as values.
      /// All times will be less than or equal to 1 day.
      std::vector<openstudio::Time> times() const;

      /// Returns a vector of values in the same order and with the same number of elements as times.
      virtual std::vector<double> values() const override;

      /// Returns the value in effect at the given time.
      /// If time is less than 0 days or greater than 1 day, 0 is returned.
      double getValue(const openstudio::Time& time) const;

      /// Returns the timeseries corresponding to simulation timestep and chosen interpolation method.
      openstudio::TimeSeries timeSeries() const;

      //@}
      /** @name Setters */
      //@{

      virtual bool setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) override;

      virtual bool resetScheduleTypeLimits() override;

      bool setInterpolatetoTimestep(const std::string& interpolatetoTimestep);

      void resetInterpolatetoTimestep();

      /// Sets the given week schedule to be in effect until (inclusive) the date given starting the day after the
      /// previous date marker or January 1st if no previous date marker exists.
      /// Returns false if time is less than 0 days or greater than 1 day.  Replaces existing value
      /// for same time if it exists.
      bool addValue(const openstudio::Time& untilTime, double value);

      boost::optional<double> removeValue(const openstudio::Time& time);

      /// Clear all values from this schedule.
      void clearValues();

      // ensure that this object does not contain the date 2/29
      virtual void ensureNoLeapDays() override;

      //@}
     protected:
      virtual bool candidateIsCompatibleWithCurrentUse(const ScheduleTypeLimits& candidate) const override;

      virtual bool okToResetScheduleTypeLimits() const override;

      friend class Timestep_Impl;
      void clearCachedTimeSeries();

      //private slots:
     private:
      void clearCachedVariables();

     private:
      REGISTER_LOGGER("openstudio.model.ScheduleDay");

      mutable boost::optional<std::vector<openstudio::Time>> m_cachedTimes;
      mutable boost::optional<std::vector<double>> m_cachedValues;
      mutable boost::optional<openstudio::TimeSeries> m_cachedTimeSeries;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULEDAY_IMPL_HPP
