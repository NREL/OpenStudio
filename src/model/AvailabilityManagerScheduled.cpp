/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AvailabilityManagerScheduled.hpp"
#include "AvailabilityManagerScheduled_Impl.hpp"

#include "Model.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AvailabilityManager_Scheduled_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AvailabilityManagerScheduled_Impl::AvailabilityManagerScheduled_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : AvailabilityManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AvailabilityManagerScheduled::iddObjectType());
    }

    AvailabilityManagerScheduled_Impl::AvailabilityManagerScheduled_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                         bool keepHandle)
      : AvailabilityManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AvailabilityManagerScheduled::iddObjectType());
    }

    AvailabilityManagerScheduled_Impl::AvailabilityManagerScheduled_Impl(const AvailabilityManagerScheduled_Impl& other, Model_Impl* model,
                                                                         bool keepHandle)
      : AvailabilityManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AvailabilityManagerScheduled_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Availability Manager Scheduled Control Status"};
      return result;
    }

    IddObjectType AvailabilityManagerScheduled_Impl::iddObjectType() const {
      return AvailabilityManagerScheduled::iddObjectType();
    }

    std::vector<ScheduleTypeKey> AvailabilityManagerScheduled_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_AvailabilityManager_ScheduledFields::ScheduleName) != e) {
        result.push_back(ScheduleTypeKey("AvailabilityManagerScheduled", "Availability Manager Scheduled"));
      }
      return result;
    }

    Schedule AvailabilityManagerScheduled_Impl::schedule() const {
      boost::optional<Schedule> value = optionalSchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Schedule attached.");
      }
      return value.get();
    }

    bool AvailabilityManagerScheduled_Impl::setSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_AvailabilityManager_ScheduledFields::ScheduleName, "AvailabilityManagerScheduled",
                                                  "Availability Manager Scheduled", schedule);
      return result;
    }

    boost::optional<Schedule> AvailabilityManagerScheduled_Impl::optionalSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AvailabilityManager_ScheduledFields::ScheduleName);
    }

  }  // namespace detail

  AvailabilityManagerScheduled::AvailabilityManagerScheduled(const Model& model)
    : AvailabilityManager(AvailabilityManagerScheduled::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AvailabilityManagerScheduled_Impl>());
    {
      auto schedule = model.alwaysOnDiscreteSchedule();
      setSchedule(schedule);
    }
  }

  IddObjectType AvailabilityManagerScheduled::iddObjectType() {
    return {IddObjectType::OS_AvailabilityManager_Scheduled};
  }

  Schedule AvailabilityManagerScheduled::schedule() const {
    return getImpl<detail::AvailabilityManagerScheduled_Impl>()->schedule();
  }

  bool AvailabilityManagerScheduled::setSchedule(Schedule& schedule) {
    return getImpl<detail::AvailabilityManagerScheduled_Impl>()->setSchedule(schedule);
  }

  /// @cond
  AvailabilityManagerScheduled::AvailabilityManagerScheduled(std::shared_ptr<detail::AvailabilityManagerScheduled_Impl> impl)
    : AvailabilityManager(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
