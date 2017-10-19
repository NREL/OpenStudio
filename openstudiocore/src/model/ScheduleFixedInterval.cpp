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

#include "ScheduleFixedInterval.hpp"
#include "ScheduleFixedInterval_Impl.hpp"
#include "Model.hpp"
#include "ModelExtensibleGroup.hpp"

#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeLimits_Impl.hpp"

#include <utilities/idd/OS_Schedule_FixedInterval_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/time/DateTime.hpp"
#include "../utilities/data/TimeSeries.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ScheduleFixedInterval_Impl::ScheduleFixedInterval_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ScheduleInterval_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ScheduleFixedInterval::iddObjectType());
  }

  ScheduleFixedInterval_Impl::ScheduleFixedInterval_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : ScheduleInterval_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ScheduleFixedInterval::iddObjectType());
  }

  ScheduleFixedInterval_Impl::ScheduleFixedInterval_Impl(const ScheduleFixedInterval_Impl& other,
                                                         Model_Impl* model,
                                                         bool keepHandle)
    : ScheduleInterval_Impl(other,model,keepHandle)
  {}

  IddObjectType ScheduleFixedInterval_Impl::iddObjectType() const {
    return ScheduleFixedInterval::iddObjectType();
  }

  boost::optional<ScheduleTypeLimits> ScheduleFixedInterval_Impl::scheduleTypeLimits() const {
    return getObject<ModelObject>().getModelObjectTarget<ScheduleTypeLimits>(OS_Schedule_FixedIntervalFields::ScheduleTypeLimitsName);
  }

  bool ScheduleFixedInterval_Impl::setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) {
    if (scheduleTypeLimits.model() != model()) {
      return false;
    }
    if (!candidateIsCompatibleWithCurrentUse(scheduleTypeLimits)) {
      return false;
    }
    return setPointer(OS_Schedule_FixedIntervalFields::ScheduleTypeLimitsName,scheduleTypeLimits.handle());
  }

  bool ScheduleFixedInterval_Impl::resetScheduleTypeLimits() {
    // can only reset if not used
    if (okToResetScheduleTypeLimits()) {
      return setString(OS_Schedule_FixedIntervalFields::ScheduleTypeLimitsName,"");
    }
    return false;
  }

  openstudio::TimeSeries ScheduleFixedInterval_Impl::timeSeries() const
  {
    Date startDate(openstudio::MonthOfYear(this->startMonth()), this->startDay());
    Time intervalLength(0, 0, this->intervalLength());

    Vector values(this->numExtensibleGroups());
    unsigned i = 0;
    for (const ModelExtensibleGroup& group : castVector<ModelExtensibleGroup>(extensibleGroups()))
    {
      OptionalDouble x = group.getDouble(0);
      OS_ASSERT(x);
      values[i] = *x;
      ++i;
    }

    TimeSeries result(startDate, intervalLength, values, "");
    result.setOutOfRangeValue(this->outOfRangeValue());

    return result;
  }

  bool ScheduleFixedInterval_Impl::setTimeSeries(const openstudio::TimeSeries& timeSeries)
  {
    // check the interval
    boost::optional<openstudio::Time> intervalTime = timeSeries.intervalLength();
    if (!intervalTime){
      return false;
    }

    // check the interval
    double intervalLength = intervalTime->totalMinutes();
    if (intervalLength - floor(intervalLength) > 0){
      return false;
    }

    // check the interval
    if (intervalTime->totalDays() > 1){
      return false;
    }

    // check that first report is whole number of intervals from start date
    DateTime firstReportDateTime = timeSeries.firstReportDateTime();
    Date startDate = firstReportDateTime.date();
    Time firstReportTime = firstReportDateTime.time();

    double numIntervalsToFirstReport = std::max(1.0, firstReportTime.totalMinutes() / intervalLength);
    if (numIntervalsToFirstReport - floor(numIntervalsToFirstReport) > 0){
      return false;
    }

    // at this point we are going to change the object
    clearExtensibleGroups(false);

    // set the interval
    this->setIntervalLength(intervalLength, false);

    // set the start date
    this->setStartMonth(startDate.monthOfYear().value(), false);
    this->setStartDay(startDate.dayOfMonth(), false);

    // set the out of range value
    double outOfRangeValue = timeSeries.outOfRangeValue();

    // add in numIntervalsToFirstReport-1 outOfRangeValues to pad the timeseries
    for (unsigned i = 0; i < numIntervalsToFirstReport - 1; ++i){
      std::vector<std::string> temp;
      temp.push_back(toString(outOfRangeValue));

      ModelExtensibleGroup group = pushExtensibleGroup(temp, false).cast<ModelExtensibleGroup>();
      OS_ASSERT(!group.empty());
    }

    // set the values
    openstudio::Vector values = timeSeries.values();
    for (unsigned i = 0; i < values.size(); ++i){
      std::vector<std::string> temp;
      temp.push_back(toString(values[i]));

      ModelExtensibleGroup group = pushExtensibleGroup(temp, false).cast<ModelExtensibleGroup>();
      OS_ASSERT(!group.empty());
    }

    this->emitChangeSignals();

    return true;
  }

  bool ScheduleFixedInterval_Impl::interpolatetoTimestep() const {
    boost::optional<std::string> value = getString(OS_Schedule_FixedIntervalFields::InterpolatetoTimestep,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool ScheduleFixedInterval_Impl::isInterpolatetoTimestepDefaulted() const {
    return isEmpty(OS_Schedule_FixedIntervalFields::InterpolatetoTimestep);
  }

  int ScheduleFixedInterval_Impl::startMonth() const {
    boost::optional<int> value = getInt(OS_Schedule_FixedIntervalFields::StartMonth,true);
    OS_ASSERT(value);
    return value.get();
  }

  int ScheduleFixedInterval_Impl::startDay() const {
    boost::optional<int> value = getInt(OS_Schedule_FixedIntervalFields::StartDay,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ScheduleFixedInterval_Impl::intervalLength() const {
    boost::optional<double> value = getDouble(OS_Schedule_FixedIntervalFields::IntervalLength,true);
    if (value){
      return value.get();
    }
    return 0;
  }

  double ScheduleFixedInterval_Impl::outOfRangeValue() const {
    boost::optional<double> value = getDouble(OS_Schedule_FixedIntervalFields::OutOfRangeValue,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ScheduleFixedInterval_Impl::isOutOfRangeValueDefaulted() const {
    return isEmpty(OS_Schedule_FixedIntervalFields::OutOfRangeValue);
  }

  void ScheduleFixedInterval_Impl::setInterpolatetoTimestep(bool interpolatetoTimestep, bool driverMethod) {
    bool result = false;
    if (interpolatetoTimestep) {
      result = setString(OS_Schedule_FixedIntervalFields::InterpolatetoTimestep, "Yes", driverMethod);
    } else {
      result = setString(OS_Schedule_FixedIntervalFields::InterpolatetoTimestep, "No", driverMethod);
    }
    OS_ASSERT(result);
  }

  void ScheduleFixedInterval_Impl::resetInterpolatetoTimestep(bool driverMethod) {
    bool result = setString(OS_Schedule_FixedIntervalFields::InterpolatetoTimestep, "", driverMethod);
    OS_ASSERT(result);
  }

  bool ScheduleFixedInterval_Impl::setStartMonth(int startMonth, bool driverMethod) {
    bool result = setInt(OS_Schedule_FixedIntervalFields::StartMonth, startMonth, driverMethod);
    return result;
  }

  bool ScheduleFixedInterval_Impl::setStartDay(int startDay, bool driverMethod) {
    bool result = setInt(OS_Schedule_FixedIntervalFields::StartDay, startDay, driverMethod);
    return result;
  }

  bool ScheduleFixedInterval_Impl::setIntervalLength(double intervalLength, bool driverMethod) {
    bool result = setDouble(OS_Schedule_FixedIntervalFields::IntervalLength, intervalLength, driverMethod);
    return result;
  }

  void ScheduleFixedInterval_Impl::setOutOfRangeValue(double outOfRangeValue, bool driverMethod) {
    bool result = setDouble(OS_Schedule_FixedIntervalFields::OutOfRangeValue, outOfRangeValue, driverMethod);
    OS_ASSERT(result);
  }

  void ScheduleFixedInterval_Impl::resetOutOfRangeValue(bool driverMethod) {
    bool result = setString(OS_Schedule_FixedIntervalFields::OutOfRangeValue, "", driverMethod);
    OS_ASSERT(result);
  }

  void ScheduleFixedInterval_Impl::ensureNoLeapDays()
  {
    boost::optional<int> month;
    boost::optional<int> day;

    month = getInt(OS_Schedule_FixedIntervalFields::StartMonth);
    if (month && (month.get() == 2)){
      day = this->getInt(OS_Schedule_FixedIntervalFields::StartDay);
      if (day && (day.get() == 29)){
        this->setInt(OS_Schedule_FixedIntervalFields::StartDay, 28);
      }
    }
  }


} // detail

ScheduleFixedInterval::ScheduleFixedInterval(const Model& model)
  : ScheduleInterval(ScheduleFixedInterval::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ScheduleFixedInterval_Impl>());
}

IddObjectType ScheduleFixedInterval::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Schedule_FixedInterval);
  return result;
}

bool ScheduleFixedInterval::interpolatetoTimestep() const {
  return getImpl<detail::ScheduleFixedInterval_Impl>()->interpolatetoTimestep();
}

bool ScheduleFixedInterval::isInterpolatetoTimestepDefaulted() const {
  return getImpl<detail::ScheduleFixedInterval_Impl>()->isInterpolatetoTimestepDefaulted();
}

int ScheduleFixedInterval::startMonth() const {
  return getImpl<detail::ScheduleFixedInterval_Impl>()->startMonth();
}

int ScheduleFixedInterval::startDay() const {
  return getImpl<detail::ScheduleFixedInterval_Impl>()->startDay();
}

double ScheduleFixedInterval::intervalLength() const {
  return getImpl<detail::ScheduleFixedInterval_Impl>()->intervalLength();
}

double ScheduleFixedInterval::outOfRangeValue() const {
  return getImpl<detail::ScheduleFixedInterval_Impl>()->outOfRangeValue();
}

bool ScheduleFixedInterval::isOutOfRangeValueDefaulted() const {
  return getImpl<detail::ScheduleFixedInterval_Impl>()->isOutOfRangeValueDefaulted();
}

void ScheduleFixedInterval::setInterpolatetoTimestep(bool interpolatetoTimestep) {
  getImpl<detail::ScheduleFixedInterval_Impl>()->setInterpolatetoTimestep(interpolatetoTimestep);
}

void ScheduleFixedInterval::resetInterpolatetoTimestep() {
  getImpl<detail::ScheduleFixedInterval_Impl>()->resetInterpolatetoTimestep();
}

bool ScheduleFixedInterval::setStartMonth(int startMonth) {
  return getImpl<detail::ScheduleFixedInterval_Impl>()->setStartMonth(startMonth);
}

bool ScheduleFixedInterval::setStartDay(int startDay) {
  return getImpl<detail::ScheduleFixedInterval_Impl>()->setStartDay(startDay);
}

bool ScheduleFixedInterval::setIntervalLength(double intervalLength) {
  return getImpl<detail::ScheduleFixedInterval_Impl>()->setIntervalLength(intervalLength);
}

void ScheduleFixedInterval::setOutOfRangeValue(double outOfRangeValue) {
  getImpl<detail::ScheduleFixedInterval_Impl>()->setOutOfRangeValue(outOfRangeValue);
}

void ScheduleFixedInterval::resetOutOfRangeValue() {
  getImpl<detail::ScheduleFixedInterval_Impl>()->resetOutOfRangeValue();
}

/// @cond
ScheduleFixedInterval::ScheduleFixedInterval(std::shared_ptr<detail::ScheduleFixedInterval_Impl> impl)
  : ScheduleInterval(std::move(impl))
{}
/// @endcond


} // model
} // openstudio

