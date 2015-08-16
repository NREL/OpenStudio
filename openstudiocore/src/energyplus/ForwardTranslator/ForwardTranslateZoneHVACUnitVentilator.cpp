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
#include "../../model/ZoneHVACUnitVentilator.hpp"
#include "../../model/ZoneHVACUnitVentilator_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
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
  if( auto node = modelObject.inletNode() ) {
    idfObject.setString(ZoneHVAC_UnitVentilatorFields::AirInletNodeName,node->name().get());
  }

  // AirOutletNodeName
  if( auto node = modelObject.inletNode() ) {
    idfObject.setString(ZoneHVAC_UnitVentilatorFields::AirInletNodeName,node->name().get());
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

  // SupplyAirFanObjectType
  // SupplyAirFanName
  {
    auto supplyAirFan = modelObject.supplyAirFan();
    if( auto _supplyAirFan = translateAndMapModelObject(supplyAirFan) ) {
      idfObject.setString(ZoneHVAC_UnitVentilatorFields::SupplyAirFanObjectType,_supplyAirFan->iddObject().name());
      idfObject.setString(ZoneHVAC_UnitVentilatorFields::SupplyAirFanName,_supplyAirFan->name().get());
    }
  }
  // CoilOption
  auto heatingCoil = modelObject.heatingCoil();
  auto coolingCoil = modelObject.coolingCoil();

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
  // HeatingCoilObjectType
  // HeatingCoilName
  if( heatingCoil ) {
    if( auto _heatingCoil = translateAndMapModelObject(heatingCoil.get()) ) {
      idfObject.setString(ZoneHVAC_UnitVentilatorFields::HeatingCoilObjectType,_heatingCoil->iddObject().name());
      idfObject.setString(ZoneHVAC_UnitVentilatorFields::HeatingCoilName,_heatingCoil->name().get());
    }
  }

  // HeatingConvergenceTolerance
  if( (value = modelObject.heatingConvergenceTolerance()) ) {
    idfObject.setDouble(ZoneHVAC_UnitVentilatorFields::HeatingConvergenceTolerance,value.get());
  }

  // CoolingCoilObjectType
  // CoolingCoilName
  if( coolingCoil ) {
    if( auto _coolingCoil = translateAndMapModelObject(coolingCoil.get()) ) {
      idfObject.setString(ZoneHVAC_UnitVentilatorFields::CoolingCoilObjectType,_coolingCoil->iddObject().name());
      idfObject.setString(ZoneHVAC_UnitVentilatorFields::CoolingCoilName,_coolingCoil->name().get());
    }
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

