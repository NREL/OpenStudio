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

#include <model/FanZoneExhaust.hpp>
#include <model/FanZoneExhaust_Impl.hpp>

// TODO: Check the following class names against object getters and setters.
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Connection.hpp>
#include <model/Connection_Impl.hpp>
#include <model/Connection.hpp>
#include <model/Connection_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Fan_ZoneExhaust_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  FanZoneExhaust_Impl::FanZoneExhaust_Impl(const IdfObject& idfObject,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == FanZoneExhaust::iddObjectType());
  }

  FanZoneExhaust_Impl::FanZoneExhaust_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == FanZoneExhaust::iddObjectType());
  }

  FanZoneExhaust_Impl::FanZoneExhaust_Impl(const FanZoneExhaust_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& FanZoneExhaust_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType FanZoneExhaust_Impl::iddObjectType() const {
    return FanZoneExhaust::iddObjectType();
  }

  std::vector<ScheduleTypeKey> FanZoneExhaust_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Fan_ZoneExhaustFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("FanZoneExhaust","Availability"));
    }
    if (std::find(b,e,OS_Fan_ZoneExhaustFields::FlowFractionScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("FanZoneExhaust","Flow Fraction"));
    }
    if (std::find(b,e,OS_Fan_ZoneExhaustFields::MinimumZoneTemperatureLimitScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("FanZoneExhaust","Minimum Zone Temperature Limit"));
    }
    if (std::find(b,e,OS_Fan_ZoneExhaustFields::BalancedExhaustFractionScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("FanZoneExhaust","Balanced Exhaust Fraction"));
    }
    return result;
  }

  boost::optional<Schedule> FanZoneExhaust_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Fan_ZoneExhaustFields::AvailabilityScheduleName);
  }

  double FanZoneExhaust_Impl::fanEfficiency() const {
    boost::optional<double> value = getDouble(OS_Fan_ZoneExhaustFields::FanEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FanZoneExhaust_Impl::pressureRise() const {
    boost::optional<double> value = getDouble(OS_Fan_ZoneExhaustFields::PressureRise,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> FanZoneExhaust_Impl::maximumFlowRate() const {
    return getDouble(OS_Fan_ZoneExhaustFields::MaximumFlowRate,true);
  }

  boost::optional<Connection> FanZoneExhaust_Impl::airInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Fan_ZoneExhaustFields::AirInletNodeName);
  }

  boost::optional<Connection> FanZoneExhaust_Impl::airOutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Fan_ZoneExhaustFields::AirOutletNodeName);
  }

  std::string FanZoneExhaust_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_Fan_ZoneExhaustFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> FanZoneExhaust_Impl::flowFractionSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Fan_ZoneExhaustFields::FlowFractionScheduleName);
  }

  std::string FanZoneExhaust_Impl::systemAvailabilityManagerCouplingMode() const {
    boost::optional<std::string> value = getString(OS_Fan_ZoneExhaustFields::SystemAvailabilityManagerCouplingMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> FanZoneExhaust_Impl::minimumZoneTemperatureLimitSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Fan_ZoneExhaustFields::MinimumZoneTemperatureLimitScheduleName);
  }

  boost::optional<Schedule> FanZoneExhaust_Impl::balancedExhaustFractionSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Fan_ZoneExhaustFields::BalancedExhaustFractionScheduleName);
  }

  bool FanZoneExhaust_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Fan_ZoneExhaustFields::AvailabilityScheduleName,
                              "FanZoneExhaust",
                              "Availability",
                              schedule);
    return result;
  }

  void FanZoneExhaust_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_Fan_ZoneExhaustFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool FanZoneExhaust_Impl::setFanEfficiency(double fanEfficiency) {
    bool result = setDouble(OS_Fan_ZoneExhaustFields::FanEfficiency, fanEfficiency);
    return result;
  }

  void FanZoneExhaust_Impl::setPressureRise(double pressureRise) {
    bool result = setDouble(OS_Fan_ZoneExhaustFields::PressureRise, pressureRise);
    OS_ASSERT(result);
  }

  bool FanZoneExhaust_Impl::setMaximumFlowRate(boost::optional<double> maximumFlowRate) {
    bool result(false);
    if (maximumFlowRate) {
      result = setDouble(OS_Fan_ZoneExhaustFields::MaximumFlowRate, maximumFlowRate.get());
    }
    else {
      resetMaximumFlowRate();
      result = true;
    }
    return result;
  }

  void FanZoneExhaust_Impl::resetMaximumFlowRate() {
    bool result = setString(OS_Fan_ZoneExhaustFields::MaximumFlowRate, "");
    OS_ASSERT(result);
  }

  bool FanZoneExhaust_Impl::setAirInletNode(const boost::optional<Connection>& connection) {
    bool result(false);
    if (connection) {
      result = setPointer(OS_Fan_ZoneExhaustFields::AirInletNodeName, connection.get().handle());
    }
    else {
      resetAirInletNode();
      result = true;
    }
    return result;
  }

  void FanZoneExhaust_Impl::resetAirInletNode() {
    bool result = setString(OS_Fan_ZoneExhaustFields::AirInletNodeName, "");
    OS_ASSERT(result);
  }

  bool FanZoneExhaust_Impl::setAirOutletNode(const boost::optional<Connection>& connection) {
    bool result(false);
    if (connection) {
      result = setPointer(OS_Fan_ZoneExhaustFields::AirOutletNodeName, connection.get().handle());
    }
    else {
      resetAirOutletNode();
      result = true;
    }
    return result;
  }

  void FanZoneExhaust_Impl::resetAirOutletNode() {
    bool result = setString(OS_Fan_ZoneExhaustFields::AirOutletNodeName, "");
    OS_ASSERT(result);
  }

  void FanZoneExhaust_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_Fan_ZoneExhaustFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
  }

  bool FanZoneExhaust_Impl::setFlowFractionSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Fan_ZoneExhaustFields::FlowFractionScheduleName,
                              "FanZoneExhaust",
                              "Flow Fraction",
                              schedule);
    return result;
  }

  void FanZoneExhaust_Impl::resetFlowFractionSchedule() {
    bool result = setString(OS_Fan_ZoneExhaustFields::FlowFractionScheduleName, "");
    OS_ASSERT(result);
  }

  bool FanZoneExhaust_Impl::setSystemAvailabilityManagerCouplingMode(std::string systemAvailabilityManagerCouplingMode) {
    bool result = setString(OS_Fan_ZoneExhaustFields::SystemAvailabilityManagerCouplingMode, systemAvailabilityManagerCouplingMode);
    return result;
  }

  bool FanZoneExhaust_Impl::setMinimumZoneTemperatureLimitSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Fan_ZoneExhaustFields::MinimumZoneTemperatureLimitScheduleName,
                              "FanZoneExhaust",
                              "Minimum Zone Temperature Limit",
                              schedule);
    return result;
  }

  void FanZoneExhaust_Impl::resetMinimumZoneTemperatureLimitSchedule() {
    bool result = setString(OS_Fan_ZoneExhaustFields::MinimumZoneTemperatureLimitScheduleName, "");
    OS_ASSERT(result);
  }

  bool FanZoneExhaust_Impl::setBalancedExhaustFractionSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Fan_ZoneExhaustFields::BalancedExhaustFractionScheduleName,
                              "FanZoneExhaust",
                              "Balanced Exhaust Fraction",
                              schedule);
    return result;
  }

  void FanZoneExhaust_Impl::resetBalancedExhaustFractionSchedule() {
    bool result = setString(OS_Fan_ZoneExhaustFields::BalancedExhaustFractionScheduleName, "");
    OS_ASSERT(result);
  }

} // detail

