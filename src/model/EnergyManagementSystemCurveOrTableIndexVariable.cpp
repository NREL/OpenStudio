/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "EnergyManagementSystemCurveOrTableIndexVariable.hpp"
#include "EnergyManagementSystemCurveOrTableIndexVariable_Impl.hpp"
#include "Curve.hpp"
#include "CurveLinear.hpp"

// TODO: Check the following class names against object getters and setters.
//#include "AllCurves.hpp"
//#include "AllCurves_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_CurveOrTableIndexVariable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    EnergyManagementSystemCurveOrTableIndexVariable_Impl::EnergyManagementSystemCurveOrTableIndexVariable_Impl(const IdfObject& idfObject,
                                                                                                               Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemCurveOrTableIndexVariable::iddObjectType());
    }

    EnergyManagementSystemCurveOrTableIndexVariable_Impl::EnergyManagementSystemCurveOrTableIndexVariable_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == EnergyManagementSystemCurveOrTableIndexVariable::iddObjectType());
    }

    EnergyManagementSystemCurveOrTableIndexVariable_Impl::EnergyManagementSystemCurveOrTableIndexVariable_Impl(
      const EnergyManagementSystemCurveOrTableIndexVariable_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& EnergyManagementSystemCurveOrTableIndexVariable_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType EnergyManagementSystemCurveOrTableIndexVariable_Impl::iddObjectType() const {
      return EnergyManagementSystemCurveOrTableIndexVariable::iddObjectType();
    }

    ModelObject EnergyManagementSystemCurveOrTableIndexVariable_Impl::curveOrTableObject() const {
      boost::optional<ModelObject> value = optionalCurveOrTableObject();
      if (!value) {
        LOG(Info, " does not have an Curveor Table Object attached.");
      }
      return value.get();
    }

    bool EnergyManagementSystemCurveOrTableIndexVariable_Impl::setCurveOrTableObject(const Curve& allCurves) {
      bool result = setPointer(OS_EnergyManagementSystem_CurveOrTableIndexVariableFields::CurveorTableObjectName, allCurves.handle());
      return result;
    }

    boost::optional<ModelObject> EnergyManagementSystemCurveOrTableIndexVariable_Impl::optionalCurveOrTableObject() const {
      return getObject<ModelObject>().getModelObjectTarget<ModelObject>(
        OS_EnergyManagementSystem_CurveOrTableIndexVariableFields::CurveorTableObjectName);
    }

  }  // namespace detail

  EnergyManagementSystemCurveOrTableIndexVariable::EnergyManagementSystemCurveOrTableIndexVariable(const Model& model, const Curve& curve)
    : ModelObject(EnergyManagementSystemCurveOrTableIndexVariable::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::EnergyManagementSystemCurveOrTableIndexVariable_Impl>());
    bool ok = setCurveOrTableObject(curve);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s Curve to " << curve.briefDescription() << ".");
    }
  }

  EnergyManagementSystemCurveOrTableIndexVariable::EnergyManagementSystemCurveOrTableIndexVariable(const Model& model)
    : ModelObject(EnergyManagementSystemCurveOrTableIndexVariable::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::EnergyManagementSystemCurveOrTableIndexVariable_Impl>());
    CurveLinear curve = CurveLinear(model);
    curve.setName("Generic Curve");
    setCurveOrTableObject(curve);
  }

  IddObjectType EnergyManagementSystemCurveOrTableIndexVariable::iddObjectType() {
    return {IddObjectType::OS_EnergyManagementSystem_CurveOrTableIndexVariable};
  }

  ModelObject EnergyManagementSystemCurveOrTableIndexVariable::curveOrTableObject() const {
    return getImpl<detail::EnergyManagementSystemCurveOrTableIndexVariable_Impl>()->curveOrTableObject();
  }

  bool EnergyManagementSystemCurveOrTableIndexVariable::setCurveOrTableObject(const Curve& allCurves) {
    return getImpl<detail::EnergyManagementSystemCurveOrTableIndexVariable_Impl>()->setCurveOrTableObject(allCurves);
  }

  /// @cond
  EnergyManagementSystemCurveOrTableIndexVariable::EnergyManagementSystemCurveOrTableIndexVariable(
    std::shared_ptr<detail::EnergyManagementSystemCurveOrTableIndexVariable_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
