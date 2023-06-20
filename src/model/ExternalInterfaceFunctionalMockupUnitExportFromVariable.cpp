/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ExternalInterfaceFunctionalMockupUnitExportFromVariable.hpp"
#include "ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitExport_From_Variable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ExternalInterfaceFunctionalMockupUnitExportFromVariable::iddObjectType());
    }

    ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ExternalInterfaceFunctionalMockupUnitExportFromVariable::iddObjectType());
    }

    ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl(
      const ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl::iddObjectType() const {
      return ExternalInterfaceFunctionalMockupUnitExportFromVariable::iddObjectType();
    }

    std::string ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl::outputVariableIndexKeyName() const {
      boost::optional<std::string> value =
        getString(OS_ExternalInterface_FunctionalMockupUnitExport_From_VariableFields::Output_VariableIndexKeyName, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    std::string ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl::outputVariableName() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitExport_From_VariableFields::Output_VariableName, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    std::string ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl::fMUVariableName() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitExport_From_VariableFields::FMUVariableName, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    bool ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl::setOutputVariableIndexKeyName(const std::string& outputVariableIndexKeyName) {
      bool result =
        setString(OS_ExternalInterface_FunctionalMockupUnitExport_From_VariableFields::Output_VariableIndexKeyName, outputVariableIndexKeyName);
      OS_ASSERT(result);
      return result;
    }

    bool ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl::setOutputVariableName(const std::string& outputVariableName) {
      bool result = setString(OS_ExternalInterface_FunctionalMockupUnitExport_From_VariableFields::Output_VariableName, outputVariableName);
      return result;
    }

    bool ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl::setFMUVariableName(const std::string& fMUVariableName) {
      bool result = setString(OS_ExternalInterface_FunctionalMockupUnitExport_From_VariableFields::FMUVariableName, fMUVariableName);
      OS_ASSERT(result);
      return result;
    }

  }  // namespace detail

  ExternalInterfaceFunctionalMockupUnitExportFromVariable::ExternalInterfaceFunctionalMockupUnitExportFromVariable(
    const Model& model, const std::string& outputVariableIndexKeyName, const std::string& outputVariableName, const std::string& fMUVariableName)
    : ModelObject(ExternalInterfaceFunctionalMockupUnitExportFromVariable::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl>());

    setOutputVariableIndexKeyName(outputVariableIndexKeyName);
    bool ok = setOutputVariableName(outputVariableName);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s OutputVariableName to " << outputVariableName << ".");
    }
    setFMUVariableName(fMUVariableName);
  }

  IddObjectType ExternalInterfaceFunctionalMockupUnitExportFromVariable::iddObjectType() {
    return {IddObjectType::OS_ExternalInterface_FunctionalMockupUnitExport_From_Variable};
  }

  std::string ExternalInterfaceFunctionalMockupUnitExportFromVariable::outputVariableIndexKeyName() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl>()->outputVariableIndexKeyName();
  }

  std::string ExternalInterfaceFunctionalMockupUnitExportFromVariable::outputVariableName() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl>()->outputVariableName();
  }

  std::string ExternalInterfaceFunctionalMockupUnitExportFromVariable::fMUVariableName() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl>()->fMUVariableName();
  }

  bool ExternalInterfaceFunctionalMockupUnitExportFromVariable::setOutputVariableIndexKeyName(const std::string& outputVariableIndexKeyName) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl>()->setOutputVariableIndexKeyName(outputVariableIndexKeyName);
  }

  bool ExternalInterfaceFunctionalMockupUnitExportFromVariable::setOutputVariableName(const std::string& outputVariableName) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl>()->setOutputVariableName(outputVariableName);
  }

  bool ExternalInterfaceFunctionalMockupUnitExportFromVariable::setFMUVariableName(const std::string& fMUVariableName) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl>()->setFMUVariableName(fMUVariableName);
  }

  /// @cond
  ExternalInterfaceFunctionalMockupUnitExportFromVariable::ExternalInterfaceFunctionalMockupUnitExportFromVariable(
    std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitExportFromVariable_Impl> impl)
    : ModelObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
