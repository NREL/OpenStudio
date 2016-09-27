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
#include "Model.hpp"

#include "../utilities/core/String.hpp"
#include "../utilities/core/StringHelpers.hpp"
#include "../utilities/core/UUID.hpp"
#include <boost/algorithm/string.hpp>

//#include "../utilities/idf/Handle.hpp"

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
    boost::optional<std::string> comment;

    auto groups = extensibleGroups();
    for (auto group = groups.begin(); group != groups.end(); ++group) {
      //get program line content
      const auto & line = group->getString(OS_EnergyManagementSystem_ProgramExtensibleFields::ProgramLine, true);
      OS_ASSERT(line);
      body += line.get();
      //get non-default comments if they exist
      comment = group->fieldComment(OS_EnergyManagementSystem_ProgramExtensibleFields::ProgramLine, false);
      if (comment.is_initialized()) {
        //remove space after !
        boost::erase_first(comment.get()," ");
        //add space between end of program line and comment
        if (!comment.get().empty()) {
          body += " " + comment.get();
        }
      }
      //add newline character
      body += '\n';
    }
    return body;
  }

  bool EnergyManagementSystemProgram_Impl::setBody(const std::string& body) {
    //set body of program to input string
    bool result = false;
    bool comment_result = false;
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

    //split the body string on newline characters and comments and insert program line for each string line
    std::vector<std::string> body_minus_nl = splitString(body_minus_r, '\n');
    std::string body_minus_comment;
    std::vector<std::string> comments;
    std::string comment;

    //add program lines to body
    std::vector<bool> ok(body_minus_nl.size(), false);
    for (size_t i = 0; i < body_minus_nl.size(); i++) {
      //split string on comment character !
      comments = splitString(body_minus_nl.at(i), '!');
      //get extensibleGroup object
      WorkspaceExtensibleGroup group = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
      //make sure program line exists and insert
      if (comments.size() > 0) {
        //remove whitespace at end of comments[0]
        boost::trim_right(comments[0]);
        //insert program line
        result = group.setString(OS_EnergyManagementSystem_ProgramExtensibleFields::ProgramLine, comments[0]);
        //check if comments exist
        if (comments.size() > 1) {
          //clear out the old comment
          comment.clear();
          //add comment to comment vector
          comment.append(comments.at(1));
          //combine any remaining comments for this line into one comment 
          if (comments.size() > 2) {
            for (size_t j = 2; j < comments.size(); j++) {
              comment.append("!" + comments.at(j));
            }
          }
          comment_result = group.setFieldComment(OS_EnergyManagementSystem_ProgramExtensibleFields::ProgramLine, comment);
        }
      } else {
        result = false;
      }
      ok.at(i) = result;
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
    bool comment_result = false;
    std::string comment;

    // remove '\r' from the line string
    std::string line_rn = line;
    std::string::size_type pos = 0; // Must initialize
    while ((pos = line_rn.find("\r", pos)) != std::string::npos) {
      line_rn.erase(pos, 1);
    }
    // remove '\n' 
    pos = 0;
    while ((pos = line_rn.find("\n", pos)) != std::string::npos) {
      line_rn.erase(pos, 1);
    }
    //get extensibleGroup object
    WorkspaceExtensibleGroup group = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();

    //split string on comment character !
    std::vector<std::string> comments = splitString(line_rn, '!');
    //make sure program line exists and insert
    if (comments.size() > 0) {
      //remove whitespace at end of comments[0]
      boost::trim_right(comments[0]);
      //insert program line
      result = group.setString(OS_EnergyManagementSystem_ProgramExtensibleFields::ProgramLine, comments[0]);
      //check if comments exist
      if (comments.size() > 1) {
        //clear out the old comment
        comment.clear();
        //add comment to comment vector
        comment.append(comments.at(1));
        //combine any remaining comments for this line into one comment 
        if (comments.size() > 2) {
          for (size_t j = 2; j < comments.size(); j++) {
            comment.append(", " + comments.at(j));
          }
        }
        comment_result = group.setFieldComment(OS_EnergyManagementSystem_ProgramExtensibleFields::ProgramLine, comment);
      }
    } else {
      result = false;
    }
    return result;
  }

  boost::optional<std::vector<std::string>> EnergyManagementSystemProgram_Impl::lines() const {
    //return vector of lines from body
    std::vector<std::string> result;
    boost::optional<std::string> comment;
    std::string whole_line;

    // loop through extensible groups and add ProgramLine to vector result.
    auto groups = extensibleGroups();

    for (auto group = groups.begin(); group != groups.end(); ++group) {
      //clear out whole_line
      whole_line.clear();
      //get program line content
      const auto & line = group->getString(OS_EnergyManagementSystem_ProgramExtensibleFields::ProgramLine, true);
      OS_ASSERT(line);
      //add programLine to whole_line
      whole_line += line.get();
      //get non-default comments if they exist
      comment = group->fieldComment(OS_EnergyManagementSystem_ProgramExtensibleFields::ProgramLine, false);
      if (comment.is_initialized()) {
        //remove space after !
        boost::erase_first(comment.get(), " ");
        //add space between end of program line and comment
        if (!comment.get().empty()) {
          whole_line += " " + comment.get();
        }
      }
      //add newline character
      whole_line += '\n';
      //add whole_line to vector
      result.push_back(whole_line);
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

    //add program lines to body
    std::vector<bool> ok(lines.size(), false);
    for (size_t i = 0; i < lines.size(); i++) {
      //use method addLine to add each line
      result = addLine(lines.at(i));
      ok.at(i) = result;
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

  std::vector<ModelObject> EnergyManagementSystemProgram_Impl::referencedObjects() const {
    //return vector of model objects that are referenced in program
    std::vector<ModelObject> result;
    const int subs[] = {1};
    std::string possible_uid;

    const Model m = this->model();
    const std::vector<model::ModelObject> modelObjects = m.getModelObjects<model::ModelObject>();

    boost::optional<std::vector<std::string>> body = this->lines();
    if (body.is_initialized()) {
      //assume body is a vector of strings
      for (size_t i = 0; i < body.get().size(); i++) {
        //find uids        
        boost::sregex_token_iterator j(body.get().at(i).begin(), body.get().at(i).end(), uuidInString(), subs);

        while (j != boost::sregex_token_iterator()) {
          possible_uid = *j++;
          //look to see if uid is in the model and return the object
          UUID uid = toUUID(possible_uid);
          //TODO cant get below to work so try the manual way
          //m.getModelObject<model::ModelObject>(uid);
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
    return result;
  }

  std::vector<std::string> EnergyManagementSystemProgram_Impl::invalidReferencedObjects() const {
    //return vector of body lines that contain missing uid strings for invalid referenced objects
    std::vector<std::string> result;
    const int subs[] = {1};
    std::string possible_uid;

    const Model m = this->model();
    const std::vector<model::ModelObject> modelObjects = m.getModelObjects<model::ModelObject>();

    boost::optional<std::vector<std::string>> body = this->lines();
    if (body.is_initialized()) {
      //assume body is a vector of strings
      for (size_t i = 0; i < body.get().size(); i++) {
        int found = 0;
        //find uids        
        boost::sregex_token_iterator j(body.get().at(i).begin(), body.get().at(i).end(), uuidInString(), subs);

        while (j != boost::sregex_token_iterator()) {
          possible_uid = *j++;
          found = 1;
          //look to see if uid is in the model and return the object
          UUID uid = toUUID(possible_uid);
          //TODO cant get below to work so try the manual way
          //m.getModelObjects<model::ModelObject>(&uid);
          if (modelObjects.size() > 0) {
            for (size_t k = 0; k < modelObjects.size(); k++) {
              if (modelObjects.at(k).handle() == uid) {
                found++;
              };
            }
          };
        }
        //possible uid NOT found in model
        if (found == 1) {
          result.push_back(body.get().at(i));
        };
      }
    }
    return result;
  }

} // detail

EnergyManagementSystemProgram::EnergyManagementSystemProgram(const Model& model)
  : ModelObject(EnergyManagementSystemProgram::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EnergyManagementSystemProgram_Impl>());
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

std::vector<ModelObject> EnergyManagementSystemProgram::referencedObjects() const {
  return getImpl<detail::EnergyManagementSystemProgram_Impl>()->referencedObjects();
}

std::vector<std::string> EnergyManagementSystemProgram::invalidReferencedObjects() const {
  return getImpl<detail::EnergyManagementSystemProgram_Impl>()->invalidReferencedObjects();
}

/// @cond
EnergyManagementSystemProgram::EnergyManagementSystemProgram(std::shared_ptr<detail::EnergyManagementSystemProgram_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

