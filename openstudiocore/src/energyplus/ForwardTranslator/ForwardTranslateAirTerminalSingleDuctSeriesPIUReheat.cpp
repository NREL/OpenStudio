/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/AirTerminalSingleDuctSeriesPIUReheat.hpp"
#include "../../model/AirTerminalSingleDuctSeriesPIUReheat_Impl.hpp"
#include "../../model/HVACComponent.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/CoilHeatingWater.hpp"
#include "../../model/CoilHeatingWater_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/AirTerminal_SingleDuct_SeriesPIU_Reheat_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include <utilities/idd/AirLoopHVAC_ZoneMixer_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Fuel_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirTerminalSingleDuctSeriesPIUReheat( AirTerminalSingleDuctSeriesPIUReheat & modelObject )
{
  OptionalModelObject temp;
  OptionalString optS;
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject _airDistributionUnit(openstudio::IddObjectType::ZoneHVAC_AirDistributionUnit);
  _airDistributionUnit.setName(modelObject.name().get() + " Air Distribution Unit");
  m_idfObjects.push_back(_airDistributionUnit);

  IdfObject idfObject(openstudio::IddObjectType::AirTerminal_SingleDuct_SeriesPIU_Reheat);
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
    idfObject.setString(AirTerminal_SingleDuct_SeriesPIU_ReheatFields::OutletNodeName,outletNodeName.get());
  }

  if( inletNodeName )
  {
    idfObject.setString(AirTerminal_SingleDuct_SeriesPIU_ReheatFields::SupplyAirInletNodeName,inletNodeName.get());
  }

  if( secondaryAirInletNodeName )
  {
    idfObject.setString(AirTerminal_SingleDuct_SeriesPIU_ReheatFields::SecondaryAirInletNodeName,secondaryAirInletNodeName.get());
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

  if( secondaryAirInletNodeName )
  {
    IdfExtensibleGroup eg = _mixer.pushExtensibleGroup();
    eg.setString(AirLoopHVAC_ZoneMixerExtensibleFields::InletNodeName,secondaryAirInletNodeName.get());
  }

  if( inletNodeName )
  {
    IdfExtensibleGroup eg = _mixer.pushExtensibleGroup();
    eg.setString(AirLoopHVAC_ZoneMixerExtensibleFields::InletNodeName,inletNodeName.get());
  }

  idfObject.setString(AirTerminal_SingleDuct_SeriesPIU_ReheatFields::ZoneMixerName,_mixer.name().get());

  // FanName
  if( _fan || _fan->name() )
  {
    idfObject.setString(AirTerminal_SingleDuct_SeriesPIU_ReheatFields::FanName,_fan->name().get());

    _fan->setString(Fan_ConstantVolumeFields::AirInletNodeName,mixerOutletNodeName);

    _fan->setString(Fan_ConstantVolumeFields::AirOutletNodeName,fanOutletNodeName);
  }

  // ReheatCoilName
  if( _reheatCoil && _reheatCoil->name() )
  {
    // Reheat Coil Name
    idfObject.setString(AirTerminal_SingleDuct_SeriesPIU_ReheatFields::ReheatCoilName,_reheatCoil->name().get());

    // Reheat Coil Type
    idfObject.setString(AirTerminal_SingleDuct_SeriesPIU_ReheatFields::ReheatCoilObjectType,_reheatCoil->iddObject().name());

    idfObject.setString(AirTerminal_SingleDuct_SeriesPIU_ReheatFields::ReheatCoilAirInletNodeName,fanOutletNodeName);

    if( outletNodeName )
    {
      if( _reheatCoil->iddObject().type() == IddObjectType::Coil_Heating_Fuel )
      {
        _reheatCoil->setString(Coil_Heating_FuelFields::AirInletNodeName,fanOutletNodeName);
        _reheatCoil->setString(Coil_Heating_FuelFields::AirOutletNodeName,outletNodeName.get());
      }
      else if( _reheatCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric )
      {
        _reheatCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName,fanOutletNodeName);
        _reheatCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName,outletNodeName.get());
      }
      else if( _reheatCoil->iddObject().type() == IddObjectType::Coil_Heating_Water )
      {
        _reheatCoil->setString(Coil_Heating_WaterFields::AirInletNodeName,fanOutletNodeName);
        _reheatCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName,outletNodeName.get());
      }
    }
  }

  // AvailabilityScheduleName
  if( boost::optional<Schedule> availabilitySchedule = modelObject.availabilitySchedule() )
  {
    boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule.get());

    if( _availabilitySchedule && _availabilitySchedule->name() )
    {
      idfObject.setString(AirTerminal_SingleDuct_SeriesPIU_ReheatFields::AvailabilityScheduleName,_availabilitySchedule->name().get());
    }
  }

  // MaximumAirFlowRate
  if( modelObject.isMaximumAirFlowRateAutosized() )
  {
    idfObject.setString(AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MaximumAirFlowRate,"Autosize");
  }
  else if( (value = modelObject.maximumAirFlowRate()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MaximumAirFlowRate,value.get());
  }

  // MaximumPrimaryAirFlowRate
  if( modelObject.isMaximumPrimaryAirFlowRateAutosized() )
  {
    idfObject.setString(AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MaximumPrimaryAirFlowRate,"Autosize");
  }
  else if( (value = modelObject.maximumPrimaryAirFlowRate()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MaximumPrimaryAirFlowRate,value.get());
  }

  // MinimumPrimaryAirFlowFraction
  if( modelObject.isMinimumPrimaryAirFlowFractionAutosized() )
  {
    idfObject.setString(AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MinimumPrimaryAirFlowFraction,"Autosize");
  }
  else if( (value = modelObject.minimumPrimaryAirFlowFraction()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MinimumPrimaryAirFlowFraction,value.get());
  }

  // MaximumHotWaterorSteamFlowRate
  if( modelObject.isMaximumHotWaterorSteamFlowRateAutosized() )
  {
    idfObject.setString(AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MaximumHotWaterorSteamFlowRate,"Autosize");
  }
  else if( (value = modelObject.maximumHotWaterorSteamFlowRate()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MaximumHotWaterorSteamFlowRate,value.get());
  }

  // MinimumHotWaterorSteamFlowRate
  if( (value = modelObject.minimumHotWaterorSteamFlowRate()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_SeriesPIU_ReheatFields::MinimumHotWaterorSteamFlowRate,value.get());
  }

  // ConvergenceTolerance
  if( (value = modelObject.convergenceTolerance()) )
  {
    idfObject.setDouble(AirTerminal_SingleDuct_SeriesPIU_ReheatFields::ConvergenceTolerance,value.get());
  }

  return _airDistributionUnit;
}

} // energyplus

} // openstudio

