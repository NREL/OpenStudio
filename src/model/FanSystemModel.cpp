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

#include "FanSystemModel.hpp"
#include "FanSystemModel_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "UnivariateFunctions.hpp"
#include "UnivariateFunctions_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Fan_SystemModel_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  FanSystemModel_Impl::FanSystemModel_Impl(const IdfObject& idfObject,
                                           Model_Impl* model,
                                           bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == FanSystemModel::iddObjectType());
  }

  FanSystemModel_Impl::FanSystemModel_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == FanSystemModel::iddObjectType());
  }

  FanSystemModel_Impl::FanSystemModel_Impl(const FanSystemModel_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& FanSystemModel_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType FanSystemModel_Impl::iddObjectType() const {
    return FanSystemModel::iddObjectType();
  }

  std::vector<ScheduleTypeKey> FanSystemModel_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Fan_SystemModelFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("FanSystemModel","Availability"));
    }
    return result;
  }

  Schedule FanSystemModel_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
    }
    return value.get();
  }

  boost::optional<Connection> FanSystemModel_Impl::airInletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Fan_SystemModelFields::AirInletNodeName);
  }

  boost::optional<Connection> FanSystemModel_Impl::airOutletNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Fan_SystemModelFields::AirOutletNodeName);
  }

  boost::optional<double> FanSystemModel_Impl::designMaximumAirFlowRate() const {
    return getDouble(OS_Fan_SystemModelFields::DesignMaximumAirFlowRate,true);
  }

  bool FanSystemModel_Impl::isDesignMaximumAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Fan_SystemModelFields::DesignMaximumAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional <double> FanSystemModel_Impl::autosizedDesignMaximumAirFlowRate() {
    return getAutosizedValue("TODO_CHECK_SQL Design Maximum Air Flow Rate", "m3/s");
  }

  std::string FanSystemModel_Impl::speedControlMethod() const {
    boost::optional<std::string> value = getString(OS_Fan_SystemModelFields::SpeedControlMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FanSystemModel_Impl::electricPowerMinimumFlowRateFraction() const {
    boost::optional<double> value = getDouble(OS_Fan_SystemModelFields::ElectricPowerMinimumFlowRateFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FanSystemModel_Impl::designPressureRise() const {
    boost::optional<double> value = getDouble(OS_Fan_SystemModelFields::DesignPressureRise,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FanSystemModel_Impl::motorEfficiency() const {
    boost::optional<double> value = getDouble(OS_Fan_SystemModelFields::MotorEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FanSystemModel_Impl::motorInAirStreamFraction() const {
    boost::optional<double> value = getDouble(OS_Fan_SystemModelFields::MotorInAirStreamFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> FanSystemModel_Impl::designElectricPowerConsumption() const {
    return getDouble(OS_Fan_SystemModelFields::DesignElectricPowerConsumption,true);
  }

  bool FanSystemModel_Impl::isDesignElectricPowerConsumptionAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_Fan_SystemModelFields::DesignElectricPowerConsumption, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional <double> FanSystemModel_Impl::autosizedDesignElectricPowerConsumption() {
    return getAutosizedValue("TODO_CHECK_SQL Design Electric Power Consumption", "W");
  }

  std::string FanSystemModel_Impl::designPowerSizingMethod() const {
    boost::optional<std::string> value = getString(OS_Fan_SystemModelFields::DesignPowerSizingMethod,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> FanSystemModel_Impl::electricPowerPerUnitFlowRate() const {
    return getDouble(OS_Fan_SystemModelFields::ElectricPowerPerUnitFlowRate,true);
  }

  boost::optional<double> FanSystemModel_Impl::electricPowerPerUnitFlowRatePerUnitPressure() const {
    return getDouble(OS_Fan_SystemModelFields::ElectricPowerPerUnitFlowRatePerUnitPressure,true);
  }

  double FanSystemModel_Impl::fanTotalEfficiency() const {
    boost::optional<double> value = getDouble(OS_Fan_SystemModelFields::FanTotalEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<UnivariateFunctions> FanSystemModel_Impl::electricPowerFunctionofFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(OS_Fan_SystemModelFields::ElectricPowerFunctionofFlowFractionCurveName);
  }

  boost::optional<double> FanSystemModel_Impl::nightVentilationModePressureRise() const {
    return getDouble(OS_Fan_SystemModelFields::NightVentilationModePressureRise,true);
  }

  boost::optional<double> FanSystemModel_Impl::nightVentilationModeFlowFraction() const {
    return getDouble(OS_Fan_SystemModelFields::NightVentilationModeFlowFraction,true);
  }

  boost::optional<ThermalZone> FanSystemModel_Impl::motorLossZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Fan_SystemModelFields::MotorLossZoneName);
  }

  boost::optional<double> FanSystemModel_Impl::motorLossRadiativeFraction() const {
    return getDouble(OS_Fan_SystemModelFields::MotorLossRadiativeFraction,true);
  }

  std::string FanSystemModel_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_Fan_SystemModelFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FanSystemModel_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_Fan_SystemModelFields::EndUseSubcategory);
  }

  boost::optional<int> FanSystemModel_Impl::numberofSpeeds() const {
    return getInt(OS_Fan_SystemModelFields::NumberofSpeeds,true);
  }

  bool FanSystemModel_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Fan_SystemModelFields::AvailabilityScheduleName,
                              "FanSystemModel",
                              "Availability",
                              schedule);
    return result;
  }

  bool FanSystemModel_Impl::setAirInletNode(const Connection& connection) {
    bool result = setPointer(OS_Fan_SystemModelFields::AirInletNodeName, connection.handle());
    return result;
  }

  void FanSystemModel_Impl::resetAirInletNode() {
    bool result = setString(OS_Fan_SystemModelFields::AirInletNodeName, "");
    OS_ASSERT(result);
  }

  bool FanSystemModel_Impl::setAirOutletNode(const Connection& connection) {
    bool result = setPointer(OS_Fan_SystemModelFields::AirOutletNodeName, connection.handle());
    return result;
  }

  void FanSystemModel_Impl::resetAirOutletNode() {
    bool result = setString(OS_Fan_SystemModelFields::AirOutletNodeName, "");
    OS_ASSERT(result);
  }

  bool FanSystemModel_Impl::setDesignMaximumAirFlowRate(double designMaximumAirFlowRate) {
    bool result = setDouble(OS_Fan_SystemModelFields::DesignMaximumAirFlowRate, designMaximumAirFlowRate);
    return result;
  }

  void FanSystemModel_Impl::autosizeDesignMaximumAirFlowRate() {
    bool result = setString(OS_Fan_SystemModelFields::DesignMaximumAirFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool FanSystemModel_Impl::setSpeedControlMethod(const std::string& speedControlMethod) {
    bool result = setString(OS_Fan_SystemModelFields::SpeedControlMethod, speedControlMethod);
    return result;
  }

  bool FanSystemModel_Impl::setElectricPowerMinimumFlowRateFraction(double electricPowerMinimumFlowRateFraction) {
    bool result = setDouble(OS_Fan_SystemModelFields::ElectricPowerMinimumFlowRateFraction, electricPowerMinimumFlowRateFraction);
    return result;
  }

  bool FanSystemModel_Impl::setDesignPressureRise(double designPressureRise) {
    bool result = setDouble(OS_Fan_SystemModelFields::DesignPressureRise, designPressureRise);
    return result;
  }

  bool FanSystemModel_Impl::setMotorEfficiency(double motorEfficiency) {
    bool result = setDouble(OS_Fan_SystemModelFields::MotorEfficiency, motorEfficiency);
    return result;
  }

  bool FanSystemModel_Impl::setMotorInAirStreamFraction(double motorInAirStreamFraction) {
    bool result = setDouble(OS_Fan_SystemModelFields::MotorInAirStreamFraction, motorInAirStreamFraction);
    return result;
  }

  bool FanSystemModel_Impl::setDesignElectricPowerConsumption(double designElectricPowerConsumption) {
    bool result = setDouble(OS_Fan_SystemModelFields::DesignElectricPowerConsumption, designElectricPowerConsumption);
    OS_ASSERT(result);
  }

  void FanSystemModel_Impl::autosizeDesignElectricPowerConsumption() {
    bool result = setString(OS_Fan_SystemModelFields::DesignElectricPowerConsumption, "autosize");
    OS_ASSERT(result);
  }

  bool FanSystemModel_Impl::setDesignPowerSizingMethod(const std::string& designPowerSizingMethod) {
    bool result = setString(OS_Fan_SystemModelFields::DesignPowerSizingMethod, designPowerSizingMethod);
    return result;
  }

  bool FanSystemModel_Impl::setElectricPowerPerUnitFlowRate(double electricPowerPerUnitFlowRate) {
    bool result = setDouble(OS_Fan_SystemModelFields::ElectricPowerPerUnitFlowRate, electricPowerPerUnitFlowRate);
    OS_ASSERT(result);
  }

  void FanSystemModel_Impl::resetElectricPowerPerUnitFlowRate() {
    bool result = setString(OS_Fan_SystemModelFields::ElectricPowerPerUnitFlowRate, "");
    OS_ASSERT(result);
  }

  bool FanSystemModel_Impl::setElectricPowerPerUnitFlowRatePerUnitPressure(double electricPowerPerUnitFlowRatePerUnitPressure) {
    bool result = setDouble(OS_Fan_SystemModelFields::ElectricPowerPerUnitFlowRatePerUnitPressure, electricPowerPerUnitFlowRatePerUnitPressure);
    OS_ASSERT(result);
  }

  void FanSystemModel_Impl::resetElectricPowerPerUnitFlowRatePerUnitPressure() {
    bool result = setString(OS_Fan_SystemModelFields::ElectricPowerPerUnitFlowRatePerUnitPressure, "");
    OS_ASSERT(result);
  }

  bool FanSystemModel_Impl::setFanTotalEfficiency(double fanTotalEfficiency) {
    bool result = setDouble(OS_Fan_SystemModelFields::FanTotalEfficiency, fanTotalEfficiency);
    return result;
  }

  bool FanSystemModel_Impl::setElectricPowerFunctionofFlowFractionCurve(const UnivariateFunctions& univariateFunctions) {
    bool result = setPointer(OS_Fan_SystemModelFields::ElectricPowerFunctionofFlowFractionCurveName, univariateFunctions.handle());
    return result;
  }

  void FanSystemModel_Impl::resetElectricPowerFunctionofFlowFractionCurve() {
    bool result = setString(OS_Fan_SystemModelFields::ElectricPowerFunctionofFlowFractionCurveName, "");
    OS_ASSERT(result);
  }

  bool FanSystemModel_Impl::setNightVentilationModePressureRise(double nightVentilationModePressureRise) {
    bool result = setDouble(OS_Fan_SystemModelFields::NightVentilationModePressureRise, nightVentilationModePressureRise);
    OS_ASSERT(result);
  }

  void FanSystemModel_Impl::resetNightVentilationModePressureRise() {
    bool result = setString(OS_Fan_SystemModelFields::NightVentilationModePressureRise, "");
    OS_ASSERT(result);
  }

  bool FanSystemModel_Impl::setNightVentilationModeFlowFraction(double nightVentilationModeFlowFraction) {
    bool result = setDouble(OS_Fan_SystemModelFields::NightVentilationModeFlowFraction, nightVentilationModeFlowFraction);
    return result;
  }

  void FanSystemModel_Impl::resetNightVentilationModeFlowFraction() {
    bool result = setString(OS_Fan_SystemModelFields::NightVentilationModeFlowFraction, "");
    OS_ASSERT(result);
  }

  bool FanSystemModel_Impl::setMotorLossZone(const ThermalZone& thermalZone) {
    bool result = setPointer(OS_Fan_SystemModelFields::MotorLossZoneName, thermalZone.handle());
    return result;
  }

  void FanSystemModel_Impl::resetMotorLossZone() {
    bool result = setString(OS_Fan_SystemModelFields::MotorLossZoneName, "");
    OS_ASSERT(result);
  }

  bool FanSystemModel_Impl::setMotorLossRadiativeFraction(double motorLossRadiativeFraction) {
    bool result = setDouble(OS_Fan_SystemModelFields::MotorLossRadiativeFraction, motorLossRadiativeFraction);
    return result;
  }

  void FanSystemModel_Impl::resetMotorLossRadiativeFraction() {
    bool result = setString(OS_Fan_SystemModelFields::MotorLossRadiativeFraction, "");
    OS_ASSERT(result);
  }

  bool FanSystemModel_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
    bool result = setString(OS_Fan_SystemModelFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
  }

  void FanSystemModel_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_Fan_SystemModelFields::EndUseSubcategory, "");
    OS_ASSERT(result);
  }

  bool FanSystemModel_Impl::setNumberofSpeeds(int numberofSpeeds) {
    bool result = setInt(OS_Fan_SystemModelFields::NumberofSpeeds, numberofSpeeds);
    OS_ASSERT(result);
  }

  void FanSystemModel_Impl::resetNumberofSpeeds() {
    bool result = setString(OS_Fan_SystemModelFields::NumberofSpeeds, "");
    OS_ASSERT(result);
  }

  void FanSystemModel_Impl::autosize() {
    autosizedDesignMaximumAirFlowRate();
    autosizedDesignElectricPowerConsumption();
  }

  void FanSystemModel_Impl::applySizingValues() {
    boost::optional<double> val;
    val = autosizedDesignMaximumAirFlowRate();
    if (val) {
      setDesignMaximumAirFlowRate(val.get());
    }

    val = autosizedDesignElectricPowerConsumption();
    if (val) {
      setDesignElectricPowerConsumption(val.get());
    }

  }

  boost::optional<Schedule> FanSystemModel_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Fan_SystemModelFields::AvailabilityScheduleName);
  }

} // detail

FanSystemModel::FanSystemModel(const Model& model)
  : StraightComponent(FanSystemModel::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::FanSystemModel_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_Fan_SystemModelFields::AvailabilityScheduleName
  bool ok = true;
  // ok = setAvailabilitySchedule();
  OS_ASSERT(ok);
  // ok = setDesignMaximumAirFlowRate();
  OS_ASSERT(ok);
  // ok = setSpeedControlMethod();
  OS_ASSERT(ok);
  // ok = setElectricPowerMinimumFlowRateFraction();
  OS_ASSERT(ok);
  // ok = setDesignPressureRise();
  OS_ASSERT(ok);
  // ok = setMotorEfficiency();
  OS_ASSERT(ok);
  // ok = setMotorInAirStreamFraction();
  OS_ASSERT(ok);
  // setDesignElectricPowerConsumption();
  // ok = setDesignPowerSizingMethod();
  OS_ASSERT(ok);
  // ok = setFanTotalEfficiency();
  OS_ASSERT(ok);
}

IddObjectType FanSystemModel::iddObjectType() {
  return IddObjectType(IddObjectType::OS_Fan_SystemModel);
}

std::vector<std::string> FanSystemModel::speedControlMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Fan_SystemModelFields::SpeedControlMethod);
}

std::vector<std::string> FanSystemModel::designPowerSizingMethodValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_Fan_SystemModelFields::DesignPowerSizingMethod);
}

