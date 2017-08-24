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
#include "../../model/ZoneHVACFourPipeFanCoil.hpp"
#include "../../model/ZoneHVACFourPipeFanCoil_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/CoilCoolingWater.hpp"
#include "../../model/CoilCoolingWater_Impl.hpp"
#include "../../model/CoilHeatingWater.hpp"
#include "../../model/CoilHeatingWater_Impl.hpp"
#include <utilities/idd/ZoneHVAC_FourPipeFanCoil_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/Fan_VariableVolume_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_Water_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/OutdoorAir_Mixer_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneHVACFourPipeFanCoil(
    ZoneHVACFourPipeFanCoil & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::ZoneHVAC_FourPipeFanCoil);

  // Get model object name and define node names for future use
  // Model Name
  std::string baseName = modelObject.name().get();
  // Node Names
  std::string mixedAirNodeName = baseName + " Mixed Air Node";
  std::string fanOutletNodeName = baseName + " Fan Outlet Node";
  std::string coolingCoilOutletNodeName = baseName + " Cooling Coil Outlet Node";
  std::string reliefAirNodeName = baseName + " Relief Air Node";
  std::string oaNodeName = baseName + " OA Node";

  boost::optional<AirLoopHVAC> t_airLoopHVAC = modelObject.airLoopHVAC();

  // AirInletNodeName
  boost::optional<std::string> airInletNodeName;
  if( boost::optional<Node> node = modelObject.inletNode() )
  {
    if( (s = node->name()) )
    {
      airInletNodeName = s;
      idfObject.setString(ZoneHVAC_FourPipeFanCoilFields::AirInletNodeName,s.get() );
    }
  }
  // AirOutletNodeName
  boost::optional<std::string> airOutletNodeName;
  if( boost::optional<Node> node = modelObject.outletNode() )
  {
    if( (s = node->name()) )
    {
      airOutletNodeName = s;
      idfObject.setString(ZoneHVAC_FourPipeFanCoilFields::AirOutletNodeName,s.get() );
    }
  }

  // hook up required objects
  try {
    // AvailabilityScheduleName
    Schedule availabilitySchedule = modelObject.availabilitySchedule();
    translateAndMapModelObject(availabilitySchedule);
    idfObject.setString(ZoneHVAC_FourPipeFanCoilFields::AvailabilityScheduleName,
                        availabilitySchedule.name().get() );

    // Supply Air Fan
    HVACComponent supplyAirFan = modelObject.supplyAirFan();
    if( boost::optional<IdfObject> _supplyAirFan = translateAndMapModelObject(supplyAirFan) )
    {
      // SupplyAirFanObjectType
      idfObject.setString(ZoneHVAC_FourPipeFanCoilFields::SupplyAirFanObjectType,_supplyAirFan->iddObject().name() );

      // SupplyAirFanName
      idfObject.setString(ZoneHVAC_FourPipeFanCoilFields::SupplyAirFanName,_supplyAirFan->name().get() );
      // Supply Air Fan Inlet and Outlet Nodes
      if( airOutletNodeName && airInletNodeName )
      {
        // If there is an AirLoopHVAC then we provide no mixer
        std::string fanInletNodeName;
        if( t_airLoopHVAC ) {
          fanInletNodeName = airInletNodeName.get();
        } else {
          fanInletNodeName = mixedAirNodeName;
        }

        if( _supplyAirFan->iddObject().type() == IddObjectType::Fan_ConstantVolume )
        {
          _supplyAirFan->setString(Fan_ConstantVolumeFields::AirInletNodeName,fanInletNodeName );
          _supplyAirFan->setString(Fan_ConstantVolumeFields::AirOutletNodeName,fanOutletNodeName );
        }
        else if( _supplyAirFan->iddObject().type() == IddObjectType::Fan_OnOff )
        {
          
          _supplyAirFan->setString(Fan_OnOffFields::AirInletNodeName,fanInletNodeName );
          _supplyAirFan->setString(Fan_OnOffFields::AirOutletNodeName,fanOutletNodeName );
        }
        else if( _supplyAirFan->iddObject().type() == IddObjectType::Fan_VariableVolume )
        {
          _supplyAirFan->setString(Fan_VariableVolumeFields::AirInletNodeName,fanInletNodeName );
          _supplyAirFan->setString(Fan_VariableVolumeFields::AirOutletNodeName,fanOutletNodeName );
        }
      }
    }

    // Cooling Coil
    HVACComponent coolingCoil = modelObject.coolingCoil();
    if( boost::optional<IdfObject> _coolingCoil = translateAndMapModelObject(coolingCoil) )
    {
      // CoolingCoilObjectType
      idfObject.setString(ZoneHVAC_FourPipeFanCoilFields::CoolingCoilObjectType,_coolingCoil->iddObject().name() );
      // CoolingCoilName
      idfObject.setString(ZoneHVAC_FourPipeFanCoilFields::CoolingCoilName,_coolingCoil->name().get() );
      // Cooling Coil Inlet and Outlet Nodes
      if( _coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_Water )
      {
        _coolingCoil->setString(Coil_Cooling_WaterFields::AirInletNodeName,fanOutletNodeName );
        _coolingCoil->setString(Coil_Cooling_WaterFields::AirOutletNodeName,coolingCoilOutletNodeName );
      }
    }

    // Heating Coil
    HVACComponent heatingCoil = modelObject.heatingCoil();
    if( boost::optional<IdfObject> _heatingCoil = translateAndMapModelObject(heatingCoil) )
    {
      // HeatingCoilObjectType
      idfObject.setString(ZoneHVAC_FourPipeFanCoilFields::HeatingCoilObjectType,_heatingCoil->iddObject().name() );
      // HeatingCoilName
      idfObject.setString(ZoneHVAC_FourPipeFanCoilFields::HeatingCoilName,_heatingCoil->name().get() );
      // Heating Coil Inlet and Outlet Nodes
      if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Water ) {
        _heatingCoil->setString(Coil_Heating_WaterFields::AirInletNodeName,coolingCoilOutletNodeName );
        _heatingCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName,airOutletNodeName.get() );
      } else if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric ) {
        _heatingCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName,coolingCoilOutletNodeName );
        _heatingCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName,airOutletNodeName.get() );
      }
    }
  }
  catch (std::exception& e) {
    LOG(Error,"Could not translate " << modelObject.briefDescription() << ", because "
        << e.what() << ".");
    return boost::none;
  }

  m_idfObjects.push_back(idfObject);

  // Name
  idfObject.setName(baseName);

  // CapacityControlMethod
  idfObject.setString(ZoneHVAC_FourPipeFanCoilFields::CapacityControlMethod,
                      modelObject.capacityControlMethod());

  // MaximumSupplyAirFlowRate
  if( modelObject.isMaximumSupplyAirFlowRateAutosized() )
  {
    idfObject.setString(ZoneHVAC_FourPipeFanCoilFields::MaximumSupplyAirFlowRate,"Autosize");
  }
  else if( (value = modelObject.maximumSupplyAirFlowRate()) )
  {
    idfObject.setDouble(ZoneHVAC_FourPipeFanCoilFields::MaximumSupplyAirFlowRate,value.get());
  }

  // LowSpeedSupplyAirFlowRatio
  if(! modelObject.isLowSpeedSupplyAirFlowRatioDefaulted() )
  {
    idfObject.setDouble(ZoneHVAC_FourPipeFanCoilFields::LowSpeedSupplyAirFlowRatio,modelObject.lowSpeedSupplyAirFlowRatio() );
  }

  // MediumSpeedSupplyAirFlowRatio
  if(! (modelObject.isMediumSpeedSupplyAirFlowRatioDefaulted()) )
  {
    idfObject.setDouble(ZoneHVAC_FourPipeFanCoilFields::MediumSpeedSupplyAirFlowRatio,modelObject.mediumSpeedSupplyAirFlowRatio() );
  }

  // MaximumOutdoorAirFlowRate
  if( modelObject.isMaximumOutdoorAirFlowRateAutosized() )
  {
    idfObject.setString(ZoneHVAC_FourPipeFanCoilFields::MaximumOutdoorAirFlowRate,"Autosize");
  }
  else if( (value = modelObject.maximumOutdoorAirFlowRate()) )
  {
    idfObject.setDouble(ZoneHVAC_FourPipeFanCoilFields::MaximumOutdoorAirFlowRate,value.get());
  }

  // OutdoorAirScheduleName
  if( boost::optional<Schedule> schedule = modelObject.outdoorAirSchedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(ZoneHVAC_FourPipeFanCoilFields::OutdoorAirScheduleName,_schedule->name().get());
    }
  }

  if( ! t_airLoopHVAC ) {
    // OutdoorAirMixerObjectType
    idfObject.setString(ZoneHVAC_FourPipeFanCoilFields::OutdoorAirMixerObjectType,
                        modelObject.outdoorAirMixerObjectType());

    // OutdoorAirMixerName
    std::string oaMixerName = modelObject.name().get() + " OA Mixer";
    idfObject.setString(ZoneHVAC_FourPipeFanCoilFields::OutdoorAirMixerName,oaMixerName);

    // Create Outdoor Air Mixer
    IdfObject _outdoorAirMixer(IddObjectType::OutdoorAir_Mixer);
    _outdoorAirMixer.setName(oaMixerName);
    m_idfObjects.push_back(_outdoorAirMixer);

    _outdoorAirMixer.setString(OutdoorAir_MixerFields::MixedAirNodeName,mixedAirNodeName);
    _outdoorAirMixer.setString(OutdoorAir_MixerFields::OutdoorAirStreamNodeName,oaNodeName);
    _outdoorAirMixer.setString(OutdoorAir_MixerFields::ReliefAirStreamNodeName,reliefAirNodeName);
    if(airInletNodeName)
    {
      _outdoorAirMixer.setString(OutdoorAir_MixerFields::ReturnAirStreamNodeName,airInletNodeName.get());
    }

    // Create Outdoor Air Node List
    IdfObject _oaNodeList(openstudio::IddObjectType::OutdoorAir_NodeList);
    _oaNodeList.setString(0,oaNodeName);
    m_idfObjects.push_back(_oaNodeList);
  }

  // MaximumColdWaterFlowRate
  if( modelObject.isMaximumColdWaterFlowRateAutosized() )
  {
    idfObject.setString(ZoneHVAC_FourPipeFanCoilFields::MaximumColdWaterFlowRate,"Autosize");
  }
  else if( (value = modelObject.maximumColdWaterFlowRate()) )
  {
    idfObject.setDouble(ZoneHVAC_FourPipeFanCoilFields::MaximumColdWaterFlowRate,value.get());
  }

  // MinimumColdWaterFlowRate
  if(! modelObject.isMinimumColdWaterFlowRateDefaulted() )
  {
    idfObject.setDouble(ZoneHVAC_FourPipeFanCoilFields::MinimumColdWaterFlowRate,modelObject.minimumColdWaterFlowRate() );
  }

  // CoolingConvergenceTolerance
  if(! modelObject.isCoolingConvergenceToleranceDefaulted() )
  {
    idfObject.setDouble(ZoneHVAC_FourPipeFanCoilFields::CoolingConvergenceTolerance,modelObject.coolingConvergenceTolerance() );
  }

  // MaximumHotWaterFlowRate
  if( modelObject.isMaximumHotWaterFlowRateAutosized() )
  {
    idfObject.setString(ZoneHVAC_FourPipeFanCoilFields::MaximumHotWaterFlowRate,"Autosize");
  }
  else if( (value = modelObject.maximumHotWaterFlowRate()) )
  {
    idfObject.setDouble(ZoneHVAC_FourPipeFanCoilFields::MaximumHotWaterFlowRate,value.get());
  }

  // MinimumHotWaterFlowRate
  if(! modelObject.isMinimumHotWaterFlowRateDefaulted() )
  {
    idfObject.setDouble(ZoneHVAC_FourPipeFanCoilFields::MinimumHotWaterFlowRate,modelObject.minimumHotWaterFlowRate() );
  }

  // HeatingConvergenceTolerance
  if(! modelObject.isHeatingConvergenceToleranceDefaulted() )
  {
    idfObject.setDouble(ZoneHVAC_FourPipeFanCoilFields::HeatingConvergenceTolerance,modelObject.heatingConvergenceTolerance() );
  }

  // SupplyAirFanOperatingModeScheduleName
  if( auto schedule = modelObject.supplyAirFanOperatingModeSchedule() )
  {
    if( auto _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(ZoneHVAC_FourPipeFanCoilFields::SupplyAirFanOperatingModeScheduleName,_schedule->name().get());
    }
  }

  // MinimumSupplyAirTemperatureInCoolingMode
  if( modelObject.isMinimumSupplyAirTemperatureInCoolingModeAutosized() )
  {
    idfObject.setString(ZoneHVAC_FourPipeFanCoilFields::MinimumSupplyAirTemperatureinCoolingMode,"Autosize");
  }
  else if( (value = modelObject.minimumSupplyAirTemperatureInCoolingMode()) )
  {
    idfObject.setDouble(ZoneHVAC_FourPipeFanCoilFields::MinimumSupplyAirTemperatureinCoolingMode,value.get());
  }

  // MaximumSupplyAirTemperatureInHeatingMode
  if( modelObject.isMaximumSupplyAirTemperatureInHeatingModeAutosized() )
  {
    idfObject.setString(ZoneHVAC_FourPipeFanCoilFields::MaximumSupplyAirTemperatureinHeatingMode,"Autosize");
  }
  else if( (value = modelObject.maximumSupplyAirTemperatureInHeatingMode()) )
  {
    idfObject.setDouble(ZoneHVAC_FourPipeFanCoilFields::MaximumSupplyAirTemperatureinHeatingMode,value.get());
  }

  return idfObject;
}

} // energyplus

} // openstudio

