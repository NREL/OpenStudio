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

#include "PlantEquipmentOperationOutdoorDewpoint.hpp"
#include "PlantEquipmentOperationOutdoorDewpoint_Impl.hpp"
#include <utilities/idd/OS_PlantEquipmentOperation_OutdoorDewpoint_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  PlantEquipmentOperationOutdoorDewpoint_Impl::PlantEquipmentOperationOutdoorDewpoint_Impl(const IdfObject& idfObject,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : PlantEquipmentOperationRangeBasedScheme_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == PlantEquipmentOperationOutdoorDewpoint::iddObjectType());
  }

  PlantEquipmentOperationOutdoorDewpoint_Impl::PlantEquipmentOperationOutdoorDewpoint_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : PlantEquipmentOperationRangeBasedScheme_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == PlantEquipmentOperationOutdoorDewpoint::iddObjectType());
  }

  PlantEquipmentOperationOutdoorDewpoint_Impl::PlantEquipmentOperationOutdoorDewpoint_Impl(const PlantEquipmentOperationOutdoorDewpoint_Impl& other,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : PlantEquipmentOperationRangeBasedScheme_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& PlantEquipmentOperationOutdoorDewpoint_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType PlantEquipmentOperationOutdoorDewpoint_Impl::iddObjectType() const {
    return PlantEquipmentOperationOutdoorDewpoint::iddObjectType();
  }

  double PlantEquipmentOperationOutdoorDewpoint_Impl::maximumUpperLimit() const
  {
    return 70.0;
  }

  double PlantEquipmentOperationOutdoorDewpoint_Impl::minimumLowerLimit() const
  {
    return -70.0;
  }

} // detail

PlantEquipmentOperationOutdoorDewpoint::PlantEquipmentOperationOutdoorDewpoint(const Model& model)
  : PlantEquipmentOperationRangeBasedScheme(PlantEquipmentOperationOutdoorDewpoint::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::PlantEquipmentOperationOutdoorDewpoint_Impl>());
}

IddObjectType PlantEquipmentOperationOutdoorDewpoint::iddObjectType() {
  return IddObjectType(IddObjectType::OS_PlantEquipmentOperation_OutdoorDewpoint);
}

/// @cond
PlantEquipmentOperationOutdoorDewpoint::PlantEquipmentOperationOutdoorDewpoint(std::shared_ptr<detail::PlantEquipmentOperationOutdoorDewpoint_Impl> impl)
  : PlantEquipmentOperationRangeBasedScheme(impl)
{}
/// @endcond

} // model
} // openstudio

