/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SCHEDULERULE_HPP
#define MODEL_SCHEDULERULE_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

class Date;

namespace model {

  class ScheduleDay;
  class ScheduleRuleset;

  namespace detail {

    class ScheduleRule_Impl;
    class ScheduleRuleset_Impl;

  }  // namespace detail

  /** ScheduleRule is a ParentObject that wraps the OpenStudio IDD object 'OS:Schedule:Rule'. */
  class MODEL_API ScheduleRule : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /// Constructor associates this rule with the given ruleset.  This rule is made to be the highest priority rule.
    /// A new ScheduleDay is created which is owned by (a child of) this rule.
    explicit ScheduleRule(ScheduleRuleset& scheduleRuleset);

    /// Constructor associates this rule with the given ruleset. This rule is made to be the highest priority rule.
    /// The given daySchedule is cloned and the new object is owned by (a child of) this rule.
    explicit ScheduleRule(ScheduleRuleset& scheduleRuleset, const ScheduleDay& daySchedule);

    virtual ~ScheduleRule() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ScheduleRule(const ScheduleRule& other) = default;
    ScheduleRule(ScheduleRule&& other) = default;
    ScheduleRule& operator=(const ScheduleRule&) = default;
    ScheduleRule& operator=(ScheduleRule&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validDateSpecificationTypeValues();

    /** @name Getters */
    //@{

    ScheduleRuleset scheduleRuleset() const;

    int ruleIndex() const;

    ScheduleDay daySchedule() const;

    bool applySunday() const;

    bool applyMonday() const;

    bool applyTuesday() const;

    bool applyWednesday() const;

    bool applyThursday() const;

    bool applyFriday() const;

    bool applySaturday() const;

    // Note: there is no applyHoliday, instead that's implemented at the ScheduleRule's level

    std::string dateSpecificationType() const;

    /// Returns the start date of the date range if set.
    /// A start date > end date indicates that the range wraps around the year.
    boost::optional<openstudio::Date> startDate() const;

    /// Returns the end date of the date range if set.
    /// A start date > end date indicates that the range wraps around the year.
    boost::optional<openstudio::Date> endDate() const;

    /// Returns the specific dates specified if any.
    std::vector<openstudio::Date> specificDates() const;

    //@}
    /** @name Setters */
    //@{

    bool setApplySunday(bool applySunday);

    void setApplySundayNoFail(bool applySunday);

    bool setApplyMonday(bool applyMonday);

    void setApplyMondayNoFail(bool applyMonday);

    bool setApplyTuesday(bool applyTuesday);

    void setApplyTuesdayNoFail(bool applyTuesday);

    bool setApplyWednesday(bool applyWednesday);

    void setApplyWednesdayNoFail(bool applyWednesday);

    bool setApplyThursday(bool applyThursday);

    void setApplyThursdayNoFail(bool applyThursday);

    bool setApplyFriday(bool applyFriday);

    void setApplyFridayNoFail(bool applyFriday);

    bool setApplySaturday(bool applySaturday);

    void setApplySaturdayNoFail(bool applySaturday);

    /// Sets the start date of the date range, will delete any specific dates specified.
    /// A start date > end date indicates that the range wraps around the year.
    bool setStartDate(const openstudio::Date& date);

    /// Sets the end date of the date range, will delete any specific dates specified.
    /// A start date > end date indicates that the range wraps around the year.
    bool setEndDate(const openstudio::Date& date);

    /// Adds a specific date, will delete start and end dates.
    bool addSpecificDate(const openstudio::Date& date);

    // ensure that this object does not contain the date 2/29
    void ensureNoLeapDays();

    //@}
    /** @name Other */
    //@{

    /// Returns true if this rule covers the given date.
    bool containsDate(const openstudio::Date& date);

    /// Returns whether or not this rule covers each given date.
    std::vector<bool> containsDates(const std::vector<openstudio::Date>& dates);

    // Convenience methods
    /** Returns true if the rule applies to all days from Monday to Sunday */
    bool applyAllDays() const;
    /** Returns true if the rule applies to all weekdays Monday to Friday */
    bool applyWeekdays() const;
    /** Returns true if the rule applies to Saturday and Sunday */
    bool applyWeekends() const;

    // calls setApplyMonday to setApplySunday */
    bool setApplyAllDays(bool applyAllDays);

    // calls setApplyMonday to setApplyFriday */
    bool setApplyWeekdays(bool applyWeekdays);

    // calls setApplySaturday and setApplySunday */
    bool setApplyWeekends(bool applyWeekends);

    //@}
   protected:
    friend class detail::ScheduleRuleset_Impl;
    bool setRuleIndex(int index);

    /// @cond
    using ImplType = detail::ScheduleRule_Impl;

    explicit ScheduleRule(std::shared_ptr<detail::ScheduleRule_Impl> impl);

    friend class detail::ScheduleRule_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ScheduleRule");
  };

  /** \relates ScheduleRule */
  using OptionalScheduleRule = boost::optional<ScheduleRule>;

  /** \relates ScheduleRule */
  using ScheduleRuleVector = std::vector<ScheduleRule>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULERULE_HPP
