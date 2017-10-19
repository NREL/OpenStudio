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

#include "ZoneHVACEnergyRecoveryVentilatorController.hpp"
#include "ZoneHVACEnergyRecoveryVentilatorController_Impl.hpp"

#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ZoneHVAC_EnergyRecoveryVentilator_Controller_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneHVACEnergyRecoveryVentilatorController_Impl::ZoneHVACEnergyRecoveryVentilatorController_Impl(const IdfObject& idfObject,
                                                                                                   Model_Impl* model,
                                                                                                   bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneHVACEnergyRecoveryVentilatorController::iddObjectType());
  }

  ZoneHVACEnergyRecoveryVentilatorController_Impl::ZoneHVACEnergyRecoveryVentilatorController_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                   Model_Impl* model,
                                                                                                   bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneHVACEnergyRecoveryVentilatorController::iddObjectType());
  }

  ZoneHVACEnergyRecoveryVentilatorController_Impl::ZoneHVACEnergyRecoveryVentilatorController_Impl(const ZoneHVACEnergyRecoveryVentilatorController_Impl& other,
                                                                                                   Model_Impl* model,
                                                                                                   bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ZoneHVACEnergyRecoveryVentilatorController_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ZoneHVACEnergyRecoveryVentilatorController_Impl::iddObjectType() const {
    return ZoneHVACEnergyRecoveryVentilatorController::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneHVACEnergyRecoveryVentilatorController_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::TimeofDayEconomizerFlowControlScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACEnergyRecoveryVentilatorController","Time of Day Economizer Flow Control"));
    }
    return result;
  }

  std::vector<ModelObject> ZoneHVACEnergyRecoveryVentilatorController_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (auto const intermediate = electronicEnthalpyLimitCurve()) {
      result.push_back(intermediate.get());
    }
    return result;
  }

  boost::optional<double> ZoneHVACEnergyRecoveryVentilatorController_Impl::temperatureHighLimit() const {
    return getDouble(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::TemperatureHighLimit,true);
  }

  boost::optional<double> ZoneHVACEnergyRecoveryVentilatorController_Impl::temperatureLowLimit() const {
    return getDouble(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::TemperatureLowLimit,true);
  }

  boost::optional<double> ZoneHVACEnergyRecoveryVentilatorController_Impl::enthalpyHighLimit() const {
    return getDouble(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::EnthalpyHighLimit,true);
  }

  boost::optional<double> ZoneHVACEnergyRecoveryVentilatorController_Impl::dewpointTemperatureLimit() const {
    return getDouble(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::DewpointTemperatureLimit,true);
  }

  boost::optional<Curve> ZoneHVACEnergyRecoveryVentilatorController_Impl::electronicEnthalpyLimitCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::ElectronicEnthalpyLimitCurveName);
  }

  std::string ZoneHVACEnergyRecoveryVentilatorController_Impl::exhaustAirTemperatureLimit() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::ExhaustAirTemperatureLimit,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ZoneHVACEnergyRecoveryVentilatorController_Impl::exhaustAirEnthalpyLimit() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::ExhaustAirEnthalpyLimit,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> ZoneHVACEnergyRecoveryVentilatorController_Impl::timeofDayEconomizerFlowControlSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::TimeofDayEconomizerFlowControlScheduleName);
  }

  bool ZoneHVACEnergyRecoveryVentilatorController_Impl::highHumidityControlFlag() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::HighHumidityControlFlag,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  double ZoneHVACEnergyRecoveryVentilatorController_Impl::highHumidityOutdoorAirFlowRatio() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::HighHumidityOutdoorAirFlowRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACEnergyRecoveryVentilatorController_Impl::controlHighIndoorHumidityBasedonOutdoorHumidityRatio() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::ControlHighIndoorHumidityBasedonOutdoorHumidityRatio,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  void ZoneHVACEnergyRecoveryVentilatorController_Impl::setTemperatureHighLimit(boost::optional<double> temperatureHighLimit) {
    bool result(false);
    if (temperatureHighLimit) {
      result = setDouble(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::TemperatureHighLimit, temperatureHighLimit.get());
    }
    else {
      resetTemperatureHighLimit();
      result = true;
    }
    OS_ASSERT(result);
  }

  void ZoneHVACEnergyRecoveryVentilatorController_Impl::resetTemperatureHighLimit() {
    bool result = setString(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::TemperatureHighLimit, "");
    OS_ASSERT(result);
  }

  void ZoneHVACEnergyRecoveryVentilatorController_Impl::setTemperatureLowLimit(boost::optional<double> temperatureLowLimit) {
    bool result(false);
    if (temperatureLowLimit) {
      result = setDouble(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::TemperatureLowLimit, temperatureLowLimit.get());
    }
    else {
      resetTemperatureLowLimit();
      result = true;
    }
    OS_ASSERT(result);
  }

  void ZoneHVACEnergyRecoveryVentilatorController_Impl::resetTemperatureLowLimit() {
    bool result = setString(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::TemperatureLowLimit, "");
    OS_ASSERT(result);
  }

  void ZoneHVACEnergyRecoveryVentilatorController_Impl::setEnthalpyHighLimit(boost::optional<double> enthalpyHighLimit) {
    bool result(false);
    if (enthalpyHighLimit) {
      result = setDouble(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::EnthalpyHighLimit, enthalpyHighLimit.get());
    }
    else {
      resetEnthalpyHighLimit();
      result = true;
    }
    OS_ASSERT(result);
  }

  void ZoneHVACEnergyRecoveryVentilatorController_Impl::resetEnthalpyHighLimit() {
    bool result = setString(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::EnthalpyHighLimit, "");
    OS_ASSERT(result);
  }

  void ZoneHVACEnergyRecoveryVentilatorController_Impl::setDewpointTemperatureLimit(boost::optional<double> dewpointTemperatureLimit) {
    bool result(false);
    if (dewpointTemperatureLimit) {
      result = setDouble(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::DewpointTemperatureLimit, dewpointTemperatureLimit.get());
    }
    else {
      resetDewpointTemperatureLimit();
      result = true;
    }
    OS_ASSERT(result);
  }

  void ZoneHVACEnergyRecoveryVentilatorController_Impl::resetDewpointTemperatureLimit() {
    bool result = setString(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::DewpointTemperatureLimit, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACEnergyRecoveryVentilatorController_Impl::setElectronicEnthalpyLimitCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::ElectronicEnthalpyLimitCurveName, curve.get().handle());
    }
    else {
      resetElectronicEnthalpyLimitCurve();
      result = true;
    }
    return result;
  }

  void ZoneHVACEnergyRecoveryVentilatorController_Impl::resetElectronicEnthalpyLimitCurve() {
    bool result = setString(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::ElectronicEnthalpyLimitCurveName, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACEnergyRecoveryVentilatorController_Impl::setExhaustAirTemperatureLimit(std::string exhaustAirTemperatureLimit) {
    bool result = setString(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::ExhaustAirTemperatureLimit, exhaustAirTemperatureLimit);
    return result;
  }

  bool ZoneHVACEnergyRecoveryVentilatorController_Impl::setExhaustAirEnthalpyLimit(std::string exhaustAirEnthalpyLimit) {
    bool result = setString(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::ExhaustAirEnthalpyLimit, exhaustAirEnthalpyLimit);
    return result;
  }

  bool ZoneHVACEnergyRecoveryVentilatorController_Impl::setTimeofDayEconomizerFlowControlSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::TimeofDayEconomizerFlowControlScheduleName,
                              "ZoneHVACEnergyRecoveryVentilatorController",
                              "Time of Day Economizer Flow Control",
                              schedule);
    return result;
  }

  void ZoneHVACEnergyRecoveryVentilatorController_Impl::resetTimeofDayEconomizerFlowControlSchedule() {
    bool result = setString(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::TimeofDayEconomizerFlowControlScheduleName, "");
    OS_ASSERT(result);
  }

  void ZoneHVACEnergyRecoveryVentilatorController_Impl::setHighHumidityControlFlag(bool highHumidityControlFlag) {
    setBooleanFieldValue(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::HighHumidityControlFlag, highHumidityControlFlag);
  }

  bool ZoneHVACEnergyRecoveryVentilatorController_Impl::setHighHumidityOutdoorAirFlowRatio(double highHumidityOutdoorAirFlowRatio) {
    bool result = setDouble(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::HighHumidityOutdoorAirFlowRatio, highHumidityOutdoorAirFlowRatio);
    return result;
  }

  void ZoneHVACEnergyRecoveryVentilatorController_Impl::setControlHighIndoorHumidityBasedonOutdoorHumidityRatio(bool controlHighIndoorHumidityBasedonOutdoorHumidityRatio) {
    setBooleanFieldValue(OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::ControlHighIndoorHumidityBasedonOutdoorHumidityRatio, controlHighIndoorHumidityBasedonOutdoorHumidityRatio);
  }

} // detail

ZoneHVACEnergyRecoveryVentilatorController::ZoneHVACEnergyRecoveryVentilatorController(const Model& model)
  : ParentObject(ZoneHVACEnergyRecoveryVentilatorController::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>());

  bool ok = true;
  ok = setExhaustAirTemperatureLimit( "NoExhaustAirTemperatureLimit" );
  OS_ASSERT(ok);
  ok = setExhaustAirEnthalpyLimit( "NoExhaustAirEnthalpyLimit" );
  OS_ASSERT(ok);
  setHighHumidityControlFlag( false );
  ok = setHighHumidityOutdoorAirFlowRatio( 1.0 );
  OS_ASSERT(ok);
  setControlHighIndoorHumidityBasedonOutdoorHumidityRatio( true );
}

IddObjectType ZoneHVACEnergyRecoveryVentilatorController::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneHVAC_EnergyRecoveryVentilator_Controller);
}

std::vector<std::string> ZoneHVACEnergyRecoveryVentilatorController::exhaustAirTemperatureLimitValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::ExhaustAirTemperatureLimit);
}

std::vector<std::string> ZoneHVACEnergyRecoveryVentilatorController::exhaustAirEnthalpyLimitValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_EnergyRecoveryVentilator_ControllerFields::ExhaustAirEnthalpyLimit);
}

boost::optional<double> ZoneHVACEnergyRecoveryVentilatorController::temperatureHighLimit() const {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->temperatureHighLimit();
}

boost::optional<double> ZoneHVACEnergyRecoveryVentilatorController::temperatureLowLimit() const {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->temperatureLowLimit();
}

boost::optional<double> ZoneHVACEnergyRecoveryVentilatorController::enthalpyHighLimit() const {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->enthalpyHighLimit();
}

boost::optional<double> ZoneHVACEnergyRecoveryVentilatorController::dewpointTemperatureLimit() const {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->dewpointTemperatureLimit();
}

boost::optional<Curve> ZoneHVACEnergyRecoveryVentilatorController::electronicEnthalpyLimitCurve() const {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->electronicEnthalpyLimitCurve();
}

std::string ZoneHVACEnergyRecoveryVentilatorController::exhaustAirTemperatureLimit() const {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->exhaustAirTemperatureLimit();
}

std::string ZoneHVACEnergyRecoveryVentilatorController::exhaustAirEnthalpyLimit() const {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->exhaustAirEnthalpyLimit();
}

boost::optional<Schedule> ZoneHVACEnergyRecoveryVentilatorController::timeofDayEconomizerFlowControlSchedule() const {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->timeofDayEconomizerFlowControlSchedule();
}

bool ZoneHVACEnergyRecoveryVentilatorController::highHumidityControlFlag() const {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->highHumidityControlFlag();
}

double ZoneHVACEnergyRecoveryVentilatorController::highHumidityOutdoorAirFlowRatio() const {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->highHumidityOutdoorAirFlowRatio();
}

bool ZoneHVACEnergyRecoveryVentilatorController::controlHighIndoorHumidityBasedonOutdoorHumidityRatio() const {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->controlHighIndoorHumidityBasedonOutdoorHumidityRatio();
}

void ZoneHVACEnergyRecoveryVentilatorController::setTemperatureHighLimit(double temperatureHighLimit) {
  getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->setTemperatureHighLimit(temperatureHighLimit);
}

void ZoneHVACEnergyRecoveryVentilatorController::resetTemperatureHighLimit() {
  getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->resetTemperatureHighLimit();
}

void ZoneHVACEnergyRecoveryVentilatorController::setTemperatureLowLimit(double temperatureLowLimit) {
  getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->setTemperatureLowLimit(temperatureLowLimit);
}

void ZoneHVACEnergyRecoveryVentilatorController::resetTemperatureLowLimit() {
  getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->resetTemperatureLowLimit();
}

void ZoneHVACEnergyRecoveryVentilatorController::setEnthalpyHighLimit(double enthalpyHighLimit) {
  getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->setEnthalpyHighLimit(enthalpyHighLimit);
}

void ZoneHVACEnergyRecoveryVentilatorController::resetEnthalpyHighLimit() {
  getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->resetEnthalpyHighLimit();
}

void ZoneHVACEnergyRecoveryVentilatorController::setDewpointTemperatureLimit(double dewpointTemperatureLimit) {
  getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->setDewpointTemperatureLimit(dewpointTemperatureLimit);
}

void ZoneHVACEnergyRecoveryVentilatorController::resetDewpointTemperatureLimit() {
  getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->resetDewpointTemperatureLimit();
}

bool ZoneHVACEnergyRecoveryVentilatorController::setElectronicEnthalpyLimitCurve(const Curve& curve) {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->setElectronicEnthalpyLimitCurve(curve);
}

void ZoneHVACEnergyRecoveryVentilatorController::resetElectronicEnthalpyLimitCurve() {
  getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->resetElectronicEnthalpyLimitCurve();
}

bool ZoneHVACEnergyRecoveryVentilatorController::setExhaustAirTemperatureLimit(std::string exhaustAirTemperatureLimit) {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->setExhaustAirTemperatureLimit(exhaustAirTemperatureLimit);
}

bool ZoneHVACEnergyRecoveryVentilatorController::setExhaustAirEnthalpyLimit(std::string exhaustAirEnthalpyLimit) {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->setExhaustAirEnthalpyLimit(exhaustAirEnthalpyLimit);
}

bool ZoneHVACEnergyRecoveryVentilatorController::setTimeofDayEconomizerFlowControlSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->setTimeofDayEconomizerFlowControlSchedule(schedule);
}

void ZoneHVACEnergyRecoveryVentilatorController::resetTimeofDayEconomizerFlowControlSchedule() {
  getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->resetTimeofDayEconomizerFlowControlSchedule();
}

void ZoneHVACEnergyRecoveryVentilatorController::setHighHumidityControlFlag(bool highHumidityControlFlag) {
  getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->setHighHumidityControlFlag(highHumidityControlFlag);
}

bool ZoneHVACEnergyRecoveryVentilatorController::setHighHumidityOutdoorAirFlowRatio(double highHumidityOutdoorAirFlowRatio) {
  return getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->setHighHumidityOutdoorAirFlowRatio(highHumidityOutdoorAirFlowRatio);
}

void ZoneHVACEnergyRecoveryVentilatorController::setControlHighIndoorHumidityBasedonOutdoorHumidityRatio(bool controlHighIndoorHumidityBasedonOutdoorHumidityRatio) {
  getImpl<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl>()->setControlHighIndoorHumidityBasedonOutdoorHumidityRatio(controlHighIndoorHumidityBasedonOutdoorHumidityRatio);
}

/// @cond
ZoneHVACEnergyRecoveryVentilatorController::ZoneHVACEnergyRecoveryVentilatorController(std::shared_ptr<detail::ZoneHVACEnergyRecoveryVentilatorController_Impl> impl)
  : ParentObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

