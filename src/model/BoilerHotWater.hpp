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

#ifndef MODEL_BOILERHOTWATER_HPP
#define MODEL_BOILERHOTWATER_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Curve;

  namespace detail {

    class BoilerHotWater_Impl;

  }  // namespace detail

  /** BoilerHotWater is a StraightComponent that wraps the OpenStudio IDD object
 *  'OS:Boiler:HotWater'. */
  class MODEL_API BoilerHotWater : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit BoilerHotWater(const Model& model);

    virtual ~BoilerHotWater() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    BoilerHotWater(const BoilerHotWater& other) = default;
    BoilerHotWater(BoilerHotWater&& other) = default;
    BoilerHotWater& operator=(const BoilerHotWater&) = default;
    BoilerHotWater& operator=(BoilerHotWater&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validFuelTypeValues();

    static std::vector<std::string> validEfficiencyCurveTemperatureEvaluationVariableValues();

    static std::vector<std::string> validBoilerFlowModeValues();

    /** @name Getters */
    //@{

    std::string fuelType() const;

    boost::optional<double> nominalCapacity() const;

    bool isNominalCapacityAutosized() const;

    double nominalThermalEfficiency() const;

    boost::optional<std::string> efficiencyCurveTemperatureEvaluationVariable() const;

    boost::optional<Curve> normalizedBoilerEfficiencyCurve() const;

    boost::optional<double> designWaterFlowRate() const;

    bool isDesignWaterFlowRateAutosized() const;

    double minimumPartLoadRatio() const;

    bool isMinimumPartLoadRatioDefaulted() const;

    double maximumPartLoadRatio() const;

    bool isMaximumPartLoadRatioDefaulted() const;

    double optimumPartLoadRatio() const;

    bool isOptimumPartLoadRatioDefaulted() const;

    double waterOutletUpperTemperatureLimit() const;

    bool isWaterOutletUpperTemperatureLimitDefaulted() const;

    std::string boilerFlowMode() const;

    bool isBoilerFlowModeDefaulted() const;

    boost::optional<double> parasiticElectricLoad() const;

    double sizingFactor() const;

    bool isSizingFactorDefaulted() const;

    std::string endUseSubcategory() const;

    //@}
    /** @name Setters */
    //@{

    bool setFuelType(const std::string& fuelType);

    bool setNominalCapacity(double nominalCapacity);

    void resetNominalCapacity();

    void autosizeNominalCapacity();

    bool setNominalThermalEfficiency(double nominalThermalEfficiency);

    bool setEfficiencyCurveTemperatureEvaluationVariable(const std::string& efficiencyCurveTemperatureEvaluationVariable);

    void resetEfficiencyCurveTemperatureEvaluationVariable();

    bool setNormalizedBoilerEfficiencyCurve(const Curve& normalizedBoilerEfficiencyCurve);

    void resetNormalizedBoilerEfficiencyCurve();

    bool setDesignWaterFlowRate(double designWaterFlowRate);

    void resetDesignWaterFlowRate();

    void autosizeDesignWaterFlowRate();

    bool setMinimumPartLoadRatio(double minimumPartLoadRatio);

    void resetMinimumPartLoadRatio();

    bool setMaximumPartLoadRatio(double maximumPartLoadRatio);

    void resetMaximumPartLoadRatio();

    bool setOptimumPartLoadRatio(double optimumPartLoadRatio);

    void resetOptimumPartLoadRatio();

    bool setWaterOutletUpperTemperatureLimit(double waterOutletUpperTemperatureLimit);

    void resetWaterOutletUpperTemperatureLimit();

    bool setBoilerFlowMode(const std::string& boilerFlowMode);

    void resetBoilerFlowMode();

    bool setParasiticElectricLoad(double parasiticElectricLoad);

    void resetParasiticElectricLoad();

    bool setSizingFactor(double sizingFactor);

    void resetSizingFactor();

    boost::optional<double> autosizedNominalCapacity() const;

    boost::optional<double> autosizedDesignWaterFlowRate() const;

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    //@}
   protected:
    /// @cond
    using ImplType = detail::BoilerHotWater_Impl;

    friend class detail::BoilerHotWater_Impl;
    friend class Model;
    friend class openstudio::detail::IdfObject_Impl;
    friend class IdfObject;

    explicit BoilerHotWater(std::shared_ptr<detail::BoilerHotWater_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.BoilerHotWater");
  };

  /** \relates BoilerHotWater*/
  using OptionalBoilerHotWater = boost::optional<BoilerHotWater>;

  /** \relates BoilerHotWater*/
  using BoilerHotWaterVector = std::vector<BoilerHotWater>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_BOILERHOTWATER_HPP
