/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HEATPUMPWATERTOWATEREQUATIONFITCOOLING_HPP
#define MODEL_HEATPUMPWATERTOWATEREQUATIONFITCOOLING_HPP

#include "ModelAPI.hpp"
#include "WaterToWaterComponent.hpp"

#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  class HeatPumpWaterToWaterEquationFitHeating;
  class CurveQuadLinear;

  namespace detail {

    class HeatPumpWaterToWaterEquationFitCooling_Impl;

  }  // namespace detail

  /** HeatPumpWaterToWaterEquationFitCooling is a WaterToWaterComponent that wraps the OpenStudio IDD object 'OS:HeatPump:WaterToWater:EquationFit:Cooling'. */
  class MODEL_API HeatPumpWaterToWaterEquationFitCooling : public WaterToWaterComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit HeatPumpWaterToWaterEquationFitCooling(const Model& model, const CurveQuadLinear& coolingCapacityCurve,
                                                    const CurveQuadLinear& coolingCompressorPowerCurve);

    explicit HeatPumpWaterToWaterEquationFitCooling(const Model& model);

    virtual ~HeatPumpWaterToWaterEquationFitCooling() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    HeatPumpWaterToWaterEquationFitCooling(const HeatPumpWaterToWaterEquationFitCooling& other) = default;
    HeatPumpWaterToWaterEquationFitCooling(HeatPumpWaterToWaterEquationFitCooling&& other) = default;
    HeatPumpWaterToWaterEquationFitCooling& operator=(const HeatPumpWaterToWaterEquationFitCooling&) = default;
    HeatPumpWaterToWaterEquationFitCooling& operator=(HeatPumpWaterToWaterEquationFitCooling&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<double> referenceLoadSideFlowRate() const;

    /** Prior to EnergyPlus 8.7.0 this field was not autosizeable. To preserve backwards compatibility this method will return -999.0 in autosized models.**/
    double ratedLoadSideFlowRate() const;

    bool isReferenceLoadSideFlowRateAutosized() const;

    boost::optional<double> referenceSourceSideFlowRate() const;

    /** Prior to EnergyPlus 8.7.0 this field was not autosizeable. To preserve backwards compatibility this method will return -999.0 in autosized models.**/
    double ratedSourceSideFlowRate() const;

    bool isReferenceSourceSideFlowRateAutosized() const;

    /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Reference Cooling Capacity" **/
    boost::optional<double> ratedCoolingCapacity() const;

    bool isRatedCoolingCapacityAutosized() const;

    /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Reference Cooling Power Consumption" **/
    boost::optional<double> ratedCoolingPowerConsumption() const;

    bool isRatedCoolingPowerConsumptionAutosized() const;

    CurveQuadLinear coolingCapacityCurve() const;
    OS_DEPRECATED(3, 2, 0) double coolingCapacityCoefficient1() const;
    OS_DEPRECATED(3, 2, 0) double coolingCapacityCoefficient2() const;
    OS_DEPRECATED(3, 2, 0) double coolingCapacityCoefficient3() const;
    OS_DEPRECATED(3, 2, 0) double coolingCapacityCoefficient4() const;
    OS_DEPRECATED(3, 2, 0) double coolingCapacityCoefficient5() const;

    CurveQuadLinear coolingCompressorPowerCurve() const;
    OS_DEPRECATED(3, 2, 0) double coolingCompressorPowerCoefficient1() const;
    OS_DEPRECATED(3, 2, 0) double coolingCompressorPowerCoefficient2() const;
    OS_DEPRECATED(3, 2, 0) double coolingCompressorPowerCoefficient3() const;
    OS_DEPRECATED(3, 2, 0) double coolingCompressorPowerCoefficient4() const;
    OS_DEPRECATED(3, 2, 0) double coolingCompressorPowerCoefficient5() const;

    double referenceCoefficientofPerformance() const;

    double sizingFactor() const;

    boost::optional<HeatPumpWaterToWaterEquationFitHeating> companionHeatingHeatPump() const;

    //@}
    /** @name Setters */
    //@{

    bool setReferenceLoadSideFlowRate(double referenceLoadSideFlowRate);

    /** Synonym of setReferenceLoadSideFlowRate for backwards compatibility **/
    bool setRatedLoadSideFlowRate(double ratedLoadSideFlowRate);

    void autosizeReferenceLoadSideFlowRate();

    bool setReferenceSourceSideFlowRate(double referenceSourceSideFlowRate);

    /** Synonym of setReferenceLoadSideFlowRate for backwards compatibility **/
    bool setRatedSourceSideFlowRate(double ratedSourceSideFlowRate);

    void autosizeReferenceSourceSideFlowRate();

    /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Reference Cooling Capacity" **/
    bool setRatedCoolingCapacity(double ratedCoolingCapacity);

    void autosizeRatedCoolingCapacity();

    /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Reference Cooling Power Consumption" **/
    bool setRatedCoolingPowerConsumption(double ratedCoolingPowerConsumption);

    void autosizeRatedCoolingPowerConsumption();

    bool setCoolingCapacityCurve(const CurveQuadLinear& coolingCapacityCurve);

    OS_DEPRECATED(3, 2, 0) bool setCoolingCapacityCoefficient1(double coolingCapacityCoefficient1);
    OS_DEPRECATED(3, 2, 0) bool setCoolingCapacityCoefficient2(double coolingCapacityCoefficient2);
    OS_DEPRECATED(3, 2, 0) bool setCoolingCapacityCoefficient3(double coolingCapacityCoefficient3);
    OS_DEPRECATED(3, 2, 0) bool setCoolingCapacityCoefficient4(double coolingCapacityCoefficient4);
    OS_DEPRECATED(3, 2, 0) bool setCoolingCapacityCoefficient5(double coolingCapacityCoefficient5);

    bool setCoolingCompressorPowerCurve(const CurveQuadLinear& coolingCompressorPowerCurve);

    OS_DEPRECATED(3, 2, 0) bool setCoolingCompressorPowerCoefficient1(double coolingCompressorPowerCoefficient1);
    OS_DEPRECATED(3, 2, 0) bool setCoolingCompressorPowerCoefficient2(double coolingCompressorPowerCoefficient2);
    OS_DEPRECATED(3, 2, 0) bool setCoolingCompressorPowerCoefficient3(double coolingCompressorPowerCoefficient3);
    OS_DEPRECATED(3, 2, 0) bool setCoolingCompressorPowerCoefficient4(double coolingCompressorPowerCoefficient4);
    OS_DEPRECATED(3, 2, 0) bool setCoolingCompressorPowerCoefficient5(double coolingCompressorPowerCoefficient5);

    bool setReferenceCoefficientofPerformance(double referenceCoefficientofPerformance);

    bool setSizingFactor(double sizingFactor);

    bool setCompanionHeatingHeatPump(const HeatPumpWaterToWaterEquationFitHeating& companionHP);

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedReferenceLoadSideFlowRate() const;

    boost::optional<double> autosizedReferenceSourceSideFlowRate() const;

    boost::optional<double> autosizedRatedCoolingCapacity() const;

    boost::optional<double> autosizedRatedCoolingPowerConsumption() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::HeatPumpWaterToWaterEquationFitCooling_Impl;

    explicit HeatPumpWaterToWaterEquationFitCooling(std::shared_ptr<detail::HeatPumpWaterToWaterEquationFitCooling_Impl> impl);

    friend class detail::HeatPumpWaterToWaterEquationFitCooling_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.HeatPumpWaterToWaterEquationFitCooling");
  };

  /** \relates HeatPumpWaterToWaterEquationFitCooling*/
  using OptionalHeatPumpWaterToWaterEquationFitCooling = boost::optional<HeatPumpWaterToWaterEquationFitCooling>;

  /** \relates HeatPumpWaterToWaterEquationFitCooling*/
  using HeatPumpWaterToWaterEquationFitCoolingVector = std::vector<HeatPumpWaterToWaterEquationFitCooling>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATPUMPWATERTOWATEREQUATIONFITCOOLING_HPP
