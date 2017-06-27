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

#include "PumpVariableSpeed.hpp"
#include "PumpVariableSpeed_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveLinear.hpp"
#include "CurveLinear_Impl.hpp"
#include "CurveQuadratic.hpp"
#include "CurveQuadratic_Impl.hpp"
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include "CurveQuartic.hpp"
#include "CurveQuartic_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Pump_VariableSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  PumpVariableSpeed_Impl::PumpVariableSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == PumpVariableSpeed::iddObjectType());
  }

  PumpVariableSpeed_Impl::PumpVariableSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == PumpVariableSpeed::iddObjectType());
  }

  PumpVariableSpeed_Impl::PumpVariableSpeed_Impl(const PumpVariableSpeed_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& PumpVariableSpeed_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  std::vector<ScheduleTypeKey> PumpVariableSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Pump_VariableSpeedFields::PumpFlowRateScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("PumpVariableSpeed","Pump Flow Rate"));
    }
    if (std::find(b,e,OS_Pump_VariableSpeedFields::PumprpmScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("PumpVariableSpeed","Pump RPM"));
    }
    if (std::find(b,e,OS_Pump_VariableSpeedFields::MinimumPressureSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("PumpVariableSpeed","Minimum Pressure"));
    }
    if (std::find(b,e,OS_Pump_VariableSpeedFields::MaximumPressureSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("PumpVariableSpeed","Maximum Pressure"));
    }
    if (std::find(b,e,OS_Pump_VariableSpeedFields::MinimumRPMSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("PumpVariableSpeed","Minimum RPM"));
    }
    if (std::find(b,e,OS_Pump_VariableSpeedFields::MaximumRPMSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("PumpVariableSpeed","Maximum RPM"));
    }
    return result;
  }


  IddObjectType PumpVariableSpeed_Impl::iddObjectType() const {
    return PumpVariableSpeed::iddObjectType();
  }

  std::vector<ModelObject> PumpVariableSpeed_Impl::children() const {
    ModelObjectVector result;
    if (OptionalCurve curve = pumpCurve()) {
      result.push_back(*curve);
    }
    return result;
  }

  unsigned PumpVariableSpeed_Impl::inletPort()
  {
    return OS_Pump_VariableSpeedFields::InletNodeName;
  }

  unsigned PumpVariableSpeed_Impl::outletPort()
  {
    return OS_Pump_VariableSpeedFields::OutletNodeName;
  }

  boost::optional<double> PumpVariableSpeed_Impl::ratedFlowRate() const {
    return getDouble(OS_Pump_VariableSpeedFields::RatedFlowRate,true);
  }

  OSOptionalQuantity PumpVariableSpeed_Impl::getRatedFlowRate(bool returnIP) const {
    OptionalDouble value = ratedFlowRate();
    return getQuantityFromDouble(OS_Pump_VariableSpeedFields::RatedFlowRate, value, returnIP);
  }

  bool PumpVariableSpeed_Impl::isRatedFlowRateDefaulted() const {
    return isEmpty(OS_Pump_VariableSpeedFields::RatedFlowRate);
  }

  bool PumpVariableSpeed_Impl::isRatedFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Pump_VariableSpeedFields::RatedFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double PumpVariableSpeed_Impl::ratedPumpHead() const {
    boost::optional<double> value = getDouble(OS_Pump_VariableSpeedFields::RatedPumpHead,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity PumpVariableSpeed_Impl::getRatedPumpHead(bool returnIP) const {
    OptionalDouble value = ratedPumpHead();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Pump_VariableSpeedFields::RatedPumpHead, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool PumpVariableSpeed_Impl::isRatedPumpHeadDefaulted() const {
    return isEmpty(OS_Pump_VariableSpeedFields::RatedPumpHead);
  }

  boost::optional<double> PumpVariableSpeed_Impl::ratedPowerConsumption() const {
    return getDouble(OS_Pump_VariableSpeedFields::RatedPowerConsumption,true);
  }

  OSOptionalQuantity PumpVariableSpeed_Impl::getRatedPowerConsumption(bool returnIP) const {
    OptionalDouble value = ratedPowerConsumption();
    return getQuantityFromDouble(OS_Pump_VariableSpeedFields::RatedPowerConsumption, value, returnIP);
  }

  bool PumpVariableSpeed_Impl::isRatedPowerConsumptionDefaulted() const {
    return isEmpty(OS_Pump_VariableSpeedFields::RatedPowerConsumption);
  }

  bool PumpVariableSpeed_Impl::isRatedPowerConsumptionAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Pump_VariableSpeedFields::RatedPowerConsumption, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double PumpVariableSpeed_Impl::motorEfficiency() const {
    boost::optional<double> value = getDouble(OS_Pump_VariableSpeedFields::MotorEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity PumpVariableSpeed_Impl::getMotorEfficiency(bool returnIP) const {
    OptionalDouble value = motorEfficiency();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Pump_VariableSpeedFields::MotorEfficiency, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool PumpVariableSpeed_Impl::isMotorEfficiencyDefaulted() const {
    return isEmpty(OS_Pump_VariableSpeedFields::MotorEfficiency);
  }

  double PumpVariableSpeed_Impl::fractionofMotorInefficienciestoFluidStream() const {
    boost::optional<double> value = getDouble(OS_Pump_VariableSpeedFields::FractionofMotorInefficienciestoFluidStream,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity PumpVariableSpeed_Impl::getFractionofMotorInefficienciestoFluidStream(bool returnIP) const {
    OptionalDouble value = fractionofMotorInefficienciestoFluidStream();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Pump_VariableSpeedFields::FractionofMotorInefficienciestoFluidStream, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool PumpVariableSpeed_Impl::isFractionofMotorInefficienciestoFluidStreamDefaulted() const {
    return isEmpty(OS_Pump_VariableSpeedFields::FractionofMotorInefficienciestoFluidStream);
  }

  double PumpVariableSpeed_Impl::coefficient1ofthePartLoadPerformanceCurve() const {
    boost::optional<double> value = getDouble(OS_Pump_VariableSpeedFields::Coefficient1ofthePartLoadPerformanceCurve,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity PumpVariableSpeed_Impl::getCoefficient1ofthePartLoadPerformanceCurve(bool returnIP) const {
    OptionalDouble value = coefficient1ofthePartLoadPerformanceCurve();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Pump_VariableSpeedFields::Coefficient1ofthePartLoadPerformanceCurve, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool PumpVariableSpeed_Impl::isCoefficient1ofthePartLoadPerformanceCurveDefaulted() const {
    return isEmpty(OS_Pump_VariableSpeedFields::Coefficient1ofthePartLoadPerformanceCurve);
  }

  double PumpVariableSpeed_Impl::coefficient2ofthePartLoadPerformanceCurve() const {
    boost::optional<double> value = getDouble(OS_Pump_VariableSpeedFields::Coefficient2ofthePartLoadPerformanceCurve,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity PumpVariableSpeed_Impl::getCoefficient2ofthePartLoadPerformanceCurve(bool returnIP) const {
    OptionalDouble value = coefficient2ofthePartLoadPerformanceCurve();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Pump_VariableSpeedFields::Coefficient2ofthePartLoadPerformanceCurve, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool PumpVariableSpeed_Impl::isCoefficient2ofthePartLoadPerformanceCurveDefaulted() const {
    return isEmpty(OS_Pump_VariableSpeedFields::Coefficient2ofthePartLoadPerformanceCurve);
  }

  double PumpVariableSpeed_Impl::coefficient3ofthePartLoadPerformanceCurve() const {
    boost::optional<double> value = getDouble(OS_Pump_VariableSpeedFields::Coefficient3ofthePartLoadPerformanceCurve,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity PumpVariableSpeed_Impl::getCoefficient3ofthePartLoadPerformanceCurve(bool returnIP) const {
    OptionalDouble value = coefficient3ofthePartLoadPerformanceCurve();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Pump_VariableSpeedFields::Coefficient3ofthePartLoadPerformanceCurve, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool PumpVariableSpeed_Impl::isCoefficient3ofthePartLoadPerformanceCurveDefaulted() const {
    return isEmpty(OS_Pump_VariableSpeedFields::Coefficient3ofthePartLoadPerformanceCurve);
  }

  double PumpVariableSpeed_Impl::coefficient4ofthePartLoadPerformanceCurve() const {
    boost::optional<double> value = getDouble(OS_Pump_VariableSpeedFields::Coefficient4ofthePartLoadPerformanceCurve,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity PumpVariableSpeed_Impl::getCoefficient4ofthePartLoadPerformanceCurve(bool returnIP) const {
    OptionalDouble value = coefficient4ofthePartLoadPerformanceCurve();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Pump_VariableSpeedFields::Coefficient4ofthePartLoadPerformanceCurve, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool PumpVariableSpeed_Impl::isCoefficient4ofthePartLoadPerformanceCurveDefaulted() const {
    return isEmpty(OS_Pump_VariableSpeedFields::Coefficient4ofthePartLoadPerformanceCurve);
  }

  double PumpVariableSpeed_Impl::minimumFlowRate() const {
    boost::optional<double> value = getDouble(OS_Pump_VariableSpeedFields::MinimumFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity PumpVariableSpeed_Impl::getMinimumFlowRate(bool returnIP) const {
    OptionalDouble value = minimumFlowRate();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Pump_VariableSpeedFields::MinimumFlowRate, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool PumpVariableSpeed_Impl::isMinimumFlowRateDefaulted() const {
    return isEmpty(OS_Pump_VariableSpeedFields::MinimumFlowRate);
  }

  std::string PumpVariableSpeed_Impl::pumpControlType() const {
    boost::optional<std::string> value = getString(OS_Pump_VariableSpeedFields::PumpControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PumpVariableSpeed_Impl::isPumpControlTypeDefaulted() const {
    return isEmpty(OS_Pump_VariableSpeedFields::PumpControlType);
  }

  boost::optional<Schedule> PumpVariableSpeed_Impl::pumpFlowRateSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Pump_VariableSpeedFields::PumpFlowRateScheduleName);
  }

  boost::optional<Curve> PumpVariableSpeed_Impl::pumpCurve() const {
    return getObject<PumpVariableSpeed>().getModelObjectTarget<Curve>(OS_Pump_VariableSpeedFields::PumpCurveName);
  }

  boost::optional<double> PumpVariableSpeed_Impl::impellerDiameter() const {
    return getDouble(OS_Pump_VariableSpeedFields::ImpellerDiameter,true);
  }

  OSOptionalQuantity PumpVariableSpeed_Impl::getImpellerDiameter(bool returnIP) const {
    OptionalDouble value = impellerDiameter();
    return getQuantityFromDouble(OS_Pump_VariableSpeedFields::ImpellerDiameter, value, returnIP);
  }

  boost::optional<std::string> PumpVariableSpeed_Impl::vFDControlType() const {
    return getString(OS_Pump_VariableSpeedFields::VFDControlType,true);
  }

  boost::optional<Schedule> PumpVariableSpeed_Impl::pumpRPMSchedule() const {
    return getObject<PumpVariableSpeed>().getModelObjectTarget<Schedule>(OS_Pump_VariableSpeedFields::PumprpmScheduleName);
  }

  boost::optional<Schedule> PumpVariableSpeed_Impl::minimumPressureSchedule() const {
    return getObject<PumpVariableSpeed>().getModelObjectTarget<Schedule>(OS_Pump_VariableSpeedFields::MinimumPressureSchedule);
  }

  boost::optional<Schedule> PumpVariableSpeed_Impl::maximumPressureSchedule() const {
    return getObject<PumpVariableSpeed>().getModelObjectTarget<Schedule>(OS_Pump_VariableSpeedFields::MaximumPressureSchedule);
  }

  boost::optional<Schedule> PumpVariableSpeed_Impl::minimumRPMSchedule() const {
    return getObject<PumpVariableSpeed>().getModelObjectTarget<Schedule>(OS_Pump_VariableSpeedFields::MinimumRPMSchedule);
  }

  boost::optional<Schedule> PumpVariableSpeed_Impl::maximumRPMSchedule() const {
    return getObject<PumpVariableSpeed>().getModelObjectTarget<Schedule>(OS_Pump_VariableSpeedFields::MaximumRPMSchedule);
  }

  void PumpVariableSpeed_Impl::setRatedFlowRate(boost::optional<double> ratedFlowRate) {
    bool result(false);
    if (ratedFlowRate) {
      result = setDouble(OS_Pump_VariableSpeedFields::RatedFlowRate, ratedFlowRate.get());
    }
    else {
      resetRatedFlowRate();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setRatedFlowRate(const OSOptionalQuantity& ratedFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (ratedFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_Pump_VariableSpeedFields::RatedFlowRate,ratedFlowRate.get());
      if (value) {
        setRatedFlowRate(value);
        result = true;
      }
    }
    else {
      setRatedFlowRate(value);
      result = true;
    }
    return result;
  }

  void PumpVariableSpeed_Impl::resetRatedFlowRate() {
    bool result = setString(OS_Pump_VariableSpeedFields::RatedFlowRate, "");
    OS_ASSERT(result);
  }

  void PumpVariableSpeed_Impl::autosizeRatedFlowRate() {
    bool result = setString(OS_Pump_VariableSpeedFields::RatedFlowRate, "autosize");
    OS_ASSERT(result);
  }

  void PumpVariableSpeed_Impl::setRatedPumpHead(double ratedPumpHead) {
    bool result = setDouble(OS_Pump_VariableSpeedFields::RatedPumpHead, ratedPumpHead);
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setRatedPumpHead(const Quantity& ratedPumpHead) {
    OptionalDouble value = getDoubleFromQuantity(OS_Pump_VariableSpeedFields::RatedPumpHead,ratedPumpHead);
    if (!value) {
      return false;
    }
    setRatedPumpHead(value.get());
    return true;
  }

  void PumpVariableSpeed_Impl::resetRatedPumpHead() {
    bool result = setString(OS_Pump_VariableSpeedFields::RatedPumpHead, "");
    OS_ASSERT(result);
  }

  void PumpVariableSpeed_Impl::setRatedPowerConsumption(boost::optional<double> ratedPowerConsumption) {
    bool result(false);
    if (ratedPowerConsumption) {
      result = setDouble(OS_Pump_VariableSpeedFields::RatedPowerConsumption, ratedPowerConsumption.get());
    }
    else {
      resetRatedPowerConsumption();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setRatedPowerConsumption(const OSOptionalQuantity& ratedPowerConsumption) {
    bool result(false);
    OptionalDouble value;
    if (ratedPowerConsumption.isSet()) {
      value = getDoubleFromQuantity(OS_Pump_VariableSpeedFields::RatedPowerConsumption,ratedPowerConsumption.get());
      if (value) {
        setRatedPowerConsumption(value);
        result = true;
      }
    }
    else {
      setRatedPowerConsumption(value);
      result = true;
    }
    return result;
  }

  void PumpVariableSpeed_Impl::resetRatedPowerConsumption() {
    bool result = setString(OS_Pump_VariableSpeedFields::RatedPowerConsumption, "");
    OS_ASSERT(result);
  }

  void PumpVariableSpeed_Impl::autosizeRatedPowerConsumption() {
    bool result = setString(OS_Pump_VariableSpeedFields::RatedPowerConsumption, "autosize");
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setMotorEfficiency(double motorEfficiency) {
    bool result = setDouble(OS_Pump_VariableSpeedFields::MotorEfficiency, motorEfficiency);
    return result;
  }

  bool PumpVariableSpeed_Impl::setMotorEfficiency(const Quantity& motorEfficiency) {
    OptionalDouble value = getDoubleFromQuantity(OS_Pump_VariableSpeedFields::MotorEfficiency,motorEfficiency);
    if (!value) {
      return false;
    }
    return setMotorEfficiency(value.get());
  }

  void PumpVariableSpeed_Impl::resetMotorEfficiency() {
    bool result = setString(OS_Pump_VariableSpeedFields::MotorEfficiency, "");
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream) {
    bool result = setDouble(OS_Pump_VariableSpeedFields::FractionofMotorInefficienciestoFluidStream, fractionofMotorInefficienciestoFluidStream);
    return result;
  }

  bool PumpVariableSpeed_Impl::setFractionofMotorInefficienciestoFluidStream(const Quantity& fractionofMotorInefficienciestoFluidStream) {
    OptionalDouble value = getDoubleFromQuantity(OS_Pump_VariableSpeedFields::FractionofMotorInefficienciestoFluidStream,fractionofMotorInefficienciestoFluidStream);
    if (!value) {
      return false;
    }
    return setFractionofMotorInefficienciestoFluidStream(value.get());
  }

  void PumpVariableSpeed_Impl::resetFractionofMotorInefficienciestoFluidStream() {
    bool result = setString(OS_Pump_VariableSpeedFields::FractionofMotorInefficienciestoFluidStream, "");
    OS_ASSERT(result);
  }

  void PumpVariableSpeed_Impl::setCoefficient1ofthePartLoadPerformanceCurve(double coefficient1ofthePartLoadPerformanceCurve) {
    bool result = setDouble(OS_Pump_VariableSpeedFields::Coefficient1ofthePartLoadPerformanceCurve, coefficient1ofthePartLoadPerformanceCurve);
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setCoefficient1ofthePartLoadPerformanceCurve(const Quantity& coefficient1ofthePartLoadPerformanceCurve) {
    OptionalDouble value = getDoubleFromQuantity(OS_Pump_VariableSpeedFields::Coefficient1ofthePartLoadPerformanceCurve,coefficient1ofthePartLoadPerformanceCurve);
    if (!value) {
      return false;
    }
    setCoefficient1ofthePartLoadPerformanceCurve(value.get());
    return true;
  }

  void PumpVariableSpeed_Impl::resetCoefficient1ofthePartLoadPerformanceCurve() {
    bool result = setString(OS_Pump_VariableSpeedFields::Coefficient1ofthePartLoadPerformanceCurve, "");
    OS_ASSERT(result);
  }

  void PumpVariableSpeed_Impl::setCoefficient2ofthePartLoadPerformanceCurve(double coefficient2ofthePartLoadPerformanceCurve) {
    bool result = setDouble(OS_Pump_VariableSpeedFields::Coefficient2ofthePartLoadPerformanceCurve, coefficient2ofthePartLoadPerformanceCurve);
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setCoefficient2ofthePartLoadPerformanceCurve(const Quantity& coefficient2ofthePartLoadPerformanceCurve) {
    OptionalDouble value = getDoubleFromQuantity(OS_Pump_VariableSpeedFields::Coefficient2ofthePartLoadPerformanceCurve,coefficient2ofthePartLoadPerformanceCurve);
    if (!value) {
      return false;
    }
    setCoefficient2ofthePartLoadPerformanceCurve(value.get());
    return true;
  }

  void PumpVariableSpeed_Impl::resetCoefficient2ofthePartLoadPerformanceCurve() {
    bool result = setString(OS_Pump_VariableSpeedFields::Coefficient2ofthePartLoadPerformanceCurve, "");
    OS_ASSERT(result);
  }

  void PumpVariableSpeed_Impl::setCoefficient3ofthePartLoadPerformanceCurve(double coefficient3ofthePartLoadPerformanceCurve) {
    bool result = setDouble(OS_Pump_VariableSpeedFields::Coefficient3ofthePartLoadPerformanceCurve, coefficient3ofthePartLoadPerformanceCurve);
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setCoefficient3ofthePartLoadPerformanceCurve(const Quantity& coefficient3ofthePartLoadPerformanceCurve) {
    OptionalDouble value = getDoubleFromQuantity(OS_Pump_VariableSpeedFields::Coefficient3ofthePartLoadPerformanceCurve,coefficient3ofthePartLoadPerformanceCurve);
    if (!value) {
      return false;
    }
    setCoefficient3ofthePartLoadPerformanceCurve(value.get());
    return true;
  }

  void PumpVariableSpeed_Impl::resetCoefficient3ofthePartLoadPerformanceCurve() {
    bool result = setString(OS_Pump_VariableSpeedFields::Coefficient3ofthePartLoadPerformanceCurve, "");
    OS_ASSERT(result);
  }

  void PumpVariableSpeed_Impl::setCoefficient4ofthePartLoadPerformanceCurve(double coefficient4ofthePartLoadPerformanceCurve) {
    bool result = setDouble(OS_Pump_VariableSpeedFields::Coefficient4ofthePartLoadPerformanceCurve, coefficient4ofthePartLoadPerformanceCurve);
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setCoefficient4ofthePartLoadPerformanceCurve(const Quantity& coefficient4ofthePartLoadPerformanceCurve) {
    OptionalDouble value = getDoubleFromQuantity(OS_Pump_VariableSpeedFields::Coefficient4ofthePartLoadPerformanceCurve,coefficient4ofthePartLoadPerformanceCurve);
    if (!value) {
      return false;
    }
    setCoefficient4ofthePartLoadPerformanceCurve(value.get());
    return true;
  }

  void PumpVariableSpeed_Impl::resetCoefficient4ofthePartLoadPerformanceCurve() {
    bool result = setString(OS_Pump_VariableSpeedFields::Coefficient4ofthePartLoadPerformanceCurve, "");
    OS_ASSERT(result);
  }

  void PumpVariableSpeed_Impl::setMinimumFlowRate(double minimumFlowRate) {
    bool result = setDouble(OS_Pump_VariableSpeedFields::MinimumFlowRate, minimumFlowRate);
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setMinimumFlowRate(const Quantity& minimumFlowRate) {
    OptionalDouble value = getDoubleFromQuantity(OS_Pump_VariableSpeedFields::MinimumFlowRate,minimumFlowRate);
    if (!value) {
      return false;
    }
    setMinimumFlowRate(value.get());
    return true;
  }

  void PumpVariableSpeed_Impl::resetMinimumFlowRate() {
    bool result = setString(OS_Pump_VariableSpeedFields::MinimumFlowRate, "");
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setPumpControlType(std::string pumpControlType) {
    bool result = setString(OS_Pump_VariableSpeedFields::PumpControlType, pumpControlType);
    return result;
  }

  void PumpVariableSpeed_Impl::resetPumpControlType() {
    bool result = setString(OS_Pump_VariableSpeedFields::PumpControlType, "");
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setPumpFlowRateSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Pump_VariableSpeedFields::PumpFlowRateScheduleName,
                              "PumpVariableSpeed",
                              "Pump Flow Rate",
                              schedule);
    return result;
  }

  void PumpVariableSpeed_Impl::resetPumpFlowRateSchedule() {
    bool result = setString(OS_Pump_VariableSpeedFields::PumpFlowRateScheduleName, "");
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setPumpCurve(const Curve& curve) {
    if (curve.optionalCast<CurveLinear>() ||
        curve.optionalCast<CurveQuadratic>() ||
        curve.optionalCast<CurveCubic>() ||
        curve.optionalCast<CurveQuartic>())
    {
      Curve wcurve = curve;
      if (wcurve.parent()) {
        wcurve = curve.clone().cast<Curve>();
      }
      bool ok = setPointer(OS_Pump_VariableSpeedFields::PumpCurveName,wcurve.handle());
      OS_ASSERT(ok);
      return true;
    }
    return false;
  }

  void PumpVariableSpeed_Impl::resetPumpCurve() {
    bool ok = setString(OS_Pump_VariableSpeedFields::PumpCurveName,"");
    OS_ASSERT(ok);
  }

  void PumpVariableSpeed_Impl::setImpellerDiameter(boost::optional<double> impellerDiameter) {
    bool result(false);
    if (impellerDiameter) {
      result = setDouble(OS_Pump_VariableSpeedFields::ImpellerDiameter, impellerDiameter.get());
    }
    else {
      resetImpellerDiameter();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setImpellerDiameter(const OSOptionalQuantity& impellerDiameter) {
    bool result(false);
    OptionalDouble value;
    if (impellerDiameter.isSet()) {
      value = getDoubleFromQuantity(OS_Pump_VariableSpeedFields::ImpellerDiameter,impellerDiameter.get());
      if (value) {
        setImpellerDiameter(value);
        result = true;
      }
    }
    else {
      setImpellerDiameter(value);
      result = true;
    }
    return result;
  }

  void PumpVariableSpeed_Impl::resetImpellerDiameter() {
    bool result = setString(OS_Pump_VariableSpeedFields::ImpellerDiameter, "");
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setVFDControlType(boost::optional<std::string> vFDControlType) {
    bool result(false);
    if (vFDControlType) {
      result = setString(OS_Pump_VariableSpeedFields::VFDControlType, vFDControlType.get());
    }
    else {
      resetVFDControlType();
      result = true;
    }
    return result;
  }

  void PumpVariableSpeed_Impl::resetVFDControlType() {
    bool result = setString(OS_Pump_VariableSpeedFields::VFDControlType, "");
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setPumpRPMSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Pump_VariableSpeedFields::PumprpmScheduleName,
                              "PumpVariableSpeed",
                              "Pump RPM",
                              schedule);
    return result;
  }

  void PumpVariableSpeed_Impl::resetPumpRPMSchedule() {
    bool result = setString(OS_Pump_VariableSpeedFields::PumprpmScheduleName, "");
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setMinimumPressureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Pump_VariableSpeedFields::MinimumPressureSchedule,
                              "PumpVariableSpeed",
                              "Minimum Pressure",
                              schedule);
    return result;
  }

  void PumpVariableSpeed_Impl::resetMinimumPressureSchedule() {
    bool result = setString(OS_Pump_VariableSpeedFields::MinimumPressureSchedule, "");
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setMaximumPressureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Pump_VariableSpeedFields::MaximumPressureSchedule,
                              "PumpVariableSpeed",
                              "Maximum Pressure",
                              schedule);
    return result;
  }

  void PumpVariableSpeed_Impl::resetMaximumPressureSchedule() {
    bool result = setString(OS_Pump_VariableSpeedFields::MaximumPressureSchedule, "");
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setMinimumRPMSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Pump_VariableSpeedFields::MinimumRPMSchedule,
                              "PumpVariableSpeed",
                              "Minimum RPM",
                              schedule);
    return result;
  }

  void PumpVariableSpeed_Impl::resetMinimumRPMSchedule() {
    bool result = setString(OS_Pump_VariableSpeedFields::MinimumRPMSchedule, "");
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setMaximumRPMSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Pump_VariableSpeedFields::MaximumRPMSchedule,
                              "PumpVariableSpeed",
                              "Maximum RPM",
                              schedule);
    return result;
  }

  void PumpVariableSpeed_Impl::resetMaximumRPMSchedule() {
    bool result = setString(OS_Pump_VariableSpeedFields::MaximumRPMSchedule, "");
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::addToNode(Node & node)
  {
    if( boost::optional<PlantLoop> plant = node.plantLoop() )
    {
      return StraightComponent_Impl::addToNode(node);
    }

    return false;
  }

  openstudio::OSOptionalQuantity PumpVariableSpeed_Impl::ratedFlowRate_SI() const {
    return getRatedFlowRate(false);
  }

  openstudio::OSOptionalQuantity PumpVariableSpeed_Impl::ratedFlowRate_IP() const {
    return getRatedFlowRate(true);
  }

  openstudio::Quantity PumpVariableSpeed_Impl::ratedPumpHead_SI() const {
    return getRatedPumpHead(false);
  }

  openstudio::Quantity PumpVariableSpeed_Impl::ratedPumpHead_IP() const {
    return getRatedPumpHead(true);
  }

  openstudio::OSOptionalQuantity PumpVariableSpeed_Impl::ratedPowerConsumption_SI() const {
    return getRatedPowerConsumption(false);
  }

  openstudio::OSOptionalQuantity PumpVariableSpeed_Impl::ratedPowerConsumption_IP() const {
    return getRatedPowerConsumption(true);
  }

  openstudio::Quantity PumpVariableSpeed_Impl::motorEfficiency_SI() const {
    return getMotorEfficiency(false);
  }

  openstudio::Quantity PumpVariableSpeed_Impl::motorEfficiency_IP() const {
    return getMotorEfficiency(true);
  }

  openstudio::Quantity PumpVariableSpeed_Impl::fractionofMotorInefficienciestoFluidStream_SI() const {
    return getFractionofMotorInefficienciestoFluidStream(false);
  }

  openstudio::Quantity PumpVariableSpeed_Impl::fractionofMotorInefficienciestoFluidStream_IP() const {
    return getFractionofMotorInefficienciestoFluidStream(true);
  }

  openstudio::Quantity PumpVariableSpeed_Impl::coefficient1ofthePartLoadPerformanceCurve_SI() const {
    return getCoefficient1ofthePartLoadPerformanceCurve(false);
  }

  openstudio::Quantity PumpVariableSpeed_Impl::coefficient1ofthePartLoadPerformanceCurve_IP() const {
    return getCoefficient1ofthePartLoadPerformanceCurve(true);
  }

  openstudio::Quantity PumpVariableSpeed_Impl::coefficient2ofthePartLoadPerformanceCurve_SI() const {
    return getCoefficient2ofthePartLoadPerformanceCurve(false);
  }

  openstudio::Quantity PumpVariableSpeed_Impl::coefficient2ofthePartLoadPerformanceCurve_IP() const {
    return getCoefficient2ofthePartLoadPerformanceCurve(true);
  }

  openstudio::Quantity PumpVariableSpeed_Impl::coefficient3ofthePartLoadPerformanceCurve_SI() const {
    return getCoefficient3ofthePartLoadPerformanceCurve(false);
  }

  openstudio::Quantity PumpVariableSpeed_Impl::coefficient3ofthePartLoadPerformanceCurve_IP() const {
    return getCoefficient3ofthePartLoadPerformanceCurve(true);
  }

  openstudio::Quantity PumpVariableSpeed_Impl::coefficient4ofthePartLoadPerformanceCurve_SI() const {
    return getCoefficient4ofthePartLoadPerformanceCurve(false);
  }

  openstudio::Quantity PumpVariableSpeed_Impl::coefficient4ofthePartLoadPerformanceCurve_IP() const {
    return getCoefficient4ofthePartLoadPerformanceCurve(true);
  }

  openstudio::Quantity PumpVariableSpeed_Impl::minimumFlowRate_SI() const {
    return getMinimumFlowRate(false);
  }

  openstudio::Quantity PumpVariableSpeed_Impl::minimumFlowRate_IP() const {
    return getMinimumFlowRate(true);
  }

  std::vector<std::string> PumpVariableSpeed_Impl::pumpControlTypeValues() const {
    return PumpVariableSpeed::pumpControlTypeValues();
  }

  openstudio::OSOptionalQuantity PumpVariableSpeed_Impl::impellerDiameter_SI() const {
    return getImpellerDiameter(false);
  }

  openstudio::OSOptionalQuantity PumpVariableSpeed_Impl::impellerDiameter_IP() const {
    return getImpellerDiameter(true);
  }

  std::vector<std::string> PumpVariableSpeed_Impl::vfdControlTypeValues() const {
    return PumpVariableSpeed::vfdControlTypeValues();
  }

  boost::optional<ModelObject> PumpVariableSpeed_Impl::pumpFlowRateScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = pumpFlowRateSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> PumpVariableSpeed_Impl::pumpCurveAsModelObject() const {
    OptionalModelObject result;
    OptionalCurve intermediate = pumpCurve();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> PumpVariableSpeed_Impl::pumpRPMScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = pumpRPMSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> PumpVariableSpeed_Impl::minimumPressureScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = minimumPressureSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> PumpVariableSpeed_Impl::maximumPressureScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = maximumPressureSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> PumpVariableSpeed_Impl::minimumRPMScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = minimumRPMSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> PumpVariableSpeed_Impl::maximumRPMScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = maximumRPMSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool PumpVariableSpeed_Impl::setPumpFlowRateScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setPumpFlowRateSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetPumpFlowRateSchedule();
    }
    return true;
  }

  bool PumpVariableSpeed_Impl::setPumpCurveAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalCurve intermediate = modelObject->optionalCast<Curve>();
      if (intermediate) {
        return setPumpCurve(*intermediate);
      }
      else {
        return false;
      }
    }
    else {
      resetPumpCurve();
    }
    return true;
  }

  bool PumpVariableSpeed_Impl::setPumpRPMScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setPumpRPMSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetPumpRPMSchedule();
    }
    return true;
  }

  bool PumpVariableSpeed_Impl::setMinimumPressureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setMinimumPressureSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetMinimumPressureSchedule();
    }
    return true;
  }

  bool PumpVariableSpeed_Impl::setMaximumPressureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setMaximumPressureSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetMaximumPressureSchedule();
    }
    return true;
  }

  bool PumpVariableSpeed_Impl::setMinimumRPMScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setMinimumRPMSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetMinimumRPMSchedule();
    }
    return true;
  }

  bool PumpVariableSpeed_Impl::setMaximumRPMScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setMaximumRPMSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetMaximumRPMSchedule();
    }
    return true;
  }

} // detail

PumpVariableSpeed::PumpVariableSpeed(const Model& model)
  : StraightComponent(PumpVariableSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::PumpVariableSpeed_Impl>());

  setPumpControlType("Intermittent");
}

IddObjectType PumpVariableSpeed::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Pump_VariableSpeed);
  return result;
}

std::vector<std::string> PumpVariableSpeed::pumpControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Pump_VariableSpeedFields::PumpControlType);
}

std::vector<std::string> PumpVariableSpeed::validPumpControlTypeValues() {
  return PumpVariableSpeed::pumpControlTypeValues();
}

std::vector<std::string> PumpVariableSpeed::vfdControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Pump_VariableSpeedFields::VFDControlType);
}

boost::optional<double> PumpVariableSpeed::ratedFlowRate() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->ratedFlowRate();
}

OSOptionalQuantity PumpVariableSpeed::getRatedFlowRate(bool returnIP) const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->getRatedFlowRate(returnIP);
}

bool PumpVariableSpeed::isRatedFlowRateDefaulted() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->isRatedFlowRateDefaulted();
}

bool PumpVariableSpeed::isRatedFlowRateAutosized() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->isRatedFlowRateAutosized();
}

double PumpVariableSpeed::ratedPumpHead() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->ratedPumpHead();
}

Quantity PumpVariableSpeed::getRatedPumpHead(bool returnIP) const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->getRatedPumpHead(returnIP);
}

bool PumpVariableSpeed::isRatedPumpHeadDefaulted() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->isRatedPumpHeadDefaulted();
}

boost::optional<double> PumpVariableSpeed::ratedPowerConsumption() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->ratedPowerConsumption();
}

OSOptionalQuantity PumpVariableSpeed::getRatedPowerConsumption(bool returnIP) const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->getRatedPowerConsumption(returnIP);
}

bool PumpVariableSpeed::isRatedPowerConsumptionDefaulted() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->isRatedPowerConsumptionDefaulted();
}

bool PumpVariableSpeed::isRatedPowerConsumptionAutosized() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->isRatedPowerConsumptionAutosized();
}

double PumpVariableSpeed::motorEfficiency() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->motorEfficiency();
}

Quantity PumpVariableSpeed::getMotorEfficiency(bool returnIP) const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->getMotorEfficiency(returnIP);
}

bool PumpVariableSpeed::isMotorEfficiencyDefaulted() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->isMotorEfficiencyDefaulted();
}

double PumpVariableSpeed::fractionofMotorInefficienciestoFluidStream() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->fractionofMotorInefficienciestoFluidStream();
}

Quantity PumpVariableSpeed::getFractionofMotorInefficienciestoFluidStream(bool returnIP) const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->getFractionofMotorInefficienciestoFluidStream(returnIP);
}

bool PumpVariableSpeed::isFractionofMotorInefficienciestoFluidStreamDefaulted() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->isFractionofMotorInefficienciestoFluidStreamDefaulted();
}

double PumpVariableSpeed::coefficient1ofthePartLoadPerformanceCurve() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->coefficient1ofthePartLoadPerformanceCurve();
}

Quantity PumpVariableSpeed::getCoefficient1ofthePartLoadPerformanceCurve(bool returnIP) const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->getCoefficient1ofthePartLoadPerformanceCurve(returnIP);
}

bool PumpVariableSpeed::isCoefficient1ofthePartLoadPerformanceCurveDefaulted() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->isCoefficient1ofthePartLoadPerformanceCurveDefaulted();
}

double PumpVariableSpeed::coefficient2ofthePartLoadPerformanceCurve() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->coefficient2ofthePartLoadPerformanceCurve();
}

Quantity PumpVariableSpeed::getCoefficient2ofthePartLoadPerformanceCurve(bool returnIP) const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->getCoefficient2ofthePartLoadPerformanceCurve(returnIP);
}

bool PumpVariableSpeed::isCoefficient2ofthePartLoadPerformanceCurveDefaulted() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->isCoefficient2ofthePartLoadPerformanceCurveDefaulted();
}

double PumpVariableSpeed::coefficient3ofthePartLoadPerformanceCurve() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->coefficient3ofthePartLoadPerformanceCurve();
}

Quantity PumpVariableSpeed::getCoefficient3ofthePartLoadPerformanceCurve(bool returnIP) const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->getCoefficient3ofthePartLoadPerformanceCurve(returnIP);
}

bool PumpVariableSpeed::isCoefficient3ofthePartLoadPerformanceCurveDefaulted() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->isCoefficient3ofthePartLoadPerformanceCurveDefaulted();
}

double PumpVariableSpeed::coefficient4ofthePartLoadPerformanceCurve() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->coefficient4ofthePartLoadPerformanceCurve();
}

Quantity PumpVariableSpeed::getCoefficient4ofthePartLoadPerformanceCurve(bool returnIP) const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->getCoefficient4ofthePartLoadPerformanceCurve(returnIP);
}

bool PumpVariableSpeed::isCoefficient4ofthePartLoadPerformanceCurveDefaulted() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->isCoefficient4ofthePartLoadPerformanceCurveDefaulted();
}

double PumpVariableSpeed::minimumFlowRate() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->minimumFlowRate();
}

Quantity PumpVariableSpeed::getMinimumFlowRate(bool returnIP) const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->getMinimumFlowRate(returnIP);
}

bool PumpVariableSpeed::isMinimumFlowRateDefaulted() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->isMinimumFlowRateDefaulted();
}

std::string PumpVariableSpeed::pumpControlType() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->pumpControlType();
}

bool PumpVariableSpeed::isPumpControlTypeDefaulted() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->isPumpControlTypeDefaulted();
}

boost::optional<Schedule> PumpVariableSpeed::pumpFlowRateSchedule() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->pumpFlowRateSchedule();
}

boost::optional<Curve> PumpVariableSpeed::pumpCurve() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->pumpCurve();
}

boost::optional<double> PumpVariableSpeed::impellerDiameter() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->impellerDiameter();
}

OSOptionalQuantity PumpVariableSpeed::getImpellerDiameter(bool returnIP) const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->getImpellerDiameter(returnIP);
}

boost::optional<std::string> PumpVariableSpeed::vFDControlType() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->vFDControlType();
}

boost::optional<Schedule> PumpVariableSpeed::pumpRPMSchedule() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->pumpRPMSchedule();
}

boost::optional<Schedule> PumpVariableSpeed::minimumPressureSchedule() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->minimumPressureSchedule();
}

boost::optional<Schedule> PumpVariableSpeed::maximumPressureSchedule() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->maximumPressureSchedule();
}

boost::optional<Schedule> PumpVariableSpeed::minimumRPMSchedule() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->minimumRPMSchedule();
}

boost::optional<Schedule> PumpVariableSpeed::maximumRPMSchedule() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->maximumRPMSchedule();
}

void PumpVariableSpeed::setRatedFlowRate(double ratedFlowRate) {
  getImpl<detail::PumpVariableSpeed_Impl>()->setRatedFlowRate(ratedFlowRate);
}

bool PumpVariableSpeed::setRatedFlowRate(const Quantity& ratedFlowRate) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setRatedFlowRate(ratedFlowRate);
}

void PumpVariableSpeed::resetRatedFlowRate() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetRatedFlowRate();
}

void PumpVariableSpeed::autosizeRatedFlowRate() {
  getImpl<detail::PumpVariableSpeed_Impl>()->autosizeRatedFlowRate();
}

void PumpVariableSpeed::setRatedPumpHead(double ratedPumpHead) {
  getImpl<detail::PumpVariableSpeed_Impl>()->setRatedPumpHead(ratedPumpHead);
}

