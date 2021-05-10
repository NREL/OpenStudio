/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "HeatExchangerDesiccantBalancedFlow.hpp"
#include "HeatExchangerDesiccantBalancedFlow_Impl.hpp"
#include "CoilSystemCoolingWaterHeatExchangerAssisted.hpp"
#include "CoilSystemCoolingWaterHeatExchangerAssisted_Impl.hpp"
#include "CoilSystemCoolingDXHeatExchangerAssisted.hpp"
#include "CoilSystemCoolingDXHeatExchangerAssisted_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "AirflowNetworkEquivalentDuct.hpp"
#include "AirflowNetworkEquivalentDuct_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_HeatExchanger_AirToAir_SensibleAndLatent_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    HeatExchangerDesiccantBalancedFlow_Impl::HeatExchangerDesiccantBalancedFlow_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                             bool keepHandle)
      : AirToAirComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == HeatExchangerDesiccantBalancedFlow::iddObjectType());
    }

    HeatExchangerDesiccantBalancedFlow_Impl::HeatExchangerDesiccantBalancedFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : AirToAirComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == HeatExchangerDesiccantBalancedFlow::iddObjectType());
    }

    HeatExchangerDesiccantBalancedFlow_Impl::HeatExchangerDesiccantBalancedFlow_Impl(const HeatExchangerDesiccantBalancedFlow_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : AirToAirComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& HeatExchangerDesiccantBalancedFlow_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Heat Exchanger Sensible Heating Rate",
                                                   "Heat Exchanger Sensible Heating Energy",
                                                   "Heat Exchanger Latent Gain Rate",
                                                   "Heat Exchanger Latent Gain Energy",
                                                   "Heat Exchanger Total Heating Rate",
                                                   "Heat Exchanger Total Heating Energy",
                                                   "Heat Exchanger Sensible Cooling Rate",
                                                   "Heat Exchanger Sensible Cooling Energy",
                                                   "Heat Exchanger Latent Cooling Rate",
                                                   "Heat Exchanger Latent Cooling Energy",
                                                   "Heat Exchanger Total Cooling Rate",
                                                   "Heat Exchanger Total Cooling Energy",
                                                   "Heat Exchanger Electricity Rate",
                                                   "Heat Exchanger Electricity Energy",
                                                   "Heat Exchanger Sensible Effectiveness",
                                                   "Heat Exchanger Latent Effectiveness",
                                                   "Heat Exchanger Supply Air Bypass Mass Flow Rate",
                                                   "Heat Exchanger Exhaust Air Bypass Mass Flow Rate",
                                                   "Heat Exchanger Defrost Time Fraction"};
      return result;
    }

    IddObjectType HeatExchangerDesiccantBalancedFlow_Impl::iddObjectType() const {
      return HeatExchangerDesiccantBalancedFlow::iddObjectType();
    }

    std::vector<ModelObject> HeatExchangerDesiccantBalancedFlow_Impl::children() const {
      std::vector<ModelObject> result;
      std::vector<AirflowNetworkEquivalentDuct> myAFNItems =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkEquivalentDuct>(AirflowNetworkEquivalentDuct::iddObjectType());
      result.insert(result.end(), myAFNItems.begin(), myAFNItems.end());
      return result;
    }

    std::vector<ScheduleTypeKey> HeatExchangerDesiccantBalancedFlow_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_HeatExchanger_AirToAir_SensibleAndLatentFields::AvailabilitySchedule) != e) {
        result.push_back(ScheduleTypeKey("HeatExchangerDesiccantBalancedFlow", "Availability"));
      }
      return result;
    }

    Schedule HeatExchangerDesiccantBalancedFlow_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
      }
      return value.get();
    }

    boost::optional<HVACComponent> HeatExchangerDesiccantBalancedFlow_Impl::containingHVACComponent() const {
      // CoilSystemCoolingWaterHeatExchangerAssisted
      {
        auto coilSystems = model().getConcreteModelObjects<CoilSystemCoolingWaterHeatExchangerAssisted>();
        for (const auto& coilSystem : coilSystems) {
          if (coilSystem.heatExchanger().handle() == handle()) {
            return coilSystem;
          }
        }
      }

      // CoilSystemCoolingDXHeatExchangerAssisted
      {
        auto coilSystems = model().getConcreteModelObjects<CoilSystemCoolingDXHeatExchangerAssisted>();
        for (const auto& coilSystem : coilSystems) {
          if (coilSystem.heatExchanger().handle() == handle()) {
            return coilSystem;
          }
        }
      }

      return boost::none;
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::addToNode(Node& node) {
      bool success(false);

      auto t_containingHVACComponent = containingHVACComponent();

      if (t_containingHVACComponent) {
        if (t_containingHVACComponent->optionalCast<CoilSystemCoolingWaterHeatExchangerAssisted>()) {
          LOG(Warn,
              this->briefDescription() << " cannot be connected directly when it's part of a parent CoilSystemCoolingWaterHeatExchangerAssisted. "
                                          "Please call CoilSystemCoolingWaterHeatExchangerAssisted::addToNode instead");
        } else if (t_containingHVACComponent->optionalCast<CoilSystemCoolingDXHeatExchangerAssisted>()) {
          LOG(Warn, this->briefDescription() << " cannot be connected directly when it's part of a parent CoilSystemCoolingDXHeatExchangerAssisted. "
                                                "Please call CoilSystemCoolingDXHeatExchangerAssisted::addToNode instead");
        } else {
          // Shouldn't happen
          OS_ASSERT(false);
        }
      } else {
        success = AirToAirComponent_Impl::addToNode(node);
      }

      return success;
    }

    boost::optional<double> HeatExchangerDesiccantBalancedFlow_Impl::nominalSupplyAirFlowRate() const {
      return getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::NominalSupplyAirFlowRate, true);
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::isNominalSupplyAirFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::NominalSupplyAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double HeatExchangerDesiccantBalancedFlow_Impl::sensibleEffectivenessat100HeatingAirFlow() const {
      boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat100_HeatingAirFlow, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlow_Impl::latentEffectivenessat100HeatingAirFlow() const {
      boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat100_HeatingAirFlow, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlow_Impl::sensibleEffectivenessat75HeatingAirFlow() const {
      boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat75_HeatingAirFlow, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlow_Impl::latentEffectivenessat75HeatingAirFlow() const {
      boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat75_HeatingAirFlow, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlow_Impl::sensibleEffectivenessat100CoolingAirFlow() const {
      boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat100_CoolingAirFlow, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlow_Impl::latentEffectivenessat100CoolingAirFlow() const {
      boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat100_CoolingAirFlow, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlow_Impl::sensibleEffectivenessat75CoolingAirFlow() const {
      boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat75_CoolingAirFlow, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlow_Impl::latentEffectivenessat75CoolingAirFlow() const {
      boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat75_CoolingAirFlow, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlow_Impl::nominalElectricPower() const {
      boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::NominalElectricPower, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::supplyAirOutletTemperatureControl() const {
      boost::optional<std::string> value = getString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirOutletTemperatureControl, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    std::string HeatExchangerDesiccantBalancedFlow_Impl::heatExchangerType() const {
      boost::optional<std::string> value = getString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::HeatExchangerType, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string HeatExchangerDesiccantBalancedFlow_Impl::frostControlType() const {
      boost::optional<std::string> value = getString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::FrostControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerDesiccantBalancedFlow_Impl::thresholdTemperature() const {
      boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::ThresholdTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::isThresholdTemperatureDefaulted() const {
      return isEmpty(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::ThresholdTemperature);
    }

    boost::optional<double> HeatExchangerDesiccantBalancedFlow_Impl::initialDefrostTimeFraction() const {
      return getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::InitialDefrostTimeFraction, true);
    }

    boost::optional<double> HeatExchangerDesiccantBalancedFlow_Impl::rateofDefrostTimeFractionIncrease() const {
      return getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::RateofDefrostTimeFractionIncrease, true);
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::economizerLockout() const {
      boost::optional<std::string> value = getString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::EconomizerLockout, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::AvailabilitySchedule, "HeatExchangerDesiccantBalancedFlow",
                                "Availability", schedule);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::setNominalSupplyAirFlowRate(boost::optional<double> nominalSupplyAirFlowRate) {
      bool result(false);
      if (nominalSupplyAirFlowRate) {
        result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::NominalSupplyAirFlowRate, nominalSupplyAirFlowRate.get());
      }
      return result;
    }

    void HeatExchangerDesiccantBalancedFlow_Impl::autosizeNominalSupplyAirFlowRate() {
      bool result = setString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::NominalSupplyAirFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::setSensibleEffectivenessat100HeatingAirFlow(double sensibleEffectivenessat100HeatingAirFlow) {
      bool result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat100_HeatingAirFlow,
                              sensibleEffectivenessat100HeatingAirFlow);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::setLatentEffectivenessat100HeatingAirFlow(double latentEffectivenessat100HeatingAirFlow) {
      bool result =
        setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat100_HeatingAirFlow, latentEffectivenessat100HeatingAirFlow);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::setSensibleEffectivenessat75HeatingAirFlow(double sensibleEffectivenessat75HeatingAirFlow) {
      bool result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat75_HeatingAirFlow,
                              sensibleEffectivenessat75HeatingAirFlow);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::setLatentEffectivenessat75HeatingAirFlow(double latentEffectivenessat75HeatingAirFlow) {
      bool result =
        setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat75_HeatingAirFlow, latentEffectivenessat75HeatingAirFlow);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::setSensibleEffectivenessat100CoolingAirFlow(double sensibleEffectivenessat100CoolingAirFlow) {
      bool result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat100_CoolingAirFlow,
                              sensibleEffectivenessat100CoolingAirFlow);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::setLatentEffectivenessat100CoolingAirFlow(double latentEffectivenessat100CoolingAirFlow) {
      bool result =
        setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat100_CoolingAirFlow, latentEffectivenessat100CoolingAirFlow);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::setSensibleEffectivenessat75CoolingAirFlow(double sensibleEffectivenessat75CoolingAirFlow) {
      bool result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat75_CoolingAirFlow,
                              sensibleEffectivenessat75CoolingAirFlow);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::setLatentEffectivenessat75CoolingAirFlow(double latentEffectivenessat75CoolingAirFlow) {
      bool result =
        setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat75_CoolingAirFlow, latentEffectivenessat75CoolingAirFlow);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::setNominalElectricPower(double nominalElectricPower) {
      bool result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::NominalElectricPower, nominalElectricPower);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::setSupplyAirOutletTemperatureControl(bool supplyAirOutletTemperatureControl) {
      return setBooleanFieldValue(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirOutletTemperatureControl,
                                  supplyAirOutletTemperatureControl);
      ;
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::setHeatExchangerType(std::string heatExchangerType) {
      bool result = setString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::HeatExchangerType, heatExchangerType);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::setFrostControlType(std::string frostControlType) {
      bool result = setString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::FrostControlType, frostControlType);
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::setThresholdTemperature(double thresholdTemperature) {
      bool result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::ThresholdTemperature, thresholdTemperature);
      OS_ASSERT(result);
      return result;
    }

    void HeatExchangerDesiccantBalancedFlow_Impl::resetThresholdTemperature() {
      bool result = setString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::ThresholdTemperature, "");
      OS_ASSERT(result);
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::setInitialDefrostTimeFraction(boost::optional<double> initialDefrostTimeFraction) {
      bool result(false);
      if (initialDefrostTimeFraction) {
        result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::InitialDefrostTimeFraction, initialDefrostTimeFraction.get());
      } else {
        resetInitialDefrostTimeFraction();
        result = true;
      }
      return result;
    }

    void HeatExchangerDesiccantBalancedFlow_Impl::resetInitialDefrostTimeFraction() {
      bool result = setString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::InitialDefrostTimeFraction, "");
      OS_ASSERT(result);
    }

    bool
      HeatExchangerDesiccantBalancedFlow_Impl::setRateofDefrostTimeFractionIncrease(boost::optional<double> rateofDefrostTimeFractionIncrease) {
      bool result(false);
      if (rateofDefrostTimeFractionIncrease) {
        result =
          setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::RateofDefrostTimeFractionIncrease, rateofDefrostTimeFractionIncrease.get());
      } else {
        resetRateofDefrostTimeFractionIncrease();
        result = true;
      }
      return result;
    }

    void HeatExchangerDesiccantBalancedFlow_Impl::resetRateofDefrostTimeFractionIncrease() {
      bool result = setString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::RateofDefrostTimeFractionIncrease, "");
      OS_ASSERT(result);
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::setEconomizerLockout(bool economizerLockout) {
      return setBooleanFieldValue(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::EconomizerLockout, economizerLockout);
      ;
    }

    boost::optional<Schedule> HeatExchangerDesiccantBalancedFlow_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::AvailabilitySchedule);
    }

    std::vector<std::string> HeatExchangerDesiccantBalancedFlow_Impl::heatExchangerTypeValues() const {
      return HeatExchangerDesiccantBalancedFlow::heatExchangerTypeValues();
    }

    std::vector<std::string> HeatExchangerDesiccantBalancedFlow_Impl::frostControlTypeValues() const {
      return HeatExchangerDesiccantBalancedFlow::frostControlTypeValues();
    }

    boost::optional<ModelObject> HeatExchangerDesiccantBalancedFlow_Impl::availabilityScheduleAsModelObject() const {
      OptionalModelObject result = availabilitySchedule();
      return result;
    }

    bool HeatExchangerDesiccantBalancedFlow_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setAvailabilitySchedule(schedule);
        }
      }
      return false;
    }

    unsigned HeatExchangerDesiccantBalancedFlow_Impl::primaryAirInletPort() const {
      return OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirInletNode;
    }

    unsigned HeatExchangerDesiccantBalancedFlow_Impl::primaryAirOutletPort() const {
      return OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirOutletNode;
    }

    unsigned HeatExchangerDesiccantBalancedFlow_Impl::secondaryAirInletPort() const {
      return OS_HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirInletNode;
    }

    unsigned HeatExchangerDesiccantBalancedFlow_Impl::secondaryAirOutletPort() const {
      return OS_HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirOutletNode;
    }

    AirflowNetworkEquivalentDuct HeatExchangerDesiccantBalancedFlow_Impl::getAirflowNetworkEquivalentDuct(double length, double diameter) {
      boost::optional<AirflowNetworkEquivalentDuct> opt = airflowNetworkEquivalentDuct();
      if (opt) {
        if (opt->airPathLength() != length) {
          opt->setAirPathLength(length);
        }
        if (opt->airPathHydraulicDiameter() != diameter) {
          opt->setAirPathHydraulicDiameter(diameter);
        }
      }
      return AirflowNetworkEquivalentDuct(model(), length, diameter, handle());
    }

    boost::optional<AirflowNetworkEquivalentDuct> HeatExchangerDesiccantBalancedFlow_Impl::airflowNetworkEquivalentDuct() const {
      std::vector<AirflowNetworkEquivalentDuct> myAFN =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkEquivalentDuct>(AirflowNetworkEquivalentDuct::iddObjectType());
      auto count = myAFN.size();
      if (count == 1) {
        return myAFN[0];
      } else if (count > 1) {
        LOG(Warn, briefDescription() << " has more than one AirflowNetwork EquivalentDuct attached, returning first.");
        return myAFN[0];
      }
      return boost::none;
    }

    boost::optional<double> HeatExchangerDesiccantBalancedFlow_Impl::autosizedNominalSupplyAirFlowRate() const {
      return getAutosizedValue("Design Size Nominal Supply Air Flow Rate", "m3/s");
    }

    void HeatExchangerDesiccantBalancedFlow_Impl::autosize() {
      autosizeNominalSupplyAirFlowRate();
    }

    void HeatExchangerDesiccantBalancedFlow_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedNominalSupplyAirFlowRate();
      if (val) {
        setNominalSupplyAirFlowRate(val.get());
      }
    }

  }  // namespace detail

  HeatExchangerDesiccantBalancedFlow::HeatExchangerDesiccantBalancedFlow(const Model& model)
    : AirToAirComponent(HeatExchangerDesiccantBalancedFlow::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>());

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

  IddObjectType HeatExchangerDesiccantBalancedFlow::iddObjectType() {
    return IddObjectType(IddObjectType::OS_HeatExchanger_AirToAir_SensibleAndLatent);
  }

  std::vector<std::string> HeatExchangerDesiccantBalancedFlow::heatExchangerTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_HeatExchanger_AirToAir_SensibleAndLatentFields::HeatExchangerType);
  }

  std::vector<std::string> HeatExchangerDesiccantBalancedFlow::frostControlTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_HeatExchanger_AirToAir_SensibleAndLatentFields::FrostControlType);
  }

  Schedule HeatExchangerDesiccantBalancedFlow::availabilitySchedule() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->availabilitySchedule();
  }

  boost::optional<double> HeatExchangerDesiccantBalancedFlow::nominalSupplyAirFlowRate() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->nominalSupplyAirFlowRate();
  }

  bool HeatExchangerDesiccantBalancedFlow::isNominalSupplyAirFlowRateAutosized() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->isNominalSupplyAirFlowRateAutosized();
  }

  double HeatExchangerDesiccantBalancedFlow::sensibleEffectivenessat100HeatingAirFlow() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->sensibleEffectivenessat100HeatingAirFlow();
  }

  double HeatExchangerDesiccantBalancedFlow::latentEffectivenessat100HeatingAirFlow() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->latentEffectivenessat100HeatingAirFlow();
  }

  double HeatExchangerDesiccantBalancedFlow::sensibleEffectivenessat75HeatingAirFlow() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->sensibleEffectivenessat75HeatingAirFlow();
  }

  double HeatExchangerDesiccantBalancedFlow::latentEffectivenessat75HeatingAirFlow() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->latentEffectivenessat75HeatingAirFlow();
  }

  double HeatExchangerDesiccantBalancedFlow::sensibleEffectivenessat100CoolingAirFlow() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->sensibleEffectivenessat100CoolingAirFlow();
  }

  double HeatExchangerDesiccantBalancedFlow::latentEffectivenessat100CoolingAirFlow() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->latentEffectivenessat100CoolingAirFlow();
  }

  double HeatExchangerDesiccantBalancedFlow::sensibleEffectivenessat75CoolingAirFlow() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->sensibleEffectivenessat75CoolingAirFlow();
  }

  double HeatExchangerDesiccantBalancedFlow::latentEffectivenessat75CoolingAirFlow() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->latentEffectivenessat75CoolingAirFlow();
  }

  double HeatExchangerDesiccantBalancedFlow::nominalElectricPower() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->nominalElectricPower();
  }

  bool HeatExchangerDesiccantBalancedFlow::supplyAirOutletTemperatureControl() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->supplyAirOutletTemperatureControl();
  }

  std::string HeatExchangerDesiccantBalancedFlow::heatExchangerType() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->heatExchangerType();
  }

  std::string HeatExchangerDesiccantBalancedFlow::frostControlType() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->frostControlType();
  }

  double HeatExchangerDesiccantBalancedFlow::thresholdTemperature() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->thresholdTemperature();
  }

  bool HeatExchangerDesiccantBalancedFlow::isThresholdTemperatureDefaulted() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->isThresholdTemperatureDefaulted();
  }

  boost::optional<double> HeatExchangerDesiccantBalancedFlow::initialDefrostTimeFraction() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->initialDefrostTimeFraction();
  }

  boost::optional<double> HeatExchangerDesiccantBalancedFlow::rateofDefrostTimeFractionIncrease() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->rateofDefrostTimeFractionIncrease();
  }

  bool HeatExchangerDesiccantBalancedFlow::economizerLockout() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->economizerLockout();
  }

  bool HeatExchangerDesiccantBalancedFlow::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool HeatExchangerDesiccantBalancedFlow::setNominalSupplyAirFlowRate(double nominalSupplyAirFlowRate) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->setNominalSupplyAirFlowRate(nominalSupplyAirFlowRate);
  }

  void HeatExchangerDesiccantBalancedFlow::autosizeNominalSupplyAirFlowRate() {
    getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->autosizeNominalSupplyAirFlowRate();
  }

  bool HeatExchangerDesiccantBalancedFlow::setSensibleEffectivenessat100HeatingAirFlow(double sensibleEffectivenessat100HeatingAirFlow) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->setSensibleEffectivenessat100HeatingAirFlow(
      sensibleEffectivenessat100HeatingAirFlow);
  }

  bool HeatExchangerDesiccantBalancedFlow::setLatentEffectivenessat100HeatingAirFlow(double latentEffectivenessat100HeatingAirFlow) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->setLatentEffectivenessat100HeatingAirFlow(
      latentEffectivenessat100HeatingAirFlow);
  }

  bool HeatExchangerDesiccantBalancedFlow::setSensibleEffectivenessat75HeatingAirFlow(double sensibleEffectivenessat75HeatingAirFlow) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->setSensibleEffectivenessat75HeatingAirFlow(
      sensibleEffectivenessat75HeatingAirFlow);
  }

  bool HeatExchangerDesiccantBalancedFlow::setLatentEffectivenessat75HeatingAirFlow(double latentEffectivenessat75HeatingAirFlow) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->setLatentEffectivenessat75HeatingAirFlow(
      latentEffectivenessat75HeatingAirFlow);
  }

  bool HeatExchangerDesiccantBalancedFlow::setSensibleEffectivenessat100CoolingAirFlow(double sensibleEffectivenessat100CoolingAirFlow) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->setSensibleEffectivenessat100CoolingAirFlow(
      sensibleEffectivenessat100CoolingAirFlow);
  }

  bool HeatExchangerDesiccantBalancedFlow::setLatentEffectivenessat100CoolingAirFlow(double latentEffectivenessat100CoolingAirFlow) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->setLatentEffectivenessat100CoolingAirFlow(
      latentEffectivenessat100CoolingAirFlow);
  }

  bool HeatExchangerDesiccantBalancedFlow::setSensibleEffectivenessat75CoolingAirFlow(double sensibleEffectivenessat75CoolingAirFlow) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->setSensibleEffectivenessat75CoolingAirFlow(
      sensibleEffectivenessat75CoolingAirFlow);
  }

  bool HeatExchangerDesiccantBalancedFlow::setLatentEffectivenessat75CoolingAirFlow(double latentEffectivenessat75CoolingAirFlow) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->setLatentEffectivenessat75CoolingAirFlow(
      latentEffectivenessat75CoolingAirFlow);
  }

  bool HeatExchangerDesiccantBalancedFlow::setNominalElectricPower(double nominalElectricPower) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->setNominalElectricPower(nominalElectricPower);
  }

  bool HeatExchangerDesiccantBalancedFlow::setSupplyAirOutletTemperatureControl(bool supplyAirOutletTemperatureControl) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->setSupplyAirOutletTemperatureControl(supplyAirOutletTemperatureControl);
  }

  bool HeatExchangerDesiccantBalancedFlow::setHeatExchangerType(std::string heatExchangerType) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->setHeatExchangerType(heatExchangerType);
  }

  bool HeatExchangerDesiccantBalancedFlow::setFrostControlType(std::string frostControlType) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->setFrostControlType(frostControlType);
  }

  bool HeatExchangerDesiccantBalancedFlow::setThresholdTemperature(double thresholdTemperature) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->setThresholdTemperature(thresholdTemperature);
  }

  void HeatExchangerDesiccantBalancedFlow::resetThresholdTemperature() {
    getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->resetThresholdTemperature();
  }

  bool HeatExchangerDesiccantBalancedFlow::setInitialDefrostTimeFraction(double initialDefrostTimeFraction) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->setInitialDefrostTimeFraction(initialDefrostTimeFraction);
  }

  void HeatExchangerDesiccantBalancedFlow::resetInitialDefrostTimeFraction() {
    getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->resetInitialDefrostTimeFraction();
  }

  bool HeatExchangerDesiccantBalancedFlow::setRateofDefrostTimeFractionIncrease(double rateofDefrostTimeFractionIncrease) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->setRateofDefrostTimeFractionIncrease(rateofDefrostTimeFractionIncrease);
  }

  void HeatExchangerDesiccantBalancedFlow::resetRateofDefrostTimeFractionIncrease() {
    getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->resetRateofDefrostTimeFractionIncrease();
  }

  bool HeatExchangerDesiccantBalancedFlow::setEconomizerLockout(bool economizerLockout) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->setEconomizerLockout(economizerLockout);
  }

  AirflowNetworkEquivalentDuct HeatExchangerDesiccantBalancedFlow::getAirflowNetworkEquivalentDuct(double length, double diameter) {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->getAirflowNetworkEquivalentDuct(length, diameter);
  }

  boost::optional<AirflowNetworkEquivalentDuct> HeatExchangerDesiccantBalancedFlow::airflowNetworkEquivalentDuct() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->airflowNetworkEquivalentDuct();
  }

  /// @cond
  HeatExchangerDesiccantBalancedFlow::HeatExchangerDesiccantBalancedFlow(
    std::shared_ptr<detail::HeatExchangerDesiccantBalancedFlow_Impl> impl)
    : AirToAirComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> HeatExchangerDesiccantBalancedFlow::autosizedNominalSupplyAirFlowRate() const {
    return getImpl<detail::HeatExchangerDesiccantBalancedFlow_Impl>()->autosizedNominalSupplyAirFlowRate();
  }

}  // namespace model
}  // namespace openstudio