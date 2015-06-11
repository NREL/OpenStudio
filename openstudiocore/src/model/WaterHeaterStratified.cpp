/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include <model/WaterHeaterStratified.hpp>
#include <model/WaterHeaterStratified_Impl.hpp>

#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_WaterHeater_Stratified_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Assert.hpp>

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

  bool WaterHeaterStratified_Impl::isEndUseSubcategoryDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::EndUseSubcategory);
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

  bool WaterHeaterStratified_Impl::isTankShapeDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::TankShape);
  }

  boost::optional<double> WaterHeaterStratified_Impl::tankPerimeter() const {
    return getDouble(OS_WaterHeater_StratifiedFields::TankPerimeter,true);
  }

  boost::optional<double> WaterHeaterStratified_Impl::maximumTemperatureLimit() const {
    return getDouble(OS_WaterHeater_StratifiedFields::MaximumTemperatureLimit,true);
  }

  std::string WaterHeaterStratified_Impl::heaterPriorityControl() const {
    boost::optional<std::string> value = getString(OS_WaterHeater_StratifiedFields::HeaterPriorityControl,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isHeaterPriorityControlDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::HeaterPriorityControl);
  }

  Schedule WaterHeaterStratified_Impl::heater1SetpointTemperatureSchedule() const {
    boost::optional<Schedule> value = optionalHeater1SetpointTemperatureSchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heater 1 Setpoint Temperature Schedule attached.");
    }
    return value.get();
  }

  double WaterHeaterStratified_Impl::heater1DeadbandTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Heater1DeadbandTemperatureDifference,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isHeater1DeadbandTemperatureDifferenceDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::Heater1DeadbandTemperatureDifference);
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

  boost::optional<double> WaterHeaterStratified_Impl::heater1Height() const {
    return getDouble(OS_WaterHeater_StratifiedFields::Heater1Height,true);
  }

  Schedule WaterHeaterStratified_Impl::heater2SetpointTemperatureSchedule() const {
    boost::optional<Schedule> value = optionalHeater2SetpointTemperatureSchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heater 2 Setpoint Temperature Schedule attached.");
    }
    return value.get();
  }

  double WaterHeaterStratified_Impl::heater2DeadbandTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Heater2DeadbandTemperatureDifference,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isHeater2DeadbandTemperatureDifferenceDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::Heater2DeadbandTemperatureDifference);
  }

  boost::optional<double> WaterHeaterStratified_Impl::heater2Capacity() const {
    return getDouble(OS_WaterHeater_StratifiedFields::Heater2Capacity,true);
  }

  boost::optional<double> WaterHeaterStratified_Impl::heater2Height() const {
    return getDouble(OS_WaterHeater_StratifiedFields::Heater2Height,true);
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

  bool WaterHeaterStratified_Impl::isOffCycleParasiticFuelConsumptionRateDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::OffCycleParasiticFuelConsumptionRate);
  }

  boost::optional<std::string> WaterHeaterStratified_Impl::offCycleParasiticFuelType() const {
    return getString(OS_WaterHeater_StratifiedFields::OffCycleParasiticFuelType,true);
  }

  double WaterHeaterStratified_Impl::offCycleParasiticHeatFractiontoTank() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::OffCycleParasiticHeatFractiontoTank,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isOffCycleParasiticHeatFractiontoTankDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::OffCycleParasiticHeatFractiontoTank);
  }

  double WaterHeaterStratified_Impl::offCycleParasiticHeight() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::OffCycleParasiticHeight,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isOffCycleParasiticHeightDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::OffCycleParasiticHeight);
  }

  double WaterHeaterStratified_Impl::onCycleParasiticFuelConsumptionRate() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::OnCycleParasiticFuelConsumptionRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isOnCycleParasiticFuelConsumptionRateDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::OnCycleParasiticFuelConsumptionRate);
  }

  boost::optional<std::string> WaterHeaterStratified_Impl::onCycleParasiticFuelType() const {
    return getString(OS_WaterHeater_StratifiedFields::OnCycleParasiticFuelType,true);
  }

  double WaterHeaterStratified_Impl::onCycleParasiticHeatFractiontoTank() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::OnCycleParasiticHeatFractiontoTank,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isOnCycleParasiticHeatFractiontoTankDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::OnCycleParasiticHeatFractiontoTank);
  }

  double WaterHeaterStratified_Impl::onCycleParasiticHeight() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::OnCycleParasiticHeight,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isOnCycleParasiticHeightDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::OnCycleParasiticHeight);
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

  bool WaterHeaterStratified_Impl::isSkinLossFractiontoZoneDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::SkinLossFractiontoZone);
  }

  boost::optional<double> WaterHeaterStratified_Impl::offCycleFlueLossCoefficienttoAmbientTemperature() const {
    return getDouble(OS_WaterHeater_StratifiedFields::OffCycleFlueLossCoefficienttoAmbientTemperature,true);
  }

  double WaterHeaterStratified_Impl::offCycleFlueLossFractiontoZone() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::OffCycleFlueLossFractiontoZone,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isOffCycleFlueLossFractiontoZoneDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::OffCycleFlueLossFractiontoZone);
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

  bool WaterHeaterStratified_Impl::isUseSideEffectivenessDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::UseSideEffectiveness);
  }

  double WaterHeaterStratified_Impl::useSideInletHeight() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::UseSideInletHeight,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isUseSideInletHeightDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::UseSideInletHeight);
  }

  boost::optional<double> WaterHeaterStratified_Impl::useSideOutletHeight() const {
    return getDouble(OS_WaterHeater_StratifiedFields::UseSideOutletHeight,true);
  }

  bool WaterHeaterStratified_Impl::isUseSideOutletHeightDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::UseSideOutletHeight);
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

  bool WaterHeaterStratified_Impl::isSourceSideEffectivenessDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::SourceSideEffectiveness);
  }

  boost::optional<double> WaterHeaterStratified_Impl::sourceSideInletHeight() const {
    return getDouble(OS_WaterHeater_StratifiedFields::SourceSideInletHeight,true);
  }

  bool WaterHeaterStratified_Impl::isSourceSideInletHeightDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::SourceSideInletHeight);
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

  bool WaterHeaterStratified_Impl::isSourceSideOutletHeightDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::SourceSideOutletHeight);
  }

  std::string WaterHeaterStratified_Impl::inletMode() const {
    boost::optional<std::string> value = getString(OS_WaterHeater_StratifiedFields::InletMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isInletModeDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::InletMode);
  }

  boost::optional<double> WaterHeaterStratified_Impl::useSideDesignFlowRate() const {
    return getDouble(OS_WaterHeater_StratifiedFields::UseSideDesignFlowRate,true);
  }

  bool WaterHeaterStratified_Impl::isUseSideDesignFlowRateDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::UseSideDesignFlowRate);
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

  bool WaterHeaterStratified_Impl::isSourceSideDesignFlowRateDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::SourceSideDesignFlowRate);
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

  bool WaterHeaterStratified_Impl::isIndirectWaterHeatingRecoveryTimeDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::IndirectWaterHeatingRecoveryTime);
  }

  int WaterHeaterStratified_Impl::numberofNodes() const {
    boost::optional<int> value = getInt(OS_WaterHeater_StratifiedFields::NumberofNodes,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isNumberofNodesDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::NumberofNodes);
  }

  double WaterHeaterStratified_Impl::additionalDestratificationConductivity() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::AdditionalDestratificationConductivity,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isAdditionalDestratificationConductivityDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::AdditionalDestratificationConductivity);
  }

  double WaterHeaterStratified_Impl::node1AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Node1AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isNode1AdditionalLossCoefficientDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::Node1AdditionalLossCoefficient);
  }

  double WaterHeaterStratified_Impl::node2AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Node2AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isNode2AdditionalLossCoefficientDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::Node2AdditionalLossCoefficient);
  }

  double WaterHeaterStratified_Impl::node3AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Node3AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isNode3AdditionalLossCoefficientDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::Node3AdditionalLossCoefficient);
  }

  double WaterHeaterStratified_Impl::node4AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Node4AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isNode4AdditionalLossCoefficientDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::Node4AdditionalLossCoefficient);
  }

  double WaterHeaterStratified_Impl::node5AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Node5AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isNode5AdditionalLossCoefficientDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::Node5AdditionalLossCoefficient);
  }

  double WaterHeaterStratified_Impl::node6AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Node6AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isNode6AdditionalLossCoefficientDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::Node6AdditionalLossCoefficient);
  }

  double WaterHeaterStratified_Impl::node7AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Node7AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isNode7AdditionalLossCoefficientDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::Node7AdditionalLossCoefficient);
  }

  double WaterHeaterStratified_Impl::node8AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Node8AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isNode8AdditionalLossCoefficientDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::Node8AdditionalLossCoefficient);
  }

  double WaterHeaterStratified_Impl::node9AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Node9AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isNode9AdditionalLossCoefficientDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::Node9AdditionalLossCoefficient);
  }

  double WaterHeaterStratified_Impl::node10AdditionalLossCoefficient() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_StratifiedFields::Node10AdditionalLossCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isNode10AdditionalLossCoefficientDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::Node10AdditionalLossCoefficient);
  }

  std::string WaterHeaterStratified_Impl::sourceSideFlowControlMode() const {
    boost::optional<std::string> value = getString(OS_WaterHeater_StratifiedFields::SourceSideFlowControlMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool WaterHeaterStratified_Impl::isSourceSideFlowControlModeDefaulted() const {
    return isEmpty(OS_WaterHeater_StratifiedFields::SourceSideFlowControlMode);
  }

  boost::optional<Schedule> WaterHeaterStratified_Impl::indirectAlternateSetpointTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_StratifiedFields::IndirectAlternateSetpointTemperatureScheduleName);
  }

  void WaterHeaterStratified_Impl::setEndUseSubcategory(std::string endUseSubcategory) {
    bool result = setString(OS_WaterHeater_StratifiedFields::EndUseSubcategory, endUseSubcategory);
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::resetEndUseSubcategory() {
    bool result = setString(OS_WaterHeater_StratifiedFields::EndUseSubcategory, "");
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

  void WaterHeaterStratified_Impl::resetTankShape() {
    bool result = setString(OS_WaterHeater_StratifiedFields::TankShape, "");
    OS_ASSERT(result);
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

  void WaterHeaterStratified_Impl::setMaximumTemperatureLimit(boost::optional<double> maximumTemperatureLimit) {
    bool result(false);
    if (maximumTemperatureLimit) {
      result = setDouble(OS_WaterHeater_StratifiedFields::MaximumTemperatureLimit, maximumTemperatureLimit.get());
    }
    else {
      resetMaximumTemperatureLimit();
      result = true;
    }
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::resetMaximumTemperatureLimit() {
    bool result = setString(OS_WaterHeater_StratifiedFields::MaximumTemperatureLimit, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setHeaterPriorityControl(std::string heaterPriorityControl) {
    bool result = setString(OS_WaterHeater_StratifiedFields::HeaterPriorityControl, heaterPriorityControl);
    return result;
  }

  void WaterHeaterStratified_Impl::resetHeaterPriorityControl() {
    bool result = setString(OS_WaterHeater_StratifiedFields::HeaterPriorityControl, "");
    OS_ASSERT(result);
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

  void WaterHeaterStratified_Impl::resetHeater1DeadbandTemperatureDifference() {
    bool result = setString(OS_WaterHeater_StratifiedFields::Heater1DeadbandTemperatureDifference, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setHeater1Capacity(boost::optional<double> heater1Capacity) {
    bool result(false);
    if (heater1Capacity) {
      result = setDouble(OS_WaterHeater_StratifiedFields::Heater1Capacity, heater1Capacity.get());
    }
    else {
      resetHeater1Capacity();
      result = true;
    }
    return result;
  }

  void WaterHeaterStratified_Impl::resetHeater1Capacity() {
    bool result = setString(OS_WaterHeater_StratifiedFields::Heater1Capacity, "");
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::autosizeHeater1Capacity() {
    bool result = setString(OS_WaterHeater_StratifiedFields::Heater1Capacity, "autosize");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setHeater1Height(boost::optional<double> heater1Height) {
    bool result(false);
    if (heater1Height) {
      result = setDouble(OS_WaterHeater_StratifiedFields::Heater1Height, heater1Height.get());
    }
    else {
      resetHeater1Height();
      result = true;
    }
    return result;
  }

  void WaterHeaterStratified_Impl::resetHeater1Height() {
    bool result = setString(OS_WaterHeater_StratifiedFields::Heater1Height, "");
    OS_ASSERT(result);
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

  void WaterHeaterStratified_Impl::resetHeater2DeadbandTemperatureDifference() {
    bool result = setString(OS_WaterHeater_StratifiedFields::Heater2DeadbandTemperatureDifference, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setHeater2Capacity(boost::optional<double> heater2Capacity) {
    bool result(false);
    if (heater2Capacity) {
      result = setDouble(OS_WaterHeater_StratifiedFields::Heater2Capacity, heater2Capacity.get());
    }
    else {
      resetHeater2Capacity();
      result = true;
    }
    return result;
  }

  void WaterHeaterStratified_Impl::resetHeater2Capacity() {
    bool result = setString(OS_WaterHeater_StratifiedFields::Heater2Capacity, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setHeater2Height(boost::optional<double> heater2Height) {
    bool result(false);
    if (heater2Height) {
      result = setDouble(OS_WaterHeater_StratifiedFields::Heater2Height, heater2Height.get());
    }
    else {
      resetHeater2Height();
      result = true;
    }
    return result;
  }

  void WaterHeaterStratified_Impl::resetHeater2Height() {
    bool result = setString(OS_WaterHeater_StratifiedFields::Heater2Height, "");
    OS_ASSERT(result);
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

  void WaterHeaterStratified_Impl::resetOffCycleParasiticFuelConsumptionRate() {
    bool result = setString(OS_WaterHeater_StratifiedFields::OffCycleParasiticFuelConsumptionRate, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setOffCycleParasiticFuelType(boost::optional<std::string> offCycleParasiticFuelType) {
    bool result(false);
    if (offCycleParasiticFuelType) {
      result = setString(OS_WaterHeater_StratifiedFields::OffCycleParasiticFuelType, offCycleParasiticFuelType.get());
    }
    else {
      resetOffCycleParasiticFuelType();
      result = true;
    }
    return result;
  }

  void WaterHeaterStratified_Impl::resetOffCycleParasiticFuelType() {
    bool result = setString(OS_WaterHeater_StratifiedFields::OffCycleParasiticFuelType, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setOffCycleParasiticHeatFractiontoTank(double offCycleParasiticHeatFractiontoTank) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::OffCycleParasiticHeatFractiontoTank, offCycleParasiticHeatFractiontoTank);
    return result;
  }

  void WaterHeaterStratified_Impl::resetOffCycleParasiticHeatFractiontoTank() {
    bool result = setString(OS_WaterHeater_StratifiedFields::OffCycleParasiticHeatFractiontoTank, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setOffCycleParasiticHeight(double offCycleParasiticHeight) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::OffCycleParasiticHeight, offCycleParasiticHeight);
    return result;
  }

  void WaterHeaterStratified_Impl::resetOffCycleParasiticHeight() {
    bool result = setString(OS_WaterHeater_StratifiedFields::OffCycleParasiticHeight, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setOnCycleParasiticFuelConsumptionRate(double onCycleParasiticFuelConsumptionRate) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::OnCycleParasiticFuelConsumptionRate, onCycleParasiticFuelConsumptionRate);
    return result;
  }

  void WaterHeaterStratified_Impl::resetOnCycleParasiticFuelConsumptionRate() {
    bool result = setString(OS_WaterHeater_StratifiedFields::OnCycleParasiticFuelConsumptionRate, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setOnCycleParasiticFuelType(boost::optional<std::string> onCycleParasiticFuelType) {
    bool result(false);
    if (onCycleParasiticFuelType) {
      result = setString(OS_WaterHeater_StratifiedFields::OnCycleParasiticFuelType, onCycleParasiticFuelType.get());
    }
    else {
      resetOnCycleParasiticFuelType();
      result = true;
    }
    return result;
  }

  void WaterHeaterStratified_Impl::resetOnCycleParasiticFuelType() {
    bool result = setString(OS_WaterHeater_StratifiedFields::OnCycleParasiticFuelType, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setOnCycleParasiticHeatFractiontoTank(double onCycleParasiticHeatFractiontoTank) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::OnCycleParasiticHeatFractiontoTank, onCycleParasiticHeatFractiontoTank);
    return result;
  }

  void WaterHeaterStratified_Impl::resetOnCycleParasiticHeatFractiontoTank() {
    bool result = setString(OS_WaterHeater_StratifiedFields::OnCycleParasiticHeatFractiontoTank, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setOnCycleParasiticHeight(double onCycleParasiticHeight) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::OnCycleParasiticHeight, onCycleParasiticHeight);
    return result;
  }

  void WaterHeaterStratified_Impl::resetOnCycleParasiticHeight() {
    bool result = setString(OS_WaterHeater_StratifiedFields::OnCycleParasiticHeight, "");
    OS_ASSERT(result);
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

  void WaterHeaterStratified_Impl::resetSkinLossFractiontoZone() {
    bool result = setString(OS_WaterHeater_StratifiedFields::SkinLossFractiontoZone, "");
    OS_ASSERT(result);
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

  void WaterHeaterStratified_Impl::resetOffCycleFlueLossFractiontoZone() {
    bool result = setString(OS_WaterHeater_StratifiedFields::OffCycleFlueLossFractiontoZone, "");
    OS_ASSERT(result);
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

  void WaterHeaterStratified_Impl::resetUseSideEffectiveness() {
    bool result = setString(OS_WaterHeater_StratifiedFields::UseSideEffectiveness, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setUseSideInletHeight(double useSideInletHeight) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::UseSideInletHeight, useSideInletHeight);
    return result;
  }

  void WaterHeaterStratified_Impl::resetUseSideInletHeight() {
    bool result = setString(OS_WaterHeater_StratifiedFields::UseSideInletHeight, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setUseSideOutletHeight(boost::optional<double> useSideOutletHeight) {
    bool result(false);
    if (useSideOutletHeight) {
      result = setDouble(OS_WaterHeater_StratifiedFields::UseSideOutletHeight, useSideOutletHeight.get());
    }
    else {
      resetUseSideOutletHeight();
      result = true;
    }
    return result;
  }

  void WaterHeaterStratified_Impl::resetUseSideOutletHeight() {
    bool result = setString(OS_WaterHeater_StratifiedFields::UseSideOutletHeight, "");
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::autocalculateUseSideOutletHeight() {
    bool result = setString(OS_WaterHeater_StratifiedFields::UseSideOutletHeight, "autocalculate");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setSourceSideEffectiveness(double sourceSideEffectiveness) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::SourceSideEffectiveness, sourceSideEffectiveness);
    return result;
  }

  void WaterHeaterStratified_Impl::resetSourceSideEffectiveness() {
    bool result = setString(OS_WaterHeater_StratifiedFields::SourceSideEffectiveness, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setSourceSideInletHeight(boost::optional<double> sourceSideInletHeight) {
    bool result(false);
    if (sourceSideInletHeight) {
      result = setDouble(OS_WaterHeater_StratifiedFields::SourceSideInletHeight, sourceSideInletHeight.get());
    }
    else {
      resetSourceSideInletHeight();
      result = true;
    }
    return result;
  }

  void WaterHeaterStratified_Impl::resetSourceSideInletHeight() {
    bool result = setString(OS_WaterHeater_StratifiedFields::SourceSideInletHeight, "");
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::autocalculateSourceSideInletHeight() {
    bool result = setString(OS_WaterHeater_StratifiedFields::SourceSideInletHeight, "autocalculate");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setSourceSideOutletHeight(double sourceSideOutletHeight) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::SourceSideOutletHeight, sourceSideOutletHeight);
    return result;
  }

  void WaterHeaterStratified_Impl::resetSourceSideOutletHeight() {
    bool result = setString(OS_WaterHeater_StratifiedFields::SourceSideOutletHeight, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setInletMode(std::string inletMode) {
    bool result = setString(OS_WaterHeater_StratifiedFields::InletMode, inletMode);
    return result;
  }

  void WaterHeaterStratified_Impl::resetInletMode() {
    bool result = setString(OS_WaterHeater_StratifiedFields::InletMode, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setUseSideDesignFlowRate(boost::optional<double> useSideDesignFlowRate) {
    bool result(false);
    if (useSideDesignFlowRate) {
      result = setDouble(OS_WaterHeater_StratifiedFields::UseSideDesignFlowRate, useSideDesignFlowRate.get());
    }
    else {
      resetUseSideDesignFlowRate();
      result = true;
    }
    return result;
  }

  void WaterHeaterStratified_Impl::resetUseSideDesignFlowRate() {
    bool result = setString(OS_WaterHeater_StratifiedFields::UseSideDesignFlowRate, "");
    OS_ASSERT(result);
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
    else {
      resetSourceSideDesignFlowRate();
      result = true;
    }
    return result;
  }

  void WaterHeaterStratified_Impl::resetSourceSideDesignFlowRate() {
    bool result = setString(OS_WaterHeater_StratifiedFields::SourceSideDesignFlowRate, "");
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::autosizeSourceSideDesignFlowRate() {
    bool result = setString(OS_WaterHeater_StratifiedFields::SourceSideDesignFlowRate, "autosize");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setIndirectWaterHeatingRecoveryTime(double indirectWaterHeatingRecoveryTime) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::IndirectWaterHeatingRecoveryTime, indirectWaterHeatingRecoveryTime);
    return result;
  }

  void WaterHeaterStratified_Impl::resetIndirectWaterHeatingRecoveryTime() {
    bool result = setString(OS_WaterHeater_StratifiedFields::IndirectWaterHeatingRecoveryTime, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setNumberofNodes(int numberofNodes) {
    bool result = setInt(OS_WaterHeater_StratifiedFields::NumberofNodes, numberofNodes);
    return result;
  }

  void WaterHeaterStratified_Impl::resetNumberofNodes() {
    bool result = setString(OS_WaterHeater_StratifiedFields::NumberofNodes, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setAdditionalDestratificationConductivity(double additionalDestratificationConductivity) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::AdditionalDestratificationConductivity, additionalDestratificationConductivity);
    return result;
  }

  void WaterHeaterStratified_Impl::resetAdditionalDestratificationConductivity() {
    bool result = setString(OS_WaterHeater_StratifiedFields::AdditionalDestratificationConductivity, "");
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setNode1AdditionalLossCoefficient(double node1AdditionalLossCoefficient) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Node1AdditionalLossCoefficient, node1AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::resetNode1AdditionalLossCoefficient() {
    bool result = setString(OS_WaterHeater_StratifiedFields::Node1AdditionalLossCoefficient, "");
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setNode2AdditionalLossCoefficient(double node2AdditionalLossCoefficient) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Node2AdditionalLossCoefficient, node2AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::resetNode2AdditionalLossCoefficient() {
    bool result = setString(OS_WaterHeater_StratifiedFields::Node2AdditionalLossCoefficient, "");
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setNode3AdditionalLossCoefficient(double node3AdditionalLossCoefficient) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Node3AdditionalLossCoefficient, node3AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::resetNode3AdditionalLossCoefficient() {
    bool result = setString(OS_WaterHeater_StratifiedFields::Node3AdditionalLossCoefficient, "");
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setNode4AdditionalLossCoefficient(double node4AdditionalLossCoefficient) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Node4AdditionalLossCoefficient, node4AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::resetNode4AdditionalLossCoefficient() {
    bool result = setString(OS_WaterHeater_StratifiedFields::Node4AdditionalLossCoefficient, "");
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setNode5AdditionalLossCoefficient(double node5AdditionalLossCoefficient) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Node5AdditionalLossCoefficient, node5AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::resetNode5AdditionalLossCoefficient() {
    bool result = setString(OS_WaterHeater_StratifiedFields::Node5AdditionalLossCoefficient, "");
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setNode6AdditionalLossCoefficient(double node6AdditionalLossCoefficient) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Node6AdditionalLossCoefficient, node6AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::resetNode6AdditionalLossCoefficient() {
    bool result = setString(OS_WaterHeater_StratifiedFields::Node6AdditionalLossCoefficient, "");
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setNode7AdditionalLossCoefficient(double node7AdditionalLossCoefficient) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Node7AdditionalLossCoefficient, node7AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::resetNode7AdditionalLossCoefficient() {
    bool result = setString(OS_WaterHeater_StratifiedFields::Node7AdditionalLossCoefficient, "");
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setNode8AdditionalLossCoefficient(double node8AdditionalLossCoefficient) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Node8AdditionalLossCoefficient, node8AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::resetNode8AdditionalLossCoefficient() {
    bool result = setString(OS_WaterHeater_StratifiedFields::Node8AdditionalLossCoefficient, "");
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setNode9AdditionalLossCoefficient(double node9AdditionalLossCoefficient) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Node9AdditionalLossCoefficient, node9AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::resetNode9AdditionalLossCoefficient() {
    bool result = setString(OS_WaterHeater_StratifiedFields::Node9AdditionalLossCoefficient, "");
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::setNode10AdditionalLossCoefficient(double node10AdditionalLossCoefficient) {
    bool result = setDouble(OS_WaterHeater_StratifiedFields::Node10AdditionalLossCoefficient, node10AdditionalLossCoefficient);
    OS_ASSERT(result);
  }

  void WaterHeaterStratified_Impl::resetNode10AdditionalLossCoefficient() {
    bool result = setString(OS_WaterHeater_StratifiedFields::Node10AdditionalLossCoefficient, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterStratified_Impl::setSourceSideFlowControlMode(std::string sourceSideFlowControlMode) {
    bool result = setString(OS_WaterHeater_StratifiedFields::SourceSideFlowControlMode, sourceSideFlowControlMode);
    return result;
  }

  void WaterHeaterStratified_Impl::resetSourceSideFlowControlMode() {
    bool result = setString(OS_WaterHeater_StratifiedFields::SourceSideFlowControlMode, "");
    OS_ASSERT(result);
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

} // detail

WaterHeaterStratified::WaterHeaterStratified(const Model& model)
  : WaterToWaterComponent(WaterHeaterStratified::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::WaterHeaterStratified_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_WaterHeater_StratifiedFields::Heater1SetpointTemperatureScheduleName
  //     OS_WaterHeater_StratifiedFields::Heater2SetpointTemperatureScheduleName
  //     OS_WaterHeater_StratifiedFields::UseSideInletNodeName
  //     OS_WaterHeater_StratifiedFields::UseSideOutletNodeName
  //     OS_WaterHeater_StratifiedFields::SourceSideInletNodeName
  //     OS_WaterHeater_StratifiedFields::SourceSideOutletNodeName
  bool ok = true;
  // ok = setHandle();
  OS_ASSERT(ok);
  // ok = setTankVolume();
  OS_ASSERT(ok);
  // ok = setTankHeight();
  OS_ASSERT(ok);
  // ok = setHeater1SetpointTemperatureSchedule();
  OS_ASSERT(ok);
  // ok = setHeater2SetpointTemperatureSchedule();
  OS_ASSERT(ok);
  // ok = setHeaterFuelType();
  OS_ASSERT(ok);
  // ok = setHeaterThermalEfficiency();
  OS_ASSERT(ok);
  // ok = setAmbientTemperatureIndicator();
  OS_ASSERT(ok);
  // ok = setUseSideInletNode();
  OS_ASSERT(ok);
  // ok = setUseSideOutletNode();
  OS_ASSERT(ok);
  // ok = setSourceSideInletNode();
  OS_ASSERT(ok);
  // ok = setSourceSideOutletNode();
  OS_ASSERT(ok);
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

bool WaterHeaterStratified::isEndUseSubcategoryDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isEndUseSubcategoryDefaulted();
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

bool WaterHeaterStratified::isTankShapeDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isTankShapeDefaulted();
}

boost::optional<double> WaterHeaterStratified::tankPerimeter() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->tankPerimeter();
}

boost::optional<double> WaterHeaterStratified::maximumTemperatureLimit() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->maximumTemperatureLimit();
}

std::string WaterHeaterStratified::heaterPriorityControl() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->heaterPriorityControl();
}

bool WaterHeaterStratified::isHeaterPriorityControlDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isHeaterPriorityControlDefaulted();
}

Schedule WaterHeaterStratified::heater1SetpointTemperatureSchedule() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->heater1SetpointTemperatureSchedule();
}

double WaterHeaterStratified::heater1DeadbandTemperatureDifference() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->heater1DeadbandTemperatureDifference();
}

bool WaterHeaterStratified::isHeater1DeadbandTemperatureDifferenceDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isHeater1DeadbandTemperatureDifferenceDefaulted();
}

boost::optional<double> WaterHeaterStratified::heater1Capacity() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->heater1Capacity();
}

bool WaterHeaterStratified::isHeater1CapacityAutosized() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isHeater1CapacityAutosized();
}

boost::optional<double> WaterHeaterStratified::heater1Height() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->heater1Height();
}

Schedule WaterHeaterStratified::heater2SetpointTemperatureSchedule() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->heater2SetpointTemperatureSchedule();
}

double WaterHeaterStratified::heater2DeadbandTemperatureDifference() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->heater2DeadbandTemperatureDifference();
}

bool WaterHeaterStratified::isHeater2DeadbandTemperatureDifferenceDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isHeater2DeadbandTemperatureDifferenceDefaulted();
}

boost::optional<double> WaterHeaterStratified::heater2Capacity() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->heater2Capacity();
}

boost::optional<double> WaterHeaterStratified::heater2Height() const {
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

bool WaterHeaterStratified::isOffCycleParasiticFuelConsumptionRateDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isOffCycleParasiticFuelConsumptionRateDefaulted();
}

boost::optional<std::string> WaterHeaterStratified::offCycleParasiticFuelType() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->offCycleParasiticFuelType();
}

double WaterHeaterStratified::offCycleParasiticHeatFractiontoTank() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->offCycleParasiticHeatFractiontoTank();
}

bool WaterHeaterStratified::isOffCycleParasiticHeatFractiontoTankDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isOffCycleParasiticHeatFractiontoTankDefaulted();
}

double WaterHeaterStratified::offCycleParasiticHeight() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->offCycleParasiticHeight();
}

bool WaterHeaterStratified::isOffCycleParasiticHeightDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isOffCycleParasiticHeightDefaulted();
}

double WaterHeaterStratified::onCycleParasiticFuelConsumptionRate() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->onCycleParasiticFuelConsumptionRate();
}

bool WaterHeaterStratified::isOnCycleParasiticFuelConsumptionRateDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isOnCycleParasiticFuelConsumptionRateDefaulted();
}

boost::optional<std::string> WaterHeaterStratified::onCycleParasiticFuelType() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->onCycleParasiticFuelType();
}

double WaterHeaterStratified::onCycleParasiticHeatFractiontoTank() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->onCycleParasiticHeatFractiontoTank();
}

bool WaterHeaterStratified::isOnCycleParasiticHeatFractiontoTankDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isOnCycleParasiticHeatFractiontoTankDefaulted();
}

double WaterHeaterStratified::onCycleParasiticHeight() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->onCycleParasiticHeight();
}

bool WaterHeaterStratified::isOnCycleParasiticHeightDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isOnCycleParasiticHeightDefaulted();
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

bool WaterHeaterStratified::isSkinLossFractiontoZoneDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isSkinLossFractiontoZoneDefaulted();
}

boost::optional<double> WaterHeaterStratified::offCycleFlueLossCoefficienttoAmbientTemperature() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->offCycleFlueLossCoefficienttoAmbientTemperature();
}

double WaterHeaterStratified::offCycleFlueLossFractiontoZone() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->offCycleFlueLossFractiontoZone();
}

bool WaterHeaterStratified::isOffCycleFlueLossFractiontoZoneDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isOffCycleFlueLossFractiontoZoneDefaulted();
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

bool WaterHeaterStratified::isUseSideEffectivenessDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isUseSideEffectivenessDefaulted();
}

double WaterHeaterStratified::useSideInletHeight() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->useSideInletHeight();
}

bool WaterHeaterStratified::isUseSideInletHeightDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isUseSideInletHeightDefaulted();
}

boost::optional<double> WaterHeaterStratified::useSideOutletHeight() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->useSideOutletHeight();
}

bool WaterHeaterStratified::isUseSideOutletHeightDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isUseSideOutletHeightDefaulted();
}

bool WaterHeaterStratified::isUseSideOutletHeightAutocalculated() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isUseSideOutletHeightAutocalculated();
}

double WaterHeaterStratified::sourceSideEffectiveness() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->sourceSideEffectiveness();
}

bool WaterHeaterStratified::isSourceSideEffectivenessDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isSourceSideEffectivenessDefaulted();
}

boost::optional<double> WaterHeaterStratified::sourceSideInletHeight() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->sourceSideInletHeight();
}

bool WaterHeaterStratified::isSourceSideInletHeightDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isSourceSideInletHeightDefaulted();
}

bool WaterHeaterStratified::isSourceSideInletHeightAutocalculated() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isSourceSideInletHeightAutocalculated();
}

double WaterHeaterStratified::sourceSideOutletHeight() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->sourceSideOutletHeight();
}

bool WaterHeaterStratified::isSourceSideOutletHeightDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isSourceSideOutletHeightDefaulted();
}

std::string WaterHeaterStratified::inletMode() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->inletMode();
}

bool WaterHeaterStratified::isInletModeDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isInletModeDefaulted();
}

boost::optional<double> WaterHeaterStratified::useSideDesignFlowRate() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->useSideDesignFlowRate();
}

bool WaterHeaterStratified::isUseSideDesignFlowRateDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isUseSideDesignFlowRateDefaulted();
}

bool WaterHeaterStratified::isUseSideDesignFlowRateAutosized() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isUseSideDesignFlowRateAutosized();
}

