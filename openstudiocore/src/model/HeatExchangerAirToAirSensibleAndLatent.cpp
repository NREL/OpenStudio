/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_HeatExchanger_AirToAir_SensibleAndLatent_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail {

  HeatExchangerAirToAirSensibleAndLatent_Impl::HeatExchangerAirToAirSensibleAndLatent_Impl(const IdfObject& idfObject,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : AirToAirComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == HeatExchangerAirToAirSensibleAndLatent::iddObjectType());
  }

  HeatExchangerAirToAirSensibleAndLatent_Impl::HeatExchangerAirToAirSensibleAndLatent_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : AirToAirComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == HeatExchangerAirToAirSensibleAndLatent::iddObjectType());
  }

  HeatExchangerAirToAirSensibleAndLatent_Impl::HeatExchangerAirToAirSensibleAndLatent_Impl(const HeatExchangerAirToAirSensibleAndLatent_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : AirToAirComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& HeatExchangerAirToAirSensibleAndLatent_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType HeatExchangerAirToAirSensibleAndLatent_Impl::iddObjectType() const {
    return HeatExchangerAirToAirSensibleAndLatent::iddObjectType();
  }

  std::vector<ScheduleTypeKey> HeatExchangerAirToAirSensibleAndLatent_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_HeatExchanger_AirToAir_SensibleAndLatentFields::AvailabilitySchedule) != e)
    {
      result.push_back(ScheduleTypeKey("HeatExchangerAirToAirSensibleAndLatent","Availability"));
    }
    return result;
  }

  Schedule HeatExchangerAirToAirSensibleAndLatent_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
    }
    return value.get();
  }

  boost::optional<double> HeatExchangerAirToAirSensibleAndLatent_Impl::nominalSupplyAirFlowRate() const {
    return getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::NominalSupplyAirFlowRate,true);
  }

  OSOptionalQuantity HeatExchangerAirToAirSensibleAndLatent_Impl::getNominalSupplyAirFlowRate(bool returnIP) const {
    OptionalDouble value = nominalSupplyAirFlowRate();
    return getQuantityFromDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::NominalSupplyAirFlowRate, value, returnIP);
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::isNominalSupplyAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::NominalSupplyAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double HeatExchangerAirToAirSensibleAndLatent_Impl::sensibleEffectivenessat100HeatingAirFlow() const {
    boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat100_HeatingAirFlow,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::getSensibleEffectivenessat100HeatingAirFlow(bool returnIP) const {
    OptionalDouble value = sensibleEffectivenessat100HeatingAirFlow();
    OSOptionalQuantity result = getQuantityFromDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat100_HeatingAirFlow, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double HeatExchangerAirToAirSensibleAndLatent_Impl::latentEffectivenessat100HeatingAirFlow() const {
    boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat100_HeatingAirFlow,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::getLatentEffectivenessat100HeatingAirFlow(bool returnIP) const {
    OptionalDouble value = latentEffectivenessat100HeatingAirFlow();
    OSOptionalQuantity result = getQuantityFromDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat100_HeatingAirFlow, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double HeatExchangerAirToAirSensibleAndLatent_Impl::sensibleEffectivenessat75HeatingAirFlow() const {
    boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat75_HeatingAirFlow,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::getSensibleEffectivenessat75HeatingAirFlow(bool returnIP) const {
    OptionalDouble value = sensibleEffectivenessat75HeatingAirFlow();
    OSOptionalQuantity result = getQuantityFromDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat75_HeatingAirFlow, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double HeatExchangerAirToAirSensibleAndLatent_Impl::latentEffectivenessat75HeatingAirFlow() const {
    boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat75_HeatingAirFlow,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::getLatentEffectivenessat75HeatingAirFlow(bool returnIP) const {
    OptionalDouble value = latentEffectivenessat75HeatingAirFlow();
    OSOptionalQuantity result = getQuantityFromDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat75_HeatingAirFlow, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double HeatExchangerAirToAirSensibleAndLatent_Impl::sensibleEffectivenessat100CoolingAirFlow() const {
    boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat100_CoolingAirFlow,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::getSensibleEffectivenessat100CoolingAirFlow(bool returnIP) const {
    OptionalDouble value = sensibleEffectivenessat100CoolingAirFlow();
    OSOptionalQuantity result = getQuantityFromDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat100_CoolingAirFlow, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double HeatExchangerAirToAirSensibleAndLatent_Impl::latentEffectivenessat100CoolingAirFlow() const {
    boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat100_CoolingAirFlow,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::getLatentEffectivenessat100CoolingAirFlow(bool returnIP) const {
    OptionalDouble value = latentEffectivenessat100CoolingAirFlow();
    OSOptionalQuantity result = getQuantityFromDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat100_CoolingAirFlow, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double HeatExchangerAirToAirSensibleAndLatent_Impl::sensibleEffectivenessat75CoolingAirFlow() const {
    boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat75_CoolingAirFlow,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::getSensibleEffectivenessat75CoolingAirFlow(bool returnIP) const {
    OptionalDouble value = sensibleEffectivenessat75CoolingAirFlow();
    OSOptionalQuantity result = getQuantityFromDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat75_CoolingAirFlow, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double HeatExchangerAirToAirSensibleAndLatent_Impl::latentEffectivenessat75CoolingAirFlow() const {
    boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat75_CoolingAirFlow,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::getLatentEffectivenessat75CoolingAirFlow(bool returnIP) const {
    OptionalDouble value = latentEffectivenessat75CoolingAirFlow();
    OSOptionalQuantity result = getQuantityFromDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat75_CoolingAirFlow, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  double HeatExchangerAirToAirSensibleAndLatent_Impl::nominalElectricPower() const {
    boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::NominalElectricPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::getNominalElectricPower(bool returnIP) const {
    OptionalDouble value = nominalElectricPower();
    OSOptionalQuantity result = getQuantityFromDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::NominalElectricPower, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::supplyAirOutletTemperatureControl() const {
    boost::optional<std::string> value = getString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirOutletTemperatureControl,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  std::string HeatExchangerAirToAirSensibleAndLatent_Impl::heatExchangerType() const {
    boost::optional<std::string> value = getString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::HeatExchangerType,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string HeatExchangerAirToAirSensibleAndLatent_Impl::frostControlType() const {
    boost::optional<std::string> value = getString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::FrostControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeatExchangerAirToAirSensibleAndLatent_Impl::thresholdTemperature() const {
    boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::ThresholdTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::getThresholdTemperature(bool returnIP) const {
    OptionalDouble value = thresholdTemperature();
    OSOptionalQuantity result = getQuantityFromDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::ThresholdTemperature, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::isThresholdTemperatureDefaulted() const {
    return isEmpty(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::ThresholdTemperature);
  }

  boost::optional<double> HeatExchangerAirToAirSensibleAndLatent_Impl::initialDefrostTimeFraction() const {
    return getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::InitialDefrostTimeFraction,true);
  }

  OSOptionalQuantity HeatExchangerAirToAirSensibleAndLatent_Impl::getInitialDefrostTimeFraction(bool returnIP) const {
    OptionalDouble value = initialDefrostTimeFraction();
    return getQuantityFromDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::InitialDefrostTimeFraction, value, returnIP);
  }

  boost::optional<double> HeatExchangerAirToAirSensibleAndLatent_Impl::rateofDefrostTimeFractionIncrease() const {
    return getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::RateofDefrostTimeFractionIncrease,true);
  }

  OSOptionalQuantity HeatExchangerAirToAirSensibleAndLatent_Impl::getRateofDefrostTimeFractionIncrease(bool returnIP) const {
    OptionalDouble value = rateofDefrostTimeFractionIncrease();
    return getQuantityFromDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::RateofDefrostTimeFractionIncrease, value, returnIP);
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::economizerLockout() const {
    boost::optional<std::string> value = getString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::EconomizerLockout,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::AvailabilitySchedule,
                              "HeatExchangerAirToAirSensibleAndLatent",
                              "Availability",
                              schedule);
    return result;
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setNominalSupplyAirFlowRate(boost::optional<double> nominalSupplyAirFlowRate) {
    bool result(false);
    if (nominalSupplyAirFlowRate) {
      result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::NominalSupplyAirFlowRate, nominalSupplyAirFlowRate.get());
    }
    return result;
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setNominalSupplyAirFlowRate(const OSOptionalQuantity& nominalSupplyAirFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (nominalSupplyAirFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::NominalSupplyAirFlowRate,nominalSupplyAirFlowRate.get());
      if (value) {
        result = setNominalSupplyAirFlowRate(value);
      }
    }
    else {
      result = setNominalSupplyAirFlowRate(value);
    }
    return result;
  }

  void HeatExchangerAirToAirSensibleAndLatent_Impl::autosizeNominalSupplyAirFlowRate() {
    bool result = setString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::NominalSupplyAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setSensibleEffectivenessat100HeatingAirFlow(double sensibleEffectivenessat100HeatingAirFlow) {
    bool result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat100_HeatingAirFlow, sensibleEffectivenessat100HeatingAirFlow);
    return result;
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setSensibleEffectivenessat100HeatingAirFlow(const Quantity& sensibleEffectivenessat100HeatingAirFlow) {
    OptionalDouble value = getDoubleFromQuantity(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat100_HeatingAirFlow,sensibleEffectivenessat100HeatingAirFlow);
    if (!value) {
      return false;
    }
    return setSensibleEffectivenessat100HeatingAirFlow(value.get());
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setLatentEffectivenessat100HeatingAirFlow(double latentEffectivenessat100HeatingAirFlow) {
    bool result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat100_HeatingAirFlow, latentEffectivenessat100HeatingAirFlow);
    return result;
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setLatentEffectivenessat100HeatingAirFlow(const Quantity& latentEffectivenessat100HeatingAirFlow) {
    OptionalDouble value = getDoubleFromQuantity(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat100_HeatingAirFlow,latentEffectivenessat100HeatingAirFlow);
    if (!value) {
      return false;
    }
    return setLatentEffectivenessat100HeatingAirFlow(value.get());
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setSensibleEffectivenessat75HeatingAirFlow(double sensibleEffectivenessat75HeatingAirFlow) {
    bool result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat75_HeatingAirFlow, sensibleEffectivenessat75HeatingAirFlow);
    return result;
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setSensibleEffectivenessat75HeatingAirFlow(const Quantity& sensibleEffectivenessat75HeatingAirFlow) {
    OptionalDouble value = getDoubleFromQuantity(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat75_HeatingAirFlow,sensibleEffectivenessat75HeatingAirFlow);
    if (!value) {
      return false;
    }
    return setSensibleEffectivenessat75HeatingAirFlow(value.get());
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setLatentEffectivenessat75HeatingAirFlow(double latentEffectivenessat75HeatingAirFlow) {
    bool result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat75_HeatingAirFlow, latentEffectivenessat75HeatingAirFlow);
    return result;
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setLatentEffectivenessat75HeatingAirFlow(const Quantity& latentEffectivenessat75HeatingAirFlow) {
    OptionalDouble value = getDoubleFromQuantity(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat75_HeatingAirFlow,latentEffectivenessat75HeatingAirFlow);
    if (!value) {
      return false;
    }
    return setLatentEffectivenessat75HeatingAirFlow(value.get());
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setSensibleEffectivenessat100CoolingAirFlow(double sensibleEffectivenessat100CoolingAirFlow) {
    bool result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat100_CoolingAirFlow, sensibleEffectivenessat100CoolingAirFlow);
    return result;
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setSensibleEffectivenessat100CoolingAirFlow(const Quantity& sensibleEffectivenessat100CoolingAirFlow) {
    OptionalDouble value = getDoubleFromQuantity(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat100_CoolingAirFlow,sensibleEffectivenessat100CoolingAirFlow);
    if (!value) {
      return false;
    }
    return setSensibleEffectivenessat100CoolingAirFlow(value.get());
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setLatentEffectivenessat100CoolingAirFlow(double latentEffectivenessat100CoolingAirFlow) {
    bool result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat100_CoolingAirFlow, latentEffectivenessat100CoolingAirFlow);
    return result;
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setLatentEffectivenessat100CoolingAirFlow(const Quantity& latentEffectivenessat100CoolingAirFlow) {
    OptionalDouble value = getDoubleFromQuantity(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat100_CoolingAirFlow,latentEffectivenessat100CoolingAirFlow);
    if (!value) {
      return false;
    }
    return setLatentEffectivenessat100CoolingAirFlow(value.get());
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setSensibleEffectivenessat75CoolingAirFlow(double sensibleEffectivenessat75CoolingAirFlow) {
    bool result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat75_CoolingAirFlow, sensibleEffectivenessat75CoolingAirFlow);
    return result;
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setSensibleEffectivenessat75CoolingAirFlow(const Quantity& sensibleEffectivenessat75CoolingAirFlow) {
    OptionalDouble value = getDoubleFromQuantity(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat75_CoolingAirFlow,sensibleEffectivenessat75CoolingAirFlow);
    if (!value) {
      return false;
    }
    return setSensibleEffectivenessat75CoolingAirFlow(value.get());
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setLatentEffectivenessat75CoolingAirFlow(double latentEffectivenessat75CoolingAirFlow) {
    bool result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat75_CoolingAirFlow, latentEffectivenessat75CoolingAirFlow);
    return result;
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setLatentEffectivenessat75CoolingAirFlow(const Quantity& latentEffectivenessat75CoolingAirFlow) {
    OptionalDouble value = getDoubleFromQuantity(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat75_CoolingAirFlow,latentEffectivenessat75CoolingAirFlow);
    if (!value) {
      return false;
    }
    return setLatentEffectivenessat75CoolingAirFlow(value.get());
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setNominalElectricPower(double nominalElectricPower) {
    bool result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::NominalElectricPower, nominalElectricPower);
    return result;
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setNominalElectricPower(const Quantity& nominalElectricPower) {
    OptionalDouble value = getDoubleFromQuantity(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::NominalElectricPower,nominalElectricPower);
    if (!value) {
      return false;
    }
    return setNominalElectricPower(value.get());
  }

  void HeatExchangerAirToAirSensibleAndLatent_Impl::setSupplyAirOutletTemperatureControl(bool supplyAirOutletTemperatureControl) {
    setBooleanFieldValue(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirOutletTemperatureControl, supplyAirOutletTemperatureControl);
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setHeatExchangerType(std::string heatExchangerType) {
    bool result = setString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::HeatExchangerType, heatExchangerType);
    return result;
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setFrostControlType(std::string frostControlType) {
    bool result = setString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::FrostControlType, frostControlType);
    return result;
  }

  void HeatExchangerAirToAirSensibleAndLatent_Impl::setThresholdTemperature(double thresholdTemperature) {
    bool result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::ThresholdTemperature, thresholdTemperature);
    OS_ASSERT(result);
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setThresholdTemperature(const Quantity& thresholdTemperature) {
    OptionalDouble value = getDoubleFromQuantity(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::ThresholdTemperature,thresholdTemperature);
    if (!value) {
      return false;
    }
    setThresholdTemperature(value.get());
    return true;
  }

  void HeatExchangerAirToAirSensibleAndLatent_Impl::resetThresholdTemperature() {
    bool result = setString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::ThresholdTemperature, "");
    OS_ASSERT(result);
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setInitialDefrostTimeFraction(boost::optional<double> initialDefrostTimeFraction) {
    bool result(false);
    if (initialDefrostTimeFraction) {
      result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::InitialDefrostTimeFraction, initialDefrostTimeFraction.get());
    }
    else {
      resetInitialDefrostTimeFraction();
      result = true;
    }
    return result;
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setInitialDefrostTimeFraction(const OSOptionalQuantity& initialDefrostTimeFraction) {
    bool result(false);
    OptionalDouble value;
    if (initialDefrostTimeFraction.isSet()) {
      value = getDoubleFromQuantity(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::InitialDefrostTimeFraction,initialDefrostTimeFraction.get());
      if (value) {
        result = setInitialDefrostTimeFraction(value);
      }
    }
    else {
      result = setInitialDefrostTimeFraction(value);
    }
    return result;
  }

  void HeatExchangerAirToAirSensibleAndLatent_Impl::resetInitialDefrostTimeFraction() {
    bool result = setString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::InitialDefrostTimeFraction, "");
    OS_ASSERT(result);
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setRateofDefrostTimeFractionIncrease(boost::optional<double> rateofDefrostTimeFractionIncrease) {
    bool result(false);
    if (rateofDefrostTimeFractionIncrease) {
      result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::RateofDefrostTimeFractionIncrease, rateofDefrostTimeFractionIncrease.get());
    }
    else {
      resetRateofDefrostTimeFractionIncrease();
      result = true;
    }
    return result;
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setRateofDefrostTimeFractionIncrease(const OSOptionalQuantity& rateofDefrostTimeFractionIncrease) {
    bool result(false);
    OptionalDouble value;
    if (rateofDefrostTimeFractionIncrease.isSet()) {
      value = getDoubleFromQuantity(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::RateofDefrostTimeFractionIncrease,rateofDefrostTimeFractionIncrease.get());
      if (value) {
        result = setRateofDefrostTimeFractionIncrease(value);
      }
    }
    else {
      result = setRateofDefrostTimeFractionIncrease(value);
    }
    return result;
  }

  void HeatExchangerAirToAirSensibleAndLatent_Impl::resetRateofDefrostTimeFractionIncrease() {
    bool result = setString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::RateofDefrostTimeFractionIncrease, "");
    OS_ASSERT(result);
  }

  void HeatExchangerAirToAirSensibleAndLatent_Impl::setEconomizerLockout(bool economizerLockout) {
    setBooleanFieldValue(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::EconomizerLockout, economizerLockout);
  }

  boost::optional<Schedule> HeatExchangerAirToAirSensibleAndLatent_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::AvailabilitySchedule);
  }

  openstudio::OSOptionalQuantity HeatExchangerAirToAirSensibleAndLatent_Impl::nominalSupplyAirFlowRate_SI() const {
    return getNominalSupplyAirFlowRate(false);
  }

  openstudio::OSOptionalQuantity HeatExchangerAirToAirSensibleAndLatent_Impl::nominalSupplyAirFlowRate_IP() const {
    return getNominalSupplyAirFlowRate(true);
  }

  openstudio::Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::sensibleEffectivenessat100HeatingAirFlow_SI() const {
    return getSensibleEffectivenessat100HeatingAirFlow(false);
  }

  openstudio::Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::sensibleEffectivenessat100HeatingAirFlow_IP() const {
    return getSensibleEffectivenessat100HeatingAirFlow(true);
  }

  openstudio::Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::latentEffectivenessat100HeatingAirFlow_SI() const {
    return getLatentEffectivenessat100HeatingAirFlow(false);
  }

  openstudio::Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::latentEffectivenessat100HeatingAirFlow_IP() const {
    return getLatentEffectivenessat100HeatingAirFlow(true);
  }

  openstudio::Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::sensibleEffectivenessat75HeatingAirFlow_SI() const {
    return getSensibleEffectivenessat75HeatingAirFlow(false);
  }

  openstudio::Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::sensibleEffectivenessat75HeatingAirFlow_IP() const {
    return getSensibleEffectivenessat75HeatingAirFlow(true);
  }

  openstudio::Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::latentEffectivenessat75HeatingAirFlow_SI() const {
    return getLatentEffectivenessat75HeatingAirFlow(false);
  }

  openstudio::Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::latentEffectivenessat75HeatingAirFlow_IP() const {
    return getLatentEffectivenessat75HeatingAirFlow(true);
  }

  openstudio::Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::sensibleEffectivenessat100CoolingAirFlow_SI() const {
    return getSensibleEffectivenessat100CoolingAirFlow(false);
  }

  openstudio::Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::sensibleEffectivenessat100CoolingAirFlow_IP() const {
    return getSensibleEffectivenessat100CoolingAirFlow(true);
  }

  openstudio::Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::latentEffectivenessat100CoolingAirFlow_SI() const {
    return getLatentEffectivenessat100CoolingAirFlow(false);
  }

  openstudio::Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::latentEffectivenessat100CoolingAirFlow_IP() const {
    return getLatentEffectivenessat100CoolingAirFlow(true);
  }

  openstudio::Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::sensibleEffectivenessat75CoolingAirFlow_SI() const {
    return getSensibleEffectivenessat75CoolingAirFlow(false);
  }

  openstudio::Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::sensibleEffectivenessat75CoolingAirFlow_IP() const {
    return getSensibleEffectivenessat75CoolingAirFlow(true);
  }

  openstudio::Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::latentEffectivenessat75CoolingAirFlow_SI() const {
    return getLatentEffectivenessat75CoolingAirFlow(false);
  }

  openstudio::Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::latentEffectivenessat75CoolingAirFlow_IP() const {
    return getLatentEffectivenessat75CoolingAirFlow(true);
  }

  openstudio::Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::nominalElectricPower_SI() const {
    return getNominalElectricPower(false);
  }

  openstudio::Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::nominalElectricPower_IP() const {
    return getNominalElectricPower(true);
  }

  std::vector<std::string> HeatExchangerAirToAirSensibleAndLatent_Impl::heatExchangerTypeValues() const {
    return HeatExchangerAirToAirSensibleAndLatent::heatExchangerTypeValues();
  }

  std::vector<std::string> HeatExchangerAirToAirSensibleAndLatent_Impl::frostControlTypeValues() const {
    return HeatExchangerAirToAirSensibleAndLatent::frostControlTypeValues();
  }

  openstudio::Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::thresholdTemperature_SI() const {
    return getThresholdTemperature(false);
  }

  openstudio::Quantity HeatExchangerAirToAirSensibleAndLatent_Impl::thresholdTemperature_IP() const {
    return getThresholdTemperature(true);
  }

  openstudio::OSOptionalQuantity HeatExchangerAirToAirSensibleAndLatent_Impl::initialDefrostTimeFraction_SI() const {
    return getInitialDefrostTimeFraction(false);
  }

  openstudio::OSOptionalQuantity HeatExchangerAirToAirSensibleAndLatent_Impl::initialDefrostTimeFraction_IP() const {
    return getInitialDefrostTimeFraction(true);
  }

  openstudio::OSOptionalQuantity HeatExchangerAirToAirSensibleAndLatent_Impl::rateofDefrostTimeFractionIncrease_SI() const {
    return getRateofDefrostTimeFractionIncrease(false);
  }

  openstudio::OSOptionalQuantity HeatExchangerAirToAirSensibleAndLatent_Impl::rateofDefrostTimeFractionIncrease_IP() const {
    return getRateofDefrostTimeFractionIncrease(true);
  }

  boost::optional<ModelObject> HeatExchangerAirToAirSensibleAndLatent_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  bool HeatExchangerAirToAirSensibleAndLatent_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

  unsigned HeatExchangerAirToAirSensibleAndLatent_Impl::primaryAirInletPort()
  {
    return OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirInletNode;
  }

  unsigned HeatExchangerAirToAirSensibleAndLatent_Impl::primaryAirOutletPort()
  {
    return OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirOutletNode;
  }

  unsigned HeatExchangerAirToAirSensibleAndLatent_Impl::secondaryAirInletPort()
  {
    return OS_HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirInletNode;
  }

  unsigned HeatExchangerAirToAirSensibleAndLatent_Impl::secondaryAirOutletPort()
  {
    return OS_HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirOutletNode;
  }

} // detail

HeatExchangerAirToAirSensibleAndLatent::HeatExchangerAirToAirSensibleAndLatent(const Model& model)
  : AirToAirComponent(HeatExchangerAirToAirSensibleAndLatent::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>());

  Schedule schedule = model.alwaysOnDiscreteSchedule();
  setAvailabilitySchedule(schedule);

  autosizeNominalSupplyAirFlowRate();

  setSensibleEffectivenessat100HeatingAirFlow(0.76);

  setLatentEffectivenessat100HeatingAirFlow(0.68);

  setSensibleEffectivenessat75HeatingAirFlow(0.81);

  setLatentEffectivenessat75HeatingAirFlow(0.73);

  setSensibleEffectivenessat100CoolingAirFlow(0.76);

  setLatentEffectivenessat100CoolingAirFlow(0.68);

  setSensibleEffectivenessat75CoolingAirFlow(0.81);

  setLatentEffectivenessat75CoolingAirFlow(0.73);

  setNominalElectricPower(0.0);

  setSupplyAirOutletTemperatureControl(true);

  setHeatExchangerType("Plate");

  setFrostControlType("None");

  setThresholdTemperature(1.7);

  setEconomizerLockout(true);
}

IddObjectType HeatExchangerAirToAirSensibleAndLatent::iddObjectType() {
  return IddObjectType(IddObjectType::OS_HeatExchanger_AirToAir_SensibleAndLatent);
}

std::vector<std::string> HeatExchangerAirToAirSensibleAndLatent::heatExchangerTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_HeatExchanger_AirToAir_SensibleAndLatentFields::HeatExchangerType);
}

std::vector<std::string> HeatExchangerAirToAirSensibleAndLatent::frostControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_HeatExchanger_AirToAir_SensibleAndLatentFields::FrostControlType);
}

Schedule HeatExchangerAirToAirSensibleAndLatent::availabilitySchedule() const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->availabilitySchedule();
}

boost::optional<double> HeatExchangerAirToAirSensibleAndLatent::nominalSupplyAirFlowRate() const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->nominalSupplyAirFlowRate();
}

OSOptionalQuantity HeatExchangerAirToAirSensibleAndLatent::getNominalSupplyAirFlowRate(bool returnIP) const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->getNominalSupplyAirFlowRate(returnIP);
}

bool HeatExchangerAirToAirSensibleAndLatent::isNominalSupplyAirFlowRateAutosized() const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->isNominalSupplyAirFlowRateAutosized();
}

double HeatExchangerAirToAirSensibleAndLatent::sensibleEffectivenessat100HeatingAirFlow() const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->sensibleEffectivenessat100HeatingAirFlow();
}

Quantity HeatExchangerAirToAirSensibleAndLatent::getSensibleEffectivenessat100HeatingAirFlow(bool returnIP) const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->getSensibleEffectivenessat100HeatingAirFlow(returnIP);
}

double HeatExchangerAirToAirSensibleAndLatent::latentEffectivenessat100HeatingAirFlow() const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->latentEffectivenessat100HeatingAirFlow();
}

Quantity HeatExchangerAirToAirSensibleAndLatent::getLatentEffectivenessat100HeatingAirFlow(bool returnIP) const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->getLatentEffectivenessat100HeatingAirFlow(returnIP);
}

double HeatExchangerAirToAirSensibleAndLatent::sensibleEffectivenessat75HeatingAirFlow() const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->sensibleEffectivenessat75HeatingAirFlow();
}

Quantity HeatExchangerAirToAirSensibleAndLatent::getSensibleEffectivenessat75HeatingAirFlow(bool returnIP) const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->getSensibleEffectivenessat75HeatingAirFlow(returnIP);
}

double HeatExchangerAirToAirSensibleAndLatent::latentEffectivenessat75HeatingAirFlow() const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->latentEffectivenessat75HeatingAirFlow();
}

Quantity HeatExchangerAirToAirSensibleAndLatent::getLatentEffectivenessat75HeatingAirFlow(bool returnIP) const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->getLatentEffectivenessat75HeatingAirFlow(returnIP);
}

double HeatExchangerAirToAirSensibleAndLatent::sensibleEffectivenessat100CoolingAirFlow() const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->sensibleEffectivenessat100CoolingAirFlow();
}

Quantity HeatExchangerAirToAirSensibleAndLatent::getSensibleEffectivenessat100CoolingAirFlow(bool returnIP) const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->getSensibleEffectivenessat100CoolingAirFlow(returnIP);
}

double HeatExchangerAirToAirSensibleAndLatent::latentEffectivenessat100CoolingAirFlow() const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->latentEffectivenessat100CoolingAirFlow();
}

Quantity HeatExchangerAirToAirSensibleAndLatent::getLatentEffectivenessat100CoolingAirFlow(bool returnIP) const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->getLatentEffectivenessat100CoolingAirFlow(returnIP);
}

double HeatExchangerAirToAirSensibleAndLatent::sensibleEffectivenessat75CoolingAirFlow() const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->sensibleEffectivenessat75CoolingAirFlow();
}

Quantity HeatExchangerAirToAirSensibleAndLatent::getSensibleEffectivenessat75CoolingAirFlow(bool returnIP) const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->getSensibleEffectivenessat75CoolingAirFlow(returnIP);
}

double HeatExchangerAirToAirSensibleAndLatent::latentEffectivenessat75CoolingAirFlow() const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->latentEffectivenessat75CoolingAirFlow();
}

