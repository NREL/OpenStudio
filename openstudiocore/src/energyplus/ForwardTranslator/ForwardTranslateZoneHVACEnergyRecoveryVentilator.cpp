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
#include "../../model/HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "../../model/HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ZoneHVACEnergyRecoveryVentilator.hpp"
#include "../../model/ZoneHVACEnergyRecoveryVentilator_Impl.hpp"
#include "../../model/ZoneHVACEnergyRecoveryVentilatorController.hpp"
#include "../../model/ZoneHVACEnergyRecoveryVentilatorController_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/HeatExchanger_AirToAir_SensibleAndLatent_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_EnergyRecoveryVentilator_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_EnergyRecoveryVentilator_Controller_FieldEnums.hxx>
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

  // Model Name
  std::string const baseName = modelObject.name().get();
  // Node Names
  std::string const oaNodeName = baseName + " OA Node";
  std::string const supplyFanInletNodeName = baseName + " Supply Fan Inlet Node";
  std::string const exhaustFanInletNodeName = baseName + " Exhaust Fan Inlet Node";
  std::string const exhaustFanOutletNodeName = baseName + " Exhaust Fan Outlet Node";

  auto const zoneExhaustNode = modelObject.inletNode();
  auto const zoneInletNode = modelObject.outletNode();

  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneHVAC_EnergyRecoveryVentilator, modelObject);

  // AvailabilityScheduleName
  {
    auto schedule = modelObject.availabilitySchedule();
    if( auto _schedule = translateAndMapModelObject(schedule) ) {
      idfObject.setString(ZoneHVAC_EnergyRecoveryVentilatorFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  // Heat Exchanger
  {
    auto heatExchanger = modelObject.heatExchanger().cast<HeatExchangerAirToAirSensibleAndLatent>();

    if ( heatExchanger.supplyAirOutletTemperatureControl() ) {
      LOG(Warn,"Overriding supplyAirOutletTemperatureControl to \"No\" for " << heatExchanger.briefDescription() << " because the HX needs to control itself, not through Setpoint Manager.");
      heatExchanger.setSupplyAirOutletTemperatureControl( false );
    }

    if( auto _heatExchanger = translateAndMapModelObject(heatExchanger) )
    {
      // HeatExchangerName
      idfObject.setString(ZoneHVAC_EnergyRecoveryVentilatorFields::HeatExchangerName,_heatExchanger->name().get() );
      // Heat Exchanger Nodes
      if( _heatExchanger->iddObject().type() == IddObjectType::HeatExchanger_AirToAir_SensibleAndLatent )
      {
        _heatExchanger->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirInletNodeName,oaNodeName );
        _heatExchanger->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirOutletNodeName,supplyFanInletNodeName );
        _heatExchanger->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirInletNodeName,zoneExhaustNode->name().get() );
        _heatExchanger->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirOutletNodeName,exhaustFanInletNodeName );

        IdfObject _oaNodeList(openstudio::IddObjectType::OutdoorAir_NodeList);
        _oaNodeList.setString(0,oaNodeName);
        m_idfObjects.push_back(_oaNodeList);
      }
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

  // Supply Air Fan
  {
    auto supplyAirFan = modelObject.supplyAirFan();
    if( auto _supplyAirFan = translateAndMapModelObject(supplyAirFan) )
    {
      // SupplyAirFanName
      idfObject.setString(ZoneHVAC_EnergyRecoveryVentilatorFields::SupplyAirFanName,_supplyAirFan->name().get() );
      // Supply Air Fan Inlet and Outlet Nodes
      if( _supplyAirFan->iddObject().type() == IddObjectType::Fan_OnOff )
      {
        
        _supplyAirFan->setString(Fan_OnOffFields::AirInletNodeName,supplyFanInletNodeName );
        _supplyAirFan->setString(Fan_OnOffFields::AirOutletNodeName,zoneInletNode->name().get() );
      }
    }
  }

  // Exhaust Air Fan
  {
    auto exhaustAirFan = modelObject.exhaustAirFan();
    if( auto _exhaustAirFan = translateAndMapModelObject(exhaustAirFan) )
    {
      // ExhaustAirFanName
      idfObject.setString(ZoneHVAC_EnergyRecoveryVentilatorFields::ExhaustAirFanName,_exhaustAirFan->name().get() );
      // Exhaust Air Fan Inlet and Outlet Nodes
      if( _exhaustAirFan->iddObject().type() == IddObjectType::Fan_OnOff )
      {
        
        _exhaustAirFan->setString(Fan_OnOffFields::AirInletNodeName,exhaustFanInletNodeName );
        _exhaustAirFan->setString(Fan_OnOffFields::AirOutletNodeName,exhaustFanOutletNodeName );
      }
    }
  }

  // ControllerName
  if( auto controller = modelObject.controller() ) {
    if( auto _controller = translateAndMapModelObject(controller.get()) ) {
      idfObject.setString(ZoneHVAC_EnergyRecoveryVentilatorFields::ControllerName,_controller->name().get());

      // Need to set Humidistat Control Thermal Zone in Controller
      if( controller->highHumidityControlFlag() ) {
        if( auto thermalZone = modelObject.thermalZone() ) {
          if( auto _thermalZone = translateAndMapModelObject(thermalZone.get()) ) {
            _controller->setString(ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::HumidistatControlZoneName,_thermalZone->name().get());
          }
        }
      }
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