boost::optional<double> WaterHeaterStratified::sourceSideDesignFlowRate() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->sourceSideDesignFlowRate();
}

bool WaterHeaterStratified::isSourceSideDesignFlowRateDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isSourceSideDesignFlowRateDefaulted();
}

bool WaterHeaterStratified::isSourceSideDesignFlowRateAutosized() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isSourceSideDesignFlowRateAutosized();
}

double WaterHeaterStratified::indirectWaterHeatingRecoveryTime() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->indirectWaterHeatingRecoveryTime();
}

bool WaterHeaterStratified::isIndirectWaterHeatingRecoveryTimeDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isIndirectWaterHeatingRecoveryTimeDefaulted();
}

int WaterHeaterStratified::numberofNodes() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->numberofNodes();
}

bool WaterHeaterStratified::isNumberofNodesDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isNumberofNodesDefaulted();
}

double WaterHeaterStratified::additionalDestratificationConductivity() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->additionalDestratificationConductivity();
}

bool WaterHeaterStratified::isAdditionalDestratificationConductivityDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isAdditionalDestratificationConductivityDefaulted();
}

double WaterHeaterStratified::node1AdditionalLossCoefficient() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->node1AdditionalLossCoefficient();
}

bool WaterHeaterStratified::isNode1AdditionalLossCoefficientDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isNode1AdditionalLossCoefficientDefaulted();
}

