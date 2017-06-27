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

#include "HeaderedPumpsConstantSpeed.hpp"
#include "HeaderedPumpsConstantSpeed_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_HeaderedPumps_ConstantSpeed_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  HeaderedPumpsConstantSpeed_Impl::HeaderedPumpsConstantSpeed_Impl(const IdfObject& idfObject,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == HeaderedPumpsConstantSpeed::iddObjectType());
  }

  HeaderedPumpsConstantSpeed_Impl::HeaderedPumpsConstantSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == HeaderedPumpsConstantSpeed::iddObjectType());
  }

  HeaderedPumpsConstantSpeed_Impl::HeaderedPumpsConstantSpeed_Impl(const HeaderedPumpsConstantSpeed_Impl& other,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& HeaderedPumpsConstantSpeed_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType HeaderedPumpsConstantSpeed_Impl::iddObjectType() const {
    return HeaderedPumpsConstantSpeed::iddObjectType();
  }

  std::vector<ScheduleTypeKey> HeaderedPumpsConstantSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_HeaderedPumps_ConstantSpeedFields::PumpFlowRateSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("HeaderedPumpsConstantSpeed","Pump Flow Rate Schedule"));
    }
    return result;
  }

  boost::optional<double> HeaderedPumpsConstantSpeed_Impl::totalRatedFlowRate() const {
    return getDouble(OS_HeaderedPumps_ConstantSpeedFields::TotalRatedFlowRate,true);
  }

  bool HeaderedPumpsConstantSpeed_Impl::isTotalRatedFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_HeaderedPumps_ConstantSpeedFields::TotalRatedFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  int HeaderedPumpsConstantSpeed_Impl::numberofPumpsinBank() const {
    boost::optional<int> value = getInt(OS_HeaderedPumps_ConstantSpeedFields::NumberofPumpsinBank,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string HeaderedPumpsConstantSpeed_Impl::flowSequencingControlScheme() const {
    boost::optional<std::string> value = getString(OS_HeaderedPumps_ConstantSpeedFields::FlowSequencingControlScheme,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeaderedPumpsConstantSpeed_Impl::ratedPumpHead() const {
    boost::optional<double> value = getDouble(OS_HeaderedPumps_ConstantSpeedFields::RatedPumpHead,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> HeaderedPumpsConstantSpeed_Impl::ratedPowerConsumption() const {
    return getDouble(OS_HeaderedPumps_ConstantSpeedFields::RatedPowerConsumption,true);
  }

  bool HeaderedPumpsConstantSpeed_Impl::isRatedPowerConsumptionAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_HeaderedPumps_ConstantSpeedFields::RatedPowerConsumption, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double HeaderedPumpsConstantSpeed_Impl::motorEfficiency() const {
    boost::optional<double> value = getDouble(OS_HeaderedPumps_ConstantSpeedFields::MotorEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeaderedPumpsConstantSpeed_Impl::fractionofMotorInefficienciestoFluidStream() const {
    boost::optional<double> value = getDouble(OS_HeaderedPumps_ConstantSpeedFields::FractionofMotorInefficienciestoFluidStream,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string HeaderedPumpsConstantSpeed_Impl::pumpControlType() const {
    boost::optional<std::string> value = getString(OS_HeaderedPumps_ConstantSpeedFields::PumpControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> HeaderedPumpsConstantSpeed_Impl::pumpFlowRateSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_HeaderedPumps_ConstantSpeedFields::PumpFlowRateSchedule);
  }

  boost::optional<ThermalZone> HeaderedPumpsConstantSpeed_Impl::thermalZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_HeaderedPumps_ConstantSpeedFields::ThermalZone);
  }

  double HeaderedPumpsConstantSpeed_Impl::skinLossRadiativeFraction() const {
    boost::optional<double> value = getDouble(OS_HeaderedPumps_ConstantSpeedFields::SkinLossRadiativeFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool HeaderedPumpsConstantSpeed_Impl::setTotalRatedFlowRate(boost::optional<double> totalRatedFlowRate) {
    bool result(false);
    if (totalRatedFlowRate) {
      result = setDouble(OS_HeaderedPumps_ConstantSpeedFields::TotalRatedFlowRate, totalRatedFlowRate.get());
    }
    return result;
  }

  void HeaderedPumpsConstantSpeed_Impl::autosizeTotalRatedFlowRate() {
    bool result = setString(OS_HeaderedPumps_ConstantSpeedFields::TotalRatedFlowRate, "autosize");
    OS_ASSERT(result);
  }

  void HeaderedPumpsConstantSpeed_Impl::setNumberofPumpsinBank(int numberofPumpsinBank) {
    bool result = setInt(OS_HeaderedPumps_ConstantSpeedFields::NumberofPumpsinBank, numberofPumpsinBank);
    OS_ASSERT(result);
  }

  bool HeaderedPumpsConstantSpeed_Impl::setFlowSequencingControlScheme(std::string flowSequencingControlScheme) {
    bool result = setString(OS_HeaderedPumps_ConstantSpeedFields::FlowSequencingControlScheme, flowSequencingControlScheme);
    return result;
  }

  void HeaderedPumpsConstantSpeed_Impl::setRatedPumpHead(double ratedPumpHead) {
    bool result = setDouble(OS_HeaderedPumps_ConstantSpeedFields::RatedPumpHead, ratedPumpHead);
    OS_ASSERT(result);
  }

  void HeaderedPumpsConstantSpeed_Impl::setRatedPowerConsumption(boost::optional<double> ratedPowerConsumption) {
    bool result(false);
    if (ratedPowerConsumption) {
      result = setDouble(OS_HeaderedPumps_ConstantSpeedFields::RatedPowerConsumption, ratedPowerConsumption.get());
    }
    OS_ASSERT(result);
  }

  void HeaderedPumpsConstantSpeed_Impl::autosizeRatedPowerConsumption() {
    bool result = setString(OS_HeaderedPumps_ConstantSpeedFields::RatedPowerConsumption, "autosize");
    OS_ASSERT(result);
  }

  bool HeaderedPumpsConstantSpeed_Impl::setMotorEfficiency(double motorEfficiency) {
    bool result = setDouble(OS_HeaderedPumps_ConstantSpeedFields::MotorEfficiency, motorEfficiency);
    return result;
  }

  bool HeaderedPumpsConstantSpeed_Impl::setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream) {
    bool result = setDouble(OS_HeaderedPumps_ConstantSpeedFields::FractionofMotorInefficienciestoFluidStream, fractionofMotorInefficienciestoFluidStream);
    return result;
  }

  bool HeaderedPumpsConstantSpeed_Impl::setPumpControlType(std::string pumpControlType) {
    bool result = setString(OS_HeaderedPumps_ConstantSpeedFields::PumpControlType, pumpControlType);
    return result;
  }

  bool HeaderedPumpsConstantSpeed_Impl::setPumpFlowRateSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_HeaderedPumps_ConstantSpeedFields::PumpFlowRateSchedule,
                              "HeaderedPumpsConstantSpeed",
                              "Pump Flow Rate Schedule",
                              schedule);
    return result;
  }

  void HeaderedPumpsConstantSpeed_Impl::resetPumpFlowRateSchedule() {
    bool result = setString(OS_HeaderedPumps_ConstantSpeedFields::PumpFlowRateSchedule, "");
    OS_ASSERT(result);
  }

  bool HeaderedPumpsConstantSpeed_Impl::setThermalZone(const boost::optional<ThermalZone>& thermalZone) {
    bool result(false);
    if (thermalZone) {
      result = setPointer(OS_HeaderedPumps_ConstantSpeedFields::ThermalZone, thermalZone.get().handle());
    }
    else {
      resetThermalZone();
      result = true;
    }
    return result;
  }

  void HeaderedPumpsConstantSpeed_Impl::resetThermalZone() {
    bool result = setString(OS_HeaderedPumps_ConstantSpeedFields::ThermalZone, "");
    OS_ASSERT(result);
  }

  bool HeaderedPumpsConstantSpeed_Impl::setSkinLossRadiativeFraction(double skinLossRadiativeFraction) {
    bool result = setDouble(OS_HeaderedPumps_ConstantSpeedFields::SkinLossRadiativeFraction, skinLossRadiativeFraction);
    return result;
  }

  unsigned HeaderedPumpsConstantSpeed_Impl::inletPort() {
    return OS_HeaderedPumps_ConstantSpeedFields::InletNodeName;
  }

  unsigned HeaderedPumpsConstantSpeed_Impl::outletPort() {
    return OS_HeaderedPumps_ConstantSpeedFields::OutletNodeName;
  }

  bool HeaderedPumpsConstantSpeed_Impl::addToNode(Node & node)
  {
    if( boost::optional<PlantLoop> plant = node.plantLoop() ) {
      return StraightComponent_Impl::addToNode(node);
    }

    return false;
  }

} // detail

HeaderedPumpsConstantSpeed::HeaderedPumpsConstantSpeed(const Model& model)
  : StraightComponent(HeaderedPumpsConstantSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::HeaderedPumpsConstantSpeed_Impl>());

  autosizeTotalRatedFlowRate();
  setNumberofPumpsinBank(2);
  setFlowSequencingControlScheme("Sequential");
  setRatedPumpHead(179352);
  autosizeRatedPowerConsumption();
  setMotorEfficiency(0.9);
  setFractionofMotorInefficienciestoFluidStream(0.0);
  setPumpControlType("Continuous");
  setSkinLossRadiativeFraction(0.1);
}

IddObjectType HeaderedPumpsConstantSpeed::iddObjectType() {
  return IddObjectType(IddObjectType::OS_HeaderedPumps_ConstantSpeed);
}

std::vector<std::string> HeaderedPumpsConstantSpeed::flowSequencingControlSchemeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_HeaderedPumps_ConstantSpeedFields::FlowSequencingControlScheme);
}

std::vector<std::string> HeaderedPumpsConstantSpeed::pumpControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_HeaderedPumps_ConstantSpeedFields::PumpControlType);
}

boost::optional<double> HeaderedPumpsConstantSpeed::totalRatedFlowRate() const {
  return getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->totalRatedFlowRate();
}

bool HeaderedPumpsConstantSpeed::isTotalRatedFlowRateAutosized() const {
  return getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->isTotalRatedFlowRateAutosized();
}

int HeaderedPumpsConstantSpeed::numberofPumpsinBank() const {
  return getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->numberofPumpsinBank();
}

std::string HeaderedPumpsConstantSpeed::flowSequencingControlScheme() const {
  return getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->flowSequencingControlScheme();
}

double HeaderedPumpsConstantSpeed::ratedPumpHead() const {
  return getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->ratedPumpHead();
}

boost::optional<double> HeaderedPumpsConstantSpeed::ratedPowerConsumption() const {
  return getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->ratedPowerConsumption();
}

bool HeaderedPumpsConstantSpeed::isRatedPowerConsumptionAutosized() const {
  return getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->isRatedPowerConsumptionAutosized();
}

double HeaderedPumpsConstantSpeed::motorEfficiency() const {
  return getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->motorEfficiency();
}

double HeaderedPumpsConstantSpeed::fractionofMotorInefficienciestoFluidStream() const {
  return getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->fractionofMotorInefficienciestoFluidStream();
}

std::string HeaderedPumpsConstantSpeed::pumpControlType() const {
  return getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->pumpControlType();
}

boost::optional<Schedule> HeaderedPumpsConstantSpeed::pumpFlowRateSchedule() const {
  return getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->pumpFlowRateSchedule();
}

boost::optional<ThermalZone> HeaderedPumpsConstantSpeed::thermalZone() const {
  return getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->thermalZone();
}

double HeaderedPumpsConstantSpeed::skinLossRadiativeFraction() const {
  return getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->skinLossRadiativeFraction();
}

bool HeaderedPumpsConstantSpeed::setTotalRatedFlowRate(double totalRatedFlowRate) {
  return getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->setTotalRatedFlowRate(totalRatedFlowRate);
}

void HeaderedPumpsConstantSpeed::autosizeTotalRatedFlowRate() {
  getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->autosizeTotalRatedFlowRate();
}

void HeaderedPumpsConstantSpeed::setNumberofPumpsinBank(int numberofPumpsinBank) {
  getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->setNumberofPumpsinBank(numberofPumpsinBank);
}

bool HeaderedPumpsConstantSpeed::setFlowSequencingControlScheme(std::string flowSequencingControlScheme) {
  return getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->setFlowSequencingControlScheme(flowSequencingControlScheme);
}

void HeaderedPumpsConstantSpeed::setRatedPumpHead(double ratedPumpHead) {
  getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->setRatedPumpHead(ratedPumpHead);
}

void HeaderedPumpsConstantSpeed::setRatedPowerConsumption(double ratedPowerConsumption) {
  getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->setRatedPowerConsumption(ratedPowerConsumption);
}

void HeaderedPumpsConstantSpeed::autosizeRatedPowerConsumption() {
  getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->autosizeRatedPowerConsumption();
}

bool HeaderedPumpsConstantSpeed::setMotorEfficiency(double motorEfficiency) {
  return getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->setMotorEfficiency(motorEfficiency);
}

bool HeaderedPumpsConstantSpeed::setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream) {
  return getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->setFractionofMotorInefficienciestoFluidStream(fractionofMotorInefficienciestoFluidStream);
}

bool HeaderedPumpsConstantSpeed::setPumpControlType(std::string pumpControlType) {
  return getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->setPumpControlType(pumpControlType);
}

bool HeaderedPumpsConstantSpeed::setPumpFlowRateSchedule(Schedule& schedule) {
  return getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->setPumpFlowRateSchedule(schedule);
}

void HeaderedPumpsConstantSpeed::resetPumpFlowRateSchedule() {
  getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->resetPumpFlowRateSchedule();
}

bool HeaderedPumpsConstantSpeed::setThermalZone(const ThermalZone& thermalZone) {
  return getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->setThermalZone(thermalZone);
}

void HeaderedPumpsConstantSpeed::resetThermalZone() {
  getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->resetThermalZone();
}

bool HeaderedPumpsConstantSpeed::setSkinLossRadiativeFraction(double skinLossRadiativeFraction) {
  return getImpl<detail::HeaderedPumpsConstantSpeed_Impl>()->setSkinLossRadiativeFraction(skinLossRadiativeFraction);
}

/// @cond
HeaderedPumpsConstantSpeed::HeaderedPumpsConstantSpeed(std::shared_ptr<detail::HeaderedPumpsConstantSpeed_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

