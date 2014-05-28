/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_HEATEXCHANGERAIRTOAIRSENSIBLEANDLATENT_HPP
#define MODEL_HEATEXCHANGERAIRTOAIRSENSIBLEANDLATENT_HPP

#include "ModelAPI.hpp"
#include "AirToAirComponent.hpp"

namespace openstudio {

class Quantity;
class OSOptionalQuantity;

namespace model {

class Schedule;

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

  void setSupplyAirOutletTemperatureControl(bool supplyAirOutletTemperatureControl);

  std::string heatExchangerType() const;

  bool setHeatExchangerType(std::string heatExchangerType);

  std::string frostControlType() const;

  bool setFrostControlType(std::string frostControlType);

  double thresholdTemperature() const;

  Quantity getThresholdTemperature(bool returnIP=false) const;

  bool isThresholdTemperatureDefaulted() const;

  void setThresholdTemperature(double thresholdTemperature);

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

  void setEconomizerLockout(bool economizerLockout);

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

