/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

    virtual ~RunPeriod() = default;
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
