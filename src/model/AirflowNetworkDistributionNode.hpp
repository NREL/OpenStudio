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

#ifndef MODEL_AIRFLOWNETWORKDISTRIBUTIONNODE_HPP
#define MODEL_AIRFLOWNETWORKDISTRIBUTIONNODE_HPP

#include "ModelAPI.hpp"

#include "AirflowNetworkNode.hpp"

namespace openstudio {

namespace model {

  class AirLoopHVACZoneMixer;
  class AirLoopHVACZoneSplitter;
  class AirLoopHVACOutdoorAirSystem;
  //OAMixerOutdoorAirStreamNode
  //OutdoorAir : NodeList
  //OutdoorAir : Node
  class Node;

  namespace detail {

    class Node_Impl;
    class AirLoopHVACZoneMixer_Impl;
    class AirLoopHVACZoneSplitter_Impl;
    class AirLoopHVACOutdoorAirSystem_Impl;
    class AirflowNetworkDistributionNode_Impl;

  }  // namespace detail

  /** AirflowNetworkDistributionNode is a AirflowNetworkNode that wraps the OpenStudio IDD object 'OS:AirflowNetworkDistributionNode'. */
  class MODEL_API AirflowNetworkDistributionNode : public AirflowNetworkNode
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    AirflowNetworkDistributionNode(const Model& model);

    virtual ~AirflowNetworkDistributionNode() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirflowNetworkDistributionNode(const AirflowNetworkDistributionNode& other) = default;
    AirflowNetworkDistributionNode(AirflowNetworkDistributionNode&& other) = default;
    AirflowNetworkDistributionNode& operator=(const AirflowNetworkDistributionNode&) = default;
    AirflowNetworkDistributionNode& operator=(AirflowNetworkDistributionNode&&) = default;

    //@}

    static IddObjectType iddObjectType();

    //static std::vector<std::string> componentObjectTypeorNodeTypeValues();

    /** @name Getters */
    //@{
    boost::optional<Node> node() const;
    boost::optional<AirLoopHVACZoneMixer> airLoopHVACZoneMixer() const;
    boost::optional<AirLoopHVACZoneSplitter> airLoopHVACZoneSplitter() const;
    boost::optional<AirLoopHVACOutdoorAirSystem> airLoopHVACOutdoorAirSystem() const;
    //OAMixerOutdoorAirStreamNode
    //OutdoorAir : NodeList
    //OutdoorAir : Node

    double nodeHeight() const;

    bool isNodeHeightDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    void setNodeHeight(double nodeHeight);

    void resetNodeHeight();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirflowNetworkDistributionNode_Impl;

    explicit AirflowNetworkDistributionNode(std::shared_ptr<detail::AirflowNetworkDistributionNode_Impl> impl);

    AirflowNetworkDistributionNode(const Model& model, const Handle& handle);

    void resetComponent();

    friend class detail::Node_Impl;
    friend class detail::AirLoopHVACZoneMixer_Impl;
    friend class detail::AirLoopHVACZoneSplitter_Impl;
    friend class detail::AirLoopHVACOutdoorAirSystem_Impl;
    friend class detail::AirflowNetworkDistributionNode_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirflowNetworkDistributionNode");
  };

  /** \relates AirflowNetworkDistributionNode*/
  using OptionalAirflowNetworkDistributionNode = boost::optional<AirflowNetworkDistributionNode>;

  /** \relates AirflowNetworkDistributionNode*/
  using AirflowNetworkDistributionNodeVector = std::vector<AirflowNetworkDistributionNode>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRFLOWNETWORKDISTRIBUTIONNODE_HPP
