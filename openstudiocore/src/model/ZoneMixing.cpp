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

#include "ZoneMixing.hpp"
#include "ZoneMixing_Impl.hpp"

#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "Model.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ZoneMixing_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneMixing_Impl::ZoneMixing_Impl(const IdfObject& idfObject,
                                   Model_Impl* model,
                                   bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneMixing::iddObjectType());
  }

  ZoneMixing_Impl::ZoneMixing_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneMixing::iddObjectType());
  }

  ZoneMixing_Impl::ZoneMixing_Impl(const ZoneMixing_Impl& other,
                                   Model_Impl* model,
                                   bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ZoneMixing_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
      result.push_back("Zone Mixing Volume");
      result.push_back("Zone Mixing Current Density Air Volume Flow Rate");
      result.push_back("Zone Mixing Standard Density Air Volume Flow Rate");
      result.push_back("Zone Mixing Mass");
      result.push_back("Zone Mixing Mass Flow Rate");
      result.push_back("Zone Mixing Sensible Heat Loss Energy");
      result.push_back("Zone Mixing Sensible Heat Gain Energy");
      result.push_back("Zone Mixing Latent Heat Loss Energy");
      result.push_back("Zone Mixing Latent Heat Gain Energy");
      result.push_back("Zone Mixing Total Heat Loss Energy");
      result.push_back("Zone Mixing Total Heat Gain Energy");
      // DLM: these are not working yet
      // https://github.com/NREL/EnergyPlus/issues/5042
      //result.push_back("Zone Mixing Receiving Air Mass Flow Rate");
      //result.push_back("Zone Mixing Source Air Mass Flow Rate");
    }
    return result;
  }

  IddObjectType ZoneMixing_Impl::iddObjectType() const {
    return ZoneMixing::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneMixing_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneMixingFields::ScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneMixing","Zone Mixing"));
    }
    if (std::find(b,e,OS_ZoneMixingFields::DeltaTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneMixing","Delta Temperature"));
    }
    if (std::find(b,e,OS_ZoneMixingFields::MinimumZoneTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneMixing","Minimum Zone Temperature"));
    }
    if (std::find(b,e,OS_ZoneMixingFields::MaximumZoneTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneMixing","Maximum Zone Temperature"));
    }
    if (std::find(b,e,OS_ZoneMixingFields::MinimumSourceZoneTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneMixing","Minimum Source Zone Temperature"));
    }
    if (std::find(b,e,OS_ZoneMixingFields::MaximumSourceZoneTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneMixing","Maximum Source Zone Temperature"));
    }
    if (std::find(b,e,OS_ZoneMixingFields::MinimumOutdoorTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneMixing","Minimum Outdoor Temperature"));
    }
    if (std::find(b,e,OS_ZoneMixingFields::MaximumOutdoorTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneMixing","Maximum Outdoor Temperature"));
    }
    return result;
  }

  ThermalZone ZoneMixing_Impl::zone() const {
    boost::optional<ThermalZone> value = getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_ZoneMixingFields::ZoneName);
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have a receiving ThermalZone.");
    }
    return value.get();
  }

  Schedule ZoneMixing_Impl::schedule() const {
    boost::optional<Schedule> value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneMixingFields::ScheduleName);
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Schedule attached.");
    }
    return value.get();
  }

  std::string ZoneMixing_Impl::designFlowRateCalculationMethod() const {
    boost::optional<std::string> value = getString(OS_ZoneMixingFields::DesignFlowRateCalculationMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> ZoneMixing_Impl::designFlowRate() const {
    return getDouble(OS_ZoneMixingFields::DesignFlowRate,true);
  }

  boost::optional<double> ZoneMixing_Impl::flowRateperZoneFloorArea() const {
    return getDouble(OS_ZoneMixingFields::FlowRateperZoneFloorArea,true);
  }

  boost::optional<double> ZoneMixing_Impl::flowRateperPerson() const {
    return getDouble(OS_ZoneMixingFields::FlowRateperPerson,true);
  }

  boost::optional<double> ZoneMixing_Impl::airChangesperHour() const {
    return getDouble(OS_ZoneMixingFields::AirChangesperHour,true);
  }

  boost::optional<ThermalZone> ZoneMixing_Impl::sourceZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_ZoneMixingFields::SourceZoneName);
  }

  boost::optional<double> ZoneMixing_Impl::deltaTemperature() const {
    return getDouble(OS_ZoneMixingFields::DeltaTemperature);
  }

  boost::optional<Schedule> ZoneMixing_Impl::deltaTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneMixingFields::DeltaTemperatureScheduleName);
  }

  boost::optional<Schedule> ZoneMixing_Impl::minimumZoneTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneMixingFields::MinimumZoneTemperatureScheduleName);
  }

  boost::optional<Schedule> ZoneMixing_Impl::maximumZoneTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneMixingFields::MaximumZoneTemperatureScheduleName);
  }

  boost::optional<Schedule> ZoneMixing_Impl::minimumSourceZoneTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneMixingFields::MinimumSourceZoneTemperatureScheduleName);
  }

  boost::optional<Schedule> ZoneMixing_Impl::maximumSourceZoneTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneMixingFields::MaximumSourceZoneTemperatureScheduleName);
  }

  boost::optional<Schedule> ZoneMixing_Impl::minimumOutdoorTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneMixingFields::MinimumOutdoorTemperatureScheduleName);
  }

  boost::optional<Schedule> ZoneMixing_Impl::maximumOutdoorTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneMixingFields::MaximumOutdoorTemperatureScheduleName);
  }

  bool ZoneMixing_Impl::setSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_ZoneMixingFields::ScheduleName,
                                                "ZoneMixing",
                                                "Zone Mixing",
                                                schedule);
    return result;
  }

  bool ZoneMixing_Impl::setDesignFlowRate(double designFlowRate) {
    bool result(false);
    result = setDouble(OS_ZoneMixingFields::DesignFlowRate, designFlowRate);
    if (result){
      result = setString(OS_ZoneMixingFields::DesignFlowRateCalculationMethod, "Flow/Zone");
      OS_ASSERT(result);
      //result = setString(OS_ZoneMixingFields::DesignFlowRate, "");
      //OS_ASSERT(result);
      result = setString(OS_ZoneMixingFields::FlowRateperZoneFloorArea, "");
      OS_ASSERT(result);
      result = setString(OS_ZoneMixingFields::FlowRateperPerson, "");
      OS_ASSERT(result);
      result = setString(OS_ZoneMixingFields::AirChangesperHour, "");
      OS_ASSERT(result);
    }
    return result;
  }

  bool ZoneMixing_Impl::setFlowRateperZoneFloorArea(double flowRateperZoneFloorArea) {
    bool result(false);
    result = setDouble(OS_ZoneMixingFields::FlowRateperZoneFloorArea, flowRateperZoneFloorArea);
    if (result){
      result = setString(OS_ZoneMixingFields::DesignFlowRateCalculationMethod, "Flow/Area");
      OS_ASSERT(result);
      result = setString(OS_ZoneMixingFields::DesignFlowRate, "");
      OS_ASSERT(result);
      //result = setString(OS_ZoneMixingFields::FlowRateperZoneFloorArea, "");
      //OS_ASSERT(result);
      result = setString(OS_ZoneMixingFields::FlowRateperPerson, "");
      OS_ASSERT(result);
      result = setString(OS_ZoneMixingFields::AirChangesperHour, "");
      OS_ASSERT(result);
    }
    return result;
  }

  bool ZoneMixing_Impl::setFlowRateperPerson(double flowRateperPerson) {
    bool result(false);
    result = setDouble(OS_ZoneMixingFields::FlowRateperPerson, flowRateperPerson);
    if (result){
      result = setString(OS_ZoneMixingFields::DesignFlowRateCalculationMethod, "Flow/Person");
      OS_ASSERT(result);
      result = setString(OS_ZoneMixingFields::DesignFlowRate, "");
      OS_ASSERT(result);
      result = setString(OS_ZoneMixingFields::FlowRateperZoneFloorArea, "");
      OS_ASSERT(result);
      //result = setString(OS_ZoneMixingFields::FlowRateperPerson, "");
      //OS_ASSERT(result);
      result = setString(OS_ZoneMixingFields::AirChangesperHour, "");
      OS_ASSERT(result);
    }
    return result;
  }

  bool ZoneMixing_Impl::setAirChangesperHour(double airChangesperHour) {
    bool result(false);
    result = setDouble(OS_ZoneMixingFields::AirChangesperHour, airChangesperHour);
    if (result){
      result = setString(OS_ZoneMixingFields::DesignFlowRateCalculationMethod, "AirChanges/Hour");
      OS_ASSERT(result);
      result = setString(OS_ZoneMixingFields::DesignFlowRate, "");
      OS_ASSERT(result);
      result = setString(OS_ZoneMixingFields::FlowRateperZoneFloorArea, "");
      OS_ASSERT(result);
      result = setString(OS_ZoneMixingFields::FlowRateperPerson, "");
      OS_ASSERT(result);
      //result = setString(OS_ZoneMixingFields::AirChangesperHour, "");
      //OS_ASSERT(result);
    }
    return result;
  }

  bool ZoneMixing_Impl::setSourceZone(const ThermalZone& zone) {
    bool result(false);

    // source zone cannot be the same as this zone
    if (zone.handle() != this->zone().handle()){
      result = setPointer(OS_ZoneMixingFields::SourceZoneName, zone.handle());
    }
    return result;
  }

  void ZoneMixing_Impl::resetSourceZone() {
    bool result = setString(OS_ZoneMixingFields::SourceZoneName, "");
    OS_ASSERT(result);
  }

  void ZoneMixing_Impl::setDeltaTemperature(double deltaTemperature) {
    bool result = setDouble(OS_ZoneMixingFields::DeltaTemperature, deltaTemperature);
    OS_ASSERT(result);
    resetDeltaTemperatureSchedule();
  }

  void ZoneMixing_Impl::resetDeltaTemperature() {
    bool result = setString(OS_ZoneMixingFields::DeltaTemperature, "");
    OS_ASSERT(result);
  }

  bool ZoneMixing_Impl::setDeltaTemperatureSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_ZoneMixingFields::DeltaTemperatureScheduleName,
                                                "ZoneMixing",
                                                "Delta Temperature",
                                                schedule);
    if (result){
      resetDeltaTemperature();
    }
    return result;
  }

  void ZoneMixing_Impl::resetDeltaTemperatureSchedule() {
    bool result = setString(OS_ZoneMixingFields::DeltaTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneMixing_Impl::setMinimumZoneTemperatureSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_ZoneMixingFields::MinimumZoneTemperatureScheduleName,
                                                "ZoneMixing",
                                                "Minimum Zone Temperature",
                                                schedule);
    return result;
  }

  void ZoneMixing_Impl::resetMinimumZoneTemperatureSchedule() {
    bool result = setString(OS_ZoneMixingFields::MinimumZoneTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneMixing_Impl::setMaximumZoneTemperatureSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_ZoneMixingFields::MaximumZoneTemperatureScheduleName,
                                                "ZoneMixing",
                                                "Maximum Zone Temperature",
                                                schedule);
    return result;
  }

  void ZoneMixing_Impl::resetMaximumZoneTemperatureSchedule() {
    bool result = setString(OS_ZoneMixingFields::MaximumZoneTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneMixing_Impl::setMinimumSourceZoneTemperatureSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_ZoneMixingFields::MinimumSourceZoneTemperatureScheduleName,
                                                "ZoneMixing",
                                                "Minimum Source Zone Temperature",
                                                schedule);
    return result;
  }

  void ZoneMixing_Impl::resetMinimumSourceZoneTemperatureSchedule() {
    bool result = setString(OS_ZoneMixingFields::MinimumSourceZoneTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneMixing_Impl::setMaximumSourceZoneTemperatureSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_ZoneMixingFields::MaximumSourceZoneTemperatureScheduleName,
                                                "ZoneMixing",
                                                "Maximum Source Zone Temperature",
                                                schedule);
    return result;
  }

  void ZoneMixing_Impl::resetMaximumSourceZoneTemperatureSchedule() {
    bool result = setString(OS_ZoneMixingFields::MaximumSourceZoneTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneMixing_Impl::setMinimumOutdoorTemperatureSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_ZoneMixingFields::MinimumOutdoorTemperatureScheduleName,
                                                "ZoneMixing",
                                                "Minimum Outdoor Temperature",
                                                schedule);
    return result;
  }

  void ZoneMixing_Impl::resetMinimumOutdoorTemperatureSchedule() {
    bool result = setString(OS_ZoneMixingFields::MinimumOutdoorTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneMixing_Impl::setMaximumOutdoorTemperatureSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_ZoneMixingFields::MaximumOutdoorTemperatureScheduleName,
                                                "ZoneMixing",
                                                "Maximum Outdoor Temperature",
                                                schedule);
    return result;
  }

  void ZoneMixing_Impl::resetMaximumOutdoorTemperatureSchedule() {
    bool result = setString(OS_ZoneMixingFields::MaximumOutdoorTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

} // detail

ZoneMixing::ZoneMixing(const ThermalZone& zone)
  : ModelObject(ZoneMixing::iddObjectType(), zone.model())
{
  OS_ASSERT(getImpl<detail::ZoneMixing_Impl>());

  bool ok = setPointer(OS_ZoneMixingFields::ZoneName, zone.handle());
  OS_ASSERT(ok);

  ok = setPointer(OS_ZoneMixingFields::ScheduleName, zone.model().alwaysOnContinuousSchedule().handle());
  OS_ASSERT(ok);

  ok = setDesignFlowRate(0.0);
  OS_ASSERT(ok);
}

IddObjectType ZoneMixing::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneMixing);
}

