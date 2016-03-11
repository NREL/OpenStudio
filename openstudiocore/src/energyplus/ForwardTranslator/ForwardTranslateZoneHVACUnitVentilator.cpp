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
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ZoneHVACUnitVentilator.hpp"
#include "../../model/ZoneHVACUnitVentilator_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include <utilities/idd/Coil_Cooling_Water_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Gas_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/Fan_VariableVolume_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_UnitVentilator_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneHVACUnitVentilator( ZoneHVACUnitVentilator & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;
  boost::optional<ModelObject> temp;

  // Model Name
  std::string const baseName = modelObject.name().get();
  // Node Names
  std::string const mixedAirNodeName = baseName + " Mixed Air Node";
  std::string const fanOutletNodeName = baseName + " Fan Outlet Node";
  std::string const coolingCoilOutletNodeName = baseName + " Cooling Coil Outlet Node";
  std::string const reliefAirNodeName = baseName + " Relief Air Node";
  std::string const exhaustAirNodeName = baseName + " Exhaust Air Node";
  std::string const oaNodeName = baseName + " OA Node";

  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneHVAC_UnitVentilator, modelObject);

  // AvailabilityScheduleName
  {
    auto schedule = modelObject.availabilitySchedule();
    if( auto _schedule = translateAndMapModelObject(schedule) ) {
      idfObject.setString(ZoneHVAC_UnitVentilatorFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  // MaximumSupplyAirFlowRate
  if( modelObject.isMaximumSupplyAirFlowRateAutosized() ) {
    idfObject.setString(ZoneHVAC_UnitVentilatorFields::MaximumSupplyAirFlowRate,"AutoSize");
  } else if( (value = modelObject.maximumSupplyAirFlowRate()) ) {
    idfObject.setDouble(ZoneHVAC_UnitVentilatorFields::MaximumSupplyAirFlowRate,value.get());
  }

  // OutdoorAirControlType
  if( (s = modelObject.outdoorAirControlType()) ) {
    idfObject.setString(ZoneHVAC_UnitVentilatorFields::OutdoorAirControlType,s.get());
  }

  // MinimumOutdoorAirFlowRate
  if( modelObject.isMinimumOutdoorAirFlowRateAutosized() ) {
    idfObject.setString(ZoneHVAC_UnitVentilatorFields::MinimumOutdoorAirFlowRate,"AutoSize");
  } else if( (value = modelObject.minimumOutdoorAirFlowRate()) ) {
    idfObject.setDouble(ZoneHVAC_UnitVentilatorFields::MinimumOutdoorAirFlowRate,value.get());
  }

  // MinimumOutdoorAirScheduleName
  {
    auto schedule = modelObject.minimumOutdoorAirSchedule();
    if( auto _schedule = translateAndMapModelObject(schedule) ) {
      idfObject.setString(ZoneHVAC_UnitVentilatorFields::MinimumOutdoorAirScheduleName,_schedule->name().get());
    }
  }

  // MaximumOutdoorAirFlowRate
  if( modelObject.isMaximumOutdoorAirFlowRateAutosized() ) {
    idfObject.setString(ZoneHVAC_UnitVentilatorFields::MaximumOutdoorAirFlowRate,"AutoSize");
  } else if( (value = modelObject.maximumOutdoorAirFlowRate()) ) {
    idfObject.setDouble(ZoneHVAC_UnitVentilatorFields::MaximumOutdoorAirFlowRate,value.get());
  }

  // MaximumOutdoorAirFractionorTemperatureScheduleName
  {
    auto schedule = modelObject.maximumOutdoorAirFractionorTemperatureSchedule();
    if( auto _schedule = translateAndMapModelObject(schedule) ) {
      idfObject.setString(ZoneHVAC_UnitVentilatorFields::MaximumOutdoorAirFractionorTemperatureScheduleName,_schedule->name().get());
    }
  }

  // AirInletNodeName
  auto airInletNode = modelObject.inletNode();
  if( airInletNode ) {
    idfObject.setString(ZoneHVAC_UnitVentilatorFields::AirInletNodeName,airInletNode->name().get());
  }

  // AirOutletNodeName
  auto airOutletNode = modelObject.outletNode();
  if( airOutletNode ) {
    idfObject.setString(ZoneHVAC_UnitVentilatorFields::AirOutletNodeName,airOutletNode->name().get());
  }

  // OutdoorAirNodeName
  // ExhaustAirNodeName
  // MixedAirNodeName
  idfObject.setString(ZoneHVAC_UnitVentilatorFields::MixedAirNodeName,mixedAirNodeName);
  idfObject.setString(ZoneHVAC_UnitVentilatorFields::OutdoorAirNodeName,oaNodeName);
  idfObject.setString(ZoneHVAC_UnitVentilatorFields::ExhaustAirNodeName,exhaustAirNodeName);
  IdfObject _oaNodeList(openstudio::IddObjectType::OutdoorAir_NodeList);
  _oaNodeList.setString(0,oaNodeName);
  m_idfObjects.push_back(_oaNodeList);

  // Supply Air Fan
  {
    auto supplyAirFan = modelObject.supplyAirFan();
    if( auto _supplyAirFan = translateAndMapModelObject(supplyAirFan) )
    {
      // SupplyAirFanObjectType
      idfObject.setString(ZoneHVAC_UnitVentilatorFields::SupplyAirFanObjectType,_supplyAirFan->iddObject().name() );

      // SupplyAirFanName
      idfObject.setString(ZoneHVAC_UnitVentilatorFields::SupplyAirFanName,_supplyAirFan->name().get() );
      // Supply Air Fan Inlet and Outlet Nodes
      if( _supplyAirFan->iddObject().type() == IddObjectType::Fan_ConstantVolume )
      {
        _supplyAirFan->setString(Fan_ConstantVolumeFields::AirInletNodeName,mixedAirNodeName );
        _supplyAirFan->setString(Fan_ConstantVolumeFields::AirOutletNodeName,fanOutletNodeName );
      }
      else if( _supplyAirFan->iddObject().type() == IddObjectType::Fan_OnOff )
      {
        
        _supplyAirFan->setString(Fan_OnOffFields::AirInletNodeName,mixedAirNodeName );
        _supplyAirFan->setString(Fan_OnOffFields::AirOutletNodeName,fanOutletNodeName );
      }
      else if( _supplyAirFan->iddObject().type() == IddObjectType::Fan_VariableVolume )
      {
        _supplyAirFan->setString(Fan_VariableVolumeFields::AirInletNodeName,mixedAirNodeName );
        _supplyAirFan->setString(Fan_VariableVolumeFields::AirOutletNodeName,fanOutletNodeName );
      }
    }
  }

  // CoilOption
  auto heatingCoil = modelObject.heatingCoil();
  auto coolingCoil = modelObject.coolingCoil();

  // Cooling Coil
  {
    if ( coolingCoil && airOutletNode ) {
      if( auto _coolingCoil = translateAndMapModelObject(coolingCoil.get()) )
      {
        // CoolingCoilObjectType
        idfObject.setString(ZoneHVAC_UnitVentilatorFields::CoolingCoilObjectType,_coolingCoil->iddObject().name() );
        // CoolingCoilName
        idfObject.setString(ZoneHVAC_UnitVentilatorFields::CoolingCoilName,_coolingCoil->name().get() );
        // Cooling Coil Inlet and Outlet Nodes
        auto coolingCoilOutletName = heatingCoil ? coolingCoilOutletNodeName : airOutletNode->name().get();

        if( _coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_Water )
        {
          _coolingCoil->setString(Coil_Cooling_WaterFields::AirInletNodeName,fanOutletNodeName );
          _coolingCoil->setString(Coil_Cooling_WaterFields::AirOutletNodeName, coolingCoilOutletName );
        }
      }
    }
  }

  // Heating Coil
  {
    if ( heatingCoil && airOutletNode ) {
      if( auto _heatingCoil = translateAndMapModelObject(heatingCoil.get()) )
      {
        // HeatingCoilObjectType
        idfObject.setString(ZoneHVAC_UnitVentilatorFields::HeatingCoilObjectType,_heatingCoil->iddObject().name() );
        // HeatingCoilName
        idfObject.setString(ZoneHVAC_UnitVentilatorFields::HeatingCoilName,_heatingCoil->name().get() );
        // Heating Coil Inlet and Outlet Nodes
        auto heatingCoilOutletName = coolingCoil ? coolingCoilOutletNodeName : fanOutletNodeName;

        if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric)
        {
          _heatingCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName,heatingCoilOutletName);
          _heatingCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName,airOutletNode->name().get());
        }
        else if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Gas )
        {
          _heatingCoil->setString(Coil_Heating_GasFields::AirInletNodeName,heatingCoilOutletName);
          _heatingCoil->setString(Coil_Heating_GasFields::AirOutletNodeName,airOutletNode->name().get());
        }
        else if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Water )
        {
          _heatingCoil->setString(Coil_Heating_WaterFields::AirInletNodeName,heatingCoilOutletName);
          _heatingCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName,airOutletNode->name().get());
        }
      }
    }
  }

  if ( heatingCoil && coolingCoil ) {
    idfObject.setString(ZoneHVAC_UnitVentilatorFields::CoilOption,"HeatingAndCooling");
  } else if ( heatingCoil ) {
    idfObject.setString(ZoneHVAC_UnitVentilatorFields::CoilOption,"Heating");
  } else if ( coolingCoil ) {
    idfObject.setString(ZoneHVAC_UnitVentilatorFields::CoilOption,"Cooling");
  } else {
    idfObject.setString(ZoneHVAC_UnitVentilatorFields::CoilOption,"None");
  }

  // SupplyAirFanOperatingModeScheduleName
  if( (temp = modelObject.supplyAirFanOperatingModeSchedule()) ){
    if( auto _schedule = translateAndMapModelObject(temp.get()) ) {
      idfObject.setString(ZoneHVAC_UnitVentilatorFields::SupplyAirFanOperatingModeScheduleName,_schedule->name().get());
    }
  }

  // HeatingConvergenceTolerance
  if( (value = modelObject.heatingConvergenceTolerance()) ) {
    idfObject.setDouble(ZoneHVAC_UnitVentilatorFields::HeatingConvergenceTolerance,value.get());
  }

  // CoolingConvergenceTolerance
  if( (value = modelObject.coolingConvergenceTolerance()) ) {
    idfObject.setDouble(ZoneHVAC_UnitVentilatorFields::CoolingConvergenceTolerance,value.get());
  }

  // AvailabilityManagerListName
  // DesignSpecificationZoneHVACSizingObjectName

  return idfObject;
}

} // energyplus

} // openstudio

