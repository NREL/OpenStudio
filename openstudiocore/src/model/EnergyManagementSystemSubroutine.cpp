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

#include "EnergyManagementSystemSubroutine.hpp"
#include "EnergyManagementSystemSubroutine_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_Subroutine_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include "ModelExtensibleGroup.hpp"
#include "Model.hpp"

#include "../utilities/core/String.hpp"
#include "../utilities/core/StringHelpers.hpp"
#include "../utilities/core/UUID.hpp"

namespace openstudio {
namespace model {

namespace detail {

  EnergyManagementSystemSubroutine_Impl::EnergyManagementSystemSubroutine_Impl(const IdfObject& idfObject,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == EnergyManagementSystemSubroutine::iddObjectType());
  }

  EnergyManagementSystemSubroutine_Impl::EnergyManagementSystemSubroutine_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == EnergyManagementSystemSubroutine::iddObjectType());
  }

  EnergyManagementSystemSubroutine_Impl::EnergyManagementSystemSubroutine_Impl(const EnergyManagementSystemSubroutine_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& EnergyManagementSystemSubroutine_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType EnergyManagementSystemSubroutine_Impl::iddObjectType() const {
    return EnergyManagementSystemSubroutine::iddObjectType();
  }

  boost::optional<std::string> EnergyManagementSystemSubroutine_Impl::body() const {
    //return program body as string

    // loop through extensible groups and add ProgramLine to body string.
    std::string body;
    auto groups = extensibleGroups();
    for (auto group = groups.begin(); group != groups.end(); ++group) {
      const auto & line = group->getString(OS_EnergyManagementSystem_SubroutineExtensibleFields::ProgramLine, true);
      OS_ASSERT(line);
      body += line.get();
    }
    return body;
  }

  bool EnergyManagementSystemSubroutine_Impl::setBody(const std::string& body) {
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
    std::string::size_type pos = 0; // Must initialize
    while ((pos = body_minus_r.find("\r", pos)) != std::string::npos) {
      body_minus_r.erase(pos, 1);
    }

    //split the body string on newline characters and insert program line for each string line
    std::vector<std::string> body_minus_nl = splitString(body_minus_r, '\n');

    //add program lines to body
    std::vector<bool> ok(body_minus_nl.size(), false);
    for (size_t i = 0; i < body_minus_nl.size(); i++) {
      WorkspaceExtensibleGroup group = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
      result = group.setString(OS_EnergyManagementSystem_SubroutineExtensibleFields::ProgramLine, body_minus_nl.at(i) + '\n');
      ok.push_back(result);
    }
    //check if all the programs set true
    result = true;
    for (size_t i = 0; i<ok.size(); i++) {
      if (!ok.at(i)) {//the value is false
        result = false; //not all values in array are true
      }
    }
    return result;
  }

  bool EnergyManagementSystemSubroutine_Impl::eraseBody() {
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

  bool EnergyManagementSystemSubroutine_Impl::addLine(const std::string& line) {
    //add line to end of program body
    bool result = true;

    // remove '\r' from the line string
    std::string line_rn = line;
    std::string::size_type pos = 0; // Must initialize
    while ((pos = line_rn.find("\r", pos)) != std::string::npos) {
      line_rn.erase(pos, 1);
    }
    // remove '\n' since we add it back in below
    pos = 0;
    while ((pos = line_rn.find("\n", pos)) != std::string::npos) {
      line_rn.erase(pos, 1);
    }

    WorkspaceExtensibleGroup group = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
    result = group.setString(OS_EnergyManagementSystem_SubroutineExtensibleFields::ProgramLine, line_rn + '\n');
    return result;
  }

  boost::optional<std::vector<std::string>> EnergyManagementSystemSubroutine_Impl::lines() const {
    //return vector of lines from body
    std::vector<std::string> result;

    // loop through extensible groups and add ProgramLine to vector result.
    auto groups = extensibleGroups();

    for (auto group = groups.begin(); group != groups.end(); ++group) {
      const auto & line = group->getString(OS_EnergyManagementSystem_SubroutineExtensibleFields::ProgramLine, true);
      OS_ASSERT(line);
      result.push_back(line.get());
    }
    return result;
  }

  bool EnergyManagementSystemSubroutine_Impl::setLines(const std::vector<std::string>& lines) {
    //set body of program to input vector of strings
    bool result = false;
    std::string temp;

    //if body string empty then return false
    if (lines.empty()) {
      return false;
    };

    //clobber existing body
    this->eraseBody();

    //add program lines to body
    std::vector<bool> ok(lines.size(), false);
    for (size_t i = 0; i < lines.size(); i++) {
      //use method addLine to add each line
      result = addLine(lines.at(i));
      ok.push_back(result);
    }
    //check if all the programs set true
    result = true;
    for (size_t i = 0; i<ok.size(); i++) {
      if (!ok.at(i)) {//the value is false
        result = false; //not all values in array are true
      }
    }
    return result;
  }

  std::vector<ModelObject> EnergyManagementSystemSubroutine_Impl::referencedObjects() const {
    //return vector of model objects that are referenced in program
    std::vector<ModelObject> result;

    boost::optional<std::vector<std::string>> body = this->lines();
    if (body.is_initialized()) {
      //assume body is a vector of strings
      for (size_t i = 0; i < body.get().size(); i++) {
        //split string on whitespaces to isolate possible uids
        std::vector<std::string> results = splitString(body.get().at(i), ' ');
        for (size_t j = 0; j < results.size(); j++) {
          if (results.at(j).size() == 38) {
            //remove {} from uid string
            std::string possible_uid = results.at(j).substr(1, results.at(j).size() - 2);
            //look to see if uid is in the model and return the object
            UUID uid = toUUID(possible_uid);
            //std::vector<Handle> handle = &uid;
            Model m = this->model();
            //TODO cant get below to work so try the harder way
            //m.getModelObjects<model::ModelObject>(&uid);
            std::vector<model::ModelObject> modelObjects = m.getModelObjects<model::ModelObject>();
            if (modelObjects.size() > 0) {
              for (size_t k = 0; k < modelObjects.size(); k++) {
                if (modelObjects.at(k).handle() == uid) {
                  result.push_back(modelObjects.at(k));
                };
              }
            };
          }
        }
      }
    }
    return result;
  }

  std::vector<std::string> EnergyManagementSystemSubroutine_Impl::invalidReferencedObjects() const {
    //return vector of body lines that contain missing uid strings for invalid referenced objects
    std::vector<std::string> result;

    boost::optional<std::vector<std::string>> body = this->lines();
    if (body.is_initialized()) {
      //assume body is a vector of strings
      for (size_t i = 0; i < body.get().size(); i++) {
        int found = 0;
        //split string on whitespaces to isolate possible uids
        std::vector<std::string> results = splitString(body.get().at(i), ' ');
        for (size_t j = 0; j < results.size(); j++) {
          if (results.at(j).size() == 38) {
            //possible uid so set found to 1
            found = 1;
            //remove {} from uid string
            std::string possible_uid = results.at(j).substr(1, results.at(j).size() - 2);
            //look to see if uid is in the model and return the object
            UUID uid = toUUID(possible_uid);
            //std::vector<Handle> handle = &uid;
            Model m = this->model();
            //TODO cant get below to work so try the harder way
            //m.getModelObjects<model::ModelObject>(&uid);
            std::vector<model::ModelObject> modelObjects = m.getModelObjects<model::ModelObject>();
            if (modelObjects.size() > 0) {
              for (size_t k = 0; k < modelObjects.size(); k++) {
                if (modelObjects.at(k).handle() == uid) {
                  found++;
                };
              }
            };
          }
        }
        //possible uid not found in model
        if (found == 1) {
          result.push_back(body.get().at(i));
        };
      }
    }
    return result;
  }
} // detail

EnergyManagementSystemSubroutine::EnergyManagementSystemSubroutine(const Model& model)
  : ModelObject(EnergyManagementSystemSubroutine::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EnergyManagementSystemSubroutine_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
}

IddObjectType EnergyManagementSystemSubroutine::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EnergyManagementSystem_Subroutine);
}


