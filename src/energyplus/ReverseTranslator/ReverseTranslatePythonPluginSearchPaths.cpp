/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/PythonPluginSearchPaths.hpp"

#include <utilities/idd/PythonPlugin_SearchPaths_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translatePythonPluginSearchPaths(const WorkspaceObject& workspaceObject) {

    // Instantiate an object of the class to store the values,
    // but we don't return it until we know it's ok
    // TODO: check constructor, it might need other objects
    openstudio::model::PythonPluginSearchPaths modelObject(m_model);

    // TODO: Note JM 2018-10-17
    // You are responsible for implementing any additional logic based on choice fields, etc.
    // The ReverseTranslator generator script is meant to facilitate your work, not get you 100% of the way

    // Name
    if (boost::optional<std::string> name_ = workspaceObject.name()) {
      modelObject.setName(name_.get());
    }

    // Add Current Working Directory to Search Path: Optional Boolean
    if (boost::optional<bool> addCurrentWorkingDirectorytoSearchPath_ =
          getString(PythonPlugin_SearchPathsFields::AddCurrentWorkingDirectorytoSearchPath, true)) {
      if (istringEqual("Yes", addCurrentWorkingDirectorytoSearchPath_.get())) {
        modelObject.setAddCurrentWorkingDirectorytoSearchPath(true);
      } else {
        modelObject.setAddCurrentWorkingDirectorytoSearchPath(false);
      }
    }

    // Add Input File Directory to Search Path: Optional Boolean
    if (boost::optional<bool> addInputFileDirectorytoSearchPath_ =
          getString(PythonPlugin_SearchPathsFields::AddInputFileDirectorytoSearchPath, true)) {
      if (istringEqual("Yes", addInputFileDirectorytoSearchPath_.get())) {
        modelObject.setAddInputFileDirectorytoSearchPath(true);
      } else {
        modelObject.setAddInputFileDirectorytoSearchPath(false);
      }
    }

    // Add epin Environment Variable to Search Path: Optional Boolean
    if (boost::optional<bool> addepinEnvironmentVariabletoSearchPath_ =
          getString(PythonPlugin_SearchPathsFields::AddepinEnvironmentVariabletoSearchPath, true)) {
      if (istringEqual("Yes", addepinEnvironmentVariabletoSearchPath_.get())) {
        modelObject.setAddepinEnvironmentVariabletoSearchPath(true);
      } else {
        modelObject.setAddepinEnvironmentVariabletoSearchPath(false);
      }
    }

    return modelObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
