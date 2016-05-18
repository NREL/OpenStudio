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

#include "ZoneControlContaminantController.hpp"
#include "ZoneControlContaminantController_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ZoneControl_ContaminantController_FieldEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneControlContaminantController_Impl::ZoneControlContaminantController_Impl(const IdfObject& idfObject,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneControlContaminantController::iddObjectType());
  }

  ZoneControlContaminantController_Impl::ZoneControlContaminantController_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneControlContaminantController::iddObjectType());
  }

  ZoneControlContaminantController_Impl::ZoneControlContaminantController_Impl(const ZoneControlContaminantController_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ZoneControlContaminantController_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ZoneControlContaminantController_Impl::iddObjectType() const {
    return ZoneControlContaminantController::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneControlContaminantController_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneControl_ContaminantControllerFields::CarbonDioxideControlAvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneControlContaminantController","Carbon Dioxide Control Availability"));
    }
    if (std::find(b,e,OS_ZoneControl_ContaminantControllerFields::CarbonDioxideSetpointScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneControlContaminantController","Carbon Dioxide Setpoint"));
    }
    if (std::find(b,e,OS_ZoneControl_ContaminantControllerFields::MinimumCarbonDioxideConcentrationScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneControlContaminantController","Minimum Carbon Dioxide Concentration"));
    }
    if (std::find(b,e,OS_ZoneControl_ContaminantControllerFields::GenericContaminantControlAvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneControlContaminantController","Generic Contaminant Control Availability"));
    }
    if (std::find(b,e,OS_ZoneControl_ContaminantControllerFields::GenericContaminantSetpointScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneControlContaminantController","Generic Contaminant Setpoint"));
    }
    return result;
  }

  boost::optional<Schedule> ZoneControlContaminantController_Impl::carbonDioxideControlAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneControl_ContaminantControllerFields::CarbonDioxideControlAvailabilityScheduleName);
  }

  boost::optional<Schedule> ZoneControlContaminantController_Impl::carbonDioxideSetpointSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneControl_ContaminantControllerFields::CarbonDioxideSetpointScheduleName);
  }

  boost::optional<Schedule> ZoneControlContaminantController_Impl::minimumCarbonDioxideConcentrationSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneControl_ContaminantControllerFields::MinimumCarbonDioxideConcentrationScheduleName);
  }

  boost::optional<Schedule> ZoneControlContaminantController_Impl::genericContaminantControlAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneControl_ContaminantControllerFields::GenericContaminantControlAvailabilityScheduleName);
  }

  boost::optional<Schedule> ZoneControlContaminantController_Impl::genericContaminantSetpointSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneControl_ContaminantControllerFields::GenericContaminantSetpointScheduleName);
  }

  bool ZoneControlContaminantController_Impl::setCarbonDioxideControlAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneControl_ContaminantControllerFields::CarbonDioxideControlAvailabilityScheduleName,
                              "ZoneControlContaminantController",
                              "Carbon Dioxide Control Availability",
                              schedule);
    return result;
  }

  void ZoneControlContaminantController_Impl::resetCarbonDioxideControlAvailabilitySchedule() {
    bool result = setString(OS_ZoneControl_ContaminantControllerFields::CarbonDioxideControlAvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneControlContaminantController_Impl::setCarbonDioxideSetpointSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneControl_ContaminantControllerFields::CarbonDioxideSetpointScheduleName,
                              "ZoneControlContaminantController",
                              "Carbon Dioxide Setpoint",
                              schedule);
    return result;
  }

  void ZoneControlContaminantController_Impl::resetCarbonDioxideSetpointSchedule() {
    bool result = setString(OS_ZoneControl_ContaminantControllerFields::CarbonDioxideSetpointScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneControlContaminantController_Impl::setMinimumCarbonDioxideConcentrationSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneControl_ContaminantControllerFields::MinimumCarbonDioxideConcentrationScheduleName,
                              "ZoneControlContaminantController",
                              "Minimum Carbon Dioxide Concentration",
                              schedule);
    return result;
  }

  void ZoneControlContaminantController_Impl::resetMinimumCarbonDioxideConcentrationSchedule() {
    bool result = setString(OS_ZoneControl_ContaminantControllerFields::MinimumCarbonDioxideConcentrationScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneControlContaminantController_Impl::setGenericContaminantControlAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneControl_ContaminantControllerFields::GenericContaminantControlAvailabilityScheduleName,
                              "ZoneControlContaminantController",
                              "Generic Contaminant Control Availability",
                              schedule);
    return result;
  }

  void ZoneControlContaminantController_Impl::resetGenericContaminantControlAvailabilitySchedule() {
    bool result = setString(OS_ZoneControl_ContaminantControllerFields::GenericContaminantControlAvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool ZoneControlContaminantController_Impl::setGenericContaminantSetpointSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneControl_ContaminantControllerFields::GenericContaminantSetpointScheduleName,
                              "ZoneControlContaminantController",
                              "Generic Contaminant Setpoint",
                              schedule);
    return result;
  }

  void ZoneControlContaminantController_Impl::resetGenericContaminantSetpointSchedule() {
    bool result = setString(OS_ZoneControl_ContaminantControllerFields::GenericContaminantSetpointScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<ThermalZone> ZoneControlContaminantController_Impl::controlledZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_ZoneControl_ContaminantControllerFields::ControlledZoneName);
  }

  bool ZoneControlContaminantController_Impl::setControlledZone(ThermalZone& zone) {
    return setPointer(OS_ZoneControl_ContaminantControllerFields::ControlledZoneName,zone.handle());
  }

  void ZoneControlContaminantController_Impl::resetControlledZone() {
    setString(OS_ZoneControl_ContaminantControllerFields::ControlledZoneName,"");
  }

  ModelObject ZoneControlContaminantController_Impl::clone(Model model) const
  {
    auto clone = ModelObject_Impl::clone(model);
    clone.setString(OS_ZoneControl_ContaminantControllerFields::ControlledZoneName,"");
    return clone;
  }

} // detail

