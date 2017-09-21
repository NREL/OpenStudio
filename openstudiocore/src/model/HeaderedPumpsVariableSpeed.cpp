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

#include "HeaderedPumpsVariableSpeed.hpp"
#include "HeaderedPumpsVariableSpeed_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_HeaderedPumps_VariableSpeed_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  HeaderedPumpsVariableSpeed_Impl::HeaderedPumpsVariableSpeed_Impl(const IdfObject& idfObject,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == HeaderedPumpsVariableSpeed::iddObjectType());
  }

  HeaderedPumpsVariableSpeed_Impl::HeaderedPumpsVariableSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == HeaderedPumpsVariableSpeed::iddObjectType());
  }

  HeaderedPumpsVariableSpeed_Impl::HeaderedPumpsVariableSpeed_Impl(const HeaderedPumpsVariableSpeed_Impl& other,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& HeaderedPumpsVariableSpeed_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType HeaderedPumpsVariableSpeed_Impl::iddObjectType() const {
    return HeaderedPumpsVariableSpeed::iddObjectType();
  }

  std::vector<ScheduleTypeKey> HeaderedPumpsVariableSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_HeaderedPumps_VariableSpeedFields::PumpFlowRateSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("HeaderedPumpsVariableSpeed","Pump Flow Rate Schedule"));
    }
    return result;
  }

  boost::optional<double> HeaderedPumpsVariableSpeed_Impl::totalRatedFlowRate() const {
    return getDouble(OS_HeaderedPumps_VariableSpeedFields::TotalRatedFlowRate,true);
  }

  bool HeaderedPumpsVariableSpeed_Impl::isTotalRatedFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_HeaderedPumps_VariableSpeedFields::TotalRatedFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  int HeaderedPumpsVariableSpeed_Impl::numberofPumpsinBank() const {
    boost::optional<int> value = getInt(OS_HeaderedPumps_VariableSpeedFields::NumberofPumpsinBank,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string HeaderedPumpsVariableSpeed_Impl::flowSequencingControlScheme() const {
    boost::optional<std::string> value = getString(OS_HeaderedPumps_VariableSpeedFields::FlowSequencingControlScheme,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeaderedPumpsVariableSpeed_Impl::ratedPumpHead() const {
    boost::optional<double> value = getDouble(OS_HeaderedPumps_VariableSpeedFields::RatedPumpHead,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> HeaderedPumpsVariableSpeed_Impl::ratedPowerConsumption() const {
    return getDouble(OS_HeaderedPumps_VariableSpeedFields::RatedPowerConsumption,true);
  }

  bool HeaderedPumpsVariableSpeed_Impl::isRatedPowerConsumptionAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_HeaderedPumps_VariableSpeedFields::RatedPowerConsumption, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double HeaderedPumpsVariableSpeed_Impl::motorEfficiency() const {
    boost::optional<double> value = getDouble(OS_HeaderedPumps_VariableSpeedFields::MotorEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeaderedPumpsVariableSpeed_Impl::fractionofMotorInefficienciestoFluidStream() const {
    boost::optional<double> value = getDouble(OS_HeaderedPumps_VariableSpeedFields::FractionofMotorInefficienciestoFluidStream,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeaderedPumpsVariableSpeed_Impl::coefficient1ofthePartLoadPerformanceCurve() const {
    boost::optional<double> value = getDouble(OS_HeaderedPumps_VariableSpeedFields::Coefficient1ofthePartLoadPerformanceCurve,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeaderedPumpsVariableSpeed_Impl::coefficient2ofthePartLoadPerformanceCurve() const {
    boost::optional<double> value = getDouble(OS_HeaderedPumps_VariableSpeedFields::Coefficient2ofthePartLoadPerformanceCurve,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeaderedPumpsVariableSpeed_Impl::coefficient3ofthePartLoadPerformanceCurve() const {
    boost::optional<double> value = getDouble(OS_HeaderedPumps_VariableSpeedFields::Coefficient3ofthePartLoadPerformanceCurve,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeaderedPumpsVariableSpeed_Impl::coefficient4ofthePartLoadPerformanceCurve() const {
    boost::optional<double> value = getDouble(OS_HeaderedPumps_VariableSpeedFields::Coefficient4ofthePartLoadPerformanceCurve,true);
    OS_ASSERT(value);
    return value.get();
  }

  double HeaderedPumpsVariableSpeed_Impl::minimumFlowRateFraction() const {
    boost::optional<double> value = getDouble(OS_HeaderedPumps_VariableSpeedFields::MinimumFlowRateFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string HeaderedPumpsVariableSpeed_Impl::pumpControlType() const {
    boost::optional<std::string> value = getString(OS_HeaderedPumps_VariableSpeedFields::PumpControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> HeaderedPumpsVariableSpeed_Impl::pumpFlowRateSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_HeaderedPumps_VariableSpeedFields::PumpFlowRateSchedule);
  }

  boost::optional<ThermalZone> HeaderedPumpsVariableSpeed_Impl::thermalZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_HeaderedPumps_VariableSpeedFields::ThermalZone);
  }

  double HeaderedPumpsVariableSpeed_Impl::skinLossRadiativeFraction() const {
    boost::optional<double> value = getDouble(OS_HeaderedPumps_VariableSpeedFields::SkinLossRadiativeFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool HeaderedPumpsVariableSpeed_Impl::setTotalRatedFlowRate(boost::optional<double> totalRatedFlowRate) {
    bool result(false);
    if (totalRatedFlowRate) {
      result = setDouble(OS_HeaderedPumps_VariableSpeedFields::TotalRatedFlowRate, totalRatedFlowRate.get());
    }
    return result;
  }

  void HeaderedPumpsVariableSpeed_Impl::autosizeTotalRatedFlowRate() {
    bool result = setString(OS_HeaderedPumps_VariableSpeedFields::TotalRatedFlowRate, "autosize");
    OS_ASSERT(result);
  }

  void HeaderedPumpsVariableSpeed_Impl::setNumberofPumpsinBank(int numberofPumpsinBank) {
    bool result = setInt(OS_HeaderedPumps_VariableSpeedFields::NumberofPumpsinBank, numberofPumpsinBank);
    OS_ASSERT(result);
  }

  bool HeaderedPumpsVariableSpeed_Impl::setFlowSequencingControlScheme(std::string flowSequencingControlScheme) {
    bool result = setString(OS_HeaderedPumps_VariableSpeedFields::FlowSequencingControlScheme, flowSequencingControlScheme);
    return result;
  }

  void HeaderedPumpsVariableSpeed_Impl::setRatedPumpHead(double ratedPumpHead) {
    bool result = setDouble(OS_HeaderedPumps_VariableSpeedFields::RatedPumpHead, ratedPumpHead);
    OS_ASSERT(result);
  }

  void HeaderedPumpsVariableSpeed_Impl::setRatedPowerConsumption(boost::optional<double> ratedPowerConsumption) {
    bool result(false);
    if (ratedPowerConsumption) {
      result = setDouble(OS_HeaderedPumps_VariableSpeedFields::RatedPowerConsumption, ratedPowerConsumption.get());
    }
    OS_ASSERT(result);
  }

  void HeaderedPumpsVariableSpeed_Impl::autosizeRatedPowerConsumption() {
    bool result = setString(OS_HeaderedPumps_VariableSpeedFields::RatedPowerConsumption, "autosize");
    OS_ASSERT(result);
  }

  bool HeaderedPumpsVariableSpeed_Impl::setMotorEfficiency(double motorEfficiency) {
    bool result = setDouble(OS_HeaderedPumps_VariableSpeedFields::MotorEfficiency, motorEfficiency);
    return result;
  }

  bool HeaderedPumpsVariableSpeed_Impl::setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream) {
    bool result = setDouble(OS_HeaderedPumps_VariableSpeedFields::FractionofMotorInefficienciestoFluidStream, fractionofMotorInefficienciestoFluidStream);
    return result;
  }

  void HeaderedPumpsVariableSpeed_Impl::setCoefficient1ofthePartLoadPerformanceCurve(double coefficient1ofthePartLoadPerformanceCurve) {
    bool result = setDouble(OS_HeaderedPumps_VariableSpeedFields::Coefficient1ofthePartLoadPerformanceCurve, coefficient1ofthePartLoadPerformanceCurve);
    OS_ASSERT(result);
  }

  void HeaderedPumpsVariableSpeed_Impl::setCoefficient2ofthePartLoadPerformanceCurve(double coefficient2ofthePartLoadPerformanceCurve) {
    bool result = setDouble(OS_HeaderedPumps_VariableSpeedFields::Coefficient2ofthePartLoadPerformanceCurve, coefficient2ofthePartLoadPerformanceCurve);
    OS_ASSERT(result);
  }

  void HeaderedPumpsVariableSpeed_Impl::setCoefficient3ofthePartLoadPerformanceCurve(double coefficient3ofthePartLoadPerformanceCurve) {
    bool result = setDouble(OS_HeaderedPumps_VariableSpeedFields::Coefficient3ofthePartLoadPerformanceCurve, coefficient3ofthePartLoadPerformanceCurve);
    OS_ASSERT(result);
  }

  void HeaderedPumpsVariableSpeed_Impl::setCoefficient4ofthePartLoadPerformanceCurve(double coefficient4ofthePartLoadPerformanceCurve) {
    bool result = setDouble(OS_HeaderedPumps_VariableSpeedFields::Coefficient4ofthePartLoadPerformanceCurve, coefficient4ofthePartLoadPerformanceCurve);
    OS_ASSERT(result);
  }

  bool HeaderedPumpsVariableSpeed_Impl::setMinimumFlowRateFraction(double minimumFlowRateFraction) {
    bool result = setDouble(OS_HeaderedPumps_VariableSpeedFields::MinimumFlowRateFraction, minimumFlowRateFraction);
    return result;
  }

  bool HeaderedPumpsVariableSpeed_Impl::setPumpControlType(std::string pumpControlType) {
    bool result = setString(OS_HeaderedPumps_VariableSpeedFields::PumpControlType, pumpControlType);
    return result;
  }

  bool HeaderedPumpsVariableSpeed_Impl::setPumpFlowRateSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_HeaderedPumps_VariableSpeedFields::PumpFlowRateSchedule,
                              "HeaderedPumpsVariableSpeed",
                              "Pump Flow Rate Schedule",
                              schedule);
    return result;
  }

  void HeaderedPumpsVariableSpeed_Impl::resetPumpFlowRateSchedule() {
    bool result = setString(OS_HeaderedPumps_VariableSpeedFields::PumpFlowRateSchedule, "");
    OS_ASSERT(result);
  }

  bool HeaderedPumpsVariableSpeed_Impl::setThermalZone(const boost::optional<ThermalZone>& thermalZone) {
    bool result(false);
    if (thermalZone) {
      result = setPointer(OS_HeaderedPumps_VariableSpeedFields::ThermalZone, thermalZone.get().handle());
    }
    else {
      resetThermalZone();
      result = true;
    }
    return result;
  }

  void HeaderedPumpsVariableSpeed_Impl::resetThermalZone() {
    bool result = setString(OS_HeaderedPumps_VariableSpeedFields::ThermalZone, "");
    OS_ASSERT(result);
  }

  bool HeaderedPumpsVariableSpeed_Impl::setSkinLossRadiativeFraction(double skinLossRadiativeFraction) {
    bool result = setDouble(OS_HeaderedPumps_VariableSpeedFields::SkinLossRadiativeFraction, skinLossRadiativeFraction);
    return result;
  }

  unsigned HeaderedPumpsVariableSpeed_Impl::inletPort() {
    return OS_HeaderedPumps_VariableSpeedFields::InletNodeName;
  }

  unsigned HeaderedPumpsVariableSpeed_Impl::outletPort() {
    return OS_HeaderedPumps_VariableSpeedFields::OutletNodeName;
  }

  bool HeaderedPumpsVariableSpeed_Impl::addToNode(Node & node)
  {
    if( boost::optional<PlantLoop> plant = node.plantLoop() ) {
      return StraightComponent_Impl::addToNode(node);
    }

    return false;
  }

} // detail

HeaderedPumpsVariableSpeed::HeaderedPumpsVariableSpeed(const Model& model)
  : StraightComponent(HeaderedPumpsVariableSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::HeaderedPumpsVariableSpeed_Impl>());

  autosizeTotalRatedFlowRate();
  setNumberofPumpsinBank(2);
  setFlowSequencingControlScheme("Sequential");
  setRatedPumpHead(179352);
  autosizeRatedPowerConsumption();
  setMotorEfficiency(0.9);
  setFractionofMotorInefficienciestoFluidStream(0.0);
  setCoefficient1ofthePartLoadPerformanceCurve(0.0);
  setCoefficient2ofthePartLoadPerformanceCurve(1.0);
  setCoefficient3ofthePartLoadPerformanceCurve(0.0);
  setCoefficient4ofthePartLoadPerformanceCurve(0.0);
  setMinimumFlowRateFraction(0.0);
  setPumpControlType("Continuous");
  setSkinLossRadiativeFraction(0.1);
}

IddObjectType HeaderedPumpsVariableSpeed::iddObjectType() {
  return IddObjectType(IddObjectType::OS_HeaderedPumps_VariableSpeed);
}

std::vector<std::string> HeaderedPumpsVariableSpeed::flowSequencingControlSchemeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_HeaderedPumps_VariableSpeedFields::FlowSequencingControlScheme);
}

std::vector<std::string> HeaderedPumpsVariableSpeed::pumpControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_HeaderedPumps_VariableSpeedFields::PumpControlType);
}

boost::optional<double> HeaderedPumpsVariableSpeed::totalRatedFlowRate() const {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->totalRatedFlowRate();
}

bool HeaderedPumpsVariableSpeed::isTotalRatedFlowRateAutosized() const {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->isTotalRatedFlowRateAutosized();
}

int HeaderedPumpsVariableSpeed::numberofPumpsinBank() const {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->numberofPumpsinBank();
}

std::string HeaderedPumpsVariableSpeed::flowSequencingControlScheme() const {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->flowSequencingControlScheme();
}

double HeaderedPumpsVariableSpeed::ratedPumpHead() const {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->ratedPumpHead();
}

boost::optional<double> HeaderedPumpsVariableSpeed::ratedPowerConsumption() const {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->ratedPowerConsumption();
}

bool HeaderedPumpsVariableSpeed::isRatedPowerConsumptionAutosized() const {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->isRatedPowerConsumptionAutosized();
}

double HeaderedPumpsVariableSpeed::motorEfficiency() const {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->motorEfficiency();
}

double HeaderedPumpsVariableSpeed::fractionofMotorInefficienciestoFluidStream() const {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->fractionofMotorInefficienciestoFluidStream();
}

double HeaderedPumpsVariableSpeed::coefficient1ofthePartLoadPerformanceCurve() const {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->coefficient1ofthePartLoadPerformanceCurve();
}

double HeaderedPumpsVariableSpeed::coefficient2ofthePartLoadPerformanceCurve() const {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->coefficient2ofthePartLoadPerformanceCurve();
}

double HeaderedPumpsVariableSpeed::coefficient3ofthePartLoadPerformanceCurve() const {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->coefficient3ofthePartLoadPerformanceCurve();
}

double HeaderedPumpsVariableSpeed::coefficient4ofthePartLoadPerformanceCurve() const {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->coefficient4ofthePartLoadPerformanceCurve();
}

double HeaderedPumpsVariableSpeed::minimumFlowRateFraction() const {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->minimumFlowRateFraction();
}

std::string HeaderedPumpsVariableSpeed::pumpControlType() const {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->pumpControlType();
}

boost::optional<Schedule> HeaderedPumpsVariableSpeed::pumpFlowRateSchedule() const {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->pumpFlowRateSchedule();
}

boost::optional<ThermalZone> HeaderedPumpsVariableSpeed::thermalZone() const {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->thermalZone();
}

double HeaderedPumpsVariableSpeed::skinLossRadiativeFraction() const {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->skinLossRadiativeFraction();
}

bool HeaderedPumpsVariableSpeed::setTotalRatedFlowRate(double totalRatedFlowRate) {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setTotalRatedFlowRate(totalRatedFlowRate);
}

void HeaderedPumpsVariableSpeed::autosizeTotalRatedFlowRate() {
  getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->autosizeTotalRatedFlowRate();
}

void HeaderedPumpsVariableSpeed::setNumberofPumpsinBank(int numberofPumpsinBank) {
  getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setNumberofPumpsinBank(numberofPumpsinBank);
}

bool HeaderedPumpsVariableSpeed::setFlowSequencingControlScheme(std::string flowSequencingControlScheme) {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setFlowSequencingControlScheme(flowSequencingControlScheme);
}

void HeaderedPumpsVariableSpeed::setRatedPumpHead(double ratedPumpHead) {
  getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setRatedPumpHead(ratedPumpHead);
}

void HeaderedPumpsVariableSpeed::setRatedPowerConsumption(double ratedPowerConsumption) {
  getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setRatedPowerConsumption(ratedPowerConsumption);
}

void HeaderedPumpsVariableSpeed::autosizeRatedPowerConsumption() {
  getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->autosizeRatedPowerConsumption();
}

bool HeaderedPumpsVariableSpeed::setMotorEfficiency(double motorEfficiency) {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setMotorEfficiency(motorEfficiency);
}

bool HeaderedPumpsVariableSpeed::setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream) {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setFractionofMotorInefficienciestoFluidStream(fractionofMotorInefficienciestoFluidStream);
}

void HeaderedPumpsVariableSpeed::setCoefficient1ofthePartLoadPerformanceCurve(double coefficient1ofthePartLoadPerformanceCurve) {
  getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setCoefficient1ofthePartLoadPerformanceCurve(coefficient1ofthePartLoadPerformanceCurve);
}

void HeaderedPumpsVariableSpeed::setCoefficient2ofthePartLoadPerformanceCurve(double coefficient2ofthePartLoadPerformanceCurve) {
  getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setCoefficient2ofthePartLoadPerformanceCurve(coefficient2ofthePartLoadPerformanceCurve);
}

void HeaderedPumpsVariableSpeed::setCoefficient3ofthePartLoadPerformanceCurve(double coefficient3ofthePartLoadPerformanceCurve) {
  getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setCoefficient3ofthePartLoadPerformanceCurve(coefficient3ofthePartLoadPerformanceCurve);
}

void HeaderedPumpsVariableSpeed::setCoefficient4ofthePartLoadPerformanceCurve(double coefficient4ofthePartLoadPerformanceCurve) {
  getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setCoefficient4ofthePartLoadPerformanceCurve(coefficient4ofthePartLoadPerformanceCurve);
}

bool HeaderedPumpsVariableSpeed::setMinimumFlowRateFraction(double minimumFlowRateFraction) {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setMinimumFlowRateFraction(minimumFlowRateFraction);
}

bool HeaderedPumpsVariableSpeed::setPumpControlType(std::string pumpControlType) {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setPumpControlType(pumpControlType);
}

bool HeaderedPumpsVariableSpeed::setPumpFlowRateSchedule(Schedule& schedule) {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setPumpFlowRateSchedule(schedule);
}

void HeaderedPumpsVariableSpeed::resetPumpFlowRateSchedule() {
  getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->resetPumpFlowRateSchedule();
}

bool HeaderedPumpsVariableSpeed::setThermalZone(const ThermalZone& thermalZone) {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setThermalZone(thermalZone);
}

void HeaderedPumpsVariableSpeed::resetThermalZone() {
  getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->resetThermalZone();
}

bool HeaderedPumpsVariableSpeed::setSkinLossRadiativeFraction(double skinLossRadiativeFraction) {
  return getImpl<detail::HeaderedPumpsVariableSpeed_Impl>()->setSkinLossRadiativeFraction(skinLossRadiativeFraction);
}

/// @cond
HeaderedPumpsVariableSpeed::HeaderedPumpsVariableSpeed(std::shared_ptr<detail::HeaderedPumpsVariableSpeed_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

