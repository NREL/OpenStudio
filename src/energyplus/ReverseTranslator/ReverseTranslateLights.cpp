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

#include "../../model/Lights.hpp"
#include "../../model/Lights_Impl.hpp"
#include "../../model/LightsDefinition.hpp"
#include "../../model/LightsDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/Lights_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateLights( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::Lights ){
    LOG(Error, "WorkspaceObject " << workspaceObject.briefDescription()
        << " is not IddObjectType: Lights");
    return boost::none;
  }

  // create the definition
  openstudio::model::LightsDefinition definition(m_model);

  OptionalString s = workspaceObject.name();
  if(s){
    definition.setName(*s + " Definition");
  }

  s = workspaceObject.getString(openstudio::LightsFields::DesignLevelCalculationMethod, true);
  OS_ASSERT(s);

  OptionalDouble d;
  if (istringEqual("LightingLevel", *s)){
    d = workspaceObject.getDouble(openstudio::LightsFields::LightingLevel);
    if (d){
      definition.setLightingLevel(*d);
    }else{
      LOG(Error, "LightingLevel value not found for workspace object " << workspaceObject);
    }
  }else if(istringEqual("Watts/Area", *s)){
    d = workspaceObject.getDouble(openstudio::LightsFields::WattsperZoneFloorArea);
    if (d){
      definition.setWattsperSpaceFloorArea(*d);
    }else{
      LOG(Error, "Watts/Area value not found for workspace object " << workspaceObject);
    }
  }else if(istringEqual("Watts/Person", *s)){
    d = workspaceObject.getDouble(openstudio::LightsFields::WattsperPerson);
    if (d){
      definition.setWattsperPerson(*d);
    }else{
      LOG(Error, "Watts/Person value not found for workspace object " << workspaceObject);
    }
  }else{
    LOG(Error, "Unknown DesignLevelCalculationMethod value for workspace object" << workspaceObject);
  }

  d = workspaceObject.getDouble(openstudio::LightsFields::ReturnAirFraction);
  if (d){
    definition.setReturnAirFraction(*d);
  }

  d = workspaceObject.getDouble(openstudio::LightsFields::FractionRadiant);
  if (d){
    definition.setFractionRadiant(*d);
  }

  d = workspaceObject.getDouble(openstudio::LightsFields::FractionVisible);
  if (d){
    definition.setFractionVisible(*d);
  }

  s = workspaceObject.getString(openstudio::LightsFields::ReturnAirFractionCalculatedfromPlenumTemperature);
  if (s){
    if (istringEqual("Yes", *s)){
      definition.setReturnAirFractionCalculatedfromPlenumTemperature(true);
    }else{
      definition.setReturnAirFractionCalculatedfromPlenumTemperature(false);
    }
  }

  d = workspaceObject.getDouble(openstudio::LightsFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient1);
  if (d){
    definition.setReturnAirFractionFunctionofPlenumTemperatureCoefficient1(*d);
  }

  d = workspaceObject.getDouble(openstudio::LightsFields::ReturnAirFractionFunctionofPlenumTemperatureCoefficient2);
  if (d){
    definition.setReturnAirFractionFunctionofPlenumTemperatureCoefficient2(*d);
  }

  // create the instance
  Lights lights(definition);

  s = workspaceObject.name();
  if(s){
    lights.setName(*s);
  }

  OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::LightsFields::ZoneorZoneListName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Space>()){
        lights.setSpace(modelObject->cast<Space>());
      }else if (modelObject->optionalCast<SpaceType>()){
        lights.setSpaceType(modelObject->cast<SpaceType>());
      }
    }
  }

  target = workspaceObject.getTarget(openstudio::LightsFields::ScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (OptionalSchedule intermediate = modelObject->optionalCast<Schedule>()){
        Schedule schedule(*intermediate);
        lights.setSchedule(schedule);
      }
    }
  }

  d = workspaceObject.getDouble(openstudio::LightsFields::FractionReplaceable);
  if (d){
    lights.setFractionReplaceable(*d);
  }

  s = workspaceObject.getString(openstudio::LightsFields::EndUseSubcategory);
  if(s){
    lights.setEndUseSubcategory(*s);
  }

  return lights;
}

} // energyplus

} // openstudio

