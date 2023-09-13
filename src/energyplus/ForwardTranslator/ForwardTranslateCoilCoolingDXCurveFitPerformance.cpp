/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/CoilCoolingDXCurveFitPerformance.hpp"

#include "../../model/Curve.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/CoilCoolingDXCurveFitOperatingMode.hpp"
#include "../../model/CoilCoolingDXCurveFitOperatingMode_Impl.hpp"

#include <utilities/idd/Coil_Cooling_DX_CurveFit_Performance_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCoilCoolingDXCurveFitPerformance(model::CoilCoolingDXCurveFitPerformance& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    // BaseOperatingMode is required, so start with that
    CoilCoolingDXCurveFitOperatingMode baseOpMode = modelObject.baseOperatingMode();
    if (boost::optional<IdfObject> _baseOpMode = translateAndMapModelObject(baseOpMode)) {
      s = _baseOpMode->name().get();
    } else {
      LOG(Warn, modelObject.briefDescription() << " cannot be translated as its Base Operating Mode object cannot be translated: "
                                               << baseOpMode.briefDescription() << ".");
      return boost::none;
    }

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Coil_Cooling_DX_CurveFit_Performance, modelObject);

    // BaseOperatingMode
    idfObject.setString(Coil_Cooling_DX_CurveFit_PerformanceFields::BaseOperatingMode, s.get());

    // CrankcaseHeaterCapacity
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_PerformanceFields::CrankcaseHeaterCapacity, modelObject.crankcaseHeaterCapacity());

    if (auto crankCurve_ = modelObject.crankcaseHeaterCapacityFunctionofTemperatureCurve()) {
      if (auto curve_ = translateAndMapModelObject(*crankCurve_)) {
        idfObject.setString(Coil_Cooling_DX_CurveFit_PerformanceFields::CrankcaseHeaterCapacityFunctionofTemperatureCurveName, curve_->nameString());
      }
    }

    // MinimumOutdoorDryBulbTemperatureforCompressorOperation
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_PerformanceFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation,
                        modelObject.minimumOutdoorDryBulbTemperatureforCompressorOperation());

    // MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_PerformanceFields::MaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation,
                        modelObject.maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation());

    // UnitInternalStaticAirPressure
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_PerformanceFields::UnitInternalStaticAirPressure, modelObject.unitInternalStaticAirPressure());

    // CapacityControlMethod
    idfObject.setString(Coil_Cooling_DX_CurveFit_PerformanceFields::CapacityControlMethod, modelObject.capacityControlMethod());

    // EvaporativeCondenserBasinHeaterCapacity
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_PerformanceFields::EvaporativeCondenserBasinHeaterCapacity,
                        modelObject.evaporativeCondenserBasinHeaterCapacity());

    // EvaporativeCondenserBasinHeaterSetpointTemperature
    idfObject.setDouble(Coil_Cooling_DX_CurveFit_PerformanceFields::EvaporativeCondenserBasinHeaterSetpointTemperature,
                        modelObject.evaporativeCondenserBasinHeaterSetpointTemperature());

    // EvaporativeCondenserBasinHeaterOperatingScheduleName
    idfObject.setString(Coil_Cooling_DX_CurveFit_PerformanceFields::EvaporativeCondenserBasinHeaterOperatingScheduleName,
                        modelObject.evaporativeCondenserBasinHeaterOperatingSchedule().name().get());

    // CompressorFuelType
    idfObject.setString(Coil_Cooling_DX_CurveFit_PerformanceFields::CompressorFuelType, modelObject.compressorFuelType());

    // AlternativeOperatingMode1
    boost::optional<CoilCoolingDXCurveFitOperatingMode> alternativeOperatingMode1 = modelObject.alternativeOperatingMode1();
    if (alternativeOperatingMode1) {
      if (boost::optional<IdfObject> _opMode = translateAndMapModelObject(alternativeOperatingMode1.get())) {
        idfObject.setString(Coil_Cooling_DX_CurveFit_PerformanceFields::AlternativeOperatingMode1, _opMode->nameString());
      }
    }

    // AlternativeOperatingMode2
    boost::optional<CoilCoolingDXCurveFitOperatingMode> alternativeOperatingMode2 = modelObject.alternativeOperatingMode2();
    if (alternativeOperatingMode2) {
      if (boost::optional<IdfObject> _opMode = translateAndMapModelObject(alternativeOperatingMode2.get())) {
        idfObject.setString(Coil_Cooling_DX_CurveFit_PerformanceFields::AlternativeOperatingMode2, _opMode->nameString());
      }
    }

    return boost::optional<IdfObject>(idfObject);
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
