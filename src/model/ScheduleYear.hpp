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

#ifndef MODEL_SCHEDULEYEAR_HPP
#define MODEL_SCHEDULEYEAR_HPP

#include "ModelAPI.hpp"
#include "Schedule.hpp"

namespace openstudio {

class Date;

namespace model {

  class ScheduleWeek;

  namespace detail {

    class ScheduleYear_Impl;

  }  // namespace detail

  /** ScheduleYear is a Schedule that wraps the OpenStudio IDD object 'OS_Schedule_Year'. */
  class MODEL_API ScheduleYear : public Schedule
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ScheduleYear(const Model& model);

    virtual ~ScheduleYear() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ScheduleYear(const ScheduleYear& other) = default;
    ScheduleYear(ScheduleYear&& other) = default;
    ScheduleYear& operator=(const ScheduleYear&) = default;
    ScheduleYear& operator=(ScheduleYear&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    // DLM@20111018: This almost needs a RunPeriod as an argument to know the year description?
    /// Returns a vector of dates marking the end of week schedules.
    /// These dates will be in order and have the same number of elements as scheduleWeeks.
    std::vector<openstudio::Date> dates() const;

    /// Returns a vector of week schedules in the same order and with the same number of elements as dates.
    std::vector<ScheduleWeek> scheduleWeeks() const;

    //@}
    /** @name Setters */
    //@{

    /// Sets the given week schedule to be in effect until (inclusive) the date given starting
    /// the day after the previous date marker or January 1st if no previous date marker exists.
    /// Replaces existing scheduleWeek for same date if it exists.
    bool addScheduleWeek(const openstudio::Date& untilDate, const ScheduleWeek& scheduleWeek);

    /// Clear all week schedules from this schedule.
    void clearScheduleWeeks();

    //@}
    /** @name Other */
    //@{

    // DLM@20111018: Eventually allow "Nth day in Month" and other descriptors
    /// Returns the week schedule in effect at the given date.
    boost::optional<ScheduleWeek> getScheduleWeek(const openstudio::Date& date) const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ScheduleYear_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::ScheduleYear_Impl;

    explicit ScheduleYear(std::shared_ptr<detail::ScheduleYear_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ScheduleYear");
  };

  /** \relates ScheduleYear*/
  using OptionalScheduleYear = boost::optional<ScheduleYear>;

  /** \relates ScheduleYear*/
  using ScheduleYearVector = std::vector<ScheduleYear>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULEYEAR_HPP
