/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SetpointManagerFollowGroundTemperature.hpp"
#include "SetpointManagerFollowGroundTemperature_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SetpointManager_FollowGroundTemperature_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SetpointManagerFollowGroundTemperature_Impl::SetpointManagerFollowGroundTemperature_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                             bool keepHandle)
      : SetpointManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SetpointManagerFollowGroundTemperature::iddObjectType());
    }

    SetpointManagerFollowGroundTemperature_Impl::SetpointManagerFollowGroundTemperature_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SetpointManagerFollowGroundTemperature::iddObjectType());
    }

    SetpointManagerFollowGroundTemperature_Impl::SetpointManagerFollowGroundTemperature_Impl(const SetpointManagerFollowGroundTemperature_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SetpointManagerFollowGroundTemperature_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SetpointManagerFollowGroundTemperature_Impl::iddObjectType() const {
      return SetpointManagerFollowGroundTemperature::iddObjectType();
    }

    /** This SPM is allowed on a PlantLoop */
    bool SetpointManagerFollowGroundTemperature_Impl::isAllowedOnPlantLoop() const {
      return true;
    }

    std::string SetpointManagerFollowGroundTemperature_Impl::controlVariable() const {
      boost::optional<std::string> value = getString(OS_SetpointManager_FollowGroundTemperatureFields::ControlVariable, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string SetpointManagerFollowGroundTemperature_Impl::referenceGroundTemperatureObjectType() const {
      boost::optional<std::string> value = getString(OS_SetpointManager_FollowGroundTemperatureFields::ReferenceGroundTemperatureObjectType, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerFollowGroundTemperature_Impl::offsetTemperatureDifference() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_FollowGroundTemperatureFields::OffsetTemperatureDifference, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerFollowGroundTemperature_Impl::maximumSetpointTemperature() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_FollowGroundTemperatureFields::MaximumSetpointTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    double SetpointManagerFollowGroundTemperature_Impl::minimumSetpointTemperature() const {
      boost::optional<double> value = getDouble(OS_SetpointManager_FollowGroundTemperatureFields::MinimumSetpointTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Node> SetpointManagerFollowGroundTemperature_Impl::setpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_FollowGroundTemperatureFields::SetpointNodeorNodeListName);
    }

    bool SetpointManagerFollowGroundTemperature_Impl::setControlVariable(const std::string& controlVariable) {
      bool result = setString(OS_SetpointManager_FollowGroundTemperatureFields::ControlVariable, controlVariable);
      return result;
    }

    bool SetpointManagerFollowGroundTemperature_Impl::setReferenceGroundTemperatureObjectType(const std::string& groundTemperatureObjType) {
      bool result = setString(OS_SetpointManager_FollowGroundTemperatureFields::ReferenceGroundTemperatureObjectType, groundTemperatureObjType);
      return result;
    }

    bool SetpointManagerFollowGroundTemperature_Impl::setOffsetTemperatureDifference(double offsetTemperatureDifference) {
      bool result = setDouble(OS_SetpointManager_FollowGroundTemperatureFields::OffsetTemperatureDifference, offsetTemperatureDifference);
      OS_ASSERT(result);
      return result;
    }

    bool SetpointManagerFollowGroundTemperature_Impl::setMaximumSetpointTemperature(double maximumSetpointTemperature) {
      bool result = setDouble(OS_SetpointManager_FollowGroundTemperatureFields::MaximumSetpointTemperature, maximumSetpointTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool SetpointManagerFollowGroundTemperature_Impl::setMinimumSetpointTemperature(double minimumSetpointTemperature) {
      bool result = setDouble(OS_SetpointManager_FollowGroundTemperatureFields::MinimumSetpointTemperature, minimumSetpointTemperature);
      OS_ASSERT(result);
      return result;
    }

    bool SetpointManagerFollowGroundTemperature_Impl::setSetpointNode(const Node& node) {
      bool result = setPointer(OS_SetpointManager_FollowGroundTemperatureFields::SetpointNodeorNodeListName, node.handle());
      return result;
    }

    void SetpointManagerFollowGroundTemperature_Impl::resetSetpointNode() {
      bool result = setString(OS_SetpointManager_FollowGroundTemperatureFields::SetpointNodeorNodeListName, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  SetpointManagerFollowGroundTemperature::SetpointManagerFollowGroundTemperature(const Model& model)
    : SetpointManager(SetpointManagerFollowGroundTemperature::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SetpointManagerFollowGroundTemperature_Impl>());

    setControlVariable("Temperature");
    setOffsetTemperatureDifference(0);
    setMaximumSetpointTemperature(80);
    setMinimumSetpointTemperature(10);
  }

  IddObjectType SetpointManagerFollowGroundTemperature::iddObjectType() {
    return {IddObjectType::OS_SetpointManager_FollowGroundTemperature};
  }

  std::vector<std::string> SetpointManagerFollowGroundTemperature::controlVariableValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_SetpointManager_FollowGroundTemperatureFields::ControlVariable);
  }

  std::vector<std::string> SetpointManagerFollowGroundTemperature::referenceGroundTemperatureObjectTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_SetpointManager_FollowGroundTemperatureFields::ReferenceGroundTemperatureObjectType);
  }

  std::string SetpointManagerFollowGroundTemperature::referenceGroundTemperatureObjectType() const {
    return getImpl<detail::SetpointManagerFollowGroundTemperature_Impl>()->referenceGroundTemperatureObjectType();
  }

  double SetpointManagerFollowGroundTemperature::offsetTemperatureDifference() const {
    return getImpl<detail::SetpointManagerFollowGroundTemperature_Impl>()->offsetTemperatureDifference();
  }

  double SetpointManagerFollowGroundTemperature::maximumSetpointTemperature() const {
    return getImpl<detail::SetpointManagerFollowGroundTemperature_Impl>()->maximumSetpointTemperature();
  }

  double SetpointManagerFollowGroundTemperature::minimumSetpointTemperature() const {
    return getImpl<detail::SetpointManagerFollowGroundTemperature_Impl>()->minimumSetpointTemperature();
  }

  bool SetpointManagerFollowGroundTemperature::setReferenceGroundTemperatureObjectType(const std::string& groundTemperatureObjType) {
    return getImpl<detail::SetpointManagerFollowGroundTemperature_Impl>()->setReferenceGroundTemperatureObjectType(groundTemperatureObjType);
  }

  bool SetpointManagerFollowGroundTemperature::setOffsetTemperatureDifference(double offsetTemperatureDifference) {
    return getImpl<detail::SetpointManagerFollowGroundTemperature_Impl>()->setOffsetTemperatureDifference(offsetTemperatureDifference);
  }

  bool SetpointManagerFollowGroundTemperature::setMaximumSetpointTemperature(double maximumSetpointTemperature) {
    return getImpl<detail::SetpointManagerFollowGroundTemperature_Impl>()->setMaximumSetpointTemperature(maximumSetpointTemperature);
  }

  bool SetpointManagerFollowGroundTemperature::setMinimumSetpointTemperature(double minimumSetpointTemperature) {
    return getImpl<detail::SetpointManagerFollowGroundTemperature_Impl>()->setMinimumSetpointTemperature(minimumSetpointTemperature);
  }

  /// @cond
  SetpointManagerFollowGroundTemperature::SetpointManagerFollowGroundTemperature(
    std::shared_ptr<detail::SetpointManagerFollowGroundTemperature_Impl> impl)
    : SetpointManager(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
