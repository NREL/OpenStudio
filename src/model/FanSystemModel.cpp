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

#include "Model.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "SetpointManagerMixedAir.hpp"

#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

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

    static std::vector<std::string> results{"Fan Electric Power", "Fan Rise in Air Temperature", "Fan Electric Energy", "Fan Air Mass Flow Rate"};
    return results;
  }

  IddObjectType FanSystemModel_Impl::iddObjectType() const {
    return FanSystemModel::iddObjectType();
  }

  std::vector<ScheduleTypeKey> FanSystemModel_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_Fan_SystemModelFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("FanSystemModel","Availability"));
    }
    return result;
  }


  std::vector<ModelObject> FanSystemModel_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (boost::optional<Curve> intermediate = electricPowerFunctionofFlowFractionCurve()) {
      result.push_back(*intermediate);
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

  boost::optional<Schedule> FanSystemModel_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Fan_SystemModelFields::AvailabilityScheduleName);
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
    // TODO: double check
    return getAutosizedValue("Design Size Maximum Flow Rate", "m3/s");
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

  double FanSystemModel_Impl::electricPowerPerUnitFlowRate() const {
    boost::optional<double> value = getDouble(OS_Fan_SystemModelFields::ElectricPowerPerUnitFlowRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FanSystemModel_Impl::electricPowerPerUnitFlowRatePerUnitPressure() const {
    boost::optional<double> value = getDouble(OS_Fan_SystemModelFields::ElectricPowerPerUnitFlowRatePerUnitPressure,true);
    OS_ASSERT(value);
    return value.get();
  }

  double FanSystemModel_Impl::fanTotalEfficiency() const {
    boost::optional<double> value = getDouble(OS_Fan_SystemModelFields::FanTotalEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Curve> FanSystemModel_Impl::electricPowerFunctionofFlowFractionCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_SystemModelFields::ElectricPowerFunctionofFlowFractionCurveName);
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

  double FanSystemModel_Impl::motorLossRadiativeFraction() const {
    boost::optional<double> value = getDouble(OS_Fan_SystemModelFields::MotorLossRadiativeFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string FanSystemModel_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_Fan_SystemModelFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  int FanSystemModel_Impl::numberofSpeeds() const {
    boost::optional<int> value = getInt(OS_Fan_SystemModelFields::NumberofSpeeds,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool FanSystemModel_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_Fan_SystemModelFields::AvailabilityScheduleName,
                              "FanSystemModel",
                              "Availability",
                              schedule);
    return result;
  }

  unsigned FanSystemModel_Impl::inletPort() const
  {
    return OS_Fan_SystemModelFields::AirInletNodeName;
  }

  unsigned FanSystemModel_Impl::outletPort() const
  {
    return OS_Fan_SystemModelFields::AirOutletNodeName;
  }

  bool FanSystemModel_Impl::addToNode(Node & node)
  {
    auto oaSystem = node.airLoopHVACOutdoorAirSystem();
    auto airLoop = node.airLoopHVAC();

    if( (airLoop && airLoop->supplyComponent(node.handle())) || (oaSystem && oaSystem->component(node.handle())) ) {
      if( StraightComponent_Impl::addToNode(node) ) {
        if ( airLoop ) {
          SetpointManagerMixedAir::updateFanInletOutletNodes(airLoop.get());
        }
        return true;
      }
    }

    return false;
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
    // OS_ASSERT(result); // shouldn't accept < 0
    return result;
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
    // OS_ASSERT(result); // shouldn't accept < 0
    return result;
  }

  bool FanSystemModel_Impl::setElectricPowerPerUnitFlowRatePerUnitPressure(double electricPowerPerUnitFlowRatePerUnitPressure) {
    bool result = setDouble(OS_Fan_SystemModelFields::ElectricPowerPerUnitFlowRatePerUnitPressure, electricPowerPerUnitFlowRatePerUnitPressure);
    // OS_ASSERT(result); // shouldn't accept < 0
    return result;
  }

  bool FanSystemModel_Impl::setFanTotalEfficiency(double fanTotalEfficiency) {
    bool result = setDouble(OS_Fan_SystemModelFields::FanTotalEfficiency, fanTotalEfficiency);
    return result;
  }

  bool FanSystemModel_Impl::setElectricPowerFunctionofFlowFractionCurve(const Curve& curve) {
    bool result = setPointer(OS_Fan_SystemModelFields::ElectricPowerFunctionofFlowFractionCurveName, curve.handle());
    return result;
  }

  void FanSystemModel_Impl::resetElectricPowerFunctionofFlowFractionCurve() {
    bool result = setString(OS_Fan_SystemModelFields::ElectricPowerFunctionofFlowFractionCurveName, "");
  }

  bool FanSystemModel_Impl::setNightVentilationModePressureRise(double nightVentilationModePressureRise) {
    bool result = setDouble(OS_Fan_SystemModelFields::NightVentilationModePressureRise, nightVentilationModePressureRise);
    // OS_ASSERT(result); // shouldn't accept < 0
    return result;
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

  bool FanSystemModel_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
    bool result = setString(OS_Fan_SystemModelFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
    return result;
  }

  bool FanSystemModel_Impl::setNumberofSpeeds(int numberofSpeeds) {
    bool result = setInt(OS_Fan_SystemModelFields::NumberofSpeeds, numberofSpeeds);
    // OS_ASSERT(result); // shouldn't accept < 0
    return result;
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

} // detail

FanSystemModel::FanSystemModel(const Model& model)
  : StraightComponent(FanSystemModel::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::FanSystemModel_Impl>());

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

  auto availabilitySchedule = model.alwaysOnDiscreteSchedule();
  ok = setAvailabilitySchedule(availabilitySchedule);
  OS_ASSERT(ok);

  autosizeDesignMaximumAirFlowRate();

  ok = setDesignPressureRise(500.0); // Completely abritrary
  OS_ASSERT(ok);
  ok = setSpeedControlMethod("Discrete");
  OS_ASSERT(ok);
  ok = setElectricPowerMinimumFlowRateFraction(0.2);
  OS_ASSERT(ok);
  ok = setMotorEfficiency(0.9);
  OS_ASSERT(ok);
  ok = setMotorInAirStreamFraction(1.0);
  OS_ASSERT(ok);

  autosizeDesignElectricPowerConsumption();

  ok = setElectricPowerPerUnitFlowRate(1.66667); // TODO
  OS_ASSERT(ok);
  ok = setElectricPowerPerUnitFlowRatePerUnitPressure(1.66667);
  OS_ASSERT(ok);
  ok = setDesignPowerSizingMethod("PowerPerFlowPerPressure");
  OS_ASSERT(ok);
  ok = setFanTotalEfficiency(0.7);
  OS_ASSERT(ok);
  ok = setMotorLossRadiativeFraction(0.0);
  OS_ASSERT(ok);
  ok = setEndUseSubcategory("General");
  OS_ASSERT(ok);
  ok = setNumberofSpeeds(1);
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

boost::optional<double> FanSystemModel::designMaximumAirFlowRate() const {
  return getImpl<detail::FanSystemModel_Impl>()->designMaximumAirFlowRate();
}

bool FanSystemModel::isDesignMaximumAirFlowRateAutosized() const {
  return getImpl<detail::FanSystemModel_Impl>()->isDesignMaximumAirFlowRateAutosized();
}

  boost::optional <double> FanSystemModel::autosizedDesignMaximumAirFlowRate() {
    return getImpl<detail::FanSystemModel_Impl>()->autosizedDesignMaximumAirFlowRate();
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
    return getImpl<detail::FanSystemModel_Impl>()->autosizedDesignElectricPowerConsumption();
  }

std::string FanSystemModel::designPowerSizingMethod() const {
  return getImpl<detail::FanSystemModel_Impl>()->designPowerSizingMethod();
}

double FanSystemModel::electricPowerPerUnitFlowRate() const {
  return getImpl<detail::FanSystemModel_Impl>()->electricPowerPerUnitFlowRate();
}

double FanSystemModel::electricPowerPerUnitFlowRatePerUnitPressure() const {
  return getImpl<detail::FanSystemModel_Impl>()->electricPowerPerUnitFlowRatePerUnitPressure();
}

double FanSystemModel::fanTotalEfficiency() const {
  return getImpl<detail::FanSystemModel_Impl>()->fanTotalEfficiency();
}

boost::optional<Curve> FanSystemModel::electricPowerFunctionofFlowFractionCurve() const {
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

double FanSystemModel::motorLossRadiativeFraction() const {
  return getImpl<detail::FanSystemModel_Impl>()->motorLossRadiativeFraction();
}

std::string FanSystemModel::endUseSubcategory() const {
  return getImpl<detail::FanSystemModel_Impl>()->endUseSubcategory();
}

int FanSystemModel::numberofSpeeds() const {
  return getImpl<detail::FanSystemModel_Impl>()->numberofSpeeds();
}

bool FanSystemModel::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::FanSystemModel_Impl>()->setAvailabilitySchedule(schedule);
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

bool FanSystemModel::setDesignElectricPowerConsumption(double designElectricPowerConsumption) {
  return getImpl<detail::FanSystemModel_Impl>()->setDesignElectricPowerConsumption(designElectricPowerConsumption);
}

void FanSystemModel::autosizeDesignElectricPowerConsumption() {
  getImpl<detail::FanSystemModel_Impl>()->autosizeDesignElectricPowerConsumption();
}

bool FanSystemModel::setDesignPowerSizingMethod(const std::string& designPowerSizingMethod) {
  return getImpl<detail::FanSystemModel_Impl>()->setDesignPowerSizingMethod(designPowerSizingMethod);
}

bool FanSystemModel::setElectricPowerPerUnitFlowRate(double electricPowerPerUnitFlowRate) {
  return getImpl<detail::FanSystemModel_Impl>()->setElectricPowerPerUnitFlowRate(electricPowerPerUnitFlowRate);
}

bool FanSystemModel::setElectricPowerPerUnitFlowRatePerUnitPressure(double electricPowerPerUnitFlowRatePerUnitPressure) {
  return getImpl<detail::FanSystemModel_Impl>()->setElectricPowerPerUnitFlowRatePerUnitPressure(electricPowerPerUnitFlowRatePerUnitPressure);
}

bool FanSystemModel::setFanTotalEfficiency(double fanTotalEfficiency) {
  return getImpl<detail::FanSystemModel_Impl>()->setFanTotalEfficiency(fanTotalEfficiency);
}

bool FanSystemModel::setElectricPowerFunctionofFlowFractionCurve(const Curve& curve) {
  return getImpl<detail::FanSystemModel_Impl>()->setElectricPowerFunctionofFlowFractionCurve(curve);
}

void FanSystemModel::resetElectricPowerFunctionofFlowFractionCurve() {
  getImpl<detail::FanSystemModel_Impl>()->resetElectricPowerFunctionofFlowFractionCurve();
}

bool FanSystemModel::setNightVentilationModePressureRise(double nightVentilationModePressureRise) {
  return getImpl<detail::FanSystemModel_Impl>()->setNightVentilationModePressureRise(nightVentilationModePressureRise);
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

bool FanSystemModel::setEndUseSubcategory(const std::string& endUseSubcategory) {
  return getImpl<detail::FanSystemModel_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

bool FanSystemModel::setNumberofSpeeds(int numberofSpeeds) {
  return getImpl<detail::FanSystemModel_Impl>()->setNumberofSpeeds(numberofSpeeds);
}

/// @cond
FanSystemModel::FanSystemModel(std::shared_ptr<detail::FanSystemModel_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

