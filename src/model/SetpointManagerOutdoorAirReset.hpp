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

#ifndef MODEL_SETPOINTMANAGEROUTDOORAIRRESET_HPP
#define MODEL_SETPOINTMANAGEROUTDOORAIRRESET_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {

namespace model {

  class Node;
  class Schedule;

  namespace detail {

    class SetpointManagerOutdoorAirReset_Impl;

  }  // namespace detail

  /** SetpointManagerOutdoorAirReset is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:OutdoorAirReset'. */
  class MODEL_API SetpointManagerOutdoorAirReset : public SetpointManager
  {
   public:
    explicit SetpointManagerOutdoorAirReset(const Model& model);

    virtual ~SetpointManagerOutdoorAirReset() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SetpointManagerOutdoorAirReset(const SetpointManagerOutdoorAirReset& other) = default;
    SetpointManagerOutdoorAirReset(SetpointManagerOutdoorAirReset&& other) = default;
    SetpointManagerOutdoorAirReset& operator=(const SetpointManagerOutdoorAirReset&) = default;
    SetpointManagerOutdoorAirReset& operator=(SetpointManagerOutdoorAirReset&&) = default;

    static IddObjectType iddObjectType();

    static std::vector<std::string> controlVariableValues();

    boost::optional<Node> setpointNode() const;

    std::string controlVariable() const;

    bool isControlVariableDefaulted() const;

    bool setControlVariable(const std::string& controlVariable);

    void resetControlVariable();

    double setpointatOutdoorLowTemperature() const;

    bool setSetpointatOutdoorLowTemperature(double setpointatOutdoorLowTemperature);

    double outdoorLowTemperature() const;

    bool setOutdoorLowTemperature(double outdoorLowTemperature);

    double setpointatOutdoorHighTemperature() const;

    bool setSetpointatOutdoorHighTemperature(double setpointatOutdoorHighTemperature);

    double outdoorHighTemperature() const;

    bool setOutdoorHighTemperature(double outdoorHighTemperature);

    boost::optional<Schedule> schedule() const;

    bool setSchedule(Schedule& schedule);

    void resetSchedule();

    boost::optional<double> setpointatOutdoorLowTemperature2() const;

    bool setSetpointatOutdoorLowTemperature2(double setpointatOutdoorLowTemperature2);

    void resetSetpointatOutdoorLowTemperature2();

    boost::optional<double> outdoorLowTemperature2() const;

    bool setOutdoorLowTemperature2(double outdoorLowTemperature2);

    void resetOutdoorLowTemperature2();

    boost::optional<double> setpointatOutdoorHighTemperature2() const;

    bool setSetpointatOutdoorHighTemperature2(double setpointatOutdoorHighTemperature2);

    void resetSetpointatOutdoorHighTemperature2();

    boost::optional<double> outdoorHighTemperature2() const;

    bool setOutdoorHighTemperature2(double outdoorHighTemperature2);

    void resetOutdoorHighTemperature2();

   protected:
    /// @cond
    using ImplType = detail::SetpointManagerOutdoorAirReset_Impl;

    explicit SetpointManagerOutdoorAirReset(std::shared_ptr<detail::SetpointManagerOutdoorAirReset_Impl> impl);

    friend class detail::SetpointManagerOutdoorAirReset_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.SetpointManagerOutdoorAirReset");
  };

  /** \relates SetpointManagerOutdoorAirReset*/
  using OptionalSetpointManagerOutdoorAirReset = boost::optional<SetpointManagerOutdoorAirReset>;

  /** \relates SetpointManagerOutdoorAirReset*/
  using SetpointManagerOutdoorAirResetVector = std::vector<SetpointManagerOutdoorAirReset>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SETPOINTMANAGEROUTDOORAIRRESET_HPP
