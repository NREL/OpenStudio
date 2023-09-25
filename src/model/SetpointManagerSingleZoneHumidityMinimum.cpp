/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SetpointManagerSingleZoneHumidityMinimum.hpp"
#include "SetpointManagerSingleZoneHumidityMinimum_Impl.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "AirLoopHVAC.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SetpointManager_SingleZone_Humidity_Minimum_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SetpointManagerSingleZoneHumidityMinimum_Impl::SetpointManagerSingleZoneHumidityMinimum_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                 bool keepHandle)
      : SetpointManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SetpointManagerSingleZoneHumidityMinimum::iddObjectType());
    }

    SetpointManagerSingleZoneHumidityMinimum_Impl::SetpointManagerSingleZoneHumidityMinimum_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SetpointManagerSingleZoneHumidityMinimum::iddObjectType());
    }

    SetpointManagerSingleZoneHumidityMinimum_Impl::SetpointManagerSingleZoneHumidityMinimum_Impl(
      const SetpointManagerSingleZoneHumidityMinimum_Impl& other, Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SetpointManagerSingleZoneHumidityMinimum_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SetpointManagerSingleZoneHumidityMinimum_Impl::iddObjectType() const {
      return SetpointManagerSingleZoneHumidityMinimum::iddObjectType();
    }

    bool SetpointManagerSingleZoneHumidityMinimum_Impl::addToNode(Node& node) {
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

    ModelObject SetpointManagerSingleZoneHumidityMinimum_Impl::clone(Model model) const {
      auto clonedObject = SetpointManager_Impl::clone(model).cast<SetpointManagerSingleZoneHumidityMinimum>();
      clonedObject.resetControlZone();
      return std::move(clonedObject);
    }

    std::string SetpointManagerSingleZoneHumidityMinimum_Impl::controlVariable() const {
      boost::optional<std::string> value = getString(OS_SetpointManager_SingleZone_Humidity_MinimumFields::ControlVariable, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Node> SetpointManagerSingleZoneHumidityMinimum_Impl::setpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_SingleZone_Humidity_MinimumFields::SetpointNodeorNodeListName);
    }

    boost::optional<ThermalZone> SetpointManagerSingleZoneHumidityMinimum_Impl::controlZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_SetpointManager_SingleZone_Humidity_MinimumFields::ControlZoneName);
    }

    bool SetpointManagerSingleZoneHumidityMinimum_Impl::setControlVariable(const std::string& controlVariable) {
      bool result = setString(OS_SetpointManager_SingleZone_Humidity_MinimumFields::ControlVariable, controlVariable);
      return result;
    }

    bool SetpointManagerSingleZoneHumidityMinimum_Impl::setSetpointNode(const Node& node) {
      bool result = setPointer(OS_SetpointManager_SingleZone_Humidity_MinimumFields::SetpointNodeorNodeListName, node.handle());
      return result;
    }

    void SetpointManagerSingleZoneHumidityMinimum_Impl::resetSetpointNode() {
      bool result = setString(OS_SetpointManager_SingleZone_Humidity_MinimumFields::SetpointNodeorNodeListName, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerSingleZoneHumidityMinimum_Impl::setControlZone(const ThermalZone& thermalZone) {
      bool result = setPointer(OS_SetpointManager_SingleZone_Humidity_MinimumFields::ControlZoneName, thermalZone.handle());
      return result;
    }

    void SetpointManagerSingleZoneHumidityMinimum_Impl::resetControlZone() {
      setString(OS_SetpointManager_SingleZone_Humidity_MinimumFields::ControlZoneName, "");
    }

  }  // namespace detail

  SetpointManagerSingleZoneHumidityMinimum::SetpointManagerSingleZoneHumidityMinimum(const Model& model)
    : SetpointManager(SetpointManagerSingleZoneHumidityMinimum::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>());

    bool ok = setControlVariable("MinimumHumidityRatio");
    OS_ASSERT(ok);
  }

  IddObjectType SetpointManagerSingleZoneHumidityMinimum::iddObjectType() {
    return {IddObjectType::OS_SetpointManager_SingleZone_Humidity_Minimum};
  }

  std::vector<std::string> SetpointManagerSingleZoneHumidityMinimum::controlVariableValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_SetpointManager_SingleZone_Humidity_MinimumFields::ControlVariable);
  }

  std::string SetpointManagerSingleZoneHumidityMinimum::controlVariable() const {
    return getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>()->controlVariable();
  }

  boost::optional<Node> SetpointManagerSingleZoneHumidityMinimum::setpointNode() const {
    return getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>()->setpointNode();
  }

  boost::optional<ThermalZone> SetpointManagerSingleZoneHumidityMinimum::controlZone() const {
    return getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>()->controlZone();
  }

  bool SetpointManagerSingleZoneHumidityMinimum::setControlVariable(const std::string& controlVariable) {
    return getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>()->setControlVariable(controlVariable);
  }

  bool SetpointManagerSingleZoneHumidityMinimum::setControlZone(const ThermalZone& thermalZone) {
    return getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>()->setControlZone(thermalZone);
  }

  void SetpointManagerSingleZoneHumidityMinimum::resetControlZone() {
    getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>()->resetControlZone();
  }

  /// @cond
  SetpointManagerSingleZoneHumidityMinimum::SetpointManagerSingleZoneHumidityMinimum(
    std::shared_ptr<detail::SetpointManagerSingleZoneHumidityMinimum_Impl> impl)
    : SetpointManager(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
