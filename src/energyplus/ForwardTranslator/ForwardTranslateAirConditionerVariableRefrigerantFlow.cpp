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
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/AirConditionerVariableRefrigerantFlow.hpp"
#include "../../model/AirConditionerVariableRefrigerantFlow_Impl.hpp"
#include "../../model/ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../../model/ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/AirConditioner_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/ZoneTerminalUnitList_FieldEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateAirConditionerVariableRefrigerantFlow(AirConditionerVariableRefrigerantFlow& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject& idfObject = createRegisterAndNameIdfObject(IddObjectType::AirConditioner_VariableRefrigerantFlow, modelObject);

    // AvailabilityScheduleName
    if (boost::optional<model::Schedule> schedule = modelObject.availabilitySchedule()) {
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::AvailabilityScheduleName, _schedule->nameString());
      }
    }

    // RatedTotalCoolingCapacity
    if (modelObject.isGrossRatedTotalCoolingCapacityAutosized()) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::GrossRatedTotalCoolingCapacity, "Autosize");
    } else if ((value = modelObject.grossRatedTotalCoolingCapacity())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::GrossRatedTotalCoolingCapacity, value.get());
    }

    // RatedCoolingCOP
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::GrossRatedCoolingCOP, modelObject.grossRatedCoolingCOP());

    // MinimumCondenserInletNodeTemperatureinCoolingMode
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MinimumCondenserInletNodeTemperatureinCoolingMode,
                        modelObject.minimumOutdoorTemperatureinCoolingMode());

    // MaximumCondenserInletNodeTemperatureinCoolingMode
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MaximumCondenserInletNodeTemperatureinCoolingMode,
                        modelObject.maximumOutdoorTemperatureinCoolingMode());

    // CoolingCapacityRatioModifierFunctionofLowTemperatureCurveName
    if (boost::optional<model::Curve> curve = modelObject.coolingCapacityRatioModifierFunctionofLowTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofLowTemperatureCurveName,
                            _curve->nameString());
      }
    }

    // CoolingCapacityRatioBoundaryCurveName
    if (boost::optional<model::Curve> curve = modelObject.coolingCapacityRatioBoundaryCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioBoundaryCurveName, _curve->nameString());
      }
    }

    // CoolingCapacityRatioModifierFunctionofHighTemperatureCurveName
    if (boost::optional<model::Curve> curve = modelObject.coolingCapacityRatioModifierFunctionofHighTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofHighTemperatureCurveName,
                            _curve->nameString());
      }
    }

    // CoolingEnergyInputRatioModifierFunctionofLowTemperatureCurveName
    if (boost::optional<model::Curve> curve = modelObject.coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofLowTemperatureCurveName,
                            _curve->nameString());
      }
    }

    // CoolingEnergyInputRatioBoundaryCurveName
    if (boost::optional<model::Curve> curve = modelObject.coolingEnergyInputRatioBoundaryCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioBoundaryCurveName, _curve->nameString());
      }
    }

    // CoolingEnergyInputRatioModifierFunctionofHighTemperatureCurveName
    if (boost::optional<model::Curve> curve = modelObject.coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofHighTemperatureCurveName,
                            _curve->nameString());
      }
    }

    // CoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName
    if (boost::optional<model::Curve> curve = modelObject.coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName,
                            _curve->nameString());
      }
    }

    // CoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName
    if (boost::optional<model::Curve> curve = modelObject.coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName,
                            _curve->nameString());
      }
    }

    // CoolingCombinationRatioCorrectionFactorCurveName
    if (boost::optional<model::Curve> curve = modelObject.coolingCombinationRatioCorrectionFactorCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingCombinationRatioCorrectionFactorCurveName, _curve->nameString());
      }
    }

    // CoolingPartLoadFractionCorrelationCurveName
    if (boost::optional<model::Curve> curve = modelObject.coolingPartLoadFractionCorrelationCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CoolingPartLoadFractionCorrelationCurveName, _curve->nameString());
      }
    }

    // RatedTotalHeatingCapacity
    if (modelObject.isGrossRatedHeatingCapacityAutosized()) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::GrossRatedHeatingCapacity, "Autosize");
    } else if ((value = modelObject.grossRatedHeatingCapacity())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::GrossRatedHeatingCapacity, value.get());
    }

    // RatedTotalHeatingCapacitySizingRatio
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::RatedHeatingCapacitySizingRatio, modelObject.ratedHeatingCapacitySizingRatio());

    // RatedHeatingCOP
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::GrossRatedHeatingCOP, modelObject.ratedHeatingCOP());

    // MinimumCondenserInletNodeTemperatureinHeatingMode
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MinimumCondenserInletNodeTemperatureinHeatingMode,
                        modelObject.minimumOutdoorTemperatureinHeatingMode());

    // MaximumCondenserInletNodeTemperatureinHeatingMode
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MaximumCondenserInletNodeTemperatureinHeatingMode,
                        modelObject.maximumOutdoorTemperatureinHeatingMode());

    // HeatingCapacityRatioModifierFunctionofLowTemperatureCurveName
    if (boost::optional<model::Curve> curve = modelObject.heatingCapacityRatioModifierFunctionofLowTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofLowTemperatureCurveName,
                            _curve->nameString());
      }
    }

    // HeatingCapacityRatioBoundaryCurveName
    if (boost::optional<model::Curve> curve = modelObject.heatingCapacityRatioBoundaryCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioBoundaryCurveName, _curve->nameString());
      }
    }

    // HeatingCapacityRatioModifierFunctionofHighTemperatureCurveName
    if (boost::optional<model::Curve> curve = modelObject.heatingCapacityRatioModifierFunctionofHighTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofHighTemperatureCurveName,
                            _curve->nameString());
      }
    }

    // HeatingEnergyInputRatioModifierFunctionofLowTemperatureCurveName
    if (boost::optional<model::Curve> curve = modelObject.heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofLowTemperatureCurveName,
                            _curve->nameString());
      }
    }

    // HeatingEnergyInputRatioBoundaryCurveName
    if (boost::optional<model::Curve> curve = modelObject.heatingEnergyInputRatioBoundaryCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioBoundaryCurveName, _curve->nameString());
      }
    }

    // HeatingEnergyInputRatioModifierFunctionofHighTemperatureCurveName
    if (boost::optional<model::Curve> curve = modelObject.heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofHighTemperatureCurveName,
                            _curve->nameString());
      }
    }

    // HeatingPerformanceCurveOutdoorTemperatureType
    if ((s = modelObject.heatingPerformanceCurveOutdoorTemperatureType())) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingPerformanceCurveOutdoorTemperatureType, s.get());
    }

    // HeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName
    if (boost::optional<model::Curve> curve = modelObject.heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName,
                            _curve->nameString());
      }
    }

    // HeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName
    if (boost::optional<model::Curve> curve = modelObject.heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName,
                            _curve->nameString());
      }
    }

    // HeatingCombinationRatioCorrectionFactorCurveName
    if (boost::optional<model::Curve> curve = modelObject.heatingCombinationRatioCorrectionFactorCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingCombinationRatioCorrectionFactorCurveName, _curve->nameString());
      }
    }

    // HeatingPartLoadFractionCorrelationCurveName
    if (boost::optional<model::Curve> curve = modelObject.heatingPartLoadFractionCorrelationCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatingPartLoadFractionCorrelationCurveName, _curve->nameString());
      }
    }

    // MinimumHeatPumpPartLoadRatio
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MinimumHeatPumpPartLoadRatio, modelObject.minimumHeatPumpPartLoadRatio());

    // ZoneNameforMasterThermostatLocation
    if (boost::optional<model::ThermalZone> zone = modelObject.zoneforMasterThermostatLocation()) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::ZoneNameforMasterThermostatLocation, zone->nameString());
    }

    // MasterThermostatPriorityControlType
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::MasterThermostatPriorityControlType,
                        modelObject.masterThermostatPriorityControlType());

    // ThermostatPriorityScheduleName
    if (boost::optional<model::Schedule> schedule = modelObject.thermostatPrioritySchedule()) {
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::ThermostatPriorityScheduleName, _schedule->nameString());
      }
    }

    // HeatPumpWasteHeatRecovery
    if (modelObject.heatPumpWasteHeatRecovery()) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatPumpWasteHeatRecovery, "Yes");
    } else {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatPumpWasteHeatRecovery, "No");
    }

    // EquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::EquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode,
                        modelObject.equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode());

    // VerticalHeightusedforPipingCorrectionFactor
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::VerticalHeightusedforPipingCorrectionFactor,
                        modelObject.verticalHeightusedforPipingCorrectionFactor());

    // PipingCorrectionFactorforLengthinCoolingModeCurveName
    if (boost::optional<model::Curve> curve = modelObject.pipingCorrectionFactorforLengthinCoolingModeCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforLengthinCoolingModeCurveName,
                            _curve->nameString());
      }
    }

    // PipingCorrectionFactorforHeightinCoolingModeCoefficient
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforHeightinCoolingModeCoefficient,
                        modelObject.pipingCorrectionFactorforHeightinCoolingModeCoefficient());

    // EquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::EquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode,
                        modelObject.equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode());

    // PipingCorrectionFactorforLengthinHeatingModeCurveName
    if (boost::optional<model::Curve> curve = modelObject.pipingCorrectionFactorforLengthinHeatingModeCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforLengthinHeatingModeCurveName,
                            _curve->nameString());
      }
    }

    // PipingCorrectionFactorforHeightinHeatingModeCoefficient
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforHeightinHeatingModeCoefficient,
                        modelObject.pipingCorrectionFactorforHeightinHeatingModeCoefficient());

    // CrankcaseHeaterPowerperCompressor
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::CrankcaseHeaterPowerperCompressor,
                        modelObject.crankcaseHeaterPowerperCompressor());

    // NumberofCompressors
    idfObject.setUnsigned(AirConditioner_VariableRefrigerantFlowFields::NumberofCompressors,
                          static_cast<unsigned>(modelObject.numberofCompressors()));

    // RatioofCompressorSizetoTotalCompressorCapacity
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::RatioofCompressorSizetoTotalCompressorCapacity,
                        modelObject.ratioofCompressorSizetoTotalCompressorCapacity());

    // MaximumOutdoorDrybulbTemperatureforCrankcaseHeater
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeater,
                        modelObject.maximumOutdoorDrybulbTemperatureforCrankcaseHeater());

    // DefrostStrategy
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::DefrostStrategy, modelObject.defrostStrategy());

    // DefrostControl
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::DefrostControl, modelObject.defrostControl());

    // DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName
    if (boost::optional<model::Curve> curve = modelObject.defrostEnergyInputRatioModifierFunctionofTemperatureCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName,
                            _curve->nameString());
      }
    }

    // DefrostTimePeriodFraction
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::DefrostTimePeriodFraction, modelObject.defrostTimePeriodFraction());

    // ResistiveDefrostHeaterCapacity

    if (modelObject.isResistiveDefrostHeaterCapacityAutosized()) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::ResistiveDefrostHeaterCapacity, "Autosize");
    } else if ((value = modelObject.resistiveDefrostHeaterCapacity())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::ResistiveDefrostHeaterCapacity, value.get());
    }

    // MaximumOutdoorDrybulbTemperatureforDefrostOperation
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation,
                        modelObject.maximumOutdoorDrybulbTemperatureforDefrostOperation());

    // Condenser Type
    // It was decided that the "smart" logic shouldn't be in the model itself.
    // So now, we do two things:
    // * If condenserType is set, respect that, but issue Errors if it's wrong
    // * If condenserType is not set, we default it (default is done in the model actually)
    //     * If VRF connected to a PlantLoop => WaterCooled, else "AirCooled" ("EvaporativelyCooled" is less common and will be reserved for people who
    //     know what they are doing and are hardsetting it)
    std::string condenserType = modelObject.condenserType();
    boost::optional<PlantLoop> plant_ = modelObject.plantLoop();

    if (modelObject.isCondenserTypeDefaulted()) {
      // We log an Info anyways, might be useful
      if (plant_.is_initialized()) {
        LOG(Info, modelObject.briefDescription() << " is connected to a PlantLoop, defaulting condenserType to 'WaterCooled'.");
        condenserType = "WaterCooled";
      } else {
        LOG(Info, modelObject.briefDescription() << " is not connected to a PlantLoop, defaulting condenserType to 'AirCooled'.");
        condenserType = "AirCooled";
      }
    } else {

      if ((openstudio::istringEqual(condenserType, "AirCooled") || openstudio::istringEqual(condenserType, "EvaporativelyCooled"))
          && plant_.is_initialized()) {
        LOG(Error,
            modelObject.briefDescription() << " has an hardcoded condenserType '" << condenserType << "' while it is connected to a PlantLoop.");
      } else if (openstudio::istringEqual(condenserType, "WaterCooled") && !plant_.is_initialized()) {
        LOG(Error,
            modelObject.briefDescription() << " has an hardcoded condenserType '" << condenserType << "' while it is NOT connected to a PlantLoop.");
      }
    }

    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CondenserType, condenserType);

    // CondenserInletNodeName
    if (boost::optional<ModelObject> mo = modelObject.inletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CondenserInletNodeName, node->nameString());
      }
    }
    // CondenserOutletNodeName
    if (boost::optional<ModelObject> mo = modelObject.outletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::CondenserOutletNodeName, node->nameString());
      }
    }

    // WaterCondenserVolumeFlowRate

    if (modelObject.isWaterCondenserVolumeFlowRateAutosized()) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::WaterCondenserVolumeFlowRate, "Autosize");
    } else if ((value = modelObject.waterCondenserVolumeFlowRate())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::WaterCondenserVolumeFlowRate, value.get());
    }

    // EvaporativeCondenserEffectiveness

    if ((value = modelObject.evaporativeCondenserEffectiveness())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserEffectiveness, value.get());
    }

    // EvaporativeCondenserAirFlowRate

    if (modelObject.isEvaporativeCondenserAirFlowRateAutosized()) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserAirFlowRate, "Autosize");
    } else if ((value = modelObject.evaporativeCondenserAirFlowRate())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserAirFlowRate, value.get());
    }

    // EvaporativeCondenserPumpRatedPowerConsumption

    if (modelObject.isEvaporativeCondenserPumpRatedPowerConsumptionAutosized()) {
      idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserPumpRatedPowerConsumption, "Autosize");
    } else if ((value = modelObject.evaporativeCondenserPumpRatedPowerConsumption())) {
      idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserPumpRatedPowerConsumption, value.get());
    }

    // BasinHeaterCapacity
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::BasinHeaterCapacity, modelObject.basinHeaterCapacity());

    // BasinHeaterSetpointTemperature
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::BasinHeaterSetpointTemperature, modelObject.basinHeaterSetpointTemperature());

    // BasinHeaterOperatingScheduleName

    if (boost::optional<model::Schedule> schedule = modelObject.basinHeaterOperatingSchedule()) {
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::BasinHeaterOperatingScheduleName, _schedule->nameString());
      }
    }

    // FuelType
    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::FuelType, modelObject.fuelType());

    // MinimumCondenserInletNodeTemperatureinHeatRecoveryMode
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MinimumCondenserInletNodeTemperatureinHeatRecoveryMode,
                        modelObject.minimumOutdoorTemperatureinHeatRecoveryMode());

    // MaximumCondenserInletNodeTemperatureinHeatRecoveryMode
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::MaximumCondenserInletNodeTemperatureinHeatRecoveryMode,
                        modelObject.maximumOutdoorTemperatureinHeatingMode());

    // HeatRecoveryCoolingCapacityModifierCurveName

    if (boost::optional<model::Curve> curve = modelObject.heatRecoveryCoolingCapacityModifierCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityModifierCurveName, _curve->nameString());
      }
    }

    // InitialHeatRecoveryCoolingCapacityFraction
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingCapacityFraction,
                        modelObject.initialHeatRecoveryCoolingEnergyFraction());

    // HeatRecoveryCoolingCapacityTimeConstant
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityTimeConstant,
                        modelObject.heatRecoveryCoolingCapacityTimeConstant());

    // HeatRecoveryCoolingEnergyModifierCurveName
    if (boost::optional<model::Curve> curve = modelObject.heatRecoveryCoolingEnergyModifierCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyModifierCurveName, _curve->nameString());
      }
    }

    // InitialHeatRecoveryCoolingEnergyFraction
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingEnergyFraction,
                        modelObject.initialHeatRecoveryCoolingEnergyFraction());

    // HeatRecoveryCoolingEnergyTimeConstant
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyTimeConstant,
                        modelObject.heatRecoveryCoolingEnergyTimeConstant());

    // HeatRecoveryHeatingCapacityModifierCurveName
    if (boost::optional<model::Curve> curve = modelObject.heatRecoveryHeatingCapacityModifierCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityModifierCurveName, _curve->nameString());
      }
    }

    // InitialHeatRecoveryHeatingCapacityFraction
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryHeatingCapacityFraction,
                        modelObject.initialHeatRecoveryHeatingCapacityFraction());

    // HeatRecoveryHeatingCapacityTimeConstant
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityTimeConstant,
                        modelObject.heatRecoveryHeatingCapacityTimeConstant());

    // HeatRecoveryHeatingEnergyModifierCurveName
    if (boost::optional<model::Curve> curve = modelObject.heatRecoveryHeatingEnergyModifierCurve()) {
      if (boost::optional<IdfObject> _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyModifierCurveName, _curve->nameString());
      }
    }

    // InitialHeatRecoveryHeatingEnergyFraction
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryHeatingEnergyFraction,
                        modelObject.initialHeatRecoveryHeatingEnergyFraction());

    // HeatRecoveryHeatingEnergyTimeConstant
    idfObject.setDouble(AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyTimeConstant,
                        modelObject.heatRecoveryHeatingEnergyTimeConstant());

    // Terminal Unit List

    IdfObject& zoneTerminalUnitList = m_idfObjects.emplace_back(IddObjectType::ZoneTerminalUnitList);

    std::string terminalUnitListName = modelObject.nameString() + " Terminal List";

    zoneTerminalUnitList.setString(ZoneTerminalUnitListFields::ZoneTerminalUnitListName, terminalUnitListName);

    idfObject.setString(AirConditioner_VariableRefrigerantFlowFields::ZoneTerminalUnitListName, terminalUnitListName);

    for (ZoneHVACTerminalUnitVariableRefrigerantFlow& terminal : modelObject.terminals()) {
      boost::optional<IdfObject> terminal_ = translateAndMapModelObject(terminal);
      OS_ASSERT(terminal_);
      IdfExtensibleGroup eg = zoneTerminalUnitList.pushExtensibleGroup();
      eg.setString(ZoneTerminalUnitListExtensibleFields::ZoneTerminalUnitName, terminal_->nameString());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