Quantity HeatExchangerAirToAirSensibleAndLatent::getLatentEffectivenessat75CoolingAirFlow(bool returnIP) const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->getLatentEffectivenessat75CoolingAirFlow(returnIP);
}

double HeatExchangerAirToAirSensibleAndLatent::nominalElectricPower() const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->nominalElectricPower();
}

Quantity HeatExchangerAirToAirSensibleAndLatent::getNominalElectricPower(bool returnIP) const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->getNominalElectricPower(returnIP);
}

bool HeatExchangerAirToAirSensibleAndLatent::supplyAirOutletTemperatureControl() const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->supplyAirOutletTemperatureControl();
}

std::string HeatExchangerAirToAirSensibleAndLatent::heatExchangerType() const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->heatExchangerType();
}

std::string HeatExchangerAirToAirSensibleAndLatent::frostControlType() const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->frostControlType();
}

double HeatExchangerAirToAirSensibleAndLatent::thresholdTemperature() const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->thresholdTemperature();
}

Quantity HeatExchangerAirToAirSensibleAndLatent::getThresholdTemperature(bool returnIP) const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->getThresholdTemperature(returnIP);
}

bool HeatExchangerAirToAirSensibleAndLatent::isThresholdTemperatureDefaulted() const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->isThresholdTemperatureDefaulted();
}

