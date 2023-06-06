/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "PlantEquipmentOperationCoolingLoad.hpp"
#include "PlantEquipmentOperationCoolingLoad_Impl.hpp"
#include <utilities/idd/OS_PlantEquipmentOperation_CoolingLoad_FieldEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    PlantEquipmentOperationCoolingLoad_Impl::PlantEquipmentOperationCoolingLoad_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == PlantEquipmentOperationCoolingLoad::iddObjectType());
    }

    PlantEquipmentOperationCoolingLoad_Impl::PlantEquipmentOperationCoolingLoad_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == PlantEquipmentOperationCoolingLoad::iddObjectType());
    }

    PlantEquipmentOperationCoolingLoad_Impl::PlantEquipmentOperationCoolingLoad_Impl(const PlantEquipmentOperationCoolingLoad_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& PlantEquipmentOperationCoolingLoad_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType PlantEquipmentOperationCoolingLoad_Impl::iddObjectType() const {
      return PlantEquipmentOperationCoolingLoad::iddObjectType();
    }

    double PlantEquipmentOperationCoolingLoad_Impl::maximumUpperLimit() const {
      return 1E9;
    }

    double PlantEquipmentOperationCoolingLoad_Impl::minimumLowerLimit() const {
      return 0.0;
    }

  }  // namespace detail

  PlantEquipmentOperationCoolingLoad::PlantEquipmentOperationCoolingLoad(const Model& model)
    : PlantEquipmentOperationRangeBasedScheme(PlantEquipmentOperationCoolingLoad::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::PlantEquipmentOperationCoolingLoad_Impl>());
  }

  IddObjectType PlantEquipmentOperationCoolingLoad::iddObjectType() {
    return {IddObjectType::OS_PlantEquipmentOperation_CoolingLoad};
  }

  /// @cond
  PlantEquipmentOperationCoolingLoad::PlantEquipmentOperationCoolingLoad(std::shared_ptr<detail::PlantEquipmentOperationCoolingLoad_Impl> impl)
    : PlantEquipmentOperationRangeBasedScheme(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
