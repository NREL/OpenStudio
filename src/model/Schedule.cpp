/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"

#include "Model.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "ScheduleDay.hpp"
#include "ScheduleDay_Impl.hpp"

#include "../utilities/idf/ValidityReport.hpp"

#include "../utilities/core/Assert.hpp"

using openstudio::Handle;
using openstudio::OptionalHandle;
using openstudio::HandleVector;
using openstudio::IdfObject;
using openstudio::WorkspaceObject;
using openstudio::OptionalWorkspaceObject;
using openstudio::WorkspaceObjectVector;
using openstudio::Workspace;

namespace openstudio {
namespace model {

  namespace detail {

    // constructor
    Schedule_Impl::Schedule_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle) : ScheduleBase_Impl(idfObject, model, keepHandle) {}

    Schedule_Impl::Schedule_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ScheduleBase_Impl(other, model, keepHandle) {}

    Schedule_Impl::Schedule_Impl(const Schedule_Impl& other, Model_Impl* model, bool keepHandles) : ScheduleBase_Impl(other, model, keepHandles) {}

    bool Schedule_Impl::candidateIsCompatibleWithCurrentUse(const ScheduleTypeLimits& candidate) const {
      ModelObjectVector users = getObject<Schedule>().getModelObjectSources<ModelObject>();
      auto copyOfThis = getObject<Schedule>();
      for (const ModelObject& user : users) {
        std::vector<ScheduleTypeKey> keys = user.getScheduleTypeKeys(copyOfThis);
        // ETH@20120806 - Ideally would make the following OS_ASSERT, but is too
        // strict for now. (Too easy for a user to trigger this first, before a unit
        // test or app tester catches it.)
        // OS_ASSERT(!keys.empty());
        for (const ScheduleTypeKey& key : keys) {
          if (!isCompatible(key.className(), key.scheduleDisplayName(), candidate)) {
            return false;
          }
        }
      }
      return true;
    }

    bool Schedule_Impl::okToResetScheduleTypeLimits() const {
      // ok to zero out if all users are schedules
      auto copyOfThis = getObject<Schedule>();
      for (const ModelObject& user : getObject<Schedule>().getModelObjectSources<ModelObject>()) {
        if (!user.getScheduleTypeKeys(copyOfThis).empty()) {
          return false;
        }
      }
      return true;
    }

  }  // namespace detail

  Schedule::Schedule(IddObjectType type, const Model& model) : ScheduleBase(type, model) {
    OS_ASSERT(getImpl<detail::Schedule_Impl>());
  }

  // constructor from impl
  Schedule::Schedule(std::shared_ptr<detail::Schedule_Impl> impl) : ScheduleBase(std::move(impl)) {
    OS_ASSERT(getImpl<detail::Schedule_Impl>());
  }

}  // namespace model
}  // namespace openstudio
