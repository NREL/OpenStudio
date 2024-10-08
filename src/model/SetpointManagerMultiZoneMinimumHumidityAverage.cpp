/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SetpointManagerMultiZoneMinimumHumidityAverage.hpp"
#include "SetpointManagerMultiZoneMinimumHumidityAverage_Impl.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SetpointManager_MultiZone_MinimumHumidity_Average_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SetpointManagerMultiZoneMinimumHumidityAverage_Impl::SetpointManagerMultiZoneMinimumHumidityAverage_Impl(const IdfObject& idfObject,
                                                                                                             Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SetpointManagerMultiZoneMinimumHumidityAverage::iddObjectType());
    }

    SetpointManagerMultiZoneMinimumHumidityAverage_Impl::SetpointManagerMultiZoneMinimumHumidityAverage_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SetpointManagerMultiZoneMinimumHumidityAverage::iddObjectType());
    }

    SetpointManagerMultiZoneMinimumHumidityAverage_Impl::SetpointManagerMultiZoneMinimumHumidityAverage_Impl(
      const SetpointManagerMultiZoneMinimumHumidityAverage_Impl& other, Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SetpointManagerMultiZoneMinimumHumidityAverage_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SetpointManagerMultiZoneMinimumHumidityAverage_Impl::iddObjectType() const {
      return SetpointManagerMultiZoneMinimumHumidityAverage::iddObjectType();
    }

    double SetpointManagerMultiZoneMinimumHumidityAverage_Impl::minimumSetpointHumidityRatio() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MinimumSetpointHumidityRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SetpointManagerMultiZoneMinimumHumidityAverage_Impl::isMinimumSetpointHumidityRatioDefaulted() const {
      return isEmpty(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MinimumSetpointHumidityRatio);
    }

    double SetpointManagerMultiZoneMinimumHumidityAverage_Impl::maximumSetpointHumidityRatio() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MaximumSetpointHumidityRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SetpointManagerMultiZoneMinimumHumidityAverage_Impl::isMaximumSetpointHumidityRatioDefaulted() const {
      return isEmpty(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MaximumSetpointHumidityRatio);
    }

    boost::optional<Node> SetpointManagerMultiZoneMinimumHumidityAverage_Impl::setpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(
        OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::SetpointNodeorNodeListName);
    }

    std::string SetpointManagerMultiZoneMinimumHumidityAverage_Impl::controlVariable() const {
      boost::optional<std::string> value = getString(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::ControlVariable, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SetpointManagerMultiZoneMinimumHumidityAverage_Impl::setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio) {
      bool result = setDouble(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MinimumSetpointHumidityRatio, minimumSetpointHumidityRatio);
      return result;
    }

    void SetpointManagerMultiZoneMinimumHumidityAverage_Impl::resetMinimumSetpointHumidityRatio() {
      bool result = setString(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MinimumSetpointHumidityRatio, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerMultiZoneMinimumHumidityAverage_Impl::setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio) {
      bool result = setDouble(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MaximumSetpointHumidityRatio, maximumSetpointHumidityRatio);
      return result;
    }

    void SetpointManagerMultiZoneMinimumHumidityAverage_Impl::resetMaximumSetpointHumidityRatio() {
      bool result = setString(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MaximumSetpointHumidityRatio, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerMultiZoneMinimumHumidityAverage_Impl::setSetpointNode(const Node& node) {
      bool result = setPointer(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::SetpointNodeorNodeListName, node.handle());
      return result;
    }

    void SetpointManagerMultiZoneMinimumHumidityAverage_Impl::resetSetpointNode() {
      bool result = setString(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::SetpointNodeorNodeListName, "");
      OS_ASSERT(result);
    }

    bool SetpointManagerMultiZoneMinimumHumidityAverage_Impl::setControlVariable(const std::string& controlVariable) {
      bool result = setString(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::ControlVariable, controlVariable);
      return result;
    }

  }  // namespace detail

  SetpointManagerMultiZoneMinimumHumidityAverage::SetpointManagerMultiZoneMinimumHumidityAverage(const Model& model)
    : SetpointManager(SetpointManagerMultiZoneMinimumHumidityAverage::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>());

    bool ok = setControlVariable("MinimumHumidityRatio");
    OS_ASSERT(ok);
  }

  IddObjectType SetpointManagerMultiZoneMinimumHumidityAverage::iddObjectType() {
    return {IddObjectType::OS_SetpointManager_MultiZone_MinimumHumidity_Average};
  }

  std::vector<std::string> SetpointManagerMultiZoneMinimumHumidityAverage::controlVariableValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::ControlVariable);
  }

  double SetpointManagerMultiZoneMinimumHumidityAverage::minimumSetpointHumidityRatio() const {
    return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->minimumSetpointHumidityRatio();
  }

  bool SetpointManagerMultiZoneMinimumHumidityAverage::isMinimumSetpointHumidityRatioDefaulted() const {
    return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->isMinimumSetpointHumidityRatioDefaulted();
  }

  double SetpointManagerMultiZoneMinimumHumidityAverage::maximumSetpointHumidityRatio() const {
    return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->maximumSetpointHumidityRatio();
  }

  bool SetpointManagerMultiZoneMinimumHumidityAverage::isMaximumSetpointHumidityRatioDefaulted() const {
    return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->isMaximumSetpointHumidityRatioDefaulted();
  }

  bool SetpointManagerMultiZoneMinimumHumidityAverage::setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio) {
    return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->setMinimumSetpointHumidityRatio(minimumSetpointHumidityRatio);
  }

  void SetpointManagerMultiZoneMinimumHumidityAverage::resetMinimumSetpointHumidityRatio() {
    getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->resetMinimumSetpointHumidityRatio();
  }

  bool SetpointManagerMultiZoneMinimumHumidityAverage::setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio) {
    return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->setMaximumSetpointHumidityRatio(maximumSetpointHumidityRatio);
  }

  void SetpointManagerMultiZoneMinimumHumidityAverage::resetMaximumSetpointHumidityRatio() {
    getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->resetMaximumSetpointHumidityRatio();
  }

  /// @cond
  SetpointManagerMultiZoneMinimumHumidityAverage::SetpointManagerMultiZoneMinimumHumidityAverage(
    std::shared_ptr<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl> impl)
    : SetpointManager(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
