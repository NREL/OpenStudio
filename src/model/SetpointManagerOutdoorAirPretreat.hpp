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

#ifndef MODEL_SETPOINTMANAGEROUTDOORAIRPRETREAT_HPP
#define MODEL_SETPOINTMANAGEROUTDOORAIRPRETREAT_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class SetpointManagerOutdoorAirPretreat_Impl;

  }  // namespace detail

  /** SetpointManagerOutdoorAirPretreat is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:OutdoorAirPretreat'. */
  class MODEL_API SetpointManagerOutdoorAirPretreat : public SetpointManager
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SetpointManagerOutdoorAirPretreat(const Model& model);

    virtual ~SetpointManagerOutdoorAirPretreat() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerOutdoorAirPretreat(const SetpointManagerOutdoorAirPretreat& other) = default;
    SetpointManagerOutdoorAirPretreat(SetpointManagerOutdoorAirPretreat&& other) = default;
    SetpointManagerOutdoorAirPretreat& operator=(const SetpointManagerOutdoorAirPretreat&) = default;
    SetpointManagerOutdoorAirPretreat& operator=(SetpointManagerOutdoorAirPretreat&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    /** @name Getters */
    //@{

    std::string controlVariable() const;

    double minimumSetpointTemperature() const;

    bool isMinimumSetpointTemperatureDefaulted() const;

    double maximumSetpointTemperature() const;

    bool isMaximumSetpointTemperatureDefaulted() const;

    double minimumSetpointHumidityRatio() const;

    bool isMinimumSetpointHumidityRatioDefaulted() const;

    double maximumSetpointHumidityRatio() const;

    bool isMaximumSetpointHumidityRatioDefaulted() const;

    boost::optional<Node> referenceSetpointNode() const;

    boost::optional<Node> mixedAirStreamNode() const;

    boost::optional<Node> outdoorAirStreamNode() const;

    boost::optional<Node> returnAirStreamNode() const;

    boost::optional<Node> setpointNode() const;

    //@}
    /** @name Setters */
    //@{

    bool setControlVariable(const std::string& controlVariable);

    void resetControlVariable();

    bool setMinimumSetpointTemperature(double minimumSetpointTemperature);

    void resetMinimumSetpointTemperature();

    bool setMaximumSetpointTemperature(double maximumSetpointTemperature);

    void resetMaximumSetpointTemperature();

    bool setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio);

    void resetMinimumSetpointHumidityRatio();

    bool setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio);

    void resetMaximumSetpointHumidityRatio();

    bool setReferenceSetpointNode(const Node& node);

    void resetReferenceSetpointNode();

    bool setMixedAirStreamNode(const Node& node);

    void resetMixedAirStreamNode();

    bool setOutdoorAirStreamNode(const Node& node);

    void resetOutdoorAirStreamNode();

    bool setReturnAirStreamNode(const Node& node);

    void resetReturnAirStreamNode();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SetpointManagerOutdoorAirPretreat_Impl;

    explicit SetpointManagerOutdoorAirPretreat(std::shared_ptr<detail::SetpointManagerOutdoorAirPretreat_Impl> impl);

    friend class detail::SetpointManagerOutdoorAirPretreat_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerOutdoorAirPretreat");
  };

  /** \relates SetpointManagerOutdoorAirPretreat*/
  using OptionalSetpointManagerOutdoorAirPretreat = boost::optional<SetpointManagerOutdoorAirPretreat>;

  /** \relates SetpointManagerOutdoorAirPretreat*/
  using SetpointManagerOutdoorAirPretreatVector = std::vector<SetpointManagerOutdoorAirPretreat>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGEROUTDOORAIRPRETREAT_HPP