double WaterHeaterStratified::node2AdditionalLossCoefficient() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->node2AdditionalLossCoefficient();
}

bool WaterHeaterStratified::isNode2AdditionalLossCoefficientDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isNode2AdditionalLossCoefficientDefaulted();
}

double WaterHeaterStratified::node3AdditionalLossCoefficient() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->node3AdditionalLossCoefficient();
}

bool WaterHeaterStratified::isNode3AdditionalLossCoefficientDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isNode3AdditionalLossCoefficientDefaulted();
}

double WaterHeaterStratified::node4AdditionalLossCoefficient() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->node4AdditionalLossCoefficient();
}

bool WaterHeaterStratified::isNode4AdditionalLossCoefficientDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isNode4AdditionalLossCoefficientDefaulted();
}

double WaterHeaterStratified::node5AdditionalLossCoefficient() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->node5AdditionalLossCoefficient();
}

bool WaterHeaterStratified::isNode5AdditionalLossCoefficientDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isNode5AdditionalLossCoefficientDefaulted();
}

double WaterHeaterStratified::node6AdditionalLossCoefficient() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->node6AdditionalLossCoefficient();
}

bool WaterHeaterStratified::isNode6AdditionalLossCoefficientDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isNode6AdditionalLossCoefficientDefaulted();
}

double WaterHeaterStratified::node7AdditionalLossCoefficient() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->node7AdditionalLossCoefficient();
}

bool WaterHeaterStratified::isNode7AdditionalLossCoefficientDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isNode7AdditionalLossCoefficientDefaulted();
}

double WaterHeaterStratified::node8AdditionalLossCoefficient() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->node8AdditionalLossCoefficient();
}

bool WaterHeaterStratified::isNode8AdditionalLossCoefficientDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isNode8AdditionalLossCoefficientDefaulted();
}

double WaterHeaterStratified::node9AdditionalLossCoefficient() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->node9AdditionalLossCoefficient();
}

bool WaterHeaterStratified::isNode9AdditionalLossCoefficientDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isNode9AdditionalLossCoefficientDefaulted();
}

double WaterHeaterStratified::node10AdditionalLossCoefficient() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->node10AdditionalLossCoefficient();
}

bool WaterHeaterStratified::isNode10AdditionalLossCoefficientDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isNode10AdditionalLossCoefficientDefaulted();
}

std::string WaterHeaterStratified::sourceSideFlowControlMode() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->sourceSideFlowControlMode();
}

bool WaterHeaterStratified::isSourceSideFlowControlModeDefaulted() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->isSourceSideFlowControlModeDefaulted();
}

boost::optional<Schedule> WaterHeaterStratified::indirectAlternateSetpointTemperatureSchedule() const {
  return getImpl<detail::WaterHeaterStratified_Impl>()->indirectAlternateSetpointTemperatureSchedule();
}

void WaterHeaterStratified::setEndUseSubcategory(std::string endUseSubcategory) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setEndUseSubcategory(endUseSubcategory);
}

void WaterHeaterStratified::resetEndUseSubcategory() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetEndUseSubcategory();
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

void WaterHeaterStratified::resetTankShape() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetTankShape();
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

void WaterHeaterStratified::resetMaximumTemperatureLimit() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetMaximumTemperatureLimit();
}

bool WaterHeaterStratified::setHeaterPriorityControl(std::string heaterPriorityControl) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setHeaterPriorityControl(heaterPriorityControl);
}

void WaterHeaterStratified::resetHeaterPriorityControl() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetHeaterPriorityControl();
}

bool WaterHeaterStratified::setHeater1SetpointTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setHeater1SetpointTemperatureSchedule(schedule);
}

bool WaterHeaterStratified::setHeater1DeadbandTemperatureDifference(double heater1DeadbandTemperatureDifference) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setHeater1DeadbandTemperatureDifference(heater1DeadbandTemperatureDifference);
}

