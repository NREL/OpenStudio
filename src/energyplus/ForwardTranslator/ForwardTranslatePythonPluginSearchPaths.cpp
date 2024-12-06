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

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/PythonPluginSearchPaths.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/PythonPlugin_SearchPaths_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

constexpr static auto pythonSearchPathsName = "Python Plugin Search Paths";

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translatePythonPluginSearchPaths(model::PythonPluginSearchPaths& modelObject) {

    auto searchPaths = modelObject.searchPaths();
    if (searchPaths.empty()) {
      return boost::none;
    }

    IdfObject idfObject = createAndRegisterIdfObject(openstudio::IddObjectType::PythonPlugin_SearchPaths, modelObject);

    idfObject.setName(pythonSearchPathsName);

    // Add Current Working Directory to Search Path: Optional Boolean
    if (modelObject.addCurrentWorkingDirectorytoSearchPath()) {
      idfObject.setString(PythonPlugin_SearchPathsFields::AddCurrentWorkingDirectorytoSearchPath, "Yes");
    } else {
      idfObject.setString(PythonPlugin_SearchPathsFields::AddCurrentWorkingDirectorytoSearchPath, "No");
    }

    // Add Input File Directory to Search Path: Optional Boolean
    if (modelObject.addInputFileDirectorytoSearchPath()) {
      idfObject.setString(PythonPlugin_SearchPathsFields::AddInputFileDirectorytoSearchPath, "Yes");
    } else {
      idfObject.setString(PythonPlugin_SearchPathsFields::AddInputFileDirectorytoSearchPath, "No");
    }

    // Add epin Environment Variable to Search Path: Optional Boolean
    if (modelObject.addepinEnvironmentVariabletoSearchPath()) {
      idfObject.setString(PythonPlugin_SearchPathsFields::AddepinEnvironmentVariabletoSearchPath, "Yes");
    } else {
      idfObject.setString(PythonPlugin_SearchPathsFields::AddepinEnvironmentVariabletoSearchPath, "No");
    }

    // Search Path
    for (const std::string& searchPath : modelObject.searchPaths()) {
      IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();
      eg.setString(PythonPlugin_SearchPathsExtensibleFields::SearchPath, searchPath);
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
