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

#include "../../model/CoilCoolingDXCurveFitPerformance.hpp"
#include "../../model/CoilCoolingDXCurveFitPerformance_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/CoilCoolingDXCurveFitOperatingMode.hpp"
#include "../../model/CoilCoolingDXCurveFitOperatingMode_Impl.hpp"

#include <utilities/idd/Coil_Cooling_DX_CurveFit_Performance_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateCoilCoolingDXCurveFitPerformance(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Coil_Cooling_DX_CurveFit_Performance) {
      LOG(Error, "WorkspaceObject is not IddObjectType: CoilCoolingDXCurveFitPerformance");
      return boost::none;
    }

    OptionalString s;
    OptionalDouble d;
    OptionalWorkspaceObject target;

    boost::optional<CoilCoolingDXCurveFitOperatingMode> baseOperatingMode;
    if ((target = workspaceObject.getTarget(openstudio::Coil_Cooling_DX_CurveFit_PerformanceFields::BaseOperatingMode))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<CoilCoolingDXCurveFitOperatingMode>()) {
          baseOperatingMode = modelObject->cast<CoilCoolingDXCurveFitOperatingMode>();
        }
      }
    }

    openstudio::model::CoilCoolingDXCurveFitPerformance performance(m_model, *baseOperatingMode);

    s = workspaceObject.name();
    if (s) {
      performance.setName(*s);
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_PerformanceFields::CrankcaseHeaterCapacity);
    if (d) {
      performance.setCrankcaseHeaterCapacity(*d);
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_PerformanceFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation);
    if (d) {
      performance.setMinimumOutdoorDryBulbTemperatureforCompressorOperation(*d);
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_PerformanceFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
    if (d) {
      performance.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(*d);
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_PerformanceFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);
    if (d) {
      performance.setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(*d);
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_PerformanceFields::UnitInternalStaticAirPressure);
    if (d) {
      performance.setUnitInternalStaticAirPressure(*d);
    }

    s = workspaceObject.getString(Coil_Cooling_DX_CurveFit_PerformanceFields::CapacityControlMethod);
    if (s) {
      performance.setCapacityControlMethod(*s);
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_PerformanceFields::EvaporativeCondenserBasinHeaterCapacity);
    if (d) {
      performance.setEvaporativeCondenserBasinHeaterCapacity(*d);
    }

    d = workspaceObject.getDouble(Coil_Cooling_DX_CurveFit_PerformanceFields::EvaporativeCondenserBasinHeaterSetpointTemperature);
    if (d) {
      performance.setEvaporativeCondenserBasinHeaterSetpointTemperature(*d);
    }

    if ((target =
           workspaceObject.getTarget(openstudio::Coil_Cooling_DX_CurveFit_PerformanceFields::EvaporativeCondenserBasinHeaterOperatingScheduleName))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject && modelObject->optionalCast<Schedule>()) {
        Schedule schedule = modelObject->cast<Schedule>();
        performance.setEvaporativeCondenserBasinHeaterOperatingSchedule(schedule);
      }
    }

    s = workspaceObject.getString(Coil_Cooling_DX_CurveFit_PerformanceFields::CompressorFuelType);
    if (s) {
      performance.setCompressorFuelType(*s);
    }

    if ((target = workspaceObject.getTarget(openstudio::Coil_Cooling_DX_CurveFit_PerformanceFields::AlternativeOperatingMode1))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject && modelObject->optionalCast<CoilCoolingDXCurveFitOperatingMode>()) {
        performance.setAlternativeOperatingMode1(modelObject->cast<CoilCoolingDXCurveFitOperatingMode>());
      }
    }

    if ((target = workspaceObject.getTarget(openstudio::Coil_Cooling_DX_CurveFit_PerformanceFields::AlternativeOperatingMode2))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject && modelObject->optionalCast<CoilCoolingDXCurveFitOperatingMode>()) {
        performance.setAlternativeOperatingMode1(modelObject->cast<CoilCoolingDXCurveFitOperatingMode>());
      }
    }

    return performance;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
