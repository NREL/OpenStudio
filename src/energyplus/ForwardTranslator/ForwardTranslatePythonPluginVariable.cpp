/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/PythonPluginVariable.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/PythonPlugin_Variables_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include <algorithm>

using namespace openstudio::model;

constexpr static auto pythonVariablesName = "Python Plugin Variables";

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translatePythonPluginVariable(model::PythonPluginVariable& modelObject) {

    // Our objects are all translated to a single E+ PythonPlugin:Variables object which is extensible

    auto it = std::find_if(m_idfObjects.begin(), m_idfObjects.end(), [](auto& idfObject) { return idfObject.nameString() == pythonVariablesName; });
    if (it == m_idfObjects.end()) {
      auto& pluginVariables = m_idfObjects.emplace_back(openstudio::IddObjectType::PythonPlugin_Variables);
      pluginVariables.setName(pythonVariablesName);
      pluginVariables.pushExtensibleGroup({modelObject.nameString()});
      return pluginVariables;
    }

    it->pushExtensibleGroup({modelObject.nameString()});
    return *it;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
