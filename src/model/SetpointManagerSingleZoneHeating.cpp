/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SetpointManagerSingleZoneHeating.hpp"
#include "SetpointManagerSingleZoneHeating_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_SetpointManager_SingleZone_Heating_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SetpointManagerSingleZoneHeating_Impl::SetpointManagerSingleZoneHeating_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SetpointManagerSingleZoneHeating::iddObjectType());
    }

    SetpointManagerSingleZoneHeating_Impl::SetpointManagerSingleZoneHeating_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SetpointManagerSingleZoneHeating::iddObjectType());
    }

    SetpointManagerSingleZoneHeating_Impl::SetpointManagerSingleZoneHeating_Impl(const SetpointManagerSingleZoneHeating_Impl& other,
                                                                                 Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SetpointManagerSingleZoneHeating_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SetpointManagerSingleZoneHeating_Impl::iddObjectType() const {
      return SetpointManagerSingleZoneHeating::iddObjectType();
    }

    double SetpointManagerSingleZoneHeating_Impl::minimumSupplyAirTemperature() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_SingleZone_HeatingFields::MinimumSupplyAirTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerSingleZoneHeating_Impl::maximumSupplyAirTemperature() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_SingleZone_HeatingFields::MaximumSupplyAirTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<ThermalZone> SetpointManagerSingleZoneHeating_Impl::controlZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_SetpointManager_SingleZone_HeatingFields::ControlZoneName);
    }

    bool SetpointManagerSingleZoneHeating_Impl::setMinimumSupplyAirTemperature(double minimumSupplyAirTemperature) {
      bool result = setDouble(OS_SetpointManager_SingleZone_HeatingFields::MinimumSupplyAirTemperature, minimumSupplyAirTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool SetpointManagerSingleZoneHeating_Impl::setMaximumSupplyAirTemperature(double maximumSupplyAirTemperature) {
      bool result = setDouble(OS_SetpointManager_SingleZone_HeatingFields::MaximumSupplyAirTemperature, maximumSupplyAirTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool SetpointManagerSingleZoneHeating_Impl::setControlZone(const ThermalZone& thermalZone) {
      bool result = setPointer(OS_SetpointManager_SingleZone_HeatingFields::ControlZoneName, thermalZone.handle());
      return result;
    }

    void SetpointManagerSingleZoneHeating_Impl::resetControlZone() {
      bool result = setString(OS_SetpointManager_SingleZone_HeatingFields::ControlZoneName, "");
      OS_ASSERT(result);
    }

    std::string SetpointManagerSingleZoneHeating_Impl::controlVariable() const {
      boost::optional<std::string> value = getString(OS_SetpointManager_SingleZone_HeatingFields::ControlVariable, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SetpointManagerSingleZoneHeating_Impl::setControlVariable(const std::string& value) {
      return setString(OS_SetpointManager_SingleZone_HeatingFields::ControlVariable, value);
    }

    boost::optional<Node> SetpointManagerSingleZoneHeating_Impl::setpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_SingleZone_HeatingFields::SetpointNodeorNodeListName);
    }

    bool SetpointManagerSingleZoneHeating_Impl::setSetpointNode(const Node& node) {
      return setPointer(OS_SetpointManager_SingleZone_HeatingFields::SetpointNodeorNodeListName, node.handle());
    }

    void SetpointManagerSingleZoneHeating_Impl::resetSetpointNode() {
      bool result = setString(OS_SetpointManager_SingleZone_HeatingFields::SetpointNodeorNodeListName, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerSingleZoneHeating_Impl::addToNode(Node& node) {
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

  SetpointManagerSingleZoneHeating::SetpointManagerSingleZoneHeating(const Model& model)
    : SetpointManager(SetpointManagerSingleZoneHeating::iddObjectType(), model) {
    auto impl = getImpl<detail::SetpointManagerSingleZoneHeating_Impl>();
    OS_ASSERT(impl);

    setMinimumSupplyAirTemperature(-99.0);
    setMaximumSupplyAirTemperature(99.0);
    setControlVariable("Temperature");
  }

  IddObjectType SetpointManagerSingleZoneHeating::iddObjectType() {
    return {IddObjectType::OS_SetpointManager_SingleZone_Heating};
  }

  double SetpointManagerSingleZoneHeating::minimumSupplyAirTemperature() const {
    return getImpl<detail::SetpointManagerSingleZoneHeating_Impl>()->minimumSupplyAirTemperature();
  }

  double SetpointManagerSingleZoneHeating::maximumSupplyAirTemperature() const {
    return getImpl<detail::SetpointManagerSingleZoneHeating_Impl>()->maximumSupplyAirTemperature();
  }

  boost::optional<ThermalZone> SetpointManagerSingleZoneHeating::controlZone() const {
    return getImpl<detail::SetpointManagerSingleZoneHeating_Impl>()->controlZone();
  }

  bool SetpointManagerSingleZoneHeating::setMinimumSupplyAirTemperature(double minimumSupplyAirTemperature) {
    return getImpl<detail::SetpointManagerSingleZoneHeating_Impl>()->setMinimumSupplyAirTemperature(minimumSupplyAirTemperature);
  }

  bool SetpointManagerSingleZoneHeating::setMaximumSupplyAirTemperature(double maximumSupplyAirTemperature) {
    return getImpl<detail::SetpointManagerSingleZoneHeating_Impl>()->setMaximumSupplyAirTemperature(maximumSupplyAirTemperature);
  }

  bool SetpointManagerSingleZoneHeating::setControlZone(const ThermalZone& thermalZone) {
    return getImpl<detail::SetpointManagerSingleZoneHeating_Impl>()->setControlZone(thermalZone);
  }

  void SetpointManagerSingleZoneHeating::resetControlZone() {
    getImpl<detail::SetpointManagerSingleZoneHeating_Impl>()->resetControlZone();
  }

  /// @cond
  SetpointManagerSingleZoneHeating::SetpointManagerSingleZoneHeating(std::shared_ptr<detail::SetpointManagerSingleZoneHeating_Impl> impl)
    : SetpointManager(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
