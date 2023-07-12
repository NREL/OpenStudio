/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "EnergyManagementSystemConstructionIndexVariable.hpp"
#include "EnergyManagementSystemConstructionIndexVariable_Impl.hpp"

#include "Construction.hpp"
#include "Construction_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_ConstructionIndexVariable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    EnergyManagementSystemConstructionIndexVariable_Impl::EnergyManagementSystemConstructionIndexVariable_Impl(const IdfObject& idfObject,
                                                                                                               Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemConstructionIndexVariable::iddObjectType());
    }

    EnergyManagementSystemConstructionIndexVariable_Impl::EnergyManagementSystemConstructionIndexVariable_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == EnergyManagementSystemConstructionIndexVariable::iddObjectType());
    }

    EnergyManagementSystemConstructionIndexVariable_Impl::EnergyManagementSystemConstructionIndexVariable_Impl(
      const EnergyManagementSystemConstructionIndexVariable_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& EnergyManagementSystemConstructionIndexVariable_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType EnergyManagementSystemConstructionIndexVariable_Impl::iddObjectType() const {
      return EnergyManagementSystemConstructionIndexVariable::iddObjectType();
    }

    ModelObject EnergyManagementSystemConstructionIndexVariable_Impl::constructionObject() const {
      boost::optional<ModelObject> value = optionalConstructionObject();
      if (!value) {
        LOG(Info, " does not have an Construction Object attached.");
      }
      return value.get();
    }

    bool EnergyManagementSystemConstructionIndexVariable_Impl::setConstructionObject(const ModelObject& construction) {
      bool result = setPointer(OS_EnergyManagementSystem_ConstructionIndexVariableFields::ConstructionObjectName, construction.handle());
      return result;
    }

    boost::optional<ModelObject> EnergyManagementSystemConstructionIndexVariable_Impl::optionalConstructionObject() const {
      return getObject<ModelObject>().getModelObjectTarget<ModelObject>(
        OS_EnergyManagementSystem_ConstructionIndexVariableFields::ConstructionObjectName);
    }

  }  // namespace detail

  EnergyManagementSystemConstructionIndexVariable::EnergyManagementSystemConstructionIndexVariable(const Model& model,
                                                                                                   const Construction& construction)
    : ModelObject(EnergyManagementSystemConstructionIndexVariable::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::EnergyManagementSystemConstructionIndexVariable_Impl>());
    bool ok = setConstructionObject(construction);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s Construction to " << construction.briefDescription() << ".");
    }
  }

  EnergyManagementSystemConstructionIndexVariable::EnergyManagementSystemConstructionIndexVariable(const Model& model)
    : ModelObject(EnergyManagementSystemConstructionIndexVariable::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::EnergyManagementSystemConstructionIndexVariable_Impl>());
    Construction construction = Construction(model);
    construction.setName("Generic Construction");
    setConstructionObject(construction);
  }

  IddObjectType EnergyManagementSystemConstructionIndexVariable::iddObjectType() {
    return {IddObjectType::OS_EnergyManagementSystem_ConstructionIndexVariable};
  }

  ModelObject EnergyManagementSystemConstructionIndexVariable::constructionObject() const {
    return getImpl<detail::EnergyManagementSystemConstructionIndexVariable_Impl>()->constructionObject();
  }

  bool EnergyManagementSystemConstructionIndexVariable::setConstructionObject(const ModelObject& construction) {
    return getImpl<detail::EnergyManagementSystemConstructionIndexVariable_Impl>()->setConstructionObject(construction);
  }

  /// @cond
  EnergyManagementSystemConstructionIndexVariable::EnergyManagementSystemConstructionIndexVariable(
    std::shared_ptr<detail::EnergyManagementSystemConstructionIndexVariable_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
