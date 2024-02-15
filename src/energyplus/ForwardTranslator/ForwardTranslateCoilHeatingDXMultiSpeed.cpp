/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/CoilHeatingDXMultiSpeed.hpp"
#include "../../model/CoilHeatingDXMultiSpeedStageData.hpp"
#include "../../model/Node.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Curve.hpp"
#include <utilities/idd/Coil_Heating_DX_MultiSpeed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilHeatingDXMultiSpeed(CoilHeatingDXMultiSpeed& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Coil_Heating_DX_MultiSpeed, modelObject);

    // AvailabilityScheduleName
    if (auto schedule = modelObject.availabilitySchedule()) {
      if (auto _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(Coil_Heating_DX_MultiSpeedFields::AvailabilityScheduleName, _schedule->name().get());
      }
    }

    // AirInletNodeName
    if (auto node = modelObject.inletModelObject()) {
      idfObject.setString(Coil_Heating_DX_MultiSpeedFields::AirInletNodeName, node->name().get());
    }

    // AirOutletNodeName
    if (auto node = modelObject.outletModelObject()) {
      idfObject.setString(Coil_Heating_DX_MultiSpeedFields::AirOutletNodeName, node->name().get());
    }

    // MinimumOutdoorDryBulbTemperatureforCompressorOperation
    idfObject.setDouble(Coil_Heating_DX_MultiSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation,
                        modelObject.minimumOutdoorDryBulbTemperatureforCompressorOperation());

    // OutdoorDryBulbTemperaturetoTurnOnCompressor
    if ((value = modelObject.outdoorDryBulbTemperaturetoTurnOnCompressor())) {
      idfObject.setDouble(Coil_Heating_DX_MultiSpeedFields::OutdoorDryBulbTemperaturetoTurnOnCompressor, value.get());
    }

    // CrankcaseHeaterCapacity
    idfObject.setDouble(Coil_Heating_DX_MultiSpeedFields::CrankcaseHeaterCapacity, modelObject.crankcaseHeaterCapacity());

    // CrankcaseHeaterCapacityFunctionofTemperatureCurveName
    if (auto crankCurve_ = modelObject.crankcaseHeaterCapacityFunctionofTemperatureCurve()) {
      if (auto curve_ = translateAndMapModelObject(*crankCurve_)) {
        idfObject.setString(Coil_Heating_DX_MultiSpeedFields::CrankcaseHeaterCapacityFunctionofTemperatureCurveName, curve_->nameString());
      }
    }

    // MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation
    idfObject.setDouble(Coil_Heating_DX_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation,
                        modelObject.maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation());

    // DefrostEnergyInputRatioFunctionofTemperatureCurveName
    if (auto curve = modelObject.defrostEnergyInputRatioFunctionofTemperatureCurve()) {
      if (auto _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(Coil_Heating_DX_MultiSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName, _curve->name().get());
      }
    }

    // MaximumOutdoorDryBulbTemperatureforDefrostOperation
    idfObject.setDouble(Coil_Heating_DX_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforDefrostOperation,
                        modelObject.maximumOutdoorDryBulbTemperatureforDefrostOperation());

    // DefrostStrategy
    idfObject.setString(Coil_Heating_DX_MultiSpeedFields::DefrostStrategy, modelObject.defrostStrategy());

    // DefrostControl
    idfObject.setString(Coil_Heating_DX_MultiSpeedFields::DefrostControl, modelObject.defrostControl());

    // DefrostTimePeriodFraction
    idfObject.setDouble(Coil_Heating_DX_MultiSpeedFields::DefrostTimePeriodFraction, modelObject.defrostTimePeriodFraction());

    // ResistiveDefrostHeaterCapacity
    if (modelObject.isResistiveDefrostHeaterCapacityAutosized()) {
      idfObject.setString(Coil_Heating_DX_MultiSpeedFields::ResistiveDefrostHeaterCapacity, "AutoSize");
    } else if ((value = modelObject.resistiveDefrostHeaterCapacity())) {
      idfObject.setDouble(Coil_Heating_DX_MultiSpeedFields::ResistiveDefrostHeaterCapacity, value.get());
    }

    // ApplyPartLoadFractiontoSpeedsGreaterthan1
    if (modelObject.applyPartLoadFractiontoSpeedsGreaterthan1()) {
      idfObject.setString(Coil_Heating_DX_MultiSpeedFields::ApplyPartLoadFractiontoSpeedsGreaterthan1, "Yes");
    } else {
      idfObject.setString(Coil_Heating_DX_MultiSpeedFields::ApplyPartLoadFractiontoSpeedsGreaterthan1, "No");
    }

    // FuelType
    idfObject.setString(Coil_Heating_DX_MultiSpeedFields::FuelType, modelObject.fuelType());

    // RegionnumberforCalculatingHSPF
    if (auto num = modelObject.regionnumberforCalculatingHSPF()) {
      idfObject.setInt(Coil_Heating_DX_MultiSpeedFields::RegionnumberforCalculatingHSPF, num);
    }

    const auto stages = modelObject.stages();

    // NumberofSpeeds
    if (!stages.empty()) {
      idfObject.setInt(Coil_Heating_DX_MultiSpeedFields::NumberofSpeeds, stages.size());
    }

    for (const auto& stage : stages) {
      auto eg = idfObject.pushExtensibleGroup();

      // GrossRatedHeatingCapacity
      if (stage.isGrossRatedHeatingCapacityAutosized()) {
        eg.setString(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedGrossRatedHeatingCapacity, "AutoSize");
      } else if ((value = stage.grossRatedHeatingCapacity())) {
        eg.setDouble(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedGrossRatedHeatingCapacity, value.get());
      }

      // GrossRatedHeatingCOP
      eg.setDouble(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedGrossRatedHeatingCOP, stage.grossRatedHeatingCOP());

      // RatedAirFlowRate
      if (stage.isRatedAirFlowRateAutosized()) {
        eg.setString(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedRatedAirFlowRate, "AutoSize");
      } else if ((value = stage.ratedAirFlowRate())) {
        eg.setDouble(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedRatedAirFlowRate, value.get());
      }

      // RatedSupplyAirFanPowerPerVolumeFlowRate2017
      eg.setDouble(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedRatedSupplyAirFanPowerPerVolumeFlowRate2017,
                   stage.ratedSupplyAirFanPowerPerVolumeFlowRate2017());

      // RatedSupplyAirFanPowerPerVolumeFlowRate2023
      eg.setDouble(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedRatedSupplyAirFanPowerPerVolumeFlowRate2023,
                   stage.ratedSupplyAirFanPowerPerVolumeFlowRate2023());

      // HeatingCapacityFunctionofTemperatureCurveName
      {
        auto curve = stage.heatingCapacityFunctionofTemperatureCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedHeatingCapacityFunctionofTemperatureCurveName, _curve->name().get());
        }
      }

      // HeatingCapacityFunctionofFlowFractionCurveName
      {
        auto curve = stage.heatingCapacityFunctionofFlowFractionCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedHeatingCapacityFunctionofFlowFractionCurveName, _curve->name().get());
        }
      }

      // SpeedEnergyInputRatioFunctionofTemperatureCurveName
      {
        auto curve = stage.energyInputRatioFunctionofTemperatureCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedEnergyInputRatioFunctionofTemperatureCurveName, _curve->name().get());
        }
      }

      // SpeedEnergyInputRatioFunctionofFlowFractionCurveName
      {
        auto curve = stage.energyInputRatioFunctionofFlowFractionCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedEnergyInputRatioFunctionofFlowFractionCurveName, _curve->name().get());
        }
      }

      // SpeedPartLoadFractionCorrelationCurveName
      {
        auto curve = stage.partLoadFractionCorrelationCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedPartLoadFractionCorrelationCurveName, _curve->name().get());
        }
      }

      // SpeedRatedWasteHeatFractionofPowerInput
      eg.setDouble(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedRatedWasteHeatFractionofPowerInput, stage.ratedWasteHeatFractionofPowerInput());

      // SpeedWasteHeatFunctionofTemperatureCurveName
      {
        auto curve = stage.wasteHeatFunctionofTemperatureCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedWasteHeatFunctionofTemperatureCurveName, _curve->name().get());
        }
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
