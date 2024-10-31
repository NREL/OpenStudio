/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HEATEXCHANGERAIRTOAIRSENSIBLEANDLATENT_HPP
#define MODEL_HEATEXCHANGERAIRTOAIRSENSIBLEANDLATENT_HPP

#include "ModelAPI.hpp"
#include "AirToAirComponent.hpp"

#include "../utilities/core/Deprecated.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class AirflowNetworkEquivalentDuct;
  class Curve;

  namespace detail {

    class HeatExchangerAirToAirSensibleAndLatent_Impl;

  }  // namespace detail

  /** HeatExchangerAirToAirSensibleAndLatent is a AirToAirComponent that wraps the OpenStudio IDD object 'OS:HeatExchanger:AirToAir:SensibleAndLatent'. */
  class MODEL_API HeatExchangerAirToAirSensibleAndLatent : public AirToAirComponent
  {

   public:
    /** @name Constructors and Destructors */
    //@{

    /** Constructor. It will **not** instantiate the optional effectivness curve/tables objects and effectiveness will be constant.
      *  You can then call the helper method `bool assignHistoricalEffectivenessCurves()` to assign 4 TableLookups that will match the pre E+ 24.1.0
      * defaults for Sensible/Latent Effectiveness at 75% Heating/Cooling airflow */
    explicit HeatExchangerAirToAirSensibleAndLatent(const Model& model);

    virtual ~HeatExchangerAirToAirSensibleAndLatent() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    HeatExchangerAirToAirSensibleAndLatent(const HeatExchangerAirToAirSensibleAndLatent& other) = default;
    HeatExchangerAirToAirSensibleAndLatent(HeatExchangerAirToAirSensibleAndLatent&& other) = default;
    HeatExchangerAirToAirSensibleAndLatent& operator=(const HeatExchangerAirToAirSensibleAndLatent&) = default;
    HeatExchangerAirToAirSensibleAndLatent& operator=(HeatExchangerAirToAirSensibleAndLatent&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> heatExchangerTypeValues();

    static std::vector<std::string> frostControlTypeValues();

    Schedule availabilitySchedule() const;

    bool setAvailabilitySchedule(Schedule& schedule);

    boost::optional<double> nominalSupplyAirFlowRate() const;

    bool isNominalSupplyAirFlowRateAutosized() const;

    bool setNominalSupplyAirFlowRate(double nominalSupplyAirFlowRate);

    void autosizeNominalSupplyAirFlowRate();

    double sensibleEffectivenessat100HeatingAirFlow() const;

    bool setSensibleEffectivenessat100HeatingAirFlow(double sensibleEffectivenessat100HeatingAirFlow);

    double latentEffectivenessat100HeatingAirFlow() const;

    bool setLatentEffectivenessat100HeatingAirFlow(double latentEffectivenessat100HeatingAirFlow);

    OS_DEPRECATED(3, 8, 0) double sensibleEffectivenessat75HeatingAirFlow() const;

    OS_DEPRECATED(3, 8, 0) bool setSensibleEffectivenessat75HeatingAirFlow(double sensibleEffectivenessat75HeatingAirFlow);

    OS_DEPRECATED(3, 8, 0) double latentEffectivenessat75HeatingAirFlow() const;

    OS_DEPRECATED(3, 8, 0) bool setLatentEffectivenessat75HeatingAirFlow(double latentEffectivenessat75HeatingAirFlow);

    double sensibleEffectivenessat100CoolingAirFlow() const;

    bool setSensibleEffectivenessat100CoolingAirFlow(double sensibleEffectivenessat100CoolingAirFlow);

    double latentEffectivenessat100CoolingAirFlow() const;

    bool setLatentEffectivenessat100CoolingAirFlow(double latentEffectivenessat100CoolingAirFlow);

    OS_DEPRECATED(3, 8, 0) double sensibleEffectivenessat75CoolingAirFlow() const;

    OS_DEPRECATED(3, 8, 0) bool setSensibleEffectivenessat75CoolingAirFlow(double sensibleEffectivenessat75CoolingAirFlow);

    OS_DEPRECATED(3, 8, 0) double latentEffectivenessat75CoolingAirFlow() const;

    OS_DEPRECATED(3, 8, 0) bool setLatentEffectivenessat75CoolingAirFlow(double latentEffectivenessat75CoolingAirFlow);

    double nominalElectricPower() const;

    bool setNominalElectricPower(double nominalElectricPower);

    bool supplyAirOutletTemperatureControl() const;

    bool setSupplyAirOutletTemperatureControl(bool supplyAirOutletTemperatureControl);

    std::string heatExchangerType() const;

    bool setHeatExchangerType(const std::string& heatExchangerType);

    std::string frostControlType() const;

    bool setFrostControlType(const std::string& frostControlType);

    double thresholdTemperature() const;

    bool isThresholdTemperatureDefaulted() const;

    bool setThresholdTemperature(double thresholdTemperature);

    void resetThresholdTemperature();

    boost::optional<double> initialDefrostTimeFraction() const;

    bool setInitialDefrostTimeFraction(double initialDefrostTimeFraction);

    void resetInitialDefrostTimeFraction();

    boost::optional<double> rateofDefrostTimeFractionIncrease() const;

    bool setRateofDefrostTimeFractionIncrease(double rateofDefrostTimeFractionIncrease);

    void resetRateofDefrostTimeFractionIncrease();

    bool economizerLockout() const;

    bool setEconomizerLockout(bool economizerLockout);

    boost::optional<Curve> sensibleEffectivenessofHeatingAirFlowCurve() const;
    bool setSensibleEffectivenessofHeatingAirFlowCurve(const Curve& sensibleEffectivenessofHeatingAirFlowCurve);
    void resetSensibleEffectivenessofHeatingAirFlowCurve();

    boost::optional<Curve> latentEffectivenessofHeatingAirFlowCurve() const;
    bool setLatentEffectivenessofHeatingAirFlowCurve(const Curve& latentEffectivenessofHeatingAirFlowCurve);
    void resetLatentEffectivenessofHeatingAirFlowCurve();

    boost::optional<Curve> sensibleEffectivenessofCoolingAirFlowCurve() const;
    bool setSensibleEffectivenessofCoolingAirFlowCurve(const Curve& sensibleEffectivenessofCoolingAirFlowCurve);
    void resetSensibleEffectivenessofCoolingAirFlowCurve();

    boost::optional<Curve> latentEffectivenessofCoolingAirFlowCurve() const;
    bool setLatentEffectivenessofCoolingAirFlowCurve(const Curve& latentEffectivenessofCoolingAirFlowCurve);
    void resetLatentEffectivenessofCoolingAirFlowCurve();

    /** @name Other */
    //@{

    boost::optional<double> autosizedNominalSupplyAirFlowRate() const;

    AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);
    boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

    // Helper that creates defaulted Optional Curves (TableLookups) to match the pre E+ 24.1.0 effectiveness defaults
    bool assignHistoricalEffectivenessCurves();

    //@}

   protected:
    /// @cond
    using ImplType = detail::HeatExchangerAirToAirSensibleAndLatent_Impl;

    explicit HeatExchangerAirToAirSensibleAndLatent(std::shared_ptr<detail::HeatExchangerAirToAirSensibleAndLatent_Impl> impl);

    friend class detail::HeatExchangerAirToAirSensibleAndLatent_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    /// @endcond

   private:
    REGISTER_LOGGER("openstudio.model.HeatExchangerAirToAirSensibleAndLatent");
  };

  /** \relates HeatExchangerAirToAirSensibleAndLatent*/
  using OptionalHeatExchangerAirToAirSensibleAndLatent = boost::optional<HeatExchangerAirToAirSensibleAndLatent>;

  /** \relates HeatExchangerAirToAirSensibleAndLatent*/
  using HeatExchangerAirToAirSensibleAndLatentVector = std::vector<HeatExchangerAirToAirSensibleAndLatent>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_HEATEXCHANGERAIRTOAIRSENSIBLEANDLATENT_HPP
