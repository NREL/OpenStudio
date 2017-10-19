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

#include "ZoneHVACIdealLoadsAirSystem.hpp"
#include "ZoneHVACIdealLoadsAirSystem_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "DesignSpecificationOutdoorAir.hpp"
#include "DesignSpecificationOutdoorAir_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_ZoneHVAC_IdealLoadsAirSystem_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneHVACIdealLoadsAirSystem_Impl::ZoneHVACIdealLoadsAirSystem_Impl(const IdfObject& idfObject,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneHVACIdealLoadsAirSystem::iddObjectType());
  }

  ZoneHVACIdealLoadsAirSystem_Impl::ZoneHVACIdealLoadsAirSystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneHVACIdealLoadsAirSystem::iddObjectType());
  }

  ZoneHVACIdealLoadsAirSystem_Impl::ZoneHVACIdealLoadsAirSystem_Impl(const ZoneHVACIdealLoadsAirSystem_Impl& other,
                                                                     Model_Impl* model,
                                                                     bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ZoneHVACIdealLoadsAirSystem_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ZoneHVACIdealLoadsAirSystem_Impl::iddObjectType() const {
    return ZoneHVACIdealLoadsAirSystem::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneHVACIdealLoadsAirSystem_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneHVAC_IdealLoadsAirSystemFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACIdealLoadsAirSystem","Availability"));
    }
    if (std::find(b,e,OS_ZoneHVAC_IdealLoadsAirSystemFields::HeatingAvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACIdealLoadsAirSystem","Heating Availability"));
    }
    if (std::find(b,e,OS_ZoneHVAC_IdealLoadsAirSystemFields::CoolingAvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACIdealLoadsAirSystem","Cooling Availability"));
    }
    return result;
  }

  unsigned ZoneHVACIdealLoadsAirSystem_Impl::inletPort() const
  {
    return OS_ZoneHVAC_IdealLoadsAirSystemFields::ZoneSupplyAirNodeName;
  }

  unsigned ZoneHVACIdealLoadsAirSystem_Impl::outletPort() const
  {
    return OS_ZoneHVAC_IdealLoadsAirSystemFields::ZoneExhaustAirNodeName;
  }

  boost::optional<Schedule> ZoneHVACIdealLoadsAirSystem_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_IdealLoadsAirSystemFields::AvailabilityScheduleName);
  }

  double ZoneHVACIdealLoadsAirSystem_Impl::maximumHeatingSupplyAirTemperature() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingSupplyAirTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity ZoneHVACIdealLoadsAirSystem_Impl::getMaximumHeatingSupplyAirTemperature(bool returnIP) const {
    OptionalDouble value = maximumHeatingSupplyAirTemperature();
    OSOptionalQuantity result = getQuantityFromDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingSupplyAirTemperature, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::isMaximumHeatingSupplyAirTemperatureDefaulted() const {
    return isEmpty(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingSupplyAirTemperature);
  }

  double ZoneHVACIdealLoadsAirSystem_Impl::minimumCoolingSupplyAirTemperature() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MinimumCoolingSupplyAirTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity ZoneHVACIdealLoadsAirSystem_Impl::getMinimumCoolingSupplyAirTemperature(bool returnIP) const {
    OptionalDouble value = minimumCoolingSupplyAirTemperature();
    OSOptionalQuantity result = getQuantityFromDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MinimumCoolingSupplyAirTemperature, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::isMinimumCoolingSupplyAirTemperatureDefaulted() const {
    return isEmpty(OS_ZoneHVAC_IdealLoadsAirSystemFields::MinimumCoolingSupplyAirTemperature);
  }

  double ZoneHVACIdealLoadsAirSystem_Impl::maximumHeatingSupplyAirHumidityRatio() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingSupplyAirHumidityRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity ZoneHVACIdealLoadsAirSystem_Impl::getMaximumHeatingSupplyAirHumidityRatio(bool returnIP) const {
    OptionalDouble value = maximumHeatingSupplyAirHumidityRatio();
    OSOptionalQuantity result = getQuantityFromDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingSupplyAirHumidityRatio, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::isMaximumHeatingSupplyAirHumidityRatioDefaulted() const {
    return isEmpty(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingSupplyAirHumidityRatio);
  }

  double ZoneHVACIdealLoadsAirSystem_Impl::minimumCoolingSupplyAirHumidityRatio() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MinimumCoolingSupplyAirHumidityRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity ZoneHVACIdealLoadsAirSystem_Impl::getMinimumCoolingSupplyAirHumidityRatio(bool returnIP) const {
    OptionalDouble value = minimumCoolingSupplyAirHumidityRatio();
    OSOptionalQuantity result = getQuantityFromDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MinimumCoolingSupplyAirHumidityRatio, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::isMinimumCoolingSupplyAirHumidityRatioDefaulted() const {
    return isEmpty(OS_ZoneHVAC_IdealLoadsAirSystemFields::MinimumCoolingSupplyAirHumidityRatio);
  }

  std::string ZoneHVACIdealLoadsAirSystem_Impl::heatingLimit() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_IdealLoadsAirSystemFields::HeatingLimit,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::isHeatingLimitDefaulted() const {
    return isEmpty(OS_ZoneHVAC_IdealLoadsAirSystemFields::HeatingLimit);
  }

  boost::optional<double> ZoneHVACIdealLoadsAirSystem_Impl::maximumHeatingAirFlowRate() const {
    return getDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingAirFlowRate,true);
  }

  OSOptionalQuantity ZoneHVACIdealLoadsAirSystem_Impl::getMaximumHeatingAirFlowRate(bool returnIP) const {
    OptionalDouble value = maximumHeatingAirFlowRate();
    return getQuantityFromDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingAirFlowRate, value, returnIP);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::isMaximumHeatingAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> ZoneHVACIdealLoadsAirSystem_Impl::maximumSensibleHeatingCapacity() const {
    return getDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumSensibleHeatingCapacity,true);
  }

  OSOptionalQuantity ZoneHVACIdealLoadsAirSystem_Impl::getMaximumSensibleHeatingCapacity(bool returnIP) const {
    OptionalDouble value = maximumSensibleHeatingCapacity();
    return getQuantityFromDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumSensibleHeatingCapacity, value, returnIP);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::isMaximumSensibleHeatingCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumSensibleHeatingCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  std::string ZoneHVACIdealLoadsAirSystem_Impl::coolingLimit() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_IdealLoadsAirSystemFields::CoolingLimit,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::isCoolingLimitDefaulted() const {
    return isEmpty(OS_ZoneHVAC_IdealLoadsAirSystemFields::CoolingLimit);
  }

  boost::optional<double> ZoneHVACIdealLoadsAirSystem_Impl::maximumCoolingAirFlowRate() const {
    return getDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumCoolingAirFlowRate,true);
  }

  OSOptionalQuantity ZoneHVACIdealLoadsAirSystem_Impl::getMaximumCoolingAirFlowRate(bool returnIP) const {
    OptionalDouble value = maximumCoolingAirFlowRate();
    return getQuantityFromDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumCoolingAirFlowRate, value, returnIP);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::isMaximumCoolingAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumCoolingAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> ZoneHVACIdealLoadsAirSystem_Impl::maximumTotalCoolingCapacity() const {
    return getDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumTotalCoolingCapacity,true);
  }

  OSOptionalQuantity ZoneHVACIdealLoadsAirSystem_Impl::getMaximumTotalCoolingCapacity(bool returnIP) const {
    OptionalDouble value = maximumTotalCoolingCapacity();
    return getQuantityFromDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumTotalCoolingCapacity, value, returnIP);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::isMaximumTotalCoolingCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumTotalCoolingCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<Schedule> ZoneHVACIdealLoadsAirSystem_Impl::heatingAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_IdealLoadsAirSystemFields::HeatingAvailabilityScheduleName);
  }

  boost::optional<Schedule> ZoneHVACIdealLoadsAirSystem_Impl::coolingAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_IdealLoadsAirSystemFields::CoolingAvailabilityScheduleName);
  }

  std::string ZoneHVACIdealLoadsAirSystem_Impl::dehumidificationControlType() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_IdealLoadsAirSystemFields::DehumidificationControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::isDehumidificationControlTypeDefaulted() const {
    return isEmpty(OS_ZoneHVAC_IdealLoadsAirSystemFields::DehumidificationControlType);
  }

  double ZoneHVACIdealLoadsAirSystem_Impl::coolingSensibleHeatRatio() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::CoolingSensibleHeatRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity ZoneHVACIdealLoadsAirSystem_Impl::getCoolingSensibleHeatRatio(bool returnIP) const {
    OptionalDouble value = coolingSensibleHeatRatio();
    OSOptionalQuantity result = getQuantityFromDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::CoolingSensibleHeatRatio, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::isCoolingSensibleHeatRatioDefaulted() const {
    return isEmpty(OS_ZoneHVAC_IdealLoadsAirSystemFields::CoolingSensibleHeatRatio);
  }

  std::string ZoneHVACIdealLoadsAirSystem_Impl::humidificationControlType() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_IdealLoadsAirSystemFields::HumidificationControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::isHumidificationControlTypeDefaulted() const {
    return isEmpty(OS_ZoneHVAC_IdealLoadsAirSystemFields::HumidificationControlType);
  }

  boost::optional<DesignSpecificationOutdoorAir> ZoneHVACIdealLoadsAirSystem_Impl::designSpecificationOutdoorAirObject() const {
    return getObject<ModelObject>().getModelObjectTarget<DesignSpecificationOutdoorAir>(OS_ZoneHVAC_IdealLoadsAirSystemFields::DesignSpecificationOutdoorAirObjectName);
  }

  std::string ZoneHVACIdealLoadsAirSystem_Impl::demandControlledVentilationType() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_IdealLoadsAirSystemFields::DemandControlledVentilationType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::isDemandControlledVentilationTypeDefaulted() const {
    return isEmpty(OS_ZoneHVAC_IdealLoadsAirSystemFields::DemandControlledVentilationType);
  }

  std::string ZoneHVACIdealLoadsAirSystem_Impl::outdoorAirEconomizerType() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_IdealLoadsAirSystemFields::OutdoorAirEconomizerType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::isOutdoorAirEconomizerTypeDefaulted() const {
    return isEmpty(OS_ZoneHVAC_IdealLoadsAirSystemFields::OutdoorAirEconomizerType);
  }

  std::string ZoneHVACIdealLoadsAirSystem_Impl::heatRecoveryType() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_IdealLoadsAirSystemFields::HeatRecoveryType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::isHeatRecoveryTypeDefaulted() const {
    return isEmpty(OS_ZoneHVAC_IdealLoadsAirSystemFields::HeatRecoveryType);
  }

  double ZoneHVACIdealLoadsAirSystem_Impl::sensibleHeatRecoveryEffectiveness() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::SensibleHeatRecoveryEffectiveness,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity ZoneHVACIdealLoadsAirSystem_Impl::getSensibleHeatRecoveryEffectiveness(bool returnIP) const {
    OptionalDouble value = sensibleHeatRecoveryEffectiveness();
    OSOptionalQuantity result = getQuantityFromDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::SensibleHeatRecoveryEffectiveness, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::isSensibleHeatRecoveryEffectivenessDefaulted() const {
    return isEmpty(OS_ZoneHVAC_IdealLoadsAirSystemFields::SensibleHeatRecoveryEffectiveness);
  }

  double ZoneHVACIdealLoadsAirSystem_Impl::latentHeatRecoveryEffectiveness() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::LatentHeatRecoveryEffectiveness,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity ZoneHVACIdealLoadsAirSystem_Impl::getLatentHeatRecoveryEffectiveness(bool returnIP) const {
    OptionalDouble value = latentHeatRecoveryEffectiveness();
    OSOptionalQuantity result = getQuantityFromDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::LatentHeatRecoveryEffectiveness, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::isLatentHeatRecoveryEffectivenessDefaulted() const {
    return isEmpty(OS_ZoneHVAC_IdealLoadsAirSystemFields::LatentHeatRecoveryEffectiveness);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_IdealLoadsAirSystemFields::AvailabilityScheduleName,
                              "ZoneHVACIdealLoadsAirSystem",
                              "Availability",
                              schedule);
    return result;
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setMaximumHeatingSupplyAirTemperature(double maximumHeatingSupplyAirTemperature) {
    bool result = setDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingSupplyAirTemperature, maximumHeatingSupplyAirTemperature);
    return result;
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setMaximumHeatingSupplyAirTemperature(const Quantity& maximumHeatingSupplyAirTemperature) {
    OptionalDouble value = getDoubleFromQuantity(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingSupplyAirTemperature,maximumHeatingSupplyAirTemperature);
    if (!value) {
      return false;
    }
    return setMaximumHeatingSupplyAirTemperature(value.get());
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::resetMaximumHeatingSupplyAirTemperature() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingSupplyAirTemperature, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setMinimumCoolingSupplyAirTemperature(double minimumCoolingSupplyAirTemperature) {
    bool result = setDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MinimumCoolingSupplyAirTemperature, minimumCoolingSupplyAirTemperature);
    return result;
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setMinimumCoolingSupplyAirTemperature(const Quantity& minimumCoolingSupplyAirTemperature) {
    OptionalDouble value = getDoubleFromQuantity(OS_ZoneHVAC_IdealLoadsAirSystemFields::MinimumCoolingSupplyAirTemperature,minimumCoolingSupplyAirTemperature);
    if (!value) {
      return false;
    }
    return setMinimumCoolingSupplyAirTemperature(value.get());
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::resetMinimumCoolingSupplyAirTemperature() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::MinimumCoolingSupplyAirTemperature, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setMaximumHeatingSupplyAirHumidityRatio(double maximumHeatingSupplyAirHumidityRatio) {
    bool result = setDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingSupplyAirHumidityRatio, maximumHeatingSupplyAirHumidityRatio);
    return result;
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setMaximumHeatingSupplyAirHumidityRatio(const Quantity& maximumHeatingSupplyAirHumidityRatio) {
    OptionalDouble value = getDoubleFromQuantity(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingSupplyAirHumidityRatio,maximumHeatingSupplyAirHumidityRatio);
    if (!value) {
      return false;
    }
    return setMaximumHeatingSupplyAirHumidityRatio(value.get());
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::resetMaximumHeatingSupplyAirHumidityRatio() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingSupplyAirHumidityRatio, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setMinimumCoolingSupplyAirHumidityRatio(double minimumCoolingSupplyAirHumidityRatio) {
    bool result = setDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MinimumCoolingSupplyAirHumidityRatio, minimumCoolingSupplyAirHumidityRatio);
    return result;
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setMinimumCoolingSupplyAirHumidityRatio(const Quantity& minimumCoolingSupplyAirHumidityRatio) {
    OptionalDouble value = getDoubleFromQuantity(OS_ZoneHVAC_IdealLoadsAirSystemFields::MinimumCoolingSupplyAirHumidityRatio,minimumCoolingSupplyAirHumidityRatio);
    if (!value) {
      return false;
    }
    return setMinimumCoolingSupplyAirHumidityRatio(value.get());
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::resetMinimumCoolingSupplyAirHumidityRatio() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::MinimumCoolingSupplyAirHumidityRatio, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setHeatingLimit(std::string heatingLimit) {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::HeatingLimit, heatingLimit);
    return result;
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::resetHeatingLimit() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::HeatingLimit, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setMaximumHeatingAirFlowRate(boost::optional<double> maximumHeatingAirFlowRate) {
    bool result = false;
    if (maximumHeatingAirFlowRate) {
      result = setDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingAirFlowRate, maximumHeatingAirFlowRate.get());
    } else {
      result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingAirFlowRate, "");
    }
    return result;
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setMaximumHeatingAirFlowRate(const OSOptionalQuantity& maximumHeatingAirFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (maximumHeatingAirFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingAirFlowRate,maximumHeatingAirFlowRate.get());
      if (value) {
        result = setMaximumHeatingAirFlowRate(value);
      }
    }
    else {
      result = setMaximumHeatingAirFlowRate(value);
    }
    return result;
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::resetMaximumHeatingAirFlowRate() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingAirFlowRate, "");
    OS_ASSERT(result);
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::autosizeMaximumHeatingAirFlowRate() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setMaximumSensibleHeatingCapacity(boost::optional<double> maximumSensibleHeatingCapacity) {
    bool result = false;
    if (maximumSensibleHeatingCapacity) {
      result = setDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumSensibleHeatingCapacity, maximumSensibleHeatingCapacity.get());
    } else {
      result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumSensibleHeatingCapacity, "");
    }
    return result;
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setMaximumSensibleHeatingCapacity(const OSOptionalQuantity& maximumSensibleHeatingCapacity) {
    bool result(false);
    OptionalDouble value;
    if (maximumSensibleHeatingCapacity.isSet()) {
      value = getDoubleFromQuantity(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumSensibleHeatingCapacity,maximumSensibleHeatingCapacity.get());
      if (value) {
        result = setMaximumSensibleHeatingCapacity(value);
      }
    }
    else {
      result = setMaximumSensibleHeatingCapacity(value);
    }
    return result;
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::resetMaximumSensibleHeatingCapacity() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumSensibleHeatingCapacity, "");
    OS_ASSERT(result);
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::autosizeMaximumSensibleHeatingCapacity() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumSensibleHeatingCapacity, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setCoolingLimit(std::string coolingLimit) {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::CoolingLimit, coolingLimit);
    return result;
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::resetCoolingLimit() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::CoolingLimit, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setMaximumCoolingAirFlowRate(boost::optional<double> maximumCoolingAirFlowRate) {
    bool result = false;
    if (maximumCoolingAirFlowRate) {
      result = setDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumCoolingAirFlowRate, maximumCoolingAirFlowRate.get());
    } else {
      result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumCoolingAirFlowRate, "");
    }
    return result;
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setMaximumCoolingAirFlowRate(const OSOptionalQuantity& maximumCoolingAirFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (maximumCoolingAirFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumCoolingAirFlowRate,maximumCoolingAirFlowRate.get());
      if (value) {
        result = setMaximumCoolingAirFlowRate(value);
      }
    }
    else {
      result = setMaximumCoolingAirFlowRate(value);
    }
    return result;
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::resetMaximumCoolingAirFlowRate() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumCoolingAirFlowRate, "");
    OS_ASSERT(result);
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::autosizeMaximumCoolingAirFlowRate() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumCoolingAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setMaximumTotalCoolingCapacity(boost::optional<double> maximumTotalCoolingCapacity) {
    bool result = false;
    if (maximumTotalCoolingCapacity) {
      result = setDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumTotalCoolingCapacity, maximumTotalCoolingCapacity.get());
    } else {
      result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumTotalCoolingCapacity, "");
    }
    return result;
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setMaximumTotalCoolingCapacity(const OSOptionalQuantity& maximumTotalCoolingCapacity) {
    bool result(false);
    OptionalDouble value;
    if (maximumTotalCoolingCapacity.isSet()) {
      value = getDoubleFromQuantity(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumTotalCoolingCapacity,maximumTotalCoolingCapacity.get());
      if (value) {
        result = setMaximumTotalCoolingCapacity(value);
      }
    }
    else {
      result = setMaximumTotalCoolingCapacity(value);
    }
    return result;
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::resetMaximumTotalCoolingCapacity() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumTotalCoolingCapacity, "");
    OS_ASSERT(result);
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::autosizeMaximumTotalCoolingCapacity() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::MaximumTotalCoolingCapacity, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setHeatingAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_IdealLoadsAirSystemFields::HeatingAvailabilityScheduleName,
                              "ZoneHVACIdealLoadsAirSystem",
                              "Heating Availability",
                              schedule);
    return result;
  }


  void ZoneHVACIdealLoadsAirSystem_Impl::resetHeatingAvailabilitySchedule() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::HeatingAvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setCoolingAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_IdealLoadsAirSystemFields::CoolingAvailabilityScheduleName,
                              "ZoneHVACIdealLoadsAirSystem",
                              "Cooling Availability",
                              schedule);
    return result;
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::resetCoolingAvailabilitySchedule() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::CoolingAvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setDehumidificationControlType(std::string dehumidificationControlType) {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::DehumidificationControlType, dehumidificationControlType);
    return result;
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::resetDehumidificationControlType() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::DehumidificationControlType, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setCoolingSensibleHeatRatio(double coolingSensibleHeatRatio) {
    bool result = setDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::CoolingSensibleHeatRatio, coolingSensibleHeatRatio);
    return result;
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setCoolingSensibleHeatRatio(const Quantity& coolingSensibleHeatRatio) {
    OptionalDouble value = getDoubleFromQuantity(OS_ZoneHVAC_IdealLoadsAirSystemFields::CoolingSensibleHeatRatio,coolingSensibleHeatRatio);
    if (!value) {
      return false;
    }
    return setCoolingSensibleHeatRatio(value.get());
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::resetCoolingSensibleHeatRatio() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::CoolingSensibleHeatRatio, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setHumidificationControlType(std::string humidificationControlType) {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::HumidificationControlType, humidificationControlType);
    return result;
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::resetHumidificationControlType() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::HumidificationControlType, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setDesignSpecificationOutdoorAirObject(const boost::optional<DesignSpecificationOutdoorAir>& designSpecificationOutdoorAirObject) {
    bool result = false;
    if (designSpecificationOutdoorAirObject) {
      result = setPointer(OS_ZoneHVAC_IdealLoadsAirSystemFields::DesignSpecificationOutdoorAirObjectName, designSpecificationOutdoorAirObject.get().handle());
    } else {
      result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::DesignSpecificationOutdoorAirObjectName, "");
    }
    return result;
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::resetDesignSpecificationOutdoorAirObject() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::DesignSpecificationOutdoorAirObjectName, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setDemandControlledVentilationType(std::string demandControlledVentilationType) {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::DemandControlledVentilationType, demandControlledVentilationType);
    return result;
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::resetDemandControlledVentilationType() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::DemandControlledVentilationType, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setOutdoorAirEconomizerType(std::string outdoorAirEconomizerType) {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::OutdoorAirEconomizerType, outdoorAirEconomizerType);
    return result;
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::resetOutdoorAirEconomizerType() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::OutdoorAirEconomizerType, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setHeatRecoveryType(std::string heatRecoveryType) {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::HeatRecoveryType, heatRecoveryType);
    return result;
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::resetHeatRecoveryType() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::HeatRecoveryType, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setSensibleHeatRecoveryEffectiveness(double sensibleHeatRecoveryEffectiveness) {
    bool result = setDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::SensibleHeatRecoveryEffectiveness, sensibleHeatRecoveryEffectiveness);
    return result;
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setSensibleHeatRecoveryEffectiveness(const Quantity& sensibleHeatRecoveryEffectiveness) {
    OptionalDouble value = getDoubleFromQuantity(OS_ZoneHVAC_IdealLoadsAirSystemFields::SensibleHeatRecoveryEffectiveness,sensibleHeatRecoveryEffectiveness);
    if (!value) {
      return false;
    }
    return setSensibleHeatRecoveryEffectiveness(value.get());
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::resetSensibleHeatRecoveryEffectiveness() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::SensibleHeatRecoveryEffectiveness, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setLatentHeatRecoveryEffectiveness(double latentHeatRecoveryEffectiveness) {
    bool result = setDouble(OS_ZoneHVAC_IdealLoadsAirSystemFields::LatentHeatRecoveryEffectiveness, latentHeatRecoveryEffectiveness);
    return result;
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setLatentHeatRecoveryEffectiveness(const Quantity& latentHeatRecoveryEffectiveness) {
    OptionalDouble value = getDoubleFromQuantity(OS_ZoneHVAC_IdealLoadsAirSystemFields::LatentHeatRecoveryEffectiveness,latentHeatRecoveryEffectiveness);
    if (!value) {
      return false;
    }
    return setLatentHeatRecoveryEffectiveness(value.get());
  }

  void ZoneHVACIdealLoadsAirSystem_Impl::resetLatentHeatRecoveryEffectiveness() {
    bool result = setString(OS_ZoneHVAC_IdealLoadsAirSystemFields::LatentHeatRecoveryEffectiveness, "");
    OS_ASSERT(result);
  }

  openstudio::Quantity ZoneHVACIdealLoadsAirSystem_Impl::maximumHeatingSupplyAirTemperature_SI() const {
    return getMaximumHeatingSupplyAirTemperature(false);
  }

  openstudio::Quantity ZoneHVACIdealLoadsAirSystem_Impl::maximumHeatingSupplyAirTemperature_IP() const {
    return getMaximumHeatingSupplyAirTemperature(true);
  }

  openstudio::Quantity ZoneHVACIdealLoadsAirSystem_Impl::minimumCoolingSupplyAirTemperature_SI() const {
    return getMinimumCoolingSupplyAirTemperature(false);
  }

  openstudio::Quantity ZoneHVACIdealLoadsAirSystem_Impl::minimumCoolingSupplyAirTemperature_IP() const {
    return getMinimumCoolingSupplyAirTemperature(true);
  }

  openstudio::Quantity ZoneHVACIdealLoadsAirSystem_Impl::maximumHeatingSupplyAirHumidityRatio_SI() const {
    return getMaximumHeatingSupplyAirHumidityRatio(false);
  }

  openstudio::Quantity ZoneHVACIdealLoadsAirSystem_Impl::maximumHeatingSupplyAirHumidityRatio_IP() const {
    return getMaximumHeatingSupplyAirHumidityRatio(true);
  }

  openstudio::Quantity ZoneHVACIdealLoadsAirSystem_Impl::minimumCoolingSupplyAirHumidityRatio_SI() const {
    return getMinimumCoolingSupplyAirHumidityRatio(false);
  }

  openstudio::Quantity ZoneHVACIdealLoadsAirSystem_Impl::minimumCoolingSupplyAirHumidityRatio_IP() const {
    return getMinimumCoolingSupplyAirHumidityRatio(true);
  }

  std::vector<std::string> ZoneHVACIdealLoadsAirSystem_Impl::heatingLimitValues() const {
    return ZoneHVACIdealLoadsAirSystem::heatingLimitValues();
  }

  openstudio::OSOptionalQuantity ZoneHVACIdealLoadsAirSystem_Impl::maximumHeatingAirFlowRate_SI() const {
    return getMaximumHeatingAirFlowRate(false);
  }

  openstudio::OSOptionalQuantity ZoneHVACIdealLoadsAirSystem_Impl::maximumHeatingAirFlowRate_IP() const {
    return getMaximumHeatingAirFlowRate(true);
  }

  openstudio::OSOptionalQuantity ZoneHVACIdealLoadsAirSystem_Impl::maximumSensibleHeatingCapacity_SI() const {
    return getMaximumSensibleHeatingCapacity(false);
  }

  openstudio::OSOptionalQuantity ZoneHVACIdealLoadsAirSystem_Impl::maximumSensibleHeatingCapacity_IP() const {
    return getMaximumSensibleHeatingCapacity(true);
  }

  std::vector<std::string> ZoneHVACIdealLoadsAirSystem_Impl::coolingLimitValues() const {
    return ZoneHVACIdealLoadsAirSystem::coolingLimitValues();
  }

  openstudio::OSOptionalQuantity ZoneHVACIdealLoadsAirSystem_Impl::maximumCoolingAirFlowRate_SI() const {
    return getMaximumCoolingAirFlowRate(false);
  }

  openstudio::OSOptionalQuantity ZoneHVACIdealLoadsAirSystem_Impl::maximumCoolingAirFlowRate_IP() const {
    return getMaximumCoolingAirFlowRate(true);
  }

  openstudio::OSOptionalQuantity ZoneHVACIdealLoadsAirSystem_Impl::maximumTotalCoolingCapacity_SI() const {
    return getMaximumTotalCoolingCapacity(false);
  }

  openstudio::OSOptionalQuantity ZoneHVACIdealLoadsAirSystem_Impl::maximumTotalCoolingCapacity_IP() const {
    return getMaximumTotalCoolingCapacity(true);
  }

  std::vector<std::string> ZoneHVACIdealLoadsAirSystem_Impl::dehumidificationControlTypeValues() const {
    return ZoneHVACIdealLoadsAirSystem::dehumidificationControlTypeValues();
  }

  openstudio::Quantity ZoneHVACIdealLoadsAirSystem_Impl::coolingSensibleHeatRatio_SI() const {
    return getCoolingSensibleHeatRatio(false);
  }

  openstudio::Quantity ZoneHVACIdealLoadsAirSystem_Impl::coolingSensibleHeatRatio_IP() const {
    return getCoolingSensibleHeatRatio(true);
  }

  std::vector<std::string> ZoneHVACIdealLoadsAirSystem_Impl::humidificationControlTypeValues() const {
    return ZoneHVACIdealLoadsAirSystem::humidificationControlTypeValues();
  }

  std::vector<std::string> ZoneHVACIdealLoadsAirSystem_Impl::demandControlledVentilationTypeValues() const {
    return ZoneHVACIdealLoadsAirSystem::demandControlledVentilationTypeValues();
  }

  std::vector<std::string> ZoneHVACIdealLoadsAirSystem_Impl::outdoorAirEconomizerTypeValues() const {
    return ZoneHVACIdealLoadsAirSystem::outdoorAirEconomizerTypeValues();
  }

  std::vector<std::string> ZoneHVACIdealLoadsAirSystem_Impl::heatRecoveryTypeValues() const {
    return ZoneHVACIdealLoadsAirSystem::heatRecoveryTypeValues();
  }

  openstudio::Quantity ZoneHVACIdealLoadsAirSystem_Impl::sensibleHeatRecoveryEffectiveness_SI() const {
    return getSensibleHeatRecoveryEffectiveness(false);
  }

  openstudio::Quantity ZoneHVACIdealLoadsAirSystem_Impl::sensibleHeatRecoveryEffectiveness_IP() const {
    return getSensibleHeatRecoveryEffectiveness(true);
  }

  openstudio::Quantity ZoneHVACIdealLoadsAirSystem_Impl::latentHeatRecoveryEffectiveness_SI() const {
    return getLatentHeatRecoveryEffectiveness(false);
  }

  openstudio::Quantity ZoneHVACIdealLoadsAirSystem_Impl::latentHeatRecoveryEffectiveness_IP() const {
    return getLatentHeatRecoveryEffectiveness(true);
  }

  boost::optional<ModelObject> ZoneHVACIdealLoadsAirSystem_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = availabilitySchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> ZoneHVACIdealLoadsAirSystem_Impl::heatingAvailabilityScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = heatingAvailabilitySchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> ZoneHVACIdealLoadsAirSystem_Impl::coolingAvailabilityScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = coolingAvailabilitySchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> ZoneHVACIdealLoadsAirSystem_Impl::designSpecificationOutdoorAirObjectAsModelObject() const {
    OptionalModelObject result;
    OptionalDesignSpecificationOutdoorAir intermediate = designSpecificationOutdoorAirObject();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetAvailabilitySchedule();
    }
    return true;
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setHeatingAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setHeatingAvailabilitySchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetHeatingAvailabilitySchedule();
    }
    return true;
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setCoolingAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setCoolingAvailabilitySchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetCoolingAvailabilitySchedule();
    }
    return true;
  }

  bool ZoneHVACIdealLoadsAirSystem_Impl::setDesignSpecificationOutdoorAirObjectAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalDesignSpecificationOutdoorAir intermediate = modelObject->optionalCast<DesignSpecificationOutdoorAir>();
      if (intermediate) {
        return setDesignSpecificationOutdoorAirObject(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetDesignSpecificationOutdoorAirObject();
    }
    return true;
  }

} // detail

