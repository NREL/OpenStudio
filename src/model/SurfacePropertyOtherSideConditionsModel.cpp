/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SurfacePropertyOtherSideConditionsModel.hpp"
#include "SurfacePropertyOtherSideConditionsModel_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_SurfaceProperty_OtherSideConditionsModel_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SurfacePropertyOtherSideConditionsModel_Impl::SurfacePropertyOtherSideConditionsModel_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                               bool keepHandle)
      : ResourceObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SurfacePropertyOtherSideConditionsModel::iddObjectType());
    }

    SurfacePropertyOtherSideConditionsModel_Impl::SurfacePropertyOtherSideConditionsModel_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                               Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SurfacePropertyOtherSideConditionsModel::iddObjectType());
    }

    SurfacePropertyOtherSideConditionsModel_Impl::SurfacePropertyOtherSideConditionsModel_Impl(
      const SurfacePropertyOtherSideConditionsModel_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& SurfacePropertyOtherSideConditionsModel_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Surface Other Side Conditions Modeled Convection Air Temperature",
                                                   "Surface Other Side Conditions Modeled Convection Heat Transfer Coefficient",
                                                   "Surface Other Side Conditions Modeled Radiation Temperature",
                                                   "Surface Other Side Conditions Modeled Radiation Heat Transfer Coefficient"};
      return result;
    }

    IddObjectType SurfacePropertyOtherSideConditionsModel_Impl::iddObjectType() const {
      return SurfacePropertyOtherSideConditionsModel::iddObjectType();
    }

    std::string SurfacePropertyOtherSideConditionsModel_Impl::typeOfModeling() const {
      boost::optional<std::string> value = getString(OS_SurfaceProperty_OtherSideConditionsModelFields::TypeofModeling, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool SurfacePropertyOtherSideConditionsModel_Impl::isTypeOfModelingDefaulted() const {
      return isEmpty(OS_SurfaceProperty_OtherSideConditionsModelFields::TypeofModeling);
    }

    bool SurfacePropertyOtherSideConditionsModel_Impl::setTypeOfModeling(const std::string& typeOfModeling) {
      bool result = setString(OS_SurfaceProperty_OtherSideConditionsModelFields::TypeofModeling, typeOfModeling);
      return result;
    }

    void SurfacePropertyOtherSideConditionsModel_Impl::resetTypeOfModeling() {
      bool result = setString(OS_SurfaceProperty_OtherSideConditionsModelFields::TypeofModeling, "");
      OS_ASSERT(result);
    }

    std::vector<EMSActuatorNames> SurfacePropertyOtherSideConditionsModel_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"Other Side Boundary Conditions", "Convection Bulk Air Temperature"},
                                              {"Other Side Boundary Conditions", "Convection Heat Transfer Coefficient"},
                                              {"Other Side Boundary Conditions", "Radiation Effective Temperature"},
                                              {"Other Side Boundary Conditions", "Radiation Linear Heat Transfer Coefficient"}};
      return actuators;
    }

    std::vector<std::string> SurfacePropertyOtherSideConditionsModel_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types;
      return types;
    }
  }  // namespace detail

  SurfacePropertyOtherSideConditionsModel::SurfacePropertyOtherSideConditionsModel(const Model& model)
    : ResourceObject(SurfacePropertyOtherSideConditionsModel::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SurfacePropertyOtherSideConditionsModel_Impl>());
  }

  IddObjectType SurfacePropertyOtherSideConditionsModel::iddObjectType() {
    return {IddObjectType::OS_SurfaceProperty_OtherSideConditionsModel};
  }

  std::vector<std::string> SurfacePropertyOtherSideConditionsModel::typeOfModelingValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_SurfaceProperty_OtherSideConditionsModelFields::TypeofModeling);
  }

  std::string SurfacePropertyOtherSideConditionsModel::typeOfModeling() const {
    return getImpl<detail::SurfacePropertyOtherSideConditionsModel_Impl>()->typeOfModeling();
  }

  bool SurfacePropertyOtherSideConditionsModel::isTypeOfModelingDefaulted() const {
    return getImpl<detail::SurfacePropertyOtherSideConditionsModel_Impl>()->isTypeOfModelingDefaulted();
  }

  bool SurfacePropertyOtherSideConditionsModel::setTypeOfModeling(const std::string& typeOfModeling) {
    return getImpl<detail::SurfacePropertyOtherSideConditionsModel_Impl>()->setTypeOfModeling(typeOfModeling);
  }

  void SurfacePropertyOtherSideConditionsModel::resetTypeOfModeling() {
    getImpl<detail::SurfacePropertyOtherSideConditionsModel_Impl>()->resetTypeOfModeling();
  }

  /// @cond
  SurfacePropertyOtherSideConditionsModel::SurfacePropertyOtherSideConditionsModel(
    std::shared_ptr<detail::SurfacePropertyOtherSideConditionsModel_Impl> impl)
    : ResourceObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
