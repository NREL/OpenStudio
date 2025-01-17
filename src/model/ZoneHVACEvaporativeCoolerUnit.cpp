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

#include "ZoneHVACEvaporativeCoolerUnit.hpp"
#include "ZoneHVACEvaporativeCoolerUnit_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "FanSystemModel.hpp"
#include "FanSystemModel_Impl.hpp"
#include "EvaporativeCoolerDirectResearchSpecial.hpp"
#include "EvaporativeCoolerDirectResearchSpecial_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ZoneHVAC_EvaporativeCoolerUnit_FieldEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    ZoneHVACEvaporativeCoolerUnit_Impl::ZoneHVACEvaporativeCoolerUnit_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ZoneHVACEvaporativeCoolerUnit::iddObjectType());
    }

    ZoneHVACEvaporativeCoolerUnit_Impl::ZoneHVACEvaporativeCoolerUnit_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ZoneHVACEvaporativeCoolerUnit::iddObjectType());
    }

    ZoneHVACEvaporativeCoolerUnit_Impl::ZoneHVACEvaporativeCoolerUnit_Impl(const ZoneHVACEvaporativeCoolerUnit_Impl& other, Model_Impl* model,
                                                                           bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {}

    ModelObject ZoneHVACEvaporativeCoolerUnit_Impl::clone(Model model) const {
      auto evaporativeCoolUnitClone = ZoneHVACComponent_Impl::clone(model).cast<ZoneHVACEvaporativeCoolerUnit>();

      if (OptionalHVACComponent intermediate = optionalSupplyAirFan()) {
        evaporativeCoolUnitClone.setSupplyAirFan(intermediate->clone(model).cast<HVACComponent>());
      }
      if (OptionalHVACComponent intermediate = optionalFirstEvaporativeCooler()) {
        evaporativeCoolUnitClone.setFirstEvaporativeCooler(intermediate->clone(model).cast<HVACComponent>());
      }
      if (OptionalHVACComponent intermediate = secondEvaporativeCooler()) {
        evaporativeCoolUnitClone.setSecondEvaporativeCooler(intermediate->clone(model).cast<HVACComponent>());
      }

      return std::move(evaporativeCoolUnitClone);
    }

    const std::vector<std::string>& ZoneHVACEvaporativeCoolerUnit_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType ZoneHVACEvaporativeCoolerUnit_Impl::iddObjectType() const {
      return ZoneHVACEvaporativeCoolerUnit::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ZoneHVACEvaporativeCoolerUnit_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      const UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      if (std::find(fieldIndices.cbegin(), fieldIndices.cend(), OS_ZoneHVAC_EvaporativeCoolerUnitFields::AvailabilityScheduleName)
          != fieldIndices.cend()) {
        result.emplace_back("ZoneHVACEvaporativeCoolerUnit", "Availability");
      }
      return result;
    }

    std::vector<ModelObject> ZoneHVACEvaporativeCoolerUnit_Impl::children() const {
      std::vector<ModelObject> result;
      if (OptionalHVACComponent intermediate = optionalSupplyAirFan()) {
        result.push_back(*intermediate);
      }
      if (OptionalHVACComponent intermediate = optionalFirstEvaporativeCooler()) {
        result.push_back(*intermediate);
      }
      if (OptionalHVACComponent intermediate = secondEvaporativeCooler()) {
        result.push_back(*intermediate);
      }
      return result;
    }

    unsigned ZoneHVACEvaporativeCoolerUnit_Impl::inletPort() const {
      return OS_ZoneHVAC_EvaporativeCoolerUnitFields::ZoneReliefAirNodeName;
    }

    unsigned ZoneHVACEvaporativeCoolerUnit_Impl::outletPort() const {
      return OS_ZoneHVAC_EvaporativeCoolerUnitFields::CoolerOutletNodeName;
    }

    ComponentType ZoneHVACEvaporativeCoolerUnit_Impl::componentType() const {
      return ComponentType::Cooling;
    }

    std::vector<FuelType> ZoneHVACEvaporativeCoolerUnit_Impl::coolingFuelTypes() const {
      std::set<FuelType> result;
      for (auto ft : firstEvaporativeCooler().coolingFuelTypes()) {
        result.insert(ft);
      }
      if (auto secondEvaporativeCooler_ = secondEvaporativeCooler()) {
        for (auto ft : secondEvaporativeCooler_->coolingFuelTypes()) {
          result.insert(ft);
        }
      }
      return {result.begin(), result.end()};
    }

    std::vector<FuelType> ZoneHVACEvaporativeCoolerUnit_Impl::heatingFuelTypes() const {
      std::set<FuelType> result;
      for (auto ft : firstEvaporativeCooler().heatingFuelTypes()) {
        result.insert(ft);
      }
      if (auto secondEvaporativeCooler_ = secondEvaporativeCooler()) {
        for (auto ft : secondEvaporativeCooler_->heatingFuelTypes()) {
          result.insert(ft);
        }
      }
      return {result.begin(), result.end()};
    }

    std::vector<AppGFuelType> ZoneHVACEvaporativeCoolerUnit_Impl::appGHeatingFuelTypes() const {
      std::set<AppGFuelType> result;
      for (auto ft : firstEvaporativeCooler().appGHeatingFuelTypes()) {
        result.insert(ft);
      }
      if (auto secondEvaporativeCooler_ = secondEvaporativeCooler()) {
        for (auto ft : secondEvaporativeCooler_->appGHeatingFuelTypes()) {
          result.insert(ft);
        }
      }
      return {result.begin(), result.end()};
    }

    Schedule ZoneHVACEvaporativeCoolerUnit_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
      }
      return value.get();
    }

    HVACComponent ZoneHVACEvaporativeCoolerUnit_Impl::supplyAirFan() const {
      boost::optional<HVACComponent> value = optionalSupplyAirFan();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Supply Air Fan attached.");
      }
      return value.get();
    }

    boost::optional<double> ZoneHVACEvaporativeCoolerUnit_Impl::designSupplyAirFlowRate() const {
      return getDouble(OS_ZoneHVAC_EvaporativeCoolerUnitFields::DesignSupplyAirFlowRate, true);
    }

    bool ZoneHVACEvaporativeCoolerUnit_Impl::isDesignSupplyAirFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_ZoneHVAC_EvaporativeCoolerUnitFields::DesignSupplyAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> ZoneHVACEvaporativeCoolerUnit_Impl::autosizedDesignSupplyAirFlowRate() {
      return getAutosizedValue("Design Size Design Supply Air Flow Rate", "m3/s");
    }

    std::string ZoneHVACEvaporativeCoolerUnit_Impl::fanPlacement() const {
      boost::optional<std::string> value = getString(OS_ZoneHVAC_EvaporativeCoolerUnitFields::FanPlacement, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string ZoneHVACEvaporativeCoolerUnit_Impl::coolerUnitControlMethod() const {
      boost::optional<std::string> value = getString(OS_ZoneHVAC_EvaporativeCoolerUnitFields::CoolerUnitControlMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ZoneHVACEvaporativeCoolerUnit_Impl::throttlingRangeTemperatureDifference() const {
      boost::optional<double> value = getDouble(OS_ZoneHVAC_EvaporativeCoolerUnitFields::ThrottlingRangeTemperatureDifference, true);
      OS_ASSERT(value);
      return value.get();
    }

    double ZoneHVACEvaporativeCoolerUnit_Impl::coolingLoadControlThresholdHeatTransferRate() const {
      boost::optional<double> value = getDouble(OS_ZoneHVAC_EvaporativeCoolerUnitFields::CoolingLoadControlThresholdHeatTransferRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    HVACComponent ZoneHVACEvaporativeCoolerUnit_Impl::firstEvaporativeCooler() const {
      boost::optional<HVACComponent> value = optionalFirstEvaporativeCooler();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an First Evaporative Cooler attached.");
      }
      return value.get();
    }

    boost::optional<HVACComponent> ZoneHVACEvaporativeCoolerUnit_Impl::secondEvaporativeCooler() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_EvaporativeCoolerUnitFields::SecondEvaporativeCooler);
    }

    double ZoneHVACEvaporativeCoolerUnit_Impl::shutOffRelativeHumidity() const {
      boost::optional<double> value = getDouble(OS_ZoneHVAC_EvaporativeCoolerUnitFields::ShutOffRelativeHumidity, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ZoneHVACEvaporativeCoolerUnit_Impl::setAvailabilitySchedule(Schedule& schedule) {
      const bool result =
        setSchedule(OS_ZoneHVAC_EvaporativeCoolerUnitFields::AvailabilityScheduleName, "ZoneHVACEvaporativeCoolerUnit", "Availability", schedule);
      return result;
    }

    bool ZoneHVACEvaporativeCoolerUnit_Impl::setSupplyAirFan(const HVACComponent& hvacComponent) {
      const bool result = setPointer(OS_ZoneHVAC_EvaporativeCoolerUnitFields::SupplyAirFanName, hvacComponent.handle());
      return result;
    }

    bool ZoneHVACEvaporativeCoolerUnit_Impl::setDesignSupplyAirFlowRate(double designSupplyAirFlowRate) {
      const bool result = setDouble(OS_ZoneHVAC_EvaporativeCoolerUnitFields::DesignSupplyAirFlowRate, designSupplyAirFlowRate);
      return result;
    }

    void ZoneHVACEvaporativeCoolerUnit_Impl::autosizeDesignSupplyAirFlowRate() {
      const bool result = setString(OS_ZoneHVAC_EvaporativeCoolerUnitFields::DesignSupplyAirFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool ZoneHVACEvaporativeCoolerUnit_Impl::setFanPlacement(const std::string& fanPlacement) {
      const bool result = setString(OS_ZoneHVAC_EvaporativeCoolerUnitFields::FanPlacement, fanPlacement);
      return result;
    }

    bool ZoneHVACEvaporativeCoolerUnit_Impl::setCoolerUnitControlMethod(const std::string& coolerUnitControlMethod) {
      const bool result = setString(OS_ZoneHVAC_EvaporativeCoolerUnitFields::CoolerUnitControlMethod, coolerUnitControlMethod);
      return result;
    }

    bool ZoneHVACEvaporativeCoolerUnit_Impl::setThrottlingRangeTemperatureDifference(double throttlingRangeTemperatureDifference) {
      const bool result =
        setDouble(OS_ZoneHVAC_EvaporativeCoolerUnitFields::ThrottlingRangeTemperatureDifference, throttlingRangeTemperatureDifference);
      return result;
    }

    bool ZoneHVACEvaporativeCoolerUnit_Impl::setCoolingLoadControlThresholdHeatTransferRate(double coolingLoadControlThresholdHeatTransferRate) {
      const bool result =
        setDouble(OS_ZoneHVAC_EvaporativeCoolerUnitFields::CoolingLoadControlThresholdHeatTransferRate, coolingLoadControlThresholdHeatTransferRate);
      return result;
    }

    bool ZoneHVACEvaporativeCoolerUnit_Impl::setFirstEvaporativeCooler(const HVACComponent& hvacComponent) {
      const bool result = setPointer(OS_ZoneHVAC_EvaporativeCoolerUnitFields::FirstEvaporativeCooler, hvacComponent.handle());
      return result;
    }

    bool ZoneHVACEvaporativeCoolerUnit_Impl::setSecondEvaporativeCooler(const HVACComponent& hvacComponent) {
      const bool result = setPointer(OS_ZoneHVAC_EvaporativeCoolerUnitFields::SecondEvaporativeCooler, hvacComponent.handle());
      return result;
    }

    void ZoneHVACEvaporativeCoolerUnit_Impl::resetSecondEvaporativeCooler() {
      const bool result = setString(OS_ZoneHVAC_EvaporativeCoolerUnitFields::SecondEvaporativeCooler, "");
      OS_ASSERT(result);
    }

    bool ZoneHVACEvaporativeCoolerUnit_Impl::setShutOffRelativeHumidity(double shutOffRelativeHumidity) {
      const bool result = setDouble(OS_ZoneHVAC_EvaporativeCoolerUnitFields::ShutOffRelativeHumidity, shutOffRelativeHumidity);
      return result;
    }

    void ZoneHVACEvaporativeCoolerUnit_Impl::autosize() {
      autosizeDesignSupplyAirFlowRate();
    }

    void ZoneHVACEvaporativeCoolerUnit_Impl::applySizingValues() {
      if (boost::optional<double> val_ = autosizedDesignSupplyAirFlowRate()) {
        setDesignSupplyAirFlowRate(*val_);
      }
    }

    boost::optional<Schedule> ZoneHVACEvaporativeCoolerUnit_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_EvaporativeCoolerUnitFields::AvailabilityScheduleName);
    }

    boost::optional<HVACComponent> ZoneHVACEvaporativeCoolerUnit_Impl::optionalSupplyAirFan() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_EvaporativeCoolerUnitFields::SupplyAirFanName);
    }

    boost::optional<HVACComponent> ZoneHVACEvaporativeCoolerUnit_Impl::optionalFirstEvaporativeCooler() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_EvaporativeCoolerUnitFields::FirstEvaporativeCooler);
    }

  }  // namespace detail

  ZoneHVACEvaporativeCoolerUnit::ZoneHVACEvaporativeCoolerUnit(const Model& model)
    : ZoneHVACComponent(ZoneHVACEvaporativeCoolerUnit::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>());

    bool ok = true;
    auto alwaysOn = model.alwaysOnDiscreteSchedule();
    ok = setAvailabilitySchedule(alwaysOn);
    OS_ASSERT(ok);

    FanSystemModel supplyAirFan(model);
    ok = setSupplyAirFan(supplyAirFan);
    OS_ASSERT(ok);

    autosizeDesignSupplyAirFlowRate();
    ok = setFanPlacement("BlowThrough");
    OS_ASSERT(ok);
    ok = setCoolerUnitControlMethod("ZoneTemperatureDeadbandOnOffCycling");
    OS_ASSERT(ok);
    ok = setThrottlingRangeTemperatureDifference(1.0);
    OS_ASSERT(ok);
    ok = setCoolingLoadControlThresholdHeatTransferRate(100.0);
    OS_ASSERT(ok);

    EvaporativeCoolerDirectResearchSpecial firstEvaporativeCooler(model, alwaysOn);
    ok = setFirstEvaporativeCooler(firstEvaporativeCooler);
    OS_ASSERT(ok);

    ok = setShutOffRelativeHumidity(100.0);
    OS_ASSERT(ok);
  }

  ZoneHVACEvaporativeCoolerUnit::ZoneHVACEvaporativeCoolerUnit(const Model& model, Schedule& availabilitySchedule, HVACComponent& supplyAirFan,
                                                               HVACComponent& firstEvaporativeCooler)
    : ZoneHVACComponent(ZoneHVACEvaporativeCoolerUnit::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>());

    bool ok = true;
    ok = setAvailabilitySchedule(availabilitySchedule);
    OS_ASSERT(ok);

    ok = setSupplyAirFan(supplyAirFan);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s supply air fan to " << supplyAirFan.briefDescription() << ".");
    }

    autosizeDesignSupplyAirFlowRate();
    ok = setFanPlacement("BlowThrough");
    OS_ASSERT(ok);
    ok = setCoolerUnitControlMethod("ZoneTemperatureDeadbandOnOffCycling");
    OS_ASSERT(ok);
    ok = setThrottlingRangeTemperatureDifference(1.0);
    OS_ASSERT(ok);
    ok = setCoolingLoadControlThresholdHeatTransferRate(100.0);
    OS_ASSERT(ok);

    ok = setFirstEvaporativeCooler(firstEvaporativeCooler);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s first evaporative cooler to " << firstEvaporativeCooler.briefDescription() << ".");
    }

    ok = setShutOffRelativeHumidity(100.0);
    OS_ASSERT(ok);
  }

  IddObjectType ZoneHVACEvaporativeCoolerUnit::iddObjectType() {
    return {IddObjectType::OS_ZoneHVAC_EvaporativeCoolerUnit};
  }

  std::vector<std::string> ZoneHVACEvaporativeCoolerUnit::fanPlacementValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_ZoneHVAC_EvaporativeCoolerUnitFields::FanPlacement);
  }

  std::vector<std::string> ZoneHVACEvaporativeCoolerUnit::coolerUnitControlMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_ZoneHVAC_EvaporativeCoolerUnitFields::CoolerUnitControlMethod);
  }

  Schedule ZoneHVACEvaporativeCoolerUnit::availabilitySchedule() const {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->availabilitySchedule();
  }

  HVACComponent ZoneHVACEvaporativeCoolerUnit::supplyAirFan() const {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->supplyAirFan();
  }

  boost::optional<double> ZoneHVACEvaporativeCoolerUnit::designSupplyAirFlowRate() const {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->designSupplyAirFlowRate();
  }

  bool ZoneHVACEvaporativeCoolerUnit::isDesignSupplyAirFlowRateAutosized() const {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->isDesignSupplyAirFlowRateAutosized();
  }

  boost::optional<double> ZoneHVACEvaporativeCoolerUnit::autosizedDesignSupplyAirFlowRate() {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->autosizedDesignSupplyAirFlowRate();
  }

  std::string ZoneHVACEvaporativeCoolerUnit::fanPlacement() const {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->fanPlacement();
  }

  std::string ZoneHVACEvaporativeCoolerUnit::coolerUnitControlMethod() const {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->coolerUnitControlMethod();
  }

  double ZoneHVACEvaporativeCoolerUnit::throttlingRangeTemperatureDifference() const {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->throttlingRangeTemperatureDifference();
  }

  double ZoneHVACEvaporativeCoolerUnit::coolingLoadControlThresholdHeatTransferRate() const {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->coolingLoadControlThresholdHeatTransferRate();
  }

  HVACComponent ZoneHVACEvaporativeCoolerUnit::firstEvaporativeCooler() const {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->firstEvaporativeCooler();
  }

  boost::optional<HVACComponent> ZoneHVACEvaporativeCoolerUnit::secondEvaporativeCooler() const {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->secondEvaporativeCooler();
  }

  double ZoneHVACEvaporativeCoolerUnit::shutOffRelativeHumidity() const {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->shutOffRelativeHumidity();
  }

  bool ZoneHVACEvaporativeCoolerUnit::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool ZoneHVACEvaporativeCoolerUnit::setSupplyAirFan(const HVACComponent& hvacComponent) {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->setSupplyAirFan(hvacComponent);
  }

  bool ZoneHVACEvaporativeCoolerUnit::setDesignSupplyAirFlowRate(double designSupplyAirFlowRate) {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->setDesignSupplyAirFlowRate(designSupplyAirFlowRate);
  }

  void ZoneHVACEvaporativeCoolerUnit::autosizeDesignSupplyAirFlowRate() {
    getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->autosizeDesignSupplyAirFlowRate();
  }

  bool ZoneHVACEvaporativeCoolerUnit::setFanPlacement(const std::string& fanPlacement) {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->setFanPlacement(fanPlacement);
  }

  bool ZoneHVACEvaporativeCoolerUnit::setCoolerUnitControlMethod(const std::string& coolerUnitControlMethod) {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->setCoolerUnitControlMethod(coolerUnitControlMethod);
  }

  bool ZoneHVACEvaporativeCoolerUnit::setThrottlingRangeTemperatureDifference(double throttlingRangeTemperatureDifference) {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->setThrottlingRangeTemperatureDifference(throttlingRangeTemperatureDifference);
  }

  bool ZoneHVACEvaporativeCoolerUnit::setCoolingLoadControlThresholdHeatTransferRate(double coolingLoadControlThresholdHeatTransferRate) {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->setCoolingLoadControlThresholdHeatTransferRate(
      coolingLoadControlThresholdHeatTransferRate);
  }

  bool ZoneHVACEvaporativeCoolerUnit::setFirstEvaporativeCooler(const HVACComponent& hvacComponent) {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->setFirstEvaporativeCooler(hvacComponent);
  }

  bool ZoneHVACEvaporativeCoolerUnit::setSecondEvaporativeCooler(const HVACComponent& hvacComponent) {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->setSecondEvaporativeCooler(hvacComponent);
  }

  void ZoneHVACEvaporativeCoolerUnit::resetSecondEvaporativeCooler() {
    getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->resetSecondEvaporativeCooler();
  }

  bool ZoneHVACEvaporativeCoolerUnit::setShutOffRelativeHumidity(double shutOffRelativeHumidity) {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->setShutOffRelativeHumidity(shutOffRelativeHumidity);
  }

  /// @cond
  ZoneHVACEvaporativeCoolerUnit::ZoneHVACEvaporativeCoolerUnit(std::shared_ptr<detail::ZoneHVACEvaporativeCoolerUnit_Impl> impl)
    : ZoneHVACComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
