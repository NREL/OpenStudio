/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "../../model/AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../../model/CoilHeatingDXSingleSpeed.hpp"
#include "../../model/CoilHeatingDXSingleSpeed_Impl.hpp"
#include <utilities/idd/AirLoopHVAC_UnitaryHeatPump_AirToAir_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Fuel_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirLoopHVACUnitaryHeatPumpAirToAir( AirLoopHVACUnitaryHeatPumpAirToAir & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::AirLoopHVAC_UnitaryHeatPump_AirToAir);

  m_idfObjects.push_back(idfObject);

  // Name

  s = modelObject.name();
  if( s )
  {
    idfObject.setName(*s);
  }

  // AvailabilityScheduleName

  if( boost::optional<Schedule> schedule = modelObject.availabilitySchedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  // AirInletNodeName

  boost::optional<std::string> airInletNodeName;

  if( boost::optional<ModelObject> mo = modelObject.inletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      if( (s = node->name()) )
      {
        airInletNodeName = s;

        idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::AirInletNodeName,s.get());
      }
    }
  }

  // AirOutletNodeName

  boost::optional<std::string> airOutletNodeName;

  if( boost::optional<ModelObject> mo = modelObject.outletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      if( (s = node->name()) )
      {
        airOutletNodeName = s;

        idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::AirOutletNodeName,s.get());
      }
    }
  }

  // SupplyAirFlowRateDuringCoolingOperation

  if( modelObject.isSupplyAirFlowRateDuringCoolingOperationAutosized() )
  {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::CoolingSupplyAirFlowRate,"Autosize");
  }
  else if( (value = modelObject.supplyAirFlowRateDuringCoolingOperation()) )
  {
    idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::CoolingSupplyAirFlowRate,value.get());
  }

  // SupplyAirFlowRateDuringHeatingOperation

  if( modelObject.isSupplyAirFlowRateDuringHeatingOperationAutosized() )
  {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::HeatingSupplyAirFlowRate,"Autosize");
  }
  else if( (value = modelObject.supplyAirFlowRateDuringHeatingOperation()) )
  {
    idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::HeatingSupplyAirFlowRate,value.get());
  }

  // SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded

  if( modelObject.isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() )
  {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::NoLoadSupplyAirFlowRate,"Autosize");
  }
  else if( (value = modelObject.supplyAirFlowRateWhenNoCoolingorHeatingisNeeded()) )
  {
    idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::NoLoadSupplyAirFlowRate,value.get());
  }

  // ControllingZoneorThermostatLocation

  if( boost::optional<ThermalZone> tz = modelObject.controllingZone() )
  {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::ControllingZoneorThermostatLocation,tz->name().get() );
  }

  // SupplyAirFanName

  HVACComponent fan = modelObject.supplyAirFan();

  boost::optional<IdfObject> _fan = translateAndMapModelObject(fan);

  if( _fan )
  {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFanObjectType,_fan->iddObject().name() );

    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFanName,_fan->name().get());
  }

  // HeatingCoilName

  boost::optional<IdfObject> _heatingCoil;

  HVACComponent heatingCoil = modelObject.heatingCoil();

  if( boost::optional<CoilHeatingDXSingleSpeed> coilHeatingDXSingleSpeed = heatingCoil.optionalCast<CoilHeatingDXSingleSpeed>() )
  {
    _heatingCoil = translateCoilHeatingDXSingleSpeedWithoutUnitary(coilHeatingDXSingleSpeed.get());

    if( _heatingCoil )
    {
      idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::HeatingCoilObjectType,_heatingCoil->iddObject().name());

      idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::HeatingCoilName,_heatingCoil->name().get());
    }
  }

  // CoolingCoilName

  boost::optional<IdfObject> _coolingCoil;

  boost::optional<CoilCoolingDXSingleSpeed> coolingCoil = modelObject.coolingCoil().optionalCast<CoilCoolingDXSingleSpeed>();

  if( coolingCoil )
  {
    _coolingCoil = translateCoilCoolingDXSingleSpeedWithoutUnitary(coolingCoil.get());
  }

  if( _coolingCoil )
  {
    m_map.insert(std::make_pair(coolingCoil->handle(),_coolingCoil.get()));

    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::CoolingCoilObjectType,_coolingCoil->iddObject().name());

    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::CoolingCoilName,_coolingCoil->name().get());
  }

  // SupplementalHeatingCoilName

  boost::optional<IdfObject> _supplementalHeatingCoil;

  boost::optional<HVACComponent> supplementalHeatingCoil = modelObject.supplementalHeatingCoil();

  if( supplementalHeatingCoil )
  {
    _supplementalHeatingCoil = translateAndMapModelObject(supplementalHeatingCoil.get());
  }

  if( _supplementalHeatingCoil )
  {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplementalHeatingCoilObjectType,
      _supplementalHeatingCoil->iddObject().name());

    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplementalHeatingCoilName,_supplementalHeatingCoil->name().get());
  }

  // MaximumSupplyAirTemperaturefromSupplementalHeater

  if( modelObject.isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() )
  {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumSupplyAirTemperaturefromSupplementalHeater,"Autosize");
  }
  else if( (value = modelObject.maximumSupplyAirTemperaturefromSupplementalHeater()) )
  {
    idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumSupplyAirTemperaturefromSupplementalHeater,value.get());
  }

  // MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation

  if( (value = modelObject.maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation()) )
  {
    idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation,value.get());
  }

  // FanPlacement

  if( (s = modelObject.fanPlacement()) )
  {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::FanPlacement,s.get());
  }

  // SupplyAirFanOperatingModeScheduleName

  if( boost::optional<Schedule> schedule = modelObject.supplyAirFanOperatingModeSchedule() )
  {
    boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get());

    if( _schedule )
    {
      idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFanOperatingModeScheduleName,_schedule->name().get());
    }
  }

  // Dehumidification Control Type
  if (!modelObject.isDehumidificationControlTypeDefaulted()) {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::DehumidificationControlType,
                        modelObject.dehumidificationControlType());
  }

  // Fill in node names for inner components

  if( airInletNodeName && _fan )
  {
    if( _fan->iddObject().type() == IddObjectType::Fan_ConstantVolume )
    {
      _fan->setString(Fan_ConstantVolumeFields::AirInletNodeName,airInletNodeName.get());
    }
    else if( _fan->iddObject().type() == IddObjectType::Fan_OnOff )
    {
      _fan->setString(Fan_OnOffFields::AirInletNodeName,airInletNodeName.get());
    }
  }

  //if( airOutletNodeName && _heatingCoil )
  //{
  //  //_heatingCoil->setString(Coil_Heating_DX_SingleSpeedFields::AirOutletNodeName,airOutletNodeName.get());
  //}

  std::string fanOutletNodeName;

  if( _fan && _coolingCoil )
  {
    std::string nodeName = modelObject.name().get() + " Fan - Cooling Coil Node";
    fanOutletNodeName = nodeName;

    if( _fan->iddObject().type() == IddObjectType::Fan_ConstantVolume )
    {
      _fan->setString(Fan_ConstantVolumeFields::AirOutletNodeName,nodeName);
    }
    else if( _fan->iddObject().type() == IddObjectType::Fan_OnOff )
    {
      _fan->setString(Fan_OnOffFields::AirOutletNodeName,nodeName);
    }

    _coolingCoil->setString(Coil_Cooling_DX_SingleSpeedFields::AirInletNodeName,nodeName);
  }

  if( airInletNodeName )
  {
    fixSPMsForUnitarySystem(modelObject,airInletNodeName.get(),fanOutletNodeName);
  }

  if( _coolingCoil && _heatingCoil )
  {
    std::string nodeName = modelObject.name().get() + " Cooling Coil - Heating Coil Node";

    _coolingCoil->setString(Coil_Cooling_DX_SingleSpeedFields::AirOutletNodeName,nodeName);

    _heatingCoil->setString(Coil_Heating_DX_SingleSpeedFields::AirInletNodeName,nodeName);
  }

  if( _supplementalHeatingCoil )
  {
    std::string nodeName = modelObject.name().get() + " Heating Coil - Supplemental Coil Node";

    if( _heatingCoil )
    {
      _heatingCoil->setString(Coil_Heating_DX_SingleSpeedFields::AirOutletNodeName,nodeName);
    }

    if( _supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Fuel )
    {
      if( airOutletNodeName )
      {
        _supplementalHeatingCoil->setString(Coil_Heating_FuelFields::AirOutletNodeName,airOutletNodeName.get());

        _supplementalHeatingCoil->setString(Coil_Heating_FuelFields::AirInletNodeName,nodeName);
      }
    }
    else if( _supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric )
    {
      if( airOutletNodeName )
      {
        _supplementalHeatingCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName,airOutletNodeName.get());

        _supplementalHeatingCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName,nodeName);
      }
    }
  }

  return idfObject;
}

} // energyplus

} // openstudio

