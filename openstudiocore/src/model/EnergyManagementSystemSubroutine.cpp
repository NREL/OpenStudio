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

  std::string EnergyManagementSystemSubroutine_Impl::body() const {
    //return program body as string

    // loop through extensible groups and add ProgramLine to body string.
    std::string body;
    boost::optional<std::string> comment;

    auto groups = extensibleGroups();
    for (auto group = groups.begin(); group != groups.end(); ++group) {
      //get program line content
      const auto & line = group->getString(OS_EnergyManagementSystem_SubroutineExtensibleFields::ProgramLine, true);
      OS_ASSERT(line);
      body += line.get();
      //get non-default comments if they exist
      comment = group->fieldComment(OS_EnergyManagementSystem_SubroutineExtensibleFields::ProgramLine, false);
      if (comment.is_initialized()) {
        //remove space after !
        boost::erase_first(comment.get(), " ");
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

  bool EnergyManagementSystemSubroutine_Impl::setBody(const std::string& body) {
    //set body of program to input string
    bool result = false;
    bool comment_result = false;
    //if body string empty then return false
    if (body.empty()) {
      return false;
    };

    //clobber existing body
    this->resetBody();

    // remove '\r' from the body string
    std::string body_minus_r = body;
    std::string::size_type pos = 0; // Must initialize
    while ((pos = body_minus_r.find("\r", pos)) != std::string::npos) {
      body_minus_r.erase(pos, 1);
    }

    //split the body string on newline characters and insert program line for each string line
    std::vector<std::string> body_minus_nl = splitString(body_minus_r, '\n');
    std::string body_minus_comment;
    std::vector<std::string> comments;
    std::string comment;

    //add program lines to body
    for (size_t i = 0; i < body_minus_nl.size(); i++) {
      //split string on comment character !
      comments = splitString(body_minus_nl.at(i), '!');
      //get extensibleGroup object
      WorkspaceExtensibleGroup group = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
      //make sure program line exists and insert
      if (comments.size() > 0) {
        //remove whitespace at end of comments[0]
        boost::trim_right(comments[0]);
        //remove whitespace at beginning of comments[0]
        boost::trim_left(comments[0]);
        //remove ,
        pos = 0;
        while ((pos = comments[0].find(",", pos)) != std::string::npos) {
          comments[0].erase(pos, 1);
        }
        //insert program line
        result = group.setString(OS_EnergyManagementSystem_SubroutineExtensibleFields::ProgramLine, comments[0]);
        if (!result) {
          return result;
        }
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
          comment_result = group.setFieldComment(OS_EnergyManagementSystem_SubroutineExtensibleFields::ProgramLine, comment);
        }
      } else {
        result = false;
      }
    }
    return result;
  }

  bool EnergyManagementSystemSubroutine_Impl::resetBody() {
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
      //remove whitespace at beginning of comments[0]
      boost::trim_left(comments[0]);
      //remove ,
      pos = 0;
      while ((pos = comments[0].find(",", pos)) != std::string::npos) {
        comments[0].erase(pos, 1);
      }
      //insert program line
      result = group.setString(OS_EnergyManagementSystem_SubroutineExtensibleFields::ProgramLine, comments[0]);
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
        comment_result = group.setFieldComment(OS_EnergyManagementSystem_SubroutineExtensibleFields::ProgramLine, comment);
      }
    } else {
      result = false;
    }    
    return result;
  }

  std::vector<std::string> EnergyManagementSystemSubroutine_Impl::lines() const {
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
      const auto & line = group->getString(OS_EnergyManagementSystem_SubroutineExtensibleFields::ProgramLine, true);
      OS_ASSERT(line);
      //add programLine to whole_line
      whole_line += line.get();
      //get non-default comments if they exist
      comment = group->fieldComment(OS_EnergyManagementSystem_SubroutineExtensibleFields::ProgramLine, false);
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

  bool EnergyManagementSystemSubroutine_Impl::setLines(const std::vector<std::string>& lines) {
    //set body of program to input vector of strings
    bool result = false;
    std::string temp;

    //if body string empty then return false
    if (lines.empty()) {
      return false;
    };

    //clobber existing body
    this->resetBody();

    //add program lines to body
    for (size_t i = 0; i < lines.size(); i++) {
      //use method addLine to add each line
      result = addLine(lines.at(i));
      if (!result) {
        return result;
      }
    }
    return result;
  }

  std::vector<ModelObject> EnergyManagementSystemSubroutine_Impl::referencedObjects() const {
    //return vector of model objects that are referenced in program
    std::vector<ModelObject> result;
    const int subs[] = {1};
    std::string possible_uid;

    const Model m = this->model();
    boost::optional<ModelObject> modelObject;

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
          modelObject = m.getModelObject<model::ModelObject>(uid);
          if (modelObject) {
            result.push_back(modelObject.get());
          }
        }
      }
    }
    return result;
  }

  std::vector<std::string> EnergyManagementSystemSubroutine_Impl::invalidReferencedObjects() const {
    //return vector of body lines that contain missing uid strings for invalid referenced objects
    std::vector<std::string> result;
    const int subs[] = {1};
    std::string possible_uid;

    const Model m = this->model();
    boost::optional<ModelObject> modelObject;

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
          modelObject = m.getModelObject<model::ModelObject>(uid);
          if (modelObject) {
            found++;
          }
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

EnergyManagementSystemSubroutine::EnergyManagementSystemSubroutine(const Model& model)
  : ModelObject(EnergyManagementSystemSubroutine::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::EnergyManagementSystemSubroutine_Impl>());
}

IddObjectType EnergyManagementSystemSubroutine::iddObjectType() {
  return IddObjectType(IddObjectType::OS_EnergyManagementSystem_Subroutine);
}


std::string EnergyManagementSystemSubroutine::body() const {
  return getImpl<detail::EnergyManagementSystemSubroutine_Impl>()->body();
}

bool EnergyManagementSystemSubroutine::setBody(const std::string& body) {
  return getImpl<detail::EnergyManagementSystemSubroutine_Impl>()->setBody(body);
}

bool EnergyManagementSystemSubroutine::resetBody() {
  return getImpl<detail::EnergyManagementSystemSubroutine_Impl>()->resetBody();
}

bool EnergyManagementSystemSubroutine::addLine(const std::string& line) {
  return getImpl<detail::EnergyManagementSystemSubroutine_Impl>()->addLine(line);
}

std::vector<std::string> EnergyManagementSystemSubroutine::lines() const {
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

