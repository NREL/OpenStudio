/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
