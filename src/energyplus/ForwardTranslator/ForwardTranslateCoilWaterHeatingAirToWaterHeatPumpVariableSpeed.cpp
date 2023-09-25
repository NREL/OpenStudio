/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/CoilWaterHeatingAirToWaterHeatPumpVariableSpeed.hpp"
#include "../../model/CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData.hpp"
#include "../../model/Curve.hpp"
#include <utilities/idd/Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateCoilWaterHeatingAirToWaterHeatPumpVariableSpeed(CoilWaterHeatingAirToWaterHeatPumpVariableSpeed& modelObject) {

    boost::optional<std::string> s;
    boost::optional<double> value;

    auto const speeds = modelObject.speeds();
    auto numSpeeds = speeds.size();
    if (numSpeeds == 0) {
      LOG(Warn, "CoilWaterHeatingAirToWaterHeatPumpVariableSpeed " << modelObject.name().get() << " has no speeds, it will not be translated.");
      return boost::none;
    }

    // Name and register
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Coil_WaterHeating_AirToWaterHeatPump_VariableSpeed, modelObject);

    // Nominal Speed Level
    if (auto speedLevel = modelObject.nominalSpeedLevel()) {
      idfObject.setInt(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::NominalSpeedLevel, speedLevel);
    }

    // Rated Water Heating Capacity
    if ((value = modelObject.ratedWaterHeatingCapacity())) {
      idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedWaterHeatingCapacity, value.get());
    }

    // Rated Evaporator Inlet Air Dry-Bulb Temperatur
    if ((value = modelObject.ratedEvaporatorInletAirDryBulbTemperature())) {
      idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorInletAirDryBulbTemperature, value.get());
    }

    // Rated Evaporator Inlet Air Wet-Bulb Temperature
    if ((value = modelObject.ratedEvaporatorInletAirWetBulbTemperature())) {
      idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorInletAirWetBulbTemperature, value.get());
    }

    // Rated Condenser Inlet Water Temperature
    if ((value = modelObject.ratedCondenserInletWaterTemperature())) {
      idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedCondenserInletWaterTemperature, value.get());
    }

    // Rated Evaporator Air Flow Rate
    if (modelObject.isRatedEvaporatorAirFlowRateAutocalculated()) {
      idfObject.setString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorAirFlowRate, "autocalculate");
    } else if ((value = modelObject.ratedEvaporatorAirFlowRate())) {
      idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedEvaporatorAirFlowRate, value.get());
    }

    // Rated Condenser Water Flow Rate
    if (modelObject.isRatedCondenserWaterFlowRateAutocalculated()) {
      idfObject.setString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedCondenserWaterFlowRate, "autocalculate");
    } else if ((value = modelObject.ratedCondenserWaterFlowRate())) {
      idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::RatedCondenserWaterFlowRate, value.get());
    }

    // Evaporator Fan Power Included in Rated COP
    if ((s = modelObject.evaporatorFanPowerIncludedinRatedCOP())) {
      idfObject.setString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::EvaporatorFanPowerIncludedinRatedCOP, s.get());
    }

    // Condenser Pump Power Included in Rated COP
    if ((s = modelObject.condenserPumpPowerIncludedinRatedCOP())) {
      idfObject.setString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CondenserPumpPowerIncludedinRatedCOP, s.get());
    }

    // Condenser Pump Heat Included in Rated Heating Capacity and Rated COP
    if ((s = modelObject.condenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP())) {
      idfObject.setString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CondenserPumpHeatIncludedinRatedHeatingCapacityandRatedCOP,
                          s.get());
    }

    // Fraction of Condenser Pump Heat to Water
    if ((value = modelObject.fractionofCondenserPumpHeattoWater())) {
      idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::FractionofCondenserPumpHeattoWater, value.get());
    }

    // Crankcase Heater Capacity
    if ((value = modelObject.crankcaseHeaterCapacity())) {
      idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CrankcaseHeaterCapacity, value.get());
    }

    // CrankcaseHeaterCapacityFunctionofTemperatureCurveName
    if (auto crankCurve_ = modelObject.crankcaseHeaterCapacityFunctionofTemperatureCurve()) {
      if (auto curve_ = translateAndMapModelObject(*crankCurve_)) {
        idfObject.setString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::CrankcaseHeaterCapacityFunctionofTemperatureCurveName,
                            curve_->nameString());
      }
    }

    // Maximum Ambient Temperature for Crankcase Heater Operation
    if ((value = modelObject.maximumAmbientTemperatureforCrankcaseHeaterOperation())) {
      idfObject.setDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::MaximumAmbientTemperatureforCrankcaseHeaterOperation,
                          value.get());
    }

    // Evaporator Air Temperature Type for Curve Objects
    if ((s = modelObject.evaporatorAirTemperatureTypeforCurveObjects())) {
      idfObject.setString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::EvaporatorAirTemperatureTypeforCurveObjects, s.get());
    }

    // Part Load Fraction Correlation Curve Name
    auto plrCurve = modelObject.partLoadFractionCorrelationCurve();
    if (boost::optional<IdfObject> _curve = translateAndMapModelObject(plrCurve)) {
      idfObject.setString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::PartLoadFractionCorrelationCurveName, _curve->name().get());
    }

    // Number of Speeds
    idfObject.setInt(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedFields::NumberofSpeeds, numSpeeds);

    for (auto const& speed : speeds) {
      auto eg = idfObject.pushExtensibleGroup();

      // Rated Water Heating Capacity at Speed
      if ((value = speed.ratedWaterHeatingCapacity())) {
        eg.setDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::RatedWaterHeatingCapacityatSpeed, value.get());
      }

      // Rated Water Heating COP at Speed
      if ((value = speed.ratedWaterHeatingCOP())) {
        eg.setDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::RatedWaterHeatingCOPatSpeed, value.get());
      }

      // Rated Sensible Heat Ratio at Speed
      if ((value = speed.ratedSensibleHeatRatio())) {
        eg.setDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::RatedSensibleHeatRatioatSpeed, value.get());
      }

      // Speed Reference Unit Rated Air Flow Rate
      if ((value = speed.referenceUnitRatedAirFlowRate())) {
        eg.setDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedReferenceUnitRatedAirFlowRate, value.get());
      }

      // Speed Reference Unit Rated Water Flow Rate
      if ((value = speed.referenceUnitRatedWaterFlowRate())) {
        eg.setDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedReferenceUnitRatedWaterFlowRate, value.get());
      }

      // Speed Reference Unit Water Pump Input Power At Rated Conditions
      if ((value = speed.referenceUnitWaterPumpInputPowerAtRatedConditions())) {
        eg.setDouble(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedReferenceUnitWaterPumpInputPowerAtRatedConditions,
                     value.get());
      }

      // Speed Total WH Capacity Function of Temperature Curve Name
      {
        auto curve = speed.totalWaterHeatingCapacityFunctionofTemperatureCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedTotalWHCapacityFunctionofTemperatureCurveName,
                       _curve->name().get());
        }
      }

      // Speed Total WH Capacity Function of Air Flow Fraction Curve Name
      {
        auto curve = speed.totalWaterHeatingCapacityFunctionofAirFlowFractionCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedTotalWHCapacityFunctionofAirFlowFractionCurveName,
                       _curve->name().get());
        }
      }

      // Speed Total WH Capacity Function of Water Flow Fraction Curve Name
      {
        auto curve = speed.totalWaterHeatingCapacityFunctionofWaterFlowFractionCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedTotalWHCapacityFunctionofWaterFlowFractionCurveName,
                       _curve->name().get());
        }
      }

      // Speed COP Function of Temperature Curve Name
      {
        auto curve = speed.copFunctionofTemperatureCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedCOPFunctionofTemperatureCurveName,
                       _curve->name().get());
        }
      }

      // Speed COP Function of Air Flow Fraction Curve Name
      {
        auto curve = speed.copFunctionofAirFlowFractionCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedCOPFunctionofAirFlowFractionCurveName,
                       _curve->name().get());
        }
      }

      // Speed COP Function of Water Flow Fraction Curve Name
      {
        auto curve = speed.copFunctionofWaterFlowFractionCurve();
        if (auto _curve = translateAndMapModelObject(curve)) {
          eg.setString(Coil_WaterHeating_AirToWaterHeatPump_VariableSpeedExtensibleFields::SpeedCOPFunctionofWaterFlowFractionCurveName,
                       _curve->name().get());
        }
      }
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
