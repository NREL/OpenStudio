/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ControllerWaterCoil.hpp"
#include "ControllerWaterCoil_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Controller_WaterCoil_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

namespace model {

  namespace detail {

    ControllerWaterCoil_Impl::ControllerWaterCoil_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ControllerWaterCoil::iddObjectType());
    }

    ControllerWaterCoil_Impl::ControllerWaterCoil_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ControllerWaterCoil::iddObjectType());
    }

    ControllerWaterCoil_Impl::ControllerWaterCoil_Impl(const ControllerWaterCoil_Impl& other, Model_Impl* model, bool keepHandle)
      : HVACComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ControllerWaterCoil_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      // Not appropriate: no specific output
      return result;
    }

    IddObjectType ControllerWaterCoil_Impl::iddObjectType() const {
      return ControllerWaterCoil::iddObjectType();
    }

    std::string ControllerWaterCoil_Impl::controlVariable() const {
      boost::optional<std::string> result = getString(OS_Controller_WaterCoilFields::ControlVariable, true);
      OS_ASSERT(result);
      return result.get();
    }

    bool ControllerWaterCoil_Impl::isControlVariableDefaulted() const {
      return isEmpty(OS_Controller_WaterCoilFields::ControlVariable);
    }

    boost::optional<std::string> ControllerWaterCoil_Impl::action() const {
      return getString(OS_Controller_WaterCoilFields::Action, true);
    }

    std::string ControllerWaterCoil_Impl::actuatorVariable() const {
      boost::optional<std::string> result = getString(OS_Controller_WaterCoilFields::ActuatorVariable, true);
      OS_ASSERT(result);
      return result.get();
    }

    bool ControllerWaterCoil_Impl::isActuatorVariableDefaulted() const {
      return isEmpty(OS_Controller_WaterCoilFields::ActuatorVariable);
    }

    boost::optional<double> ControllerWaterCoil_Impl::controllerConvergenceTolerance() const {
      return getDouble(OS_Controller_WaterCoilFields::ControllerConvergenceTolerance, true);
    }

    bool ControllerWaterCoil_Impl::isControllerConvergenceToleranceDefaulted() const {
      return isEmpty(OS_Controller_WaterCoilFields::ControllerConvergenceTolerance);
    }

    bool ControllerWaterCoil_Impl::isControllerConvergenceToleranceAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Controller_WaterCoilFields::ControllerConvergenceTolerance, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> ControllerWaterCoil_Impl::maximumActuatedFlow() const {
      return getDouble(OS_Controller_WaterCoilFields::MaximumActuatedFlow, true);
    }

    bool ControllerWaterCoil_Impl::isMaximumActuatedFlowAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Controller_WaterCoilFields::MaximumActuatedFlow, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    double ControllerWaterCoil_Impl::minimumActuatedFlow() const {
      boost::optional<double> value = getDouble(OS_Controller_WaterCoilFields::MinimumActuatedFlow, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ControllerWaterCoil_Impl::isMinimumActuatedFlowDefaulted() const {
      return isEmpty(OS_Controller_WaterCoilFields::MinimumActuatedFlow);
    }

    bool ControllerWaterCoil_Impl::setControlVariable(const std::string& controlVariable) {
      bool result = setString(OS_Controller_WaterCoilFields::ControlVariable, controlVariable);
      return result;
    }

    void ControllerWaterCoil_Impl::resetControlVariable() {
      bool result = setString(OS_Controller_WaterCoilFields::ControlVariable, "");
      OS_ASSERT(result);
    }

    bool ControllerWaterCoil_Impl::setAction(const std::string& action) {
      bool result = setString(OS_Controller_WaterCoilFields::Action, action);
      return result;
    }

    void ControllerWaterCoil_Impl::resetAction() {
      bool result = setString(OS_Controller_WaterCoilFields::Action, "");
      OS_ASSERT(result);
    }

    bool ControllerWaterCoil_Impl::setActuatorVariable(const std::string& actuatorVariable) {
      bool result = setString(OS_Controller_WaterCoilFields::ActuatorVariable, actuatorVariable);
      return result;
    }

    void ControllerWaterCoil_Impl::resetActuatorVariable() {
      bool result = setString(OS_Controller_WaterCoilFields::ActuatorVariable, "");
      OS_ASSERT(result);
    }

    bool ControllerWaterCoil_Impl::setControllerConvergenceTolerance(double controllerConvergenceTolerance) {
      bool result = setDouble(OS_Controller_WaterCoilFields::ControllerConvergenceTolerance, controllerConvergenceTolerance);
      OS_ASSERT(result);
      return result;
    }

    void ControllerWaterCoil_Impl::resetControllerConvergenceTolerance() {
      bool result = setString(OS_Controller_WaterCoilFields::ControllerConvergenceTolerance, "");
      OS_ASSERT(result);
    }

    void ControllerWaterCoil_Impl::autosizeControllerConvergenceTolerance() {
      bool result = setString(OS_Controller_WaterCoilFields::ControllerConvergenceTolerance, "Autosize");
      OS_ASSERT(result);
    }

    bool ControllerWaterCoil_Impl::setMaximumActuatedFlow(double maximumActuatedFlow) {
      bool result = setDouble(OS_Controller_WaterCoilFields::MaximumActuatedFlow, maximumActuatedFlow);
      return result;
    }

    void ControllerWaterCoil_Impl::resetMaximumActuatedFlow() {
      bool result = setString(OS_Controller_WaterCoilFields::MaximumActuatedFlow, "");
      OS_ASSERT(result);
    }

    void ControllerWaterCoil_Impl::autosizeMaximumActuatedFlow() {
      bool result = setString(OS_Controller_WaterCoilFields::MaximumActuatedFlow, "Autosize");
      OS_ASSERT(result);
    }

    bool ControllerWaterCoil_Impl::setMinimumActuatedFlow(double minimumActuatedFlow) {
      bool result = setDouble(OS_Controller_WaterCoilFields::MinimumActuatedFlow, minimumActuatedFlow);
      return result;
    }

    void ControllerWaterCoil_Impl::resetMinimumActuatedFlow() {
      bool result = setString(OS_Controller_WaterCoilFields::MinimumActuatedFlow, "");
      OS_ASSERT(result);
    }

    boost::optional<Node> ControllerWaterCoil_Impl::sensorNode() const {
      return this->getObject<ModelObject>().getModelObjectTarget<Node>(OS_Controller_WaterCoilFields::SensorNodeName);
    }

    boost::optional<Node> ControllerWaterCoil_Impl::actuatorNode() const {
      return this->getObject<ModelObject>().getModelObjectTarget<Node>(OS_Controller_WaterCoilFields::ActuatorNodeName);
    }

    bool ControllerWaterCoil_Impl::setSensorNode(const Node& node) {
      return this->setPointer(OS_Controller_WaterCoilFields::SensorNodeName, node.handle());
    }

    bool ControllerWaterCoil_Impl::setActuatorNode(const Node& node) {
      return this->setPointer(OS_Controller_WaterCoilFields::ActuatorNodeName, node.handle());
    }

    bool ControllerWaterCoil_Impl::setWaterCoil(const HVACComponent& comp) {
      auto result = setPointer(OS_Controller_WaterCoilFields::WaterCoilName, comp.handle());
      OS_ASSERT(result);
      return result;
    }

    boost::optional<HVACComponent> ControllerWaterCoil_Impl::waterCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_Controller_WaterCoilFields::WaterCoilName);
    }

    boost::optional<double> ControllerWaterCoil_Impl::autosizedControllerConvergenceTolerance() const {
      return getAutosizedValue("Controller Convergence Tolerance", "");
    }

    boost::optional<double> ControllerWaterCoil_Impl::autosizedMaximumActuatedFlow() const {
      return getAutosizedValue("Maximum Actuated Flow", "m3/s");
    }

    void ControllerWaterCoil_Impl::autosize() {
      autosizeControllerConvergenceTolerance();
      autosizeMaximumActuatedFlow();
    }

    void ControllerWaterCoil_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedControllerConvergenceTolerance();
      if (val) {
        setControllerConvergenceTolerance(val.get());
      }

      val = autosizedMaximumActuatedFlow();
      if (val) {
        setMaximumActuatedFlow(val.get());
      }
    }

    ComponentType ControllerWaterCoil_Impl::componentType() const {
      return ComponentType::None;
    }

    std::vector<FuelType> ControllerWaterCoil_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> ControllerWaterCoil_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> ControllerWaterCoil_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  ControllerWaterCoil::ControllerWaterCoil(const Model& model) : HVACComponent(ControllerWaterCoil::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ControllerWaterCoil_Impl>());
    resetMinimumActuatedFlow();
  }

  IddObjectType ControllerWaterCoil::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Controller_WaterCoil);
    return result;
  }

  std::vector<std::string> ControllerWaterCoil::validControlVariableValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Controller_WaterCoilFields::ControlVariable);
  }

  std::vector<std::string> ControllerWaterCoil::validActionValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Controller_WaterCoilFields::Action);
  }

  std::vector<std::string> ControllerWaterCoil::validActuatorVariableValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Controller_WaterCoilFields::ActuatorVariable);
  }

  boost::optional<std::string> ControllerWaterCoil::controlVariable() const {
    return getImpl<detail::ControllerWaterCoil_Impl>()->controlVariable();
  }

  boost::optional<HVACComponent> ControllerWaterCoil::waterCoil() const {
    return getImpl<detail::ControllerWaterCoil_Impl>()->waterCoil();
  }

  bool ControllerWaterCoil::isControlVariableDefaulted() const {
    return getImpl<detail::ControllerWaterCoil_Impl>()->isControlVariableDefaulted();
  }

  boost::optional<std::string> ControllerWaterCoil::action() const {
    return getImpl<detail::ControllerWaterCoil_Impl>()->action();
  }

  boost::optional<std::string> ControllerWaterCoil::actuatorVariable() const {
    return getImpl<detail::ControllerWaterCoil_Impl>()->actuatorVariable();
  }

  bool ControllerWaterCoil::isActuatorVariableDefaulted() const {
    return getImpl<detail::ControllerWaterCoil_Impl>()->isActuatorVariableDefaulted();
  }

  boost::optional<double> ControllerWaterCoil::controllerConvergenceTolerance() const {
    return getImpl<detail::ControllerWaterCoil_Impl>()->controllerConvergenceTolerance();
  }

  bool ControllerWaterCoil::isControllerConvergenceToleranceDefaulted() const {
    return getImpl<detail::ControllerWaterCoil_Impl>()->isControllerConvergenceToleranceDefaulted();
  }

  bool ControllerWaterCoil::isControllerConvergenceToleranceAutosized() const {
    return getImpl<detail::ControllerWaterCoil_Impl>()->isControllerConvergenceToleranceAutosized();
  }

  boost::optional<double> ControllerWaterCoil::maximumActuatedFlow() const {
    return getImpl<detail::ControllerWaterCoil_Impl>()->maximumActuatedFlow();
  }

  bool ControllerWaterCoil::isMaximumActuatedFlowAutosized() const {
    return getImpl<detail::ControllerWaterCoil_Impl>()->isMaximumActuatedFlowAutosized();
  }

  double ControllerWaterCoil::minimumActuatedFlow() const {
    return getImpl<detail::ControllerWaterCoil_Impl>()->minimumActuatedFlow();
  }

  bool ControllerWaterCoil::isMinimumActuatedFlowDefaulted() const {
    return getImpl<detail::ControllerWaterCoil_Impl>()->isMinimumActuatedFlowDefaulted();
  }

  bool ControllerWaterCoil::setControlVariable(const std::string& controlVariable) {
    return getImpl<detail::ControllerWaterCoil_Impl>()->setControlVariable(controlVariable);
  }

  void ControllerWaterCoil::resetControlVariable() {
    getImpl<detail::ControllerWaterCoil_Impl>()->resetControlVariable();
  }

  bool ControllerWaterCoil::setAction(const std::string& action) {
    return getImpl<detail::ControllerWaterCoil_Impl>()->setAction(action);
  }

  void ControllerWaterCoil::resetAction() {
    getImpl<detail::ControllerWaterCoil_Impl>()->resetAction();
  }

  bool ControllerWaterCoil::setActuatorVariable(const std::string& actuatorVariable) {
    return getImpl<detail::ControllerWaterCoil_Impl>()->setActuatorVariable(actuatorVariable);
  }

  void ControllerWaterCoil::resetActuatorVariable() {
    getImpl<detail::ControllerWaterCoil_Impl>()->resetActuatorVariable();
  }

  bool ControllerWaterCoil::setControllerConvergenceTolerance(double controllerConvergenceTolerance) {
    return getImpl<detail::ControllerWaterCoil_Impl>()->setControllerConvergenceTolerance(controllerConvergenceTolerance);
  }

  void ControllerWaterCoil::resetControllerConvergenceTolerance() {
    getImpl<detail::ControllerWaterCoil_Impl>()->resetControllerConvergenceTolerance();
  }

  void ControllerWaterCoil::autosizeControllerConvergenceTolerance() {
    getImpl<detail::ControllerWaterCoil_Impl>()->autosizeControllerConvergenceTolerance();
  }

  bool ControllerWaterCoil::setMaximumActuatedFlow(double maximumActuatedFlow) {
    return getImpl<detail::ControllerWaterCoil_Impl>()->setMaximumActuatedFlow(maximumActuatedFlow);
  }

  void ControllerWaterCoil::resetMaximumActuatedFlow() {
    getImpl<detail::ControllerWaterCoil_Impl>()->resetMaximumActuatedFlow();
  }

  void ControllerWaterCoil::autosizeMaximumActuatedFlow() {
    getImpl<detail::ControllerWaterCoil_Impl>()->autosizeMaximumActuatedFlow();
  }

  bool ControllerWaterCoil::setMinimumActuatedFlow(double minimumActuatedFlow) {
    return getImpl<detail::ControllerWaterCoil_Impl>()->setMinimumActuatedFlow(minimumActuatedFlow);
  }

  void ControllerWaterCoil::resetMinimumActuatedFlow() {
    getImpl<detail::ControllerWaterCoil_Impl>()->resetMinimumActuatedFlow();
  }

  boost::optional<Node> ControllerWaterCoil::sensorNode() const {
    return getImpl<detail::ControllerWaterCoil_Impl>()->sensorNode();
  }

  boost::optional<Node> ControllerWaterCoil::actuatorNode() const {
    return getImpl<detail::ControllerWaterCoil_Impl>()->actuatorNode();
  }

  bool ControllerWaterCoil::setSensorNode(const Node& node) {
    return getImpl<detail::ControllerWaterCoil_Impl>()->setSensorNode(node);
  }

  bool ControllerWaterCoil::setActuatorNode(const Node& node) {
    return getImpl<detail::ControllerWaterCoil_Impl>()->setActuatorNode(node);
  }

  /// @cond
  ControllerWaterCoil::ControllerWaterCoil(std::shared_ptr<detail::ControllerWaterCoil_Impl> impl) : HVACComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> ControllerWaterCoil::autosizedControllerConvergenceTolerance() const {
    return getImpl<detail::ControllerWaterCoil_Impl>()->autosizedControllerConvergenceTolerance();
  }

  boost::optional<double> ControllerWaterCoil::autosizedMaximumActuatedFlow() const {
    return getImpl<detail::ControllerWaterCoil_Impl>()->autosizedMaximumActuatedFlow();
  }

}  // namespace model

}  // namespace openstudio
