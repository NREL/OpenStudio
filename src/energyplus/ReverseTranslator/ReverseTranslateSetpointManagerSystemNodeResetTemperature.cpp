/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../../model/SetpointManagerSystemNodeResetTemperature.hpp"

#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include <utilities/idd/SetpointManager_SystemNodeReset_Temperature_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateSetpointManagerSystemNodeResetTemperature(const WorkspaceObject& workspaceObject) {

    boost::optional<Node> setpointNode;
    if (boost::optional<std::string> setpointNodeName =
          workspaceObject.getString(SetpointManager_SystemNodeReset_TemperatureFields::SetpointNodeorNodeListName)) {
      setpointNode = m_model.getConcreteModelObjectByName<Node>(setpointNodeName.get());
    }

    if (!setpointNode) {
      LOG(Error, workspaceObject.briefDescription() << " is not attached to a Setpoint Node in the model");

      return boost::none;
    }

    boost::optional<Node> referenceNode;
    if (boost::optional<std::string> referenceNodeName =
          workspaceObject.getString(SetpointManager_SystemNodeReset_TemperatureFields::ReferenceNodeName)) {
      referenceNode = m_model.getConcreteModelObjectByName<Node>(referenceNodeName.get());
    }

    if (!referenceNode) {
      LOG(Error, workspaceObject.briefDescription() << " is not attached to a Reference Node in the model");

      return boost::none;
    }

    openstudio::model::SetpointManagerSystemNodeResetTemperature modelObject(m_model);

    // Name
    if (boost::optional<std::string> _name = workspaceObject.name()) {
      modelObject.setName(_name.get());
    }

    // Control Variable: Required String
    if (boost::optional<std::string> _controlVariable =
          workspaceObject.getString(SetpointManager_SystemNodeReset_TemperatureFields::ControlVariable)) {
      modelObject.setControlVariable(_controlVariable.get());
    } else {
      LOG(Warn, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Control Variable'");
    }

    // Setpoint at Low Reference Temperature: Required Double
    if (boost::optional<double> _setpointatLowReferenceTemperature =
          workspaceObject.getDouble(SetpointManager_SystemNodeReset_TemperatureFields::SetpointatLowReferenceTemperature)) {
      modelObject.setSetpointatLowReferenceTemperature(_setpointatLowReferenceTemperature.get());
    } else {
      LOG(Warn, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Setpoint at Low Reference Temperature'");
    }

    // Setpoint at High Reference Temperature: Required Double
    if (boost::optional<double> _setpointatHighReferenceTemperature =
          workspaceObject.getDouble(SetpointManager_SystemNodeReset_TemperatureFields::SetpointatHighReferenceTemperature)) {
      modelObject.setSetpointatHighReferenceTemperature(_setpointatHighReferenceTemperature.get());
    } else {
      LOG(Warn, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Setpoint at High Reference Temperature'");
    }

    // Low Reference Temperature: Required Double
    if (boost::optional<double> _lowReferenceTemperature =
          workspaceObject.getDouble(SetpointManager_SystemNodeReset_TemperatureFields::LowReferenceTemperature)) {
      modelObject.setLowReferenceTemperature(_lowReferenceTemperature.get());
    } else {
      LOG(Warn, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Low Reference Temperature'");
    }

    // High Reference Temperature: Required Double
    if (boost::optional<double> _highReferenceTemperature =
          workspaceObject.getDouble(SetpointManager_SystemNodeReset_TemperatureFields::HighReferenceTemperature)) {
      modelObject.setHighReferenceTemperature(_highReferenceTemperature.get());
    } else {
      LOG(Warn, "For " << workspaceObject.briefDescription() << ", cannot find required property 'High Reference Temperature'");
    }

    // Reference Node Name: Required Node
    modelObject.setReferenceNode(referenceNode.get());

    // Setpoint Node or NodeList Name: Required Node
    modelObject.addToNode(setpointNode.get());

    return modelObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
