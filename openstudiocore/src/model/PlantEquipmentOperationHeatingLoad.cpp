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

#include "PlantEquipmentOperationHeatingLoad.hpp"
#include "PlantEquipmentOperationHeatingLoad_Impl.hpp"
#include <utilities/idd/OS_PlantEquipmentOperation_HeatingLoad_FieldEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

namespace detail {

  PlantEquipmentOperationHeatingLoad_Impl::PlantEquipmentOperationHeatingLoad_Impl(const IdfObject& idfObject,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : PlantEquipmentOperationRangeBasedScheme_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == PlantEquipmentOperationHeatingLoad::iddObjectType());
  }

  PlantEquipmentOperationHeatingLoad_Impl::PlantEquipmentOperationHeatingLoad_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : PlantEquipmentOperationRangeBasedScheme_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == PlantEquipmentOperationHeatingLoad::iddObjectType());
  }

  PlantEquipmentOperationHeatingLoad_Impl::PlantEquipmentOperationHeatingLoad_Impl(const PlantEquipmentOperationHeatingLoad_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : PlantEquipmentOperationRangeBasedScheme_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& PlantEquipmentOperationHeatingLoad_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType PlantEquipmentOperationHeatingLoad_Impl::iddObjectType() const {
    return PlantEquipmentOperationHeatingLoad::iddObjectType();
  }

  double PlantEquipmentOperationHeatingLoad_Impl::maximumUpperLimit() const
  {
    return 1E9;
  }

  double PlantEquipmentOperationHeatingLoad_Impl::minimumLowerLimit() const
  {
    return 0.0;
  }

} // detail

PlantEquipmentOperationHeatingLoad::PlantEquipmentOperationHeatingLoad(const Model& model)
  : PlantEquipmentOperationRangeBasedScheme(PlantEquipmentOperationHeatingLoad::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::PlantEquipmentOperationHeatingLoad_Impl>());
}

IddObjectType PlantEquipmentOperationHeatingLoad::iddObjectType() {
  return IddObjectType(IddObjectType::OS_PlantEquipmentOperation_HeatingLoad);
}

/// @cond
PlantEquipmentOperationHeatingLoad::PlantEquipmentOperationHeatingLoad(std::shared_ptr<detail::PlantEquipmentOperationHeatingLoad_Impl> impl)
  : PlantEquipmentOperationRangeBasedScheme(impl)
{}
/// @endcond

} // model
} // openstudio

