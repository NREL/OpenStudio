/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "EnergyManagementSystemInternalVariable.hpp"
#include "EnergyManagementSystemInternalVariable_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_InternalVariable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    EnergyManagementSystemInternalVariable_Impl::EnergyManagementSystemInternalVariable_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                             bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemInternalVariable::iddObjectType());
    }

    EnergyManagementSystemInternalVariable_Impl::EnergyManagementSystemInternalVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == EnergyManagementSystemInternalVariable::iddObjectType());
    }

    EnergyManagementSystemInternalVariable_Impl::EnergyManagementSystemInternalVariable_Impl(const EnergyManagementSystemInternalVariable_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& EnergyManagementSystemInternalVariable_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType EnergyManagementSystemInternalVariable_Impl::iddObjectType() const {
      return EnergyManagementSystemInternalVariable::iddObjectType();
    }

    std::string EnergyManagementSystemInternalVariable_Impl::internalDataIndexKeyName() const {
      OptionalString result = getString(OS_EnergyManagementSystem_InternalVariableFields::InternalDataIndexKeyName, true);
      if (result) {
        return result.get();
      }
      return "";
    }

    std::string EnergyManagementSystemInternalVariable_Impl::internalDataType() const {
      boost::optional<std::string> value = getString(OS_EnergyManagementSystem_InternalVariableFields::InternalDataType, true);
      return value.get();
    }

    bool EnergyManagementSystemInternalVariable_Impl::setInternalDataIndexKeyName(const std::string& internalDataIndexKeyName) {
      bool result = setString(OS_EnergyManagementSystem_InternalVariableFields::InternalDataIndexKeyName, internalDataIndexKeyName);
      return result;
    }

    void EnergyManagementSystemInternalVariable_Impl::resetInternalDataIndexKeyName() {
      bool result = setString(OS_EnergyManagementSystem_InternalVariableFields::InternalDataIndexKeyName, "");
      OS_ASSERT(result);
    }

    bool EnergyManagementSystemInternalVariable_Impl::setInternalDataType(const std::string& internalDataType) {
      bool result = setString(OS_EnergyManagementSystem_InternalVariableFields::InternalDataType, internalDataType);
      return result;
    }

  }  // namespace detail

  EnergyManagementSystemInternalVariable::EnergyManagementSystemInternalVariable(const Model& model, const std::string& internalDataType)
    : ModelObject(EnergyManagementSystemInternalVariable::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::EnergyManagementSystemInternalVariable_Impl>());
    bool ok = setInternalDataType(internalDataType);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s internalDataType to " << internalDataType << ".");
    }
  }

  EnergyManagementSystemInternalVariable::EnergyManagementSystemInternalVariable(const Model& model)
    : ModelObject(EnergyManagementSystemInternalVariable::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::EnergyManagementSystemInternalVariable_Impl>());
    //TODO setInternalDataType to valid EIO default value
  }

  IddObjectType EnergyManagementSystemInternalVariable::iddObjectType() {
    return {IddObjectType::OS_EnergyManagementSystem_InternalVariable};
  }

  std::string EnergyManagementSystemInternalVariable::internalDataIndexKeyName() const {
    return getImpl<detail::EnergyManagementSystemInternalVariable_Impl>()->internalDataIndexKeyName();
  }

  std::string EnergyManagementSystemInternalVariable::internalDataType() const {
    return getImpl<detail::EnergyManagementSystemInternalVariable_Impl>()->internalDataType();
  }

  bool EnergyManagementSystemInternalVariable::setInternalDataIndexKeyName(const std::string& internalDataIndexKeyName) {
    return getImpl<detail::EnergyManagementSystemInternalVariable_Impl>()->setInternalDataIndexKeyName(internalDataIndexKeyName);
  }

  void EnergyManagementSystemInternalVariable::resetInternalDataIndexKeyName() {
    getImpl<detail::EnergyManagementSystemInternalVariable_Impl>()->resetInternalDataIndexKeyName();
  }

  bool EnergyManagementSystemInternalVariable::setInternalDataType(const std::string& internalDataType) {
    return getImpl<detail::EnergyManagementSystemInternalVariable_Impl>()->setInternalDataType(internalDataType);
  }

  /// @cond
  EnergyManagementSystemInternalVariable::EnergyManagementSystemInternalVariable(
    std::shared_ptr<detail::EnergyManagementSystemInternalVariable_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
