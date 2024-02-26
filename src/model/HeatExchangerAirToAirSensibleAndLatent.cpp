/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"
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

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_HeatExchanger_AirToAir_SensibleAndLatent_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

// Remove when deprecated methods are removed
#include "../utilities/core/DeprecatedHelpers.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    HeatExchangerAirToAirSensibleAndLatent_Impl::HeatExchangerAirToAirSensibleAndLatent_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                             bool keepHandle)
      : AirToAirComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == HeatExchangerAirToAirSensibleAndLatent::iddObjectType());
    }

    HeatExchangerAirToAirSensibleAndLatent_Impl::HeatExchangerAirToAirSensibleAndLatent_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : AirToAirComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == HeatExchangerAirToAirSensibleAndLatent::iddObjectType());
    }

    HeatExchangerAirToAirSensibleAndLatent_Impl::HeatExchangerAirToAirSensibleAndLatent_Impl(const HeatExchangerAirToAirSensibleAndLatent_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : AirToAirComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& HeatExchangerAirToAirSensibleAndLatent_Impl::outputVariableNames() const {
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

    IddObjectType HeatExchangerAirToAirSensibleAndLatent_Impl::iddObjectType() const {
      return HeatExchangerAirToAirSensibleAndLatent::iddObjectType();
    }

    std::vector<ModelObject> HeatExchangerAirToAirSensibleAndLatent_Impl::children() const {
      std::vector<ModelObject> result;
      std::vector<AirflowNetworkEquivalentDuct> myAFNItems =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkEquivalentDuct>(AirflowNetworkEquivalentDuct::iddObjectType());
      result.insert(result.end(), myAFNItems.begin(), myAFNItems.end());
      return result;
    }

    std::vector<ScheduleTypeKey> HeatExchangerAirToAirSensibleAndLatent_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_HeatExchanger_AirToAir_SensibleAndLatentFields::AvailabilitySchedule) != e) {
        result.push_back(ScheduleTypeKey("HeatExchangerAirToAirSensibleAndLatent", "Availability"));
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

    boost::optional<HVACComponent> HeatExchangerAirToAirSensibleAndLatent_Impl::containingHVACComponent() const {
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

    bool HeatExchangerAirToAirSensibleAndLatent_Impl::addToNode(Node& node) {
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

    boost::optional<double> HeatExchangerAirToAirSensibleAndLatent_Impl::nominalSupplyAirFlowRate() const {
      return getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::NominalSupplyAirFlowRate, true);
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
      boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat100_HeatingAirFlow, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerAirToAirSensibleAndLatent_Impl::latentEffectivenessat100HeatingAirFlow() const {
      boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat100_HeatingAirFlow, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerAirToAirSensibleAndLatent_Impl::sensibleEffectivenessat100CoolingAirFlow() const {
      boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat100_CoolingAirFlow, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerAirToAirSensibleAndLatent_Impl::latentEffectivenessat100CoolingAirFlow() const {
      boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat100_CoolingAirFlow, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerAirToAirSensibleAndLatent_Impl::nominalElectricPower() const {
      boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::NominalElectricPower, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool HeatExchangerAirToAirSensibleAndLatent_Impl::supplyAirOutletTemperatureControl() const {
      boost::optional<std::string> value = getString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirOutletTemperatureControl, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    std::string HeatExchangerAirToAirSensibleAndLatent_Impl::heatExchangerType() const {
      boost::optional<std::string> value = getString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::HeatExchangerType, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string HeatExchangerAirToAirSensibleAndLatent_Impl::frostControlType() const {
      boost::optional<std::string> value = getString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::FrostControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    double HeatExchangerAirToAirSensibleAndLatent_Impl::thresholdTemperature() const {
      boost::optional<double> value = getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::ThresholdTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool HeatExchangerAirToAirSensibleAndLatent_Impl::isThresholdTemperatureDefaulted() const {
      return isEmpty(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::ThresholdTemperature);
    }

    boost::optional<double> HeatExchangerAirToAirSensibleAndLatent_Impl::initialDefrostTimeFraction() const {
      return getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::InitialDefrostTimeFraction, true);
    }

    boost::optional<double> HeatExchangerAirToAirSensibleAndLatent_Impl::rateofDefrostTimeFractionIncrease() const {
      return getDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::RateofDefrostTimeFractionIncrease, true);
    }

    bool HeatExchangerAirToAirSensibleAndLatent_Impl::economizerLockout() const {
      boost::optional<std::string> value = getString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::EconomizerLockout, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    boost::optional<Curve> HeatExchangerAirToAirSensibleAndLatent_Impl::sensibleEffectivenessofHeatingAirFlowCurve() const {
      boost::optional<Curve> value = optionalSensibleEffectivenessofHeatingAirFlowCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a Sensible Effectiveness of Heating Air Flow Curve attached.");
      }
      return value.get();
    }

    boost::optional<Curve> HeatExchangerAirToAirSensibleAndLatent_Impl::optionalSensibleEffectivenessofHeatingAirFlowCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessofHeatingAirFlowCurveName);
    }

    boost::optional<Curve> HeatExchangerAirToAirSensibleAndLatent_Impl::latentEffectivenessofHeatingAirFlowCurve() const {
      boost::optional<Curve> value = optionalLatentEffectivenessofHeatingAirFlowCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a Latent Effectiveness of Heating Air Flow Curve attached.");
      }
      return value.get();
    }

    boost::optional<Curve> HeatExchangerAirToAirSensibleAndLatent_Impl::optionalLatentEffectivenessofHeatingAirFlowCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessofHeatingAirFlowCurve);
    }

    boost::optional<Curve> HeatExchangerAirToAirSensibleAndLatent_Impl::sensibleEffectivenessofCoolingAirFlowCurve() const {
      boost::optional<Curve> value = optionalSensibleEffectivenessofCoolingAirFlowCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a Sensible Effectiveness of Cooling Air Flow Curve attached.");
      }
      return value.get();
    }

    boost::optional<Curve> HeatExchangerAirToAirSensibleAndLatent_Impl::optionalSensibleEffectivenessofCoolingAirFlowCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessofCoolingAirFlowCurve);
    }

    boost::optional<Curve> HeatExchangerAirToAirSensibleAndLatent_Impl::latentEffectivenessofCoolingAirFlowCurve() const {
      boost::optional<Curve> value = optionalLatentEffectivenessofCoolingAirFlowCurve();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a Latent Effectiveness of Cooling Air Flow Curve attached.");
      }
      return value.get();
    }

    boost::optional<Curve> HeatExchangerAirToAirSensibleAndLatent_Impl::optionalLatentEffectivenessofCoolingAirFlowCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessofCoolingAirFlowCurve);
    }

    bool HeatExchangerAirToAirSensibleAndLatent_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::AvailabilitySchedule, "HeatExchangerAirToAirSensibleAndLatent",
                                "Availability", schedule);
      return result;
    }

    bool HeatExchangerAirToAirSensibleAndLatent_Impl::setNominalSupplyAirFlowRate(boost::optional<double> nominalSupplyAirFlowRate) {
      bool result(false);
      if (nominalSupplyAirFlowRate) {
        result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::NominalSupplyAirFlowRate, nominalSupplyAirFlowRate.get());
      }
      return result;
    }

    void HeatExchangerAirToAirSensibleAndLatent_Impl::autosizeNominalSupplyAirFlowRate() {
      bool result = setString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::NominalSupplyAirFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool HeatExchangerAirToAirSensibleAndLatent_Impl::setSensibleEffectivenessat100HeatingAirFlow(double sensibleEffectivenessat100HeatingAirFlow) {
      bool result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat100_HeatingAirFlow,
                              sensibleEffectivenessat100HeatingAirFlow);
      return result;
    }

    bool HeatExchangerAirToAirSensibleAndLatent_Impl::setLatentEffectivenessat100HeatingAirFlow(double latentEffectivenessat100HeatingAirFlow) {
      bool result =
        setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat100_HeatingAirFlow, latentEffectivenessat100HeatingAirFlow);
      return result;
    }

    bool HeatExchangerAirToAirSensibleAndLatent_Impl::setSensibleEffectivenessat100CoolingAirFlow(double sensibleEffectivenessat100CoolingAirFlow) {
      bool result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SensibleEffectivenessat100_CoolingAirFlow,
                              sensibleEffectivenessat100CoolingAirFlow);
      return result;
    }

    bool HeatExchangerAirToAirSensibleAndLatent_Impl::setLatentEffectivenessat100CoolingAirFlow(double latentEffectivenessat100CoolingAirFlow) {
      bool result =
        setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::LatentEffectivenessat100_CoolingAirFlow, latentEffectivenessat100CoolingAirFlow);
      return result;
    }

    bool HeatExchangerAirToAirSensibleAndLatent_Impl::setNominalElectricPower(double nominalElectricPower) {
      bool result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::NominalElectricPower, nominalElectricPower);
      return result;
    }

    bool HeatExchangerAirToAirSensibleAndLatent_Impl::setSupplyAirOutletTemperatureControl(bool supplyAirOutletTemperatureControl) {
      return setBooleanFieldValue(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirOutletTemperatureControl,
                                  supplyAirOutletTemperatureControl);
    }

    bool HeatExchangerAirToAirSensibleAndLatent_Impl::setHeatExchangerType(const std::string& heatExchangerType) {
      bool result = setString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::HeatExchangerType, heatExchangerType);
      return result;
    }

    bool HeatExchangerAirToAirSensibleAndLatent_Impl::setFrostControlType(const std::string& frostControlType) {
      bool result = setString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::FrostControlType, frostControlType);
      return result;
    }

    bool HeatExchangerAirToAirSensibleAndLatent_Impl::setThresholdTemperature(double thresholdTemperature) {
      bool result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::ThresholdTemperature, thresholdTemperature);
      OS_ASSERT(result);
      return result;
    }

    void HeatExchangerAirToAirSensibleAndLatent_Impl::resetThresholdTemperature() {
      bool result = setString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::ThresholdTemperature, "");
      OS_ASSERT(result);
    }

    bool HeatExchangerAirToAirSensibleAndLatent_Impl::setInitialDefrostTimeFraction(boost::optional<double> initialDefrostTimeFraction) {
      bool result(false);
      if (initialDefrostTimeFraction) {
        result = setDouble(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::InitialDefrostTimeFraction, initialDefrostTimeFraction.get());
      } else {
        resetInitialDefrostTimeFraction();
        result = true;
      }
      return result;
    }

    void HeatExchangerAirToAirSensibleAndLatent_Impl::resetInitialDefrostTimeFraction() {
      bool result = setString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::InitialDefrostTimeFraction, "");
      OS_ASSERT(result);
    }

    bool
      HeatExchangerAirToAirSensibleAndLatent_Impl::setRateofDefrostTimeFractionIncrease(boost::optional<double> rateofDefrostTimeFractionIncrease) {
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

    void HeatExchangerAirToAirSensibleAndLatent_Impl::resetRateofDefrostTimeFractionIncrease() {
      bool result = setString(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::RateofDefrostTimeFractionIncrease, "");
      OS_ASSERT(result);
    }

    bool HeatExchangerAirToAirSensibleAndLatent_Impl::setEconomizerLockout(bool economizerLockout) {
      return setBooleanFieldValue(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::EconomizerLockout, economizerLockout);
    }

    bool HeatExchangerAirToAirSensibleAndLatent_Impl::setSensibleEffectivenessofHeatingAirFlowCurve(
      const Curve& sensibleEffectivenessofHeatingAirFlowCurve) {}

    bool HeatExchangerAirToAirSensibleAndLatent_Impl::setLatentEffectivenessofHeatingAirFlowCurve(
      const Curve& latentEffectivenessofHeatingAirFlowCurve) {}

    bool HeatExchangerAirToAirSensibleAndLatent_Impl::setSensibleEffectivenessofCoolingAirFlowCurve(
      const Curve& sensibleEffectivenessofCoolingAirFlowCurve) {}

    bool HeatExchangerAirToAirSensibleAndLatent_Impl::setLatentEffectivenessofCoolingAirFlowCurve(
      const Curve& latentEffectivenessofCoolingAirFlowCurve) {}

    boost::optional<Schedule> HeatExchangerAirToAirSensibleAndLatent_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_HeatExchanger_AirToAir_SensibleAndLatentFields::AvailabilitySchedule);
    }

    std::vector<std::string> HeatExchangerAirToAirSensibleAndLatent_Impl::heatExchangerTypeValues() const {
      return HeatExchangerAirToAirSensibleAndLatent::heatExchangerTypeValues();
    }

    std::vector<std::string> HeatExchangerAirToAirSensibleAndLatent_Impl::frostControlTypeValues() const {
      return HeatExchangerAirToAirSensibleAndLatent::frostControlTypeValues();
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

    unsigned HeatExchangerAirToAirSensibleAndLatent_Impl::primaryAirInletPort() const {
      return OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirInletNode;
    }

    unsigned HeatExchangerAirToAirSensibleAndLatent_Impl::primaryAirOutletPort() const {
      return OS_HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirOutletNode;
    }

    unsigned HeatExchangerAirToAirSensibleAndLatent_Impl::secondaryAirInletPort() const {
      return OS_HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirInletNode;
    }

    unsigned HeatExchangerAirToAirSensibleAndLatent_Impl::secondaryAirOutletPort() const {
      return OS_HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirOutletNode;
    }

    AirflowNetworkEquivalentDuct HeatExchangerAirToAirSensibleAndLatent_Impl::getAirflowNetworkEquivalentDuct(double length, double diameter) {
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

    boost::optional<AirflowNetworkEquivalentDuct> HeatExchangerAirToAirSensibleAndLatent_Impl::airflowNetworkEquivalentDuct() const {
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

    boost::optional<double> HeatExchangerAirToAirSensibleAndLatent_Impl::autosizedNominalSupplyAirFlowRate() const {
      return getAutosizedValue("Design Size Nominal Supply Air Flow Rate", "m3/s");
    }

    void HeatExchangerAirToAirSensibleAndLatent_Impl::autosize() {
      autosizeNominalSupplyAirFlowRate();
    }

    void HeatExchangerAirToAirSensibleAndLatent_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedNominalSupplyAirFlowRate();
      if (val) {
        setNominalSupplyAirFlowRate(val.get());
      }
    }

    ComponentType HeatExchangerAirToAirSensibleAndLatent_Impl::componentType() const {
      return ComponentType::None;
    }

    std::vector<FuelType> HeatExchangerAirToAirSensibleAndLatent_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> HeatExchangerAirToAirSensibleAndLatent_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> HeatExchangerAirToAirSensibleAndLatent_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  HeatExchangerAirToAirSensibleAndLatent::HeatExchangerAirToAirSensibleAndLatent(const Model& model)
    : AirToAirComponent(HeatExchangerAirToAirSensibleAndLatent::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>());

    Schedule schedule = model.alwaysOnDiscreteSchedule();
    setAvailabilitySchedule(schedule);

    autosizeNominalSupplyAirFlowRate();

    setSensibleEffectivenessat100HeatingAirFlow(0.76);

    setLatentEffectivenessat100HeatingAirFlow(0.68);

    // setSensibleEffectivenessat75HeatingAirFlow(0.81); FIXME

    // setLatentEffectivenessat75HeatingAirFlow(0.73); FIXME

    setSensibleEffectivenessat100CoolingAirFlow(0.76);

    setLatentEffectivenessat100CoolingAirFlow(0.68);

    // setSensibleEffectivenessat75CoolingAirFlow(0.81); FIXME

    // setLatentEffectivenessat75CoolingAirFlow(0.73); FIXME

    setNominalElectricPower(0.0);

    setSupplyAirOutletTemperatureControl(true);

    setHeatExchangerType("Plate");

    setFrostControlType("None");

    setThresholdTemperature(1.7);

    setEconomizerLockout(true);

    // TODO
    // setSensibleEffectivenessofHeatingAirFlowCurve
    // setLatentEffectivenessofHeatingAirFlowCurve
    // setSensibleEffectivenessofCoolingAirFlowCurve
    // setLatentEffectivenessofCoolingAirFlowCurve
  }

  IddObjectType HeatExchangerAirToAirSensibleAndLatent::iddObjectType() {
    return {IddObjectType::OS_HeatExchanger_AirToAir_SensibleAndLatent};
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

  bool HeatExchangerAirToAirSensibleAndLatent::isNominalSupplyAirFlowRateAutosized() const {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->isNominalSupplyAirFlowRateAutosized();
  }

  double HeatExchangerAirToAirSensibleAndLatent::sensibleEffectivenessat100HeatingAirFlow() const {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->sensibleEffectivenessat100HeatingAirFlow();
  }

  double HeatExchangerAirToAirSensibleAndLatent::latentEffectivenessat100HeatingAirFlow() const {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->latentEffectivenessat100HeatingAirFlow();
  }

  double HeatExchangerAirToAirSensibleAndLatent::sensibleEffectivenessat100CoolingAirFlow() const {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->sensibleEffectivenessat100CoolingAirFlow();
  }

  double HeatExchangerAirToAirSensibleAndLatent::latentEffectivenessat100CoolingAirFlow() const {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->latentEffectivenessat100CoolingAirFlow();
  }

  double HeatExchangerAirToAirSensibleAndLatent::nominalElectricPower() const {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->nominalElectricPower();
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

  bool HeatExchangerAirToAirSensibleAndLatent::isThresholdTemperatureDefaulted() const {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->isThresholdTemperatureDefaulted();
  }

  boost::optional<double> HeatExchangerAirToAirSensibleAndLatent::initialDefrostTimeFraction() const {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->initialDefrostTimeFraction();
  }

  boost::optional<double> HeatExchangerAirToAirSensibleAndLatent::rateofDefrostTimeFractionIncrease() const {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->rateofDefrostTimeFractionIncrease();
  }

  bool HeatExchangerAirToAirSensibleAndLatent::economizerLockout() const {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->economizerLockout();
  }

  boost::optional<Curve> HeatExchangerAirToAirSensibleAndLatent::sensibleEffectivenessofHeatingAirFlowCurve() const {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->sensibleEffectivenessofHeatingAirFlowCurve();
  }

  boost::optional<Curve> HeatExchangerAirToAirSensibleAndLatent::latentEffectivenessofHeatingAirFlowCurve() const {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->latentEffectivenessofHeatingAirFlowCurve();
  }

  boost::optional<Curve> HeatExchangerAirToAirSensibleAndLatent::sensibleEffectivenessofCoolingAirFlowCurve() const {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->sensibleEffectivenessofCoolingAirFlowCurve();
  }

  boost::optional<Curve> HeatExchangerAirToAirSensibleAndLatent::latentEffectivenessofCoolingAirFlowCurve() const {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->latentEffectivenessofCoolingAirFlowCurve();
  }

  bool HeatExchangerAirToAirSensibleAndLatent::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool HeatExchangerAirToAirSensibleAndLatent::setNominalSupplyAirFlowRate(double nominalSupplyAirFlowRate) {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setNominalSupplyAirFlowRate(nominalSupplyAirFlowRate);
  }

  void HeatExchangerAirToAirSensibleAndLatent::autosizeNominalSupplyAirFlowRate() {
    getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->autosizeNominalSupplyAirFlowRate();
  }

  bool HeatExchangerAirToAirSensibleAndLatent::setSensibleEffectivenessat100HeatingAirFlow(double sensibleEffectivenessat100HeatingAirFlow) {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setSensibleEffectivenessat100HeatingAirFlow(
      sensibleEffectivenessat100HeatingAirFlow);
  }

  bool HeatExchangerAirToAirSensibleAndLatent::setLatentEffectivenessat100HeatingAirFlow(double latentEffectivenessat100HeatingAirFlow) {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setLatentEffectivenessat100HeatingAirFlow(
      latentEffectivenessat100HeatingAirFlow);
  }

  bool HeatExchangerAirToAirSensibleAndLatent::setSensibleEffectivenessat100CoolingAirFlow(double sensibleEffectivenessat100CoolingAirFlow) {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setSensibleEffectivenessat100CoolingAirFlow(
      sensibleEffectivenessat100CoolingAirFlow);
  }

  bool HeatExchangerAirToAirSensibleAndLatent::setLatentEffectivenessat100CoolingAirFlow(double latentEffectivenessat100CoolingAirFlow) {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setLatentEffectivenessat100CoolingAirFlow(
      latentEffectivenessat100CoolingAirFlow);
  }

  bool HeatExchangerAirToAirSensibleAndLatent::setNominalElectricPower(double nominalElectricPower) {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setNominalElectricPower(nominalElectricPower);
  }

  bool HeatExchangerAirToAirSensibleAndLatent::setSupplyAirOutletTemperatureControl(bool supplyAirOutletTemperatureControl) {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setSupplyAirOutletTemperatureControl(supplyAirOutletTemperatureControl);
  }

  bool HeatExchangerAirToAirSensibleAndLatent::setHeatExchangerType(const std::string& heatExchangerType) {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setHeatExchangerType(heatExchangerType);
  }

  bool HeatExchangerAirToAirSensibleAndLatent::setFrostControlType(const std::string& frostControlType) {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setFrostControlType(frostControlType);
  }

  bool HeatExchangerAirToAirSensibleAndLatent::setThresholdTemperature(double thresholdTemperature) {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setThresholdTemperature(thresholdTemperature);
  }

  void HeatExchangerAirToAirSensibleAndLatent::resetThresholdTemperature() {
    getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->resetThresholdTemperature();
  }

  bool HeatExchangerAirToAirSensibleAndLatent::setInitialDefrostTimeFraction(double initialDefrostTimeFraction) {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setInitialDefrostTimeFraction(initialDefrostTimeFraction);
  }

  void HeatExchangerAirToAirSensibleAndLatent::resetInitialDefrostTimeFraction() {
    getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->resetInitialDefrostTimeFraction();
  }

  bool HeatExchangerAirToAirSensibleAndLatent::setRateofDefrostTimeFractionIncrease(double rateofDefrostTimeFractionIncrease) {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setRateofDefrostTimeFractionIncrease(rateofDefrostTimeFractionIncrease);
  }

  void HeatExchangerAirToAirSensibleAndLatent::resetRateofDefrostTimeFractionIncrease() {
    getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->resetRateofDefrostTimeFractionIncrease();
  }

  bool HeatExchangerAirToAirSensibleAndLatent::setEconomizerLockout(bool economizerLockout) {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setEconomizerLockout(economizerLockout);
  }

  bool
    HeatExchangerAirToAirSensibleAndLatent::setSensibleEffectivenessofHeatingAirFlowCurve(const Curve& sensibleEffectivenessofHeatingAirFlowCurve) {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setSensibleEffectivenessofHeatingAirFlowCurve(
      sensibleEffectivenessofHeatingAirFlowCurve);
  }

  bool HeatExchangerAirToAirSensibleAndLatent::setLatentEffectivenessofHeatingAirFlowCurve(const Curve& latentEffectivenessofHeatingAirFlowCurve) {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setLatentEffectivenessofHeatingAirFlowCurve(
      latentEffectivenessofHeatingAirFlowCurve);
  }

  bool
    HeatExchangerAirToAirSensibleAndLatent::setSensibleEffectivenessofCoolingAirFlowCurve(const Curve& sensibleEffectivenessofCoolingAirFlowCurve) {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setSensibleEffectivenessofCoolingAirFlowCurve(
      sensibleEffectivenessofCoolingAirFlowCurve);
  }

  bool HeatExchangerAirToAirSensibleAndLatent::setLatentEffectivenessofCoolingAirFlowCurve(const Curve& latentEffectivenessofCoolingAirFlowCurve) {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->setLatentEffectivenessofCoolingAirFlowCurve(
      latentEffectivenessofCoolingAirFlowCurve);
  }

  AirflowNetworkEquivalentDuct HeatExchangerAirToAirSensibleAndLatent::getAirflowNetworkEquivalentDuct(double length, double diameter) {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->getAirflowNetworkEquivalentDuct(length, diameter);
  }

  boost::optional<AirflowNetworkEquivalentDuct> HeatExchangerAirToAirSensibleAndLatent::airflowNetworkEquivalentDuct() const {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->airflowNetworkEquivalentDuct();
  }

  /// @cond
  HeatExchangerAirToAirSensibleAndLatent::HeatExchangerAirToAirSensibleAndLatent(
    std::shared_ptr<detail::HeatExchangerAirToAirSensibleAndLatent_Impl> impl)
    : AirToAirComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> HeatExchangerAirToAirSensibleAndLatent::autosizedNominalSupplyAirFlowRate() const {
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->autosizedNominalSupplyAirFlowRate();
  }

  // DEPRECATED
  double HeatExchangerAirToAirSensibleAndLatent::sensibleEffectivenessat75HeatingAirFlow() const {
    DEPRECATED_AT_MSG(3, 8, 0, "As of EnergyPlus 24.1.0, this property ...");
    // FIXME: get this value from curve; if no curve, return the 100 value
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->sensibleEffectivenessat100HeatingAirFlow();
  }

  double HeatExchangerAirToAirSensibleAndLatent::latentEffectivenessat75HeatingAirFlow() const {
    DEPRECATED_AT_MSG(3, 8, 0, "As of EnergyPlus 24.1.0, this property ...");
    // FIXME: get this value from curve; if no curve, return the 100 value
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->latentEffectivenessat100HeatingAirFlow();
  }

  double HeatExchangerAirToAirSensibleAndLatent::sensibleEffectivenessat75CoolingAirFlow() const {
    DEPRECATED_AT_MSG(3, 8, 0, "As of EnergyPlus 24.1.0, this property ...");
    // FIXME: get this value from curve; if no curve, return the 100 value
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->sensibleEffectivenessat100CoolingAirFlow();
  }

  double HeatExchangerAirToAirSensibleAndLatent::latentEffectivenessat75CoolingAirFlow() const {
    DEPRECATED_AT_MSG(3, 8, 0, "As of EnergyPlus 24.1.0, this property ...");
    // FIXME: get this value from curve; if no curve, return the 100 value
    return getImpl<detail::HeatExchangerAirToAirSensibleAndLatent_Impl>()->latentEffectivenessat100CoolingAirFlow();
  }

  bool HeatExchangerAirToAirSensibleAndLatent::setSensibleEffectivenessat75HeatingAirFlow(double sensibleEffectivenessat75HeatingAirFlow) {
    DEPRECATED_AT_MSG(3, 8, 0, "As of EnergyPlus 24.1.0, this property ...");
    // FIXME: create a curve here, or just do nothing and return false?
    return false;
  }

  bool HeatExchangerAirToAirSensibleAndLatent::setLatentEffectivenessat75HeatingAirFlow(double latentEffectivenessat75HeatingAirFlow) {
    DEPRECATED_AT_MSG(3, 8, 0, "As of EnergyPlus 24.1.0, this property ...");
    // FIXME: create a curve here, or just do nothing and return false?
    return false;
  }

  bool HeatExchangerAirToAirSensibleAndLatent::setSensibleEffectivenessat75CoolingAirFlow(double sensibleEffectivenessat75CoolingAirFlow) {
    DEPRECATED_AT_MSG(3, 8, 0, "As of EnergyPlus 24.1.0, this property ...");
    // FIXME: create a curve here, or just do nothing and return false?
    return false;
  }

  bool HeatExchangerAirToAirSensibleAndLatent::setLatentEffectivenessat75CoolingAirFlow(double latentEffectivenessat75CoolingAirFlow) {
    DEPRECATED_AT_MSG(3, 8, 0, "As of EnergyPlus 24.1.0, this property ...");
    // FIXME: create a curve here, or just do nothing and return false?
    return false;
  }

}  // namespace model
}  // namespace openstudio
