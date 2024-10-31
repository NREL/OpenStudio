/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SetpointManagerColdest.hpp"
#include "SetpointManagerColdest_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SetpointManager_Coldest_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SetpointManagerColdest_Impl::SetpointManagerColdest_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SetpointManagerColdest::iddObjectType());
    }

    SetpointManagerColdest_Impl::SetpointManagerColdest_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                             bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SetpointManagerColdest::iddObjectType());
    }

    SetpointManagerColdest_Impl::SetpointManagerColdest_Impl(const SetpointManagerColdest_Impl& other, Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SetpointManagerColdest_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SetpointManagerColdest_Impl::iddObjectType() const {
      return SetpointManagerColdest::iddObjectType();
    }

    std::string SetpointManagerColdest_Impl::controlVariable() const {
      boost::optional<std::string> value = getString(OS_SetpointManager_ColdestFields::ControlVariable, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerColdest_Impl::minimumSetpointTemperature() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_ColdestFields::MinimumSetpointTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerColdest_Impl::maximumSetpointTemperature() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_ColdestFields::MaximumSetpointTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string SetpointManagerColdest_Impl::strategy() const {
      boost::optional<std::string> value = getString(OS_SetpointManager_ColdestFields::Strategy, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Node> SetpointManagerColdest_Impl::setpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_ColdestFields::SetpointNodeorNodeListName);
    }

    bool SetpointManagerColdest_Impl::setControlVariable(const std::string& controlVariable) {
      bool result = setString(OS_SetpointManager_ColdestFields::ControlVariable, controlVariable);
      return result;
    }

    bool SetpointManagerColdest_Impl::setMinimumSetpointTemperature(double minimumSetpointTemperature) {
      bool result = setDouble(OS_SetpointManager_ColdestFields::MinimumSetpointTemperature, minimumSetpointTemperature);
      return result;
    }

    bool SetpointManagerColdest_Impl::setMaximumSetpointTemperature(double maximumSetpointTemperature) {
      bool result = setDouble(OS_SetpointManager_ColdestFields::MaximumSetpointTemperature, maximumSetpointTemperature);
      return result;
    }

    bool SetpointManagerColdest_Impl::setStrategy(const std::string& strategy) {
      bool result = setString(OS_SetpointManager_ColdestFields::Strategy, strategy);
      return result;
    }

    bool SetpointManagerColdest_Impl::setSetpointNode(const Node& node) {
      bool result = setPointer(OS_SetpointManager_ColdestFields::SetpointNodeorNodeListName, node.handle());
      return result;
    }

    void SetpointManagerColdest_Impl::resetSetpointNode() {
      bool result = setString(OS_SetpointManager_ColdestFields::SetpointNodeorNodeListName, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  SetpointManagerColdest::SetpointManagerColdest(const Model& model) : SetpointManager(SetpointManagerColdest::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SetpointManagerColdest_Impl>());

    setControlVariable("Temperature");
    setMinimumSetpointTemperature(20.);
    setMaximumSetpointTemperature(50.);
    setStrategy("MinimumTemperature");
  }

  IddObjectType SetpointManagerColdest::iddObjectType() {
    return {IddObjectType::OS_SetpointManager_Coldest};
  }

  std::vector<std::string> SetpointManagerColdest::controlVariableValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_SetpointManager_ColdestFields::ControlVariable);
  }

  std::vector<std::string> SetpointManagerColdest::strategyValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_SetpointManager_ColdestFields::Strategy);
  }

  double SetpointManagerColdest::minimumSetpointTemperature() const {
    return getImpl<detail::SetpointManagerColdest_Impl>()->minimumSetpointTemperature();
  }

  double SetpointManagerColdest::maximumSetpointTemperature() const {
    return getImpl<detail::SetpointManagerColdest_Impl>()->maximumSetpointTemperature();
  }

  std::string SetpointManagerColdest::strategy() const {
    return getImpl<detail::SetpointManagerColdest_Impl>()->strategy();
  }

  bool SetpointManagerColdest::setMinimumSetpointTemperature(double minimumSetpointTemperature) {
    return getImpl<detail::SetpointManagerColdest_Impl>()->setMinimumSetpointTemperature(minimumSetpointTemperature);
  }

  bool SetpointManagerColdest::setMaximumSetpointTemperature(double maximumSetpointTemperature) {
    return getImpl<detail::SetpointManagerColdest_Impl>()->setMaximumSetpointTemperature(maximumSetpointTemperature);
  }

  bool SetpointManagerColdest::setStrategy(const std::string& strategy) {
    return getImpl<detail::SetpointManagerColdest_Impl>()->setStrategy(strategy);
  }

  /// @cond
  SetpointManagerColdest::SetpointManagerColdest(std::shared_ptr<detail::SetpointManagerColdest_Impl> impl) : SetpointManager(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
