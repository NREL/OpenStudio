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
#include "../../model/CoilCoolingDXMultiSpeed.hpp"
#include "../../model/CoilCoolingDXMultiSpeedStageData.hpp"
#include "../../model/Node.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Curve.hpp"
#include <utilities/idd/Coil_Cooling_DX_MultiSpeed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingDXMultiSpeed(CoilCoolingDXMultiSpeed& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Coil_Cooling_DX_MultiSpeed, modelObject);

    // AvailabilityScheduleName
    if (auto schedule = modelObject.availabilitySchedule()) {
      if (auto _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(Coil_Cooling_DX_MultiSpeedFields::AvailabilityScheduleName, _schedule->name().get());
      }
    }

    // AirInletNodeName
    if (auto node = modelObject.inletModelObject()) {
      idfObject.setString(Coil_Cooling_DX_MultiSpeedFields::AirInletNodeName, node->name().get());
    }

    // AirOutletNodeName
    if (auto node = modelObject.outletModelObject()) {
      idfObject.setString(Coil_Cooling_DX_MultiSpeedFields::AirOutletNodeName, node->name().get());
    }

    // CondenserAirInletNodeName: stays blank so we get condition from weather file

    // CondenserType
    idfObject.setString(Coil_Cooling_DX_MultiSpeedFields::CondenserType, modelObject.condenserType());

    // SupplyWaterStorageTankName

    // CondensateCollectionWaterStorageTankName

    // ApplyPartLoadFractiontoSpeedsGreaterthan1

    if (modelObject.applyPartLoadFractiontoSpeedsGreaterthan1()) {
      idfObject.setString(Coil_Cooling_DX_MultiSpeedFields::ApplyPartLoadFractiontoSpeedsGreaterthan1, "Yes");
    } else {
      idfObject.setString(Coil_Cooling_DX_MultiSpeedFields::ApplyPartLoadFractiontoSpeedsGreaterthan1, "No");
    }

    // ApplyLatentDegradationtoSpeedsGreaterthan1
    if (modelObject.applyLatentDegradationtoSpeedsGreaterthan1()) {
      idfObject.setString(Coil_Cooling_DX_MultiSpeedFields::ApplyLatentDegradationtoSpeedsGreaterthan1, "Yes");
    } else {
      idfObject.setString(Coil_Cooling_DX_MultiSpeedFields::ApplyLatentDegradationtoSpeedsGreaterthan1, "No");
    }

    // Minimum Outdoor Dry-Bulb Temperature for Compressor Operation
    idfObject.setDouble(Coil_Cooling_DX_MultiSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation,
                        modelObject.minimumOutdoorDryBulbTemperatureforCompressorOperation());

    // CrankcaseHeaterCapacity
    idfObject.setDouble(Coil_Cooling_DX_MultiSpeedFields::CrankcaseHeaterCapacity, modelObject.crankcaseHeaterCapacity());

    // MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation
    idfObject.setDouble(Coil_Cooling_DX_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation,
                        modelObject.maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation());

    // BasinHeaterCapacity
    idfObject.setDouble(Coil_Cooling_DX_MultiSpeedFields::BasinHeaterCapacity, modelObject.basinHeaterCapacity());

    // BasinHeaterSetpointTemperature
    idfObject.setDouble(Coil_Cooling_DX_MultiSpeedFields::BasinHeaterSetpointTemperature, modelObject.basinHeaterSetpointTemperature());

    // BasinHeaterOperatingScheduleName
    if (auto schedule = modelObject.basinHeaterOperatingSchedule()) {
      if (auto _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(Coil_Cooling_DX_MultiSpeedFields::BasinHeaterOperatingScheduleName, _schedule->name().get());
      }
    }

    // FuelType
    idfObject.setString(Coil_Cooling_DX_MultiSpeedFields::FuelType, modelObject.fuelType());

    const auto stages = modelObject.stages();

    // NumberofSpeeds
    if (!stages.empty()) {
      idfObject.setInt(Coil_Cooling_DX_MultiSpeedFields::NumberofSpeeds, stages.size());
    }

    for (const auto& stage : stages) {
      auto eg = idfObject.pushExtensibleGroup();

      // SpeedGrossRatedTotalCoolingCapacity
      if (stage.isGrossRatedTotalCoolingCapacityAutosized()) {
        eg.setString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedGrossRatedTotalCoolingCapacity, "AutoSize");
      } else if ((value = stage.grossRatedTotalCoolingCapacity())) {
        eg.setDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedGrossRatedTotalCoolingCapacity, value.get());
      }

      // SpeedGrossRatedSensibleHeatRatio
      if (stage.isGrossRatedSensibleHeatRatioAutosized()) {
        eg.setString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedGrossRatedSensibleHeatRatio, "AutoSize");
      } else if ((value = stage.grossRatedSensibleHeatRatio())) {
        eg.setDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedGrossRatedSensibleHeatRatio, value.get());
      }

      // SpeedGrossRatedCoolingCOP
      eg.setDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedGrossRatedCoolingCOP, tage.grossRatedCoolingCOP());

      // SpeedRatedAirFlowRate
      if (stage.isRatedAirFlowRateAutosized()) {
        eg.setString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedRatedAirFlowRate, "AutoSize");
      } else if ((value = stage.ratedAirFlowRate())) {
        eg.setDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedRatedAirFlowRate, value.get());
      }

      // RatedEvaporatorFanPowerPerVolumeFlowRate2017
      eg.setDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedRatedEvaporatorFanPowerPerVolumeFlowRate2017,
                   stage.ratedEvaporatorFanPowerPerVolumeFlowRate2017());

      // RatedEvaporatorFanPowerPerVolumeFlowRate2023
      eg.setDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedRatedEvaporatorFanPowerPerVolumeFlowRate2023,
                   stage.ratedEvaporatorFanPowerPerVolumeFlowRate2023());

      // SpeedTotalCoolingCapacityFunctionofTemperatureCurveName
      {
        auto curve = stage.totalCoolingCapacityFunctionofTemperatureCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedTotalCoolingCapacityFunctionofTemperatureCurveName, _curve->name().get());
        }
      }

      // SpeedTotalCoolingCapacityFunctionofFlowFractionCurveName
      {
        auto curve = stage.totalCoolingCapacityFunctionofFlowFractionCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedTotalCoolingCapacityFunctionofFlowFractionCurveName, _curve->name().get());
        }
      }

      // SpeedEnergyInputRatioFunctionofTemperatureCurveName
      {
        auto curve = stage.energyInputRatioFunctionofTemperatureCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedEnergyInputRatioFunctionofTemperatureCurveName, _curve->name().get());
        }
      }

      // SpeedEnergyInputRatioFunctionofFlowFractionCurveName
      {
        auto curve = stage.energyInputRatioFunctionofFlowFractionCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedEnergyInputRatioFunctionofFlowFractionCurveName, _curve->name().get());
        }
      }

      // SpeedPartLoadFractionCorrelationCurveName
      {
        auto curve = stage.partLoadFractionCorrelationCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedPartLoadFractionCorrelationCurveName, _curve->name().get());
        }
      }

      // SpeedNominalTimeforCondensateRemovaltoBegin
      if ((value = stage.nominalTimeforCondensateRemovaltoBegin())) {
        eg.setDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedNominalTimeforCondensateRemovaltoBegin, value.get());
      }

      // SpeedRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity
      eg.setDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedRatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity,
                   stage.ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity());

      // SpeedMaximumCyclingRate
      eg.setDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedMaximumCyclingRate, stage.maximumCyclingRate());

      // SpeedLatentCapacityTimeConstant
      eg.setDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedLatentCapacityTimeConstant, stage.latentCapacityTimeConstant());

      // SpeedRatedWasteHeatFractionofPowerInput
      eg.setDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedRatedWasteHeatFractionofPowerInput, stage.ratedWasteHeatFractionofPowerInput());

      // SpeedWasteHeatFunctionofTemperatureCurveName
      {
        auto curve = stage.wasteHeatFunctionofTemperatureCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedWasteHeatFunctionofTemperatureCurveName, _curve->name().get());
        }
      }

      // SpeedEvaporativeCondenserEffectiveness
      eg.setDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedEvaporativeCondenserEffectiveness, stage.evaporativeCondenserEffectiveness());

      // SpeedEvaporativeCondenserAirFlowRate
      if (stage.isEvaporativeCondenserAirFlowRateAutosized()) {
        eg.setString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedEvaporativeCondenserAirFlowRate, "AutoSize");
      } else if ((value = stage.evaporativeCondenserAirFlowRate())) {
        eg.setDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedEvaporativeCondenserAirFlowRate, value.get());
      }

      // SpeedRatedEvaporativeCondenserPumpPowerConsumption
      if (stage.isRatedEvaporativeCondenserPumpPowerConsumptionAutosized()) {
        eg.setString(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedRatedEvaporativeCondenserPumpPowerConsumption, "AutoSize");
      } else if ((value = stage.ratedEvaporativeCondenserPumpPowerConsumption())) {
        eg.setDouble(Coil_Cooling_DX_MultiSpeedExtensibleFields::SpeedRatedEvaporativeCondenserPumpPowerConsumption, value.get());
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
