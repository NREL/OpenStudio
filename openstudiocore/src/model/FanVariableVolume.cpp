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

#include "FanConstantVolume.hpp"
#include "FanConstantVolume_Impl.hpp"
#include "FanVariableVolume.hpp"
#include "FanVariableVolume_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "ZoneHVACFourPipeFanCoil.hpp"
#include "ZoneHVACFourPipeFanCoil_Impl.hpp"
#include "ZoneHVACUnitHeater.hpp"
#include "ZoneHVACUnitHeater_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "SetpointManagerMixedAir.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Fan_VariableVolume_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  FanVariableVolume_Impl::FanVariableVolume_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == FanVariableVolume::iddObjectType());
  }

  FanVariableVolume_Impl::FanVariableVolume_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
                                                   : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == FanVariableVolume::iddObjectType());
  }

  FanVariableVolume_Impl::FanVariableVolume_Impl(const FanVariableVolume_Impl& other,
                                                 Model_Impl* model,
                                                 bool keepHandle)
                                                   : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& FanVariableVolume_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType FanVariableVolume_Impl::iddObjectType() const {
    return FanVariableVolume::iddObjectType();
  }

  std::vector<ScheduleTypeKey> FanVariableVolume_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Fan_VariableVolumeFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("FanVariableVolume","Availability"));
    }
    return result;
  }

  Schedule FanVariableVolume_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Fan_VariableVolumeFields::AvailabilityScheduleName);
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<FanVariableVolume_Impl*>(this)->setAvailabilitySchedule(*value);
      value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Fan_VariableVolumeFields::AvailabilityScheduleName);
    }
    OS_ASSERT(value);
    return value.get();
  }

  double FanVariableVolume_Impl::fanEfficiency() const {
    boost::optional<double> value = getDouble(OS_Fan_VariableVolumeFields::FanEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity FanVariableVolume_Impl::getFanEfficiency(bool returnIP) const {
    OptionalDouble value = fanEfficiency();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Fan_VariableVolumeFields::FanEfficiency, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool FanVariableVolume_Impl::isFanEfficiencyDefaulted() const {
    return isEmpty(OS_Fan_VariableVolumeFields::FanEfficiency);
  }

  double FanVariableVolume_Impl::pressureRise() const {
    boost::optional<double> value = getDouble(OS_Fan_VariableVolumeFields::PressureRise,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity FanVariableVolume_Impl::getPressureRise(bool returnIP) const {
    OptionalDouble value = pressureRise();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Fan_VariableVolumeFields::PressureRise, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  boost::optional<double> FanVariableVolume_Impl::maximumFlowRate() const {
    return getDouble(OS_Fan_VariableVolumeFields::MaximumFlowRate,true);
  }

  OSOptionalQuantity FanVariableVolume_Impl::getMaximumFlowRate(bool returnIP) const {
    OptionalDouble value = maximumFlowRate();
    return getQuantityFromDouble(OS_Fan_VariableVolumeFields::MaximumFlowRate, value, returnIP);
  }

  bool FanVariableVolume_Impl::isMaximumFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Fan_VariableVolumeFields::MaximumFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  std::string FanVariableVolume_Impl::fanPowerMinimumFlowRateInputMethod() const {
    boost::optional<std::string> value = getString(OS_Fan_VariableVolumeFields::FanPowerMinimumFlowRateInputMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FanVariableVolume_Impl::isFanPowerMinimumFlowRateInputMethodDefaulted() const {
    return isEmpty(OS_Fan_VariableVolumeFields::FanPowerMinimumFlowRateInputMethod);
  }

  double FanVariableVolume_Impl::fanPowerMinimumFlowFraction() const {
    boost::optional<double> value = getDouble(OS_Fan_VariableVolumeFields::FanPowerMinimumFlowFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity FanVariableVolume_Impl::getFanPowerMinimumFlowFraction(bool returnIP) const {
    OptionalDouble value = fanPowerMinimumFlowFraction();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Fan_VariableVolumeFields::FanPowerMinimumFlowFraction, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool FanVariableVolume_Impl::isFanPowerMinimumFlowFractionDefaulted() const {
    return isEmpty(OS_Fan_VariableVolumeFields::FanPowerMinimumFlowFraction);
  }

  boost::optional<double> FanVariableVolume_Impl::fanPowerMinimumAirFlowRate() const {
    return getDouble(OS_Fan_VariableVolumeFields::FanPowerMinimumAirFlowRate,true);
  }

  OSOptionalQuantity FanVariableVolume_Impl::getFanPowerMinimumAirFlowRate(bool returnIP) const {
    OptionalDouble value = fanPowerMinimumAirFlowRate();
    return getQuantityFromDouble(OS_Fan_VariableVolumeFields::FanPowerMinimumAirFlowRate, value, returnIP);
  }

  double FanVariableVolume_Impl::motorEfficiency() const {
    boost::optional<double> value = getDouble(OS_Fan_VariableVolumeFields::MotorEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity FanVariableVolume_Impl::getMotorEfficiency(bool returnIP) const {
    OptionalDouble value = motorEfficiency();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Fan_VariableVolumeFields::MotorEfficiency, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool FanVariableVolume_Impl::isMotorEfficiencyDefaulted() const {
    return isEmpty(OS_Fan_VariableVolumeFields::MotorEfficiency);
  }

  double FanVariableVolume_Impl::motorInAirstreamFraction() const {
    boost::optional<double> value = getDouble(OS_Fan_VariableVolumeFields::MotorInAirstreamFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  Quantity FanVariableVolume_Impl::getMotorInAirstreamFraction(bool returnIP) const {
    OptionalDouble value = motorInAirstreamFraction();
    OSOptionalQuantity result = getQuantityFromDouble(OS_Fan_VariableVolumeFields::MotorInAirstreamFraction, value, returnIP);
    OS_ASSERT(result.isSet());
    return result.get();
  }

  bool FanVariableVolume_Impl::isMotorInAirstreamFractionDefaulted() const {
    return isEmpty(OS_Fan_VariableVolumeFields::MotorInAirstreamFraction);
  }

  boost::optional<double> FanVariableVolume_Impl::fanPowerCoefficient1() const {
    return getDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient1,true);
  }

  OSOptionalQuantity FanVariableVolume_Impl::getFanPowerCoefficient1(bool returnIP) const {
    OptionalDouble value = fanPowerCoefficient1();
    return getQuantityFromDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient1, value, returnIP);
  }

  boost::optional<double> FanVariableVolume_Impl::fanPowerCoefficient2() const {
    return getDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient2,true);
  }

  OSOptionalQuantity FanVariableVolume_Impl::getFanPowerCoefficient2(bool returnIP) const {
    OptionalDouble value = fanPowerCoefficient2();
    return getQuantityFromDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient2, value, returnIP);
  }

  boost::optional<double> FanVariableVolume_Impl::fanPowerCoefficient3() const {
    return getDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient3,true);
  }

  OSOptionalQuantity FanVariableVolume_Impl::getFanPowerCoefficient3(bool returnIP) const {
    OptionalDouble value = fanPowerCoefficient3();
    return getQuantityFromDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient3, value, returnIP);
  }

  boost::optional<double> FanVariableVolume_Impl::fanPowerCoefficient4() const {
    return getDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient4,true);
  }

  OSOptionalQuantity FanVariableVolume_Impl::getFanPowerCoefficient4(bool returnIP) const {
    OptionalDouble value = fanPowerCoefficient4();
    return getQuantityFromDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient4, value, returnIP);
  }

  boost::optional<double> FanVariableVolume_Impl::fanPowerCoefficient5() const {
    return getDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient5,true);
  }

  OSOptionalQuantity FanVariableVolume_Impl::getFanPowerCoefficient5(bool returnIP) const {
    OptionalDouble value = fanPowerCoefficient5();
    return getQuantityFromDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient5, value, returnIP);
  }

  std::string FanVariableVolume_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_Fan_VariableVolumeFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FanVariableVolume_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_Fan_VariableVolumeFields::EndUseSubcategory);
  }

  bool FanVariableVolume_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Fan_VariableVolumeFields::AvailabilityScheduleName,
                              "FanVariableVolume",
                              "Availability",
                              schedule);
    return result;
  }

  bool FanVariableVolume_Impl::setFanEfficiency(double fanEfficiency) {
    bool result = setDouble(OS_Fan_VariableVolumeFields::FanEfficiency, fanEfficiency);
    return result;
  }

  bool FanVariableVolume_Impl::setFanEfficiency(const Quantity& fanEfficiency) {
    OptionalDouble value = getDoubleFromQuantity(OS_Fan_VariableVolumeFields::FanEfficiency,fanEfficiency);
    if (!value) {
      return false;
    }
    return setFanEfficiency(value.get());
  }

  void FanVariableVolume_Impl::resetFanEfficiency() {
    bool result = setString(OS_Fan_VariableVolumeFields::FanEfficiency, "");
    OS_ASSERT(result);
  }

  void FanVariableVolume_Impl::setPressureRise(double pressureRise) {
    bool result = setDouble(OS_Fan_VariableVolumeFields::PressureRise, pressureRise);
    OS_ASSERT(result);
  }

  bool FanVariableVolume_Impl::setPressureRise(const Quantity& pressureRise) {
    OptionalDouble value = getDoubleFromQuantity(OS_Fan_VariableVolumeFields::PressureRise,pressureRise);
    if (!value) {
      return false;
    }
    setPressureRise(value.get());
    return true;
  }

  bool FanVariableVolume_Impl::setMaximumFlowRate(boost::optional<double> maximumFlowRate) {
    bool result = false;
    if (maximumFlowRate) {
      result = setDouble(OS_Fan_VariableVolumeFields::MaximumFlowRate, maximumFlowRate.get());
    } else {
      result = setString(OS_Fan_VariableVolumeFields::MaximumFlowRate, "");
    }
    return result;
  }

  bool FanVariableVolume_Impl::setMaximumFlowRate(const OSOptionalQuantity& maximumFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (maximumFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_Fan_VariableVolumeFields::MaximumFlowRate,maximumFlowRate.get());
      if (value) {
        result = setMaximumFlowRate(value);
      }
    }
    else {
      result = setMaximumFlowRate(value);
    }
    return result;
  }

  void FanVariableVolume_Impl::resetMaximumFlowRate() {
    bool result = setString(OS_Fan_VariableVolumeFields::MaximumFlowRate, "");
    OS_ASSERT(result);
  }

  void FanVariableVolume_Impl::autosizeMaximumFlowRate() {
    bool result = setString(OS_Fan_VariableVolumeFields::MaximumFlowRate, "Autosize");
    OS_ASSERT(result);
  }

  bool FanVariableVolume_Impl::setFanPowerMinimumFlowRateInputMethod(std::string fanPowerMinimumFlowRateInputMethod) {
    bool result = setString(OS_Fan_VariableVolumeFields::FanPowerMinimumFlowRateInputMethod, fanPowerMinimumFlowRateInputMethod);
    return result;
  }

  void FanVariableVolume_Impl::resetFanPowerMinimumFlowRateInputMethod() {
    bool result = setString(OS_Fan_VariableVolumeFields::FanPowerMinimumFlowRateInputMethod, "");
    OS_ASSERT(result);
  }

  bool FanVariableVolume_Impl::setFanPowerMinimumFlowFraction(double fanPowerMinimumFlowFraction) {
    bool result = setDouble(OS_Fan_VariableVolumeFields::FanPowerMinimumFlowFraction, fanPowerMinimumFlowFraction);
    return result;
  }

  bool FanVariableVolume_Impl::setFanPowerMinimumFlowFraction(const Quantity& fanPowerMinimumFlowFraction) {
    OptionalDouble value = getDoubleFromQuantity(OS_Fan_VariableVolumeFields::FanPowerMinimumFlowFraction,fanPowerMinimumFlowFraction);
    if (!value) {
      return false;
    }
    return setFanPowerMinimumFlowFraction(value.get());
  }

  void FanVariableVolume_Impl::resetFanPowerMinimumFlowFraction() {
    bool result = setString(OS_Fan_VariableVolumeFields::FanPowerMinimumFlowFraction, "");
    OS_ASSERT(result);
  }

  bool FanVariableVolume_Impl::setFanPowerMinimumAirFlowRate(boost::optional<double> fanPowerMinimumAirFlowRate) {
    bool result(false);
    if (fanPowerMinimumAirFlowRate) {
      result = setDouble(OS_Fan_VariableVolumeFields::FanPowerMinimumAirFlowRate, fanPowerMinimumAirFlowRate.get());
    }
    else {
      resetFanPowerMinimumAirFlowRate();
      result = true;
    }
    return result;
  }

  bool FanVariableVolume_Impl::setFanPowerMinimumAirFlowRate(const OSOptionalQuantity& fanPowerMinimumAirFlowRate) {
    bool result(false);
    OptionalDouble value;
    if (fanPowerMinimumAirFlowRate.isSet()) {
      value = getDoubleFromQuantity(OS_Fan_VariableVolumeFields::FanPowerMinimumAirFlowRate,fanPowerMinimumAirFlowRate.get());
      if (value) {
        result = setFanPowerMinimumAirFlowRate(value);
      }
    }
    else {
      result = setFanPowerMinimumAirFlowRate(value);
    }
    return result;
  }

  void FanVariableVolume_Impl::resetFanPowerMinimumAirFlowRate() {
    bool result = setString(OS_Fan_VariableVolumeFields::FanPowerMinimumAirFlowRate, "");
    OS_ASSERT(result);
  }

  bool FanVariableVolume_Impl::setMotorEfficiency(double motorEfficiency) {
    bool result = setDouble(OS_Fan_VariableVolumeFields::MotorEfficiency, motorEfficiency);
    return result;
  }

  bool FanVariableVolume_Impl::setMotorEfficiency(const Quantity& motorEfficiency) {
    OptionalDouble value = getDoubleFromQuantity(OS_Fan_VariableVolumeFields::MotorEfficiency,motorEfficiency);
    if (!value) {
      return false;
    }
    return setMotorEfficiency(value.get());
  }

  void FanVariableVolume_Impl::resetMotorEfficiency() {
    bool result = setString(OS_Fan_VariableVolumeFields::MotorEfficiency, "");
    OS_ASSERT(result);
  }

  bool FanVariableVolume_Impl::setMotorInAirstreamFraction(double motorInAirstreamFraction) {
    bool result = setDouble(OS_Fan_VariableVolumeFields::MotorInAirstreamFraction, motorInAirstreamFraction);
    return result;
  }

  bool FanVariableVolume_Impl::setMotorInAirstreamFraction(const Quantity& motorInAirstreamFraction) {
    OptionalDouble value = getDoubleFromQuantity(OS_Fan_VariableVolumeFields::MotorInAirstreamFraction,motorInAirstreamFraction);
    if (!value) {
      return false;
    }
    return setMotorInAirstreamFraction(value.get());
  }

  void FanVariableVolume_Impl::resetMotorInAirstreamFraction() {
    bool result = setString(OS_Fan_VariableVolumeFields::MotorInAirstreamFraction, "");
    OS_ASSERT(result);
  }

  void FanVariableVolume_Impl::setFanPowerCoefficient1(boost::optional<double> fanPowerCoefficient1) {
    bool result(false);
    if (fanPowerCoefficient1) {
      result = setDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient1, fanPowerCoefficient1.get());
    }
    else {
      resetFanPowerCoefficient1();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool FanVariableVolume_Impl::setFanPowerCoefficient1(const OSOptionalQuantity& fanPowerCoefficient1) {
    bool result(false);
    OptionalDouble value;
    if (fanPowerCoefficient1.isSet()) {
      value = getDoubleFromQuantity(OS_Fan_VariableVolumeFields::FanPowerCoefficient1,fanPowerCoefficient1.get());
      if (value) {
        setFanPowerCoefficient1(value);
        result = true;
      }
    }
    else {
      setFanPowerCoefficient1(value);
      result = true;
    }
    return result;
  }

  void FanVariableVolume_Impl::resetFanPowerCoefficient1() {
    bool result = setString(OS_Fan_VariableVolumeFields::FanPowerCoefficient1, "");
    OS_ASSERT(result);
  }

  void FanVariableVolume_Impl::setFanPowerCoefficient2(boost::optional<double> fanPowerCoefficient2) {
    bool result(false);
    if (fanPowerCoefficient2) {
      result = setDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient2, fanPowerCoefficient2.get());
    }
    else {
      resetFanPowerCoefficient2();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool FanVariableVolume_Impl::setFanPowerCoefficient2(const OSOptionalQuantity& fanPowerCoefficient2) {
    bool result(false);
    OptionalDouble value;
    if (fanPowerCoefficient2.isSet()) {
      value = getDoubleFromQuantity(OS_Fan_VariableVolumeFields::FanPowerCoefficient2,fanPowerCoefficient2.get());
      if (value) {
        setFanPowerCoefficient2(value);
        result = true;
      }
    }
    else {
      setFanPowerCoefficient2(value);
      result = true;
    }
    return result;
  }

  void FanVariableVolume_Impl::resetFanPowerCoefficient2() {
    bool result = setString(OS_Fan_VariableVolumeFields::FanPowerCoefficient2, "");
    OS_ASSERT(result);
  }

  void FanVariableVolume_Impl::setFanPowerCoefficient3(boost::optional<double> fanPowerCoefficient3) {
    bool result(false);
    if (fanPowerCoefficient3) {
      result = setDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient3, fanPowerCoefficient3.get());
    }
    else {
      resetFanPowerCoefficient3();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool FanVariableVolume_Impl::setFanPowerCoefficient3(const OSOptionalQuantity& fanPowerCoefficient3) {
    bool result(false);
    OptionalDouble value;
    if (fanPowerCoefficient3.isSet()) {
      value = getDoubleFromQuantity(OS_Fan_VariableVolumeFields::FanPowerCoefficient3,fanPowerCoefficient3.get());
      if (value) {
        setFanPowerCoefficient3(value);
        result = true;
      }
    }
    else {
      setFanPowerCoefficient3(value);
      result = true;
    }
    return result;
  }

  void FanVariableVolume_Impl::resetFanPowerCoefficient3() {
    bool result = setString(OS_Fan_VariableVolumeFields::FanPowerCoefficient3, "");
    OS_ASSERT(result);
  }

  void FanVariableVolume_Impl::setFanPowerCoefficient4(boost::optional<double> fanPowerCoefficient4) {
    bool result(false);
    if (fanPowerCoefficient4) {
      result = setDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient4, fanPowerCoefficient4.get());
    }
    else {
      resetFanPowerCoefficient4();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool FanVariableVolume_Impl::setFanPowerCoefficient4(const OSOptionalQuantity& fanPowerCoefficient4) {
    bool result(false);
    OptionalDouble value;
    if (fanPowerCoefficient4.isSet()) {
      value = getDoubleFromQuantity(OS_Fan_VariableVolumeFields::FanPowerCoefficient4,fanPowerCoefficient4.get());
      if (value) {
        setFanPowerCoefficient4(value);
        result = true;
      }
    }
    else {
      setFanPowerCoefficient4(value);
      result = true;
    }
    return result;
  }

  void FanVariableVolume_Impl::resetFanPowerCoefficient4() {
    bool result = setString(OS_Fan_VariableVolumeFields::FanPowerCoefficient4, "");
    OS_ASSERT(result);
  }

  void FanVariableVolume_Impl::setFanPowerCoefficient5(boost::optional<double> fanPowerCoefficient5) {
    bool result(false);
    if (fanPowerCoefficient5) {
      result = setDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient5, fanPowerCoefficient5.get());
    }
    else {
      resetFanPowerCoefficient5();
      result = true;
    }
    OS_ASSERT(result);
  }

  bool FanVariableVolume_Impl::setFanPowerCoefficient5(const OSOptionalQuantity& fanPowerCoefficient5) {
    bool result(false);
    OptionalDouble value;
    if (fanPowerCoefficient5.isSet()) {
      value = getDoubleFromQuantity(OS_Fan_VariableVolumeFields::FanPowerCoefficient5,fanPowerCoefficient5.get());
      if (value) {
        setFanPowerCoefficient5(value);
        result = true;
      }
    }
    else {
      setFanPowerCoefficient5(value);
      result = true;
    }
    return result;
  }

  void FanVariableVolume_Impl::resetFanPowerCoefficient5() {
    bool result = setString(OS_Fan_VariableVolumeFields::FanPowerCoefficient5, "");
    OS_ASSERT(result);
  }

  void FanVariableVolume_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_Fan_VariableVolumeFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
  }

  void FanVariableVolume_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_Fan_VariableVolumeFields::EndUseSubcategory, "");
    OS_ASSERT(result);
  }

  unsigned FanVariableVolume_Impl::inletPort()
  {
    return OS_Fan_VariableVolumeFields::AirInletNodeName;
  }

  unsigned FanVariableVolume_Impl::outletPort()
  {
    return OS_Fan_VariableVolumeFields::AirOutletNodeName;
  }

  bool FanVariableVolume_Impl::addToNode(Node & node)
  {
    if( boost::optional<AirLoopHVAC> airLoop = node.airLoopHVAC() )
    {
      boost::optional<AirLoopHVACOutdoorAirSystem> oaSystem = airLoop->airLoopHVACOutdoorAirSystem();
      if( airLoop->supplyComponent(node.handle()) || (oaSystem && oaSystem->component(node.handle())) )
      {
        unsigned fanCount = airLoop->supplyComponents(IddObjectType::OS_Fan_ConstantVolume).size();
        fanCount += airLoop->supplyComponents(IddObjectType::OS_Fan_VariableVolume).size();

        if( oaSystem )
        {
          fanCount += subsetCastVector<FanConstantVolume>(oaSystem->components()).size();
          fanCount += subsetCastVector<FanVariableVolume>(oaSystem->components()).size();
        }

        if( StraightComponent_Impl::addToNode(node) )
        {
          SetpointManagerMixedAir::updateFanInletOutletNodes(airLoop.get());
          return true;
        }
      }
    }

    return false;
  }

  openstudio::Quantity FanVariableVolume_Impl::fanEfficiency_SI() const {
    return getFanEfficiency(false);
  }

  openstudio::Quantity FanVariableVolume_Impl::fanEfficiency_IP() const {
    return getFanEfficiency(true);
  }

  openstudio::Quantity FanVariableVolume_Impl::pressureRise_SI() const {
    return getPressureRise(false);
  }

  openstudio::Quantity FanVariableVolume_Impl::pressureRise_IP() const {
    return getPressureRise(true);
  }

  openstudio::OSOptionalQuantity FanVariableVolume_Impl::maximumFlowRate_SI() const {
    return getMaximumFlowRate(false);
  }

  openstudio::OSOptionalQuantity FanVariableVolume_Impl::maximumFlowRate_IP() const {
    return getMaximumFlowRate(true);
  }

  std::vector<std::string> FanVariableVolume_Impl::fanPowerMinimumFlowRateInputMethodValues() const {
    return FanVariableVolume::fanPowerMinimumFlowRateInputMethodValues();
  }

  openstudio::Quantity FanVariableVolume_Impl::fanPowerMinimumFlowFraction_SI() const {
    return getFanPowerMinimumFlowFraction(false);
  }

  openstudio::Quantity FanVariableVolume_Impl::fanPowerMinimumFlowFraction_IP() const {
    return getFanPowerMinimumFlowFraction(true);
  }

  openstudio::OSOptionalQuantity FanVariableVolume_Impl::fanPowerMinimumAirFlowRate_SI() const {
    return getFanPowerMinimumAirFlowRate(false);
  }

  openstudio::OSOptionalQuantity FanVariableVolume_Impl::fanPowerMinimumAirFlowRate_IP() const {
    return getFanPowerMinimumAirFlowRate(true);
  }

  openstudio::Quantity FanVariableVolume_Impl::motorEfficiency_SI() const {
    return getMotorEfficiency(false);
  }

  openstudio::Quantity FanVariableVolume_Impl::motorEfficiency_IP() const {
    return getMotorEfficiency(true);
  }

  openstudio::Quantity FanVariableVolume_Impl::motorInAirstreamFraction_SI() const {
    return getMotorInAirstreamFraction(false);
  }

  openstudio::Quantity FanVariableVolume_Impl::motorInAirstreamFraction_IP() const {
    return getMotorInAirstreamFraction(true);
  }

  openstudio::OSOptionalQuantity FanVariableVolume_Impl::fanPowerCoefficient1_SI() const {
    return getFanPowerCoefficient1(false);
  }

  openstudio::OSOptionalQuantity FanVariableVolume_Impl::fanPowerCoefficient1_IP() const {
    return getFanPowerCoefficient1(true);
  }

  openstudio::OSOptionalQuantity FanVariableVolume_Impl::fanPowerCoefficient2_SI() const {
    return getFanPowerCoefficient2(false);
  }

  openstudio::OSOptionalQuantity FanVariableVolume_Impl::fanPowerCoefficient2_IP() const {
    return getFanPowerCoefficient2(true);
  }

  openstudio::OSOptionalQuantity FanVariableVolume_Impl::fanPowerCoefficient3_SI() const {
    return getFanPowerCoefficient3(false);
  }

  openstudio::OSOptionalQuantity FanVariableVolume_Impl::fanPowerCoefficient3_IP() const {
    return getFanPowerCoefficient3(true);
  }

  openstudio::OSOptionalQuantity FanVariableVolume_Impl::fanPowerCoefficient4_SI() const {
    return getFanPowerCoefficient4(false);
  }

  openstudio::OSOptionalQuantity FanVariableVolume_Impl::fanPowerCoefficient4_IP() const {
    return getFanPowerCoefficient4(true);
  }

  openstudio::OSOptionalQuantity FanVariableVolume_Impl::fanPowerCoefficient5_SI() const {
    return getFanPowerCoefficient5(false);
  }

  openstudio::OSOptionalQuantity FanVariableVolume_Impl::fanPowerCoefficient5_IP() const {
    return getFanPowerCoefficient5(true);
  }

  boost::optional<ModelObject> FanVariableVolume_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  bool FanVariableVolume_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

  boost::optional<HVACComponent> FanVariableVolume_Impl::containingHVACComponent() const
  {
    // AirLoopHVACUnitarySystem
    std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

    for( const auto & airLoopHVACUnitarySystem : airLoopHVACUnitarySystems )
    {
      if( boost::optional<HVACComponent> fan = airLoopHVACUnitarySystem.supplyFan() )
      {
        if( fan->handle() == this->handle() )
        {
          return airLoopHVACUnitarySystem;
        }
      }
    }
    return boost::none;
  }

  boost::optional<ZoneHVACComponent> FanVariableVolume_Impl::containingZoneHVACComponent() const
  {
    // ZoneHVACFourPipeFanCoil

    std::vector<ZoneHVACFourPipeFanCoil> zoneHVACFourPipeFanCoils;

    zoneHVACFourPipeFanCoils = this->model().getConcreteModelObjects<ZoneHVACFourPipeFanCoil>();

    for( const auto & zoneHVACFourPipeFanCoil : zoneHVACFourPipeFanCoils )
    {
      if( boost::optional<HVACComponent> coil = zoneHVACFourPipeFanCoil.supplyAirFan() )
      {
        if( coil->handle() == this->handle() )
        {
          return zoneHVACFourPipeFanCoil;
        }
      }
    }

    // ZoneHVACUnitHeater

    std::vector<ZoneHVACUnitHeater> zoneHVACUnitHeater;

    zoneHVACUnitHeater = this->model().getConcreteModelObjects<ZoneHVACUnitHeater>();

    for( const auto & elem : zoneHVACUnitHeater )
    {
      if( boost::optional<HVACComponent> coil = elem.supplyAirFan() )
      {
        if( coil->handle() == this->handle() )
        {
          return elem;
        }
      }
    }

    return boost::none;
  }

} // detail

