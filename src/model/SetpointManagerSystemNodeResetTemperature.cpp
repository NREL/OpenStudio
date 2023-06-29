/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SetpointManagerSystemNodeResetTemperature.hpp"
#include "SetpointManagerSystemNodeResetTemperature_Impl.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"
#include "Model.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_SetpointManager_SystemNodeReset_Temperature_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SetpointManagerSystemNodeResetTemperature_Impl::SetpointManagerSystemNodeResetTemperature_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                   bool keepHandle)
      : SetpointManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SetpointManagerSystemNodeResetTemperature::iddObjectType());
    }

    SetpointManagerSystemNodeResetTemperature_Impl::SetpointManagerSystemNodeResetTemperature_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SetpointManagerSystemNodeResetTemperature::iddObjectType());
    }

    SetpointManagerSystemNodeResetTemperature_Impl::SetpointManagerSystemNodeResetTemperature_Impl(
      const SetpointManagerSystemNodeResetTemperature_Impl& other, Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SetpointManagerSystemNodeResetTemperature_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType SetpointManagerSystemNodeResetTemperature_Impl::iddObjectType() const {
      return SetpointManagerSystemNodeResetTemperature::iddObjectType();
    }

    /** This SPM **IS** allowed on a PlantLoop */
    bool SetpointManagerSystemNodeResetTemperature_Impl::isAllowedOnPlantLoop() const {
      return true;
    }

    ModelObject SetpointManagerSystemNodeResetTemperature_Impl::clone(Model model) const {
      auto clonedObject = SetpointManager_Impl::clone(model).cast<SetpointManagerSystemNodeResetTemperature>();
      clonedObject.resetReferenceNode();
      return std::move(clonedObject);
    }

    std::string SetpointManagerSystemNodeResetTemperature_Impl::controlVariable() const {
      boost::optional<std::string> value = getString(OS_SetpointManager_SystemNodeReset_TemperatureFields::ControlVariable, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerSystemNodeResetTemperature_Impl::setpointatLowReferenceTemperature() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_SystemNodeReset_TemperatureFields::SetpointatLowReferenceTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerSystemNodeResetTemperature_Impl::setpointatHighReferenceTemperature() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_SystemNodeReset_TemperatureFields::SetpointatHighReferenceTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerSystemNodeResetTemperature_Impl::lowReferenceTemperature() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_SystemNodeReset_TemperatureFields::LowReferenceTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerSystemNodeResetTemperature_Impl::highReferenceTemperature() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_SystemNodeReset_TemperatureFields::HighReferenceTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Node> SetpointManagerSystemNodeResetTemperature_Impl::referenceNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_SystemNodeReset_TemperatureFields::ReferenceNodeName);
    }

    boost::optional<Node> SetpointManagerSystemNodeResetTemperature_Impl::setpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_SystemNodeReset_TemperatureFields::SetpointNodeorNodeListName);
    }

    bool SetpointManagerSystemNodeResetTemperature_Impl::setControlVariable(const std::string& controlVariable) {
      bool result = setString(OS_SetpointManager_SystemNodeReset_TemperatureFields::ControlVariable, controlVariable);
      return result;
    }

    bool SetpointManagerSystemNodeResetTemperature_Impl::setSetpointatLowReferenceTemperature(double setpointatLowReferenceTemperature) {
      bool result =
        setDouble(OS_SetpointManager_SystemNodeReset_TemperatureFields::SetpointatLowReferenceTemperature, setpointatLowReferenceTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool SetpointManagerSystemNodeResetTemperature_Impl::setSetpointatHighReferenceTemperature(double setpointatHighReferenceTemperature) {
      bool result =
        setDouble(OS_SetpointManager_SystemNodeReset_TemperatureFields::SetpointatHighReferenceTemperature, setpointatHighReferenceTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool SetpointManagerSystemNodeResetTemperature_Impl::setLowReferenceTemperature(double lowReferenceTemperature) {
      bool result = setDouble(OS_SetpointManager_SystemNodeReset_TemperatureFields::LowReferenceTemperature, lowReferenceTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool SetpointManagerSystemNodeResetTemperature_Impl::setHighReferenceTemperature(double highReferenceTemperature) {
      bool result = setDouble(OS_SetpointManager_SystemNodeReset_TemperatureFields::HighReferenceTemperature, highReferenceTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool SetpointManagerSystemNodeResetTemperature_Impl::setReferenceNode(const Node& node) {
      bool result = setPointer(OS_SetpointManager_SystemNodeReset_TemperatureFields::ReferenceNodeName, node.handle());
      return result;
    }

    void SetpointManagerSystemNodeResetTemperature_Impl::resetReferenceNode() {
      bool result = setString(OS_SetpointManager_SystemNodeReset_TemperatureFields::ReferenceNodeName, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerSystemNodeResetTemperature_Impl::setSetpointNode(const Node& node) {
      bool result = setPointer(OS_SetpointManager_SystemNodeReset_TemperatureFields::SetpointNodeorNodeListName, node.handle());
      return result;
    }

    void SetpointManagerSystemNodeResetTemperature_Impl::resetSetpointNode() {
      bool result = setString(OS_SetpointManager_SystemNodeReset_TemperatureFields::SetpointNodeorNodeListName, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  SetpointManagerSystemNodeResetTemperature::SetpointManagerSystemNodeResetTemperature(const Model& model)
    : SetpointManager(SetpointManagerSystemNodeResetTemperature::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SetpointManagerSystemNodeResetTemperature_Impl>());

    // From E+ 5ZoneSystemNodeReset.idf (22.1.0)
    bool ok = setControlVariable("Temperature");
    OS_ASSERT(ok);
    setSetpointatLowReferenceTemperature(16.7);
    setSetpointatHighReferenceTemperature(12.8);
    setLowReferenceTemperature(20.0);
    setHighReferenceTemperature(23.3);
  }

  IddObjectType SetpointManagerSystemNodeResetTemperature::iddObjectType() {
    return IddObjectType(IddObjectType::OS_SetpointManager_SystemNodeReset_Temperature);
  }

  std::vector<std::string> SetpointManagerSystemNodeResetTemperature::controlVariableValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_SetpointManager_SystemNodeReset_TemperatureFields::ControlVariable);
  }

  double SetpointManagerSystemNodeResetTemperature::setpointatLowReferenceTemperature() const {
    return getImpl<detail::SetpointManagerSystemNodeResetTemperature_Impl>()->setpointatLowReferenceTemperature();
  }

  double SetpointManagerSystemNodeResetTemperature::setpointatHighReferenceTemperature() const {
    return getImpl<detail::SetpointManagerSystemNodeResetTemperature_Impl>()->setpointatHighReferenceTemperature();
  }

  double SetpointManagerSystemNodeResetTemperature::lowReferenceTemperature() const {
    return getImpl<detail::SetpointManagerSystemNodeResetTemperature_Impl>()->lowReferenceTemperature();
  }

  double SetpointManagerSystemNodeResetTemperature::highReferenceTemperature() const {
    return getImpl<detail::SetpointManagerSystemNodeResetTemperature_Impl>()->highReferenceTemperature();
  }

  boost::optional<Node> SetpointManagerSystemNodeResetTemperature::referenceNode() const {
    return getImpl<detail::SetpointManagerSystemNodeResetTemperature_Impl>()->referenceNode();
  }

  bool SetpointManagerSystemNodeResetTemperature::setSetpointatLowReferenceTemperature(double setpointatLowReferenceTemperature) {
    return getImpl<detail::SetpointManagerSystemNodeResetTemperature_Impl>()->setSetpointatLowReferenceTemperature(setpointatLowReferenceTemperature);
  }

  bool SetpointManagerSystemNodeResetTemperature::setSetpointatHighReferenceTemperature(double setpointatHighReferenceTemperature) {
    return getImpl<detail::SetpointManagerSystemNodeResetTemperature_Impl>()->setSetpointatHighReferenceTemperature(
      setpointatHighReferenceTemperature);
  }

  bool SetpointManagerSystemNodeResetTemperature::setLowReferenceTemperature(double lowReferenceTemperature) {
    return getImpl<detail::SetpointManagerSystemNodeResetTemperature_Impl>()->setLowReferenceTemperature(lowReferenceTemperature);
  }

  bool SetpointManagerSystemNodeResetTemperature::setHighReferenceTemperature(double highReferenceTemperature) {
    return getImpl<detail::SetpointManagerSystemNodeResetTemperature_Impl>()->setHighReferenceTemperature(highReferenceTemperature);
  }

  bool SetpointManagerSystemNodeResetTemperature::setReferenceNode(const Node& node) {
    return getImpl<detail::SetpointManagerSystemNodeResetTemperature_Impl>()->setReferenceNode(node);
  }

  void SetpointManagerSystemNodeResetTemperature::resetReferenceNode() {
    getImpl<detail::SetpointManagerSystemNodeResetTemperature_Impl>()->resetReferenceNode();
  }

  /// @cond
  SetpointManagerSystemNodeResetTemperature::SetpointManagerSystemNodeResetTemperature(
    std::shared_ptr<detail::SetpointManagerSystemNodeResetTemperature_Impl> impl)
    : SetpointManager(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
