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

#include "AvailabilityManagerNightVentilation.hpp"
#include "AvailabilityManagerNightVentilation_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleRuleset.hpp"
#include "ScheduleRuleset_Impl.hpp"
#include "ScheduleDay.hpp"
#include "ScheduleDay_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/OS_AvailabilityManager_NightVentilation_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AvailabilityManagerNightVentilation_Impl::AvailabilityManagerNightVentilation_Impl(const IdfObject& idfObject,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : AvailabilityManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AvailabilityManagerNightVentilation::iddObjectType());
  }

  AvailabilityManagerNightVentilation_Impl::AvailabilityManagerNightVentilation_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : AvailabilityManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AvailabilityManagerNightVentilation::iddObjectType());
  }

  AvailabilityManagerNightVentilation_Impl::AvailabilityManagerNightVentilation_Impl(const AvailabilityManagerNightVentilation_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : AvailabilityManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AvailabilityManagerNightVentilation_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AvailabilityManagerNightVentilation_Impl::iddObjectType() const {
    return AvailabilityManagerNightVentilation::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AvailabilityManagerNightVentilation_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AvailabilityManager_NightVentilationFields::ApplicabilitySchedule) != e)
    {
      result.push_back(ScheduleTypeKey("AvailabilityManagerNightVentilation","Applicability Schedule"));
    }
    if (std::find(b,e,OS_AvailabilityManager_NightVentilationFields::FanSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("AvailabilityManagerNightVentilation","Fan Schedule"));
    }
    if (std::find(b,e,OS_AvailabilityManager_NightVentilationFields::VentilationTemperatureSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("AvailabilityManagerNightVentilation","Ventilation Temperature Schedule"));
    }
    return result;
  }

  Schedule AvailabilityManagerNightVentilation_Impl::applicabilitySchedule() const {
    boost::optional<Schedule> value = optionalApplicabilitySchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Applicability Schedule attached.");
    }
    return value.get();
  }

  boost::optional<Schedule> AvailabilityManagerNightVentilation_Impl::ventilationTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AvailabilityManager_NightVentilationFields::VentilationTemperatureSchedule);
  }

  double AvailabilityManagerNightVentilation_Impl::ventilationTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_AvailabilityManager_NightVentilationFields::VentilationTemperatureDifference,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AvailabilityManagerNightVentilation_Impl::ventilationTemperatureLowLimit() const {
    boost::optional<double> value = getDouble(OS_AvailabilityManager_NightVentilationFields::VentilationTemperatureLowLimit,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AvailabilityManagerNightVentilation_Impl::nightVentingFlowFraction() const {
    boost::optional<double> value = getDouble(OS_AvailabilityManager_NightVentilationFields::NightVentingFlowFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<ThermalZone> AvailabilityManagerNightVentilation_Impl::controlZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_AvailabilityManager_NightVentilationFields::ControlZone);
  }

  bool AvailabilityManagerNightVentilation_Impl::setApplicabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AvailabilityManager_NightVentilationFields::ApplicabilitySchedule,
                              "AvailabilityManagerNightVentilation",
                              "Applicability Schedule",
                              schedule);
    return result;
  }

  bool AvailabilityManagerNightVentilation_Impl::setVentilationTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AvailabilityManager_NightVentilationFields::VentilationTemperatureSchedule,
                              "AvailabilityManagerNightVentilation",
                              "Ventilation Temperature Schedule",
                              schedule);
    return result;
  }

  void AvailabilityManagerNightVentilation_Impl::resetVentilationTemperatureSchedule() {
    bool result = setString(OS_AvailabilityManager_NightVentilationFields::VentilationTemperatureSchedule, "");
    OS_ASSERT(result);
  }

  void AvailabilityManagerNightVentilation_Impl::setVentilationTemperatureDifference(double ventilationTemperatureDifference) {
    bool result = setDouble(OS_AvailabilityManager_NightVentilationFields::VentilationTemperatureDifference, ventilationTemperatureDifference);
    OS_ASSERT(result);
  }

  void AvailabilityManagerNightVentilation_Impl::setVentilationTemperatureLowLimit(double ventilationTemperatureLowLimit) {
    bool result = setDouble(OS_AvailabilityManager_NightVentilationFields::VentilationTemperatureLowLimit, ventilationTemperatureLowLimit);
    OS_ASSERT(result);
  }

  bool AvailabilityManagerNightVentilation_Impl::setNightVentingFlowFraction(double nightVentingFlowFraction) {
    bool result = setDouble(OS_AvailabilityManager_NightVentilationFields::NightVentingFlowFraction, nightVentingFlowFraction);
    return result;
  }

  bool AvailabilityManagerNightVentilation_Impl::setControlZone(const boost::optional<ThermalZone>& thermalZone) {
    bool result(false);
    if (thermalZone) {
      result = setPointer(OS_AvailabilityManager_NightVentilationFields::ControlZone, thermalZone.get().handle());
    }
    else {
      resetControlZone();
      result = true;
    }
    return result;
  }

  void AvailabilityManagerNightVentilation_Impl::resetControlZone() {
    bool result = setString(OS_AvailabilityManager_NightVentilationFields::ControlZone, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> AvailabilityManagerNightVentilation_Impl::optionalApplicabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AvailabilityManager_NightVentilationFields::ApplicabilitySchedule);
  }

} // detail

AvailabilityManagerNightVentilation::AvailabilityManagerNightVentilation(const Model& model)
  : AvailabilityManager(AvailabilityManagerNightVentilation::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AvailabilityManagerNightVentilation_Impl>());

  {
    auto schedule = model.alwaysOnDiscreteSchedule();
    setApplicabilitySchedule(schedule);
  }

  setVentilationTemperatureDifference(2.0);
  setVentilationTemperatureLowLimit(15.0);
  setNightVentingFlowFraction(0.333);
}

IddObjectType AvailabilityManagerNightVentilation::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AvailabilityManager_NightVentilation);
}

Schedule AvailabilityManagerNightVentilation::applicabilitySchedule() const {
  return getImpl<detail::AvailabilityManagerNightVentilation_Impl>()->applicabilitySchedule();
}

boost::optional<Schedule> AvailabilityManagerNightVentilation::ventilationTemperatureSchedule() const {
  return getImpl<detail::AvailabilityManagerNightVentilation_Impl>()->ventilationTemperatureSchedule();
}

double AvailabilityManagerNightVentilation::ventilationTemperatureDifference() const {
  return getImpl<detail::AvailabilityManagerNightVentilation_Impl>()->ventilationTemperatureDifference();
}

double AvailabilityManagerNightVentilation::ventilationTemperatureLowLimit() const {
  return getImpl<detail::AvailabilityManagerNightVentilation_Impl>()->ventilationTemperatureLowLimit();
}

double AvailabilityManagerNightVentilation::nightVentingFlowFraction() const {
  return getImpl<detail::AvailabilityManagerNightVentilation_Impl>()->nightVentingFlowFraction();
}

boost::optional<ThermalZone> AvailabilityManagerNightVentilation::controlZone() const {
  return getImpl<detail::AvailabilityManagerNightVentilation_Impl>()->controlZone();
}

bool AvailabilityManagerNightVentilation::setApplicabilitySchedule(Schedule& schedule) {
  return getImpl<detail::AvailabilityManagerNightVentilation_Impl>()->setApplicabilitySchedule(schedule);
}

bool AvailabilityManagerNightVentilation::setVentilationTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::AvailabilityManagerNightVentilation_Impl>()->setVentilationTemperatureSchedule(schedule);
}

void AvailabilityManagerNightVentilation::resetVentilationTemperatureSchedule() {
  getImpl<detail::AvailabilityManagerNightVentilation_Impl>()->resetVentilationTemperatureSchedule();
}

void AvailabilityManagerNightVentilation::setVentilationTemperatureDifference(double ventilationTemperatureDifference) {
  getImpl<detail::AvailabilityManagerNightVentilation_Impl>()->setVentilationTemperatureDifference(ventilationTemperatureDifference);
}

void AvailabilityManagerNightVentilation::setVentilationTemperatureLowLimit(double ventilationTemperatureLowLimit) {
  getImpl<detail::AvailabilityManagerNightVentilation_Impl>()->setVentilationTemperatureLowLimit(ventilationTemperatureLowLimit);
}

bool AvailabilityManagerNightVentilation::setNightVentingFlowFraction(double nightVentingFlowFraction) {
  return getImpl<detail::AvailabilityManagerNightVentilation_Impl>()->setNightVentingFlowFraction(nightVentingFlowFraction);
}

bool AvailabilityManagerNightVentilation::setControlZone(const ThermalZone& thermalZone) {
  return getImpl<detail::AvailabilityManagerNightVentilation_Impl>()->setControlZone(thermalZone);
}

void AvailabilityManagerNightVentilation::resetControlZone() {
  getImpl<detail::AvailabilityManagerNightVentilation_Impl>()->resetControlZone();
}

/// @cond
AvailabilityManagerNightVentilation::AvailabilityManagerNightVentilation(std::shared_ptr<detail::AvailabilityManagerNightVentilation_Impl> impl)
  : AvailabilityManager(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

