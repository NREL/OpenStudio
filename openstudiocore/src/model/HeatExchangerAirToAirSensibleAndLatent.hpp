/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

class Quantity;
class OSOptionalQuantity;

namespace model {

class Schedule;
class AirflowNetworkEquivalentDuct;

namespace detail {

  class HeatExchangerAirToAirSensibleAndLatent_Impl;

} // detail

/** HeatExchangerAirToAirSensibleAndLatent is a AirToAirComponent that wraps the OpenStudio IDD object 'OS:HeatExchanger:AirToAir:SensibleAndLatent'. */
class MODEL_API HeatExchangerAirToAirSensibleAndLatent : public AirToAirComponent {

 public:

  explicit HeatExchangerAirToAirSensibleAndLatent(const Model& model);

  virtual ~HeatExchangerAirToAirSensibleAndLatent() {}

  static IddObjectType iddObjectType();

  static std::vector<std::string> heatExchangerTypeValues();

  static std::vector<std::string> frostControlTypeValues();

  Schedule availabilitySchedule() const;

  bool setAvailabilitySchedule(Schedule& schedule);

  boost::optional<double> nominalSupplyAirFlowRate() const;

  OSOptionalQuantity getNominalSupplyAirFlowRate(bool returnIP=false) const;

  bool isNominalSupplyAirFlowRateAutosized() const;

  bool setNominalSupplyAirFlowRate(double nominalSupplyAirFlowRate);

  bool setNominalSupplyAirFlowRate(const Quantity& nominalSupplyAirFlowRate);

  void autosizeNominalSupplyAirFlowRate();

  double sensibleEffectivenessat100HeatingAirFlow() const;

  Quantity getSensibleEffectivenessat100HeatingAirFlow(bool returnIP=false) const;

  bool setSensibleEffectivenessat100HeatingAirFlow(double sensibleEffectivenessat100HeatingAirFlow);

  bool setSensibleEffectivenessat100HeatingAirFlow(const Quantity& sensibleEffectivenessat100HeatingAirFlow);

  double latentEffectivenessat100HeatingAirFlow() const;

  Quantity getLatentEffectivenessat100HeatingAirFlow(bool returnIP=false) const;

  bool setLatentEffectivenessat100HeatingAirFlow(double latentEffectivenessat100HeatingAirFlow);

  bool setLatentEffectivenessat100HeatingAirFlow(const Quantity& latentEffectivenessat100HeatingAirFlow);

  double sensibleEffectivenessat75HeatingAirFlow() const;

  Quantity getSensibleEffectivenessat75HeatingAirFlow(bool returnIP=false) const;

  bool setSensibleEffectivenessat75HeatingAirFlow(double sensibleEffectivenessat75HeatingAirFlow);

  bool setSensibleEffectivenessat75HeatingAirFlow(const Quantity& sensibleEffectivenessat75HeatingAirFlow);

  double latentEffectivenessat75HeatingAirFlow() const;

  Quantity getLatentEffectivenessat75HeatingAirFlow(bool returnIP=false) const;

  bool setLatentEffectivenessat75HeatingAirFlow(double latentEffectivenessat75HeatingAirFlow);

  bool setLatentEffectivenessat75HeatingAirFlow(const Quantity& latentEffectivenessat75HeatingAirFlow);

  double sensibleEffectivenessat100CoolingAirFlow() const;

  Quantity getSensibleEffectivenessat100CoolingAirFlow(bool returnIP=false) const;

  bool setSensibleEffectivenessat100CoolingAirFlow(double sensibleEffectivenessat100CoolingAirFlow);

  bool setSensibleEffectivenessat100CoolingAirFlow(const Quantity& sensibleEffectivenessat100CoolingAirFlow);

  double latentEffectivenessat100CoolingAirFlow() const;

  Quantity getLatentEffectivenessat100CoolingAirFlow(bool returnIP=false) const;

  bool setLatentEffectivenessat100CoolingAirFlow(double latentEffectivenessat100CoolingAirFlow);

  bool setLatentEffectivenessat100CoolingAirFlow(const Quantity& latentEffectivenessat100CoolingAirFlow);

  double sensibleEffectivenessat75CoolingAirFlow() const;

  Quantity getSensibleEffectivenessat75CoolingAirFlow(bool returnIP=false) const;

  bool setSensibleEffectivenessat75CoolingAirFlow(double sensibleEffectivenessat75CoolingAirFlow);

  bool setSensibleEffectivenessat75CoolingAirFlow(const Quantity& sensibleEffectivenessat75CoolingAirFlow);

  double latentEffectivenessat75CoolingAirFlow() const;

  Quantity getLatentEffectivenessat75CoolingAirFlow(bool returnIP=false) const;

  bool setLatentEffectivenessat75CoolingAirFlow(double latentEffectivenessat75CoolingAirFlow);

  bool setLatentEffectivenessat75CoolingAirFlow(const Quantity& latentEffectivenessat75CoolingAirFlow);

  double nominalElectricPower() const;

  Quantity getNominalElectricPower(bool returnIP=false) const;

  bool setNominalElectricPower(double nominalElectricPower);

  bool setNominalElectricPower(const Quantity& nominalElectricPower);

  bool supplyAirOutletTemperatureControl() const;

  bool setSupplyAirOutletTemperatureControl(bool supplyAirOutletTemperatureControl);

  std::string heatExchangerType() const;

  bool setHeatExchangerType(std::string heatExchangerType);

  std::string frostControlType() const;

  bool setFrostControlType(std::string frostControlType);

  double thresholdTemperature() const;

  Quantity getThresholdTemperature(bool returnIP=false) const;

  bool isThresholdTemperatureDefaulted() const;

  bool setThresholdTemperature(double thresholdTemperature);

  bool setThresholdTemperature(const Quantity& thresholdTemperature);

  void resetThresholdTemperature();

  boost::optional<double> initialDefrostTimeFraction() const;

  OSOptionalQuantity getInitialDefrostTimeFraction(bool returnIP=false) const;

  bool setInitialDefrostTimeFraction(double initialDefrostTimeFraction);

  bool setInitialDefrostTimeFraction(const Quantity& initialDefrostTimeFraction);

  void resetInitialDefrostTimeFraction();

  boost::optional<double> rateofDefrostTimeFractionIncrease() const;

  OSOptionalQuantity getRateofDefrostTimeFractionIncrease(bool returnIP=false) const;

  bool setRateofDefrostTimeFractionIncrease(double rateofDefrostTimeFractionIncrease);

  bool setRateofDefrostTimeFractionIncrease(const Quantity& rateofDefrostTimeFractionIncrease);

  void resetRateofDefrostTimeFractionIncrease();

  bool economizerLockout() const;

  bool setEconomizerLockout(bool economizerLockout);

  boost::optional<double> autosizedNominalSupplyAirFlowRate() const ;



  AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);
  boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

 protected:

  /// @cond
  typedef detail::HeatExchangerAirToAirSensibleAndLatent_Impl ImplType;

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
typedef boost::optional<HeatExchangerAirToAirSensibleAndLatent> OptionalHeatExchangerAirToAirSensibleAndLatent;

/** \relates HeatExchangerAirToAirSensibleAndLatent*/
typedef std::vector<HeatExchangerAirToAirSensibleAndLatent> HeatExchangerAirToAirSensibleAndLatentVector;

} // model
} // openstudio

#endif // MODEL_HEATEXCHANGERAIRTOAIRSENSIBLEANDLATENT_HPP
