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

#include "../../model/SetpointManagerSystemNodeResetHumidity.hpp"

// TODO: Check the following class names against object getters and setters.
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include <utilities/idd/SetpointManager_SystemNodeReset_Humidity_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateSetpointManagerSystemNodeResetHumidity(const WorkspaceObject& workspaceObject) {
    boost::optional<ModelObject> result;

    // Instantiate an object of the class to store the values,
    // but we don't return it until we know it's ok
    // TODO: check constructor, it might need other objects
    openstudio::model::SetpointManagerSystemNodeResetHumidity modelObject(m_model);

    // TODO: Note JM 2018-10-17
    // You are responsible for implementing any additional logic based on choice fields, etc.
    // The ReverseTranslator generator script is meant to facilitate your work, not get you 100% of the way

    // Name
    if (boost::optional<std::string> _name = workspaceObject.name()) {
      modelObject.setName(_name.get());
    }

    // Control Variable: Required String
    if (boost::optional<std::string> _controlVariable = workspaceObject.getString(SetpointManager_SystemNodeReset_HumidityFields::ControlVariable)) {
      modelObject.setControlVariable(_controlVariable.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Control Variable'");
      return result;
    }

    // Setpoint at Low Reference Humidity Ratio: Required Double
    if (boost::optional<double> _setpointatLowReferenceHumidityRatio =
          workspaceObject.getDouble(SetpointManager_SystemNodeReset_HumidityFields::SetpointatLowReferenceHumidityRatio)) {
      modelObject.setSetpointatLowReferenceHumidityRatio(_setpointatLowReferenceHumidityRatio.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Setpoint at Low Reference Humidity Ratio'");
      return result;
    }

    // Setpoint at High Reference Humidity Ratio: Required Double
    if (boost::optional<double> _setpointatHighReferenceHumidityRatio =
          workspaceObject.getDouble(SetpointManager_SystemNodeReset_HumidityFields::SetpointatHighReferenceHumidityRatio)) {
      modelObject.setSetpointatHighReferenceHumidityRatio(_setpointatHighReferenceHumidityRatio.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Setpoint at High Reference Humidity Ratio'");
      return result;
    }

    // Low Reference Humidity Ratio: Required Double
    if (boost::optional<double> _lowReferenceHumidityRatio =
          workspaceObject.getDouble(SetpointManager_SystemNodeReset_HumidityFields::LowReferenceHumidityRatio)) {
      modelObject.setLowReferenceHumidityRatio(_lowReferenceHumidityRatio.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Low Reference Humidity Ratio'");
      return result;
    }

    // High Reference Humidity Ratio: Required Double
    if (boost::optional<double> _highReferenceHumidityRatio =
          workspaceObject.getDouble(SetpointManager_SystemNodeReset_HumidityFields::HighReferenceHumidityRatio)) {
      modelObject.setHighReferenceHumidityRatio(_highReferenceHumidityRatio.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'High Reference Humidity Ratio'");
      return result;
    }

    // Reference Node Name: Required Node
    if ((_wo = workspaceObject.getTarget(SetpointManager_SystemNodeReset_HumidityFields::ReferenceNodeName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Node> _referenceNodeName = _mo->optionalCast<Node>()) {
          modelObject.setReferenceNodeName(_referenceNodeName.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Reference Node Name'");
        }
      } else {
        LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot reverse translate required object 'Reference Node Name'");
        return result;
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Reference Node Name'");
      return result;
    }
    // Setpoint Node or NodeList Name: Required Node
    if ((_wo = workspaceObject.getTarget(SetpointManager_SystemNodeReset_HumidityFields::SetpointNodeorNodeListName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Node> _setpointNodeorNodeListName = _mo->optionalCast<Node>()) {
          modelObject.setSetpointNodeorNodeListName(_setpointNodeorNodeListName.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Setpoint Node or NodeList Name'");
        }
      } else {
        LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot reverse translate required object 'Setpoint Node or NodeList Name'");
        return result;
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Setpoint Node or NodeList Name'");
      return result;
    }
    result = modelObject;
    return result;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio