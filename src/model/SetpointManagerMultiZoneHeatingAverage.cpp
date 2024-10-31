/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SetpointManagerMultiZoneHeatingAverage.hpp"
#include "SetpointManagerMultiZoneHeatingAverage_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SetpointManager_MultiZone_Heating_Average_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SetpointManagerMultiZoneHeatingAverage_Impl::SetpointManagerMultiZoneHeatingAverage_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                             bool keepHandle)
      : SetpointManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SetpointManagerMultiZoneHeatingAverage::iddObjectType());
    }

    SetpointManagerMultiZoneHeatingAverage_Impl::SetpointManagerMultiZoneHeatingAverage_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SetpointManagerMultiZoneHeatingAverage::iddObjectType());
    }

    SetpointManagerMultiZoneHeatingAverage_Impl::SetpointManagerMultiZoneHeatingAverage_Impl(const SetpointManagerMultiZoneHeatingAverage_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SetpointManagerMultiZoneHeatingAverage_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SetpointManagerMultiZoneHeatingAverage_Impl::iddObjectType() const {
      return SetpointManagerMultiZoneHeatingAverage::iddObjectType();
    }

    std::string SetpointManagerMultiZoneHeatingAverage_Impl::controlVariable() const {
      boost::optional<std::string> value = getString(OS_SetpointManager_MultiZone_Heating_AverageFields::ControlVariable, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerMultiZoneHeatingAverage_Impl::minimumSetpointTemperature() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_Heating_AverageFields::MinimumSetpointTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerMultiZoneHeatingAverage_Impl::maximumSetpointTemperature() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_Heating_AverageFields::MaximumSetpointTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Node> SetpointManagerMultiZoneHeatingAverage_Impl::setpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_MultiZone_Heating_AverageFields::SetpointNodeorNodeListName);
    }

    bool SetpointManagerMultiZoneHeatingAverage_Impl::setControlVariable(const std::string& controlVariable) {
      bool result = setString(OS_SetpointManager_MultiZone_Heating_AverageFields::ControlVariable, controlVariable);
      return result;
    }

    bool SetpointManagerMultiZoneHeatingAverage_Impl::setMinimumSetpointTemperature(double minimumSetpointTemperature) {
      bool result = setDouble(OS_SetpointManager_MultiZone_Heating_AverageFields::MinimumSetpointTemperature, minimumSetpointTemperature);
      return result;
    }

    bool SetpointManagerMultiZoneHeatingAverage_Impl::setMaximumSetpointTemperature(double maximumSetpointTemperature) {
      bool result = setDouble(OS_SetpointManager_MultiZone_Heating_AverageFields::MaximumSetpointTemperature, maximumSetpointTemperature);
      return result;
    }

    bool SetpointManagerMultiZoneHeatingAverage_Impl::setSetpointNode(const Node& node) {
      bool result = setPointer(OS_SetpointManager_MultiZone_Heating_AverageFields::SetpointNodeorNodeListName, node.handle());
      return result;
    }

    void SetpointManagerMultiZoneHeatingAverage_Impl::resetSetpointNode() {
      bool result = setString(OS_SetpointManager_MultiZone_Heating_AverageFields::SetpointNodeorNodeListName, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  SetpointManagerMultiZoneHeatingAverage::SetpointManagerMultiZoneHeatingAverage(const Model& model)
    : SetpointManager(SetpointManagerMultiZoneHeatingAverage::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SetpointManagerMultiZoneHeatingAverage_Impl>());

    setControlVariable("Temperature");
    setMinimumSetpointTemperature(20.);
    setMaximumSetpointTemperature(50.);
  }

  IddObjectType SetpointManagerMultiZoneHeatingAverage::iddObjectType() {
    return {IddObjectType::OS_SetpointManager_MultiZone_Heating_Average};
  }

  std::vector<std::string> SetpointManagerMultiZoneHeatingAverage::controlVariableValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_SetpointManager_MultiZone_Heating_AverageFields::ControlVariable);
  }

  double SetpointManagerMultiZoneHeatingAverage::minimumSetpointTemperature() const {
    return getImpl<detail::SetpointManagerMultiZoneHeatingAverage_Impl>()->minimumSetpointTemperature();
  }

  double SetpointManagerMultiZoneHeatingAverage::maximumSetpointTemperature() const {
    return getImpl<detail::SetpointManagerMultiZoneHeatingAverage_Impl>()->maximumSetpointTemperature();
  }

  bool SetpointManagerMultiZoneHeatingAverage::setMinimumSetpointTemperature(double minimumSetpointTemperature) {
    return getImpl<detail::SetpointManagerMultiZoneHeatingAverage_Impl>()->setMinimumSetpointTemperature(minimumSetpointTemperature);
  }

  bool SetpointManagerMultiZoneHeatingAverage::setMaximumSetpointTemperature(double maximumSetpointTemperature) {
    return getImpl<detail::SetpointManagerMultiZoneHeatingAverage_Impl>()->setMaximumSetpointTemperature(maximumSetpointTemperature);
  }

  /// @cond
  SetpointManagerMultiZoneHeatingAverage::SetpointManagerMultiZoneHeatingAverage(
    std::shared_ptr<detail::SetpointManagerMultiZoneHeatingAverage_Impl> impl)
    : SetpointManager(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
