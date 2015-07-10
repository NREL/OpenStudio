/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include <model/ZoneMixing.hpp>
#include <model/ZoneMixing_Impl.hpp>

// TODO: Check the following class names against object getters and setters.
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ZoneMixing_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Assert.hpp>

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

  Zone ZoneMixing_Impl::zone() const {
    boost::optional<Zone> value = optionalZone();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Zone attached.");
    }
    return value.get();
  }

  Schedule ZoneMixing_Impl::schedule() const {
    boost::optional<Schedule> value = optionalSchedule();
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

  bool ZoneMixing_Impl::isDesignFlowRateCalculationMethodDefaulted() const {
    return isEmpty(OS_ZoneMixingFields::DesignFlowRateCalculationMethod);
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

  boost::optional<Zone> ZoneMixing_Impl::sourceZone() const {
    return getObject<ModelObject>().getModelObjectTarget<Zone>(OS_ZoneMixingFields::SourceZoneName);
  }

  double ZoneMixing_Impl::deltaTemperature() const {
    boost::optional<double> value = getDouble(OS_ZoneMixingFields::DeltaTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneMixing_Impl::isDeltaTemperatureDefaulted() const {
    return isEmpty(OS_ZoneMixingFields::DeltaTemperature);
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

  bool ZoneMixing_Impl::setZone(const Zone& zone) {
    bool result = setPointer(OS_ZoneMixingFields::ZoneName, zone.handle());
    return result;
  }

  bool ZoneMixing_Impl::setSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_ZoneMixingFields::ScheduleName,
                                                "ZoneMixing",
                                                "Zone Mixing",
                                                schedule);
    return result;
  }

  bool ZoneMixing_Impl::setDesignFlowRateCalculationMethod(std::string designFlowRateCalculationMethod) {
    bool result = setString(OS_ZoneMixingFields::DesignFlowRateCalculationMethod, designFlowRateCalculationMethod);
    return result;
  }

  void ZoneMixing_Impl::resetDesignFlowRateCalculationMethod() {
    bool result = setString(OS_ZoneMixingFields::DesignFlowRateCalculationMethod, "");
    OS_ASSERT(result);
  }

  bool ZoneMixing_Impl::setDesignFlowRate(boost::optional<double> designFlowRate) {
    bool result(false);
    if (designFlowRate) {
      result = setDouble(OS_ZoneMixingFields::DesignFlowRate, designFlowRate.get());
    }
    else {
      resetDesignFlowRate();
      result = true;
    }
    return result;
  }

  void ZoneMixing_Impl::resetDesignFlowRate() {
    bool result = setString(OS_ZoneMixingFields::DesignFlowRate, "");
    OS_ASSERT(result);
  }

  bool ZoneMixing_Impl::setFlowRateperZoneFloorArea(boost::optional<double> flowRateperZoneFloorArea) {
    bool result(false);
    if (flowRateperZoneFloorArea) {
      result = setDouble(OS_ZoneMixingFields::FlowRateperZoneFloorArea, flowRateperZoneFloorArea.get());
    }
    else {
      resetFlowRateperZoneFloorArea();
      result = true;
    }
    return result;
  }

  void ZoneMixing_Impl::resetFlowRateperZoneFloorArea() {
    bool result = setString(OS_ZoneMixingFields::FlowRateperZoneFloorArea, "");
    OS_ASSERT(result);
  }

  bool ZoneMixing_Impl::setFlowRateperPerson(boost::optional<double> flowRateperPerson) {
    bool result(false);
    if (flowRateperPerson) {
      result = setDouble(OS_ZoneMixingFields::FlowRateperPerson, flowRateperPerson.get());
    }
    else {
      resetFlowRateperPerson();
      result = true;
    }
    return result;
  }

  void ZoneMixing_Impl::resetFlowRateperPerson() {
    bool result = setString(OS_ZoneMixingFields::FlowRateperPerson, "");
    OS_ASSERT(result);
  }

  bool ZoneMixing_Impl::setAirChangesperHour(boost::optional<double> airChangesperHour) {
    bool result(false);
    if (airChangesperHour) {
      result = setDouble(OS_ZoneMixingFields::AirChangesperHour, airChangesperHour.get());
    }
    else {
      resetAirChangesperHour();
      result = true;
    }
    return result;
  }

  void ZoneMixing_Impl::resetAirChangesperHour() {
    bool result = setString(OS_ZoneMixingFields::AirChangesperHour, "");
    OS_ASSERT(result);
  }

  bool ZoneMixing_Impl::setSourceZone(const boost::optional<Zone>& zone) {
    bool result(false);
    if (zone) {
      result = setPointer(OS_ZoneMixingFields::SourceZoneName, zone.get().handle());
    }
    else {
      resetSourceZone();
      result = true;
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
  }

  void ZoneMixing_Impl::resetDeltaTemperature() {
    bool result = setString(OS_ZoneMixingFields::DeltaTemperature, "");
    OS_ASSERT(result);
  }

  bool ZoneMixing_Impl::setDeltaTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneMixingFields::DeltaTemperatureScheduleName,
                              "ZoneMixing",
                              "Delta Temperature",
                              schedule);
    return result;
  }

  void ZoneMixing_Impl::resetDeltaTemperatureSchedule() {
    bool result = setString(OS_ZoneMixingFields::DeltaTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneMixing_Impl::setMinimumZoneTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneMixingFields::MinimumZoneTemperatureScheduleName,
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
    bool result = setSchedule(OS_ZoneMixingFields::MaximumZoneTemperatureScheduleName,
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
    bool result = setSchedule(OS_ZoneMixingFields::MinimumSourceZoneTemperatureScheduleName,
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
    bool result = setSchedule(OS_ZoneMixingFields::MaximumSourceZoneTemperatureScheduleName,
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
    bool result = setSchedule(OS_ZoneMixingFields::MinimumOutdoorTemperatureScheduleName,
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
    bool result = setSchedule(OS_ZoneMixingFields::MaximumOutdoorTemperatureScheduleName,
                              "ZoneMixing",
                              "Maximum Outdoor Temperature",
                              schedule);
    return result;
  }

  void ZoneMixing_Impl::resetMaximumOutdoorTemperatureSchedule() {
    bool result = setString(OS_ZoneMixingFields::MaximumOutdoorTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<Zone> ZoneMixing_Impl::optionalZone() const {
    return getObject<ModelObject>().getModelObjectTarget<Zone>(OS_ZoneMixingFields::ZoneName);
  }

  boost::optional<Schedule> ZoneMixing_Impl::optionalSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneMixingFields::ScheduleName);
  }

} // detail

ZoneMixing::ZoneMixing(const Model& model)
  : ModelObject(ZoneMixing::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneMixing_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_ZoneMixingFields::ZoneName
  //     OS_ZoneMixingFields::ScheduleName
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setZone();
  OS_ASSERT(ok);
  // ok = setSchedule();
  OS_ASSERT(ok);
}

IddObjectType ZoneMixing::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneMixing);
}

std::vector<std::string> ZoneMixing::designFlowRateCalculationMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneMixingFields::DesignFlowRateCalculationMethod);
}

Zone ZoneMixing::zone() const {
  return getImpl<detail::ZoneMixing_Impl>()->zone();
}

Schedule ZoneMixing::schedule() const {
  return getImpl<detail::ZoneMixing_Impl>()->schedule();
}

std::string ZoneMixing::designFlowRateCalculationMethod() const {
  return getImpl<detail::ZoneMixing_Impl>()->designFlowRateCalculationMethod();
}

bool ZoneMixing::isDesignFlowRateCalculationMethodDefaulted() const {
  return getImpl<detail::ZoneMixing_Impl>()->isDesignFlowRateCalculationMethodDefaulted();
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

boost::optional<Zone> ZoneMixing::sourceZone() const {
  return getImpl<detail::ZoneMixing_Impl>()->sourceZone();
}

double ZoneMixing::deltaTemperature() const {
  return getImpl<detail::ZoneMixing_Impl>()->deltaTemperature();
}

bool ZoneMixing::isDeltaTemperatureDefaulted() const {
  return getImpl<detail::ZoneMixing_Impl>()->isDeltaTemperatureDefaulted();
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

bool ZoneMixing::setZone(const Zone& zone) {
  return getImpl<detail::ZoneMixing_Impl>()->setZone(zone);
}

bool ZoneMixing::setSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneMixing_Impl>()->setSchedule(schedule);
}

bool ZoneMixing::setDesignFlowRateCalculationMethod(std::string designFlowRateCalculationMethod) {
  return getImpl<detail::ZoneMixing_Impl>()->setDesignFlowRateCalculationMethod(designFlowRateCalculationMethod);
}

void ZoneMixing::resetDesignFlowRateCalculationMethod() {
  getImpl<detail::ZoneMixing_Impl>()->resetDesignFlowRateCalculationMethod();
}

bool ZoneMixing::setDesignFlowRate(double designFlowRate) {
  return getImpl<detail::ZoneMixing_Impl>()->setDesignFlowRate(designFlowRate);
}

void ZoneMixing::resetDesignFlowRate() {
  getImpl<detail::ZoneMixing_Impl>()->resetDesignFlowRate();
}

bool ZoneMixing::setFlowRateperZoneFloorArea(double flowRateperZoneFloorArea) {
  return getImpl<detail::ZoneMixing_Impl>()->setFlowRateperZoneFloorArea(flowRateperZoneFloorArea);
}

void ZoneMixing::resetFlowRateperZoneFloorArea() {
  getImpl<detail::ZoneMixing_Impl>()->resetFlowRateperZoneFloorArea();
}

bool ZoneMixing::setFlowRateperPerson(double flowRateperPerson) {
  return getImpl<detail::ZoneMixing_Impl>()->setFlowRateperPerson(flowRateperPerson);
}

void ZoneMixing::resetFlowRateperPerson() {
  getImpl<detail::ZoneMixing_Impl>()->resetFlowRateperPerson();
}

bool ZoneMixing::setAirChangesperHour(double airChangesperHour) {
  return getImpl<detail::ZoneMixing_Impl>()->setAirChangesperHour(airChangesperHour);
}

void ZoneMixing::resetAirChangesperHour() {
  getImpl<detail::ZoneMixing_Impl>()->resetAirChangesperHour();
}

bool ZoneMixing::setSourceZone(const Zone& zone) {
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
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

