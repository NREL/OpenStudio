/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "AvailabilityManagerScheduled.hpp"
#include "AvailabilityManagerScheduled_Impl.hpp"

#include "Model.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_AvailabilityManager_Scheduled_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AvailabilityManagerScheduled_Impl::AvailabilityManagerScheduled_Impl(const IdfObject& idfObject,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : AvailabilityManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AvailabilityManagerScheduled::iddObjectType());
  }

  AvailabilityManagerScheduled_Impl::AvailabilityManagerScheduled_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : AvailabilityManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AvailabilityManagerScheduled::iddObjectType());
  }

  AvailabilityManagerScheduled_Impl::AvailabilityManagerScheduled_Impl(const AvailabilityManagerScheduled_Impl& other,
                                                                       Model_Impl* model,
                                                                       bool keepHandle)
    : AvailabilityManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AvailabilityManagerScheduled_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result{
      "Availability Manager Scheduled Control Status"
    };
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
      result.push_back(ScheduleTypeKey("AvailabilityManagerScheduled","Availability Manager Scheduled"));
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
    bool result = ModelObject_Impl::setSchedule(OS_AvailabilityManager_ScheduledFields::ScheduleName,
                                                "AvailabilityManagerScheduled",
                                                "Availability Manager Scheduled",
                                                schedule);
    return result;
  }

  boost::optional<Schedule> AvailabilityManagerScheduled_Impl::optionalSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AvailabilityManager_ScheduledFields::ScheduleName);
  }

} // detail

AvailabilityManagerScheduled::AvailabilityManagerScheduled(const Model& model)
  : AvailabilityManager(AvailabilityManagerScheduled::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AvailabilityManagerScheduled_Impl>());
  {
    auto schedule = model.alwaysOnDiscreteSchedule();
    setSchedule(schedule);
  }
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
  : AvailabilityManager(impl)
{}
/// @endcond

} // model
} // openstudio

