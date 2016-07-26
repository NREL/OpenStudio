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
#include "EnergyManagementSystemProgram.hpp"
#include "EnergyManagementSystemProgram_Impl.hpp"

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

  boost::optional<std::vector<EnergyManagementSystemProgram>> EnergyManagementSystemProgramCallingManager_Impl::programs() const {
    //return getString(OS_EnergyManagementSystem_ProgramCallingManagerFields::EnergyPlusModelCallingPoint, true);
    //TODO return vector of program names of type string
    boost::optional<std::vector<EnergyManagementSystemProgram>> result;
    return result;
  }

  boost::optional<EnergyManagementSystemProgram> EnergyManagementSystemProgramCallingManager_Impl::getProgram(int index) const {
    //return getString(OS_EnergyManagementSystem_ProgramCallingManagerFields::EnergyPlusModelCallingPoint, true);
    //TODO return program at input index 
    boost::optional<EnergyManagementSystemProgram> result;
    //std::string index_string = std::to_string(index);
    return result;
  }

  bool EnergyManagementSystemProgramCallingManager_Impl::eraseProgram(int index) {
    //bool result = setString(OS_EnergyManagementSystem_ProgramCallingManagerFields::EnergyPlusModelCallingPoint, callingPoint);
    //TODO erase program at index
    bool result = true;
    return result;
  }

  bool EnergyManagementSystemProgramCallingManager_Impl::erasePrograms() {
    //bool result = setString(OS_EnergyManagementSystem_ProgramCallingManagerFields::EnergyPlusModelCallingPoint, callingPoint);
    //TODO erase all programs in this calling manager
    bool result = true;
    return result;
  }

  bool EnergyManagementSystemProgramCallingManager_Impl::insertProgram(const EnergyManagementSystemProgram& program, int index) {
    //bool result = setString(OS_EnergyManagementSystem_ProgramCallingManagerFields::EnergyPlusModelCallingPoint, callingPoint);
    //TODO erase program at index
    bool result = true;
    return result;
  }

  bool EnergyManagementSystemProgramCallingManager_Impl::addProgram(const EnergyManagementSystemProgram& program) {
    //bool result = setString(OS_EnergyManagementSystem_ProgramCallingManagerFields::EnergyPlusModelCallingPoint, callingPoint);
    //TODO add program to end of vector of programs
    bool result = true;
    return result;
  }

  bool EnergyManagementSystemProgramCallingManager_Impl::setProgram(const EnergyManagementSystemProgram& program, int index) {
    //bool result = setString(OS_EnergyManagementSystem_ProgramCallingManagerFields::EnergyPlusModelCallingPoint, callingPoint);
    //TODO add program to end of vector of programs
    bool result = true;
    return result;
  }

  bool EnergyManagementSystemProgramCallingManager_Impl::setPrograms(const std::vector<const EnergyManagementSystemProgram>& programs) {
    //bool result = setString(OS_EnergyManagementSystem_ProgramCallingManagerFields::EnergyPlusModelCallingPoint, callingPoint);
    //TODO add programs to end of vector of programs
    bool result = true;
    return result;
  }

  std::vector<int> EnergyManagementSystemProgramCallingManager_Impl::nullPrograms() const {
    //return getString(OS_EnergyManagementSystem_ProgramCallingManagerFields::EnergyPlusModelCallingPoint, true);
    //TODO return vector of indices of null programs in the program vector
    std::vector<int> result;
    return result;
  }

  bool EnergyManagementSystemProgramCallingManager_Impl::removeNullPrograms() {
    //bool result = setString(OS_EnergyManagementSystem_ProgramCallingManagerFields::EnergyPlusModelCallingPoint, callingPoint);
    //TODO remove any null entries in the vector of programs
    bool result = true;
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

boost::optional<std::vector<EnergyManagementSystemProgram>> EnergyManagementSystemProgramCallingManager::programs() const {
  return getImpl<detail::EnergyManagementSystemProgramCallingManager_Impl>()->programs();
}

boost::optional<EnergyManagementSystemProgram> EnergyManagementSystemProgramCallingManager::getProgram(int index) const {
  return getImpl<detail::EnergyManagementSystemProgramCallingManager_Impl>()->getProgram(index);
}

bool EnergyManagementSystemProgramCallingManager::eraseProgram(int index) {
  return getImpl<detail::EnergyManagementSystemProgramCallingManager_Impl>()->eraseProgram(index);
}

bool EnergyManagementSystemProgramCallingManager::erasePrograms() {
  return getImpl<detail::EnergyManagementSystemProgramCallingManager_Impl>()->erasePrograms();
}

bool EnergyManagementSystemProgramCallingManager::insertProgram(const EnergyManagementSystemProgram& program, int index) {
  return getImpl<detail::EnergyManagementSystemProgramCallingManager_Impl>()->insertProgram(program, index);
}

bool EnergyManagementSystemProgramCallingManager::addProgram(const EnergyManagementSystemProgram& program) {
  return getImpl<detail::EnergyManagementSystemProgramCallingManager_Impl>()->addProgram(program);
}

bool EnergyManagementSystemProgramCallingManager::setProgram(const EnergyManagementSystemProgram& program, int index) {
  return getImpl<detail::EnergyManagementSystemProgramCallingManager_Impl>()->setProgram(program, index);
}

bool EnergyManagementSystemProgramCallingManager::setPrograms(const std::vector<const EnergyManagementSystemProgram>& programs) {
  return getImpl<detail::EnergyManagementSystemProgramCallingManager_Impl>()->setPrograms(programs);
}

std::vector<int> EnergyManagementSystemProgramCallingManager::nullPrograms() const {
  return getImpl<detail::EnergyManagementSystemProgramCallingManager_Impl>()->nullPrograms();
}

bool EnergyManagementSystemProgramCallingManager::removeNullPrograms() {
  return getImpl<detail::EnergyManagementSystemProgramCallingManager_Impl>()->removeNullPrograms();
}

/// @cond
EnergyManagementSystemProgramCallingManager::EnergyManagementSystemProgramCallingManager(std::shared_ptr<detail::EnergyManagementSystemProgramCallingManager_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

