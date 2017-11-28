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
    auto zones = model().getConcreteModelObjects<model::ThermalZone>();
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
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

