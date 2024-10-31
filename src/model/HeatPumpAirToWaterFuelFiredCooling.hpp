/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HEATPUMPAIRTOWATERFUELFIREDCOOLING_HPP
#define MODEL_HEATPUMPAIRTOWATERFUELFIREDCOOLING_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class HeatPumpAirToWaterFuelFiredHeating;
  class Curve;
  // class OutdoorAirNode;

  namespace detail {

    class HeatPumpAirToWaterFuelFiredCooling_Impl;

  }  // namespace detail

  /** HeatPumpAirToWaterFuelFiredCooling is a StraightComponent that wraps the OpenStudio IDD object 'OS:HeatPump:AirToWater:FuelFired:Cooling'. */
  class MODEL_API HeatPumpAirToWaterFuelFiredCooling : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit HeatPumpAirToWaterFuelFiredCooling(const Model& model);

    explicit HeatPumpAirToWaterFuelFiredCooling(const Model& model, const Curve& normalizedCapacityFunctionofTemperatureCurve,
                                                const Curve& fuelEnergyInputRatioFunctionofTemperatureCurve,
                                                const Curve& fuelEnergyInputRatioFunctionofPLRCurve);

    virtual ~HeatPumpAirToWaterFuelFiredCooling() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    HeatPumpAirToWaterFuelFiredCooling(const HeatPumpAirToWaterFuelFiredCooling& other) = default;
    HeatPumpAirToWaterFuelFiredCooling(HeatPumpAirToWaterFuelFiredCooling&& other) = default;
    HeatPumpAirToWaterFuelFiredCooling& operator=(const HeatPumpAirToWaterFuelFiredCooling&) = default;
    HeatPumpAirToWaterFuelFiredCooling& operator=(HeatPumpAirToWaterFuelFiredCooling&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> fuelTypeValues();

    static std::vector<std::string> flowModeValues();

    static std::vector<std::string> outdoorAirTemperatureCurveInputVariableValues();

    static std::vector<std::string> waterTemperatureCurveInputVariableValues();

    /** @name Getters */
    //@{

    // boost::optional<OutdoorAirNode> airSourceNode() const;

    boost::optional<HeatPumpAirToWaterFuelFiredHeating> companionHeatingHeatPump() const;

    std::string fuelType() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    boost::optional<double> nominalCoolingCapacity() const;

    bool isNominalCoolingCapacityAutosized() const;

    boost::optional<double> autosizedNominalCoolingCapacity();

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

    bool setCompanionHeatingHeatPump(const HeatPumpAirToWaterFuelFiredHeating& heatPumpAirToWaterFuelFiredHeating);

    void resetCompanionHeatingHeatPump();

    bool setFuelType(const std::string& fuelType);

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    void resetEndUseSubcategory();

    bool setNominalCoolingCapacity(double nominalCoolingCapacity);

    void resetNominalCoolingCapacity();

    void autosizeNominalCoolingCapacity();

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
    using ImplType = detail::HeatPumpAirToWaterFuelFiredCooling_Impl;

    explicit HeatPumpAirToWaterFuelFiredCooling(std::shared_ptr<detail::HeatPumpAirToWaterFuelFiredCooling_Impl> impl);

    friend class detail::HeatPumpAirToWaterFuelFiredCooling_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.HeatPumpAirToWaterFuelFiredCooling");
  };

  /** \relates HeatPumpAirToWaterFuelFiredCooling*/
  using OptionalHeatPumpAirToWaterFuelFiredCooling = boost::optional<HeatPumpAirToWaterFuelFiredCooling>;

  /** \relates HeatPumpAirToWaterFuelFiredCooling*/
  using HeatPumpAirToWaterFuelFiredCoolingVector = std::vector<HeatPumpAirToWaterFuelFiredCooling>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATPUMPAIRTOWATERFUELFIREDCOOLING_HPP