ThermalZone ZoneMixing::zone() const {
  return getImpl<detail::ZoneMixing_Impl>()->zone();
}

Schedule ZoneMixing::schedule() const {
  return getImpl<detail::ZoneMixing_Impl>()->schedule();
}

std::string ZoneMixing::designFlowRateCalculationMethod() const {
  return getImpl<detail::ZoneMixing_Impl>()->designFlowRateCalculationMethod();
}

boost::optional<double> ZoneMixing::designFlowRate() const {
  return getImpl<detail::ZoneMixing_Impl>()->designFlowRate();
}

boost::optional<double> ZoneMixing::flowRateperZoneFloorArea() const {
  return getImpl<detail::ZoneMixing_Impl>()->flowRateperZoneFloorArea();
}

boost::optional<double> ZoneMixing::flowRateperPerson() const {
  return getImpl<detail::ZoneMixing_Impl>()->flowRateperPerson();
}

boost::optional<double> ZoneMixing::airChangesperHour() const {
  return getImpl<detail::ZoneMixing_Impl>()->airChangesperHour();
}

boost::optional<ThermalZone> ZoneMixing::sourceZone() const {
  return getImpl<detail::ZoneMixing_Impl>()->sourceZone();
}

boost::optional<double> ZoneMixing::deltaTemperature() const {
  return getImpl<detail::ZoneMixing_Impl>()->deltaTemperature();
}

