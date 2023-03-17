/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "ForwardTranslatePlantEquipmentOperationSchemes.hpp"

#include "../../model/Model.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/BoilerHotWater.hpp"
#include "../../model/BoilerHotWater_Impl.hpp"

#include "../../model/CentralHeatPumpSystem.hpp"
#include "../../model/CentralHeatPumpSystem_Impl.hpp"
#include "../../model/CentralHeatPumpSystemModule.hpp"
#include "../../model/ChillerHeaterPerformanceElectricEIR.hpp"

#include "../../model/ChillerElectricASHRAE205.hpp"
#include "../../model/ChillerElectricASHRAE205_Impl.hpp"
#include "../../model/ChillerElectricEIR.hpp"
#include "../../model/ChillerElectricEIR_Impl.hpp"
#include "../../model/ChillerElectricReformulatedEIR.hpp"
#include "../../model/ChillerElectricReformulatedEIR_Impl.hpp"
#include "../../model/ChillerAbsorptionIndirect.hpp"
#include "../../model/ChillerAbsorptionIndirect_Impl.hpp"
#include "../../model/ChillerAbsorption.hpp"
#include "../../model/ChillerAbsorption_Impl.hpp"
#include "../../model/CoolingTowerSingleSpeed.hpp"
#include "../../model/CoolingTowerSingleSpeed_Impl.hpp"
#include "../../model/CoolingTowerVariableSpeed.hpp"
#include "../../model/CoolingTowerVariableSpeed_Impl.hpp"
#include "../../model/CoolingTowerTwoSpeed.hpp"
#include "../../model/CoolingTowerTwoSpeed_Impl.hpp"
#include "../../model/GeneratorFuelCellExhaustGasToWaterHeatExchanger.hpp"
#include "../../model/GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl.hpp"
#include "../../model/GroundHeatExchangerVertical.hpp"
#include "../../model/GroundHeatExchangerVertical_Impl.hpp"
#include "../../model/GroundHeatExchangerHorizontalTrench.hpp"
#include "../../model/GroundHeatExchangerHorizontalTrench_Impl.hpp"
#include "../../model/HeatExchangerFluidToFluid.hpp"
#include "../../model/HeatExchangerFluidToFluid_Impl.hpp"
#include "../../model/PlantEquipmentOperationCoolingLoad.hpp"
#include "../../model/PlantEquipmentOperationCoolingLoad_Impl.hpp"
#include "../../model/PlantEquipmentOperationHeatingLoad.hpp"
#include "../../model/PlantEquipmentOperationHeatingLoad_Impl.hpp"
#include "../../model/PlantEquipmentOperationScheme.hpp"
#include "../../model/PlantEquipmentOperationScheme_Impl.hpp"
#include "../../model/EvaporativeFluidCoolerSingleSpeed.hpp"
#include "../../model/EvaporativeFluidCoolerSingleSpeed_Impl.hpp"
#include "../../model/EvaporativeFluidCoolerTwoSpeed.hpp"
#include "../../model/EvaporativeFluidCoolerTwoSpeed_Impl.hpp"
#include "../../model/FluidCoolerSingleSpeed.hpp"
#include "../../model/FluidCoolerSingleSpeed_Impl.hpp"
#include "../../model/FluidCoolerTwoSpeed.hpp"
#include "../../model/FluidCoolerTwoSpeed_Impl.hpp"
#include "../../model/SetpointManager.hpp"
#include "../../model/SetpointManager_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalStorageChilledWaterStratified.hpp"
#include "../../model/ThermalStorageChilledWaterStratified_Impl.hpp"
#include "../../model/WaterHeaterMixed.hpp"
#include "../../model/WaterHeaterMixed_Impl.hpp"
#include "../../model/WaterHeaterStratified.hpp"
#include "../../model/WaterHeaterStratified_Impl.hpp"
#include "../../model/ZoneHVACComponent.hpp"
#include "../../model/ZoneHVACComponent_Impl.hpp"
#include "../../model/SolarCollectorIntegralCollectorStorage.hpp"
#include "../../model/SolarCollectorIntegralCollectorStorage_Impl.hpp"
#include "../../model/SolarCollectorFlatPlateWater.hpp"
#include "../../model/SolarCollectorFlatPlateWater_Impl.hpp"
#include "../../model/SolarCollectorFlatPlatePhotovoltaicThermal.hpp"
#include "../../model/SolarCollectorFlatPlatePhotovoltaicThermal_Impl.hpp"
#include "../../model/PlantComponentTemperatureSource.hpp"
#include "../../model/PlantComponentTemperatureSource_Impl.hpp"
#include "../../model/PlantComponentUserDefined.hpp"
#include "../../model/PlantComponentUserDefined_Impl.hpp"
#include "../../model/HeatPumpAirToWaterFuelFiredHeating.hpp"
#include "../../model/HeatPumpAirToWaterFuelFiredHeating_Impl.hpp"
#include "../../model/HeatPumpAirToWaterFuelFiredCooling.hpp"
#include "../../model/HeatPumpAirToWaterFuelFiredCooling_Impl.hpp"
#include "../../model/HeatPumpWaterToWaterEquationFitHeating.hpp"
#include "../../model/HeatPumpWaterToWaterEquationFitHeating_Impl.hpp"
#include "../../model/HeatPumpWaterToWaterEquationFitCooling.hpp"
#include "../../model/HeatPumpWaterToWaterEquationFitCooling_Impl.hpp"
#include "../../model/HeatPumpPlantLoopEIRHeating.hpp"
#include "../../model/HeatPumpPlantLoopEIRHeating_Impl.hpp"
#include "../../model/HeatPumpPlantLoopEIRCooling.hpp"
#include "../../model/HeatPumpPlantLoopEIRCooling_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/PlantEquipmentOperationSchemes_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_HeatingLoad_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_CoolingLoad_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_ComponentSetpoint_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentOperation_Uncontrolled_FieldEnums.hxx>
#include <utilities/idd/PlantEquipmentList_FieldEnums.hxx>

