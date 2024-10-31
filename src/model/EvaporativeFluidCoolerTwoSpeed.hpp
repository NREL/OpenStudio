/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EVAPORATIVEFLUIDCOOLERTWOSPEED_HPP
#define MODEL_EVAPORATIVEFLUIDCOOLERTWOSPEED_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  // class WaterStorageTank;

  namespace detail {

    class EvaporativeFluidCoolerTwoSpeed_Impl;

  }  // namespace detail

  /** EvaporativeFluidCoolerTwoSpeed is a StraightComponent that wraps the OpenStudio IDD object 'OS:EvaporativeFluidCooler:TwoSpeed'. */
  class MODEL_API EvaporativeFluidCoolerTwoSpeed : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit EvaporativeFluidCoolerTwoSpeed(const Model& model);

    virtual ~EvaporativeFluidCoolerTwoSpeed() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    EvaporativeFluidCoolerTwoSpeed(const EvaporativeFluidCoolerTwoSpeed& other) = default;
    EvaporativeFluidCoolerTwoSpeed(EvaporativeFluidCoolerTwoSpeed&& other) = default;
    EvaporativeFluidCoolerTwoSpeed& operator=(const EvaporativeFluidCoolerTwoSpeed&) = default;
    EvaporativeFluidCoolerTwoSpeed& operator=(EvaporativeFluidCoolerTwoSpeed&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> performanceInputMethodValues();

    static std::vector<std::string> evaporationLossModeValues();

    static std::vector<std::string> blowdownCalculationModeValues();

    /** @name Getters */
    //@{

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

    double designSprayWaterFlowRate() const;

    std::string performanceInputMethod() const;

    boost::optional<std::string> outdoorAirInletNodeName() const;

    double heatRejectionCapacityandNominalCapacitySizingRatio() const;

    boost::optional<double> highSpeedStandardDesignCapacity() const;

    boost::optional<double> lowSpeedStandardDesignCapacity() const;

    bool isLowSpeedStandardDesignCapacityAutosized() const;

    double lowSpeedStandardCapacitySizingFactor() const;

    boost::optional<double> highFanSpeedUfactorTimesAreaValue() const;

    bool isHighFanSpeedUfactorTimesAreaValueAutosized() const;

    boost::optional<double> lowFanSpeedUfactorTimesAreaValue() const;

    bool isLowFanSpeedUfactorTimesAreaValueAutosized() const;

    double lowFanSpeedUFactorTimesAreaSizingFactor() const;

    boost::optional<double> designWaterFlowRate() const;

    bool isDesignWaterFlowRateAutosized() const;

    boost::optional<double> highSpeedUserSpecifiedDesignCapacity() const;

    boost::optional<double> lowSpeedUserSpecifiedDesignCapacity() const;

    bool isLowSpeedUserSpecifiedDesignCapacityAutosized() const;

    double lowSpeedUserSpecifiedDesignCapacitySizingFactor() const;

    boost::optional<double> designEnteringWaterTemperature() const;

    boost::optional<double> designEnteringAirTemperature() const;

    boost::optional<double> designEnteringAirWetbulbTemperature() const;

    double highSpeedSizingFactor() const;

    std::string evaporationLossMode() const;

    boost::optional<double> evaporationLossFactor() const;

    double driftLossPercent() const;

    std::string blowdownCalculationMode() const;

    double blowdownConcentrationRatio() const;

    boost::optional<Schedule> blowdownMakeupWaterUsageSchedule() const;

    // boost::optional<WaterStorageTank> supplyWaterStorageTank() const;

    //@}
    /** @name Setters */
    //@{

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

    bool setDesignSprayWaterFlowRate(double designSprayWaterFlowRate);

    bool setPerformanceInputMethod(const std::string& performanceInputMethod);

    bool setOutdoorAirInletNodeName(const std::string& outdoorAirInletNodeName);

    void resetOutdoorAirInletNodeName();

    bool setHeatRejectionCapacityandNominalCapacitySizingRatio(double heatRejectionCapacityandNominalCapacitySizingRatio);

    bool setHighSpeedStandardDesignCapacity(double highSpeedStandardDesignCapacity);

    void resetHighSpeedStandardDesignCapacity();

    bool setLowSpeedStandardDesignCapacity(double lowSpeedStandardDesignCapacity);

    void resetLowSpeedStandardDesignCapacity();

    void autosizeLowSpeedStandardDesignCapacity();

    bool setLowSpeedStandardCapacitySizingFactor(double lowSpeedStandardCapacitySizingFactor);

    bool setHighFanSpeedUfactorTimesAreaValue(double highFanSpeedUfactorTimesAreaValue);

    void resetHighFanSpeedUfactorTimesAreaValue();

    void autosizeHighFanSpeedUfactorTimesAreaValue();

    bool setLowFanSpeedUfactorTimesAreaValue(double lowFanSpeedUfactorTimesAreaValue);

    void resetLowFanSpeedUfactorTimesAreaValue();

    void autosizeLowFanSpeedUfactorTimesAreaValue();

    bool setLowFanSpeedUFactorTimesAreaSizingFactor(double lowFanSpeedUFactorTimesAreaSizingFactor);

    bool setDesignWaterFlowRate(double designWaterFlowRate);

    void resetDesignWaterFlowRate();

    void autosizeDesignWaterFlowRate();

    bool setHighSpeedUserSpecifiedDesignCapacity(double highSpeedUserSpecifiedDesignCapacity);

    void resetHighSpeedUserSpecifiedDesignCapacity();

    bool setLowSpeedUserSpecifiedDesignCapacity(double lowSpeedUserSpecifiedDesignCapacity);

    void resetLowSpeedUserSpecifiedDesignCapacity();

    void autosizeLowSpeedUserSpecifiedDesignCapacity();

    bool setLowSpeedUserSpecifiedDesignCapacitySizingFactor(double lowSpeedUserSpecifiedDesignCapacitySizingFactor);

    bool setDesignEnteringWaterTemperature(double designEnteringWaterTemperature);

    void resetDesignEnteringWaterTemperature();

    bool setDesignEnteringAirTemperature(double designEnteringAirTemperature);

    void resetDesignEnteringAirTemperature();

    bool setDesignEnteringAirWetbulbTemperature(double designEnteringAirWetbulbTemperature);

    void resetDesignEnteringAirWetbulbTemperature();

    bool setHighSpeedSizingFactor(double highSpeedSizingFactor);

    bool setEvaporationLossMode(const std::string& evaporationLossMode);

    bool setEvaporationLossFactor(double evaporationLossFactor);

    void resetEvaporationLossFactor();

    bool setDriftLossPercent(double driftLossPercent);

    bool setBlowdownCalculationMode(const std::string& blowdownCalculationMode);

    bool setBlowdownConcentrationRatio(double blowdownConcentrationRatio);

    bool setBlowdownMakeupWaterUsageSchedule(Schedule& schedule);

    void resetBlowdownMakeupWaterUsageSchedule();

    // bool setSupplyWaterStorageTank(const WaterStorageTank& waterStorageTank);

    // void resetSupplyWaterStorageTank();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> autosizedHighFanSpeedAirFlowRate() const;

    boost::optional<double> autosizedHighFanSpeedFanPower() const;

    boost::optional<double> autosizedLowFanSpeedAirFlowRate() const;

    boost::optional<double> autosizedLowFanSpeedFanPower() const;

    boost::optional<double> autosizedLowSpeedStandardDesignCapacity() const;

    boost::optional<double> autosizedHighFanSpeedUfactorTimesAreaValue() const;

    boost::optional<double> autosizedLowFanSpeedUfactorTimesAreaValue() const;

    boost::optional<double> autosizedDesignWaterFlowRate() const;

    boost::optional<double> autosizedLowSpeedUserSpecifiedDesignCapacity() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::EvaporativeFluidCoolerTwoSpeed_Impl;

    explicit EvaporativeFluidCoolerTwoSpeed(std::shared_ptr<detail::EvaporativeFluidCoolerTwoSpeed_Impl> impl);

    friend class detail::EvaporativeFluidCoolerTwoSpeed_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.EvaporativeFluidCoolerTwoSpeed");
  };

  /** \relates EvaporativeFluidCoolerTwoSpeed*/
  using OptionalEvaporativeFluidCoolerTwoSpeed = boost::optional<EvaporativeFluidCoolerTwoSpeed>;

  /** \relates EvaporativeFluidCoolerTwoSpeed*/
  using EvaporativeFluidCoolerTwoSpeedVector = std::vector<EvaporativeFluidCoolerTwoSpeed>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EVAPORATIVEFLUIDCOOLERTWOSPEED_HPP
