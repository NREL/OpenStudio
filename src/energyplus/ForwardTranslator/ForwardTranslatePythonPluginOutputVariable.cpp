/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/PythonPluginOutputVariable.hpp"
#include "../../model/PythonPluginVariable.hpp"

#include <utilities/idd/PythonPlugin_OutputVariable_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translatePythonPluginOutputVariable(model::PythonPluginOutputVariable& modelObject) {
    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::PythonPlugin_OutputVariable, modelObject);

    // Python Plugin Variable Name: Required String
    idfObject.setString(PythonPlugin_OutputVariableFields::PythonPluginVariableName, modelObject.pythonPluginVariable().nameString());

    // Type of Data in Variable: Required String
    std::string typeofDatainVariable = modelObject.typeofDatainVariable();
    idfObject.setString(PythonPlugin_OutputVariableFields::TypeofDatainVariable, typeofDatainVariable);

    // Update Frequency: Required String
    std::string updateFrequency = modelObject.updateFrequency();
    idfObject.setString(PythonPlugin_OutputVariableFields::UpdateFrequency, updateFrequency);

    // Units: boost::optional<std::string>
    if (boost::optional<std::string> _units = modelObject.units()) {
      idfObject.setString(PythonPlugin_OutputVariableFields::Units, _units.get());
    }

    // Resource Type: boost::optional<std::string>
    if (boost::optional<std::string> _resourceType = modelObject.resourceType()) {
      idfObject.setString(PythonPlugin_OutputVariableFields::ResourceType, _resourceType.get());
    }

    // Group Type: boost::optional<std::string>
    if (boost::optional<std::string> _groupType = modelObject.groupType()) {
      idfObject.setString(PythonPlugin_OutputVariableFields::GroupType, _groupType.get());
    }

    // End-Use Category: boost::optional<std::string>
    if (boost::optional<std::string> _endUseCategory = modelObject.endUseCategory()) {
      idfObject.setString(PythonPlugin_OutputVariableFields::EndUseCategory, _endUseCategory.get());
    }

    // End-Use Subcategory: boost::optional<std::string>
    if (boost::optional<std::string> _endUseSubcategory = modelObject.endUseSubcategory()) {
      idfObject.setString(PythonPlugin_OutputVariableFields::EndUseSubcategory, _endUseSubcategory.get());
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
