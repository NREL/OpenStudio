/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "../../model/AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl.hpp"
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../../model/CoilCoolingDXVariableSpeed.hpp"
#include "../../model/CoilCoolingDXVariableSpeed_Impl.hpp"
#include "../../model/CoilSystemCoolingDXHeatExchangerAssisted.hpp"
#include "../../model/CoilSystemCoolingDXHeatExchangerAssisted_Impl.hpp"
#include "../../model/CoilCoolingDXTwoStageWithHumidityControlMode.hpp"
#include "../../model/CoilCoolingDXTwoStageWithHumidityControlMode_Impl.hpp"
#include "../../model/CoilHeatingDXSingleSpeed.hpp"
#include "../../model/CoilHeatingDXSingleSpeed_Impl.hpp"
#include "../../model/CoilHeatingDXVariableSpeed.hpp"
#include "../../model/CoilHeatingDXVariableSpeed_Impl.hpp"
#include "../../model/CoilHeatingGas.hpp"
#include "../../model/CoilHeatingGas_Impl.hpp"
#include "../../model/CoilHeatingElectric.hpp"
#include "../../model/CoilHeatingElectric_Impl.hpp"
#include "../../model/CoilHeatingWater.hpp"
#include "../../model/CoilHeatingWater_Impl.hpp"
#include "../../model/AirToAirComponent.hpp"
#include "../../model/AirToAirComponent_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Mixer.hpp"
#include <utilities/idd/AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_VariableSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_TwoStageWithHumidityControlMode_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_VariableSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Fuel_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include <utilities/idd/HeatExchanger_AirToAir_SensibleAndLatent_FieldEnums.hxx>
#include <utilities/idd/HeatExchanger_Desiccant_BalancedFlow_FieldEnums.hxx>
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
    boost::optional<HVACComponent> heatingCoil = modelObject.heatingCoil();
    if (heatingCoil) {
      if (boost::optional<CoilHeatingDXSingleSpeed> dxCoil = heatingCoil->optionalCast<CoilHeatingDXSingleSpeed>()) {
        // Not calling `translateAndMapModelObject` but directly the without unitary function, so need to insert into m_map
        _heatingCoil = translateCoilHeatingDXSingleSpeedWithoutUnitary(dxCoil.get());
        m_map.insert(std::make_pair(heatingCoil->handle(), _heatingCoil.get()));
      } else if (boost::optional<CoilHeatingDXVariableSpeed> dxCoil = heatingCoil->optionalCast<CoilHeatingDXVariableSpeed>()) {
        _heatingCoil = translateCoilHeatingDXVariableSpeedWithoutUnitary(dxCoil.get());
        m_map.insert(std::make_pair(heatingCoil->handle(), _heatingCoil.get()));
      } else if ((heatingCoil->optionalCast<CoilHeatingGas>()) || (heatingCoil->optionalCast<CoilHeatingElectric>())
                 || (heatingCoil->optionalCast<CoilHeatingWater>())) {
        // translateAndMapModelObject already inserts into m_map
        _heatingCoil = translateAndMapModelObject(heatingCoil.get());
      } else {
        LOG(Fatal, modelObject.briefDescription() << " appears to have a heating coil that shouldn't have been accepted: "
                                                  << heatingCoil->briefDescription());
        OS_ASSERT(false);
      }

      if (_heatingCoil && _heatingCoil->name()) {
        unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::HeatingCoilObjectType, _heatingCoil->iddObject().name());
        unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::HeatingCoilName, _heatingCoil->name().get());
      }
    }

    // Cooling Coil Object Type
    // Cooling Coil Name
    boost::optional<IdfObject> _coolingCoil;
    boost::optional<HVACComponent> coolingCoil = modelObject.coolingCoil();
    if (coolingCoil) {
      if (boost::optional<CoilCoolingDXSingleSpeed> dxCoil = coolingCoil->optionalCast<CoilCoolingDXSingleSpeed>()) {
        _coolingCoil = translateCoilCoolingDXSingleSpeedWithoutUnitary(dxCoil.get());
        m_map.insert(std::make_pair(coolingCoil->handle(), _coolingCoil.get()));
      } else if (boost::optional<CoilCoolingDXVariableSpeed> dxCoil = coolingCoil->optionalCast<CoilCoolingDXVariableSpeed>()) {
        _coolingCoil = translateCoilCoolingDXVariableSpeedWithoutUnitary(dxCoil.get());
        m_map.insert(std::make_pair(coolingCoil->handle(), _coolingCoil.get()));
      } else if (boost::optional<CoilSystemCoolingDXHeatExchangerAssisted> dxCoil =
                   coolingCoil->optionalCast<CoilSystemCoolingDXHeatExchangerAssisted>()) {
        _coolingCoil = translateAndMapModelObject(dxCoil.get());
      } else if (boost::optional<CoilCoolingDXTwoStageWithHumidityControlMode> dxCoil =
                   coolingCoil->optionalCast<CoilCoolingDXTwoStageWithHumidityControlMode>()) {
        _coolingCoil = translateCoilCoolingDXTwoStageWithHumidityControlModeWithoutUnitary(dxCoil.get());
        m_map.insert(std::make_pair(coolingCoil->handle(), _coolingCoil.get()));
      } else {
        LOG(Fatal, modelObject.briefDescription() << " appears to have a cooling coil that shouldn't have been accepted: "
                                                  << coolingCoil->briefDescription());
        OS_ASSERT(false);
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
      } else if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_VariableSpeed) {
        _coolingCoil->setString(Coil_Cooling_DX_VariableSpeedFields::IndoorAirInletNodeName, coolInletNodeName);
        _coolingCoil->setString(Coil_Cooling_DX_VariableSpeedFields::IndoorAirOutletNodeName, coolOutletNodeName);
      } else if (_coolingCoil->iddObject().type() == IddObjectType::CoilSystem_Cooling_DX_HeatExchangerAssisted) {
        OS_ASSERT(coolingCoil);
        auto coilSystem = coolingCoil->optionalCast<model::CoilSystemCoolingDXHeatExchangerAssisted>();
        OS_ASSERT(coilSystem);
        auto hx = coilSystem->heatExchanger();
        auto _hx = translateAndMapModelObject(hx);
        OS_ASSERT(_hx);
        if (_hx->iddObject().type() == IddObjectType::HeatExchanger_AirToAir_SensibleAndLatent) {
          _hx->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirInletNodeName, coolInletNodeName);
          _hx->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirOutletNodeName, coolOutletNodeName);
        } else if (_hx->iddObject().type() == IddObjectType::HeatExchanger_Desiccant_BalancedFlow) {
          _hx->setString(HeatExchanger_Desiccant_BalancedFlowFields::RegenerationAirInletNodeName, coolInletNodeName);
          _hx->setString(HeatExchanger_Desiccant_BalancedFlowFields::ProcessAirOutletNodeName, coolOutletNodeName);
        } else {
          LOG(Warn, modelObject.briefDescription() << ": Contains an unsupported type " << _hx->iddObject().type() << ".");
        }
      } else if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_TwoStageWithHumidityControlMode) {
        _coolingCoil->setString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::AirInletNodeName, coolInletNodeName);
        _coolingCoil->setString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::AirOutletNodeName, coolOutletNodeName);
      }

      if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_DX_SingleSpeed) {
        _heatingCoil->setString(Coil_Heating_DX_SingleSpeedFields::AirInletNodeName, heatInletNodeName);
        _heatingCoil->setString(Coil_Heating_DX_SingleSpeedFields::AirOutletNodeName, heatOutletNodeName);
      } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_DX_VariableSpeed) {
        _heatingCoil->setString(Coil_Heating_DX_VariableSpeedFields::IndoorAirInletNodeName, heatInletNodeName);
        _heatingCoil->setString(Coil_Heating_DX_VariableSpeedFields::IndoorAirOutletNodeName, heatOutletNodeName);
      } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Fuel) {
        _heatingCoil->setString(Coil_Heating_FuelFields::AirInletNodeName, heatInletNodeName);
        _heatingCoil->setString(Coil_Heating_FuelFields::AirOutletNodeName, heatOutletNodeName);
      } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric) {
        _heatingCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName, heatInletNodeName);
        _heatingCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName, heatOutletNodeName);
      } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Water) {
        _heatingCoil->setString(Coil_Heating_WaterFields::AirInletNodeName, heatInletNodeName);
        _heatingCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName, heatOutletNodeName);
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