ZoneHVACIdealLoadsAirSystem::ZoneHVACIdealLoadsAirSystem(const Model& model)
  : ZoneHVACComponent(ZoneHVACIdealLoadsAirSystem::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>());
}

IddObjectType ZoneHVACIdealLoadsAirSystem::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneHVAC_IdealLoadsAirSystem);
}

std::vector<std::string> ZoneHVACIdealLoadsAirSystem::heatingLimitValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_IdealLoadsAirSystemFields::HeatingLimit);
}

std::vector<std::string> ZoneHVACIdealLoadsAirSystem::coolingLimitValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_IdealLoadsAirSystemFields::CoolingLimit);
}

std::vector<std::string> ZoneHVACIdealLoadsAirSystem::dehumidificationControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_IdealLoadsAirSystemFields::DehumidificationControlType);
}

std::vector<std::string> ZoneHVACIdealLoadsAirSystem::humidificationControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_IdealLoadsAirSystemFields::HumidificationControlType);
}

std::vector<std::string> ZoneHVACIdealLoadsAirSystem::demandControlledVentilationTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_IdealLoadsAirSystemFields::DemandControlledVentilationType);
}

std::vector<std::string> ZoneHVACIdealLoadsAirSystem::outdoorAirEconomizerTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_IdealLoadsAirSystemFields::OutdoorAirEconomizerType);
}