// Special case
#include "../../model/GeneratorMicroTurbineHeatRecovery.hpp"
#include "../../model/GeneratorMicroTurbineHeatRecovery_Impl.hpp"
#include "../../model/GeneratorMicroTurbine.hpp"
#include "../../model/GeneratorMicroTurbine_Impl.hpp"

using namespace openstudio::model;
using namespace openstudio::energyplus;

using namespace std;

namespace openstudio {

namespace energyplus {

  bool _isSetpointComponent(const PlantLoop& plantLoop, const ModelObject& comp) {
    auto supplyOutletNode = plantLoop.supplyOutletNode();
    auto hvacComp = comp.optionalCast<HVACComponent>();
    OS_ASSERT(hvacComp);

    auto nodes = subsetCastVector<Node>(plantLoop.supplyComponents(hvacComp.get(), supplyOutletNode));
    OS_ASSERT(!nodes.empty());
    auto componentOutletNode = nodes.front();

    if (componentOutletNode != supplyOutletNode) {
      auto _setpointManagers = componentOutletNode.setpointManagers();
      if (!_setpointManagers.empty()) {
        return true;
      }
    }

    return false;
  }

  boost::optional<Node> inletNode(const PlantLoop& plantLoop, const HVACComponent& component) {
    auto comps = plantLoop.supplyComponents(plantLoop.supplyInletNode(), component);
    OS_ASSERT(comps.size() >= 2u);
    return (comps.end() - 2u)->optionalCast<Node>();
  }

  boost::optional<Node> outletNode(const PlantLoop& plantLoop, const HVACComponent& component) {
    auto comps = plantLoop.supplyComponents(component, plantLoop.supplyOutletNode());
    OS_ASSERT(comps.size() >= 2u);
    return comps[1].optionalCast<Node>();
  }

