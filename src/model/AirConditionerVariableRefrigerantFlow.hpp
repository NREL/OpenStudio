/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOW_HPP
#define MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOW_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Curve;
  class ThermalZone;
  class ZoneHVACTerminalUnitVariableRefrigerantFlow;

  namespace detail {

    class AirConditionerVariableRefrigerantFlow_Impl;

  }  // namespace detail

  /** AirConditionerVariableRefrigerantFlow is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirConditioner:VariableRefrigerantFlow'. */
  class MODEL_API AirConditionerVariableRefrigerantFlow : public StraightComponent
  {

   public:
    explicit AirConditionerVariableRefrigerantFlow(const Model& model);

    virtual ~AirConditionerVariableRefrigerantFlow() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirConditionerVariableRefrigerantFlow(const AirConditionerVariableRefrigerantFlow& other) = default;
    AirConditionerVariableRefrigerantFlow(AirConditionerVariableRefrigerantFlow&& other) = default;
    AirConditionerVariableRefrigerantFlow& operator=(const AirConditionerVariableRefrigerantFlow&) = default;
    AirConditionerVariableRefrigerantFlow& operator=(AirConditionerVariableRefrigerantFlow&&) = default;

    static IddObjectType iddObjectType();

    static std::vector<std::string> heatingPerformanceCurveOutdoorTemperatureTypeValues();

    static std::vector<std::string> masterThermostatPriorityControlTypeValues();

    static std::vector<std::string> defrostStrategyValues();

    static std::vector<std::string> defrostControlValues();

    static std::vector<std::string> fuelTypeValues();

    Schedule availabilitySchedule() const;

    bool setAvailabilitySchedule(Schedule& schedule);

    boost::optional<double> grossRatedTotalCoolingCapacity() const;
    bool isGrossRatedTotalCoolingCapacityAutosized() const;
    bool setGrossRatedTotalCoolingCapacity(double grossRatedTotalCoolingCapacity);
    void autosizeGrossRatedTotalCoolingCapacity();
    double grossRatedCoolingCOP() const;
    bool setGrossRatedCoolingCOP(double grossRatedCoolingCOP);

    // Deprecated
    OS_DEPRECATED boost::optional<double> ratedTotalCoolingCapacity() const;
    OS_DEPRECATED bool isRatedTotalCoolingCapacityAutosized() const;
    OS_DEPRECATED bool setRatedTotalCoolingCapacity(double ratedTotalCoolingCapacity);
    OS_DEPRECATED void autosizeRatedTotalCoolingCapacity();
    OS_DEPRECATED double ratedCoolingCOP() const;
    OS_DEPRECATED bool setRatedCoolingCOP(double ratedCoolingCOP);

    /** In EnergyPlus 9.6.0 and above this property maps to the EnergyPlus field "Minimum Condenser Inlet Node Temperature in Cooling Mode" **/
    double minimumOutdoorTemperatureinCoolingMode() const;

    bool setMinimumOutdoorTemperatureinCoolingMode(double minimumOutdoorTemperatureinCoolingMode);

    /** In EnergyPlus 9.6.0 and above this property maps to the EnergyPlus field "Maximum Condenser Inlet Node Temperature in Cooling Mode" **/
    double maximumOutdoorTemperatureinCoolingMode() const;

    bool setMaximumOutdoorTemperatureinCoolingMode(double maximumOutdoorTemperatureinCoolingMode);

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

    boost::optional<double> grossRatedHeatingCapacity() const;
    bool isGrossRatedHeatingCapacityAutosized() const;
    bool setGrossRatedHeatingCapacity(double grossRatedHeatingCapacity);
    void autosizeGrossRatedHeatingCapacity();
    double ratedHeatingCapacitySizingRatio() const;
    bool setRatedHeatingCapacitySizingRatio(double ratedHeatingCapacitySizingRatio);

    // Deprecated
    OS_DEPRECATED boost::optional<double> ratedTotalHeatingCapacity() const;
    OS_DEPRECATED bool isRatedTotalHeatingCapacityAutosized() const;
    OS_DEPRECATED bool setRatedTotalHeatingCapacity(double ratedTotalHeatingCapacity);
    OS_DEPRECATED void autosizeRatedTotalHeatingCapacity();
    OS_DEPRECATED double ratedTotalHeatingCapacitySizingRatio() const;
    OS_DEPRECATED bool setRatedTotalHeatingCapacitySizingRatio(double ratedTotalHeatingCapacitySizingRatio);

    double ratedHeatingCOP() const;

    bool setRatedHeatingCOP(double ratedHeatingCOP);

    /** In EnergyPlus 9.6.0 and above this property maps to the EnergyPlus field "Minimum Condenser Inlet Node Temperature in Heating Mode" **/
    double minimumOutdoorTemperatureinHeatingMode() const;

    bool setMinimumOutdoorTemperatureinHeatingMode(double minimumOutdoorTemperatureinHeatingMode);

    /** In EnergyPlus 9.6.0 and above this property maps to the EnergyPlus field "Maximum Condenser Inlet Node Temperature in Heating Mode" **/
    double maximumOutdoorTemperatureinHeatingMode() const;

    bool setMaximumOutdoorTemperatureinHeatingMode(double maximumOutdoorTemperatureinHeatingMode);

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

    bool setHeatingPerformanceCurveOutdoorTemperatureType(const std::string& heatingPerformanceCurveOutdoorTemperatureType);

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

    bool setMinimumHeatPumpPartLoadRatio(double minimumHeatPumpPartLoadRatio);

    boost::optional<ThermalZone> zoneforMasterThermostatLocation() const;

    bool setZoneforMasterThermostatLocation(const ThermalZone& zone);

    void resetZoneforMasterThermostatLocation();

    std::string masterThermostatPriorityControlType() const;

    bool setMasterThermostatPriorityControlType(const std::string& masterThermostatPriorityControlType);

    boost::optional<Schedule> thermostatPrioritySchedule() const;

    bool setThermostatPrioritySchedule(Schedule& schedule);

    void resetThermostatPrioritySchedule();

    bool heatPumpWasteHeatRecovery() const;

    bool setHeatPumpWasteHeatRecovery(bool heatPumpWasteHeatRecovery);

    double equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode() const;

    bool setEquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode(double equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode);

    double verticalHeightusedforPipingCorrectionFactor() const;

    bool setVerticalHeightusedforPipingCorrectionFactor(double verticalHeightusedforPipingCorrectionFactor);

    boost::optional<Curve> pipingCorrectionFactorforLengthinCoolingModeCurve() const;

    bool setPipingCorrectionFactorforLengthinCoolingModeCurve(const Curve& curve);

    void resetPipingCorrectionFactorforLengthinCoolingModeCurve();

    double pipingCorrectionFactorforHeightinCoolingModeCoefficient() const;

    bool setPipingCorrectionFactorforHeightinCoolingModeCoefficient(double pipingCorrectionFactorforHeightinCoolingModeCoefficient);

    double equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode() const;

    bool setEquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode(double equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode);

    boost::optional<Curve> pipingCorrectionFactorforLengthinHeatingModeCurve() const;

    bool setPipingCorrectionFactorforLengthinHeatingModeCurve(const Curve& curve);

    void resetPipingCorrectionFactorforLengthinHeatingModeCurve();

    double pipingCorrectionFactorforHeightinHeatingModeCoefficient() const;

    bool setPipingCorrectionFactorforHeightinHeatingModeCoefficient(double pipingCorrectionFactorforHeightinHeatingModeCoefficient);

    double crankcaseHeaterPowerperCompressor() const;

    bool setCrankcaseHeaterPowerperCompressor(double crankcaseHeaterPowerperCompressor);

    int numberofCompressors() const;

    bool setNumberofCompressors(int numberofCompressors);

    double ratioofCompressorSizetoTotalCompressorCapacity() const;

    bool setRatioofCompressorSizetoTotalCompressorCapacity(double ratioofCompressorSizetoTotalCompressorCapacity);

    double maximumOutdoorDrybulbTemperatureforCrankcaseHeater() const;

    bool setMaximumOutdoorDrybulbTemperatureforCrankcaseHeater(double maximumOutdoorDrybulbTemperatureforCrankcaseHeater);

    std::string defrostStrategy() const;

    bool setDefrostStrategy(const std::string& defrostStrategy);

    std::string defrostControl() const;

    bool setDefrostControl(const std::string& defrostControl);

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

    bool setMaximumOutdoorDrybulbTemperatureforDefrostOperation(double maximumOutdoorDrybulbTemperatureforDefrostOperation);

    // Returns the hardcoded condenserType, or the defaulted one if not (If PlantLoop => 'WaterCooled', else 'AirCooled')
    std::string condenserType() const;

    // Sets the condenser type explicitly. Note is was decided that not "smart" logic would be implemented in the model api, and it was moved to the FT
    // If you harcode the condenser type, you are responsible to ensure that you are matching the plant loop connection status (eg: If you set this to
    // 'AirCooled' or 'EvaporativelyCooled', the object should not be on a PlantLoop. If 'WaterCooled', it should be on a PlantLoop)
    bool setCondenserType(const std::string& condenserType);

    bool isCondenserTypeDefaulted() const;
    void resetCondenserType();

    boost::optional<double> waterCondenserVolumeFlowRate() const;

    bool isWaterCondenserVolumeFlowRateAutosized() const;

    bool setWaterCondenserVolumeFlowRate(double waterCondenserVolumeFlowRate);

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

    bool setFuelType(const std::string& fuelType);

    /** In EnergyPlus 9.6.0 and above this property maps to the EnergyPlus field "Minimum Condenser Inlet Node Temperature in Heat Recovery Mode" **/
    double minimumOutdoorTemperatureinHeatRecoveryMode() const;

    bool setMinimumOutdoorTemperatureinHeatRecoveryMode(double minimumOutdoorTemperatureinHeatRecoveryMode);

    /** In EnergyPlus 9.6.0 and above this property maps to the EnergyPlus field "Maximum Condenser Inlet Node Temperature in Heat Recovery Mode" **/
    double maximumOutdoorTemperatureinHeatRecoveryMode() const;

    bool setMaximumOutdoorTemperatureinHeatRecoveryMode(double maximumOutdoorTemperatureinHeatRecoveryMode);

    boost::optional<Curve> heatRecoveryCoolingCapacityModifierCurve() const;

    bool setHeatRecoveryCoolingCapacityModifierCurve(const Curve& curve);

    void resetHeatRecoveryCoolingCapacityModifierCurve();

    double initialHeatRecoveryCoolingCapacityFraction() const;

    bool setInitialHeatRecoveryCoolingCapacityFraction(double initialHeatRecoveryCoolingCapacityFraction);

    double heatRecoveryCoolingCapacityTimeConstant() const;

    bool setHeatRecoveryCoolingCapacityTimeConstant(double heatRecoveryCoolingCapacityTimeConstant);

    boost::optional<Curve> heatRecoveryCoolingEnergyModifierCurve() const;

    bool setHeatRecoveryCoolingEnergyModifierCurve(const Curve& curve);

    void resetHeatRecoveryCoolingEnergyModifierCurve();

    double initialHeatRecoveryCoolingEnergyFraction() const;

    bool setInitialHeatRecoveryCoolingEnergyFraction(double initialHeatRecoveryCoolingEnergyFraction);

    double heatRecoveryCoolingEnergyTimeConstant() const;

    bool setHeatRecoveryCoolingEnergyTimeConstant(double heatRecoveryCoolingEnergyTimeConstant);

    boost::optional<Curve> heatRecoveryHeatingCapacityModifierCurve() const;

    bool setHeatRecoveryHeatingCapacityModifierCurve(const Curve& curve);

    void resetHeatRecoveryHeatingCapacityModifierCurve();

    double initialHeatRecoveryHeatingCapacityFraction() const;

    bool setInitialHeatRecoveryHeatingCapacityFraction(double initialHeatRecoveryHeatingCapacityFraction);

    double heatRecoveryHeatingCapacityTimeConstant() const;

    bool setHeatRecoveryHeatingCapacityTimeConstant(double heatRecoveryHeatingCapacityTimeConstant);

    boost::optional<Curve> heatRecoveryHeatingEnergyModifierCurve() const;

    bool setHeatRecoveryHeatingEnergyModifierCurve(const Curve& curve);

    void resetHeatRecoveryHeatingEnergyModifierCurve();

    double initialHeatRecoveryHeatingEnergyFraction() const;

    bool setInitialHeatRecoveryHeatingEnergyFraction(double initialHeatRecoveryHeatingEnergyFraction);

    double heatRecoveryHeatingEnergyTimeConstant() const;

    bool setHeatRecoveryHeatingEnergyTimeConstant(double heatRecoveryHeatingEnergyTimeConstant);

    bool addTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf);

    void removeTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf);

    void removeAllTerminals();

    std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> terminals() const;

    boost::optional<double> autosizedGrossRatedTotalCoolingCapacity() const;
    boost::optional<double> autosizedGrossRatedHeatingCapacity() const;

    // Deprecated
    OS_DEPRECATED boost::optional<double> autosizedRatedTotalCoolingCapacity() const;
    OS_DEPRECATED boost::optional<double> autosizedRatedTotalHeatingCapacity() const;

    boost::optional<double> autosizedResistiveDefrostHeaterCapacity() const;

    boost::optional<double> autosizedWaterCondenserVolumeFlowRate() const;

    boost::optional<double> autosizedEvaporativeCondenserAirFlowRate() const;

    boost::optional<double> autosizedEvaporativeCondenserPumpRatedPowerConsumption() const;

   protected:
    /// @cond
    using ImplType = detail::AirConditionerVariableRefrigerantFlow_Impl;

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
  using OptionalAirConditionerVariableRefrigerantFlow = boost::optional<AirConditionerVariableRefrigerantFlow>;

  /** \relates AirConditionerVariableRefrigerantFlow*/
  using AirConditionerVariableRefrigerantFlowVector = std::vector<AirConditionerVariableRefrigerantFlow>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOW_HPP
