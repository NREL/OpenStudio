 
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

#include "AirWallMaterial.hpp"
#include "AirWallMaterial_Impl.hpp"

#include <utilities/idd/OS_Material_AirWall_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirWallMaterial_Impl::AirWallMaterial_Impl(const IdfObject& idfObject,
                                             Model_Impl* model,
                                             bool keepHandle)
    : ModelPartitionMaterial_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirWallMaterial::iddObjectType());
  }

  AirWallMaterial_Impl::AirWallMaterial_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : ModelPartitionMaterial_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirWallMaterial::iddObjectType());
  }

  AirWallMaterial_Impl::AirWallMaterial_Impl(const AirWallMaterial_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : ModelPartitionMaterial_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirWallMaterial_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirWallMaterial_Impl::iddObjectType() const {
    return AirWallMaterial::iddObjectType();
  }

} // detail

AirWallMaterial::AirWallMaterial(const Model& model)
  : ModelPartitionMaterial(AirWallMaterial::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirWallMaterial_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
}

IddObjectType AirWallMaterial::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Material_AirWall);
}

/// @cond
AirWallMaterial::AirWallMaterial(std::shared_ptr<detail::AirWallMaterial_Impl> impl)
  : ModelPartitionMaterial(impl)
{}
/// @endcond

} // model
} // openstudio

