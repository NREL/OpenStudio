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
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ZoneVentilationDesignFlowRate.hpp"
#include "../../model/ZoneVentilationDesignFlowRate_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/StraightComponent.hpp"
#include "../../model/StraightComponent_Impl.hpp"
#include <utilities/idd/ZoneVentilation_DesignFlowRate_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneVentilationDesignFlowRate(
    ZoneVentilationDesignFlowRate & modelObject )
{
  auto idfObject = createRegisterAndNameIdfObject(IddObjectType::ZoneVentilation_DesignFlowRate,modelObject);

  if( auto value = modelObject.thermalZone() ) {
    idfObject.setString(ZoneVentilation_DesignFlowRateFields::ZoneorZoneListName,value->name().get());
  }
  
  {
    auto mo = modelObject.schedule();
    idfObject.setString(ZoneVentilation_DesignFlowRateFields::ScheduleName,mo.name().get());
  }

  {
    auto value = modelObject.designFlowRateCalculationMethod();
    idfObject.setString(ZoneVentilation_DesignFlowRateFields::DesignFlowRateCalculationMethod,value); 
  }
  
  {
    auto value = modelObject.designFlowRate();
    idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::DesignFlowRate,value);
  }

  {
    auto value = modelObject.flowRateperZoneFloorArea();
    idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::FlowRateperZoneFloorArea,value);
  }

  {
    auto value = modelObject.flowRateperPerson();
    idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::FlowRateperPerson,value);
  }

  {
    auto value = modelObject.airChangesperHour();
    idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::AirChangesperHour,value);
  }

  {
    auto value = modelObject.ventilationType();
    idfObject.setString(ZoneVentilation_DesignFlowRateFields::VentilationType,value);
  }

  {
    auto value = modelObject.fanPressureRise();
    idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::FanPressureRise,value);
  }

  {
    auto value = modelObject.fanTotalEfficiency();
    idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::FanTotalEfficiency,value);
  }

  {
    auto value = modelObject.constantTermCoefficient();
    idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::ConstantTermCoefficient,value);
  }

  {
    auto value = modelObject.temperatureTermCoefficient();
    idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::TemperatureTermCoefficient,value);
  }

  {
    auto value = modelObject.velocityTermCoefficient();
    idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::VelocityTermCoefficient,value);
  }

  {
    auto value = modelObject.velocitySquaredTermCoefficient();
    idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::VelocitySquaredTermCoefficient,value);
  }

  {
    auto value = modelObject.minimumIndoorTemperature();
    idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::MinimumIndoorTemperature,value);
  }

  if( auto value = modelObject.minimumIndoorTemperatureSchedule() ) {
    idfObject.setString(ZoneVentilation_DesignFlowRateFields::MinimumIndoorTemperatureScheduleName,value->name().get());
  }

  {
    auto value = modelObject.maximumIndoorTemperature();
    idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::MaximumIndoorTemperature,value);
  }

  if( auto value = modelObject.maximumIndoorTemperatureSchedule() ) {
    idfObject.setString(ZoneVentilation_DesignFlowRateFields::MaximumIndoorTemperatureScheduleName,value->name().get());  
  }

  {
    auto value = modelObject.deltaTemperature();
    idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::DeltaTemperature,value);
  }

  if( auto value = modelObject.deltaTemperatureSchedule() ) {
    idfObject.setString(ZoneVentilation_DesignFlowRateFields::DeltaTemperatureScheduleName,value->name().get());
  }

  {
    auto value = modelObject.minimumOutdoorTemperature();
    idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::MinimumOutdoorTemperature,value);
  }

  if( auto value = modelObject.minimumOutdoorTemperatureSchedule() ) {
    idfObject.setString(ZoneVentilation_DesignFlowRateFields::MinimumOutdoorTemperatureScheduleName,value->name().get());
  }

  {
    auto value = modelObject.maximumOutdoorTemperature();
    idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::MaximumOutdoorTemperature,value);
  }

  if( auto value = modelObject.maximumOutdoorTemperatureSchedule() ) {
    idfObject.setString(ZoneVentilation_DesignFlowRateFields::MaximumOutdoorTemperatureScheduleName,value->name().get()); 
  }

  {
    auto value = modelObject.maximumWindSpeed();
    idfObject.setDouble(ZoneVentilation_DesignFlowRateFields::MaximumWindSpeed,value);
  }

  return idfObject;
}

} // energyplus

} // openstudio