Schedule FanSystemModel::availabilitySchedule() const {
  return getImpl<detail::FanSystemModel_Impl>()->availabilitySchedule();
}

boost::optional<Connection> FanSystemModel::airInletNode() const {
  return getImpl<detail::FanSystemModel_Impl>()->airInletNode();
}

boost::optional<Connection> FanSystemModel::airOutletNode() const {
  return getImpl<detail::FanSystemModel_Impl>()->airOutletNode();
}

boost::optional<double> FanSystemModel::designMaximumAirFlowRate() const {
  return getImpl<detail::FanSystemModel_Impl>()->designMaximumAirFlowRate();
}

bool FanSystemModel::isDesignMaximumAirFlowRateAutosized() const {
  return getImpl<detail::FanSystemModel_Impl>()->isDesignMaximumAirFlowRateAutosized();
}

  boost::optional <double> FanSystemModel::autosizedDesignMaximumAirFlowRate() {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizedDesignMaximumAirFlowRate();
  }

std::string FanSystemModel::speedControlMethod() const {
  return getImpl<detail::FanSystemModel_Impl>()->speedControlMethod();
}

double FanSystemModel::electricPowerMinimumFlowRateFraction() const {
  return getImpl<detail::FanSystemModel_Impl>()->electricPowerMinimumFlowRateFraction();
}

