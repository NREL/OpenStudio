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

#include "../ReverseTranslator.hpp"

#include "../../model/FanConstantVolume.hpp"
#include "../../model/FanConstantVolume_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateFanConstantVolume( const WorkspaceObject & workspaceObject )
{
OptionalModelObject result,temp;
  OptionalSchedule schedule;

  OptionalWorkspaceObject owo = workspaceObject.getTarget(Fan_ConstantVolumeFields::AvailabilityScheduleName);
  if(!owo)
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             << " Can't find Schedule: ");
    return result;
  }
  temp = translateAndMapWorkspaceObject( *owo);
  if(temp)
  {
    schedule = temp->optionalCast<Schedule>();
  }

  if( !schedule )
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.name().get()
             <<"Failed to convert iddObjects into model Objects. Maybe they do not exist in model yet");

    return result;
  }

  openstudio::model::FanConstantVolume fan( m_model, *schedule );
  OptionalString optS = workspaceObject.name();
  if(optS)
  {
    fan.setName(*optS);
  }
  //inlet and outlet nodes are set my the HVACAirLoop
  OptionalDouble d;
  d = workspaceObject.getDouble(openstudio::Fan_ConstantVolumeFields::FanTotalEfficiency);
  if(d)
  {
    fan.setFanEfficiency(*d);
  }
  d = workspaceObject.getDouble(openstudio::Fan_ConstantVolumeFields::PressureRise);
  if(d)
  {
    fan.setPressureRise(*d);
  }
  d = workspaceObject.getDouble(openstudio::Fan_ConstantVolumeFields::MaximumFlowRate);
  if(d)
  {
    fan.setMaximumFlowRate(*d);
  }
  d = workspaceObject.getDouble(openstudio::Fan_ConstantVolumeFields::MotorEfficiency);
  if(d)
  {
    fan.setMotorEfficiency(*d);
  }
  d = workspaceObject.getDouble(openstudio::Fan_ConstantVolumeFields::MotorInAirstreamFraction);
  if(d)
  {
    fan.setMotorInAirstreamFraction(*d);
  }

  optS=workspaceObject.getString(openstudio::Fan_ConstantVolumeFields::EndUseSubcategory);
  if(optS)
  {
    fan.setEndUseSubcategory(*optS);
  }

  result=fan;
  return result;
}

} // energyplus

} // openstudio

