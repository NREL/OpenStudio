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

#include "../ReverseTranslator.hpp"

#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/ZoneVentilation_DesignFlowRate_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateZoneVentilationDesignFlowRate(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::ZoneVentilation_DesignFlowRate) {
      LOG(Error, "WorkspaceObject is not IddObjectType: ZoneVentilation:DesignFlowRate");
      return boost::none;
    }

    return boost::none;

    /*
  openstudio::model::SpaceVentilationDesignFlowRate ventilation(m_model);

  OptionalString s = workspaceObject.name();
  if(s){
    ventilation.setName(*s);
  }

  OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::ZoneVentilation_DesignFlowRateFields::ZoneorZoneListName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Space>()){
        ventilation.setSpace(modelObject->cast<Space>());
      }else if (modelObject->optionalCast<SpaceType>()){
        ventilation.setSpaceType(modelObject->cast<SpaceType>());
      }
    }
  }

  target = workspaceObject.getTarget(openstudio::ZoneVentilation_DesignFlowRateFields::ScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Schedule>()){
        ventilation.setSchedule(modelObject->cast<Schedule>());
      }
    }
  }

  s = workspaceObject.getString(openstudio::ZoneVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod, true);
  OS_ASSERT(s);

  OptionalDouble d;
  if (istringEqual("Flow/Zone", *s)){
    d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::DesignFlowRate);
    if (d){
      ventilation.setDesignFlowRate(*d);
    }else{
      LOG(Error, "Flow/Zone value not found for workspace object " << workspaceObject);
    }
  }else if(istringEqual("Flow/Area", *s)){
    d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::FlowRateperZoneFloorArea);
    if (d){
      ventilation.setFlowRateperSpaceFloorArea(*d);
    }else{
      LOG(Error, "Flow/Area value not found for workspace object " << workspaceObject);
    }
  }else if(istringEqual("Flow/Person", *s)){
    d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::FlowRateperPerson);
    if (d){
      ventilation.setFlowRateperPerson(*d);
    }else{
      LOG(Error, "Flow/Person value not found for workspace object " << workspaceObject);
    }
  }else if(istringEqual("AirChanges/Hour", *s)){
    d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::AirChangesperHour);
    if (d){
      ventilation.setAirChangesperHour(*d);
    }else{
      LOG(Error, "AirChanges/Hour value not found for workspace object " << workspaceObject);
    }
  }else{
    LOG(Error, "Unknown DesignLevelCalculationMethod value for workspace object" << workspaceObject);
  }

  s = workspaceObject.getString(openstudio::ZoneVentilation_DesignFlowRateFields::VentilationType);
  if (s){
    ventilation.setVentilationType(*s);
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::FanPressureRise);
  if (d){
    ventilation.setFanPressureRise(*d);
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::FanTotalEfficiency);
  if (d){
    ventilation.setFanTotalEfficiency(*d);
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::ConstantTermCoefficient);
  if (d){
    ventilation.setConstantTermCoefficient(*d);
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::TemperatureTermCoefficient);
  if (d){
    ventilation.setTemperatureTermCoefficient(*d);
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::VelocityTermCoefficient);
  if (d){
    ventilation.setVelocityTermCoefficient(*d);
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::VelocitySquaredTermCoefficient);
  if (d){
    ventilation.setVelocitySquaredTermCoefficient(*d);
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::MinimumIndoorTemperature);
  if (d){
    ventilation.setMinimumIndoorTemperature(*d);
  }

  target = workspaceObject.getTarget(openstudio::ZoneVentilation_DesignFlowRateFields::MinimumIndoorTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Schedule>()){
        ventilation.setMinimumIndoorTemperatureSchedule(modelObject->cast<Schedule>());
      }
    }
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::MaximumIndoorTemperature);
  if (d){
    ventilation.setMaximumIndoorTemperature(*d);
  }

  target = workspaceObject.getTarget(openstudio::ZoneVentilation_DesignFlowRateFields::MaximumIndoorTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Schedule>()){
        ventilation.setMaximumIndoorTemperatureSchedule(modelObject->cast<Schedule>());
      }
    }
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::DeltaTemperature);
  if (d){
    ventilation.setDeltaTemperature(*d);
  }

  target = workspaceObject.getTarget(openstudio::ZoneVentilation_DesignFlowRateFields::DeltaTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Schedule>()){
        ventilation.setDeltaTemperatureSchedule(modelObject->cast<Schedule>());
      }
    }
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::MinimumOutdoorTemperature);
  if (d){
    ventilation.setMinimumOutdoorTemperature(*d);
  }

  target = workspaceObject.getTarget(openstudio::ZoneVentilation_DesignFlowRateFields::MinimumOutdoorTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Schedule>()){
        ventilation.setMinimumOutdoorTemperatureSchedule(modelObject->cast<Schedule>());
      }
    }
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::MaximumOutdoorTemperature);
  if (d){
    ventilation.setMaximumOutdoorTemperature(*d);
  }

  target = workspaceObject.getTarget(openstudio::ZoneVentilation_DesignFlowRateFields::MaximumOutdoorTemperatureScheduleName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Schedule>()){
        ventilation.setMaximumOutdoorTemperatureSchedule(modelObject->cast<Schedule>());
      }
    }
  }

  d = workspaceObject.getDouble(openstudio::ZoneVentilation_DesignFlowRateFields::MaximumWindSpeed);
  if (d){
    ventilation.setMaximumWindSpeed(*d);
  }

  return ventilation;
  */
  }

}  // namespace energyplus

}  // namespace openstudio
