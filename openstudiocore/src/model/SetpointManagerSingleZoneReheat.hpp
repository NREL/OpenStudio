/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef MODEL_SETPOINTMANAGERSINGLEZONEREHEAT_HPP
#define MODEL_SETPOINTMANAGERSINGLEZONEREHEAT_HPP

#include <model/ModelAPI.hpp>
#include <model/HVACComponent.hpp>

namespace openstudio {

namespace model {

class ThermalZone;

namespace detail {

  class SetpointManagerSingleZoneReheat_Impl;

}

/** SetpointManagerSingleZoneReheat is an interface to theIDD object
 *  named "OS:SetpointManager:SingleZone:Reheat"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  of the EnergyPlus SetpointManager:SingleZone:Reheat object.
 */
class MODEL_API SetpointManagerSingleZoneReheat : public HVACComponent {
  
  public:
  virtual ~SetpointManagerSingleZoneReheat() {}

  /** Constructs a new SetpointManagerSingleZoneReheat object and places it inside the
   *  model.  The loop is fully initialized with all companion objects.
   */
  explicit SetpointManagerSingleZoneReheat(const Model& model);

  /** Returns the value of the MinimumSupplyAirTemperature field. **/
  double minimumSupplyAirTemperature();

  /** Sets the value of the MinimumSupplyAirTemperature field. **/
  void setMinimumSupplyAirTemperature( double value );

  /** Returns the value of the MaximumSupplyAirTemperature field. **/
  double maximumSupplyAirTemperature();

  /** Sets the value of the MaximumSupplyAirTemperature field. **/
  void setMaximumSupplyAirTemperature( double value );

  /** Returns the Node referred to by the SetpointNodeName field. **/
  boost::optional<Node> setpointNode();

  /** Sets the Node referred to by the SetpointNodeName field. **/
  void setSetpointNode( Node & node );

  boost::optional<ThermalZone> controlZone();

  void setControlZone(ThermalZone & thermalZone);

  void resetControlZone();

  virtual bool addToNode(Node & node);

  virtual std::vector<openstudio::IdfObject> remove();

  virtual ModelObject clone(Model model) const;

  static IddObjectType iddObjectType();

  protected:

  friend class Model;

  friend class openstudio::IdfObject;

  friend class openstudio::detail::IdfObject_Impl;

  friend class detail::SetpointManagerSingleZoneReheat_Impl;

  /// @cond

  typedef detail::SetpointManagerSingleZoneReheat_Impl ImplType;

  explicit SetpointManagerSingleZoneReheat(boost::shared_ptr<detail::SetpointManagerSingleZoneReheat_Impl> impl);

  private:

  REGISTER_LOGGER("openstudio.model.SetpointManagerSingleZoneReheat");

  /// @endcond

};

typedef boost::optional<SetpointManagerSingleZoneReheat> OptionalSetpointManagerSingleZoneReheat;

typedef std::vector<SetpointManagerSingleZoneReheat> SetpointManagerSingleZoneReheatVector;

} // model

} // openstudio

#endif // MODEL_SETPOINTMANAGERSINGLEZONEREHEAT_HPP