std::vector<std::string> ZoneHVACIdealLoadsAirSystem::heatRecoveryTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_IdealLoadsAirSystemFields::HeatRecoveryType);
}

boost::optional<Schedule> ZoneHVACIdealLoadsAirSystem::availabilitySchedule() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->availabilitySchedule();
}

double ZoneHVACIdealLoadsAirSystem::maximumHeatingSupplyAirTemperature() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->maximumHeatingSupplyAirTemperature();
}

Quantity ZoneHVACIdealLoadsAirSystem::getMaximumHeatingSupplyAirTemperature(bool returnIP) const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->getMaximumHeatingSupplyAirTemperature(returnIP);
}

bool ZoneHVACIdealLoadsAirSystem::isMaximumHeatingSupplyAirTemperatureDefaulted() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->isMaximumHeatingSupplyAirTemperatureDefaulted();
}

double ZoneHVACIdealLoadsAirSystem::minimumCoolingSupplyAirTemperature() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->minimumCoolingSupplyAirTemperature();
}

Quantity ZoneHVACIdealLoadsAirSystem::getMinimumCoolingSupplyAirTemperature(bool returnIP) const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->getMinimumCoolingSupplyAirTemperature(returnIP);
}

bool ZoneHVACIdealLoadsAirSystem::isMinimumCoolingSupplyAirTemperatureDefaulted() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->isMinimumCoolingSupplyAirTemperatureDefaulted();
}

