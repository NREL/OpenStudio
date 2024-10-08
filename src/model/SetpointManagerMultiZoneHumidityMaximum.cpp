/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SetpointManagerMultiZoneHumidityMaximum.hpp"
#include "SetpointManagerMultiZoneHumidityMaximum_Impl.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SetpointManager_MultiZone_Humidity_Maximum_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SetpointManagerMultiZoneHumidityMaximum_Impl::SetpointManagerMultiZoneHumidityMaximum_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                               bool keepHandle)
      : SetpointManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SetpointManagerMultiZoneHumidityMaximum::iddObjectType());
    }

    SetpointManagerMultiZoneHumidityMaximum_Impl::SetpointManagerMultiZoneHumidityMaximum_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                               Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SetpointManagerMultiZoneHumidityMaximum::iddObjectType());
    }

    SetpointManagerMultiZoneHumidityMaximum_Impl::SetpointManagerMultiZoneHumidityMaximum_Impl(
      const SetpointManagerMultiZoneHumidityMaximum_Impl& other, Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SetpointManagerMultiZoneHumidityMaximum_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SetpointManagerMultiZoneHumidityMaximum_Impl::iddObjectType() const {
      return SetpointManagerMultiZoneHumidityMaximum::iddObjectType();
    }

    std::string SetpointManagerMultiZoneHumidityMaximum_Impl::controlVariable() const {
      boost::optional<std::string> value = getString(OS_SetpointManager_MultiZone_Humidity_MaximumFields::ControlVariable, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerMultiZoneHumidityMaximum_Impl::minimumSetpointHumidityRatio() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_Humidity_MaximumFields::MinimumSetpointHumidityRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerMultiZoneHumidityMaximum_Impl::maximumSetpointHumidityRatio() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_Humidity_MaximumFields::MaximumSetpointHumidityRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Node> SetpointManagerMultiZoneHumidityMaximum_Impl::setpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_MultiZone_Humidity_MaximumFields::SetpointNodeorNodeListName);
    }

    bool SetpointManagerMultiZoneHumidityMaximum_Impl::setControlVariable(const std::string& controlVariable) {
      bool result = setString(OS_SetpointManager_MultiZone_Humidity_MaximumFields::ControlVariable, controlVariable);
      return result;
    }

    bool SetpointManagerMultiZoneHumidityMaximum_Impl::setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio) {
      bool result = setDouble(OS_SetpointManager_MultiZone_Humidity_MaximumFields::MinimumSetpointHumidityRatio, minimumSetpointHumidityRatio);
      return result;
    }

    bool SetpointManagerMultiZoneHumidityMaximum_Impl::setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio) {
      bool result = setDouble(OS_SetpointManager_MultiZone_Humidity_MaximumFields::MaximumSetpointHumidityRatio, maximumSetpointHumidityRatio);
      return result;
    }

    bool SetpointManagerMultiZoneHumidityMaximum_Impl::setSetpointNode(const Node& node) {
      bool result = setPointer(OS_SetpointManager_MultiZone_Humidity_MaximumFields::SetpointNodeorNodeListName, node.handle());
      return result;
    }

    void SetpointManagerMultiZoneHumidityMaximum_Impl::resetSetpointNode() {
      bool result = setString(OS_SetpointManager_MultiZone_Humidity_MaximumFields::SetpointNodeorNodeListName, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  SetpointManagerMultiZoneHumidityMaximum::SetpointManagerMultiZoneHumidityMaximum(const Model& model)
    : SetpointManager(SetpointManagerMultiZoneHumidityMaximum::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>());

    setControlVariable("MaximumHumidityRatio");
    setMinimumSetpointHumidityRatio(0.008);
    setMaximumSetpointHumidityRatio(0.015);
  }

  IddObjectType SetpointManagerMultiZoneHumidityMaximum::iddObjectType() {
    return {IddObjectType::OS_SetpointManager_MultiZone_Humidity_Maximum};
  }

  std::vector<std::string> SetpointManagerMultiZoneHumidityMaximum::controlVariableValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_SetpointManager_MultiZone_Humidity_MaximumFields::ControlVariable);
  }

  double SetpointManagerMultiZoneHumidityMaximum::minimumSetpointHumidityRatio() const {
    return getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>()->minimumSetpointHumidityRatio();
  }

  double SetpointManagerMultiZoneHumidityMaximum::maximumSetpointHumidityRatio() const {
    return getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>()->maximumSetpointHumidityRatio();
  }

  bool SetpointManagerMultiZoneHumidityMaximum::setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio) {
    return getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>()->setMinimumSetpointHumidityRatio(minimumSetpointHumidityRatio);
  }

  bool SetpointManagerMultiZoneHumidityMaximum::setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio) {
    return getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>()->setMaximumSetpointHumidityRatio(maximumSetpointHumidityRatio);
  }

  /// @cond
  SetpointManagerMultiZoneHumidityMaximum::SetpointManagerMultiZoneHumidityMaximum(
    std::shared_ptr<detail::SetpointManagerMultiZoneHumidityMaximum_Impl> impl)
    : SetpointManager(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
