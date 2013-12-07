/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <energyplus/ForwardTranslator.hpp>
#include <utilities/idf/IdfExtensibleGroup.hpp>
#include <model/ZoneHVACEquipmentList.hpp>
#include <model/ZoneHVACEquipmentList_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/ThermalZone.hpp>
#include <model/RefrigerationAirChiller.hpp>
#include <model/RefrigerationAirChiller_Impl.hpp>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/ZoneHVAC_EquipmentList_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_RefrigerationChillerSet_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneHVACEquipmentList( ZoneHVACEquipmentList & modelObject )
{
  OptionalString s;
  OptionalDouble d;
  OptionalModelObject temp;

  std::vector<ModelObject> objects = modelObject.equipment();

  if (objects.empty()){
    // do not write out this object
    return boost::none;
  }

  IdfObject idfObject(IddObjectType::ZoneHVAC_EquipmentList);

  // Name
  s = modelObject.name();
  if(s)
  {
    idfObject.setName(*s);
  }

  std::vector<ModelObject> airChillers;

  for( std::vector<ModelObject>::iterator it = objects.begin();
       it != objects.end();
       it++ )
  {
    if (boost::optional<RefrigerationAirChiller> airChiller = it->optionalCast<RefrigerationAirChiller>())
    {
      airChillers.push_back(airChiller.get());
      continue;
    }

    unsigned coolingPriority = modelObject.coolingPriority(*it);
    unsigned heatingPriority = modelObject.heatingPriority(*it);

    boost::optional<IdfObject> _equipment = translateAndMapModelObject(*it);

    if( _equipment )
    {
      IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();

      eg.setString(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentObjectType,_equipment->iddObject().name()); 
      eg.setString(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentName,_equipment->name().get()); 
      eg.setUnsigned(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentCoolingSequence,coolingPriority); 
      eg.setUnsigned(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentHeatingorNoLoadSequence,heatingPriority); 
    }
  }

  if (!airChillers.empty()) {
    // ZoneHVAC:RefrigerationChillerSet
    // Name
      IdfObject _chillerSet(IddObjectType::ZoneHVAC_RefrigerationChillerSet);

      m_idfObjects.push_back(_chillerSet);

      _chillerSet.setName("ZoneHVAC Refrigeration Chiller Set");

    // AvailabilityScheduleName
      boost::optional<Schedule> availabilitySchedule = modelObject.model().alwaysOnDiscreteSchedule();

      if( availabilitySchedule )
      {
        boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule.get());

        if( _availabilitySchedule && _availabilitySchedule->name() )
        {
          _chillerSet.setString(ZoneHVAC_RefrigerationChillerSetFields::AvailabilityScheduleName,_availabilitySchedule->name().get());
        }
      }

    // ZoneName
      boost::optional<ThermalZone> thermalZone = modelObject.thermalZone();

      if( thermalZone )
      {
        boost::optional<IdfObject> _thermalZone = translateAndMapModelObject(thermalZone.get());

        if( _thermalZone && _thermalZone->name() )
        {
          _chillerSet.setString(ZoneHVAC_RefrigerationChillerSetFields::ZoneName,_thermalZone->name().get());
        }
      }

    // AirInletNodeName
      _chillerSet.setString(ZoneHVAC_RefrigerationChillerSetFields::AirInletNodeName,"");

    // AirOutletNodeName
      _chillerSet.setString(ZoneHVAC_RefrigerationChillerSetFields::AirOutletNodeName,"");

    // AirChiller (extensible)
      for( std::vector<ModelObject>::iterator it = airChillers.begin();
         it != airChillers.end();
         it++ )
      {
        boost::optional<IdfObject> _airChiller = translateAndMapModelObject(*it);

        if( _airChiller )
        {
          IdfExtensibleGroup eg = _chillerSet.pushExtensibleGroup();

          eg.setString(ZoneHVAC_RefrigerationChillerSetExtensibleFields::AirChillerName,_airChiller->name().get()); 
        }
      }

    unsigned coolingPriority = modelObject.coolingPriority(airChillers.front());
    unsigned heatingPriority = modelObject.heatingPriority(airChillers.front());

    IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();

    eg.setString(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentObjectType,_chillerSet.iddObject().name()); 
    eg.setString(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentName,_chillerSet.name().get()); 
    eg.setUnsigned(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentCoolingSequence,coolingPriority); 
    eg.setUnsigned(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentHeatingorNoLoadSequence,heatingPriority); 
  }

  m_idfObjects.push_back(idfObject);

  return idfObject;
}

} // energyplus

} // openstudio

