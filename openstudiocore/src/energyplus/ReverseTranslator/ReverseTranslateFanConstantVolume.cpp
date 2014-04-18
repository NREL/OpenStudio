/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include <energyplus/ReverseTranslator.hpp>

#include <model/FanConstantVolume.hpp>
#include <model/FanConstantVolume_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>

#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
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

