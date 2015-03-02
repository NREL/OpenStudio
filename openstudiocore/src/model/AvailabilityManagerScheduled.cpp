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

#include "AvailabilityManagerScheduled.hpp"
#include "AvailabilityManagerScheduled_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleConstant.hpp"
#include "ScheduleConstant_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "Model.hpp"
#include <utilities/idd/OS_AvailabilityManager_Scheduled_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AvailabilityManagerScheduled_Impl::AvailabilityManagerScheduled_Impl(const IdfObject& idfObject,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AvailabilityManagerScheduled::iddObjectType());
  }

  AvailabilityManagerScheduled_Impl::AvailabilityManagerScheduled_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AvailabilityManagerScheduled::iddObjectType());
  }

  AvailabilityManagerScheduled_Impl::AvailabilityManagerScheduled_Impl(const AvailabilityManagerScheduled_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AvailabilityManagerScheduled_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AvailabilityManagerScheduled_Impl::iddObjectType() const {
    return AvailabilityManagerScheduled::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AvailabilityManagerScheduled_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AvailabilityManager_ScheduledFields::ScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AvailabilityManagerScheduled","Availability"));
    }
    return result;
  }

  Schedule AvailabilityManagerScheduled_Impl::schedule() const {
    boost::optional<Schedule> value = optionalSchedule();
    if (!value) {
      LOG(Info,briefDescription() << " does not have an Schedule attached.  Returning always on.");
      return model().alwaysOnDiscreteSchedule();
    }
    return value.get();
  }

  bool AvailabilityManagerScheduled_Impl::setSchedule(Schedule& schedule) {
    bool result = ModelObject_Impl::setSchedule(OS_AvailabilityManager_ScheduledFields::ScheduleName,
                                                "AvailabilityManagerScheduled",
                                                "Availability",
                                                schedule);
    return result;
    //return setPointer(OS_AvailabilityManager_ScheduledFields::ScheduleName,schedule.handle());
  }

  boost::optional<Schedule> AvailabilityManagerScheduled_Impl::optionalSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AvailabilityManager_ScheduledFields::ScheduleName);
  }

  boost::optional<ModelObject> AvailabilityManagerScheduled_Impl::scheduleAsModelObject() const {
    OptionalModelObject result = schedule();
    return result;
  }

  bool AvailabilityManagerScheduled_Impl::setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setSchedule(schedule);
      }
    }
    return false;
  }

  ModelObject AvailabilityManagerScheduled_Impl::clone(Model model) const
  {
    AvailabilityManagerScheduled amsClone = ModelObject_Impl::clone(model).cast<AvailabilityManagerScheduled>();

    Schedule scheduleClone = schedule().clone(model).cast<Schedule>();

    amsClone.setSchedule(scheduleClone);

    return amsClone;
  }

} // detail

AvailabilityManagerScheduled::AvailabilityManagerScheduled(const Model& model)
  : ModelObject(AvailabilityManagerScheduled::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AvailabilityManagerScheduled_Impl>());

  Schedule s = model.alwaysOnDiscreteSchedule();

  OS_ASSERT(setSchedule(s));
}

IddObjectType AvailabilityManagerScheduled::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AvailabilityManager_Scheduled);
}

Schedule AvailabilityManagerScheduled::schedule() const {
  return getImpl<detail::AvailabilityManagerScheduled_Impl>()->schedule();
}

bool AvailabilityManagerScheduled::setSchedule(Schedule& schedule) {
  return getImpl<detail::AvailabilityManagerScheduled_Impl>()->setSchedule(schedule);
}

/// @cond
AvailabilityManagerScheduled::AvailabilityManagerScheduled(std::shared_ptr<detail::AvailabilityManagerScheduled_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio

