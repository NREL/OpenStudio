/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#ifndef MODEL_SETPOINTMANAGERSINGLEZONEHUMIDITYMINIMUM_HPP
#define MODEL_SETPOINTMANAGERSINGLEZONEHUMIDITYMINIMUM_HPP

#include <model/ModelAPI.hpp>
#include <model/SetpointManager.hpp>

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
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

  bool isControlVariableDefaulted() const;

  // TODO: Check return type. From object lists, some candidates are: Node.
  Node setpointNodeorNodeList() const;

  // TODO: Check return type. From object lists, some candidates are: ThermalZone.
  ThermalZone controlZone() const;

  //@}
  /** @name Setters */
  //@{

  bool setControlVariable(std::string controlVariable);

  void resetControlVariable();

  // TODO: Check argument type. From object lists, some candidates are: Node.
  bool setSetpointNodeorNodeList(const Node& node);

  // TODO: Check argument type. From object lists, some candidates are: ThermalZone.
  bool setControlZone(const ThermalZone& thermalZone);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::SetpointManagerSingleZoneHumidityMinimum_Impl ImplType;

  explicit SetpointManagerSingleZoneHumidityMinimum(boost::shared_ptr<detail::SetpointManagerSingleZoneHumidityMinimum_Impl> impl);

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

