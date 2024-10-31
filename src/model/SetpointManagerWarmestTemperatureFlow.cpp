/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SetpointManagerWarmestTemperatureFlow.hpp"
#include "SetpointManagerWarmestTemperatureFlow_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SetpointManager_WarmestTemperatureFlow_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SetpointManagerWarmestTemperatureFlow_Impl::SetpointManagerWarmestTemperatureFlow_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                           bool keepHandle)
      : SetpointManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SetpointManagerWarmestTemperatureFlow::iddObjectType());
    }

    SetpointManagerWarmestTemperatureFlow_Impl::SetpointManagerWarmestTemperatureFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                           Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SetpointManagerWarmestTemperatureFlow::iddObjectType());
    }

    SetpointManagerWarmestTemperatureFlow_Impl::SetpointManagerWarmestTemperatureFlow_Impl(const SetpointManagerWarmestTemperatureFlow_Impl& other,
                                                                                           Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SetpointManagerWarmestTemperatureFlow_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SetpointManagerWarmestTemperatureFlow_Impl::iddObjectType() const {
      return SetpointManagerWarmestTemperatureFlow::iddObjectType();
    }

    std::string SetpointManagerWarmestTemperatureFlow_Impl::controlVariable() const {
      boost::optional<std::string> value = getString(OS_SetpointManager_WarmestTemperatureFlowFields::ControlVariable, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerWarmestTemperatureFlow_Impl::minimumSetpointTemperature() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_WarmestTemperatureFlowFields::MinimumSetpointTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerWarmestTemperatureFlow_Impl::maximumSetpointTemperature() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_WarmestTemperatureFlowFields::MaximumSetpointTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string SetpointManagerWarmestTemperatureFlow_Impl::strategy() const {
      boost::optional<std::string> value = getString(OS_SetpointManager_WarmestTemperatureFlowFields::Strategy, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerWarmestTemperatureFlow_Impl::minimumTurndownRatio() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_WarmestTemperatureFlowFields::MinimumTurndownRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SetpointManagerWarmestTemperatureFlow_Impl::setControlVariable(const std::string& controlVariable) {
      bool result = setString(OS_SetpointManager_WarmestTemperatureFlowFields::ControlVariable, controlVariable);
      return result;
    }

    bool SetpointManagerWarmestTemperatureFlow_Impl::setMinimumSetpointTemperature(double minimumSetpointTemperature) {
      bool result = setDouble(OS_SetpointManager_WarmestTemperatureFlowFields::MinimumSetpointTemperature, minimumSetpointTemperature);
      return result;
    }

    bool SetpointManagerWarmestTemperatureFlow_Impl::setMaximumSetpointTemperature(double maximumSetpointTemperature) {
      bool result = setDouble(OS_SetpointManager_WarmestTemperatureFlowFields::MaximumSetpointTemperature, maximumSetpointTemperature);
      return result;
    }

    bool SetpointManagerWarmestTemperatureFlow_Impl::setStrategy(const std::string& strategy) {
      bool result = setString(OS_SetpointManager_WarmestTemperatureFlowFields::Strategy, strategy);
      return result;
    }

    bool SetpointManagerWarmestTemperatureFlow_Impl::setMinimumTurndownRatio(double minimumTurndownRatio) {
      bool result = setDouble(OS_SetpointManager_WarmestTemperatureFlowFields::MinimumTurndownRatio, minimumTurndownRatio);
      return result;
    }

    //std::string SetpointManagerWarmestTemperatureFlow_Impl::controlVariable() const {
    //}

    //bool SetpointManagerWarmestTemperatureFlow_Impl::setControlVariable( const std::string & value ) {
    //}

    boost::optional<Node> SetpointManagerWarmestTemperatureFlow_Impl::setpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_WarmestTemperatureFlowFields::SetpointNodeorNodeListName);
    }

    bool SetpointManagerWarmestTemperatureFlow_Impl::setSetpointNode(const Node& node) {
      bool result = setPointer(OS_SetpointManager_WarmestTemperatureFlowFields::SetpointNodeorNodeListName, node.handle());
      return result;
    }

    void SetpointManagerWarmestTemperatureFlow_Impl::resetSetpointNode() {
      setString(OS_SetpointManager_WarmestTemperatureFlowFields::SetpointNodeorNodeListName, "");
    }

  }  // namespace detail

  SetpointManagerWarmestTemperatureFlow::SetpointManagerWarmestTemperatureFlow(const Model& model)
    : SetpointManager(SetpointManagerWarmestTemperatureFlow::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>());

    setControlVariable("Temperature");
    setMinimumSetpointTemperature(12.0);
    setMaximumSetpointTemperature(18.0);
    setStrategy("TemperatureFirst");
    setMinimumTurndownRatio(0.2);
  }

  IddObjectType SetpointManagerWarmestTemperatureFlow::iddObjectType() {
    return {IddObjectType::OS_SetpointManager_WarmestTemperatureFlow};
  }

  std::vector<std::string> SetpointManagerWarmestTemperatureFlow::controlVariableValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_SetpointManager_WarmestTemperatureFlowFields::ControlVariable);
  }

  std::vector<std::string> SetpointManagerWarmestTemperatureFlow::strategyValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_SetpointManager_WarmestTemperatureFlowFields::Strategy);
  }

  double SetpointManagerWarmestTemperatureFlow::minimumSetpointTemperature() const {
    return getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->minimumSetpointTemperature();
  }

  double SetpointManagerWarmestTemperatureFlow::maximumSetpointTemperature() const {
    return getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->maximumSetpointTemperature();
  }

  std::string SetpointManagerWarmestTemperatureFlow::strategy() const {
    return getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->strategy();
  }

  double SetpointManagerWarmestTemperatureFlow::minimumTurndownRatio() const {
    return getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->minimumTurndownRatio();
  }

  bool SetpointManagerWarmestTemperatureFlow::setMinimumSetpointTemperature(double minimumSetpointTemperature) {
    return getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->setMinimumSetpointTemperature(minimumSetpointTemperature);
  }

  bool SetpointManagerWarmestTemperatureFlow::setMaximumSetpointTemperature(double maximumSetpointTemperature) {
    return getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->setMaximumSetpointTemperature(maximumSetpointTemperature);
  }

  bool SetpointManagerWarmestTemperatureFlow::setStrategy(const std::string& strategy) {
    return getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->setStrategy(strategy);
  }

  bool SetpointManagerWarmestTemperatureFlow::setMinimumTurndownRatio(double minimumTurndownRatio) {
    return getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->setMinimumTurndownRatio(minimumTurndownRatio);
  }

  bool SetpointManagerWarmestTemperatureFlow::setSetpointNode(const Node& node) {
    return getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->setSetpointNode(node);
  }

  void SetpointManagerWarmestTemperatureFlow::resetSetpointNode() {
    getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->resetSetpointNode();
  }

  /// @cond
  SetpointManagerWarmestTemperatureFlow::SetpointManagerWarmestTemperatureFlow(
    std::shared_ptr<detail::SetpointManagerWarmestTemperatureFlow_Impl> impl)
    : SetpointManager(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
