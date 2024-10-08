/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SetpointManagerOutdoorAirPretreat.hpp"
#include "SetpointManagerOutdoorAirPretreat_Impl.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SetpointManager_OutdoorAirPretreat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SetpointManagerOutdoorAirPretreat_Impl::SetpointManagerOutdoorAirPretreat_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SetpointManagerOutdoorAirPretreat::iddObjectType());
    }

    SetpointManagerOutdoorAirPretreat_Impl::SetpointManagerOutdoorAirPretreat_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                   Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SetpointManagerOutdoorAirPretreat::iddObjectType());
    }

    SetpointManagerOutdoorAirPretreat_Impl::SetpointManagerOutdoorAirPretreat_Impl(const SetpointManagerOutdoorAirPretreat_Impl& other,
                                                                                   Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SetpointManagerOutdoorAirPretreat_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SetpointManagerOutdoorAirPretreat_Impl::iddObjectType() const {
      return SetpointManagerOutdoorAirPretreat::iddObjectType();
    }

    std::string SetpointManagerOutdoorAirPretreat_Impl::controlVariable() const {
      boost::optional<std::string> value = getString(OS_SetpointManager_OutdoorAirPretreatFields::ControlVariable, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerOutdoorAirPretreat_Impl::minimumSetpointTemperature() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_OutdoorAirPretreatFields::MinimumSetpointTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SetpointManagerOutdoorAirPretreat_Impl::isMinimumSetpointTemperatureDefaulted() const {
      return isEmpty(OS_SetpointManager_OutdoorAirPretreatFields::MinimumSetpointTemperature);
    }

    double SetpointManagerOutdoorAirPretreat_Impl::maximumSetpointTemperature() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_OutdoorAirPretreatFields::MaximumSetpointTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SetpointManagerOutdoorAirPretreat_Impl::isMaximumSetpointTemperatureDefaulted() const {
      return isEmpty(OS_SetpointManager_OutdoorAirPretreatFields::MaximumSetpointTemperature);
    }

    double SetpointManagerOutdoorAirPretreat_Impl::minimumSetpointHumidityRatio() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_OutdoorAirPretreatFields::MinimumSetpointHumidityRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SetpointManagerOutdoorAirPretreat_Impl::isMinimumSetpointHumidityRatioDefaulted() const {
      return isEmpty(OS_SetpointManager_OutdoorAirPretreatFields::MinimumSetpointHumidityRatio);
    }

    double SetpointManagerOutdoorAirPretreat_Impl::maximumSetpointHumidityRatio() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_OutdoorAirPretreatFields::MaximumSetpointHumidityRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SetpointManagerOutdoorAirPretreat_Impl::isMaximumSetpointHumidityRatioDefaulted() const {
      return isEmpty(OS_SetpointManager_OutdoorAirPretreatFields::MaximumSetpointHumidityRatio);
    }

    boost::optional<Node> SetpointManagerOutdoorAirPretreat_Impl::referenceSetpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_OutdoorAirPretreatFields::ReferenceSetpointNodeName);
    }

    boost::optional<Node> SetpointManagerOutdoorAirPretreat_Impl::mixedAirStreamNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_OutdoorAirPretreatFields::MixedAirStreamNodeName);
    }

    boost::optional<Node> SetpointManagerOutdoorAirPretreat_Impl::outdoorAirStreamNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_OutdoorAirPretreatFields::OutdoorAirStreamNodeName);
    }

    boost::optional<Node> SetpointManagerOutdoorAirPretreat_Impl::returnAirStreamNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_OutdoorAirPretreatFields::ReturnAirStreamNodeName);
    }

    boost::optional<Node> SetpointManagerOutdoorAirPretreat_Impl::setpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_OutdoorAirPretreatFields::SetpointNodeorNodeListName);
    }

    bool SetpointManagerOutdoorAirPretreat_Impl::setControlVariable(const std::string& controlVariable) {
      bool result = setString(OS_SetpointManager_OutdoorAirPretreatFields::ControlVariable, controlVariable);
      return result;
    }

    void SetpointManagerOutdoorAirPretreat_Impl::resetControlVariable() {
      bool result = setString(OS_SetpointManager_OutdoorAirPretreatFields::ControlVariable, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerOutdoorAirPretreat_Impl::setMinimumSetpointTemperature(double minimumSetpointTemperature) {
      bool result = setDouble(OS_SetpointManager_OutdoorAirPretreatFields::MinimumSetpointTemperature, minimumSetpointTemperature);
      return result;
    }

    void SetpointManagerOutdoorAirPretreat_Impl::resetMinimumSetpointTemperature() {
      bool result = setString(OS_SetpointManager_OutdoorAirPretreatFields::MinimumSetpointTemperature, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerOutdoorAirPretreat_Impl::setMaximumSetpointTemperature(double maximumSetpointTemperature) {
      bool result = setDouble(OS_SetpointManager_OutdoorAirPretreatFields::MaximumSetpointTemperature, maximumSetpointTemperature);
      return result;
    }

    void SetpointManagerOutdoorAirPretreat_Impl::resetMaximumSetpointTemperature() {
      bool result = setString(OS_SetpointManager_OutdoorAirPretreatFields::MaximumSetpointTemperature, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerOutdoorAirPretreat_Impl::setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio) {
      bool result = setDouble(OS_SetpointManager_OutdoorAirPretreatFields::MinimumSetpointHumidityRatio, minimumSetpointHumidityRatio);
      return result;
    }

    void SetpointManagerOutdoorAirPretreat_Impl::resetMinimumSetpointHumidityRatio() {
      bool result = setString(OS_SetpointManager_OutdoorAirPretreatFields::MinimumSetpointHumidityRatio, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerOutdoorAirPretreat_Impl::setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio) {
      bool result = setDouble(OS_SetpointManager_OutdoorAirPretreatFields::MaximumSetpointHumidityRatio, maximumSetpointHumidityRatio);
      return result;
    }

    void SetpointManagerOutdoorAirPretreat_Impl::resetMaximumSetpointHumidityRatio() {
      bool result = setString(OS_SetpointManager_OutdoorAirPretreatFields::MaximumSetpointHumidityRatio, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerOutdoorAirPretreat_Impl::setReferenceSetpointNode(const Node& node) {
      bool result = setPointer(OS_SetpointManager_OutdoorAirPretreatFields::ReferenceSetpointNodeName, node.handle());
      return result;
    }

    void SetpointManagerOutdoorAirPretreat_Impl::resetReferenceSetpointNode() {
      bool result = setString(OS_SetpointManager_OutdoorAirPretreatFields::ReferenceSetpointNodeName, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerOutdoorAirPretreat_Impl::setMixedAirStreamNode(const Node& node) {
      bool result = setPointer(OS_SetpointManager_OutdoorAirPretreatFields::MixedAirStreamNodeName, node.handle());
      return result;
    }

    void SetpointManagerOutdoorAirPretreat_Impl::resetMixedAirStreamNode() {
      bool result = setString(OS_SetpointManager_OutdoorAirPretreatFields::MixedAirStreamNodeName, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerOutdoorAirPretreat_Impl::setOutdoorAirStreamNode(const Node& node) {
      bool result = setPointer(OS_SetpointManager_OutdoorAirPretreatFields::OutdoorAirStreamNodeName, node.handle());
      return result;
    }

    void SetpointManagerOutdoorAirPretreat_Impl::resetOutdoorAirStreamNode() {
      bool result = setString(OS_SetpointManager_OutdoorAirPretreatFields::OutdoorAirStreamNodeName, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerOutdoorAirPretreat_Impl::setReturnAirStreamNode(const Node& node) {
      bool result = setPointer(OS_SetpointManager_OutdoorAirPretreatFields::ReturnAirStreamNodeName, node.handle());
      return result;
    }

    void SetpointManagerOutdoorAirPretreat_Impl::resetReturnAirStreamNode() {
      bool result = setString(OS_SetpointManager_OutdoorAirPretreatFields::ReturnAirStreamNodeName, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerOutdoorAirPretreat_Impl::setSetpointNode(const Node& node) {
      bool result = setPointer(OS_SetpointManager_OutdoorAirPretreatFields::SetpointNodeorNodeListName, node.handle());
      return result;
    }

    void SetpointManagerOutdoorAirPretreat_Impl::resetSetpointNode() {
      bool result = setString(OS_SetpointManager_OutdoorAirPretreatFields::SetpointNodeorNodeListName, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  SetpointManagerOutdoorAirPretreat::SetpointManagerOutdoorAirPretreat(const Model& model)
    : SetpointManager(SetpointManagerOutdoorAirPretreat::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>());
  }

  IddObjectType SetpointManagerOutdoorAirPretreat::iddObjectType() {
    return {IddObjectType::OS_SetpointManager_OutdoorAirPretreat};
  }

  std::vector<std::string> SetpointManagerOutdoorAirPretreat::controlVariableValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_SetpointManager_OutdoorAirPretreatFields::ControlVariable);
  }

  double SetpointManagerOutdoorAirPretreat::minimumSetpointTemperature() const {
    return getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->minimumSetpointTemperature();
  }

  bool SetpointManagerOutdoorAirPretreat::isMinimumSetpointTemperatureDefaulted() const {
    return getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->isMinimumSetpointTemperatureDefaulted();
  }

  double SetpointManagerOutdoorAirPretreat::maximumSetpointTemperature() const {
    return getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->maximumSetpointTemperature();
  }

  bool SetpointManagerOutdoorAirPretreat::isMaximumSetpointTemperatureDefaulted() const {
    return getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->isMaximumSetpointTemperatureDefaulted();
  }

  double SetpointManagerOutdoorAirPretreat::minimumSetpointHumidityRatio() const {
    return getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->minimumSetpointHumidityRatio();
  }

  bool SetpointManagerOutdoorAirPretreat::isMinimumSetpointHumidityRatioDefaulted() const {
    return getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->isMinimumSetpointHumidityRatioDefaulted();
  }

  double SetpointManagerOutdoorAirPretreat::maximumSetpointHumidityRatio() const {
    return getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->maximumSetpointHumidityRatio();
  }

  bool SetpointManagerOutdoorAirPretreat::isMaximumSetpointHumidityRatioDefaulted() const {
    return getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->isMaximumSetpointHumidityRatioDefaulted();
  }

  boost::optional<Node> SetpointManagerOutdoorAirPretreat::referenceSetpointNode() const {
    return getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->referenceSetpointNode();
  }

  boost::optional<Node> SetpointManagerOutdoorAirPretreat::mixedAirStreamNode() const {
    return getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->mixedAirStreamNode();
  }

  boost::optional<Node> SetpointManagerOutdoorAirPretreat::outdoorAirStreamNode() const {
    return getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->outdoorAirStreamNode();
  }

  boost::optional<Node> SetpointManagerOutdoorAirPretreat::returnAirStreamNode() const {
    return getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->returnAirStreamNode();
  }

  void SetpointManagerOutdoorAirPretreat::resetControlVariable() {
    getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->resetControlVariable();
  }

  bool SetpointManagerOutdoorAirPretreat::setMinimumSetpointTemperature(double minimumSetpointTemperature) {
    return getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->setMinimumSetpointTemperature(minimumSetpointTemperature);
  }

  void SetpointManagerOutdoorAirPretreat::resetMinimumSetpointTemperature() {
    getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->resetMinimumSetpointTemperature();
  }

  bool SetpointManagerOutdoorAirPretreat::setMaximumSetpointTemperature(double maximumSetpointTemperature) {
    return getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->setMaximumSetpointTemperature(maximumSetpointTemperature);
  }

  void SetpointManagerOutdoorAirPretreat::resetMaximumSetpointTemperature() {
    getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->resetMaximumSetpointTemperature();
  }

  bool SetpointManagerOutdoorAirPretreat::setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio) {
    return getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->setMinimumSetpointHumidityRatio(minimumSetpointHumidityRatio);
  }

  void SetpointManagerOutdoorAirPretreat::resetMinimumSetpointHumidityRatio() {
    getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->resetMinimumSetpointHumidityRatio();
  }

  bool SetpointManagerOutdoorAirPretreat::setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio) {
    return getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->setMaximumSetpointHumidityRatio(maximumSetpointHumidityRatio);
  }

  void SetpointManagerOutdoorAirPretreat::resetMaximumSetpointHumidityRatio() {
    getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->resetMaximumSetpointHumidityRatio();
  }

  bool SetpointManagerOutdoorAirPretreat::setReferenceSetpointNode(const Node& node) {
    return getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->setReferenceSetpointNode(node);
  }

  void SetpointManagerOutdoorAirPretreat::resetReferenceSetpointNode() {
    getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->resetReferenceSetpointNode();
  }

  bool SetpointManagerOutdoorAirPretreat::setMixedAirStreamNode(const Node& node) {
    return getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->setMixedAirStreamNode(node);
  }

  void SetpointManagerOutdoorAirPretreat::resetMixedAirStreamNode() {
    getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->resetMixedAirStreamNode();
  }

  bool SetpointManagerOutdoorAirPretreat::setOutdoorAirStreamNode(const Node& node) {
    return getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->setOutdoorAirStreamNode(node);
  }

  void SetpointManagerOutdoorAirPretreat::resetOutdoorAirStreamNode() {
    getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->resetOutdoorAirStreamNode();
  }

  bool SetpointManagerOutdoorAirPretreat::setReturnAirStreamNode(const Node& node) {
    return getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->setReturnAirStreamNode(node);
  }

  void SetpointManagerOutdoorAirPretreat::resetReturnAirStreamNode() {
    getImpl<detail::SetpointManagerOutdoorAirPretreat_Impl>()->resetReturnAirStreamNode();
  }

  /// @cond
  SetpointManagerOutdoorAirPretreat::SetpointManagerOutdoorAirPretreat(std::shared_ptr<detail::SetpointManagerOutdoorAirPretreat_Impl> impl)
    : SetpointManager(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
