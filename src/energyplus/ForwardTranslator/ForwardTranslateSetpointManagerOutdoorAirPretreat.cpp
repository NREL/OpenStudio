/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/SetpointManagerOutdoorAirPretreat.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem.hpp"
#include <utilities/idd/SetpointManager_OutdoorAirPretreat_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerOutdoorAirPretreat(SetpointManagerOutdoorAirPretreat& modelObject) {
    boost::optional<Node> node;
    boost::optional<ModelObject> mo;
    boost::optional<std::string> s;
    boost::optional<double> d;

    boost::optional<Node> referenceNode;
    boost::optional<Node> mixedAirNode;
    boost::optional<Node> outdoorAirNode;
    boost::optional<Node> returnAirNode;

    boost::optional<AirLoopHVAC> airloop = modelObject.airLoopHVAC();
    boost::optional<AirLoopHVACOutdoorAirSystem> outdoorAirSystem;
    if (airloop) {
      outdoorAirSystem = airloop->airLoopHVACOutdoorAirSystem();
    }

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SetpointManager_OutdoorAirPretreat, modelObject);

    // ControlVariable
    s = modelObject.controlVariable();
    if (s) {
      idfObject.setString(SetpointManager_OutdoorAirPretreatFields::ControlVariable, s.get());
    }

    // Minimum Setpoint Temperature
    d = modelObject.minimumSetpointTemperature();
    if (d) {
      idfObject.setDouble(SetpointManager_OutdoorAirPretreatFields::MinimumSetpointTemperature, d.get());
    }

    // Maximum Setpoint Temperature
    d = modelObject.maximumSetpointTemperature();
    if (d) {
      idfObject.setDouble(SetpointManager_OutdoorAirPretreatFields::MaximumSetpointTemperature, d.get());
    }

    // Minimum Setpoint Humidity Ratio
    d = modelObject.minimumSetpointHumidityRatio();
    if (d) {
      idfObject.setDouble(SetpointManager_OutdoorAirPretreatFields::MinimumSetpointHumidityRatio, d.get());
    }

    // Maximum Setpoint Humidity Ratio
    d = modelObject.maximumSetpointHumidityRatio();
    if (d) {
      idfObject.setDouble(SetpointManager_OutdoorAirPretreatFields::MaximumSetpointHumidityRatio, d.get());
    }

    // Reference Setpoint Node Name
    // Mixed Air Stream Node Name
    mixedAirNode = modelObject.mixedAirStreamNode();
    if (!mixedAirNode) {
      if (outdoorAirSystem && (mo = outdoorAirSystem->mixedAirModelObject())) {
        mixedAirNode = mo->optionalCast<Node>();
      }
    }

    referenceNode = modelObject.referenceSetpointNode();
    if (!referenceNode) {
      referenceNode = mixedAirNode;
    }
    if (referenceNode) {
      idfObject.setString(SetpointManager_OutdoorAirPretreatFields::ReferenceSetpointNodeName, referenceNode->name().get());
    }

    if (mixedAirNode) {
      idfObject.setString(SetpointManager_OutdoorAirPretreatFields::MixedAirStreamNodeName, mixedAirNode->name().get());
    }

    // Outdoor Air Stream Node Name
    outdoorAirNode = modelObject.outdoorAirStreamNode();
    if (!outdoorAirNode) {
      if (outdoorAirSystem && (mo = outdoorAirSystem->outdoorAirModelObject())) {
        outdoorAirNode = mo->optionalCast<Node>();
      }
    }
    if (outdoorAirNode) {
      idfObject.setString(SetpointManager_OutdoorAirPretreatFields::OutdoorAirStreamNodeName, outdoorAirNode->name().get());
    }

    // Return Air Stream Node Name
    returnAirNode = modelObject.returnAirStreamNode();
    if (!returnAirNode) {
      if (outdoorAirSystem && (mo = outdoorAirSystem->returnAirModelObject())) {
        returnAirNode = mo->optionalCast<Node>();
      }
    }
    if (returnAirNode) {
      idfObject.setString(SetpointManager_OutdoorAirPretreatFields::ReturnAirStreamNodeName, returnAirNode->name().get());
    }

    // Setpoint Node or NodeList Name
    node = modelObject.setpointNode();
    if (node) {
      idfObject.setString(SetpointManager_OutdoorAirPretreatFields::SetpointNodeorNodeListName, node->name().get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