ZoneControlContaminantController::ZoneControlContaminantController(const Model& model)
  : ModelObject(ZoneControlContaminantController::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneControlContaminantController_Impl>());
}

IddObjectType ZoneControlContaminantController::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneControl_ContaminantController);
}

boost::optional<Schedule> ZoneControlContaminantController::carbonDioxideControlAvailabilitySchedule() const {
  return getImpl<detail::ZoneControlContaminantController_Impl>()->carbonDioxideControlAvailabilitySchedule();
}

boost::optional<Schedule> ZoneControlContaminantController::carbonDioxideSetpointSchedule() const {
  return getImpl<detail::ZoneControlContaminantController_Impl>()->carbonDioxideSetpointSchedule();
}

boost::optional<Schedule> ZoneControlContaminantController::minimumCarbonDioxideConcentrationSchedule() const {
  return getImpl<detail::ZoneControlContaminantController_Impl>()->minimumCarbonDioxideConcentrationSchedule();
}

boost::optional<Schedule> ZoneControlContaminantController::genericContaminantControlAvailabilitySchedule() const {
  return getImpl<detail::ZoneControlContaminantController_Impl>()->genericContaminantControlAvailabilitySchedule();
}

boost::optional<Schedule> ZoneControlContaminantController::genericContaminantSetpointSchedule() const {
  return getImpl<detail::ZoneControlContaminantController_Impl>()->genericContaminantSetpointSchedule();
}

bool ZoneControlContaminantController::setCarbonDioxideControlAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::ZoneControlContaminantController_Impl>()->setCarbonDioxideControlAvailabilitySchedule(schedule);
}

void ZoneControlContaminantController::resetCarbonDioxideControlAvailabilitySchedule() {
  getImpl<detail::ZoneControlContaminantController_Impl>()->resetCarbonDioxideControlAvailabilitySchedule();
}

bool ZoneControlContaminantController::setCarbonDioxideSetpointSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneControlContaminantController_Impl>()->setCarbonDioxideSetpointSchedule(schedule);
}

void ZoneControlContaminantController::resetCarbonDioxideSetpointSchedule() {
  getImpl<detail::ZoneControlContaminantController_Impl>()->resetCarbonDioxideSetpointSchedule();
}

bool ZoneControlContaminantController::setMinimumCarbonDioxideConcentrationSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneControlContaminantController_Impl>()->setMinimumCarbonDioxideConcentrationSchedule(schedule);
}

void ZoneControlContaminantController::resetMinimumCarbonDioxideConcentrationSchedule() {
  getImpl<detail::ZoneControlContaminantController_Impl>()->resetMinimumCarbonDioxideConcentrationSchedule();
}

bool ZoneControlContaminantController::setGenericContaminantControlAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::ZoneControlContaminantController_Impl>()->setGenericContaminantControlAvailabilitySchedule(schedule);
}

void ZoneControlContaminantController::resetGenericContaminantControlAvailabilitySchedule() {
  getImpl<detail::ZoneControlContaminantController_Impl>()->resetGenericContaminantControlAvailabilitySchedule();
}

bool ZoneControlContaminantController::setGenericContaminantSetpointSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneControlContaminantController_Impl>()->setGenericContaminantSetpointSchedule(schedule);
}

void ZoneControlContaminantController::resetGenericContaminantSetpointSchedule() {
  getImpl<detail::ZoneControlContaminantController_Impl>()->resetGenericContaminantSetpointSchedule();
}

boost::optional<ThermalZone> ZoneControlContaminantController::controlledZone() const {
  return getImpl<detail::ZoneControlContaminantController_Impl>()->controlledZone();
}

/// @cond
ZoneControlContaminantController::ZoneControlContaminantController(std::shared_ptr<detail::ZoneControlContaminantController_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

