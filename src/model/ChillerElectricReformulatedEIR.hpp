/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CHILLERELECTRICREFORMULATEDEIR_HPP
#define MODEL_CHILLERELECTRICREFORMULATEDEIR_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

  class Curve;
  class Schedule;
  class Node;

  namespace detail {

    class ChillerElectricReformulatedEIR_Impl;

  }  // namespace detail

  /** ChillerElectricReformulatedEIR is a StraightComponent that wraps the OpenStudio IDD object
 *  'OS:Chiller:Electric:EIR'. */
  class MODEL_API ChillerElectricReformulatedEIR : public WaterToWaterComponent
  {
   public:
    /**
   *
   *\param model the Model to put the chiller in
   *\param CCFofT Cooling Capacity as a Function of Temp
   *\param EItoCORFofT Electric Input to Cooling Output Ratio as a Function of Temp
   *\param EItoCORFofPLR Electric Input to Cooling Output Ratio as a Function of Partial Load Ratio
   */
    explicit ChillerElectricReformulatedEIR(const Model& model, const Curve& CCFofT, const Curve& EItoCORFofT, const Curve& EItoCORFofPLR);

    explicit ChillerElectricReformulatedEIR(const Model& model);

    virtual ~ChillerElectricReformulatedEIR() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ChillerElectricReformulatedEIR(const ChillerElectricReformulatedEIR& other) = default;
    ChillerElectricReformulatedEIR(ChillerElectricReformulatedEIR&& other) = default;
    ChillerElectricReformulatedEIR& operator=(const ChillerElectricReformulatedEIR&) = default;
    ChillerElectricReformulatedEIR& operator=(ChillerElectricReformulatedEIR&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validChillerFlowModeValues();

    static std::vector<std::string> validCondenserFlowControlValues();

    static std::vector<std::string> validElectricInputToCoolingOutputRatioFunctionOfPLRTypeValues();

    /** @name Getters */
    //@{

    boost::optional<double> referenceCapacity() const;

    bool isReferenceCapacityAutosized() const;

    double referenceCOP() const;

    double referenceLeavingChilledWaterTemperature() const;

    bool isReferenceLeavingChilledWaterTemperatureDefaulted() const;

    double referenceLeavingCondenserWaterTemperature() const;

    bool isReferenceLeavingCondenserWaterTemperatureDefaulted() const;

    boost::optional<double> referenceChilledWaterFlowRate() const;

    bool isReferenceChilledWaterFlowRateAutosized() const;

    boost::optional<double> referenceCondenserWaterFlowRate() const;

    bool isReferenceCondenserWaterFlowRateAutosized() const;

    Curve coolingCapacityFunctionOfTemperature() const;

    Curve electricInputToCoolingOutputRatioFunctionOfTemperature() const;

    std::string electricInputToCoolingOutputRatioFunctionOfPLRType() const;

    Curve electricInputToCoolingOutputRatioFunctionOfPLR() const;

    double minimumPartLoadRatio() const;

    bool isMinimumPartLoadRatioDefaulted() const;

    double maximumPartLoadRatio() const;

    bool isMaximumPartLoadRatioDefaulted() const;

    double optimumPartLoadRatio() const;

    bool isOptimumPartLoadRatioDefaulted() const;

    double minimumUnloadingRatio() const;

    bool isMinimumUnloadingRatioDefaulted() const;

    double fractionofCompressorElectricConsumptionRejectedbyCondenser() const;

    bool isFractionofCompressorElectricConsumptionRejectedbyCondenserDefaulted() const;

    double leavingChilledWaterLowerTemperatureLimit() const;

    bool isLeavingChilledWaterLowerTemperatureLimitDefaulted() const;

    std::string chillerFlowMode() const;

    bool isChillerFlowModeDefaulted() const;

    boost::optional<double> designHeatRecoveryWaterFlowRate() const;

    bool isDesignHeatRecoveryWaterFlowRateAutosized() const;

    double sizingFactor() const;

    bool isSizingFactorDefaulted() const;

    double condenserHeatRecoveryRelativeCapacityFraction() const;

    boost::optional<Schedule> heatRecoveryInletHighTemperatureLimitSchedule() const;

    boost::optional<Node> heatRecoveryLeavingTemperatureSetpointNode() const;

    std::string endUseSubcategory() const;

    std::string condenserFlowControl() const;

    boost::optional<Curve> condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve() const;

    boost::optional<Schedule> temperatureDifferenceAcrossCondenserSchedule() const;

    double condenserMinimumFlowFraction() const;

    boost::optional<Curve> thermosiphonCapacityFractionCurve() const;

    double thermosiphonMinimumTemperatureDifference() const;

    //@}
    /** @name Setters */
    //@{

    bool setReferenceCapacity(boost::optional<double> referenceCapacity);

    bool setReferenceCapacity(double referenceCapacity);

    void autosizeReferenceCapacity();

    bool setReferenceCOP(double referenceCOP);

    bool setReferenceLeavingChilledWaterTemperature(double referenceLeavingChilledWaterTemperature);

    void resetReferenceLeavingChilledWaterTemperature();

    bool setReferenceLeavingCondenserWaterTemperature(double referenceLeavingCondenserWaterTemperature);

    void resetReferenceLeavingCondenserWaterTemperature();

    bool setReferenceChilledWaterFlowRate(boost::optional<double> referenceChilledWaterFlowRate);

    bool setReferenceChilledWaterFlowRate(double referenceChilledWaterFlowRate);

    void resetReferenceChilledWaterFlowRate();

    void autosizeReferenceChilledWaterFlowRate();

    bool setReferenceCondenserWaterFlowRate(boost::optional<double> referenceCondenserWaterFlowRate);

    bool setReferenceCondenserWaterFlowRate(double referenceCondenserWaterFlowRate);

    void resetReferenceCondenserWaterFlowRate();

    void autosizeReferenceCondenserWaterFlowRate();

    bool setCoolingCapacityFunctionOfTemperature(const Curve&);

    bool setElectricInputToCoolingOutputRatioFunctionOfTemperature(const Curve&);

    bool setElectricInputToCoolingOutputRatioFunctionOfPLRType(const std::string& electricInputToCoolingOutputRatioFunctionOfPLRType);

    void resetElectricInputToCoolingOutputRatioFunctionOfPLRType();

    bool setElectricInputToCoolingOutputRatioFunctionOfPLR(const Curve&);

    bool setMinimumPartLoadRatio(double minimumPartLoadRatio);

    void resetMinimumPartLoadRatio();

    bool setMaximumPartLoadRatio(double maximumPartLoadRatio);

    void resetMaximumPartLoadRatio();

    bool setOptimumPartLoadRatio(double optimumPartLoadRatio);

    void resetOptimumPartLoadRatio();

    bool setMinimumUnloadingRatio(double minimumUnloadingRatio);

    void resetMinimumUnloadingRatio();

    bool setFractionofCompressorElectricConsumptionRejectedbyCondenser(double fractionofCompressorElectricConsumptionRejectedbyCondenser);

    void resetFractionofCompressorElectricConsumptionRejectedbyCondenser();

    bool setLeavingChilledWaterLowerTemperatureLimit(double leavingChilledWaterLowerTemperatureLimit);

    void resetLeavingChilledWaterLowerTemperatureLimit();

    bool setChillerFlowMode(const std::string& chillerFlowMode);

    void resetChillerFlowMode();

    bool setDesignHeatRecoveryWaterFlowRate(double designHeatRecoveryWaterFlowRate);

    void autosizeDesignHeatRecoveryWaterFlowRate();

    bool setSizingFactor(double sizingFactor);

    void resetSizingFactor();

    bool setCondenserHeatRecoveryRelativeCapacityFraction(double condenserHeatRecoveryRelativeCapacityFraction);

    bool setHeatRecoveryInletHighTemperatureLimitSchedule(Schedule& s);
    void resetHeatRecoveryInletHighTemperatureLimitSchedule();

    bool setHeatRecoveryLeavingTemperatureSetpointNode(const Node& node);
    void resetHeatRecoveryLeavingTemperatureSetpointNode();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    bool setCondenserFlowControl(const std::string& condenserFlowControl);

    bool setCondenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve(const Curve& condenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve);
    void resetCondenserLoopFlowRateFractionFunctionofLoopPartLoadRatioCurve();

    bool setTemperatureDifferenceAcrossCondenserSchedule(Schedule& temperatureDifferenceAcrossCondenserSchedule);
    void resetTemperatureDifferenceAcrossCondenserSchedule();

    bool setCondenserMinimumFlowFraction(double condenserMinimumFlowFraction);

    bool setThermosiphonCapacityFractionCurve(const Curve& thermosiphonCapacityFractionCurve);
    void resetThermosiphonCapacityFractionCurve();

    bool setThermosiphonMinimumTemperatureDifference(double thermosiphonMinimumTemperatureDifference);

    //@}
    /** @name Other */
    //@{
    //

    boost::optional<double> autosizedReferenceCapacity() const;

    boost::optional<double> autosizedReferenceChilledWaterFlowRate() const;

    boost::optional<double> autosizedReferenceCondenserWaterFlowRate() const;

    boost::optional<double> autosizedDesignHeatRecoveryWaterFlowRate() const;

    /** Convenience Function to return the Chilled Water Loop (chiller on supply) **/
    boost::optional<PlantLoop> chilledWaterLoop() const;
    // Same as supplyInletModelObject, but cast to a Node
    boost::optional<Node> chilledWaterInletNode() const;
    boost::optional<Node> chilledWaterOutletNode() const;

    /** Convenience Function to return the Condenser Water Loop (chiller on demand side) **/
    boost::optional<PlantLoop> condenserWaterLoop() const;
    boost::optional<Node> condenserInletNode() const;
    boost::optional<Node> condenserOutletNode() const;

    /** Convenience Function to return the Heat Recovery Loop (chiller on demand side - tertiary) **/
    boost::optional<PlantLoop> heatRecoveryLoop() const;
    boost::optional<Node> heatRecoveryInletNode() const;
    boost::optional<Node> heatRecoveryOutletNode() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ChillerElectricReformulatedEIR_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit ChillerElectricReformulatedEIR(std::shared_ptr<detail::ChillerElectricReformulatedEIR_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ChillerElectricReformulatedEIR");
  };

  /** \relates ChillerElectricReformulatedEIR*/
  using OptionalChillerElectricReformulatedEIR = boost::optional<ChillerElectricReformulatedEIR>;

  /** \relates ChillerElectricReformulatedEIR*/
  using ChillerElectricReformulatedEIRVector = std::vector<ChillerElectricReformulatedEIR>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_CHILLERELECTRICREFORMULATEDEIR_HPP
