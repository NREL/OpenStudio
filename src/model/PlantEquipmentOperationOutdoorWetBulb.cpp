/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "PlantEquipmentOperationOutdoorWetBulb.hpp"
#include "PlantEquipmentOperationOutdoorWetBulb_Impl.hpp"
#include <utilities/idd/OS_PlantEquipmentOperation_OutdoorWetBulb_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    PlantEquipmentOperationOutdoorWetBulb_Impl::PlantEquipmentOperationOutdoorWetBulb_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                           bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == PlantEquipmentOperationOutdoorWetBulb::iddObjectType());
    }

    PlantEquipmentOperationOutdoorWetBulb_Impl::PlantEquipmentOperationOutdoorWetBulb_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                           Model_Impl* model, bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == PlantEquipmentOperationOutdoorWetBulb::iddObjectType());
    }

    PlantEquipmentOperationOutdoorWetBulb_Impl::PlantEquipmentOperationOutdoorWetBulb_Impl(const PlantEquipmentOperationOutdoorWetBulb_Impl& other,
                                                                                           Model_Impl* model, bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& PlantEquipmentOperationOutdoorWetBulb_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType PlantEquipmentOperationOutdoorWetBulb_Impl::iddObjectType() const {
      return PlantEquipmentOperationOutdoorWetBulb::iddObjectType();
    }

    double PlantEquipmentOperationOutdoorWetBulb_Impl::maximumUpperLimit() const {
      return 70.0;
    }

    double PlantEquipmentOperationOutdoorWetBulb_Impl::minimumLowerLimit() const {
      return -70.0;
    }

  }  // namespace detail

  PlantEquipmentOperationOutdoorWetBulb::PlantEquipmentOperationOutdoorWetBulb(const Model& model)
    : PlantEquipmentOperationRangeBasedScheme(PlantEquipmentOperationOutdoorWetBulb::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::PlantEquipmentOperationOutdoorWetBulb_Impl>());
  }

  IddObjectType PlantEquipmentOperationOutdoorWetBulb::iddObjectType() {
    return {IddObjectType::OS_PlantEquipmentOperation_OutdoorWetBulb};
  }

  /// @cond
  PlantEquipmentOperationOutdoorWetBulb::PlantEquipmentOperationOutdoorWetBulb(
    std::shared_ptr<detail::PlantEquipmentOperationOutdoorWetBulb_Impl> impl)
    : PlantEquipmentOperationRangeBasedScheme(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
