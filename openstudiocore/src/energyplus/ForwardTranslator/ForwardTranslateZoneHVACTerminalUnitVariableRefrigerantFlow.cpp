/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include <energyplus/ForwardTranslator.hpp>
#include <model/Model.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/FanOnOff.hpp>
#include <model/FanOnOff_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp>
#include <model/ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl.hpp>
#include <model/Curve.hpp>
#include <model/Curve_Impl.hpp>
#include <model/CurveBiquadratic.hpp>
#include <model/CurveBiquadratic_Impl.hpp>
#include <model/CurveCubic.hpp>
#include <model/CurveCubic_Impl.hpp>
#include <model/CoilCoolingDXVariableRefrigerantFlow.hpp>
#include <model/CoilCoolingDXVariableRefrigerantFlow_Impl.hpp>
#include <model/CoilHeatingDXVariableRefrigerantFlow.hpp>
#include <model/CoilHeatingDXVariableRefrigerantFlow_Impl.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/idd/ZoneHVAC_TerminalUnit_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/OutdoorAir_Mixer_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneHVACTerminalUnitVariableRefrigerantFlow( ZoneHVACTerminalUnitVariableRefrigerantFlow & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow);

  m_idfObjects.push_back(idfObject);

  // Name

  s = modelObject.name();
  if( s )
  {
    idfObject.setName(*s);
  }

  // TerminalUnitAvailabilityschedule

  if( boost::optional<model::Schedule> schedule = modelObject.terminalUnitAvailabilityschedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAvailabilitySchedule,_schedule->name().get());
    }
  }

  // SupplyAirFlowRateDuringCoolingOperation
  
  if( modelObject.isSupplyAirFlowRateDuringCoolingOperationAutosized() )
  {
    idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateDuringCoolingOperation,"Autosize");
  }
  else if( (value = modelObject.supplyAirFlowRateDuringCoolingOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateDuringCoolingOperation,value.get()); 
  }
  
  // SupplyAirFlowRateWhenNoCoolingisNeeded

  if( modelObject.isSupplyAirFlowRateWhenNoCoolingisNeededAutosized() )
  {
    idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateWhenNoCoolingisNeeded,"Autosize");
  }
  else if( (value = modelObject.supplyAirFlowRateWhenNoCoolingisNeeded()) )
  {
    idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateWhenNoCoolingisNeeded,value.get());
  }

  // SupplyAirFlowRateDuringHeatingOperation
  
  if( modelObject.isSupplyAirFlowRateDuringHeatingOperationAutosized() )
  {
    idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateDuringHeatingOperation,"Autosize");
  }
  else if( (value = modelObject.supplyAirFlowRateDuringHeatingOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateDuringHeatingOperation,value.get()); 
  }
  
  // SupplyAirFlowRateWhenNoHeatingisNeeded

  if( modelObject.isSupplyAirFlowRateWhenNoHeatingisNeededAutosized() )
  {
    idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateWhenNoHeatingisNeeded,"Autosize");
  }
  else if( (value = modelObject.supplyAirFlowRateWhenNoHeatingisNeeded()) )
  {
    idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateWhenNoHeatingisNeeded,value.get());
  }

  // OutdoorAirFlowRateDuringCoolingOperation

  if( modelObject.isOutdoorAirFlowRateDuringCoolingOperationAutosized() )
  {
    idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateDuringCoolingOperation,"Autosize");
  }
  else if( (value = modelObject.outdoorAirFlowRateDuringCoolingOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateDuringCoolingOperation,value.get());
  }

  // OutdoorAirFlowRateDuringHeatingOperation

  if( modelObject.isOutdoorAirFlowRateDuringHeatingOperationAutosized() )
  {
    idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateDuringHeatingOperation,"Autosize");
  }
  else if( (value = modelObject.outdoorAirFlowRateDuringHeatingOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateDuringHeatingOperation,value.get());
  }

  // OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded

  if( modelObject.isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() )
  {
    idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded,"Autosize");
  }
  else if( (value = modelObject.outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded()) )
  {
    idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded,value.get());
  }

  // SupplyAirFanOperatingModeScheduleName

  if( boost::optional<model::Schedule> schedule = modelObject.supplyAirFanOperatingModeSchedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanOperatingModeScheduleName,_schedule->name().get());
    }
  }

  // SupplyAirFanplacement

  idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanPlacement,"DrawThrough");

  model::ModelObject fan = modelObject.supplyAirFan();

  // Node Names

  std::string inletNodeName;
  std::string outletNodeName;
  std::string mixerOutletNodeName;
  std::string coolOutletNodeName;
  std::string heatOutletNodeName;
  std::string oaNodeName;

  if( boost::optional<model::Node> node = modelObject.inletNode() )
  {
    inletNodeName = node->name().get();      
  } 

  if( boost::optional<model::Node> node = modelObject.outletNode() )
  {
    outletNodeName = node->name().get();
  }

  mixerOutletNodeName = modelObject.name().get() + " Mixer Outlet Node";

  coolOutletNodeName = modelObject.name().get() + " Cooling Coil Outlet Node";

  heatOutletNodeName = modelObject.name().get() + " Heating Coil Outlet Node";

  oaNodeName = modelObject.name().get() + " Outdoor Air Node";

  // TerminalUnitAirInletNodeName

  idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAirInletNodeName,inletNodeName);

  // TerminalUnitAirOutletNodeName

  idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAirOutletNodeName,outletNodeName);

  if( boost::optional<IdfObject> _fan = translateAndMapModelObject(fan) )
  {
    // SupplyAirFanObjectType

    idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanObjectType,_fan->iddObject().name());

    // SupplyAirFanObjectName

    idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanObjectName,_fan->name().get());

    if( fan.iddObject().type() == model::FanOnOff::iddObjectType() )
    {
      _fan->setString(Fan_OnOffFields::AirInletNodeName,heatOutletNodeName);

      _fan->setString(Fan_OnOffFields::AirOutletNodeName,outletNodeName);
    }
    else
    {
      LOG(Error, "VRF named " << modelObject.name().get() << " uses an unsupported fan type.");
    }
  }

  // OutdoorAirMixer

  IdfObject _outdoorAirMixer(IddObjectType::OutdoorAir_Mixer);
  _outdoorAirMixer.setName(modelObject.name().get() + " OA Mixer");
  m_idfObjects.push_back(_outdoorAirMixer);

  _outdoorAirMixer.setString(OutdoorAir_MixerFields::MixedAirNodeName,mixerOutletNodeName);

  _outdoorAirMixer.setString(OutdoorAir_MixerFields::ReturnAirStreamNodeName,inletNodeName);

  IdfObject _oaNodeList(openstudio::IddObjectType::OutdoorAir_NodeList);
  _oaNodeList.setString(0,oaNodeName);
  m_idfObjects.push_back(_oaNodeList);

  _outdoorAirMixer.setString(OutdoorAir_MixerFields::ReliefAirStreamNodeName,modelObject.name().get() + " Relief Node Name");

  _outdoorAirMixer.setString(OutdoorAir_MixerFields::OutdoorAirStreamNodeName,oaNodeName);

  // OutsideAirMixerObjectType

  idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutsideAirMixerObjectType,_outdoorAirMixer.iddObject().name());

  // OutsideAirMixerObjectName

  idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutsideAirMixerObjectName,_outdoorAirMixer.name().get());

  model::ModelObject coolingCoil = modelObject.coolingCoil();
  
  if( boost::optional<IdfObject> _coolingCoil = translateAndMapModelObject(coolingCoil) )
  {
    // CoolingCoilObjectType

    idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::CoolingCoilObjectType,_coolingCoil->iddObject().name());

    // CoolingCoilObjectName
    
    idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::CoolingCoilObjectName,_coolingCoil->name().get());

    _coolingCoil->setString(Coil_Cooling_DX_VariableRefrigerantFlowFields::CoilAirInletNode,mixerOutletNodeName);

    _coolingCoil->setString(Coil_Cooling_DX_VariableRefrigerantFlowFields::CoilAirOutletNode,coolOutletNodeName);
  }

  model::ModelObject heatingCoil = modelObject.heatingCoil();
  
  if( boost::optional<IdfObject> _heatingCoil = translateAndMapModelObject(heatingCoil) )
  {
    // HeatingCoilObjectType

    idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::HeatingCoilObjectType,_heatingCoil->iddObject().name());

    // HeatingCoilObjectName
    
    idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::HeatingCoilObjectName,_heatingCoil->name().get());

    _heatingCoil->setString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirInletNode,coolOutletNodeName);

    _heatingCoil->setString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirOutletNode,heatOutletNodeName);
  }

  // ZoneTerminalUnitOnParasiticElectricEnergyUse

  if( (value = modelObject.zoneTerminalUnitOnParasiticElectricEnergyUse()) )
  {
    idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::ZoneTerminalUnitOnParasiticElectricEnergyUse,value.get());
  }

  // ZoneTerminalUnitOffParasiticElectricEnergyUse

  if( (value = modelObject.zoneTerminalUnitOffParasiticElectricEnergyUse()) )
  {
    idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::ZoneTerminalUnitOffParasiticElectricEnergyUse,value.get());
  }

  // RatedTotalHeatingCapacitySizingRatio

  if( (value = modelObject.ratedTotalHeatingCapacitySizingRatio()) )
  {
    idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::RatedTotalHeatingCapacitySizingRatio,value.get());
  }

  return idfObject;
}

} // energyplus

} // openstudio

