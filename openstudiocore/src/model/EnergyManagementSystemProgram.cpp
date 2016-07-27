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

#include "EnergyManagementSystemProgram.hpp"
#include "EnergyManagementSystemProgram_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_Program_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include "ModelExtensibleGroup.hpp"

namespace openstudio {
namespace model {

namespace detail {

  EnergyManagementSystemProgram_Impl::EnergyManagementSystemProgram_Impl(const IdfObject& idfObject,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemProgram::iddObjectType());
  }

  EnergyManagementSystemProgram_Impl::EnergyManagementSystemProgram_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == EnergyManagementSystemProgram::iddObjectType());
  }

  EnergyManagementSystemProgram_Impl::EnergyManagementSystemProgram_Impl(const EnergyManagementSystemProgram_Impl& other,
                                                                         Model_Impl* model,
                                                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& EnergyManagementSystemProgram_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType EnergyManagementSystemProgram_Impl::iddObjectType() const {
    return EnergyManagementSystemProgram::iddObjectType();
  }

  boost::optional<std::string> EnergyManagementSystemProgram_Impl::body() const {
    //TODO return program body as string
    //Doesn't there need to be a loop here?
    //boost::optional<std::string> value = getString(OS_EnergyManagementSystem_ProgramExtensibleFields::ProgramLine, true);
    //OS_ASSERT(value);
    //return value.get();
    // loop through extensible groups
    std::string body;
    auto groups = extensibleGroups();
    for (auto group = groups.begin(); group != groups.end(); ++group) {
      const auto & line = group->getString(OS_EnergyManagementSystem_ProgramExtensibleFields::ProgramLine, true);
      OS_ASSERT(line);
      body += line.get();
    }

    return body;

  }

  bool EnergyManagementSystemProgram_Impl::setBody(const std::string& body) {
    //bool result = setString(OS_EnergyManagementSystem_ProgramFields::EnergyPlusModelCallingPoint, body);
    //TODO set body of program to input string
    bool result = true;
    return result;
  }

  bool EnergyManagementSystemProgram_Impl::addLine(const std::string& line) {
    //bool result = setString(OS_EnergyManagementSystem_ProgramFields::EnergyPlusModelCallingPoint, body);
    //TODO add line to end of program body
    bool result = true;
    return result;
  }

  boost::optional<std::vector<std::string>> EnergyManagementSystemProgram_Impl::lines() const {
    //return getString(OS_EnergyManagementSystem_ProgramFields::EnergyPlusModelCallingPoint, true);
    //TODO return lines from body
    std::vector<std::string> result;
    return result;
  }

  bool EnergyManagementSystemProgram_Impl::setLines(const std::vector<std::string>& lines) {
    //bool result = setString(OS_EnergyManagementSystem_ProgramFields::EnergyPlusModelCallingPoint, body);
    //TODO set body of program to input vector of strings
    bool result = true;
    return result;
  }

  boost::optional<std::vector<ModelObject>> EnergyManagementSystemProgram_Impl::referencedObjects() const {
    //return getString(OS_EnergyManagementSystem_ProgramFields::EnergyPlusModelCallingPoint, true);
    //TODO return vector of model objects that are referenced in program
    std::vector<ModelObject> result;
    return result;
  }

  boost::optional<std::vector<ModelObject>> EnergyManagementSystemProgram_Impl::invalidReferencedObjects() const {
    //return getString(OS_EnergyManagementSystem_ProgramFields::EnergyPlusModelCallingPoint, true);
    //TODO return vector of body lines that contain missing uid strings for invalid referenced objects
    std::vector<ModelObject> result;
    return result;
  }

} // detail

EnergyManagementSystemProgram::EnergyManagementSystemProgram(const Model& model)
  : ModelObject(EnergyManagementSystemProgram::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EnergyManagementSystemProgram_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
}

IddObjectType EnergyManagementSystemProgram::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EnergyManagementSystem_Program);
}

boost::optional<std::string> EnergyManagementSystemProgram::body() const {
  return getImpl<detail::EnergyManagementSystemProgram_Impl>()->body();
}

bool EnergyManagementSystemProgram::setBody(const std::string& body) {
  return getImpl<detail::EnergyManagementSystemProgram_Impl>()->setBody(body);
}

bool EnergyManagementSystemProgram::addLine(const std::string& line) {
  return getImpl<detail::EnergyManagementSystemProgram_Impl>()->addLine(line);
}

boost::optional<std::vector<std::string>> EnergyManagementSystemProgram::lines() const {
  return getImpl<detail::EnergyManagementSystemProgram_Impl>()->lines();
}

bool EnergyManagementSystemProgram::setLines(const std::vector<std::string>& lines) {
  return getImpl<detail::EnergyManagementSystemProgram_Impl>()->setLines(lines);
}

boost::optional<std::vector<ModelObject>> EnergyManagementSystemProgram::referencedObjects() const {
  return getImpl<detail::EnergyManagementSystemProgram_Impl>()->referencedObjects();
}

boost::optional<std::vector<ModelObject>> EnergyManagementSystemProgram::invalidReferencedObjects() const {
  return getImpl<detail::EnergyManagementSystemProgram_Impl>()->invalidReferencedObjects();
}

/// @cond
EnergyManagementSystemProgram::EnergyManagementSystemProgram(std::shared_ptr<detail::EnergyManagementSystemProgram_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