boost::optional<double> HeatExchangerAirToAirSensibleAndLatent::initialDefrostTimeFraction() const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->initialDefrostTimeFraction();
}

OSOptionalQuantity HeatExchangerAirToAirSensibleAndLatent::getInitialDefrostTimeFraction(bool returnIP) const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->getInitialDefrostTimeFraction(returnIP);
}

boost::optional<double> HeatExchangerAirToAirSensibleAndLatent::rateofDefrostTimeFractionIncrease() const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->rateofDefrostTimeFractionIncrease();
}

OSOptionalQuantity HeatExchangerAirToAirSensibleAndLatent::getRateofDefrostTimeFractionIncrease(bool returnIP) const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->getRateofDefrostTimeFractionIncrease(returnIP);
}

bool HeatExchangerAirToAirSensibleAndLatent::economizerLockout() const {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->economizerLockout();
}

bool HeatExchangerAirToAirSensibleAndLatent::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setAvailabilitySchedule(schedule);
}

bool HeatExchangerAirToAirSensibleAndLatent::setNominalSupplyAirFlowRate(double nominalSupplyAirFlowRate) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setNominalSupplyAirFlowRate(nominalSupplyAirFlowRate);
}

bool HeatExchangerAirToAirSensibleAndLatent::setNominalSupplyAirFlowRate(const Quantity& nominalSupplyAirFlowRate) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setNominalSupplyAirFlowRate(nominalSupplyAirFlowRate);
}

