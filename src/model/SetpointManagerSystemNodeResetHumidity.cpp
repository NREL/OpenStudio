/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SetpointManagerSystemNodeResetHumidity.hpp"
#include "SetpointManagerSystemNodeResetHumidity_Impl.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"
#include "Model.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_SetpointManager_SystemNodeReset_Humidity_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SetpointManagerSystemNodeResetHumidity_Impl::SetpointManagerSystemNodeResetHumidity_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                             bool keepHandle)
      : SetpointManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SetpointManagerSystemNodeResetHumidity::iddObjectType());
    }

    SetpointManagerSystemNodeResetHumidity_Impl::SetpointManagerSystemNodeResetHumidity_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SetpointManagerSystemNodeResetHumidity::iddObjectType());
    }

    SetpointManagerSystemNodeResetHumidity_Impl::SetpointManagerSystemNodeResetHumidity_Impl(const SetpointManagerSystemNodeResetHumidity_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SetpointManagerSystemNodeResetHumidity_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType SetpointManagerSystemNodeResetHumidity_Impl::iddObjectType() const {
      return SetpointManagerSystemNodeResetHumidity::iddObjectType();
    }

    /** This SPM is **NOT** allowed on a PlantLoop */
    bool SetpointManagerSystemNodeResetHumidity_Impl::isAllowedOnPlantLoop() const {
      return false;
    }

    ModelObject SetpointManagerSystemNodeResetHumidity_Impl::clone(Model model) const {
      auto clonedObject = SetpointManager_Impl::clone(model).cast<SetpointManagerSystemNodeResetHumidity>();
      clonedObject.resetReferenceNode();
      return std::move(clonedObject);
    }

    std::string SetpointManagerSystemNodeResetHumidity_Impl::controlVariable() const {
      boost::optional<std::string> value = getString(OS_SetpointManager_SystemNodeReset_HumidityFields::ControlVariable, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerSystemNodeResetHumidity_Impl::setpointatLowReferenceHumidityRatio() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_SystemNodeReset_HumidityFields::SetpointatLowReferenceHumidityRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerSystemNodeResetHumidity_Impl::setpointatHighReferenceHumidityRatio() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_SystemNodeReset_HumidityFields::SetpointatHighReferenceHumidityRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerSystemNodeResetHumidity_Impl::lowReferenceHumidityRatio() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_SystemNodeReset_HumidityFields::LowReferenceHumidityRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerSystemNodeResetHumidity_Impl::highReferenceHumidityRatio() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_SystemNodeReset_HumidityFields::HighReferenceHumidityRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Node> SetpointManagerSystemNodeResetHumidity_Impl::referenceNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_SystemNodeReset_HumidityFields::ReferenceNodeName);
    }

    boost::optional<Node> SetpointManagerSystemNodeResetHumidity_Impl::setpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_SystemNodeReset_HumidityFields::SetpointNodeorNodeListName);
    }

    bool SetpointManagerSystemNodeResetHumidity_Impl::setControlVariable(const std::string& controlVariable) {
      bool result = setString(OS_SetpointManager_SystemNodeReset_HumidityFields::ControlVariable, controlVariable);
      return result;
    }

    bool SetpointManagerSystemNodeResetHumidity_Impl::setSetpointatLowReferenceHumidityRatio(double setpointatLowReferenceHumidityRatio) {
      bool result =
        setDouble(OS_SetpointManager_SystemNodeReset_HumidityFields::SetpointatLowReferenceHumidityRatio, setpointatLowReferenceHumidityRatio);
      OS_ASSERT(result);
      return result;
    }

    bool SetpointManagerSystemNodeResetHumidity_Impl::setSetpointatHighReferenceHumidityRatio(double setpointatHighReferenceHumidityRatio) {
      bool result =
        setDouble(OS_SetpointManager_SystemNodeReset_HumidityFields::SetpointatHighReferenceHumidityRatio, setpointatHighReferenceHumidityRatio);
      OS_ASSERT(result);
      return result;
    }

    bool SetpointManagerSystemNodeResetHumidity_Impl::setLowReferenceHumidityRatio(double lowReferenceHumidityRatio) {
      bool result = setDouble(OS_SetpointManager_SystemNodeReset_HumidityFields::LowReferenceHumidityRatio, lowReferenceHumidityRatio);
      OS_ASSERT(result);
      return result;
    }

    bool SetpointManagerSystemNodeResetHumidity_Impl::setHighReferenceHumidityRatio(double highReferenceHumidityRatio) {
      bool result = setDouble(OS_SetpointManager_SystemNodeReset_HumidityFields::HighReferenceHumidityRatio, highReferenceHumidityRatio);
      OS_ASSERT(result);
      return result;
    }

    bool SetpointManagerSystemNodeResetHumidity_Impl::setReferenceNode(const Node& node) {
      if (node.plantLoop()) {
        LOG(Error, "Cannot set the Reference node to a node that is on a PlantLoop, occurred for " << briefDescription());
        return false;
      }
      bool result = setPointer(OS_SetpointManager_SystemNodeReset_HumidityFields::ReferenceNodeName, node.handle());
      return result;
    }

    void SetpointManagerSystemNodeResetHumidity_Impl::resetReferenceNode() {
      bool result = setString(OS_SetpointManager_SystemNodeReset_HumidityFields::ReferenceNodeName, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerSystemNodeResetHumidity_Impl::setSetpointNode(const Node& node) {
      bool result = setPointer(OS_SetpointManager_SystemNodeReset_HumidityFields::SetpointNodeorNodeListName, node.handle());
      return result;
    }

    void SetpointManagerSystemNodeResetHumidity_Impl::resetSetpointNode() {
      bool result = setString(OS_SetpointManager_SystemNodeReset_HumidityFields::SetpointNodeorNodeListName, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  SetpointManagerSystemNodeResetHumidity::SetpointManagerSystemNodeResetHumidity(const Model& model)
    : SetpointManager(SetpointManagerSystemNodeResetHumidity::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SetpointManagerSystemNodeResetHumidity_Impl>());

    // From E+ 5ZoneSystemNodeReset.idf (22.1.0)
    bool ok = setControlVariable("MaximumHumidityRatio");
    OS_ASSERT(ok);
    setSetpointatLowReferenceHumidityRatio(0.00924);
    setSetpointatHighReferenceHumidityRatio(0.00600);
    setLowReferenceHumidityRatio(0.00850);
    setHighReferenceHumidityRatio(0.01000);
  }

  IddObjectType SetpointManagerSystemNodeResetHumidity::iddObjectType() {
    return IddObjectType(IddObjectType::OS_SetpointManager_SystemNodeReset_Humidity);
  }

  std::vector<std::string> SetpointManagerSystemNodeResetHumidity::controlVariableValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_SetpointManager_SystemNodeReset_HumidityFields::ControlVariable);
  }

  double SetpointManagerSystemNodeResetHumidity::setpointatLowReferenceHumidityRatio() const {
    return getImpl<detail::SetpointManagerSystemNodeResetHumidity_Impl>()->setpointatLowReferenceHumidityRatio();
  }

  double SetpointManagerSystemNodeResetHumidity::setpointatHighReferenceHumidityRatio() const {
    return getImpl<detail::SetpointManagerSystemNodeResetHumidity_Impl>()->setpointatHighReferenceHumidityRatio();
  }

  double SetpointManagerSystemNodeResetHumidity::lowReferenceHumidityRatio() const {
    return getImpl<detail::SetpointManagerSystemNodeResetHumidity_Impl>()->lowReferenceHumidityRatio();
  }

  double SetpointManagerSystemNodeResetHumidity::highReferenceHumidityRatio() const {
    return getImpl<detail::SetpointManagerSystemNodeResetHumidity_Impl>()->highReferenceHumidityRatio();
  }

  boost::optional<Node> SetpointManagerSystemNodeResetHumidity::referenceNode() const {
    return getImpl<detail::SetpointManagerSystemNodeResetHumidity_Impl>()->referenceNode();
  }

  bool SetpointManagerSystemNodeResetHumidity::setSetpointatLowReferenceHumidityRatio(double setpointatLowReferenceHumidityRatio) {
    return getImpl<detail::SetpointManagerSystemNodeResetHumidity_Impl>()->setSetpointatLowReferenceHumidityRatio(
      setpointatLowReferenceHumidityRatio);
  }

  bool SetpointManagerSystemNodeResetHumidity::setSetpointatHighReferenceHumidityRatio(double setpointatHighReferenceHumidityRatio) {
    return getImpl<detail::SetpointManagerSystemNodeResetHumidity_Impl>()->setSetpointatHighReferenceHumidityRatio(
      setpointatHighReferenceHumidityRatio);
  }

  bool SetpointManagerSystemNodeResetHumidity::setLowReferenceHumidityRatio(double lowReferenceHumidityRatio) {
    return getImpl<detail::SetpointManagerSystemNodeResetHumidity_Impl>()->setLowReferenceHumidityRatio(lowReferenceHumidityRatio);
  }

  bool SetpointManagerSystemNodeResetHumidity::setHighReferenceHumidityRatio(double highReferenceHumidityRatio) {
    return getImpl<detail::SetpointManagerSystemNodeResetHumidity_Impl>()->setHighReferenceHumidityRatio(highReferenceHumidityRatio);
  }

  bool SetpointManagerSystemNodeResetHumidity::setReferenceNode(const Node& node) {
    return getImpl<detail::SetpointManagerSystemNodeResetHumidity_Impl>()->setReferenceNode(node);
  }

  void SetpointManagerSystemNodeResetHumidity::resetReferenceNode() {
    getImpl<detail::SetpointManagerSystemNodeResetHumidity_Impl>()->resetReferenceNode();
  }

  /// @cond
  SetpointManagerSystemNodeResetHumidity::SetpointManagerSystemNodeResetHumidity(
    std::shared_ptr<detail::SetpointManagerSystemNodeResetHumidity_Impl> impl)
    : SetpointManager(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
