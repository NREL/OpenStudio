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
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/DesignSpecificationOutdoorAir_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include <utilities/idd/DesignSpecification_OutdoorAir_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateDesignSpecificationOutdoorAir( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::DesignSpecification_OutdoorAir ){
    LOG(Error, "WorkspaceObject is not IddObjectType: DesignSpecification:OutdoorAir");
    return boost::none;
  }

  OptionalString outdoorAirMethod = workspaceObject.getString(DesignSpecification_OutdoorAirFields::OutdoorAirMethod, true);
  if (!outdoorAirMethod){
    LOG(Error, "No OutdoorAirMethod specified for DesignSpecification:OutdoorAir named '" << workspaceObject.name().get() << "'");
    return boost::none;
  }

  DesignSpecificationOutdoorAir result(m_model);

  OptionalString name = workspaceObject.name();
  if(name){
    result.setName(*name);
  }

  result.setOutdoorAirMethod(*outdoorAirMethod);

  boost::optional<double> flowPerPerson = workspaceObject.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson);
  boost::optional<double> flowPerArea = workspaceObject.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZoneFloorArea);
  boost::optional<double> flowPerZone = workspaceObject.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZone);
  boost::optional<double> ach = workspaceObject.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour);

  if (istringEqual(*outdoorAirMethod, "Flow/Person")){
    if (flowPerPerson){
      result.setOutdoorAirFlowperPerson(*flowPerPerson);
    }
  }else if (istringEqual(*outdoorAirMethod, "Flow/Area")){
    if (flowPerArea){
      result.setOutdoorAirFlowperFloorArea(*flowPerArea);
    }
  }else if (istringEqual(*outdoorAirMethod, "Flow/Zone")){
    if (flowPerZone){
      result.setOutdoorAirFlowRate(*flowPerZone);
    }
  }else if (istringEqual(*outdoorAirMethod, "AirChanges/Hour")){
    if (ach){
      result.setOutdoorAirFlowRate(*ach);
    }
  }else if (istringEqual(*outdoorAirMethod, "Sum") || istringEqual(*outdoorAirMethod, "Maximum")){

    if (flowPerPerson){
      result.setOutdoorAirFlowperPerson(*flowPerPerson);
    }
    if (flowPerArea){
      result.setOutdoorAirFlowperFloorArea(*flowPerArea);
    }
    if (flowPerZone){
      result.setOutdoorAirFlowRate(*flowPerZone);
    }
    if (ach){
      result.setOutdoorAirFlowRate(*ach);
    }

  }else{
    LOG(Error, "Unknown OutdoorAirMethod '" << *outdoorAirMethod << "' specified for DesignSpecification:OutdoorAir named '" << workspaceObject.name().get() << "'");
  }

  OptionalWorkspaceObject target = workspaceObject.getTarget(DesignSpecification_OutdoorAirFields::OutdoorAirScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (OptionalSchedule intermediate = modelObject->optionalCast<Schedule>()){
        Schedule schedule = *intermediate;
        result.setOutdoorAirFlowRateFractionSchedule(schedule);
      }
    }
  }

  return result;
}

} // energyplus

} // openstudio

