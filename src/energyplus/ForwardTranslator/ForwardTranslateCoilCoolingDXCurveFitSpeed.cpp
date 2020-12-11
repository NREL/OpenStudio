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

#include "../../model/CoilCoolingDXCurveFitSpeed.hpp"

#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"

#include <utilities/idd/Coil_Cooling_DX_CurveFit_Speed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingDXCurveFitSpeed(CoilCoolingDXCurveFitSpeed& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(openstudio::IddObjectType::Coil_Cooling_DX_CurveFit_Speed);

    m_idfObjects.push_back(idfObject);

    // Name
    idfObject.setString(Coil_Cooling_DX_CurveFit_SpeedFields::Name, modelObject.name().get());

    // GrossTotalCoolingCapacityFunction
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_SpeedFields::GrossTotalCoolingCapacityFraction, modelObject.grossTotalCoolingCapacityFraction());

    // EvaporatorAirFlowRateFraction
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_SpeedFields::EvaporatorAirFlowRateFraction, modelObject.evaporatorAirFlowRateFraction());

    // CondenserAirFlowRateFraction: (Unused if parent operatingMode's condenserType = "AirCooled")
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_SpeedFields::CondenserAirFlowRateFraction, modelObject.condenserAirFlowRateFraction());

    // GrossSensibleHeatRatio
    value = modelObject.grossSensibleHeatRatio();
    if (value) {
      idfObject.setDouble(Coil_Cooling_DX_CurveFit_SpeedFields::GrossSensibleHeatRatio, value.get());
    } else {
      idfObject.setString(Coil_Cooling_DX_CurveFit_SpeedFields::GrossSensibleHeatRatio, "Autosize");
    }

    // GrossCoolingCOP
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_SpeedFields::GrossCoolingCOP, modelObject.grossCoolingCOP());

    // ActiveFractionofCoilFaceArea
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_SpeedFields::ActiveFractionofCoilFaceArea, modelObject.activeFractionofCoilFaceArea());

    // RatedEvaporatorFanPowerPerVolumeFlowRate
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_SpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate,
                        modelObject.ratedEvaporatorFanPowerPerVolumeFlowRate());

    // EvaporativeCondenserPumpPowerFraction
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_SpeedFields::EvaporativeCondenserPumpPowerFraction,
                        modelObject.evaporativeCondenserPumpPowerFraction());

    // EvaporativeCondenserEffectiveness
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_SpeedFields::EvaporativeCondenserEffectiveness, modelObject.evaporativeCondenserEffectiveness());

    // TotalCoolingCapacityModifierFunctionofTemperatureCurveName
    if (auto optCurve = modelObject.totalCoolingCapacityModifierFunctionofTemperatureCurve()) {
      if (auto _curve = translateAndMapModelObject(optCurve.get())) {
        idfObject.setString(Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofTemperatureCurveName, _curve->name().get());
      }
    }

    // TotalCoolingCapacityModifierFunctionofAirFlowFractionCurveName
    if (auto optCurve = modelObject.totalCoolingCapacityModifierFunctionofAirFlowFractionCurve()) {
      if (auto _curve = translateAndMapModelObject(optCurve.get())) {
        idfObject.setString(Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofAirFlowFractionCurveName,
                            _curve->name().get());
      }
    }

    // EnergyInputRatioModifierFunctionofTemperatureCurveName
    if (auto optCurve = modelObject.energyInputRatioModifierFunctionofTemperatureCurve()) {
      if (auto _curve = translateAndMapModelObject(optCurve.get())) {
        idfObject.setString(Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofTemperatureCurveName, _curve->name().get());
      }
    }

    // EnergyInputRatioModifierFunctionofAirFlowFractionCurveName
    if (auto optCurve = modelObject.energyInputRatioModifierFunctionofAirFlowFractionCurve()) {
      if (auto _curve = translateAndMapModelObject(optCurve.get())) {
        idfObject.setString(Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofAirFlowFractionCurveName, _curve->name().get());
      }
    }

    // PartLoadFractionCorrelationCurveName
    if (auto optCurve = modelObject.partLoadFractionCorrelationCurve()) {
      if (auto _curve = translateAndMapModelObject(optCurve.get())) {
        idfObject.setString(Coil_Cooling_DX_CurveFit_SpeedFields::PartLoadFractionCorrelationCurveName, _curve->name().get());
      }
    }

    // WasteHeatModifierFunctionofTemperatureCurveName
    if (auto optCurve = modelObject.wasteHeatModifierFunctionofTemperatureCurve()) {
      if (auto _curve = translateAndMapModelObject(optCurve.get())) {
        idfObject.setString(Coil_Cooling_DX_CurveFit_SpeedFields::WasteHeatModifierFunctionofTemperatureCurveName, _curve->name().get());
      }
    }

    // RatedWasteHeatFractionofPowerInput
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_SpeedFields::RatedWasteHeatFractionofPowerInput, modelObject.ratedWasteHeatFractionofPowerInput());

    // SensibleHeatRatioModifierFunctionofTemperatureCurveName
    if (auto optCurve = modelObject.sensibleHeatRatioModifierFunctionofTemperatureCurve()) {
      if (auto _curve = translateAndMapModelObject(optCurve.get())) {
        idfObject.setString(Coil_Cooling_DX_CurveFit_SpeedFields::SensibleHeatRatioModifierFunctionofTemperatureCurveName, _curve->name().get());
      }
    }

    // SensibleHeatRatioModifierFunctionofFlowFractionCurveName
    if (auto optCurve = modelObject.sensibleHeatRatioModifierFunctionofFlowFractionCurve()) {
      if (auto _curve = translateAndMapModelObject(optCurve.get())) {
        idfObject.setString(Coil_Cooling_DX_CurveFit_SpeedFields::SensibleHeatRatioModifierFunctionofFlowFractionCurveName, _curve->name().get());
      }
    }

    return boost::optional<IdfObject>(idfObject);
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
