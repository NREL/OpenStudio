/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SCHEDULERULESET_HPP
#define MODEL_SCHEDULERULESET_HPP

#include "ModelAPI.hpp"
#include "Schedule.hpp"

namespace openstudio {

class Date;

namespace model {

  class ScheduleDay;
  class ScheduleRule;

  namespace detail {

    class ScheduleRuleset_Impl;
    class ScheduleRule_Impl;

  }  // namespace detail

  /** ScheduleRuleset is a Schedule that wraps the OpenStudio IDD object 'OS_Schedule_Ruleset'.
 *  ScheduleRuleset is the parent of its default day schedule, any non-default summer or winter
 *  design day schedules, and its \link ScheduleRule ScheduleRules\endlink. */
  class MODEL_API ScheduleRuleset : public Schedule
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Constructs a new ScheduleRuleset, and also constructs a new ScheduleDay and assigns it to
   *  be the defaultDaySchedule(). */
    explicit ScheduleRuleset(const Model& model);

    /** Constructs a new ScheduleRuleset, constructs a new ScheduleDay with value at all times
  *  and assigns it to be the defaultDaySchedule(). */
    ScheduleRuleset(const Model& model, double value);

    virtual ~ScheduleRuleset() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ScheduleRuleset(const ScheduleRuleset& other) = default;
    ScheduleRuleset(ScheduleRuleset&& other) = default;
    ScheduleRuleset& operator=(const ScheduleRuleset&) = default;
    ScheduleRuleset& operator=(ScheduleRuleset&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    /// Returns the default day schedule.
    ScheduleDay defaultDaySchedule() const;

    /// Returns the summer design day schedule, which may be the same ScheduleDay as
    /// defaultDaySchedule().
    ScheduleDay summerDesignDaySchedule() const;

    /// Returns true if the summer design day is defaulted; false if it is explicitly set.
    bool isSummerDesignDayScheduleDefaulted() const;

    /// Returns the winter design day schedule, which may be the same ScheduleDay as
    /// defaultDaySchedule().
    ScheduleDay winterDesignDaySchedule() const;

    /// Returns true if the winter design day is defaulted; false if it is explicitly set.
    bool isWinterDesignDayScheduleDefaulted() const;

    /// Returns the holiday schedule, which may be the same ScheduleDay as defaultDaySchedule().
    ScheduleDay holidaySchedule() const;

    /// Returns true if the holiday schedule is defaulted; false if it is explicitly set.
    bool isHolidayScheduleDefaulted() const;

    /// Returns the custom day 1 schedule, which may be the same ScheduleDay as defaultDaySchedule().
    ScheduleDay customDay1Schedule() const;

    /// Returns true if the custom day 1 schedule is defaulted; false if it is explicitly set.
    bool isCustomDay1ScheduleDefaulted() const;

    /// Returns the custom day 2 schedule, which may be the same ScheduleDay as defaultDaySchedule().
    ScheduleDay customDay2Schedule() const;

    /// Returns true if the custom day 2 schedule is defaulted; false if it is explicitly set.
    bool isCustomDay2ScheduleDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    /// Sets the summer design day schedule if ScheduleTypeLimits are compatible. Calls remove
    /// on any existing non-default summer design day schedule. Clones schedule and parents the
    /// clone, but does not call remove on the original schedule.
    bool setSummerDesignDaySchedule(const ScheduleDay& schedule);

    /// Resets the summer design day schedule. Calls remove on any existing non-default summer
    /// design day schedule.
    void resetSummerDesignDaySchedule();

    /// Sets the winter design day schedule if ScheduleTypeLimits are compatible. Calls remove
    /// on any existing non-default winter design day schedule. Clones schedule and parents the
    /// clone, but does not call remove on the original schedule.
    bool setWinterDesignDaySchedule(const ScheduleDay& schedule);

    /// Resets the winter design day schedule. Calls remove on any existing non-default winter
    /// design day schedule.
    void resetWinterDesignDaySchedule();

    /// Sets the holiday schedule if ScheduleTypeLimits are compatible. Calls remove
    /// on any existing non-default holiday schedule. Clones schedule and parents the
    /// clone, but does not call remove on the original schedule.
    bool setHolidaySchedule(const ScheduleDay& schedule);

    /// Resets the holiday schedule. Calls remove on any existing non-default holiday schedule.
    void resetHolidaySchedule();

    /// Sets the custom day 1 schedule if ScheduleTypeLimits are compatible. Calls remove
    /// on any existing non-default custom day 1 schedule. Clones schedule and parents the
    /// clone, but does not call remove on the original schedule.
    bool setCustomDay1Schedule(const ScheduleDay& schedule);

    /// Resets the custom day 1 schedule. Calls remove on any existing non-default custom day 1 schedule.
    void resetCustomDay1Schedule();

    /// Sets the custom day 2 schedule if ScheduleTypeLimits are compatible. Calls remove
    /// on any existing non-default custom day 2 schedule. Clones schedule and parents the
    /// clone, but does not call remove on the original schedule.
    bool setCustomDay2Schedule(const ScheduleDay& schedule);

    /// Resets the custom day 2 schedule. Calls remove on any existing non-default custom day 2 schedule.
    void resetCustomDay2Schedule();

    //@}
    /** @name Other */
    //@{

    /// Returns a vector of the rules associated with this schedule, this may be empty.
    /// The rules returned are in order from highest priority to lowest priority.
    std::vector<ScheduleRule> scheduleRules() const;

    /// Sets the index of the given rule; 0 is the highest priority.
    /// Returns false if this rule is not a child of this rule set or if index > number of
    /// rules - 1.
    bool setScheduleRuleIndex(ScheduleRule& scheduleRule, unsigned index);

    /// Returns a vector of indices into the result of scheduleRules() between start date
    /// (inclusive) and end date (inclusive). If no rule is in place on a given day then -1 is
    /// returned.
    std::vector<int> getActiveRuleIndices(const openstudio::Date& startDate, const openstudio::Date& endDate) const;

    /// Returns a vector of day schedules between start date (inclusive) and end date (inclusive).
    std::vector<ScheduleDay> getDaySchedules(const openstudio::Date& startDate, const openstudio::Date& endDate) const;

    //@}
   protected:
    friend class ScheduleRule;
    friend class detail::ScheduleRule_Impl;

    // Moves this rule to the last position. Called in ScheduleRule remove.
    bool moveToEnd(ScheduleRule& scheduleRule);

    /// @cond
    using ImplType = detail::ScheduleRuleset_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class detail::ScheduleRuleset_Impl;

    explicit ScheduleRuleset(std::shared_ptr<detail::ScheduleRuleset_Impl> impl);
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ScheduleRuleset");
  };

  /** \relates ScheduleRuleset*/
  using OptionalScheduleRuleset = boost::optional<ScheduleRuleset>;

  /** \relates ScheduleRuleset*/
  using ScheduleRulesetVector = std::vector<ScheduleRuleset>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULERULESET_HPP