void HeatExchangerAirToAirSensibleAndLatent::autosizeNominalSupplyAirFlowRate() {
  getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->autosizeNominalSupplyAirFlowRate();
}

bool HeatExchangerAirToAirSensibleAndLatent::setSensibleEffectivenessat100HeatingAirFlow(double sensibleEffectivenessat100HeatingAirFlow) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setSensibleEffectivenessat100HeatingAirFlow(sensibleEffectivenessat100HeatingAirFlow);
}

bool HeatExchangerAirToAirSensibleAndLatent::setSensibleEffectivenessat100HeatingAirFlow(const Quantity& sensibleEffectivenessat100HeatingAirFlow) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setSensibleEffectivenessat100HeatingAirFlow(sensibleEffectivenessat100HeatingAirFlow);
}

bool HeatExchangerAirToAirSensibleAndLatent::setLatentEffectivenessat100HeatingAirFlow(double latentEffectivenessat100HeatingAirFlow) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setLatentEffectivenessat100HeatingAirFlow(latentEffectivenessat100HeatingAirFlow);
}

bool HeatExchangerAirToAirSensibleAndLatent::setLatentEffectivenessat100HeatingAirFlow(const Quantity& latentEffectivenessat100HeatingAirFlow) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setLatentEffectivenessat100HeatingAirFlow(latentEffectivenessat100HeatingAirFlow);
}

