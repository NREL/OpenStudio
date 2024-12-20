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

// TODO: Check the following class names against object getters and setters.
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "SystemAvailabilityManagerLists.hpp"
#include "SystemAvailabilityManagerLists_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Fans.hpp"
#include "Fans_Impl.hpp"
#include "EvapCooler.hpp"
#include "EvapCooler_Impl.hpp"
#include "DesignSpecificationZoneHVACSizingName.hpp"
#include "DesignSpecificationZoneHVACSizingName_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

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
      // TODO: Check schedule display names.
      std::vector<ScheduleTypeKey> result;
      const UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      if (std::find(fieldIndices.cbegin(), fieldIndices.cend(), OS_ZoneHVAC_EvaporativeCoolerUnitFields::AvailabilityScheduleName)
          != fieldIndices.cend()) {
        result.emplace_back("ZoneHVACEvaporativeCoolerUnit", "Availability");
      }
      return result;
    }

    ComponentType ZoneHVACEvaporativeCoolerUnit_Impl::componentType() const {
      // TODO
      return ComponentType::None;
    }

    std::vector<FuelType> ZoneHVACEvaporativeCoolerUnit_Impl::coolingFuelTypes() const {
      // TODO
      return {};
    }

    std::vector<FuelType> ZoneHVACEvaporativeCoolerUnit_Impl::heatingFuelTypes() const {
      // TODO
      return {};
    }

    std::vector<AppGFuelType> ZoneHVACEvaporativeCoolerUnit_Impl::appGHeatingFuelTypes() const {

      // TODO
      return {};
    }

    Schedule ZoneHVACEvaporativeCoolerUnit_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
      }
      return value.get();
    }

    boost::optional<SystemAvailabilityManagerLists> ZoneHVACEvaporativeCoolerUnit_Impl::availabilityManagerList() const {
      return getObject<ModelObject>().getModelObjectTarget<SystemAvailabilityManagerLists>(
        OS_ZoneHVAC_EvaporativeCoolerUnitFields::AvailabilityManagerListName);
    }

    Connection ZoneHVACEvaporativeCoolerUnit_Impl::outdoorAirInletNode() const {
      boost::optional<Connection> value = optionalOutdoorAirInletNode();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Outdoor Air Inlet Node attached.");
      }
      return value.get();
    }

    Connection ZoneHVACEvaporativeCoolerUnit_Impl::coolerOutletNode() const {
      boost::optional<Connection> value = optionalCoolerOutletNode();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Cooler Outlet Node attached.");
      }
      return value.get();
    }

    boost::optional<Connection> ZoneHVACEvaporativeCoolerUnit_Impl::zoneReliefAirNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_ZoneHVAC_EvaporativeCoolerUnitFields::ZoneReliefAirNodeName);
    }

    Fans ZoneHVACEvaporativeCoolerUnit_Impl::supplyAirFan() const {
      boost::optional<Fans> value = optionalSupplyAirFan();
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
      return getAutosizedValue("TODO_CHECK_SQL Design Supply Air Flow Rate", "m3/s");
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

    EvapCooler ZoneHVACEvaporativeCoolerUnit_Impl::firstEvaporativeCooler() const {
      boost::optional<EvapCooler> value = optionalFirstEvaporativeCooler();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an First Evaporative Cooler attached.");
      }
      return value.get();
    }

    boost::optional<EvapCooler> ZoneHVACEvaporativeCoolerUnit_Impl::secondEvaporativeCooler() const {
      return getObject<ModelObject>().getModelObjectTarget<EvapCooler>(OS_ZoneHVAC_EvaporativeCoolerUnitFields::SecondEvaporativeCoolerName);
    }

    boost::optional<DesignSpecificationZoneHVACSizingName> ZoneHVACEvaporativeCoolerUnit_Impl::designSpecificationZoneHVACSizing() const {
      return getObject<ModelObject>().getModelObjectTarget<DesignSpecificationZoneHVACSizingName>(
        OS_ZoneHVAC_EvaporativeCoolerUnitFields::DesignSpecificationZoneHVACSizing);
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

    bool ZoneHVACEvaporativeCoolerUnit_Impl::setAvailabilityManagerList(const SystemAvailabilityManagerLists& systemAvailabilityManagerLists) {
      const bool result = setPointer(OS_ZoneHVAC_EvaporativeCoolerUnitFields::AvailabilityManagerListName, systemAvailabilityManagerLists.handle());
      return result;
    }

    void ZoneHVACEvaporativeCoolerUnit_Impl::resetAvailabilityManagerList() {
      const bool result = setString(OS_ZoneHVAC_EvaporativeCoolerUnitFields::AvailabilityManagerListName, "");
      OS_ASSERT(result);
    }

    bool ZoneHVACEvaporativeCoolerUnit_Impl::setOutdoorAirInletNode(const Connection& connection) {
      const bool result = setPointer(OS_ZoneHVAC_EvaporativeCoolerUnitFields::OutdoorAirInletNodeName, connection.handle());
      return result;
    }

    bool ZoneHVACEvaporativeCoolerUnit_Impl::setCoolerOutletNode(const Connection& connection) {
      const bool result = setPointer(OS_ZoneHVAC_EvaporativeCoolerUnitFields::CoolerOutletNodeName, connection.handle());
      return result;
    }

    bool ZoneHVACEvaporativeCoolerUnit_Impl::setZoneReliefAirNode(const Connection& connection) {
      const bool result = setPointer(OS_ZoneHVAC_EvaporativeCoolerUnitFields::ZoneReliefAirNodeName, connection.handle());
      return result;
    }

    void ZoneHVACEvaporativeCoolerUnit_Impl::resetZoneReliefAirNode() {
      const bool result = setString(OS_ZoneHVAC_EvaporativeCoolerUnitFields::ZoneReliefAirNodeName, "");
      OS_ASSERT(result);
    }

    bool ZoneHVACEvaporativeCoolerUnit_Impl::setSupplyAirFan(const Fans& fans) {
      const bool result = setPointer(OS_ZoneHVAC_EvaporativeCoolerUnitFields::SupplyAirFanName, fans.handle());
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

    bool ZoneHVACEvaporativeCoolerUnit_Impl::setFirstEvaporativeCooler(const EvapCooler& evapCooler) {
      const bool result = setPointer(OS_ZoneHVAC_EvaporativeCoolerUnitFields::FirstEvaporativeCooler, evapCooler.handle());
      return result;
    }

    bool ZoneHVACEvaporativeCoolerUnit_Impl::setSecondEvaporativeCooler(const EvapCooler& evapCooler) {
      const bool result = setPointer(OS_ZoneHVAC_EvaporativeCoolerUnitFields::SecondEvaporativeCoolerName, evapCooler.handle());
      return result;
    }

    void ZoneHVACEvaporativeCoolerUnit_Impl::resetSecondEvaporativeCooler() {
      const bool result = setString(OS_ZoneHVAC_EvaporativeCoolerUnitFields::SecondEvaporativeCoolerName, "");
      OS_ASSERT(result);
    }

    bool ZoneHVACEvaporativeCoolerUnit_Impl::setDesignSpecificationZoneHVACSizing(
      const DesignSpecificationZoneHVACSizingName& designSpecificationZoneHVACSizingName) {
      const bool result =
        setPointer(OS_ZoneHVAC_EvaporativeCoolerUnitFields::DesignSpecificationZoneHVACSizing, designSpecificationZoneHVACSizingName.handle());
      return result;
    }

    void ZoneHVACEvaporativeCoolerUnit_Impl::resetDesignSpecificationZoneHVACSizing() {
      const bool result = setString(OS_ZoneHVAC_EvaporativeCoolerUnitFields::DesignSpecificationZoneHVACSizing, "");
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
        setDesignSupplyAirFlowRate(*val_));
      }
    }

    boost::optional<Schedule> ZoneHVACEvaporativeCoolerUnit_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_EvaporativeCoolerUnitFields::AvailabilityScheduleName);
    }

    boost::optional<Connection> ZoneHVACEvaporativeCoolerUnit_Impl::optionalOutdoorAirInletNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_ZoneHVAC_EvaporativeCoolerUnitFields::OutdoorAirInletNodeName);
    }

    boost::optional<Connection> ZoneHVACEvaporativeCoolerUnit_Impl::optionalCoolerOutletNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_ZoneHVAC_EvaporativeCoolerUnitFields::CoolerOutletNodeName);
    }

    boost::optional<Fans> ZoneHVACEvaporativeCoolerUnit_Impl::optionalSupplyAirFan() const {
      return getObject<ModelObject>().getModelObjectTarget<Fans>(OS_ZoneHVAC_EvaporativeCoolerUnitFields::SupplyAirFanName);
    }

    boost::optional<EvapCooler> ZoneHVACEvaporativeCoolerUnit_Impl::optionalFirstEvaporativeCooler() const {
      return getObject<ModelObject>().getModelObjectTarget<EvapCooler>(OS_ZoneHVAC_EvaporativeCoolerUnitFields::FirstEvaporativeCooler);
    }

  }  // namespace detail

  ZoneHVACEvaporativeCoolerUnit::ZoneHVACEvaporativeCoolerUnit(const Model& model)
    : ZoneHVACComponent(ZoneHVACEvaporativeCoolerUnit::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>());

    bool ok = true;
    auto alwaysOn = model.alwaysOnDiscreteSchedule();
    ok = setAvailabilitySchedule(alwaysOn);
    OS_ASSERT(ok);
    // ok = setOutdoorAirInletNode();
    OS_ASSERT(ok);
    // ok = setCoolerOutletNode();
    OS_ASSERT(ok);
    // ok = setSupplyAirFan();
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
    // ok = setFirstEvaporativeCooler();
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
    // ok = setOutdoorAirInletNode();
    OS_ASSERT(ok);
    // ok = setCoolerOutletNode();
    OS_ASSERT(ok);
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
    ok = setFirstEvaporativeCooler(firstEvaporativeCooler);
    OS_ASSERT(ok);
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

  boost::optional<SystemAvailabilityManagerLists> ZoneHVACEvaporativeCoolerUnit::availabilityManagerList() const {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->availabilityManagerList();
  }

  Connection ZoneHVACEvaporativeCoolerUnit::outdoorAirInletNode() const {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->outdoorAirInletNode();
  }

  Connection ZoneHVACEvaporativeCoolerUnit::coolerOutletNode() const {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->coolerOutletNode();
  }

  boost::optional<Connection> ZoneHVACEvaporativeCoolerUnit::zoneReliefAirNode() const {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->zoneReliefAirNode();
  }

  Fans ZoneHVACEvaporativeCoolerUnit::supplyAirFan() const {
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

  EvapCooler ZoneHVACEvaporativeCoolerUnit::firstEvaporativeCooler() const {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->firstEvaporativeCooler();
  }

  boost::optional<EvapCooler> ZoneHVACEvaporativeCoolerUnit::secondEvaporativeCooler() const {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->secondEvaporativeCooler();
  }

  boost::optional<DesignSpecificationZoneHVACSizingName> ZoneHVACEvaporativeCoolerUnit::designSpecificationZoneHVACSizing() const {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->designSpecificationZoneHVACSizing();
  }

  double ZoneHVACEvaporativeCoolerUnit::shutOffRelativeHumidity() const {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->shutOffRelativeHumidity();
  }

  bool ZoneHVACEvaporativeCoolerUnit::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool ZoneHVACEvaporativeCoolerUnit::setAvailabilityManagerList(const SystemAvailabilityManagerLists& systemAvailabilityManagerLists) {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->setAvailabilityManagerList(systemAvailabilityManagerLists);
  }

  void ZoneHVACEvaporativeCoolerUnit::resetAvailabilityManagerList() {
    getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->resetAvailabilityManagerList();
  }

  bool ZoneHVACEvaporativeCoolerUnit::setOutdoorAirInletNode(const Connection& connection) {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->setOutdoorAirInletNode(connection);
  }

  bool ZoneHVACEvaporativeCoolerUnit::setCoolerOutletNode(const Connection& connection) {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->setCoolerOutletNode(connection);
  }

  bool ZoneHVACEvaporativeCoolerUnit::setZoneReliefAirNode(const Connection& connection) {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->setZoneReliefAirNode(connection);
  }

  void ZoneHVACEvaporativeCoolerUnit::resetZoneReliefAirNode() {
    getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->resetZoneReliefAirNode();
  }

  bool ZoneHVACEvaporativeCoolerUnit::setSupplyAirFan(const Fans& fans) {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->setSupplyAirFan(fans);
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

  bool ZoneHVACEvaporativeCoolerUnit::setFirstEvaporativeCooler(const EvapCooler& evapCooler) {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->setFirstEvaporativeCooler(evapCooler);
  }

  bool ZoneHVACEvaporativeCoolerUnit::setSecondEvaporativeCooler(const EvapCooler& evapCooler) {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->setSecondEvaporativeCooler(evapCooler);
  }

  void ZoneHVACEvaporativeCoolerUnit::resetSecondEvaporativeCooler() {
    getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->resetSecondEvaporativeCooler();
  }

  bool ZoneHVACEvaporativeCoolerUnit::setDesignSpecificationZoneHVACSizing(
    const DesignSpecificationZoneHVACSizingName& designSpecificationZoneHVACSizingName) {
    return getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->setDesignSpecificationZoneHVACSizing(designSpecificationZoneHVACSizingName);
  }

  void ZoneHVACEvaporativeCoolerUnit::resetDesignSpecificationZoneHVACSizing() {
    getImpl<detail::ZoneHVACEvaporativeCoolerUnit_Impl>()->resetDesignSpecificationZoneHVACSizing();
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
