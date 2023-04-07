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

#include "../../model/SetpointManagerSystemNodeResetTemperature.hpp"

#include "../../model/Node.hpp"

#include <utilities/idd/SetpointManager_SystemNodeReset_Temperature_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateSetpointManagerSystemNodeResetTemperature(model::SetpointManagerSystemNodeResetTemperature& modelObject) {

    // Translation is triggered via the setpointNode already, so no need to check
    if (!modelObject.referenceNode()) {
      LOG(Error, modelObject.briefDescription() << " does not have a Reference Node attached, it will be not be translated.");
      return boost::none;
    }

    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SetpointManager_SystemNodeReset_Temperature, modelObject);

    // Control Variable: Required String
    std::string controlVariable = modelObject.controlVariable();
    idfObject.setString(SetpointManager_SystemNodeReset_TemperatureFields::ControlVariable, controlVariable);

    // Setpoint at Low Reference Temperature: Required Double
    double setpointatLowReferenceTemperature = modelObject.setpointatLowReferenceTemperature();
    idfObject.setDouble(SetpointManager_SystemNodeReset_TemperatureFields::SetpointatLowReferenceTemperature, setpointatLowReferenceTemperature);

    // Setpoint at High Reference Temperature: Required Double
    double setpointatHighReferenceTemperature = modelObject.setpointatHighReferenceTemperature();
    idfObject.setDouble(SetpointManager_SystemNodeReset_TemperatureFields::SetpointatHighReferenceTemperature, setpointatHighReferenceTemperature);

    // Low Reference Temperature: Required Double
    double lowReferenceTemperature = modelObject.lowReferenceTemperature();
    idfObject.setDouble(SetpointManager_SystemNodeReset_TemperatureFields::LowReferenceTemperature, lowReferenceTemperature);

    // High Reference Temperature: Required Double
    double highReferenceTemperature = modelObject.highReferenceTemperature();
    idfObject.setDouble(SetpointManager_SystemNodeReset_TemperatureFields::HighReferenceTemperature, highReferenceTemperature);

    // Reference Node Name: Required Node (but we checked it above)
    idfObject.setString(SetpointManager_SystemNodeReset_TemperatureFields::ReferenceNodeName, modelObject.referenceNode()->nameString());

    // Setpoint Node or NodeList Name: Required Node (but we know it exists)
    idfObject.setString(SetpointManager_SystemNodeReset_TemperatureFields::SetpointNodeorNodeListName, modelObject.setpointNode()->nameString());

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
