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

  bool EnergyManagementSystemGlobalVariable_Impl::addVariable(const std::string& variable) {
    //set global variable
    bool result = false;

    WorkspaceExtensibleGroup group = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
    result = group.setString(OS_EnergyManagementSystem_GlobalVariableExtensibleFields::ErlVariableName, variable);

    return result;
  }

  bool EnergyManagementSystemGlobalVariable_Impl::removeVariable(const std::string& variable) {
    //remove global variable
    bool result = false;

    auto groups = extensibleGroups();
    unsigned sizeOfGroup = numExtensibleGroups();
    int i = 0;
    for (auto group = groups.begin(); group != groups.end(); ++group) {
      if (group->getString(OS_EnergyManagementSystem_GlobalVariableExtensibleFields::ErlVariableName, true) == variable) {
        eraseExtensibleGroup(i);
        i++;
        result = true;
      }
    }
    return result;
  }

  std::vector<std::string> EnergyManagementSystemGlobalVariable_Impl::getVariables() const {
    //return vector of global variables
    std::vector<std::string> result;
    auto groups = extensibleGroups();

    for (auto group = groups.begin(); group != groups.end(); ++group) {
      const auto & variable = group->getString(OS_EnergyManagementSystem_GlobalVariableExtensibleFields::ErlVariableName, true);
      OS_ASSERT(variable);
      result.push_back(variable.get());
    }
    return result;
  }

  void EnergyManagementSystemGlobalVariable_Impl::eraseVariables() {
    //erase all Variables in this global variable object
    clearExtensibleGroups();
  }
} // detail

EnergyManagementSystemGlobalVariable::EnergyManagementSystemGlobalVariable(const Model& model)
  : ModelObject(EnergyManagementSystemGlobalVariable::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EnergyManagementSystemGlobalVariable_Impl>());
}

IddObjectType EnergyManagementSystemGlobalVariable::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EnergyManagementSystem_GlobalVariable);
}

bool EnergyManagementSystemGlobalVariable::addVariable(const std::string& variable) {
  return getImpl<detail::EnergyManagementSystemGlobalVariable_Impl>()->addVariable(variable);
}

bool EnergyManagementSystemGlobalVariable::removeVariable(const std::string& variable) {
  return getImpl<detail::EnergyManagementSystemGlobalVariable_Impl>()->removeVariable(variable);
}

std::vector<std::string> EnergyManagementSystemGlobalVariable::getVariables() const {
  return getImpl<detail::EnergyManagementSystemGlobalVariable_Impl>()->getVariables();
}

void EnergyManagementSystemGlobalVariable::eraseVariables() {
  getImpl<detail::EnergyManagementSystemGlobalVariable_Impl>()->eraseVariables();
}

/// @cond
EnergyManagementSystemGlobalVariable::EnergyManagementSystemGlobalVariable(std::shared_ptr<detail::EnergyManagementSystemGlobalVariable_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