boost::optional<Schedule> ZoneMixing::deltaTemperatureSchedule() const {
  return getImpl<detail::ZoneMixing_Impl>()->deltaTemperatureSchedule();
}

boost::optional<Schedule> ZoneMixing::minimumZoneTemperatureSchedule() const {
  return getImpl<detail::ZoneMixing_Impl>()->minimumZoneTemperatureSchedule();
}

boost::optional<Schedule> ZoneMixing::maximumZoneTemperatureSchedule() const {
  return getImpl<detail::ZoneMixing_Impl>()->maximumZoneTemperatureSchedule();
}

boost::optional<Schedule> ZoneMixing::minimumSourceZoneTemperatureSchedule() const {
  return getImpl<detail::ZoneMixing_Impl>()->minimumSourceZoneTemperatureSchedule();
}

boost::optional<Schedule> ZoneMixing::maximumSourceZoneTemperatureSchedule() const {
  return getImpl<detail::ZoneMixing_Impl>()->maximumSourceZoneTemperatureSchedule();
}

boost::optional<Schedule> ZoneMixing::minimumOutdoorTemperatureSchedule() const {
  return getImpl<detail::ZoneMixing_Impl>()->minimumOutdoorTemperatureSchedule();
}

boost::optional<Schedule> ZoneMixing::maximumOutdoorTemperatureSchedule() const {
  return getImpl<detail::ZoneMixing_Impl>()->maximumOutdoorTemperatureSchedule();
}