boost::optional<std::string> EnergyManagementSystemSubroutine::body() const {
  return getImpl<detail::EnergyManagementSystemSubroutine_Impl>()->body();
}

bool EnergyManagementSystemSubroutine::setBody(const std::string& body) {
  return getImpl<detail::EnergyManagementSystemSubroutine_Impl>()->setBody(body);
}

bool EnergyManagementSystemSubroutine::eraseBody() {
  return getImpl<detail::EnergyManagementSystemSubroutine_Impl>()->eraseBody();
}

bool EnergyManagementSystemSubroutine::addLine(const std::string& line) {
  return getImpl<detail::EnergyManagementSystemSubroutine_Impl>()->addLine(line);
}

boost::optional<std::vector<std::string>> EnergyManagementSystemSubroutine::lines() const {
  return getImpl<detail::EnergyManagementSystemSubroutine_Impl>()->lines();
}

bool EnergyManagementSystemSubroutine::setLines(const std::vector<std::string>& lines) {
  return getImpl<detail::EnergyManagementSystemSubroutine_Impl>()->setLines(lines);
}

std::vector<ModelObject> EnergyManagementSystemSubroutine::referencedObjects() const {
  return getImpl<detail::EnergyManagementSystemSubroutine_Impl>()->referencedObjects();
}

std::vector<std::string> EnergyManagementSystemSubroutine::invalidReferencedObjects() const {
  return getImpl<detail::EnergyManagementSystemSubroutine_Impl>()->invalidReferencedObjects();
}

/// @cond
EnergyManagementSystemSubroutine::EnergyManagementSystemSubroutine(std::shared_ptr<detail::EnergyManagementSystemSubroutine_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

