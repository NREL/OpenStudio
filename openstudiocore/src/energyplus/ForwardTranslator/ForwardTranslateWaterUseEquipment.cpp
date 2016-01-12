/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
#include "../../model/WaterUseEquipment.hpp"
#include "../../model/WaterUseEquipment_Impl.hpp"
#include "../../model/WaterUseEquipmentDefinition.hpp"
#include "../../model/WaterUseEquipmentDefinition_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/LifeCycleCost.hpp"

#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/WaterUse_Equipment_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateWaterUseEquipment( 
    WaterUseEquipment & modelObject)
{
  boost::optional<std::string> s;
  boost::optional<double> value;
  OptionalSchedule schedule;

  model::WaterUseEquipmentDefinition definition = modelObject.waterUseEquipmentDefinition();

  IdfObject idfObject(IddObjectType::WaterUse_Equipment);
  m_idfObjects.push_back(idfObject);

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

  return idfObject;
}

} // energyplus
} // openstudio

