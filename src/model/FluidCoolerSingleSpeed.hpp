/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_FLUIDCOOLERSINGLESPEED_HPP
#define MODEL_FLUIDCOOLERSINGLESPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Node;

  namespace detail {

    class FluidCoolerSingleSpeed_Impl;

  }  // namespace detail

  /** FluidCoolerSingleSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:FluidCooler:SingleSpeed'. */
  class MODEL_API FluidCoolerSingleSpeed : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit FluidCoolerSingleSpeed(const Model& model);

    virtual ~FluidCoolerSingleSpeed() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    FluidCoolerSingleSpeed(const FluidCoolerSingleSpeed& other) = default;
    FluidCoolerSingleSpeed(FluidCoolerSingleSpeed&& other) = default;
    FluidCoolerSingleSpeed& operator=(const FluidCoolerSingleSpeed&) = default;
    FluidCoolerSingleSpeed& operator=(FluidCoolerSingleSpeed&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> performanceInputMethodValues();

    /** @name Getters */
    //@{

    std::string performanceInputMethod() const;

    boost::optional<double> designAirFlowRateUfactorTimesAreaValue() const;

    bool isDesignAirFlowRateUfactorTimesAreaValueAutosized() const;

    double nominalCapacity() const;

    double designEnteringWaterTemperature() const;

    double designEnteringAirTemperature() const;

    double designEnteringAirWetbulbTemperature() const;

    boost::optional<double> designWaterFlowRate() const;

    bool isDesignWaterFlowRateAutosized() const;

    boost::optional<double> designAirFlowRate() const;

    bool isDesignAirFlowRateAutosized() const;

    boost::optional<double> designAirFlowRateFanPower() const;

    bool isDesignAirFlowRateFanPowerAutosized() const;

    boost::optional<Node> outdoorAirInletNode() const;

    //@}
    /** @name Setters */
    //@{

    bool setPerformanceInputMethod(const std::string& performanceInputMethod);

    bool setDesignAirFlowRateUfactorTimesAreaValue(double designAirFlowRateUfactorTimesAreaValue);

    void autosizeDesignAirFlowRateUfactorTimesAreaValue();

    bool setNominalCapacity(double nominalCapacity);

    bool setDesignEnteringWaterTemperature(double designEnteringWaterTemperature);

    bool setDesignEnteringAirTemperature(double designEnteringAirTemperature);

    bool setDesignEnteringAirWetbulbTemperature(double designEnteringAirWetbulbTemperature);

    bool setDesignWaterFlowRate(double designWaterFlowRate);

    void autosizeDesignWaterFlowRate();

    bool setDesignAirFlowRate(double designAirFlowRate);

    void autosizeDesignAirFlowRate();

    bool setDesignAirFlowRateFanPower(double designAirFlowRateFanPower);

    void autosizeDesignAirFlowRateFanPower();

    bool setOutdoorAirInletNode(const Node& node);

    void resetOutdoorAirInletNode();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedDesignAirFlowRateUfactorTimesAreaValue() const;

    boost::optional<double> autosizedDesignWaterFlowRate() const;

    boost::optional<double> autosizedDesignAirFlowRate() const;

    boost::optional<double> autosizedDesignAirFlowRateFanPower() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::FluidCoolerSingleSpeed_Impl;

    explicit FluidCoolerSingleSpeed(std::shared_ptr<detail::FluidCoolerSingleSpeed_Impl> impl);

    friend class detail::FluidCoolerSingleSpeed_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.FluidCoolerSingleSpeed");
  };

  /** \relates FluidCoolerSingleSpeed*/
  using OptionalFluidCoolerSingleSpeed = boost::optional<FluidCoolerSingleSpeed>;

  /** \relates FluidCoolerSingleSpeed*/
  using FluidCoolerSingleSpeedVector = std::vector<FluidCoolerSingleSpeed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FLUIDCOOLERSINGLESPEED_HPP
