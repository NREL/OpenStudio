/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
        auto schedule = modelObject->cast<Schedule>();
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
