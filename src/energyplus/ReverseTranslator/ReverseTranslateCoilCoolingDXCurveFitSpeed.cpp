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

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_SpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate);
    if (d) {
      speed.setRatedEvaporatorFanPowerPerVolumeFlowRate(*d);
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
