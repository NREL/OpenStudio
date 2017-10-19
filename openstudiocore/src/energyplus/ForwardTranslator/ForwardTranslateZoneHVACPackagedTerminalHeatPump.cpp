/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ZoneHVACPackagedTerminalHeatPump.hpp"
#include "../../model/ZoneHVACPackagedTerminalHeatPump_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../../model/CoilHeatingDXSingleSpeed.hpp"
#include "../../model/CoilHeatingDXSingleSpeed_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include <utilities/idd/ZoneHVAC_PackagedTerminalHeatPump_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Fuel_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/OutdoorAir_Mixer_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/math/FloatCompare.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneHVACPackagedTerminalHeatPump( 
    ZoneHVACPackagedTerminalHeatPump & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::ZoneHVAC_PackagedTerminalHeatPump);

  m_idfObjects.push_back(idfObject);

  // Name

  std::string baseName = modelObject.name().get();

  idfObject.setName(baseName);

  std::string mixedAirNodeName = baseName + " Mixed Air Node";

  std::string coolingCoilOutletNodeName = baseName + " Cooling Coil Outlet Node";

  std::string heatingCoilOutletNodeName = baseName + " Heating Coil Outlet Node";

  std::string fanOutletNodeName = baseName + " Fan Outlet Node";

  std::string reliefAirNodeName = baseName + " Relief Air Node";

  std::string oaNodeName = baseName + " OA Node";

  std::string airInletNodeName;
  if( auto node = modelObject.inletNode() ) {
    airInletNodeName = node->nameString();
  }

  std::string airOutletNodeName;
  if( auto node = modelObject.outletNode() ) {
    airOutletNodeName = node->nameString();
  }

  auto translateMixer = [&]() {
    auto t_airLoopHVAC = modelObject.airLoopHVAC();
    if( t_airLoopHVAC ) {
      return false;
    }

    bool zeroOA = false;
    if( (value = modelObject.outdoorAirFlowRateDuringCoolingOperation()) ) {
      zeroOA = equal(value.get(),0.0);
    }
    if( (value = modelObject.outdoorAirFlowRateDuringHeatingOperation()) ) {
      zeroOA = (zeroOA && equal(value.get(),0.0));
    }
    if( (value = modelObject.outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded()) ) {
      zeroOA = (zeroOA && equal(value.get(),0.0));
    }

    if( zeroOA ) return false;

    return true;
  };

  // AvailabilityScheduleName

  if( boost::optional<Schedule> schedule = modelObject.availabilitySchedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  // AirInletNodeName
  idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::AirInletNodeName,airInletNodeName);

  // AirOutletNodeName
  idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::AirOutletNodeName,airOutletNodeName);

  if( translateMixer() ) {
    // OutdoorAirMixerObjectType
    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirMixerObjectType,
                        modelObject.outdoorAirMixerObjectType());

    // OutdoorAirMixerName
    
    std::string oaMixerName = modelObject.name().get() + " OA Mixer";  

    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirMixerName,oaMixerName);

    IdfObject _outdoorAirMixer(IddObjectType::OutdoorAir_Mixer);
    _outdoorAirMixer.setName(oaMixerName);
    m_idfObjects.push_back(_outdoorAirMixer);

    _outdoorAirMixer.setString(OutdoorAir_MixerFields::MixedAirNodeName,mixedAirNodeName);

    _outdoorAirMixer.setString(OutdoorAir_MixerFields::OutdoorAirStreamNodeName,oaNodeName);

    IdfObject _oaNodeList(openstudio::IddObjectType::OutdoorAir_NodeList);
    _oaNodeList.setString(0,oaNodeName);
    m_idfObjects.push_back(_oaNodeList);

    _outdoorAirMixer.setString(OutdoorAir_MixerFields::ReliefAirStreamNodeName,reliefAirNodeName);

    _outdoorAirMixer.setString(OutdoorAir_MixerFields::ReturnAirStreamNodeName,airInletNodeName);
  }

  // SupplyAirFlowRateDuringCoolingOperation

  if( modelObject.isSupplyAirFlowRateDuringCoolingOperationAutosized() )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingSupplyAirFlowRate,"Autosize");
  }
  else if( (value = modelObject.supplyAirFlowRateDuringCoolingOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingSupplyAirFlowRate,value.get());
  }

  // SupplyAirFlowRateDuringHeatingOperation

  if( modelObject.isSupplyAirFlowRateDuringHeatingOperationAutosized() )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingSupplyAirFlowRate,"Autosize");
  }
  else if( (value = modelObject.supplyAirFlowRateDuringHeatingOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingSupplyAirFlowRate,value.get());
  }

  // SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded

  if( modelObject.isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::NoLoadSupplyAirFlowRate,"Autosize");
  } 
  else if( (value = modelObject.supplyAirFlowRateWhenNoCoolingorHeatingisNeeded()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalHeatPumpFields::NoLoadSupplyAirFlowRate,value.get());
  }

  // OutdoorAirFlowRateDuringCoolingOperation
  
  if( modelObject.isOutdoorAirFlowRateDuringCoolingOperationAutosized() )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingOutdoorAirFlowRate,"Autosize");
  }
  else if( (value = modelObject.outdoorAirFlowRateDuringCoolingOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingOutdoorAirFlowRate,value.get());
  }

  // OutdoorAirFlowRateDuringHeatingOperation
  
  if( modelObject.isOutdoorAirFlowRateDuringHeatingOperationAutosized() )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingOutdoorAirFlowRate,"Autosize");
  }
  else if( (value = modelObject.outdoorAirFlowRateDuringHeatingOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingOutdoorAirFlowRate,value.get());
  }

  // OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded
  
  if( modelObject.isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::NoLoadOutdoorAirFlowRate,"Autosize");
  }
  else if( (value = modelObject.outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalHeatPumpFields::NoLoadOutdoorAirFlowRate,value.get());
  }

  // SupplyAirFanObjectType

  HVACComponent supplyAirFan = modelObject.supplyAirFan();

  if( boost::optional<IdfObject> _supplyAirFan = translateAndMapModelObject(supplyAirFan) )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFanObjectType,_supplyAirFan->iddObject().name() );

    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFanName,_supplyAirFan->name().get());

    if( istringEqual(modelObject.fanPlacement(),"BlowThrough") ) {
      if( translateMixer() ) {
        if( _supplyAirFan->iddObject().type() == IddObjectType::Fan_ConstantVolume )
        {
          _supplyAirFan->setString(Fan_ConstantVolumeFields::AirInletNodeName,mixedAirNodeName);
          _supplyAirFan->setString(Fan_ConstantVolumeFields::AirOutletNodeName,fanOutletNodeName);
        }
        else if( _supplyAirFan->iddObject().type() == IddObjectType::Fan_OnOff )
        {
          _supplyAirFan->setString(Fan_OnOffFields::AirInletNodeName,mixedAirNodeName);
          _supplyAirFan->setString(Fan_OnOffFields::AirOutletNodeName,fanOutletNodeName);
        }
      } else {
        if( _supplyAirFan->iddObject().type() == IddObjectType::Fan_ConstantVolume )
        {
          _supplyAirFan->setString(Fan_ConstantVolumeFields::AirInletNodeName,airInletNodeName);
          _supplyAirFan->setString(Fan_ConstantVolumeFields::AirOutletNodeName,fanOutletNodeName);
        }
        else if( _supplyAirFan->iddObject().type() == IddObjectType::Fan_OnOff )
        {
          _supplyAirFan->setString(Fan_OnOffFields::AirInletNodeName,airInletNodeName);
          _supplyAirFan->setString(Fan_OnOffFields::AirOutletNodeName,fanOutletNodeName);
        }
      }
    } else {
      if( _supplyAirFan->iddObject().type() == IddObjectType::Fan_ConstantVolume )
      {
        _supplyAirFan->setString(Fan_ConstantVolumeFields::AirInletNodeName,heatingCoilOutletNodeName);
        _supplyAirFan->setString(Fan_ConstantVolumeFields::AirOutletNodeName,fanOutletNodeName);
      }
      else if( _supplyAirFan->iddObject().type() == IddObjectType::Fan_OnOff )
      {
        _supplyAirFan->setString(Fan_OnOffFields::AirInletNodeName,heatingCoilOutletNodeName);
        _supplyAirFan->setString(Fan_OnOffFields::AirOutletNodeName,fanOutletNodeName);
      }
    }
  }

  // HeatingCoilObjectType
  
  HVACComponent heatingCoil = modelObject.heatingCoil();

  if( boost::optional<CoilHeatingDXSingleSpeed> coilHeatingDXSingleSpeed = heatingCoil.optionalCast<CoilHeatingDXSingleSpeed>() )
  {
    if( boost::optional<IdfObject> _heatingCoil = translateCoilHeatingDXSingleSpeedWithoutUnitary(coilHeatingDXSingleSpeed.get()) )
    {
      idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingCoilObjectType,_heatingCoil->iddObject().name() );

      idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingCoilName,_heatingCoil->name().get() ); 

      if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_DX_SingleSpeed )
      {
        _heatingCoil->setString(Coil_Heating_DX_SingleSpeedFields::AirInletNodeName,coolingCoilOutletNodeName);
        _heatingCoil->setString(Coil_Heating_DX_SingleSpeedFields::AirOutletNodeName,heatingCoilOutletNodeName);
      }
    }
  }

  // HeatingConvergenceTolerance

  if( (value = modelObject.heatingConvergenceTolerance()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingConvergenceTolerance,value.get());
  }

  // CoolingCoilObjectType

  HVACComponent coolingCoil = modelObject.coolingCoil();

  boost::optional<IdfObject> _coolingCoil;

  if( boost::optional<CoilCoolingDXSingleSpeed> dxCoil = coolingCoil.optionalCast<CoilCoolingDXSingleSpeed>() )
  {
    _coolingCoil = translateCoilCoolingDXSingleSpeedWithoutUnitary(dxCoil.get());

    m_map.insert(std::make_pair(coolingCoil.handle(),_coolingCoil.get()));
  }
  else
  {
    _coolingCoil = translateAndMapModelObject(coolingCoil);
  }

  if( _coolingCoil )
  {
    std::string coolingCoilInletNodeName;

    if( istringEqual(modelObject.fanPlacement(),"BlowThrough") ) {
      coolingCoilInletNodeName = fanOutletNodeName;
    } else {
      if( translateMixer() ) {
        coolingCoilInletNodeName = mixedAirNodeName;
      } else {
        coolingCoilInletNodeName = airInletNodeName;
      }
    }

    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingCoilObjectType,_coolingCoil->iddObject().name() );

    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingCoilName,_coolingCoil->name().get() );

    if( _coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_SingleSpeed )
    {
      _coolingCoil->setString(Coil_Cooling_DX_SingleSpeedFields::AirInletNodeName,coolingCoilInletNodeName);

      _coolingCoil->setString(Coil_Cooling_DX_SingleSpeedFields::AirOutletNodeName,coolingCoilOutletNodeName);
    }
  }

  // CoolingConvergenceTolerance
  
  if( (value = modelObject.coolingConvergenceTolerance()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingConvergenceTolerance,value.get());
  }

  // SupplementalHeatingCoilObjectType

  HVACComponent supplementalHeatingCoil = modelObject.supplementalHeatingCoil();

  if( boost::optional<IdfObject> _supplementalHeatingCoil = translateAndMapModelObject(supplementalHeatingCoil) )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::SupplementalHeatingCoilObjectType,_supplementalHeatingCoil->iddObject().name() );

    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::SupplementalHeatingCoilName,_supplementalHeatingCoil->name().get() );

    if( istringEqual(modelObject.fanPlacement(),"BlowThrough")  ) {
      if( _supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Fuel )
      {
        _supplementalHeatingCoil->setString(Coil_Heating_FuelFields::AirInletNodeName,heatingCoilOutletNodeName);

        _supplementalHeatingCoil->setString(Coil_Heating_FuelFields::AirOutletNodeName,airOutletNodeName);
      }
      else if( _supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric )
      {
        _supplementalHeatingCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName,heatingCoilOutletNodeName);

        _supplementalHeatingCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName,airOutletNodeName);
      }
      else if( _supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Water )
      {
        _supplementalHeatingCoil->setString(Coil_Heating_WaterFields::AirInletNodeName,heatingCoilOutletNodeName);

        _supplementalHeatingCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName,airOutletNodeName);
      }
    } else {
      if( _supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Fuel )
      {
        _supplementalHeatingCoil->setString(Coil_Heating_FuelFields::AirInletNodeName,fanOutletNodeName);

        _supplementalHeatingCoil->setString(Coil_Heating_FuelFields::AirOutletNodeName,airOutletNodeName);
      }
      else if( _supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric )
      {
        _supplementalHeatingCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName,fanOutletNodeName);

        _supplementalHeatingCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName,airOutletNodeName);
      }
      else if( _supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Water )
      {
        _supplementalHeatingCoil->setString(Coil_Heating_WaterFields::AirInletNodeName,fanOutletNodeName);

        _supplementalHeatingCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName,airOutletNodeName);
      }
    }
  }

  // MaximumSupplyAirTemperaturefromSupplementalHeater
  
  if( modelObject.isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::MaximumSupplyAirTemperaturefromSupplementalHeater,"Autosize"); 
  }
  else if( (value = modelObject.maximumSupplyAirTemperaturefromSupplementalHeater()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalHeatPumpFields::MaximumSupplyAirTemperaturefromSupplementalHeater,value.get());
  }

  // MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation

  if( (value = modelObject.maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalHeatPumpFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation,value.get());
  }

  // FanPlacement

  idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::FanPlacement,modelObject.fanPlacement());

  // SupplyAirFanOperatingModeScheduleName
  
  if( boost::optional<Schedule> schedule = modelObject.supplyAirFanOperatingModeSchedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFanOperatingModeScheduleName,_schedule->name().get());
    }
  }

  return idfObject;
}

} // energyplus

} // openstudio

