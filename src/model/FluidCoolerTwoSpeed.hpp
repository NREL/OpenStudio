/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FLUIDCOOLERTWOSPEED_HPP
#define MODEL_FLUIDCOOLERTWOSPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class FluidCoolerTwoSpeed_Impl;

  }  // namespace detail

  /** FluidCoolerTwoSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:FluidCooler:TwoSpeed'. */
  class MODEL_API FluidCoolerTwoSpeed : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit FluidCoolerTwoSpeed(const Model& model);

    virtual ~FluidCoolerTwoSpeed() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    FluidCoolerTwoSpeed(const FluidCoolerTwoSpeed& other) = default;
    FluidCoolerTwoSpeed(FluidCoolerTwoSpeed&& other) = default;
    FluidCoolerTwoSpeed& operator=(const FluidCoolerTwoSpeed&) = default;
    FluidCoolerTwoSpeed& operator=(FluidCoolerTwoSpeed&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> performanceInputMethodValues();

    /** @name Getters */
    //@{

    std::string performanceInputMethod() const;

    boost::optional<double> highFanSpeedUfactorTimesAreaValue() const;

    bool isHighFanSpeedUfactorTimesAreaValueAutosized() const;

    boost::optional<double> lowFanSpeedUfactorTimesAreaValue() const;

    bool isLowFanSpeedUfactorTimesAreaValueAutosized() const;

    double lowFanSpeedUFactorTimesAreaSizingFactor() const;

    double highSpeedNominalCapacity() const;

    boost::optional<double> lowSpeedNominalCapacity() const;

    bool isLowSpeedNominalCapacityAutosized() const;

    double lowSpeedNominalCapacitySizingFactor() const;

    double designEnteringWaterTemperature() const;

    double designEnteringAirTemperature() const;

    double designEnteringAirWetbulbTemperature() const;

    boost::optional<double> designWaterFlowRate() const;

    bool isDesignWaterFlowRateAutosized() const;

    boost::optional<double> highFanSpeedAirFlowRate() const;

    bool isHighFanSpeedAirFlowRateAutosized() const;

    boost::optional<double> highFanSpeedFanPower() const;

    bool isHighFanSpeedFanPowerAutosized() const;

    boost::optional<double> lowFanSpeedAirFlowRate() const;

    bool isLowFanSpeedAirFlowRateAutosized() const;

    double lowFanSpeedAirFlowRateSizingFactor() const;

    boost::optional<double> lowFanSpeedFanPower() const;

    bool isLowFanSpeedFanPowerAutosized() const;

    double lowFanSpeedFanPowerSizingFactor() const;

    boost::optional<Node> outdoorAirInletNode() const;

    //@}
    /** @name Setters */
    //@{

    bool setPerformanceInputMethod(const std::string& performanceInputMethod);

    bool setHighFanSpeedUfactorTimesAreaValue(double highFanSpeedUfactorTimesAreaValue);

    void autosizeHighFanSpeedUfactorTimesAreaValue();

    bool setLowFanSpeedUfactorTimesAreaValue(double lowFanSpeedUfactorTimesAreaValue);

    void autosizeLowFanSpeedUfactorTimesAreaValue();

    bool setLowFanSpeedUFactorTimesAreaSizingFactor(double lowFanSpeedUFactorTimesAreaSizingFactor);

    bool setHighSpeedNominalCapacity(double highSpeedNominalCapacity);

    bool setLowSpeedNominalCapacity(double lowSpeedNominalCapacity);

    void autosizeLowSpeedNominalCapacity();

    bool setLowSpeedNominalCapacitySizingFactor(double lowSpeedNominalCapacitySizingFactor);

    bool setDesignEnteringWaterTemperature(double designEnteringWaterTemperature);

    bool setDesignEnteringAirTemperature(double designEnteringAirTemperature);

    bool setDesignEnteringAirWetbulbTemperature(double designEnteringAirWetbulbTemperature);

    bool setDesignWaterFlowRate(double designWaterFlowRate);

    void autosizeDesignWaterFlowRate();

    bool setHighFanSpeedAirFlowRate(double highFanSpeedAirFlowRate);

    void autosizeHighFanSpeedAirFlowRate();

    bool setHighFanSpeedFanPower(double highFanSpeedFanPower);

    void autosizeHighFanSpeedFanPower();

    bool setLowFanSpeedAirFlowRate(double lowFanSpeedAirFlowRate);

    void autosizeLowFanSpeedAirFlowRate();

    bool setLowFanSpeedAirFlowRateSizingFactor(double lowFanSpeedAirFlowRateSizingFactor);

    bool setLowFanSpeedFanPower(double lowFanSpeedFanPower);

    void autosizeLowFanSpeedFanPower();

    bool setLowFanSpeedFanPowerSizingFactor(double lowFanSpeedFanPowerSizingFactor);

    bool setOutdoorAirInletNode(const Node& node);

    void resetOutdoorAirInletNode();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedHighFanSpeedUfactorTimesAreaValue() const;

    boost::optional<double> autosizedLowFanSpeedUfactorTimesAreaValue() const;

    boost::optional<double> autosizedLowSpeedNominalCapacity() const;

    boost::optional<double> autosizedDesignWaterFlowRate() const;

    boost::optional<double> autosizedHighFanSpeedAirFlowRate() const;

    boost::optional<double> autosizedHighFanSpeedFanPower() const;

    boost::optional<double> autosizedLowFanSpeedAirFlowRate() const;

    boost::optional<double> autosizedLowFanSpeedFanPower() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::FluidCoolerTwoSpeed_Impl;

    explicit FluidCoolerTwoSpeed(std::shared_ptr<detail::FluidCoolerTwoSpeed_Impl> impl);

    friend class detail::FluidCoolerTwoSpeed_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.FluidCoolerTwoSpeed");
  };

  /** \relates FluidCoolerTwoSpeed*/
  using OptionalFluidCoolerTwoSpeed = boost::optional<FluidCoolerTwoSpeed>;

  /** \relates FluidCoolerTwoSpeed*/
  using FluidCoolerTwoSpeedVector = std::vector<FluidCoolerTwoSpeed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FLUIDCOOLERTWOSPEED_HPP
