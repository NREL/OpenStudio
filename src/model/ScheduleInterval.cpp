/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
      : Schedule_Impl(idfObject, model, keepHandle) {}

    ScheduleInterval_Impl::ScheduleInterval_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : Schedule_Impl(other, model, keepHandle) {}

    ScheduleInterval_Impl::ScheduleInterval_Impl(const ScheduleInterval_Impl& other, Model_Impl* model, bool keepHandle)
      : Schedule_Impl(other, model, keepHandle) {}

    // Get all output variable names that could be associated with this object.
    const std::vector<std::string>& ScheduleInterval_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    // return the parent object in the hierarchy
    boost::optional<ParentObject> ScheduleInterval_Impl::parent() const {
      return {};
    }

    // return any children objects in the hierarchy
    std::vector<ModelObject> ScheduleInterval_Impl::children() const {
      std::vector<ModelObject> result;
      return result;
    }

    std::vector<double> ScheduleInterval_Impl::values() const {
      return toStandardVector(timeSeries().values());
    }

  }  // namespace detail

  boost::optional<ScheduleInterval> ScheduleInterval::fromTimeSeries(const openstudio::TimeSeries& timeSeries, Model& model) {
    boost::optional<ScheduleInterval> result;
    if (timeSeries.intervalLength()) {
      result = ScheduleFixedInterval(model);
      if (!result->setTimeSeries(timeSeries)) {
        result->remove();
        return {};
      }
    } else {
      result = ScheduleVariableInterval(model);
      if (!result->setTimeSeries(timeSeries)) {
        result->remove();
        return {};
      }
    }
    return result;
  }

  openstudio::TimeSeries ScheduleInterval::timeSeries() const {
    return getImpl<detail::ScheduleInterval_Impl>()->timeSeries();
  }

  bool ScheduleInterval::setTimeSeries(const openstudio::TimeSeries& timeSeries) {
    return getImpl<detail::ScheduleInterval_Impl>()->setTimeSeries(timeSeries);
  }

  // constructor
  ScheduleInterval::ScheduleInterval(IddObjectType type, const Model& model) : Schedule(type, model) {
    OS_ASSERT(getImpl<detail::ScheduleInterval_Impl>());
  }

  ScheduleInterval::ScheduleInterval(std::shared_ptr<detail::ScheduleInterval_Impl> impl) : Schedule(std::move(impl)) {
    OS_ASSERT(getImpl<detail::ScheduleInterval_Impl>());
  }

}  // namespace model
}  // namespace openstudio
