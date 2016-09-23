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

#include "EnergyManagementSystemMeteredOutputVariable.hpp"
#include "EnergyManagementSystemMeteredOutputVariable_Impl.hpp"
#include "EnergyManagementSystemProgram.hpp"
#include "EnergyManagementSystemProgram_Impl.hpp"
#include "EnergyManagementSystemSubroutine.hpp"
#include "EnergyManagementSystemSubroutine_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_MeteredOutputVariable_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  EnergyManagementSystemMeteredOutputVariable_Impl::EnergyManagementSystemMeteredOutputVariable_Impl(const IdfObject& idfObject,
                                                                                                     Model_Impl* model,
                                                                                                     bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemMeteredOutputVariable::iddObjectType());
  }

  EnergyManagementSystemMeteredOutputVariable_Impl::EnergyManagementSystemMeteredOutputVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                     Model_Impl* model,
                                                                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == EnergyManagementSystemMeteredOutputVariable::iddObjectType());
  }

  EnergyManagementSystemMeteredOutputVariable_Impl::EnergyManagementSystemMeteredOutputVariable_Impl(const EnergyManagementSystemMeteredOutputVariable_Impl& other,
                                                                                                     Model_Impl* model,
                                                                                                     bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& EnergyManagementSystemMeteredOutputVariable_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType EnergyManagementSystemMeteredOutputVariable_Impl::iddObjectType() const {
    return EnergyManagementSystemMeteredOutputVariable::iddObjectType();
  }

  std::string EnergyManagementSystemMeteredOutputVariable_Impl::eMSVariableName() const {
    boost::optional<std::string> value = getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName,true);
    return value.get();
  }

  std::string EnergyManagementSystemMeteredOutputVariable_Impl::updateFrequency() const {
    boost::optional<std::string> value = getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::UpdateFrequency,true);
    return value.get();
  }

  boost::optional<std::string> EnergyManagementSystemMeteredOutputVariable_Impl::eMSProgramorSubroutineName() const {
    return getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSProgramorSubroutineName,true);
  }

  std::string EnergyManagementSystemMeteredOutputVariable_Impl::resourceType() const {
    boost::optional<std::string> value = getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::ResourceType,true);
    return value.get();
  }

  std::string EnergyManagementSystemMeteredOutputVariable_Impl::groupType() const {
    boost::optional<std::string> value = getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::GroupType,true);
    return value.get();
  }

  std::string EnergyManagementSystemMeteredOutputVariable_Impl::endUseCategory() const {
    boost::optional<std::string> value = getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EndUseCategory,true);
    return value.get();
  }

  boost::optional<std::string> EnergyManagementSystemMeteredOutputVariable_Impl::endUseSubcategory() const {
    return getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EndUseSubcategory,true);
  }

  boost::optional<std::string> EnergyManagementSystemMeteredOutputVariable_Impl::units() const {
    return getString(OS_EnergyManagementSystem_MeteredOutputVariableFields::Units,true);
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setEMSVariableName(const std::string& eMSVariableName) {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSVariableName, eMSVariableName);
    return result;
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setUpdateFrequency(const std::string& updateFrequency) {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::UpdateFrequency, updateFrequency);
    return result;
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setEMSProgramorSubroutineName(const EnergyManagementSystemProgram& program) {
    bool result = setPointer(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSProgramorSubroutineName, program.handle());
    return result;
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setEMSProgramorSubroutineName(const EnergyManagementSystemSubroutine& subroutine) {
    bool result = setPointer(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSProgramorSubroutineName, subroutine.handle());
    return result;
  }

  void EnergyManagementSystemMeteredOutputVariable_Impl::resetEMSProgramorSubroutineName() {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EMSProgramorSubroutineName, "");
    OS_ASSERT(result);
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setResourceType(const std::string& resourceType) {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::ResourceType, resourceType);
    return result;
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setGroupType(const std::string& groupType) {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::GroupType, groupType);
    return result;
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setEndUseCategory(const std::string& endUseCategory) {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EndUseCategory, endUseCategory);
    return result;
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EndUseSubcategory, endUseSubcategory);
    return result;
  }

  void EnergyManagementSystemMeteredOutputVariable_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::EndUseSubcategory, "");
    OS_ASSERT(result);
  }

  bool EnergyManagementSystemMeteredOutputVariable_Impl::setUnits(const std::string& units) {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::Units, units);
    return result;
  }

  void EnergyManagementSystemMeteredOutputVariable_Impl::resetUnits() {
    bool result = setString(OS_EnergyManagementSystem_MeteredOutputVariableFields::Units, "");
    OS_ASSERT(result);
  }

} // detail

