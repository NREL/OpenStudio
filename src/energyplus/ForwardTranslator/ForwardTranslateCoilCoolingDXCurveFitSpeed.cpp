/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    // RatedEvaporatorFanPowerPerVolumeFlowRate2017
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_SpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate2017,
                        modelObject.ratedEvaporatorFanPowerPerVolumeFlowRate2017());

    // RatedEvaporatorFanPowerPerVolumeFlowRate2023
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_SpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate2023,
                        modelObject.ratedEvaporatorFanPowerPerVolumeFlowRate2023());

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
