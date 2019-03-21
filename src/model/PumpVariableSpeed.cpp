/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"

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
    static std::vector<std::string> result{
      "Pump Electric Power",
      "Pump Electric Energy",
      "Pump Shaft Power",
      "Pump Fluid Heat Gain Rate",
      "Pump Fluid Heat Gain Energy",
      "Pump Outlet Temperature",
      "Pump Mass Flow Rate",
      "Pump Operating Pumps Count",

      // The Key is the Pump, not the zone, so it's right to report here
      // EnergyPlus/Pumps.cc::GetPumpInput()
      // TODO: Implement this check and make not static above once ModelObject return type has changed
      //if (! p.zone().empty() ) {
        "Pump Zone Total Heating Rate",
        "Pump Zone Total Heating Energy",
        "Pump Zone Convective Heating Rate",
        "Pump Zone Radiative Heating Rate"
      // }
    };
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

  unsigned PumpVariableSpeed_Impl::inletPort() const
  {
    return OS_Pump_VariableSpeedFields::InletNodeName;
  }

  unsigned PumpVariableSpeed_Impl::outletPort() const
  {
    return OS_Pump_VariableSpeedFields::OutletNodeName;
  }

  boost::optional<double> PumpVariableSpeed_Impl::ratedFlowRate() const {
    return getDouble(OS_Pump_VariableSpeedFields::RatedFlowRate,true);
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

  bool PumpVariableSpeed_Impl::isRatedPumpHeadDefaulted() const {
    return isEmpty(OS_Pump_VariableSpeedFields::RatedPumpHead);
  }

  boost::optional<double> PumpVariableSpeed_Impl::ratedPowerConsumption() const {
    return getDouble(OS_Pump_VariableSpeedFields::RatedPowerConsumption,true);
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

  bool PumpVariableSpeed_Impl::isMotorEfficiencyDefaulted() const {
    return isEmpty(OS_Pump_VariableSpeedFields::MotorEfficiency);
  }

  double PumpVariableSpeed_Impl::fractionofMotorInefficienciestoFluidStream() const {
    boost::optional<double> value = getDouble(OS_Pump_VariableSpeedFields::FractionofMotorInefficienciestoFluidStream,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PumpVariableSpeed_Impl::isFractionofMotorInefficienciestoFluidStreamDefaulted() const {
    return isEmpty(OS_Pump_VariableSpeedFields::FractionofMotorInefficienciestoFluidStream);
  }

  double PumpVariableSpeed_Impl::coefficient1ofthePartLoadPerformanceCurve() const {
    boost::optional<double> value = getDouble(OS_Pump_VariableSpeedFields::Coefficient1ofthePartLoadPerformanceCurve,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PumpVariableSpeed_Impl::isCoefficient1ofthePartLoadPerformanceCurveDefaulted() const {
    return isEmpty(OS_Pump_VariableSpeedFields::Coefficient1ofthePartLoadPerformanceCurve);
  }

  double PumpVariableSpeed_Impl::coefficient2ofthePartLoadPerformanceCurve() const {
    boost::optional<double> value = getDouble(OS_Pump_VariableSpeedFields::Coefficient2ofthePartLoadPerformanceCurve,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PumpVariableSpeed_Impl::isCoefficient2ofthePartLoadPerformanceCurveDefaulted() const {
    return isEmpty(OS_Pump_VariableSpeedFields::Coefficient2ofthePartLoadPerformanceCurve);
  }

  double PumpVariableSpeed_Impl::coefficient3ofthePartLoadPerformanceCurve() const {
    boost::optional<double> value = getDouble(OS_Pump_VariableSpeedFields::Coefficient3ofthePartLoadPerformanceCurve,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PumpVariableSpeed_Impl::isCoefficient3ofthePartLoadPerformanceCurveDefaulted() const {
    return isEmpty(OS_Pump_VariableSpeedFields::Coefficient3ofthePartLoadPerformanceCurve);
  }

  double PumpVariableSpeed_Impl::coefficient4ofthePartLoadPerformanceCurve() const {
    boost::optional<double> value = getDouble(OS_Pump_VariableSpeedFields::Coefficient4ofthePartLoadPerformanceCurve,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PumpVariableSpeed_Impl::isCoefficient4ofthePartLoadPerformanceCurveDefaulted() const {
    return isEmpty(OS_Pump_VariableSpeedFields::Coefficient4ofthePartLoadPerformanceCurve);
  }

  double PumpVariableSpeed_Impl::minimumFlowRate() const {
    boost::optional<double> value = getDouble(OS_Pump_VariableSpeedFields::MinimumFlowRate,true);
    OS_ASSERT(value);
    return value.get();
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

  bool PumpVariableSpeed_Impl::setRatedFlowRate(boost::optional<double> ratedFlowRate) {
    bool result(false);
    if (ratedFlowRate) {
      result = setDouble(OS_Pump_VariableSpeedFields::RatedFlowRate, ratedFlowRate.get());
    }
    else {
      resetRatedFlowRate();
      result = true;
    }
    OS_ASSERT(result);
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

  bool PumpVariableSpeed_Impl::setRatedPumpHead(double ratedPumpHead) {
    bool result = setDouble(OS_Pump_VariableSpeedFields::RatedPumpHead, ratedPumpHead);
    OS_ASSERT(result);
    return result;
  }

  void PumpVariableSpeed_Impl::resetRatedPumpHead() {
    bool result = setString(OS_Pump_VariableSpeedFields::RatedPumpHead, "");
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setRatedPowerConsumption(boost::optional<double> ratedPowerConsumption) {
    bool result(false);
    if (ratedPowerConsumption) {
      result = setDouble(OS_Pump_VariableSpeedFields::RatedPowerConsumption, ratedPowerConsumption.get());
    }
    else {
      resetRatedPowerConsumption();
      result = true;
    }
    OS_ASSERT(result);
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

  void PumpVariableSpeed_Impl::resetMotorEfficiency() {
    bool result = setString(OS_Pump_VariableSpeedFields::MotorEfficiency, "");
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream) {
    bool result = setDouble(OS_Pump_VariableSpeedFields::FractionofMotorInefficienciestoFluidStream, fractionofMotorInefficienciestoFluidStream);
    return result;
  }

  void PumpVariableSpeed_Impl::resetFractionofMotorInefficienciestoFluidStream() {
    bool result = setString(OS_Pump_VariableSpeedFields::FractionofMotorInefficienciestoFluidStream, "");
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setCoefficient1ofthePartLoadPerformanceCurve(double coefficient1ofthePartLoadPerformanceCurve) {
    bool result = setDouble(OS_Pump_VariableSpeedFields::Coefficient1ofthePartLoadPerformanceCurve, coefficient1ofthePartLoadPerformanceCurve);
    OS_ASSERT(result);
    return result;
  }

  void PumpVariableSpeed_Impl::resetCoefficient1ofthePartLoadPerformanceCurve() {
    bool result = setString(OS_Pump_VariableSpeedFields::Coefficient1ofthePartLoadPerformanceCurve, "");
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setCoefficient2ofthePartLoadPerformanceCurve(double coefficient2ofthePartLoadPerformanceCurve) {
    bool result = setDouble(OS_Pump_VariableSpeedFields::Coefficient2ofthePartLoadPerformanceCurve, coefficient2ofthePartLoadPerformanceCurve);
    OS_ASSERT(result);
    return result;
  }

  void PumpVariableSpeed_Impl::resetCoefficient2ofthePartLoadPerformanceCurve() {
    bool result = setString(OS_Pump_VariableSpeedFields::Coefficient2ofthePartLoadPerformanceCurve, "");
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setCoefficient3ofthePartLoadPerformanceCurve(double coefficient3ofthePartLoadPerformanceCurve) {
    bool result = setDouble(OS_Pump_VariableSpeedFields::Coefficient3ofthePartLoadPerformanceCurve, coefficient3ofthePartLoadPerformanceCurve);
    OS_ASSERT(result);
    return result;
  }

  void PumpVariableSpeed_Impl::resetCoefficient3ofthePartLoadPerformanceCurve() {
    bool result = setString(OS_Pump_VariableSpeedFields::Coefficient3ofthePartLoadPerformanceCurve, "");
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setCoefficient4ofthePartLoadPerformanceCurve(double coefficient4ofthePartLoadPerformanceCurve) {
    bool result = setDouble(OS_Pump_VariableSpeedFields::Coefficient4ofthePartLoadPerformanceCurve, coefficient4ofthePartLoadPerformanceCurve);
    OS_ASSERT(result);
    return result;
  }

  void PumpVariableSpeed_Impl::resetCoefficient4ofthePartLoadPerformanceCurve() {
    bool result = setString(OS_Pump_VariableSpeedFields::Coefficient4ofthePartLoadPerformanceCurve, "");
    OS_ASSERT(result);
  }

  bool PumpVariableSpeed_Impl::setMinimumFlowRate(double minimumFlowRate) {
    bool result = setDouble(OS_Pump_VariableSpeedFields::MinimumFlowRate, minimumFlowRate);
    OS_ASSERT(result);
    return result;
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

  bool PumpVariableSpeed_Impl::setImpellerDiameter(boost::optional<double> impellerDiameter) {
    bool result(false);
    if (impellerDiameter) {
      result = setDouble(OS_Pump_VariableSpeedFields::ImpellerDiameter, impellerDiameter.get());
    }
    else {
      resetImpellerDiameter();
      result = true;
    }
    OS_ASSERT(result);
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

  std::vector<std::string> PumpVariableSpeed_Impl::pumpControlTypeValues() const {
    return PumpVariableSpeed::pumpControlTypeValues();
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

  boost::optional<double> PumpVariableSpeed_Impl::autosizedRatedFlowRate() const {
    return getAutosizedValue("Design Flow Rate", "m3/s");
  }

  boost::optional<double> PumpVariableSpeed_Impl::autosizedRatedPowerConsumption() const {
    return getAutosizedValue("Design Power Consumption", "W");
  }

  void PumpVariableSpeed_Impl::autosize() {
    autosizeRatedFlowRate();
    autosizeRatedPowerConsumption();
  }

  void PumpVariableSpeed_Impl::applySizingValues() {
    boost::optional<double> val;
    val = autosizedRatedFlowRate();
    if (val) {
      setRatedFlowRate(val.get());
    }

    val = autosizedRatedPowerConsumption();
    if (val) {
      setRatedPowerConsumption(val.get());
    }
  }

  std::string PumpVariableSpeed_Impl::designPowerSizingMethod() const {
    auto value = getString(OS_Pump_VariableSpeedFields::DesignPowerSizingMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PumpVariableSpeed_Impl::setDesignPowerSizingMethod(const std::string & designPowerSizingMethod) {
    return setString(OS_Pump_VariableSpeedFields::DesignPowerSizingMethod,designPowerSizingMethod);
  }

  double PumpVariableSpeed_Impl::designElectricPowerPerUnitFlowRate() const {
    auto value = getDouble(OS_Pump_VariableSpeedFields::DesignElectricPowerperUnitFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PumpVariableSpeed_Impl::setDesignElectricPowerPerUnitFlowRate(double designElectricPowerPerUnitFlowRate) {
    return setDouble(OS_Pump_VariableSpeedFields::DesignElectricPowerperUnitFlowRate,designElectricPowerPerUnitFlowRate);
  }

  double PumpVariableSpeed_Impl::designShaftPowerPerUnitFlowRatePerUnitHead() const {
    auto value = getDouble(OS_Pump_VariableSpeedFields::DesignShaftPowerperUnitFlowRateperUnitHead,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PumpVariableSpeed_Impl::setDesignShaftPowerPerUnitFlowRatePerUnitHead(double designShaftPowerPerUnitFlowRatePerUnitHead) {
    return setDouble(OS_Pump_VariableSpeedFields::DesignShaftPowerperUnitFlowRateperUnitHead,designShaftPowerPerUnitFlowRatePerUnitHead);
  }

  double PumpVariableSpeed_Impl::designMinimumFlowRateFraction() const {
    auto value = getDouble(OS_Pump_VariableSpeedFields::DesignMinimumFlowRateFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PumpVariableSpeed_Impl::setDesignMinimumFlowRateFraction(double designMinimumFlowRateFraction) {
    return setDouble(OS_Pump_VariableSpeedFields::DesignMinimumFlowRateFraction,designMinimumFlowRateFraction);
  }

  double PumpVariableSpeed_Impl::skinLossRadiativeFraction() const {
    auto value = getDouble(OS_Pump_VariableSpeedFields::SkinLossRadiativeFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PumpVariableSpeed_Impl::setSkinLossRadiativeFraction(double skinLossRadiativeFraction) {
    return setDouble(OS_Pump_VariableSpeedFields::SkinLossRadiativeFraction,skinLossRadiativeFraction);
  }

  boost::optional<ThermalZone> PumpVariableSpeed_Impl::zone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Pump_VariableSpeedFields::ZoneName);
  }

  bool PumpVariableSpeed_Impl::setZone(const ThermalZone& thermalZone) {
    return setPointer(OS_Pump_VariableSpeedFields::ZoneName, thermalZone.handle());
  }

  void PumpVariableSpeed_Impl::resetZone() {
    bool result = setString(OS_Pump_VariableSpeedFields::ZoneName, "");
    OS_ASSERT(result);
  }

  std::vector<EMSActuatorNames> PumpVariableSpeed_Impl::emsActuatorNames() const {
    std::vector<EMSActuatorNames> actuators{{"Pump", "Pump Mass Flow Rate"},
                                            {"Pump", "Pump Pressure Rise"}};
    return actuators;
  }

  std::vector<std::string> PumpVariableSpeed_Impl::emsInternalVariableNames() const {
    std::vector<std::string> types{"Pump Maximum Mass Flow Rate"};
    return types;
  }

  std::string PumpVariableSpeed_Impl::endUseSubcategory() const {
    auto value = getString(OS_Pump_VariableSpeedFields::EndUseSubcategory, true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PumpVariableSpeed_Impl::setEndUseSubcategory(const std::string & endUseSubcategory) {
    return setString(OS_Pump_VariableSpeedFields::EndUseSubcategory, endUseSubcategory);
  }

} // detail

PumpVariableSpeed::PumpVariableSpeed(const Model& model)
  : StraightComponent(PumpVariableSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::PumpVariableSpeed_Impl>());

  setPumpControlType("Intermittent");

  setSkinLossRadiativeFraction(0.5);
  setDesignPowerSizingMethod("PowerPerFlowPerPressure");
  setDesignElectricPowerPerUnitFlowRate(348701.1);
  setDesignShaftPowerPerUnitFlowRatePerUnitHead(1.282051282);
  setDesignMinimumFlowRateFraction(0.0);

  setEndUseSubcategory("General");
}

IddObjectType PumpVariableSpeed::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Pump_VariableSpeed);
  return result;
}

std::vector<std::string> PumpVariableSpeed::designPowerSizingMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Pump_VariableSpeedFields::DesignPowerSizingMethod);
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

bool PumpVariableSpeed::isRatedFlowRateDefaulted() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->isRatedFlowRateDefaulted();
}

bool PumpVariableSpeed::isRatedFlowRateAutosized() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->isRatedFlowRateAutosized();
}

double PumpVariableSpeed::ratedPumpHead() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->ratedPumpHead();
}

bool PumpVariableSpeed::isRatedPumpHeadDefaulted() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->isRatedPumpHeadDefaulted();
}

boost::optional<double> PumpVariableSpeed::ratedPowerConsumption() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->ratedPowerConsumption();
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

bool PumpVariableSpeed::isMotorEfficiencyDefaulted() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->isMotorEfficiencyDefaulted();
}

double PumpVariableSpeed::fractionofMotorInefficienciestoFluidStream() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->fractionofMotorInefficienciestoFluidStream();
}

bool PumpVariableSpeed::isFractionofMotorInefficienciestoFluidStreamDefaulted() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->isFractionofMotorInefficienciestoFluidStreamDefaulted();
}

double PumpVariableSpeed::coefficient1ofthePartLoadPerformanceCurve() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->coefficient1ofthePartLoadPerformanceCurve();
}

bool PumpVariableSpeed::isCoefficient1ofthePartLoadPerformanceCurveDefaulted() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->isCoefficient1ofthePartLoadPerformanceCurveDefaulted();
}

double PumpVariableSpeed::coefficient2ofthePartLoadPerformanceCurve() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->coefficient2ofthePartLoadPerformanceCurve();
}

bool PumpVariableSpeed::isCoefficient2ofthePartLoadPerformanceCurveDefaulted() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->isCoefficient2ofthePartLoadPerformanceCurveDefaulted();
}

double PumpVariableSpeed::coefficient3ofthePartLoadPerformanceCurve() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->coefficient3ofthePartLoadPerformanceCurve();
}

bool PumpVariableSpeed::isCoefficient3ofthePartLoadPerformanceCurveDefaulted() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->isCoefficient3ofthePartLoadPerformanceCurveDefaulted();
}

double PumpVariableSpeed::coefficient4ofthePartLoadPerformanceCurve() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->coefficient4ofthePartLoadPerformanceCurve();
}

bool PumpVariableSpeed::isCoefficient4ofthePartLoadPerformanceCurveDefaulted() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->isCoefficient4ofthePartLoadPerformanceCurveDefaulted();
}

double PumpVariableSpeed::minimumFlowRate() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->minimumFlowRate();
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

bool PumpVariableSpeed::setRatedFlowRate(double ratedFlowRate) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setRatedFlowRate(ratedFlowRate);
}

void PumpVariableSpeed::resetRatedFlowRate() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetRatedFlowRate();
}

void PumpVariableSpeed::autosizeRatedFlowRate() {
  getImpl<detail::PumpVariableSpeed_Impl>()->autosizeRatedFlowRate();
}

bool PumpVariableSpeed::setRatedPumpHead(double ratedPumpHead) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setRatedPumpHead(ratedPumpHead);
}

void PumpVariableSpeed::resetRatedPumpHead() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetRatedPumpHead();
}

bool PumpVariableSpeed::setRatedPowerConsumption(double ratedPowerConsumption) {
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

void PumpVariableSpeed::resetMotorEfficiency() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetMotorEfficiency();
}

bool PumpVariableSpeed::setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setFractionofMotorInefficienciestoFluidStream(fractionofMotorInefficienciestoFluidStream);
}

void PumpVariableSpeed::resetFractionofMotorInefficienciestoFluidStream() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetFractionofMotorInefficienciestoFluidStream();
}

bool PumpVariableSpeed::setCoefficient1ofthePartLoadPerformanceCurve(double coefficient1ofthePartLoadPerformanceCurve) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setCoefficient1ofthePartLoadPerformanceCurve(coefficient1ofthePartLoadPerformanceCurve);
}

void PumpVariableSpeed::resetCoefficient1ofthePartLoadPerformanceCurve() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetCoefficient1ofthePartLoadPerformanceCurve();
}

bool PumpVariableSpeed::setCoefficient2ofthePartLoadPerformanceCurve(double coefficient2ofthePartLoadPerformanceCurve) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setCoefficient2ofthePartLoadPerformanceCurve(coefficient2ofthePartLoadPerformanceCurve);
}

void PumpVariableSpeed::resetCoefficient2ofthePartLoadPerformanceCurve() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetCoefficient2ofthePartLoadPerformanceCurve();
}

bool PumpVariableSpeed::setCoefficient3ofthePartLoadPerformanceCurve(double coefficient3ofthePartLoadPerformanceCurve) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setCoefficient3ofthePartLoadPerformanceCurve(coefficient3ofthePartLoadPerformanceCurve);
}

void PumpVariableSpeed::resetCoefficient3ofthePartLoadPerformanceCurve() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetCoefficient3ofthePartLoadPerformanceCurve();
}

bool PumpVariableSpeed::setCoefficient4ofthePartLoadPerformanceCurve(double coefficient4ofthePartLoadPerformanceCurve) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setCoefficient4ofthePartLoadPerformanceCurve(coefficient4ofthePartLoadPerformanceCurve);
}

