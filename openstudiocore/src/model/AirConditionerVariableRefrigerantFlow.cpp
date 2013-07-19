/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/AirConditionerVariableRefrigerantFlow.hpp>
#include <model/AirConditionerVariableRefrigerantFlow_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/CurveBiquadratic.hpp>
#include <model/CurveBiquadratic_Impl.hpp>
#include <model/CurveLinear.hpp>
#include <model/CurveLinear_Impl.hpp>
#include <model/CurveCubic.hpp>
#include <model/CurveCubic_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Connection.hpp>
#include <model/Connection_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_AirConditioner_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/units/Unit.hpp>
#include <utilities/core/Assert.hpp>

namespace openstudio {

namespace model {

namespace detail {

  AirConditionerVariableRefrigerantFlow_Impl::AirConditionerVariableRefrigerantFlow_Impl(const IdfObject& idfObject,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    BOOST_ASSERT(idfObject.iddObject().type() == AirConditionerVariableRefrigerantFlow::iddObjectType());
  }

  AirConditionerVariableRefrigerantFlow_Impl::AirConditionerVariableRefrigerantFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    BOOST_ASSERT(other.iddObject().type() == AirConditionerVariableRefrigerantFlow::iddObjectType());
  }

  AirConditionerVariableRefrigerantFlow_Impl::AirConditionerVariableRefrigerantFlow_Impl(const AirConditionerVariableRefrigerantFlow_Impl& other,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirConditionerVariableRefrigerantFlow_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirConditionerVariableRefrigerantFlow_Impl::iddObjectType() const {
    return AirConditionerVariableRefrigerantFlow::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AirConditionerVariableRefrigerantFlow_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AirConditioner_VariableRefrigerantFlowFields::AvailabilitySchedule) != e)
    {
      result.push_back(ScheduleTypeKey("AirConditionerVariableRefrigerantFlow","Availability Schedule"));
    }
    if (std::find(b,e,OS_AirConditioner_VariableRefrigerantFlowFields::ThermostatPrioritySchedule) != e)
    {
      result.push_back(ScheduleTypeKey("AirConditionerVariableRefrigerantFlow","Thermostat Priority Schedule"));
    }
    if (std::find(b,e,OS_AirConditioner_VariableRefrigerantFlowFields::BasinHeaterOperatingSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("AirConditionerVariableRefrigerantFlow","Basin Heater Operating Schedule"));
    }
    return result;
  }