double ZoneHVACIdealLoadsAirSystem::maximumHeatingSupplyAirHumidityRatio() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->maximumHeatingSupplyAirHumidityRatio();
}

Quantity ZoneHVACIdealLoadsAirSystem::getMaximumHeatingSupplyAirHumidityRatio(bool returnIP) const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->getMaximumHeatingSupplyAirHumidityRatio(returnIP);
}

bool ZoneHVACIdealLoadsAirSystem::isMaximumHeatingSupplyAirHumidityRatioDefaulted() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->isMaximumHeatingSupplyAirHumidityRatioDefaulted();
}

double ZoneHVACIdealLoadsAirSystem::minimumCoolingSupplyAirHumidityRatio() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->minimumCoolingSupplyAirHumidityRatio();
}

Quantity ZoneHVACIdealLoadsAirSystem::getMinimumCoolingSupplyAirHumidityRatio(bool returnIP) const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->getMinimumCoolingSupplyAirHumidityRatio(returnIP);
}

bool ZoneHVACIdealLoadsAirSystem::isMinimumCoolingSupplyAirHumidityRatioDefaulted() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->isMinimumCoolingSupplyAirHumidityRatioDefaulted();
}

std::string ZoneHVACIdealLoadsAirSystem::heatingLimit() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->heatingLimit();
}

