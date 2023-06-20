/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    AirflowNetworkDistributionNode_Impl::AirflowNetworkDistributionNode_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : AirflowNetworkNode_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirflowNetworkDistributionNode::iddObjectType());
    }

    AirflowNetworkDistributionNode_Impl::AirflowNetworkDistributionNode_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                             bool keepHandle)
      : AirflowNetworkNode_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirflowNetworkDistributionNode::iddObjectType());
    }

    AirflowNetworkDistributionNode_Impl::AirflowNetworkDistributionNode_Impl(const AirflowNetworkDistributionNode_Impl& other, Model_Impl* model,
                                                                             bool keepHandle)
      : AirflowNetworkNode_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirflowNetworkDistributionNode_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType AirflowNetworkDistributionNode_Impl::iddObjectType() const {
      return AirflowNetworkDistributionNode::iddObjectType();
    }

    boost::optional<HVACComponent> AirflowNetworkDistributionNode_Impl::component() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirflowNetworkDistributionNodeFields::ComponentNameorNodeName);
    }

    boost::optional<Node> AirflowNetworkDistributionNode_Impl::node() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_AirflowNetworkDistributionNodeFields::ComponentNameorNodeName);
    }

    boost::optional<AirLoopHVACZoneMixer> AirflowNetworkDistributionNode_Impl::airLoopHVACZoneMixer() const {
      return getObject<ModelObject>().getModelObjectTarget<AirLoopHVACZoneMixer>(OS_AirflowNetworkDistributionNodeFields::ComponentNameorNodeName);
    }

    boost::optional<AirLoopHVACZoneSplitter> AirflowNetworkDistributionNode_Impl::airLoopHVACZoneSplitter() const {
      return getObject<ModelObject>().getModelObjectTarget<AirLoopHVACZoneSplitter>(OS_AirflowNetworkDistributionNodeFields::ComponentNameorNodeName);
    }

    boost::optional<AirLoopHVACOutdoorAirSystem> AirflowNetworkDistributionNode_Impl::airLoopHVACOutdoorAirSystem() const {
      return getObject<ModelObject>().getModelObjectTarget<AirLoopHVACOutdoorAirSystem>(
        OS_AirflowNetworkDistributionNodeFields::ComponentNameorNodeName);
    }

    double AirflowNetworkDistributionNode_Impl::nodeHeight() const {
      boost::optional<double> value = getDouble(OS_AirflowNetworkDistributionNodeFields::NodeHeight, true);
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

  }  // namespace detail

  AirflowNetworkDistributionNode::AirflowNetworkDistributionNode(const Model& model, const Handle& handle)
    : AirflowNetworkNode(AirflowNetworkDistributionNode::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkDistributionNode_Impl>());
    bool ok =
      getImpl<detail::AirflowNetworkDistributionNode_Impl>()->setPointer(OS_AirflowNetworkDistributionNodeFields::ComponentNameorNodeName, handle);
    OS_ASSERT(ok);
  }

  AirflowNetworkDistributionNode::AirflowNetworkDistributionNode(const Model& model)
    : AirflowNetworkNode(AirflowNetworkDistributionNode::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirflowNetworkDistributionNode_Impl>());
  }

  IddObjectType AirflowNetworkDistributionNode::iddObjectType() {
    return {IddObjectType::OS_AirflowNetworkDistributionNode};
  }

  boost::optional<Node> AirflowNetworkDistributionNode::node() const {
    return getImpl<detail::AirflowNetworkDistributionNode_Impl>()->node();
  }

  boost::optional<AirLoopHVACZoneMixer> AirflowNetworkDistributionNode::airLoopHVACZoneMixer() const {
    return getImpl<detail::AirflowNetworkDistributionNode_Impl>()->airLoopHVACZoneMixer();
  }

  boost::optional<AirLoopHVACZoneSplitter> AirflowNetworkDistributionNode::airLoopHVACZoneSplitter() const {
    return getImpl<detail::AirflowNetworkDistributionNode_Impl>()->airLoopHVACZoneSplitter();
  }

  boost::optional<AirLoopHVACOutdoorAirSystem> AirflowNetworkDistributionNode::airLoopHVACOutdoorAirSystem() const {
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
    : AirflowNetworkNode(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
