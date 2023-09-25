/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ConnectorSplitter.hpp"
#include "ConnectorSplitter_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Node.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/OS_Connector_Splitter_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

namespace model {

  namespace detail {

    ConnectorSplitter_Impl::ConnectorSplitter_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Splitter_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ConnectorSplitter::iddObjectType());
    }

    ConnectorSplitter_Impl::ConnectorSplitter_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : Splitter_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ConnectorSplitter::iddObjectType());
    }

    ConnectorSplitter_Impl::ConnectorSplitter_Impl(const ConnectorSplitter_Impl& other, Model_Impl* model, bool keepHandle)
      : Splitter_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ConnectorSplitter_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ConnectorSplitter_Impl::iddObjectType() const {
      return ConnectorSplitter::iddObjectType();
    }

    std::vector<openstudio::IdfObject> ConnectorSplitter_Impl::remove() {
      OptionalConnectorSplitter self = model().getModelObject<ConnectorSplitter>(handle());
      model().disconnect(*self, inletPort());
      for (int i = 0; i < int(nextBranchIndex()) - 1; i++) {
        model().disconnect(*self, outletPort(i));
      }
      return ModelObject_Impl::remove();
    }

    unsigned ConnectorSplitter_Impl::inletPort() const {
      return OS_Connector_SplitterFields::InletBranchName;
    }

    unsigned ConnectorSplitter_Impl::outletPort(unsigned branchIndex) const {
      unsigned result;
      result = numNonextensibleFields();
      result = result + branchIndex;
      return result;
    }

    unsigned ConnectorSplitter_Impl::nextOutletPort() const {
      return outletPort(this->nextBranchIndex());
    }

    bool ConnectorSplitter_Impl::addToNode(Node& node) {
      auto t_model = model();

      if (node.model() != t_model) {
        return false;
      }

      if (loop()) {
        return false;
      }

      if (!outletModelObjects().empty()) {
        LOG(Warn, briefDescription() << " must not have existing outlet connections to use addToNode method.");
        return false;
      }

      auto t_nodeAirLoop = node.airLoopHVAC();

      if (!t_nodeAirLoop) {
        LOG(Warn, briefDescription() << " must be added to an AirLoopHVAC supply node use addToNode method.");
        return false;
      }

      if (!t_nodeAirLoop->supplyComponent(node.handle())) {
        LOG(Warn, briefDescription() << " must be added to an AirLoopHVAC supply node use addToNode method.");
        return false;
      }

      if (!t_nodeAirLoop->supplyComponents(iddObjectType()).empty()) {
        LOG(Warn, briefDescription() << " already has a splitter.");
        return false;
      }

      auto supplyInletNode = t_nodeAirLoop->supplyInletNode();
      auto supplyOutletNode = t_nodeAirLoop->supplyOutletNode();

      // Hook Up Duct "A"
      auto t_inletPort = inletPort();
      auto portA = nextOutletPort();
      HVACComponent_Impl::addToNode(node, supplyInletNode, supplyOutletNode, t_inletPort, portA);

      // Hook Up Duct "B"
      Node supplyOutletNodeB(t_model);
      auto thisObject = getObject<model::HVACComponent>();
      auto portB = nextOutletPort();
      t_model.connect(thisObject, portB, supplyOutletNodeB, supplyOutletNodeB.inletPort());
      t_model.connect(supplyOutletNodeB, supplyOutletNodeB.outletPort(), t_nodeAirLoop.get(),
                      t_nodeAirLoop->getImpl<detail::AirLoopHVAC_Impl>()->supplyOutletPortB());

      return true;
    }

    ModelObject ConnectorSplitter_Impl::clone(Model model) const {
      return HVACComponent_Impl::clone(model);
    }

    ComponentType ConnectorSplitter_Impl::componentType() const {
      return ComponentType::None;
    }

    std::vector<FuelType> ConnectorSplitter_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> ConnectorSplitter_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> ConnectorSplitter_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  ConnectorSplitter::ConnectorSplitter(const Model& model) : Splitter(ConnectorSplitter::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ConnectorSplitter_Impl>());
  }

  ConnectorSplitter::ConnectorSplitter(std::shared_ptr<detail::ConnectorSplitter_Impl> p) : Splitter(std::move(p)) {}

  std::vector<openstudio::IdfObject> ConnectorSplitter::remove() {
    return getImpl<detail::ConnectorSplitter_Impl>()->remove();
  }

  unsigned ConnectorSplitter::inletPort() const {
    return getImpl<detail::ConnectorSplitter_Impl>()->inletPort();
  }

  unsigned ConnectorSplitter::outletPort(unsigned branchIndex) const {
    return getImpl<detail::ConnectorSplitter_Impl>()->outletPort(branchIndex);
  }

  unsigned ConnectorSplitter::nextOutletPort() const {
    return getImpl<detail::ConnectorSplitter_Impl>()->nextOutletPort();
  }

  bool ConnectorSplitter::addToNode(Node& node) {
    return getImpl<detail::ConnectorSplitter_Impl>()->addToNode(node);
  }

  ModelObject ConnectorSplitter::clone(Model model) const {
    return getImpl<detail::ConnectorSplitter_Impl>()->clone(model);
  }

  IddObjectType ConnectorSplitter::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Connector_Splitter);
    return result;
  }

}  // namespace model
}  // namespace openstudio
