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
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../model/ZoneHVACEquipmentList.hpp"
#include "../../model/ZoneHVACEquipmentList_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/RefrigerationAirChiller.hpp"
#include "../../model/RefrigerationAirChiller_Impl.hpp"
#include "../../model/ZoneVentilationDesignFlowRate.hpp"
#include "../../model/ZoneVentilationDesignFlowRate_Impl.hpp"
#include "../../utilities/idd/IddEnums.hpp"
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

  // If there is nothing but ZoneVentilationDesignFlowRate then stop
  // We don't want a zone hvac equipment list in the idf
  if( subsetCastVector<model::ZoneVentilationDesignFlowRate>(objects).size() == objects.size() ) {
    return boost::none;
  }

  std::vector<ModelObject> coolingVector = modelObject.equipmentInCoolingOrder();
  std::vector<ModelObject> heatingVector = modelObject.equipmentInHeatingOrder();

  std::vector<ModelObject> airChillers;
  std::vector<ModelObject> stdEquipment;

  for( const auto & elem : objects )
  {
    if (boost::optional<RefrigerationAirChiller> airChiller = elem.optionalCast<RefrigerationAirChiller>()) {
      airChillers.push_back(airChiller.get());
    } else {
      stdEquipment.push_back(elem);
    }
  }

  boost::optional<RefrigerationAirChiller> airChiller;
  std::map<ModelObject, unsigned> coolingMap;
  unsigned chillerSetCoolingPriority = 0;
  unsigned priority = 1;
  int airChillerOffset = -1;
  for( const auto & elem : coolingVector )
  {
    if (airChillers.size() > 0 && (airChiller = elem.optionalCast<RefrigerationAirChiller>()) )
    {
      if (chillerSetCoolingPriority == 0) {
        chillerSetCoolingPriority = priority;
      }
      airChillerOffset++;
    }
    else {
      coolingMap.insert ( std::pair<ModelObject,unsigned>(elem, ((airChillerOffset > 0) ? (priority - airChillerOffset) : priority) ) );
    }
    priority++;
  }

  std::map<ModelObject, unsigned> heatingMap;
  unsigned chillerSetHeatingPriority = 0;
  priority = 1;
  airChillerOffset = -1;
  for( const auto & elem : heatingVector )
  {
    if (airChillers.size() > 0 && (airChiller= elem.optionalCast<RefrigerationAirChiller>()) )
    {
      if (chillerSetHeatingPriority == 0) {
        chillerSetHeatingPriority = priority;
      }
      airChillerOffset++;
    }
    else {
      heatingMap.insert ( std::pair<ModelObject,unsigned>(elem, ((airChillerOffset > 0) ? (priority - airChillerOffset) : priority) ) );
    }
    priority++;
  }

  IdfObject idfObject(IddObjectType::ZoneHVAC_EquipmentList);

  // Name
  std::string name;
  s = modelObject.thermalZone().name();
  if(s)
  {
    name = s.get() + " Equipment List";
    idfObject.setName(name);
  }

  for( auto & elem : stdEquipment )
  {
    unsigned coolingPriority = coolingMap[elem];
    unsigned heatingPriority = heatingMap[elem];

    boost::optional<IdfObject> _equipment;

    // We will take care of ZoneVentilationDesignFlowRate elsewher since it doesn't belong on equipment list
    if( ! elem.optionalCast<ZoneVentilationDesignFlowRate>() ) {
      if( auto _equipment = translateAndMapModelObject(elem) ) {
        IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();
        eg.setString(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentObjectType,_equipment->iddObject().name()); 
        eg.setString(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentName,_equipment->name().get()); 
        eg.setUnsigned(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentCoolingSequence,coolingPriority); 
        eg.setUnsigned(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentHeatingorNoLoadSequence,heatingPriority); 
      }
    }
  }

  if (!airChillers.empty()) {
    // ZoneHVAC:RefrigerationChillerSet
    // Name
      IdfObject _chillerSet(IddObjectType::ZoneHVAC_RefrigerationChillerSet);

      m_idfObjects.push_back(_chillerSet);

      _chillerSet.setName(name + " Refrigeration Chiller Set");

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
      for( auto & elem : airChillers )
      {
        boost::optional<IdfObject> _airChiller = translateAndMapModelObject(elem);

        if( _airChiller )
        {
          IdfExtensibleGroup eg = _chillerSet.pushExtensibleGroup();

          eg.setString(ZoneHVAC_RefrigerationChillerSetExtensibleFields::AirChillerName,_airChiller->name().get()); 
        }
      }

    IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();

    eg.setString(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentObjectType,_chillerSet.iddObject().name()); 
    eg.setString(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentName,_chillerSet.name().get()); 
    eg.setUnsigned(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentCoolingSequence, chillerSetCoolingPriority); 
    eg.setUnsigned(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentHeatingorNoLoadSequence, chillerSetHeatingPriority); 
  }

  m_idfObjects.push_back(idfObject);

  return idfObject;
}

} // energyplus

} // openstudio

