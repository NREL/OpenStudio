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
    if ((value = modelObject.minimumOutdoorDryBulbTemperatureforCompressorOperation())) {
      idfObject.setDouble(Coil_Heating_DX_MultiSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation, value.get());
    }

    // OutdoorDryBulbTemperaturetoTurnOnCompressor
    if ((value = modelObject.outdoorDryBulbTemperaturetoTurnOnCompressor())) {
      idfObject.setDouble(Coil_Heating_DX_MultiSpeedFields::OutdoorDryBulbTemperaturetoTurnOnCompressor, value.get());
    }

    // CrankcaseHeaterCapacity
    if ((value = modelObject.crankcaseHeaterCapacity())) {
      idfObject.setDouble(Coil_Heating_DX_MultiSpeedFields::CrankcaseHeaterCapacity, value.get());
    }

    // MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation
    if ((value = modelObject.maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation())) {
      idfObject.setDouble(Coil_Heating_DX_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation, value.get());
    }

    // DefrostEnergyInputRatioFunctionofTemperatureCurveName
    if (auto curve = modelObject.defrostEnergyInputRatioFunctionofTemperatureCurve()) {
      if (auto _curve = translateAndMapModelObject(curve.get())) {
        idfObject.setString(Coil_Heating_DX_MultiSpeedFields::DefrostEnergyInputRatioFunctionofTemperatureCurveName, _curve->name().get());
      }
    }

    // MaximumOutdoorDryBulbTemperatureforDefrostOperation
    if ((value = modelObject.maximumOutdoorDryBulbTemperatureforDefrostOperation())) {
      idfObject.setDouble(Coil_Heating_DX_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforDefrostOperation, value.get());
    }

    // DefrostStrategy
    if ((s = modelObject.defrostStrategy())) {
      idfObject.setString(Coil_Heating_DX_MultiSpeedFields::DefrostStrategy, s.get());
    }

    // DefrostControl
    if ((s = modelObject.defrostControl())) {
      idfObject.setString(Coil_Heating_DX_MultiSpeedFields::DefrostControl, s.get());
    }

    // DefrostTimePeriodFraction
    if ((value = modelObject.defrostTimePeriodFraction())) {
      idfObject.setDouble(Coil_Heating_DX_MultiSpeedFields::DefrostTimePeriodFraction, value.get());
    }

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
    if ((s = modelObject.fuelType())) {
      idfObject.setString(Coil_Heating_DX_MultiSpeedFields::FuelType, s.get());
    }

    // RegionnumberforCalculatingHSPF
    if (auto num = modelObject.regionnumberforCalculatingHSPF()) {
      idfObject.setInt(Coil_Heating_DX_MultiSpeedFields::RegionnumberforCalculatingHSPF, num);
    }

    auto const stages = modelObject.stages();

    // NumberofSpeeds
    if (auto num = stages.size()) {
      idfObject.setInt(Coil_Heating_DX_MultiSpeedFields::NumberofSpeeds, num);
    }

    for (auto const& stage : stages) {
      auto eg = idfObject.pushExtensibleGroup();

      // GrossRatedHeatingCapacity
      if (stage.isGrossRatedHeatingCapacityAutosized()) {
        eg.setString(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedGrossRatedHeatingCapacity, "AutoSize");
      } else if ((value = stage.grossRatedHeatingCapacity())) {
        eg.setDouble(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedGrossRatedHeatingCapacity, value.get());
      }

      // GrossRatedHeatingCOP
      if ((value = stage.grossRatedHeatingCOP())) {
        eg.setDouble(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedGrossRatedHeatingCOP, value.get());
      }

      // RatedAirFlowRate
      if (stage.isRatedAirFlowRateAutosized()) {
        eg.setString(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedRatedAirFlowRate, "AutoSize");
      } else if ((value = stage.ratedAirFlowRate())) {
        eg.setDouble(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedRatedAirFlowRate, value.get());
      }

      // RatedSupplyAirFanPowerPerVolumeFlowRate
      if ((value = stage.ratedSupplyAirFanPowerPerVolumeFlowRate())) {
        eg.setDouble(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedRatedSupplyAirFanPowerPerVolumeFlowRate, value.get());
      }

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
      if ((value = stage.ratedWasteHeatFractionofPowerInput())) {
        eg.setDouble(Coil_Heating_DX_MultiSpeedExtensibleFields::SpeedRatedWasteHeatFractionofPowerInput, value.get());
      }

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
