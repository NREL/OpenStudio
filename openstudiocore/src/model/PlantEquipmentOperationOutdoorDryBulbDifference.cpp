/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "PlantEquipmentOperationOutdoorDryBulbDifference.hpp"
#include "PlantEquipmentOperationOutdoorDryBulbDifference_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include <utilities/idd/OS_PlantEquipmentOperation_OutdoorDryBulbDifference_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  PlantEquipmentOperationOutdoorDryBulbDifference_Impl::PlantEquipmentOperationOutdoorDryBulbDifference_Impl(const IdfObject& idfObject,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : PlantEquipmentOperationRangeBasedScheme_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == PlantEquipmentOperationOutdoorDryBulbDifference::iddObjectType());
  }

  PlantEquipmentOperationOutdoorDryBulbDifference_Impl::PlantEquipmentOperationOutdoorDryBulbDifference_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : PlantEquipmentOperationRangeBasedScheme_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == PlantEquipmentOperationOutdoorDryBulbDifference::iddObjectType());
  }

  PlantEquipmentOperationOutdoorDryBulbDifference_Impl::PlantEquipmentOperationOutdoorDryBulbDifference_Impl(const PlantEquipmentOperationOutdoorDryBulbDifference_Impl& other,
                                                                                                             Model_Impl* model,
                                                                                                             bool keepHandle)
    : PlantEquipmentOperationRangeBasedScheme_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& PlantEquipmentOperationOutdoorDryBulbDifference_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType PlantEquipmentOperationOutdoorDryBulbDifference_Impl::iddObjectType() const {
    return PlantEquipmentOperationOutdoorDryBulbDifference::iddObjectType();
  }

  boost::optional<Node> PlantEquipmentOperationOutdoorDryBulbDifference_Impl::referenceTemperatureNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_PlantEquipmentOperation_OutdoorDryBulbDifferenceFields::ReferenceTemperatureNodeName);
  }

  bool PlantEquipmentOperationOutdoorDryBulbDifference_Impl::setReferenceTemperatureNode(const boost::optional<Node>& node) {
    bool result(false);
    if (node) {
      result = setPointer(OS_PlantEquipmentOperation_OutdoorDryBulbDifferenceFields::ReferenceTemperatureNodeName, node.get().handle());
    }
    else {
      resetReferenceTemperatureNode();
      result = true;
    }
    return result;
  }

  void PlantEquipmentOperationOutdoorDryBulbDifference_Impl::resetReferenceTemperatureNode() {
    bool result = setString(OS_PlantEquipmentOperation_OutdoorDryBulbDifferenceFields::ReferenceTemperatureNodeName, "");
    OS_ASSERT(result);
  }

  double PlantEquipmentOperationOutdoorDryBulbDifference_Impl::maximumUpperLimit() const
  {
    return 100.0;
  }

  double PlantEquipmentOperationOutdoorDryBulbDifference_Impl::minimumLowerLimit() const
  {
    return -50.0;
  }

} // detail

PlantEquipmentOperationOutdoorDryBulbDifference::PlantEquipmentOperationOutdoorDryBulbDifference(const Model& model)
  : PlantEquipmentOperationRangeBasedScheme(PlantEquipmentOperationOutdoorDryBulbDifference::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::PlantEquipmentOperationOutdoorDryBulbDifference_Impl>());
}

IddObjectType PlantEquipmentOperationOutdoorDryBulbDifference::iddObjectType() {
  return IddObjectType(IddObjectType::OS_PlantEquipmentOperation_OutdoorDryBulbDifference);
}

boost::optional<Node> PlantEquipmentOperationOutdoorDryBulbDifference::referenceTemperatureNode() const {
  return getImpl<detail::PlantEquipmentOperationOutdoorDryBulbDifference_Impl>()->referenceTemperatureNode();
}

bool PlantEquipmentOperationOutdoorDryBulbDifference::setReferenceTemperatureNode(const Node& node) {
  return getImpl<detail::PlantEquipmentOperationOutdoorDryBulbDifference_Impl>()->setReferenceTemperatureNode(node);
}

void PlantEquipmentOperationOutdoorDryBulbDifference::resetReferenceTemperatureNode() {
  getImpl<detail::PlantEquipmentOperationOutdoorDryBulbDifference_Impl>()->resetReferenceTemperatureNode();
}

/// @cond
PlantEquipmentOperationOutdoorDryBulbDifference::PlantEquipmentOperationOutdoorDryBulbDifference(std::shared_ptr<detail::PlantEquipmentOperationOutdoorDryBulbDifference_Impl> impl)
  : PlantEquipmentOperationRangeBasedScheme(impl)
{}
/// @endcond

} // model
} // openstudio