  Schedule AirConditionerVariableRefrigerantFlow_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
    }
    return value.get();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlow_Impl::ratedTotalCoolingCapacity() const {
    return getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatedTotalCoolingCapacity,true);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::isRatedTotalCoolingCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::RatedTotalCoolingCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double AirConditionerVariableRefrigerantFlow_Impl::ratedCoolingCOP() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatedCoolingCOP,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::minimumOutdoorTemperatureinCoolingMode() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumOutdoorTemperatureinCoolingMode,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::maximumOutdoorTemperatureinCoolingMode() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorTemperatureinCoolingMode,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow_Impl::coolingCapacityRatioModifierFunctionofLowTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofLowTemperatureCurve);
  }

  boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow_Impl::coolingCapacityRatioBoundaryCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveLinear>(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioBoundaryCurve);
  }

  boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow_Impl::coolingCapacityRatioModifierFunctionofHighTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofHighTemperatureCurve);
  }

  boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow_Impl::coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve);
  }

  boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow_Impl::coolingEnergyInputRatioBoundaryCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveLinear>(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioBoundaryCurve);
  }

  boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow_Impl::coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve);
  }

  boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow_Impl::coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveLinear>(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve);
  }

  boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow_Impl::coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveLinear>(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve);
  }

  boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow_Impl::coolingCombinationRatioCorrectionFactorCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveLinear>(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCombinationRatioCorrectionFactorCurve);
  }

  boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow_Impl::coolingPartLoadFractionCorrelationCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveLinear>(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingPartLoadFractionCorrelationCurve);
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlow_Impl::ratedTotalHeatingCapacity() const {
    return getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatedTotalHeatingCapacity,true);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::isRatedTotalHeatingCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::RatedTotalHeatingCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double AirConditionerVariableRefrigerantFlow_Impl::ratedTotalHeatingCapacitySizingRatio() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatedTotalHeatingCapacitySizingRatio,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::ratedHeatingCOP() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatedHeatingCOP,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::minimumOutdoorTemperatureinHeatingMode() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumOutdoorTemperatureinHeatingMode,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::maximumOutdoorTemperatureinHeatingMode() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorTemperatureinHeatingMode,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow_Impl::heatingCapacityRatioModifierFunctionofLowTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofLowTemperatureCurve);
  }

  boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow_Impl::heatingCapacityRatioBoundaryCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveLinear>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioBoundaryCurve);
  }

  boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow_Impl::heatingCapacityRatioModifierFunctionofHighTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofHighTemperatureCurve);
  }

  boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow_Impl::heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve);
  }

  boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow_Impl::heatingEnergyInputRatioBoundaryCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveLinear>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioBoundaryCurve);
  }

  boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow_Impl::heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve);
  }

  std::string AirConditionerVariableRefrigerantFlow_Impl::heatingPerformanceCurveOutdoorTemperatureType() const {
    boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingPerformanceCurveOutdoorTemperatureType,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  boost::optional<CurveCubic> AirConditionerVariableRefrigerantFlow_Impl::heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveCubic>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve);
  }

  boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow_Impl::heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveLinear>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve);
  }

  boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow_Impl::heatingCombinationRatioCorrectionFactorCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveLinear>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCombinationRatioCorrectionFactorCurve);
  }

  boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow_Impl::heatingPartLoadFractionCorrelationCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveLinear>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingPartLoadFractionCorrelationCurve);
  }

  double AirConditionerVariableRefrigerantFlow_Impl::minimumHeatPumpPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumHeatPumpPartLoadRatio,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  boost::optional<ThermalZone> AirConditionerVariableRefrigerantFlow_Impl::zoneforMasterThermostatLocation() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_AirConditioner_VariableRefrigerantFlowFields::ZoneNameforMasterThermostatLocation);
  }

  std::string AirConditionerVariableRefrigerantFlow_Impl::masterThermostatPriorityControlType() const {
    boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::MasterThermostatPriorityControlType,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> AirConditionerVariableRefrigerantFlow_Impl::thermostatPrioritySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirConditioner_VariableRefrigerantFlowFields::ThermostatPrioritySchedule);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::heatPumpWasteHeatRecovery() const {
    boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatPumpWasteHeatRecovery,true);
    BOOST_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  double AirConditionerVariableRefrigerantFlow_Impl::equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::verticalHeightusedforPipingCorrectionFactor() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::VerticalHeightusedforPipingCorrectionFactor,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow_Impl::pipingCorrectionFactorforLengthinCoolingModeCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveLinear>(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforLengthinCoolingModeCurve);
  }

  double AirConditionerVariableRefrigerantFlow_Impl::pipingCorrectionFactorforHeightinCoolingModeCoefficient() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforHeightinCoolingModeCoefficient,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow_Impl::pipingCorrectionFactorforLengthinHeatingModeCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveLinear>(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforLengthinHeatingModeCurve);
  }

  double AirConditionerVariableRefrigerantFlow_Impl::pipingCorrectionFactorforHeightinHeatingModeCoefficient() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforHeightinHeatingModeCoefficient,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::crankcaseHeaterPowerperCompressor() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::CrankcaseHeaterPowerperCompressor,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  int AirConditionerVariableRefrigerantFlow_Impl::numberofCompressors() const {
    boost::optional<int> value = getInt(OS_AirConditioner_VariableRefrigerantFlowFields::NumberofCompressors,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::ratioofCompressorSizetoTotalCompressorCapacity() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatioofCompressorSizetoTotalCompressorCapacity,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::maximumOutdoorDrybulbTemperatureforCrankcaseHeater() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorDrybulbTemperatureforCrankcaseHeater,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  std::string AirConditionerVariableRefrigerantFlow_Impl::defrostStrategy() const {
    boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostStrategy,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  std::string AirConditionerVariableRefrigerantFlow_Impl::defrostControl() const {
    boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostControl,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow_Impl::defrostEnergyInputRatioModifierFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurve);
  }

  double AirConditionerVariableRefrigerantFlow_Impl::defrostTimePeriodFraction() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostTimePeriodFraction,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlow_Impl::resistiveDefrostHeaterCapacity() const {
    return getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::ResistiveDefrostHeaterCapacity,true);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::isResistiveDefrostHeaterCapacityAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::ResistiveDefrostHeaterCapacity, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double AirConditionerVariableRefrigerantFlow_Impl::maximumOutdoorDrybulbTemperatureforDefrostOperation() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlow_Impl::waterCondenserVolumeFlowRate() const {
    return getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::WaterCondenserVolumeFlowRate,true);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::isWaterCondenserVolumeFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::WaterCondenserVolumeFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double AirConditionerVariableRefrigerantFlow_Impl::evaporativeCondenserEffectiveness() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserEffectiveness,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlow_Impl::evaporativeCondenserAirFlowRate() const {
    return getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserAirFlowRate,true);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::isEvaporativeCondenserAirFlowRateAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlow_Impl::evaporativeCondenserPumpRatedPowerConsumption() const {
    return getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserPumpRatedPowerConsumption,true);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserPumpRatedPowerConsumption, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double AirConditionerVariableRefrigerantFlow_Impl::basinHeaterCapacity() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::BasinHeaterCapacity,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::basinHeaterSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::BasinHeaterSetpointTemperature,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> AirConditionerVariableRefrigerantFlow_Impl::basinHeaterOperatingSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirConditioner_VariableRefrigerantFlowFields::BasinHeaterOperatingSchedule);
  }

  std::string AirConditionerVariableRefrigerantFlow_Impl::fuelType() const {
    boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::FuelType,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::minimumOutdoorTemperatureinHeatRecoveryMode() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumOutdoorTemperatureinHeatRecoveryMode,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::maximumOutdoorTemperatureinHeatRecoveryMode() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorTemperatureinHeatRecoveryMode,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow_Impl::heatRecoveryCoolingCapacityModifierCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityModifierCurve);
  }

  double AirConditionerVariableRefrigerantFlow_Impl::initialHeatRecoveryCoolingCapacityFraction() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingCapacityFraction,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::heatRecoveryCoolingCapacityTimeConstant() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityTimeConstant,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow_Impl::heatRecoveryCoolingEnergyModifierCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyModifierCurve);
  }

  double AirConditionerVariableRefrigerantFlow_Impl::initialHeatRecoveryCoolingEnergyFraction() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingEnergyFraction,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::heatRecoveryCoolingEnergyTimeConstant() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyTimeConstant,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow_Impl::heatRecoveryHeatingCapacityModifierCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityModifierCurve);
  }

  double AirConditionerVariableRefrigerantFlow_Impl::initialHeatRecoveryHeatingCapacityFraction() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryHeatingCapacityFraction,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::heatRecoveryHeatingCapacityTimeConstant() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityTimeConstant,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow_Impl::heatRecoveryHeatingEnergyModifierCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<CurveBiquadratic>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyModifierCurve);
  }

  double AirConditionerVariableRefrigerantFlow_Impl::initialHeatRecoveryHeatingEnergyFraction() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryHeatingEnergyFraction,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::heatRecoveryHeatingEnergyTimeConstant() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyTimeConstant,true);
    BOOST_ASSERT(value);
    return value.get();
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirConditioner_VariableRefrigerantFlowFields::AvailabilitySchedule,
                              "AirConditionerVariableRefrigerantFlow",
                              "Availability Schedule",
                              schedule);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setRatedTotalCoolingCapacity(boost::optional<double> ratedTotalCoolingCapacity) {
    bool result(false);
    if (ratedTotalCoolingCapacity) {
      result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatedTotalCoolingCapacity, ratedTotalCoolingCapacity.get());
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::autosizeRatedTotalCoolingCapacity() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::RatedTotalCoolingCapacity, "autosize");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setRatedCoolingCOP(double ratedCoolingCOP) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatedCoolingCOP, ratedCoolingCOP);
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setMinimumOutdoorTemperatureinCoolingMode(double minimumOutdoorTemperatureinCoolingMode) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumOutdoorTemperatureinCoolingMode, minimumOutdoorTemperatureinCoolingMode);
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setMaximumOutdoorTemperatureinCoolingMode(double maximumOutdoorTemperatureinCoolingMode) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorTemperatureinCoolingMode, maximumOutdoorTemperatureinCoolingMode);
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setCoolingCapacityRatioModifierFunctionofLowTemperatureCurve(const boost::optional<CurveBiquadratic>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofLowTemperatureCurve, curve.get().handle());
    }
    else {
      resetCoolingCapacityRatioModifierFunctionofLowTemperatureCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetCoolingCapacityRatioModifierFunctionofLowTemperatureCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofLowTemperatureCurve, "");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setCoolingCapacityRatioBoundaryCurve(const boost::optional<CurveLinear>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioBoundaryCurve, curve.get().handle());
    }
    else {
      resetCoolingCapacityRatioBoundaryCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetCoolingCapacityRatioBoundaryCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioBoundaryCurve, "");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setCoolingCapacityRatioModifierFunctionofHighTemperatureCurve(const boost::optional<CurveBiquadratic>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofHighTemperatureCurve, curve.get().handle());
    }
    else {
      resetCoolingCapacityRatioModifierFunctionofHighTemperatureCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetCoolingCapacityRatioModifierFunctionofHighTemperatureCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofHighTemperatureCurve, "");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve(const boost::optional<CurveBiquadratic>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve, curve.get().handle());
    }
    else {
      resetCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve, "");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setCoolingEnergyInputRatioBoundaryCurve(const boost::optional<CurveLinear>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioBoundaryCurve, curve.get().handle());
    }
    else {
      resetCoolingEnergyInputRatioBoundaryCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetCoolingEnergyInputRatioBoundaryCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioBoundaryCurve, "");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve(const boost::optional<CurveBiquadratic>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve, curve.get().handle());
    }
    else {
      resetCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve, "");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(const boost::optional<CurveLinear>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve, curve.get().handle());
    }
    else {
      resetCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve, "");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(const boost::optional<CurveLinear>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve, curve.get().handle());
    }
    else {
      resetCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve, "");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setCoolingCombinationRatioCorrectionFactorCurve(const boost::optional<CurveLinear>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCombinationRatioCorrectionFactorCurve, curve.get().handle());
    }
    else {
      resetCoolingCombinationRatioCorrectionFactorCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetCoolingCombinationRatioCorrectionFactorCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCombinationRatioCorrectionFactorCurve, "");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setCoolingPartLoadFractionCorrelationCurve(const boost::optional<CurveLinear>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingPartLoadFractionCorrelationCurve, curve.get().handle());
    }
    else {
      resetCoolingPartLoadFractionCorrelationCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetCoolingPartLoadFractionCorrelationCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingPartLoadFractionCorrelationCurve, "");
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setRatedTotalHeatingCapacity(boost::optional<double> ratedTotalHeatingCapacity) {
    bool result(false);
    if (ratedTotalHeatingCapacity) {
      result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatedTotalHeatingCapacity, ratedTotalHeatingCapacity.get());
    }
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::autosizeRatedTotalHeatingCapacity() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::RatedTotalHeatingCapacity, "autosize");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setRatedTotalHeatingCapacitySizingRatio(double ratedTotalHeatingCapacitySizingRatio) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatedTotalHeatingCapacitySizingRatio, ratedTotalHeatingCapacitySizingRatio);
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setRatedHeatingCOP(double ratedHeatingCOP) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatedHeatingCOP, ratedHeatingCOP);
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setMinimumOutdoorTemperatureinHeatingMode(double minimumOutdoorTemperatureinHeatingMode) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumOutdoorTemperatureinHeatingMode, minimumOutdoorTemperatureinHeatingMode);
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setMaximumOutdoorTemperatureinHeatingMode(double maximumOutdoorTemperatureinHeatingMode) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorTemperatureinHeatingMode, maximumOutdoorTemperatureinHeatingMode);
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatingCapacityRatioModifierFunctionofLowTemperatureCurve(const boost::optional<CurveBiquadratic>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofLowTemperatureCurve, curve.get().handle());
    }
    else {
      resetHeatingCapacityRatioModifierFunctionofLowTemperatureCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetHeatingCapacityRatioModifierFunctionofLowTemperatureCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofLowTemperatureCurve, "");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatingCapacityRatioBoundaryCurve(const boost::optional<CurveLinear>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioBoundaryCurve, curve.get().handle());
    }
    else {
      resetHeatingCapacityRatioBoundaryCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetHeatingCapacityRatioBoundaryCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioBoundaryCurve, "");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatingCapacityRatioModifierFunctionofHighTemperatureCurve(const boost::optional<CurveBiquadratic>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofHighTemperatureCurve, curve.get().handle());
    }
    else {
      resetHeatingCapacityRatioModifierFunctionofHighTemperatureCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetHeatingCapacityRatioModifierFunctionofHighTemperatureCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofHighTemperatureCurve, "");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve(const boost::optional<CurveBiquadratic>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve, curve.get().handle());
    }
    else {
      resetHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve, "");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatingEnergyInputRatioBoundaryCurve(const boost::optional<CurveLinear>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioBoundaryCurve, curve.get().handle());
    }
    else {
      resetHeatingEnergyInputRatioBoundaryCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetHeatingEnergyInputRatioBoundaryCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioBoundaryCurve, "");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve(const boost::optional<CurveBiquadratic>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve, curve.get().handle());
    }
    else {
      resetHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve, "");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatingPerformanceCurveOutdoorTemperatureType(std::string heatingPerformanceCurveOutdoorTemperatureType) {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingPerformanceCurveOutdoorTemperatureType, heatingPerformanceCurveOutdoorTemperatureType);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(const boost::optional<CurveCubic>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve, curve.get().handle());
    }
    else {
      resetHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve, "");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(const boost::optional<CurveLinear>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve, curve.get().handle());
    }
    else {
      resetHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve, "");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatingCombinationRatioCorrectionFactorCurve(const boost::optional<CurveLinear>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCombinationRatioCorrectionFactorCurve, curve.get().handle());
    }
    else {
      resetHeatingCombinationRatioCorrectionFactorCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetHeatingCombinationRatioCorrectionFactorCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCombinationRatioCorrectionFactorCurve, "");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatingPartLoadFractionCorrelationCurve(const boost::optional<CurveLinear>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingPartLoadFractionCorrelationCurve, curve.get().handle());
    }
    else {
      resetHeatingPartLoadFractionCorrelationCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetHeatingPartLoadFractionCorrelationCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingPartLoadFractionCorrelationCurve, "");
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setMinimumHeatPumpPartLoadRatio(double minimumHeatPumpPartLoadRatio) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumHeatPumpPartLoadRatio, minimumHeatPumpPartLoadRatio);
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setZoneforMasterThermostatLocation(const boost::optional<ThermalZone>& zone) {
    bool result(false);
    if (zone) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::ZoneNameforMasterThermostatLocation, zone.get().handle());
    }
    else {
      resetZoneforMasterThermostatLocation();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetZoneforMasterThermostatLocation() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::ZoneNameforMasterThermostatLocation, "");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setMasterThermostatPriorityControlType(std::string masterThermostatPriorityControlType) {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::MasterThermostatPriorityControlType, masterThermostatPriorityControlType);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setThermostatPrioritySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirConditioner_VariableRefrigerantFlowFields::ThermostatPrioritySchedule,
                              "AirConditionerVariableRefrigerantFlow",
                              "Thermostat Priority Schedule",
                              schedule);
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetThermostatPrioritySchedule() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::ThermostatPrioritySchedule, "");
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setHeatPumpWasteHeatRecovery(bool heatPumpWasteHeatRecovery) {
    if (heatPumpWasteHeatRecovery) {
      setBooleanFieldValue(OS_AirConditioner_VariableRefrigerantFlowFields::HeatPumpWasteHeatRecovery, "Yes");
    } else {
      setBooleanFieldValue(OS_AirConditioner_VariableRefrigerantFlowFields::HeatPumpWasteHeatRecovery, "No");
    }
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setEquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode(double equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode, equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode);
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setVerticalHeightusedforPipingCorrectionFactor(double verticalHeightusedforPipingCorrectionFactor) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::VerticalHeightusedforPipingCorrectionFactor, verticalHeightusedforPipingCorrectionFactor);
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setPipingCorrectionFactorforLengthinCoolingModeCurve(const boost::optional<CurveLinear>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforLengthinCoolingModeCurve, curve.get().handle());
    }
    else {
      resetPipingCorrectionFactorforLengthinCoolingModeCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetPipingCorrectionFactorforLengthinCoolingModeCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforLengthinCoolingModeCurve, "");
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setPipingCorrectionFactorforHeightinCoolingModeCoefficient(double pipingCorrectionFactorforHeightinCoolingModeCoefficient) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforHeightinCoolingModeCoefficient, pipingCorrectionFactorforHeightinCoolingModeCoefficient);
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setEquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode(double equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode, equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode);
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setPipingCorrectionFactorforLengthinHeatingModeCurve(const boost::optional<CurveLinear>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforLengthinHeatingModeCurve, curve.get().handle());
    }
    else {
      resetPipingCorrectionFactorforLengthinHeatingModeCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetPipingCorrectionFactorforLengthinHeatingModeCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforLengthinHeatingModeCurve, "");
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setPipingCorrectionFactorforHeightinHeatingModeCoefficient(double pipingCorrectionFactorforHeightinHeatingModeCoefficient) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforHeightinHeatingModeCoefficient, pipingCorrectionFactorforHeightinHeatingModeCoefficient);
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setCrankcaseHeaterPowerperCompressor(double crankcaseHeaterPowerperCompressor) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::CrankcaseHeaterPowerperCompressor, crankcaseHeaterPowerperCompressor);
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setNumberofCompressors(int numberofCompressors) {
    bool result = setInt(OS_AirConditioner_VariableRefrigerantFlowFields::NumberofCompressors, numberofCompressors);
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setRatioofCompressorSizetoTotalCompressorCapacity(double ratioofCompressorSizetoTotalCompressorCapacity) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatioofCompressorSizetoTotalCompressorCapacity, ratioofCompressorSizetoTotalCompressorCapacity);
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setMaximumOutdoorDrybulbTemperatureforCrankcaseHeater(double maximumOutdoorDrybulbTemperatureforCrankcaseHeater) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorDrybulbTemperatureforCrankcaseHeater, maximumOutdoorDrybulbTemperatureforCrankcaseHeater);
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setDefrostStrategy(std::string defrostStrategy) {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostStrategy, defrostStrategy);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setDefrostControl(std::string defrostControl) {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostControl, defrostControl);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(const boost::optional<CurveBiquadratic>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurve, curve.get().handle());
    }
    else {
      resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurve, "");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setDefrostTimePeriodFraction(double defrostTimePeriodFraction) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostTimePeriodFraction, defrostTimePeriodFraction);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setResistiveDefrostHeaterCapacity(boost::optional<double> resistiveDefrostHeaterCapacity) {
    bool result(false);
    if (resistiveDefrostHeaterCapacity) {
      result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::ResistiveDefrostHeaterCapacity, resistiveDefrostHeaterCapacity.get());
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::autosizeResistiveDefrostHeaterCapacity() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::ResistiveDefrostHeaterCapacity, "autosize");
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setMaximumOutdoorDrybulbTemperatureforDefrostOperation(double maximumOutdoorDrybulbTemperatureforDefrostOperation) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation, maximumOutdoorDrybulbTemperatureforDefrostOperation);
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setWaterCondenserVolumeFlowRate(boost::optional<double> waterCondenserVolumeFlowRate) {
    bool result(false);
    if (waterCondenserVolumeFlowRate) {
      result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::WaterCondenserVolumeFlowRate, waterCondenserVolumeFlowRate.get());
    }
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::autosizeWaterCondenserVolumeFlowRate() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::WaterCondenserVolumeFlowRate, "autosize");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserEffectiveness, evaporativeCondenserEffectiveness);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setEvaporativeCondenserAirFlowRate(boost::optional<double> evaporativeCondenserAirFlowRate) {
    bool result(false);
    if (evaporativeCondenserAirFlowRate) {
      result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserAirFlowRate, evaporativeCondenserAirFlowRate.get());
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::autosizeEvaporativeCondenserAirFlowRate() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserAirFlowRate, "autosize");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setEvaporativeCondenserPumpRatedPowerConsumption(boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption) {
    bool result(false);
    if (evaporativeCondenserPumpRatedPowerConsumption) {
      result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserPumpRatedPowerConsumption, evaporativeCondenserPumpRatedPowerConsumption.get());
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::autosizeEvaporativeCondenserPumpRatedPowerConsumption() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserPumpRatedPowerConsumption, "autosize");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setBasinHeaterCapacity(double basinHeaterCapacity) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::BasinHeaterCapacity, basinHeaterCapacity);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::BasinHeaterSetpointTemperature, basinHeaterSetpointTemperature);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setBasinHeaterOperatingSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirConditioner_VariableRefrigerantFlowFields::BasinHeaterOperatingSchedule,
                              "AirConditionerVariableRefrigerantFlow",
                              "Basin Heater Operating Schedule",
                              schedule);
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetBasinHeaterOperatingSchedule() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::BasinHeaterOperatingSchedule, "");
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setFuelType(std::string fuelType) {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::FuelType, fuelType);
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setMinimumOutdoorTemperatureinHeatRecoveryMode(double minimumOutdoorTemperatureinHeatRecoveryMode) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumOutdoorTemperatureinHeatRecoveryMode, minimumOutdoorTemperatureinHeatRecoveryMode);
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setMaximumOutdoorTemperatureinHeatRecoveryMode(double maximumOutdoorTemperatureinHeatRecoveryMode) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorTemperatureinHeatRecoveryMode, maximumOutdoorTemperatureinHeatRecoveryMode);
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatRecoveryCoolingCapacityModifierCurve(const boost::optional<CurveBiquadratic>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityModifierCurve, curve.get().handle());
    }
    else {
      resetHeatRecoveryCoolingCapacityModifierCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetHeatRecoveryCoolingCapacityModifierCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityModifierCurve, "");
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setInitialHeatRecoveryCoolingCapacityFraction(double initialHeatRecoveryCoolingCapacityFraction) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingCapacityFraction, initialHeatRecoveryCoolingCapacityFraction);
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setHeatRecoveryCoolingCapacityTimeConstant(double heatRecoveryCoolingCapacityTimeConstant) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityTimeConstant, heatRecoveryCoolingCapacityTimeConstant);
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatRecoveryCoolingEnergyModifierCurve(const boost::optional<CurveBiquadratic>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyModifierCurve, curve.get().handle());
    }
    else {
      resetHeatRecoveryCoolingEnergyModifierCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetHeatRecoveryCoolingEnergyModifierCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyModifierCurve, "");
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setInitialHeatRecoveryCoolingEnergyFraction(double initialHeatRecoveryCoolingEnergyFraction) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingEnergyFraction, initialHeatRecoveryCoolingEnergyFraction);
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setHeatRecoveryCoolingEnergyTimeConstant(double heatRecoveryCoolingEnergyTimeConstant) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyTimeConstant, heatRecoveryCoolingEnergyTimeConstant);
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatRecoveryHeatingCapacityModifierCurve(const boost::optional<CurveBiquadratic>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityModifierCurve, curve.get().handle());
    }
    else {
      resetHeatRecoveryHeatingCapacityModifierCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetHeatRecoveryHeatingCapacityModifierCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityModifierCurve, "");
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setInitialHeatRecoveryHeatingCapacityFraction(double initialHeatRecoveryHeatingCapacityFraction) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryHeatingCapacityFraction, initialHeatRecoveryHeatingCapacityFraction);
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setHeatRecoveryHeatingCapacityTimeConstant(double heatRecoveryHeatingCapacityTimeConstant) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityTimeConstant, heatRecoveryHeatingCapacityTimeConstant);
    BOOST_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatRecoveryHeatingEnergyModifierCurve(const boost::optional<CurveBiquadratic>& curve) {
    bool result(false);
    if (curve) {
      result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyModifierCurve, curve.get().handle());
    }
    else {
      resetHeatRecoveryHeatingEnergyModifierCurve();
      result = true;
    }
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::resetHeatRecoveryHeatingEnergyModifierCurve() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyModifierCurve, "");
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setInitialHeatRecoveryHeatingEnergyFraction(double initialHeatRecoveryHeatingEnergyFraction) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryHeatingEnergyFraction, initialHeatRecoveryHeatingEnergyFraction);
    BOOST_ASSERT(result);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::setHeatRecoveryHeatingEnergyTimeConstant(double heatRecoveryHeatingEnergyTimeConstant) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyTimeConstant, heatRecoveryHeatingEnergyTimeConstant);
    BOOST_ASSERT(result);
  }

  boost::optional<Schedule> AirConditionerVariableRefrigerantFlow_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirConditioner_VariableRefrigerantFlowFields::AvailabilitySchedule);
  }

  unsigned AirConditionerVariableRefrigerantFlow_Impl::inletPort()
  {
    return OS_AirConditioner_VariableRefrigerantFlowFields::CondenserInletNode;
  }

  unsigned AirConditionerVariableRefrigerantFlow_Impl::outletPort()
  {
    return OS_AirConditioner_VariableRefrigerantFlowFields::CondenserOutletNode;
  }

} // detail

