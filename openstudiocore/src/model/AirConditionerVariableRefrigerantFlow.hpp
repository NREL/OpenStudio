/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOW_HPP
#define MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOW_HPP

#include <model/ModelAPI.hpp>
#include <model/StraightComponent.hpp>

namespace openstudio {

namespace model {

class Schedule;
class CurveBiquadratic;
class CurveCubic;
class CurveLinear;
class ThermalZone;

namespace detail {

  class AirConditionerVariableRefrigerantFlow_Impl;

} // detail

/** AirConditionerVariableRefrigerantFlow is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirConditioner:VariableRefrigerantFlow'. */
class MODEL_API AirConditionerVariableRefrigerantFlow : public StraightComponent {

 public:

  explicit AirConditionerVariableRefrigerantFlow(const Model& model);

  virtual ~AirConditionerVariableRefrigerantFlow() {}

  static IddObjectType iddObjectType();

  static std::vector<std::string> heatingPerformanceCurveOutdoorTemperatureTypeValues();

  static std::vector<std::string> masterThermostatPriorityControlTypeValues();

  static std::vector<std::string> defrostStrategyValues();

  static std::vector<std::string> defrostControlValues();

  static std::vector<std::string> fuelTypeValues();


  Schedule availabilitySchedule() const;

  bool setAvailabilitySchedule(Schedule& schedule);


  boost::optional<double> ratedTotalCoolingCapacity() const;

  bool isRatedTotalCoolingCapacityAutosized() const;

  bool setRatedTotalCoolingCapacity(double ratedTotalCoolingCapacity);

  void autosizeRatedTotalCoolingCapacity();


  double ratedCoolingCOP() const;

  bool setRatedCoolingCOP(double ratedCoolingCOP);


  double minimumOutdoorTemperatureinCoolingMode() const;

  void setMinimumOutdoorTemperatureinCoolingMode(double minimumOutdoorTemperatureinCoolingMode);


  double maximumOutdoorTemperatureinCoolingMode() const;

  void setMaximumOutdoorTemperatureinCoolingMode(double maximumOutdoorTemperatureinCoolingMode);


  boost::optional<CurveBiquadratic> coolingCapacityRatioModifierFunctionofLowTemperatureCurve() const;

  bool setCoolingCapacityRatioModifierFunctionofLowTemperatureCurve(const CurveBiquadratic& curve);

  void resetCoolingCapacityRatioModifierFunctionofLowTemperatureCurve();


  boost::optional<CurveLinear> coolingCapacityRatioBoundaryCurve() const;

  bool setCoolingCapacityRatioBoundaryCurve(const CurveLinear& curveLinear);

  void resetCoolingCapacityRatioBoundaryCurve();


  boost::optional<CurveBiquadratic> coolingCapacityRatioModifierFunctionofHighTemperatureCurve() const;

  bool setCoolingCapacityRatioModifierFunctionofHighTemperatureCurve(const CurveBiquadratic& curve);

  void resetCoolingCapacityRatioModifierFunctionofHighTemperatureCurve();


  boost::optional<CurveBiquadratic> coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve() const;

  bool setCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve(const CurveBiquadratic& curve);

  void resetCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve();


  boost::optional<CurveLinear> coolingEnergyInputRatioBoundaryCurve() const;

  bool setCoolingEnergyInputRatioBoundaryCurve(const CurveLinear& curveLinear);

  void resetCoolingEnergyInputRatioBoundaryCurve();


  boost::optional<CurveBiquadratic> coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve() const;

  bool setCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve(const CurveBiquadratic& curve);

  void resetCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve();


  boost::optional<CurveLinear> coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() const;

  bool setCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(const CurveLinear& curveLinear);

  void resetCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();


  boost::optional<CurveLinear> coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() const;

  bool setCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(const CurveLinear& curveLinear);

  void resetCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();


  boost::optional<CurveLinear> coolingCombinationRatioCorrectionFactorCurve() const;

  bool setCoolingCombinationRatioCorrectionFactorCurve(const CurveLinear& curveLinear);

  void resetCoolingCombinationRatioCorrectionFactorCurve();


  boost::optional<CurveLinear> coolingPartLoadFractionCorrelationCurve() const;

  bool setCoolingPartLoadFractionCorrelationCurve(const CurveLinear& curveLinear);

