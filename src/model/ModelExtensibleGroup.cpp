/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ModelExtensibleGroup.hpp"
#include "ModelObject_Impl.hpp"

#include "ScheduleTypeRegistry.hpp"
#include "Schedule.hpp"
#include "ScheduleTypeLimits.hpp"

#include "../utilities/core/Logger.hpp"

namespace openstudio {
namespace model {

  bool ModelExtensibleGroup::setSchedule(unsigned index, const std::string& className, const std::string& scheduleDisplayName, Schedule& schedule) {
    bool result = checkOrAssignScheduleTypeLimits(className, scheduleDisplayName, schedule);
    if (!result) {
      if (boost::optional<ScheduleTypeLimits> scheduleTypeLimits = schedule.scheduleTypeLimits()) {
        LOG(Warn, "For object of type " << className << " cannot set Schedule " << scheduleDisplayName << "=" << schedule.nameString()
                                        << " because it has an incompatible ScheduleTypeLimits (" << scheduleTypeLimits->nameString() << ")");
      }
      return result;
    }
    return setPointer(index, schedule.handle());
  }

  // PROTECTED

  /// @cond
  ModelExtensibleGroup::ModelExtensibleGroup(std::shared_ptr<detail::ModelObject_Impl> impl, unsigned index)
    : WorkspaceExtensibleGroup(impl, index) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