void PumpVariableSpeed::resetCoefficient4ofthePartLoadPerformanceCurve() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetCoefficient4ofthePartLoadPerformanceCurve();
}

bool PumpVariableSpeed::setMinimumFlowRate(double minimumFlowRate) {
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

bool PumpVariableSpeed::setImpellerDiameter(double impellerDiameter) {
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

std::string PumpVariableSpeed::designPowerSizingMethod() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->designPowerSizingMethod();
}

bool PumpVariableSpeed::setDesignPowerSizingMethod(const std::string & designPowerSizingMethod) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setDesignPowerSizingMethod(designPowerSizingMethod);
}

double PumpVariableSpeed::designElectricPowerPerUnitFlowRate() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->designElectricPowerPerUnitFlowRate();
}

bool PumpVariableSpeed::setDesignElectricPowerPerUnitFlowRate(double designElectricPowerPerUnitFlowRate) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setDesignElectricPowerPerUnitFlowRate(designElectricPowerPerUnitFlowRate);
}

double PumpVariableSpeed::designShaftPowerPerUnitFlowRatePerUnitHead() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->designShaftPowerPerUnitFlowRatePerUnitHead();
}

bool PumpVariableSpeed::setDesignShaftPowerPerUnitFlowRatePerUnitHead(double designShaftPowerPerUnitFlowRatePerUnitHead) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setDesignShaftPowerPerUnitFlowRatePerUnitHead(designShaftPowerPerUnitFlowRatePerUnitHead);
}

boost::optional<ThermalZone> PumpVariableSpeed::zone() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->zone();
}

bool PumpVariableSpeed::setZone(const ThermalZone& thermalZone) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setZone(thermalZone);
}

void PumpVariableSpeed::resetZone() {
  getImpl<detail::PumpVariableSpeed_Impl>()->resetZone();
}

double PumpVariableSpeed::skinLossRadiativeFraction() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->skinLossRadiativeFraction();
}

bool PumpVariableSpeed::setSkinLossRadiativeFraction(double skinLossRadiativeFraction) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setSkinLossRadiativeFraction(skinLossRadiativeFraction);
}

double PumpVariableSpeed::designMinimumFlowRateFraction() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->designMinimumFlowRateFraction();
}

bool PumpVariableSpeed::setDesignMinimumFlowRateFraction(double designMinimumFlowRateFraction) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setDesignMinimumFlowRateFraction(designMinimumFlowRateFraction);
}

std::string PumpVariableSpeed::endUseSubcategory() const {
  return getImpl<detail::PumpVariableSpeed_Impl>()->endUseSubcategory();
}

bool PumpVariableSpeed::setEndUseSubcategory(const std::string & endUseSubcategory) {
  return getImpl<detail::PumpVariableSpeed_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

/// @cond
PumpVariableSpeed::PumpVariableSpeed(std::shared_ptr<detail::PumpVariableSpeed_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond


  boost::optional<double> PumpVariableSpeed::autosizedRatedFlowRate() const {
    return getImpl<detail::PumpVariableSpeed_Impl>()->autosizedRatedFlowRate();
  }

  boost::optional<double> PumpVariableSpeed::autosizedRatedPowerConsumption() const {
    return getImpl<detail::PumpVariableSpeed_Impl>()->autosizedRatedPowerConsumption();
  }

} // model
} // openstudio