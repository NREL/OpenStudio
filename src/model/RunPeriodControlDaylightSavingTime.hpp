/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_RUNPERIODCONTROLDAYLIGHTSAVINGTIME_HPP
#define MODEL_RUNPERIODCONTROLDAYLIGHTSAVINGTIME_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

class Date;
class MonthOfYear;
class NthDayOfWeekInMonth;
class DayOfWeek;

namespace model {

  namespace detail {

    class RunPeriodControlDaylightSavingTime_Impl;

  }  // namespace detail

  /** RunPeriodControlDaylightSavingTime is a ModelObject that wraps the OpenStudio IDD object 'OS_RunPeriodControl_DaylightSavingTime'. */
  class MODEL_API RunPeriodControlDaylightSavingTime : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~RunPeriodControlDaylightSavingTime() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RunPeriodControlDaylightSavingTime(const RunPeriodControlDaylightSavingTime& other) = default;
    RunPeriodControlDaylightSavingTime(RunPeriodControlDaylightSavingTime&& other) = default;
    RunPeriodControlDaylightSavingTime& operator=(const RunPeriodControlDaylightSavingTime&) = default;
    RunPeriodControlDaylightSavingTime& operator=(RunPeriodControlDaylightSavingTime&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    openstudio::Date startDate() const;

    boost::optional<openstudio::NthDayOfWeekInMonth> startNthDayOfWeekInMonth() const;

    openstudio::Date endDate() const;

    boost::optional<openstudio::NthDayOfWeekInMonth> endNthDayOfWeekInMonth() const;

    //@}
    /** @name Setters */
    //@{

    bool setStartDate(const std::string& startDate);
    bool setStartDate(const openstudio::MonthOfYear& monthOfYear, unsigned day);
    bool setStartDate(const openstudio::NthDayOfWeekInMonth& nth, const openstudio::DayOfWeek& dayOfWeek, const openstudio::MonthOfYear& monthOfYear);

    bool setEndDate(const std::string& endDate);
    bool setEndDate(const openstudio::MonthOfYear& monthOfYear, unsigned day);
    bool setEndDate(const openstudio::NthDayOfWeekInMonth& nth, const openstudio::DayOfWeek& dayOfWeek, const openstudio::MonthOfYear& monthOfYear);

    // ensure that this object does not contain the date 2/29
    void ensureNoLeapDays();

    //@}
   protected:
    /// @cond
    using ImplType = detail::RunPeriodControlDaylightSavingTime_Impl;

    friend class detail::RunPeriodControlDaylightSavingTime_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    explicit RunPeriodControlDaylightSavingTime(std::shared_ptr<detail::RunPeriodControlDaylightSavingTime_Impl> impl);

    explicit RunPeriodControlDaylightSavingTime(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.RunPeriodControlDaylightSavingTime");
  };

  /** \relates RunPeriodControlDaylightSavingTime*/
  using OptionalRunPeriodControlDaylightSavingTime = boost::optional<RunPeriodControlDaylightSavingTime>;

  /** \relates RunPeriodControlDaylightSavingTime*/
  using RunPeriodControlDaylightSavingTimeVector = std::vector<RunPeriodControlDaylightSavingTime>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_RUNPERIODCONTROLDAYLIGHTSAVINGTIME_HPP
