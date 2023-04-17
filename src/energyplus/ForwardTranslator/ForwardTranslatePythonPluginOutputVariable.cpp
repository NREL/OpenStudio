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
