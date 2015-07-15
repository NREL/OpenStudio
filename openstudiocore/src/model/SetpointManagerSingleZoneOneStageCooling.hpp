/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_SETPOINTMANAGERSINGLEZONEONESTAGECOOLING_HPP
#define MODEL_SETPOINTMANAGERSINGLEZONEONESTAGECOOLING_HPP

#include <model/ModelAPI.hpp>
#include <model/SetpointManager.hpp>

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class ThermalZone;
class Node;

namespace detail {

  class SetpointManagerSingleZoneOneStageCooling_Impl;

} // detail

/** SetpointManagerSingleZoneOneStageCooling is a SetpointManager that wraps the OpenStudio IDD object 'OS:SetpointManager:SingleZone:OneStageCooling'. */
class MODEL_API SetpointManagerSingleZoneOneStageCooling : public SetpointManager {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SetpointManagerSingleZoneOneStageCooling(const Model& model);

  virtual ~SetpointManagerSingleZoneOneStageCooling() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> controlVariableValues();

  /** @name Getters */
  //@{

  std::string controlVariable() const;

  double coolingStageOnSupplyAirSetpointTemperature() const;

  double coolingStageOffSupplyAirSetpointTemperature() const;

  // TODO: Check return type. From object lists, some candidates are: ThermalZone.
  boost::optional<ThermalZone> controlZone() const;

  // TODO: Check return type. From object lists, some candidates are: Node.
  boost::optional<Node> setpointNodeorNodeList() const;

  //@}
  /** @name Setters */
  //@{

  bool setControlVariable(std::string controlVariable);

  void setCoolingStageOnSupplyAirSetpointTemperature(double coolingStageOnSupplyAirSetpointTemperature);

  void setCoolingStageOffSupplyAirSetpointTemperature(double coolingStageOffSupplyAirSetpointTemperature);

  // TODO: Check argument type. From object lists, some candidates are: ThermalZone.
  bool setControlZone(const ThermalZone& thermalZone);

  void resetControlZone();

  // TODO: Check argument type. From object lists, some candidates are: Node.
  bool setSetpointNodeorNodeList(const Node& node);

  void resetSetpointNodeorNodeList();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::SetpointManagerSingleZoneOneStageCooling_Impl ImplType;

  explicit SetpointManagerSingleZoneOneStageCooling(std::shared_ptr<detail::SetpointManagerSingleZoneOneStageCooling_Impl> impl);

  friend class detail::SetpointManagerSingleZoneOneStageCooling_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SetpointManagerSingleZoneOneStageCooling");
};

/** \relates SetpointManagerSingleZoneOneStageCooling*/
typedef boost::optional<SetpointManagerSingleZoneOneStageCooling> OptionalSetpointManagerSingleZoneOneStageCooling;

/** \relates SetpointManagerSingleZoneOneStageCooling*/
typedef std::vector<SetpointManagerSingleZoneOneStageCooling> SetpointManagerSingleZoneOneStageCoolingVector;

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGERSINGLEZONEONESTAGECOOLING_HPP

