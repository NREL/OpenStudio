/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "PlantEquipmentOperationOutdoorRelativeHumidity.hpp"
#include "PlantEquipmentOperationOutdoorRelativeHumidity_Impl.hpp"
#include <utilities/idd/OS_PlantEquipmentOperation_OutdoorRelativeHumidity_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    PlantEquipmentOperationOutdoorRelativeHumidity_Impl::PlantEquipmentOperationOutdoorRelativeHumidity_Impl(const IdfObject& idfObject,
                                                                                                             Model_Impl* model, bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == PlantEquipmentOperationOutdoorRelativeHumidity::iddObjectType());
    }

    PlantEquipmentOperationOutdoorRelativeHumidity_Impl::PlantEquipmentOperationOutdoorRelativeHumidity_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == PlantEquipmentOperationOutdoorRelativeHumidity::iddObjectType());
    }

    PlantEquipmentOperationOutdoorRelativeHumidity_Impl::PlantEquipmentOperationOutdoorRelativeHumidity_Impl(
      const PlantEquipmentOperationOutdoorRelativeHumidity_Impl& other, Model_Impl* model, bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& PlantEquipmentOperationOutdoorRelativeHumidity_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType PlantEquipmentOperationOutdoorRelativeHumidity_Impl::iddObjectType() const {
      return PlantEquipmentOperationOutdoorRelativeHumidity::iddObjectType();
    }

    double PlantEquipmentOperationOutdoorRelativeHumidity_Impl::maximumUpperLimit() const {
      return 100.0;
    }

    double PlantEquipmentOperationOutdoorRelativeHumidity_Impl::minimumLowerLimit() const {
      return 0.0;
    }

  }  // namespace detail

  PlantEquipmentOperationOutdoorRelativeHumidity::PlantEquipmentOperationOutdoorRelativeHumidity(const Model& model)
    : PlantEquipmentOperationRangeBasedScheme(PlantEquipmentOperationOutdoorRelativeHumidity::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::PlantEquipmentOperationOutdoorRelativeHumidity_Impl>());
  }

  IddObjectType PlantEquipmentOperationOutdoorRelativeHumidity::iddObjectType() {
    return {IddObjectType::OS_PlantEquipmentOperation_OutdoorRelativeHumidity};
  }

  /// @cond
  PlantEquipmentOperationOutdoorRelativeHumidity::PlantEquipmentOperationOutdoorRelativeHumidity(
    std::shared_ptr<detail::PlantEquipmentOperationOutdoorRelativeHumidity_Impl> impl)
    : PlantEquipmentOperationRangeBasedScheme(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
