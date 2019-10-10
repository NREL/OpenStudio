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
#include "../../model/ZoneHVACHighTemperatureRadiant.hpp"
#include "../../model/ZoneHVACHighTemperatureRadiant_Impl.hpp"
#include "../../model/ZoneHVACEquipmentList.hpp"
#include "../../model/ZoneHVACEquipmentList_Impl.hpp"
#include "../../model/ZoneHVACComponent.hpp"
#include "../../model/ZoneHVACComponent_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/ZoneHVAC_HighTemperatureRadiant_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneHVACHighTemperatureRadiant(ZoneHVACHighTemperatureRadiant& modelObject)
{
  boost::optional<std::string> s;
  boost::optional<double> value;
  boost::optional<ModelObject> temp;

  IdfObject idfObject(IddObjectType::ZoneHVAC_HighTemperatureRadiant);
  m_idfObjects.push_back(idfObject);

  //Name
  std::string baseName = modelObject.name().get();
  idfObject.setName(baseName);

  // AvailabilityScheduleName
  if( boost::optional<Schedule> schedule = modelObject.availabilitySchedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(ZoneHVAC_HighTemperatureRadiantFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  //field Zone Name
  boost::optional<std::string> thermalZoneName;
  if( boost::optional<ThermalZone> zone = modelObject.thermalZone() )
  {
    if( (s = zone->name()) )
    {
      thermalZoneName = s;
      idfObject.setString(ZoneHVAC_HighTemperatureRadiantFields::ZoneName,thermalZoneName.get());
    }
  }

  // OpenStudio only supports "Heating Design Capacity", which is the default in IDD
  idfObject.setString(ZoneHVAC_HighTemperatureRadiantFields::HeatingDesignCapacityMethod, "HeatingDesignCapacity");

  //MaximumPowerInput
  if( modelObject.isMaximumPowerInputAutosized() )
  {
    idfObject.setString(ZoneHVAC_HighTemperatureRadiantFields::HeatingDesignCapacity,"AutoSize");
  }
  else if( (value = modelObject.maximumPowerInput()) )
  {
    idfObject.setDouble(ZoneHVAC_HighTemperatureRadiantFields::HeatingDesignCapacity,value.get());
  }

  //FuelType
  if( (s = modelObject.fuelType()) )
  {
    idfObject.setString(ZoneHVAC_HighTemperatureRadiantFields::FuelType,s.get());
  }

  //CombustionEfficiency
  if( (value = modelObject.combustionEfficiency()) )
  {
    idfObject.setDouble(ZoneHVAC_HighTemperatureRadiantFields::CombustionEfficiency,value.get());
  }

  //FractionofInputConvertedtoRadiantEnergy
  if( (value = modelObject.fractionofInputConvertedtoRadiantEnergy()) )
  {
    idfObject.setDouble(ZoneHVAC_HighTemperatureRadiantFields::FractionofInputConvertedtoRadiantEnergy,value.get());
  }

  //FractionofInputConvertedtoLatentEnergy
  if( (value = modelObject.fractionofInputConvertedtoLatentEnergy()) )
  {
    idfObject.setDouble(ZoneHVAC_HighTemperatureRadiantFields::FractionofInputConvertedtoLatentEnergy,value.get());
  }

  //FractionofInputthatIsLost
  if( (value = modelObject.fractionofInputthatIsLost()) )
  {
    idfObject.setDouble(ZoneHVAC_HighTemperatureRadiantFields::FractionofInputthatIsLost,value.get());
  }

  //TemperatureControlType
  if( (s = modelObject.temperatureControlType()) )
  {
    idfObject.setString(ZoneHVAC_HighTemperatureRadiantFields::TemperatureControlType,s.get());
  }

  //HeatingThrottlingRange
  if( (value = modelObject.heatingThrottlingRange()) )
  {
    idfObject.setDouble(ZoneHVAC_HighTemperatureRadiantFields::HeatingThrottlingRange,value.get());
  }

  //HeatingSetpointTemperatureScheduleName
  if( boost::optional<Schedule> schedule = modelObject.heatingSetpointTemperatureSchedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(ZoneHVAC_HighTemperatureRadiantFields::HeatingSetpointTemperatureScheduleName,_schedule->name().get());
    }
  }

  //FractionofRadiantEnergyIncidentonPeople
  double fractionofRadiantEnergyIncidentonPeople = 0.04;
  if( (value = modelObject.fractionofRadiantEnergyIncidentonPeople()) )
  {
    idfObject.setDouble(ZoneHVAC_HighTemperatureRadiantFields::FractionofRadiantEnergyIncidentonPeople,value.get());
    fractionofRadiantEnergyIncidentonPeople = value.get();
  }

  //get rid of any existing surface (just to be safe)
  idfObject.clearExtensibleGroups();

  //aggregator for total area; will be used to create weighted area
  double totalAreaOfNonFloorSurfaces = 0;
  double totalAreaOfFloorSurfaces = 0;

  //loop through all surfaces, adding up their area
  for (const Surface& surface : modelObject.getImpl<model::detail::ZoneHVACHighTemperatureRadiant_Impl>()->surfaces()){
    if( istringEqual(surface.surfaceType(),"Floor") ) {
      totalAreaOfFloorSurfaces = totalAreaOfFloorSurfaces + surface.grossArea();
    } else {
      totalAreaOfNonFloorSurfaces = totalAreaOfNonFloorSurfaces + surface.grossArea();
    }
  }

  // Assume that 70% of what is not on people is on the floor
  double fractionOnFloor = (1.0 - fractionofRadiantEnergyIncidentonPeople) * 0.70;
  // Assume that 30% of what is not on people is on the non floor surfaces
  double fractionOnNonFloor = (1.0 - fractionofRadiantEnergyIncidentonPeople) * 0.30;
  //loop through all the surfaces, adding them and their flow fractions (weighted per-area)
  for (const Surface& surface : modelObject.getImpl<model::detail::ZoneHVACHighTemperatureRadiant_Impl>()->surfaces()){
    IdfExtensibleGroup group = idfObject.pushExtensibleGroup();
    group.setString(ZoneHVAC_HighTemperatureRadiantExtensibleFields::SurfaceName, surface.name().get());
    if( istringEqual(surface.surfaceType(),"Floor") ) {
      group.setDouble(ZoneHVAC_HighTemperatureRadiantExtensibleFields::FractionofRadiantEnergytoSurface, (surface.grossArea()/totalAreaOfFloorSurfaces * fractionOnFloor) );
    } else {
      group.setDouble(ZoneHVAC_HighTemperatureRadiantExtensibleFields::FractionofRadiantEnergytoSurface, (surface.grossArea()/totalAreaOfNonFloorSurfaces * fractionOnNonFloor) );
    }
  }

  return idfObject;
}

} // energyplus

} // openstudio

