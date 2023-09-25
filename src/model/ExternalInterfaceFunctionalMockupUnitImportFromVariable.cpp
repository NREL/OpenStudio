/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ExternalInterfaceFunctionalMockupUnitImportFromVariable.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitImport_From_Variable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ExternalInterfaceFunctionalMockupUnitImportFromVariable::iddObjectType());
    }

    ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ExternalInterfaceFunctionalMockupUnitImportFromVariable::iddObjectType());
    }

    ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl(
      const ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::iddObjectType() const {
      return ExternalInterfaceFunctionalMockupUnitImportFromVariable::iddObjectType();
    }

    std::string ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::outputVariableIndexKeyName() const {
      boost::optional<std::string> value =
        getString(OS_ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::Output_VariableIndexKeyName, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    std::string ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::outputVariableName() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::Output_VariableName, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    ExternalInterfaceFunctionalMockupUnitImport ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::fMUFile() const {
      return this->getTarget(OS_ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::FMUFileName)
        ->cast<ExternalInterfaceFunctionalMockupUnitImport>();
    }

    std::string ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::fMUInstanceName() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::FMUInstanceName, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    std::string ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::fMUVariableName() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::FMUVariableName, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    bool ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::setOutputVariableIndexKeyName(const std::string& outputVariableIndexKeyName) {
      bool result =
        setString(OS_ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::Output_VariableIndexKeyName, outputVariableIndexKeyName);
      OS_ASSERT(result);
      return result;
    }

    bool ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::setOutputVariableName(const std::string& outputVariableName) {
      bool result = setString(OS_ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::Output_VariableName, outputVariableName);
      return result;
    }

    bool ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::setFMUFile(const ExternalInterfaceFunctionalMockupUnitImport& fMUFile) {
      bool result = setPointer(OS_ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::FMUFileName, fMUFile.handle());
      //TODO in forward translator set the field to the .fMUFileName()
      return result;
    }

    bool ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::setFMUInstanceName(const std::string& fMUInstanceName) {
      bool result = setString(OS_ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::FMUInstanceName, fMUInstanceName);
      OS_ASSERT(result);
      return result;
    }

    bool ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl::setFMUVariableName(const std::string& fMUVariableName) {
      bool result = setString(OS_ExternalInterface_FunctionalMockupUnitImport_From_VariableFields::FMUVariableName, fMUVariableName);
      OS_ASSERT(result);
      return result;
    }

  }  // namespace detail

  ExternalInterfaceFunctionalMockupUnitImportFromVariable::ExternalInterfaceFunctionalMockupUnitImportFromVariable(
    const Model& model, const std::string& outputVariableIndexKeyName, const std::string& outputVariableName,
    const ExternalInterfaceFunctionalMockupUnitImport& fMUFile, const std::string& fMUInstanceName, const std::string& fMUVariableName)
    : ModelObject(ExternalInterfaceFunctionalMockupUnitImportFromVariable::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl>());

    setOutputVariableIndexKeyName(outputVariableIndexKeyName);
    bool ok = setOutputVariableName(outputVariableName);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s OutputVariableName to " << outputVariableName << ".");
    }
    ok = setFMUFile(fMUFile);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s FMUFileName to " << fMUFile.fMUFileName() << ".");
    }
    setFMUInstanceName(fMUInstanceName);
    setFMUVariableName(fMUVariableName);
  }

  IddObjectType ExternalInterfaceFunctionalMockupUnitImportFromVariable::iddObjectType() {
    return {IddObjectType::OS_ExternalInterface_FunctionalMockupUnitImport_From_Variable};
  }

  std::string ExternalInterfaceFunctionalMockupUnitImportFromVariable::outputVariableIndexKeyName() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl>()->outputVariableIndexKeyName();
  }

  std::string ExternalInterfaceFunctionalMockupUnitImportFromVariable::outputVariableName() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl>()->outputVariableName();
  }

  ExternalInterfaceFunctionalMockupUnitImport ExternalInterfaceFunctionalMockupUnitImportFromVariable::fMUFile() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl>()->fMUFile();
  }

  std::string ExternalInterfaceFunctionalMockupUnitImportFromVariable::fMUInstanceName() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl>()->fMUInstanceName();
  }

  std::string ExternalInterfaceFunctionalMockupUnitImportFromVariable::fMUVariableName() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl>()->fMUVariableName();
  }

  bool ExternalInterfaceFunctionalMockupUnitImportFromVariable::setOutputVariableIndexKeyName(const std::string& outputVariableIndexKeyName) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl>()->setOutputVariableIndexKeyName(outputVariableIndexKeyName);
  }

  bool ExternalInterfaceFunctionalMockupUnitImportFromVariable::setOutputVariableName(const std::string& outputVariableName) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl>()->setOutputVariableName(outputVariableName);
  }

  bool ExternalInterfaceFunctionalMockupUnitImportFromVariable::setFMUFile(const ExternalInterfaceFunctionalMockupUnitImport& fMUFile) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl>()->setFMUFile(fMUFile);
  }

  bool ExternalInterfaceFunctionalMockupUnitImportFromVariable::setFMUInstanceName(const std::string& fMUInstanceName) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl>()->setFMUInstanceName(fMUInstanceName);
  }

  bool ExternalInterfaceFunctionalMockupUnitImportFromVariable::setFMUVariableName(const std::string& fMUVariableName) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl>()->setFMUVariableName(fMUVariableName);
  }

  /// @cond
  ExternalInterfaceFunctionalMockupUnitImportFromVariable::ExternalInterfaceFunctionalMockupUnitImportFromVariable(
    std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl> impl)
    : ModelObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