bool ZoneHVACIdealLoadsAirSystem::isHeatingLimitDefaulted() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->isHeatingLimitDefaulted();
}

boost::optional<double> ZoneHVACIdealLoadsAirSystem::maximumHeatingAirFlowRate() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->maximumHeatingAirFlowRate();
}

OSOptionalQuantity ZoneHVACIdealLoadsAirSystem::getMaximumHeatingAirFlowRate(bool returnIP) const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->getMaximumHeatingAirFlowRate(returnIP);
}

bool ZoneHVACIdealLoadsAirSystem::isMaximumHeatingAirFlowRateAutosized() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->isMaximumHeatingAirFlowRateAutosized();
}

boost::optional<double> ZoneHVACIdealLoadsAirSystem::maximumSensibleHeatingCapacity() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->maximumSensibleHeatingCapacity();
}

OSOptionalQuantity ZoneHVACIdealLoadsAirSystem::getMaximumSensibleHeatingCapacity(bool returnIP) const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->getMaximumSensibleHeatingCapacity(returnIP);
}

bool ZoneHVACIdealLoadsAirSystem::isMaximumSensibleHeatingCapacityAutosized() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->isMaximumSensibleHeatingCapacityAutosized();
}

std::string ZoneHVACIdealLoadsAirSystem::coolingLimit() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->coolingLimit();
}