AirConditionerVariableRefrigerantFlow::AirConditionerVariableRefrigerantFlow(const Model& model)
  : StraightComponent(AirConditionerVariableRefrigerantFlow::iddObjectType(),model)
{
  BOOST_ASSERT(getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>());

  // TODO: Appropriately handle the following required object-list fields.
  //     OS_AirConditioner_VariableRefrigerantFlowFields::AvailabilitySchedule
  //     OS_AirConditioner_VariableRefrigerantFlowFields::ZoneTerminalUnitList
  bool ok = true;
  // ok = setHandle();
  BOOST_ASSERT(ok);
  // ok = setAvailabilitySchedule();
  BOOST_ASSERT(ok);
  // ok = setRatedTotalCoolingCapacity();
  BOOST_ASSERT(ok);
  // ok = setRatedCoolingCOP();
  BOOST_ASSERT(ok);
  // setMinimumOutdoorTemperatureinCoolingMode();
  // setMaximumOutdoorTemperatureinCoolingMode();
  // setRatedTotalHeatingCapacity();
  // ok = setRatedTotalHeatingCapacitySizingRatio();
  BOOST_ASSERT(ok);
  // setRatedHeatingCOP();
  // setMinimumOutdoorTemperatureinHeatingMode();
  // setMaximumOutdoorTemperatureinHeatingMode();
  // ok = setHeatingPerformanceCurveOutdoorTemperatureType();
  BOOST_ASSERT(ok);
  // setMinimumHeatPumpPartLoadRatio();
  // ok = setMasterThermostatPriorityControlType();
  BOOST_ASSERT(ok);
  // ok = setZoneTerminalUnitList();
  BOOST_ASSERT(ok);
  // setHeatPumpWasteHeatRecovery();
  // setEquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode();
  // setVerticalHeightusedforPipingCorrectionFactor();
  // setPipingCorrectionFactorforHeightinCoolingModeCoefficient();
  // setEquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode();
  // setPipingCorrectionFactorforHeightinHeatingModeCoefficient();
  // setCrankcaseHeaterPowerperCompressor();
  // setNumberofCompressors();
  // setRatioofCompressorSizetoTotalCompressorCapacity();
  // setMaximumOutdoorDrybulbTemperatureforCrankcaseHeater();
  // ok = setDefrostStrategy();
  BOOST_ASSERT(ok);
  // ok = setDefrostControl();
  BOOST_ASSERT(ok);
  // ok = setDefrostTimePeriodFraction();
  BOOST_ASSERT(ok);
  // ok = setResistiveDefrostHeaterCapacity();
  BOOST_ASSERT(ok);
  // setMaximumOutdoorDrybulbTemperatureforDefrostOperation();
  BOOST_ASSERT(ok);
  // setWaterCondenserVolumeFlowRate();
  // ok = setEvaporativeCondenserEffectiveness();
  BOOST_ASSERT(ok);
  // ok = setEvaporativeCondenserAirFlowRate();
  BOOST_ASSERT(ok);
  // ok = setEvaporativeCondenserPumpRatedPowerConsumption();
  BOOST_ASSERT(ok);
  // ok = setBasinHeaterCapacity();
  BOOST_ASSERT(ok);
  // ok = setBasinHeaterSetpointTemperature();
  BOOST_ASSERT(ok);
  // ok = setFuelType();
  BOOST_ASSERT(ok);
  // setMinimumOutdoorTemperatureinHeatRecoveryMode();
  // setMaximumOutdoorTemperatureinHeatRecoveryMode();
  // setInitialHeatRecoveryCoolingCapacityFraction();
  // setHeatRecoveryCoolingCapacityTimeConstant();
  // setInitialHeatRecoveryCoolingEnergyFraction();
  // setHeatRecoveryCoolingEnergyTimeConstant();
  // setInitialHeatRecoveryHeatingCapacityFraction();
  // setHeatRecoveryHeatingCapacityTimeConstant();
  // setInitialHeatRecoveryHeatingEnergyFraction();
  // setHeatRecoveryHeatingEnergyTimeConstant();
}

