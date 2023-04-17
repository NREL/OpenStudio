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

#ifndef MODEL_SCHEDULEDAY_HPP
#define MODEL_SCHEDULEDAY_HPP

#include "ModelAPI.hpp"
#include "ScheduleBase.hpp"

namespace openstudio {

class Time;

namespace model {

  class ScheduleTypeLimits;

  namespace detail {

    class ScheduleDay_Impl;

  }  // namespace detail

  /** ScheduleDay is a ScheduleBase that wraps the OpenStudio IDD object 'OS_Schedule_Day'. */
  class MODEL_API ScheduleDay : public ScheduleBase
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Initializes the schedule to a value of 0.0 until 24:00. */
    explicit ScheduleDay(const Model& model);

    /** Initializes the schedule to value until 24:00. */
    ScheduleDay(const Model& model, double value);

    virtual ~ScheduleDay() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ScheduleDay(const ScheduleDay& other) = default;
    ScheduleDay(ScheduleDay&& other) = default;
    ScheduleDay& operator=(const ScheduleDay&) = default;
    ScheduleDay& operator=(ScheduleDay&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    /** Returns true if this object does not specify a ScheduleTypeLimits directly, but rather
   *  inherits one from a Schedule. */
    bool isScheduleTypeLimitsDefaulted() const;

    bool interpolatetoTimestep() const;

    bool isInterpolatetoTimestepDefaulted() const;

    /// Returns a vector of times marking the end value intervals.
    /// These times will be in order and have the same number of elements as values.
    /// All times will be less than or equal to 1 day.
    std::vector<openstudio::Time> times() const;

    /// Returns a vector of values in the same order and with the same number of elements as times.
    std::vector<double> values() const;

    /// Returns the value in effect at the given time.  If time is less than 0 days or greater than 1 day, 0 is returned.
    double getValue(const openstudio::Time& time) const;

    //@}
    /** @name Setters */
    //@{

    bool setInterpolatetoTimestep(bool interpolatetoTimestep);

    void resetInterpolatetoTimestep();

    /** Returns false if time is less than 0 days or greater than 1 day.  Replaces existing value
   *  for same time. */
    bool addValue(const openstudio::Time& untilTime, double value);

    /** Remove a value added by addValue at the exact time.  Returns the removed
   *  value if there was one. */
    boost::optional<double> removeValue(const openstudio::Time& time);

    /// Clear all values from this schedule.
    void clearValues();

    //@}
   protected:
    /// @cond
    using ImplType = detail::ScheduleDay_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class detail::ScheduleDay_Impl;

    explicit ScheduleDay(std::shared_ptr<detail::ScheduleDay_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ScheduleDay");
  };

  /** \relates ScheduleDay*/
  using OptionalScheduleDay = boost::optional<ScheduleDay>;

  /** \relates ScheduleDay*/
  using ScheduleDayVector = std::vector<ScheduleDay>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULEDAY_HPP