bool ZoneMixing::setSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneMixing_Impl>()->setSchedule(schedule);
}

bool ZoneMixing::setDesignFlowRate(double designFlowRate) {
  return getImpl<detail::ZoneMixing_Impl>()->setDesignFlowRate(designFlowRate);
}

bool ZoneMixing::setFlowRateperZoneFloorArea(double flowRateperZoneFloorArea) {
  return getImpl<detail::ZoneMixing_Impl>()->setFlowRateperZoneFloorArea(flowRateperZoneFloorArea);
}

bool ZoneMixing::setFlowRateperPerson(double flowRateperPerson) {
  return getImpl<detail::ZoneMixing_Impl>()->setFlowRateperPerson(flowRateperPerson);
}

bool ZoneMixing::setAirChangesperHour(double airChangesperHour) {
  return getImpl<detail::ZoneMixing_Impl>()->setAirChangesperHour(airChangesperHour);
}

bool ZoneMixing::setSourceZone(const ThermalZone& zone) {
  return getImpl<detail::ZoneMixing_Impl>()->setSourceZone(zone);
}

void ZoneMixing::resetSourceZone() {
  getImpl<detail::ZoneMixing_Impl>()->resetSourceZone();
}

void ZoneMixing::setDeltaTemperature(double deltaTemperature) {
  getImpl<detail::ZoneMixing_Impl>()->setDeltaTemperature(deltaTemperature);
}

void ZoneMixing::resetDeltaTemperature() {
  getImpl<detail::ZoneMixing_Impl>()->resetDeltaTemperature();
}

bool ZoneMixing::setDeltaTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneMixing_Impl>()->setDeltaTemperatureSchedule(schedule);
}

void ZoneMixing::resetDeltaTemperatureSchedule() {
  getImpl<detail::ZoneMixing_Impl>()->resetDeltaTemperatureSchedule();
}

bool ZoneMixing::setMinimumZoneTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneMixing_Impl>()->setMinimumZoneTemperatureSchedule(schedule);
}

void ZoneMixing::resetMinimumZoneTemperatureSchedule() {
  getImpl<detail::ZoneMixing_Impl>()->resetMinimumZoneTemperatureSchedule();
}

bool ZoneMixing::setMaximumZoneTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneMixing_Impl>()->setMaximumZoneTemperatureSchedule(schedule);
}

void ZoneMixing::resetMaximumZoneTemperatureSchedule() {
  getImpl<detail::ZoneMixing_Impl>()->resetMaximumZoneTemperatureSchedule();
}

bool ZoneMixing::setMinimumSourceZoneTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneMixing_Impl>()->setMinimumSourceZoneTemperatureSchedule(schedule);
}

void ZoneMixing::resetMinimumSourceZoneTemperatureSchedule() {
  getImpl<detail::ZoneMixing_Impl>()->resetMinimumSourceZoneTemperatureSchedule();
}

bool ZoneMixing::setMaximumSourceZoneTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneMixing_Impl>()->setMaximumSourceZoneTemperatureSchedule(schedule);
}

void ZoneMixing::resetMaximumSourceZoneTemperatureSchedule() {
  getImpl<detail::ZoneMixing_Impl>()->resetMaximumSourceZoneTemperatureSchedule();
}

bool ZoneMixing::setMinimumOutdoorTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneMixing_Impl>()->setMinimumOutdoorTemperatureSchedule(schedule);
}

void ZoneMixing::resetMinimumOutdoorTemperatureSchedule() {
  getImpl<detail::ZoneMixing_Impl>()->resetMinimumOutdoorTemperatureSchedule();
}

bool ZoneMixing::setMaximumOutdoorTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneMixing_Impl>()->setMaximumOutdoorTemperatureSchedule(schedule);
}

void ZoneMixing::resetMaximumOutdoorTemperatureSchedule() {
  getImpl<detail::ZoneMixing_Impl>()->resetMaximumOutdoorTemperatureSchedule();
}

/// @cond
ZoneMixing::ZoneMixing(std::shared_ptr<detail::ZoneMixing_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

