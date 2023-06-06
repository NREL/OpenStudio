/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ExternalInterfaceFunctionalMockupUnitExportToVariable.hpp"
#include "ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_FunctionalMockupUnitExport_To_Variable_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl::ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl(const IdfObject& idfObject,
                                                                                                                           Model_Impl* model,
                                                                                                                           bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ExternalInterfaceFunctionalMockupUnitExportToVariable::iddObjectType());
    }

    ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl::ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ExternalInterfaceFunctionalMockupUnitExportToVariable::iddObjectType());
    }

    ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl::ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl(
      const ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl::iddObjectType() const {
      return ExternalInterfaceFunctionalMockupUnitExportToVariable::iddObjectType();
    }

    std::string ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl::fMUVariableName() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_FunctionalMockupUnitExport_To_VariableFields::FMUVariableName, true);
      if (value) {
        return value.get();
      }
      return "";
    }

    double ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl::initialValue() const {
      boost::optional<double> value = getDouble(OS_ExternalInterface_FunctionalMockupUnitExport_To_VariableFields::InitialValue, true);
      if (value) {
        return value.get();
      }
      return -9999;
    }

    bool ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl::setFMUVariableName(const std::string& fMUVariableName) {
      bool result = setString(OS_ExternalInterface_FunctionalMockupUnitExport_To_VariableFields::FMUVariableName, fMUVariableName);
      OS_ASSERT(result);
      return result;
    }

    bool ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl::setInitialValue(double initialValue) {
      bool result = setDouble(OS_ExternalInterface_FunctionalMockupUnitExport_To_VariableFields::InitialValue, initialValue);
      OS_ASSERT(result);
      return result;
    }

  }  // namespace detail

  ExternalInterfaceFunctionalMockupUnitExportToVariable::ExternalInterfaceFunctionalMockupUnitExportToVariable(const Model& model,
                                                                                                               const std::string& fMUVariableName,
                                                                                                               double initialValue)
    : ModelObject(ExternalInterfaceFunctionalMockupUnitExportToVariable::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl>());

    setFMUVariableName(fMUVariableName);
    setInitialValue(initialValue);
  }

  IddObjectType ExternalInterfaceFunctionalMockupUnitExportToVariable::iddObjectType() {
    return {IddObjectType::OS_ExternalInterface_FunctionalMockupUnitExport_To_Variable};
  }

  std::string ExternalInterfaceFunctionalMockupUnitExportToVariable::fMUVariableName() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl>()->fMUVariableName();
  }

  double ExternalInterfaceFunctionalMockupUnitExportToVariable::initialValue() const {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl>()->initialValue();
  }

  bool ExternalInterfaceFunctionalMockupUnitExportToVariable::setFMUVariableName(const std::string& fMUVariableName) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl>()->setFMUVariableName(fMUVariableName);
  }

  bool ExternalInterfaceFunctionalMockupUnitExportToVariable::setInitialValue(double initialValue) {
    return getImpl<detail::ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl>()->setInitialValue(initialValue);
  }

  /// @cond
  ExternalInterfaceFunctionalMockupUnitExportToVariable::ExternalInterfaceFunctionalMockupUnitExportToVariable(
    std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitExportToVariable_Impl> impl)
    : ModelObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
