/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SetpointManagerSingleZoneHumidityMaximum.hpp"
#include "SetpointManagerSingleZoneHumidityMaximum_Impl.hpp"

#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "AirLoopHVAC.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SetpointManager_SingleZone_Humidity_Maximum_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SetpointManagerSingleZoneHumidityMaximum_Impl::SetpointManagerSingleZoneHumidityMaximum_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                 bool keepHandle)
      : SetpointManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SetpointManagerSingleZoneHumidityMaximum::iddObjectType());
    }

    SetpointManagerSingleZoneHumidityMaximum_Impl::SetpointManagerSingleZoneHumidityMaximum_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SetpointManagerSingleZoneHumidityMaximum::iddObjectType());
    }

    SetpointManagerSingleZoneHumidityMaximum_Impl::SetpointManagerSingleZoneHumidityMaximum_Impl(
      const SetpointManagerSingleZoneHumidityMaximum_Impl& other, Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SetpointManagerSingleZoneHumidityMaximum_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SetpointManagerSingleZoneHumidityMaximum_Impl::iddObjectType() const {
      return SetpointManagerSingleZoneHumidityMaximum::iddObjectType();
    }

    bool SetpointManagerSingleZoneHumidityMaximum_Impl::addToNode(Node& node) {
      bool added = SetpointManager_Impl::addToNode(node);
      if (added) {
        if (boost::optional<AirLoopHVAC> _airLoop = node.airLoopHVAC()) {
          ModelObjectVector modelObjectVector = _airLoop->demandComponents(openstudio::IddObjectType::OS_ThermalZone);
          if (!modelObjectVector.empty()) {
            ModelObject mo = modelObjectVector.front();
            auto thermalZone = mo.cast<ThermalZone>();
            this->setControlZone(thermalZone);
          }
          return true;
        }
      }
      return false;
    }

    ModelObject SetpointManagerSingleZoneHumidityMaximum_Impl::clone(Model model) const {
      auto clonedObject = SetpointManager_Impl::clone(model).cast<SetpointManagerSingleZoneHumidityMaximum>();
      clonedObject.resetControlZone();
      return std::move(clonedObject);
    }

    std::string SetpointManagerSingleZoneHumidityMaximum_Impl::controlVariable() const {
      boost::optional<std::string> value = getString(OS_SetpointManager_SingleZone_Humidity_MaximumFields::ControlVariable, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<ThermalZone> SetpointManagerSingleZoneHumidityMaximum_Impl::controlZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_SetpointManager_SingleZone_Humidity_MaximumFields::ControlZoneName);
    }

    boost::optional<Node> SetpointManagerSingleZoneHumidityMaximum_Impl::setpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_SingleZone_Humidity_MaximumFields::SetpointNodeorNodeListName);
    }

    bool SetpointManagerSingleZoneHumidityMaximum_Impl::setControlVariable(const std::string& controlVariable) {
      bool result = setString(OS_SetpointManager_SingleZone_Humidity_MaximumFields::ControlVariable, controlVariable);
      return result;
    }

    bool SetpointManagerSingleZoneHumidityMaximum_Impl::setControlZone(const ThermalZone& thermalZone) {
      bool result = setPointer(OS_SetpointManager_SingleZone_Humidity_MaximumFields::ControlZoneName, thermalZone.handle());
      return result;
    }

    void SetpointManagerSingleZoneHumidityMaximum_Impl::resetControlZone() {
      bool result = setString(OS_SetpointManager_SingleZone_Humidity_MaximumFields::ControlZoneName, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerSingleZoneHumidityMaximum_Impl::setSetpointNode(const Node& node) {
      bool result = setPointer(OS_SetpointManager_SingleZone_Humidity_MaximumFields::SetpointNodeorNodeListName, node.handle());
      return result;
    }

    void SetpointManagerSingleZoneHumidityMaximum_Impl::resetSetpointNode() {
      bool result = setString(OS_SetpointManager_SingleZone_Humidity_MaximumFields::SetpointNodeorNodeListName, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  SetpointManagerSingleZoneHumidityMaximum::SetpointManagerSingleZoneHumidityMaximum(const Model& model)
    : SetpointManager(SetpointManagerSingleZoneHumidityMaximum::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SetpointManagerSingleZoneHumidityMaximum_Impl>());

    setControlVariable("MaximumHumidityRatio");
  }

  IddObjectType SetpointManagerSingleZoneHumidityMaximum::iddObjectType() {
    return {IddObjectType::OS_SetpointManager_SingleZone_Humidity_Maximum};
  }

  std::vector<std::string> SetpointManagerSingleZoneHumidityMaximum::controlVariableValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_SetpointManager_SingleZone_Humidity_MaximumFields::ControlVariable);
  }

  std::string SetpointManagerSingleZoneHumidityMaximum::controlVariable() const {
    return getImpl<detail::SetpointManagerSingleZoneHumidityMaximum_Impl>()->controlVariable();
  }

  boost::optional<ThermalZone> SetpointManagerSingleZoneHumidityMaximum::controlZone() const {
    return getImpl<detail::SetpointManagerSingleZoneHumidityMaximum_Impl>()->controlZone();
  }

  boost::optional<Node> SetpointManagerSingleZoneHumidityMaximum::setpointNode() const {
    return getImpl<detail::SetpointManagerSingleZoneHumidityMaximum_Impl>()->setpointNode();
  }

  bool SetpointManagerSingleZoneHumidityMaximum::setControlVariable(const std::string& controlVariable) {
    return getImpl<detail::SetpointManagerSingleZoneHumidityMaximum_Impl>()->setControlVariable(controlVariable);
  }

  bool SetpointManagerSingleZoneHumidityMaximum::setControlZone(const ThermalZone& thermalZone) {
    return getImpl<detail::SetpointManagerSingleZoneHumidityMaximum_Impl>()->setControlZone(thermalZone);
  }

  void SetpointManagerSingleZoneHumidityMaximum::resetControlZone() {
    getImpl<detail::SetpointManagerSingleZoneHumidityMaximum_Impl>()->resetControlZone();
  }

  /// @cond
  SetpointManagerSingleZoneHumidityMaximum::SetpointManagerSingleZoneHumidityMaximum(
    std::shared_ptr<detail::SetpointManagerSingleZoneHumidityMaximum_Impl> impl)
    : SetpointManager(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
