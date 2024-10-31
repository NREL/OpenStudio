/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROL_HPP
#define MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROL_HPP

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

    class AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl;

  }  // namespace detail

  /** AirConditionerVariableRefrigerantFlowFluidTemperatureControl is a HVACComponent that wraps the OpenStudio IDD object 'OS:AirConditioner:VariableRefrigerantFlow:FluidTemperatureControl'. */
  class MODEL_API AirConditionerVariableRefrigerantFlowFluidTemperatureControl : public HVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit AirConditionerVariableRefrigerantFlowFluidTemperatureControl(const Model& model);

    virtual ~AirConditionerVariableRefrigerantFlowFluidTemperatureControl() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl(const AirConditionerVariableRefrigerantFlowFluidTemperatureControl& other) = default;
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl(AirConditionerVariableRefrigerantFlowFluidTemperatureControl&& other) = default;
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl&
      operator=(const AirConditionerVariableRefrigerantFlowFluidTemperatureControl&) = default;
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl& operator=(AirConditionerVariableRefrigerantFlowFluidTemperatureControl&&) = default;

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

    double minimumOutdoorAirTemperatureinCoolingMode() const;

    double maximumOutdoorAirTemperatureinCoolingMode() const;

    double minimumOutdoorAirTemperatureinHeatingMode() const;

    double maximumOutdoorAirTemperatureinHeatingMode() const;

    double referenceOutdoorUnitSuperheating() const;

    double referenceOutdoorUnitSubcooling() const;

    std::string refrigerantTemperatureControlAlgorithmforIndoorUnit() const;

    double referenceEvaporatingTemperatureforIndoorUnit() const;

    double referenceCondensingTemperatureforIndoorUnit() const;

    double variableEvaporatingTemperatureMinimumforIndoorUnit() const;

    double variableEvaporatingTemperatureMaximumforIndoorUnit() const;

    double variableCondensingTemperatureMinimumforIndoorUnit() const;

    double variableCondensingTemperatureMaximumforIndoorUnit() const;

    double outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity() const;

    double outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity() const;

    Curve outdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve() const;

    Curve outdoorUnitCondensingTemperatureFunctionofSubcoolingCurve() const;

    double diameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint() const;

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

    double compressorMaximumDeltaPressure() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setRefrigerantType(const std::string& refrigerantType);

    bool setRatedEvaporativeCapacity(double ratedEvaporativeCapacity);

    void autosizeRatedEvaporativeCapacity();

    bool setRatedCompressorPowerPerUnitofRatedEvaporativeCapacity(double ratedCompressorPowerPerUnitofRatedEvaporativeCapacity);

    bool setMinimumOutdoorAirTemperatureinCoolingMode(double minimumOutdoorAirTemperatureinCoolingMode);

    bool setMaximumOutdoorAirTemperatureinCoolingMode(double maximumOutdoorAirTemperatureinCoolingMode);

    bool setMinimumOutdoorAirTemperatureinHeatingMode(double minimumOutdoorAirTemperatureinHeatingMode);

    bool setMaximumOutdoorAirTemperatureinHeatingMode(double maximumOutdoorAirTemperatureinHeatingMode);

    bool setReferenceOutdoorUnitSuperheating(double referenceOutdoorUnitSuperheating);

    bool setReferenceOutdoorUnitSubcooling(double referenceOutdoorUnitSubcooling);

    bool setRefrigerantTemperatureControlAlgorithmforIndoorUnit(const std::string& refrigerantTemperatureControlAlgorithmforIndoorUnit);

    bool setReferenceEvaporatingTemperatureforIndoorUnit(double referenceEvaporatingTemperatureforIndoorUnit);

    bool setReferenceCondensingTemperatureforIndoorUnit(double referenceCondensingTemperatureforIndoorUnit);

    bool setVariableEvaporatingTemperatureMinimumforIndoorUnit(double variableEvaporatingTemperatureMinimumforIndoorUnit);

    bool setVariableEvaporatingTemperatureMaximumforIndoorUnit(double variableEvaporatingTemperatureMaximumforIndoorUnit);

    bool setVariableCondensingTemperatureMinimumforIndoorUnit(double variableCondensingTemperatureMinimumforIndoorUnit);

    bool setVariableCondensingTemperatureMaximumforIndoorUnit(double variableCondensingTemperatureMaximumforIndoorUnit);

    bool setOutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity(double outdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity);

    bool setOutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity(double outdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity);

    bool setOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve(const Curve& curve);

    bool setOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve(const Curve& curve);

    bool setDiameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(double diameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint);

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

    bool setCompressorMaximumDeltaPressure(double compressorMaximumDeltaPressure);

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
    using ImplType = detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl;

    explicit AirConditionerVariableRefrigerantFlowFluidTemperatureControl(
      std::shared_ptr<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl> impl);

    friend class detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.AirConditionerVariableRefrigerantFlowFluidTemperatureControl");
  };

  /** \relates AirConditionerVariableRefrigerantFlowFluidTemperatureControl*/
  using OptionalAirConditionerVariableRefrigerantFlowFluidTemperatureControl =
    boost::optional<AirConditionerVariableRefrigerantFlowFluidTemperatureControl>;

  /** \relates AirConditionerVariableRefrigerantFlowFluidTemperatureControl*/
  using AirConditionerVariableRefrigerantFlowFluidTemperatureControlVector =
    std::vector<AirConditionerVariableRefrigerantFlowFluidTemperatureControl>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRCONDITIONERVARIABLEREFRIGERANTFLOWFLUIDTEMPERATURECONTROL_HPP
