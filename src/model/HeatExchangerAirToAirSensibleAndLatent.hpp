/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_HEATEXCHANGERAIRTOAIRSENSIBLEANDLATENT_HPP
#define MODEL_HEATEXCHANGERAIRTOAIRSENSIBLEANDLATENT_HPP

#include "ModelAPI.hpp"
#include "AirToAirComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class AirflowNetworkEquivalentDuct;

  namespace detail {

    class HeatExchangerAirToAirSensibleAndLatent_Impl;

  }  // namespace detail

  /** HeatExchangerAirToAirSensibleAndLatent is a AirToAirComponent that wraps the OpenStudio IDD object 'OS:HeatExchanger:AirToAir:SensibleAndLatent'. */
  class MODEL_API HeatExchangerAirToAirSensibleAndLatent : public AirToAirComponent
  {

   public:
    explicit HeatExchangerAirToAirSensibleAndLatent(const Model& model);

    virtual ~HeatExchangerAirToAirSensibleAndLatent() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    HeatExchangerAirToAirSensibleAndLatent(const HeatExchangerAirToAirSensibleAndLatent& other) = default;
    HeatExchangerAirToAirSensibleAndLatent(HeatExchangerAirToAirSensibleAndLatent&& other) = default;
    HeatExchangerAirToAirSensibleAndLatent& operator=(const HeatExchangerAirToAirSensibleAndLatent&) = default;
    HeatExchangerAirToAirSensibleAndLatent& operator=(HeatExchangerAirToAirSensibleAndLatent&&) = default;

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

    double sensibleEffectivenessat75HeatingAirFlow() const;

    bool setSensibleEffectivenessat75HeatingAirFlow(double sensibleEffectivenessat75HeatingAirFlow);

    double latentEffectivenessat75HeatingAirFlow() const;

    bool setLatentEffectivenessat75HeatingAirFlow(double latentEffectivenessat75HeatingAirFlow);

    double sensibleEffectivenessat100CoolingAirFlow() const;

    bool setSensibleEffectivenessat100CoolingAirFlow(double sensibleEffectivenessat100CoolingAirFlow);

    double latentEffectivenessat100CoolingAirFlow() const;

    bool setLatentEffectivenessat100CoolingAirFlow(double latentEffectivenessat100CoolingAirFlow);

    double sensibleEffectivenessat75CoolingAirFlow() const;

    bool setSensibleEffectivenessat75CoolingAirFlow(double sensibleEffectivenessat75CoolingAirFlow);

    double latentEffectivenessat75CoolingAirFlow() const;

    bool setLatentEffectivenessat75CoolingAirFlow(double latentEffectivenessat75CoolingAirFlow);

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

    boost::optional<double> autosizedNominalSupplyAirFlowRate() const;

    AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);
    boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

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