bool HeatExchangerAirToAirSensibleAndLatent::setSensibleEffectivenessat75HeatingAirFlow(double sensibleEffectivenessat75HeatingAirFlow) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setSensibleEffectivenessat75HeatingAirFlow(sensibleEffectivenessat75HeatingAirFlow);
}

bool HeatExchangerAirToAirSensibleAndLatent::setSensibleEffectivenessat75HeatingAirFlow(const Quantity& sensibleEffectivenessat75HeatingAirFlow) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setSensibleEffectivenessat75HeatingAirFlow(sensibleEffectivenessat75HeatingAirFlow);
}

bool HeatExchangerAirToAirSensibleAndLatent::setLatentEffectivenessat75HeatingAirFlow(double latentEffectivenessat75HeatingAirFlow) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setLatentEffectivenessat75HeatingAirFlow(latentEffectivenessat75HeatingAirFlow);
}

bool HeatExchangerAirToAirSensibleAndLatent::setLatentEffectivenessat75HeatingAirFlow(const Quantity& latentEffectivenessat75HeatingAirFlow) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setLatentEffectivenessat75HeatingAirFlow(latentEffectivenessat75HeatingAirFlow);
}

bool HeatExchangerAirToAirSensibleAndLatent::setSensibleEffectivenessat100CoolingAirFlow(double sensibleEffectivenessat100CoolingAirFlow) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setSensibleEffectivenessat100CoolingAirFlow(sensibleEffectivenessat100CoolingAirFlow);
}

