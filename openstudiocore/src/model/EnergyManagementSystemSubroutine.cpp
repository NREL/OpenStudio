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

#include "EnergyManagementSystemSubroutine.hpp"
#include "EnergyManagementSystemSubroutine_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_Subroutine_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  EnergyManagementSystemSubroutine_Impl::EnergyManagementSystemSubroutine_Impl(const IdfObject& idfObject,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemSubroutine::iddObjectType());
  }

  EnergyManagementSystemSubroutine_Impl::EnergyManagementSystemSubroutine_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == EnergyManagementSystemSubroutine::iddObjectType());
  }

  EnergyManagementSystemSubroutine_Impl::EnergyManagementSystemSubroutine_Impl(const EnergyManagementSystemSubroutine_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& EnergyManagementSystemSubroutine_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType EnergyManagementSystemSubroutine_Impl::iddObjectType() const {
    return EnergyManagementSystemSubroutine::iddObjectType();
  }

} // detail

EnergyManagementSystemSubroutine::EnergyManagementSystemSubroutine(const Model& model)
  : ModelObject(EnergyManagementSystemSubroutine::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EnergyManagementSystemSubroutine_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
}

IddObjectType EnergyManagementSystemSubroutine::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EnergyManagementSystem_Subroutine);
}

/// @cond
EnergyManagementSystemSubroutine::EnergyManagementSystemSubroutine(std::shared_ptr<detail::EnergyManagementSystemSubroutine_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