IddObjectType AirConditionerVariableRefrigerantFlow::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirConditioner_VariableRefrigerantFlow);
}

std::vector<std::string> AirConditionerVariableRefrigerantFlow::heatingPerformanceCurveOutdoorTemperatureTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_AirConditioner_VariableRefrigerantFlowFields::HeatingPerformanceCurveOutdoorTemperatureType);
}

std::vector<std::string> AirConditionerVariableRefrigerantFlow::masterThermostatPriorityControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_AirConditioner_VariableRefrigerantFlowFields::MasterThermostatPriorityControlType);
}

std::vector<std::string> AirConditionerVariableRefrigerantFlow::defrostStrategyValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_AirConditioner_VariableRefrigerantFlowFields::DefrostStrategy);
}

std::vector<std::string> AirConditionerVariableRefrigerantFlow::defrostControlValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_AirConditioner_VariableRefrigerantFlowFields::DefrostControl);
}

std::vector<std::string> AirConditionerVariableRefrigerantFlow::fuelTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_AirConditioner_VariableRefrigerantFlowFields::FuelType);
}

Schedule AirConditionerVariableRefrigerantFlow::availabilitySchedule() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->availabilitySchedule();
}

boost::optional<double> AirConditionerVariableRefrigerantFlow::ratedTotalCoolingCapacity() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->ratedTotalCoolingCapacity();
}

bool AirConditionerVariableRefrigerantFlow::isRatedTotalCoolingCapacityAutosized() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->isRatedTotalCoolingCapacityAutosized();
}

double AirConditionerVariableRefrigerantFlow::ratedCoolingCOP() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->ratedCoolingCOP();
}

double AirConditionerVariableRefrigerantFlow::minimumOutdoorTemperatureinCoolingMode() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->minimumOutdoorTemperatureinCoolingMode();
}

double AirConditionerVariableRefrigerantFlow::maximumOutdoorTemperatureinCoolingMode() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->maximumOutdoorTemperatureinCoolingMode();
}

boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow::coolingCapacityRatioModifierFunctionofLowTemperatureCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->coolingCapacityRatioModifierFunctionofLowTemperatureCurve();
}

boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow::coolingCapacityRatioBoundaryCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->coolingCapacityRatioBoundaryCurve();
}

boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow::coolingCapacityRatioModifierFunctionofHighTemperatureCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->coolingCapacityRatioModifierFunctionofHighTemperatureCurve();
}

boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow::coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
}

boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow::coolingEnergyInputRatioBoundaryCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->coolingEnergyInputRatioBoundaryCurve();
}

boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow::coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
}

boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow::coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
}

boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow::coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
}

boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow::coolingCombinationRatioCorrectionFactorCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->coolingCombinationRatioCorrectionFactorCurve();
}

boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow::coolingPartLoadFractionCorrelationCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->coolingPartLoadFractionCorrelationCurve();
}

boost::optional<double> AirConditionerVariableRefrigerantFlow::ratedTotalHeatingCapacity() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->ratedTotalHeatingCapacity();
}

bool AirConditionerVariableRefrigerantFlow::isRatedTotalHeatingCapacityAutosized() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->isRatedTotalHeatingCapacityAutosized();
}

double AirConditionerVariableRefrigerantFlow::ratedTotalHeatingCapacitySizingRatio() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->ratedTotalHeatingCapacitySizingRatio();
}

double AirConditionerVariableRefrigerantFlow::ratedHeatingCOP() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->ratedHeatingCOP();
}

double AirConditionerVariableRefrigerantFlow::minimumOutdoorTemperatureinHeatingMode() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->minimumOutdoorTemperatureinHeatingMode();
}

double AirConditionerVariableRefrigerantFlow::maximumOutdoorTemperatureinHeatingMode() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->maximumOutdoorTemperatureinHeatingMode();
}

boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow::heatingCapacityRatioModifierFunctionofLowTemperatureCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatingCapacityRatioModifierFunctionofLowTemperatureCurve();
}

boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow::heatingCapacityRatioBoundaryCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatingCapacityRatioBoundaryCurve();
}

boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow::heatingCapacityRatioModifierFunctionofHighTemperatureCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatingCapacityRatioModifierFunctionofHighTemperatureCurve();
}

boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow::heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
}

boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow::heatingEnergyInputRatioBoundaryCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatingEnergyInputRatioBoundaryCurve();
}

boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow::heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
}

std::string AirConditionerVariableRefrigerantFlow::heatingPerformanceCurveOutdoorTemperatureType() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatingPerformanceCurveOutdoorTemperatureType();
}

boost::optional<CurveCubic> AirConditionerVariableRefrigerantFlow::heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
}

boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow::heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
}

boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow::heatingCombinationRatioCorrectionFactorCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatingCombinationRatioCorrectionFactorCurve();
}

boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow::heatingPartLoadFractionCorrelationCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatingPartLoadFractionCorrelationCurve();
}

double AirConditionerVariableRefrigerantFlow::minimumHeatPumpPartLoadRatio() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->minimumHeatPumpPartLoadRatio();
}

boost::optional<ThermalZone> AirConditionerVariableRefrigerantFlow::zoneforMasterThermostatLocation() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->zoneforMasterThermostatLocation();
}

std::string AirConditionerVariableRefrigerantFlow::masterThermostatPriorityControlType() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->masterThermostatPriorityControlType();
}

boost::optional<Schedule> AirConditionerVariableRefrigerantFlow::thermostatPrioritySchedule() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->thermostatPrioritySchedule();
}

bool AirConditionerVariableRefrigerantFlow::heatPumpWasteHeatRecovery() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatPumpWasteHeatRecovery();
}

double AirConditionerVariableRefrigerantFlow::equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode();
}

double AirConditionerVariableRefrigerantFlow::verticalHeightusedforPipingCorrectionFactor() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->verticalHeightusedforPipingCorrectionFactor();
}

boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow::pipingCorrectionFactorforLengthinCoolingModeCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->pipingCorrectionFactorforLengthinCoolingModeCurve();
}

double AirConditionerVariableRefrigerantFlow::pipingCorrectionFactorforHeightinCoolingModeCoefficient() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->pipingCorrectionFactorforHeightinCoolingModeCoefficient();
}

double AirConditionerVariableRefrigerantFlow::equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode();
}

boost::optional<CurveLinear> AirConditionerVariableRefrigerantFlow::pipingCorrectionFactorforLengthinHeatingModeCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->pipingCorrectionFactorforLengthinHeatingModeCurve();
}

double AirConditionerVariableRefrigerantFlow::pipingCorrectionFactorforHeightinHeatingModeCoefficient() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->pipingCorrectionFactorforHeightinHeatingModeCoefficient();
}

double AirConditionerVariableRefrigerantFlow::crankcaseHeaterPowerperCompressor() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->crankcaseHeaterPowerperCompressor();
}

int AirConditionerVariableRefrigerantFlow::numberofCompressors() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->numberofCompressors();
}

double AirConditionerVariableRefrigerantFlow::ratioofCompressorSizetoTotalCompressorCapacity() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->ratioofCompressorSizetoTotalCompressorCapacity();
}

double AirConditionerVariableRefrigerantFlow::maximumOutdoorDrybulbTemperatureforCrankcaseHeater() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->maximumOutdoorDrybulbTemperatureforCrankcaseHeater();
}

std::string AirConditionerVariableRefrigerantFlow::defrostStrategy() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->defrostStrategy();
}

std::string AirConditionerVariableRefrigerantFlow::defrostControl() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->defrostControl();
}

boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow::defrostEnergyInputRatioModifierFunctionofTemperatureCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->defrostEnergyInputRatioModifierFunctionofTemperatureCurve();
}

double AirConditionerVariableRefrigerantFlow::defrostTimePeriodFraction() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->defrostTimePeriodFraction();
}

boost::optional<double> AirConditionerVariableRefrigerantFlow::resistiveDefrostHeaterCapacity() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resistiveDefrostHeaterCapacity();
}

bool AirConditionerVariableRefrigerantFlow::isResistiveDefrostHeaterCapacityAutosized() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->isResistiveDefrostHeaterCapacityAutosized();
}

double AirConditionerVariableRefrigerantFlow::maximumOutdoorDrybulbTemperatureforDefrostOperation() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->maximumOutdoorDrybulbTemperatureforDefrostOperation();
}

boost::optional<double> AirConditionerVariableRefrigerantFlow::waterCondenserVolumeFlowRate() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->waterCondenserVolumeFlowRate();
}

bool AirConditionerVariableRefrigerantFlow::isWaterCondenserVolumeFlowRateAutosized() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->isWaterCondenserVolumeFlowRateAutosized();
}

double AirConditionerVariableRefrigerantFlow::evaporativeCondenserEffectiveness() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->evaporativeCondenserEffectiveness();
}

boost::optional<double> AirConditionerVariableRefrigerantFlow::evaporativeCondenserAirFlowRate() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->evaporativeCondenserAirFlowRate();
}

bool AirConditionerVariableRefrigerantFlow::isEvaporativeCondenserAirFlowRateAutosized() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->isEvaporativeCondenserAirFlowRateAutosized();
}

boost::optional<double> AirConditionerVariableRefrigerantFlow::evaporativeCondenserPumpRatedPowerConsumption() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->evaporativeCondenserPumpRatedPowerConsumption();
}

bool AirConditionerVariableRefrigerantFlow::isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->isEvaporativeCondenserPumpRatedPowerConsumptionAutosized();
}

double AirConditionerVariableRefrigerantFlow::basinHeaterCapacity() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->basinHeaterCapacity();
}

double AirConditionerVariableRefrigerantFlow::basinHeaterSetpointTemperature() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->basinHeaterSetpointTemperature();
}

boost::optional<Schedule> AirConditionerVariableRefrigerantFlow::basinHeaterOperatingSchedule() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->basinHeaterOperatingSchedule();
}

std::string AirConditionerVariableRefrigerantFlow::fuelType() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->fuelType();
}

double AirConditionerVariableRefrigerantFlow::minimumOutdoorTemperatureinHeatRecoveryMode() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->minimumOutdoorTemperatureinHeatRecoveryMode();
}

double AirConditionerVariableRefrigerantFlow::maximumOutdoorTemperatureinHeatRecoveryMode() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->maximumOutdoorTemperatureinHeatRecoveryMode();
}

boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow::heatRecoveryCoolingCapacityModifierCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatRecoveryCoolingCapacityModifierCurve();
}

double AirConditionerVariableRefrigerantFlow::initialHeatRecoveryCoolingCapacityFraction() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->initialHeatRecoveryCoolingCapacityFraction();
}

double AirConditionerVariableRefrigerantFlow::heatRecoveryCoolingCapacityTimeConstant() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatRecoveryCoolingCapacityTimeConstant();
}

boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow::heatRecoveryCoolingEnergyModifierCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatRecoveryCoolingEnergyModifierCurve();
}

double AirConditionerVariableRefrigerantFlow::initialHeatRecoveryCoolingEnergyFraction() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->initialHeatRecoveryCoolingEnergyFraction();
}

double AirConditionerVariableRefrigerantFlow::heatRecoveryCoolingEnergyTimeConstant() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatRecoveryCoolingEnergyTimeConstant();
}

boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow::heatRecoveryHeatingCapacityModifierCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatRecoveryHeatingCapacityModifierCurve();
}

double AirConditionerVariableRefrigerantFlow::initialHeatRecoveryHeatingCapacityFraction() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->initialHeatRecoveryHeatingCapacityFraction();
}

double AirConditionerVariableRefrigerantFlow::heatRecoveryHeatingCapacityTimeConstant() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatRecoveryHeatingCapacityTimeConstant();
}

boost::optional<CurveBiquadratic> AirConditionerVariableRefrigerantFlow::heatRecoveryHeatingEnergyModifierCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatRecoveryHeatingEnergyModifierCurve();
}

double AirConditionerVariableRefrigerantFlow::initialHeatRecoveryHeatingEnergyFraction() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->initialHeatRecoveryHeatingEnergyFraction();
}

double AirConditionerVariableRefrigerantFlow::heatRecoveryHeatingEnergyTimeConstant() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatRecoveryHeatingEnergyTimeConstant();
}

bool AirConditionerVariableRefrigerantFlow::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setAvailabilitySchedule(schedule);
}

bool AirConditionerVariableRefrigerantFlow::setRatedTotalCoolingCapacity(double ratedTotalCoolingCapacity) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setRatedTotalCoolingCapacity(ratedTotalCoolingCapacity);
}

void AirConditionerVariableRefrigerantFlow::autosizeRatedTotalCoolingCapacity() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->autosizeRatedTotalCoolingCapacity();
}

bool AirConditionerVariableRefrigerantFlow::setRatedCoolingCOP(double ratedCoolingCOP) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setRatedCoolingCOP(ratedCoolingCOP);
}

void AirConditionerVariableRefrigerantFlow::setMinimumOutdoorTemperatureinCoolingMode(double minimumOutdoorTemperatureinCoolingMode) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setMinimumOutdoorTemperatureinCoolingMode(minimumOutdoorTemperatureinCoolingMode);
}

void AirConditionerVariableRefrigerantFlow::setMaximumOutdoorTemperatureinCoolingMode(double maximumOutdoorTemperatureinCoolingMode) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setMaximumOutdoorTemperatureinCoolingMode(maximumOutdoorTemperatureinCoolingMode);
}

bool AirConditionerVariableRefrigerantFlow::setCoolingCapacityRatioModifierFunctionofLowTemperatureCurve(const CurveBiquadratic& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setCoolingCapacityRatioModifierFunctionofLowTemperatureCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetCoolingCapacityRatioModifierFunctionofLowTemperatureCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetCoolingCapacityRatioModifierFunctionofLowTemperatureCurve();
}

bool AirConditionerVariableRefrigerantFlow::setCoolingCapacityRatioBoundaryCurve(const CurveLinear& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setCoolingCapacityRatioBoundaryCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetCoolingCapacityRatioBoundaryCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetCoolingCapacityRatioBoundaryCurve();
}

bool AirConditionerVariableRefrigerantFlow::setCoolingCapacityRatioModifierFunctionofHighTemperatureCurve(const CurveBiquadratic& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setCoolingCapacityRatioModifierFunctionofHighTemperatureCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetCoolingCapacityRatioModifierFunctionofHighTemperatureCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetCoolingCapacityRatioModifierFunctionofHighTemperatureCurve();
}

bool AirConditionerVariableRefrigerantFlow::setCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve(const CurveBiquadratic& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
}

bool AirConditionerVariableRefrigerantFlow::setCoolingEnergyInputRatioBoundaryCurve(const CurveLinear& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setCoolingEnergyInputRatioBoundaryCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetCoolingEnergyInputRatioBoundaryCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetCoolingEnergyInputRatioBoundaryCurve();
}

bool AirConditionerVariableRefrigerantFlow::setCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve(const CurveBiquadratic& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
}

bool AirConditionerVariableRefrigerantFlow::setCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(const CurveLinear& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
}

bool AirConditionerVariableRefrigerantFlow::setCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(const CurveLinear& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
}

bool AirConditionerVariableRefrigerantFlow::setCoolingCombinationRatioCorrectionFactorCurve(const CurveLinear& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setCoolingCombinationRatioCorrectionFactorCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetCoolingCombinationRatioCorrectionFactorCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetCoolingCombinationRatioCorrectionFactorCurve();
}

bool AirConditionerVariableRefrigerantFlow::setCoolingPartLoadFractionCorrelationCurve(const CurveLinear& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setCoolingPartLoadFractionCorrelationCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetCoolingPartLoadFractionCorrelationCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetCoolingPartLoadFractionCorrelationCurve();
}

void AirConditionerVariableRefrigerantFlow::setRatedTotalHeatingCapacity(double ratedTotalHeatingCapacity) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setRatedTotalHeatingCapacity(ratedTotalHeatingCapacity);
}

void AirConditionerVariableRefrigerantFlow::autosizeRatedTotalHeatingCapacity() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->autosizeRatedTotalHeatingCapacity();
}

bool AirConditionerVariableRefrigerantFlow::setRatedTotalHeatingCapacitySizingRatio(double ratedTotalHeatingCapacitySizingRatio) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setRatedTotalHeatingCapacitySizingRatio(ratedTotalHeatingCapacitySizingRatio);
}

void AirConditionerVariableRefrigerantFlow::setRatedHeatingCOP(double ratedHeatingCOP) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setRatedHeatingCOP(ratedHeatingCOP);
}

void AirConditionerVariableRefrigerantFlow::setMinimumOutdoorTemperatureinHeatingMode(double minimumOutdoorTemperatureinHeatingMode) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setMinimumOutdoorTemperatureinHeatingMode(minimumOutdoorTemperatureinHeatingMode);
}

void AirConditionerVariableRefrigerantFlow::setMaximumOutdoorTemperatureinHeatingMode(double maximumOutdoorTemperatureinHeatingMode) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setMaximumOutdoorTemperatureinHeatingMode(maximumOutdoorTemperatureinHeatingMode);
}

bool AirConditionerVariableRefrigerantFlow::setHeatingCapacityRatioModifierFunctionofLowTemperatureCurve(const CurveBiquadratic& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatingCapacityRatioModifierFunctionofLowTemperatureCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatingCapacityRatioModifierFunctionofLowTemperatureCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatingCapacityRatioModifierFunctionofLowTemperatureCurve();
}

bool AirConditionerVariableRefrigerantFlow::setHeatingCapacityRatioBoundaryCurve(const CurveLinear& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatingCapacityRatioBoundaryCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatingCapacityRatioBoundaryCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatingCapacityRatioBoundaryCurve();
}

bool AirConditionerVariableRefrigerantFlow::setHeatingCapacityRatioModifierFunctionofHighTemperatureCurve(const CurveBiquadratic& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatingCapacityRatioModifierFunctionofHighTemperatureCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatingCapacityRatioModifierFunctionofHighTemperatureCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatingCapacityRatioModifierFunctionofHighTemperatureCurve();
}

bool AirConditionerVariableRefrigerantFlow::setHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve(const CurveBiquadratic& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
}

bool AirConditionerVariableRefrigerantFlow::setHeatingEnergyInputRatioBoundaryCurve(const CurveLinear& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatingEnergyInputRatioBoundaryCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatingEnergyInputRatioBoundaryCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatingEnergyInputRatioBoundaryCurve();
}

bool AirConditionerVariableRefrigerantFlow::setHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve(const CurveBiquadratic& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
}

bool AirConditionerVariableRefrigerantFlow::setHeatingPerformanceCurveOutdoorTemperatureType(std::string heatingPerformanceCurveOutdoorTemperatureType) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatingPerformanceCurveOutdoorTemperatureType(heatingPerformanceCurveOutdoorTemperatureType);
}

bool AirConditionerVariableRefrigerantFlow::setHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(const CurveCubic& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
}

bool AirConditionerVariableRefrigerantFlow::setHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(const CurveLinear& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
}

bool AirConditionerVariableRefrigerantFlow::setHeatingCombinationRatioCorrectionFactorCurve(const CurveLinear& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatingCombinationRatioCorrectionFactorCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatingCombinationRatioCorrectionFactorCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatingCombinationRatioCorrectionFactorCurve();
}

bool AirConditionerVariableRefrigerantFlow::setHeatingPartLoadFractionCorrelationCurve(const CurveLinear& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatingPartLoadFractionCorrelationCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatingPartLoadFractionCorrelationCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatingPartLoadFractionCorrelationCurve();
}

void AirConditionerVariableRefrigerantFlow::setMinimumHeatPumpPartLoadRatio(double minimumHeatPumpPartLoadRatio) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setMinimumHeatPumpPartLoadRatio(minimumHeatPumpPartLoadRatio);
}

bool AirConditionerVariableRefrigerantFlow::setZoneforMasterThermostatLocation(const ThermalZone& zone) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setZoneforMasterThermostatLocation(zone);
}

void AirConditionerVariableRefrigerantFlow::resetZoneforMasterThermostatLocation() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetZoneforMasterThermostatLocation();
}

bool AirConditionerVariableRefrigerantFlow::setMasterThermostatPriorityControlType(std::string masterThermostatPriorityControlType) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setMasterThermostatPriorityControlType(masterThermostatPriorityControlType);
}

bool AirConditionerVariableRefrigerantFlow::setThermostatPrioritySchedule(Schedule& schedule) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setThermostatPrioritySchedule(schedule);
}

void AirConditionerVariableRefrigerantFlow::resetThermostatPrioritySchedule() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetThermostatPrioritySchedule();
}

void AirConditionerVariableRefrigerantFlow::setHeatPumpWasteHeatRecovery(bool heatPumpWasteHeatRecovery) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatPumpWasteHeatRecovery(heatPumpWasteHeatRecovery);
}

void AirConditionerVariableRefrigerantFlow::setEquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode(double equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setEquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode(equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode);
}

void AirConditionerVariableRefrigerantFlow::setVerticalHeightusedforPipingCorrectionFactor(double verticalHeightusedforPipingCorrectionFactor) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setVerticalHeightusedforPipingCorrectionFactor(verticalHeightusedforPipingCorrectionFactor);
}

bool AirConditionerVariableRefrigerantFlow::setPipingCorrectionFactorforLengthinCoolingModeCurve(const CurveLinear& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setPipingCorrectionFactorforLengthinCoolingModeCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetPipingCorrectionFactorforLengthinCoolingModeCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetPipingCorrectionFactorforLengthinCoolingModeCurve();
}

void AirConditionerVariableRefrigerantFlow::setPipingCorrectionFactorforHeightinCoolingModeCoefficient(double pipingCorrectionFactorforHeightinCoolingModeCoefficient) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setPipingCorrectionFactorforHeightinCoolingModeCoefficient(pipingCorrectionFactorforHeightinCoolingModeCoefficient);
}

void AirConditionerVariableRefrigerantFlow::setEquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode(double equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setEquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode(equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode);
}

bool AirConditionerVariableRefrigerantFlow::setPipingCorrectionFactorforLengthinHeatingModeCurve(const CurveLinear& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setPipingCorrectionFactorforLengthinHeatingModeCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetPipingCorrectionFactorforLengthinHeatingModeCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetPipingCorrectionFactorforLengthinHeatingModeCurve();
}

void AirConditionerVariableRefrigerantFlow::setPipingCorrectionFactorforHeightinHeatingModeCoefficient(double pipingCorrectionFactorforHeightinHeatingModeCoefficient) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setPipingCorrectionFactorforHeightinHeatingModeCoefficient(pipingCorrectionFactorforHeightinHeatingModeCoefficient);
}

void AirConditionerVariableRefrigerantFlow::setCrankcaseHeaterPowerperCompressor(double crankcaseHeaterPowerperCompressor) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setCrankcaseHeaterPowerperCompressor(crankcaseHeaterPowerperCompressor);
}

void AirConditionerVariableRefrigerantFlow::setNumberofCompressors(int numberofCompressors) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setNumberofCompressors(numberofCompressors);
}

void AirConditionerVariableRefrigerantFlow::setRatioofCompressorSizetoTotalCompressorCapacity(double ratioofCompressorSizetoTotalCompressorCapacity) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setRatioofCompressorSizetoTotalCompressorCapacity(ratioofCompressorSizetoTotalCompressorCapacity);
}

void AirConditionerVariableRefrigerantFlow::setMaximumOutdoorDrybulbTemperatureforCrankcaseHeater(double maximumOutdoorDrybulbTemperatureforCrankcaseHeater) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setMaximumOutdoorDrybulbTemperatureforCrankcaseHeater(maximumOutdoorDrybulbTemperatureforCrankcaseHeater);
}

bool AirConditionerVariableRefrigerantFlow::setDefrostStrategy(std::string defrostStrategy) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setDefrostStrategy(defrostStrategy);
}

bool AirConditionerVariableRefrigerantFlow::setDefrostControl(std::string defrostControl) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setDefrostControl(defrostControl);
}

bool AirConditionerVariableRefrigerantFlow::setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(const CurveBiquadratic& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve();
}

bool AirConditionerVariableRefrigerantFlow::setDefrostTimePeriodFraction(double defrostTimePeriodFraction) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setDefrostTimePeriodFraction(defrostTimePeriodFraction);
}

bool AirConditionerVariableRefrigerantFlow::setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setResistiveDefrostHeaterCapacity(resistiveDefrostHeaterCapacity);
}

void AirConditionerVariableRefrigerantFlow::autosizeResistiveDefrostHeaterCapacity() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->autosizeResistiveDefrostHeaterCapacity();
}

void AirConditionerVariableRefrigerantFlow::setMaximumOutdoorDrybulbTemperatureforDefrostOperation(double maximumOutdoorDrybulbTemperatureforDefrostOperation) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setMaximumOutdoorDrybulbTemperatureforDefrostOperation(maximumOutdoorDrybulbTemperatureforDefrostOperation);
}

void AirConditionerVariableRefrigerantFlow::setWaterCondenserVolumeFlowRate(double waterCondenserVolumeFlowRate) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setWaterCondenserVolumeFlowRate(waterCondenserVolumeFlowRate);
}

void AirConditionerVariableRefrigerantFlow::autosizeWaterCondenserVolumeFlowRate() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->autosizeWaterCondenserVolumeFlowRate();
}

bool AirConditionerVariableRefrigerantFlow::setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setEvaporativeCondenserEffectiveness(evaporativeCondenserEffectiveness);
}

bool AirConditionerVariableRefrigerantFlow::setEvaporativeCondenserAirFlowRate(double evaporativeCondenserAirFlowRate) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setEvaporativeCondenserAirFlowRate(evaporativeCondenserAirFlowRate);
}

void AirConditionerVariableRefrigerantFlow::autosizeEvaporativeCondenserAirFlowRate() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->autosizeEvaporativeCondenserAirFlowRate();
}

bool AirConditionerVariableRefrigerantFlow::setEvaporativeCondenserPumpRatedPowerConsumption(double evaporativeCondenserPumpRatedPowerConsumption) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setEvaporativeCondenserPumpRatedPowerConsumption(evaporativeCondenserPumpRatedPowerConsumption);
}

void AirConditionerVariableRefrigerantFlow::autosizeEvaporativeCondenserPumpRatedPowerConsumption() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->autosizeEvaporativeCondenserPumpRatedPowerConsumption();
}

bool AirConditionerVariableRefrigerantFlow::setBasinHeaterCapacity(double basinHeaterCapacity) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setBasinHeaterCapacity(basinHeaterCapacity);
}

bool AirConditionerVariableRefrigerantFlow::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setBasinHeaterSetpointTemperature(basinHeaterSetpointTemperature);
}

bool AirConditionerVariableRefrigerantFlow::setBasinHeaterOperatingSchedule(Schedule& schedule) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setBasinHeaterOperatingSchedule(schedule);
}

void AirConditionerVariableRefrigerantFlow::resetBasinHeaterOperatingSchedule() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetBasinHeaterOperatingSchedule();
}

bool AirConditionerVariableRefrigerantFlow::setFuelType(std::string fuelType) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setFuelType(fuelType);
}

void AirConditionerVariableRefrigerantFlow::setMinimumOutdoorTemperatureinHeatRecoveryMode(double minimumOutdoorTemperatureinHeatRecoveryMode) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setMinimumOutdoorTemperatureinHeatRecoveryMode(minimumOutdoorTemperatureinHeatRecoveryMode);
}

void AirConditionerVariableRefrigerantFlow::setMaximumOutdoorTemperatureinHeatRecoveryMode(double maximumOutdoorTemperatureinHeatRecoveryMode) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setMaximumOutdoorTemperatureinHeatRecoveryMode(maximumOutdoorTemperatureinHeatRecoveryMode);
}

bool AirConditionerVariableRefrigerantFlow::setHeatRecoveryCoolingCapacityModifierCurve(const CurveBiquadratic& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatRecoveryCoolingCapacityModifierCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatRecoveryCoolingCapacityModifierCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatRecoveryCoolingCapacityModifierCurve();
}

void AirConditionerVariableRefrigerantFlow::setInitialHeatRecoveryCoolingCapacityFraction(double initialHeatRecoveryCoolingCapacityFraction) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setInitialHeatRecoveryCoolingCapacityFraction(initialHeatRecoveryCoolingCapacityFraction);
}

void AirConditionerVariableRefrigerantFlow::setHeatRecoveryCoolingCapacityTimeConstant(double heatRecoveryCoolingCapacityTimeConstant) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatRecoveryCoolingCapacityTimeConstant(heatRecoveryCoolingCapacityTimeConstant);
}

bool AirConditionerVariableRefrigerantFlow::setHeatRecoveryCoolingEnergyModifierCurve(const CurveBiquadratic& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatRecoveryCoolingEnergyModifierCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatRecoveryCoolingEnergyModifierCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatRecoveryCoolingEnergyModifierCurve();
}

void AirConditionerVariableRefrigerantFlow::setInitialHeatRecoveryCoolingEnergyFraction(double initialHeatRecoveryCoolingEnergyFraction) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setInitialHeatRecoveryCoolingEnergyFraction(initialHeatRecoveryCoolingEnergyFraction);
}

void AirConditionerVariableRefrigerantFlow::setHeatRecoveryCoolingEnergyTimeConstant(double heatRecoveryCoolingEnergyTimeConstant) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatRecoveryCoolingEnergyTimeConstant(heatRecoveryCoolingEnergyTimeConstant);
}

bool AirConditionerVariableRefrigerantFlow::setHeatRecoveryHeatingCapacityModifierCurve(const CurveBiquadratic& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatRecoveryHeatingCapacityModifierCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatRecoveryHeatingCapacityModifierCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatRecoveryHeatingCapacityModifierCurve();
}

void AirConditionerVariableRefrigerantFlow::setInitialHeatRecoveryHeatingCapacityFraction(double initialHeatRecoveryHeatingCapacityFraction) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setInitialHeatRecoveryHeatingCapacityFraction(initialHeatRecoveryHeatingCapacityFraction);
}

void AirConditionerVariableRefrigerantFlow::setHeatRecoveryHeatingCapacityTimeConstant(double heatRecoveryHeatingCapacityTimeConstant) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatRecoveryHeatingCapacityTimeConstant(heatRecoveryHeatingCapacityTimeConstant);
}

bool AirConditionerVariableRefrigerantFlow::setHeatRecoveryHeatingEnergyModifierCurve(const CurveBiquadratic& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatRecoveryHeatingEnergyModifierCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatRecoveryHeatingEnergyModifierCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatRecoveryHeatingEnergyModifierCurve();
}

void AirConditionerVariableRefrigerantFlow::setInitialHeatRecoveryHeatingEnergyFraction(double initialHeatRecoveryHeatingEnergyFraction) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setInitialHeatRecoveryHeatingEnergyFraction(initialHeatRecoveryHeatingEnergyFraction);
}

void AirConditionerVariableRefrigerantFlow::setHeatRecoveryHeatingEnergyTimeConstant(double heatRecoveryHeatingEnergyTimeConstant) {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatRecoveryHeatingEnergyTimeConstant(heatRecoveryHeatingEnergyTimeConstant);
}

/// @cond
AirConditionerVariableRefrigerantFlow::AirConditionerVariableRefrigerantFlow(boost::shared_ptr<detail::AirConditionerVariableRefrigerantFlow_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

