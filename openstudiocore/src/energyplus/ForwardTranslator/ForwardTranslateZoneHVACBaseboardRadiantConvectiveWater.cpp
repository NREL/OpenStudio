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
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ModelObject.hpp"
#include "../../model/ModelObject_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ZoneHVACBaseboardRadiantConvectiveWater.hpp"
#include "../../model/ZoneHVACBaseboardRadiantConvectiveWater_Impl.hpp"
#include "../../model/ZoneHVACEquipmentList.hpp"
#include "../../model/ZoneHVACEquipmentList_Impl.hpp"
#include "../../model/ZoneHVACComponent.hpp"
#include "../../model/ZoneHVACComponent_Impl.hpp"
#include "../../model/CoilHeatingWaterBaseboardRadiant.hpp"
#include "../../model/CoilHeatingWaterBaseboardRadiant_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/ZoneHVAC_Baseboard_RadiantConvective_Water_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneHVACBaseboardRadiantConvectiveWater(ZoneHVACBaseboardRadiantConvectiveWater& modelObject)
{
  boost::optional<std::string> s;
  boost::optional<double> value;
  boost::optional<ModelObject> temp;

  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneHVAC_Baseboard_RadiantConvective_Water, modelObject);

  // AvailabilityScheduleName
  {
    auto schedule = modelObject.availabilitySchedule();
    if( auto _schedule = translateAndMapModelObject(schedule) ) {
      idfObject.setString(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  if ( auto heatingCoil = modelObject.heatingCoil().optionalCast<CoilHeatingWaterBaseboardRadiant>() ) {

    // InletNodeName
    if( auto node = heatingCoil->inletModelObject() ) {
      idfObject.setString(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::InletNodeName,node->name().get());
    }

    // OutletNodeName
    if( auto node = heatingCoil->outletModelObject() ) {
      idfObject.setString(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::OutletNodeName,node->name().get());
    }

    // RatedAverageWaterTemperature
    if( (value = heatingCoil->ratedAverageWaterTemperature()) ) {
      idfObject.setDouble(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::RatedAverageWaterTemperature,value.get());
    }

    // RatedWaterMassFlowRate
    if( (value = heatingCoil->ratedWaterMassFlowRate()) ) {
      idfObject.setDouble(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::RatedWaterMassFlowRate,value.get());
    }

    // HeatingDesignCapacityMethod
    if( (s = heatingCoil->heatingDesignCapacityMethod()) ) {
      idfObject.setString(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::HeatingDesignCapacityMethod,s.get());
    }

    // HeatingDesignCapacity
    if( heatingCoil->isHeatingDesignCapacityAutosized() ) {
      idfObject.setString(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::HeatingDesignCapacity,"AutoSize");
    } else if( (value = heatingCoil->heatingDesignCapacity()) ) {
      idfObject.setDouble(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::HeatingDesignCapacity,value.get());
    }

    // HeatingDesignCapacityPerFloorArea
    if( (value = heatingCoil->heatingDesignCapacityPerFloorArea()) ) {
      idfObject.setDouble(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::HeatingDesignCapacityPerFloorArea,value.get());
    }

    // FractionofAutosizedHeatingDesignCapacity
    if( (value = heatingCoil->fractionofAutosizedHeatingDesignCapacity()) ) {
      idfObject.setDouble(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::FractionofAutosizedHeatingDesignCapacity,value.get());
    }

    // MaximumWaterFlowRate
    if( heatingCoil->isMaximumWaterFlowRateAutosized() ) {
      idfObject.setString(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::MaximumWaterFlowRate,"AutoSize");
    } else if( (value = heatingCoil->maximumWaterFlowRate()) ) {
      idfObject.setDouble(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::MaximumWaterFlowRate,value.get());
    }

    // ConvergenceTolerance
    if( (value = heatingCoil->convergenceTolerance()) ) {
      idfObject.setDouble(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::ConvergenceTolerance,value.get());
    }

  }

  // FractionRadiant
  if( (value = modelObject.fractionRadiant()) ) {
    idfObject.setDouble(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::FractionRadiant,value.get());
  }

  // FractionofRadiantEnergyIncidentonPeople
  double fractionofRadiantEnergyIncidentonPeople = modelObject.fractionofRadiantEnergyIncidentonPeople();
  {
    idfObject.setDouble(ZoneHVAC_Baseboard_RadiantConvective_WaterFields::FractionofRadiantEnergyIncidentonPeople,fractionofRadiantEnergyIncidentonPeople);
  }

  //get rid of any existing surface (just to be safe)
  idfObject.clearExtensibleGroups();  
 
  //aggregator for total area; will be used to create weighted area
  double totalAreaOfWallSurfaces = 0;
  double totalAreaOfCeilingSurfaces = 0;
  double totalAreaOfFloorSurfaces = 0;

  //loop through all surfaces, adding up their area
  auto const& surfaces = modelObject.getImpl<model::detail::ZoneHVACBaseboardRadiantConvectiveWater_Impl>()->surfaces();

  for ( auto const& surface : surfaces ) {
    if( istringEqual(surface.surfaceType(),"Floor") ) {
      totalAreaOfFloorSurfaces += surface.grossArea();
    } else if( istringEqual(surface.surfaceType(),"RoofCeiling") ) {
      totalAreaOfCeilingSurfaces += surface.grossArea();
    } else {
      totalAreaOfWallSurfaces += surface.grossArea();
    }
  }

  // Assume that 5% of what is not on people is on the floor
  double fractionOnFloor = (1.0 - fractionofRadiantEnergyIncidentonPeople) * 0.05;
  // Assume that 55% of what is not on people is on the walls
  double fractionOnWall = (1.0 - fractionofRadiantEnergyIncidentonPeople) * 0.55;
  // Assume that 40% of what is not on people is on the ceiling
  double fractionOnCeiling = (1.0 - fractionofRadiantEnergyIncidentonPeople) * 0.40;
  //loop through all the surfaces, adding them and their flow fractions (weighted per-area)
  for ( auto const& surface : surfaces ) {
    IdfExtensibleGroup group = idfObject.pushExtensibleGroup();
    group.setString(ZoneHVAC_Baseboard_RadiantConvective_WaterExtensibleFields::SurfaceName, surface.name().get());        
    if( istringEqual(surface.surfaceType(),"Floor") ) {
      group.setDouble(ZoneHVAC_Baseboard_RadiantConvective_WaterExtensibleFields::FractionofRadiantEnergytoSurface, (surface.grossArea()/totalAreaOfFloorSurfaces * fractionOnFloor) );     
    } else if( istringEqual(surface.surfaceType(),"RoofCeiling") ) {
      group.setDouble(ZoneHVAC_Baseboard_RadiantConvective_WaterExtensibleFields::FractionofRadiantEnergytoSurface, (surface.grossArea()/totalAreaOfCeilingSurfaces * fractionOnCeiling) );     
    } else {
      group.setDouble(ZoneHVAC_Baseboard_RadiantConvective_WaterExtensibleFields::FractionofRadiantEnergytoSurface, (surface.grossArea()/totalAreaOfWallSurfaces * fractionOnWall) );     
    }
  }

  return idfObject;
}

} // energyplus

} // openstudio

