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

#ifndef MODEL_SETPOINTMANAGERSCHEDULED_HPP
#define MODEL_SETPOINTMANAGERSCHEDULED_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class SetpointManagerScheduled_Impl;

  }

  /** SetpointManagerScheduled is an interface to theIDD object
 *  named "OS:SetpointManager:Scheduled"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of the EnergyPlus SetpointManager:Scheduled object.
 */
  class MODEL_API SetpointManagerScheduled : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Constructs a new SetpointManagerScheduled object and places it inside
   *  model. Sets controlVariable to 'Temperature'. Sets setpointSchedule's
   *  ScheduleTypeLimits if necessary and possible. */
    SetpointManagerScheduled(const Model& model, Schedule& setpointSchedule);

    /** Constructs a new SetpointManagerScheduled object and places it inside
   *  model. */
    SetpointManagerScheduled(const Model& model, const std::string& controlVariable, Schedule& setpointSchedule);

    virtual ~SetpointManagerScheduled() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerScheduled(const SetpointManagerScheduled& other) = default;
    SetpointManagerScheduled(SetpointManagerScheduled&& other) = default;
    SetpointManagerScheduled& operator=(const SetpointManagerScheduled&) = default;
    SetpointManagerScheduled& operator=(SetpointManagerScheduled&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    /** @name Getters */
    //@{

    /** Returns the Node referred to by the SetpointNodeName field. **/
    boost::optional<Node> setpointNode() const;

    /** Returns the Control Variable **/
    std::string controlVariable() const;

    /** Returns the Schedule **/
    Schedule schedule() const;

    /** Returns true if this object has a schedule, it is an error if this object does not have a schedule **/
    bool hasSchedule() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the Control Variable **/
    bool setControlVariable(const std::string& controlVariable);

    /** Sets the Schedule **/
    bool setSchedule(Schedule& schedule);

    //@}
    /** @name Other */
    //@{

    /** Sets control variable and schedule simultaneously. Use in the case that the
   *  control variable change implies a change in schedule units. */
    bool setControlVariableAndSchedule(const std::string& controlVariable, Schedule& schedule);

    //@}
   protected:
    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class detail::SetpointManagerScheduled_Impl;

    /// @cond
    using ImplType = detail::SetpointManagerScheduled_Impl;

    explicit SetpointManagerScheduled(std::shared_ptr<detail::SetpointManagerScheduled_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerScheduled");
    /// @endcond
  };

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSCHEDULED_HPP
