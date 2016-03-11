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
#include "../../model/AirTerminalSingleDuctVAVHeatAndCoolReheat.hpp"
#include "../../model/AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl.hpp"
#include "../../model/HVACComponent.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/WaterToAirComponent.hpp"
#include "../../model/WaterToAirComponent_Impl.hpp"
#include <utilities/idd/AirTerminal_SingleDuct_VAV_HeatAndCool_Reheat_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Gas_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirTerminalSingleDuctVAVHeatAndCoolReheat( AirTerminalSingleDuctVAVHeatAndCoolReheat & modelObject )
{
  OptionalModelObject temp;
  OptionalString optS;
  boost::optional<std::string> s;
  boost::optional<double> value;

  std::string baseName = modelObject.name().get();

  IdfObject _airDistributionUnit(openstudio::IddObjectType::ZoneHVAC_AirDistributionUnit);
  _airDistributionUnit.setName("ADU " + baseName ); //ADU: Air Distribution Unit

  IdfObject idfObject(openstudio::IddObjectType::AirTerminal_SingleDuct_VAV_HeatAndCool_Reheat);
  idfObject.setName(baseName);

  m_idfObjects.push_back(_airDistributionUnit);
  m_idfObjects.push_back(idfObject);

  auto coil = modelObject.reheatCoil();
  auto _coil = translateAndMapModelObject(coil);


  boost::optional<std::string> inletNodeName;
  boost::optional<std::string> outletNodeName;

  if( boost::optional<ModelObject> inletModelObject = modelObject.inletModelObject() )
  {
    if( boost::optional<Node> inletNode = inletModelObject->optionalCast<Node>() )
    {
      inletNodeName = inletNode->name().get();
    }
  }

  if( boost::optional<ModelObject> outletModelObject = modelObject.outletModelObject() )
  {
    if( boost::optional<Node> outletNode = outletModelObject->optionalCast<Node>() )
    {
      outletNodeName = outletNode->name().get();
    }
  }

  std::string damperOutletNodeName = baseName + " Damper Outlet Node";

  if( outletNodeName && inletNodeName )
  {
    if( _coil ) {
      if( _coil->iddObject().type() == IddObjectType::Coil_Heating_Gas )
      {
        _coil->setString(Coil_Heating_GasFields::AirInletNodeName,damperOutletNodeName);
        _coil->setString(Coil_Heating_GasFields::AirOutletNodeName,outletNodeName.get());
      }
      else if( _coil->iddObject().type() == IddObjectType::Coil_Heating_Electric )
      {
        _coil->setString(Coil_Heating_ElectricFields::AirInletNodeName,damperOutletNodeName);
        _coil->setString(Coil_Heating_ElectricFields::AirOutletNodeName,outletNodeName.get());
      }
      else if( _coil->iddObject().type() == IddObjectType::Coil_Heating_Water )
      {
        _coil->setString(Coil_Heating_WaterFields::AirInletNodeName,damperOutletNodeName);
        _coil->setString(Coil_Heating_WaterFields::AirOutletNodeName,outletNodeName.get());
      }
    }

    idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::AirOutletNodeName,outletNodeName.get());
    idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::AirInletNodeName,inletNodeName.get());
  }

  // AvailabilityScheduleName
  if( auto schedule = modelObject.availabilitySchedule() ) {
    if( auto _schedule = translateAndMapModelObject(schedule.get()) ) {
      idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  // DamperAirOutletNodeName
  idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::DamperAirOutletNodeName,damperOutletNodeName);

  // AirInletNodeName
  if( auto node = modelObject.inletModelObject() ) {
    idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::AirInletNodeName,node->name().get());
  }

  // MaximumAirFlowRate
  if( modelObject.isMaximumAirFlowRateAutosized() ) {
    idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumAirFlowRate,"AutoSize");
  } else if( (value = modelObject.maximumAirFlowRate()) ) {
    idfObject.setDouble(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumAirFlowRate,value.get());
  }

  // ZoneMinimumAirFlowFraction
  if( (value = modelObject.zoneMinimumAirFlowFraction()) ) {
    idfObject.setDouble(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::ZoneMinimumAirFlowFraction,value.get());
  }

  if( _coil ) {
    // ReheatCoilObjectType
    idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::ReheatCoilObjectType,_coil->iddObject().name());

    // ReheatCoilName
    idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::ReheatCoilName,_coil->name().get());
  }

  // MaximumHotWaterorSteamFlowRate
  if( modelObject.isMaximumHotWaterorSteamFlowRateAutosized() ) {
    idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumHotWaterorSteamFlowRate,"AutoSize");
  } else if( (value = modelObject.maximumHotWaterorSteamFlowRate()) ) {
    idfObject.setDouble(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumHotWaterorSteamFlowRate,value.get());
  }

  // MinimumHotWaterorSteamFlowRate
  if( (value = modelObject.minimumHotWaterorSteamFlowRate()) ) {
    idfObject.setDouble(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MinimumHotWaterorSteamFlowRate,value.get());
  }

  // AirOutletNodeName
  if( auto node = modelObject.outletModelObject() ) {
    idfObject.setString(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::AirOutletNodeName,node->name().get());
  }

  // ConvergenceTolerance
  if( (value = modelObject.convergenceTolerance()) ) {
    idfObject.setDouble(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::ConvergenceTolerance,value.get());
  }

  // MaximumReheatAirTemperature
  if( (value = modelObject.maximumReheatAirTemperature()) ) {
    idfObject.setDouble(AirTerminal_SingleDuct_VAV_HeatAndCool_ReheatFields::MaximumReheatAirTemperature,value.get());
  }

  // Populate fields for AirDistributionUnit
  if( boost::optional<ModelObject> outletNode = modelObject.outletModelObject() )
  {
    _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirDistributionUnitOutletNodeName,outletNode->name().get());
  }
  _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalObjectType,idfObject.iddObject().name());
  _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalName,idfObject.name().get());

  return _airDistributionUnit;
}

} // energyplus

} // openstudio