bool ZoneHVACIdealLoadsAirSystem::isCoolingLimitDefaulted() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->isCoolingLimitDefaulted();
}

boost::optional<double> ZoneHVACIdealLoadsAirSystem::maximumCoolingAirFlowRate() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->maximumCoolingAirFlowRate();
}

OSOptionalQuantity ZoneHVACIdealLoadsAirSystem::getMaximumCoolingAirFlowRate(bool returnIP) const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->getMaximumCoolingAirFlowRate(returnIP);
}

bool ZoneHVACIdealLoadsAirSystem::isMaximumCoolingAirFlowRateAutosized() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->isMaximumCoolingAirFlowRateAutosized();
}

boost::optional<double> ZoneHVACIdealLoadsAirSystem::maximumTotalCoolingCapacity() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->maximumTotalCoolingCapacity();
}

OSOptionalQuantity ZoneHVACIdealLoadsAirSystem::getMaximumTotalCoolingCapacity(bool returnIP) const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->getMaximumTotalCoolingCapacity(returnIP);
}

bool ZoneHVACIdealLoadsAirSystem::isMaximumTotalCoolingCapacityAutosized() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->isMaximumTotalCoolingCapacityAutosized();
}

boost::optional<Schedule> ZoneHVACIdealLoadsAirSystem::heatingAvailabilitySchedule() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->heatingAvailabilitySchedule();
}

boost::optional<Schedule> ZoneHVACIdealLoadsAirSystem::coolingAvailabilitySchedule() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->coolingAvailabilitySchedule();
}

std::string ZoneHVACIdealLoadsAirSystem::dehumidificationControlType() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->dehumidificationControlType();
}

bool ZoneHVACIdealLoadsAirSystem::isDehumidificationControlTypeDefaulted() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->isDehumidificationControlTypeDefaulted();
}

double ZoneHVACIdealLoadsAirSystem::coolingSensibleHeatRatio() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->coolingSensibleHeatRatio();
}

Quantity ZoneHVACIdealLoadsAirSystem::getCoolingSensibleHeatRatio(bool returnIP) const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->getCoolingSensibleHeatRatio(returnIP);
}

bool ZoneHVACIdealLoadsAirSystem::isCoolingSensibleHeatRatioDefaulted() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->isCoolingSensibleHeatRatioDefaulted();
}

std::string ZoneHVACIdealLoadsAirSystem::humidificationControlType() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->humidificationControlType();
}

bool ZoneHVACIdealLoadsAirSystem::isHumidificationControlTypeDefaulted() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->isHumidificationControlTypeDefaulted();
}

boost::optional<DesignSpecificationOutdoorAir> ZoneHVACIdealLoadsAirSystem::designSpecificationOutdoorAirObject() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->designSpecificationOutdoorAirObject();
}

std::string ZoneHVACIdealLoadsAirSystem::demandControlledVentilationType() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->demandControlledVentilationType();
}

bool ZoneHVACIdealLoadsAirSystem::isDemandControlledVentilationTypeDefaulted() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->isDemandControlledVentilationTypeDefaulted();
}

std::string ZoneHVACIdealLoadsAirSystem::outdoorAirEconomizerType() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->outdoorAirEconomizerType();
}

