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
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ZoneHVACEnergyRecoveryVentilator.hpp"
#include "../../model/ZoneHVACEnergyRecoveryVentilator_Impl.hpp"
#include "../../model/ZoneHVACEnergyRecoveryVentilatorController.hpp"
#include "../../model/ZoneHVACEnergyRecoveryVentilatorController_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include <utilities/idd/ZoneHVAC_EnergyRecoveryVentilator_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneHVACEnergyRecoveryVentilator( ZoneHVACEnergyRecoveryVentilator & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;
  boost::optional<ModelObject> temp;

  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneHVAC_EnergyRecoveryVentilator, modelObject);

  // AvailabilityScheduleName
  {
    auto schedule = modelObject.availabilitySchedule();
    if( auto _schedule = translateAndMapModelObject(schedule) ) {
      idfObject.setString(ZoneHVAC_EnergyRecoveryVentilatorFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  // HeatExchangerName
  { 
    auto heatExchanger = modelObject.heatExchanger();
    if( auto _heatExchanger = translateAndMapModelObject(heatExchanger) ) {
      idfObject.setString(ZoneHVAC_EnergyRecoveryVentilatorFields::HeatExchangerName,_heatExchanger->name().get());
    }
  }

  // SupplyAirFlowRate
  if( modelObject.isSupplyAirFlowRateAutosized() ) {
    idfObject.setString(ZoneHVAC_EnergyRecoveryVentilatorFields::SupplyAirFlowRate,"AutoSize");
  } else if( (value = modelObject.supplyAirFlowRate()) ) {
    idfObject.setDouble(ZoneHVAC_EnergyRecoveryVentilatorFields::SupplyAirFlowRate,value.get());
  }

  // ExhaustAirFlowRate
  if( modelObject.isExhaustAirFlowRateAutosized() ) {
    idfObject.setString(ZoneHVAC_EnergyRecoveryVentilatorFields::ExhaustAirFlowRate,"AutoSize");
  } else if( (value = modelObject.exhaustAirFlowRate()) ) {
    idfObject.setDouble(ZoneHVAC_EnergyRecoveryVentilatorFields::ExhaustAirFlowRate,value.get());
  }

  // SupplyAirFanName
  { 
    auto supplyAirFan = modelObject.supplyAirFan();
    if( auto _supplyAirFan = translateAndMapModelObject(supplyAirFan) ) {
      idfObject.setString(ZoneHVAC_EnergyRecoveryVentilatorFields::SupplyAirFanName,_supplyAirFan->name().get());
    }
  }

  // ExhaustAirFanName
  { 
    auto exhaustAirFan = modelObject.exhaustAirFan();
    if( auto _exhaustAirFan = translateAndMapModelObject(exhaustAirFan) ) {
      idfObject.setString(ZoneHVAC_EnergyRecoveryVentilatorFields::ExhaustAirFanName,_exhaustAirFan->name().get());
    }
  }

  // ControllerName
  if( (temp = modelObject.controller()) ) {
    if( auto _controller = translateAndMapModelObject(temp.get()) ) {
      idfObject.setString(ZoneHVAC_EnergyRecoveryVentilatorFields::ControllerName,_controller->name().get());
    }
  }

  // VentilationRateperUnitFloorArea
  if( (value = modelObject.ventilationRateperUnitFloorArea()) ) {
    idfObject.setDouble(ZoneHVAC_EnergyRecoveryVentilatorFields::VentilationRateperUnitFloorArea,value.get());
  }

  // VentilationRateperOccupant
  if( (value = modelObject.ventilationRateperOccupant()) ) {
    idfObject.setDouble(ZoneHVAC_EnergyRecoveryVentilatorFields::VentilationRateperOccupant,value.get());
  }

  // AvailabilityManagerListName

  return idfObject;
}

} // energyplus

} // openstudio

