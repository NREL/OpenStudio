/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
    for (const openstudio::path& searchPath : modelObject.searchPaths()) {
      IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();
      eg.setString(PythonPlugin_SearchPathsExtensibleFields::SearchPath, searchPath.generic_string());
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