void WaterHeaterStratified::resetHeater1DeadbandTemperatureDifference() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetHeater1DeadbandTemperatureDifference();
}

bool WaterHeaterStratified::setHeater1Capacity(double heater1Capacity) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setHeater1Capacity(heater1Capacity);
}

void WaterHeaterStratified::resetHeater1Capacity() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetHeater1Capacity();
}

void WaterHeaterStratified::autosizeHeater1Capacity() {
  getImpl<detail::WaterHeaterStratified_Impl>()->autosizeHeater1Capacity();
}

bool WaterHeaterStratified::setHeater1Height(double heater1Height) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setHeater1Height(heater1Height);
}

void WaterHeaterStratified::resetHeater1Height() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetHeater1Height();
}

bool WaterHeaterStratified::setHeater2SetpointTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setHeater2SetpointTemperatureSchedule(schedule);
}

bool WaterHeaterStratified::setHeater2DeadbandTemperatureDifference(double heater2DeadbandTemperatureDifference) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setHeater2DeadbandTemperatureDifference(heater2DeadbandTemperatureDifference);
}

void WaterHeaterStratified::resetHeater2DeadbandTemperatureDifference() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetHeater2DeadbandTemperatureDifference();
}

bool WaterHeaterStratified::setHeater2Capacity(double heater2Capacity) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setHeater2Capacity(heater2Capacity);
}

void WaterHeaterStratified::resetHeater2Capacity() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetHeater2Capacity();
}

bool WaterHeaterStratified::setHeater2Height(double heater2Height) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setHeater2Height(heater2Height);
}

void WaterHeaterStratified::resetHeater2Height() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetHeater2Height();
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

void WaterHeaterStratified::resetOffCycleParasiticFuelConsumptionRate() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetOffCycleParasiticFuelConsumptionRate();
}

bool WaterHeaterStratified::setOffCycleParasiticFuelType(std::string offCycleParasiticFuelType) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setOffCycleParasiticFuelType(offCycleParasiticFuelType);
}

void WaterHeaterStratified::resetOffCycleParasiticFuelType() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetOffCycleParasiticFuelType();
}

bool WaterHeaterStratified::setOffCycleParasiticHeatFractiontoTank(double offCycleParasiticHeatFractiontoTank) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setOffCycleParasiticHeatFractiontoTank(offCycleParasiticHeatFractiontoTank);
}

void WaterHeaterStratified::resetOffCycleParasiticHeatFractiontoTank() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetOffCycleParasiticHeatFractiontoTank();
}

bool WaterHeaterStratified::setOffCycleParasiticHeight(double offCycleParasiticHeight) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setOffCycleParasiticHeight(offCycleParasiticHeight);
}

void WaterHeaterStratified::resetOffCycleParasiticHeight() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetOffCycleParasiticHeight();
}

bool WaterHeaterStratified::setOnCycleParasiticFuelConsumptionRate(double onCycleParasiticFuelConsumptionRate) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setOnCycleParasiticFuelConsumptionRate(onCycleParasiticFuelConsumptionRate);
}

void WaterHeaterStratified::resetOnCycleParasiticFuelConsumptionRate() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetOnCycleParasiticFuelConsumptionRate();
}

bool WaterHeaterStratified::setOnCycleParasiticFuelType(std::string onCycleParasiticFuelType) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setOnCycleParasiticFuelType(onCycleParasiticFuelType);
}

void WaterHeaterStratified::resetOnCycleParasiticFuelType() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetOnCycleParasiticFuelType();
}

bool WaterHeaterStratified::setOnCycleParasiticHeatFractiontoTank(double onCycleParasiticHeatFractiontoTank) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setOnCycleParasiticHeatFractiontoTank(onCycleParasiticHeatFractiontoTank);
}

void WaterHeaterStratified::resetOnCycleParasiticHeatFractiontoTank() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetOnCycleParasiticHeatFractiontoTank();
}

bool WaterHeaterStratified::setOnCycleParasiticHeight(double onCycleParasiticHeight) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setOnCycleParasiticHeight(onCycleParasiticHeight);
}

void WaterHeaterStratified::resetOnCycleParasiticHeight() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetOnCycleParasiticHeight();
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

void WaterHeaterStratified::resetSkinLossFractiontoZone() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetSkinLossFractiontoZone();
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

void WaterHeaterStratified::resetOffCycleFlueLossFractiontoZone() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetOffCycleFlueLossFractiontoZone();
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

void WaterHeaterStratified::resetUseSideEffectiveness() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetUseSideEffectiveness();
}

bool WaterHeaterStratified::setUseSideInletHeight(double useSideInletHeight) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setUseSideInletHeight(useSideInletHeight);
}

void WaterHeaterStratified::resetUseSideInletHeight() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetUseSideInletHeight();
}

bool WaterHeaterStratified::setUseSideOutletHeight(double useSideOutletHeight) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setUseSideOutletHeight(useSideOutletHeight);
}

void WaterHeaterStratified::resetUseSideOutletHeight() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetUseSideOutletHeight();
}

void WaterHeaterStratified::autocalculateUseSideOutletHeight() {
  getImpl<detail::WaterHeaterStratified_Impl>()->autocalculateUseSideOutletHeight();
}

bool WaterHeaterStratified::setSourceSideEffectiveness(double sourceSideEffectiveness) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setSourceSideEffectiveness(sourceSideEffectiveness);
}

void WaterHeaterStratified::resetSourceSideEffectiveness() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetSourceSideEffectiveness();
}

bool WaterHeaterStratified::setSourceSideInletHeight(double sourceSideInletHeight) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setSourceSideInletHeight(sourceSideInletHeight);
}

void WaterHeaterStratified::resetSourceSideInletHeight() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetSourceSideInletHeight();
}

void WaterHeaterStratified::autocalculateSourceSideInletHeight() {
  getImpl<detail::WaterHeaterStratified_Impl>()->autocalculateSourceSideInletHeight();
}

bool WaterHeaterStratified::setSourceSideOutletHeight(double sourceSideOutletHeight) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setSourceSideOutletHeight(sourceSideOutletHeight);
}