bool HeatExchangerAirToAirSensibleAndLatent::setSensibleEffectivenessat100CoolingAirFlow(const Quantity& sensibleEffectivenessat100CoolingAirFlow) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setSensibleEffectivenessat100CoolingAirFlow(sensibleEffectivenessat100CoolingAirFlow);
}

bool HeatExchangerAirToAirSensibleAndLatent::setLatentEffectivenessat100CoolingAirFlow(double latentEffectivenessat100CoolingAirFlow) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setLatentEffectivenessat100CoolingAirFlow(latentEffectivenessat100CoolingAirFlow);
}

bool HeatExchangerAirToAirSensibleAndLatent::setLatentEffectivenessat100CoolingAirFlow(const Quantity& latentEffectivenessat100CoolingAirFlow) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setLatentEffectivenessat100CoolingAirFlow(latentEffectivenessat100CoolingAirFlow);
}

bool HeatExchangerAirToAirSensibleAndLatent::setSensibleEffectivenessat75CoolingAirFlow(double sensibleEffectivenessat75CoolingAirFlow) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setSensibleEffectivenessat75CoolingAirFlow(sensibleEffectivenessat75CoolingAirFlow);
}

bool HeatExchangerAirToAirSensibleAndLatent::setSensibleEffectivenessat75CoolingAirFlow(const Quantity& sensibleEffectivenessat75CoolingAirFlow) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setSensibleEffectivenessat75CoolingAirFlow(sensibleEffectivenessat75CoolingAirFlow);
}

