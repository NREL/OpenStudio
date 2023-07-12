/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ConnectorMixer.hpp"
#include "ConnectorMixer_Impl.hpp"
#include "Node.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"

#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/OS_Connector_Mixer_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    ConnectorMixer_Impl::ConnectorMixer_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : Mixer_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ConnectorMixer::iddObjectType());
    }

    ConnectorMixer_Impl::ConnectorMixer_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : Mixer_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ConnectorMixer::iddObjectType());
    }

    ConnectorMixer_Impl::ConnectorMixer_Impl(const ConnectorMixer_Impl& other, Model_Impl* model, bool keepHandle)
      : Mixer_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ConnectorMixer_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ConnectorMixer_Impl::iddObjectType() const {
      return ConnectorMixer::iddObjectType();
    }

    std::vector<openstudio::IdfObject> ConnectorMixer_Impl::remove() {
      OptionalMixer self = model().getModelObject<Mixer>(handle());
      model().disconnect(*self, outletPort());
      for (int i = 0; i < int(nextBranchIndex()) - 1; i++) {
        model().disconnect(*self, inletPort(i));
      }
      return ModelObject_Impl::remove();
    }

    unsigned ConnectorMixer_Impl::outletPort() const {
      return OS_Connector_MixerFields::OutletBranchName;
    }

    unsigned ConnectorMixer_Impl::inletPort(unsigned branchIndex) const {
      unsigned result;
      result = numNonextensibleFields();
      result = result + branchIndex;
      return result;
    }

    unsigned ConnectorMixer_Impl::nextInletPort() const {
      return inletPort(this->nextBranchIndex());
    }

    bool ConnectorMixer_Impl::addToNode(Node& node) {
      return HVACComponent_Impl::addToNode(node);
    }

    ModelObject ConnectorMixer_Impl::clone(Model model) const {
      return HVACComponent_Impl::clone(model);
    }

    ComponentType ConnectorMixer_Impl::componentType() const {
      return ComponentType::None;
    }

    std::vector<FuelType> ConnectorMixer_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> ConnectorMixer_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> ConnectorMixer_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  // create a new Mixer object in the model's workspace
  ConnectorMixer::ConnectorMixer(const Model& model) : Mixer(ConnectorMixer::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ConnectorMixer_Impl>());
  }

  ConnectorMixer::ConnectorMixer(std::shared_ptr<detail::ConnectorMixer_Impl> p) : Mixer(std::move(p)) {}

  std::vector<openstudio::IdfObject> ConnectorMixer::remove() {
    return getImpl<detail::ConnectorMixer_Impl>()->remove();
  }

  unsigned ConnectorMixer::outletPort() const {
    return getImpl<detail::ConnectorMixer_Impl>()->outletPort();
  }

  unsigned ConnectorMixer::inletPort(unsigned branchIndex) const {
    return getImpl<detail::ConnectorMixer_Impl>()->inletPort(branchIndex);
  }

  unsigned ConnectorMixer::nextInletPort() const {
    return getImpl<detail::ConnectorMixer_Impl>()->nextInletPort();
  }

  bool ConnectorMixer::addToNode(Node& node) {
    return getImpl<detail::ConnectorMixer_Impl>()->addToNode(node);
  }

  ModelObject ConnectorMixer::clone(Model model) const {
    return getImpl<detail::ConnectorMixer_Impl>()->clone(model);
  }

  IddObjectType ConnectorMixer::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Connector_Mixer);
    return result;
  }

}  // namespace model
}  // namespace openstudio