bool ZoneHVACIdealLoadsAirSystem::isOutdoorAirEconomizerTypeDefaulted() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->isOutdoorAirEconomizerTypeDefaulted();
}

std::string ZoneHVACIdealLoadsAirSystem::heatRecoveryType() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->heatRecoveryType();
}

bool ZoneHVACIdealLoadsAirSystem::isHeatRecoveryTypeDefaulted() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->isHeatRecoveryTypeDefaulted();
}

double ZoneHVACIdealLoadsAirSystem::sensibleHeatRecoveryEffectiveness() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->sensibleHeatRecoveryEffectiveness();
}

Quantity ZoneHVACIdealLoadsAirSystem::getSensibleHeatRecoveryEffectiveness(bool returnIP) const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->getSensibleHeatRecoveryEffectiveness(returnIP);
}

bool ZoneHVACIdealLoadsAirSystem::isSensibleHeatRecoveryEffectivenessDefaulted() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->isSensibleHeatRecoveryEffectivenessDefaulted();
}

double ZoneHVACIdealLoadsAirSystem::latentHeatRecoveryEffectiveness() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->latentHeatRecoveryEffectiveness();
}

Quantity ZoneHVACIdealLoadsAirSystem::getLatentHeatRecoveryEffectiveness(bool returnIP) const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->getLatentHeatRecoveryEffectiveness(returnIP);
}

bool ZoneHVACIdealLoadsAirSystem::isLatentHeatRecoveryEffectivenessDefaulted() const {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->isLatentHeatRecoveryEffectivenessDefaulted();
}

bool ZoneHVACIdealLoadsAirSystem::setAvailabilitySchedule(Schedule& availabilitySchedule) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setAvailabilitySchedule(availabilitySchedule);
}

void ZoneHVACIdealLoadsAirSystem::resetAvailabilitySchedule() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->resetAvailabilitySchedule();
}

bool ZoneHVACIdealLoadsAirSystem::setMaximumHeatingSupplyAirTemperature(double maximumHeatingSupplyAirTemperature) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setMaximumHeatingSupplyAirTemperature(maximumHeatingSupplyAirTemperature);
}

bool ZoneHVACIdealLoadsAirSystem::setMaximumHeatingSupplyAirTemperature(const Quantity& maximumHeatingSupplyAirTemperature) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setMaximumHeatingSupplyAirTemperature(maximumHeatingSupplyAirTemperature);
}

void ZoneHVACIdealLoadsAirSystem::resetMaximumHeatingSupplyAirTemperature() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->resetMaximumHeatingSupplyAirTemperature();
}

bool ZoneHVACIdealLoadsAirSystem::setMinimumCoolingSupplyAirTemperature(double minimumCoolingSupplyAirTemperature) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setMinimumCoolingSupplyAirTemperature(minimumCoolingSupplyAirTemperature);
}

bool ZoneHVACIdealLoadsAirSystem::setMinimumCoolingSupplyAirTemperature(const Quantity& minimumCoolingSupplyAirTemperature) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setMinimumCoolingSupplyAirTemperature(minimumCoolingSupplyAirTemperature);
}

void ZoneHVACIdealLoadsAirSystem::resetMinimumCoolingSupplyAirTemperature() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->resetMinimumCoolingSupplyAirTemperature();
}

bool ZoneHVACIdealLoadsAirSystem::setMaximumHeatingSupplyAirHumidityRatio(double maximumHeatingSupplyAirHumidityRatio) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setMaximumHeatingSupplyAirHumidityRatio(maximumHeatingSupplyAirHumidityRatio);
}

bool ZoneHVACIdealLoadsAirSystem::setMaximumHeatingSupplyAirHumidityRatio(const Quantity& maximumHeatingSupplyAirHumidityRatio) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setMaximumHeatingSupplyAirHumidityRatio(maximumHeatingSupplyAirHumidityRatio);
}

void ZoneHVACIdealLoadsAirSystem::resetMaximumHeatingSupplyAirHumidityRatio() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->resetMaximumHeatingSupplyAirHumidityRatio();
}

bool ZoneHVACIdealLoadsAirSystem::setMinimumCoolingSupplyAirHumidityRatio(double minimumCoolingSupplyAirHumidityRatio) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setMinimumCoolingSupplyAirHumidityRatio(minimumCoolingSupplyAirHumidityRatio);
}

bool ZoneHVACIdealLoadsAirSystem::setMinimumCoolingSupplyAirHumidityRatio(const Quantity& minimumCoolingSupplyAirHumidityRatio) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setMinimumCoolingSupplyAirHumidityRatio(minimumCoolingSupplyAirHumidityRatio);
}

void ZoneHVACIdealLoadsAirSystem::resetMinimumCoolingSupplyAirHumidityRatio() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->resetMinimumCoolingSupplyAirHumidityRatio();
}

bool ZoneHVACIdealLoadsAirSystem::setHeatingLimit(std::string heatingLimit) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setHeatingLimit(heatingLimit);
}

void ZoneHVACIdealLoadsAirSystem::resetHeatingLimit() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->resetHeatingLimit();
}

bool ZoneHVACIdealLoadsAirSystem::setMaximumHeatingAirFlowRate(double maximumHeatingAirFlowRate) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setMaximumHeatingAirFlowRate(maximumHeatingAirFlowRate);
}

bool ZoneHVACIdealLoadsAirSystem::setMaximumHeatingAirFlowRate(const Quantity& maximumHeatingAirFlowRate) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setMaximumHeatingAirFlowRate(maximumHeatingAirFlowRate);
}

void ZoneHVACIdealLoadsAirSystem::resetMaximumHeatingAirFlowRate() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->resetMaximumHeatingAirFlowRate();
}

void ZoneHVACIdealLoadsAirSystem::autosizeMaximumHeatingAirFlowRate() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->autosizeMaximumHeatingAirFlowRate();
}

bool ZoneHVACIdealLoadsAirSystem::setMaximumSensibleHeatingCapacity(double maximumSensibleHeatingCapacity) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setMaximumSensibleHeatingCapacity(maximumSensibleHeatingCapacity);
}

bool ZoneHVACIdealLoadsAirSystem::setMaximumSensibleHeatingCapacity(const Quantity& maximumSensibleHeatingCapacity) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setMaximumSensibleHeatingCapacity(maximumSensibleHeatingCapacity);
}

void ZoneHVACIdealLoadsAirSystem::resetMaximumSensibleHeatingCapacity() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->resetMaximumSensibleHeatingCapacity();
}

void ZoneHVACIdealLoadsAirSystem::autosizeMaximumSensibleHeatingCapacity() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->autosizeMaximumSensibleHeatingCapacity();
}

bool ZoneHVACIdealLoadsAirSystem::setCoolingLimit(std::string coolingLimit) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setCoolingLimit(coolingLimit);
}

