/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "PlantEquipmentOperationOutdoorDewpoint.hpp"
#include "PlantEquipmentOperationOutdoorDewpoint_Impl.hpp"
#include <utilities/idd/OS_PlantEquipmentOperation_OutdoorDewpoint_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    PlantEquipmentOperationOutdoorDewpoint_Impl::PlantEquipmentOperationOutdoorDewpoint_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                             bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == PlantEquipmentOperationOutdoorDewpoint::iddObjectType());
    }

    PlantEquipmentOperationOutdoorDewpoint_Impl::PlantEquipmentOperationOutdoorDewpoint_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == PlantEquipmentOperationOutdoorDewpoint::iddObjectType());
    }

    PlantEquipmentOperationOutdoorDewpoint_Impl::PlantEquipmentOperationOutdoorDewpoint_Impl(const PlantEquipmentOperationOutdoorDewpoint_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& PlantEquipmentOperationOutdoorDewpoint_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType PlantEquipmentOperationOutdoorDewpoint_Impl::iddObjectType() const {
      return PlantEquipmentOperationOutdoorDewpoint::iddObjectType();
    }

    double PlantEquipmentOperationOutdoorDewpoint_Impl::maximumUpperLimit() const {
      return 70.0;
    }

    double PlantEquipmentOperationOutdoorDewpoint_Impl::minimumLowerLimit() const {
      return -70.0;
    }

  }  // namespace detail

  PlantEquipmentOperationOutdoorDewpoint::PlantEquipmentOperationOutdoorDewpoint(const Model& model)
    : PlantEquipmentOperationRangeBasedScheme(PlantEquipmentOperationOutdoorDewpoint::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::PlantEquipmentOperationOutdoorDewpoint_Impl>());
  }

  IddObjectType PlantEquipmentOperationOutdoorDewpoint::iddObjectType() {
    return {IddObjectType::OS_PlantEquipmentOperation_OutdoorDewpoint};
  }

  /// @cond
  PlantEquipmentOperationOutdoorDewpoint::PlantEquipmentOperationOutdoorDewpoint(
    std::shared_ptr<detail::PlantEquipmentOperationOutdoorDewpoint_Impl> impl)
    : PlantEquipmentOperationRangeBasedScheme(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