double FanSystemModel::designPressureRise() const {
  return getImpl<detail::FanSystemModel_Impl>()->designPressureRise();
}

double FanSystemModel::motorEfficiency() const {
  return getImpl<detail::FanSystemModel_Impl>()->motorEfficiency();
}

double FanSystemModel::motorInAirStreamFraction() const {
  return getImpl<detail::FanSystemModel_Impl>()->motorInAirStreamFraction();
}

boost::optional<double> FanSystemModel::designElectricPowerConsumption() const {
  return getImpl<detail::FanSystemModel_Impl>()->designElectricPowerConsumption();
}

bool FanSystemModel::isDesignElectricPowerConsumptionAutosized() const {
  return getImpl<detail::FanSystemModel_Impl>()->isDesignElectricPowerConsumptionAutosized();
}

  boost::optional <double> FanSystemModel::autosizedDesignElectricPowerConsumption() {
    return getImpl<detail::CoilCoolingDXSingleSpeed_Impl>()->autosizedDesignElectricPowerConsumption();
  }

std::string FanSystemModel::designPowerSizingMethod() const {
  return getImpl<detail::FanSystemModel_Impl>()->designPowerSizingMethod();
}

boost::optional<double> FanSystemModel::electricPowerPerUnitFlowRate() const {
  return getImpl<detail::FanSystemModel_Impl>()->electricPowerPerUnitFlowRate();
}

