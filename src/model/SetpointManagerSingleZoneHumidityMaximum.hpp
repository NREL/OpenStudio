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

#ifndef MODEL_SETPOINTMANAGERSINGLEZONEHUMIDITYMAXIMUM_HPP
#define MODEL_SETPOINTMANAGERSINGLEZONEHUMIDITYMAXIMUM_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  class Node;

  namespace detail {

    class SetpointManagerSingleZoneHumidityMaximum_Impl;

  }  // namespace detail

  /** SetpointManagerSingleZoneHumidityMaximum is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:SingleZone:Humidity:Maximum'. */
  class MODEL_API SetpointManagerSingleZoneHumidityMaximum : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerSingleZoneHumidityMaximum(const Model& model);

    virtual ~SetpointManagerSingleZoneHumidityMaximum() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerSingleZoneHumidityMaximum(const SetpointManagerSingleZoneHumidityMaximum& other) = default;
    SetpointManagerSingleZoneHumidityMaximum(SetpointManagerSingleZoneHumidityMaximum&& other) = default;
    SetpointManagerSingleZoneHumidityMaximum& operator=(const SetpointManagerSingleZoneHumidityMaximum&) = default;
    SetpointManagerSingleZoneHumidityMaximum& operator=(SetpointManagerSingleZoneHumidityMaximum&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    /** @name Getters */
    //@{

    std::string controlVariable() const;

    boost::optional<ThermalZone> controlZone() const;

    boost::optional<Node> setpointNode() const;

    //@}
    /** @name Setters */
    //@{

    bool setControlVariable(const std::string& controlVariable);

    bool setControlZone(const ThermalZone& thermalZone);

    void resetControlZone();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SetpointManagerSingleZoneHumidityMaximum_Impl;

    explicit SetpointManagerSingleZoneHumidityMaximum(std::shared_ptr<detail::SetpointManagerSingleZoneHumidityMaximum_Impl> impl);

    friend class detail::SetpointManagerSingleZoneHumidityMaximum_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerSingleZoneHumidityMaximum");
  };

  /** \relates SetpointManagerSingleZoneHumidityMaximum*/
  using OptionalSetpointManagerSingleZoneHumidityMaximum = boost::optional<SetpointManagerSingleZoneHumidityMaximum>;

  /** \relates SetpointManagerSingleZoneHumidityMaximum*/
  using SetpointManagerSingleZoneHumidityMaximumVector = std::vector<SetpointManagerSingleZoneHumidityMaximum>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERSINGLEZONEHUMIDITYMAXIMUM_HPP
