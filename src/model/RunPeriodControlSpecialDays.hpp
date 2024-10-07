/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_RUNPERIODCONTROLSPECIALDAYS_HPP
#define MODEL_RUNPERIODCONTROLSPECIALDAYS_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

class Date;
class MonthOfYear;
class NthDayOfWeekInMonth;
class DayOfWeek;

namespace model {

  namespace detail {
    class RunPeriodControlSpecialDays_Impl;
  }  // namespace detail

  /** RunPeriodControlSpecialDays derives from ModelObject and is an interface to the OpenStudio IDD object named "OS:RunPeriodControl:SpecialDays".
 */
  class MODEL_API RunPeriodControlSpecialDays : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Constructor with string, will throw if incorrect format. */
    RunPeriodControlSpecialDays(const std::string& startDate, Model& model);

    /** Constructor with month of year and day, e.g. Jan, 1. */
    RunPeriodControlSpecialDays(const openstudio::MonthOfYear& monthOfYear, unsigned day, Model& model);

    /** Constructor from nth day in month, e.g. 1st Friday in January. */
    RunPeriodControlSpecialDays(const openstudio::NthDayOfWeekInMonth& nth, const openstudio::DayOfWeek& dayOfWeek,
                                const openstudio::MonthOfYear& monthOfYear, Model& model);

    virtual ~RunPeriodControlSpecialDays() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RunPeriodControlSpecialDays(const RunPeriodControlSpecialDays& other) = default;
    RunPeriodControlSpecialDays(RunPeriodControlSpecialDays&& other) = default;
    RunPeriodControlSpecialDays& operator=(const RunPeriodControlSpecialDays&) = default;
    RunPeriodControlSpecialDays& operator=(RunPeriodControlSpecialDays&&) = default;

    /** @name Static Methods */
    //@{

    static std::vector<std::string> specialDayTypeValues();

    // Forwards to specialDayTypeValues()
    static std::vector<std::string> validSpecialDayTypeValues();

    //@}
    /** @name Getters */
    //@{

    openstudio::Date startDate() const;
    unsigned duration() const;
    std::string specialDayType() const;

    //@}
    /** @name Setters */
    //@{

    bool setStartDate(const std::string& startDate);
    bool setStartDate(const openstudio::MonthOfYear& monthOfYear, unsigned day);
    bool setStartDate(const openstudio::NthDayOfWeekInMonth& nth, const openstudio::DayOfWeek& dayOfWeek, const openstudio::MonthOfYear& monthOfYear);
    bool setDuration(unsigned duration);
    bool setSpecialDayType(const std::string& specialDayType);

    // ensure that this object does not contain the date 2/29
    void ensureNoLeapDays();

    //@}

    /// Returns the IddObjectType.
    static IddObjectType iddObjectType();

   protected:
    /// @cond

    using ImplType = detail::RunPeriodControlSpecialDays_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    // constructor
    explicit RunPeriodControlSpecialDays(std::shared_ptr<detail::RunPeriodControlSpecialDays_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.RunPeriodControlSpecialDays");

    /// @endcond
  };

  /** \relates RunPeriodControlSpecialDays */
  using OptionalRunPeriodControlSpecialDays = boost::optional<RunPeriodControlSpecialDays>;

  /** \relates RunPeriodControlSpecialDays */
  using RunPeriodControlSpecialDaysVector = std::vector<RunPeriodControlSpecialDays>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_RUNPERIODCONTROLSPECIALDAYS_HPP
