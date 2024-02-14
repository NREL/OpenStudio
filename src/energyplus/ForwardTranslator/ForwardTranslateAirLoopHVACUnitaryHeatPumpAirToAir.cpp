/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
#include "../../model/CoilCoolingDXVariableSpeed.hpp"
#include "../../model/CoilCoolingDXVariableSpeed_Impl.hpp"
#include "../../model/CoilHeatingDXVariableSpeed.hpp"
#include "../../model/CoilHeatingDXVariableSpeed_Impl.hpp"
#include "../../model/CoilSystemIntegratedHeatPumpAirSource.hpp"
#include "../../model/CoilSystemIntegratedHeatPumpAirSource_Impl.hpp"
#include <utilities/idd/AirLoopHVAC_UnitaryHeatPump_AirToAir_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/Fan_SystemModel_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_VariableSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Fuel_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_ThermalStorage_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_VariableSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateAirLoopHVACUnitaryHeatPumpAirToAir(AirLoopHVACUnitaryHeatPumpAirToAir& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::AirLoopHVAC_UnitaryHeatPump_AirToAir);

    m_idfObjects.push_back(idfObject);

    // Name

    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // AvailabilityScheduleName

    if (boost::optional<Schedule> schedule = modelObject.availabilitySchedule()) {
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::AvailabilityScheduleName, _schedule->name().get());
      }
    }

    // AirInletNodeName

    boost::optional<std::string> airInletNodeName;

    if (boost::optional<ModelObject> mo = modelObject.inletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        if ((s = node->name())) {
          airInletNodeName = s;

          idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::AirInletNodeName, s.get());
        }
      }
    }

    // AirOutletNodeName

    boost::optional<std::string> airOutletNodeName;

    if (boost::optional<ModelObject> mo = modelObject.outletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        if ((s = node->name())) {
          airOutletNodeName = s;

          idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::AirOutletNodeName, s.get());
        }
      }
    }

    // SupplyAirFlowRateDuringCoolingOperation

    if (modelObject.isSupplyAirFlowRateDuringCoolingOperationAutosized()) {
      idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::CoolingSupplyAirFlowRate, "Autosize");
    } else if ((value = modelObject.supplyAirFlowRateDuringCoolingOperation())) {
      idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::CoolingSupplyAirFlowRate, value.get());
    }

    // SupplyAirFlowRateDuringHeatingOperation

    if (modelObject.isSupplyAirFlowRateDuringHeatingOperationAutosized()) {
      idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::HeatingSupplyAirFlowRate, "Autosize");
    } else if ((value = modelObject.supplyAirFlowRateDuringHeatingOperation())) {
      idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::HeatingSupplyAirFlowRate, value.get());
    }

    // SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded

    if (modelObject.isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized()) {
      idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::NoLoadSupplyAirFlowRate, "Autosize");
    } else if ((value = modelObject.supplyAirFlowRateWhenNoCoolingorHeatingisNeeded())) {
      idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::NoLoadSupplyAirFlowRate, value.get());
    }

    // ControllingZoneorThermostatLocation

    if (boost::optional<ThermalZone> tz = modelObject.controllingZone()) {
      idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::ControllingZoneorThermostatLocation, tz->name().get());
    }

    // SupplyAirFanName

    HVACComponent fan = modelObject.supplyAirFan();

    boost::optional<IdfObject> _fan = translateAndMapModelObject(fan);

    if (_fan) {
      idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFanObjectType, _fan->iddObject().name());
      idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFanName, _fan->name().get());
    }

    // HeatingCoilName

    boost::optional<IdfObject> _heatingCoil;

    HVACComponent heatingCoil = modelObject.heatingCoil();

    if (boost::optional<CoilHeatingDXSingleSpeed> coilHeatingDXSingleSpeed = heatingCoil.optionalCast<CoilHeatingDXSingleSpeed>()) {
      _heatingCoil = translateCoilHeatingDXSingleSpeedWithoutUnitary(coilHeatingDXSingleSpeed.get());
    } else if (boost::optional<CoilHeatingDXVariableSpeed> coilHeatingDXVariableSpeed = heatingCoil.optionalCast<CoilHeatingDXVariableSpeed>()) {
      _heatingCoil = translateCoilHeatingDXVariableSpeedWithoutUnitary(coilHeatingDXVariableSpeed.get());
    } else if (boost::optional<CoilSystemIntegratedHeatPumpAirSource> coilSystemIntegratedHeatPumpAirSource =
                 heatingCoil.optionalCast<CoilSystemIntegratedHeatPumpAirSource>()) {
      _heatingCoil = translateAndMapModelObject(coilSystemIntegratedHeatPumpAirSource.get());
    }

    if (_heatingCoil) {
      idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::HeatingCoilObjectType, _heatingCoil->iddObject().name());
      idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::HeatingCoilName, _heatingCoil->name().get());
    }

    // CoolingCoilName

    boost::optional<IdfObject> _coolingCoil;

    HVACComponent coolingCoil = modelObject.coolingCoil();

    if (boost::optional<CoilCoolingDXSingleSpeed> coilCoolingDXSingleSpeed = coolingCoil.optionalCast<CoilCoolingDXSingleSpeed>()) {
      _coolingCoil = translateCoilCoolingDXSingleSpeedWithoutUnitary(coilCoolingDXSingleSpeed.get());
    } else if (boost::optional<CoilCoolingDXSingleSpeedThermalStorage> coilCoolingDXSingleSpeedThermalStorage =
                 coolingCoil.optionalCast<CoilCoolingDXSingleSpeedThermalStorage>()) {
      _coolingCoil = translateCoilCoolingDXSingleSpeedThermalStorageWithoutUnitary(coilCoolingDXSingleSpeedThermalStorage.get());
    } else if (boost::optional<CoilCoolingDXVariableSpeed> coilCoolingDXVariableSpeed = coolingCoil.optionalCast<CoilCoolingDXVariableSpeed>()) {
      _coolingCoil = translateCoilCoolingDXVariableSpeedWithoutUnitary(coilCoolingDXVariableSpeed.get());
    } else if (boost::optional<CoilSystemIntegratedHeatPumpAirSource> coilSystemIntegratedHeatPumpAirSource =
                 coolingCoil.optionalCast<CoilSystemIntegratedHeatPumpAirSource>()) {
      _coolingCoil = translateAndMapModelObject(coilSystemIntegratedHeatPumpAirSource.get());
    }

    if (_coolingCoil) {
      // Need to explicitly add it to m_map for CoilCoolingDXSingleSpeed since translateCoilCoolingDXSingleSpeedWithoutUnitary doesn't
      m_map.insert(std::make_pair(coolingCoil.handle(), _coolingCoil.get()));

      idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::CoolingCoilObjectType, _coolingCoil->iddObject().name());
      idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::CoolingCoilName, _coolingCoil->name().get());
    }

    // SupplementalHeatingCoilName

    boost::optional<IdfObject> _supplementalHeatingCoil;

    boost::optional<HVACComponent> supplementalHeatingCoil = modelObject.supplementalHeatingCoil();

    if (supplementalHeatingCoil) {
      _supplementalHeatingCoil = translateAndMapModelObject(supplementalHeatingCoil.get());
    }

    if (_supplementalHeatingCoil) {
      idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplementalHeatingCoilObjectType,
                          _supplementalHeatingCoil->iddObject().name());
      idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplementalHeatingCoilName, _supplementalHeatingCoil->name().get());
    }

    // MaximumSupplyAirTemperaturefromSupplementalHeater

    if (modelObject.isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized()) {
      idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumSupplyAirTemperaturefromSupplementalHeater, "Autosize");
    } else if ((value = modelObject.maximumSupplyAirTemperaturefromSupplementalHeater())) {
      idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumSupplyAirTemperaturefromSupplementalHeater, value.get());
    }

    // MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation

    if ((value = modelObject.maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation())) {
      idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation, value.get());
    }

    // FanPlacement

    if ((s = modelObject.fanPlacement())) {
      idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::FanPlacement, s.get());
    }

    // SupplyAirFanOperatingModeScheduleName

    if (boost::optional<Schedule> schedule = modelObject.supplyAirFanOperatingModeSchedule()) {
      boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get());

      if (_schedule) {
        idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFanOperatingModeScheduleName, _schedule->name().get());
      }
    }

    // Dehumidification Control Type
    if (!modelObject.isDehumidificationControlTypeDefaulted()) {
      idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAirFields::DehumidificationControlType, modelObject.dehumidificationControlType());
    }

    // Fill in node names for inner components

    if (airInletNodeName && _fan) {
      if (_fan->iddObject().type() == IddObjectType::Fan_ConstantVolume) {
        _fan->setString(Fan_ConstantVolumeFields::AirInletNodeName, airInletNodeName.get());
      } else if (_fan->iddObject().type() == IddObjectType::Fan_OnOff) {
        _fan->setString(Fan_OnOffFields::AirInletNodeName, airInletNodeName.get());
      } else if (_fan->iddObject().type() == IddObjectType::Fan_SystemModel) {
        // This isn't supported by E+ right now
        _fan->setString(Fan_SystemModelFields::AirInletNodeName, airInletNodeName.get());
        OS_ASSERT(false);
      }
    }

    std::string fanOutletNodeName;

    boost::optional<CoilSystemIntegratedHeatPumpAirSource> coilSystemIntegratedHeatPumpAirSource;

    if (_fan && _coolingCoil) {
      std::string nodeName = modelObject.name().get() + " Fan - Cooling Coil Node";
      fanOutletNodeName = nodeName;

      if (_fan->iddObject().type() == IddObjectType::Fan_ConstantVolume) {
        _fan->setString(Fan_ConstantVolumeFields::AirOutletNodeName, nodeName);
      } else if (_fan->iddObject().type() == IddObjectType::Fan_OnOff) {
        _fan->setString(Fan_OnOffFields::AirOutletNodeName, nodeName);
      } else if (_fan->iddObject().type() == IddObjectType::Fan_SystemModel) {
        // This isn't supported by E+ right now
        _fan->setString(Fan_SystemModelFields::AirOutletNodeName, nodeName);
        OS_ASSERT(false);
      }

      if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_SingleSpeed) {
        _coolingCoil->setString(Coil_Cooling_DX_SingleSpeedFields::AirInletNodeName, nodeName);
      } else if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_SingleSpeed_ThermalStorage) {
        _coolingCoil->setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirInletNodeName, nodeName);
      } else if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_VariableSpeed) {
        _coolingCoil->setString(Coil_Cooling_DX_VariableSpeedFields::IndoorAirInletNodeName, nodeName);
      } else if (_coolingCoil->iddObject().type() == IddObjectType::CoilSystem_IntegratedHeatPump_AirSource) {
        coilSystemIntegratedHeatPumpAirSource = coolingCoil.cast<CoilSystemIntegratedHeatPumpAirSource>();

        StraightComponent spaceCoolingCoil = coilSystemIntegratedHeatPumpAirSource->spaceCoolingCoil();
        boost::optional<IdfObject> _spaceCoolingCoil = translateAndMapModelObject(spaceCoolingCoil);
        _spaceCoolingCoil->setString(Coil_Cooling_DX_VariableSpeedFields::IndoorAirInletNodeName, nodeName);

        if (boost::optional<HVACComponent> scwhCoil = coilSystemIntegratedHeatPumpAirSource->scwhCoil()) {
          boost::optional<IdfObject> _scwhCoil = translateAndMapModelObject(scwhCoil.get());
          _scwhCoil->setString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::EvaporatorAirInletNodeName, nodeName);
        }

        if (boost::optional<StraightComponent> scdwhCoolingCoil = coilSystemIntegratedHeatPumpAirSource->scdwhCoolingCoil()) {
          boost::optional<IdfObject> _scdwhCoolingCoil = translateAndMapModelObject(scdwhCoolingCoil.get());
          _scdwhCoolingCoil->setString(Coil_Cooling_DX_VariableSpeedFields::IndoorAirInletNodeName, nodeName);
        }
      }
    }

    if (airInletNodeName) {
      fixSPMsForUnitarySystem(modelObject, airInletNodeName.get(), fanOutletNodeName);
    }

    if (_coolingCoil && _heatingCoil) {
      std::string nodeName = modelObject.name().get() + " Cooling Coil - Heating Coil Node";

      if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_SingleSpeed) {
        _coolingCoil->setString(Coil_Cooling_DX_SingleSpeedFields::AirOutletNodeName, nodeName);
      } else if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_SingleSpeed_ThermalStorage) {
        _coolingCoil->setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirOutletNodeName, nodeName);
      } else if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_VariableSpeed) {
        _coolingCoil->setString(Coil_Cooling_DX_VariableSpeedFields::IndoorAirOutletNodeName, nodeName);
      } else if (_coolingCoil->iddObject().type() == IddObjectType::CoilSystem_IntegratedHeatPump_AirSource) {
        StraightComponent spaceCoolingCoil = coilSystemIntegratedHeatPumpAirSource->spaceCoolingCoil();
        boost::optional<IdfObject> _spaceCoolingCoil = translateAndMapModelObject(spaceCoolingCoil);
        _spaceCoolingCoil->setString(Coil_Cooling_DX_VariableSpeedFields::IndoorAirOutletNodeName, nodeName);

        if (boost::optional<HVACComponent> scwhCoil = coilSystemIntegratedHeatPumpAirSource->scwhCoil()) {
          boost::optional<IdfObject> _scwhCoil = translateAndMapModelObject(scwhCoil.get());
          _scwhCoil->setString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::EvaporatorAirOutletNodeName, nodeName);
        }

        if (boost::optional<StraightComponent> scdwhCoolingCoil = coilSystemIntegratedHeatPumpAirSource->scdwhCoolingCoil()) {
          boost::optional<IdfObject> _scdwhCoolingCoil = translateAndMapModelObject(scdwhCoolingCoil.get());
          _scdwhCoolingCoil->setString(Coil_Cooling_DX_VariableSpeedFields::IndoorAirOutletNodeName, nodeName);
        }
      }

      if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_DX_SingleSpeed) {
        _heatingCoil->setString(Coil_Heating_DX_SingleSpeedFields::AirInletNodeName, nodeName);
      } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_DX_VariableSpeed) {
        _heatingCoil->setString(Coil_Heating_DX_VariableSpeedFields::IndoorAirInletNodeName, nodeName);
      } else if (_heatingCoil->iddObject().type() == IddObjectType::CoilSystem_IntegratedHeatPump_AirSource) {
        coilSystemIntegratedHeatPumpAirSource = heatingCoil.cast<CoilSystemIntegratedHeatPumpAirSource>();

        StraightComponent spaceHeatingCoil = coilSystemIntegratedHeatPumpAirSource->spaceHeatingCoil();
        boost::optional<IdfObject> _spaceHeatingCoil = translateAndMapModelObject(spaceHeatingCoil);
        _spaceHeatingCoil->setString(Coil_Heating_DX_VariableSpeedFields::IndoorAirInletNodeName, nodeName);

        if (boost::optional<StraightComponent> shdwhHeatingCoil = coilSystemIntegratedHeatPumpAirSource->shdwhHeatingCoil()) {
          boost::optional<IdfObject> _shdwhHeatingCoil = translateAndMapModelObject(shdwhHeatingCoil.get());
          _shdwhHeatingCoil->setString(Coil_Heating_DX_VariableSpeedFields::IndoorAirInletNodeName, nodeName);
        }
      }
    }

    if (_supplementalHeatingCoil) {
      std::string nodeName = modelObject.name().get() + " Heating Coil - Supplemental Coil Node";

      if (_heatingCoil) {
        if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_DX_SingleSpeed) {
          _heatingCoil->setString(Coil_Heating_DX_SingleSpeedFields::AirOutletNodeName, nodeName);
        } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_DX_VariableSpeed) {
          _heatingCoil->setString(Coil_Heating_DX_VariableSpeedFields::IndoorAirOutletNodeName, nodeName);
        } else if (_heatingCoil->iddObject().type() == IddObjectType::CoilSystem_IntegratedHeatPump_AirSource) {
          coilSystemIntegratedHeatPumpAirSource = heatingCoil.cast<CoilSystemIntegratedHeatPumpAirSource>();

          StraightComponent spaceHeatingCoil = coilSystemIntegratedHeatPumpAirSource->spaceHeatingCoil();
          boost::optional<IdfObject> _spaceHeatingCoil = translateAndMapModelObject(spaceHeatingCoil);
          _spaceHeatingCoil->setString(Coil_Heating_DX_VariableSpeedFields::IndoorAirOutletNodeName, nodeName);

          if (boost::optional<StraightComponent> shdwhHeatingCoil = coilSystemIntegratedHeatPumpAirSource->shdwhHeatingCoil()) {
            boost::optional<IdfObject> _shdwhHeatingCoil = translateAndMapModelObject(shdwhHeatingCoil.get());
            _shdwhHeatingCoil->setString(Coil_Heating_DX_VariableSpeedFields::IndoorAirOutletNodeName, nodeName);
          }
        }
      }

      if (_supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Fuel) {
        if (airOutletNodeName) {
          _supplementalHeatingCoil->setString(Coil_Heating_FuelFields::AirOutletNodeName, airOutletNodeName.get());
          _supplementalHeatingCoil->setString(Coil_Heating_FuelFields::AirInletNodeName, nodeName);
        }
      } else if (_supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric) {
        if (airOutletNodeName) {
          _supplementalHeatingCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName, airOutletNodeName.get());
          _supplementalHeatingCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName, nodeName);
        }
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