boost::optional<double> FanSystemModel::electricPowerPerUnitFlowRatePerUnitPressure() const {
  return getImpl<detail::FanSystemModel_Impl>()->electricPowerPerUnitFlowRatePerUnitPressure();
}

double FanSystemModel::fanTotalEfficiency() const {
  return getImpl<detail::FanSystemModel_Impl>()->fanTotalEfficiency();
}

boost::optional<UnivariateFunctions> FanSystemModel::electricPowerFunctionofFlowFractionCurve() const {
  return getImpl<detail::FanSystemModel_Impl>()->electricPowerFunctionofFlowFractionCurve();
}

boost::optional<double> FanSystemModel::nightVentilationModePressureRise() const {
  return getImpl<detail::FanSystemModel_Impl>()->nightVentilationModePressureRise();
}

boost::optional<double> FanSystemModel::nightVentilationModeFlowFraction() const {
  return getImpl<detail::FanSystemModel_Impl>()->nightVentilationModeFlowFraction();
}

boost::optional<ThermalZone> FanSystemModel::motorLossZone() const {
  return getImpl<detail::FanSystemModel_Impl>()->motorLossZone();
}

boost::optional<double> FanSystemModel::motorLossRadiativeFraction() const {
  return getImpl<detail::FanSystemModel_Impl>()->motorLossRadiativeFraction();
}