EnergyManagementSystemMeteredOutputVariable::EnergyManagementSystemMeteredOutputVariable(const Model& model)
  : ModelObject(EnergyManagementSystemMeteredOutputVariable::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>());
  //TODO setEMSVariableName to a valid default
  setUpdateFrequency("SystemTimestep");
  setResourceType("Electricity");
  setGroupType("Building");
  setEndUseCategory("Fans");
}

IddObjectType EnergyManagementSystemMeteredOutputVariable::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EnergyManagementSystem_MeteredOutputVariable);
}

std::vector<std::string> EnergyManagementSystemMeteredOutputVariable::updateFrequencyValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EnergyManagementSystem_MeteredOutputVariableFields::UpdateFrequency);
}

std::vector<std::string> EnergyManagementSystemMeteredOutputVariable::resourceTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EnergyManagementSystem_MeteredOutputVariableFields::ResourceType);
}

std::vector<std::string> EnergyManagementSystemMeteredOutputVariable::groupTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EnergyManagementSystem_MeteredOutputVariableFields::GroupType);
}

std::vector<std::string> EnergyManagementSystemMeteredOutputVariable::endUseCategoryValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_EnergyManagementSystem_MeteredOutputVariableFields::EndUseCategory);
}

std::string EnergyManagementSystemMeteredOutputVariable::eMSVariableName() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->eMSVariableName();
}

std::string EnergyManagementSystemMeteredOutputVariable::updateFrequency() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->updateFrequency();
}

boost::optional<std::string> EnergyManagementSystemMeteredOutputVariable::eMSProgramorSubroutineName() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->eMSProgramorSubroutineName();
}

std::string EnergyManagementSystemMeteredOutputVariable::resourceType() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->resourceType();
}

std::string EnergyManagementSystemMeteredOutputVariable::groupType() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->groupType();
}

std::string EnergyManagementSystemMeteredOutputVariable::endUseCategory() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->endUseCategory();
}

boost::optional<std::string> EnergyManagementSystemMeteredOutputVariable::endUseSubcategory() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->endUseSubcategory();
}

boost::optional<std::string> EnergyManagementSystemMeteredOutputVariable::units() const {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->units();
}

bool EnergyManagementSystemMeteredOutputVariable::setEMSVariableName(const std::string& eMSVariableName) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setEMSVariableName(eMSVariableName);
}

bool EnergyManagementSystemMeteredOutputVariable::setUpdateFrequency(const std::string& updateFrequency) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setUpdateFrequency(updateFrequency);
}

bool EnergyManagementSystemMeteredOutputVariable::setEMSProgramorSubroutineName(const EnergyManagementSystemProgram& program) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setEMSProgramorSubroutineName(program);
}

bool EnergyManagementSystemMeteredOutputVariable::setEMSProgramorSubroutineName(const EnergyManagementSystemSubroutine& subroutine) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setEMSProgramorSubroutineName(subroutine);
}

void EnergyManagementSystemMeteredOutputVariable::resetEMSProgramorSubroutineName() {
  getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->resetEMSProgramorSubroutineName();
}

bool EnergyManagementSystemMeteredOutputVariable::setResourceType(const std::string& resourceType) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setResourceType(resourceType);
}

bool EnergyManagementSystemMeteredOutputVariable::setGroupType(const std::string& groupType) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setGroupType(groupType);
}

bool EnergyManagementSystemMeteredOutputVariable::setEndUseCategory(const std::string& endUseCategory) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setEndUseCategory(endUseCategory);
}

bool EnergyManagementSystemMeteredOutputVariable::setEndUseSubcategory(const std::string& endUseSubcategory) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void EnergyManagementSystemMeteredOutputVariable::resetEndUseSubcategory() {
  getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->resetEndUseSubcategory();
}

bool EnergyManagementSystemMeteredOutputVariable::setUnits(const std::string& units) {
  return getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->setUnits(units);
}

void EnergyManagementSystemMeteredOutputVariable::resetUnits() {
  getImpl<detail::EnergyManagementSystemMeteredOutputVariable_Impl>()->resetUnits();
}

/// @cond
EnergyManagementSystemMeteredOutputVariable::EnergyManagementSystemMeteredOutputVariable(std::shared_ptr<detail::EnergyManagementSystemMeteredOutputVariable_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