  boost::optional<double> flowrate(const HVACComponent& component) {
    boost::optional<double> result;
    switch (component.iddObject().type().value()) {
      case openstudio::IddObjectType::OS_Generator_FuelCell_ExhaustGasToWaterHeatExchanger: {
        auto hr = component.cast<GeneratorFuelCellExhaustGasToWaterHeatExchanger>();
        result = hr.heatRecoveryWaterMaximumFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_Generator_MicroTurbine_HeatRecovery: {
        auto mchpHR = component.cast<GeneratorMicroTurbineHeatRecovery>();
        result = mchpHR.maximumHeatRecoveryWaterFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_Boiler_HotWater: {
        auto boiler = component.cast<BoilerHotWater>();
        result = boiler.designWaterFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_WaterHeater_Mixed: {
        auto waterHeater = component.cast<WaterHeaterMixed>();
        result = waterHeater.useSideDesignFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_WaterHeater_Stratified: {
        auto waterHeater = component.cast<WaterHeaterStratified>();
        result = waterHeater.useSideDesignFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_DistrictHeating: {
        break;
      }
      case openstudio::IddObjectType::OS_CentralHeatPumpSystem: {
        // TODO: JM define behavior needed here...
        // Probably check if all modules have a ChillerHeaterPerformanceComponent
        // that has a flow rate hardsized, and multiple by the number of performance comp
        // Better to add a method to centralheatpumpsystem, for eg "totalDesignFlowRate"
        // Problem is that you need to know which loop is requesting this since there are two supply loops with
        // "Design XXX Water Flow Rate" (XXX= Chilled Water, or Hot Water)
        break;
      }

      case openstudio::IddObjectType::OS_Chiller_Electric_ASHRAE205: {
        auto chiller = component.cast<ChillerElectricASHRAE205>();
        result = chiller.chilledWaterMaximumRequestedFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_Chiller_Electric_EIR: {
        auto chiller = component.cast<ChillerElectricEIR>();
        result = chiller.referenceChilledWaterFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_Chiller_Electric_ReformulatedEIR: {
        auto chiller = component.cast<ChillerElectricReformulatedEIR>();
        result = chiller.referenceChilledWaterFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_Chiller_Absorption_Indirect: {
        auto chiller = component.cast<ChillerAbsorptionIndirect>();
        result = chiller.designChilledWaterFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_Chiller_Absorption: {
        auto chiller = component.cast<ChillerAbsorption>();
        result = chiller.designChilledWaterFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_ThermalStorage_ChilledWater_Stratified: {
        auto ts = component.cast<ThermalStorageChilledWaterStratified>();
        result = ts.useSideDesignFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_ThermalStorage_Ice_Detailed: {
        break;
      }
      case openstudio::IddObjectType::OS_DistrictCooling: {
        break;
      }
      case openstudio::IddObjectType::OS_CoolingTower_SingleSpeed: {
        auto tower = component.cast<CoolingTowerSingleSpeed>();
        result = tower.designWaterFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_CoolingTower_VariableSpeed: {
        auto tower = component.cast<CoolingTowerVariableSpeed>();
        result = tower.designWaterFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_CoolingTower_TwoSpeed: {
        auto tower = component.cast<CoolingTowerTwoSpeed>();
        result = tower.designWaterFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_EvaporativeFluidCooler_SingleSpeed: {
        auto mo = component.cast<EvaporativeFluidCoolerSingleSpeed>();
        result = mo.designWaterFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_EvaporativeFluidCooler_TwoSpeed: {
        auto mo = component.cast<EvaporativeFluidCoolerTwoSpeed>();
        result = mo.designWaterFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_FluidCooler_SingleSpeed: {
        auto mo = component.cast<FluidCoolerSingleSpeed>();
        result = mo.designWaterFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_FluidCooler_TwoSpeed: {
        auto mo = component.cast<FluidCoolerTwoSpeed>();
        result = mo.designWaterFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_GroundHeatExchanger_Vertical: {
        auto hx = component.cast<GroundHeatExchangerVertical>();
        result = hx.designFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_GroundHeatExchanger_HorizontalTrench: {
        auto hx = component.cast<GroundHeatExchangerHorizontalTrench>();
        result = hx.designFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_HeatExchanger_FluidToFluid: {
        auto hx = component.cast<HeatExchangerFluidToFluid>();
        result = hx.loopSupplySideDesignFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_SolarCollector_FlatPlate_PhotovoltaicThermal: {
        auto mo = component.cast<SolarCollectorFlatPlatePhotovoltaicThermal>();
        result = mo.designFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_SolarCollector_FlatPlate_Water: {
        auto mo = component.cast<SolarCollectorFlatPlateWater>();
        result = mo.maximumFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_SolarCollector_IntegralCollectorStorage: {
        auto mo = component.cast<SolarCollectorIntegralCollectorStorage>();
        result = mo.maximumFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_PlantComponent_TemperatureSource: {
        auto mo = component.cast<PlantComponentTemperatureSource>();
        result = mo.designVolumeFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_PlantComponent_UserDefined: {
        auto mo = component.cast<PlantComponentUserDefined>();
        //TODO use Design Volume Flow Rate Actuator for PlantLoop 1
        //result = mo.designVolumeFlowRateActuator().get();
        break;
      }
      case openstudio::IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Cooling: {
        auto mo = component.cast<HeatPumpWaterToWaterEquationFitCooling>();
        result = mo.ratedLoadSideFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Heating: {
        auto mo = component.cast<HeatPumpWaterToWaterEquationFitHeating>();
        result = mo.ratedLoadSideFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_HeatPump_PlantLoop_EIR_Cooling: {
        auto mo = component.cast<HeatPumpPlantLoopEIRCooling>();
        result = mo.loadSideReferenceFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_HeatPump_PlantLoop_EIR_Heating: {
        auto mo = component.cast<HeatPumpPlantLoopEIRHeating>();
        result = mo.loadSideReferenceFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_HeatPump_AirToWater_FuelFired_Heating: {
        auto mo = component.cast<HeatPumpAirToWaterFuelFiredHeating>();
        result = mo.designFlowRate();
        break;
      }
      case openstudio::IddObjectType::OS_HeatPump_AirToWater_FuelFired_Cooling: {
        auto mo = component.cast<HeatPumpAirToWaterFuelFiredCooling>();
        result = mo.designFlowRate();
        break;
      }
      default: {
        break;
      }
    }

    return result;
  }

  // Now using an HPP
  // enum class ComponentType {HEATING, COOLING, BOTH, NONE};

  // Forward declaration (needed because componentType and plantLoopType call each other)
  // Now using an HPP
  // ComponentType componentType(const HVACComponent & component);

  /*
 * Check the overall type of a plantLoop by checking what is on the supply side
 * The logic is as follows:
 * * If there is only heating, no cooling, no "both": Heating
 * * If there is only cooling, no heating, no "both": Cooling
 * * If there is no cooling, no heating, no "both": None
 * * All other cases: "both"
 */
  ComponentType plantLoopType(const PlantLoop& plantLoop) {

    bool has_cooling = false;
    bool has_heating = false;
    bool has_both = false;
    for (const auto& comp : subsetCastVector<HVACComponent>(plantLoop.supplyComponents())) {
      if (componentType(comp) == ComponentType::COOLING) {
        has_cooling = true;
      } else if (componentType(comp) == ComponentType::HEATING) {
        has_heating = true;
      } else if (componentType(comp) == ComponentType::BOTH) {
        has_both = true;
      }
    }

    // If source side is purely cooling
    if (has_cooling && !has_heating && !has_both) {
      return ComponentType::COOLING;

      // If source side is purely heating
    } else if (!has_cooling && has_heating && !has_both) {
      return ComponentType::HEATING;

      // If there is nothing
    } else if (!has_cooling && !has_heating && !has_both) {
      return ComponentType::NONE;

      // All other cases: BOTH
    } else {
      return ComponentType::BOTH;
    }
  }

  ComponentType componentType(const HVACComponent& component) {
    switch (component.iddObject().type().value()) {
      case openstudio::IddObjectType::OS_Generator_FuelCell_ExhaustGasToWaterHeatExchanger: {
        return ComponentType::HEATING;
      }
      case openstudio::IddObjectType::OS_Boiler_HotWater: {
        return ComponentType::HEATING;
      }
      case openstudio::IddObjectType::OS_WaterHeater_Mixed: {
        // Need to handle the case where this is used purely as a storage tank
        auto wh = component.cast<WaterHeaterMixed>();
        if (boost::optional<double> _cap = wh.heaterMaximumCapacity()) {
          if (_cap.get() == 0.0) {
            // Capacity is zero: it's a storage tank!
            // get the Source loop
            if (boost::optional<PlantLoop> _p = wh.secondaryPlantLoop()) {
              // I guess we could rely on Heating/Cooling/Condenser set in the sizingPlant
              // if ( openstudio::istringEqual(p->sizingPlant().loopType(), "Heating") { }
              // Might be better to do a recursive call to see what type of equipment you have on the supply side of the plant loop though
              return plantLoopType(_p.get());

              // We also need to check if the tank isn't attached to a HPWH, in which case we return Heating
              // There's some magic later that'll put the HPWH itself and not the tank on the PlantEquipmentOperation,
              // see function operationSchemeComponent()
            } else if (boost::optional<ZoneHVACComponent> zoneComp = wh.containingZoneHVACComponent()) {
              // We don't have to explicitly check if it's a WaterHeaterHeatPump or WaterHeaterHeatPumpWrappedCondenser since
              // these are the only two types of zoneHVACComponent that could have a WaterHeater object, but this might change in the future
              IddObjectType zoneCompIdd = zoneComp->iddObjectType();
              if ((zoneCompIdd == openstudio::IddObjectType::OS_WaterHeater_HeatPump)
                  || (zoneCompIdd == openstudio::IddObjectType::OS_WaterHeater_HeatPump_WrappedCondenser)) {
                return ComponentType::HEATING;
              }
            } else {
              // It isn't connected to a source side, and has zero capacity, and not part of HPWH => it's a buffer tank => NONE
              return ComponentType::NONE;
            }  // End if has source loop

          } else {
            // If there is a capacity, and it's not zero, it's heating
            return ComponentType::HEATING;
          }  // End cap == 0

        } else {
          // If autosize, then it's Heating
          return ComponentType::HEATING;
        }  // end has a heater capacity
      }
      case openstudio::IddObjectType::OS_WaterHeater_Stratified: {
        // Need to handle the case where this is used purely as a storage tank
        auto wh = component.cast<WaterHeaterStratified>();

        if (wh.heater2Capacity() == 0.0) {
          if (boost::optional<double> _cap = wh.heater1Capacity()) {
            if (_cap.get() == 0.0) {
              // Capacity is zero: it's a storage tank!
              // get the Source loop
              if (boost::optional<PlantLoop> _p = wh.secondaryPlantLoop()) {
                // Here we go check what's on the supply side of the secondary plantLoop
                return plantLoopType(_p.get());

                // If it's attached to a HPWH, it's Heating (see detailed note for WaterHeaterMixed above)
              } else if (boost::optional<ZoneHVACComponent> zoneComp = wh.containingZoneHVACComponent()) {
                IddObjectType zoneCompIdd = zoneComp->iddObjectType();
                if ((zoneCompIdd == openstudio::IddObjectType::OS_WaterHeater_HeatPump)
                    || (zoneCompIdd == openstudio::IddObjectType::OS_WaterHeater_HeatPump_WrappedCondenser)) {
                  return ComponentType::HEATING;
                }
              } else {
                // It isn't connected to a source side, and has zero capacity, and not part of HPWH => it's a buffer tank => NONE
                return ComponentType::NONE;
              }
            }
          }
        }

        // If we reach here, it means there is at least one heater that has a capacity non zero, so it's a HEATING type
        return ComponentType::HEATING;
      }
      case openstudio::IddObjectType::OS_DistrictHeating: {
        return ComponentType::HEATING;
      }
      case openstudio::IddObjectType::OS_CentralHeatPumpSystem: {
        // Note (JM): This really depends on which loop is calling this...

        // This component has a tertiary loop, and is placed on the supply side of TWO loops: Heating and Cooling
        // If it's the supplyLoop (= cooling), we should probably have a PlantEquipmentOperation::CoolingLoad
        // If it's the tertiaryLoop (= heating), PlantEquipmentOperation::HeatingLoad
        // Returning BOTH will place it on a PlantEquipmentOperation::Uncontrolled

        // As a result, this is handled in coolingComponents() and heatingComponents() directly
        return ComponentType::NONE;
      }
      case openstudio::IddObjectType::OS_Chiller_Electric_ASHRAE205: {
        return ComponentType::COOLING;
      }
      case openstudio::IddObjectType::OS_Chiller_Electric_EIR: {
        return ComponentType::COOLING;
      }
      case openstudio::IddObjectType::OS_Chiller_Electric_ReformulatedEIR: {
        return ComponentType::COOLING;
      }
      case openstudio::IddObjectType::OS_Chiller_Absorption_Indirect: {
        return ComponentType::COOLING;
      }
      case openstudio::IddObjectType::OS_Chiller_Absorption: {
        return ComponentType::COOLING;
      }
      case openstudio::IddObjectType::OS_ThermalStorage_ChilledWater_Stratified: {
        return ComponentType::COOLING;
      }
      case openstudio::IddObjectType::OS_ThermalStorage_Ice_Detailed: {
        // TODO: Are you sure about this @kbenne?
        return ComponentType::BOTH;
      }
      case openstudio::IddObjectType::OS_DistrictCooling: {
        return ComponentType::COOLING;
      }
      case openstudio::IddObjectType::OS_CoolingTower_SingleSpeed: {
        return ComponentType::COOLING;
      }
      case openstudio::IddObjectType::OS_CoolingTower_VariableSpeed: {
        return ComponentType::COOLING;
      }
      case openstudio::IddObjectType::OS_CoolingTower_TwoSpeed: {
        return ComponentType::COOLING;
      }
      case openstudio::IddObjectType::OS_EvaporativeFluidCooler_SingleSpeed: {
        return ComponentType::COOLING;
      }
      case openstudio::IddObjectType::OS_EvaporativeFluidCooler_TwoSpeed: {
        return ComponentType::COOLING;
      }
      case openstudio::IddObjectType::OS_FluidCooler_SingleSpeed: {
        return ComponentType::COOLING;
      }
      case openstudio::IddObjectType::OS_FluidCooler_TwoSpeed: {
        return ComponentType::COOLING;
      }
      case openstudio::IddObjectType::OS_GroundHeatExchanger_Vertical: {
        return ComponentType::BOTH;
      }
      case openstudio::IddObjectType::OS_GroundHeatExchanger_HorizontalTrench: {
        return ComponentType::BOTH;
      }
      case openstudio::IddObjectType::OS_HeatExchanger_FluidToFluid: {
        // "Smart" defaults instead of ComponentType::BOTH;
        auto hx = component.cast<HeatExchangerFluidToFluid>();

        std::string controlType = hx.controlType();

        if (openstudio::istringEqual(controlType, "HeatingSetpointModulated") || openstudio::istringEqual(controlType, "HeatingSetpointOnOff")) {
          return ComponentType::HEATING;

        } else if (openstudio::istringEqual(controlType, "CoolingSetpointModulated") || openstudio::istringEqual(controlType, "CoolingSetpointOnOff")
                   || openstudio::istringEqual(controlType, "CoolingDifferentialOnOff")
                   || openstudio::istringEqual(controlType, "CoolingSetpointOnOffWithComponentOverride")) {
          return ComponentType::COOLING;

        } else if (openstudio::istringEqual(controlType, "DualDeadbandSetpointModulated")
                   || openstudio::istringEqual(controlType, "DualDeadbandSetpointOnOff")) {
          return ComponentType::BOTH;
        } else if (openstudio::istringEqual(controlType, "UncontrolledOn")) {
          // Here we go check what's on the supply side of the secondary plantLoop if there is one
          // It's going to crash if there isn't a secondary plant Loop anyways
          // get the Source loop
          if (boost::optional<PlantLoop> _p = hx.secondaryPlantLoop()) {
            return plantLoopType(_p.get());
          } else {
            // It isn't connected to a source side (beats the point of the HX)
            return ComponentType::NONE;
          }  // End if has source loop

        } else {
          // For OperationSchemeModulated and OperationSchemeOnOff

          // TODO: would need to open a log channel
          //Log(Debug, "HeatExchangerFluidToFluid '" << hx.nameString() << "' doesn't have a "
          //"controlType ('" << controlType << "') that allows us to specify whether "
          //"it's heating or cooling, setting to ComponentType::BOTH");
          return ComponentType::BOTH;
        }
      }
      case openstudio::IddObjectType::OS_SolarCollector_FlatPlate_PhotovoltaicThermal: {
        return ComponentType::HEATING;
      }
      case openstudio::IddObjectType::OS_SolarCollector_FlatPlate_Water: {
        return ComponentType::HEATING;
      }
      case openstudio::IddObjectType::OS_SolarCollector_IntegralCollectorStorage: {
        return ComponentType::HEATING;
      }
      case openstudio::IddObjectType::OS_PlantComponent_TemperatureSource: {
        //TODO can this be inferred from PlantLoadingMode?
        return ComponentType::BOTH;
      }
      case openstudio::IddObjectType::OS_PlantComponent_UserDefined: {
        return ComponentType::BOTH;
      }
      case openstudio::IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Heating: {
        return ComponentType::HEATING;
      }
      case openstudio::IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Cooling: {
        return ComponentType::COOLING;
      }
      case openstudio::IddObjectType::OS_HeatPump_PlantLoop_EIR_Heating: {
        return ComponentType::HEATING;
      }
      case openstudio::IddObjectType::OS_HeatPump_PlantLoop_EIR_Cooling: {
        return ComponentType::COOLING;
      }
      case openstudio::IddObjectType::OS_HeatPump_AirToWater_FuelFired_Heating: {
        return ComponentType::HEATING;
      }
      case openstudio::IddObjectType::OS_HeatPump_AirToWater_FuelFired_Cooling: {
        return ComponentType::COOLING;
      }
      case openstudio::IddObjectType::OS_Generator_MicroTurbine_HeatRecovery: {
        // Maybe that should be both, in the case of an absorption chiller?
        // Also, should maybe check if it's in mode FollowThermal or FollowThermalLimitElectrical?
        // If not in these two modes, it doesn't care and just runs. Also, it's typically on the demand Side, and this method
        // is only called on the supply side
        return ComponentType::HEATING;
      }
      default: {
        return ComponentType::NONE;
      }
    }
  }

  // Some plant components air in a containingHVACComponent() and it is that
  // container which needs to go on the plant operation scheme. Here is a filter to
  // figure that out.
  HVACComponent operationSchemeComponent(const HVACComponent& component) {
    boost::optional<HVACComponent> result;

    switch (component.iddObject().type().value()) {
      case openstudio::IddObjectType::OS_WaterHeater_Mixed: {
        auto waterHeater = component.cast<WaterHeaterMixed>();
        if (auto hpwh = waterHeater.containingZoneHVACComponent()) {
          result = hpwh;
        }
        break;
      }
      case openstudio::IddObjectType::OS_WaterHeater_Stratified: {
        auto waterHeater = component.cast<WaterHeaterStratified>();
        if (auto hpwh = waterHeater.containingZoneHVACComponent()) {
          result = hpwh;
        }
        break;
      }
      default: {
        break;
      }
    }

    if (result) {
      return result.get();
    }

    return component;
  }

  std::vector<HVACComponent> setpointComponents(const PlantLoop& plantLoop) {
    std::vector<HVACComponent> result;

    for (const auto& comp : subsetCastVector<HVACComponent>(plantLoop.supplyComponents())) {
      if (!comp.optionalCast<Node>()) {
        if (_isSetpointComponent(plantLoop, comp)) {
          result.push_back(operationSchemeComponent(comp));
        }
      }
    }

    return result;
  }

  std::vector<HVACComponent> coolingComponents(const PlantLoop& plantLoop) {
    std::vector<HVACComponent> result;

    for (const auto& comp : subsetCastVector<HVACComponent>(plantLoop.supplyComponents())) {
      // Special case for CentralHeatPumpSystem. If plantLoop = central_hp.coolingLoop, we add it
      if (comp.iddObject().type().value() == openstudio::IddObjectType::OS_CentralHeatPumpSystem) {
        auto central_hp = comp.cast<CentralHeatPumpSystem>();
        if (central_hp.coolingPlantLoop().is_initialized()) {
          if (plantLoop.handle() == central_hp.coolingPlantLoop()->handle()) {
            result.push_back(operationSchemeComponent(comp));
          }
        }

        // Regular case
      } else if (componentType(comp) == ComponentType::COOLING) {
        result.push_back(operationSchemeComponent(comp));
      }
    }

    return result;
  }

  std::vector<HVACComponent> heatingComponents(const PlantLoop& plantLoop) {
    std::vector<HVACComponent> result;

    for (const auto& comp : subsetCastVector<HVACComponent>(plantLoop.supplyComponents())) {
      // Special case for CentralHeatPumpSystem. If plantLoop = central_hp.heatingPlantLoop, we add it
      if (comp.iddObject().type().value() == openstudio::IddObjectType::OS_CentralHeatPumpSystem) {
        auto central_hp = comp.cast<CentralHeatPumpSystem>();
        if (central_hp.heatingPlantLoop().is_initialized()) {
          if (plantLoop.handle() == central_hp.heatingPlantLoop()->handle()) {
            result.push_back(operationSchemeComponent(comp));
          }
        }

        // Regular case
      } else if (componentType(comp) == ComponentType::HEATING) {
        result.push_back(operationSchemeComponent(comp));
      }
    }

    return result;
  }

  std::vector<HVACComponent> uncontrolledComponents(const PlantLoop& plantLoop) {
    std::vector<HVACComponent> result;

    for (const auto& comp : subsetCastVector<HVACComponent>(plantLoop.supplyComponents())) {
      if ((componentType(comp) == ComponentType::BOTH) && !(_isSetpointComponent(plantLoop, comp))) {
        result.push_back(operationSchemeComponent(comp));
      }
    }

    return result;
  }

  boost::optional<IdfObject> ForwardTranslator::translatePlantEquipmentOperationSchemes(PlantLoop& plantLoop) {
    IdfObject operationSchemes(IddObjectType::PlantEquipmentOperationSchemes);
    m_idfObjects.push_back(operationSchemes);

    operationSchemes.setName(plantLoop.nameString() + " Operation Schemes");

    auto alwaysOnSchedule = plantLoop.model().alwaysOnDiscreteSchedule();
    auto _alwaysOn = translateAndMapModelObject(alwaysOnSchedule);
    OS_ASSERT(_alwaysOn);

    // Lambda does what the name suggests, create setpoint operation schemes.
    // This is for any component that has a setpoint manager on its outlet node
    auto createSetpointOperationScheme = [&](PlantLoop& plantLoop) {
      auto t_setpointComponents = setpointComponents(plantLoop);
      if (!t_setpointComponents.empty()) {
        IdfObject setpointOperation(IddObjectType::PlantEquipmentOperation_ComponentSetpoint);
        setpointOperation.setName(plantLoop.nameString() + " Setpoint Operation Scheme");
        m_idfObjects.push_back(setpointOperation);
        setpointOperation.clearExtensibleGroups();

        IdfExtensibleGroup eg = operationSchemes.pushExtensibleGroup();
        eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType, setpointOperation.iddObject().name());
        eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName, setpointOperation.nameString());
        if (const auto& schedule = plantLoop.componentSetpointOperationSchemeSchedule()) {
          eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeScheduleName, schedule->nameString());
        } else {
          eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeScheduleName, _alwaysOn->nameString());
        }

        for (auto& setpointComponent : t_setpointComponents) {
          // TODO: @kbenne, Find the right way to deal with this
          // For now, "dirty" (?) fix for Generator:MicroTurbine

          boost::optional<IdfObject> _idfObject;

          if (boost::optional<GeneratorMicroTurbineHeatRecovery> mchpHR = setpointComponent.optionalCast<GeneratorMicroTurbineHeatRecovery>()) {
            GeneratorMicroTurbine mchp = mchpHR->generatorMicroTurbine();
            _idfObject = translateAndMapModelObject(mchp);
          } else {
            _idfObject = translateAndMapModelObject(setpointComponent);
          }

          OS_ASSERT(_idfObject);

          IdfExtensibleGroup eg = setpointOperation.pushExtensibleGroup();
          eg.setString(PlantEquipmentOperation_ComponentSetpointExtensibleFields::EquipmentObjectType, _idfObject->iddObject().name());
          eg.setString(PlantEquipmentOperation_ComponentSetpointExtensibleFields::EquipmentName, _idfObject->nameString());
          if (const auto& t_inletNode = inletNode(plantLoop, setpointComponent)) {
            eg.setString(PlantEquipmentOperation_ComponentSetpointExtensibleFields::DemandCalculationNodeName, t_inletNode->nameString());
          }
          if (const auto& t_outletNode = outletNode(plantLoop, setpointComponent)) {
            eg.setString(PlantEquipmentOperation_ComponentSetpointExtensibleFields::SetpointNodeName, t_outletNode->nameString());
          }
          if (auto value = flowrate(setpointComponent)) {
            eg.setDouble(PlantEquipmentOperation_ComponentSetpointExtensibleFields::ComponentFlowRate, value.get());
          } else {
            eg.setString(PlantEquipmentOperation_ComponentSetpointExtensibleFields::ComponentFlowRate, "Autosize");
          }
          auto t_componentType = componentType(setpointComponent);
          switch (t_componentType) {
            case ComponentType::HEATING:
              eg.setString(PlantEquipmentOperation_ComponentSetpointExtensibleFields::OperationType, "Heating");
              break;
            case ComponentType::COOLING:
              eg.setString(PlantEquipmentOperation_ComponentSetpointExtensibleFields::OperationType, "Cooling");
              break;
            default:
              eg.setString(PlantEquipmentOperation_ComponentSetpointExtensibleFields::OperationType, "Dual");
              break;
          }
        }
      }
    };

    bool applyDefault = true;

    // If any operation schemes are defined in the model then don't apply default operation schemes

    if (auto coolingLoadScheme = plantLoop.plantEquipmentOperationCoolingLoad()) {
      auto _scheme = translateAndMapModelObject(coolingLoadScheme.get());
      OS_ASSERT(_scheme);
      auto eg = operationSchemes.pushExtensibleGroup();
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType, _scheme->iddObject().name());
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName, _scheme->nameString());
      if (const auto& schedule = plantLoop.plantEquipmentOperationCoolingLoadSchedule()) {
        eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeScheduleName, schedule->nameString());
      } else {
        eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeScheduleName, _alwaysOn->nameString());
      }

      applyDefault = false;
    }

    if (auto heatingLoadScheme = plantLoop.plantEquipmentOperationHeatingLoad()) {
      auto _scheme = translateAndMapModelObject(heatingLoadScheme.get());
      OS_ASSERT(_scheme);
      auto eg = operationSchemes.pushExtensibleGroup();
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType, _scheme->iddObject().name());
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName, _scheme->nameString());
      if (const auto& schedule = plantLoop.plantEquipmentOperationHeatingLoadSchedule()) {
        eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeScheduleName, schedule->nameString());
      } else {
        eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeScheduleName, _alwaysOn->nameString());
      }

      applyDefault = false;
    }

    if (auto primaryScheme = plantLoop.primaryPlantEquipmentOperationScheme()) {
      auto _scheme = translateAndMapModelObject(primaryScheme.get());
      OS_ASSERT(_scheme);
      auto eg = operationSchemes.pushExtensibleGroup();
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType, _scheme->iddObject().name());
      eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName, _scheme->nameString());
      if (const auto& schedule = plantLoop.primaryPlantEquipmentOperationSchemeSchedule()) {
        eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeScheduleName, schedule->nameString());
      } else {
        eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeScheduleName, _alwaysOn->nameString());
      }

      createSetpointOperationScheme(plantLoop);
      applyDefault = false;
    }

    if (applyDefault) {
      // If we get here then there must not be any operation schemes defined in the model
      // and we should go ahead and create default schemes.
      auto t_heatingComponents = heatingComponents(plantLoop);
      if (!t_heatingComponents.empty()) {
        IdfObject heatingOperation(IddObjectType::PlantEquipmentOperation_HeatingLoad);
        heatingOperation.setName(plantLoop.nameString() + " Heating Operation Scheme");
        m_idfObjects.push_back(heatingOperation);
        heatingOperation.clearExtensibleGroups();

        IdfObject plantEquipmentList(IddObjectType::PlantEquipmentList);
        plantEquipmentList.setName(plantLoop.nameString() + " Heating Equipment List");
        plantEquipmentList.clearExtensibleGroups();
        m_idfObjects.push_back(plantEquipmentList);

        IdfExtensibleGroup eg = heatingOperation.pushExtensibleGroup();
        eg.setDouble(PlantEquipmentOperation_HeatingLoadExtensibleFields::LoadRangeLowerLimit, 0.0);
        eg.setDouble(PlantEquipmentOperation_HeatingLoadExtensibleFields::LoadRangeUpperLimit, 1E9);
        eg.setString(PlantEquipmentOperation_HeatingLoadExtensibleFields::RangeEquipmentListName, plantEquipmentList.nameString());

        eg = operationSchemes.pushExtensibleGroup();
        eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType, heatingOperation.iddObject().name());
        eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName, heatingOperation.nameString());
        eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeScheduleName, _alwaysOn->nameString());

        for (auto& heatingComponent : t_heatingComponents) {
          if (auto idfObject_ = translateAndMapModelObject(heatingComponent)) {
            IdfExtensibleGroup eg = plantEquipmentList.pushExtensibleGroup();
            eg.setString(PlantEquipmentListExtensibleFields::EquipmentObjectType, idfObject_->iddObject().name());
            eg.setString(PlantEquipmentListExtensibleFields::EquipmentName, idfObject_->nameString());
          }
        }
      }

      auto t_coolingComponents = coolingComponents(plantLoop);
      if (!t_coolingComponents.empty()) {
        IdfObject coolingOperation(IddObjectType::PlantEquipmentOperation_CoolingLoad);
        coolingOperation.setName(plantLoop.nameString() + " Cooling Operation Scheme");
        m_idfObjects.push_back(coolingOperation);
        coolingOperation.clearExtensibleGroups();

        IdfObject plantEquipmentList(IddObjectType::PlantEquipmentList);
        plantEquipmentList.setName(plantLoop.nameString() + " Cooling Equipment List");
        plantEquipmentList.clearExtensibleGroups();
        m_idfObjects.push_back(plantEquipmentList);

        IdfExtensibleGroup eg = coolingOperation.pushExtensibleGroup();
        eg.setDouble(PlantEquipmentOperation_CoolingLoadExtensibleFields::LoadRangeLowerLimit, 0.0);
        eg.setDouble(PlantEquipmentOperation_CoolingLoadExtensibleFields::LoadRangeUpperLimit, 1E9);
        eg.setString(PlantEquipmentOperation_CoolingLoadExtensibleFields::RangeEquipmentListName, plantEquipmentList.nameString());

        eg = operationSchemes.pushExtensibleGroup();
        eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType, coolingOperation.iddObject().name());
        eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName, coolingOperation.nameString());
        eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeScheduleName, _alwaysOn->nameString());

        for (auto& coolingComponent : t_coolingComponents) {
          if (auto idfObject_ = translateAndMapModelObject(coolingComponent)) {
            IdfExtensibleGroup eg = plantEquipmentList.pushExtensibleGroup();
            eg.setString(PlantEquipmentListExtensibleFields::EquipmentObjectType, idfObject_->iddObject().name());
            eg.setString(PlantEquipmentListExtensibleFields::EquipmentName, idfObject_->nameString());
          }
        }
      }

      auto t_uncontrolledComponents = uncontrolledComponents(plantLoop);
      if (!t_uncontrolledComponents.empty()) {

        IdfObject uncontrolledOperation(IddObjectType::PlantEquipmentOperation_Uncontrolled);
        uncontrolledOperation.setName(plantLoop.nameString() + " Uncontrolled Operation Scheme");
        m_idfObjects.push_back(uncontrolledOperation);
        uncontrolledOperation.clearExtensibleGroups();

        IdfObject plantEquipmentList(IddObjectType::PlantEquipmentList);
        plantEquipmentList.setName(plantLoop.nameString() + " Uncontrolled Equipment List");
        plantEquipmentList.clearExtensibleGroups();
        m_idfObjects.push_back(plantEquipmentList);

        uncontrolledOperation.setString(PlantEquipmentOperation_UncontrolledFields::EquipmentListName, plantEquipmentList.nameString());

        auto eg = operationSchemes.pushExtensibleGroup();
        eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeObjectType, uncontrolledOperation.iddObject().name());
        eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeName, uncontrolledOperation.nameString());
        eg.setString(PlantEquipmentOperationSchemesExtensibleFields::ControlSchemeScheduleName, _alwaysOn->nameString());

        for (auto& uncontrolledComponent : t_uncontrolledComponents) {
          if (auto idfObject_ = translateAndMapModelObject(uncontrolledComponent)) {
            IdfExtensibleGroup eg = plantEquipmentList.pushExtensibleGroup();
            eg.setString(PlantEquipmentListExtensibleFields::EquipmentObjectType, idfObject_->iddObject().name());
            eg.setString(PlantEquipmentListExtensibleFields::EquipmentName, idfObject_->nameString());
          }
        }
      }

      createSetpointOperationScheme(plantLoop);
    }

    return operationSchemes;
  }

}  // namespace energyplus

}  // namespace openstudio
