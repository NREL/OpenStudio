/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/PythonPluginInstance.hpp"
#include "../../model/ExternalFile.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/PythonPlugin_Instance_FieldEnums.hxx>
#include <utilities/idd/PythonPlugin_SearchPaths_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include <algorithm>

constexpr static auto pythonSearchPathsName = "Python Plugin Search Paths";

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translatePythonPluginInstance(PythonPluginInstance& modelObject) {

    path filePath = modelObject.externalFile().filePath();
    if (!openstudio::filesystem::exists(filePath)) {
      LOG(Warn, modelObject.briefDescription() << " will not be translated, cannot find the referenced file '" << filePath << "'");
      return boost::none;
    }

    // make the path correct for this system
    filePath = openstudio::filesystem::system_complete(filePath);

    // If the plugin class name can't be found in the referenced external file, then it shouldn't be translated
    // We do it here and inside model, because between the time the user created the model object and now, the python file may have been modified
    std::string pluginClassName = modelObject.pluginClassName();
    if (!modelObject.findPluginClassNameInFile(pluginClassName)) {
      LOG(Warn, "PythonPluginInstance " << modelObject.name().get() << " does not have plugin class name '" << pluginClassName
                                        << "' contained in referenced external file");
      return boost::none;
    }

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::PythonPlugin_Instance, modelObject);

    // Run During Warmup Days
    if (modelObject.runDuringWarmupDays()) {
      idfObject.setString(openstudio::PythonPlugin_InstanceFields::RunDuringWarmupDays, "Yes");
    } else {
      idfObject.setString(openstudio::PythonPlugin_InstanceFields::RunDuringWarmupDays, "No");
    }

    // Python Module Name
    std::string pythonModuleName = toString(filePath.stem());
    idfObject.setString(openstudio::PythonPlugin_InstanceFields::PythonModuleName, pythonModuleName);

    // Python Class Name
    idfObject.setString(openstudio::PythonPlugin_InstanceFields::PluginClassName, pluginClassName);

    // PythonPlugin:SearchPaths
    std::string searchPath = toString(filePath.parent_path());

    auto it = std::find_if(m_idfObjects.begin(), m_idfObjects.end(), [](auto& idfObject) { return idfObject.nameString() == pythonSearchPathsName; });
    if (it == m_idfObjects.end()) {
      // First time: create it
      auto& pythonPluginSearchPaths = m_idfObjects.emplace_back(IddObjectType::PythonPlugin_SearchPaths);

      pythonPluginSearchPaths.setName(pythonSearchPathsName);
      // These are IDD defaults
      pythonPluginSearchPaths.setString(PythonPlugin_SearchPathsFields::AddCurrentWorkingDirectorytoSearchPath, "Yes");
      pythonPluginSearchPaths.setString(PythonPlugin_SearchPathsFields::AddInputFileDirectorytoSearchPath, "Yes");
      pythonPluginSearchPaths.setString(PythonPlugin_SearchPathsFields::AddepinEnvironmentVariabletoSearchPath, "Yes");

      // Just push the searchPath to it
      pythonPluginSearchPaths.pushExtensibleGroup({searchPath});
    } else {

      // Try to locate it before adding it
      bool newSearchPath = true;
      for (const IdfExtensibleGroup& eg : it->extensibleGroups()) {
        if (searchPath == eg.getString(0)) {
          newSearchPath = false;
          break;
        }
      }

      if (newSearchPath) {
        it->pushExtensibleGroup({searchPath});
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