FanVariableVolume::FanVariableVolume(const Model& model, Schedule & schedule)
  : StraightComponent(FanVariableVolume::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::FanVariableVolume_Impl>());

  bool ok = setAvailabilitySchedule(schedule);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s availability schedule to "
                  << schedule.briefDescription() << ".");
  }
  setEndUseSubcategory("");
  setFanEfficiency(0.6045);
  setPressureRise(1017.592);
  autosizeMaximumFlowRate();
  setFanPowerMinimumFlowRateInputMethod("FixedFlowRate");
  setFanPowerMinimumFlowFraction(0.0);
  setFanPowerMinimumAirFlowRate(0.0);
  setMotorEfficiency(0.93);
  setMotorInAirstreamFraction(1.0);
  setFanPowerCoefficient1(0.0407598940);
  setFanPowerCoefficient2(0.08804497);
  setFanPowerCoefficient3(-0.072926120);
  setFanPowerCoefficient4(0.9437398230);
  setFanPowerCoefficient5(0.0);
}

FanVariableVolume::FanVariableVolume(const Model& model)
  : StraightComponent(FanVariableVolume::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::FanVariableVolume_Impl>());

  auto schedule = model.alwaysOnDiscreteSchedule();
  setAvailabilitySchedule(schedule);

  setEndUseSubcategory("");
  setFanEfficiency(0.6045);
  setPressureRise(1017.592);
  autosizeMaximumFlowRate();
  setFanPowerMinimumFlowRateInputMethod("FixedFlowRate");
  setFanPowerMinimumFlowFraction(0.0);
  setFanPowerMinimumAirFlowRate(0.0);
  setMotorEfficiency(0.93);
  setMotorInAirstreamFraction(1.0);
  setFanPowerCoefficient1(0.0407598940);
  setFanPowerCoefficient2(0.08804497);
  setFanPowerCoefficient3(-0.072926120);
  setFanPowerCoefficient4(0.9437398230);
  setFanPowerCoefficient5(0.0);
}

