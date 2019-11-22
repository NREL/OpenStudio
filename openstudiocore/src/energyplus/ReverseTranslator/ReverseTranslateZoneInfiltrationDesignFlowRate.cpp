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

#include "../../model/SpaceInfiltrationDesignFlowRate.hpp"
#include "../../model/SpaceInfiltrationDesignFlowRate_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/ZoneInfiltration_DesignFlowRate_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateZoneInfiltrationDesignFlowRate( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::ZoneInfiltration_DesignFlowRate ){
    LOG(Error, "WorkspaceObject is not IddObjectType: ZoneInfiltration:DesignFlowRate");
    return boost::none;
  }

  openstudio::model::SpaceInfiltrationDesignFlowRate infiltration(m_model);

  OptionalString s = workspaceObject.name();
  if(s){
    infiltration.setName(*s);
  }

  OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::ZoneInfiltration_DesignFlowRateFields::ZoneorZoneListName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Space>()){
        infiltration.setSpace(modelObject->cast<Space>());
      }else if (modelObject->optionalCast<SpaceType>()){
        infiltration.setSpaceType(modelObject->cast<SpaceType>());
      }
    }
  }

  target = workspaceObject.getTarget(openstudio::ZoneInfiltration_DesignFlowRateFields::ScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (OptionalSchedule intermediate = modelObject->optionalCast<Schedule>()){
        Schedule schedule(*intermediate);
        infiltration.setSchedule(schedule);
      }
    }
  }

  s = workspaceObject.getString(openstudio::ZoneInfiltration_DesignFlowRateFields::DesignFlowRateCalculationMethod, true);
  OS_ASSERT(s);

  OptionalDouble d;
  if (istringEqual("Flow/Zone", *s)){
    d = workspaceObject.getDouble(openstudio::ZoneInfiltration_DesignFlowRateFields::DesignFlowRate);
    if (d){
      infiltration.setDesignFlowRate(*d);
    }else{
      LOG(Error, "Flow/Zone value not found for workspace object " << workspaceObject);
    }
  }else if(istringEqual("Flow/Area", *s)){
    d = workspaceObject.getDouble(openstudio::ZoneInfiltration_DesignFlowRateFields::FlowperZoneFloorArea);
    if (d){
      infiltration.setFlowperSpaceFloorArea(*d);
    }else{
      LOG(Error, "Flow/Area value not found for workspace object " << workspaceObject);
    }
  }else if(istringEqual("Flow/ExteriorArea", *s)){
    d = workspaceObject.getDouble(openstudio::ZoneInfiltration_DesignFlowRateFields::FlowperExteriorSurfaceArea);
    if (d){
      infiltration.setFlowperExteriorSurfaceArea(*d);
    }else{
      LOG(Error, "Flow/ExteriorArea value not found for workspace object " << workspaceObject);
    }
  }else if(istringEqual("Flow/ExteriorWallArea", *s)){
    d = workspaceObject.getDouble(openstudio::ZoneInfiltration_DesignFlowRateFields::FlowperExteriorSurfaceArea);
    if (d){
      infiltration.setFlowperExteriorWallArea(*d);
    }else{
      LOG(Error, "Flow/ExteriorWallArea value not found for workspace object " << workspaceObject);
    }
  }else if(istringEqual("AirChanges/Hour", *s)){
    d = workspaceObject.getDouble(openstudio::ZoneInfiltration_DesignFlowRateFields::AirChangesperHour);
    if (d){
      infiltration.setAirChangesperHour(*d);
    }else{
      LOG(Error, "AirChanges/Hour value not found for workspace object " << workspaceObject);
    }
  }else{
    LOG(Error, "Unknown DesignLevelCalculationMethod value for workspace object" << workspaceObject);
  }

  d = workspaceObject.getDouble(openstudio::ZoneInfiltration_DesignFlowRateFields::ConstantTermCoefficient);
  if (d){
    infiltration.setConstantTermCoefficient(*d);
  }

  d = workspaceObject.getDouble(openstudio::ZoneInfiltration_DesignFlowRateFields::TemperatureTermCoefficient);
  if (d){
    infiltration.setTemperatureTermCoefficient(*d);
  }

  d = workspaceObject.getDouble(openstudio::ZoneInfiltration_DesignFlowRateFields::VelocityTermCoefficient);
  if (d){
    infiltration.setVelocityTermCoefficient(*d);
  }

  d = workspaceObject.getDouble(openstudio::ZoneInfiltration_DesignFlowRateFields::VelocitySquaredTermCoefficient);
  if (d){
    infiltration.setVelocitySquaredTermCoefficient(*d);
  }

  return infiltration;
}

} // energyplus

} // openstudio

