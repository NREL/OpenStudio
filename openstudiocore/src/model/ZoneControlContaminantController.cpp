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

#include "ZoneControlContaminantController.hpp"
#include "ZoneControlContaminantController_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
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
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

