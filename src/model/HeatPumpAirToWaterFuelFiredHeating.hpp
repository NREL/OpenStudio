/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HEATPUMPAIRTOWATERFUELFIREDHEATING_HPP
#define MODEL_HEATPUMPAIRTOWATERFUELFIREDHEATING_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class HeatPumpAirToWaterFuelFiredCooling;
  class Curve;
  // class OutdoorAirNode;

  namespace detail {

    class HeatPumpAirToWaterFuelFiredHeating_Impl;

  }  // namespace detail

  /** HeatPumpAirToWaterFuelFiredHeating is a StraightComponent that wraps the OpenStudio IDD object 'OS:HeatPump:AirToWater:FuelFired:Heating'. */
  class MODEL_API HeatPumpAirToWaterFuelFiredHeating : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit HeatPumpAirToWaterFuelFiredHeating(const Model& model);

    explicit HeatPumpAirToWaterFuelFiredHeating(const Model& model, const Curve& normalizedCapacityFunctionofTemperatureCurve,
                                                const Curve& fuelEnergyInputRatioFunctionofTemperatureCurve,
                                                const Curve& fuelEnergyInputRatioFunctionofPLRCurve);

    virtual ~HeatPumpAirToWaterFuelFiredHeating() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    HeatPumpAirToWaterFuelFiredHeating(const HeatPumpAirToWaterFuelFiredHeating& other) = default;
    HeatPumpAirToWaterFuelFiredHeating(HeatPumpAirToWaterFuelFiredHeating&& other) = default;
    HeatPumpAirToWaterFuelFiredHeating& operator=(const HeatPumpAirToWaterFuelFiredHeating&) = default;
    HeatPumpAirToWaterFuelFiredHeating& operator=(HeatPumpAirToWaterFuelFiredHeating&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> fuelTypeValues();

    static std::vector<std::string> flowModeValues();

    static std::vector<std::string> outdoorAirTemperatureCurveInputVariableValues();

    static std::vector<std::string> waterTemperatureCurveInputVariableValues();

    static std::vector<std::string> defrostControlTypeValues();

    /** @name Getters */
    //@{

    // boost::optional<OutdoorAirNode> airSourceNode() const;

    boost::optional<HeatPumpAirToWaterFuelFiredCooling> companionCoolingHeatPump() const;

    std::string fuelType() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    boost::optional<double> nominalHeatingCapacity() const;

    bool isNominalHeatingCapacityAutosized() const;

    boost::optional<double> autosizedNominalHeatingCapacity();

    double nominalCOP() const;

    boost::optional<double> designFlowRate() const;

    bool isDesignFlowRateAutosized() const;

    boost::optional<double> autosizedDesignFlowRate();

    double designSupplyTemperature() const;

    boost::optional<double> designTemperatureLift() const;

    bool isDesignTemperatureLiftAutosized() const;

    boost::optional<double> autosizedDesignTemperatureLift();

    double sizingFactor() const;

    std::string flowMode() const;

    std::string outdoorAirTemperatureCurveInputVariable() const;

    std::string waterTemperatureCurveInputVariable() const;

    Curve normalizedCapacityFunctionofTemperatureCurve() const;

    Curve fuelEnergyInputRatioFunctionofTemperatureCurve() const;

    Curve fuelEnergyInputRatioFunctionofPLRCurve() const;

    double minimumPartLoadRatio() const;

    double maximumPartLoadRatio() const;

    std::string defrostControlType() const;

    double defrostOperationTimeFraction() const;

    boost::optional<Curve> fuelEnergyInputRatioDefrostAdjustmentCurve() const;

    double resistiveDefrostHeaterCapacity() const;

    double maximumOutdoorDrybulbTemperatureforDefrostOperation() const;

    boost::optional<Curve> cyclingRatioFactorCurve() const;

    double nominalAuxiliaryElectricPower() const;

    boost::optional<Curve> auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve() const;

    boost::optional<Curve> auxiliaryElectricEnergyInputRatioFunctionofPLRCurve() const;

    double standbyElectricPower() const;

    //@}
    /** @name Setters */
    //@{

    // bool setAirSourceNode(const OutdoorAirNode& outdoorAirNode);

    // void resetAirSourceNode();

    bool setCompanionCoolingHeatPump(const HeatPumpAirToWaterFuelFiredCooling& heatPumpAirToWaterFuelFiredCooling);

    void resetCompanionCoolingHeatPump();

    bool setFuelType(const std::string& fuelType);

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    void resetEndUseSubcategory();

    bool setNominalHeatingCapacity(double nominalHeatingCapacity);

    void resetNominalHeatingCapacity();

    void autosizeNominalHeatingCapacity();

    bool setNominalCOP(double nominalCOP);

    bool setDesignFlowRate(double designFlowRate);

    void resetDesignFlowRate();

    void autosizeDesignFlowRate();

    bool setDesignSupplyTemperature(double designSupplyTemperature);

    bool setDesignTemperatureLift(double designTemperatureLift);

    void autosizeDesignTemperatureLift();

    bool setSizingFactor(double sizingFactor);

    bool setFlowMode(const std::string& flowMode);

    bool setOutdoorAirTemperatureCurveInputVariable(const std::string& outdoorAirTemperatureCurveInputVariable);

    bool setWaterTemperatureCurveInputVariable(const std::string& waterTemperatureCurveInputVariable);

    bool setNormalizedCapacityFunctionofTemperatureCurve(const Curve& normalizedCapacityFunctionofTemperatureCurve);

    bool setFuelEnergyInputRatioFunctionofTemperatureCurve(const Curve& fuelEnergyInputRatioFunctionofTemperatureCurve);

    bool setFuelEnergyInputRatioFunctionofPLRCurve(const Curve& fuelEnergyInputRatioFunctionofPLRCurve);

    bool setMinimumPartLoadRatio(double minimumPartLoadRatio);

    bool setMaximumPartLoadRatio(double maximumPartLoadRatio);

    bool setDefrostControlType(const std::string& defrostControlType);

    bool setDefrostOperationTimeFraction(double defrostOperationTimeFraction);

    bool setFuelEnergyInputRatioDefrostAdjustmentCurve(const Curve& fuelEnergyInputRatioDefrostAdjustmentCurve);

    void resetFuelEnergyInputRatioDefrostAdjustmentCurve();

    bool setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity);

    bool setMaximumOutdoorDrybulbTemperatureforDefrostOperation(double maximumOutdoorDrybulbTemperatureforDefrostOperation);

    bool setCyclingRatioFactorCurve(const Curve& cyclingRatioFactorCurve);

    void resetCyclingRatioFactorCurve();

    bool setNominalAuxiliaryElectricPower(double nominalAuxiliaryElectricPower);

    bool setAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve(const Curve& auxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve);

    void resetAuxiliaryElectricEnergyInputRatioFunctionofTemperatureCurve();

    bool setAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve(const Curve& auxiliaryElectricEnergyInputRatioFunctionofPLRCurve);

    void resetAuxiliaryElectricEnergyInputRatioFunctionofPLRCurve();

    bool setStandbyElectricPower(double standbyElectricPower);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::HeatPumpAirToWaterFuelFiredHeating_Impl;

    explicit HeatPumpAirToWaterFuelFiredHeating(std::shared_ptr<detail::HeatPumpAirToWaterFuelFiredHeating_Impl> impl);

    friend class detail::HeatPumpAirToWaterFuelFiredHeating_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.HeatPumpAirToWaterFuelFiredHeating");
  };

  /** \relates HeatPumpAirToWaterFuelFiredHeating*/
  using OptionalHeatPumpAirToWaterFuelFiredHeating = boost::optional<HeatPumpAirToWaterFuelFiredHeating>;

  /** \relates HeatPumpAirToWaterFuelFiredHeating*/
  using HeatPumpAirToWaterFuelFiredHeatingVector = std::vector<HeatPumpAirToWaterFuelFiredHeating>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATPUMPAIRTOWATERFUELFIREDHEATING_HPP