void WaterHeaterStratified::resetSourceSideOutletHeight() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetSourceSideOutletHeight();
}

bool WaterHeaterStratified::setInletMode(std::string inletMode) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setInletMode(inletMode);
}

void WaterHeaterStratified::resetInletMode() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetInletMode();
}

bool WaterHeaterStratified::setUseSideDesignFlowRate(double useSideDesignFlowRate) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setUseSideDesignFlowRate(useSideDesignFlowRate);
}

void WaterHeaterStratified::resetUseSideDesignFlowRate() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetUseSideDesignFlowRate();
}

void WaterHeaterStratified::autosizeUseSideDesignFlowRate() {
  getImpl<detail::WaterHeaterStratified_Impl>()->autosizeUseSideDesignFlowRate();
}

bool WaterHeaterStratified::setSourceSideDesignFlowRate(double sourceSideDesignFlowRate) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setSourceSideDesignFlowRate(sourceSideDesignFlowRate);
}

void WaterHeaterStratified::resetSourceSideDesignFlowRate() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetSourceSideDesignFlowRate();
}

void WaterHeaterStratified::autosizeSourceSideDesignFlowRate() {
  getImpl<detail::WaterHeaterStratified_Impl>()->autosizeSourceSideDesignFlowRate();
}

bool WaterHeaterStratified::setIndirectWaterHeatingRecoveryTime(double indirectWaterHeatingRecoveryTime) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setIndirectWaterHeatingRecoveryTime(indirectWaterHeatingRecoveryTime);
}

void WaterHeaterStratified::resetIndirectWaterHeatingRecoveryTime() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetIndirectWaterHeatingRecoveryTime();
}

bool WaterHeaterStratified::setNumberofNodes(int numberofNodes) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setNumberofNodes(numberofNodes);
}

void WaterHeaterStratified::resetNumberofNodes() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetNumberofNodes();
}

bool WaterHeaterStratified::setAdditionalDestratificationConductivity(double additionalDestratificationConductivity) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setAdditionalDestratificationConductivity(additionalDestratificationConductivity);
}

void WaterHeaterStratified::resetAdditionalDestratificationConductivity() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetAdditionalDestratificationConductivity();
}

void WaterHeaterStratified::setNode1AdditionalLossCoefficient(double node1AdditionalLossCoefficient) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setNode1AdditionalLossCoefficient(node1AdditionalLossCoefficient);
}

void WaterHeaterStratified::resetNode1AdditionalLossCoefficient() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetNode1AdditionalLossCoefficient();
}

void WaterHeaterStratified::setNode2AdditionalLossCoefficient(double node2AdditionalLossCoefficient) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setNode2AdditionalLossCoefficient(node2AdditionalLossCoefficient);
}

void WaterHeaterStratified::resetNode2AdditionalLossCoefficient() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetNode2AdditionalLossCoefficient();
}

void WaterHeaterStratified::setNode3AdditionalLossCoefficient(double node3AdditionalLossCoefficient) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setNode3AdditionalLossCoefficient(node3AdditionalLossCoefficient);
}

void WaterHeaterStratified::resetNode3AdditionalLossCoefficient() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetNode3AdditionalLossCoefficient();
}

void WaterHeaterStratified::setNode4AdditionalLossCoefficient(double node4AdditionalLossCoefficient) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setNode4AdditionalLossCoefficient(node4AdditionalLossCoefficient);
}

void WaterHeaterStratified::resetNode4AdditionalLossCoefficient() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetNode4AdditionalLossCoefficient();
}

void WaterHeaterStratified::setNode5AdditionalLossCoefficient(double node5AdditionalLossCoefficient) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setNode5AdditionalLossCoefficient(node5AdditionalLossCoefficient);
}

void WaterHeaterStratified::resetNode5AdditionalLossCoefficient() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetNode5AdditionalLossCoefficient();
}

void WaterHeaterStratified::setNode6AdditionalLossCoefficient(double node6AdditionalLossCoefficient) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setNode6AdditionalLossCoefficient(node6AdditionalLossCoefficient);
}

void WaterHeaterStratified::resetNode6AdditionalLossCoefficient() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetNode6AdditionalLossCoefficient();
}

void WaterHeaterStratified::setNode7AdditionalLossCoefficient(double node7AdditionalLossCoefficient) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setNode7AdditionalLossCoefficient(node7AdditionalLossCoefficient);
}

void WaterHeaterStratified::resetNode7AdditionalLossCoefficient() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetNode7AdditionalLossCoefficient();
}

void WaterHeaterStratified::setNode8AdditionalLossCoefficient(double node8AdditionalLossCoefficient) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setNode8AdditionalLossCoefficient(node8AdditionalLossCoefficient);
}

void WaterHeaterStratified::resetNode8AdditionalLossCoefficient() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetNode8AdditionalLossCoefficient();
}

void WaterHeaterStratified::setNode9AdditionalLossCoefficient(double node9AdditionalLossCoefficient) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setNode9AdditionalLossCoefficient(node9AdditionalLossCoefficient);
}

void WaterHeaterStratified::resetNode9AdditionalLossCoefficient() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetNode9AdditionalLossCoefficient();
}

void WaterHeaterStratified::setNode10AdditionalLossCoefficient(double node10AdditionalLossCoefficient) {
  getImpl<detail::WaterHeaterStratified_Impl>()->setNode10AdditionalLossCoefficient(node10AdditionalLossCoefficient);
}

void WaterHeaterStratified::resetNode10AdditionalLossCoefficient() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetNode10AdditionalLossCoefficient();
}

bool WaterHeaterStratified::setSourceSideFlowControlMode(std::string sourceSideFlowControlMode) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setSourceSideFlowControlMode(sourceSideFlowControlMode);
}

void WaterHeaterStratified::resetSourceSideFlowControlMode() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetSourceSideFlowControlMode();
}

bool WaterHeaterStratified::setIndirectAlternateSetpointTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::WaterHeaterStratified_Impl>()->setIndirectAlternateSetpointTemperatureSchedule(schedule);
}

void WaterHeaterStratified::resetIndirectAlternateSetpointTemperatureSchedule() {
  getImpl<detail::WaterHeaterStratified_Impl>()->resetIndirectAlternateSetpointTemperatureSchedule();
}

/// @cond
WaterHeaterStratified::WaterHeaterStratified(std::shared_ptr<detail::WaterHeaterStratified_Impl> impl)
  : WaterToWaterComponent(impl)
{}
/// @endcond

} // model
} // openstudio

