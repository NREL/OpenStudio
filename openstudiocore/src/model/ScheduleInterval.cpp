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

#include "ScheduleInterval.hpp"
#include "ScheduleInterval_Impl.hpp"
#include "ScheduleFixedInterval.hpp"
#include "ScheduleFixedInterval_Impl.hpp"
#include "ScheduleVariableInterval.hpp"
#include "ScheduleVariableInterval_Impl.hpp"
#include "Model.hpp"

#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeLimits_Impl.hpp"

#include "../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/OS_Schedule_Compact_FieldEnums.hxx>

#include "../utilities/data/TimeSeries.hpp"
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

  ScheduleInterval_Impl::ScheduleInterval_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : Schedule_Impl(idfObject, model, keepHandle)
  {}

  ScheduleInterval_Impl::ScheduleInterval_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : Schedule_Impl(other,model,keepHandle)
  {}

  ScheduleInterval_Impl::ScheduleInterval_Impl(const ScheduleInterval_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle)
    : Schedule_Impl(other,model,keepHandle)
  {}

  // Get all output variable names that could be associated with this object.
  const std::vector<std::string>& ScheduleInterval_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  // return the parent object in the hierarchy
  boost::optional<ParentObject> ScheduleInterval_Impl::parent() const
  {
    return boost::optional<ParentObject>();
  }

  // return any children objects in the hierarchy
  std::vector<ModelObject> ScheduleInterval_Impl::children() const
  {
    std::vector<ModelObject> result;
    return result;
  }

  std::vector<double> ScheduleInterval_Impl::values() const {
    return toStandardVector(timeSeries().values());
  }

} // detail

boost::optional<ScheduleInterval> ScheduleInterval::fromTimeSeries(const openstudio::TimeSeries& timeSeries, Model& model)
{
  boost::optional<ScheduleInterval> result;
  if (timeSeries.intervalLength()){
    result = ScheduleFixedInterval(model);
    if(!result->setTimeSeries(timeSeries)){
      result->remove();
      return boost::optional<ScheduleInterval>();
    }
  }else{
    result = ScheduleVariableInterval(model);
    if(!result->setTimeSeries(timeSeries)){
      result->remove();
      return boost::optional<ScheduleInterval>();
    }
  }
  return result;
}

openstudio::TimeSeries ScheduleInterval::timeSeries() const
{
  return getImpl<detail::ScheduleInterval_Impl>()->timeSeries();
}

bool ScheduleInterval::setTimeSeries(const openstudio::TimeSeries& timeSeries)
{
  return getImpl<detail::ScheduleInterval_Impl>()->setTimeSeries(timeSeries);
}

// constructor
ScheduleInterval::ScheduleInterval(IddObjectType type,const Model& model)
  : Schedule(type,model)
{
  OS_ASSERT(getImpl<detail::ScheduleInterval_Impl>());
}

ScheduleInterval::ScheduleInterval(std::shared_ptr<detail::ScheduleInterval_Impl> impl)
  : Schedule(std::move(impl))
{
  OS_ASSERT(getImpl<detail::ScheduleInterval_Impl>());
}

} // model
} // openstudio