bool PumpVariableSpeed::setRatedPumpHead(const Quantity& ratedPumpHead) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setRatedPumpHead(ratedPumpHead);
}

void PumpVariableSpeed::resetRatedPumpHead() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetRatedPumpHead();
}

void PumpVariableSpeed::setRatedPowerConsumption(double ratedPowerConsumption) {
  getImpl<detail::PumpVariableSpeed_Impl>()->setRatedPowerConsumption(ratedPowerConsumption);
}

bool PumpVariableSpeed::setRatedPowerConsumption(const Quantity& ratedPowerConsumption) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setRatedPowerConsumption(ratedPowerConsumption);
}

void PumpVariableSpeed::resetRatedPowerConsumption() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetRatedPowerConsumption();
}

void PumpVariableSpeed::autosizeRatedPowerConsumption() {
  getImpl<detail::PumpVariableSpeed_Impl>()->autosizeRatedPowerConsumption();
}

bool PumpVariableSpeed::setMotorEfficiency(double motorEfficiency) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setMotorEfficiency(motorEfficiency);
}

bool PumpVariableSpeed::setMotorEfficiency(const Quantity& motorEfficiency) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setMotorEfficiency(motorEfficiency);
}

void PumpVariableSpeed::resetMotorEfficiency() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetMotorEfficiency();
}

bool PumpVariableSpeed::setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setFractionofMotorInefficienciestoFluidStream(fractionofMotorInefficienciestoFluidStream);
}

bool PumpVariableSpeed::setFractionofMotorInefficienciestoFluidStream(const Quantity& fractionofMotorInefficienciestoFluidStream) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setFractionofMotorInefficienciestoFluidStream(fractionofMotorInefficienciestoFluidStream);
}

void PumpVariableSpeed::resetFractionofMotorInefficienciestoFluidStream() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetFractionofMotorInefficienciestoFluidStream();
}

void PumpVariableSpeed::setCoefficient1ofthePartLoadPerformanceCurve(double coefficient1ofthePartLoadPerformanceCurve) {
  getImpl<detail::PumpVariableSpeed_Impl>()->setCoefficient1ofthePartLoadPerformanceCurve(coefficient1ofthePartLoadPerformanceCurve);
}

bool PumpVariableSpeed::setCoefficient1ofthePartLoadPerformanceCurve(const Quantity& coefficient1ofthePartLoadPerformanceCurve) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setCoefficient1ofthePartLoadPerformanceCurve(coefficient1ofthePartLoadPerformanceCurve);
}

void PumpVariableSpeed::resetCoefficient1ofthePartLoadPerformanceCurve() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetCoefficient1ofthePartLoadPerformanceCurve();
}

