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
#include "../../model/SetpointManagerScheduledDualSetpoint.hpp"
#include "../../model/Node.hpp"
#include "../../model/Schedule.hpp"
#include <utilities/idd/SetpointManager_Scheduled_DualSetpoint_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerScheduledDualSetpoint( SetpointManagerScheduledDualSetpoint & modelObject )
{
  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SetpointManager_Scheduled_DualSetpoint, modelObject);

  // ControlVariable
  std::string s = modelObject.controlVariable();
  idfObject.setString(SetpointManager_Scheduled_DualSetpointFields::ControlVariable,s);

  // High Setpoint Schedule Name
  if( boost::optional<Schedule> highSetpointSchedule = modelObject.highSetpointSchedule() )
  {
    boost::optional<IdfObject> _highSetpointSchedule = translateAndMapModelObject(highSetpointSchedule.get());
    if( _highSetpointSchedule && _highSetpointSchedule->name() )
    {
      idfObject.setString(SetpointManager_Scheduled_DualSetpointFields::HighSetpointScheduleName,_highSetpointSchedule->name().get());
    }
  } else {
    LOG(Error, "SetpointManager:Scheduled:DualSetpoint '" << modelObject.name().get() << "' is missing required high setpoint schedule");
  }

  // Low Setpoint Schedule Name
  if( boost::optional<Schedule> lowSetpointSchedule = modelObject.lowSetpointSchedule() )
  {
    boost::optional<IdfObject> _lowSetpointSchedule = translateAndMapModelObject(lowSetpointSchedule.get());
    if( _lowSetpointSchedule && _lowSetpointSchedule->name() )
    {
      idfObject.setString(SetpointManager_Scheduled_DualSetpointFields::LowSetpointScheduleName,_lowSetpointSchedule->name().get());
    }
  } else {
    LOG(Error, "SetpointManager:Scheduled:DualSetpoint '" << modelObject.name().get() << "' is missing required low setpoint schedule");
  }

  // SetpointNodeorNodeListName
  if( boost::optional<Node> node = modelObject.setpointNode() )
  {
    idfObject.setString(SetpointManager_Scheduled_DualSetpointFields::SetpointNodeorNodeListName,node->name().get());
  }

  return idfObject;
}

} // energyplus

} // openstudio