std::string FanSystemModel::endUseSubcategory() const {
  return getImpl<detail::FanSystemModel_Impl>()->endUseSubcategory();
}

bool FanSystemModel::isEndUseSubcategoryDefaulted() const {
  return getImpl<detail::FanSystemModel_Impl>()->isEndUseSubcategoryDefaulted();
}

boost::optional<int> FanSystemModel::numberofSpeeds() const {
  return getImpl<detail::FanSystemModel_Impl>()->numberofSpeeds();
}

bool FanSystemModel::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::FanSystemModel_Impl>()->setAvailabilitySchedule(schedule);
}

bool FanSystemModel::setAirInletNode(const Connection& connection) {
  return getImpl<detail::FanSystemModel_Impl>()->setAirInletNode(connection);
}

void FanSystemModel::resetAirInletNode() {
  getImpl<detail::FanSystemModel_Impl>()->resetAirInletNode();
}

bool FanSystemModel::setAirOutletNode(const Connection& connection) {
  return getImpl<detail::FanSystemModel_Impl>()->setAirOutletNode(connection);
}

void FanSystemModel::resetAirOutletNode() {
  getImpl<detail::FanSystemModel_Impl>()->resetAirOutletNode();
}

bool FanSystemModel::setDesignMaximumAirFlowRate(double designMaximumAirFlowRate) {
  return getImpl<detail::FanSystemModel_Impl>()->setDesignMaximumAirFlowRate(designMaximumAirFlowRate);
}

void FanSystemModel::autosizeDesignMaximumAirFlowRate() {
  getImpl<detail::FanSystemModel_Impl>()->autosizeDesignMaximumAirFlowRate();
}

bool FanSystemModel::setSpeedControlMethod(const std::string& speedControlMethod) {
  return getImpl<detail::FanSystemModel_Impl>()->setSpeedControlMethod(speedControlMethod);
}

bool FanSystemModel::setElectricPowerMinimumFlowRateFraction(double electricPowerMinimumFlowRateFraction) {
  return getImpl<detail::FanSystemModel_Impl>()->setElectricPowerMinimumFlowRateFraction(electricPowerMinimumFlowRateFraction);
}

bool FanSystemModel::setDesignPressureRise(double designPressureRise) {
  return getImpl<detail::FanSystemModel_Impl>()->setDesignPressureRise(designPressureRise);
}

bool FanSystemModel::setMotorEfficiency(double motorEfficiency) {
  return getImpl<detail::FanSystemModel_Impl>()->setMotorEfficiency(motorEfficiency);
}

bool FanSystemModel::setMotorInAirStreamFraction(double motorInAirStreamFraction) {
  return getImpl<detail::FanSystemModel_Impl>()->setMotorInAirStreamFraction(motorInAirStreamFraction);
}

void FanSystemModel::setDesignElectricPowerConsumption(double designElectricPowerConsumption) {
  getImpl<detail::FanSystemModel_Impl>()->setDesignElectricPowerConsumption(designElectricPowerConsumption);
}