bool HeatExchangerAirToAirSensibleAndLatent::setLatentEffectivenessat75CoolingAirFlow(double latentEffectivenessat75CoolingAirFlow) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setLatentEffectivenessat75CoolingAirFlow(latentEffectivenessat75CoolingAirFlow);
}

bool HeatExchangerAirToAirSensibleAndLatent::setLatentEffectivenessat75CoolingAirFlow(const Quantity& latentEffectivenessat75CoolingAirFlow) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setLatentEffectivenessat75CoolingAirFlow(latentEffectivenessat75CoolingAirFlow);
}

bool HeatExchangerAirToAirSensibleAndLatent::setNominalElectricPower(double nominalElectricPower) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setNominalElectricPower(nominalElectricPower);
}

bool HeatExchangerAirToAirSensibleAndLatent::setNominalElectricPower(const Quantity& nominalElectricPower) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setNominalElectricPower(nominalElectricPower);
}

void HeatExchangerAirToAirSensibleAndLatent::setSupplyAirOutletTemperatureControl(bool supplyAirOutletTemperatureControl) {
  getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setSupplyAirOutletTemperatureControl(supplyAirOutletTemperatureControl);
}

bool HeatExchangerAirToAirSensibleAndLatent::setHeatExchangerType(std::string heatExchangerType) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setHeatExchangerType(heatExchangerType);
}

bool HeatExchangerAirToAirSensibleAndLatent::setFrostControlType(std::string frostControlType) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setFrostControlType(frostControlType);
}

void HeatExchangerAirToAirSensibleAndLatent::setThresholdTemperature(double thresholdTemperature) {
  getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setThresholdTemperature(thresholdTemperature);
}

bool HeatExchangerAirToAirSensibleAndLatent::setThresholdTemperature(const Quantity& thresholdTemperature) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setThresholdTemperature(thresholdTemperature);
}

void HeatExchangerAirToAirSensibleAndLatent::resetThresholdTemperature() {
  getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->resetThresholdTemperature();
}

bool HeatExchangerAirToAirSensibleAndLatent::setInitialDefrostTimeFraction(double initialDefrostTimeFraction) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setInitialDefrostTimeFraction(initialDefrostTimeFraction);
}

bool HeatExchangerAirToAirSensibleAndLatent::setInitialDefrostTimeFraction(const Quantity& initialDefrostTimeFraction) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setInitialDefrostTimeFraction(initialDefrostTimeFraction);
}

void HeatExchangerAirToAirSensibleAndLatent::resetInitialDefrostTimeFraction() {
  getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->resetInitialDefrostTimeFraction();
}

bool HeatExchangerAirToAirSensibleAndLatent::setRateofDefrostTimeFractionIncrease(double rateofDefrostTimeFractionIncrease) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setRateofDefrostTimeFractionIncrease(rateofDefrostTimeFractionIncrease);
}

bool HeatExchangerAirToAirSensibleAndLatent::setRateofDefrostTimeFractionIncrease(const Quantity& rateofDefrostTimeFractionIncrease) {
  return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setRateofDefrostTimeFractionIncrease(rateofDefrostTimeFractionIncrease);
}

void HeatExchangerAirToAirSensibleAndLatent::resetRateofDefrostTimeFractionIncrease() {
  getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->resetRateofDefrostTimeFractionIncrease();
}

void HeatExchangerAirToAirSensibleAndLatent::setEconomizerLockout(bool economizerLockout) {
  getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setEconomizerLockout(economizerLockout);
}

/// @cond
HeatExchangerAirToAirSensibleAndLatent::HeatExchangerAirToAirSensibleAndLatent(std::shared_ptr<detail::HeatExchangerAirToAirSensibleAndLatent_Impl> impl)
  : AirToAirComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

