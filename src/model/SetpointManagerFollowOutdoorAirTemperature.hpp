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

#ifndef MODEL_SETPOINTMANAGERFOLLOWOUTDOORAIRTEMPERATURE_HPP
#define MODEL_SETPOINTMANAGERFOLLOWOUTDOORAIRTEMPERATURE_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class Schedule;

  namespace detail {

    class SetpointManagerFollowOutdoorAirTemperature_Impl;

  }

  /** SetpointManagerFollowOutdoorAirTemperature is an SetpointManager that wraps the IDD object
 *  named "OS:SetpointManager:FollowOutdoorAirTemperature"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of the EnergyPlus SetpointManagerFollowOutdoorAirTemperature object.
 */
  class MODEL_API SetpointManagerFollowOutdoorAirTemperature : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Constructs a new SetpointManagerFollowOutdoorAirTemperature object and places it inside the
   *  model. */
    explicit SetpointManagerFollowOutdoorAirTemperature(const Model& model);

    virtual ~SetpointManagerFollowOutdoorAirTemperature() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerFollowOutdoorAirTemperature(const SetpointManagerFollowOutdoorAirTemperature& other) = default;
    SetpointManagerFollowOutdoorAirTemperature(SetpointManagerFollowOutdoorAirTemperature&& other) = default;
    SetpointManagerFollowOutdoorAirTemperature& operator=(const SetpointManagerFollowOutdoorAirTemperature&) = default;
    SetpointManagerFollowOutdoorAirTemperature& operator=(SetpointManagerFollowOutdoorAirTemperature&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters and Setters */
    //@{

    /** Returns the Node referred to by the SetpointNodeName field. **/
    boost::optional<Node> setpointNode() const;

    /** Returns the Control Variable **/
    std::string controlVariable() const;

    /** Sets the Control Variable **/
    bool setControlVariable(const std::string& value);

    std::string referenceTemperatureType() const;

    bool setReferenceTemperatureType(const std::string& value);

    double offsetTemperatureDifference() const;

    bool setOffsetTemperatureDifference(double value);

    double maximumSetpointTemperature() const;

    bool setMaximumSetpointTemperature(double value);

    double minimumSetpointTemperature() const;

    bool setMinimumSetpointTemperature(double value);

    //@}
   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    friend class openstudio::detail::IdfObject_Impl;

    friend class detail::SetpointManagerFollowOutdoorAirTemperature_Impl;

    /// @cond

    using ImplType = detail::SetpointManagerFollowOutdoorAirTemperature_Impl;

    explicit SetpointManagerFollowOutdoorAirTemperature(std::shared_ptr<detail::SetpointManagerFollowOutdoorAirTemperature_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerFollowOutdoorAirTemperature");

    /// @endcond
  };

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERFOLLOWOUTDOORAIRTEMPERATURE_HPP
