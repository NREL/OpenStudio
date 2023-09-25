/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "PlantEquipmentOperationHeatingLoad.hpp"
#include "PlantEquipmentOperationHeatingLoad_Impl.hpp"
#include <utilities/idd/OS_PlantEquipmentOperation_HeatingLoad_FieldEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    PlantEquipmentOperationHeatingLoad_Impl::PlantEquipmentOperationHeatingLoad_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == PlantEquipmentOperationHeatingLoad::iddObjectType());
    }

    PlantEquipmentOperationHeatingLoad_Impl::PlantEquipmentOperationHeatingLoad_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == PlantEquipmentOperationHeatingLoad::iddObjectType());
    }

    PlantEquipmentOperationHeatingLoad_Impl::PlantEquipmentOperationHeatingLoad_Impl(const PlantEquipmentOperationHeatingLoad_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& PlantEquipmentOperationHeatingLoad_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType PlantEquipmentOperationHeatingLoad_Impl::iddObjectType() const {
      return PlantEquipmentOperationHeatingLoad::iddObjectType();
    }

    double PlantEquipmentOperationHeatingLoad_Impl::maximumUpperLimit() const {
      return 1E9;
    }

    double PlantEquipmentOperationHeatingLoad_Impl::minimumLowerLimit() const {
      return 0.0;
    }

  }  // namespace detail

  PlantEquipmentOperationHeatingLoad::PlantEquipmentOperationHeatingLoad(const Model& model)
    : PlantEquipmentOperationRangeBasedScheme(PlantEquipmentOperationHeatingLoad::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::PlantEquipmentOperationHeatingLoad_Impl>());
  }

  IddObjectType PlantEquipmentOperationHeatingLoad::iddObjectType() {
    return {IddObjectType::OS_PlantEquipmentOperation_HeatingLoad};
  }

  /// @cond
  PlantEquipmentOperationHeatingLoad::PlantEquipmentOperationHeatingLoad(std::shared_ptr<detail::PlantEquipmentOperationHeatingLoad_Impl> impl)
    : PlantEquipmentOperationRangeBasedScheme(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
