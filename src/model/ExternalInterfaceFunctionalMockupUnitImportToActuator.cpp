/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ExternalInterfaceFunctionalMockupUnitImportToActuator.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl.hpp"

#include "ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitImport_To_Actuator_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl::ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl(const IdfObject& idfObject,
                                                                                                                           Model_Impl* model,
                                                                                                                           bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ExternalInterfaceFunctionalMockupUnitImportToActuator::iddObjectType());
    }

    ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl::ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ExternalInterfaceFunctionalMockupUnitImportToActuator::iddObjectType());
    }

    ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl::ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl(
      const ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl::iddObjectType() const {
      return ExternalInterfaceFunctionalMockupUnitImportToActuator::iddObjectType();
    }

    ModelObject ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl::actuatedComponentUnique() const {
      return this->getTarget(OS_ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::ActuatedComponentUniqueName)->cast<ModelObject>();
    }

    std::string ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl::actuatedComponentType() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::ActuatedComponentType, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    std::string ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl::actuatedComponentControlType() const {
      boost::optional<std::string> value =
        getString(OS_ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::ActuatedComponentControlType, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    ExternalInterfaceFunctionalMockupUnitImport ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl::fMUFile() const {
      return this->getTarget(OS_ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::FMUFileName)
        ->cast<ExternalInterfaceFunctionalMockupUnitImport>();
    }

    std::string ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl::fMUInstanceName() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::FMUInstanceName, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    std::string ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl::fMUVariableName() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::FMUVariableName, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    double ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl::initialValue() const {
      boost::optional<double> value = getDouble(OS_ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::InitialValue, true);
      if (value) {
        return value.get();
      }
      return -9999;
    }

    bool ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl::setActuatedComponentUnique(const ModelObject& modelObject) {
      bool result = setPointer(OS_ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::ActuatedComponentUniqueName, modelObject.handle());
      return result;
    }

    bool ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl::setActuatedComponentType(const std::string& actuatedComponentType) {
      bool result = setString(OS_ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::ActuatedComponentType, actuatedComponentType);
      OS_ASSERT(result);
      return result;
    }

    bool
      ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl::setActuatedComponentControlType(const std::string& actuatedComponentControlType) {
      bool result =
        setString(OS_ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::ActuatedComponentControlType, actuatedComponentControlType);
      OS_ASSERT(result);
      return result;
    }

    bool ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl::setFMUFile(const ExternalInterfaceFunctionalMockupUnitImport& fMUFile) {
      bool result = setPointer(OS_ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::FMUFileName, fMUFile.handle());
      return result;
      //TODO in forward translator set this to the file name
    }

    bool ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl::setFMUInstanceName(const std::string& fMUInstanceName) {
      bool result = setString(OS_ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::FMUInstanceName, fMUInstanceName);
      OS_ASSERT(result);
      return result;
    }

    bool ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl::setFMUVariableName(const std::string& fMUVariableName) {
      bool result = setString(OS_ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::FMUVariableName, fMUVariableName);
      OS_ASSERT(result);
      return result;
    }

    bool ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl::setInitialValue(double initialValue) {
      bool result = setDouble(OS_ExternalInterface_FunctionalMockupUnitImport_To_ActuatorFields::InitialValue, initialValue);
      OS_ASSERT(result);
      return result;
    }

  }  // namespace detail

  ExternalInterfaceFunctionalMockupUnitImportToActuator::ExternalInterfaceFunctionalMockupUnitImportToActuator(
    const ModelObject& modelObject, const std::string& actuatedComponentType, const std::string& actuatedComponentControlType,
    const ExternalInterfaceFunctionalMockupUnitImport& fMUFile, const std::string& fMUInstanceName, const std::string& fMUVariableName,
    double initialValue)
    : ModelObject(ExternalInterfaceFunctionalMockupUnitImportToActuator::iddObjectType(), modelObject.model()) {
    OS_ASSERT(getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl>());

    bool ok = setActuatedComponentUnique(modelObject);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s ActuatedComponentUnique to " << modelObject.nameString() << ".");
    }
    setActuatedComponentType(actuatedComponentType);
    setActuatedComponentControlType(actuatedComponentControlType);
    ok = setFMUFile(fMUFile);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s FMUFileName to " << fMUFile.fMUFileName() << ".");
    }
    setFMUInstanceName(fMUInstanceName);
    setFMUVariableName(fMUVariableName);
    setInitialValue(initialValue);
  }

  IddObjectType ExternalInterfaceFunctionalMockupUnitImportToActuator::iddObjectType() {
    return {IddObjectType::OS_ExternalInterface_FunctionalMockupUnitImport_To_Actuator};
  }

  ModelObject ExternalInterfaceFunctionalMockupUnitImportToActuator::actuatedComponentUnique() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl>()->actuatedComponentUnique();
  }

  std::string ExternalInterfaceFunctionalMockupUnitImportToActuator::actuatedComponentType() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl>()->actuatedComponentType();
  }

  std::string ExternalInterfaceFunctionalMockupUnitImportToActuator::actuatedComponentControlType() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl>()->actuatedComponentControlType();
  }

  ExternalInterfaceFunctionalMockupUnitImport ExternalInterfaceFunctionalMockupUnitImportToActuator::fMUFile() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl>()->fMUFile();
  }

  std::string ExternalInterfaceFunctionalMockupUnitImportToActuator::fMUInstanceName() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl>()->fMUInstanceName();
  }

  std::string ExternalInterfaceFunctionalMockupUnitImportToActuator::fMUVariableName() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl>()->fMUVariableName();
  }

  double ExternalInterfaceFunctionalMockupUnitImportToActuator::initialValue() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl>()->initialValue();
  }

  bool ExternalInterfaceFunctionalMockupUnitImportToActuator::setActuatedComponentUnique(const ModelObject& modelObject) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl>()->setActuatedComponentUnique(modelObject);
  }

  bool ExternalInterfaceFunctionalMockupUnitImportToActuator::setActuatedComponentType(const std::string& actuatedComponentType) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl>()->setActuatedComponentType(actuatedComponentType);
  }

  bool ExternalInterfaceFunctionalMockupUnitImportToActuator::setActuatedComponentControlType(const std::string& actuatedComponentControlType) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl>()->setActuatedComponentControlType(
      actuatedComponentControlType);
  }

  bool ExternalInterfaceFunctionalMockupUnitImportToActuator::setFMUFile(const ExternalInterfaceFunctionalMockupUnitImport& fMUFile) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl>()->setFMUFile(fMUFile);
  }

  bool ExternalInterfaceFunctionalMockupUnitImportToActuator::setFMUInstanceName(const std::string& fMUInstanceName) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl>()->setFMUInstanceName(fMUInstanceName);
  }

  bool ExternalInterfaceFunctionalMockupUnitImportToActuator::setFMUVariableName(const std::string& fMUVariableName) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl>()->setFMUVariableName(fMUVariableName);
  }

  bool ExternalInterfaceFunctionalMockupUnitImportToActuator::setInitialValue(double initialValue) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl>()->setInitialValue(initialValue);
  }

  /// @cond
  ExternalInterfaceFunctionalMockupUnitImportToActuator::ExternalInterfaceFunctionalMockupUnitImportToActuator(
    std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitImportToActuator_Impl> impl)
    : ModelObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
