/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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
#include <model/AirTerminalSingleDuctParallelPIUReheat.hpp>
#include <model/AirTerminalSingleDuctParallelPIUReheat_Impl.hpp>
#include <model/HVACComponent.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/CoilHeatingWater.hpp>
#include <model/CoilHeatingWater_Impl.hpp>
#include <utilities/idf/IdfExtensibleGroup.hpp>
#include <utilities/idd/AirTerminal_SingleDuct_ParallelPIU_Reheat_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_ZoneMixer_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Gas_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirTerminalSingleDuctParallelPIUReheat( AirTerminalSingleDuctParallelPIUReheat & modelObject )
{
  OptionalModelObject temp;
  OptionalString optS;
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject _airDistributionUnit(openstudio::IddObjectType::ZoneHVAC_AirDistributionUnit);
  _airDistributionUnit.setName(modelObject.name().get() + " Air Distribution Unit");
  m_idfObjects.push_back(_airDistributionUnit);

  IdfObject idfObject(openstudio::IddObjectType::AirTerminal_SingleDuct_ParallelPIU_Reheat);
  idfObject.setName(modelObject.name().get());
  m_idfObjects.push_back(idfObject);

  HVACComponent coil = modelObject.reheatCoil();

  boost::optional<IdfObject> _reheatCoil = translateAndMapModelObject(coil);

  HVACComponent fan = modelObject.fan();

  boost::optional<IdfObject> _fan = translateAndMapModelObject(fan);

  std::string fanOutletNodeName = modelObject.name().get() + " Fan Outlet";

  std::string mixerOutletNodeName = modelObject.name().get() + " Mixer Outlet";

  boost::optional<std::string> inletNodeName;
  boost::optional<std::string> secondaryAirInletNodeName;
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

  if( boost::optional<Node> secondaryInletNode = modelObject.secondaryAirInletNode() )
  {
    if( secondaryInletNode )
    {
      secondaryAirInletNodeName = secondaryInletNode->name().get();
    }
  }

  if( outletNodeName )
  {
    idfObject.setString(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::OutletNodeName,outletNodeName.get());
  }

  if( inletNodeName )
  {
    idfObject.setString(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::SupplyAirInletNodeName,inletNodeName.get());
  }

  if( secondaryAirInletNodeName )
  {
    idfObject.setString(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::SecondaryAirInletNodeName,secondaryAirInletNodeName.get());
  }

  // Populate fields for AirDistributionUnit
  if( outletNodeName )
  {
    _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirDistributionUnitOutletNodeName,outletNodeName.get());
  }
  _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalObjectType,idfObject.iddObject().name());
  _airDistributionUnit.setString(ZoneHVAC_AirDistributionUnitFields::AirTerminalName,idfObject.name().get());

  // MixerName
  IdfObject _mixer(IddObjectType::AirLoopHVAC_ZoneMixer);
  _mixer.setName(modelObject.name().get() + " Mixer");
  m_idfObjects.push_back(_mixer);
  _mixer.clearExtensibleGroups();

  _mixer.setString(AirLoopHVAC_ZoneMixerFields::OutletNodeName,mixerOutletNodeName);

  IdfExtensibleGroup eg = _mixer.pushExtensibleGroup();
  eg.setString(AirLoopHVAC_ZoneMixerExtensibleFields::InletNodeName,fanOutletNodeName);

  if( inletNodeName )
  {
    eg = _mixer.pushExtensibleGroup();
    eg.setString(AirLoopHVAC_ZoneMixerExtensibleFields::InletNodeName,inletNodeName.get());
  }
  
  idfObject.setString(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::ZoneMixerName,_mixer.name().get());

  // FanName
  if( _fan || _fan->name() )
  {
    idfObject.setString(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::FanName,_fan->name().get());

    if( secondaryAirInletNodeName )
    {
      _fan->setString(Fan_ConstantVolumeFields::AirInletNodeName,secondaryAirInletNodeName.get());
    }

    _fan->setString(Fan_ConstantVolumeFields::AirOutletNodeName,fanOutletNodeName);
  } 

  // ReheatCoilName
  if( _reheatCoil && _reheatCoil->name() )
  {
    // Reheat Coil Name
    idfObject.setString(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::ReheatCoilName,_reheatCoil->name().get());

    // Reheat Coil Type
    idfObject.setString(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::ReheatCoilObjectType,_reheatCoil->iddObject().name());

    idfObject.setString(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::ReheatCoilAirInletNodeName,mixerOutletNodeName);

    if( outletNodeName && inletNodeName )
    {
      if( _reheatCoil->iddObject().type() == IddObjectType::Coil_Heating_Gas )
      {
        _reheatCoil->setString(Coil_Heating_GasFields::AirInletNodeName,mixerOutletNodeName);
        _reheatCoil->setString(Coil_Heating_GasFields::AirOutletNodeName,outletNodeName.get());
      }
      else if( _reheatCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric )
      {
        _reheatCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName,mixerOutletNodeName);
        _reheatCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName,outletNodeName.get());
      }
      else if( _reheatCoil->iddObject().type() == IddObjectType::Coil_Heating_Water )
      {
        _reheatCoil->setString(Coil_Heating_WaterFields::AirInletNodeName,mixerOutletNodeName);
        _reheatCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName,outletNodeName.get());
      }
    }
  }

  // AvailabilityScheduleName
  Schedule availabilitySchedule = modelObject.availabilitySchedule();

  boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule);

  if( _availabilitySchedule && _availabilitySchedule->name() )
  {
    idfObject.setString(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::AvailabilityScheduleName,_availabilitySchedule->name().get());
  }

  // MaximumPrimaryAirFlowRate
  if( modelObject.isMaximumPrimaryAirFlowRateAutosized() )
  {
    idfObject.setString(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MaximumPrimaryAirFlowRate,"Autosize");
  }
  else if( (value = modelObject.maximumPrimaryAirFlowRate()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MaximumPrimaryAirFlowRate,value.get());
  }

  // MaximumSecondaryAirFlowRate
  if( modelObject.isMaximumSecondaryAirFlowRateAutosized() )
  {
    idfObject.setString(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MaximumSecondaryAirFlowRate,"Autosize");
  }
  else if( (value = modelObject.maximumSecondaryAirFlowRate()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MaximumSecondaryAirFlowRate,value.get());
  }

  // MinimumPrimaryAirFlowFraction
  if( modelObject.isMinimumPrimaryAirFlowFractionAutosized() )
  {
    idfObject.setString(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MinimumPrimaryAirFlowFraction,"Autosize");
  }
  else if( (value = modelObject.minimumPrimaryAirFlowFraction()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MinimumPrimaryAirFlowFraction,value.get());
  }

  // FanOnFlowFraction
  if( modelObject.isFanOnFlowFractionAutosized() )
  {
    idfObject.setString(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::FanOnFlowFraction,"Autosize");
  }
  else if( (value = modelObject.fanOnFlowFraction()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::FanOnFlowFraction,value.get());
  }

  // MaximumHotWaterorSteamFlowRate
  if( modelObject.isMaximumHotWaterorSteamFlowRateAutosized() )
  {
    idfObject.setString(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MaximumHotWaterorSteamFlowRate,"Autosize");
  }
  else if( (value = modelObject.maximumHotWaterorSteamFlowRate()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MaximumHotWaterorSteamFlowRate,value.get());
  }

  // MinimumHotWaterorSteamFlowRate
  if( (value = modelObject.minimumHotWaterorSteamFlowRate()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::MinimumHotWaterorSteamFlowRate,value.get());
  }

  // HotWaterorSteamInletNodeName
  if( boost::optional<CoilHeatingWater> coil = modelObject.reheatCoil().optionalCast<CoilHeatingWater>() )
  {
    if( boost::optional<ModelObject> mo = coil->waterInletModelObject() )
    {
      if( boost::optional<Node> node = mo->optionalCast<Node>() )
      {
        idfObject.setString(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::HotWaterorSteamInletNodeName,node->name().get());
      }
    }
  }

  // ConvergenceTolerance
  if( (value = modelObject.convergenceTolerance()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ParallelPIU_ReheatFields::ConvergenceTolerance,value.get());
  }
  
  return _airDistributionUnit;
}

} // energyplus

} // openstudio

