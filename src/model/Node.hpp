/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_NODE_HPP
#define MODEL_NODE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"

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

    virtual ~Node() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Node(const Node& other) = default;
    Node(Node&& other) = default;
    Node& operator=(const Node&) = default;
    Node& operator=(Node&&) = default;

    std::vector<SetpointManager> setpointManagers() const;

    static IddObjectType iddObjectType();

    AirflowNetworkDistributionNode getAirflowNetworkDistributionNode();

    boost::optional<AirflowNetworkDistributionNode> airflowNetworkDistributionNode() const;

   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    friend class openstudio::detail::IdfObject_Impl;

    /// @cond

    using ImplType = detail::Node_Impl;

    explicit Node(std::shared_ptr<detail::Node_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.Node");

    /// @endcond
  };

  /** \relates Node */
  using OptionalNode = boost::optional<Node>;

  /** \relates Node */
  using NodeVector = std::vector<Node>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_Node_HPP
