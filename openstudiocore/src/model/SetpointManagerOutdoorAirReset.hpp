/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

class Quantity;
class OSOptionalQuantity;

namespace model {

class Node;
class Schedule;

namespace detail {

  class SetpointManagerOutdoorAirReset_Impl;

} // detail

/** SetpointManagerOutdoorAirReset is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:OutdoorAirReset'. */
class MODEL_API SetpointManagerOutdoorAirReset : public SetpointManager {
  public:

  explicit SetpointManagerOutdoorAirReset(const Model& model);

  virtual ~SetpointManagerOutdoorAirReset() {}

  static IddObjectType iddObjectType();

  static std::vector<std::string> controlVariableValues();


  boost::optional<Node> setpointNode() const;


  std::string controlVariable() const;

  bool isControlVariableDefaulted() const;

  bool setControlVariable(const std::string& controlVariable);

  void resetControlVariable();


  double setpointatOutdoorLowTemperature() const;

  Quantity getSetpointatOutdoorLowTemperature(bool returnIP=false) const;

  bool setSetpointatOutdoorLowTemperature(double setpointatOutdoorLowTemperature);

  bool setSetpointatOutdoorLowTemperature(const Quantity& setpointatOutdoorLowTemperature);


  double outdoorLowTemperature() const;

  Quantity getOutdoorLowTemperature(bool returnIP=false) const;

  bool setOutdoorLowTemperature(double outdoorLowTemperature);

  bool setOutdoorLowTemperature(const Quantity& outdoorLowTemperature);


  double setpointatOutdoorHighTemperature() const;

  Quantity getSetpointatOutdoorHighTemperature(bool returnIP=false) const;

  bool setSetpointatOutdoorHighTemperature(double setpointatOutdoorHighTemperature);

  bool setSetpointatOutdoorHighTemperature(const Quantity& setpointatOutdoorHighTemperature);


  double outdoorHighTemperature() const;

  Quantity getOutdoorHighTemperature(bool returnIP=false) const;

  bool setOutdoorHighTemperature(double outdoorHighTemperature);

  bool setOutdoorHighTemperature(const Quantity& outdoorHighTemperature);


  boost::optional<Schedule> schedule() const;

  bool setSchedule(Schedule& schedule);

  void resetSchedule();


  boost::optional<double> setpointatOutdoorLowTemperature2() const;

  OSOptionalQuantity getSetpointatOutdoorLowTemperature2(bool returnIP=false) const;

  bool setSetpointatOutdoorLowTemperature2(double setpointatOutdoorLowTemperature2);

  bool setSetpointatOutdoorLowTemperature2(const Quantity& setpointatOutdoorLowTemperature2);

  void resetSetpointatOutdoorLowTemperature2();


  boost::optional<double> outdoorLowTemperature2() const;

  OSOptionalQuantity getOutdoorLowTemperature2(bool returnIP=false) const;

  bool setOutdoorLowTemperature2(double outdoorLowTemperature2);

  bool setOutdoorLowTemperature2(const Quantity& outdoorLowTemperature2);

  void resetOutdoorLowTemperature2();


  boost::optional<double> setpointatOutdoorHighTemperature2() const;

  OSOptionalQuantity getSetpointatOutdoorHighTemperature2(bool returnIP=false) const;

  bool setSetpointatOutdoorHighTemperature2(double setpointatOutdoorHighTemperature2);

  bool setSetpointatOutdoorHighTemperature2(const Quantity& setpointatOutdoorHighTemperature2);

  void resetSetpointatOutdoorHighTemperature2();


  boost::optional<double> outdoorHighTemperature2() const;

  OSOptionalQuantity getOutdoorHighTemperature2(bool returnIP=false) const;

  bool setOutdoorHighTemperature2(double outdoorHighTemperature2);

  bool setOutdoorHighTemperature2(const Quantity& outdoorHighTemperature2);

  void resetOutdoorHighTemperature2();

 protected:

  /// @cond
  typedef detail::SetpointManagerOutdoorAirReset_Impl ImplType;

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
typedef boost::optional<SetpointManagerOutdoorAirReset> OptionalSetpointManagerOutdoorAirReset;

/** \relates SetpointManagerOutdoorAirReset*/
typedef std::vector<SetpointManagerOutdoorAirReset> SetpointManagerOutdoorAirResetVector;

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGEROUTDOORAIRRESET_HPP
