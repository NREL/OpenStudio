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

#ifndef MODEL_CHILLERELECTRICEIR_HPP
#define MODEL_CHILLERELECTRICEIR_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

namespace openstudio {

namespace model {

  class Curve;
  class Schedule;
  class Node;

  namespace detail {

    class ChillerElectricEIR_Impl;

  }  // namespace detail

  /** ChillerElectricEIR is a StraightComponent that wraps the OpenStudio IDD object
 *  'OS:Chiller:Electric:EIR'. */
  class MODEL_API ChillerElectricEIR : public WaterToWaterComponent
  {
   public:
    /**
   *
   *\param model the Model to put the chiller in
   *\param CCFofT Cooling Capacity as a Function of Temp
   *\param EItoCORFofT Electric Input to Cooling Output Ratio as a Function of Temp
   *\param EItoCORFofPLR Electric Input to Cooling Output Ratio as a Function of Partial Load Ratio
   */
    explicit ChillerElectricEIR(const Model& model, const Curve& CCFofT, const Curve& EItoCORFofT, const Curve& EItoCORFofPLR);

    explicit ChillerElectricEIR(const Model& model);

    virtual ~ChillerElectricEIR() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ChillerElectricEIR(const ChillerElectricEIR& other) = default;
    ChillerElectricEIR(ChillerElectricEIR&& other) = default;
    ChillerElectricEIR& operator=(const ChillerElectricEIR&) = default;
    ChillerElectricEIR& operator=(ChillerElectricEIR&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validCondenserTypeValues();

    static std::vector<std::string> validChillerFlowModeValues();

    /** @name Getters */
    //@{

    boost::optional<double> referenceCapacity() const;

    bool isReferenceCapacityAutosized() const;

    double referenceCOP() const;

    double referenceLeavingChilledWaterTemperature() const;

    bool isReferenceLeavingChilledWaterTemperatureDefaulted() const;

    double referenceEnteringCondenserFluidTemperature() const;

    bool isReferenceEnteringCondenserFluidTemperatureDefaulted() const;

    boost::optional<double> referenceChilledWaterFlowRate() const;

    bool isReferenceChilledWaterFlowRateAutosized() const;

    boost::optional<double> referenceCondenserFluidFlowRate() const;

    bool isReferenceCondenserFluidFlowRateAutosized() const;

    Curve coolingCapacityFunctionOfTemperature() const;

    Curve electricInputToCoolingOutputRatioFunctionOfTemperature() const;

    Curve electricInputToCoolingOutputRatioFunctionOfPLR() const;

    double minimumPartLoadRatio() const;

    bool isMinimumPartLoadRatioDefaulted() const;

    double maximumPartLoadRatio() const;

    bool isMaximumPartLoadRatioDefaulted() const;

    double optimumPartLoadRatio() const;

    bool isOptimumPartLoadRatioDefaulted() const;

    double minimumUnloadingRatio() const;

    bool isMinimumUnloadingRatioDefaulted() const;

    std::string condenserType() const;

    bool isCondenserTypeDefaulted() const;

    double condenserFanPowerRatio() const;

    bool isCondenserFanPowerRatioDefaulted() const;

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

    double basinHeaterCapacity() const;

    bool isBasinHeaterCapacityDefaulted() const;

    double basinHeaterSetpointTemperature() const;

    bool isBasinHeaterSetpointTemperatureDefaulted() const;

    boost::optional<Schedule> basinHeaterSchedule() const;

    double condenserHeatRecoveryRelativeCapacityFraction() const;

    boost::optional<Schedule> heatRecoveryInletHighTemperatureLimitSchedule() const;

    boost::optional<Node> heatRecoveryLeavingTemperatureSetpointNode() const;

    std::string endUseSubcategory() const;

    //@}
    /** @name Setters */
    //@{

    bool setReferenceCapacity(boost::optional<double> referenceCapacity);

    bool setReferenceCapacity(double referenceCapacity);

    void autosizeReferenceCapacity();

    bool setReferenceCOP(double referenceCOP);

    bool setReferenceLeavingChilledWaterTemperature(double referenceLeavingChilledWaterTemperature);

    void resetReferenceLeavingChilledWaterTemperature();

    bool setReferenceEnteringCondenserFluidTemperature(double referenceEnteringCondenserFluidTemperature);

    void resetReferenceEnteringCondenserFluidTemperature();

    bool setReferenceChilledWaterFlowRate(boost::optional<double> referenceChilledWaterFlowRate);

    bool setReferenceChilledWaterFlowRate(double referenceChilledWaterFlowRate);

    void resetReferenceChilledWaterFlowRate();

    void autosizeReferenceChilledWaterFlowRate();

    bool setReferenceCondenserFluidFlowRate(boost::optional<double> referenceCondenserFluidFlowRate);

    bool setReferenceCondenserFluidFlowRate(double referenceCondenserFluidFlowRate);

    void resetReferenceCondenserFluidFlowRate();

    void autosizeReferenceCondenserFluidFlowRate();

    bool setCoolingCapacityFunctionOfTemperature(const Curve&);

    bool setElectricInputToCoolingOutputRatioFunctionOfTemperature(const Curve&);

    bool setElectricInputToCoolingOutputRatioFunctionOfPLR(const Curve&);

    bool setMinimumPartLoadRatio(double minimumPartLoadRatio);

    void resetMinimumPartLoadRatio();

    bool setMaximumPartLoadRatio(double maximumPartLoadRatio);

    void resetMaximumPartLoadRatio();

    bool setOptimumPartLoadRatio(double optimumPartLoadRatio);

    void resetOptimumPartLoadRatio();

    bool setMinimumUnloadingRatio(double minimumUnloadingRatio);

    void resetMinimumUnloadingRatio();

    bool setCondenserType(const std::string& condenserType);

    void resetCondenserType();

    bool setCondenserFanPowerRatio(double condenserFanPowerRatio);

    void resetCondenserFanPowerRatio();

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

    bool setBasinHeaterCapacity(double basinHeaterCapacity);

    void resetBasinHeaterCapacity();

    bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

    void resetBasinHeaterSetpointTemperature();

    bool setBasinHeaterSchedule(Schedule& s);

    void resetBasinHeaterSchedule();

    bool setCondenserHeatRecoveryRelativeCapacityFraction(double condenserHeatRecoveryRelativeCapacityFraction);

    bool setHeatRecoveryInletHighTemperatureLimitSchedule(Schedule& s);
    void resetHeatRecoveryInletHighTemperatureLimitSchedule();

    bool setHeatRecoveryLeavingTemperatureSetpointNode(const Node& node);
    void resetHeatRecoveryLeavingTemperatureSetpointNode();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    //@}
    /** @name Other */
    //@{
    //
    boost::optional<double> autosizedReferenceCapacity() const;

    boost::optional<double> autosizedReferenceChilledWaterFlowRate() const;

    boost::optional<double> autosizedReferenceCondenserFluidFlowRate() const;

    boost::optional<double> autosizedDesignHeatRecoveryWaterFlowRate() const;

    /** Convenience Function to return the Chilled Water Loop (chiller on supply) **/
    boost::optional<PlantLoop> chilledWaterLoop() const;

    /** Convenience Function to return the Condenser Water Loop (chiller on demand side) **/
    boost::optional<PlantLoop> condenserWaterLoop() const;

    /** Convenience Function to return the Heat Recovery Loop (chiller on demand side - tertiary) **/
    boost::optional<PlantLoop> heatRecoveryLoop() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::ChillerElectricEIR_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit ChillerElectricEIR(std::shared_ptr<detail::ChillerElectricEIR_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ChillerElectricEIR");
  };

  /** \relates ChillerElectricEIR*/
  using OptionalChillerElectricEIR = boost::optional<ChillerElectricEIR>;

  /** \relates ChillerElectricEIR*/
  using ChillerElectricEIRVector = std::vector<ChillerElectricEIR>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_CHILLERELECTRICEIR_HPP
