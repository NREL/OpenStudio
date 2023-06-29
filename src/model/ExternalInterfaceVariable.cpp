/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ExternalInterfaceVariable.hpp"
#include "ExternalInterfaceVariable_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ExternalInterface_Variable_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ExternalInterfaceVariable_Impl::ExternalInterfaceVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ExternalInterfaceVariable::iddObjectType());
    }

    ExternalInterfaceVariable_Impl::ExternalInterfaceVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                   bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ExternalInterfaceVariable::iddObjectType());
    }

    ExternalInterfaceVariable_Impl::ExternalInterfaceVariable_Impl(const ExternalInterfaceVariable_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& ExternalInterfaceVariable_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType ExternalInterfaceVariable_Impl::iddObjectType() const {
      return ExternalInterfaceVariable::iddObjectType();
    }

    double ExternalInterfaceVariable_Impl::initialValue() const {
      boost::optional<double> value = getDouble(OS_ExternalInterface_VariableFields::InitialValue, true);
      if (value) {
        return value.get();
      }
      return -9999;
    }

    bool ExternalInterfaceVariable_Impl::setInitialValue(double initialValue) {
      bool result = setDouble(OS_ExternalInterface_VariableFields::InitialValue, initialValue);
      OS_ASSERT(result);
      return result;
    }

    bool ExternalInterfaceVariable_Impl::exportToBCVTB() const {
      boost::optional<std::string> value = getString(OS_ExternalInterface_VariableFields::ExportToBCVTB, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "True");
    }

    bool ExternalInterfaceVariable_Impl::isExportToBCVTBDefaulted() const {
      return isEmpty(OS_ExternalInterface_VariableFields::ExportToBCVTB);
    }

    bool ExternalInterfaceVariable_Impl::setExportToBCVTB(bool exportToBCVTB) {
      bool result = false;
      if (exportToBCVTB) {
        result = setString(OS_ExternalInterface_VariableFields::ExportToBCVTB, "True");
      } else {
        result = setString(OS_ExternalInterface_VariableFields::ExportToBCVTB, "False");
      }
      OS_ASSERT(result);
      return result;
    }

    void ExternalInterfaceVariable_Impl::resetExportToBCVTB() {
      bool result = setString(OS_ExternalInterface_VariableFields::ExportToBCVTB, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  ExternalInterfaceVariable::ExternalInterfaceVariable(const Model& model, const std::string& variableName, double initialValue)
    : ModelObject(ExternalInterfaceVariable::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ExternalInterfaceVariable_Impl>());
    bool ok = getImpl<detail::ExternalInterfaceVariable_Impl>()->setName(variableName).has_value();
    if ((!ok) || (variableName != this->nameString())) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s Name to " << variableName << ".");
    }
    setInitialValue(initialValue);
  }

  IddObjectType ExternalInterfaceVariable::iddObjectType() {
    return {IddObjectType::OS_ExternalInterface_Variable};
  }

  double ExternalInterfaceVariable::initialValue() const {
    return getImpl<detail::ExternalInterfaceVariable_Impl>()->initialValue();
  }

  bool ExternalInterfaceVariable::setInitialValue(double initialValue) {
    return getImpl<detail::ExternalInterfaceVariable_Impl>()->setInitialValue(initialValue);
  }

  bool ExternalInterfaceVariable::exportToBCVTB() const {
    return getImpl<detail::ExternalInterfaceVariable_Impl>()->exportToBCVTB();
  }

  bool ExternalInterfaceVariable::isExportToBCVTBDefaulted() const {
    return getImpl<detail::ExternalInterfaceVariable_Impl>()->isExportToBCVTBDefaulted();
  }

  bool ExternalInterfaceVariable::setExportToBCVTB(bool exportToBCVTB) {
    return getImpl<detail::ExternalInterfaceVariable_Impl>()->setExportToBCVTB(exportToBCVTB);
  }

  void ExternalInterfaceVariable::resetExportToBCVTB() {
    getImpl<detail::ExternalInterfaceVariable_Impl>()->resetExportToBCVTB();
  }

  /// @cond
  ExternalInterfaceVariable::ExternalInterfaceVariable(std::shared_ptr<detail::ExternalInterfaceVariable_Impl> impl) : ModelObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