void ZoneHVACIdealLoadsAirSystem::resetCoolingLimit() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->resetCoolingLimit();
}

bool ZoneHVACIdealLoadsAirSystem::setMaximumCoolingAirFlowRate(double maximumCoolingAirFlowRate) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setMaximumCoolingAirFlowRate(maximumCoolingAirFlowRate);
}

bool ZoneHVACIdealLoadsAirSystem::setMaximumCoolingAirFlowRate(const Quantity& maximumCoolingAirFlowRate) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setMaximumCoolingAirFlowRate(maximumCoolingAirFlowRate);
}

void ZoneHVACIdealLoadsAirSystem::resetMaximumCoolingAirFlowRate() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->resetMaximumCoolingAirFlowRate();
}

void ZoneHVACIdealLoadsAirSystem::autosizeMaximumCoolingAirFlowRate() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->autosizeMaximumCoolingAirFlowRate();
}

bool ZoneHVACIdealLoadsAirSystem::setMaximumTotalCoolingCapacity(double maximumTotalCoolingCapacity) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setMaximumTotalCoolingCapacity(maximumTotalCoolingCapacity);
}

bool ZoneHVACIdealLoadsAirSystem::setMaximumTotalCoolingCapacity(const Quantity& maximumTotalCoolingCapacity) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setMaximumTotalCoolingCapacity(maximumTotalCoolingCapacity);
}

void ZoneHVACIdealLoadsAirSystem::resetMaximumTotalCoolingCapacity() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->resetMaximumTotalCoolingCapacity();
}

void ZoneHVACIdealLoadsAirSystem::autosizeMaximumTotalCoolingCapacity() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->autosizeMaximumTotalCoolingCapacity();
}

bool ZoneHVACIdealLoadsAirSystem::setHeatingAvailabilitySchedule(Schedule& heatingAvailabilitySchedule) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setHeatingAvailabilitySchedule(heatingAvailabilitySchedule);
}

void ZoneHVACIdealLoadsAirSystem::resetHeatingAvailabilitySchedule() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->resetHeatingAvailabilitySchedule();
}

bool ZoneHVACIdealLoadsAirSystem::setCoolingAvailabilitySchedule(Schedule& coolingAvailabilitySchedule) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setCoolingAvailabilitySchedule(coolingAvailabilitySchedule);
}

void ZoneHVACIdealLoadsAirSystem::resetCoolingAvailabilitySchedule() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->resetCoolingAvailabilitySchedule();
}

bool ZoneHVACIdealLoadsAirSystem::setDehumidificationControlType(std::string dehumidificationControlType) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setDehumidificationControlType(dehumidificationControlType);
}

void ZoneHVACIdealLoadsAirSystem::resetDehumidificationControlType() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->resetDehumidificationControlType();
}

bool ZoneHVACIdealLoadsAirSystem::setCoolingSensibleHeatRatio(double coolingSensibleHeatRatio) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setCoolingSensibleHeatRatio(coolingSensibleHeatRatio);
}

bool ZoneHVACIdealLoadsAirSystem::setCoolingSensibleHeatRatio(const Quantity& coolingSensibleHeatRatio) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setCoolingSensibleHeatRatio(coolingSensibleHeatRatio);
}

void ZoneHVACIdealLoadsAirSystem::resetCoolingSensibleHeatRatio() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->resetCoolingSensibleHeatRatio();
}

bool ZoneHVACIdealLoadsAirSystem::setHumidificationControlType(std::string humidificationControlType) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setHumidificationControlType(humidificationControlType);
}

void ZoneHVACIdealLoadsAirSystem::resetHumidificationControlType() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->resetHumidificationControlType();
}

bool ZoneHVACIdealLoadsAirSystem::setDesignSpecificationOutdoorAirObject(const DesignSpecificationOutdoorAir& designSpecificationOutdoorAirObject) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setDesignSpecificationOutdoorAirObject(designSpecificationOutdoorAirObject);
}

void ZoneHVACIdealLoadsAirSystem::resetDesignSpecificationOutdoorAirObject() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->resetDesignSpecificationOutdoorAirObject();
}

bool ZoneHVACIdealLoadsAirSystem::setDemandControlledVentilationType(std::string demandControlledVentilationType) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setDemandControlledVentilationType(demandControlledVentilationType);
}

void ZoneHVACIdealLoadsAirSystem::resetDemandControlledVentilationType() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->resetDemandControlledVentilationType();
}

bool ZoneHVACIdealLoadsAirSystem::setOutdoorAirEconomizerType(std::string outdoorAirEconomizerType) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setOutdoorAirEconomizerType(outdoorAirEconomizerType);
}

void ZoneHVACIdealLoadsAirSystem::resetOutdoorAirEconomizerType() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->resetOutdoorAirEconomizerType();
}

bool ZoneHVACIdealLoadsAirSystem::setHeatRecoveryType(std::string heatRecoveryType) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setHeatRecoveryType(heatRecoveryType);
}

void ZoneHVACIdealLoadsAirSystem::resetHeatRecoveryType() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->resetHeatRecoveryType();
}

bool ZoneHVACIdealLoadsAirSystem::setSensibleHeatRecoveryEffectiveness(double sensibleHeatRecoveryEffectiveness) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setSensibleHeatRecoveryEffectiveness(sensibleHeatRecoveryEffectiveness);
}

bool ZoneHVACIdealLoadsAirSystem::setSensibleHeatRecoveryEffectiveness(const Quantity& sensibleHeatRecoveryEffectiveness) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setSensibleHeatRecoveryEffectiveness(sensibleHeatRecoveryEffectiveness);
}

void ZoneHVACIdealLoadsAirSystem::resetSensibleHeatRecoveryEffectiveness() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->resetSensibleHeatRecoveryEffectiveness();
}

bool ZoneHVACIdealLoadsAirSystem::setLatentHeatRecoveryEffectiveness(double latentHeatRecoveryEffectiveness) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setLatentHeatRecoveryEffectiveness(latentHeatRecoveryEffectiveness);
}

bool ZoneHVACIdealLoadsAirSystem::setLatentHeatRecoveryEffectiveness(const Quantity& latentHeatRecoveryEffectiveness) {
  return getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->setLatentHeatRecoveryEffectiveness(latentHeatRecoveryEffectiveness);
}

void ZoneHVACIdealLoadsAirSystem::resetLatentHeatRecoveryEffectiveness() {
  getImpl<detail::ZoneHVACIdealLoadsAirSystem_Impl>()->resetLatentHeatRecoveryEffectiveness();
}

/// @cond
ZoneHVACIdealLoadsAirSystem::ZoneHVACIdealLoadsAirSystem(std::shared_ptr<detail::ZoneHVACIdealLoadsAirSystem_Impl> impl)
  : ZoneHVACComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

