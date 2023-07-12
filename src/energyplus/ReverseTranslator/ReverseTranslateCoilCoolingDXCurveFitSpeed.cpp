/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/CoilCoolingDXCurveFitSpeed.hpp"
#include "../../model/CoilCoolingDXCurveFitSpeed_Impl.hpp"

#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"

#include <utilities/idd/Coil_Cooling_DX_CurveFit_Speed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateCoilCoolingDXCurveFitSpeed(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Coil_Cooling_DX_CurveFit_Speed) {
      LOG(Error, "WorkspaceObject is not IddObjectType: CoilCoolingDXCurveFitSpeed");
      return boost::none;
    }

    OptionalString s;
    OptionalDouble d;
    OptionalWorkspaceObject target;

    openstudio::model::CoilCoolingDXCurveFitSpeed speed(m_model);

    if ((target =
           workspaceObject.getTarget(openstudio::Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofTemperatureCurveName))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto optCurve = modelObject->optionalCast<Curve>()) {
          speed.setTotalCoolingCapacityModifierFunctionofTemperatureCurve(optCurve.get());
        }
      }
    }

    if ((target = workspaceObject.getTarget(
           openstudio::Coil_Cooling_DX_CurveFit_SpeedFields::TotalCoolingCapacityModifierFunctionofAirFlowFractionCurveName))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto optCurve = modelObject->optionalCast<Curve>()) {
          speed.setTotalCoolingCapacityModifierFunctionofAirFlowFractionCurve(optCurve.get());
        }
      }
    }

    if ((target =
           workspaceObject.getTarget(openstudio::Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofTemperatureCurveName))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto optCurve = modelObject->optionalCast<Curve>()) {
          speed.setEnergyInputRatioModifierFunctionofTemperatureCurve(optCurve.get());
        }
      }
    }

    if ((target =
           workspaceObject.getTarget(openstudio::Coil_Cooling_DX_CurveFit_SpeedFields::EnergyInputRatioModifierFunctionofAirFlowFractionCurveName))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto optCurve = modelObject->optionalCast<Curve>()) {
          speed.setEnergyInputRatioModifierFunctionofAirFlowFractionCurve(optCurve.get());
        }
      }
    }

    if ((target = workspaceObject.getTarget(openstudio::Coil_Cooling_DX_CurveFit_SpeedFields::PartLoadFractionCorrelationCurveName))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto optCurve = modelObject->optionalCast<Curve>()) {
          speed.setPartLoadFractionCorrelationCurve(optCurve.get());
        }
      }
    }

    if ((target = workspaceObject.getTarget(openstudio::Coil_Cooling_DX_CurveFit_SpeedFields::WasteHeatModifierFunctionofTemperatureCurveName))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto optCurve = modelObject->optionalCast<Curve>()) {
          speed.setWasteHeatModifierFunctionofTemperatureCurve(optCurve.get());
        }
      }
    }

    s = workspaceObject.name();
    if (s) {
      speed.setName(*s);
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_SpeedFields::GrossTotalCoolingCapacityFraction);
    if (d) {
      speed.setGrossTotalCoolingCapacityFraction(*d);
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_SpeedFields::EvaporatorAirFlowRateFraction);
    if (d) {
      speed.setEvaporatorAirFlowRateFraction(*d);
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_SpeedFields::CondenserAirFlowRateFraction);
    if (d) {
      speed.setCondenserAirFlowRateFraction(*d);
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_SpeedFields::GrossSensibleHeatRatio);
    if (d) {
      speed.setGrossSensibleHeatRatio(*d);
    } else {
      speed.autosizeGrossSensibleHeatRatio();
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_SpeedFields::GrossCoolingCOP);
    if (d) {
      speed.setGrossCoolingCOP(*d);
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_SpeedFields::ActiveFractionofCoilFaceArea);
    if (d) {
      speed.setActiveFractionofCoilFaceArea(*d);
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_SpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate2017);
    if (d) {
      speed.setRatedEvaporatorFanPowerPerVolumeFlowRate2017(*d);
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_SpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate2023);
    if (d) {
      speed.setRatedEvaporatorFanPowerPerVolumeFlowRate2023(*d);
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_SpeedFields::EvaporativeCondenserPumpPowerFraction);
    if (d) {
      speed.setEvaporativeCondenserPumpPowerFraction(*d);
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_SpeedFields::EvaporativeCondenserEffectiveness);
    if (d) {
      speed.setEvaporativeCondenserEffectiveness(*d);
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_SpeedFields::RatedWasteHeatFractionofPowerInput);
    if (d) {
      speed.setRatedWasteHeatFractionofPowerInput(*d);
    }

    if ((target =
           workspaceObject.getTarget(openstudio::Coil_Cooling_DX_CurveFit_SpeedFields::SensibleHeatRatioModifierFunctionofTemperatureCurveName))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject && modelObject->optionalCast<Curve>()) {
        speed.setSensibleHeatRatioModifierFunctionofTemperatureCurve(modelObject->cast<Curve>());
      }
    }

    if ((target =
           workspaceObject.getTarget(openstudio::Coil_Cooling_DX_CurveFit_SpeedFields::SensibleHeatRatioModifierFunctionofFlowFractionCurveName))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject && modelObject->optionalCast<Curve>()) {
        speed.setSensibleHeatRatioModifierFunctionofFlowFractionCurve(modelObject->cast<Curve>());
      }
    }

    return speed;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
