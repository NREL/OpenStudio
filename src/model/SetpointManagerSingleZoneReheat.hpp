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

#ifndef MODEL_SETPOINTMANAGERSINGLEZONEREHEAT_HPP
#define MODEL_SETPOINTMANAGERSINGLEZONEREHEAT_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class ThermalZone;

  namespace detail {

    class SetpointManagerSingleZoneReheat_Impl;

  }

  /** SetpointManagerSingleZoneReheat is an interface to theIDD object
 *  named "OS:SetpointManager:SingleZone:Reheat"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of the EnergyPlus SetpointManager:SingleZone:Reheat object.
 */
  class MODEL_API SetpointManagerSingleZoneReheat : public SetpointManager
  {

   public:
    virtual ~SetpointManagerSingleZoneReheat() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerSingleZoneReheat(const SetpointManagerSingleZoneReheat& other) = default;
    SetpointManagerSingleZoneReheat(SetpointManagerSingleZoneReheat&& other) = default;
    SetpointManagerSingleZoneReheat& operator=(const SetpointManagerSingleZoneReheat&) = default;
    SetpointManagerSingleZoneReheat& operator=(SetpointManagerSingleZoneReheat&&) = default;

    /** Constructs a new SetpointManagerSingleZoneReheat object and places it inside the
   *  model.  The loop is fully initialized with all companion objects.
   */
    explicit SetpointManagerSingleZoneReheat(const Model& model);

    /** Returns the value of the MinimumSupplyAirTemperature field. **/
    double minimumSupplyAirTemperature();

    /** Sets the value of the MinimumSupplyAirTemperature field. **/
    bool setMinimumSupplyAirTemperature(double value);

    /** Returns the value of the MaximumSupplyAirTemperature field. **/
    double maximumSupplyAirTemperature();

    /** Sets the value of the MaximumSupplyAirTemperature field. **/
    bool setMaximumSupplyAirTemperature(double value);

    /** Returns the Node referred to by the SetpointNodeName field. **/
    boost::optional<Node> setpointNode() const;

    std::string controlVariable() const;

    bool setControlVariable(const std::string& controlVariable);

    boost::optional<ThermalZone> controlZone();

    bool setControlZone(ThermalZone& thermalZone);

    void resetControlZone();

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    friend class openstudio::detail::IdfObject_Impl;

    friend class detail::SetpointManagerSingleZoneReheat_Impl;

    /// @cond

    using ImplType = detail::SetpointManagerSingleZoneReheat_Impl;

    explicit SetpointManagerSingleZoneReheat(std::shared_ptr<detail::SetpointManagerSingleZoneReheat_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerSingleZoneReheat");

    /// @endcond
  };

  using OptionalSetpointManagerSingleZoneReheat = boost::optional<SetpointManagerSingleZoneReheat>;

  using SetpointManagerSingleZoneReheatVector = std::vector<SetpointManagerSingleZoneReheat>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSINGLEZONEREHEAT_HPP
