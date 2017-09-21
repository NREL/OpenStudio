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

#include "AvailabilityManagerOptimumStart.hpp"
#include "AvailabilityManagerOptimumStart_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_AvailabilityManager_OptimumStart_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AvailabilityManagerOptimumStart_Impl::AvailabilityManagerOptimumStart_Impl(const IdfObject& idfObject,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : AvailabilityManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AvailabilityManagerOptimumStart::iddObjectType());
  }

  AvailabilityManagerOptimumStart_Impl::AvailabilityManagerOptimumStart_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : AvailabilityManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AvailabilityManagerOptimumStart::iddObjectType());
  }

  AvailabilityManagerOptimumStart_Impl::AvailabilityManagerOptimumStart_Impl(const AvailabilityManagerOptimumStart_Impl& other,
                                                                             Model_Impl* model,
                                                                             bool keepHandle)
    : AvailabilityManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AvailabilityManagerOptimumStart_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AvailabilityManagerOptimumStart_Impl::iddObjectType() const {
    return AvailabilityManagerOptimumStart::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AvailabilityManagerOptimumStart_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AvailabilityManager_OptimumStartFields::ApplicabilitySchedule) != e)
    {
      result.push_back(ScheduleTypeKey("AvailabilityManagerOptimumStart","Applicability Schedule"));
    }
    if (std::find(b,e,OS_AvailabilityManager_OptimumStartFields::FanSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("AvailabilityManagerOptimumStart","Fan Schedule"));
    }
    return result;
  }

  Schedule AvailabilityManagerOptimumStart_Impl::applicabilitySchedule() const {
    boost::optional<Schedule> value = optionalApplicabilitySchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Applicability Schedule attached.");
    }
    return value.get();
  }

  std::string AvailabilityManagerOptimumStart_Impl::controlType() const {
    boost::optional<std::string> value = getString(OS_AvailabilityManager_OptimumStartFields::ControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<ThermalZone> AvailabilityManagerOptimumStart_Impl::controlZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_AvailabilityManager_OptimumStartFields::ControlZone);
  }

  double AvailabilityManagerOptimumStart_Impl::maximumValueforOptimumStartTime() const {
    boost::optional<double> value = getDouble(OS_AvailabilityManager_OptimumStartFields::MaximumValueforOptimumStartTime,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string AvailabilityManagerOptimumStart_Impl::controlAlgorithm() const {
    boost::optional<std::string> value = getString(OS_AvailabilityManager_OptimumStartFields::ControlAlgorithm,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AvailabilityManagerOptimumStart_Impl::constantTemperatureGradientduringCooling() const {
    boost::optional<double> value = getDouble(OS_AvailabilityManager_OptimumStartFields::ConstantTemperatureGradientduringCooling,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AvailabilityManagerOptimumStart_Impl::constantTemperatureGradientduringHeating() const {
    boost::optional<double> value = getDouble(OS_AvailabilityManager_OptimumStartFields::ConstantTemperatureGradientduringHeating,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AvailabilityManagerOptimumStart_Impl::initialTemperatureGradientduringCooling() const {
    boost::optional<double> value = getDouble(OS_AvailabilityManager_OptimumStartFields::InitialTemperatureGradientduringCooling,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AvailabilityManagerOptimumStart_Impl::initialTemperatureGradientduringHeating() const {
    boost::optional<double> value = getDouble(OS_AvailabilityManager_OptimumStartFields::InitialTemperatureGradientduringHeating,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AvailabilityManagerOptimumStart_Impl::constantStartTime() const {
    boost::optional<double> value = getDouble(OS_AvailabilityManager_OptimumStartFields::ConstantStartTime,true);
    OS_ASSERT(value);
    return value.get();
  }

  int AvailabilityManagerOptimumStart_Impl::numberofPreviousDays() const {
    boost::optional<int> value = getInt(OS_AvailabilityManager_OptimumStartFields::NumberofPreviousDays,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AvailabilityManagerOptimumStart_Impl::setApplicabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AvailabilityManager_OptimumStartFields::ApplicabilitySchedule,
                              "AvailabilityManagerOptimumStart",
                              "Applicability Schedule",
                              schedule);
    return result;
  }

  bool AvailabilityManagerOptimumStart_Impl::setControlType(std::string controlType) {
    bool result = setString(OS_AvailabilityManager_OptimumStartFields::ControlType, controlType);
    return result;
  }

  bool AvailabilityManagerOptimumStart_Impl::setControlZone(const boost::optional<ThermalZone>& thermalZone) {
    bool result(false);
    if (thermalZone) {
      result = setPointer(OS_AvailabilityManager_OptimumStartFields::ControlZone, thermalZone.get().handle());
    }
    else {
      resetControlZone();
      result = true;
    }
    return result;
  }

  void AvailabilityManagerOptimumStart_Impl::resetControlZone() {
    bool result = setString(OS_AvailabilityManager_OptimumStartFields::ControlZone, "");
    OS_ASSERT(result);
  }

  void AvailabilityManagerOptimumStart_Impl::setMaximumValueforOptimumStartTime(double maximumValueforOptimumStartTime) {
    bool result = setDouble(OS_AvailabilityManager_OptimumStartFields::MaximumValueforOptimumStartTime, maximumValueforOptimumStartTime);
    OS_ASSERT(result);
  }

  bool AvailabilityManagerOptimumStart_Impl::setControlAlgorithm(std::string controlAlgorithm) {
    bool result = setString(OS_AvailabilityManager_OptimumStartFields::ControlAlgorithm, controlAlgorithm);
    return result;
  }

  void AvailabilityManagerOptimumStart_Impl::setConstantTemperatureGradientduringCooling(double constantTemperatureGradientduringCooling) {
    bool result = setDouble(OS_AvailabilityManager_OptimumStartFields::ConstantTemperatureGradientduringCooling, constantTemperatureGradientduringCooling);
    OS_ASSERT(result);
  }

  void AvailabilityManagerOptimumStart_Impl::setConstantTemperatureGradientduringHeating(double constantTemperatureGradientduringHeating) {
    bool result = setDouble(OS_AvailabilityManager_OptimumStartFields::ConstantTemperatureGradientduringHeating, constantTemperatureGradientduringHeating);
    OS_ASSERT(result);
  }

  void AvailabilityManagerOptimumStart_Impl::setInitialTemperatureGradientduringCooling(double initialTemperatureGradientduringCooling) {
    bool result = setDouble(OS_AvailabilityManager_OptimumStartFields::InitialTemperatureGradientduringCooling, initialTemperatureGradientduringCooling);
    OS_ASSERT(result);
  }

  void AvailabilityManagerOptimumStart_Impl::setInitialTemperatureGradientduringHeating(double initialTemperatureGradientduringHeating) {
    bool result = setDouble(OS_AvailabilityManager_OptimumStartFields::InitialTemperatureGradientduringHeating, initialTemperatureGradientduringHeating);
    OS_ASSERT(result);
  }

  void AvailabilityManagerOptimumStart_Impl::setConstantStartTime(double constantStartTime) {
    bool result = setDouble(OS_AvailabilityManager_OptimumStartFields::ConstantStartTime, constantStartTime);
    OS_ASSERT(result);
  }

  bool AvailabilityManagerOptimumStart_Impl::setNumberofPreviousDays(int numberofPreviousDays) {
    bool result = setInt(OS_AvailabilityManager_OptimumStartFields::NumberofPreviousDays, numberofPreviousDays);
    return result;
  }

  boost::optional<Schedule> AvailabilityManagerOptimumStart_Impl::optionalApplicabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AvailabilityManager_OptimumStartFields::ApplicabilitySchedule);
  }

} // detail

AvailabilityManagerOptimumStart::AvailabilityManagerOptimumStart(const Model& model)
  : AvailabilityManager(AvailabilityManagerOptimumStart::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AvailabilityManagerOptimumStart_Impl>());

  {
    auto schedule = model.alwaysOnDiscreteSchedule();
    setApplicabilitySchedule(schedule);
  }

  setControlType("MaximumofZoneList");
  setMaximumValueforOptimumStartTime(6.0);
  setControlAlgorithm("AdaptiveTemperatureGradient");
  setConstantTemperatureGradientduringCooling(3.0);
  setConstantTemperatureGradientduringHeating(3.0);
  setInitialTemperatureGradientduringCooling(2.0);
  setInitialTemperatureGradientduringHeating(2.0);
  setConstantStartTime(2.0);
  setNumberofPreviousDays(3);
}

IddObjectType AvailabilityManagerOptimumStart::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AvailabilityManager_OptimumStart);
}

std::vector<std::string> AvailabilityManagerOptimumStart::controlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_AvailabilityManager_OptimumStartFields::ControlType);
}

std::vector<std::string> AvailabilityManagerOptimumStart::controlAlgorithmValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_AvailabilityManager_OptimumStartFields::ControlAlgorithm);
}

Schedule AvailabilityManagerOptimumStart::applicabilitySchedule() const {
  return getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->applicabilitySchedule();
}

std::string AvailabilityManagerOptimumStart::controlType() const {
  return getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->controlType();
}

boost::optional<ThermalZone> AvailabilityManagerOptimumStart::controlZone() const {
  return getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->controlZone();
}

double AvailabilityManagerOptimumStart::maximumValueforOptimumStartTime() const {
  return getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->maximumValueforOptimumStartTime();
}

std::string AvailabilityManagerOptimumStart::controlAlgorithm() const {
  return getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->controlAlgorithm();
}

double AvailabilityManagerOptimumStart::constantTemperatureGradientduringCooling() const {
  return getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->constantTemperatureGradientduringCooling();
}

double AvailabilityManagerOptimumStart::constantTemperatureGradientduringHeating() const {
  return getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->constantTemperatureGradientduringHeating();
}

double AvailabilityManagerOptimumStart::initialTemperatureGradientduringCooling() const {
  return getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->initialTemperatureGradientduringCooling();
}

double AvailabilityManagerOptimumStart::initialTemperatureGradientduringHeating() const {
  return getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->initialTemperatureGradientduringHeating();
}

double AvailabilityManagerOptimumStart::constantStartTime() const {
  return getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->constantStartTime();
}

int AvailabilityManagerOptimumStart::numberofPreviousDays() const {
  return getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->numberofPreviousDays();
}

bool AvailabilityManagerOptimumStart::setApplicabilitySchedule(Schedule& schedule) {
  return getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->setApplicabilitySchedule(schedule);
}

bool AvailabilityManagerOptimumStart::setControlType(std::string controlType) {
  return getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->setControlType(controlType);
}

bool AvailabilityManagerOptimumStart::setControlZone(const ThermalZone& thermalZone) {
  return getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->setControlZone(thermalZone);
}

void AvailabilityManagerOptimumStart::resetControlZone() {
  getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->resetControlZone();
}

void AvailabilityManagerOptimumStart::setMaximumValueforOptimumStartTime(double maximumValueforOptimumStartTime) {
  getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->setMaximumValueforOptimumStartTime(maximumValueforOptimumStartTime);
}

bool AvailabilityManagerOptimumStart::setControlAlgorithm(std::string controlAlgorithm) {
  return getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->setControlAlgorithm(controlAlgorithm);
}

void AvailabilityManagerOptimumStart::setConstantTemperatureGradientduringCooling(double constantTemperatureGradientduringCooling) {
  getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->setConstantTemperatureGradientduringCooling(constantTemperatureGradientduringCooling);
}

void AvailabilityManagerOptimumStart::setConstantTemperatureGradientduringHeating(double constantTemperatureGradientduringHeating) {
  getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->setConstantTemperatureGradientduringHeating(constantTemperatureGradientduringHeating);
}

void AvailabilityManagerOptimumStart::setInitialTemperatureGradientduringCooling(double initialTemperatureGradientduringCooling) {
  getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->setInitialTemperatureGradientduringCooling(initialTemperatureGradientduringCooling);
}

void AvailabilityManagerOptimumStart::setInitialTemperatureGradientduringHeating(double initialTemperatureGradientduringHeating) {
  getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->setInitialTemperatureGradientduringHeating(initialTemperatureGradientduringHeating);
}

void AvailabilityManagerOptimumStart::setConstantStartTime(double constantStartTime) {
  getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->setConstantStartTime(constantStartTime);
}

bool AvailabilityManagerOptimumStart::setNumberofPreviousDays(int numberofPreviousDays) {
  return getImpl<detail::AvailabilityManagerOptimumStart_Impl>()->setNumberofPreviousDays(numberofPreviousDays);
}

/// @cond
AvailabilityManagerOptimumStart::AvailabilityManagerOptimumStart(std::shared_ptr<detail::AvailabilityManagerOptimumStart_Impl> impl)
  : AvailabilityManager(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

