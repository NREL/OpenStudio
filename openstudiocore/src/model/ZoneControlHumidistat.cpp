/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "ZoneControlHumidistat.hpp"
#include "ZoneControlHumidistat_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"

#include <utilities/idd/OS_ZoneControl_Humidistat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneControlHumidistat_Impl::ZoneControlHumidistat_Impl(const IdfObject& idfObject,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneControlHumidistat::iddObjectType());
  }

  ZoneControlHumidistat_Impl::ZoneControlHumidistat_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneControlHumidistat::iddObjectType());
  }

  ZoneControlHumidistat_Impl::ZoneControlHumidistat_Impl(const ZoneControlHumidistat_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ZoneControlHumidistat_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ZoneControlHumidistat_Impl::iddObjectType() const {
    return ZoneControlHumidistat::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneControlHumidistat_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneControl_HumidistatFields::HumidifyingRelativeHumiditySetpointScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneControlHumidistat","Humidifying Relative Humidity Setpoint"));
    }
    if (std::find(b,e,OS_ZoneControl_HumidistatFields::DehumidifyingRelativeHumiditySetpointScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneControlHumidistat","Dehumidifying Relative Humidity Setpoint"));
    }
    return result;
  }

  boost::optional<Schedule> ZoneControlHumidistat_Impl::humidifyingRelativeHumiditySetpointSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneControl_HumidistatFields::HumidifyingRelativeHumiditySetpointScheduleName);
  }

  boost::optional<Schedule> ZoneControlHumidistat_Impl::dehumidifyingRelativeHumiditySetpointSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneControl_HumidistatFields::DehumidifyingRelativeHumiditySetpointScheduleName);
  }

  bool ZoneControlHumidistat_Impl::setHumidifyingRelativeHumiditySetpointSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneControl_HumidistatFields::HumidifyingRelativeHumiditySetpointScheduleName,
                              "ZoneControlHumidistat",
                              "Humidifying Relative Humidity Setpoint",
                              schedule);
    return result;
  }

  bool ZoneControlHumidistat_Impl::setDehumidifyingRelativeHumiditySetpointSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneControl_HumidistatFields::DehumidifyingRelativeHumiditySetpointScheduleName,
                              "ZoneControlHumidistat",
                              "Dehumidifying Relative Humidity Setpoint",
                              schedule);
    return result;
  }

  void ZoneControlHumidistat_Impl::resetHumidifyingRelativeHumiditySetpointSchedule() {
    bool result = setString(OS_ZoneControl_HumidistatFields::HumidifyingRelativeHumiditySetpointScheduleName, "");
    OS_ASSERT(result);
  }

  void ZoneControlHumidistat_Impl::resetDehumidifyingRelativeHumiditySetpointSchedule() {
    bool result = setString(OS_ZoneControl_HumidistatFields::DehumidifyingRelativeHumiditySetpointScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<ThermalZone> ZoneControlHumidistat_Impl::controlledZone() const
  {
    Handle h = handle();
    auto zones = model().getModelObjects<model::ThermalZone>();
    for( const auto & zone: zones ) {
      if( auto humidistat = zone.zoneControlHumidistat() ) {
        if( humidistat->handle() == h ) {
          return zone;
        }
      }
    }
    return boost::none;
  }

} // detail

ZoneControlHumidistat::ZoneControlHumidistat(const Model& model)
  : ModelObject(ZoneControlHumidistat::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneControlHumidistat_Impl>());
}

IddObjectType ZoneControlHumidistat::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneControl_Humidistat);
}

boost::optional<Schedule> ZoneControlHumidistat::humidifyingRelativeHumiditySetpointSchedule() const {
  return getImpl<detail::ZoneControlHumidistat_Impl>()->humidifyingRelativeHumiditySetpointSchedule();
}

boost::optional<Schedule> ZoneControlHumidistat::dehumidifyingRelativeHumiditySetpointSchedule() const {
  return getImpl<detail::ZoneControlHumidistat_Impl>()->dehumidifyingRelativeHumiditySetpointSchedule();
}

bool ZoneControlHumidistat::setHumidifyingRelativeHumiditySetpointSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneControlHumidistat_Impl>()->setHumidifyingRelativeHumiditySetpointSchedule(schedule);
}

bool ZoneControlHumidistat::setDehumidifyingRelativeHumiditySetpointSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneControlHumidistat_Impl>()->setDehumidifyingRelativeHumiditySetpointSchedule(schedule);
}

void ZoneControlHumidistat::resetHumidifyingRelativeHumiditySetpointSchedule() {
  getImpl<detail::ZoneControlHumidistat_Impl>()->resetHumidifyingRelativeHumiditySetpointSchedule();
}

void ZoneControlHumidistat::resetDehumidifyingRelativeHumiditySetpointSchedule() {
  getImpl<detail::ZoneControlHumidistat_Impl>()->resetDehumidifyingRelativeHumiditySetpointSchedule();
}

boost::optional<ThermalZone> ZoneControlHumidistat::controlledZone() const {
  return getImpl<detail::ZoneControlHumidistat_Impl>()->controlledZone();
}

/// @cond
ZoneControlHumidistat::ZoneControlHumidistat(std::shared_ptr<detail::ZoneControlHumidistat_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

