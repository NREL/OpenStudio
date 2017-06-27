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

#include "AvailabilityManagerHybridVentilation.hpp"
#include "AvailabilityManagerHybridVentilation_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleDay.hpp"
#include "ScheduleDay_Impl.hpp"
#include "ScheduleRuleset.hpp"
#include "ScheduleRuleset_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/OS_AvailabilityManager_HybridVentilation_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AvailabilityManagerHybridVentilation_Impl::AvailabilityManagerHybridVentilation_Impl(const IdfObject& idfObject,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : AvailabilityManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AvailabilityManagerHybridVentilation::iddObjectType());
  }

  AvailabilityManagerHybridVentilation_Impl::AvailabilityManagerHybridVentilation_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : AvailabilityManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AvailabilityManagerHybridVentilation::iddObjectType());
  }

  AvailabilityManagerHybridVentilation_Impl::AvailabilityManagerHybridVentilation_Impl(const AvailabilityManagerHybridVentilation_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : AvailabilityManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AvailabilityManagerHybridVentilation_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AvailabilityManagerHybridVentilation_Impl::iddObjectType() const {
    return AvailabilityManagerHybridVentilation::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AvailabilityManagerHybridVentilation_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AvailabilityManager_HybridVentilationFields::VentilationControlModeSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("AvailabilityManagerHybridVentilation","Ventilation Control Mode Schedule"));
    }
    if (std::find(b,e,OS_AvailabilityManager_HybridVentilationFields::MinimumOutdoorVentilationAirSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("AvailabilityManagerHybridVentilation","Minimum Outdoor Ventilation Air Schedule"));
    }
    if (std::find(b,e,OS_AvailabilityManager_HybridVentilationFields::AirflowNetworkControlTypeSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("AvailabilityManagerHybridVentilation","AirflowNetwork Control Type Schedule"));
    }
    if (std::find(b,e,OS_AvailabilityManager_HybridVentilationFields::SimpleAirflowControlTypeSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("AvailabilityManagerHybridVentilation","Simple Airflow Control Type Schedule"));
    }
    return result;
  }

  boost::optional<ThermalZone> AvailabilityManagerHybridVentilation_Impl::controlledZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_AvailabilityManager_HybridVentilationFields::ControlledZone);
  }

  Schedule AvailabilityManagerHybridVentilation_Impl::ventilationControlModeSchedule() const {
    boost::optional<Schedule> value = optionalVentilationControlModeSchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Ventilation Control Mode Schedule attached.");
    }
    return value.get();
  }

  bool AvailabilityManagerHybridVentilation_Impl::useWeatherFileRainIndicators() const {
    boost::optional<std::string> value = getString(OS_AvailabilityManager_HybridVentilationFields::UseWeatherFileRainIndicators,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  double AvailabilityManagerHybridVentilation_Impl::maximumWindSpeed() const {
    boost::optional<double> value = getDouble(OS_AvailabilityManager_HybridVentilationFields::MaximumWindSpeed,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AvailabilityManagerHybridVentilation_Impl::minimumOutdoorTemperature() const {
    boost::optional<double> value = getDouble(OS_AvailabilityManager_HybridVentilationFields::MinimumOutdoorTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AvailabilityManagerHybridVentilation_Impl::maximumOutdoorTemperature() const {
    boost::optional<double> value = getDouble(OS_AvailabilityManager_HybridVentilationFields::MaximumOutdoorTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AvailabilityManagerHybridVentilation_Impl::minimumOutdoorEnthalpy() const {
    boost::optional<double> value = getDouble(OS_AvailabilityManager_HybridVentilationFields::MinimumOutdoorEnthalpy,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AvailabilityManagerHybridVentilation_Impl::maximumOutdoorEnthalpy() const {
    boost::optional<double> value = getDouble(OS_AvailabilityManager_HybridVentilationFields::MaximumOutdoorEnthalpy,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AvailabilityManagerHybridVentilation_Impl::minimumOutdoorDewpoint() const {
    boost::optional<double> value = getDouble(OS_AvailabilityManager_HybridVentilationFields::MinimumOutdoorDewpoint,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AvailabilityManagerHybridVentilation_Impl::maximumOutdoorDewpoint() const {
    boost::optional<double> value = getDouble(OS_AvailabilityManager_HybridVentilationFields::MaximumOutdoorDewpoint,true);
    OS_ASSERT(value);
    return value.get();
  }

  Schedule AvailabilityManagerHybridVentilation_Impl::minimumOutdoorVentilationAirSchedule() const {
    auto result = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AvailabilityManager_HybridVentilationFields::MinimumOutdoorVentilationAirSchedule);
    OS_ASSERT(result);
    return result.get();
  }

  boost::optional<Curve> AvailabilityManagerHybridVentilation_Impl::openingFactorFunctionofWindSpeedCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AvailabilityManager_HybridVentilationFields::OpeningFactorFunctionofWindSpeedCurve);
  }

  bool AvailabilityManagerHybridVentilation_Impl::setControlledZone(const boost::optional<ThermalZone>& thermalZone) {
    bool result(false);
    if (thermalZone) {
      result = setPointer(OS_AvailabilityManager_HybridVentilationFields::ControlledZone, thermalZone.get().handle());
    }
    else {
      resetControlledZone();
      result = true;
    }
    return result;
  }

  void AvailabilityManagerHybridVentilation_Impl::resetControlledZone() {
    bool result = setString(OS_AvailabilityManager_HybridVentilationFields::ControlledZone, "");
    OS_ASSERT(result);
  }

  bool AvailabilityManagerHybridVentilation_Impl::setVentilationControlModeSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AvailabilityManager_HybridVentilationFields::VentilationControlModeSchedule,
                              "AvailabilityManagerHybridVentilation",
                              "Ventilation Control Mode Schedule",
                              schedule);
    return result;
  }

  void AvailabilityManagerHybridVentilation_Impl::setUseWeatherFileRainIndicators(bool useWeatherFileRainIndicators) {
    return setBooleanFieldValue(OS_AvailabilityManager_HybridVentilationFields::UseWeatherFileRainIndicators, useWeatherFileRainIndicators);
  }

  bool AvailabilityManagerHybridVentilation_Impl::setMaximumWindSpeed(double maximumWindSpeed) {
    bool result = setDouble(OS_AvailabilityManager_HybridVentilationFields::MaximumWindSpeed, maximumWindSpeed);
    return result;
  }

  bool AvailabilityManagerHybridVentilation_Impl::setMinimumOutdoorTemperature(double minimumOutdoorTemperature) {
    bool result = setDouble(OS_AvailabilityManager_HybridVentilationFields::MinimumOutdoorTemperature, minimumOutdoorTemperature);
    return result;
  }

  bool AvailabilityManagerHybridVentilation_Impl::setMaximumOutdoorTemperature(double maximumOutdoorTemperature) {
    bool result = setDouble(OS_AvailabilityManager_HybridVentilationFields::MaximumOutdoorTemperature, maximumOutdoorTemperature);
    return result;
  }

  bool AvailabilityManagerHybridVentilation_Impl::setMinimumOutdoorEnthalpy(double minimumOutdoorEnthalpy) {
    bool result = setDouble(OS_AvailabilityManager_HybridVentilationFields::MinimumOutdoorEnthalpy, minimumOutdoorEnthalpy);
    return result;
  }

  bool AvailabilityManagerHybridVentilation_Impl::setMaximumOutdoorEnthalpy(double maximumOutdoorEnthalpy) {
    bool result = setDouble(OS_AvailabilityManager_HybridVentilationFields::MaximumOutdoorEnthalpy, maximumOutdoorEnthalpy);
    return result;
  }

  bool AvailabilityManagerHybridVentilation_Impl::setMinimumOutdoorDewpoint(double minimumOutdoorDewpoint) {
    bool result = setDouble(OS_AvailabilityManager_HybridVentilationFields::MinimumOutdoorDewpoint, minimumOutdoorDewpoint);
    return result;
  }

  bool AvailabilityManagerHybridVentilation_Impl::setMaximumOutdoorDewpoint(double maximumOutdoorDewpoint) {
    bool result = setDouble(OS_AvailabilityManager_HybridVentilationFields::MaximumOutdoorDewpoint, maximumOutdoorDewpoint);
    return result;
  }

  bool AvailabilityManagerHybridVentilation_Impl::setMinimumOutdoorVentilationAirSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AvailabilityManager_HybridVentilationFields::MinimumOutdoorVentilationAirSchedule,
                              "AvailabilityManagerHybridVentilation",
                              "Minimum Outdoor Ventilation Air Schedule",
                              schedule);
    return result;
  }

  bool AvailabilityManagerHybridVentilation_Impl::setOpeningFactorFunctionofWindSpeedCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AvailabilityManager_HybridVentilationFields::OpeningFactorFunctionofWindSpeedCurve, curve.get().handle());
    }
    else {
      resetOpeningFactorFunctionofWindSpeedCurve();
      result = true;
    }
    return result;
  }

  void AvailabilityManagerHybridVentilation_Impl::resetOpeningFactorFunctionofWindSpeedCurve() {
    bool result = setString(OS_AvailabilityManager_HybridVentilationFields::OpeningFactorFunctionofWindSpeedCurve, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> AvailabilityManagerHybridVentilation_Impl::optionalVentilationControlModeSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AvailabilityManager_HybridVentilationFields::VentilationControlModeSchedule);
  }

} // detail

AvailabilityManagerHybridVentilation::AvailabilityManagerHybridVentilation(const Model& model)
  : AvailabilityManager(AvailabilityManagerHybridVentilation::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AvailabilityManagerHybridVentilation_Impl>());

  {
    ScheduleRuleset schedule(model);
    schedule.defaultDaySchedule().addValue(Time(0,24,0,0),1.0);
    setVentilationControlModeSchedule(schedule);
  }

  {
    ScheduleRuleset schedule(model);
    schedule.defaultDaySchedule().addValue(Time(0,24,0,0),0.0);
    setMinimumOutdoorVentilationAirSchedule(schedule);
  }

  setUseWeatherFileRainIndicators(true);
  setMaximumWindSpeed(40.0);
  setMinimumOutdoorTemperature(20.0);
  setMaximumOutdoorTemperature(30.0);
  setMinimumOutdoorEnthalpy(20000.0);
  setMaximumOutdoorEnthalpy(30000.0);
  setMinimumOutdoorDewpoint(15.0);
  setMaximumOutdoorDewpoint(30.0);
}

AvailabilityManagerHybridVentilation::AvailabilityManagerHybridVentilation(const Model& model, Schedule& ventilationControlModeSchedule, Schedule& minimumOutdoorVentilationAirSchedule)
  : AvailabilityManager(AvailabilityManagerHybridVentilation::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AvailabilityManagerHybridVentilation_Impl>());

  setVentilationControlModeSchedule(ventilationControlModeSchedule);
  setMinimumOutdoorVentilationAirSchedule(minimumOutdoorVentilationAirSchedule);

  setUseWeatherFileRainIndicators(true);
  setMaximumWindSpeed(40.0);
  setMinimumOutdoorTemperature(-100.0);
  setMaximumOutdoorTemperature(100.0);
  setMinimumOutdoorEnthalpy(0.0);
  setMaximumOutdoorEnthalpy(300000.0);
  setMinimumOutdoorDewpoint(-100.0);
  setMaximumOutdoorDewpoint(100.0);
}

IddObjectType AvailabilityManagerHybridVentilation::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AvailabilityManager_HybridVentilation);
}

boost::optional<ThermalZone> AvailabilityManagerHybridVentilation::controlledZone() const {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->controlledZone();
}

Schedule AvailabilityManagerHybridVentilation::ventilationControlModeSchedule() const {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->ventilationControlModeSchedule();
}

bool AvailabilityManagerHybridVentilation::useWeatherFileRainIndicators() const {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->useWeatherFileRainIndicators();
}

double AvailabilityManagerHybridVentilation::maximumWindSpeed() const {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->maximumWindSpeed();
}

double AvailabilityManagerHybridVentilation::minimumOutdoorTemperature() const {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->minimumOutdoorTemperature();
}

double AvailabilityManagerHybridVentilation::maximumOutdoorTemperature() const {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->maximumOutdoorTemperature();
}

double AvailabilityManagerHybridVentilation::minimumOutdoorEnthalpy() const {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->minimumOutdoorEnthalpy();
}

double AvailabilityManagerHybridVentilation::maximumOutdoorEnthalpy() const {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->maximumOutdoorEnthalpy();
}

double AvailabilityManagerHybridVentilation::minimumOutdoorDewpoint() const {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->minimumOutdoorDewpoint();
}

double AvailabilityManagerHybridVentilation::maximumOutdoorDewpoint() const {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->maximumOutdoorDewpoint();
}

Schedule AvailabilityManagerHybridVentilation::minimumOutdoorVentilationAirSchedule() const {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->minimumOutdoorVentilationAirSchedule();
}

boost::optional<Curve> AvailabilityManagerHybridVentilation::openingFactorFunctionofWindSpeedCurve() const {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->openingFactorFunctionofWindSpeedCurve();
}

bool AvailabilityManagerHybridVentilation::setControlledZone(const ThermalZone& thermalZone) {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->setControlledZone(thermalZone);
}

void AvailabilityManagerHybridVentilation::resetControlledZone() {
  getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->resetControlledZone();
}

bool AvailabilityManagerHybridVentilation::setVentilationControlModeSchedule(Schedule& schedule) {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->setVentilationControlModeSchedule(schedule);
}

void AvailabilityManagerHybridVentilation::setUseWeatherFileRainIndicators(bool useWeatherFileRainIndicators) {
  getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->setUseWeatherFileRainIndicators(useWeatherFileRainIndicators);
}

bool AvailabilityManagerHybridVentilation::setMaximumWindSpeed(double maximumWindSpeed) {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->setMaximumWindSpeed(maximumWindSpeed);
}

bool AvailabilityManagerHybridVentilation::setMinimumOutdoorTemperature(double minimumOutdoorTemperature) {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->setMinimumOutdoorTemperature(minimumOutdoorTemperature);
}

bool AvailabilityManagerHybridVentilation::setMaximumOutdoorTemperature(double maximumOutdoorTemperature) {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->setMaximumOutdoorTemperature(maximumOutdoorTemperature);
}

bool AvailabilityManagerHybridVentilation::setMinimumOutdoorEnthalpy(double minimumOutdoorEnthalpy) {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->setMinimumOutdoorEnthalpy(minimumOutdoorEnthalpy);
}

bool AvailabilityManagerHybridVentilation::setMaximumOutdoorEnthalpy(double maximumOutdoorEnthalpy) {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->setMaximumOutdoorEnthalpy(maximumOutdoorEnthalpy);
}

bool AvailabilityManagerHybridVentilation::setMinimumOutdoorDewpoint(double minimumOutdoorDewpoint) {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->setMinimumOutdoorDewpoint(minimumOutdoorDewpoint);
}

bool AvailabilityManagerHybridVentilation::setMaximumOutdoorDewpoint(double maximumOutdoorDewpoint) {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->setMaximumOutdoorDewpoint(maximumOutdoorDewpoint);
}

bool AvailabilityManagerHybridVentilation::setMinimumOutdoorVentilationAirSchedule(Schedule& schedule) {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->setMinimumOutdoorVentilationAirSchedule(schedule);
}

bool AvailabilityManagerHybridVentilation::setOpeningFactorFunctionofWindSpeedCurve(const Curve& curve) {
  return getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->setOpeningFactorFunctionofWindSpeedCurve(curve);
}

void AvailabilityManagerHybridVentilation::resetOpeningFactorFunctionofWindSpeedCurve() {
  getImpl<detail::AvailabilityManagerHybridVentilation_Impl>()->resetOpeningFactorFunctionofWindSpeedCurve();
}

/// @cond
AvailabilityManagerHybridVentilation::AvailabilityManagerHybridVentilation(std::shared_ptr<detail::AvailabilityManagerHybridVentilation_Impl> impl)
  : AvailabilityManager(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

