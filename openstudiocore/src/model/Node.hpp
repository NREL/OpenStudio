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

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"
#include "../utilities/core/Deprecated.hpp"

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

  /** \deprecated Node::addSetpointManager has been deprecated and will be removed in a future release, please use SetpointManagerSingleZoneReheat::addToNode \n
    * Adds setPointManager of type SetpointManagerSingleZoneReheat to this Node. **/
  OS_DEPRECATED void addSetpointManager( SetpointManagerSingleZoneReheat & setPointManager );

  /** \deprecated Node::removeSetpointManagerSingleZoneReheat has been deprecated and will be removed in a future release, please use SetpointManagerSingleZoneReheat::remove \n
    * Removes any SetpointManagerSingleZoneReheat objects from this Node. **/
  OS_DEPRECATED void removeSetpointManagerSingleZoneReheat();

  /** \deprecated Node::getSetpointManagerSingleZoneReheat has been deprecated and will be removed in a future release, please use Node::setpointManagers \n
    * Returns the optional SetpointManagerSingleZoneReheat associated with this Node.  **/
  OS_DEPRECATED boost::optional<SetpointManagerSingleZoneReheat> getSetpointManagerSingleZoneReheat();

  /** \deprecated Node::addSetpointManager has been deprecated and will be removed in a future release, please use SetpointManagerMixedAir::addToNode \n
    * Adds setPointManager of type SetpointManagerMixedAir to this Node. **/
  OS_DEPRECATED void addSetpointManager( SetpointManagerMixedAir & setPointManager );

  /** \deprecated Node::removeSetpointManagerMixedAir has been deprecated and will be removed in a future release, please use SetpointManagerMixedAir::remove \n
    * Removes any SetpointManagerMixedAir objects from this Node. **/
  OS_DEPRECATED void removeSetpointManagerMixedAir();

  /** \deprecated Node::getSetpointManagerMixedAir has been deprecated and will be removed in a future release, please use Node::setpointManagers \n
    * Returns the optional SetpointManagerMixedAir associated with this Node.  **/
  OS_DEPRECATED boost::optional<SetpointManagerMixedAir> getSetpointManagerMixedAir();

  /** \deprecated Node::setpointManagerScheduled has been deprecated and will be removed in a future release, please use Node::setpointManagers \n
    * Returns the optional SetpointManagerScheduled associated with this Node.  **/
  OS_DEPRECATED boost::optional<SetpointManagerScheduled> setpointManagerScheduled() const;

  /** \deprecated Node::addSetpointManager has been deprecated and will be removed in a future release, please use SetpointManagerScheduled::addToNode \n
    * Adds setPointManager of type SetpointManagerScheduled to this Node. **/
  OS_DEPRECATED void addSetpointManager( SetpointManagerScheduled & setPointManager );

  /** \deprecated Node::removeSetpointManagerScheduled has been deprecated and will be removed in a future release, please use SetpointManagerScheduled::remove \n
    * Removes any SetpointManagerScheduled objects from this Node. **/
  OS_DEPRECATED void removeSetpointManagerScheduled();

  /** \deprecated Node::setpointManagerFollowOutdoorAirTemperature has been deprecated and will be removed in a future release, please use Node::setpointManagers \n
    * Returns the optional SetpointManagerFollowOutdoorAirTemperature associated with this Node.  **/
  OS_DEPRECATED boost::optional<SetpointManagerFollowOutdoorAirTemperature> setpointManagerFollowOutdoorAirTemperature() const;

  /** \deprecated Node::addSetpointManager has been deprecated and will be removed in a future release, please use SetpointManagerFollowOutdoorAirTemperature::addToNode \n
    * Adds setPointManager of type SetpointManagerFollowOutdoorAirTemperature to this Node. **/
  OS_DEPRECATED void addSetpointManager( SetpointManagerFollowOutdoorAirTemperature & setPointManager );

  /** \deprecated Node::removeSetpointManagerFollowOutdoorAirTemperature has been deprecated and will be removed in a future release, please use SetpointManagerFollowOutdoorAirTemperature::remove \n
    * Removes any SetpointManagerFollowOutdoorAirTemperature objects from this Node. **/
  OS_DEPRECATED void removeSetpointManagerFollowOutdoorAirTemperature();

  /** \deprecated Node::addSetpointManager has been deprecated and will be removed in a future release, please use SetpointManagerOutdoorAirReset::addToNode \n
    * Adds setPointManager of type SetpointManagerOutdoorAirReset to this Node. **/
  OS_DEPRECATED void addSetpointManager( SetpointManagerOutdoorAirReset & setPointManager );

  /** \deprecated Node::removeSetpointManagerOutdoorAirReset has been deprecated and will be removed in a future release, please use SetpointManagerOutdoorAirReset::remove \n
    * Removes any SetpointManagerOutdoorAirReset objects from this Node. **/
  OS_DEPRECATED void removeSetpointManagerOutdoorAirReset();

  /** \deprecated Node::setpointManagerOutdoorAirReset has been deprecated and will be removed in a future release, please use Node::setpointManagers \n
    * Returns the optional SetpointManagerOutdoorAirReset associated with this Node.  **/
  OS_DEPRECATED boost::optional<SetpointManagerOutdoorAirReset> setpointManagerOutdoorAirReset() const;

  /** \deprecated Node::removeSetpointManagerWarmest has been deprecated and will be removed in a future release, please use SetpointManagerWarmest::remove \n
    * Removes any SetpointManagerWarmest objects from this Node. **/
  OS_DEPRECATED void removeSetpointManagerWarmest();

  /** \deprecated Node::setpointManagerWarmest has been deprecated and will be removed in a future release, please use Node::setpointManagers \n
    * Returns the optional SetpointManagerWarmest associated with this Node.  **/
  OS_DEPRECATED boost::optional<SetpointManagerWarmest> setpointManagerWarmest() const;

  /** \deprecated Node::addSetpointManagerWarmest has been deprecated and will be removed in a future release, please use SetpointManagerWarmest::addToNode \n
    * Adds setPointManager of type SetpointManagerWarmest to this Node. **/
  OS_DEPRECATED void addSetpointManagerWarmest( SetpointManagerWarmest & setPointManager );

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

  explicit Node(std::shared_ptr<detail::Node_Impl> impl);

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

