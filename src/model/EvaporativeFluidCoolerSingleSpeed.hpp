/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EVAPORATIVEFLUIDCOOLERSINGLESPEED_HPP
#define MODEL_EVAPORATIVEFLUIDCOOLERSINGLESPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  //class WaterStorageTank;

  namespace detail {

    class EvaporativeFluidCoolerSingleSpeed_Impl;

  }  // namespace detail

  /** EvaporativeFluidCoolerSingleSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:EvaporativeFluidCooler:SingleSpeed'. */
  class MODEL_API EvaporativeFluidCoolerSingleSpeed : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit EvaporativeFluidCoolerSingleSpeed(const Model& model);

    virtual ~EvaporativeFluidCoolerSingleSpeed() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    EvaporativeFluidCoolerSingleSpeed(const EvaporativeFluidCoolerSingleSpeed& other) = default;
    EvaporativeFluidCoolerSingleSpeed(EvaporativeFluidCoolerSingleSpeed&& other) = default;
    EvaporativeFluidCoolerSingleSpeed& operator=(const EvaporativeFluidCoolerSingleSpeed&) = default;
    EvaporativeFluidCoolerSingleSpeed& operator=(EvaporativeFluidCoolerSingleSpeed&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> performanceInputMethodValues();

    static std::vector<std::string> capacityControlValues();

    static std::vector<std::string> evaporationLossModeValues();

    static std::vector<std::string> blowdownCalculationModeValues();

    /** @name Getters */
    //@{

    boost::optional<double> designAirFlowRate() const;

    bool isDesignAirFlowRateAutosized() const;

    boost::optional<double> fanPoweratDesignAirFlowRate() const;

    bool isFanPoweratDesignAirFlowRateAutosized() const;

    double designSprayWaterFlowRate() const;

    std::string performanceInputMethod() const;

    boost::optional<double> standardDesignCapacity() const;

    boost::optional<double> ufactorTimesAreaValueatDesignAirFlowRate() const;

    bool isUfactorTimesAreaValueatDesignAirFlowRateAutosized() const;

    boost::optional<double> designWaterFlowRate() const;

    bool isDesignWaterFlowRateAutosized() const;

    boost::optional<double> userSpecifiedDesignCapacity() const;

    boost::optional<double> designEnteringWaterTemperature() const;

    boost::optional<double> designEnteringAirTemperature() const;

    boost::optional<double> designEnteringAirWetbulbTemperature() const;

    std::string capacityControl() const;

    bool isCapacityControlDefaulted() const;

    double sizingFactor() const;

    bool isSizingFactorDefaulted() const;

    std::string evaporationLossMode() const;

    bool isEvaporationLossModeDefaulted() const;

    boost::optional<double> evaporationLossFactor() const;

    double driftLossPercent() const;

    bool isDriftLossPercentDefaulted() const;

    std::string blowdownCalculationMode() const;

    bool isBlowdownCalculationModeDefaulted() const;

    double blowdownConcentrationRatio() const;

    bool isBlowdownConcentrationRatioDefaulted() const;

    boost::optional<Schedule> blowdownMakeupWaterUsageSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setDesignAirFlowRate(double designAirFlowRate);

    void autosizeDesignAirFlowRate();

    bool setFanPoweratDesignAirFlowRate(double fanPoweratDesignAirFlowRate);

    void autosizeFanPoweratDesignAirFlowRate();

    bool setDesignSprayWaterFlowRate(double designSprayWaterFlowRate);

    bool setPerformanceInputMethod(const std::string& performanceInputMethod);

    bool setStandardDesignCapacity(double standardDesignCapacity);

    void resetStandardDesignCapacity();

    bool setUfactorTimesAreaValueatDesignAirFlowRate(double ufactorTimesAreaValueatDesignAirFlowRate);

    void resetUfactorTimesAreaValueatDesignAirFlowRate();

    void autosizeUfactorTimesAreaValueatDesignAirFlowRate();

    bool setDesignWaterFlowRate(double designWaterFlowRate);

    void resetDesignWaterFlowRate();

    void autosizeDesignWaterFlowRate();

    bool setUserSpecifiedDesignCapacity(double userSpecifiedDesignCapacity);

    void resetUserSpecifiedDesignCapacity();

    bool setDesignEnteringWaterTemperature(double designEnteringWaterTemperature);

    void resetDesignEnteringWaterTemperature();

    bool setDesignEnteringAirTemperature(double designEnteringAirTemperature);

    void resetDesignEnteringAirTemperature();

    bool setDesignEnteringAirWetbulbTemperature(double designEnteringAirWetbulbTemperature);

    void resetDesignEnteringAirWetbulbTemperature();

    bool setCapacityControl(const std::string& capacityControl);

    void resetCapacityControl();

    bool setSizingFactor(double sizingFactor);

    void resetSizingFactor();

    bool setEvaporationLossMode(const std::string& evaporationLossMode);

    void resetEvaporationLossMode();

    bool setEvaporationLossFactor(double evaporationLossFactor);

    void resetEvaporationLossFactor();

    bool setDriftLossPercent(double driftLossPercent);

    void resetDriftLossPercent();

    bool setBlowdownCalculationMode(const std::string& blowdownCalculationMode);

    void resetBlowdownCalculationMode();

    bool setBlowdownConcentrationRatio(double blowdownConcentrationRatio);

    void resetBlowdownConcentrationRatio();

    bool setBlowdownMakeupWaterUsageSchedule(Schedule& schedule);

    void resetBlowdownMakeupWaterUsageSchedule();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedDesignAirFlowRate() const;

    boost::optional<double> autosizedFanPoweratDesignAirFlowRate() const;

    boost::optional<double> autosizedUfactorTimesAreaValueatDesignAirFlowRate() const;

    boost::optional<double> autosizedDesignWaterFlowRate() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::EvaporativeFluidCoolerSingleSpeed_Impl;

    explicit EvaporativeFluidCoolerSingleSpeed(std::shared_ptr<detail::EvaporativeFluidCoolerSingleSpeed_Impl> impl);

    friend class detail::EvaporativeFluidCoolerSingleSpeed_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.EvaporativeFluidCoolerSingleSpeed");
  };

  /** \relates EvaporativeFluidCoolerSingleSpeed*/
  using OptionalEvaporativeFluidCoolerSingleSpeed = boost::optional<EvaporativeFluidCoolerSingleSpeed>;

  /** \relates EvaporativeFluidCoolerSingleSpeed*/
  using EvaporativeFluidCoolerSingleSpeedVector = std::vector<EvaporativeFluidCoolerSingleSpeed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EVAPORATIVEFLUIDCOOLERSINGLESPEED_HPP
