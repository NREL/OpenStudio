/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOW_HPP
#define MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOW_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class Curve;
class ThermalZone;
class ZoneHVACTerminalUnitVariableRefrigerantFlow;

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


  boost::optional<Curve> coolingCapacityRatioModifierFunctionofLowTemperatureCurve() const;

  bool setCoolingCapacityRatioModifierFunctionofLowTemperatureCurve(const Curve& curve);

  void resetCoolingCapacityRatioModifierFunctionofLowTemperatureCurve();


  boost::optional<Curve> coolingCapacityRatioBoundaryCurve() const;

  bool setCoolingCapacityRatioBoundaryCurve(const Curve& curve);

  void resetCoolingCapacityRatioBoundaryCurve();


  boost::optional<Curve> coolingCapacityRatioModifierFunctionofHighTemperatureCurve() const;

  bool setCoolingCapacityRatioModifierFunctionofHighTemperatureCurve(const Curve& curve);

  void resetCoolingCapacityRatioModifierFunctionofHighTemperatureCurve();


  boost::optional<Curve> coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve() const;

  bool setCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve(const Curve& curve);

  void resetCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve();


  boost::optional<Curve> coolingEnergyInputRatioBoundaryCurve() const;

  bool setCoolingEnergyInputRatioBoundaryCurve(const Curve& curve);

  void resetCoolingEnergyInputRatioBoundaryCurve();


  boost::optional<Curve> coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve() const;

  bool setCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve(const Curve& curve);

  void resetCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve();


  boost::optional<Curve> coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() const;

  bool setCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(const Curve& curve);

  void resetCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();


  boost::optional<Curve> coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() const;

  bool setCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(const Curve& curve);

  void resetCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();


  boost::optional<Curve> coolingCombinationRatioCorrectionFactorCurve() const;

  bool setCoolingCombinationRatioCorrectionFactorCurve(const Curve& curve);

  void resetCoolingCombinationRatioCorrectionFactorCurve();


  boost::optional<Curve> coolingPartLoadFractionCorrelationCurve() const;

  bool setCoolingPartLoadFractionCorrelationCurve(const Curve& curve);

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


  boost::optional<Curve> heatingCapacityRatioModifierFunctionofLowTemperatureCurve() const;

  bool setHeatingCapacityRatioModifierFunctionofLowTemperatureCurve(const Curve& curve);

  void resetHeatingCapacityRatioModifierFunctionofLowTemperatureCurve();


  boost::optional<Curve> heatingCapacityRatioBoundaryCurve() const;

  bool setHeatingCapacityRatioBoundaryCurve(const Curve& curve);

  void resetHeatingCapacityRatioBoundaryCurve();


  boost::optional<Curve> heatingCapacityRatioModifierFunctionofHighTemperatureCurve() const;

  bool setHeatingCapacityRatioModifierFunctionofHighTemperatureCurve(const Curve& curve);

  void resetHeatingCapacityRatioModifierFunctionofHighTemperatureCurve();


  boost::optional<Curve> heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve() const;

  bool setHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve(const Curve& curve);

  void resetHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve();


  boost::optional<Curve> heatingEnergyInputRatioBoundaryCurve() const;

  bool setHeatingEnergyInputRatioBoundaryCurve(const Curve& curve);

  void resetHeatingEnergyInputRatioBoundaryCurve();


  boost::optional<Curve> heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve() const;

  bool setHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve(const Curve& curve);

  void resetHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve();


  std::string heatingPerformanceCurveOutdoorTemperatureType() const;

  bool setHeatingPerformanceCurveOutdoorTemperatureType(std::string heatingPerformanceCurveOutdoorTemperatureType);


  boost::optional<Curve> heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() const;

  bool setHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(const Curve& curve);

  void resetHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();


  boost::optional<Curve> heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() const;

  bool setHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(const Curve& curve);

  void resetHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();


  boost::optional<Curve> heatingCombinationRatioCorrectionFactorCurve() const;

  bool setHeatingCombinationRatioCorrectionFactorCurve(const Curve& curve);

  void resetHeatingCombinationRatioCorrectionFactorCurve();


  boost::optional<Curve> heatingPartLoadFractionCorrelationCurve() const;

  bool setHeatingPartLoadFractionCorrelationCurve(const Curve& curve);

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


  boost::optional<Curve> pipingCorrectionFactorforLengthinCoolingModeCurve() const;

  bool setPipingCorrectionFactorforLengthinCoolingModeCurve(const Curve& curve);

  void resetPipingCorrectionFactorforLengthinCoolingModeCurve();


  double pipingCorrectionFactorforHeightinCoolingModeCoefficient() const;

  void setPipingCorrectionFactorforHeightinCoolingModeCoefficient(double pipingCorrectionFactorforHeightinCoolingModeCoefficient);


  double equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode() const;

  void setEquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode(double equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode);


  boost::optional<Curve> pipingCorrectionFactorforLengthinHeatingModeCurve() const;

  bool setPipingCorrectionFactorforLengthinHeatingModeCurve(const Curve& curve);

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


  boost::optional<Curve> defrostEnergyInputRatioModifierFunctionofTemperatureCurve() const;

  bool setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(const Curve& curve);

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


  boost::optional<Curve> heatRecoveryCoolingCapacityModifierCurve() const;

  bool setHeatRecoveryCoolingCapacityModifierCurve(const Curve& curve);

  void resetHeatRecoveryCoolingCapacityModifierCurve();


  double initialHeatRecoveryCoolingCapacityFraction() const;

  void setInitialHeatRecoveryCoolingCapacityFraction(double initialHeatRecoveryCoolingCapacityFraction);


  double heatRecoveryCoolingCapacityTimeConstant() const;

  void setHeatRecoveryCoolingCapacityTimeConstant(double heatRecoveryCoolingCapacityTimeConstant);


  boost::optional<Curve> heatRecoveryCoolingEnergyModifierCurve() const;

  bool setHeatRecoveryCoolingEnergyModifierCurve(const Curve& curve);

  void resetHeatRecoveryCoolingEnergyModifierCurve();


  double initialHeatRecoveryCoolingEnergyFraction() const;

  void setInitialHeatRecoveryCoolingEnergyFraction(double initialHeatRecoveryCoolingEnergyFraction);


  double heatRecoveryCoolingEnergyTimeConstant() const;

  void setHeatRecoveryCoolingEnergyTimeConstant(double heatRecoveryCoolingEnergyTimeConstant);


  boost::optional<Curve> heatRecoveryHeatingCapacityModifierCurve() const;

  bool setHeatRecoveryHeatingCapacityModifierCurve(const Curve& curve);

  void resetHeatRecoveryHeatingCapacityModifierCurve();


  double initialHeatRecoveryHeatingCapacityFraction() const;

  void setInitialHeatRecoveryHeatingCapacityFraction(double initialHeatRecoveryHeatingCapacityFraction);


  double heatRecoveryHeatingCapacityTimeConstant() const;

  void setHeatRecoveryHeatingCapacityTimeConstant(double heatRecoveryHeatingCapacityTimeConstant);


  boost::optional<Curve> heatRecoveryHeatingEnergyModifierCurve() const;

  bool setHeatRecoveryHeatingEnergyModifierCurve(const Curve& curve);

  void resetHeatRecoveryHeatingEnergyModifierCurve();


  double initialHeatRecoveryHeatingEnergyFraction() const;

  void setInitialHeatRecoveryHeatingEnergyFraction(double initialHeatRecoveryHeatingEnergyFraction);


  double heatRecoveryHeatingEnergyTimeConstant() const;

  void setHeatRecoveryHeatingEnergyTimeConstant(double heatRecoveryHeatingEnergyTimeConstant);


  void addTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow & vrf);

  void removeTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow & vrf);

  void removeAllTerminals();

  std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> terminals() const;

 protected:

  /// @cond
  typedef detail::AirConditionerVariableRefrigerantFlow_Impl ImplType;

  explicit AirConditionerVariableRefrigerantFlow(std::shared_ptr<detail::AirConditionerVariableRefrigerantFlow_Impl> impl);

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