  void resetCoolingPartLoadFractionCorrelationCurve();


  boost::optional<double> ratedTotalHeatingCapacity() const;

  bool isRatedTotalHeatingCapacityAutosized() const;

  void setRatedTotalHeatingCapacity(double ratedTotalHeatingCapacity);

  void autosizeRatedTotalHeatingCapacity();


  double ratedTotalHeatingCapacitySizingRatio() const;

  bool setRatedTotalHeatingCapacitySizingRatio(double ratedTotalHeatingCapacitySizingRatio);


  double ratedHeatingCOP() const;

  void setRatedHeatingCOP(double ratedHeatingCOP);


  double minimumOutdoorTemperatureinHeatingMode() const;

  void setMinimumOutdoorTemperatureinHeatingMode(double minimumOutdoorTemperatureinHeatingMode);


  double maximumOutdoorTemperatureinHeatingMode() const;

  void setMaximumOutdoorTemperatureinHeatingMode(double maximumOutdoorTemperatureinHeatingMode);


  boost::optional<CurveBiquadratic> heatingCapacityRatioModifierFunctionofLowTemperatureCurve() const;

  bool setHeatingCapacityRatioModifierFunctionofLowTemperatureCurve(const CurveBiquadratic& curve);

  void resetHeatingCapacityRatioModifierFunctionofLowTemperatureCurve();


  boost::optional<CurveLinear> heatingCapacityRatioBoundaryCurve() const;

  bool setHeatingCapacityRatioBoundaryCurve(const CurveLinear& curveLinear);

  void resetHeatingCapacityRatioBoundaryCurve();


  boost::optional<CurveBiquadratic> heatingCapacityRatioModifierFunctionofHighTemperatureCurve() const;

  bool setHeatingCapacityRatioModifierFunctionofHighTemperatureCurve(const CurveBiquadratic& curve);

  void resetHeatingCapacityRatioModifierFunctionofHighTemperatureCurve();


  boost::optional<CurveBiquadratic> heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve() const;

  bool setHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve(const CurveBiquadratic& curve);

  void resetHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve();


  boost::optional<CurveLinear> heatingEnergyInputRatioBoundaryCurve() const;

  bool setHeatingEnergyInputRatioBoundaryCurve(const CurveLinear& curveLinear);

  void resetHeatingEnergyInputRatioBoundaryCurve();


  boost::optional<CurveBiquadratic> heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve() const;

  bool setHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve(const CurveBiquadratic& curve);

  void resetHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve();


  std::string heatingPerformanceCurveOutdoorTemperatureType() const;

  bool setHeatingPerformanceCurveOutdoorTemperatureType(std::string heatingPerformanceCurveOutdoorTemperatureType);


  boost::optional<CurveCubic> heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() const;

  bool setHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(const CurveCubic& curve);

  void resetHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();


  boost::optional<CurveLinear> heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() const;

  bool setHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(const CurveLinear& curveLinear);

  void resetHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();


  boost::optional<CurveLinear> heatingCombinationRatioCorrectionFactorCurve() const;

  bool setHeatingCombinationRatioCorrectionFactorCurve(const CurveLinear& curveLinear);

  void resetHeatingCombinationRatioCorrectionFactorCurve();


  boost::optional<CurveLinear> heatingPartLoadFractionCorrelationCurve() const;

  bool setHeatingPartLoadFractionCorrelationCurve(const CurveLinear& curveLinear);

  void resetHeatingPartLoadFractionCorrelationCurve();


  double minimumHeatPumpPartLoadRatio() const;

  void setMinimumHeatPumpPartLoadRatio(double minimumHeatPumpPartLoadRatio);


  boost::optional<ThermalZone> zoneforMasterThermostatLocation() const;

  bool setZoneforMasterThermostatLocation(const ThermalZone& zone);

  void resetZoneforMasterThermostatLocation();


  std::string masterThermostatPriorityControlType() const;

  bool setMasterThermostatPriorityControlType(std::string masterThermostatPriorityControlType);


  boost::optional<Schedule> thermostatPrioritySchedule() const;

  bool setThermostatPrioritySchedule(Schedule& schedule);

  void resetThermostatPrioritySchedule();


  bool heatPumpWasteHeatRecovery() const;

