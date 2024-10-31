/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROLHR_HPP
#define MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROLHR_HPP

#include <vector>
#include <model/ModelAPI.hpp>
#include "HVACComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Curve;
  class ThermalZone;
  class ZoneHVACTerminalUnitVariableRefrigerantFlow;
  class LoadingIndex;

  namespace detail {

    class AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl;

  }  // namespace detail

  /** AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR is a HVACComponent that wraps the OpenStudio IDD object 'OS:AirConditioner:VariableRefrigerantFlow:FluidTemperatureControl:HR'. */
  class MODEL_API AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR : public HVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR(const Model& model);

    virtual ~AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR(const AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR& other) =
      default;
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR(AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR&& other) = default;
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR&
      operator=(const AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR&) = default;
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR&
      operator=(AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> refrigerantTypeValues();
    static std::vector<std::string> validRefrigerantTypeValues();

    static std::vector<std::string> refrigerantTemperatureControlAlgorithmforIndoorUnitValues();
    static std::vector<std::string> validRefrigerantTemperatureControlAlgorithmforIndoorUnitValues();

    static std::vector<std::string> defrostStrategyValues();
    static std::vector<std::string> validDefrostStrategyValues();

    static std::vector<std::string> defrostControlValues();
    static std::vector<std::string> validDefrostControlValues();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    std::string refrigerantType() const;

    boost::optional<double> ratedEvaporativeCapacity() const;

    bool isRatedEvaporativeCapacityAutosized() const;

    double ratedCompressorPowerPerUnitofRatedEvaporativeCapacity() const;

    double minimumOutdoorAirTemperatureinCoolingOnlyMode() const;

    double maximumOutdoorAirTemperatureinCoolingOnlyMode() const;

    double minimumOutdoorAirTemperatureinHeatingOnlyMode() const;

    double maximumOutdoorAirTemperatureinHeatingOnlyMode() const;

    double minimumOutdoorTemperatureinHeatRecoveryMode() const;

    double maximumOutdoorTemperatureinHeatRecoveryMode() const;

    std::string refrigerantTemperatureControlAlgorithmforIndoorUnit() const;

    double referenceEvaporatingTemperatureforIndoorUnit() const;

    double referenceCondensingTemperatureforIndoorUnit() const;

    double variableEvaporatingTemperatureMinimumforIndoorUnit() const;

    double variableEvaporatingTemperatureMaximumforIndoorUnit() const;

    double variableCondensingTemperatureMinimumforIndoorUnit() const;

    double variableCondensingTemperatureMaximumforIndoorUnit() const;

    double outdoorUnitEvaporatorReferenceSuperheating() const;

    double outdoorUnitCondenserReferenceSubcooling() const;

    double outdoorUnitEvaporatorRatedBypassFactor() const;

    double outdoorUnitCondenserRatedBypassFactor() const;

    double differencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode() const;

    double outdoorUnitHeatExchangerCapacityRatio() const;

    double outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity() const;

    double outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity() const;

    Curve outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve() const;

    Curve outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve() const;

    double diameterofMainPipeforSuctionGas() const;

    double diameterofMainPipeforDischargeGas() const;

    double lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint() const;

    double equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint() const;

    double heightDifferenceBetweenOutdoorUnitandIndoorUnits() const;

    double mainPipeInsulationThickness() const;

    double mainPipeInsulationThermalConductivity() const;

    double crankcaseHeaterPowerperCompressor() const;

    int numberofCompressors() const;

    double ratioofCompressorSizetoTotalCompressorCapacity() const;

    double maximumOutdoorDryBulbTemperatureforCrankcaseHeater() const;

    std::string defrostStrategy() const;

    std::string defrostControl() const;

    boost::optional<Curve> defrostEnergyInputRatioModifierFunctionofTemperatureCurve() const;

    double defrostTimePeriodFraction() const;

    boost::optional<double> resistiveDefrostHeaterCapacity() const;

    bool isResistiveDefrostHeaterCapacityAutosized() const;

    double maximumOutdoorDrybulbTemperatureforDefrostOperation() const;

    double initialHeatRecoveryCoolingCapacityFraction() const;

    double heatRecoveryCoolingCapacityTimeConstant() const;

    double initialHeatRecoveryCoolingEnergyFraction() const;

    double heatRecoveryCoolingEnergyTimeConstant() const;

    double initialHeatRecoveryHeatingCapacityFraction() const;

    double heatRecoveryHeatingCapacityTimeConstant() const;

    double initialHeatRecoveryHeatingEnergyFraction() const;

    double heatRecoveryHeatingEnergyTimeConstant() const;

    double compressorMaximumDeltaPressure() const;

    double compressorInverterEfficiency() const;

    double compressorEvaporativeCapacityCorrectionFactor() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setRefrigerantType(const std::string& refrigerantType);

    bool setRatedEvaporativeCapacity(double ratedEvaporativeCapacity);

    void autosizeRatedEvaporativeCapacity();

    bool setRatedCompressorPowerPerUnitofRatedEvaporativeCapacity(double ratedCompressorPowerPerUnitofRatedEvaporativeCapacity);

    bool setMinimumOutdoorAirTemperatureinCoolingOnlyMode(double minimumOutdoorAirTemperatureinCoolingOnlyMode);

    bool setMaximumOutdoorAirTemperatureinCoolingOnlyMode(double maximumOutdoorAirTemperatureinCoolingOnlyMode);

    bool setMinimumOutdoorAirTemperatureinHeatingOnlyMode(double minimumOutdoorAirTemperatureinHeatingOnlyMode);

    bool setMaximumOutdoorAirTemperatureinHeatingOnlyMode(double maximumOutdoorAirTemperatureinHeatingOnlyMode);

    bool setMinimumOutdoorTemperatureinHeatRecoveryMode(double minimumOutdoorTemperatureinHeatRecoveryMode);

    bool setMaximumOutdoorTemperatureinHeatRecoveryMode(double maximumOutdoorTemperatureinHeatRecoveryMode);

    bool setRefrigerantTemperatureControlAlgorithmforIndoorUnit(const std::string& refrigerantTemperatureControlAlgorithmforIndoorUnit);

    bool setReferenceEvaporatingTemperatureforIndoorUnit(double referenceEvaporatingTemperatureforIndoorUnit);

    bool setReferenceCondensingTemperatureforIndoorUnit(double referenceCondensingTemperatureforIndoorUnit);

    bool setVariableEvaporatingTemperatureMinimumforIndoorUnit(double variableEvaporatingTemperatureMinimumforIndoorUnit);

    bool setVariableEvaporatingTemperatureMaximumforIndoorUnit(double variableEvaporatingTemperatureMaximumforIndoorUnit);

    bool setVariableCondensingTemperatureMinimumforIndoorUnit(double variableCondensingTemperatureMinimumforIndoorUnit);

    bool setVariableCondensingTemperatureMaximumforIndoorUnit(double variableCondensingTemperatureMaximumforIndoorUnit);

    bool setOutdoorUnitEvaporatorReferenceSuperheating(double outdoorUnitEvaporatorReferenceSuperheating);

    bool setOutdoorUnitCondenserReferenceSubcooling(double outdoorUnitCondenserReferenceSubcooling);

    bool setOutdoorUnitEvaporatorRatedBypassFactor(double outdoorUnitEvaporatorRatedBypassFactor);

    bool setOutdoorUnitCondenserRatedBypassFactor(double outdoorUnitCondenserRatedBypassFactor);

    bool setDifferencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode(
      double differencebetweenOutdoorUnitEvaporatingTemperatureandOutdoorAirTemperatureinHeatRecoveryMode);

    bool setOutdoorUnitHeatExchangerCapacityRatio(double outdoorUnitHeatExchangerCapacityRatio);

    bool setOutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity(double outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity);

    bool setOutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity(double outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity);

    bool setOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(const Curve& curve);

    bool setOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve(const Curve& curve);

    bool setDiameterofMainPipeforSuctionGas(double diameterofMainPipeforSuctionGas);

    bool setDiameterofMainPipeforDischargeGas(double diameterofMainPipeforDischargeGas);

    bool setLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(double lengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);

    bool setEquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(
      double equivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);

    bool setHeightDifferenceBetweenOutdoorUnitandIndoorUnits(double heightDifferenceBetweenOutdoorUnitandIndoorUnits);

    bool setMainPipeInsulationThickness(double mainPipeInsulationThickness);

    bool setMainPipeInsulationThermalConductivity(double mainPipeInsulationThermalConductivity);

    bool setCrankcaseHeaterPowerperCompressor(double crankcaseHeaterPowerperCompressor);

    bool setNumberofCompressors(int numberofCompressors);

    bool setRatioofCompressorSizetoTotalCompressorCapacity(double ratioofCompressorSizetoTotalCompressorCapacity);

    bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeater(double maximumOutdoorDryBulbTemperatureforCrankcaseHeater);

    bool setDefrostStrategy(const std::string& defrostStrategy);

    bool setDefrostControl(const std::string& defrostControl);

    bool setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(const Curve& curve);

    void resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve();

    bool setDefrostTimePeriodFraction(double defrostTimePeriodFraction);

    bool setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity);

    void autosizeResistiveDefrostHeaterCapacity();

    bool setMaximumOutdoorDrybulbTemperatureforDefrostOperation(double maximumOutdoorDrybulbTemperatureforDefrostOperation);

    bool setInitialHeatRecoveryCoolingCapacityFraction(double initialHeatRecoveryCoolingCapacityFraction);

    bool setHeatRecoveryCoolingCapacityTimeConstant(double heatRecoveryCoolingCapacityTimeConstant);

    bool setInitialHeatRecoveryCoolingEnergyFraction(double initialHeatRecoveryCoolingEnergyFraction);

    bool setHeatRecoveryCoolingEnergyTimeConstant(double heatRecoveryCoolingEnergyTimeConstant);

    bool setInitialHeatRecoveryHeatingCapacityFraction(double initialHeatRecoveryHeatingCapacityFraction);

    bool setHeatRecoveryHeatingCapacityTimeConstant(double heatRecoveryHeatingCapacityTimeConstant);

    bool setInitialHeatRecoveryHeatingEnergyFraction(double initialHeatRecoveryHeatingEnergyFraction);

    bool setHeatRecoveryHeatingEnergyTimeConstant(double heatRecoveryHeatingEnergyTimeConstant);

    bool setCompressorMaximumDeltaPressure(double compressorMaximumDeltaPressure);

    bool setCompressorInverterEfficiency(double compressorInverterEfficiency);

    bool setCompressorEvaporativeCapacityCorrectionFactor(double compressorEvaporativeCapacityCorrectionFactor);

    //@}
    /** @name Other */
    //@{

    bool addTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf);

    void removeTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf);

    void removeAllTerminals();

    std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> terminals() const;

    bool addLoadingIndex(LoadingIndex& loadingIndex);

    void removeLoadingIndex(LoadingIndex& loadingIndex);

    void removeAllLoadingIndexes();

    std::vector<LoadingIndex> loadingIndexes() const;

    boost::optional<double> autosizedRatedEvaporativeCapacity();

    boost::optional<double> autosizedResistiveDefrostHeaterCapacity();

    boost::optional<double> autosizedGrossRatedTotalCoolingCapacity() const;

    boost::optional<double> autosizedGrossRatedHeatingCapacity() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl;

    explicit AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR(
      std::shared_ptr<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl> impl);

    friend class detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR");
  };

  /** \relates AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR*/
  using OptionalAirConditionerVariableRefrigerantFlowFluidTemperatureControlHR =
    boost::optional<AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR>;

  /** \relates AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR*/
  using AirConditionerVariableRefrigerantFlowFluidTemperatureControlHRVector =
    std::vector<AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROLHR_HPP
