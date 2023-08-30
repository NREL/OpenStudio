/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "PlantEquipmentOperationOutdoorDewpointDifference.hpp"
#include "PlantEquipmentOperationOutdoorDewpointDifference_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include <utilities/idd/OS_PlantEquipmentOperation_OutdoorDewpointDifference_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    PlantEquipmentOperationOutdoorDewpointDifference_Impl::PlantEquipmentOperationOutdoorDewpointDifference_Impl(const IdfObject& idfObject,
                                                                                                                 Model_Impl* model, bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == PlantEquipmentOperationOutdoorDewpointDifference::iddObjectType());
    }

    PlantEquipmentOperationOutdoorDewpointDifference_Impl::PlantEquipmentOperationOutdoorDewpointDifference_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == PlantEquipmentOperationOutdoorDewpointDifference::iddObjectType());
    }

    PlantEquipmentOperationOutdoorDewpointDifference_Impl::PlantEquipmentOperationOutdoorDewpointDifference_Impl(
      const PlantEquipmentOperationOutdoorDewpointDifference_Impl& other, Model_Impl* model, bool keepHandle)
      : PlantEquipmentOperationRangeBasedScheme_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& PlantEquipmentOperationOutdoorDewpointDifference_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType PlantEquipmentOperationOutdoorDewpointDifference_Impl::iddObjectType() const {
      return PlantEquipmentOperationOutdoorDewpointDifference::iddObjectType();
    }

    boost::optional<Node> PlantEquipmentOperationOutdoorDewpointDifference_Impl::referenceTemperatureNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Node>(
        OS_PlantEquipmentOperation_OutdoorDewpointDifferenceFields::ReferenceTemperatureNodeName);
    }

    bool PlantEquipmentOperationOutdoorDewpointDifference_Impl::setReferenceTemperatureNode(const boost::optional<Node>& node) {
      bool result(false);
      if (node) {
        result = setPointer(OS_PlantEquipmentOperation_OutdoorDewpointDifferenceFields::ReferenceTemperatureNodeName, node.get().handle());
      } else {
        resetReferenceTemperatureNode();
        result = true;
      }
      return result;
    }

    void PlantEquipmentOperationOutdoorDewpointDifference_Impl::resetReferenceTemperatureNode() {
      bool result = setString(OS_PlantEquipmentOperation_OutdoorDewpointDifferenceFields::ReferenceTemperatureNodeName, "");
      OS_ASSERT(result);
    }

    double PlantEquipmentOperationOutdoorDewpointDifference_Impl::maximumUpperLimit() const {
      return 100.0;
    }

    double PlantEquipmentOperationOutdoorDewpointDifference_Impl::minimumLowerLimit() const {
      return -50.0;
    }

  }  // namespace detail

  PlantEquipmentOperationOutdoorDewpointDifference::PlantEquipmentOperationOutdoorDewpointDifference(const Model& model)
    : PlantEquipmentOperationRangeBasedScheme(PlantEquipmentOperationOutdoorDewpointDifference::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::PlantEquipmentOperationOutdoorDewpointDifference_Impl>());
  }

  IddObjectType PlantEquipmentOperationOutdoorDewpointDifference::iddObjectType() {
    return {IddObjectType::OS_PlantEquipmentOperation_OutdoorDewpointDifference};
  }

  boost::optional<Node> PlantEquipmentOperationOutdoorDewpointDifference::referenceTemperatureNode() const {
    return getImpl<detail::PlantEquipmentOperationOutdoorDewpointDifference_Impl>()->referenceTemperatureNode();
  }

  bool PlantEquipmentOperationOutdoorDewpointDifference::setReferenceTemperatureNode(const Node& node) {
    return getImpl<detail::PlantEquipmentOperationOutdoorDewpointDifference_Impl>()->setReferenceTemperatureNode(node);
  }

  void PlantEquipmentOperationOutdoorDewpointDifference::resetReferenceTemperatureNode() {
    getImpl<detail::PlantEquipmentOperationOutdoorDewpointDifference_Impl>()->resetReferenceTemperatureNode();
  }

  /// @cond
  PlantEquipmentOperationOutdoorDewpointDifference::PlantEquipmentOperationOutdoorDewpointDifference(
    std::shared_ptr<detail::PlantEquipmentOperationOutdoorDewpointDifference_Impl> impl)
    : PlantEquipmentOperationRangeBasedScheme(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
