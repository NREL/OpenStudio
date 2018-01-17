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
#include "../../model/AirTerminalSingleDuctConstantVolumeReheat.hpp"
#include "../../model/AirTerminalSingleDuctConstantVolumeReheat_Impl.hpp"
#include "../../model/HVACComponent.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include <utilities/idd/AirTerminal_SingleDuct_ConstantVolume_Reheat_FieldEnums.hxx>
#include <utilities/idd/ZoneHVAC_AirDistributionUnit_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Fuel_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
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

  std::string baseName = modelObject.name().get();

  IdfObject _airDistributionUnit(openstudio::IddObjectType::ZoneHVAC_AirDistributionUnit);
  _airDistributionUnit.setName("ADU " + baseName ); //ADU: Air Distribution Unit

  IdfObject idfObject(openstudio::IddObjectType::AirTerminal_SingleDuct_ConstantVolume_Reheat);

  idfObject.setName(baseName);

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
      if( _reheatCoil->iddObject().type() == IddObjectType::Coil_Heating_Fuel )
      {
        _reheatCoil->setString(Coil_Heating_FuelFields::AirInletNodeName,inletNodeName.get());
        _reheatCoil->setString(Coil_Heating_FuelFields::AirOutletNodeName,outletNodeName.get());
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

