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

#include "PlantEquipmentOperationOutdoorDryBulb.hpp"
#include "PlantEquipmentOperationOutdoorDryBulb_Impl.hpp"
#include <utilities/idd/OS_PlantEquipmentOperation_OutdoorDryBulb_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  PlantEquipmentOperationOutdoorDryBulb_Impl::PlantEquipmentOperationOutdoorDryBulb_Impl(const IdfObject& idfObject,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : PlantEquipmentOperationRangeBasedScheme_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == PlantEquipmentOperationOutdoorDryBulb::iddObjectType());
  }

  PlantEquipmentOperationOutdoorDryBulb_Impl::PlantEquipmentOperationOutdoorDryBulb_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : PlantEquipmentOperationRangeBasedScheme_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == PlantEquipmentOperationOutdoorDryBulb::iddObjectType());
  }

  PlantEquipmentOperationOutdoorDryBulb_Impl::PlantEquipmentOperationOutdoorDryBulb_Impl(const PlantEquipmentOperationOutdoorDryBulb_Impl& other,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : PlantEquipmentOperationRangeBasedScheme_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& PlantEquipmentOperationOutdoorDryBulb_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType PlantEquipmentOperationOutdoorDryBulb_Impl::iddObjectType() const {
    return PlantEquipmentOperationOutdoorDryBulb::iddObjectType();
  }

  double PlantEquipmentOperationOutdoorDryBulb_Impl::maximumUpperLimit() const
  {
    return 70.0;
  }

  double PlantEquipmentOperationOutdoorDryBulb_Impl::minimumLowerLimit() const
  {
    return -70.0;
  }

} // detail

PlantEquipmentOperationOutdoorDryBulb::PlantEquipmentOperationOutdoorDryBulb(const Model& model)
  : PlantEquipmentOperationRangeBasedScheme(PlantEquipmentOperationOutdoorDryBulb::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::PlantEquipmentOperationOutdoorDryBulb_Impl>());
}

IddObjectType PlantEquipmentOperationOutdoorDryBulb::iddObjectType() {
  return IddObjectType(IddObjectType::OS_PlantEquipmentOperation_OutdoorDryBulb);
}

/// @cond
PlantEquipmentOperationOutdoorDryBulb::PlantEquipmentOperationOutdoorDryBulb(std::shared_ptr<detail::PlantEquipmentOperationOutdoorDryBulb_Impl> impl)
  : PlantEquipmentOperationRangeBasedScheme(impl)
{}
/// @endcond

} // model
} // openstudio

