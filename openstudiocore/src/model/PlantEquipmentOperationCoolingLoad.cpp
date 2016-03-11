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

#include "PlantEquipmentOperationCoolingLoad.hpp"
#include "PlantEquipmentOperationCoolingLoad_Impl.hpp"
#include <utilities/idd/OS_PlantEquipmentOperation_CoolingLoad_FieldEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

namespace detail {

  PlantEquipmentOperationCoolingLoad_Impl::PlantEquipmentOperationCoolingLoad_Impl(const IdfObject& idfObject,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : PlantEquipmentOperationRangeBasedScheme_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == PlantEquipmentOperationCoolingLoad::iddObjectType());
  }

  PlantEquipmentOperationCoolingLoad_Impl::PlantEquipmentOperationCoolingLoad_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : PlantEquipmentOperationRangeBasedScheme_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == PlantEquipmentOperationCoolingLoad::iddObjectType());
  }

  PlantEquipmentOperationCoolingLoad_Impl::PlantEquipmentOperationCoolingLoad_Impl(const PlantEquipmentOperationCoolingLoad_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : PlantEquipmentOperationRangeBasedScheme_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& PlantEquipmentOperationCoolingLoad_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType PlantEquipmentOperationCoolingLoad_Impl::iddObjectType() const {
    return PlantEquipmentOperationCoolingLoad::iddObjectType();
  }

  double PlantEquipmentOperationCoolingLoad_Impl::maximumUpperLimit() const
  {
    return 1E9;
  }

  double PlantEquipmentOperationCoolingLoad_Impl::minimumLowerLimit() const
  {
    return 0.0;
  }

} // detail

PlantEquipmentOperationCoolingLoad::PlantEquipmentOperationCoolingLoad(const Model& model)
  : PlantEquipmentOperationRangeBasedScheme(PlantEquipmentOperationCoolingLoad::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::PlantEquipmentOperationCoolingLoad_Impl>());
}

IddObjectType PlantEquipmentOperationCoolingLoad::iddObjectType() {
  return IddObjectType(IddObjectType::OS_PlantEquipmentOperation_CoolingLoad);
}

/// @cond
PlantEquipmentOperationCoolingLoad::PlantEquipmentOperationCoolingLoad(std::shared_ptr<detail::PlantEquipmentOperationCoolingLoad_Impl> impl)
  : PlantEquipmentOperationRangeBasedScheme(impl)
{}
/// @endcond

} // model
} // openstudio

