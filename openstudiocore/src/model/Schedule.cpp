/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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
  Schedule_Impl::Schedule_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ScheduleBase_Impl(idfObject, model, keepHandle)
  {}

  Schedule_Impl::Schedule_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                               Model_Impl* model,
                               bool keepHandle)
    : ScheduleBase_Impl(other, model,keepHandle)
  {}

  Schedule_Impl::Schedule_Impl(const Schedule_Impl& other, Model_Impl* model,bool keepHandles)
    : ScheduleBase_Impl(other, model,keepHandles)
  {}

  bool Schedule_Impl::candidateIsCompatibleWithCurrentUse(const ScheduleTypeLimits& candidate) const {
    ModelObjectVector users = getObject<Schedule>().getModelObjectSources<ModelObject>();
    Schedule copyOfThis = getObject<Schedule>();
    for (const ModelObject& user : users) {
      std::vector<ScheduleTypeKey> keys = user.getScheduleTypeKeys(copyOfThis);
      // ETH@20120806 - Ideally would make the following OS_ASSERT, but is too
      // strict for now. (Too easy for a user to trigger this first, before a unit
      // test or app tester catches it.)
      // OS_ASSERT(!keys.empty());
      for (const ScheduleTypeKey& key : keys) {
        if (!isCompatible(key.first,key.second,candidate)) {
          return false;
        }
      }
    }
    return true;
  }

  bool Schedule_Impl::okToResetScheduleTypeLimits() const {
    // ok to zero out if all users are schedules
    Schedule copyOfThis = getObject<Schedule>();
    for (const ModelObject& user : getObject<Schedule>().getModelObjectSources<ModelObject>()) {
      if (!user.getScheduleTypeKeys(copyOfThis).empty()) {
        return false;
      }
    }
    return true;
  }

} // detail

Schedule::Schedule(IddObjectType type,const Model& model)
  : ScheduleBase(type,model)
{
  OS_ASSERT(getImpl<detail::Schedule_Impl>());
}

// constructor from impl
Schedule::Schedule(std::shared_ptr<detail::Schedule_Impl> impl)
  : ScheduleBase(impl)
{
  OS_ASSERT(getImpl<detail::Schedule_Impl>());
}

} // model
} // openstudio
