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

#ifndef MODEL_SCHEDULE_HPP
#define MODEL_SCHEDULE_HPP

#include "ModelAPI.hpp"
#include "ScheduleBase.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class Schedule_Impl;
  }

  /** Schedule is a ScheduleBase abstract class that represents the reference list 'ScheduleNames'.
 *  The 'ScheduleNames' reference list includes objects of type:
 *     \li OS:Schedule:Year
 *     \li OS:Schedule:Compact
 *     \li OS:Schedule:Constant
 *     \li OS:Schedule:Ruleset
 *     \li OS:Schedule:FixedInterval
 *     \li OS:Schedule:VariableInterval
 *
 *  For detailed information on ScheduleTypeLimits, please see ScheduleBase and
 *  \link ScheduleTypeRegistrySingleton ScheduleTypeRegistry \endlink. In general, ModelObject
 *  setters that take a Schedule will fail if the ScheduleTypeLimits are set and are incompatible
 *  with the proposed use. This ensures that the Schedule is written using units and bounds
 *  expected by the user object (e.g. fractional for lighting schedules, W for people activity
 *  level schedules, C or F for thermostat schedules). */
  class MODEL_API Schedule : public ScheduleBase
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~Schedule() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Schedule(const Schedule& other) = default;
    Schedule(Schedule&& other) = default;
    Schedule& operator=(const Schedule&) = default;
    Schedule& operator=(Schedule&&) = default;

    //@}
   protected:
    /// @cond
    using ImplType = detail::Schedule_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    // constructor
    explicit Schedule(std::shared_ptr<detail::Schedule_Impl> impl);

    Schedule(IddObjectType type, const Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.Schedule");
  };

  // optional Schedule
  using OptionalSchedule = boost::optional<Schedule>;

  // vector of Schedule
  using ScheduleVector = std::vector<Schedule>;

}  // namespace model
}  // namespace openstudio

#endif