IddObjectType FanVariableVolume::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Fan_VariableVolume);
  return result;
}

std::vector<std::string> FanVariableVolume::fanPowerMinimumFlowRateInputMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Fan_VariableVolumeFields::FanPowerMinimumFlowRateInputMethod);
}

std::vector<std::string> FanVariableVolume::validFanPowerMinimumFlowRateInputMethodValues() {
  return FanVariableVolume::fanPowerMinimumFlowRateInputMethodValues();
}

Schedule FanVariableVolume::availabilitySchedule() const {
  return getImpl<detail::FanVariableVolume_Impl>()->availabilitySchedule();
}

double FanVariableVolume::fanEfficiency() const {
  return getImpl<detail::FanVariableVolume_Impl>()->fanEfficiency();
}

Quantity FanVariableVolume::getFanEfficiency(bool returnIP) const {
  return getImpl<detail::FanVariableVolume_Impl>()->getFanEfficiency(returnIP);
}

bool FanVariableVolume::isFanEfficiencyDefaulted() const {
  return getImpl<detail::FanVariableVolume_Impl>()->isFanEfficiencyDefaulted();
}

double FanVariableVolume::pressureRise() const {
  return getImpl<detail::FanVariableVolume_Impl>()->pressureRise();
}

Quantity FanVariableVolume::getPressureRise(bool returnIP) const {
  return getImpl<detail::FanVariableVolume_Impl>()->getPressureRise(returnIP);
}

boost::optional<double> FanVariableVolume::maximumFlowRate() const {
  return getImpl<detail::FanVariableVolume_Impl>()->maximumFlowRate();
}

OSOptionalQuantity FanVariableVolume::getMaximumFlowRate(bool returnIP) const {
  return getImpl<detail::FanVariableVolume_Impl>()->getMaximumFlowRate(returnIP);
}

bool FanVariableVolume::isMaximumFlowRateAutosized() const {
  return getImpl<detail::FanVariableVolume_Impl>()->isMaximumFlowRateAutosized();
}

std::string FanVariableVolume::fanPowerMinimumFlowRateInputMethod() const {
  return getImpl<detail::FanVariableVolume_Impl>()->fanPowerMinimumFlowRateInputMethod();
}

bool FanVariableVolume::isFanPowerMinimumFlowRateInputMethodDefaulted() const {
  return getImpl<detail::FanVariableVolume_Impl>()->isFanPowerMinimumFlowRateInputMethodDefaulted();
}

double FanVariableVolume::fanPowerMinimumFlowFraction() const {
  return getImpl<detail::FanVariableVolume_Impl>()->fanPowerMinimumFlowFraction();
}

Quantity FanVariableVolume::getFanPowerMinimumFlowFraction(bool returnIP) const {
  return getImpl<detail::FanVariableVolume_Impl>()->getFanPowerMinimumFlowFraction(returnIP);
}

bool FanVariableVolume::isFanPowerMinimumFlowFractionDefaulted() const {
  return getImpl<detail::FanVariableVolume_Impl>()->isFanPowerMinimumFlowFractionDefaulted();
}

boost::optional<double> FanVariableVolume::fanPowerMinimumAirFlowRate() const {
  return getImpl<detail::FanVariableVolume_Impl>()->fanPowerMinimumAirFlowRate();
}

OSOptionalQuantity FanVariableVolume::getFanPowerMinimumAirFlowRate(bool returnIP) const {
  return getImpl<detail::FanVariableVolume_Impl>()->getFanPowerMinimumAirFlowRate(returnIP);
}

double FanVariableVolume::motorEfficiency() const {
  return getImpl<detail::FanVariableVolume_Impl>()->motorEfficiency();
}

Quantity FanVariableVolume::getMotorEfficiency(bool returnIP) const {
  return getImpl<detail::FanVariableVolume_Impl>()->getMotorEfficiency(returnIP);
}

bool FanVariableVolume::isMotorEfficiencyDefaulted() const {
  return getImpl<detail::FanVariableVolume_Impl>()->isMotorEfficiencyDefaulted();
}

double FanVariableVolume::motorInAirstreamFraction() const {
  return getImpl<detail::FanVariableVolume_Impl>()->motorInAirstreamFraction();
}

Quantity FanVariableVolume::getMotorInAirstreamFraction(bool returnIP) const {
  return getImpl<detail::FanVariableVolume_Impl>()->getMotorInAirstreamFraction(returnIP);
}

bool FanVariableVolume::isMotorInAirstreamFractionDefaulted() const {
  return getImpl<detail::FanVariableVolume_Impl>()->isMotorInAirstreamFractionDefaulted();
}

boost::optional<double> FanVariableVolume::fanPowerCoefficient1() const {
  return getImpl<detail::FanVariableVolume_Impl>()->fanPowerCoefficient1();
}

OSOptionalQuantity FanVariableVolume::getFanPowerCoefficient1(bool returnIP) const {
  return getImpl<detail::FanVariableVolume_Impl>()->getFanPowerCoefficient1(returnIP);
}

boost::optional<double> FanVariableVolume::fanPowerCoefficient2() const {
  return getImpl<detail::FanVariableVolume_Impl>()->fanPowerCoefficient2();
}

OSOptionalQuantity FanVariableVolume::getFanPowerCoefficient2(bool returnIP) const {
  return getImpl<detail::FanVariableVolume_Impl>()->getFanPowerCoefficient2(returnIP);
}

boost::optional<double> FanVariableVolume::fanPowerCoefficient3() const {
  return getImpl<detail::FanVariableVolume_Impl>()->fanPowerCoefficient3();
}

OSOptionalQuantity FanVariableVolume::getFanPowerCoefficient3(bool returnIP) const {
  return getImpl<detail::FanVariableVolume_Impl>()->getFanPowerCoefficient3(returnIP);
}

boost::optional<double> FanVariableVolume::fanPowerCoefficient4() const {
  return getImpl<detail::FanVariableVolume_Impl>()->fanPowerCoefficient4();
}

OSOptionalQuantity FanVariableVolume::getFanPowerCoefficient4(bool returnIP) const {
  return getImpl<detail::FanVariableVolume_Impl>()->getFanPowerCoefficient4(returnIP);
}

boost::optional<double> FanVariableVolume::fanPowerCoefficient5() const {
  return getImpl<detail::FanVariableVolume_Impl>()->fanPowerCoefficient5();
}

OSOptionalQuantity FanVariableVolume::getFanPowerCoefficient5(bool returnIP) const {
  return getImpl<detail::FanVariableVolume_Impl>()->getFanPowerCoefficient5(returnIP);
}

std::string FanVariableVolume::endUseSubcategory() const {
  return getImpl<detail::FanVariableVolume_Impl>()->endUseSubcategory();
}

bool FanVariableVolume::isEndUseSubcategoryDefaulted() const {
  return getImpl<detail::FanVariableVolume_Impl>()->isEndUseSubcategoryDefaulted();
}

bool FanVariableVolume::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::FanVariableVolume_Impl>()->setAvailabilitySchedule(schedule);
}

bool FanVariableVolume::setFanEfficiency(double fanEfficiency) {
  return getImpl<detail::FanVariableVolume_Impl>()->setFanEfficiency(fanEfficiency);
}

bool FanVariableVolume::setFanEfficiency(const Quantity& fanEfficiency) {
  return getImpl<detail::FanVariableVolume_Impl>()->setFanEfficiency(fanEfficiency);
}

void FanVariableVolume::resetFanEfficiency() {
  getImpl<detail::FanVariableVolume_Impl>()->resetFanEfficiency();
}

void FanVariableVolume::setPressureRise(double pressureRise) {
  getImpl<detail::FanVariableVolume_Impl>()->setPressureRise(pressureRise);
}

bool FanVariableVolume::setPressureRise(const Quantity& pressureRise) {
  return getImpl<detail::FanVariableVolume_Impl>()->setPressureRise(pressureRise);
}

bool FanVariableVolume::setMaximumFlowRate(double maximumFlowRate) {
  return getImpl<detail::FanVariableVolume_Impl>()->setMaximumFlowRate(maximumFlowRate);
}

bool FanVariableVolume::setMaximumFlowRate(const Quantity& maximumFlowRate) {
  return getImpl<detail::FanVariableVolume_Impl>()->setMaximumFlowRate(maximumFlowRate);
}

void FanVariableVolume::resetMaximumFlowRate() {
  getImpl<detail::FanVariableVolume_Impl>()->resetMaximumFlowRate();
}

void FanVariableVolume::autosizeMaximumFlowRate() {
  getImpl<detail::FanVariableVolume_Impl>()->autosizeMaximumFlowRate();
}

bool FanVariableVolume::setFanPowerMinimumFlowRateInputMethod(std::string fanPowerMinimumFlowRateInputMethod) {
  return getImpl<detail::FanVariableVolume_Impl>()->setFanPowerMinimumFlowRateInputMethod(fanPowerMinimumFlowRateInputMethod);
}

void FanVariableVolume::resetFanPowerMinimumFlowRateInputMethod() {
  getImpl<detail::FanVariableVolume_Impl>()->resetFanPowerMinimumFlowRateInputMethod();
}

bool FanVariableVolume::setFanPowerMinimumFlowFraction(double fanPowerMinimumFlowFraction) {
  return getImpl<detail::FanVariableVolume_Impl>()->setFanPowerMinimumFlowFraction(fanPowerMinimumFlowFraction);
}

bool FanVariableVolume::setFanPowerMinimumFlowFraction(const Quantity& fanPowerMinimumFlowFraction) {
  return getImpl<detail::FanVariableVolume_Impl>()->setFanPowerMinimumFlowFraction(fanPowerMinimumFlowFraction);
}