FanZoneExhaust::FanZoneExhaust(const Model& model)
  : ZoneHVACComponent(FanZoneExhaust::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::FanZoneExhaust_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setFanEfficiency();
  OS_ASSERT(ok);
  // setPressureRise();
  // setEndUseSubcategory();
  // ok = setSystemAvailabilityManagerCouplingMode();
  OS_ASSERT(ok);
}

IddObjectType FanZoneExhaust::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Fan_ZoneExhaust);
}

std::vector<std::string> FanZoneExhaust::systemAvailabilityManagerCouplingModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Fan_ZoneExhaustFields::SystemAvailabilityManagerCouplingMode);
}

boost::optional<Schedule> FanZoneExhaust::availabilitySchedule() const {
  return getImpl<detail::FanZoneExhaust_Impl>()->availabilitySchedule();
}

double FanZoneExhaust::fanEfficiency() const {
  return getImpl<detail::FanZoneExhaust_Impl>()->fanEfficiency();
}

double FanZoneExhaust::pressureRise() const {
  return getImpl<detail::FanZoneExhaust_Impl>()->pressureRise();
}

boost::optional<double> FanZoneExhaust::maximumFlowRate() const {
  return getImpl<detail::FanZoneExhaust_Impl>()->maximumFlowRate();
}

boost::optional<Connection> FanZoneExhaust::airInletNode() const {
  return getImpl<detail::FanZoneExhaust_Impl>()->airInletNode();
}

boost::optional<Connection> FanZoneExhaust::airOutletNode() const {
  return getImpl<detail::FanZoneExhaust_Impl>()->airOutletNode();
}

