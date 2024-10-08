/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SetpointManagerMultiZoneHumidityMinimum.hpp"
#include "SetpointManagerMultiZoneHumidityMinimum_Impl.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SetpointManager_MultiZone_Humidity_Minimum_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SetpointManagerMultiZoneHumidityMinimum_Impl::SetpointManagerMultiZoneHumidityMinimum_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                               bool keepHandle)
      : SetpointManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SetpointManagerMultiZoneHumidityMinimum::iddObjectType());
    }

    SetpointManagerMultiZoneHumidityMinimum_Impl::SetpointManagerMultiZoneHumidityMinimum_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                               Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SetpointManagerMultiZoneHumidityMinimum::iddObjectType());
    }

    SetpointManagerMultiZoneHumidityMinimum_Impl::SetpointManagerMultiZoneHumidityMinimum_Impl(
      const SetpointManagerMultiZoneHumidityMinimum_Impl& other, Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SetpointManagerMultiZoneHumidityMinimum_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SetpointManagerMultiZoneHumidityMinimum_Impl::iddObjectType() const {
      return SetpointManagerMultiZoneHumidityMinimum::iddObjectType();
    }

    double SetpointManagerMultiZoneHumidityMinimum_Impl::minimumSetpointHumidityRatio() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MinimumSetpointHumidityRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SetpointManagerMultiZoneHumidityMinimum_Impl::isMinimumSetpointHumidityRatioDefaulted() const {
      return isEmpty(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MinimumSetpointHumidityRatio);
    }

    double SetpointManagerMultiZoneHumidityMinimum_Impl::maximumSetpointHumidityRatio() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MaximumSetpointHumidityRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SetpointManagerMultiZoneHumidityMinimum_Impl::isMaximumSetpointHumidityRatioDefaulted() const {
      return isEmpty(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MaximumSetpointHumidityRatio);
    }

    boost::optional<Node> SetpointManagerMultiZoneHumidityMinimum_Impl::setpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_MultiZone_Humidity_MinimumFields::SetpointNodeorNodeListName);
    }

    std::string SetpointManagerMultiZoneHumidityMinimum_Impl::controlVariable() const {
      boost::optional<std::string> value = getString(OS_SetpointManager_MultiZone_Humidity_MinimumFields::ControlVariable, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SetpointManagerMultiZoneHumidityMinimum_Impl::setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio) {
      bool result = setDouble(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MinimumSetpointHumidityRatio, minimumSetpointHumidityRatio);
      return result;
    }

    void SetpointManagerMultiZoneHumidityMinimum_Impl::resetMinimumSetpointHumidityRatio() {
      bool result = setString(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MinimumSetpointHumidityRatio, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerMultiZoneHumidityMinimum_Impl::setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio) {
      bool result = setDouble(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MaximumSetpointHumidityRatio, maximumSetpointHumidityRatio);
      return result;
    }

    void SetpointManagerMultiZoneHumidityMinimum_Impl::resetMaximumSetpointHumidityRatio() {
      bool result = setString(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MaximumSetpointHumidityRatio, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerMultiZoneHumidityMinimum_Impl::setSetpointNode(const Node& node) {
      bool result = setPointer(OS_SetpointManager_MultiZone_Humidity_MinimumFields::SetpointNodeorNodeListName, node.handle());
      return result;
    }

    void SetpointManagerMultiZoneHumidityMinimum_Impl::resetSetpointNode() {
      bool result = setString(OS_SetpointManager_MultiZone_Humidity_MinimumFields::SetpointNodeorNodeListName, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerMultiZoneHumidityMinimum_Impl::setControlVariable(const std::string& controlVariable) {
      bool result = setString(OS_SetpointManager_MultiZone_Humidity_MinimumFields::ControlVariable, controlVariable);
      return result;
    }

  }  // namespace detail

  SetpointManagerMultiZoneHumidityMinimum::SetpointManagerMultiZoneHumidityMinimum(const Model& model)
    : SetpointManager(SetpointManagerMultiZoneHumidityMinimum::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>());

    bool ok = setControlVariable("MinimumHumidityRatio");
    OS_ASSERT(ok);
  }

  IddObjectType SetpointManagerMultiZoneHumidityMinimum::iddObjectType() {
    return {IddObjectType::OS_SetpointManager_MultiZone_Humidity_Minimum};
  }

  std::vector<std::string> SetpointManagerMultiZoneHumidityMinimum::controlVariableValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_SetpointManager_MultiZone_Humidity_MinimumFields::ControlVariable);
  }

  double SetpointManagerMultiZoneHumidityMinimum::minimumSetpointHumidityRatio() const {
    return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->minimumSetpointHumidityRatio();
  }

  bool SetpointManagerMultiZoneHumidityMinimum::isMinimumSetpointHumidityRatioDefaulted() const {
    return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->isMinimumSetpointHumidityRatioDefaulted();
  }

  double SetpointManagerMultiZoneHumidityMinimum::maximumSetpointHumidityRatio() const {
    return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->maximumSetpointHumidityRatio();
  }

  bool SetpointManagerMultiZoneHumidityMinimum::isMaximumSetpointHumidityRatioDefaulted() const {
    return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->isMaximumSetpointHumidityRatioDefaulted();
  }

  bool SetpointManagerMultiZoneHumidityMinimum::setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio) {
    return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->setMinimumSetpointHumidityRatio(minimumSetpointHumidityRatio);
  }

  void SetpointManagerMultiZoneHumidityMinimum::resetMinimumSetpointHumidityRatio() {
    getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->resetMinimumSetpointHumidityRatio();
  }

  bool SetpointManagerMultiZoneHumidityMinimum::setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio) {
    return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->setMaximumSetpointHumidityRatio(maximumSetpointHumidityRatio);
  }

  void SetpointManagerMultiZoneHumidityMinimum::resetMaximumSetpointHumidityRatio() {
    getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->resetMaximumSetpointHumidityRatio();
  }

  /// @cond
  SetpointManagerMultiZoneHumidityMinimum::SetpointManagerMultiZoneHumidityMinimum(
    std::shared_ptr<detail::SetpointManagerMultiZoneHumidityMinimum_Impl> impl)
    : SetpointManager(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
