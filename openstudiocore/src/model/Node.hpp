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

#ifndef MODEL_NODE_HPP
#define MODEL_NODE_HPP

#include <model/ModelAPI.hpp>
#include <model/ModelObject.hpp>
#include <model/StraightComponent.hpp>
#include <model/Connection.hpp>

namespace openstudio {
namespace model {

class SetpointManagerSingleZoneReheat;

class SetpointManagerMixedAir;

class SetpointManagerScheduled;

class SetpointManagerFollowOutdoorAirTemperature;

class SetpointManagerOutdoorAirReset;

class SetpointManagerWarmest;

class SetpointManager;

namespace detail {
  class Node_Impl;
} // detail

/** Node is a ModelObject that defines a point in an EnergyPlus HVAC topology where fluid conditions are known and
 *  control devices can be applied.  EnergyPlus IDD does not define a Node object, however the concept of a node
 *  is central to modeling HVAC systems in EnergyPlus. 
 */
class MODEL_API Node : public StraightComponent {

 public:

  /** Constructs a new Node object and places it inside the 
   *  model.  The object is fully initialized with all companion objects.
   */
  explicit Node(const Model& model);

  virtual ~Node() {}

  std::vector<SetpointManager> setpointManagers() const;

  /** DEPRECATED - use ::addToNode() in setpoint manager object
    * Adds setPointManager of type SetpointManagerSingleZoneReheat to this Node. **/
  void addSetpointManager( SetpointManagerSingleZoneReheat & setPointManager );

  /** DEPRECATED - use ::remove() in setpoint manager object
    * Removes any SetpointManagerSingleZoneReheat objects from this Node. **/
  void removeSetpointManagerSingleZoneReheat();

  /** DEPRECATED - use Node::setpointManagers()
    * Returns the optional SetpointManagerSingleZoneReheat associated with this Node.  **/
  boost::optional<SetpointManagerSingleZoneReheat> getSetpointManagerSingleZoneReheat();

  /** DEPRECATED - use ::addToNode() in setpoint manager object
    * Adds setPointManager of type SetpointManagerMixedAir to this Node. **/
  void addSetpointManager( SetpointManagerMixedAir & setPointManager );

  /** DEPRECATED - use ::remove() in setpoint manager object
    * Removes any SetpointManagerMixedAir objects from this Node. **/
  void removeSetpointManagerMixedAir();

  /** DEPRECATED - use Node::setpointManagers()
    * Returns the optional SetpointManagerMixedAir associated with this Node.  **/
  boost::optional<SetpointManagerMixedAir> getSetpointManagerMixedAir();

  /** DEPRECATED - use Node::setpointManagers()
    * Returns the optional SetpointManagerScheduled associated with this Node.  **/
  boost::optional<SetpointManagerScheduled> setpointManagerScheduled() const;

  /** DEPRECATED - use ::addToNode() in setpoint manager object
    * Adds setPointManager of type SetpointManagerScheduled to this Node. **/
  void addSetpointManager( SetpointManagerScheduled & setPointManager );

  /** DEPRECATED - use ::remove() in setpoint manager object
    * Removes any SetpointManagerScheduled objects from this Node. **/
  void removeSetpointManagerScheduled();

  /** DEPRECATED - use Node::setpointManagers()
    * Returns the optional SetpointManagerFollowOutdoorAirTemperature associated with this Node.  **/
  boost::optional<SetpointManagerFollowOutdoorAirTemperature> setpointManagerFollowOutdoorAirTemperature() const;

  /** DEPRECATED - use ::addToNode() in setpoint manager object
    * Adds setPointManager of type SetpointManagerFollowOutdoorAirTemperature to this Node. **/
  void addSetpointManager( SetpointManagerFollowOutdoorAirTemperature & setPointManager );

  /** DEPRECATED - use ::remove() in setpoint manager object
    * Removes any SetpointManagerFollowOutdoorAirTemperature objects from this Node. **/
  void removeSetpointManagerFollowOutdoorAirTemperature();

  /** DEPRECATED - use ::addToNode() in setpoint manager object
    * Adds setPointManager of type SetpointManagerOutdoorAirReset to this Node. **/
  void addSetpointManager( SetpointManagerOutdoorAirReset & setPointManager );

  /** DEPRECATED - use ::remove() in setpoint manager object
    * Removes any SetpointManagerOutdoorAirReset objects from this Node. **/
  void removeSetpointManagerOutdoorAirReset();

  /** DEPRECATED - use Node::setpointManagers()
    * Returns the optional SetpointManagerOutdoorAirReset associated with this Node.  **/
  boost::optional<SetpointManagerOutdoorAirReset> setpointManagerOutdoorAirReset() const;

  /** DEPRECATED - use ::remove() in setpoint manager object
    * Removes any SetpointManagerWarmest objects from this Node. **/
  void removeSetpointManagerWarmest();

  /** DEPRECATED - use Node::setpointManagers()
    * Returns the optional SetpointManagerWarmest associated with this Node.  **/
  boost::optional<SetpointManagerWarmest> setpointManagerWarmest() const;

  /** DEPRECATED - use ::addToNode() in setpoint manager object
    * Adds setPointManager of type SetpointManagerWarmest to this Node. **/
  void addSetpointManagerWarmest( SetpointManagerWarmest & setPointManager );

  bool addToNode(Node & node);

  ModelObject clone(Model model) const;

  bool isRemovable() const;

  std::vector<IdfObject> remove();

  static IddObjectType iddObjectType();

 protected:

  friend class Model;

  friend class openstudio::IdfObject;

  friend class openstudio::detail::IdfObject_Impl;

  /// @cond 

  typedef detail::Node_Impl ImplType;

  explicit Node(boost::shared_ptr<detail::Node_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.Node");

  /// @endcond 

};

/** \relates Node */
typedef boost::optional<Node> OptionalNode;

/** \relates Node */
typedef std::vector<Node> NodeVector;

} // model
} // openstudio

#endif // MODEL_Node_HPP

