/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_SETPOINTMANAGERSINGLEZONEHUMIDITYMINIMUM_HPP
#define MODEL_SETPOINTMANAGERSINGLEZONEHUMIDITYMINIMUM_HPP

#include "ModelAPI.hpp"
#include "SetpointManager.hpp"

namespace openstudio {
namespace model {

class Node;
class ThermalZone;

namespace detail {

  class SetpointManagerSingleZoneHumidityMinimum_Impl;

} // detail

/** SetpointManagerSingleZoneHumidityMinimum is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:SingleZone:Humidity:Minimum'. */
class MODEL_API SetpointManagerSingleZoneHumidityMinimum : public SetpointManager {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SetpointManagerSingleZoneHumidityMinimum(const Model& model);

  virtual ~SetpointManagerSingleZoneHumidityMinimum() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> controlVariableValues();

  /** @name Getters */
  //@{

  std::string controlVariable() const;

  boost::optional<Node> setpointNode() const;

  boost::optional<ThermalZone> controlZone() const;

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
  typedef detail::SetpointManagerSingleZoneHumidityMinimum_Impl ImplType;

  explicit SetpointManagerSingleZoneHumidityMinimum(std::shared_ptr<detail::SetpointManagerSingleZoneHumidityMinimum_Impl> impl);

  friend class detail::SetpointManagerSingleZoneHumidityMinimum_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SetpointManagerSingleZoneHumidityMinimum");
};

/** \relates SetpointManagerSingleZoneHumidityMinimum*/
typedef boost::optional<SetpointManagerSingleZoneHumidityMinimum> OptionalSetpointManagerSingleZoneHumidityMinimum;

/** \relates SetpointManagerSingleZoneHumidityMinimum*/
typedef std::vector<SetpointManagerSingleZoneHumidityMinimum> SetpointManagerSingleZoneHumidityMinimumVector;

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGERSINGLEZONEHUMIDITYMINIMUM_HPP

