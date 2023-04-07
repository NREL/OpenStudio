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

    virtual ~RunPeriodControlSpecialDays() = default;
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