void PumpVariableSpeed::setCoefficient2ofthePartLoadPerformanceCurve(double coefficient2ofthePartLoadPerformanceCurve) {
  getImpl<detail::PumpVariableSpeed_Impl>()->setCoefficient2ofthePartLoadPerformanceCurve(coefficient2ofthePartLoadPerformanceCurve);
}

bool PumpVariableSpeed::setCoefficient2ofthePartLoadPerformanceCurve(const Quantity& coefficient2ofthePartLoadPerformanceCurve) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setCoefficient2ofthePartLoadPerformanceCurve(coefficient2ofthePartLoadPerformanceCurve);
}

void PumpVariableSpeed::resetCoefficient2ofthePartLoadPerformanceCurve() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetCoefficient2ofthePartLoadPerformanceCurve();
}

void PumpVariableSpeed::setCoefficient3ofthePartLoadPerformanceCurve(double coefficient3ofthePartLoadPerformanceCurve) {
  getImpl<detail::PumpVariableSpeed_Impl>()->setCoefficient3ofthePartLoadPerformanceCurve(coefficient3ofthePartLoadPerformanceCurve);
}

bool PumpVariableSpeed::setCoefficient3ofthePartLoadPerformanceCurve(const Quantity& coefficient3ofthePartLoadPerformanceCurve) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setCoefficient3ofthePartLoadPerformanceCurve(coefficient3ofthePartLoadPerformanceCurve);
}

void PumpVariableSpeed::resetCoefficient3ofthePartLoadPerformanceCurve() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetCoefficient3ofthePartLoadPerformanceCurve();
}

void PumpVariableSpeed::setCoefficient4ofthePartLoadPerformanceCurve(double coefficient4ofthePartLoadPerformanceCurve) {
  getImpl<detail::PumpVariableSpeed_Impl>()->setCoefficient4ofthePartLoadPerformanceCurve(coefficient4ofthePartLoadPerformanceCurve);
}

bool PumpVariableSpeed::setCoefficient4ofthePartLoadPerformanceCurve(const Quantity& coefficient4ofthePartLoadPerformanceCurve) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setCoefficient4ofthePartLoadPerformanceCurve(coefficient4ofthePartLoadPerformanceCurve);
}

void PumpVariableSpeed::resetCoefficient4ofthePartLoadPerformanceCurve() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetCoefficient4ofthePartLoadPerformanceCurve();
}

void PumpVariableSpeed::setMinimumFlowRate(double minimumFlowRate) {
  getImpl<detail::PumpVariableSpeed_Impl>()->setMinimumFlowRate(minimumFlowRate);
}

bool PumpVariableSpeed::setMinimumFlowRate(const Quantity& minimumFlowRate) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setMinimumFlowRate(minimumFlowRate);
}

void PumpVariableSpeed::resetMinimumFlowRate() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetMinimumFlowRate();
}

bool PumpVariableSpeed::setPumpControlType(std::string pumpControlType) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setPumpControlType(pumpControlType);
}

void PumpVariableSpeed::resetPumpControlType() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetPumpControlType();
}

bool PumpVariableSpeed::setPumpFlowRateSchedule(Schedule& schedule) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setPumpFlowRateSchedule(schedule);
}

void PumpVariableSpeed::resetPumpFlowRateSchedule() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetPumpFlowRateSchedule();
}

bool PumpVariableSpeed::setPumpCurve(const Curve& curve) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setPumpCurve(curve);
}

void PumpVariableSpeed::resetPumpCurve() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetPumpCurve();
}

void PumpVariableSpeed::setImpellerDiameter(double impellerDiameter) {
  getImpl<detail::PumpVariableSpeed_Impl>()->setImpellerDiameter(impellerDiameter);
}

bool PumpVariableSpeed::setImpellerDiameter(const Quantity& impellerDiameter) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setImpellerDiameter(impellerDiameter);
}

void PumpVariableSpeed::resetImpellerDiameter() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetImpellerDiameter();
}

bool PumpVariableSpeed::setVFDControlType(std::string vFDControlType) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setVFDControlType(vFDControlType);
}

void PumpVariableSpeed::resetVFDControlType() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetVFDControlType();
}

bool PumpVariableSpeed::setPumpRPMSchedule(Schedule& schedule) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setPumpRPMSchedule(schedule);
}

void PumpVariableSpeed::resetPumpRPMSchedule() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetPumpRPMSchedule();
}

bool PumpVariableSpeed::setMinimumPressureSchedule(Schedule& schedule) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setMinimumPressureSchedule(schedule);
}

void PumpVariableSpeed::resetMinimumPressureSchedule() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetMinimumPressureSchedule();
}

bool PumpVariableSpeed::setMaximumPressureSchedule(Schedule& schedule) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setMaximumPressureSchedule(schedule);
}

void PumpVariableSpeed::resetMaximumPressureSchedule() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetMaximumPressureSchedule();
}

bool PumpVariableSpeed::setMinimumRPMSchedule(Schedule& schedule) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setMinimumRPMSchedule(schedule);
}

void PumpVariableSpeed::resetMinimumRPMSchedule() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetMinimumRPMSchedule();
}

bool PumpVariableSpeed::setMaximumRPMSchedule(Schedule& schedule) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setMaximumRPMSchedule(schedule);
}

void PumpVariableSpeed::resetMaximumRPMSchedule() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetMaximumRPMSchedule();
}

/// @cond
PumpVariableSpeed::PumpVariableSpeed(std::shared_ptr<detail::PumpVariableSpeed_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond


} // model
} // openstudio