std::string FanZoneExhaust::endUseSubcategory() const {
  return getImpl<detail::FanZoneExhaust_Impl>()->endUseSubcategory();
}

boost::optional<Schedule> FanZoneExhaust::flowFractionSchedule() const {
  return getImpl<detail::FanZoneExhaust_Impl>()->flowFractionSchedule();
}

std::string FanZoneExhaust::systemAvailabilityManagerCouplingMode() const {
  return getImpl<detail::FanZoneExhaust_Impl>()->systemAvailabilityManagerCouplingMode();
}

boost::optional<Schedule> FanZoneExhaust::minimumZoneTemperatureLimitSchedule() const {
  return getImpl<detail::FanZoneExhaust_Impl>()->minimumZoneTemperatureLimitSchedule();
}

boost::optional<Schedule> FanZoneExhaust::balancedExhaustFractionSchedule() const {
  return getImpl<detail::FanZoneExhaust_Impl>()->balancedExhaustFractionSchedule();
}

bool FanZoneExhaust::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::FanZoneExhaust_Impl>()->setAvailabilitySchedule(schedule);
}

void FanZoneExhaust::resetAvailabilitySchedule() {
  getImpl<detail::FanZoneExhaust_Impl>()->resetAvailabilitySchedule();
}

bool FanZoneExhaust::setFanEfficiency(double fanEfficiency) {
  return getImpl<detail::FanZoneExhaust_Impl>()->setFanEfficiency(fanEfficiency);
}

void FanZoneExhaust::setPressureRise(double pressureRise) {
  getImpl<detail::FanZoneExhaust_Impl>()->setPressureRise(pressureRise);
}

bool FanZoneExhaust::setMaximumFlowRate(double maximumFlowRate) {
  return getImpl<detail::FanZoneExhaust_Impl>()->setMaximumFlowRate(maximumFlowRate);
}

void FanZoneExhaust::resetMaximumFlowRate() {
  getImpl<detail::FanZoneExhaust_Impl>()->resetMaximumFlowRate();
}

bool FanZoneExhaust::setAirInletNode(const Connection& connection) {
  return getImpl<detail::FanZoneExhaust_Impl>()->setAirInletNode(connection);
}

void FanZoneExhaust::resetAirInletNode() {
  getImpl<detail::FanZoneExhaust_Impl>()->resetAirInletNode();
}

bool FanZoneExhaust::setAirOutletNode(const Connection& connection) {
  return getImpl<detail::FanZoneExhaust_Impl>()->setAirOutletNode(connection);
}

void FanZoneExhaust::resetAirOutletNode() {
  getImpl<detail::FanZoneExhaust_Impl>()->resetAirOutletNode();
}

void FanZoneExhaust::setEndUseSubcategory(std::string endUseSubcategory) {
  getImpl<detail::FanZoneExhaust_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

bool FanZoneExhaust::setFlowFractionSchedule(Schedule& schedule) {
  return getImpl<detail::FanZoneExhaust_Impl>()->setFlowFractionSchedule(schedule);
}

void FanZoneExhaust::resetFlowFractionSchedule() {
  getImpl<detail::FanZoneExhaust_Impl>()->resetFlowFractionSchedule();
}

bool FanZoneExhaust::setSystemAvailabilityManagerCouplingMode(std::string systemAvailabilityManagerCouplingMode) {
  return getImpl<detail::FanZoneExhaust_Impl>()->setSystemAvailabilityManagerCouplingMode(systemAvailabilityManagerCouplingMode);
}

bool FanZoneExhaust::setMinimumZoneTemperatureLimitSchedule(Schedule& schedule) {
  return getImpl<detail::FanZoneExhaust_Impl>()->setMinimumZoneTemperatureLimitSchedule(schedule);
}

void FanZoneExhaust::resetMinimumZoneTemperatureLimitSchedule() {
  getImpl<detail::FanZoneExhaust_Impl>()->resetMinimumZoneTemperatureLimitSchedule();
}

bool FanZoneExhaust::setBalancedExhaustFractionSchedule(Schedule& schedule) {
  return getImpl<detail::FanZoneExhaust_Impl>()->setBalancedExhaustFractionSchedule(schedule);
}

void FanZoneExhaust::resetBalancedExhaustFractionSchedule() {
  getImpl<detail::FanZoneExhaust_Impl>()->resetBalancedExhaustFractionSchedule();
}

/// @cond
FanZoneExhaust::FanZoneExhaust(boost::shared_ptr<detail::FanZoneExhaust_Impl> impl)
  : ZoneHVACComponent(impl)
{}
/// @endcond

} // model
} // openstudio

