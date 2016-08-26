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

#include "EnergyManagementSystemTrendVariable.hpp"
#include "EnergyManagementSystemTrendVariable_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_TrendVariable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  EnergyManagementSystemTrendVariable_Impl::EnergyManagementSystemTrendVariable_Impl(const IdfObject& idfObject,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemTrendVariable::iddObjectType());
  }

  EnergyManagementSystemTrendVariable_Impl::EnergyManagementSystemTrendVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == EnergyManagementSystemTrendVariable::iddObjectType());
  }

  EnergyManagementSystemTrendVariable_Impl::EnergyManagementSystemTrendVariable_Impl(const EnergyManagementSystemTrendVariable_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& EnergyManagementSystemTrendVariable_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType EnergyManagementSystemTrendVariable_Impl::iddObjectType() const {
    return EnergyManagementSystemTrendVariable::iddObjectType();
  }

  std::string EnergyManagementSystemTrendVariable_Impl::eMSVariableName() const {
    boost::optional<std::string> value = getString(OS_EnergyManagementSystem_TrendVariableFields::EMSVariableName,true);
    OS_ASSERT(value);
    return value.get();
  }

  int EnergyManagementSystemTrendVariable_Impl::numberofTimestepstobeLogged() const {
    boost::optional<int> value = getInt(OS_EnergyManagementSystem_TrendVariableFields::NumberofTimestepstobeLogged,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool EnergyManagementSystemTrendVariable_Impl::setEMSVariableName(const std::string& eMSVariableName) {
    bool result = setString(OS_EnergyManagementSystem_TrendVariableFields::EMSVariableName, eMSVariableName);
    OS_ASSERT(result);
    return result;
  }

  bool EnergyManagementSystemTrendVariable_Impl::setNumberofTimestepstobeLogged(int numberofTimestepstobeLogged) {
    bool result = setInt(OS_EnergyManagementSystem_TrendVariableFields::NumberofTimestepstobeLogged, numberofTimestepstobeLogged);
    return result;
  }

} // detail

EnergyManagementSystemTrendVariable::EnergyManagementSystemTrendVariable(const Model& model)
  : ModelObject(EnergyManagementSystemTrendVariable::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EnergyManagementSystemTrendVariable_Impl>());
}

IddObjectType EnergyManagementSystemTrendVariable::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EnergyManagementSystem_TrendVariable);
}

std::string EnergyManagementSystemTrendVariable::eMSVariableName() const {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->eMSVariableName();
}

int EnergyManagementSystemTrendVariable::numberofTimestepstobeLogged() const {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->numberofTimestepstobeLogged();
}

bool EnergyManagementSystemTrendVariable::setEMSVariableName(const std::string& eMSVariableName) {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->setEMSVariableName(eMSVariableName);
}

bool EnergyManagementSystemTrendVariable::setNumberofTimestepstobeLogged(int numberofTimestepstobeLogged) {
  return getImpl<detail::EnergyManagementSystemTrendVariable_Impl>()->setNumberofTimestepstobeLogged(numberofTimestepstobeLogged);
}

/// @cond
EnergyManagementSystemTrendVariable::EnergyManagementSystemTrendVariable(std::shared_ptr<detail::EnergyManagementSystemTrendVariable_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

