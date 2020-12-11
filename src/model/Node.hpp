/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

  class AirflowNetworkDistributionNode;

  namespace detail {
    class Node_Impl;
  }  // namespace detail

  /** Node is a ModelObject that defines a point in an EnergyPlus HVAC topology where fluid conditions are known and
 *  control devices can be applied.  EnergyPlus IDD does not define a Node object, however the concept of a node
 *  is central to modeling HVAC systems in EnergyPlus.
 */
  class MODEL_API Node : public StraightComponent
  {

   public:
    /** Constructs a new Node object and places it inside the
   *  model.  The object is fully initialized with all companion objects.
   */
    explicit Node(const Model& model);

    virtual ~Node() {}

    std::vector<SetpointManager> setpointManagers() const;

    /** \deprecated Node::addSetpointManager has been deprecated and will be removed in a future release, please use SetpointManagerSingleZoneReheat::addToNode \n
    * Adds setPointManager of type SetpointManagerSingleZoneReheat to this Node. **/
    OS_DEPRECATED void addSetpointManager(SetpointManagerSingleZoneReheat& setPointManager);

    /** \deprecated Node::removeSetpointManagerSingleZoneReheat has been deprecated and will be removed in a future release, please use SetpointManagerSingleZoneReheat::remove \n
    * Removes any SetpointManagerSingleZoneReheat objects from this Node. **/
    OS_DEPRECATED void removeSetpointManagerSingleZoneReheat();

    /** \deprecated Node::getSetpointManagerSingleZoneReheat has been deprecated and will be removed in a future release, please use Node::setpointManagers \n
    * Returns the optional SetpointManagerSingleZoneReheat associated with this Node.  **/
    OS_DEPRECATED boost::optional<SetpointManagerSingleZoneReheat> getSetpointManagerSingleZoneReheat();

    /** \deprecated Node::addSetpointManager has been deprecated and will be removed in a future release, please use SetpointManagerMixedAir::addToNode \n
    * Adds setPointManager of type SetpointManagerMixedAir to this Node. **/
    OS_DEPRECATED void addSetpointManager(SetpointManagerMixedAir& setPointManager);

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
    OS_DEPRECATED void addSetpointManager(SetpointManagerScheduled& setPointManager);

    /** \deprecated Node::removeSetpointManagerScheduled has been deprecated and will be removed in a future release, please use SetpointManagerScheduled::remove \n
    * Removes any SetpointManagerScheduled objects from this Node. **/
    OS_DEPRECATED void removeSetpointManagerScheduled();

    /** \deprecated Node::setpointManagerFollowOutdoorAirTemperature has been deprecated and will be removed in a future release, please use Node::setpointManagers \n
    * Returns the optional SetpointManagerFollowOutdoorAirTemperature associated with this Node.  **/
    OS_DEPRECATED boost::optional<SetpointManagerFollowOutdoorAirTemperature> setpointManagerFollowOutdoorAirTemperature() const;

    /** \deprecated Node::addSetpointManager has been deprecated and will be removed in a future release, please use SetpointManagerFollowOutdoorAirTemperature::addToNode \n
    * Adds setPointManager of type SetpointManagerFollowOutdoorAirTemperature to this Node. **/
    OS_DEPRECATED void addSetpointManager(SetpointManagerFollowOutdoorAirTemperature& setPointManager);

    /** \deprecated Node::removeSetpointManagerFollowOutdoorAirTemperature has been deprecated and will be removed in a future release, please use SetpointManagerFollowOutdoorAirTemperature::remove \n
    * Removes any SetpointManagerFollowOutdoorAirTemperature objects from this Node. **/
    OS_DEPRECATED void removeSetpointManagerFollowOutdoorAirTemperature();

    /** \deprecated Node::addSetpointManager has been deprecated and will be removed in a future release, please use SetpointManagerOutdoorAirReset::addToNode \n
    * Adds setPointManager of type SetpointManagerOutdoorAirReset to this Node. **/
    OS_DEPRECATED void addSetpointManager(SetpointManagerOutdoorAirReset& setPointManager);

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
    OS_DEPRECATED void addSetpointManagerWarmest(SetpointManagerWarmest& setPointManager);

    bool addToNode(Node& node);

    ModelObject clone(Model model) const;

    bool isRemovable() const;

    std::vector<IdfObject> remove();

    static IddObjectType iddObjectType();

    AirflowNetworkDistributionNode getAirflowNetworkDistributionNode();

    boost::optional<AirflowNetworkDistributionNode> airflowNetworkDistributionNode() const;

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

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_Node_HPP
