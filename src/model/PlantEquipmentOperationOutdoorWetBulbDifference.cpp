/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "PlantEquipmentOperationOutdoorWetBulbDifference.hpp"
#include "PlantEquipmentOperationOutdoorWetBulbDifference_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include <utilities/idd/OS_PlantEquipmentOperation_OutdoorWetBulbDifference_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    PlantEquipmentOperationOutdoorWetBulbDifference_Impl::PlantEquipmentOperationOutdoorWetBulbDifference_Impl(const IdfObject& idfObject,
                                                                                                               Model_Impl* model, bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == PlantEquipmentOperationOutdoorWetBulbDifference::iddObjectType());
    }

    PlantEquipmentOperationOutdoorWetBulbDifference_Impl::PlantEquipmentOperationOutdoorWetBulbDifference_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == PlantEquipmentOperationOutdoorWetBulbDifference::iddObjectType());
    }

    PlantEquipmentOperationOutdoorWetBulbDifference_Impl::PlantEquipmentOperationOutdoorWetBulbDifference_Impl(
      const PlantEquipmentOperationOutdoorWetBulbDifference_Impl& other, Model_Impl* model, bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& PlantEquipmentOperationOutdoorWetBulbDifference_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType PlantEquipmentOperationOutdoorWetBulbDifference_Impl::iddObjectType() const {
      return PlantEquipmentOperationOutdoorWetBulbDifference::iddObjectType();
    }

    boost::optional<Node> PlantEquipmentOperationOutdoorWetBulbDifference_Impl::referenceTemperatureNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(
        OS_PlantEquipmentOperation_OutdoorWetBulbDifferenceFields::ReferenceTemperatureNodeName);
    }

    bool PlantEquipmentOperationOutdoorWetBulbDifference_Impl::setReferenceTemperatureNode(const boost::optional<Node>& node) {
      bool result(false);
      if (node) {
        result = setPointer(OS_PlantEquipmentOperation_OutdoorWetBulbDifferenceFields::ReferenceTemperatureNodeName, node.get().handle());
      } else {
        resetReferenceTemperatureNode();
        result = true;
      }
      return result;
    }

    void PlantEquipmentOperationOutdoorWetBulbDifference_Impl::resetReferenceTemperatureNode() {
      bool result = setString(OS_PlantEquipmentOperation_OutdoorWetBulbDifferenceFields::ReferenceTemperatureNodeName, "");
      OS_ASSERT(result);
    }

    double PlantEquipmentOperationOutdoorWetBulbDifference_Impl::maximumUpperLimit() const {
      return 100.0;
    }

    double PlantEquipmentOperationOutdoorWetBulbDifference_Impl::minimumLowerLimit() const {
      return -50.0;
    }

  }  // namespace detail

  PlantEquipmentOperationOutdoorWetBulbDifference::PlantEquipmentOperationOutdoorWetBulbDifference(const Model& model)
    : PlantEquipmentOperationRangeBasedScheme(PlantEquipmentOperationOutdoorWetBulbDifference::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::PlantEquipmentOperationOutdoorWetBulbDifference_Impl>());
  }

  IddObjectType PlantEquipmentOperationOutdoorWetBulbDifference::iddObjectType() {
    return {IddObjectType::OS_PlantEquipmentOperation_OutdoorWetBulbDifference};
  }

  boost::optional<Node> PlantEquipmentOperationOutdoorWetBulbDifference::referenceTemperatureNode() const {
    return getImpl<detail::PlantEquipmentOperationOutdoorWetBulbDifference_Impl>()->referenceTemperatureNode();
  }

  bool PlantEquipmentOperationOutdoorWetBulbDifference::setReferenceTemperatureNode(const Node& node) {
    return getImpl<detail::PlantEquipmentOperationOutdoorWetBulbDifference_Impl>()->setReferenceTemperatureNode(node);
  }

  void PlantEquipmentOperationOutdoorWetBulbDifference::resetReferenceTemperatureNode() {
    getImpl<detail::PlantEquipmentOperationOutdoorWetBulbDifference_Impl>()->resetReferenceTemperatureNode();
  }

  /// @cond
  PlantEquipmentOperationOutdoorWetBulbDifference::PlantEquipmentOperationOutdoorWetBulbDifference(
    std::shared_ptr<detail::PlantEquipmentOperationOutdoorWetBulbDifference_Impl> impl)
    : PlantEquipmentOperationRangeBasedScheme(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
