/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AvailabilityManagerScheduledOn.hpp"
#include "AvailabilityManagerScheduledOn_Impl.hpp"

#include "Model.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AvailabilityManager_ScheduledOn_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AvailabilityManagerScheduledOn_Impl::AvailabilityManagerScheduledOn_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : AvailabilityManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AvailabilityManagerScheduledOn::iddObjectType());
    }

    AvailabilityManagerScheduledOn_Impl::AvailabilityManagerScheduledOn_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                             bool keepHandle)
      : AvailabilityManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AvailabilityManagerScheduledOn::iddObjectType());
    }

    AvailabilityManagerScheduledOn_Impl::AvailabilityManagerScheduledOn_Impl(const AvailabilityManagerScheduledOn_Impl& other, Model_Impl* model,
                                                                             bool keepHandle)
      : AvailabilityManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AvailabilityManagerScheduledOn_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Availability Manager Scheduled On Control Status"};
      return result;
    }

    IddObjectType AvailabilityManagerScheduledOn_Impl::iddObjectType() const {
      return AvailabilityManagerScheduledOn::iddObjectType();
    }

    std::vector<ScheduleTypeKey> AvailabilityManagerScheduledOn_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_AvailabilityManager_ScheduledOnFields::ScheduleName) != e) {
        result.push_back(ScheduleTypeKey("AvailabilityManagerScheduledOn", "Availability Manager Scheduled On"));
      }
      return result;
    }

    Schedule AvailabilityManagerScheduledOn_Impl::schedule() const {
      boost::optional<Schedule> value = optionalSchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Schedule attached.");
      }
      return value.get();
    }

    bool AvailabilityManagerScheduledOn_Impl::setSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_AvailabilityManager_ScheduledOnFields::ScheduleName, "AvailabilityManagerScheduledOn",
                                                  "Availability Manager Scheduled On", schedule);
      return result;
    }

    boost::optional<Schedule> AvailabilityManagerScheduledOn_Impl::optionalSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AvailabilityManager_ScheduledOnFields::ScheduleName);
    }

  }  // namespace detail

  AvailabilityManagerScheduledOn::AvailabilityManagerScheduledOn(const Model& model)
    : AvailabilityManager(AvailabilityManagerScheduledOn::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AvailabilityManagerScheduledOn_Impl>());
    {
      auto schedule = model.alwaysOnDiscreteSchedule();
      setSchedule(schedule);
    }
  }

  IddObjectType AvailabilityManagerScheduledOn::iddObjectType() {
    return {IddObjectType::OS_AvailabilityManager_ScheduledOn};
  }

  Schedule AvailabilityManagerScheduledOn::schedule() const {
    return getImpl<detail::AvailabilityManagerScheduledOn_Impl>()->schedule();
  }

  bool AvailabilityManagerScheduledOn::setSchedule(Schedule& schedule) {
    return getImpl<detail::AvailabilityManagerScheduledOn_Impl>()->setSchedule(schedule);
  }

  /// @cond
  AvailabilityManagerScheduledOn::AvailabilityManagerScheduledOn(std::shared_ptr<detail::AvailabilityManagerScheduledOn_Impl> impl)
    : AvailabilityManager(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
