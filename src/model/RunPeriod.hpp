/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_RUNPERIOD_HPP
#define MODEL_RUNPERIOD_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class RunPeriod_Impl;
  }  // namespace detail

  /** RunPeriod derives from ParentObject and is an interface to the unique OpenStudio IDD object named "RunPeriod".
 *
 *  RunPeriod defines a period of time over which to run an EnergyPlus weather file simulation.  Some applications
 *  require a full annual EnergyPlus simulation, others do not.
 */
  class MODEL_API RunPeriod : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~RunPeriod() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RunPeriod(const RunPeriod& other) = default;
    RunPeriod(RunPeriod&& other) = default;
    RunPeriod& operator=(const RunPeriod&) = default;
    RunPeriod& operator=(RunPeriod&&) = default;

    //@}
    /** @name Getters */
    //@{

    // DLM@20110607: these should all have 'get' removed
    // DLM@20110614: I would prefer to not abbreviate names like 'NumTimePeriodRepeats'

    int getBeginMonth() const;
    int getBeginDayOfMonth() const;
    int getEndMonth() const;
    int getEndDayOfMonth() const;
    bool getUseWeatherFileHolidays() const;
    bool getUseWeatherFileDaylightSavings() const;
    bool getApplyWeekendHolidayRule() const;
    bool getUseWeatherFileRainInd() const;
    bool getUseWeatherFileSnowInd() const;
    int getNumTimePeriodRepeats() const;

    //@}
    /** @name Setters */
    //@{

    // DLM@20110607: should all these sets return bool?

    bool setBeginMonth(int month);
    bool setBeginDayOfMonth(int day);
    bool setEndMonth(int month);
    bool setEndDayOfMonth(int day);
    bool setUseWeatherFileHolidays(bool use);
    bool setUseWeatherFileDaylightSavings(bool use);
    bool setApplyWeekendHolidayRule(bool apply);
    bool setUseWeatherFileRainInd(bool rainInd);
    bool setUseWeatherFileSnowInd(bool snowInd);
    bool setNumTimePeriodRepeats(int numRepeats);

    // ensure that this object does not contain the date 2/29
    void ensureNoLeapDays();

    //@}

    /// Returns the IddObjectType.
    static IddObjectType iddObjectType();

    /// Returns true if RunPeriod specifies run between 1/1-12/31.
    bool isAnnual() const;

    /// Returns true if not annual simulation.
    bool isPartialYear() const;

    /// Returns true if numTimePeriodRepeats is greater than 1.
    bool isRepeated() const;

   protected:
    /// @cond

    using ImplType = detail::RunPeriod_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    // constructor
    explicit RunPeriod(std::shared_ptr<detail::RunPeriod_Impl> impl);

    /** Constructor adds a new RunPeriod object to model. */
    explicit RunPeriod(const Model& model);

   private:
    REGISTER_LOGGER("openstudio.model.RunPeriod");

    /// @endcond
  };

  /** \relates RunPeriod */
  using OptionalRunPeriod = boost::optional<RunPeriod>;

  /** \relates RunPeriod */
  using RunPeriodVector = std::vector<RunPeriod>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_RUNPERIOD_HPP
