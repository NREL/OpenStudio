/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/ScheduleInterval.hpp>
#include <model/ScheduleInterval_Impl.hpp>
#include <model/ScheduleFixedInterval.hpp>
#include <model/ScheduleFixedInterval_Impl.hpp>
#include <model/ScheduleVariableInterval.hpp>
#include <model/ScheduleVariableInterval_Impl.hpp>
#include <model/Model.hpp>

#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeLimits_Impl.hpp>

#include <utilities/idf/IdfExtensibleGroup.hpp>

#include <utilities/idd/OS_Schedule_Compact_FieldEnums.hxx>

#include <utilities/data/TimeSeries.hpp>
#include <utilities/core/Assert.hpp>

#include <boost/foreach.hpp>

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

ScheduleInterval::ScheduleInterval(boost::shared_ptr<detail::ScheduleInterval_Impl> impl)
  : Schedule(impl)
{
  OS_ASSERT(getImpl<detail::ScheduleInterval_Impl>());
}

} // model
} // openstudio
