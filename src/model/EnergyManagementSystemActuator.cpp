/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "EnergyManagementSystemActuator.hpp"
#include "EnergyManagementSystemActuator_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "SpaceLoadInstance.hpp"
#include "SpaceLoadInstance_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_Actuator_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/DeprecatedHelpers.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    EnergyManagementSystemActuator_Impl::EnergyManagementSystemActuator_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemActuator::iddObjectType());
    }

    EnergyManagementSystemActuator_Impl::EnergyManagementSystemActuator_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                             bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == EnergyManagementSystemActuator::iddObjectType());
    }

    EnergyManagementSystemActuator_Impl::EnergyManagementSystemActuator_Impl(const EnergyManagementSystemActuator_Impl& other, Model_Impl* model,
                                                                             bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& EnergyManagementSystemActuator_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType EnergyManagementSystemActuator_Impl::iddObjectType() const {
      return EnergyManagementSystemActuator::iddObjectType();
    }

    boost::optional<ModelObject> EnergyManagementSystemActuator_Impl::actuatedComponent() const {
      boost::optional<ModelObject> result;
      boost::optional<WorkspaceObject> wo = this->getTarget(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentName);
      if (wo) {
        result = wo->cast<ModelObject>();
      }
      return result;
    }

    std::string EnergyManagementSystemActuator_Impl::actuatedComponentControlType() const {
      boost::optional<std::string> value = getString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, true);
      if (value) {
        return value.get();
      } else {
        return "";
      }
    }

    std::string EnergyManagementSystemActuator_Impl::actuatedComponentType() const {
      boost::optional<std::string> value = getString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentType, true);
      if (value) {
        return value.get();
      } else {
        return "";
      }
    }

    bool EnergyManagementSystemActuator_Impl::setActuatedComponent(const ModelObject& modelObject) {
      if (!modelObject.optionalCast<SpaceLoadInstance>()) {
        resetZoneOrSpace();
      }

      return setPointer(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentName, modelObject.handle());
    }

    bool EnergyManagementSystemActuator_Impl::setActuatedComponentControlType(const std::string& actuatedComponentControlType) {
      const bool result = setString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType, actuatedComponentControlType);
      return result;
    }

    bool EnergyManagementSystemActuator_Impl::setActuatedComponentType(const std::string& actuatedComponentType) {
      const bool result = setString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentType, actuatedComponentType);
      return result;
    }

    boost::optional<ModelObject> EnergyManagementSystemActuator_Impl::zoneOrSpace() const {
      return getObject<ModelObject>().getModelObjectTarget<ModelObject>(OS_EnergyManagementSystem_ActuatorFields::ZoneorSpaceName);
    }

    boost::optional<ThermalZone> EnergyManagementSystemActuator_Impl::thermalZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_EnergyManagementSystem_ActuatorFields::ZoneorSpaceName);
    }

    boost::optional<Space> EnergyManagementSystemActuator_Impl::space() const {
      return getObject<ModelObject>().getModelObjectTarget<Space>(OS_EnergyManagementSystem_ActuatorFields::ZoneorSpaceName);
    }

    bool EnergyManagementSystemActuator_Impl::setThermalZone(const ThermalZone& thermalZone) {
      if (auto comp_ = actuatedComponent()) {
        if (comp_->optionalCast<SpaceLoadInstance>()) {
          return setPointer(OS_EnergyManagementSystem_ActuatorFields::ZoneorSpaceName, thermalZone.handle());
        }
      }
      LOG(Warn, "For " << briefDescription() << " cannot setThermalZone has it does not have an attached Component Type that is a SpaceLoadInstance");
      return false;
    }

    bool EnergyManagementSystemActuator_Impl::setSpace(const Space& space) {
      if (auto comp_ = actuatedComponent()) {
        if (comp_->optionalCast<SpaceLoadInstance>()) {
          return setPointer(OS_EnergyManagementSystem_ActuatorFields::ZoneorSpaceName, space.handle());
        }
      }
      LOG(Warn, "For " << briefDescription() << " cannot setSpace has it does not have an attached Component Type that is a SpaceLoadInstance");
      return false;
    }

    void EnergyManagementSystemActuator_Impl::resetZoneOrSpace() {
      const bool result = setString(OS_EnergyManagementSystem_ActuatorFields::ZoneorSpaceName, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  EnergyManagementSystemActuator::EnergyManagementSystemActuator(const ModelObject& actuatedComponent, const std::string& actuatedComponentType,
                                                                 const std::string& actuatedComponentControlType)
    : ModelObject(EnergyManagementSystemActuator::iddObjectType(), actuatedComponent.model()) {
    OS_ASSERT(getImpl<detail::EnergyManagementSystemActuator_Impl>());
    /** Do not use this constructor for spaceloads that are defined in SpaceType objects **/

    if (actuatedComponent.optionalCast<SpaceLoadInstance>()) {
      LOG(Warn, "When instantiating an EnergyManagementSystemActuator with an actuated component that is a SpaceLoadInstance, you should instead use "
                "the overloaded constructor that takes a Space (or a ThermalZone if you plan on disabling the Space Feature during "
                "ForwardTranslation) as the last parameter");
    }

    bool ok = setActuatedComponent(actuatedComponent);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s setActuatedComponent to " << actuatedComponent.briefDescription() << ".");
    }
    ok = setActuatedComponentType(actuatedComponentType);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s actuatedComponentType to " << actuatedComponentType << ".");
    }
    ok = setActuatedComponentControlType(actuatedComponentControlType);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s actuatedComponentControlType to " << actuatedComponentControlType << ".");
    }
  }

  EnergyManagementSystemActuator::EnergyManagementSystemActuator(const ModelObject& actuatedComponent, const std::string& actuatedComponentType,
                                                                 const std::string& actuatedComponentControlType, const ThermalZone& thermalZone)
    : ModelObject(EnergyManagementSystemActuator::iddObjectType(), actuatedComponent.model()) {
    OS_ASSERT(getImpl<detail::EnergyManagementSystemActuator_Impl>());
    /** This constructor uses the ThermalZone as the ZoneName field
      * This is what is expected in E+ for spaceloads in OS that are defined in SpaceTypes and used in Spaces **/

    if (!actuatedComponent.optionalCast<SpaceLoadInstance>()) {
      remove();
      LOG_AND_THROW("When using this overloaded EnergyManagementSystemActuator constructor that takes a ThermalZone as the last parameter, the "
                    "actuated component must be "
                    "a SpaceLoadInstance, but you supplied "
                    << actuatedComponent.briefDescription());
    }

    bool ok = setActuatedComponent(actuatedComponent);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s setActuatedComponent to " << actuatedComponent.briefDescription() << ".");
    }
    ok = setActuatedComponentType(actuatedComponentType);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s actuatedComponentType to " << actuatedComponentType << ".");
    }
    ok = setActuatedComponentControlType(actuatedComponentControlType);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s actuatedComponentControlType to " << actuatedComponentControlType << ".");
    }
    //set the ModelObject to the ZoneName field (uses name of this object on FT)
    ok = setThermalZone(thermalZone);
    //Fail on failure to set modelobject to ZoneName field
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s setThermalZone to " << thermalZone.briefDescription() << ".");
    }
  }

  EnergyManagementSystemActuator::EnergyManagementSystemActuator(const ModelObject& actuatedComponent, const std::string& actuatedComponentType,
                                                                 const std::string& actuatedComponentControlType, const Space& space)
    : ModelObject(EnergyManagementSystemActuator::iddObjectType(), actuatedComponent.model()) {
    OS_ASSERT(getImpl<detail::EnergyManagementSystemActuator_Impl>());

    if (!actuatedComponent.optionalCast<SpaceLoadInstance>()) {
      remove();
      LOG_AND_THROW("When using this overloaded EnergyManagementSystemActuator constructor that takes a Space as the last parameter, the actuated "
                    "component must be "
                    "a SpaceLoadInstance, but you supplied "
                    << actuatedComponent.briefDescription());
    }

    bool ok = setActuatedComponent(actuatedComponent);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s setActuatedComponent to " << actuatedComponent.briefDescription() << ".");
    }
    ok = setActuatedComponentType(actuatedComponentType);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s actuatedComponentType to " << actuatedComponentType << ".");
    }
    ok = setActuatedComponentControlType(actuatedComponentControlType);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s actuatedComponentControlType to " << actuatedComponentControlType << ".");
    }
    //set the Space's ThermalZone to the ZoneName field (uses name of this object on FT)
    ok = setSpace(space);
    //Fail on failure to set space to ZoneName field
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s setSpace to " << space.briefDescription() << ".");
    }
  }

  EnergyManagementSystemActuator::EnergyManagementSystemActuator(const ModelObject& actuatedComponent)
    : ModelObject(EnergyManagementSystemActuator::iddObjectType(), actuatedComponent.model()) {
    OS_ASSERT(getImpl<detail::EnergyManagementSystemActuator_Impl>());
    setActuatedComponent(actuatedComponent);
    //TODO set below to some appropriate default that depends on the actuatedComponent::IddType
    //setActuatedComponentControlType();
    //setActuatedComponentType();
  }

  EnergyManagementSystemActuator::EnergyManagementSystemActuator(const Model& model)
    : ModelObject(EnergyManagementSystemActuator::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::EnergyManagementSystemActuator_Impl>());
    //This constructor is used in the ReverseTranslator when the ActuatedComponent doesnt Reverse Translate.
    //In that case, a generic Actuator is created with ActuatedComponent blank (but the old value is in a comment)
    //TODO setActuatedComponent to a default actuatedComponent.
    //TODO set below to some appropriate default that depends on the actuatedComponent::IddType
    //setActuatedComponentControlType();
    //setActuatedComponentType();
  }

  IddObjectType EnergyManagementSystemActuator::iddObjectType() {
    return {IddObjectType::OS_EnergyManagementSystem_Actuator};
  }

  boost::optional<ModelObject> EnergyManagementSystemActuator::actuatedComponent() const {
    return getImpl<detail::EnergyManagementSystemActuator_Impl>()->actuatedComponent();
  }

  std::string EnergyManagementSystemActuator::actuatedComponentControlType() const {
    return getImpl<detail::EnergyManagementSystemActuator_Impl>()->actuatedComponentControlType();
  }

  std::string EnergyManagementSystemActuator::actuatedComponentType() const {
    return getImpl<detail::EnergyManagementSystemActuator_Impl>()->actuatedComponentType();
  }

  bool EnergyManagementSystemActuator::setActuatedComponent(const ModelObject& modelObject) {
    return getImpl<detail::EnergyManagementSystemActuator_Impl>()->setActuatedComponent(modelObject);
  }

  bool EnergyManagementSystemActuator::setActuatedComponentControlType(const std::string& actuatedComponentControlType) {
    return getImpl<detail::EnergyManagementSystemActuator_Impl>()->setActuatedComponentControlType(actuatedComponentControlType);
  }

  bool EnergyManagementSystemActuator::setActuatedComponentType(const std::string& actuatedComponentType) {
    return getImpl<detail::EnergyManagementSystemActuator_Impl>()->setActuatedComponentType(actuatedComponentType);
  }

  boost::optional<Space> EnergyManagementSystemActuator::space() const {
    return getImpl<detail::EnergyManagementSystemActuator_Impl>()->space();
  }

  boost::optional<ThermalZone> EnergyManagementSystemActuator::thermalZone() const {
    return getImpl<detail::EnergyManagementSystemActuator_Impl>()->thermalZone();
  }

  boost::optional<ModelObject> EnergyManagementSystemActuator::zoneOrSpace() const {
    return getImpl<detail::EnergyManagementSystemActuator_Impl>()->zoneOrSpace();
  }

  bool EnergyManagementSystemActuator::setThermalZone(const ThermalZone& thermalZone) {
    return getImpl<detail::EnergyManagementSystemActuator_Impl>()->setThermalZone(thermalZone);
  }

  bool EnergyManagementSystemActuator::setSpace(const Space& space) {
    return getImpl<detail::EnergyManagementSystemActuator_Impl>()->setSpace(space);
  }

  void EnergyManagementSystemActuator::resetZoneOrSpace() {
    getImpl<detail::EnergyManagementSystemActuator_Impl>()->resetZoneOrSpace();
  }

  /// @cond
  EnergyManagementSystemActuator::EnergyManagementSystemActuator(std::shared_ptr<detail::EnergyManagementSystemActuator_Impl> impl)
    : ModelObject(std::move(impl)) {}

  // DEPRECATED

  boost::optional<ModelObject> EnergyManagementSystemActuator::zoneName() const {
    DEPRECATED_AT_MSG(3, 6, 0, "Use zoneOrSpace instead.");
    return getImpl<detail::EnergyManagementSystemActuator_Impl>()->zoneOrSpace();
  }

  void EnergyManagementSystemActuator::resetZoneName() {
    DEPRECATED_AT_MSG(3, 6, 0, "Use resetZoneOrSpace instead.");
    getImpl<detail::EnergyManagementSystemActuator_Impl>()->resetZoneOrSpace();
  }
  /// @endcond

}  // namespace model
}  // namespace openstudio
