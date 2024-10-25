/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/FanOnOff.hpp"
#include "../../model/FanOnOff_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/FanConstantVolume_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../../model/ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveBiquadratic_Impl.hpp"
#include "../../model/CurveCubic.hpp"
#include "../../model/CurveCubic_Impl.hpp"
#include "../../model/CoilCoolingDXVariableRefrigerantFlow.hpp"
#include "../../model/CoilCoolingDXVariableRefrigerantFlow_Impl.hpp"
#include "../../model/CoilHeatingDXVariableRefrigerantFlow.hpp"
#include "../../model/CoilHeatingDXVariableRefrigerantFlow_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include "../../utilities/math/FloatCompare.hpp"
#include <utilities/idd/ZoneHVAC_TerminalUnit_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/Fan_SystemModel_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/OutdoorAir_Mixer_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControl_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControl_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Fuel_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateZoneHVACTerminalUnitVariableRefrigerantFlow(ZoneHVACTerminalUnitVariableRefrigerantFlow& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow);

    m_idfObjects.push_back(idfObject);

    // Name

    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    auto t_airLoopHVAC = modelObject.airLoopHVAC();

    // TerminalUnitAvailabilityschedule

    if (boost::optional<model::Schedule> schedule = modelObject.terminalUnitAvailabilityschedule()) {
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAvailabilitySchedule, _schedule->name().get());
      }
    }

    // SupplyAirFlowRateDuringCoolingOperation

    if (modelObject.isSupplyAirFlowRateDuringCoolingOperationAutosized()) {
      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::CoolingSupplyAirFlowRate, "Autosize");
    } else if ((value = modelObject.supplyAirFlowRateDuringCoolingOperation())) {
      idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::CoolingSupplyAirFlowRate, value.get());
    }

    // SupplyAirFlowRateWhenNoCoolingisNeeded

    if (modelObject.isSupplyAirFlowRateWhenNoCoolingisNeededAutosized()) {
      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::NoCoolingSupplyAirFlowRate, "Autosize");
    } else if ((value = modelObject.supplyAirFlowRateWhenNoCoolingisNeeded())) {
      idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::NoCoolingSupplyAirFlowRate, value.get());
    }

    // SupplyAirFlowRateDuringHeatingOperation

    if (modelObject.isSupplyAirFlowRateDuringHeatingOperationAutosized()) {
      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::HeatingSupplyAirFlowRate, "Autosize");
    } else if ((value = modelObject.supplyAirFlowRateDuringHeatingOperation())) {
      idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::HeatingSupplyAirFlowRate, value.get());
    }

    // SupplyAirFlowRateWhenNoHeatingisNeeded

    if (modelObject.isSupplyAirFlowRateWhenNoHeatingisNeededAutosized()) {
      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::NoHeatingSupplyAirFlowRate, "Autosize");
    } else if ((value = modelObject.supplyAirFlowRateWhenNoHeatingisNeeded())) {
      idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::NoHeatingSupplyAirFlowRate, value.get());
    }

    // OutdoorAirFlowRateDuringCoolingOperation

    if (modelObject.isOutdoorAirFlowRateDuringCoolingOperationAutosized()) {
      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::CoolingOutdoorAirFlowRate, "Autosize");
    } else if ((value = modelObject.outdoorAirFlowRateDuringCoolingOperation())) {
      idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::CoolingOutdoorAirFlowRate, value.get());
    }

    // OutdoorAirFlowRateDuringHeatingOperation

    if (modelObject.isOutdoorAirFlowRateDuringHeatingOperationAutosized()) {
      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::HeatingOutdoorAirFlowRate, "Autosize");
    } else if ((value = modelObject.outdoorAirFlowRateDuringHeatingOperation())) {
      idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::HeatingOutdoorAirFlowRate, value.get());
    }

    // OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded

    if (modelObject.isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized()) {
      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::NoLoadOutdoorAirFlowRate, "Autosize");
    } else if ((value = modelObject.outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded())) {
      idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::NoLoadOutdoorAirFlowRate, value.get());
    }

    // SupplyAirFanOperatingModeScheduleName

    if (boost::optional<model::Schedule> schedule = modelObject.supplyAirFanOperatingModeSchedule()) {
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanOperatingModeScheduleName, _schedule->name().get());
      }
    }

    // SupplyAirFanplacement
    bool t_blowThrough = false;
    auto fanPlacement = modelObject.supplyAirFanPlacement();
    idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanPlacement, fanPlacement);
    if (istringEqual(fanPlacement, "BlowThrough")) {
      t_blowThrough = true;
    }

    // Node Names
    // Currently we default the Fan to DrawThrough, which means the components are ordered as such:
    // InletNode ----- (Mixer) ---- CoolingCoil ----- HeatingCoil ----- Fan ---- (Supplemental Heating Coil) -------- OutletNode
    // BlowThrough
    // InletNode ----- (Mixer) ---- Fan ---- CoolingCoil ----- HeatingCoil ----- (Supplemental Heating Coil) -------- OutletNode

    std::string inletNodeName;
    std::string outletNodeName;

    if (boost::optional<model::Node> node = modelObject.inletNode()) {
      inletNodeName = node->name().get();
    }

    if (boost::optional<model::Node> node = modelObject.outletNode()) {
      outletNodeName = node->name().get();
    }

    // TerminalUnitAirInletNodeName

    idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAirInletNodeName, inletNodeName);

    // TerminalUnitAirOutletNodeName

    idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAirOutletNodeName, outletNodeName);

    auto translateMixer = [&]() {
      if (modelObject.airLoopHVAC() || modelObject.airLoopHVACOutdoorAirSystem()) {
        return false;
      }

      bool zeroOA = false;
      if ((value = modelObject.outdoorAirFlowRateDuringCoolingOperation())) {
        zeroOA = equal(value.get(), 0.0);
      }
      if ((value = modelObject.outdoorAirFlowRateDuringHeatingOperation())) {
        zeroOA = (zeroOA && equal(value.get(), 0.0));
      }
      if ((value = modelObject.outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded())) {
        zeroOA = (zeroOA && equal(value.get(), 0.0));
      }

      if (zeroOA) {
        return false;
      }

      return true;
    };

    // OutdoorAirMixer
    if (translateMixer()) {
      IdfObject _outdoorAirMixer(IddObjectType::OutdoorAir_Mixer);
      _outdoorAirMixer.setName(modelObject.name().get() + " OA Mixer");
      m_idfObjects.push_back(_outdoorAirMixer);

      std::string mixerOutletNodeName = modelObject.name().get() + " Mixer Outlet Node";
      std::string oaNodeName = modelObject.name().get() + " Outdoor Air Node";

      _outdoorAirMixer.setString(OutdoorAir_MixerFields::MixedAirNodeName, mixerOutletNodeName);

      _outdoorAirMixer.setString(OutdoorAir_MixerFields::ReturnAirStreamNodeName, inletNodeName);

      IdfObject _oaNodeList(openstudio::IddObjectType::OutdoorAir_NodeList);
      _oaNodeList.setString(0, oaNodeName);
      m_idfObjects.push_back(_oaNodeList);

      _outdoorAirMixer.setString(OutdoorAir_MixerFields::ReliefAirStreamNodeName, modelObject.name().get() + " Relief Node Name");

      _outdoorAirMixer.setString(OutdoorAir_MixerFields::OutdoorAirStreamNodeName, oaNodeName);

      // OutsideAirMixerObjectType

      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutsideAirMixerObjectType, _outdoorAirMixer.iddObject().name());

      // OutsideAirMixerObjectName

      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutsideAirMixerObjectName, _outdoorAirMixer.name().get());

      // For the remaining components, the first node is the mixerOutletNodeName
      inletNodeName = mixerOutletNodeName;
    }

    struct Component
    {
      Component(const IdfObject& obj, const std::string& t_outletNodeName, unsigned t_inletIndex, unsigned t_outletIndex)
        : idfObject(obj), outletNodeName(t_outletNodeName), inletIndex(t_inletIndex), outletIndex(t_outletIndex) {}
      IdfObject idfObject;
      std::string outletNodeName;
      unsigned inletIndex;
      unsigned outletIndex;
    };

    size_t fanIndex = -1;

    // We'll store the Components in the vector, so we can write their inlet/outlet nodes in the right order
    std::vector<Component> compsInOrder;

    auto getFanInletOutletIndexes = [&modelObject](const IdfObject& idf_fan) {
      if (idf_fan.iddObject().type() == IddObjectType::Fan_OnOff) {
        return std::pair<unsigned, unsigned>{Fan_OnOffFields::AirInletNodeName, Fan_OnOffFields::AirOutletNodeName};
      } else if (idf_fan.iddObject().type() == IddObjectType::Fan_ConstantVolume) {
        return std::pair<unsigned, unsigned>{Fan_ConstantVolumeFields::AirInletNodeName, Fan_ConstantVolumeFields::AirOutletNodeName};
      } else if (idf_fan.iddObject().type() == IddObjectType::Fan_SystemModel) {
        return std::pair<unsigned, unsigned>{Fan_SystemModelFields::AirInletNodeName, Fan_SystemModelFields::AirOutletNodeName};
      } else {
        LOG(Error, "VRF named " << modelObject.name().get() << " uses an unsupported fan type.");
        OS_ASSERT(false);
        return std::pair<unsigned, unsigned>{0, 0};
      }
    };

    auto translateFan = [&](ModelObject& fan) {
      // Only translate fan if it's not on an AirLoopHVACOutdoorAirSystem,n or on a AirLoopHVAC main branch that already has a fan
      if (modelObject.airLoopHVACOutdoorAirSystem()) {
        LOG(Info, "Will not translate fan for VRF named '" << modelObject.name().get() << "' since it is on an AirLoopHVACOutdoorAirSystem.");
        return;
      }
      if (modelObject.airLoopHVAC() && modelObject.airLoopHVAC()->supplyFan() && modelObject.airLoopHVAC()->supplyComponent(modelObject.handle())) {
        LOG(Info, "Will not translate fan for VRF named '" << modelObject.name().get()
                                                           << "' since it is on an AirLoopHVAC main branch that already has a supply Fan assigned.");
        return;
      }

      if (boost::optional<IdfObject> idf_fan_ = translateAndMapModelObject(fan)) {
        idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanObjectType, idf_fan_->iddObject().name());
        idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanObjectName, idf_fan_->name().get());
        auto [inletIndex, outletIndex] = getFanInletOutletIndexes(idf_fan_.get());
        fanIndex = compsInOrder.size();  // Store index
        compsInOrder.emplace_back(Component(idf_fan_.get(), modelObject.name().get() + " Fan Outlet Node", inletIndex, outletIndex));
      }
    };

    if (t_blowThrough) {
      // For now this is always instantiated...
      if (boost::optional<HVACComponent> fan_ = modelObject.supplyAirFan()) {
        translateFan(fan_.get());
      }
    }

    if (auto coolingCoil = modelObject.coolingCoil()) {

      if (boost::optional<IdfObject> _coolingCoil = translateAndMapModelObject(coolingCoil.get())) {

        idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::CoolingCoilObjectType, _coolingCoil->iddObject().name());
        idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::CoolingCoilObjectName, _coolingCoil->name().get());

        if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow) {
          compsInOrder.emplace_back(Component(_coolingCoil.get(), modelObject.name().get() + " Cooling Coil Outlet Node",
                                              Coil_Cooling_DX_VariableRefrigerantFlowFields::CoilAirInletNode,
                                              Coil_Cooling_DX_VariableRefrigerantFlowFields::CoilAirOutletNode));
        } else if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControl) {
          compsInOrder.emplace_back(Component(_coolingCoil.get(), modelObject.name().get() + " Cooling Coil Outlet Node",
                                              Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::CoilAirInletNode,
                                              Coil_Cooling_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::CoilAirOutletNode));
        }
      }
    }

    if (auto heatingCoil = modelObject.heatingCoil()) {
      if (boost::optional<IdfObject> _heatingCoil = translateAndMapModelObject(heatingCoil.get())) {

        idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::HeatingCoilObjectType, _heatingCoil->iddObject().name());
        idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::HeatingCoilObjectName, _heatingCoil->name().get());

        if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow) {
          compsInOrder.emplace_back(Component(_heatingCoil.get(), modelObject.name().get() + " Heating Coil Outlet Node",
                                              Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirInletNode,
                                              Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirOutletNode));
        } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControl) {
          compsInOrder.emplace_back(Component(_heatingCoil.get(), modelObject.name().get() + " Heating Coil Outlet Node",
                                              Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::CoilAirInletNode,
                                              Coil_Heating_DX_VariableRefrigerantFlow_FluidTemperatureControlFields::CoilAirOutletNode));
        }
      }
    }

    if (!t_blowThrough) {
      // For now this is always instantiated...
      if (boost::optional<HVACComponent> fan_ = modelObject.supplyAirFan()) {
        translateFan(fan_.get());
      }
    }

    if (auto _mo_supplementalHeatingCoil = modelObject.supplementalHeatingCoil()) {

      if (boost::optional<IdfObject> _supplementalHeatingCoil = translateAndMapModelObject(_mo_supplementalHeatingCoil.get())) {

        idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplementalHeatingCoilObjectType,
                            _supplementalHeatingCoil->iddObject().name());
        idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplementalHeatingCoilName, _supplementalHeatingCoil->name().get());

        auto getCoilInletOutletIndexes = [&modelObject](const IdfObject& idf_coil) {
          if (idf_coil.iddObject().type() == IddObjectType::Coil_Heating_Fuel) {
            return std::pair<unsigned, unsigned>{Coil_Heating_FuelFields::AirInletNodeName, Coil_Heating_FuelFields::AirOutletNodeName};
          } else if (idf_coil.iddObject().type() == IddObjectType::Coil_Heating_Electric) {
            return std::pair<unsigned, unsigned>{Coil_Heating_ElectricFields::AirInletNodeName, Coil_Heating_ElectricFields::AirOutletNodeName};
          } else if (idf_coil.iddObject().type() == IddObjectType::Coil_Heating_Water) {
            return std::pair<unsigned, unsigned>{Coil_Heating_WaterFields::AirInletNodeName, Coil_Heating_WaterFields::AirOutletNodeName};
          } else if (idf_coil.iddObject().type() == IddObjectType::Coil_Heating_Steam) {
            // Not yet supported in OS
            OS_ASSERT(false);
          } else {
            LOG(Error, "Unsupported supplemental heating Coil type for " << modelObject.briefDescription());
            OS_ASSERT(false);
          }
          return std::pair<unsigned, unsigned>{0, 0};
        };

        auto [inletIndex, outletIndex] = getCoilInletOutletIndexes(_supplementalHeatingCoil.get());

        compsInOrder.emplace_back(
          Component(_supplementalHeatingCoil.get(), modelObject.name().get() + " Supplemental Heating Coil Outlet Node", inletIndex, outletIndex));
      }
    }

    // Now we write the nodes in the order we determine
    // Last Node is the VRT TU outlet Node
    compsInOrder.back().outletNodeName = outletNodeName;

    size_t curIndex = 0;
    for (auto& comp : compsInOrder) {
      comp.idfObject.setString(comp.inletIndex, inletNodeName);
      comp.idfObject.setString(comp.outletIndex, comp.outletNodeName);

      if (curIndex == fanIndex) {
        // Adjust the Fan Inlet/OutletNode to pick up the fan inside this unit.
        fixSPMsForUnitarySystem(modelObject, inletNodeName, comp.outletNodeName);
      }

      inletNodeName = comp.outletNodeName;
      ++curIndex;
    }

    // ZoneTerminalUnitOnParasiticElectricEnergyUse

    if ((value = modelObject.zoneTerminalUnitOnParasiticElectricEnergyUse())) {
      idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::ZoneTerminalUnitOnParasiticElectricEnergyUse, value.get());
    }

    // ZoneTerminalUnitOffParasiticElectricEnergyUse

    if ((value = modelObject.zoneTerminalUnitOffParasiticElectricEnergyUse())) {
      idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::ZoneTerminalUnitOffParasiticElectricEnergyUse, value.get());
    }

    // RatedTotalHeatingCapacitySizingRatio

    if ((value = modelObject.ratedTotalHeatingCapacitySizingRatio())) {
      idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::RatedHeatingCapacitySizingRatio, value.get());
    }

    // \field Availability Manager List Name
    // \field Design Specification ZoneHVAC Sizing Object Name

    //  Maximum Supply Air Temperature from Supplemental Heater (autosized)
    if (modelObject.isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized()) {
      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::MaximumSupplyAirTemperaturefromSupplementalHeater, "Autosize");
    } else if ((value = modelObject.maximumSupplyAirTemperaturefromSupplementalHeater())) {
      idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::MaximumSupplyAirTemperaturefromSupplementalHeater, value.get());
    }

    // \field Maximum Outdoor Dry-Bulb Temperature for Supplemental Heater Operation (default 21C)
    idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation,
                        modelObject.maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation());

    // ControllingZoneorThermostatLocation
    if (boost::optional<ThermalZone> tz = modelObject.controllingZoneorThermostatLocation()) {
      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::ControllingZoneorThermostatLocation, tz->name().get());
    } else if (auto airLoop = modelObject.airLoopHVAC()) {
      auto zones = airLoop->thermalZones();
      if (zones.size() == 1u) {
        if (auto zone = translateAndMapModelObject(zones.front())) {
          idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::ControllingZoneorThermostatLocation, zone->name().get());
        }
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
