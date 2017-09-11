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

#include "WaterHeaterStratified.hpp"
#include "WaterHeaterStratified_Impl.hpp"
#include "WaterHeaterHeatPump.hpp"
#include "WaterHeaterHeatPump_Impl.hpp"
#include "WaterHeaterHeatPumpWrappedCondenser.hpp"
#include "WaterHeaterHeatPumpWrappedCondenser_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "ScheduleDay.hpp"
#include "ScheduleDay_Impl.hpp"
#include "ScheduleRuleset.hpp"
#include "ScheduleRuleset_Impl.hpp"
#include "Model.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_WaterHeater_Stratified_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  WaterHeaterStratified_Impl::WaterHeaterStratified_Impl(const IdfObject& idfObject,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : WaterToWaterComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == WaterHeaterStratified::iddObjectType());
  }

  WaterHeaterStratified_Impl::WaterHeaterStratified_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == WaterHeaterStratified::iddObjectType());
  }

  WaterHeaterStratified_Impl::WaterHeaterStratified_Impl(const WaterHeaterStratified_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : WaterToWaterComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& WaterHeaterStratified_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType WaterHeaterStratified_Impl::iddObjectType() const {
    return WaterHeaterStratified::iddObjectType();
  }

  std::vector<ScheduleTypeKey> WaterHeaterStratified_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_WaterHeater_StratifiedFields::Heater1SetpointTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("WaterHeaterStratified","Heater 1 Setpoint Temperature"));
    }
    if (std::find(b,e,OS_WaterHeater_StratifiedFields::Heater2SetpointTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("WaterHeaterStratified","Heater 2 Setpoint Temperature"));
    }
    if (std::find(b,e,OS_WaterHeater_StratifiedFields::AmbientTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("WaterHeaterStratified","Ambient Temperature"));
    }
    if (std::find(b,e,OS_WaterHeater_StratifiedFields::UseFlowRateFractionScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("WaterHeaterStratified","Use Flow Rate Fraction"));
    }
    if (std::find(b,e,OS_WaterHeater_StratifiedFields::ColdWaterSupplyTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("WaterHeaterStratified","Cold Water Supply Temperature"));
    }
    if (std::find(b,e,OS_WaterHeater_StratifiedFields::IndirectAlternateSetpointTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("WaterHeaterStratified","Indirect Alternate Setpoint Temperature"));
    }
    return result;
  }

  unsigned WaterHeaterStratified_Impl::supplyInletPort()
  {
    return OS_WaterHeater_StratifiedFields::UseSideInletNodeName;
  }

  unsigned WaterHeaterStratified_Impl::supplyOutletPort()
  {
    return OS_WaterHeater_StratifiedFields::UseSideOutletNodeName;
  }

  unsigned WaterHeaterStratified_Impl::demandInletPort()
  {
    return OS_WaterHeater_StratifiedFields::SourceSideInletNodeName;
  }

  unsigned WaterHeaterStratified_Impl::demandOutletPort()
  {
    return OS_WaterHeater_StratifiedFields::SourceSideOutletNodeName;
  }

  std::string WaterHeaterStratified_Impl::endUseSubcategory() const {
    boost::optional<std::string> value = getString(OS_WaterHeater_StratifiedFields::EndUseSubcategory,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> WaterHeaterStratified_Impl::tankVolume() const {
    return getDouble(OS_WaterHeater_StratifiedFields::TankVolume,true);
  }

  bool WaterHeaterStratified_Impl::isTankVolumeAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_WaterHeater_StratifiedFields::TankVolume, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> WaterHeaterStratified_Impl::tankHeight() const {
    return getDouble(OS_WaterHeater_StratifiedFields::TankHeight,true);
  }

  bool WaterHeaterStratified_Impl::isTankHeightAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_WaterHeater_StratifiedFields::TankHeight, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  std::string WaterHeaterStratified_Impl::tankShape() const {
    boost::optional<std::string> value = getString(OS_WaterHeater_StratifiedFields::TankShape,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> WaterHeaterStratified_Impl::tankPerimeter() const {
    return getDouble(OS_WaterHeater_StratifiedFields::TankPerimeter,true);
  }

  double WaterHeaterStratified_Impl::maximumTemperatureLimit() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::MaximumTemperatureLimit,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string WaterHeaterStratified_Impl::heaterPriorityControl() const {
    boost::optional<std::string> value = getString(OS_WaterHeater_StratifiedFields::HeaterPriorityControl,true);
    OS_ASSERT(value);
    return value.get();
  }

  Schedule WaterHeaterStratified_Impl::heater1SetpointTemperatureSchedule() const {
    boost::optional<Schedule> value = optionalHeater1SetpointTemperatureSchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heater1Setpoint Temperature Schedule attached.");
    }
    return value.get();
  }

  double WaterHeaterStratified_Impl::heater1DeadbandTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Heater1DeadbandTemperatureDifference,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> WaterHeaterStratified_Impl::heater1Capacity() const {
    return getDouble(OS_WaterHeater_StratifiedFields::Heater1Capacity,true);
  }

  bool WaterHeaterStratified_Impl::isHeater1CapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_WaterHeater_StratifiedFields::Heater1Capacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double WaterHeaterStratified_Impl::heater1Height() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Heater1Height,true);
    OS_ASSERT(value);
    return value.get();
  }

  Schedule WaterHeaterStratified_Impl::heater2SetpointTemperatureSchedule() const {
    boost::optional<Schedule> value = optionalHeater2SetpointTemperatureSchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heater2Setpoint Temperature Schedule attached.");
    }
    return value.get();
  }

  double WaterHeaterStratified_Impl::heater2DeadbandTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Heater2DeadbandTemperatureDifference,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::heater2Capacity() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Heater2Capacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::heater2Height() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Heater2Height,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string WaterHeaterStratified_Impl::heaterFuelType() const {
    boost::optional<std::string> value = getString(OS_WaterHeater_StratifiedFields::HeaterFuelType,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::heaterThermalEfficiency() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::HeaterThermalEfficiency,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::offCycleParasiticFuelConsumptionRate() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::OffCycleParasiticFuelConsumptionRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string WaterHeaterStratified_Impl::offCycleParasiticFuelType() const {
    boost::optional<std::string> value = getString(OS_WaterHeater_StratifiedFields::OffCycleParasiticFuelType,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::offCycleParasiticHeatFractiontoTank() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::OffCycleParasiticHeatFractiontoTank,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::offCycleParasiticHeight() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::OffCycleParasiticHeight,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::onCycleParasiticFuelConsumptionRate() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::OnCycleParasiticFuelConsumptionRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string WaterHeaterStratified_Impl::onCycleParasiticFuelType() const {
    boost::optional<std::string> value = getString(OS_WaterHeater_StratifiedFields::OnCycleParasiticFuelType,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::onCycleParasiticHeatFractiontoTank() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::OnCycleParasiticHeatFractiontoTank,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::onCycleParasiticHeight() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::OnCycleParasiticHeight,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string WaterHeaterStratified_Impl::ambientTemperatureIndicator() const {
    boost::optional<std::string> value = getString(OS_WaterHeater_StratifiedFields::AmbientTemperatureIndicator,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> WaterHeaterStratified_Impl::ambientTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_StratifiedFields::AmbientTemperatureScheduleName);
  }

  boost::optional<ThermalZone> WaterHeaterStratified_Impl::ambientTemperatureThermalZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_WaterHeater_StratifiedFields::AmbientTemperatureThermalZoneName);
  }

  boost::optional<std::string> WaterHeaterStratified_Impl::ambientTemperatureOutdoorAirNodeName() const {
    return getString(OS_WaterHeater_StratifiedFields::AmbientTemperatureOutdoorAirNodeName,true);
  }

  boost::optional<double> WaterHeaterStratified_Impl::uniformSkinLossCoefficientperUnitAreatoAmbientTemperature() const {
    return getDouble(OS_WaterHeater_StratifiedFields::UniformSkinLossCoefficientperUnitAreatoAmbientTemperature,true);
  }

  double WaterHeaterStratified_Impl::skinLossFractiontoZone() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::SkinLossFractiontoZone,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> WaterHeaterStratified_Impl::offCycleFlueLossCoefficienttoAmbientTemperature() const {
    return getDouble(OS_WaterHeater_StratifiedFields::OffCycleFlueLossCoefficienttoAmbientTemperature,true);
  }

  double WaterHeaterStratified_Impl::offCycleFlueLossFractiontoZone() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::OffCycleFlueLossFractiontoZone,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> WaterHeaterStratified_Impl::peakUseFlowRate() const {
    return getDouble(OS_WaterHeater_StratifiedFields::PeakUseFlowRate,true);
  }

  boost::optional<Schedule> WaterHeaterStratified_Impl::useFlowRateFractionSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_StratifiedFields::UseFlowRateFractionScheduleName);
  }

  boost::optional<Schedule> WaterHeaterStratified_Impl::coldWaterSupplyTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_StratifiedFields::ColdWaterSupplyTemperatureScheduleName);
  }

  double WaterHeaterStratified_Impl::useSideEffectiveness() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::UseSideEffectiveness,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::useSideInletHeight() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::UseSideInletHeight,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> WaterHeaterStratified_Impl::useSideOutletHeight() const {
    return getDouble(OS_WaterHeater_StratifiedFields::UseSideOutletHeight,true);
  }

  bool WaterHeaterStratified_Impl::isUseSideOutletHeightAutocalculated() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_WaterHeater_StratifiedFields::UseSideOutletHeight, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autocalculate");
    }
    return result;
  }

  double WaterHeaterStratified_Impl::sourceSideEffectiveness() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::SourceSideEffectiveness,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> WaterHeaterStratified_Impl::sourceSideInletHeight() const {
    return getDouble(OS_WaterHeater_StratifiedFields::SourceSideInletHeight,true);
  }

  bool WaterHeaterStratified_Impl::isSourceSideInletHeightAutocalculated() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_WaterHeater_StratifiedFields::SourceSideInletHeight, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autocalculate");
    }
    return result;
  }

  double WaterHeaterStratified_Impl::sourceSideOutletHeight() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::SourceSideOutletHeight,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string WaterHeaterStratified_Impl::inletMode() const {
    boost::optional<std::string> value = getString(OS_WaterHeater_StratifiedFields::InletMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> WaterHeaterStratified_Impl::useSideDesignFlowRate() const {
    return getDouble(OS_WaterHeater_StratifiedFields::UseSideDesignFlowRate,true);
  }

  bool WaterHeaterStratified_Impl::isUseSideDesignFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_WaterHeater_StratifiedFields::UseSideDesignFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> WaterHeaterStratified_Impl::sourceSideDesignFlowRate() const {
    return getDouble(OS_WaterHeater_StratifiedFields::SourceSideDesignFlowRate,true);
  }

  bool WaterHeaterStratified_Impl::isSourceSideDesignFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_WaterHeater_StratifiedFields::SourceSideDesignFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double WaterHeaterStratified_Impl::indirectWaterHeatingRecoveryTime() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::IndirectWaterHeatingRecoveryTime,true);
    OS_ASSERT(value);
    return value.get();
  }

  int WaterHeaterStratified_Impl::numberofNodes() const {
    boost::optional<int> value = getInt(OS_WaterHeater_StratifiedFields::NumberofNodes,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::additionalDestratificationConductivity() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::AdditionalDestratificationConductivity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::node1AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Node1AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::node2AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Node2AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::node3AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Node3AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::node4AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Node4AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::node5AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Node5AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::node6AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Node6AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::node7AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Node7AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::node8AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Node8AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::node9AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Node9AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::node10AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Node10AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::node11AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Node11AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterStratified_Impl::node12AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Node12AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string WaterHeaterStratified_Impl::sourceSideFlowControlMode() const {
    boost::optional<std::string> value = getString(OS_WaterHeater_StratifiedFields::SourceSideFlowControlMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> WaterHeaterStratified_Impl::indirectAlternateSetpointTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_StratifiedFields::IndirectAlternateSetpointTemperatureScheduleName);
  }

  void WaterHeaterStratified_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_WaterHeater_StratifiedFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setTankVolume(boost::optional<double> tankVolume) {
    bool result(false);
    if (tankVolume) {
      result = setDouble(OS_WaterHeater_StratifiedFields::TankVolume, tankVolume.get());
    }
    return result;
  }

  void WaterHeaterStratified_Impl::autosizeTankVolume() {
    bool result = setString(OS_WaterHeater_StratifiedFields::TankVolume, "autosize");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setTankHeight(boost::optional<double> tankHeight) {
    bool result(false);
    if (tankHeight) {
      result = setDouble(OS_WaterHeater_StratifiedFields::TankHeight, tankHeight.get());
    }
    return result;
  }

  void WaterHeaterStratified_Impl::autosizeTankHeight() {
    bool result = setString(OS_WaterHeater_StratifiedFields::TankHeight, "autosize");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setTankShape(std::string tankShape) {
    bool result = setString(OS_WaterHeater_StratifiedFields::TankShape, tankShape);
    return result;
  }

  bool WaterHeaterStratified_Impl::setTankPerimeter(boost::optional<double> tankPerimeter) {
    bool result(false);
    if (tankPerimeter) {
      result = setDouble(OS_WaterHeater_StratifiedFields::TankPerimeter, tankPerimeter.get());
    }
    else {
      resetTankPerimeter();
      result = true;
    }
    return result;
  }

  void WaterHeaterStratified_Impl::resetTankPerimeter() {
    bool result = setString(OS_WaterHeater_StratifiedFields::TankPerimeter, "");
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setMaximumTemperatureLimit(double maximumTemperatureLimit) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::MaximumTemperatureLimit, maximumTemperatureLimit);
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setHeaterPriorityControl(std::string heaterPriorityControl) {
    bool result = setString(OS_WaterHeater_StratifiedFields::HeaterPriorityControl, heaterPriorityControl);
    return result;
  }

  bool WaterHeaterStratified_Impl::setHeater1SetpointTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_WaterHeater_StratifiedFields::Heater1SetpointTemperatureScheduleName,
                              "WaterHeaterStratified",
                              "Heater 1 Setpoint Temperature",
                              schedule);
    return result;
  }

  bool WaterHeaterStratified_Impl::setHeater1DeadbandTemperatureDifference(double heater1DeadbandTemperatureDifference) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Heater1DeadbandTemperatureDifference, heater1DeadbandTemperatureDifference);
    return result;
  }

  bool WaterHeaterStratified_Impl::setHeater1Capacity(boost::optional<double> heater1Capacity) {
    bool result(false);
    if (heater1Capacity) {
      result = setDouble(OS_WaterHeater_StratifiedFields::Heater1Capacity, heater1Capacity.get());
    }
    return result;
  }

  void WaterHeaterStratified_Impl::autosizeHeater1Capacity() {
    bool result = setString(OS_WaterHeater_StratifiedFields::Heater1Capacity, "autosize");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setHeater1Height(double heater1Height) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Heater1Height, heater1Height);
    return result;
  }

  bool WaterHeaterStratified_Impl::setHeater2SetpointTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_WaterHeater_StratifiedFields::Heater2SetpointTemperatureScheduleName,
                              "WaterHeaterStratified",
                              "Heater 2 Setpoint Temperature",
                              schedule);
    return result;
  }

  bool WaterHeaterStratified_Impl::setHeater2DeadbandTemperatureDifference(double heater2DeadbandTemperatureDifference) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Heater2DeadbandTemperatureDifference, heater2DeadbandTemperatureDifference);
    return result;
  }

  bool WaterHeaterStratified_Impl::setHeater2Capacity(double heater2Capacity) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Heater2Capacity, heater2Capacity);
    return result;
  }

  bool WaterHeaterStratified_Impl::setHeater2Height(double heater2Height) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Heater2Height, heater2Height);
    return result;
  }

  bool WaterHeaterStratified_Impl::setHeaterFuelType(std::string heaterFuelType) {
    bool result = setString(OS_WaterHeater_StratifiedFields::HeaterFuelType, heaterFuelType);
    return result;
  }

  bool WaterHeaterStratified_Impl::setHeaterThermalEfficiency(double heaterThermalEfficiency) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::HeaterThermalEfficiency, heaterThermalEfficiency);
    return result;
  }

  bool WaterHeaterStratified_Impl::setOffCycleParasiticFuelConsumptionRate(double offCycleParasiticFuelConsumptionRate) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::OffCycleParasiticFuelConsumptionRate, offCycleParasiticFuelConsumptionRate);
    return result;
  }

  bool WaterHeaterStratified_Impl::setOffCycleParasiticFuelType(std::string offCycleParasiticFuelType) {
    bool result = setString(OS_WaterHeater_StratifiedFields::OffCycleParasiticFuelType, offCycleParasiticFuelType);
    return result;
  }

  bool WaterHeaterStratified_Impl::setOffCycleParasiticHeatFractiontoTank(double offCycleParasiticHeatFractiontoTank) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::OffCycleParasiticHeatFractiontoTank, offCycleParasiticHeatFractiontoTank);
    return result;
  }

  bool WaterHeaterStratified_Impl::setOffCycleParasiticHeight(double offCycleParasiticHeight) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::OffCycleParasiticHeight, offCycleParasiticHeight);
    return result;
  }

  bool WaterHeaterStratified_Impl::setOnCycleParasiticFuelConsumptionRate(double onCycleParasiticFuelConsumptionRate) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::OnCycleParasiticFuelConsumptionRate, onCycleParasiticFuelConsumptionRate);
    return result;
  }

  bool WaterHeaterStratified_Impl::setOnCycleParasiticFuelType(std::string onCycleParasiticFuelType) {
    bool result = setString(OS_WaterHeater_StratifiedFields::OnCycleParasiticFuelType, onCycleParasiticFuelType);
    return result;
  }

  bool WaterHeaterStratified_Impl::setOnCycleParasiticHeatFractiontoTank(double onCycleParasiticHeatFractiontoTank) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::OnCycleParasiticHeatFractiontoTank, onCycleParasiticHeatFractiontoTank);
    return result;
  }

  bool WaterHeaterStratified_Impl::setOnCycleParasiticHeight(double onCycleParasiticHeight) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::OnCycleParasiticHeight, onCycleParasiticHeight);
    return result;
  }

  bool WaterHeaterStratified_Impl::setAmbientTemperatureIndicator(std::string ambientTemperatureIndicator) {
    bool result = setString(OS_WaterHeater_StratifiedFields::AmbientTemperatureIndicator, ambientTemperatureIndicator);
    return result;
  }

  bool WaterHeaterStratified_Impl::setAmbientTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_WaterHeater_StratifiedFields::AmbientTemperatureScheduleName,
                              "WaterHeaterStratified",
                              "Ambient Temperature",
                              schedule);
    return result;
  }

  void WaterHeaterStratified_Impl::resetAmbientTemperatureSchedule() {
    bool result = setString(OS_WaterHeater_StratifiedFields::AmbientTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setAmbientTemperatureThermalZone(const boost::optional<ThermalZone>& thermalZone) {
    bool result(false);
    if (thermalZone) {
      result = setPointer(OS_WaterHeater_StratifiedFields::AmbientTemperatureThermalZoneName, thermalZone.get().handle());
    }
    else {
      resetAmbientTemperatureThermalZone();
      result = true;
    }
    return result;
  }

  void WaterHeaterStratified_Impl::resetAmbientTemperatureThermalZone() {
    bool result = setString(OS_WaterHeater_StratifiedFields::AmbientTemperatureThermalZoneName, "");
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setAmbientTemperatureOutdoorAirNodeName(boost::optional<std::string> ambientTemperatureOutdoorAirNodeName) {
    bool result(false);
    if (ambientTemperatureOutdoorAirNodeName) {
      result = setString(OS_WaterHeater_StratifiedFields::AmbientTemperatureOutdoorAirNodeName, ambientTemperatureOutdoorAirNodeName.get());
    }
    else {
      resetAmbientTemperatureOutdoorAirNodeName();
      result = true;
    }
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::resetAmbientTemperatureOutdoorAirNodeName() {
    bool result = setString(OS_WaterHeater_StratifiedFields::AmbientTemperatureOutdoorAirNodeName, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setUniformSkinLossCoefficientperUnitAreatoAmbientTemperature(boost::optional<double> uniformSkinLossCoefficientperUnitAreatoAmbientTemperature) {
    bool result(false);
    if (uniformSkinLossCoefficientperUnitAreatoAmbientTemperature) {
      result = setDouble(OS_WaterHeater_StratifiedFields::UniformSkinLossCoefficientperUnitAreatoAmbientTemperature, uniformSkinLossCoefficientperUnitAreatoAmbientTemperature.get());
    }
    else {
      resetUniformSkinLossCoefficientperUnitAreatoAmbientTemperature();
      result = true;
    }
    return result;
  }

  void WaterHeaterStratified_Impl::resetUniformSkinLossCoefficientperUnitAreatoAmbientTemperature() {
    bool result = setString(OS_WaterHeater_StratifiedFields::UniformSkinLossCoefficientperUnitAreatoAmbientTemperature, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setSkinLossFractiontoZone(double skinLossFractiontoZone) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::SkinLossFractiontoZone, skinLossFractiontoZone);
    return result;
  }

  bool WaterHeaterStratified_Impl::setOffCycleFlueLossCoefficienttoAmbientTemperature(boost::optional<double> offCycleFlueLossCoefficienttoAmbientTemperature) {
    bool result(false);
    if (offCycleFlueLossCoefficienttoAmbientTemperature) {
      result = setDouble(OS_WaterHeater_StratifiedFields::OffCycleFlueLossCoefficienttoAmbientTemperature, offCycleFlueLossCoefficienttoAmbientTemperature.get());
    }
    else {
      resetOffCycleFlueLossCoefficienttoAmbientTemperature();
      result = true;
    }
    return result;
  }

  void WaterHeaterStratified_Impl::resetOffCycleFlueLossCoefficienttoAmbientTemperature() {
    bool result = setString(OS_WaterHeater_StratifiedFields::OffCycleFlueLossCoefficienttoAmbientTemperature, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setOffCycleFlueLossFractiontoZone(double offCycleFlueLossFractiontoZone) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::OffCycleFlueLossFractiontoZone, offCycleFlueLossFractiontoZone);
    return result;
  }

  bool WaterHeaterStratified_Impl::setPeakUseFlowRate(boost::optional<double> peakUseFlowRate) {
    bool result(false);
    if (peakUseFlowRate) {
      result = setDouble(OS_WaterHeater_StratifiedFields::PeakUseFlowRate, peakUseFlowRate.get());
    }
    else {
      resetPeakUseFlowRate();
      result = true;
    }
    return result;
  }

  void WaterHeaterStratified_Impl::resetPeakUseFlowRate() {
    bool result = setString(OS_WaterHeater_StratifiedFields::PeakUseFlowRate, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setUseFlowRateFractionSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_WaterHeater_StratifiedFields::UseFlowRateFractionScheduleName,
                              "WaterHeaterStratified",
                              "Use Flow Rate Fraction",
                              schedule);
    return result;
  }

  void WaterHeaterStratified_Impl::resetUseFlowRateFractionSchedule() {
    bool result = setString(OS_WaterHeater_StratifiedFields::UseFlowRateFractionScheduleName, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setColdWaterSupplyTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_WaterHeater_StratifiedFields::ColdWaterSupplyTemperatureScheduleName,
                              "WaterHeaterStratified",
                              "Cold Water Supply Temperature",
                              schedule);
    return result;
  }

  void WaterHeaterStratified_Impl::resetColdWaterSupplyTemperatureSchedule() {
    bool result = setString(OS_WaterHeater_StratifiedFields::ColdWaterSupplyTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setUseSideEffectiveness(double useSideEffectiveness) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::UseSideEffectiveness, useSideEffectiveness);
    return result;
  }

  bool WaterHeaterStratified_Impl::setUseSideInletHeight(double useSideInletHeight) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::UseSideInletHeight, useSideInletHeight);
    return result;
  }

  bool WaterHeaterStratified_Impl::setUseSideOutletHeight(boost::optional<double> useSideOutletHeight) {
    bool result(false);
    if (useSideOutletHeight) {
      result = setDouble(OS_WaterHeater_StratifiedFields::UseSideOutletHeight, useSideOutletHeight.get());
    }
    return result;
  }

  void WaterHeaterStratified_Impl::autocalculateUseSideOutletHeight() {
    bool result = setString(OS_WaterHeater_StratifiedFields::UseSideOutletHeight, "autocalculate");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setSourceSideEffectiveness(double sourceSideEffectiveness) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::SourceSideEffectiveness, sourceSideEffectiveness);
    return result;
  }

  bool WaterHeaterStratified_Impl::setSourceSideInletHeight(boost::optional<double> sourceSideInletHeight) {
    bool result(false);
    if (sourceSideInletHeight) {
      result = setDouble(OS_WaterHeater_StratifiedFields::SourceSideInletHeight, sourceSideInletHeight.get());
    }
    return result;
  }

  void WaterHeaterStratified_Impl::autocalculateSourceSideInletHeight() {
    bool result = setString(OS_WaterHeater_StratifiedFields::SourceSideInletHeight, "autocalculate");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setSourceSideOutletHeight(double sourceSideOutletHeight) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::SourceSideOutletHeight, sourceSideOutletHeight);
    return result;
  }

  bool WaterHeaterStratified_Impl::setInletMode(std::string inletMode) {
    bool result = setString(OS_WaterHeater_StratifiedFields::InletMode, inletMode);
    return result;
  }

  bool WaterHeaterStratified_Impl::setUseSideDesignFlowRate(boost::optional<double> useSideDesignFlowRate) {
    bool result(false);
    if (useSideDesignFlowRate) {
      result = setDouble(OS_WaterHeater_StratifiedFields::UseSideDesignFlowRate, useSideDesignFlowRate.get());
    }
    return result;
  }

  void WaterHeaterStratified_Impl::autosizeUseSideDesignFlowRate() {
    bool result = setString(OS_WaterHeater_StratifiedFields::UseSideDesignFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setSourceSideDesignFlowRate(boost::optional<double> sourceSideDesignFlowRate) {
    bool result(false);
    if (sourceSideDesignFlowRate) {
      result = setDouble(OS_WaterHeater_StratifiedFields::SourceSideDesignFlowRate, sourceSideDesignFlowRate.get());
    }
    return result;
  }

  void WaterHeaterStratified_Impl::autosizeSourceSideDesignFlowRate() {
    bool result = setString(OS_WaterHeater_StratifiedFields::SourceSideDesignFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setIndirectWaterHeatingRecoveryTime(double indirectWaterHeatingRecoveryTime) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::IndirectWaterHeatingRecoveryTime, indirectWaterHeatingRecoveryTime);
    return result;
  }

  bool WaterHeaterStratified_Impl::setNumberofNodes(int numberofNodes) {
    bool result = setInt(OS_WaterHeater_StratifiedFields::NumberofNodes, numberofNodes);
    return result;
  }

  bool WaterHeaterStratified_Impl::setAdditionalDestratificationConductivity(double additionalDestratificationConductivity) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::AdditionalDestratificationConductivity, additionalDestratificationConductivity);
    return result;
  }

  void WaterHeaterStratified_Impl::setNode1AdditionalLossCoefficient(double node1AdditionalLossCoefficient) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Node1AdditionalLossCoefficient, node1AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setNode2AdditionalLossCoefficient(double node2AdditionalLossCoefficient) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Node2AdditionalLossCoefficient, node2AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setNode3AdditionalLossCoefficient(double node3AdditionalLossCoefficient) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Node3AdditionalLossCoefficient, node3AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setNode4AdditionalLossCoefficient(double node4AdditionalLossCoefficient) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Node4AdditionalLossCoefficient, node4AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setNode5AdditionalLossCoefficient(double node5AdditionalLossCoefficient) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Node5AdditionalLossCoefficient, node5AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setNode6AdditionalLossCoefficient(double node6AdditionalLossCoefficient) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Node6AdditionalLossCoefficient, node6AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setNode7AdditionalLossCoefficient(double node7AdditionalLossCoefficient) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Node7AdditionalLossCoefficient, node7AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setNode8AdditionalLossCoefficient(double node8AdditionalLossCoefficient) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Node8AdditionalLossCoefficient, node8AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setNode9AdditionalLossCoefficient(double node9AdditionalLossCoefficient) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Node9AdditionalLossCoefficient, node9AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setNode10AdditionalLossCoefficient(double node10AdditionalLossCoefficient) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Node10AdditionalLossCoefficient, node10AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setNode11AdditionalLossCoefficient(double node11AdditionalLossCoefficient) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Node11AdditionalLossCoefficient, node11AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setNode12AdditionalLossCoefficient(double node12AdditionalLossCoefficient) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Node12AdditionalLossCoefficient, node12AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setSourceSideFlowControlMode(std::string sourceSideFlowControlMode) {
    bool result = setString(OS_WaterHeater_StratifiedFields::SourceSideFlowControlMode, sourceSideFlowControlMode);
    return result;
  }

  bool WaterHeaterStratified_Impl::setIndirectAlternateSetpointTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_WaterHeater_StratifiedFields::IndirectAlternateSetpointTemperatureScheduleName,
                              "WaterHeaterStratified",
                              "Indirect Alternate Setpoint Temperature",
                              schedule);
    return result;
  }

  void WaterHeaterStratified_Impl::resetIndirectAlternateSetpointTemperatureSchedule() {
    bool result = setString(OS_WaterHeater_StratifiedFields::IndirectAlternateSetpointTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> WaterHeaterStratified_Impl::optionalHeater1SetpointTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_StratifiedFields::Heater1SetpointTemperatureScheduleName);
  }

  boost::optional<Schedule> WaterHeaterStratified_Impl::optionalHeater2SetpointTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_StratifiedFields::Heater2SetpointTemperatureScheduleName);
  }

  boost::optional<ZoneHVACComponent> WaterHeaterStratified_Impl::containingZoneHVACComponent() const
  {
    {
      auto hpwhs = model().getConcreteModelObjects<model::WaterHeaterHeatPump>();
      auto t_Handle = handle();
      for( const auto & hpwh : hpwhs ) {
        if( hpwh.tank().handle() == t_Handle ) {
          return hpwh;
        }
      }
    }

    {
      auto hpwhs = model().getConcreteModelObjects<model::WaterHeaterHeatPumpWrappedCondenser>();
      auto t_Handle = handle();
      for( const auto & hpwh : hpwhs ) {
        if( hpwh.tank().handle() == t_Handle ) {
          return hpwh;
        }
      }
    }

    return boost::none;
  }

} // detail

WaterHeaterStratified::WaterHeaterStratified(const Model& model)
  : WaterToWaterComponent(WaterHeaterStratified::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::WaterHeaterStratified_Impl>());

  setTankVolume(0.1893); // autosizable
  setTankHeight(1.4); // autosizable

  setTankShape("VerticalCylinder");
  setMaximumTemperatureLimit(82.22);
  setHeaterPriorityControl("MasterSlave");

  ScheduleRuleset setpoint_schedule_1(model);
  setpoint_schedule_1.defaultDaySchedule().addValue(Time(0,24,0,0),48.89);

  ScheduleRuleset setpoint_schedule_2(model);
  setpoint_schedule_2.defaultDaySchedule().addValue(Time(0,24,0,0),48.89);

  setHeater1SetpointTemperatureSchedule(setpoint_schedule_1);
  setHeater1DeadbandTemperatureDifference(2);
  setHeater1Capacity(4500);
  setHeater1Height(1);

  setHeater2SetpointTemperatureSchedule(setpoint_schedule_2);
  setHeater2DeadbandTemperatureDifference(5);
  setHeater2Capacity(4500);
  setHeater2Height(0);

  setHeaterFuelType("Electricity");
  setHeaterThermalEfficiency(0.98);

  setOffCycleParasiticFuelConsumptionRate(10.0);
  setOffCycleParasiticFuelType("Electricity");
  setOffCycleParasiticHeatFractiontoTank(0);
  setOffCycleParasiticHeight(0);

  setOnCycleParasiticFuelConsumptionRate(10.0);
  setOnCycleParasiticFuelType("Electricity");
  setOnCycleParasiticHeatFractiontoTank(0);
  setOnCycleParasiticHeight(0);

  setAmbientTemperatureIndicator("Schedule");
  ScheduleRuleset amb_schedule(model);
  amb_schedule.defaultDaySchedule().addValue(Time(0,24,0,0),22.0);
  setAmbientTemperatureSchedule(amb_schedule);

  setSkinLossFractiontoZone(1);

  setUniformSkinLossCoefficientperUnitAreatoAmbientTemperature(0.846);

  setOffCycleFlueLossCoefficienttoAmbientTemperature(0);
  setOffCycleFlueLossFractiontoZone(1);

  setUseSideEffectiveness(1.0);
  setUseSideInletHeight(0);
  autocalculateUseSideOutletHeight();
  setSourceSideEffectiveness(1.0);
  autocalculateSourceSideInletHeight();
  setSourceSideOutletHeight(0);
  setInletMode("Fixed");
  autosizeUseSideDesignFlowRate();
  autosizeSourceSideDesignFlowRate();
  setIndirectWaterHeatingRecoveryTime(1.5);
  setEndUseSubcategory("General");

  setNumberofNodes(6);
  setAdditionalDestratificationConductivity(0.1);
  setNode1AdditionalLossCoefficient(0.15);
  setNode2AdditionalLossCoefficient(0);
  setNode3AdditionalLossCoefficient(0);
  setNode4AdditionalLossCoefficient(0);
  setNode5AdditionalLossCoefficient(0);
  setNode6AdditionalLossCoefficient(0.1);
  setNode7AdditionalLossCoefficient(0);
  setNode8AdditionalLossCoefficient(0);
  setNode9AdditionalLossCoefficient(0);
  setNode10AdditionalLossCoefficient(0);
  setNode11AdditionalLossCoefficient(0);
  setNode12AdditionalLossCoefficient(0);

  setSourceSideFlowControlMode("IndirectHeatPrimarySetpoint");
}

IddObjectType WaterHeaterStratified::iddObjectType() {
  return IddObjectType(IddObjectType::OS_WaterHeater_Stratified);
}

std::vector<std::string> WaterHeaterStratified::tankShapeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WaterHeater_StratifiedFields::TankShape);
}

std::vector<std::string> WaterHeaterStratified::heaterPriorityControlValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WaterHeater_StratifiedFields::HeaterPriorityControl);
}

std::vector<std::string> WaterHeaterStratified::heaterFuelTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WaterHeater_StratifiedFields::HeaterFuelType);
}

std::vector<std::string> WaterHeaterStratified::offCycleParasiticFuelTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WaterHeater_StratifiedFields::OffCycleParasiticFuelType);
}

std::vector<std::string> WaterHeaterStratified::onCycleParasiticFuelTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WaterHeater_StratifiedFields::OnCycleParasiticFuelType);
}

std::vector<std::string> WaterHeaterStratified::ambientTemperatureIndicatorValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WaterHeater_StratifiedFields::AmbientTemperatureIndicator);
}

std::vector<std::string> WaterHeaterStratified::inletModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WaterHeater_StratifiedFields::InletMode);
}

std::vector<std::string> WaterHeaterStratified::sourceSideFlowControlModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WaterHeater_StratifiedFields::SourceSideFlowControlMode);
}

std::string WaterHeaterStratified::endUseSubcategory() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->endUseSubcategory();
}

boost::optional<double> WaterHeaterStratified::tankVolume() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->tankVolume();
}

bool WaterHeaterStratified::isTankVolumeAutosized() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isTankVolumeAutosized();
}

boost::optional<double> WaterHeaterStratified::tankHeight() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->tankHeight();
}

bool WaterHeaterStratified::isTankHeightAutosized() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isTankHeightAutosized();
}

std::string WaterHeaterStratified::tankShape() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->tankShape();
}

boost::optional<double> WaterHeaterStratified::tankPerimeter() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->tankPerimeter();
}

double WaterHeaterStratified::maximumTemperatureLimit() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->maximumTemperatureLimit();
}

std::string WaterHeaterStratified::heaterPriorityControl() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->heaterPriorityControl();
}

Schedule WaterHeaterStratified::heater1SetpointTemperatureSchedule() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->heater1SetpointTemperatureSchedule();
}

double WaterHeaterStratified::heater1DeadbandTemperatureDifference() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->heater1DeadbandTemperatureDifference();
}

boost::optional<double> WaterHeaterStratified::heater1Capacity() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->heater1Capacity();
}

bool WaterHeaterStratified::isHeater1CapacityAutosized() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isHeater1CapacityAutosized();
}

double WaterHeaterStratified::heater1Height() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->heater1Height();
}

Schedule WaterHeaterStratified::heater2SetpointTemperatureSchedule() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->heater2SetpointTemperatureSchedule();
}

double WaterHeaterStratified::heater2DeadbandTemperatureDifference() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->heater2DeadbandTemperatureDifference();
}

double WaterHeaterStratified::heater2Capacity() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->heater2Capacity();
}

double WaterHeaterStratified::heater2Height() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->heater2Height();
}

std::string WaterHeaterStratified::heaterFuelType() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->heaterFuelType();
}

double WaterHeaterStratified::heaterThermalEfficiency() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->heaterThermalEfficiency();
}

double WaterHeaterStratified::offCycleParasiticFuelConsumptionRate() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->offCycleParasiticFuelConsumptionRate();
}

std::string WaterHeaterStratified::offCycleParasiticFuelType() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->offCycleParasiticFuelType();
}

double WaterHeaterStratified::offCycleParasiticHeatFractiontoTank() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->offCycleParasiticHeatFractiontoTank();
}

double WaterHeaterStratified::offCycleParasiticHeight() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->offCycleParasiticHeight();
}

double WaterHeaterStratified::onCycleParasiticFuelConsumptionRate() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->onCycleParasiticFuelConsumptionRate();
}

std::string WaterHeaterStratified::onCycleParasiticFuelType() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->onCycleParasiticFuelType();
}

double WaterHeaterStratified::onCycleParasiticHeatFractiontoTank() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->onCycleParasiticHeatFractiontoTank();
}

double WaterHeaterStratified::onCycleParasiticHeight() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->onCycleParasiticHeight();
}

std::string WaterHeaterStratified::ambientTemperatureIndicator() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->ambientTemperatureIndicator();
}

boost::optional<Schedule> WaterHeaterStratified::ambientTemperatureSchedule() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->ambientTemperatureSchedule();
}

boost::optional<ThermalZone> WaterHeaterStratified::ambientTemperatureThermalZone() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->ambientTemperatureThermalZone();
}

boost::optional<std::string> WaterHeaterStratified::ambientTemperatureOutdoorAirNodeName() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->ambientTemperatureOutdoorAirNodeName();
}

boost::optional<double> WaterHeaterStratified::uniformSkinLossCoefficientperUnitAreatoAmbientTemperature() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->uniformSkinLossCoefficientperUnitAreatoAmbientTemperature();
}

double WaterHeaterStratified::skinLossFractiontoZone() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->skinLossFractiontoZone();
}

boost::optional<double> WaterHeaterStratified::offCycleFlueLossCoefficienttoAmbientTemperature() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->offCycleFlueLossCoefficienttoAmbientTemperature();
}

double WaterHeaterStratified::offCycleFlueLossFractiontoZone() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->offCycleFlueLossFractiontoZone();
}

boost::optional<double> WaterHeaterStratified::peakUseFlowRate() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->peakUseFlowRate();
}

boost::optional<Schedule> WaterHeaterStratified::useFlowRateFractionSchedule() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->useFlowRateFractionSchedule();
}

boost::optional<Schedule> WaterHeaterStratified::coldWaterSupplyTemperatureSchedule() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->coldWaterSupplyTemperatureSchedule();
}

double WaterHeaterStratified::useSideEffectiveness() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->useSideEffectiveness();
}

double WaterHeaterStratified::useSideInletHeight() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->useSideInletHeight();
}

boost::optional<double> WaterHeaterStratified::useSideOutletHeight() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->useSideOutletHeight();
}

bool WaterHeaterStratified::isUseSideOutletHeightAutocalculated() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isUseSideOutletHeightAutocalculated();
}

double WaterHeaterStratified::sourceSideEffectiveness() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->sourceSideEffectiveness();
}

boost::optional<double> WaterHeaterStratified::sourceSideInletHeight() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->sourceSideInletHeight();
}

bool WaterHeaterStratified::isSourceSideInletHeightAutocalculated() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isSourceSideInletHeightAutocalculated();
}

double WaterHeaterStratified::sourceSideOutletHeight() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->sourceSideOutletHeight();
}

std::string WaterHeaterStratified::inletMode() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->inletMode();
}

boost::optional<double> WaterHeaterStratified::useSideDesignFlowRate() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->useSideDesignFlowRate();
}

bool WaterHeaterStratified::isUseSideDesignFlowRateAutosized() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isUseSideDesignFlowRateAutosized();
}

boost::optional<double> WaterHeaterStratified::sourceSideDesignFlowRate() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->sourceSideDesignFlowRate();
}

bool WaterHeaterStratified::isSourceSideDesignFlowRateAutosized() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isSourceSideDesignFlowRateAutosized();
}

double WaterHeaterStratified::indirectWaterHeatingRecoveryTime() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->indirectWaterHeatingRecoveryTime();
}

int WaterHeaterStratified::numberofNodes() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->numberofNodes();
}

double WaterHeaterStratified::additionalDestratificationConductivity() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->additionalDestratificationConductivity();
}

double WaterHeaterStratified::node1AdditionalLossCoefficient() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->node1AdditionalLossCoefficient();
}

double WaterHeaterStratified::node2AdditionalLossCoefficient() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->node2AdditionalLossCoefficient();
}

double WaterHeaterStratified::node3AdditionalLossCoefficient() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->node3AdditionalLossCoefficient();
}

double WaterHeaterStratified::node4AdditionalLossCoefficient() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->node4AdditionalLossCoefficient();
}

double WaterHeaterStratified::node5AdditionalLossCoefficient() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->node5AdditionalLossCoefficient();
}

double WaterHeaterStratified::node6AdditionalLossCoefficient() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->node6AdditionalLossCoefficient();
}

double WaterHeaterStratified::node7AdditionalLossCoefficient() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->node7AdditionalLossCoefficient();
}

double WaterHeaterStratified::node8AdditionalLossCoefficient() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->node8AdditionalLossCoefficient();
}

double WaterHeaterStratified::node9AdditionalLossCoefficient() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->node9AdditionalLossCoefficient();
}

double WaterHeaterStratified::node10AdditionalLossCoefficient() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->node10AdditionalLossCoefficient();
}

double WaterHeaterStratified::node11AdditionalLossCoefficient() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->node11AdditionalLossCoefficient();
}

double WaterHeaterStratified::node12AdditionalLossCoefficient() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->node12AdditionalLossCoefficient();
}

std::string WaterHeaterStratified::sourceSideFlowControlMode() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->sourceSideFlowControlMode();
}

boost::optional<Schedule> WaterHeaterStratified::indirectAlternateSetpointTemperatureSchedule() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->indirectAlternateSetpointTemperatureSchedule();
}

void WaterHeaterStratified::setEndUseSubcategory(std::string endUseSubcategory) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

bool WaterHeaterStratified::setTankVolume(double tankVolume) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setTankVolume(tankVolume);
}

void WaterHeaterStratified::autosizeTankVolume() {
  getImpl<detail::WaterHeaterStratified_Impl>()->autosizeTankVolume();
}

bool WaterHeaterStratified::setTankHeight(double tankHeight) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setTankHeight(tankHeight);
}

void WaterHeaterStratified::autosizeTankHeight() {
  getImpl<detail::WaterHeaterStratified_Impl>()->autosizeTankHeight();
}

bool WaterHeaterStratified::setTankShape(std::string tankShape) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setTankShape(tankShape);
}

bool WaterHeaterStratified::setTankPerimeter(double tankPerimeter) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setTankPerimeter(tankPerimeter);
}

void WaterHeaterStratified::resetTankPerimeter() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetTankPerimeter();
}

void WaterHeaterStratified::setMaximumTemperatureLimit(double maximumTemperatureLimit) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setMaximumTemperatureLimit(maximumTemperatureLimit);
}

bool WaterHeaterStratified::setHeaterPriorityControl(std::string heaterPriorityControl) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setHeaterPriorityControl(heaterPriorityControl);
}

bool WaterHeaterStratified::setHeater1SetpointTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setHeater1SetpointTemperatureSchedule(schedule);
}

bool WaterHeaterStratified::setHeater1DeadbandTemperatureDifference(double heater1DeadbandTemperatureDifference) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setHeater1DeadbandTemperatureDifference(heater1DeadbandTemperatureDifference);
}

bool WaterHeaterStratified::setHeater1Capacity(double heater1Capacity) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setHeater1Capacity(heater1Capacity);
}

void WaterHeaterStratified::autosizeHeater1Capacity() {
  getImpl<detail::WaterHeaterStratified_Impl>()->autosizeHeater1Capacity();
}

bool WaterHeaterStratified::setHeater1Height(double heater1Height) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setHeater1Height(heater1Height);
}

bool WaterHeaterStratified::setHeater2SetpointTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setHeater2SetpointTemperatureSchedule(schedule);
}

bool WaterHeaterStratified::setHeater2DeadbandTemperatureDifference(double heater2DeadbandTemperatureDifference) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setHeater2DeadbandTemperatureDifference(heater2DeadbandTemperatureDifference);
}

bool WaterHeaterStratified::setHeater2Capacity(double heater2Capacity) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setHeater2Capacity(heater2Capacity);
}

bool WaterHeaterStratified::setHeater2Height(double heater2Height) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setHeater2Height(heater2Height);
}

bool WaterHeaterStratified::setHeaterFuelType(std::string heaterFuelType) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setHeaterFuelType(heaterFuelType);
}

bool WaterHeaterStratified::setHeaterThermalEfficiency(double heaterThermalEfficiency) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setHeaterThermalEfficiency(heaterThermalEfficiency);
}

bool WaterHeaterStratified::setOffCycleParasiticFuelConsumptionRate(double offCycleParasiticFuelConsumptionRate) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setOffCycleParasiticFuelConsumptionRate(offCycleParasiticFuelConsumptionRate);
}

bool WaterHeaterStratified::setOffCycleParasiticFuelType(std::string offCycleParasiticFuelType) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setOffCycleParasiticFuelType(offCycleParasiticFuelType);
}

bool WaterHeaterStratified::setOffCycleParasiticHeatFractiontoTank(double offCycleParasiticHeatFractiontoTank) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setOffCycleParasiticHeatFractiontoTank(offCycleParasiticHeatFractiontoTank);
}

bool WaterHeaterStratified::setOffCycleParasiticHeight(double offCycleParasiticHeight) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setOffCycleParasiticHeight(offCycleParasiticHeight);
}

bool WaterHeaterStratified::setOnCycleParasiticFuelConsumptionRate(double onCycleParasiticFuelConsumptionRate) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setOnCycleParasiticFuelConsumptionRate(onCycleParasiticFuelConsumptionRate);
}

bool WaterHeaterStratified::setOnCycleParasiticFuelType(std::string onCycleParasiticFuelType) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setOnCycleParasiticFuelType(onCycleParasiticFuelType);
}

bool WaterHeaterStratified::setOnCycleParasiticHeatFractiontoTank(double onCycleParasiticHeatFractiontoTank) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setOnCycleParasiticHeatFractiontoTank(onCycleParasiticHeatFractiontoTank);
}

bool WaterHeaterStratified::setOnCycleParasiticHeight(double onCycleParasiticHeight) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setOnCycleParasiticHeight(onCycleParasiticHeight);
}

bool WaterHeaterStratified::setAmbientTemperatureIndicator(std::string ambientTemperatureIndicator) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setAmbientTemperatureIndicator(ambientTemperatureIndicator);
}

bool WaterHeaterStratified::setAmbientTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setAmbientTemperatureSchedule(schedule);
}

void WaterHeaterStratified::resetAmbientTemperatureSchedule() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetAmbientTemperatureSchedule();
}

bool WaterHeaterStratified::setAmbientTemperatureThermalZone(const ThermalZone& thermalZone) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setAmbientTemperatureThermalZone(thermalZone);
}

void WaterHeaterStratified::resetAmbientTemperatureThermalZone() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetAmbientTemperatureThermalZone();
}

void WaterHeaterStratified::setAmbientTemperatureOutdoorAirNodeName(std::string ambientTemperatureOutdoorAirNodeName) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setAmbientTemperatureOutdoorAirNodeName(ambientTemperatureOutdoorAirNodeName);
}

void WaterHeaterStratified::resetAmbientTemperatureOutdoorAirNodeName() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetAmbientTemperatureOutdoorAirNodeName();
}

bool WaterHeaterStratified::setUniformSkinLossCoefficientperUnitAreatoAmbientTemperature(double uniformSkinLossCoefficientperUnitAreatoAmbientTemperature) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setUniformSkinLossCoefficientperUnitAreatoAmbientTemperature(uniformSkinLossCoefficientperUnitAreatoAmbientTemperature);
}

void WaterHeaterStratified::resetUniformSkinLossCoefficientperUnitAreatoAmbientTemperature() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetUniformSkinLossCoefficientperUnitAreatoAmbientTemperature();
}

bool WaterHeaterStratified::setSkinLossFractiontoZone(double skinLossFractiontoZone) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setSkinLossFractiontoZone(skinLossFractiontoZone);
}

bool WaterHeaterStratified::setOffCycleFlueLossCoefficienttoAmbientTemperature(double offCycleFlueLossCoefficienttoAmbientTemperature) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setOffCycleFlueLossCoefficienttoAmbientTemperature(offCycleFlueLossCoefficienttoAmbientTemperature);
}

void WaterHeaterStratified::resetOffCycleFlueLossCoefficienttoAmbientTemperature() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetOffCycleFlueLossCoefficienttoAmbientTemperature();
}

bool WaterHeaterStratified::setOffCycleFlueLossFractiontoZone(double offCycleFlueLossFractiontoZone) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setOffCycleFlueLossFractiontoZone(offCycleFlueLossFractiontoZone);
}

bool WaterHeaterStratified::setPeakUseFlowRate(double peakUseFlowRate) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setPeakUseFlowRate(peakUseFlowRate);
}

void WaterHeaterStratified::resetPeakUseFlowRate() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetPeakUseFlowRate();
}

bool WaterHeaterStratified::setUseFlowRateFractionSchedule(Schedule& schedule) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setUseFlowRateFractionSchedule(schedule);
}

void WaterHeaterStratified::resetUseFlowRateFractionSchedule() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetUseFlowRateFractionSchedule();
}

bool WaterHeaterStratified::setColdWaterSupplyTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setColdWaterSupplyTemperatureSchedule(schedule);
}

void WaterHeaterStratified::resetColdWaterSupplyTemperatureSchedule() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetColdWaterSupplyTemperatureSchedule();
}

bool WaterHeaterStratified::setUseSideEffectiveness(double useSideEffectiveness) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setUseSideEffectiveness(useSideEffectiveness);
}

bool WaterHeaterStratified::setUseSideInletHeight(double useSideInletHeight) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setUseSideInletHeight(useSideInletHeight);
}

bool WaterHeaterStratified::setUseSideOutletHeight(double useSideOutletHeight) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setUseSideOutletHeight(useSideOutletHeight);
}

void WaterHeaterStratified::autocalculateUseSideOutletHeight() {
  getImpl<detail::WaterHeaterStratified_Impl>()->autocalculateUseSideOutletHeight();
}

bool WaterHeaterStratified::setSourceSideEffectiveness(double sourceSideEffectiveness) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setSourceSideEffectiveness(sourceSideEffectiveness);
}

bool WaterHeaterStratified::setSourceSideInletHeight(double sourceSideInletHeight) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setSourceSideInletHeight(sourceSideInletHeight);
}

void WaterHeaterStratified::autocalculateSourceSideInletHeight() {
  getImpl<detail::WaterHeaterStratified_Impl>()->autocalculateSourceSideInletHeight();
}

bool WaterHeaterStratified::setSourceSideOutletHeight(double sourceSideOutletHeight) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setSourceSideOutletHeight(sourceSideOutletHeight);
}

bool WaterHeaterStratified::setInletMode(std::string inletMode) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setInletMode(inletMode);
}

bool WaterHeaterStratified::setUseSideDesignFlowRate(double useSideDesignFlowRate) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setUseSideDesignFlowRate(useSideDesignFlowRate);
}

void WaterHeaterStratified::autosizeUseSideDesignFlowRate() {
  getImpl<detail::WaterHeaterStratified_Impl>()->autosizeUseSideDesignFlowRate();
}

bool WaterHeaterStratified::setSourceSideDesignFlowRate(double sourceSideDesignFlowRate) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setSourceSideDesignFlowRate(sourceSideDesignFlowRate);
}

void WaterHeaterStratified::autosizeSourceSideDesignFlowRate() {
  getImpl<detail::WaterHeaterStratified_Impl>()->autosizeSourceSideDesignFlowRate();
}

bool WaterHeaterStratified::setIndirectWaterHeatingRecoveryTime(double indirectWaterHeatingRecoveryTime) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setIndirectWaterHeatingRecoveryTime(indirectWaterHeatingRecoveryTime);
}

bool WaterHeaterStratified::setNumberofNodes(int numberofNodes) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setNumberofNodes(numberofNodes);
}

bool WaterHeaterStratified::setAdditionalDestratificationConductivity(double additionalDestratificationConductivity) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setAdditionalDestratificationConductivity(additionalDestratificationConductivity);
}

void WaterHeaterStratified::setNode1AdditionalLossCoefficient(double node1AdditionalLossCoefficient) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setNode1AdditionalLossCoefficient(node1AdditionalLossCoefficient);
}

void WaterHeaterStratified::setNode2AdditionalLossCoefficient(double node2AdditionalLossCoefficient) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setNode2AdditionalLossCoefficient(node2AdditionalLossCoefficient);
}

void WaterHeaterStratified::setNode3AdditionalLossCoefficient(double node3AdditionalLossCoefficient) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setNode3AdditionalLossCoefficient(node3AdditionalLossCoefficient);
}

void WaterHeaterStratified::setNode4AdditionalLossCoefficient(double node4AdditionalLossCoefficient) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setNode4AdditionalLossCoefficient(node4AdditionalLossCoefficient);
}

void WaterHeaterStratified::setNode5AdditionalLossCoefficient(double node5AdditionalLossCoefficient) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setNode5AdditionalLossCoefficient(node5AdditionalLossCoefficient);
}

void WaterHeaterStratified::setNode6AdditionalLossCoefficient(double node6AdditionalLossCoefficient) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setNode6AdditionalLossCoefficient(node6AdditionalLossCoefficient);
}

void WaterHeaterStratified::setNode7AdditionalLossCoefficient(double node7AdditionalLossCoefficient) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setNode7AdditionalLossCoefficient(node7AdditionalLossCoefficient);
}

void WaterHeaterStratified::setNode8AdditionalLossCoefficient(double node8AdditionalLossCoefficient) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setNode8AdditionalLossCoefficient(node8AdditionalLossCoefficient);
}

void WaterHeaterStratified::setNode9AdditionalLossCoefficient(double node9AdditionalLossCoefficient) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setNode9AdditionalLossCoefficient(node9AdditionalLossCoefficient);
}

void WaterHeaterStratified::setNode10AdditionalLossCoefficient(double node10AdditionalLossCoefficient) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setNode10AdditionalLossCoefficient(node10AdditionalLossCoefficient);
}

void WaterHeaterStratified::setNode11AdditionalLossCoefficient(double node11AdditionalLossCoefficient) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setNode11AdditionalLossCoefficient(node11AdditionalLossCoefficient);
}

void WaterHeaterStratified::setNode12AdditionalLossCoefficient(double node12AdditionalLossCoefficient) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setNode12AdditionalLossCoefficient(node12AdditionalLossCoefficient);
}

bool WaterHeaterStratified::setSourceSideFlowControlMode(std::string sourceSideFlowControlMode) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setSourceSideFlowControlMode(sourceSideFlowControlMode);
}

bool WaterHeaterStratified::setIndirectAlternateSetpointTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setIndirectAlternateSetpointTemperatureSchedule(schedule);
}

void WaterHeaterStratified::resetIndirectAlternateSetpointTemperatureSchedule() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetIndirectAlternateSetpointTemperatureSchedule();
}

/// @cond
WaterHeaterStratified::WaterHeaterStratified(std::shared_ptr<detail::WaterHeaterStratified_Impl> impl)
  : WaterToWaterComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