void FanSystemModel::autosizeDesignElectricPowerConsumption() {
  getImpl<detail::FanSystemModel_Impl>()->autosizeDesignElectricPowerConsumption();
}

bool FanSystemModel::setDesignPowerSizingMethod(const std::string& designPowerSizingMethod) {
  return getImpl<detail::FanSystemModel_Impl>()->setDesignPowerSizingMethod(designPowerSizingMethod);
}

void FanSystemModel::setElectricPowerPerUnitFlowRate(double electricPowerPerUnitFlowRate) {
  getImpl<detail::FanSystemModel_Impl>()->setElectricPowerPerUnitFlowRate(electricPowerPerUnitFlowRate);
}

void FanSystemModel::resetElectricPowerPerUnitFlowRate() {
  getImpl<detail::FanSystemModel_Impl>()->resetElectricPowerPerUnitFlowRate();
}

void FanSystemModel::setElectricPowerPerUnitFlowRatePerUnitPressure(double electricPowerPerUnitFlowRatePerUnitPressure) {
  getImpl<detail::FanSystemModel_Impl>()->setElectricPowerPerUnitFlowRatePerUnitPressure(electricPowerPerUnitFlowRatePerUnitPressure);
}

void FanSystemModel::resetElectricPowerPerUnitFlowRatePerUnitPressure() {
  getImpl<detail::FanSystemModel_Impl>()->resetElectricPowerPerUnitFlowRatePerUnitPressure();
}

bool FanSystemModel::setFanTotalEfficiency(double fanTotalEfficiency) {
  return getImpl<detail::FanSystemModel_Impl>()->setFanTotalEfficiency(fanTotalEfficiency);
}

bool FanSystemModel::setElectricPowerFunctionofFlowFractionCurve(const UnivariateFunctions& univariateFunctions) {
  return getImpl<detail::FanSystemModel_Impl>()->setElectricPowerFunctionofFlowFractionCurve(univariateFunctions);
}

void FanSystemModel::resetElectricPowerFunctionofFlowFractionCurve() {
  getImpl<detail::FanSystemModel_Impl>()->resetElectricPowerFunctionofFlowFractionCurve();
}

void FanSystemModel::setNightVentilationModePressureRise(double nightVentilationModePressureRise) {
  getImpl<detail::FanSystemModel_Impl>()->setNightVentilationModePressureRise(nightVentilationModePressureRise);
}

void FanSystemModel::resetNightVentilationModePressureRise() {
  getImpl<detail::FanSystemModel_Impl>()->resetNightVentilationModePressureRise();
}

bool FanSystemModel::setNightVentilationModeFlowFraction(double nightVentilationModeFlowFraction) {
  return getImpl<detail::FanSystemModel_Impl>()->setNightVentilationModeFlowFraction(nightVentilationModeFlowFraction);
}

void FanSystemModel::resetNightVentilationModeFlowFraction() {
  getImpl<detail::FanSystemModel_Impl>()->resetNightVentilationModeFlowFraction();
}

bool FanSystemModel::setMotorLossZone(const ThermalZone& thermalZone) {
  return getImpl<detail::FanSystemModel_Impl>()->setMotorLossZone(thermalZone);
}

void FanSystemModel::resetMotorLossZone() {
  getImpl<detail::FanSystemModel_Impl>()->resetMotorLossZone();
}

bool FanSystemModel::setMotorLossRadiativeFraction(double motorLossRadiativeFraction) {
  return getImpl<detail::FanSystemModel_Impl>()->setMotorLossRadiativeFraction(motorLossRadiativeFraction);
}

void FanSystemModel::resetMotorLossRadiativeFraction() {
  getImpl<detail::FanSystemModel_Impl>()->resetMotorLossRadiativeFraction();
}

void FanSystemModel::setEndUseSubcategory(const std::string& endUseSubcategory) {
  getImpl<detail::FanSystemModel_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void FanSystemModel::resetEndUseSubcategory() {
  getImpl<detail::FanSystemModel_Impl>()->resetEndUseSubcategory();
}

void FanSystemModel::setNumberofSpeeds(int numberofSpeeds) {
  getImpl<detail::FanSystemModel_Impl>()->setNumberofSpeeds(numberofSpeeds);
}

void FanSystemModel::resetNumberofSpeeds() {
  getImpl<detail::FanSystemModel_Impl>()->resetNumberofSpeeds();
}

/// @cond
FanSystemModel::FanSystemModel(std::shared_ptr<detail::FanSystemModel_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

