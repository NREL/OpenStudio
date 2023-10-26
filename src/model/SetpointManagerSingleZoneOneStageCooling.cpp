/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SetpointManagerSingleZoneOneStageCooling.hpp"
#include "SetpointManagerSingleZoneOneStageCooling_Impl.hpp"

#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "AirLoopHVAC.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SetpointManager_SingleZone_OneStageCooling_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SetpointManagerSingleZoneOneStageCooling_Impl::SetpointManagerSingleZoneOneStageCooling_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                 bool keepHandle)
      : SetpointManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SetpointManagerSingleZoneOneStageCooling::iddObjectType());
    }

    SetpointManagerSingleZoneOneStageCooling_Impl::SetpointManagerSingleZoneOneStageCooling_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SetpointManagerSingleZoneOneStageCooling::iddObjectType());
    }

    SetpointManagerSingleZoneOneStageCooling_Impl::SetpointManagerSingleZoneOneStageCooling_Impl(
      const SetpointManagerSingleZoneOneStageCooling_Impl& other, Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SetpointManagerSingleZoneOneStageCooling_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SetpointManagerSingleZoneOneStageCooling_Impl::iddObjectType() const {
      return SetpointManagerSingleZoneOneStageCooling::iddObjectType();
    }

    bool SetpointManagerSingleZoneOneStageCooling_Impl::addToNode(Node& node) {
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

    ModelObject SetpointManagerSingleZoneOneStageCooling_Impl::clone(Model model) const {
      auto clonedObject = SetpointManager_Impl::clone(model).cast<SetpointManagerSingleZoneOneStageCooling>();
      clonedObject.resetControlZone();
      return std::move(clonedObject);
    }

    std::string SetpointManagerSingleZoneOneStageCooling_Impl::controlVariable() const {
      boost::optional<std::string> value = getString(OS_SetpointManager_SingleZone_OneStageCoolingFields::ControlVariable, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerSingleZoneOneStageCooling_Impl::coolingStageOnSupplyAirSetpointTemperature() const {
      boost::optional<double> value =
        getDouble(OS_SetpointManager_SingleZone_OneStageCoolingFields::CoolingStageOnSupplyAirSetpointTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerSingleZoneOneStageCooling_Impl::coolingStageOffSupplyAirSetpointTemperature() const {
      boost::optional<double> value =
        getDouble(OS_SetpointManager_SingleZone_OneStageCoolingFields::CoolingStageOffSupplyAirSetpointTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<ThermalZone> SetpointManagerSingleZoneOneStageCooling_Impl::controlZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_SetpointManager_SingleZone_OneStageCoolingFields::ControlZoneName);
    }

    boost::optional<Node> SetpointManagerSingleZoneOneStageCooling_Impl::setpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_SingleZone_OneStageCoolingFields::SetpointNodeorNodeListName);
    }

    bool SetpointManagerSingleZoneOneStageCooling_Impl::setControlVariable(const std::string& controlVariable) {
      bool result = setString(OS_SetpointManager_SingleZone_OneStageCoolingFields::ControlVariable, controlVariable);
      return result;
    }

    bool SetpointManagerSingleZoneOneStageCooling_Impl::setCoolingStageOnSupplyAirSetpointTemperature(
      double coolingStageOnSupplyAirSetpointTemperature) {
      bool result = setDouble(OS_SetpointManager_SingleZone_OneStageCoolingFields::CoolingStageOnSupplyAirSetpointTemperature,
                              coolingStageOnSupplyAirSetpointTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool SetpointManagerSingleZoneOneStageCooling_Impl::setCoolingStageOffSupplyAirSetpointTemperature(
      double coolingStageOffSupplyAirSetpointTemperature) {
      bool result = setDouble(OS_SetpointManager_SingleZone_OneStageCoolingFields::CoolingStageOffSupplyAirSetpointTemperature,
                              coolingStageOffSupplyAirSetpointTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool SetpointManagerSingleZoneOneStageCooling_Impl::setControlZone(const ThermalZone& thermalZone) {
      bool result = setPointer(OS_SetpointManager_SingleZone_OneStageCoolingFields::ControlZoneName, thermalZone.handle());
      return result;
    }

    void SetpointManagerSingleZoneOneStageCooling_Impl::resetControlZone() {
      bool result = setString(OS_SetpointManager_SingleZone_OneStageCoolingFields::ControlZoneName, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerSingleZoneOneStageCooling_Impl::setSetpointNode(const Node& node) {
      bool result = setPointer(OS_SetpointManager_SingleZone_OneStageCoolingFields::SetpointNodeorNodeListName, node.handle());
      return result;
    }

    void SetpointManagerSingleZoneOneStageCooling_Impl::resetSetpointNode() {
      bool result = setString(OS_SetpointManager_SingleZone_OneStageCoolingFields::SetpointNodeorNodeListName, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  SetpointManagerSingleZoneOneStageCooling::SetpointManagerSingleZoneOneStageCooling(const Model& model)
    : SetpointManager(SetpointManagerSingleZoneOneStageCooling::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>());

    setControlVariable("Temperature");
    setCoolingStageOnSupplyAirSetpointTemperature(-99);
    setCoolingStageOffSupplyAirSetpointTemperature(99);
  }

  IddObjectType SetpointManagerSingleZoneOneStageCooling::iddObjectType() {
    return {IddObjectType::OS_SetpointManager_SingleZone_OneStageCooling};
  }

  std::vector<std::string> SetpointManagerSingleZoneOneStageCooling::controlVariableValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_SetpointManager_SingleZone_OneStageCoolingFields::ControlVariable);
  }

  std::string SetpointManagerSingleZoneOneStageCooling::controlVariable() const {
    return getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->controlVariable();
  }

  double SetpointManagerSingleZoneOneStageCooling::coolingStageOnSupplyAirSetpointTemperature() const {
    return getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->coolingStageOnSupplyAirSetpointTemperature();
  }

  double SetpointManagerSingleZoneOneStageCooling::coolingStageOffSupplyAirSetpointTemperature() const {
    return getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->coolingStageOffSupplyAirSetpointTemperature();
  }

  boost::optional<ThermalZone> SetpointManagerSingleZoneOneStageCooling::controlZone() const {
    return getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->controlZone();
  }

  boost::optional<Node> SetpointManagerSingleZoneOneStageCooling::setpointNode() const {
    return getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->setpointNode();
  }

  bool SetpointManagerSingleZoneOneStageCooling::setControlVariable(const std::string& controlVariable) {
    return getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->setControlVariable(controlVariable);
  }

  bool SetpointManagerSingleZoneOneStageCooling::setCoolingStageOnSupplyAirSetpointTemperature(double coolingStageOnSupplyAirSetpointTemperature) {
    return getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->setCoolingStageOnSupplyAirSetpointTemperature(
      coolingStageOnSupplyAirSetpointTemperature);
  }

  bool SetpointManagerSingleZoneOneStageCooling::setCoolingStageOffSupplyAirSetpointTemperature(double coolingStageOffSupplyAirSetpointTemperature) {
    return getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->setCoolingStageOffSupplyAirSetpointTemperature(
      coolingStageOffSupplyAirSetpointTemperature);
  }

  bool SetpointManagerSingleZoneOneStageCooling::setControlZone(const ThermalZone& thermalZone) {
    return getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->setControlZone(thermalZone);
  }

  void SetpointManagerSingleZoneOneStageCooling::resetControlZone() {
    getImpl<detail::SetpointManagerSingleZoneOneStageCooling_Impl>()->resetControlZone();
  }

  /// @cond
  SetpointManagerSingleZoneOneStageCooling::SetpointManagerSingleZoneOneStageCooling(
    std::shared_ptr<detail::SetpointManagerSingleZoneOneStageCooling_Impl> impl)
    : SetpointManager(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
