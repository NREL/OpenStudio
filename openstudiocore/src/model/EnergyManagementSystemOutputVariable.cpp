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

#include "EnergyManagementSystemOutputVariable.hpp"
#include "EnergyManagementSystemOutputVariable_Impl.hpp"
#include "EnergyManagementSystemProgram.hpp"
#include "EnergyManagementSystemProgram_Impl.hpp"
#include "EnergyManagementSystemSubroutine.hpp"
#include "EnergyManagementSystemSubroutine_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_OutputVariable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  EnergyManagementSystemOutputVariable_Impl::EnergyManagementSystemOutputVariable_Impl(const IdfObject& idfObject,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemOutputVariable::iddObjectType());
  }

  EnergyManagementSystemOutputVariable_Impl::EnergyManagementSystemOutputVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == EnergyManagementSystemOutputVariable::iddObjectType());
  }

  EnergyManagementSystemOutputVariable_Impl::EnergyManagementSystemOutputVariable_Impl(const EnergyManagementSystemOutputVariable_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& EnergyManagementSystemOutputVariable_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType EnergyManagementSystemOutputVariable_Impl::iddObjectType() const {
    return EnergyManagementSystemOutputVariable::iddObjectType();
  }

  std::string EnergyManagementSystemOutputVariable_Impl::eMSVariableName() const {
    boost::optional<std::string> value = getString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName,true);
    //OS_ASSERT(value);
    return value.get();
  }

  std::string EnergyManagementSystemOutputVariable_Impl::typeofDatainVariable() const {
    boost::optional<std::string> value = getString(OS_EnergyManagementSystem_OutputVariableFields::TypeofDatainVariable,true);
    //OS_ASSERT(value);
    return value.get();
  }

  std::string EnergyManagementSystemOutputVariable_Impl::updateFrequency() const {
    boost::optional<std::string> value = getString(OS_EnergyManagementSystem_OutputVariableFields::UpdateFrequency,true);
    //OS_ASSERT(value);
    return value.get();
  }

  boost::optional<std::string> EnergyManagementSystemOutputVariable_Impl::eMSProgramorSubroutineName() const {
    return getString(OS_EnergyManagementSystem_OutputVariableFields::EMSProgramorSubroutineName,true);
  }

  boost::optional<std::string> EnergyManagementSystemOutputVariable_Impl::units() const {
    return getString(OS_EnergyManagementSystem_OutputVariableFields::Units,true);
  }

  bool EnergyManagementSystemOutputVariable_Impl::setEMSVariableName(const std::string& eMSVariableName) {
    bool result = setString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName, eMSVariableName);
    return result;
  }

  bool EnergyManagementSystemOutputVariable_Impl::setTypeofDatainVariable(const std::string& typeofDatainVariable) {
    bool result = setString(OS_EnergyManagementSystem_OutputVariableFields::TypeofDatainVariable, typeofDatainVariable);
    return result;
  }

  bool EnergyManagementSystemOutputVariable_Impl::setUpdateFrequency(const std::string& updateFrequency) {
    bool result = setString(OS_EnergyManagementSystem_OutputVariableFields::UpdateFrequency, updateFrequency);
    return result;
  }

  bool EnergyManagementSystemOutputVariable_Impl::setEMSProgramorSubroutineName(const EnergyManagementSystemProgram& program) {
    bool result = setPointer(OS_EnergyManagementSystem_OutputVariableFields::EMSProgramorSubroutineName, program.handle());
    //OS_ASSERT(result);
    return result;
  }

  bool EnergyManagementSystemOutputVariable_Impl::setEMSProgramorSubroutineName(const EnergyManagementSystemSubroutine& subroutine) {
    bool result = setPointer(OS_EnergyManagementSystem_OutputVariableFields::EMSProgramorSubroutineName, subroutine.handle());
    OS_ASSERT(result);
    return result;
  }

  void EnergyManagementSystemOutputVariable_Impl::resetEMSProgramorSubroutineName() {
    bool result = setString(OS_EnergyManagementSystem_OutputVariableFields::EMSProgramorSubroutineName, "");
    //OS_ASSERT(result);
  }

  bool EnergyManagementSystemOutputVariable_Impl::setUnits(const std::string& units) {
    bool result = setString(OS_EnergyManagementSystem_OutputVariableFields::Units, units);
    return result;
  }

  void EnergyManagementSystemOutputVariable_Impl::resetUnits() {
    bool result = setString(OS_EnergyManagementSystem_OutputVariableFields::Units, "");
    //OS_ASSERT(result);
  }

} // detail

EnergyManagementSystemOutputVariable::EnergyManagementSystemOutputVariable(const Model& model)
  : ModelObject(EnergyManagementSystemOutputVariable::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EnergyManagementSystemOutputVariable_Impl>());
  setUpdateFrequency("ZoneTimestep");
  setTypeofDatainVariable("Averaged");
}

IddObjectType EnergyManagementSystemOutputVariable::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EnergyManagementSystem_OutputVariable);
}

std::vector<std::string> EnergyManagementSystemOutputVariable::typeofDatainVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EnergyManagementSystem_OutputVariableFields::TypeofDatainVariable);
}

std::vector<std::string> EnergyManagementSystemOutputVariable::updateFrequencyValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EnergyManagementSystem_OutputVariableFields::UpdateFrequency);
}

std::string EnergyManagementSystemOutputVariable::eMSVariableName() const {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->eMSVariableName();
}

std::string EnergyManagementSystemOutputVariable::typeofDatainVariable() const {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->typeofDatainVariable();
}

std::string EnergyManagementSystemOutputVariable::updateFrequency() const {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->updateFrequency();
}

boost::optional<std::string> EnergyManagementSystemOutputVariable::eMSProgramorSubroutineName() const {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->eMSProgramorSubroutineName();
}

boost::optional<std::string> EnergyManagementSystemOutputVariable::units() const {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->units();
}

bool EnergyManagementSystemOutputVariable::setEMSVariableName(const std::string& eMSVariableName) {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->setEMSVariableName(eMSVariableName);
}

bool EnergyManagementSystemOutputVariable::setTypeofDatainVariable(const std::string& typeofDatainVariable) {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->setTypeofDatainVariable(typeofDatainVariable);
}

bool EnergyManagementSystemOutputVariable::setUpdateFrequency(const std::string& updateFrequency) {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->setUpdateFrequency(updateFrequency);
}

bool EnergyManagementSystemOutputVariable::setEMSProgramorSubroutineName(const EnergyManagementSystemProgram& program) {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->setEMSProgramorSubroutineName(program);
}

bool EnergyManagementSystemOutputVariable::setEMSProgramorSubroutineName(const EnergyManagementSystemSubroutine& subroutine) {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->setEMSProgramorSubroutineName(subroutine);
}

void EnergyManagementSystemOutputVariable::resetEMSProgramorSubroutineName() {
  getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->resetEMSProgramorSubroutineName();
}

bool EnergyManagementSystemOutputVariable::setUnits(const std::string& units) {
  return getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->setUnits(units);
}

void EnergyManagementSystemOutputVariable::resetUnits() {
  getImpl<detail::EnergyManagementSystemOutputVariable_Impl>()->resetUnits();
}

/// @cond
EnergyManagementSystemOutputVariable::EnergyManagementSystemOutputVariable(std::shared_ptr<detail::EnergyManagementSystemOutputVariable_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