void FanVariableVolume::resetFanPowerMinimumFlowFraction() {
  getImpl<detail::FanVariableVolume_Impl>()->resetFanPowerMinimumFlowFraction();
}

bool FanVariableVolume::setFanPowerMinimumAirFlowRate(double fanPowerMinimumAirFlowRate) {
  return getImpl<detail::FanVariableVolume_Impl>()->setFanPowerMinimumAirFlowRate(fanPowerMinimumAirFlowRate);
}

bool FanVariableVolume::setFanPowerMinimumAirFlowRate(const Quantity& fanPowerMinimumAirFlowRate) {
  return getImpl<detail::FanVariableVolume_Impl>()->setFanPowerMinimumAirFlowRate(fanPowerMinimumAirFlowRate);
}

void FanVariableVolume::resetFanPowerMinimumAirFlowRate() {
  getImpl<detail::FanVariableVolume_Impl>()->resetFanPowerMinimumAirFlowRate();
}

bool FanVariableVolume::setMotorEfficiency(double motorEfficiency) {
  return getImpl<detail::FanVariableVolume_Impl>()->setMotorEfficiency(motorEfficiency);
}

bool FanVariableVolume::setMotorEfficiency(const Quantity& motorEfficiency) {
  return getImpl<detail::FanVariableVolume_Impl>()->setMotorEfficiency(motorEfficiency);
}

void FanVariableVolume::resetMotorEfficiency() {
  getImpl<detail::FanVariableVolume_Impl>()->resetMotorEfficiency();
}

bool FanVariableVolume::setMotorInAirstreamFraction(double motorInAirstreamFraction) {
  return getImpl<detail::FanVariableVolume_Impl>()->setMotorInAirstreamFraction(motorInAirstreamFraction);
}

bool FanVariableVolume::setMotorInAirstreamFraction(const Quantity& motorInAirstreamFraction) {
  return getImpl<detail::FanVariableVolume_Impl>()->setMotorInAirstreamFraction(motorInAirstreamFraction);
}

void FanVariableVolume::resetMotorInAirstreamFraction() {
  getImpl<detail::FanVariableVolume_Impl>()->resetMotorInAirstreamFraction();
}

void FanVariableVolume::setFanPowerCoefficient1(double fanPowerCoefficient1) {
  getImpl<detail::FanVariableVolume_Impl>()->setFanPowerCoefficient1(fanPowerCoefficient1);
}

bool FanVariableVolume::setFanPowerCoefficient1(const Quantity& fanPowerCoefficient1) {
  return getImpl<detail::FanVariableVolume_Impl>()->setFanPowerCoefficient1(fanPowerCoefficient1);
}

void FanVariableVolume::resetFanPowerCoefficient1() {
  getImpl<detail::FanVariableVolume_Impl>()->resetFanPowerCoefficient1();
}

void FanVariableVolume::setFanPowerCoefficient2(double fanPowerCoefficient2) {
  getImpl<detail::FanVariableVolume_Impl>()->setFanPowerCoefficient2(fanPowerCoefficient2);
}

bool FanVariableVolume::setFanPowerCoefficient2(const Quantity& fanPowerCoefficient2) {
  return getImpl<detail::FanVariableVolume_Impl>()->setFanPowerCoefficient2(fanPowerCoefficient2);
}

void FanVariableVolume::resetFanPowerCoefficient2() {
  getImpl<detail::FanVariableVolume_Impl>()->resetFanPowerCoefficient2();
}

void FanVariableVolume::setFanPowerCoefficient3(double fanPowerCoefficient3) {
  getImpl<detail::FanVariableVolume_Impl>()->setFanPowerCoefficient3(fanPowerCoefficient3);
}

bool FanVariableVolume::setFanPowerCoefficient3(const Quantity& fanPowerCoefficient3) {
  return getImpl<detail::FanVariableVolume_Impl>()->setFanPowerCoefficient3(fanPowerCoefficient3);
}

void FanVariableVolume::resetFanPowerCoefficient3() {
  getImpl<detail::FanVariableVolume_Impl>()->resetFanPowerCoefficient3();
}

void FanVariableVolume::setFanPowerCoefficient4(double fanPowerCoefficient4) {
  getImpl<detail::FanVariableVolume_Impl>()->setFanPowerCoefficient4(fanPowerCoefficient4);
}

bool FanVariableVolume::setFanPowerCoefficient4(const Quantity& fanPowerCoefficient4) {
  return getImpl<detail::FanVariableVolume_Impl>()->setFanPowerCoefficient4(fanPowerCoefficient4);
}

void FanVariableVolume::resetFanPowerCoefficient4() {
  getImpl<detail::FanVariableVolume_Impl>()->resetFanPowerCoefficient4();
}

void FanVariableVolume::setFanPowerCoefficient5(double fanPowerCoefficient5) {
  getImpl<detail::FanVariableVolume_Impl>()->setFanPowerCoefficient5(fanPowerCoefficient5);
}

bool FanVariableVolume::setFanPowerCoefficient5(const Quantity& fanPowerCoefficient5) {
  return getImpl<detail::FanVariableVolume_Impl>()->setFanPowerCoefficient5(fanPowerCoefficient5);
}

void FanVariableVolume::resetFanPowerCoefficient5() {
  getImpl<detail::FanVariableVolume_Impl>()->resetFanPowerCoefficient5();
}

void FanVariableVolume::setEndUseSubcategory(std::string endUseSubcategory) {
  getImpl<detail::FanVariableVolume_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void FanVariableVolume::resetEndUseSubcategory() {
  getImpl<detail::FanVariableVolume_Impl>()->resetEndUseSubcategory();
}

/// @cond
FanVariableVolume::FanVariableVolume(std::shared_ptr<detail::FanVariableVolume_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio
