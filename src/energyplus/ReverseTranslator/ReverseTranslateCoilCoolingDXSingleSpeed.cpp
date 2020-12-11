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

#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"

#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateCoilCoolingDXSingleSpeed(const WorkspaceObject& workspaceObject) {
    OptionalModelObject result, temp;
    OptionalSchedule schedule;

    //get the Schedule
    OptionalWorkspaceObject owo = workspaceObject.getTarget(Coil_Cooling_DX_SingleSpeedFields::AvailabilityScheduleName);
    if (!owo) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " Can't find Schedule.");
      return result;
    }
    temp = translateAndMapWorkspaceObject(*owo);
    if (temp) {
      schedule = temp->optionalCast<Schedule>();
    }

    if (!schedule) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription()
                                            << "Failed to convert iddObject (schedule) into ModelObject. Maybe it does not exist in model yet");
      return result;
    }

    //collect the curves
    owo = workspaceObject.getTarget(Coil_Cooling_DX_SingleSpeedFields::TotalCoolingCapacityFunctionofTemperatureCurveName);
    if (!owo) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " Can't find TotalCoolingCapacityFunctionOfTemperatureCurve.");
      return result;
    }
    if (owo->numSources() > 1) {
      owo = owo->workspace().addObject(owo.get());
    }

    temp = translateAndMapWorkspaceObject(*owo);

    if (!temp) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " Can't convert workspace curve into a model curve. ");
      return result;
    }
    boost::optional<Curve> tccfot = temp->optionalCast<Curve>();
    if (!tccfot) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " curve is wrong type. ");
      return result;
    }

    owo = workspaceObject.getTarget(Coil_Cooling_DX_SingleSpeedFields::EnergyInputRatioFunctionofTemperatureCurveName);
    if (!owo) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " Can't find EnergyInputRatioFunctionofTemperatureCurveName.");
      return result;
    }
    if (owo->numSources() > 1) {
      owo = owo->workspace().addObject(owo.get());
    }
    temp = translateAndMapWorkspaceObject(*owo);
    if (!temp) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " Can't convert workspace curve into a model curve. ");
      return result;
    }
    boost::optional<Curve> eirfot = temp->optionalCast<Curve>();
    if (!eirfot) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " curve is wrong type. ");
      return result;
    }

    owo = workspaceObject.getTarget(Coil_Cooling_DX_SingleSpeedFields::TotalCoolingCapacityFunctionofFlowFractionCurveName);
    if (!owo) {
      LOG(Error,
          "Error importing object: " << workspaceObject.briefDescription() << " Can't find TotalCoolingCapacityFunctionofFlowFractionCurveName.");
      return result;
    }
    if (owo->numSources() > 1) {
      owo = owo->workspace().addObject(owo.get());
    }
    temp = translateAndMapWorkspaceObject(*owo);
    if (!temp) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " Can't convert workspace curve into a model curve. ");
      return result;
    }
    boost::optional<Curve> tccfoff = temp->optionalCast<Curve>();
    if (!tccfoff) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " curve is wrong type. ");
      return result;
    }

    owo = workspaceObject.getTarget(Coil_Cooling_DX_SingleSpeedFields::EnergyInputRatioFunctionofFlowFractionCurveName);
    if (!owo) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " Can't find EnergyInputRatioFunctionofFlowFractionCurveName.");
      return result;
    }
    if (owo->numSources() > 1) {
      owo = owo->workspace().addObject(owo.get());
    }
    temp = translateAndMapWorkspaceObject(*owo);
    if (!temp) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " Can't convert workspace curve into a model curve. ");
      return result;
    }
    boost::optional<Curve> eirfoff = temp->optionalCast<Curve>();
    if (!eirfoff) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " curve is wrong type. ");
      return result;
    }

    owo = workspaceObject.getTarget(Coil_Cooling_DX_SingleSpeedFields::PartLoadFractionCorrelationCurveName);
    if (!owo) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " Can't find PartLoadFractionCorrelationCurveName.");
      return result;
    }
    if (owo->numSources() > 1) {
      owo = owo->workspace().addObject(owo.get());
    }
    temp = translateAndMapWorkspaceObject(*owo);
    if (!temp) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " Can't convert workspace curve into a model curve. ");
      return result;
    }
    boost::optional<Curve> plfcc = temp->optionalCast<Curve>();
    if (!plfcc) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " curve is wrong type. ");
      return result;
    }

    try {
      CoilCoolingDXSingleSpeed coil(m_model, *schedule, *tccfot, *tccfoff, *eirfot, *eirfoff, *plfcc);

      OptionalString optS = workspaceObject.name();
      if (optS) {
        coil.setName(*optS);
      }

      OptionalDouble d = workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::GrossRatedTotalCoolingCapacity);
      if (d) {
        coil.setRatedTotalCoolingCapacity(*d);
      }

      d = workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::GrossRatedSensibleHeatRatio);
      if (d) {
        coil.setRatedSensibleHeatRatio(*d);
      }

      d = workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::GrossRatedCoolingCOP);
      if (d) {
        coil.setRatedCOP(*d);
      }

      d = workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::RatedAirFlowRate);
      if (d) {
        coil.setRatedAirFlowRate(*d);
      }

      d = workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::RatedEvaporatorFanPowerPerVolumeFlowRate);
      if (d) {
        coil.setRatedEvaporatorFanPowerPerVolumeFlowRate(*d);
      }

      d = workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::NominalTimeforCondensateRemovaltoBegin);
      if (d) {
        coil.setNominalTimeForCondensateRemovalToBegin(*d);
      }

      d = workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation);
      if (d) {
        coil.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(*d);
      }

      d = workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity);
      if (d) {
        coil.setRatioOfInitialMoistureEvaporationRateAndSteadyStateLatentCapacity(*d);
      }

      d = workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::MaximumCyclingRate);
      if (d) {
        coil.setMaximumCyclingRate(*d);
      }

      d = workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::LatentCapacityTimeConstant);
      if (d) {
        coil.setLatentCapacityTimeConstant(*d);
      }

      optS = workspaceObject.getString(Coil_Cooling_DX_SingleSpeedFields::CondenserAirInletNodeName);
      if (optS) {
        coil.setCondenserAirInletNodeName(*optS);
      }

      optS = workspaceObject.getString(Coil_Cooling_DX_SingleSpeedFields::CondenserType);
      if (optS) {
        coil.setCondenserType(*optS);
      }

      d = workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserEffectiveness);
      if (d) {
        coil.setEvaporativeCondenserEffectiveness(*d);
      }

      d = workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserAirFlowRate);
      if (d) {
        coil.setEvaporativeCondenserAirFlowRate(*d);
      }

      d = workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::EvaporativeCondenserPumpRatedPowerConsumption);
      if (d) {
        coil.setEvaporativeCondenserPumpRatedPowerConsumption(*d);
      }

      d = workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::CrankcaseHeaterCapacity);
      if (d) {
        coil.setCrankcaseHeaterCapacity(*d);
      }

      d = workspaceObject.getDouble(Coil_Cooling_DX_SingleSpeedFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
      if (d) {
        coil.setMaximumOutdoorDryBulbTemperatureForCrankcaseHeaterOperation(*d);
      }

      result = coil;

    } catch (std::exception& e) {
      LOG(Error, "Could not reverse translate " << workspaceObject.briefDescription() << ", because " << e.what() << ".");
    }
    return result;
  }

}  // namespace energyplus

}  // namespace openstudio
