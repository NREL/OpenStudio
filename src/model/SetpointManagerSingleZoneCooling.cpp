/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SetpointManagerSingleZoneCooling.hpp"
#include "SetpointManagerSingleZoneCooling_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_SetpointManager_SingleZone_Cooling_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SetpointManagerSingleZoneCooling_Impl::SetpointManagerSingleZoneCooling_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SetpointManagerSingleZoneCooling::iddObjectType());
    }

    SetpointManagerSingleZoneCooling_Impl::SetpointManagerSingleZoneCooling_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SetpointManagerSingleZoneCooling::iddObjectType());
    }

    SetpointManagerSingleZoneCooling_Impl::SetpointManagerSingleZoneCooling_Impl(const SetpointManagerSingleZoneCooling_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SetpointManagerSingleZoneCooling_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SetpointManagerSingleZoneCooling_Impl::iddObjectType() const {
      return SetpointManagerSingleZoneCooling::iddObjectType();
    }

    double SetpointManagerSingleZoneCooling_Impl::minimumSupplyAirTemperature() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_SingleZone_CoolingFields::MinimumSupplyAirTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerSingleZoneCooling_Impl::maximumSupplyAirTemperature() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_SingleZone_CoolingFields::MaximumSupplyAirTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<ThermalZone> SetpointManagerSingleZoneCooling_Impl::controlZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_SetpointManager_SingleZone_CoolingFields::ControlZoneName);
    }

    bool SetpointManagerSingleZoneCooling_Impl::setMinimumSupplyAirTemperature(double minimumSupplyAirTemperature) {
      bool result = setDouble(OS_SetpointManager_SingleZone_CoolingFields::MinimumSupplyAirTemperature, minimumSupplyAirTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool SetpointManagerSingleZoneCooling_Impl::setMaximumSupplyAirTemperature(double maximumSupplyAirTemperature) {
      bool result = setDouble(OS_SetpointManager_SingleZone_CoolingFields::MaximumSupplyAirTemperature, maximumSupplyAirTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool SetpointManagerSingleZoneCooling_Impl::setControlZone(const ThermalZone& thermalZone) {
      bool result = setPointer(OS_SetpointManager_SingleZone_CoolingFields::ControlZoneName, thermalZone.handle());
      return result;
    }

    void SetpointManagerSingleZoneCooling_Impl::resetControlZone() {
      bool result = setString(OS_SetpointManager_SingleZone_CoolingFields::ControlZoneName, "");
      OS_ASSERT(result);
    }

    std::string SetpointManagerSingleZoneCooling_Impl::controlVariable() const {
      boost::optional<std::string> value = getString(OS_SetpointManager_SingleZone_CoolingFields::ControlVariable, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SetpointManagerSingleZoneCooling_Impl::setControlVariable(const std::string& value) {
      return setString(OS_SetpointManager_SingleZone_CoolingFields::ControlVariable, value);
    }

    boost::optional<Node> SetpointManagerSingleZoneCooling_Impl::setpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_SingleZone_CoolingFields::SetpointNodeorNodeListName);
    }

    bool SetpointManagerSingleZoneCooling_Impl::setSetpointNode(const Node& node) {
      return setPointer(OS_SetpointManager_SingleZone_CoolingFields::SetpointNodeorNodeListName, node.handle());
    }

    void SetpointManagerSingleZoneCooling_Impl::resetSetpointNode() {
      bool result = setString(OS_SetpointManager_SingleZone_CoolingFields::SetpointNodeorNodeListName, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerSingleZoneCooling_Impl::addToNode(Node& node) {
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

  }  // namespace detail

  SetpointManagerSingleZoneCooling::SetpointManagerSingleZoneCooling(const Model& model)
    : SetpointManager(SetpointManagerSingleZoneCooling::iddObjectType(), model) {
    auto impl = getImpl<detail::SetpointManagerSingleZoneCooling_Impl>();
    OS_ASSERT(impl);

    setMinimumSupplyAirTemperature(-99.0);
    setMaximumSupplyAirTemperature(99.0);
    setControlVariable("Temperature");
  }

  IddObjectType SetpointManagerSingleZoneCooling::iddObjectType() {
    return {IddObjectType::OS_SetpointManager_SingleZone_Cooling};
  }

  double SetpointManagerSingleZoneCooling::minimumSupplyAirTemperature() const {
    return getImpl<detail::SetpointManagerSingleZoneCooling_Impl>()->minimumSupplyAirTemperature();
  }

  double SetpointManagerSingleZoneCooling::maximumSupplyAirTemperature() const {
    return getImpl<detail::SetpointManagerSingleZoneCooling_Impl>()->maximumSupplyAirTemperature();
  }

  boost::optional<ThermalZone> SetpointManagerSingleZoneCooling::controlZone() const {
    return getImpl<detail::SetpointManagerSingleZoneCooling_Impl>()->controlZone();
  }

  bool SetpointManagerSingleZoneCooling::setMinimumSupplyAirTemperature(double minimumSupplyAirTemperature) {
    return getImpl<detail::SetpointManagerSingleZoneCooling_Impl>()->setMinimumSupplyAirTemperature(minimumSupplyAirTemperature);
  }

  bool SetpointManagerSingleZoneCooling::setMaximumSupplyAirTemperature(double maximumSupplyAirTemperature) {
    return getImpl<detail::SetpointManagerSingleZoneCooling_Impl>()->setMaximumSupplyAirTemperature(maximumSupplyAirTemperature);
  }

  bool SetpointManagerSingleZoneCooling::setControlZone(const ThermalZone& thermalZone) {
    return getImpl<detail::SetpointManagerSingleZoneCooling_Impl>()->setControlZone(thermalZone);
  }

  void SetpointManagerSingleZoneCooling::resetControlZone() {
    getImpl<detail::SetpointManagerSingleZoneCooling_Impl>()->resetControlZone();
  }

  /// @cond
  SetpointManagerSingleZoneCooling::SetpointManagerSingleZoneCooling(std::shared_ptr<detail::SetpointManagerSingleZoneCooling_Impl> impl)
    : SetpointManager(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
