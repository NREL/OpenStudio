/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
    class MODEL_API ScheduleRuleset_Impl : public Schedule_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ScheduleRuleset_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ScheduleRuleset_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ScheduleRuleset_Impl(const ScheduleRuleset_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ScheduleRuleset_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ResourceObject> resources() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual boost::optional<ScheduleTypeLimits> scheduleTypeLimits() const override;

      /** Returns empty vector, because individual ScheduleDay objects will check values() against
     *  bounds. */
      virtual std::vector<double> values() const override;

      virtual bool setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) override;

      virtual bool resetScheduleTypeLimits() override;

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

      /// Returns the holiday schedule.
      ScheduleDay holidaySchedule() const;

      /// Returns true if the holiday schedule is defaulted.
      bool isHolidayScheduleDefaulted() const;

      /// Returns the custom day 1 schedule.
      ScheduleDay customDay1Schedule() const;

      /// Returns true if the custom day 1 schedule is defaulted.
      bool isCustomDay1ScheduleDefaulted() const;

      /// Returns the custom day 2 schedule.
      ScheduleDay customDay2Schedule() const;

      /// Returns true if the custom day 2 schedule is defaulted.
      bool isCustomDay2ScheduleDefaulted() const;

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

      /// Sets the holiday schedule.
      bool setHolidaySchedule(const ScheduleDay& schedule);

      /// Resets the holiday schedule.
      void resetHolidaySchedule();

      /// Sets the custom day 1 schedule.
      bool setCustomDay1Schedule(const ScheduleDay& schedule);

      /// Resets the custom day 1 schedule.
      void resetCustomDay1Schedule();

      /// Sets the custom day 2 schedule.
      bool setCustomDay2Schedule(const ScheduleDay& schedule);

      /// Resets the custom day 2 schedule.
      void resetCustomDay2Schedule();

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
      virtual void ensureNoLeapDays() override;

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.ScheduleRuleset");

      boost::optional<ScheduleDay> optionalDefaultDaySchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULERULESET_IMPL_HPP
