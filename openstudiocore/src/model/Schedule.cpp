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
  : ScheduleBase(std::move(impl))
{
  OS_ASSERT(getImpl<detail::Schedule_Impl>());
}

} // model
} // openstudio
