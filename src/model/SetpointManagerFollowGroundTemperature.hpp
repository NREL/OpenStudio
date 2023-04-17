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

#ifndef MODEL_SETPOINTMANAGERFOLLOWGROUNDTEMPERATURE_HPP
#define MODEL_SETPOINTMANAGERFOLLOWGROUNDTEMPERATURE_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class SetpointManagerFollowGroundTemperature_Impl;

  }  // namespace detail

  /** SetpointManagerFollowGroundTemperature is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:FollowGroundTemperature'. */
  class MODEL_API SetpointManagerFollowGroundTemperature : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerFollowGroundTemperature(const Model& model);

    virtual ~SetpointManagerFollowGroundTemperature() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerFollowGroundTemperature(const SetpointManagerFollowGroundTemperature& other) = default;
    SetpointManagerFollowGroundTemperature(SetpointManagerFollowGroundTemperature&& other) = default;
    SetpointManagerFollowGroundTemperature& operator=(const SetpointManagerFollowGroundTemperature&) = default;
    SetpointManagerFollowGroundTemperature& operator=(SetpointManagerFollowGroundTemperature&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    static std::vector<std::string> referenceGroundTemperatureObjectTypeValues();

    /** @name Getters */
    //@{

    std::string controlVariable() const;

    std::string referenceGroundTemperatureObjectType() const;

    double offsetTemperatureDifference() const;

    double maximumSetpointTemperature() const;

    double minimumSetpointTemperature() const;

    boost::optional<Node> setpointNode() const;

    //@}
    /** @name Setters */
    //@{

    bool setControlVariable(const std::string& controlVariable);

    bool setReferenceGroundTemperatureObjectType(const std::string& groundTemperatureObjType);

    bool setOffsetTemperatureDifference(double offsetTemperatureDifference);

    bool setMaximumSetpointTemperature(double maximumSetpointTemperature);

    bool setMinimumSetpointTemperature(double minimumSetpointTemperature);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SetpointManagerFollowGroundTemperature_Impl;

    explicit SetpointManagerFollowGroundTemperature(std::shared_ptr<detail::SetpointManagerFollowGroundTemperature_Impl> impl);

    friend class detail::SetpointManagerFollowGroundTemperature_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerFollowGroundTemperature");
  };

  /** \relates SetpointManagerFollowGroundTemperature*/
  using OptionalSetpointManagerFollowGroundTemperature = boost::optional<SetpointManagerFollowGroundTemperature>;

  /** \relates SetpointManagerFollowGroundTemperature*/
  using SetpointManagerFollowGroundTemperatureVector = std::vector<SetpointManagerFollowGroundTemperature>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGERFOLLOWGROUNDTEMPERATURE_HPP
