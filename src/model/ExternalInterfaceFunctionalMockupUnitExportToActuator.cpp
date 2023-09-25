/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ExternalInterfaceFunctionalMockupUnitExportToActuator.hpp"
#include "ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitExport_To_Actuator_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl(const IdfObject& idfObject,
                                                                                                                           Model_Impl* model,
                                                                                                                           bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ExternalInterfaceFunctionalMockupUnitExportToActuator::iddObjectType());
    }

    ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ExternalInterfaceFunctionalMockupUnitExportToActuator::iddObjectType());
    }

    ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl(
      const ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::iddObjectType() const {
      return ExternalInterfaceFunctionalMockupUnitExportToActuator::iddObjectType();
    }

    ModelObject ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::actuatedComponentUnique() const {
      return this->getTarget(OS_ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::ActuatedComponentUniqueName)->cast<ModelObject>();
    }

    std::string ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::actuatedComponentType() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::ActuatedComponentType, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    std::string ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::actuatedComponentControlType() const {
      boost::optional<std::string> value =
        getString(OS_ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::ActuatedComponentControlType, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    std::string ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::fMUVariableName() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::FMUVariableName, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    double ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::initialValue() const {
      boost::optional<double> value = getDouble(OS_ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::InitialValue, true);
      if (value) {
        return value.get();
      }
      return -9999;
    }

    bool ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::setActuatedComponentUnique(const ModelObject& modelObject) {
      bool result = setPointer(OS_ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::ActuatedComponentUniqueName, modelObject.handle());
      return result;
    }

    bool ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::setActuatedComponentType(const std::string& actuatedComponentType) {
      bool result = setString(OS_ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::ActuatedComponentType, actuatedComponentType);
      OS_ASSERT(result);
      return result;
    }

    bool
      ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::setActuatedComponentControlType(const std::string& actuatedComponentControlType) {
      bool result =
        setString(OS_ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::ActuatedComponentControlType, actuatedComponentControlType);
      OS_ASSERT(result);
      return result;
    }

    bool ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::setFMUVariableName(const std::string& fMUVariableName) {
      bool result = setString(OS_ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::FMUVariableName, fMUVariableName);
      OS_ASSERT(result);
      return result;
    }

    bool ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl::setInitialValue(double initialValue) {
      bool result = setDouble(OS_ExternalInterface_FunctionalMockupUnitExport_To_ActuatorFields::InitialValue, initialValue);
      OS_ASSERT(result);
      return result;
    }

  }  // namespace detail

  ExternalInterfaceFunctionalMockupUnitExportToActuator::ExternalInterfaceFunctionalMockupUnitExportToActuator(
    const ModelObject& modelObject, const std::string& actuatedComponentType, const std::string& actuatedComponentControlType,
    const std::string& fMUVariableName, double initialValue)
    : ModelObject(ExternalInterfaceFunctionalMockupUnitExportToActuator::iddObjectType(), modelObject.model()) {
    OS_ASSERT(getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl>());

    bool ok = setActuatedComponentUnique(modelObject);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s ActuatedComponentUnique to " << modelObject.nameString() << ".");
    }
    setActuatedComponentType(actuatedComponentType);
    setActuatedComponentControlType(actuatedComponentControlType);
    setFMUVariableName(fMUVariableName);
    setInitialValue(initialValue);
  }

  IddObjectType ExternalInterfaceFunctionalMockupUnitExportToActuator::iddObjectType() {
    return {IddObjectType::OS_ExternalInterface_FunctionalMockupUnitExport_To_Actuator};
  }

  ModelObject ExternalInterfaceFunctionalMockupUnitExportToActuator::actuatedComponentUnique() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl>()->actuatedComponentUnique();
  }

  std::string ExternalInterfaceFunctionalMockupUnitExportToActuator::actuatedComponentType() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl>()->actuatedComponentType();
  }

  std::string ExternalInterfaceFunctionalMockupUnitExportToActuator::actuatedComponentControlType() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl>()->actuatedComponentControlType();
  }

  std::string ExternalInterfaceFunctionalMockupUnitExportToActuator::fMUVariableName() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl>()->fMUVariableName();
  }

  double ExternalInterfaceFunctionalMockupUnitExportToActuator::initialValue() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl>()->initialValue();
  }

  bool ExternalInterfaceFunctionalMockupUnitExportToActuator::setActuatedComponentUnique(const ModelObject& modelObject) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl>()->setActuatedComponentUnique(modelObject);
  }

  bool ExternalInterfaceFunctionalMockupUnitExportToActuator::setActuatedComponentType(const std::string& actuatedComponentType) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl>()->setActuatedComponentType(actuatedComponentType);
  }

  bool ExternalInterfaceFunctionalMockupUnitExportToActuator::setActuatedComponentControlType(const std::string& actuatedComponentControlType) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl>()->setActuatedComponentControlType(
      actuatedComponentControlType);
  }

  bool ExternalInterfaceFunctionalMockupUnitExportToActuator::setFMUVariableName(const std::string& fMUVariableName) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl>()->setFMUVariableName(fMUVariableName);
  }

  bool ExternalInterfaceFunctionalMockupUnitExportToActuator::setInitialValue(double initialValue) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl>()->setInitialValue(initialValue);
  }

  /// @cond
  ExternalInterfaceFunctionalMockupUnitExportToActuator::ExternalInterfaceFunctionalMockupUnitExportToActuator(
    std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl> impl)
    : ModelObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