  void setHeatPumpWasteHeatRecovery(bool heatPumpWasteHeatRecovery);


  double equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode() const;

  void setEquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode(double equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode);


  double verticalHeightusedforPipingCorrectionFactor() const;

  void setVerticalHeightusedforPipingCorrectionFactor(double verticalHeightusedforPipingCorrectionFactor);


  boost::optional<CurveLinear> pipingCorrectionFactorforLengthinCoolingModeCurve() const;

  bool setPipingCorrectionFactorforLengthinCoolingModeCurve(const CurveLinear& curveLinear);

  void resetPipingCorrectionFactorforLengthinCoolingModeCurve();


  double pipingCorrectionFactorforHeightinCoolingModeCoefficient() const;

  void setPipingCorrectionFactorforHeightinCoolingModeCoefficient(double pipingCorrectionFactorforHeightinCoolingModeCoefficient);


  double equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode() const;

  void setEquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode(double equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode);


  boost::optional<CurveLinear> pipingCorrectionFactorforLengthinHeatingModeCurve() const;

  bool setPipingCorrectionFactorforLengthinHeatingModeCurve(const CurveLinear& curveLinear);

  void resetPipingCorrectionFactorforLengthinHeatingModeCurve();


  double pipingCorrectionFactorforHeightinHeatingModeCoefficient() const;

  void setPipingCorrectionFactorforHeightinHeatingModeCoefficient(double pipingCorrectionFactorforHeightinHeatingModeCoefficient);


  double crankcaseHeaterPowerperCompressor() const;

  void setCrankcaseHeaterPowerperCompressor(double crankcaseHeaterPowerperCompressor);


  int numberofCompressors() const;

  void setNumberofCompressors(int numberofCompressors);


  double ratioofCompressorSizetoTotalCompressorCapacity() const;

  void setRatioofCompressorSizetoTotalCompressorCapacity(double ratioofCompressorSizetoTotalCompressorCapacity);


  double maximumOutdoorDrybulbTemperatureforCrankcaseHeater() const;

  void setMaximumOutdoorDrybulbTemperatureforCrankcaseHeater(double maximumOutdoorDrybulbTemperatureforCrankcaseHeater);


  std::string defrostStrategy() const;

  bool setDefrostStrategy(std::string defrostStrategy);


  std::string defrostControl() const;

  bool setDefrostControl(std::string defrostControl);


  boost::optional<CurveBiquadratic> defrostEnergyInputRatioModifierFunctionofTemperatureCurve() const;

  bool setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(const CurveBiquadratic& curve);

  void resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve();


  double defrostTimePeriodFraction() const;

  bool setDefrostTimePeriodFraction(double defrostTimePeriodFraction);


  boost::optional<double> resistiveDefrostHeaterCapacity() const;

  bool isResistiveDefrostHeaterCapacityAutosized() const;

  bool setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity);

  void autosizeResistiveDefrostHeaterCapacity();


  double maximumOutdoorDrybulbTemperatureforDefrostOperation() const;

  void setMaximumOutdoorDrybulbTemperatureforDefrostOperation(double maximumOutdoorDrybulbTemperatureforDefrostOperation);


  boost::optional<double> waterCondenserVolumeFlowRate() const;

  bool isWaterCondenserVolumeFlowRateAutosized() const;

  void setWaterCondenserVolumeFlowRate(double waterCondenserVolumeFlowRate);

  void autosizeWaterCondenserVolumeFlowRate();


  double evaporativeCondenserEffectiveness() const;

  bool setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness);


  boost::optional<double> evaporativeCondenserAirFlowRate() const;

  bool isEvaporativeCondenserAirFlowRateAutosized() const;

  bool setEvaporativeCondenserAirFlowRate(double evaporativeCondenserAirFlowRate);

  void autosizeEvaporativeCondenserAirFlowRate();


  boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption() const;

  bool isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const;

  bool setEvaporativeCondenserPumpRatedPowerConsumption(double evaporativeCondenserPumpRatedPowerConsumption);

  void autosizeEvaporativeCondenserPumpRatedPowerConsumption();


  double basinHeaterCapacity() const;

  bool setBasinHeaterCapacity(double basinHeaterCapacity);


  double basinHeaterSetpointTemperature() const;

  bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);


  boost::optional<Schedule> basinHeaterOperatingSchedule() const;

  bool setBasinHeaterOperatingSchedule(Schedule& schedule);

  void resetBasinHeaterOperatingSchedule();


  std::string fuelType() const;

  bool setFuelType(std::string fuelType);


  double minimumOutdoorTemperatureinHeatRecoveryMode() const;

  void setMinimumOutdoorTemperatureinHeatRecoveryMode(double minimumOutdoorTemperatureinHeatRecoveryMode);


  double maximumOutdoorTemperatureinHeatRecoveryMode() const;

  void setMaximumOutdoorTemperatureinHeatRecoveryMode(double maximumOutdoorTemperatureinHeatRecoveryMode);


  boost::optional<CurveBiquadratic> heatRecoveryCoolingCapacityModifierCurve() const;

  bool setHeatRecoveryCoolingCapacityModifierCurve(const CurveBiquadratic& curve);

  void resetHeatRecoveryCoolingCapacityModifierCurve();


  double initialHeatRecoveryCoolingCapacityFraction() const;

  void setInitialHeatRecoveryCoolingCapacityFraction(double initialHeatRecoveryCoolingCapacityFraction);


  double heatRecoveryCoolingCapacityTimeConstant() const;

  void setHeatRecoveryCoolingCapacityTimeConstant(double heatRecoveryCoolingCapacityTimeConstant);


  boost::optional<CurveBiquadratic> heatRecoveryCoolingEnergyModifierCurve() const;

  bool setHeatRecoveryCoolingEnergyModifierCurve(const CurveBiquadratic& curve);

  void resetHeatRecoveryCoolingEnergyModifierCurve();


  double initialHeatRecoveryCoolingEnergyFraction() const;

  void setInitialHeatRecoveryCoolingEnergyFraction(double initialHeatRecoveryCoolingEnergyFraction);


  double heatRecoveryCoolingEnergyTimeConstant() const;

  void setHeatRecoveryCoolingEnergyTimeConstant(double heatRecoveryCoolingEnergyTimeConstant);


  boost::optional<CurveBiquadratic> heatRecoveryHeatingCapacityModifierCurve() const;

  bool setHeatRecoveryHeatingCapacityModifierCurve(const CurveBiquadratic& curve);

  void resetHeatRecoveryHeatingCapacityModifierCurve();


  double initialHeatRecoveryHeatingCapacityFraction() const;

  void setInitialHeatRecoveryHeatingCapacityFraction(double initialHeatRecoveryHeatingCapacityFraction);


  double heatRecoveryHeatingCapacityTimeConstant() const;

  void setHeatRecoveryHeatingCapacityTimeConstant(double heatRecoveryHeatingCapacityTimeConstant);


  boost::optional<CurveBiquadratic> heatRecoveryHeatingEnergyModifierCurve() const;

  bool setHeatRecoveryHeatingEnergyModifierCurve(const CurveBiquadratic& curve);

  void resetHeatRecoveryHeatingEnergyModifierCurve();


  double initialHeatRecoveryHeatingEnergyFraction() const;

  void setInitialHeatRecoveryHeatingEnergyFraction(double initialHeatRecoveryHeatingEnergyFraction);


  double heatRecoveryHeatingEnergyTimeConstant() const;

  void setHeatRecoveryHeatingEnergyTimeConstant(double heatRecoveryHeatingEnergyTimeConstant);

 protected:

  /// @cond
  typedef detail::AirConditionerVariableRefrigerantFlow_Impl ImplType;

  explicit AirConditionerVariableRefrigerantFlow(boost::shared_ptr<detail::AirConditionerVariableRefrigerantFlow_Impl> impl);

  friend class detail::AirConditionerVariableRefrigerantFlow_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond

 private:

  REGISTER_LOGGER("openstudio.model.AirConditionerVariableRefrigerantFlow");
};

/** \relates AirConditionerVariableRefrigerantFlow*/
typedef boost::optional<AirConditionerVariableRefrigerantFlow> OptionalAirConditionerVariableRefrigerantFlow;

/** \relates AirConditionerVariableRefrigerantFlow*/
typedef std::vector<AirConditionerVariableRefrigerantFlow> AirConditionerVariableRefrigerantFlowVector;

} // model
} // openstudio

#endif // MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOW_HPP

