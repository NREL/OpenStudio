/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "AirflowNetworkDistributionNode.hpp"
#include "AirflowNetworkDistributionNode_Impl.hpp"
#include "AirLoopHVACZoneMixer.hpp"
#include "AirLoopHVACZoneMixer_Impl.hpp"
#include "AirLoopHVACZoneSplitter.hpp"
#include "AirLoopHVACZoneSplitter_Impl.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AirflowNetworkDistributionNode_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirflowNetworkDistributionNode_Impl::AirflowNetworkDistributionNode_Impl(const IdfObject& idfObject,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : AirflowNetworkNode_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkDistributionNode::iddObjectType());
  }

  AirflowNetworkDistributionNode_Impl::AirflowNetworkDistributionNode_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : AirflowNetworkNode_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirflowNetworkDistributionNode::iddObjectType());
  }

  AirflowNetworkDistributionNode_Impl::AirflowNetworkDistributionNode_Impl(const AirflowNetworkDistributionNode_Impl& other,
                                                                           Model_Impl* model,
                                                                           bool keepHandle)
    : AirflowNetworkNode_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirflowNetworkDistributionNode_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    return result;
  }

  IddObjectType AirflowNetworkDistributionNode_Impl::iddObjectType() const {
    return AirflowNetworkDistributionNode::iddObjectType();
  }

  boost::optional<HVACComponent> AirflowNetworkDistributionNode_Impl::component() const
  {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirflowNetworkDistributionNodeFields::ComponentNameorNodeName);
  }

  boost::optional<Node> AirflowNetworkDistributionNode_Impl::node() const
  {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_AirflowNetworkDistributionNodeFields::ComponentNameorNodeName);
  }

  boost::optional<AirLoopHVACZoneMixer> AirflowNetworkDistributionNode_Impl::airLoopHVACZoneMixer() const
  {
    return getObject<ModelObject>().getModelObjectTarget<AirLoopHVACZoneMixer>(OS_AirflowNetworkDistributionNodeFields::ComponentNameorNodeName);
  }

  boost::optional<AirLoopHVACZoneSplitter> AirflowNetworkDistributionNode_Impl::airLoopHVACZoneSplitter() const
  {
    return getObject<ModelObject>().getModelObjectTarget<AirLoopHVACZoneSplitter>(OS_AirflowNetworkDistributionNodeFields::ComponentNameorNodeName);
  }

  boost::optional<AirLoopHVACOutdoorAirSystem> AirflowNetworkDistributionNode_Impl::airLoopHVACOutdoorAirSystem() const
  {
    return getObject<ModelObject>().getModelObjectTarget<AirLoopHVACOutdoorAirSystem>(OS_AirflowNetworkDistributionNodeFields::ComponentNameorNodeName);
  }

  double AirflowNetworkDistributionNode_Impl::nodeHeight() const {
    boost::optional<double> value = getDouble(OS_AirflowNetworkDistributionNodeFields::NodeHeight,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirflowNetworkDistributionNode_Impl::isNodeHeightDefaulted() const {
    return isEmpty(OS_AirflowNetworkDistributionNodeFields::NodeHeight);
  }

  void AirflowNetworkDistributionNode_Impl::setNodeHeight(double nodeHeight) {
    bool result = setDouble(OS_AirflowNetworkDistributionNodeFields::NodeHeight, nodeHeight);
    OS_ASSERT(result);
  }

  void AirflowNetworkDistributionNode_Impl::resetNodeHeight() {
    bool result = setString(OS_AirflowNetworkDistributionNodeFields::NodeHeight, "");
    OS_ASSERT(result);
  }

  void AirflowNetworkDistributionNode_Impl::resetComponent() {
    bool result = setString(OS_AirflowNetworkDistributionNodeFields::ComponentNameorNodeName, "");
    OS_ASSERT(result);
  }

} // detail

AirflowNetworkDistributionNode::AirflowNetworkDistributionNode(const Model& model, const Handle &handle)
  : AirflowNetworkNode(AirflowNetworkDistributionNode::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkDistributionNode_Impl>());
  bool ok = getImpl<detail::AirflowNetworkDistributionNode_Impl>()->setPointer(OS_AirflowNetworkDistributionNodeFields::ComponentNameorNodeName, handle);
  OS_ASSERT(ok);
}

AirflowNetworkDistributionNode::AirflowNetworkDistributionNode(const Model& model)
  : AirflowNetworkNode(AirflowNetworkDistributionNode::iddObjectType(), model)
{
  OS_ASSERT(getImpl<detail::AirflowNetworkDistributionNode_Impl>());
}

IddObjectType AirflowNetworkDistributionNode::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirflowNetworkDistributionNode);
}

boost::optional<Node> AirflowNetworkDistributionNode::node() const
{
  return getImpl<detail::AirflowNetworkDistributionNode_Impl>()->node();
}

boost::optional<AirLoopHVACZoneMixer> AirflowNetworkDistributionNode::airLoopHVACZoneMixer() const
{
  return getImpl<detail::AirflowNetworkDistributionNode_Impl>()->airLoopHVACZoneMixer();
}

boost::optional<AirLoopHVACZoneSplitter> AirflowNetworkDistributionNode::airLoopHVACZoneSplitter() const
{
  return getImpl<detail::AirflowNetworkDistributionNode_Impl>()->airLoopHVACZoneSplitter();
}

boost::optional<AirLoopHVACOutdoorAirSystem> AirflowNetworkDistributionNode::airLoopHVACOutdoorAirSystem() const
{
  return getImpl<detail::AirflowNetworkDistributionNode_Impl>()->airLoopHVACOutdoorAirSystem();
}

//OAMixerOutdoorAirStreamNode
//OutdoorAir : NodeList
//OutdoorAir : Node

double AirflowNetworkDistributionNode::nodeHeight() const {
  return getImpl<detail::AirflowNetworkDistributionNode_Impl>()->nodeHeight();
}

bool AirflowNetworkDistributionNode::isNodeHeightDefaulted() const {
  return getImpl<detail::AirflowNetworkDistributionNode_Impl>()->isNodeHeightDefaulted();
}

void AirflowNetworkDistributionNode::setNodeHeight(double nodeHeight) {
  getImpl<detail::AirflowNetworkDistributionNode_Impl>()->setNodeHeight(nodeHeight);
}

void AirflowNetworkDistributionNode::resetNodeHeight() {
  getImpl<detail::AirflowNetworkDistributionNode_Impl>()->resetNodeHeight();
}

void AirflowNetworkDistributionNode::resetComponent() {
  getImpl<detail::AirflowNetworkDistributionNode_Impl>()->resetComponent();
}

/// @cond
AirflowNetworkDistributionNode::AirflowNetworkDistributionNode(std::shared_ptr<detail::AirflowNetworkDistributionNode_Impl> impl)
  : AirflowNetworkNode(impl)
{}
/// @endcond

} // model
} // openstudio

