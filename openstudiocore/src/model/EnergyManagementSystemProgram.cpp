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

#include "../utilities/core/String.hpp"
#include "../utilities/core/StringHelpers.hpp"

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
    //return program body as string

    // loop through extensible groups and add ProgramLine to body string.
    std::string body;
    auto groups = extensibleGroups();
    for (auto group = groups.begin(); group != groups.end(); ++group) {
      const auto & line = group->getString(OS_EnergyManagementSystem_ProgramExtensibleFields::ProgramLine, true);
      OS_ASSERT(line);
      body += line.get();
    }
    return body;
  }

  bool EnergyManagementSystemProgram_Impl::setBody(std::string& body) {
    //set body of program to input string
    bool result = false;
    //if body string empty then return false
    if (body.empty()) {
      return false;
    };

    //clobber existing body
    this->eraseBody();

    // remove '\r' from the body string
    std::string body_minus_r = body;
    body_minus_r.erase(std::remove(body.begin(), body.end(), '\r'), body.end());

    //split the body string on newline characters and insert program line for each string line
    std::vector<std::string> body_minus_nl = splitString(body_minus_r, '\n');

    //add program lines to body
    std::vector<bool> ok(body_minus_nl.size(), false);
    for (int i = 0; i < body_minus_nl.size(); i++) {
      WorkspaceExtensibleGroup group = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
      result = group.setString(OS_EnergyManagementSystem_ProgramExtensibleFields::ProgramLine, body_minus_nl[i]);
      ok.push_back(result);
    }
    //check if all the programs set true
    result = true;
    for (int i = 0; i<ok.size(); i++) {
      if (!ok[i]) {//the value is false
        result = false; //not all values in array are true
      }
    }
    return result;
  }

  bool EnergyManagementSystemProgram_Impl::eraseBody() {
    //erase body of program
    bool results = false;
    std::vector< std::vector<std::string> > result;
    result = clearExtensibleGroups();
    // if result is not empty, then the group was cleared out since the old values were returned
    if (!result.empty()) {
      results = true;
    };
    return results;
  }

  bool EnergyManagementSystemProgram_Impl::addLine(const std::string& line) {
    //add line to end of program body
    bool result = true;
    WorkspaceExtensibleGroup group = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
    result = group.setString(OS_EnergyManagementSystem_ProgramExtensibleFields::ProgramLine, line);
    return result;
  }

  boost::optional<std::vector<std::string>> EnergyManagementSystemProgram_Impl::lines() const {
    //return vector of lines from body
    std::vector<std::string> result;

    // loop through extensible groups and add ProgramLine to vector result.
    auto groups = extensibleGroups();
    unsigned i = 0;
    for (auto group = groups.begin(); group != groups.end(); ++group) {
      const auto & line = group->getString(OS_EnergyManagementSystem_ProgramExtensibleFields::ProgramLine, true);
      OS_ASSERT(line);
      result[i] += line.get();
      i++;
    }
    return result;
  }

  bool EnergyManagementSystemProgram_Impl::setLines(const std::vector<std::string>& lines) {
    //set body of program to input vector of strings
    bool result = false;
    std::string temp;

    //if body string empty then return false
    if (lines.empty()) {
      return false;
    };

    //clobber existing body
    this->eraseBody();

    // remove '\r' from the body string
    //for (int i = 0; i < lines.size(); i++) {
    //  temp = lines[i];
    //  lines[i] = temp.erase(std::remove(lines[i].begin(), lines[i].end(), '\r'), lines[i].end());
    //}
    //add program lines to body
    std::vector<bool> ok(lines.size(), false);
    for (int i = 0; i < lines.size(); i++) {
      WorkspaceExtensibleGroup group = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
      result = group.setString(OS_EnergyManagementSystem_ProgramExtensibleFields::ProgramLine, lines[i]);
      ok.push_back(result);
    }
    //check if all the programs set true
    result = true;
    for (int i = 0; i<ok.size(); i++) {
      if (!ok[i]) {//the value is false
        result = false; //not all values in array are true
      }
    }
    return result;
  }

  boost::optional<std::vector<ModelObject>> EnergyManagementSystemProgram_Impl::referencedObjects() const {
    //return getString(OS_EnergyManagementSystem_ProgramFields::EnergyPlusModelCallingPoint, true);
    //TODO return vector of model objects that are referenced in program
    std::vector<ModelObject> result;

    std::vector<std::string> body;
    //test vector
    std::string test;
    test = "blah";
    body.push_back(test);
    test = "set temp = 92093437-80c7-47e2-82bf-015241c60012";
    body.push_back(test);
    test = "blah";
    body.push_back(test);

    //assume body is a vector of strings
    for (int i = 0; i < body.size(); i++) {
      std::vector<std::string> results = splitString(body[i], ' ');
      for (int j = 0; j < results.size(); j++) {
        if (results[j].size() == 36) {
          std::string possible_uid = results[j];
          //look to see if uid is in the model and return the object
        }
      }
    }
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

bool EnergyManagementSystemProgram::setBody(std::string& body) {
  return getImpl<detail::EnergyManagementSystemProgram_Impl>()->setBody(body);
}

bool EnergyManagementSystemProgram::eraseBody() {
  return getImpl<detail::EnergyManagementSystemProgram_Impl>()->eraseBody();
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

