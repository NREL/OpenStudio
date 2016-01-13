/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

