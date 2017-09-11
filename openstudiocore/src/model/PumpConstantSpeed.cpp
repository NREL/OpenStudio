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

#include "PumpConstantSpeed.hpp"
#include "PumpConstantSpeed_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Pump_ConstantSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail {

  PumpConstantSpeed_Impl::PumpConstantSpeed_Impl(const IdfObject& idfObject,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == PumpConstantSpeed::iddObjectType());
  }

  PumpConstantSpeed_Impl::PumpConstantSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == PumpConstantSpeed::iddObjectType());
  }

  PumpConstantSpeed_Impl::PumpConstantSpeed_Impl(const PumpConstantSpeed_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& PumpConstantSpeed_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType PumpConstantSpeed_Impl::iddObjectType() const {
    return PumpConstantSpeed::iddObjectType();
  }

  std::vector<ScheduleTypeKey> PumpConstantSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Pump_ConstantSpeedFields::PumpFlowRateSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("PumpConstantSpeed","Pump Flow Rate"));
    }
    return result;
  }

  boost::optional<double> PumpConstantSpeed_Impl::ratedFlowRate() const {
    return getDouble(OS_Pump_ConstantSpeedFields::RatedFlowRate,true);
  }

  OSOptionalQuantity PumpConstantSpeed_Impl::getRatedFlowRate(bool returnIP) const {
    OptionalDouble value = ratedFlowRate();
    return getQuantityFromDouble(OS_Pump_ConstantSpeedFields::RatedFlowRate, value, returnIP);
  }

  bool PumpConstantSpeed_Impl::isRatedFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Pump_ConstantSpeedFields::RatedFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double PumpConstantSpeed_Impl::ratedPumpHead() const {
    boost::optional<double> value = getDouble(OS_Pump_ConstantSpeedFields::RatedPumpHead,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity PumpConstantSpeed_Impl::getRatedPumpHead(bool returnIP) const {
    OptionalDouble value = ratedPumpHead();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Pump_ConstantSpeedFields::RatedPumpHead, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool PumpConstantSpeed_Impl::isRatedPumpHeadDefaulted() const {
    return isEmpty(OS_Pump_ConstantSpeedFields::RatedPumpHead);
  }

  boost::optional<double> PumpConstantSpeed_Impl::ratedPowerConsumption() const {
    return getDouble(OS_Pump_ConstantSpeedFields::RatedPowerConsumption,true);
  }

  OSOptionalQuantity PumpConstantSpeed_Impl::getRatedPowerConsumption(bool returnIP) const {
    OptionalDouble value = ratedPowerConsumption();
    return getQuantityFromDouble(OS_Pump_ConstantSpeedFields::RatedPowerConsumption, value, returnIP);
  }

  bool PumpConstantSpeed_Impl::isRatedPowerConsumptionAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Pump_ConstantSpeedFields::RatedPowerConsumption, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double PumpConstantSpeed_Impl::motorEfficiency() const {
    boost::optional<double> value = getDouble(OS_Pump_ConstantSpeedFields::MotorEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity PumpConstantSpeed_Impl::getMotorEfficiency(bool returnIP) const {
    OptionalDouble value = motorEfficiency();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Pump_ConstantSpeedFields::MotorEfficiency, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool PumpConstantSpeed_Impl::isMotorEfficiencyDefaulted() const {
    return isEmpty(OS_Pump_ConstantSpeedFields::MotorEfficiency);
  }

  double PumpConstantSpeed_Impl::fractionofMotorInefficienciestoFluidStream() const {
    boost::optional<double> value = getDouble(OS_Pump_ConstantSpeedFields::FractionofMotorInefficienciestoFluidStream,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity PumpConstantSpeed_Impl::getFractionofMotorInefficienciestoFluidStream(bool returnIP) const {
    OptionalDouble value = fractionofMotorInefficienciestoFluidStream();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Pump_ConstantSpeedFields::FractionofMotorInefficienciestoFluidStream, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool PumpConstantSpeed_Impl::isFractionofMotorInefficienciestoFluidStreamDefaulted() const {
    return isEmpty(OS_Pump_ConstantSpeedFields::FractionofMotorInefficienciestoFluidStream);
  }

  std::string PumpConstantSpeed_Impl::pumpControlType() const {
    boost::optional<std::string> value = getString(OS_Pump_ConstantSpeedFields::PumpControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool PumpConstantSpeed_Impl::isPumpControlTypeDefaulted() const {
    return isEmpty(OS_Pump_ConstantSpeedFields::PumpControlType);
  }

  boost::optional<Schedule> PumpConstantSpeed_Impl::pumpFlowRateSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Pump_ConstantSpeedFields::PumpFlowRateSchedule);
  }

  boost::optional<Curve> PumpConstantSpeed_Impl::pumpCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Pump_ConstantSpeedFields::PumpCurve);
  }

  boost::optional<double> PumpConstantSpeed_Impl::impellerDiameter() const {
    return getDouble(OS_Pump_ConstantSpeedFields::ImpellerDiameter,true);
  }

  OSOptionalQuantity PumpConstantSpeed_Impl::getImpellerDiameter(bool returnIP) const {
    OptionalDouble value = impellerDiameter();
    return getQuantityFromDouble(OS_Pump_ConstantSpeedFields::ImpellerDiameter, value, returnIP);
  }

  boost::optional<double> PumpConstantSpeed_Impl::rotationalSpeed() const {
    return getDouble(OS_Pump_ConstantSpeedFields::RotationalSpeed,true);
  }

  OSOptionalQuantity PumpConstantSpeed_Impl::getRotationalSpeed(bool returnIP) const {
    OptionalDouble value = rotationalSpeed();
    return getQuantityFromDouble(OS_Pump_ConstantSpeedFields::RotationalSpeed, value, returnIP);
  }

  boost::optional<ThermalZone> PumpConstantSpeed_Impl::zone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Pump_ConstantSpeedFields::Zone);
  }

  boost::optional<double> PumpConstantSpeed_Impl::skinLossRadiativeFraction() const {
    return getDouble(OS_Pump_ConstantSpeedFields::SkinLossRadiativeFraction,true);
  }

  OSOptionalQuantity PumpConstantSpeed_Impl::getSkinLossRadiativeFraction(bool returnIP) const {
    OptionalDouble value = skinLossRadiativeFraction();
    return getQuantityFromDouble(OS_Pump_ConstantSpeedFields::SkinLossRadiativeFraction, value, returnIP);
  }

  void PumpConstantSpeed_Impl::setRatedFlowRate(boost::optional<double> ratedFlowRate) {
    bool result(false);
    if (ratedFlowRate) {
      result = setDouble(OS_Pump_ConstantSpeedFields::RatedFlowRate, ratedFlowRate.get());
    }
    else {
      resetRatedFlowRate();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool PumpConstantSpeed_Impl::setRatedFlowRate(const OSOptionalQuantity& ratedFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (ratedFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_Pump_ConstantSpeedFields::RatedFlowRate,ratedFlowRate.get());
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

  void PumpConstantSpeed_Impl::resetRatedFlowRate() {
    bool result = setString(OS_Pump_ConstantSpeedFields::RatedFlowRate, "");
    OS_ASSERT(result);
  }

  void PumpConstantSpeed_Impl::autosizeRatedFlowRate() {
    bool result = setString(OS_Pump_ConstantSpeedFields::RatedFlowRate, "autosize");
    OS_ASSERT(result);
  }

  void PumpConstantSpeed_Impl::setRatedPumpHead(double ratedPumpHead) {
    bool result = setDouble(OS_Pump_ConstantSpeedFields::RatedPumpHead, ratedPumpHead);
    OS_ASSERT(result);
  }

  bool PumpConstantSpeed_Impl::setRatedPumpHead(const Quantity& ratedPumpHead) {
    OptionalDouble value = getDoubleFromQuantity(OS_Pump_ConstantSpeedFields::RatedPumpHead,ratedPumpHead);
    if (!value) {
      return false;
    }
    setRatedPumpHead(value.get());
    return true;
  }

  void PumpConstantSpeed_Impl::resetRatedPumpHead() {
    bool result = setString(OS_Pump_ConstantSpeedFields::RatedPumpHead, "");
    OS_ASSERT(result);
  }

  void PumpConstantSpeed_Impl::setRatedPowerConsumption(boost::optional<double> ratedPowerConsumption) {
    bool result(false);
    if (ratedPowerConsumption) {
      result = setDouble(OS_Pump_ConstantSpeedFields::RatedPowerConsumption, ratedPowerConsumption.get());
    }
    else {
      resetRatedPowerConsumption();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool PumpConstantSpeed_Impl::setRatedPowerConsumption(const OSOptionalQuantity& ratedPowerConsumption) {
    bool result(false);
    OptionalDouble value;
    if (ratedPowerConsumption.isSet()) {
      value = getDoubleFromQuantity(OS_Pump_ConstantSpeedFields::RatedPowerConsumption,ratedPowerConsumption.get());
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

  void PumpConstantSpeed_Impl::resetRatedPowerConsumption() {
    bool result = setString(OS_Pump_ConstantSpeedFields::RatedPowerConsumption, "");
    OS_ASSERT(result);
  }

  void PumpConstantSpeed_Impl::autosizeRatedPowerConsumption() {
    bool result = setString(OS_Pump_ConstantSpeedFields::RatedPowerConsumption, "autosize");
    OS_ASSERT(result);
  }

  bool PumpConstantSpeed_Impl::setMotorEfficiency(double motorEfficiency) {
    bool result = setDouble(OS_Pump_ConstantSpeedFields::MotorEfficiency, motorEfficiency);
    return result;
  }

  bool PumpConstantSpeed_Impl::setMotorEfficiency(const Quantity& motorEfficiency) {
    OptionalDouble value = getDoubleFromQuantity(OS_Pump_ConstantSpeedFields::MotorEfficiency,motorEfficiency);
    if (!value) {
      return false;
    }
    return setMotorEfficiency(value.get());
  }

  void PumpConstantSpeed_Impl::resetMotorEfficiency() {
    bool result = setString(OS_Pump_ConstantSpeedFields::MotorEfficiency, "");
    OS_ASSERT(result);
  }

  bool PumpConstantSpeed_Impl::setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream) {
    bool result = setDouble(OS_Pump_ConstantSpeedFields::FractionofMotorInefficienciestoFluidStream, fractionofMotorInefficienciestoFluidStream);
    return result;
  }

  bool PumpConstantSpeed_Impl::setFractionofMotorInefficienciestoFluidStream(const Quantity& fractionofMotorInefficienciestoFluidStream) {
    OptionalDouble value = getDoubleFromQuantity(OS_Pump_ConstantSpeedFields::FractionofMotorInefficienciestoFluidStream,fractionofMotorInefficienciestoFluidStream);
    if (!value) {
      return false;
    }
    return setFractionofMotorInefficienciestoFluidStream(value.get());
  }

  void PumpConstantSpeed_Impl::resetFractionofMotorInefficienciestoFluidStream() {
    bool result = setString(OS_Pump_ConstantSpeedFields::FractionofMotorInefficienciestoFluidStream, "");
    OS_ASSERT(result);
  }

  bool PumpConstantSpeed_Impl::setPumpControlType(std::string pumpControlType) {
    bool result = setString(OS_Pump_ConstantSpeedFields::PumpControlType, pumpControlType);
    return result;
  }

  void PumpConstantSpeed_Impl::resetPumpControlType() {
    bool result = setString(OS_Pump_ConstantSpeedFields::PumpControlType, "");
    OS_ASSERT(result);
  }

  bool PumpConstantSpeed_Impl::setPumpFlowRateSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Pump_ConstantSpeedFields::PumpFlowRateSchedule,
                              "PumpConstantSpeed",
                              "Pump Flow Rate",
                              schedule);
    return result;
  }

  void PumpConstantSpeed_Impl::resetPumpFlowRateSchedule() {
    bool result = setString(OS_Pump_ConstantSpeedFields::PumpFlowRateSchedule, "");
    OS_ASSERT(result);
  }

  bool PumpConstantSpeed_Impl::setPumpCurve(const boost::optional<Curve>& curve) {
    bool result(false);
    if (curve) {
      setPointer(OS_Pump_ConstantSpeedFields::PumpCurve, curve.get().handle());

      result = true;
    }
    else {
      resetPumpCurve();
      result = true;
    }
    return result;
  }

  void PumpConstantSpeed_Impl::resetPumpCurve() {
    bool result = setString(OS_Pump_ConstantSpeedFields::PumpCurve, "");
    OS_ASSERT(result);
  }

  void PumpConstantSpeed_Impl::setImpellerDiameter(boost::optional<double> impellerDiameter) {
    bool result(false);
    if (impellerDiameter) {
      result = setDouble(OS_Pump_ConstantSpeedFields::ImpellerDiameter, impellerDiameter.get());
    }
    else {
      resetImpellerDiameter();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool PumpConstantSpeed_Impl::setImpellerDiameter(const OSOptionalQuantity& impellerDiameter) {
    bool result(false);
    OptionalDouble value;
    if (impellerDiameter.isSet()) {
      value = getDoubleFromQuantity(OS_Pump_ConstantSpeedFields::ImpellerDiameter,impellerDiameter.get());
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

  void PumpConstantSpeed_Impl::resetImpellerDiameter() {
    bool result = setString(OS_Pump_ConstantSpeedFields::ImpellerDiameter, "");
    OS_ASSERT(result);
  }

  void PumpConstantSpeed_Impl::setRotationalSpeed(boost::optional<double> rotationalSpeed) {
    bool result(false);
    if (rotationalSpeed) {
      result = setDouble(OS_Pump_ConstantSpeedFields::RotationalSpeed, rotationalSpeed.get());
    }
    else {
      resetRotationalSpeed();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool PumpConstantSpeed_Impl::setRotationalSpeed(const OSOptionalQuantity& rotationalSpeed) {
    bool result(false);
    OptionalDouble value;
    if (rotationalSpeed.isSet()) {
      value = getDoubleFromQuantity(OS_Pump_ConstantSpeedFields::RotationalSpeed,rotationalSpeed.get());
      if (value) {
        setRotationalSpeed(value);

        result = true;
      }
    }
    else {
      setRotationalSpeed(value);
      result = true;
    }
    return result;
  }

  void PumpConstantSpeed_Impl::resetRotationalSpeed() {
    bool result = setString(OS_Pump_ConstantSpeedFields::RotationalSpeed, "");
    OS_ASSERT(result);
  }

  bool PumpConstantSpeed_Impl::setZone(const boost::optional<ThermalZone>& thermalZone) {
    bool result(false);
    if (thermalZone) {
      result = setPointer(OS_Pump_ConstantSpeedFields::Zone, thermalZone.get().handle());
    }
    else {
      resetZone();
      result = true;
    }
    return result;
  }

  void PumpConstantSpeed_Impl::resetZone() {
    bool result = setString(OS_Pump_ConstantSpeedFields::Zone, "");
    OS_ASSERT(result);
  }

  bool PumpConstantSpeed_Impl::setSkinLossRadiativeFraction(boost::optional<double> skinLossRadiativeFraction) {
    bool result(false);
    if (skinLossRadiativeFraction) {
      result = setDouble(OS_Pump_ConstantSpeedFields::SkinLossRadiativeFraction, skinLossRadiativeFraction.get());
    }
    else {
      resetSkinLossRadiativeFraction();
      result = true;
    }
    return result;
  }

  bool PumpConstantSpeed_Impl::setSkinLossRadiativeFraction(const OSOptionalQuantity& skinLossRadiativeFraction) {
    bool result(false);
    OptionalDouble value;
    if (skinLossRadiativeFraction.isSet()) {
      value = getDoubleFromQuantity(OS_Pump_ConstantSpeedFields::SkinLossRadiativeFraction,skinLossRadiativeFraction.get());
      if (value) {
        result = setSkinLossRadiativeFraction(value);
      }
    }
    else {
      result = setSkinLossRadiativeFraction(value);
    }
    return result;
  }

  void PumpConstantSpeed_Impl::resetSkinLossRadiativeFraction() {
    bool result = setString(OS_Pump_ConstantSpeedFields::SkinLossRadiativeFraction, "");
    OS_ASSERT(result);
  }

  openstudio::OSOptionalQuantity PumpConstantSpeed_Impl::ratedFlowRate_SI() const {
    return getRatedFlowRate(false);
  }

  openstudio::OSOptionalQuantity PumpConstantSpeed_Impl::ratedFlowRate_IP() const {
    return getRatedFlowRate(true);
  }

  openstudio::Quantity PumpConstantSpeed_Impl::ratedPumpHead_SI() const {
    return getRatedPumpHead(false);
  }

  openstudio::Quantity PumpConstantSpeed_Impl::ratedPumpHead_IP() const {
    return getRatedPumpHead(true);
  }

  openstudio::OSOptionalQuantity PumpConstantSpeed_Impl::ratedPowerConsumption_SI() const {
    return getRatedPowerConsumption(false);
  }

  openstudio::OSOptionalQuantity PumpConstantSpeed_Impl::ratedPowerConsumption_IP() const {
    return getRatedPowerConsumption(true);
  }

  openstudio::Quantity PumpConstantSpeed_Impl::motorEfficiency_SI() const {
    return getMotorEfficiency(false);
  }

  openstudio::Quantity PumpConstantSpeed_Impl::motorEfficiency_IP() const {
    return getMotorEfficiency(true);
  }

  openstudio::Quantity PumpConstantSpeed_Impl::fractionofMotorInefficienciestoFluidStream_SI() const {
    return getFractionofMotorInefficienciestoFluidStream(false);
  }

  openstudio::Quantity PumpConstantSpeed_Impl::fractionofMotorInefficienciestoFluidStream_IP() const {
    return getFractionofMotorInefficienciestoFluidStream(true);
  }

  std::vector<std::string> PumpConstantSpeed_Impl::pumpControlTypeValues() const {
    return PumpConstantSpeed::pumpControlTypeValues();
  }

  openstudio::OSOptionalQuantity PumpConstantSpeed_Impl::impellerDiameter_SI() const {
    return getImpellerDiameter(false);
  }

  openstudio::OSOptionalQuantity PumpConstantSpeed_Impl::impellerDiameter_IP() const {
    return getImpellerDiameter(true);
  }

  openstudio::OSOptionalQuantity PumpConstantSpeed_Impl::rotationalSpeed_SI() const {
    return getRotationalSpeed(false);
  }

  openstudio::OSOptionalQuantity PumpConstantSpeed_Impl::rotationalSpeed_IP() const {
    return getRotationalSpeed(true);
  }

  openstudio::OSOptionalQuantity PumpConstantSpeed_Impl::skinLossRadiativeFraction_SI() const {
    return getSkinLossRadiativeFraction(false);
  }

  openstudio::OSOptionalQuantity PumpConstantSpeed_Impl::skinLossRadiativeFraction_IP() const {
    return getSkinLossRadiativeFraction(true);
  }

  boost::optional<ModelObject> PumpConstantSpeed_Impl::pumpFlowRateScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = pumpFlowRateSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> PumpConstantSpeed_Impl::pumpCurveAsModelObject() const {
    OptionalModelObject result;
    OptionalCurve intermediate = pumpCurve();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> PumpConstantSpeed_Impl::zoneAsModelObject() const {
    OptionalModelObject result;
    OptionalThermalZone intermediate = zone();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool PumpConstantSpeed_Impl::setPumpFlowRateScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
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

  bool PumpConstantSpeed_Impl::setPumpCurveAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalCurve intermediate = modelObject->optionalCast<Curve>();
      if (intermediate) {
        Curve curve(*intermediate);
        return setPumpCurve(curve);
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

  bool PumpConstantSpeed_Impl::setZoneAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalThermalZone intermediate = modelObject->optionalCast<ThermalZone>();
      if (intermediate) {
        ThermalZone thermalZone(*intermediate);
        return setZone(thermalZone);
      }
      else {
        return false;
      }
    }
    else {
      resetZone();
    }
    return true;
  }

  unsigned PumpConstantSpeed_Impl::inletPort()
  {
    return OS_Pump_ConstantSpeedFields::InletNodeName;
  }

  unsigned PumpConstantSpeed_Impl::outletPort()
  {
    return OS_Pump_ConstantSpeedFields::OutletNodeName;
  }

  bool PumpConstantSpeed_Impl::addToNode(Node & node)
  {
    if( boost::optional<PlantLoop> plant = node.plantLoop() )
    {
      return StraightComponent_Impl::addToNode(node);
    }

    return false;
  }

} // detail

PumpConstantSpeed::PumpConstantSpeed(const Model& model)
  : StraightComponent(PumpConstantSpeed::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::PumpConstantSpeed_Impl>());

  autosizeRatedFlowRate();
  autosizeRatedPowerConsumption();
  setRatedPumpHead(179352.0);
  setMotorEfficiency(0.9);
  setPumpControlType("Intermittent");
  setFractionofMotorInefficienciestoFluidStream(0.0);

  setString(OS_Pump_ConstantSpeedFields::PumpFlowRateSchedule,"");
  setString(OS_Pump_ConstantSpeedFields::PumpCurve,"");
  setString(OS_Pump_ConstantSpeedFields::ImpellerDiameter,"");
  setString(OS_Pump_ConstantSpeedFields::RotationalSpeed,"");
  setString(OS_Pump_ConstantSpeedFields::Zone,"");
  setString(OS_Pump_ConstantSpeedFields::SkinLossRadiativeFraction,"");
}

IddObjectType PumpConstantSpeed::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Pump_ConstantSpeed);
}

std::vector<std::string> PumpConstantSpeed::pumpControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Pump_ConstantSpeedFields::PumpControlType);
}

boost::optional<double> PumpConstantSpeed::ratedFlowRate() const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->ratedFlowRate();
}

OSOptionalQuantity PumpConstantSpeed::getRatedFlowRate(bool returnIP) const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->getRatedFlowRate(returnIP);
}

bool PumpConstantSpeed::isRatedFlowRateAutosized() const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->isRatedFlowRateAutosized();
}

double PumpConstantSpeed::ratedPumpHead() const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->ratedPumpHead();
}

Quantity PumpConstantSpeed::getRatedPumpHead(bool returnIP) const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->getRatedPumpHead(returnIP);
}

bool PumpConstantSpeed::isRatedPumpHeadDefaulted() const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->isRatedPumpHeadDefaulted();
}

boost::optional<double> PumpConstantSpeed::ratedPowerConsumption() const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->ratedPowerConsumption();
}

OSOptionalQuantity PumpConstantSpeed::getRatedPowerConsumption(bool returnIP) const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->getRatedPowerConsumption(returnIP);
}

bool PumpConstantSpeed::isRatedPowerConsumptionAutosized() const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->isRatedPowerConsumptionAutosized();
}

double PumpConstantSpeed::motorEfficiency() const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->motorEfficiency();
}

Quantity PumpConstantSpeed::getMotorEfficiency(bool returnIP) const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->getMotorEfficiency(returnIP);
}

bool PumpConstantSpeed::isMotorEfficiencyDefaulted() const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->isMotorEfficiencyDefaulted();
}

double PumpConstantSpeed::fractionofMotorInefficienciestoFluidStream() const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->fractionofMotorInefficienciestoFluidStream();
}

Quantity PumpConstantSpeed::getFractionofMotorInefficienciestoFluidStream(bool returnIP) const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->getFractionofMotorInefficienciestoFluidStream(returnIP);
}

bool PumpConstantSpeed::isFractionofMotorInefficienciestoFluidStreamDefaulted() const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->isFractionofMotorInefficienciestoFluidStreamDefaulted();
}

std::string PumpConstantSpeed::pumpControlType() const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->pumpControlType();
}

bool PumpConstantSpeed::isPumpControlTypeDefaulted() const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->isPumpControlTypeDefaulted();
}

boost::optional<Schedule> PumpConstantSpeed::pumpFlowRateSchedule() const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->pumpFlowRateSchedule();
}

boost::optional<Curve> PumpConstantSpeed::pumpCurve() const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->pumpCurve();
}

boost::optional<double> PumpConstantSpeed::impellerDiameter() const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->impellerDiameter();
}

OSOptionalQuantity PumpConstantSpeed::getImpellerDiameter(bool returnIP) const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->getImpellerDiameter(returnIP);
}

boost::optional<double> PumpConstantSpeed::rotationalSpeed() const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->rotationalSpeed();
}

OSOptionalQuantity PumpConstantSpeed::getRotationalSpeed(bool returnIP) const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->getRotationalSpeed(returnIP);
}

boost::optional<ThermalZone> PumpConstantSpeed::zone() const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->zone();
}

boost::optional<double> PumpConstantSpeed::skinLossRadiativeFraction() const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->skinLossRadiativeFraction();
}

OSOptionalQuantity PumpConstantSpeed::getSkinLossRadiativeFraction(bool returnIP) const {
  return getImpl<detail::PumpConstantSpeed_Impl>()->getSkinLossRadiativeFraction(returnIP);
}

void PumpConstantSpeed::setRatedFlowRate(double ratedFlowRate) {
  getImpl<detail::PumpConstantSpeed_Impl>()->setRatedFlowRate(ratedFlowRate);
}

bool PumpConstantSpeed::setRatedFlowRate(const Quantity& ratedFlowRate) {
  return getImpl<detail::PumpConstantSpeed_Impl>()->setRatedFlowRate(ratedFlowRate);
}

void PumpConstantSpeed::resetRatedFlowRate() {
  getImpl<detail::PumpConstantSpeed_Impl>()->resetRatedFlowRate();
}

void PumpConstantSpeed::autosizeRatedFlowRate() {
  getImpl<detail::PumpConstantSpeed_Impl>()->autosizeRatedFlowRate();
}

void PumpConstantSpeed::setRatedPumpHead(double ratedPumpHead) {
  getImpl<detail::PumpConstantSpeed_Impl>()->setRatedPumpHead(ratedPumpHead);
}

bool PumpConstantSpeed::setRatedPumpHead(const Quantity& ratedPumpHead) {
  return getImpl<detail::PumpConstantSpeed_Impl>()->setRatedPumpHead(ratedPumpHead);
}

void PumpConstantSpeed::resetRatedPumpHead() {
  getImpl<detail::PumpConstantSpeed_Impl>()->resetRatedPumpHead();
}

void PumpConstantSpeed::setRatedPowerConsumption(double ratedPowerConsumption) {
  getImpl<detail::PumpConstantSpeed_Impl>()->setRatedPowerConsumption(ratedPowerConsumption);
}

bool PumpConstantSpeed::setRatedPowerConsumption(const Quantity& ratedPowerConsumption) {
  return getImpl<detail::PumpConstantSpeed_Impl>()->setRatedPowerConsumption(ratedPowerConsumption);
}

void PumpConstantSpeed::resetRatedPowerConsumption() {
  getImpl<detail::PumpConstantSpeed_Impl>()->resetRatedPowerConsumption();
}

void PumpConstantSpeed::autosizeRatedPowerConsumption() {
  getImpl<detail::PumpConstantSpeed_Impl>()->autosizeRatedPowerConsumption();
}

bool PumpConstantSpeed::setMotorEfficiency(double motorEfficiency) {
  return getImpl<detail::PumpConstantSpeed_Impl>()->setMotorEfficiency(motorEfficiency);
}

bool PumpConstantSpeed::setMotorEfficiency(const Quantity& motorEfficiency) {
  return getImpl<detail::PumpConstantSpeed_Impl>()->setMotorEfficiency(motorEfficiency);
}

void PumpConstantSpeed::resetMotorEfficiency() {
  getImpl<detail::PumpConstantSpeed_Impl>()->resetMotorEfficiency();
}

bool PumpConstantSpeed::setFractionofMotorInefficienciestoFluidStream(double fractionofMotorInefficienciestoFluidStream) {
  return getImpl<detail::PumpConstantSpeed_Impl>()->setFractionofMotorInefficienciestoFluidStream(fractionofMotorInefficienciestoFluidStream);
}

bool PumpConstantSpeed::setFractionofMotorInefficienciestoFluidStream(const Quantity& fractionofMotorInefficienciestoFluidStream) {
  return getImpl<detail::PumpConstantSpeed_Impl>()->setFractionofMotorInefficienciestoFluidStream(fractionofMotorInefficienciestoFluidStream);
}

void PumpConstantSpeed::resetFractionofMotorInefficienciestoFluidStream() {
  getImpl<detail::PumpConstantSpeed_Impl>()->resetFractionofMotorInefficienciestoFluidStream();
}

bool PumpConstantSpeed::setPumpControlType(std::string pumpControlType) {
  return getImpl<detail::PumpConstantSpeed_Impl>()->setPumpControlType(pumpControlType);
}

void PumpConstantSpeed::resetPumpControlType() {
  getImpl<detail::PumpConstantSpeed_Impl>()->resetPumpControlType();
}

bool PumpConstantSpeed::setPumpFlowRateSchedule(Schedule& schedule) {
  return getImpl<detail::PumpConstantSpeed_Impl>()->setPumpFlowRateSchedule(schedule);
}

void PumpConstantSpeed::resetPumpFlowRateSchedule() {
  getImpl<detail::PumpConstantSpeed_Impl>()->resetPumpFlowRateSchedule();
}

bool PumpConstantSpeed::setPumpCurve(const Curve& curve) {
  return getImpl<detail::PumpConstantSpeed_Impl>()->setPumpCurve(curve);
}

void PumpConstantSpeed::resetPumpCurve() {
  getImpl<detail::PumpConstantSpeed_Impl>()->resetPumpCurve();
}

void PumpConstantSpeed::setImpellerDiameter(double impellerDiameter) {
  getImpl<detail::PumpConstantSpeed_Impl>()->setImpellerDiameter(impellerDiameter);
}

bool PumpConstantSpeed::setImpellerDiameter(const Quantity& impellerDiameter) {
  return getImpl<detail::PumpConstantSpeed_Impl>()->setImpellerDiameter(impellerDiameter);
}

void PumpConstantSpeed::resetImpellerDiameter() {
  getImpl<detail::PumpConstantSpeed_Impl>()->resetImpellerDiameter();
}

void PumpConstantSpeed::setRotationalSpeed(double rotationalSpeed) {
  getImpl<detail::PumpConstantSpeed_Impl>()->setRotationalSpeed(rotationalSpeed);
}

bool PumpConstantSpeed::setRotationalSpeed(const Quantity& rotationalSpeed) {
  return getImpl<detail::PumpConstantSpeed_Impl>()->setRotationalSpeed(rotationalSpeed);
}

void PumpConstantSpeed::resetRotationalSpeed() {
  getImpl<detail::PumpConstantSpeed_Impl>()->resetRotationalSpeed();
}

bool PumpConstantSpeed::setZone(const ThermalZone& thermalZone) {
  return getImpl<detail::PumpConstantSpeed_Impl>()->setZone(thermalZone);
}

void PumpConstantSpeed::resetZone() {
  getImpl<detail::PumpConstantSpeed_Impl>()->resetZone();
}

bool PumpConstantSpeed::setSkinLossRadiativeFraction(double skinLossRadiativeFraction) {
  return getImpl<detail::PumpConstantSpeed_Impl>()->setSkinLossRadiativeFraction(skinLossRadiativeFraction);
}

bool PumpConstantSpeed::setSkinLossRadiativeFraction(const Quantity& skinLossRadiativeFraction) {
  return getImpl<detail::PumpConstantSpeed_Impl>()->setSkinLossRadiativeFraction(skinLossRadiativeFraction);
}

void PumpConstantSpeed::resetSkinLossRadiativeFraction() {
  getImpl<detail::PumpConstantSpeed_Impl>()->resetSkinLossRadiativeFraction();
}

/// @cond
PumpConstantSpeed::PumpConstantSpeed(std::shared_ptr<detail::PumpConstantSpeed_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

