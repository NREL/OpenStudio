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

#include "EnergyManagementSystemProgramCallingManager.hpp"
#include "EnergyManagementSystemProgramCallingManager_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_ProgramCallingManager_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  EnergyManagementSystemProgramCallingManager_Impl::EnergyManagementSystemProgramCallingManager_Impl(const IdfObject& idfObject,
                                                                                                     Model_Impl* model,
                                                                                                     bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemProgramCallingManager::iddObjectType());
  }

  EnergyManagementSystemProgramCallingManager_Impl::EnergyManagementSystemProgramCallingManager_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                     Model_Impl* model,
                                                                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == EnergyManagementSystemProgramCallingManager::iddObjectType());
  }

  EnergyManagementSystemProgramCallingManager_Impl::EnergyManagementSystemProgramCallingManager_Impl(const EnergyManagementSystemProgramCallingManager_Impl& other,
                                                                                                     Model_Impl* model,
                                                                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& EnergyManagementSystemProgramCallingManager_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType EnergyManagementSystemProgramCallingManager_Impl::iddObjectType() const {
    return EnergyManagementSystemProgramCallingManager::iddObjectType();
  }

  boost::optional<std::string> EnergyManagementSystemProgramCallingManager_Impl::callingPoint() const {
    return getString(OS_EnergyManagementSystem_ProgramCallingManagerFields::EnergyPlusModelCallingPoint,true);
  }

  bool EnergyManagementSystemProgramCallingManager_Impl::setCallingPoint(const std::string& callingPoint) {
    bool result = setString(OS_EnergyManagementSystem_ProgramCallingManagerFields::EnergyPlusModelCallingPoint, callingPoint);
    return result;
  }

} // detail

EnergyManagementSystemProgramCallingManager::EnergyManagementSystemProgramCallingManager(const Model& model)
  : ModelObject(EnergyManagementSystemProgramCallingManager::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EnergyManagementSystemProgramCallingManager_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
}

IddObjectType EnergyManagementSystemProgramCallingManager::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EnergyManagementSystem_ProgramCallingManager);
}

std::vector<std::string> EnergyManagementSystemProgramCallingManager::validCallingPointValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EnergyManagementSystem_ProgramCallingManagerFields::EnergyPlusModelCallingPoint);
}

boost::optional<std::string> EnergyManagementSystemProgramCallingManager::callingPoint() const {
  return getImpl<detail::EnergyManagementSystemProgramCallingManager_Impl>()->callingPoint();
}

bool EnergyManagementSystemProgramCallingManager::setCallingPoint(const std::string& callingPoint) {
  return getImpl<detail::EnergyManagementSystemProgramCallingManager_Impl>()->setCallingPoint(callingPoint);
}

/// @cond
EnergyManagementSystemProgramCallingManager::EnergyManagementSystemProgramCallingManager(std::shared_ptr<detail::EnergyManagementSystemProgramCallingManager_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

