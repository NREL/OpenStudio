/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "../ForwardTranslator.hpp"
#include "../../model/WindowPropertyFrameAndDivider.hpp"
#include "../../model/SubSurface.hpp"

#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/WindowProperty_FrameAndDivider_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateWindowPropertyFrameAndDivider( 
  WindowPropertyFrameAndDivider & modelObject)
{
  IdfObject idfObject(IddObjectType::WindowProperty_FrameAndDivider);
  m_idfObjects.push_back(idfObject);
  /*
  for (LifeCycleCost lifeCycleCost : modelObject.lifeCycleCosts()){
    translateAndMapModelObject(lifeCycleCost);
  }

  // Name

  s = modelObject.name();
  if( s )
  {
    idfObject.setName(*s);
  }

  // EndUseSubcategory
  
  s = definition.endUseSubcategory(); 
  if( s )
  {
    idfObject.setString(WaterUse_EquipmentFields::EndUseSubcategory,s.get());
  }

  // PeakFlowRate
  
  value = definition.peakFlowRate();
  if( value )
  {
    idfObject.setDouble(WaterUse_EquipmentFields::PeakFlowRate,value.get());
  }

  // FlowRateFractionScheduleName
  
  if( boost::optional<Schedule> s = modelObject.flowRateFractionSchedule() )
  {
    translateAndMapModelObject(s.get());

    idfObject.setString(WaterUse_EquipmentFields::FlowRateFractionScheduleName,s->name().get());
  }

  // TargetTemperatureScheduleName
  
  if( boost::optional<Schedule> s = definition.targetTemperatureSchedule() )
  {
    translateAndMapModelObject(s.get());

    idfObject.setString(WaterUse_EquipmentFields::TargetTemperatureScheduleName,s->name().get());
  }

  // ZoneName

  if( boost::optional<Space> space = modelObject.space() )
  {
    if( boost::optional<ThermalZone> zone = space->thermalZone() )
    {
      translateAndMapModelObject(zone.get());

      idfObject.setString(WaterUse_EquipmentFields::ZoneName,zone->name().get());
    }
  }

  // SensibleFractionScheduleName
  
  if( boost::optional<Schedule> s = definition.sensibleFractionSchedule() )
  {
    translateAndMapModelObject(s.get());

    idfObject.setString(WaterUse_EquipmentFields::SensibleFractionScheduleName,s->name().get());
  }

  // LatentFractionScheduleName
  
  if( boost::optional<Schedule> s = definition.latentFractionSchedule() )
  {
    translateAndMapModelObject(s.get());

    idfObject.setString(WaterUse_EquipmentFields::LatentFractionScheduleName,s->name().get());
  }
  */
  return idfObject;
}

} // energyplus
} // openstudio

