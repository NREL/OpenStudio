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
#include <model/AirTerminalSingleDuctConstantVolumeReheat.hpp>
#include <model/AirTerminalSingleDuctConstantVolumeReheat_Impl.hpp>
#include <model/HVACComponent.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <utilities/idd/AirTerminal_SingleDuct_ConstantVolume_Reheat_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Gas_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirTerminalSingleDuctConstantVolumeReheat( AirTerminalSingleDuctConstantVolumeReheat& modelObject )
{
  OptionalModelObject temp;
  OptionalString optS;
  boost::optional<std::string> s;

  IdfObject _airDistributionUnit(openstudio::IddObjectType::ZoneHVAC_AirDistributionUnit);
  _airDistributionUnit.createName();

  IdfObject idfObject(openstudio::IddObjectType::AirTerminal_SingleDuct_ConstantVolume_Reheat);

  idfObject.setName(modelObject.name().get());

  HVACComponent coil = modelObject.reheatCoil();

  m_idfObjects.push_back(_airDistributionUnit);

  m_idfObjects.push_back(idfObject);

  boost::optional<IdfObject> _reheatCoil = translateAndMapModelObject(coil);

  if( _reheatCoil && _reheatCoil->name() )
  {
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

    // Reheat Coil Name
    idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ReheatCoilName,_reheatCoil->name().get());

    // Reheat Coil Type
    idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ReheatCoilObjectType,_reheatCoil->iddObject().name());

    if( outletNodeName && inletNodeName )
    {
      if( _reheatCoil->iddObject().type() == IddObjectType::Coil_Heating_Gas )
      {
        _reheatCoil->setString(Coil_Heating_GasFields::AirInletNodeName,inletNodeName.get());
        _reheatCoil->setString(Coil_Heating_GasFields::AirOutletNodeName,outletNodeName.get());
      }
      else if( _reheatCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric )
      {
        _reheatCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName,inletNodeName.get());
        _reheatCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName,outletNodeName.get());
      }
      else if( _reheatCoil->iddObject().type() == IddObjectType::Coil_Heating_Water )
      {
        _reheatCoil->setString(Coil_Heating_WaterFields::AirInletNodeName,inletNodeName.get());
        _reheatCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName,outletNodeName.get());
      }

      idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::AirOutletNodeName,outletNodeName.get());
      idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::AirInletNodeName,inletNodeName.get());
    }
  }
  else
  {
    LOG(Error,modelObject.briefDescription() << ": Could not translate heating coil");

    return boost::none;
  }

  // AvailabilityScheduleName
  Schedule availabilitySchedule = modelObject.availabilitySchedule();

  boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule);

  if( _availabilitySchedule && _availabilitySchedule->name() )
  {
    idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::AvailabilityScheduleName,_availabilitySchedule->name().get());
  }

  // MaximumAirFlowRate
  boost::optional<double> value = modelObject.maximumAirFlowRate();
  if( value )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumAirFlowRate,value.get());
  }
  else if( modelObject.isMaximumAirFlowRateAutosized() )
  {
    idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumAirFlowRate,"Autosize");
  }

  // HotWaterorSteamInletNodeName
  idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::HotWaterorSteamInletNodeName,"");

  // MaximumHotWaterOrSteamFlowRate
  value = modelObject.maximumHotWaterorSteamFlowRate();
  
  if( value )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumHotWaterorSteamFlowRate,value.get());
  }
  else if( modelObject.isMaximumHotWaterorSteamFlowRateAutosized() )
  {
    idfObject.setString(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumHotWaterorSteamFlowRate,"Autosize");
  }

  // MinimumHotWaterOrSteamFlowRate
  value = modelObject.minimumHotWaterorSteamFlowRate();
  idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MinimumHotWaterorSteamFlowRate,value.get());

  // ConvergenceTolerance
  value = modelObject.convergenceTolerance();
  idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::ConvergenceTolerance,value.get());

  // MaximumReheatAirTemperature
  value = modelObject.maximumReheatAirTemperature();
  idfObject.setDouble(AirTerminal_SingleDuct_ConstantVolume_ReheatFields::MaximumReheatAirTemperature,value.get());

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

