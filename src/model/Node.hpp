/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

    virtual ~Node() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Node(const Node& other) = default;
    Node(Node&& other) = default;
    Node& operator=(const Node&) = default;
    Node& operator=(Node&&) = default;

    std::vector<SetpointManager> setpointManagers() const;

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
