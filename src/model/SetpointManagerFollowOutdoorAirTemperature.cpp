/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SetpointManagerFollowOutdoorAirTemperature.hpp"
#include "SetpointManagerFollowOutdoorAirTemperature_Impl.hpp"

#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"

#include <utilities/idd/OS_SetpointManager_FollowOutdoorAirTemperature_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    SetpointManagerFollowOutdoorAirTemperature_Impl::SetpointManagerFollowOutdoorAirTemperature_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                                     bool keepHandle)
      : SetpointManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SetpointManagerFollowOutdoorAirTemperature::iddObjectType());
    }

    SetpointManagerFollowOutdoorAirTemperature_Impl::SetpointManagerFollowOutdoorAirTemperature_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : SetpointManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SetpointManagerFollowOutdoorAirTemperature::iddObjectType());
    }

    SetpointManagerFollowOutdoorAirTemperature_Impl::SetpointManagerFollowOutdoorAirTemperature_Impl(
      const SetpointManagerFollowOutdoorAirTemperature_Impl& other, Model_Impl* model, bool keepHandles)
      : SetpointManager_Impl(other, model, keepHandles) {}

    const std::vector<std::string>& SetpointManagerFollowOutdoorAirTemperature_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SetpointManagerFollowOutdoorAirTemperature_Impl::iddObjectType() const {
      return SetpointManagerFollowOutdoorAirTemperature::iddObjectType();
    }

    /** This SPM is allowed on a PlantLoop */
    bool SetpointManagerFollowOutdoorAirTemperature_Impl::isAllowedOnPlantLoop() const {
      return true;
    }

    boost::optional<Node> SetpointManagerFollowOutdoorAirTemperature_Impl::setpointNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_FollowOutdoorAirTemperatureFields::SetpointNodeorNodeListName);
    }

    bool SetpointManagerFollowOutdoorAirTemperature_Impl::setSetpointNode(const Node& node) {
      return setPointer(OS_SetpointManager_FollowOutdoorAirTemperatureFields::SetpointNodeorNodeListName, node.handle());
    }

    void SetpointManagerFollowOutdoorAirTemperature_Impl::resetSetpointNode() {
      bool result = setString(OS_SetpointManager_FollowOutdoorAirTemperatureFields::SetpointNodeorNodeListName, "");
      OS_ASSERT(result);
    }

    std::string SetpointManagerFollowOutdoorAirTemperature_Impl::controlVariable() const {
      return this->getString(OS_SetpointManager_FollowOutdoorAirTemperatureFields::ControlVariable).get();
    }

    bool SetpointManagerFollowOutdoorAirTemperature_Impl::setControlVariable(const std::string& value) {
      if (istringEqual(value, "Temperature")) {
        return this->setString(OS_SetpointManager_FollowOutdoorAirTemperatureFields::ControlVariable, "Temperature");
      } else if (istringEqual(value, "MaximumTemperature")) {
        return this->setString(OS_SetpointManager_FollowOutdoorAirTemperatureFields::ControlVariable, "MaximumTemperature");
      } else if (istringEqual(value, "MinimumTemperature")) {
        return this->setString(OS_SetpointManager_FollowOutdoorAirTemperatureFields::ControlVariable, "MinimumTemperature");
      } else {
        return false;
      }
    }

    bool SetpointManagerFollowOutdoorAirTemperature_Impl::setReferenceTemperatureType(const std::string& value) {
      if (istringEqual(value, "OutdoorAirWetBulb")) {
        return this->setString(OS_SetpointManager_FollowOutdoorAirTemperatureFields::ReferenceTemperatureType, "OutdoorAirWetBulb");
      } else if (istringEqual(value, "OutdoorAirDryBulb")) {
        return this->setString(OS_SetpointManager_FollowOutdoorAirTemperatureFields::ReferenceTemperatureType, "OutdoorAirDryBulb");
      }
      return false;
    }

    std::string SetpointManagerFollowOutdoorAirTemperature_Impl::referenceTemperatureType() const {
      return getString(OS_SetpointManager_FollowOutdoorAirTemperatureFields::ReferenceTemperatureType).get();
    }

    double SetpointManagerFollowOutdoorAirTemperature_Impl::offsetTemperatureDifference() const {
      return getDouble(OS_SetpointManager_FollowOutdoorAirTemperatureFields::OffsetTemperatureDifference).get();
    }

    bool SetpointManagerFollowOutdoorAirTemperature_Impl::setOffsetTemperatureDifference(double value) {
      return setDouble(OS_SetpointManager_FollowOutdoorAirTemperatureFields::OffsetTemperatureDifference, value);
    }

    double SetpointManagerFollowOutdoorAirTemperature_Impl::maximumSetpointTemperature() const {
      return getDouble(OS_SetpointManager_FollowOutdoorAirTemperatureFields::MaximumSetpointTemperature).get();
    }

    bool SetpointManagerFollowOutdoorAirTemperature_Impl::setMaximumSetpointTemperature(double value) {
      return setDouble(OS_SetpointManager_FollowOutdoorAirTemperatureFields::MaximumSetpointTemperature, value);
    }

    double SetpointManagerFollowOutdoorAirTemperature_Impl::minimumSetpointTemperature() const {
      return getDouble(OS_SetpointManager_FollowOutdoorAirTemperatureFields::MinimumSetpointTemperature).get();
    }

    bool SetpointManagerFollowOutdoorAirTemperature_Impl::setMinimumSetpointTemperature(double value) {
      return setDouble(OS_SetpointManager_FollowOutdoorAirTemperatureFields::MinimumSetpointTemperature, value);
    }

  }  // namespace detail

  SetpointManagerFollowOutdoorAirTemperature::SetpointManagerFollowOutdoorAirTemperature(const Model& model)
    : SetpointManager(SetpointManagerFollowOutdoorAirTemperature::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>());

    setControlVariable("Temperature");
    setReferenceTemperatureType("OutdoorAirWetBulb");
    setOffsetTemperatureDifference(1.5);
    setMaximumSetpointTemperature(80.0);
    setMinimumSetpointTemperature(6.0);
  }

  SetpointManagerFollowOutdoorAirTemperature::SetpointManagerFollowOutdoorAirTemperature(
    std::shared_ptr<detail::SetpointManagerFollowOutdoorAirTemperature_Impl> p)
    : SetpointManager(std::move(p)) {}

  IddObjectType SetpointManagerFollowOutdoorAirTemperature::iddObjectType() {
    return {IddObjectType::OS_SetpointManager_FollowOutdoorAirTemperature};
  }

  boost::optional<Node> SetpointManagerFollowOutdoorAirTemperature::setpointNode() const {
    return getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->setpointNode();
  }

  std::string SetpointManagerFollowOutdoorAirTemperature::controlVariable() const {
    return getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->controlVariable();
  }

  bool SetpointManagerFollowOutdoorAirTemperature::setControlVariable(const std::string& value) {
    return getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->setControlVariable(value);
  }

  std::string SetpointManagerFollowOutdoorAirTemperature::referenceTemperatureType() const {
    return getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->referenceTemperatureType();
  }

  bool SetpointManagerFollowOutdoorAirTemperature::setReferenceTemperatureType(const std::string& value) {
    return getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->setReferenceTemperatureType(value);
  }

  double SetpointManagerFollowOutdoorAirTemperature::offsetTemperatureDifference() const {
    return getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->offsetTemperatureDifference();
  }

  bool SetpointManagerFollowOutdoorAirTemperature::setOffsetTemperatureDifference(double value) {
    return getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->setOffsetTemperatureDifference(value);
  }

  double SetpointManagerFollowOutdoorAirTemperature::maximumSetpointTemperature() const {
    return getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->maximumSetpointTemperature();
  }

  bool SetpointManagerFollowOutdoorAirTemperature::setMaximumSetpointTemperature(double value) {
    return getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->setMaximumSetpointTemperature(value);
  }

  double SetpointManagerFollowOutdoorAirTemperature::minimumSetpointTemperature() const {
    return getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->minimumSetpointTemperature();
  }

  bool SetpointManagerFollowOutdoorAirTemperature::setMinimumSetpointTemperature(double value) {
    return getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->setMinimumSetpointTemperature(value);
  }

}  // namespace model

}  // namespace openstudio
