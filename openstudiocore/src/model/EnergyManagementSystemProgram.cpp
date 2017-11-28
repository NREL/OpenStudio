/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

  std::string EnergyManagementSystemProgram_Impl::body() const {
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
    this->resetBody();

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
        result = group.setString(OS_EnergyManagementSystem_ProgramExtensibleFields::ProgramLine, comments[0]);
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
          comment_result = group.setFieldComment(OS_EnergyManagementSystem_ProgramExtensibleFields::ProgramLine, comment);
        }
      } else {
        result = false;
      }
    }
    return result;
  }

  bool EnergyManagementSystemProgram_Impl::resetBody() {
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
      //remove whitespace at beginning of comments[0]
      boost::trim_left(comments[0]);
      //remove ,
      pos = 0;
      while ((pos = comments[0].find(",", pos)) != std::string::npos) {
        comments[0].erase(pos, 1);
      }
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

  std::vector<std::string> EnergyManagementSystemProgram_Impl::lines() const {
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

  std::vector<ModelObject> EnergyManagementSystemProgram_Impl::referencedObjects() const {
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

  std::vector<std::string> EnergyManagementSystemProgram_Impl::invalidReferencedObjects() const {
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
        //find uids
        boost::sregex_token_iterator j(body.get().at(i).begin(), body.get().at(i).end(), uuidInString(), subs);

        while (j != boost::sregex_token_iterator()) {
          possible_uid = *j++;
          //look to see if uid is in the model and return the object
          UUID uid = toUUID(possible_uid);
          modelObject = m.getModelObject<model::ModelObject>(uid);
          if (!modelObject) {
            result.push_back(body.get().at(i));
            break;
          }
        }
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

std::string EnergyManagementSystemProgram::body() const {
  return getImpl<detail::EnergyManagementSystemProgram_Impl>()->body();
}

bool EnergyManagementSystemProgram::setBody(const std::string& body) {
  return getImpl<detail::EnergyManagementSystemProgram_Impl>()->setBody(body);
}

bool EnergyManagementSystemProgram::resetBody() {
  return getImpl<detail::EnergyManagementSystemProgram_Impl>()->resetBody();
}

bool EnergyManagementSystemProgram::addLine(const std::string& line) {
  return getImpl<detail::EnergyManagementSystemProgram_Impl>()->addLine(line);
}

std::vector<std::string> EnergyManagementSystemProgram::lines() const {
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
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

