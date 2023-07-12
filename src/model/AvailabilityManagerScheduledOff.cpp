/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AvailabilityManagerScheduledOff.hpp"
#include "AvailabilityManagerScheduledOff_Impl.hpp"

#include "Model.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AvailabilityManager_ScheduledOff_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AvailabilityManagerScheduledOff_Impl::AvailabilityManagerScheduledOff_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : AvailabilityManager_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AvailabilityManagerScheduledOff::iddObjectType());
    }

    AvailabilityManagerScheduledOff_Impl::AvailabilityManagerScheduledOff_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model, bool keepHandle)
      : AvailabilityManager_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AvailabilityManagerScheduledOff::iddObjectType());
    }

    AvailabilityManagerScheduledOff_Impl::AvailabilityManagerScheduledOff_Impl(const AvailabilityManagerScheduledOff_Impl& other, Model_Impl* model,
                                                                               bool keepHandle)
      : AvailabilityManager_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AvailabilityManagerScheduledOff_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Availability Manager Scheduled Off Control Status"};
      return result;
    }

    IddObjectType AvailabilityManagerScheduledOff_Impl::iddObjectType() const {
      return AvailabilityManagerScheduledOff::iddObjectType();
    }

    std::vector<ScheduleTypeKey> AvailabilityManagerScheduledOff_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_AvailabilityManager_ScheduledOffFields::ScheduleName) != e) {
        result.push_back(ScheduleTypeKey("AvailabilityManagerScheduledOff", "Availability Manager Scheduled Off"));
      }
      return result;
    }

    Schedule AvailabilityManagerScheduledOff_Impl::schedule() const {
      boost::optional<Schedule> value = optionalSchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Schedule attached.");
      }
      return value.get();
    }

    bool AvailabilityManagerScheduledOff_Impl::setSchedule(Schedule& schedule) {
      bool result = ModelObject_Impl::setSchedule(OS_AvailabilityManager_ScheduledOffFields::ScheduleName, "AvailabilityManagerScheduledOff",
                                                  "Availability Manager Scheduled Off", schedule);
      return result;
    }

    boost::optional<Schedule> AvailabilityManagerScheduledOff_Impl::optionalSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AvailabilityManager_ScheduledOffFields::ScheduleName);
    }

  }  // namespace detail

  AvailabilityManagerScheduledOff::AvailabilityManagerScheduledOff(const Model& model)
    : AvailabilityManager(AvailabilityManagerScheduledOff::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AvailabilityManagerScheduledOff_Impl>());
    {
      auto schedule = model.alwaysOffDiscreteSchedule();
      setSchedule(schedule);
    }
  }

  IddObjectType AvailabilityManagerScheduledOff::iddObjectType() {
    return {IddObjectType::OS_AvailabilityManager_ScheduledOff};
  }

  Schedule AvailabilityManagerScheduledOff::schedule() const {
    return getImpl<detail::AvailabilityManagerScheduledOff_Impl>()->schedule();
  }

  bool AvailabilityManagerScheduledOff::setSchedule(Schedule& schedule) {
    return getImpl<detail::AvailabilityManagerScheduledOff_Impl>()->setSchedule(schedule);
  }

  /// @cond
  AvailabilityManagerScheduledOff::AvailabilityManagerScheduledOff(std::shared_ptr<detail::AvailabilityManagerScheduledOff_Impl> impl)
    : AvailabilityManager(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
