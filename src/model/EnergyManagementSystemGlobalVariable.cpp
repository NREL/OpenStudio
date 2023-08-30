/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "EnergyManagementSystemGlobalVariable.hpp"
#include "EnergyManagementSystemGlobalVariable_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_GlobalVariable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include "ModelExtensibleGroup.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    EnergyManagementSystemGlobalVariable_Impl::EnergyManagementSystemGlobalVariable_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                         bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemGlobalVariable::iddObjectType());
    }

    EnergyManagementSystemGlobalVariable_Impl::EnergyManagementSystemGlobalVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                         Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == EnergyManagementSystemGlobalVariable::iddObjectType());
    }

    EnergyManagementSystemGlobalVariable_Impl::EnergyManagementSystemGlobalVariable_Impl(const EnergyManagementSystemGlobalVariable_Impl& other,
                                                                                         Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& EnergyManagementSystemGlobalVariable_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType EnergyManagementSystemGlobalVariable_Impl::iddObjectType() const {
      return EnergyManagementSystemGlobalVariable::iddObjectType();
    }

    bool EnergyManagementSystemGlobalVariable_Impl::exportToBCVTB() const {
      boost::optional<std::string> value = getString(OS_EnergyManagementSystem_GlobalVariableFields::ExportToBCVTB, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "True");
    }

    bool EnergyManagementSystemGlobalVariable_Impl::isExportToBCVTBDefaulted() const {
      return isEmpty(OS_EnergyManagementSystem_GlobalVariableFields::ExportToBCVTB);
    }

    bool EnergyManagementSystemGlobalVariable_Impl::setExportToBCVTB(bool exportToBCVTB) {
      bool result = false;
      if (exportToBCVTB) {
        result = setString(OS_EnergyManagementSystem_GlobalVariableFields::ExportToBCVTB, "True");
      } else {
        result = setString(OS_EnergyManagementSystem_GlobalVariableFields::ExportToBCVTB, "False");
      }
      OS_ASSERT(result);
      return result;
    }

    void EnergyManagementSystemGlobalVariable_Impl::resetExportToBCVTB() {
      bool result = setString(OS_EnergyManagementSystem_GlobalVariableFields::ExportToBCVTB, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  EnergyManagementSystemGlobalVariable::EnergyManagementSystemGlobalVariable(const Model& model, const std::string& variableName)
    : ModelObject(EnergyManagementSystemGlobalVariable::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::EnergyManagementSystemGlobalVariable_Impl>());
    bool ok = getImpl<detail::EnergyManagementSystemGlobalVariable_Impl>()->setName(variableName).has_value();
    if ((!ok) || (variableName != this->nameString())) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s Name to " << variableName << ".");
    }
  }

  IddObjectType EnergyManagementSystemGlobalVariable::iddObjectType() {
    return {IddObjectType::OS_EnergyManagementSystem_GlobalVariable};
  }

  bool EnergyManagementSystemGlobalVariable::exportToBCVTB() const {
    return getImpl<detail::EnergyManagementSystemGlobalVariable_Impl>()->exportToBCVTB();
  }

  bool EnergyManagementSystemGlobalVariable::isExportToBCVTBDefaulted() const {
    return getImpl<detail::EnergyManagementSystemGlobalVariable_Impl>()->isExportToBCVTBDefaulted();
  }

  bool EnergyManagementSystemGlobalVariable::setExportToBCVTB(bool exportToBCVTB) {
    return getImpl<detail::EnergyManagementSystemGlobalVariable_Impl>()->setExportToBCVTB(exportToBCVTB);
  }

  void EnergyManagementSystemGlobalVariable::resetExportToBCVTB() {
    getImpl<detail::EnergyManagementSystemGlobalVariable_Impl>()->resetExportToBCVTB();
  }

  /// @cond
  EnergyManagementSystemGlobalVariable::EnergyManagementSystemGlobalVariable(std::shared_ptr<detail::EnergyManagementSystemGlobalVariable_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
