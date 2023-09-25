/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "PlantEquipmentOperationOutdoorDryBulb.hpp"
#include "PlantEquipmentOperationOutdoorDryBulb_Impl.hpp"
#include <utilities/idd/OS_PlantEquipmentOperation_OutdoorDryBulb_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    PlantEquipmentOperationOutdoorDryBulb_Impl::PlantEquipmentOperationOutdoorDryBulb_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                           bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == PlantEquipmentOperationOutdoorDryBulb::iddObjectType());
    }

    PlantEquipmentOperationOutdoorDryBulb_Impl::PlantEquipmentOperationOutdoorDryBulb_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                           Model_Impl* model, bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == PlantEquipmentOperationOutdoorDryBulb::iddObjectType());
    }

    PlantEquipmentOperationOutdoorDryBulb_Impl::PlantEquipmentOperationOutdoorDryBulb_Impl(const PlantEquipmentOperationOutdoorDryBulb_Impl& other,
                                                                                           Model_Impl* model, bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& PlantEquipmentOperationOutdoorDryBulb_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType PlantEquipmentOperationOutdoorDryBulb_Impl::iddObjectType() const {
      return PlantEquipmentOperationOutdoorDryBulb::iddObjectType();
    }

    double PlantEquipmentOperationOutdoorDryBulb_Impl::maximumUpperLimit() const {
      return 70.0;
    }

    double PlantEquipmentOperationOutdoorDryBulb_Impl::minimumLowerLimit() const {
      return -70.0;
    }

  }  // namespace detail

  PlantEquipmentOperationOutdoorDryBulb::PlantEquipmentOperationOutdoorDryBulb(const Model& model)
    : PlantEquipmentOperationRangeBasedScheme(PlantEquipmentOperationOutdoorDryBulb::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::PlantEquipmentOperationOutdoorDryBulb_Impl>());
  }

  IddObjectType PlantEquipmentOperationOutdoorDryBulb::iddObjectType() {
    return {IddObjectType::OS_PlantEquipmentOperation_OutdoorDryBulb};
  }

  /// @cond
  PlantEquipmentOperationOutdoorDryBulb::PlantEquipmentOperationOutdoorDryBulb(
    std::shared_ptr<detail::PlantEquipmentOperationOutdoorDryBulb_Impl> impl)
    : PlantEquipmentOperationRangeBasedScheme(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
