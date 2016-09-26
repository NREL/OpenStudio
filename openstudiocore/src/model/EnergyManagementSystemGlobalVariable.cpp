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

#include "EnergyManagementSystemGlobalVariable.hpp"
#include "EnergyManagementSystemGlobalVariable_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_GlobalVariable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include "ModelExtensibleGroup.hpp"

namespace openstudio {
namespace model {

namespace detail {

  EnergyManagementSystemGlobalVariable_Impl::EnergyManagementSystemGlobalVariable_Impl(const IdfObject& idfObject,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemGlobalVariable::iddObjectType());
  }

  EnergyManagementSystemGlobalVariable_Impl::EnergyManagementSystemGlobalVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == EnergyManagementSystemGlobalVariable::iddObjectType());
  }

  EnergyManagementSystemGlobalVariable_Impl::EnergyManagementSystemGlobalVariable_Impl(const EnergyManagementSystemGlobalVariable_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& EnergyManagementSystemGlobalVariable_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType EnergyManagementSystemGlobalVariable_Impl::iddObjectType() const {
    return EnergyManagementSystemGlobalVariable::iddObjectType();
  }

} // detail

EnergyManagementSystemGlobalVariable::EnergyManagementSystemGlobalVariable(const Model& model, const std::string& variableName)
  : ModelObject(EnergyManagementSystemGlobalVariable::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EnergyManagementSystemGlobalVariable_Impl>());
  bool ok = getImpl<detail::EnergyManagementSystemGlobalVariable_Impl>()->setName(variableName);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s Name to " << variableName << ".");
  }
}

IddObjectType EnergyManagementSystemGlobalVariable::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EnergyManagementSystem_GlobalVariable);
}

/// @cond
EnergyManagementSystemGlobalVariable::EnergyManagementSystemGlobalVariable(std::shared_ptr<detail::EnergyManagementSystemGlobalVariable_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

