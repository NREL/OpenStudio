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

#include "EnergyManagementSystemInternalVariable.hpp"
#include "EnergyManagementSystemInternalVariable_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_InternalVariable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  EnergyManagementSystemInternalVariable_Impl::EnergyManagementSystemInternalVariable_Impl(const IdfObject& idfObject,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemInternalVariable::iddObjectType());
  }

  EnergyManagementSystemInternalVariable_Impl::EnergyManagementSystemInternalVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == EnergyManagementSystemInternalVariable::iddObjectType());
  }

  EnergyManagementSystemInternalVariable_Impl::EnergyManagementSystemInternalVariable_Impl(const EnergyManagementSystemInternalVariable_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& EnergyManagementSystemInternalVariable_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType EnergyManagementSystemInternalVariable_Impl::iddObjectType() const {
    return EnergyManagementSystemInternalVariable::iddObjectType();
  }

  boost::optional<std::string> EnergyManagementSystemInternalVariable_Impl::internalDataIndexKeyName() const {
    return getString(OS_EnergyManagementSystem_InternalVariableFields::InternalDataIndexKeyName,true);
  }

  std::string EnergyManagementSystemInternalVariable_Impl::internalDataType() const {
    boost::optional<std::string> value = getString(OS_EnergyManagementSystem_InternalVariableFields::InternalDataType,true);
    OS_ASSERT(value);
    return value.get();
  }

  void EnergyManagementSystemInternalVariable_Impl::setInternalDataIndexKeyName(const std::string& internalDataIndexKeyName) {
    bool result = setString(OS_EnergyManagementSystem_InternalVariableFields::InternalDataIndexKeyName, internalDataIndexKeyName);
    OS_ASSERT(result);
  }

  void EnergyManagementSystemInternalVariable_Impl::resetInternalDataIndexKeyName() {
    bool result = setString(OS_EnergyManagementSystem_InternalVariableFields::InternalDataIndexKeyName, "");
    OS_ASSERT(result);
  }

  void EnergyManagementSystemInternalVariable_Impl::setInternalDataType(const std::string& internalDataType) {
    bool result = setString(OS_EnergyManagementSystem_InternalVariableFields::InternalDataType, internalDataType);
    OS_ASSERT(result);
  }

} // detail

EnergyManagementSystemInternalVariable::EnergyManagementSystemInternalVariable(const Model& model)
  : ModelObject(EnergyManagementSystemInternalVariable::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EnergyManagementSystemInternalVariable_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  // setInternalDataType();
}

IddObjectType EnergyManagementSystemInternalVariable::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EnergyManagementSystem_InternalVariable);
}

boost::optional<std::string> EnergyManagementSystemInternalVariable::internalDataIndexKeyName() const {
  return getImpl<detail::EnergyManagementSystemInternalVariable_Impl>()->internalDataIndexKeyName();
}

std::string EnergyManagementSystemInternalVariable::internalDataType() const {
  return getImpl<detail::EnergyManagementSystemInternalVariable_Impl>()->internalDataType();
}

void EnergyManagementSystemInternalVariable::setInternalDataIndexKeyName(const std::string& internalDataIndexKeyName) {
  getImpl<detail::EnergyManagementSystemInternalVariable_Impl>()->setInternalDataIndexKeyName(internalDataIndexKeyName);
}

void EnergyManagementSystemInternalVariable::resetInternalDataIndexKeyName() {
  getImpl<detail::EnergyManagementSystemInternalVariable_Impl>()->resetInternalDataIndexKeyName();
}

void EnergyManagementSystemInternalVariable::setInternalDataType(const std::string& internalDataType) {
  getImpl<detail::EnergyManagementSystemInternalVariable_Impl>()->setInternalDataType(internalDataType);
}

/// @cond
EnergyManagementSystemInternalVariable::EnergyManagementSystemInternalVariable(std::shared_ptr<detail::EnergyManagementSystemInternalVariable_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

