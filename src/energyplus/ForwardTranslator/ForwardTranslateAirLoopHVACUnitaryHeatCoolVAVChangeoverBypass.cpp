/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "../../model/AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl.hpp"
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Mixer.hpp"
#include <utilities/idd/AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_TwoSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_Water_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Desuperheater_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Fuel_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/Fan_VariableVolume_FieldEnums.hxx>
#include <utilities/idd/Fan_SystemModel_FieldEnums.hxx>
#include <utilities/idd/OutdoorAir_Mixer_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateAirLoopHVACUnitaryHeatCoolVAVChangeoverBypass(AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> d;

    // Name
    IdfObject unitarySystem = createRegisterAndNameIdfObject(openstudio::IddObjectType::AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass, modelObject);

    // Availability Schedule Name
    if (boost::optional<Schedule> availabilitySchedule = modelObject.availabilitySchedule()) {
      boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule.get());

      if (_availabilitySchedule && _availabilitySchedule->name()) {
        unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::AvailabilityScheduleName, _availabilitySchedule->name().get());
      }
    }

    //  SystemAirFlowRateDuringCoolingOperation
    if (modelObject.isSystemAirFlowRateDuringCoolingOperationAutosized()) {
      unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::CoolingSupplyAirFlowRate, "AutoSize");
    } else if ((d = modelObject.systemAirFlowRateDuringCoolingOperation())) {
      unitarySystem.setDouble(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::CoolingSupplyAirFlowRate, d.get());
    }

    //  SystemAirFlowRateDuringHeatingOperation
    if (modelObject.isSystemAirFlowRateDuringHeatingOperationAutosized()) {
      unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::HeatingSupplyAirFlowRate, "AutoSize");
    } else if ((d = modelObject.systemAirFlowRateDuringHeatingOperation())) {
      unitarySystem.setDouble(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::HeatingSupplyAirFlowRate, d.get());
    }

    //  SystemAirFlowRateWhenNoCoolingorHeatingisNeeded
    if (modelObject.isSystemAirFlowRateWhenNoCoolingorHeatingisNeededAutosized()) {
      unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::NoLoadSupplyAirFlowRate, "AutoSize");
    } else if ((d = modelObject.systemAirFlowRateWhenNoCoolingorHeatingisNeeded())) {
      unitarySystem.setDouble(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::NoLoadSupplyAirFlowRate, d.get());
    }

    //  OutdoorAirFlowRateDuringCoolingOperation
    if (modelObject.isOutdoorAirFlowRateDuringCoolingOperationAutosized()) {
      unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::CoolingOutdoorAirFlowRate, "AutoSize");
    } else if ((d = modelObject.outdoorAirFlowRateDuringCoolingOperation())) {
      unitarySystem.setDouble(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::CoolingOutdoorAirFlowRate, d.get());
    }

    //  OutdoorAirFlowRateDuringHeatingOperation
    if (modelObject.isOutdoorAirFlowRateDuringHeatingOperationAutosized()) {
      unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::HeatingOutdoorAirFlowRate, "AutoSize");
    } else if ((d = modelObject.outdoorAirFlowRateDuringHeatingOperation())) {
      unitarySystem.setDouble(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::HeatingOutdoorAirFlowRate, d.get());
    }

    //  OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded
    if (modelObject.isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized()) {
      unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::NoLoadOutdoorAirFlowRate, "AutoSize");
    } else if ((d = modelObject.outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded())) {
      unitarySystem.setDouble(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::NoLoadOutdoorAirFlowRate, d.get());
    }

    //  OutdoorAirFlowRateMultiplierScheduleName
    if (boost::optional<Schedule> schedule = modelObject.outdoorAirFlowRateMultiplierSchedule()) {
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get())) {
        unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateMultiplierScheduleName,
                                _schedule->name().get());
      }
    }

    boost::optional<std::string> airInletNodeName;
    boost::optional<std::string> airOutletNodeName;

    // Air Inlet Node Name
    if (boost::optional<ModelObject> mo = modelObject.inletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        airInletNodeName = node->name().get();
        unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::AirInletNodeName, node->name().get());
      }
    }

    // Air Outlet Node Name
    if (boost::optional<ModelObject> mo = modelObject.outletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        airOutletNodeName = node->name().get();
        unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::AirOutletNodeName, node->name().get());
      }
    }

    std::string baseName = modelObject.name().get();

    //  BypassDuctMixerNodeName
    std::string bypassDuctMixerNodeName = baseName + " Bypass Duct Mixer Node";
    unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::BypassDuctMixerNodeName, bypassDuctMixerNodeName);

    //  BypassDuctSplitterNodeName
    std::string bypassDuctSplitterNodeName = baseName + " Bypass Duct Splitter Node";
    unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::BypassDuctSplitterNodeName, bypassDuctSplitterNodeName);

    //  OutdoorAirMixerObjectType
    unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirMixerObjectType, "OutdoorAir:Mixer");

    //  OutdoorAirMixerName
    std::string outdoorAirMixerName = baseName + " Outdoor Air Mixer";
    unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirMixerName, outdoorAirMixerName);

    //  SupplyAirFanName
    boost::optional<IdfObject> _fan;
    if (boost::optional<HVACComponent> fan = modelObject.supplyAirFan()) {
      if ((_fan = translateAndMapModelObject(fan.get()))) {
        unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SupplyAirFanObjectType, _fan->iddObject().name());
        unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SupplyAirFanName, _fan->name().get());
      }
    }

    bool blowThroughFan = false;
    //  SupplyAirFanPlacement
    if ((s = modelObject.supplyAirFanPlacement())) {
      unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SupplyAirFanPlacement, s.get());
      if (istringEqual(s.get(), "BlowThrough")) {
        blowThroughFan = true;
      }
    }

    //  SupplyAirFanOperatingModeScheduleName
    if (boost::optional<Schedule> schedule = modelObject.supplyAirFanOperatingModeSchedule()) {
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get())) {
        unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SupplyAirFanOperatingModeScheduleName,
                                _schedule->name().get());
      }
    }

    // Heating Coil Object Type
    // Heating Coil Name
    boost::optional<IdfObject> _heatingCoil;
    if (boost::optional<HVACComponent> heatingCoil = modelObject.heatingCoil()) {
      _heatingCoil = translateAndMapModelObject(heatingCoil.get());

      if (_heatingCoil && _heatingCoil->name()) {
        unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::HeatingCoilObjectType, _heatingCoil->iddObject().name());
        unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::HeatingCoilName, _heatingCoil->name().get());
      }
    }

    // Cooling Coil Object Type
    // Cooling Coil Name
    boost::optional<IdfObject> _coolingCoil;
    if (boost::optional<HVACComponent> coolingCoil = modelObject.coolingCoil()) {
      if (boost::optional<CoilCoolingDXSingleSpeed> dxCoil = coolingCoil->optionalCast<CoilCoolingDXSingleSpeed>()) {
        _coolingCoil = translateCoilCoolingDXSingleSpeedWithoutUnitary(dxCoil.get());
        m_map.insert(std::make_pair(coolingCoil->handle(), _coolingCoil.get()));
      } else {
        _coolingCoil = translateAndMapModelObject(coolingCoil.get());
      }

      if (_coolingCoil && _coolingCoil->name()) {
        unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::CoolingCoilObjectType, _coolingCoil->iddObject().name());
        unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::CoolingCoilName, _coolingCoil->name().get());
      }
    }

    //  PriorityControlMode
    if ((s = modelObject.priorityControlMode())) {
      unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::PriorityControlMode, s.get());
    }

    //  MinimumOutletAirTemperatureDuringCoolingOperation
    if ((d = modelObject.minimumOutletAirTemperatureDuringCoolingOperation())) {
      unitarySystem.setDouble(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::MinimumOutletAirTemperatureDuringCoolingOperation, d.get());
    }

    //  MaximumOutletAirTemperatureDuringHeatingOperation)(Maximum Outlet Air Temperature During Heating Operation))
    if ((d = modelObject.maximumOutletAirTemperatureDuringHeatingOperation())) {
      unitarySystem.setDouble(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::MaximumOutletAirTemperatureDuringHeatingOperation, d.get());
    }

    // DehumidificationControlType
    if ((s = modelObject.dehumidificationControlType())) {
      unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::DehumidificationControlType, s.get());
    }

    // Minimum Runtime Before Operating Mode Change
    unitarySystem.setDouble(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::MinimumRuntimeBeforeOperatingModeChange,
                            modelObject.minimumRuntimeBeforeOperatingModeChange());

    if (_fan && _coolingCoil && _heatingCoil) {
      std::string fanOutletNodeName;
      std::string fanInletNodeName;
      std::string heatOutletNodeName;
      std::string heatInletNodeName;
      std::string coolOutletNodeName;
      std::string coolInletNodeName;
      std::string mixedAirNodeName;

      if (blowThroughFan) {
        mixedAirNodeName = baseName + " Mixed Air Node";
        fanInletNodeName = mixedAirNodeName;
        fanOutletNodeName = baseName + " Fan Outlet Node";
        coolInletNodeName = fanOutletNodeName;
        coolOutletNodeName = baseName + " Cooling Coil Outlet Node";
        heatInletNodeName = coolOutletNodeName;
        heatOutletNodeName = bypassDuctSplitterNodeName;
      } else {
        mixedAirNodeName = baseName + " Mixed Air Node";
        coolInletNodeName = mixedAirNodeName;
        coolOutletNodeName = baseName + " Cooling Coil Outlet Node";
        heatInletNodeName = coolOutletNodeName;
        heatOutletNodeName = baseName + " Heating Coil Outlet Node";
        fanInletNodeName = heatOutletNodeName;
        fanOutletNodeName = bypassDuctSplitterNodeName;
      }

      if (_fan->iddObject().type() == IddObjectType::Fan_ConstantVolume) {
        _fan->setString(Fan_ConstantVolumeFields::AirInletNodeName, fanInletNodeName);
        _fan->setString(Fan_ConstantVolumeFields::AirOutletNodeName, fanOutletNodeName);
      } else if (_fan->iddObject().type() == IddObjectType::Fan_VariableVolume) {
        _fan->setString(Fan_VariableVolumeFields::AirInletNodeName, fanInletNodeName);
        _fan->setString(Fan_VariableVolumeFields::AirOutletNodeName, fanOutletNodeName);
      } else if (_fan->iddObject().type() == IddObjectType::Fan_OnOff) {
        _fan->setString(Fan_OnOffFields::AirInletNodeName, fanInletNodeName);
        _fan->setString(Fan_OnOffFields::AirOutletNodeName, fanOutletNodeName);
      } else if (_fan->iddObject().type() == IddObjectType::Fan_SystemModel) {
        _fan->setString(Fan_SystemModelFields::AirInletNodeName, fanInletNodeName);
        _fan->setString(Fan_SystemModelFields::AirOutletNodeName, fanOutletNodeName);
      }

      fixSPMsForUnitarySystem(modelObject, fanInletNodeName, fanOutletNodeName);

      if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_SingleSpeed) {
        _coolingCoil->setString(Coil_Cooling_DX_SingleSpeedFields::AirInletNodeName, coolInletNodeName);
        _coolingCoil->setString(Coil_Cooling_DX_SingleSpeedFields::AirOutletNodeName, coolOutletNodeName);
      } else if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_TwoSpeed) {
        _coolingCoil->setString(Coil_Cooling_DX_TwoSpeedFields::AirInletNodeName, coolInletNodeName);
        _coolingCoil->setString(Coil_Cooling_DX_TwoSpeedFields::AirOutletNodeName, coolOutletNodeName);
      } else if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_Water) {
        _coolingCoil->setString(Coil_Cooling_WaterFields::AirInletNodeName, coolInletNodeName);
        _coolingCoil->setString(Coil_Cooling_WaterFields::AirOutletNodeName, coolOutletNodeName);
      } else if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_WaterToAirHeatPump_EquationFit) {
        _coolingCoil->setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirInletNodeName, coolInletNodeName);
        _coolingCoil->setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName, coolOutletNodeName);
      }

      if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_DX_SingleSpeed) {
        _heatingCoil->setString(Coil_Heating_DX_SingleSpeedFields::AirInletNodeName, heatInletNodeName);
        _heatingCoil->setString(Coil_Heating_DX_SingleSpeedFields::AirOutletNodeName, heatOutletNodeName);
      } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric) {
        _heatingCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName, heatInletNodeName);
        _heatingCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName, heatOutletNodeName);
      } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Fuel) {
        _heatingCoil->setString(Coil_Heating_FuelFields::AirInletNodeName, heatInletNodeName);
        _heatingCoil->setString(Coil_Heating_FuelFields::AirOutletNodeName, heatOutletNodeName);
      } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Water) {
        _heatingCoil->setString(Coil_Heating_WaterFields::AirInletNodeName, heatInletNodeName);
        _heatingCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName, heatOutletNodeName);
      } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Desuperheater) {
        _heatingCoil->setString(Coil_Heating_DesuperheaterFields::AirInletNodeName, heatInletNodeName);
        _heatingCoil->setString(Coil_Heating_DesuperheaterFields::AirOutletNodeName, heatOutletNodeName);
      } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_WaterToAirHeatPump_EquationFit) {
        _heatingCoil->setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirInletNodeName, heatInletNodeName);
        _heatingCoil->setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName, heatOutletNodeName);
      }

      IdfObject _outdoorAirMixer(IddObjectType::OutdoorAir_Mixer);
      _outdoorAirMixer.setName(outdoorAirMixerName);
      m_idfObjects.push_back(_outdoorAirMixer);

      std::string oaNodeName = baseName + " OA Node";
      _outdoorAirMixer.setString(OutdoorAir_MixerFields::MixedAirNodeName, mixedAirNodeName);
      _outdoorAirMixer.setString(OutdoorAir_MixerFields::OutdoorAirStreamNodeName, oaNodeName);

      IdfObject _oaNodeList(openstudio::IddObjectType::OutdoorAir_NodeList);
      _oaNodeList.setString(0, oaNodeName);
      m_idfObjects.push_back(_oaNodeList);
      _outdoorAirMixer.setString(OutdoorAir_MixerFields::ReliefAirStreamNodeName, baseName + " Relief Air Node");
      _outdoorAirMixer.setString(OutdoorAir_MixerFields::ReturnAirStreamNodeName, bypassDuctMixerNodeName);

      // Plenum or Mixer
      if (boost::optional<Mixer> returnPathComponent = modelObject.plenumorMixer()) {
        // In this case, do translate the node
        unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::PlenumorMixerInletNodeName,
                                modelObject.plenumorMixerNode().name().get());
      }
    }

    return unitarySystem;
  }

}  // namespace energyplus

}  // namespace openstudio
