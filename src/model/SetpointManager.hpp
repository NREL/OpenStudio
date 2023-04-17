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

#ifndef MODEL_SETPOINTMANAGER_HPP
#define MODEL_SETPOINTMANAGER_HPP

#include "ModelAPI.hpp"
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class SetpointManager_Impl;

  }

  /** SetpointManager is the base class for setpoint managers.
 *
 */
  class MODEL_API SetpointManager : public HVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~SetpointManager() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManager(const SetpointManager& other) = default;
    SetpointManager(SetpointManager&& other) = default;
    SetpointManager& operator=(const SetpointManager&) = default;
    SetpointManager& operator=(SetpointManager&&) = default;

    //@}

    /** @name Getters */
    //@{

    /** Returns the Node referred to by the SetpointNodeName field. **/
    boost::optional<Node> setpointNode() const;

    /** Returns the Control Variable **/
    std::string controlVariable() const;

    /** Returns whether this SPM is allowed to be placed on a PlantLoop
   * (*all* SPMs are allowed on an AirLoopHVAC, *some* are allowed on a PlantLoop) **/
    bool isAllowedOnPlantLoop() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the Control Variable **/
    bool setControlVariable(const std::string& controlVariable);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /** Constructs a new SetpointManager object and places it inside
   *  model. */
    SetpointManager(IddObjectType type, const Model& model);

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class detail::SetpointManager_Impl;

    /// @cond
    using ImplType = detail::SetpointManager_Impl;

    explicit SetpointManager(std::shared_ptr<detail::SetpointManager_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.SetpointManager");
    /// @endcond
  };

  using OptionalSetpointManager = boost::optional<SetpointManager>;
  using SetpointManagerVector = std::vector<SetpointManager>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGER_HPP
