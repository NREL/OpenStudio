/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirLoopHVAC.hpp"
#include "AirLoopHVACZoneSplitter.hpp"
#include "AirLoopHVACZoneSplitter_Impl.hpp"
#include "AirflowNetworkDistributionNode.hpp"
#include "AirflowNetworkDistributionNode_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "Node.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include <utilities/idd/OS_AirLoopHVAC_ZoneSplitter_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirLoopHVACZoneSplitter_Impl::AirLoopHVACZoneSplitter_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Splitter_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirLoopHVACZoneSplitter::iddObjectType());
    }

    AirLoopHVACZoneSplitter_Impl::AirLoopHVACZoneSplitter_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                               bool keepHandle)
      : Splitter_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirLoopHVACZoneSplitter::iddObjectType());
    }

    AirLoopHVACZoneSplitter_Impl::AirLoopHVACZoneSplitter_Impl(const AirLoopHVACZoneSplitter_Impl& other, Model_Impl* model, bool keepHandle)
      : Splitter_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirLoopHVACZoneSplitter_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      // Not Appropriate: No variables available
      return result;
    }

    IddObjectType AirLoopHVACZoneSplitter_Impl::iddObjectType() const {
      return AirLoopHVACZoneSplitter::iddObjectType();
    }

    std::vector<ModelObject> AirLoopHVACZoneSplitter_Impl::children() const {
      std::vector<ModelObject> result;

      std::vector<AirflowNetworkDistributionNode> myAFNItems =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkDistributionNode>(AirflowNetworkDistributionNode::iddObjectType());
      result.insert(result.end(), myAFNItems.begin(), myAFNItems.end());

      return result;
    }

    std::vector<openstudio::IdfObject> AirLoopHVACZoneSplitter_Impl::remove() {
      if (this->airLoopHVAC()) {
        return {};
      } else {
        OptionalAirLoopHVACZoneSplitter self = model().getModelObject<AirLoopHVACZoneSplitter>(handle());
        model().disconnect(*self, inletPort());
        for (int i = 0; i < int(nextBranchIndex()) - 1; i++) {
          model().disconnect(*self, outletPort(i));
        }
        return HVACComponent_Impl::remove();
      }
    }

    void AirLoopHVACZoneSplitter_Impl::disconnect() {
      auto mo = this->getObject<ModelObject>();
      model().disconnect(mo, inletPort());
      for (int i = 0; i < int(nextBranchIndex()); i++) {
        model().disconnect(mo, outletPort(i));
      }
    }

    unsigned AirLoopHVACZoneSplitter_Impl::inletPort() const {
      return OS_AirLoopHVAC_ZoneSplitterFields::InletNodeName;
    }

    unsigned AirLoopHVACZoneSplitter_Impl::outletPort(unsigned branchIndex) const {
      unsigned result;
      result = numNonextensibleFields();
      result = result + branchIndex;
      return result;
    }

    unsigned AirLoopHVACZoneSplitter_Impl::nextOutletPort() const {
      return outletPort(this->nextBranchIndex());
    }

    std::vector<ThermalZone> AirLoopHVACZoneSplitter_Impl::thermalZones() {
      std::vector<ThermalZone> zones;
      std::vector<ModelObject> modelObjects;
      //std::vector<ModelObject> _outletModelObjects = outletModelObjects();

      OptionalAirLoopHVAC _airLoopHVAC = airLoopHVAC();
      OptionalNode demandOutletNode;
      OptionalNode demandInletNode;
      if (_airLoopHVAC) {
        demandOutletNode = _airLoopHVAC->demandOutletNode();
        demandInletNode = _airLoopHVAC->demandInletNode();
      } else {
        return zones;
      }

      modelObjects = _airLoopHVAC->demandComponents(demandInletNode.get(), demandOutletNode.get(), ThermalZone::iddObjectType());

      for (const auto& modelObject : modelObjects) {
        OptionalThermalZone zone;
        zone = modelObject.optionalCast<ThermalZone>();
        if (zone) {
          zones.push_back(*zone);
        }
      }

      return zones;
    }

    AirflowNetworkDistributionNode AirLoopHVACZoneSplitter_Impl::getAirflowNetworkDistributionNode() {
      boost::optional<AirflowNetworkDistributionNode> opt = airflowNetworkDistributionNode();
      if (opt) {
        return opt.get();
      }
      return AirflowNetworkDistributionNode(model(), handle());
    }

    boost::optional<AirflowNetworkDistributionNode> AirLoopHVACZoneSplitter_Impl::airflowNetworkDistributionNode() const {
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

    ComponentType AirLoopHVACZoneSplitter_Impl::componentType() const {
      return ComponentType::None;
    }

    std::vector<FuelType> AirLoopHVACZoneSplitter_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> AirLoopHVACZoneSplitter_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> AirLoopHVACZoneSplitter_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  AirLoopHVACZoneSplitter::AirLoopHVACZoneSplitter(const Model& model) : Splitter(AirLoopHVACZoneSplitter::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirLoopHVACZoneSplitter_Impl>());
  }

  AirLoopHVACZoneSplitter::AirLoopHVACZoneSplitter(std::shared_ptr<detail::AirLoopHVACZoneSplitter_Impl> impl) : Splitter(std::move(impl)) {}

  unsigned AirLoopHVACZoneSplitter::inletPort() const {
    return getImpl<detail::AirLoopHVACZoneSplitter_Impl>()->inletPort();
  }

  unsigned AirLoopHVACZoneSplitter::outletPort(unsigned branchIndex) const {
    return getImpl<detail::AirLoopHVACZoneSplitter_Impl>()->outletPort(branchIndex);
  }

  unsigned AirLoopHVACZoneSplitter::nextOutletPort() const {
    return getImpl<detail::AirLoopHVACZoneSplitter_Impl>()->nextOutletPort();
  }

  IddObjectType AirLoopHVACZoneSplitter::iddObjectType() {
    IddObjectType result(IddObjectType::OS_AirLoopHVAC_ZoneSplitter);
    return result;
  }

  std::vector<ThermalZone> AirLoopHVACZoneSplitter::thermalZones() {
    return getImpl<detail::AirLoopHVACZoneSplitter_Impl>()->thermalZones();
  }

  AirflowNetworkDistributionNode AirLoopHVACZoneSplitter::getAirflowNetworkDistributionNode() {
    return getImpl<detail::AirLoopHVACZoneSplitter_Impl>()->getAirflowNetworkDistributionNode();
  }

  boost::optional<AirflowNetworkDistributionNode> AirLoopHVACZoneSplitter::airflowNetworkDistributionNode() const {
    return getImpl<detail::AirLoopHVACZoneSplitter_Impl>()->airflowNetworkDistributionNode();
  }

}  // namespace model

}  // namespace openstudio
