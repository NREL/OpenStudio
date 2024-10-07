/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CHILLERHEATERPERFORMANCEELECTRICEIR_HPP
#define MODEL_CHILLERHEATERPERFORMANCEELECTRICEIR_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

  class Curve;
  class CentralHeatPumpSystem;
  class CentralHeatPumpSystemModule;

  namespace detail {

    class ChillerHeaterPerformanceElectricEIR_Impl;

  }  // namespace detail

  /** ChillerHeaterPerformanceElectricEIR is a ParentObject that wraps the OpenStudio IDD object 'OS:ChillerHeaterPerformance:Electric:EIR'. */
  class MODEL_API ChillerHeaterPerformanceElectricEIR : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ChillerHeaterPerformanceElectricEIR(const Model& model);

    explicit ChillerHeaterPerformanceElectricEIR(const Model& model, const Curve& chillerHeaterClgCapFT, const Curve& chillerHeaterClgEIRFT,
                                                 const Curve& chillerHeaterClgEIRFPLR, const Curve& chillerHeaterHtgCapFT,
                                                 const Curve& chillerHeaterHtgEIRFT, const Curve& chillerHeaterHtgEIRFPLR);

    virtual ~ChillerHeaterPerformanceElectricEIR() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ChillerHeaterPerformanceElectricEIR(const ChillerHeaterPerformanceElectricEIR& other) = default;
    ChillerHeaterPerformanceElectricEIR(ChillerHeaterPerformanceElectricEIR&& other) = default;
    ChillerHeaterPerformanceElectricEIR& operator=(const ChillerHeaterPerformanceElectricEIR&) = default;
    ChillerHeaterPerformanceElectricEIR& operator=(ChillerHeaterPerformanceElectricEIR&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> chilledWaterFlowModeTypeValues();

    static std::vector<std::string> condenserTypeValues();

    static std::vector<std::string> coolingModeTemperatureCurveCondenserWaterIndependentVariableValues();

    static std::vector<std::string> heatingModeTemperatureCurveCondenserWaterIndependentVariableValues();

    /** @name Getters */
    //@{

    boost::optional<double> referenceCoolingModeEvaporatorCapacity() const;

    bool isReferenceCoolingModeEvaporatorCapacityAutosized() const;

    double referenceCoolingModeCOP() const;

    double referenceCoolingModeLeavingChilledWaterTemperature() const;

    double referenceCoolingModeEnteringCondenserFluidTemperature() const;

    double referenceCoolingModeLeavingCondenserWaterTemperature() const;

    double referenceHeatingModeCoolingCapacityRatio() const;

    double referenceHeatingModeCoolingPowerInputRatio() const;

    double referenceHeatingModeLeavingChilledWaterTemperature() const;

    double referenceHeatingModeLeavingCondenserWaterTemperature() const;

    double referenceHeatingModeEnteringCondenserFluidTemperature() const;

    double heatingModeEnteringChilledWaterTemperatureLowLimit() const;

    std::string chilledWaterFlowModeType() const;

    boost::optional<double> designChilledWaterFlowRate() const;

    bool isDesignChilledWaterFlowRateAutosized() const;

    boost::optional<double> designCondenserWaterFlowRate() const;

    bool isDesignCondenserWaterFlowRateAutosized() const;

    double designHotWaterFlowRate() const;

    double compressorMotorEfficiency() const;

    std::string condenserType() const;

    std::string coolingModeTemperatureCurveCondenserWaterIndependentVariable() const;

    Curve coolingModeCoolingCapacityFunctionOfTemperatureCurve() const;

    Curve coolingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve() const;

    Curve coolingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve() const;

    double coolingModeCoolingCapacityOptimumPartLoadRatio() const;

    std::string heatingModeTemperatureCurveCondenserWaterIndependentVariable() const;

    Curve heatingModeCoolingCapacityFunctionOfTemperatureCurve() const;

    Curve heatingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve() const;

    Curve heatingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve() const;

    double heatingModeCoolingCapacityOptimumPartLoadRatio() const;

    double sizingFactor() const;

    //@}
    /** @name Setters */
    //@{

    bool setReferenceCoolingModeEvaporatorCapacity(double referenceCoolingModeEvaporatorCapacity);

    void autosizeReferenceCoolingModeEvaporatorCapacity();

    bool setReferenceCoolingModeCOP(double referenceCoolingModeCOP);

    bool setReferenceCoolingModeLeavingChilledWaterTemperature(double referenceCoolingModeLeavingChilledWaterTemperature);

    bool setReferenceCoolingModeEnteringCondenserFluidTemperature(double referenceCoolingModeEnteringCondenserFluidTemperature);

    bool setReferenceCoolingModeLeavingCondenserWaterTemperature(double referenceCoolingModeLeavingCondenserWaterTemperature);

    bool setReferenceHeatingModeCoolingCapacityRatio(double referenceHeatingModeCoolingCapacityRatio);

    bool setReferenceHeatingModeCoolingPowerInputRatio(double referenceHeatingModeCoolingPowerInputRatio);

    bool setReferenceHeatingModeLeavingChilledWaterTemperature(double referenceHeatingModeLeavingChilledWaterTemperature);

    bool setReferenceHeatingModeLeavingCondenserWaterTemperature(double referenceHeatingModeLeavingCondenserWaterTemperature);

    bool setReferenceHeatingModeEnteringCondenserFluidTemperature(double referenceHeatingModeEnteringCondenserFluidTemperature);

    bool setHeatingModeEnteringChilledWaterTemperatureLowLimit(double heatingModeEnteringChilledWaterTemperatureLowLimit);

    bool setChilledWaterFlowModeType(const std::string& chilledWaterFlowModeType);

    bool setDesignChilledWaterFlowRate(double designChilledWaterFlowRate);

    void resetDesignChilledWaterFlowRate();

    void autosizeDesignChilledWaterFlowRate();

    bool setDesignCondenserWaterFlowRate(double designCondenserWaterFlowRate);

    void resetDesignCondenserWaterFlowRate();

    void autosizeDesignCondenserWaterFlowRate();

    bool setDesignHotWaterFlowRate(double designHotWaterFlowRate);

    bool setCompressorMotorEfficiency(double compressorMotorEfficiency);

    bool setCondenserType(const std::string& condenserType);

    bool setCoolingModeTemperatureCurveCondenserWaterIndependentVariable(
      const std::string& coolingModeTemperatureCurveCondenserWaterIndependentVariable);

    bool setCoolingModeCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve);

    bool setCoolingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve(const Curve& curve);

    bool setCoolingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve(const Curve& curve);

    bool setCoolingModeCoolingCapacityOptimumPartLoadRatio(double coolingModeCoolingCapacityOptimumPartLoadRatio);

    bool setHeatingModeTemperatureCurveCondenserWaterIndependentVariable(
      const std::string& heatingModeTemperatureCurveCondenserWaterIndependentVariable);

    bool setHeatingModeCoolingCapacityFunctionOfTemperatureCurve(const Curve& curve);

    bool setHeatingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve(const Curve& curve);

    bool setHeatingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve(const Curve& curve);

    bool setHeatingModeCoolingCapacityOptimumPartLoadRatio(double heatingModeCoolingCapacityOptimumPartLoadRatio);

    bool setSizingFactor(double sizingFactor);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedReferenceCoolingModeEvaporatorCapacity() const;

    boost::optional<double> autosizedDesignChilledWaterFlowRate() const;

    boost::optional<double> autosizedDesignCondenserWaterFlowRate() const;

    void autosize();

    void applySizingValues();

    // Reverse lookups
    // Convenience functions to return parent CentralHeatPumpSystem and CentralHeatPumpSystemModule
    // I've decided that multiple CentralHeatPumpSystemModules can reference the same ChillerHeaterPerformanceElectricEIR

    /* Returns an Array of CentralHeatPumpSystemModule that reference this object as chillerHeaterModulesPerformanceComponent */
    std::vector<CentralHeatPumpSystemModule> centralHeatPumpSystemModules() const;

    /* Returns an Array of CentralHeatPumpSystem that have a CentralHeatPumpSystemModume that reference this object
   * as chillerHeaterModulesPerformanceComponent */
    std::vector<CentralHeatPumpSystem> centralHeatPumpSystems() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ChillerHeaterPerformanceElectricEIR_Impl;

    explicit ChillerHeaterPerformanceElectricEIR(std::shared_ptr<detail::ChillerHeaterPerformanceElectricEIR_Impl> impl);

    friend class detail::ChillerHeaterPerformanceElectricEIR_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ChillerHeaterPerformanceElectricEIR");
  };

  /** \relates ChillerHeaterPerformanceElectricEIR*/
  using OptionalChillerHeaterPerformanceElectricEIR = boost::optional<ChillerHeaterPerformanceElectricEIR>;

  /** \relates ChillerHeaterPerformanceElectricEIR*/
  using ChillerHeaterPerformanceElectricEIRVector = std::vector<ChillerHeaterPerformanceElectricEIR>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CHILLERHEATERPERFORMANCEELECTRICEIR_HPP
