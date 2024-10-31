/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirLoopHVACZoneMixer.hpp"
#include "AirLoopHVACZoneMixer_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "AirflowNetworkDistributionNode.hpp"
#include "AirflowNetworkDistributionNode_Impl.hpp"
#include "Node.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include <utilities/idd/OS_AirLoopHVAC_ZoneMixer_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirLoopHVACZoneMixer_Impl::AirLoopHVACZoneMixer_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Mixer_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirLoopHVACZoneMixer::iddObjectType());
    }

    AirLoopHVACZoneMixer_Impl::AirLoopHVACZoneMixer_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : Mixer_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirLoopHVACZoneMixer::iddObjectType());
    }

    AirLoopHVACZoneMixer_Impl::AirLoopHVACZoneMixer_Impl(const AirLoopHVACZoneMixer_Impl& other, Model_Impl* model, bool keepHandle)
      : Mixer_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirLoopHVACZoneMixer_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      // Not Appropriate: No variables available
      return result;
    }

    IddObjectType AirLoopHVACZoneMixer_Impl::iddObjectType() const {
      return AirLoopHVACZoneMixer::iddObjectType();
    }

    std::vector<ModelObject> AirLoopHVACZoneMixer_Impl::children() const {
      std::vector<ModelObject> result;

      std::vector<AirflowNetworkDistributionNode> myAFNItems =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkDistributionNode>(AirflowNetworkDistributionNode::iddObjectType());
      result.insert(result.end(), myAFNItems.begin(), myAFNItems.end());

      return result;
    }

    std::vector<openstudio::IdfObject> AirLoopHVACZoneMixer_Impl::remove() {
      if (this->airLoopHVAC()) {
        return {};
      } else {
        this->disconnect();

        return HVACComponent_Impl::remove();
      }
    }

    unsigned AirLoopHVACZoneMixer_Impl::outletPort() const {
      return OS_AirLoopHVAC_ZoneMixerFields::OutletNodeName;
    }

    unsigned AirLoopHVACZoneMixer_Impl::inletPort(unsigned branchIndex) const {
      unsigned result;
      result = numNonextensibleFields();
      result = result + branchIndex;
      return result;
    }

    unsigned AirLoopHVACZoneMixer_Impl::nextInletPort() const {
      return inletPort(this->nextBranchIndex());
    }

    bool AirLoopHVACZoneMixer_Impl::addToNode(Node& node) {
      return HVACComponent_Impl::addToNode(node);
    }

    ModelObject AirLoopHVACZoneMixer_Impl::clone(Model model) const {
      return HVACComponent_Impl::clone(model);
    }

    void AirLoopHVACZoneMixer_Impl::disconnect() {
      auto mo = this->getObject<ModelObject>();
      model().disconnect(mo, outletPort());
      for (int i = 0; i < int(nextBranchIndex()); i++) {
        model().disconnect(mo, inletPort(i));
      }
    }

    AirflowNetworkDistributionNode AirLoopHVACZoneMixer_Impl::getAirflowNetworkDistributionNode() {
      boost::optional<AirflowNetworkDistributionNode> opt = airflowNetworkDistributionNode();
      if (opt) {
        return opt.get();
      }
      return AirflowNetworkDistributionNode(model(), handle());
    }

    boost::optional<AirflowNetworkDistributionNode> AirLoopHVACZoneMixer_Impl::airflowNetworkDistributionNode() const {
      std::vector<AirflowNetworkDistributionNode> myAFNItems =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkDistributionNode>(AirflowNetworkDistributionNode::iddObjectType());
      auto count = myAFNItems.size();
      if (count == 1) {
        return myAFNItems[0];
      } else if (count > 1) {
        LOG(Warn, briefDescription() << " has more than one AirflowNetwork DistributionNode attached, returning first.");
        return myAFNItems[0];
      }
      return boost::none;
    }

    ComponentType AirLoopHVACZoneMixer_Impl::componentType() const {
      return ComponentType::None;
    }

    std::vector<FuelType> AirLoopHVACZoneMixer_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> AirLoopHVACZoneMixer_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> AirLoopHVACZoneMixer_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  // create a new AirLoopHVACZoneMixer object in the model's workspace
  AirLoopHVACZoneMixer::AirLoopHVACZoneMixer(const Model& model) : Mixer(AirLoopHVACZoneMixer::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirLoopHVACZoneMixer_Impl>());
  }

  AirLoopHVACZoneMixer::AirLoopHVACZoneMixer(std::shared_ptr<detail::AirLoopHVACZoneMixer_Impl> impl) : Mixer(std::move(impl)) {}

  IddObjectType AirLoopHVACZoneMixer::iddObjectType() {
    IddObjectType result(IddObjectType::OS_AirLoopHVAC_ZoneMixer);
    return result;
  }

  unsigned AirLoopHVACZoneMixer::outletPort() const {
    return getImpl<detail::AirLoopHVACZoneMixer_Impl>()->outletPort();
  }

  unsigned AirLoopHVACZoneMixer::inletPort(unsigned branchIndex) const {
    return getImpl<detail::AirLoopHVACZoneMixer_Impl>()->inletPort(branchIndex);
  }

  unsigned AirLoopHVACZoneMixer::nextInletPort() const {
    return getImpl<detail::AirLoopHVACZoneMixer_Impl>()->nextInletPort();
  }

  AirflowNetworkDistributionNode AirLoopHVACZoneMixer::getAirflowNetworkDistributionNode() {
    return getImpl<detail::AirLoopHVACZoneMixer_Impl>()->getAirflowNetworkDistributionNode();
  }

  boost::optional<AirflowNetworkDistributionNode> AirLoopHVACZoneMixer::airflowNetworkDistributionNode() const {
    return getImpl<detail::AirLoopHVACZoneMixer_Impl>()->airflowNetworkDistributionNode();
  }

}  // namespace model
}  // namespace openstudio
