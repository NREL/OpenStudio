/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/SetpointManagerWarmestTemperatureFlow.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PortList.hpp"
#include <utilities/idd/SetpointManager_WarmestTemperatureFlow_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerWarmestTemperatureFlow( SetpointManagerWarmestTemperatureFlow & modelObject )
{
  std::string s;
  double n;
  boost::optional<Node> node;

  IdfObject idfObject(IddObjectType::SetpointManager_WarmestTemperatureFlow);

  m_idfObjects.push_back(idfObject);

  // Name
  s = modelObject.name().get();
  idfObject.setString(SetpointManager_WarmestTemperatureFlowFields::Name,s);

  // ControlVariable
  idfObject.setString(SetpointManager_WarmestTemperatureFlowFields::ControlVariable,"Temperature");

  // HVACAirLoopName
  if( auto node = modelObject.setpointNode() ) {
    if( auto airLoop = node->airLoopHVAC() ) {
      idfObject.setString(SetpointManager_WarmestTemperatureFlowFields::HVACAirLoopName,airLoop->name().get());
    }
  }

  // Strategy
  s = modelObject.strategy();
  idfObject.setString(SetpointManager_WarmestTemperatureFlowFields::Strategy,s);

  // MinimumSupplyAirTemperature
  n = modelObject.minimumSetpointTemperature();
  idfObject.setDouble(SetpointManager_WarmestTemperatureFlowFields::MinimumSetpointTemperature,n);

  // MaximumSupplyAirTemperature
  n = modelObject.maximumSetpointTemperature();
  idfObject.setDouble(SetpointManager_WarmestTemperatureFlowFields::MaximumSetpointTemperature,n);

  // SetpointNodeorNodeListName
  node = modelObject.setpointNode();
  if( node ) {
    idfObject.setString(SetpointManager_WarmestTemperatureFlowFields::SetpointNodeorNodeListName,node->name().get());
  }

  // MinimumTurndownRatio
  n = modelObject.minimumTurndownRatio();
  idfObject.setDouble(SetpointManager_WarmestTemperatureFlowFields::MinimumTurndownRatio,n);

  return idfObject;
}

} // energyplus

} // openstudio

