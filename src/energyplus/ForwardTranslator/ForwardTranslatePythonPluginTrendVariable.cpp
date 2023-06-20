/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/PythonPluginTrendVariable.hpp"
#include "../../model/PythonPluginVariable.hpp"

#include <utilities/idd/PythonPlugin_TrendVariable_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translatePythonPluginTrendVariable(model::PythonPluginTrendVariable& modelObject) {
    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::PythonPlugin_TrendVariable, modelObject);

    // Name of a Python Plugin Variable: Required String
    idfObject.setString(PythonPlugin_TrendVariableFields::NameofaPythonPluginVariable, modelObject.pythonPluginVariable().nameString());

    // Number of Timesteps to be Logged: Required Integer
    int numberofTimestepstobeLogged = modelObject.numberofTimestepstobeLogged();
    idfObject.setInt(PythonPlugin_TrendVariableFields::NumberofTimestepstobeLogged, numberofTimestepstobeLogged);

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
