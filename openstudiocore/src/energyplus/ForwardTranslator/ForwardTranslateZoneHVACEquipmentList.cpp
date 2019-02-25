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

#include "../ForwardTranslator.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../model/ZoneHVACEquipmentList.hpp"
#include "../../model/ZoneHVACEquipmentList_Impl.hpp"
#include "../../model/ZoneHVACComponent.hpp"
#include "../../model/ZoneHVACComponent_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/RefrigerationAirChiller.hpp"
#include "../../model/RefrigerationAirChiller_Impl.hpp"
#include "../../model/ZoneVentilationDesignFlowRate.hpp"
#include "../../model/ZoneVentilationDesignFlowRate_Impl.hpp"
#include "../../model/AirLoopHVACReturnPlenum.hpp"
#include "../../model/AirLoopHVACReturnPlenum_Impl.hpp"
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
    } else if( elem.optionalCast<ZoneVentilationDesignFlowRate>() ) {
      continue;
    } else {
      stdEquipment.push_back(elem);
    }
  }

  boost::optional<RefrigerationAirChiller> airChiller;
  std::map<ModelObject, unsigned> coolingMap;
  unsigned chillerSetCoolingPriority = 0;
  unsigned priority = 1;
  int offset = 0;
  for( const auto & elem : coolingVector )
  {
    if (airChillers.size() > 0 && (airChiller = elem.optionalCast<RefrigerationAirChiller>()) ) {
      if (chillerSetCoolingPriority == 0) {
        chillerSetCoolingPriority = priority;
      } else {
        offset++;
      }
    } else if( elem.optionalCast<ZoneVentilationDesignFlowRate>() ) {
      // ZoneVentilationDesignFlowRate is not ZoneHVAC from E+ perspective
      offset++;
    } else {
      coolingMap.insert ( std::pair<ModelObject,unsigned>(elem, priority - offset) );
    }
    priority++;
  }

  std::map<ModelObject, unsigned> heatingMap;
  unsigned chillerSetHeatingPriority = 0;
  priority = 1;
  offset = 0;
  for( const auto & elem : heatingVector )
  {
    if (airChillers.size() > 0 && (airChiller= elem.optionalCast<RefrigerationAirChiller>()) ) {
      if (chillerSetHeatingPriority == 0) {
        chillerSetHeatingPriority = priority;
      } else {
        offset++;
      }
    } else if( elem.optionalCast<ZoneVentilationDesignFlowRate>() ) {
      // ZoneVentilationDesignFlowRate is not ZoneHVAC from E+ perspective
      offset++;
    } else {
      heatingMap.insert ( std::pair<ModelObject,unsigned>(elem,priority - offset) );
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

  // LoadDistributionScheme
  {
    auto scheme = modelObject.loadDistributionScheme();
    idfObject.setString(ZoneHVAC_EquipmentListFields::LoadDistributionScheme,scheme);
  }

  for( auto & elem : stdEquipment )
  {
    unsigned coolingPriority = coolingMap[elem];
    unsigned heatingPriority = heatingMap[elem];

    boost::optional<IdfObject> _equipment;

    if( auto _equipment = translateAndMapModelObject(elem) ) {
      IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();
      eg.setString(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentObjectType,_equipment->iddObject().name());
      eg.setString(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentName,_equipment->name().get());
      eg.setUnsigned(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentCoolingSequence,coolingPriority);
      eg.setUnsigned(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentHeatingorNoLoadSequence,heatingPriority);

      // Only translate the Sequential Load Fractions if appropriate (model checks for Load Distribution = SequentialLoad, and has heating/cooling
      // Priority that isn't zero)
      if (boost::optional<double> _frac = modelObject.sequentialCoolingFraction(elem)) {
        eg.setDouble(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentSequentialCoolingFraction, _frac.get());
      }

      if (boost::optional<double> _frac = modelObject.sequentialHeatingFraction(elem)) {
        eg.setDouble(ZoneHVAC_EquipmentListExtensibleFields::ZoneEquipmentSequentialHeatingFraction, _frac.get());
      }

    }

    auto zoneHVAC = elem.optionalCast<ZoneHVACComponent>();
    if ( zoneHVAC ) {
      auto plenum = zoneHVAC->returnPlenum();
      if ( plenum ) {
        auto _plenum = translateAndMapModelObject(plenum.get());
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

