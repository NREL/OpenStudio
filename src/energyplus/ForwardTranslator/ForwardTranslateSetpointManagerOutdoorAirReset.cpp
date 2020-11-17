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
#include "../../model/SetpointManagerOutdoorAirReset.hpp"
#include "../../model/Node.hpp"
#include "../../model/Schedule.hpp"
#include <utilities/idd/SetpointManager_OutdoorAirReset_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerOutdoorAirReset(SetpointManagerOutdoorAirReset& modelObject) {
    boost::optional<Node> node;
    boost::optional<std::string> s;
    boost::optional<double> d;

    IdfObject idfObject(IddObjectType::SetpointManager_OutdoorAirReset);

    // Name
    s = modelObject.name();
    idfObject.setString(SetpointManager_OutdoorAirResetFields::Name, s.get());

    // ControlVariable
    s = modelObject.controlVariable();
    if (s) {
      idfObject.setString(SetpointManager_OutdoorAirResetFields::ControlVariable, s.get());
    }

    // SetpointatOutdoorLowTemperature
    d = modelObject.setpointatOutdoorLowTemperature();
    if (d) {
      idfObject.setDouble(SetpointManager_OutdoorAirResetFields::SetpointatOutdoorLowTemperature, d.get());
    }

    // OutdoorLowTemperature
    d = modelObject.outdoorLowTemperature();
    if (d) {
      idfObject.setDouble(SetpointManager_OutdoorAirResetFields::OutdoorLowTemperature, d.get());
    }

    // SetpointatOutdoorHighTemperature
    d = modelObject.setpointatOutdoorHighTemperature();
    if (d) {
      idfObject.setDouble(SetpointManager_OutdoorAirResetFields::SetpointatOutdoorHighTemperature, d.get());
    }

    // OutdoorHighTemperature
    d = modelObject.outdoorHighTemperature();
    if (d) {
      idfObject.setDouble(SetpointManager_OutdoorAirResetFields::OutdoorHighTemperature, d.get());
    }

    // SetpointNodeorNodeListName
    node = modelObject.setpointNode();
    if (node) {
      idfObject.setString(SetpointManager_OutdoorAirResetFields::SetpointNodeorNodeListName, node->name().get());
    }

    // Schedule
    boost::optional<model::Schedule> schedule = modelObject.schedule();
    if (schedule) {
      boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get());
      if (_schedule && _schedule->name()) {
        idfObject.setString(SetpointManager_OutdoorAirResetFields::ScheduleName, _schedule->name().get());
      }
    }

    // SetpointatOutdoorLowTemperature2
    d = modelObject.setpointatOutdoorLowTemperature2();
    if (d) {
      idfObject.setDouble(SetpointManager_OutdoorAirResetFields::SetpointatOutdoorLowTemperature2, d.get());
    }

    // OutdoorLowTemperature2
    d = modelObject.outdoorLowTemperature2();
    if (d) {
      idfObject.setDouble(SetpointManager_OutdoorAirResetFields::OutdoorLowTemperature2, d.get());
    }

    // SetpointatOutdoorHighTemperature2
    d = modelObject.setpointatOutdoorHighTemperature2();
    if (d) {
      idfObject.setDouble(SetpointManager_OutdoorAirResetFields::SetpointatOutdoorHighTemperature2, d.get());
    }

    // OutdoorHighTemperature2
    d = modelObject.outdoorHighTemperature2();
    if (d) {
      idfObject.setDouble(SetpointManager_OutdoorAirResetFields::OutdoorHighTemperature2, d.get());
    }

    m_idfObjects.push_back(idfObject);

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
