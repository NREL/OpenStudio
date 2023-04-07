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

    virtual ~ChillerHeaterPerformanceElectricEIR() = default;
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
