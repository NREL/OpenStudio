/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_BOILERHOTWATER_HPP
#define MODEL_BOILERHOTWATER_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"
#include "../utilities/core/Deprecated.hpp"

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

    virtual ~BoilerHotWater() override = default;
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

    OS_DEPRECATED(3, 7, 0) boost::optional<double> parasiticElectricLoad() const;

    double onCycleParasiticElectricLoad() const;

    double sizingFactor() const;

    bool isSizingFactorDefaulted() const;

    std::string endUseSubcategory() const;

    double offCycleParasiticFuelLoad() const;

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

    OS_DEPRECATED(3, 7, 0) bool setParasiticElectricLoad(double parasiticElectricLoad);

    bool setOnCycleParasiticElectricLoad(double onCyclearasiticElectricLoad);

    OS_DEPRECATED(3, 7, 0) void resetParasiticElectricLoad();

    bool setSizingFactor(double sizingFactor);

    void resetSizingFactor();

    boost::optional<double> autosizedNominalCapacity() const;

    boost::optional<double> autosizedDesignWaterFlowRate() const;

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    bool setOffCycleParasiticFuelLoad(double offCycleParasiticFuelLoad);

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
