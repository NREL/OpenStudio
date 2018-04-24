/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "AirConditionerVariableRefrigerantFlow.hpp"
#include "AirConditionerVariableRefrigerantFlow_Impl.hpp"
#include "ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveBiquadratic.hpp"
#include "CurveBiquadratic_Impl.hpp"
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ModelObjectList.hpp"
#include "ModelObjectList_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_AirConditioner_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Containers.hpp"

namespace openstudio {

namespace model {

namespace detail {

  AirConditionerVariableRefrigerantFlow_Impl::AirConditionerVariableRefrigerantFlow_Impl(const IdfObject& idfObject,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirConditionerVariableRefrigerantFlow::iddObjectType());
  }

  AirConditionerVariableRefrigerantFlow_Impl::AirConditionerVariableRefrigerantFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirConditionerVariableRefrigerantFlow::iddObjectType());
  }

  AirConditionerVariableRefrigerantFlow_Impl::AirConditionerVariableRefrigerantFlow_Impl(const AirConditionerVariableRefrigerantFlow_Impl& other,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirConditionerVariableRefrigerantFlow_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result{ "VRF Heat Pump Total Cooling Rate",
      "VRF Heat Pump Total Heating Rate",
      "VRF Heat Pump Cooling COP",
      "VRF Heat Pump Heating COP",
      "VRF Heat Pump COP",
      "VRF Heat Pump Part Load Ratio",
      "VRF Heat Pump Runtime Fraction",
      "VRF Heat Pump Cycling Ratio",
      "VRF Heat Pump Operating Mode",
      "VRF Heat Pump Condenser Inlet Temperature",
      "VRF Heat Pump Maximum Capacity Cooling Rate",
      "VRF Heat Pump Maximum Capacity Heating Rate",
      "VRF Heat Pump Crankcase Heater Electric Power",
      "VRF Heat Pump Crankcase Heater Electric Energy",
      "VRF Heat Pump Terminal Unit Heating Load Rate",
      "VRF Heat Pump Terminal Unit Cooling Load Rate",

      // TODO: add proper tests once the ModelObject return type is changed.
      // For now include all
      // Heat Recovery:
      "VRF Heat Pump Heat Recovery Status Change Multiplier",
      "VRF Heat Pump Simultaneous Cooling and Heating Efficiency",
      // Evap-cooled:
      "VRF Heat Pump Evaporative Condenser Water Use Volume",
      "VRF Heat Pump Evaporative Condenser Pump Electric Power",
      "VRF Heat Pump Evaporative Condenser Pump Electric Energy",
      "VRF Heat Pump Basin Heater Electric Power",
      "VRF Heat Pump Basin Heater Electric Energy",
      "VRF Heat Pump Heat Recovery Status Change Multiplier",
      // Water-cooled:
      "VRF Heat Pump Condenser Outlet Temperature",
      "VRF Heat Pump Condenser Mass Flow Rate",
      "VRF Heat Pump Condenser Heat Transfer Energy",
      "VRF Heat Pump Condenser Heat Transfer Rate",
      // Electric Fuel type (default):
      "VRF Heat Pump Cooling Electric Power",
      "VRF Heat Pump Cooling Electric Energy",
      "VRF Heat Pump Heating Electric Power",
      "VRF Heat Pump Heating Electric Energy",
      // Electric defrost always used for Defrost Strategy = Resistive regardless of fuel type
      "VRF Heat Pump Defrost Electric Power",
      "VRF Heat Pump Defrost Electric Energy"
      // Alternate Fuel types (e.g., FuelType = NaturalGas):
      //"VRF Heat Pump Cooling <FuelType> Rate",
      //"VRF Heat Pump Cooling <FuelType> Energy",
      //"VRF Heat Pump Heating <FuelType> Rate",
      //"VRF Heat Pump Heating <FuelType> Energy",
      //"VRF Heat Pump Defrost <FuelType> Rate",
      //"VRF Heat Pump Defrost <FuelType> Energy",

    };
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
    OS_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::minimumOutdoorTemperatureinCoolingMode() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumOutdoorTemperatureinCoolingMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::maximumOutdoorTemperatureinCoolingMode() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorTemperatureinCoolingMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::coolingCapacityRatioModifierFunctionofLowTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofLowTemperatureCurve);
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::coolingCapacityRatioBoundaryCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioBoundaryCurve);
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::coolingCapacityRatioModifierFunctionofHighTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofHighTemperatureCurve);
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve);
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::coolingEnergyInputRatioBoundaryCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioBoundaryCurve);
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve);
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve);
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve);
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::coolingCombinationRatioCorrectionFactorCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCombinationRatioCorrectionFactorCurve);
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::coolingPartLoadFractionCorrelationCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingPartLoadFractionCorrelationCurve);
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
    OS_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::ratedHeatingCOP() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatedHeatingCOP,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::minimumOutdoorTemperatureinHeatingMode() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumOutdoorTemperatureinHeatingMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::maximumOutdoorTemperatureinHeatingMode() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorTemperatureinHeatingMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::heatingCapacityRatioModifierFunctionofLowTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofLowTemperatureCurve);
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::heatingCapacityRatioBoundaryCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioBoundaryCurve);
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::heatingCapacityRatioModifierFunctionofHighTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofHighTemperatureCurve);
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve);
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::heatingEnergyInputRatioBoundaryCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioBoundaryCurve);
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve);
  }

  std::string AirConditionerVariableRefrigerantFlow_Impl::heatingPerformanceCurveOutdoorTemperatureType() const {
    boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingPerformanceCurveOutdoorTemperatureType,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve);
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve);
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::heatingCombinationRatioCorrectionFactorCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCombinationRatioCorrectionFactorCurve);
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::heatingPartLoadFractionCorrelationCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingPartLoadFractionCorrelationCurve);
  }

  double AirConditionerVariableRefrigerantFlow_Impl::minimumHeatPumpPartLoadRatio() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumHeatPumpPartLoadRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<ThermalZone> AirConditionerVariableRefrigerantFlow_Impl::zoneforMasterThermostatLocation() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_AirConditioner_VariableRefrigerantFlowFields::ZoneNameforMasterThermostatLocation);
  }

  std::string AirConditionerVariableRefrigerantFlow_Impl::masterThermostatPriorityControlType() const {
    boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::MasterThermostatPriorityControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> AirConditionerVariableRefrigerantFlow_Impl::thermostatPrioritySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirConditioner_VariableRefrigerantFlowFields::ThermostatPrioritySchedule);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::heatPumpWasteHeatRecovery() const {
    boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatPumpWasteHeatRecovery,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  double AirConditionerVariableRefrigerantFlow_Impl::equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::verticalHeightusedforPipingCorrectionFactor() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::VerticalHeightusedforPipingCorrectionFactor,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::pipingCorrectionFactorforLengthinCoolingModeCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforLengthinCoolingModeCurve);
  }

  double AirConditionerVariableRefrigerantFlow_Impl::pipingCorrectionFactorforHeightinCoolingModeCoefficient() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforHeightinCoolingModeCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::pipingCorrectionFactorforLengthinHeatingModeCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforLengthinHeatingModeCurve);
  }

  double AirConditionerVariableRefrigerantFlow_Impl::pipingCorrectionFactorforHeightinHeatingModeCoefficient() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforHeightinHeatingModeCoefficient,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::crankcaseHeaterPowerperCompressor() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::CrankcaseHeaterPowerperCompressor,true);
    OS_ASSERT(value);
    return value.get();
  }

  int AirConditionerVariableRefrigerantFlow_Impl::numberofCompressors() const {
    boost::optional<int> value = getInt(OS_AirConditioner_VariableRefrigerantFlowFields::NumberofCompressors,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::ratioofCompressorSizetoTotalCompressorCapacity() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatioofCompressorSizetoTotalCompressorCapacity,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::maximumOutdoorDrybulbTemperatureforCrankcaseHeater() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorDrybulbTemperatureforCrankcaseHeater,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string AirConditionerVariableRefrigerantFlow_Impl::defrostStrategy() const {
    boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostStrategy,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string AirConditionerVariableRefrigerantFlow_Impl::defrostControl() const {
    boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostControl,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::defrostEnergyInputRatioModifierFunctionofTemperatureCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurve);
  }

  double AirConditionerVariableRefrigerantFlow_Impl::defrostTimePeriodFraction() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostTimePeriodFraction,true);
    OS_ASSERT(value);
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
    OS_ASSERT(value);
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
    OS_ASSERT(value);
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
    OS_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::basinHeaterSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::BasinHeaterSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> AirConditionerVariableRefrigerantFlow_Impl::basinHeaterOperatingSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirConditioner_VariableRefrigerantFlowFields::BasinHeaterOperatingSchedule);
  }

  std::string AirConditionerVariableRefrigerantFlow_Impl::fuelType() const {
    boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::FuelType,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::minimumOutdoorTemperatureinHeatRecoveryMode() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumOutdoorTemperatureinHeatRecoveryMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::maximumOutdoorTemperatureinHeatRecoveryMode() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorTemperatureinHeatRecoveryMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::heatRecoveryCoolingCapacityModifierCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityModifierCurve);
  }

  double AirConditionerVariableRefrigerantFlow_Impl::initialHeatRecoveryCoolingCapacityFraction() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingCapacityFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::heatRecoveryCoolingCapacityTimeConstant() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityTimeConstant,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::heatRecoveryCoolingEnergyModifierCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyModifierCurve);
  }

  double AirConditionerVariableRefrigerantFlow_Impl::initialHeatRecoveryCoolingEnergyFraction() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingEnergyFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::heatRecoveryCoolingEnergyTimeConstant() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyTimeConstant,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::heatRecoveryHeatingCapacityModifierCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityModifierCurve);
  }

  double AirConditionerVariableRefrigerantFlow_Impl::initialHeatRecoveryHeatingCapacityFraction() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryHeatingCapacityFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::heatRecoveryHeatingCapacityTimeConstant() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityTimeConstant,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlow_Impl::heatRecoveryHeatingEnergyModifierCurve() const {
    return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyModifierCurve);
  }

  double AirConditionerVariableRefrigerantFlow_Impl::initialHeatRecoveryHeatingEnergyFraction() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryHeatingEnergyFraction,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirConditionerVariableRefrigerantFlow_Impl::heatRecoveryHeatingEnergyTimeConstant() const {
    boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyTimeConstant,true);
    OS_ASSERT(value);
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setRatedCoolingCOP(double ratedCoolingCOP) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatedCoolingCOP, ratedCoolingCOP);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setMinimumOutdoorTemperatureinCoolingMode(double minimumOutdoorTemperatureinCoolingMode) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumOutdoorTemperatureinCoolingMode, minimumOutdoorTemperatureinCoolingMode);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setMaximumOutdoorTemperatureinCoolingMode(double maximumOutdoorTemperatureinCoolingMode) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorTemperatureinCoolingMode, maximumOutdoorTemperatureinCoolingMode);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setCoolingCapacityRatioModifierFunctionofLowTemperatureCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setCoolingCapacityRatioBoundaryCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setCoolingCapacityRatioModifierFunctionofHighTemperatureCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setCoolingEnergyInputRatioBoundaryCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setCoolingCombinationRatioCorrectionFactorCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setCoolingPartLoadFractionCorrelationCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setRatedTotalHeatingCapacity(boost::optional<double> ratedTotalHeatingCapacity) {
    bool result(false);
    if (ratedTotalHeatingCapacity) {
      result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatedTotalHeatingCapacity, ratedTotalHeatingCapacity.get());
    }
    OS_ASSERT(result);
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::autosizeRatedTotalHeatingCapacity() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::RatedTotalHeatingCapacity, "autosize");
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setRatedTotalHeatingCapacitySizingRatio(double ratedTotalHeatingCapacitySizingRatio) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatedTotalHeatingCapacitySizingRatio, ratedTotalHeatingCapacitySizingRatio);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setRatedHeatingCOP(double ratedHeatingCOP) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatedHeatingCOP, ratedHeatingCOP);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setMinimumOutdoorTemperatureinHeatingMode(double minimumOutdoorTemperatureinHeatingMode) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumOutdoorTemperatureinHeatingMode, minimumOutdoorTemperatureinHeatingMode);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setMaximumOutdoorTemperatureinHeatingMode(double maximumOutdoorTemperatureinHeatingMode) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorTemperatureinHeatingMode, maximumOutdoorTemperatureinHeatingMode);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatingCapacityRatioModifierFunctionofLowTemperatureCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatingCapacityRatioBoundaryCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatingCapacityRatioModifierFunctionofHighTemperatureCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatingEnergyInputRatioBoundaryCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatingPerformanceCurveOutdoorTemperatureType(std::string heatingPerformanceCurveOutdoorTemperatureType) {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingPerformanceCurveOutdoorTemperatureType, heatingPerformanceCurveOutdoorTemperatureType);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatingCombinationRatioCorrectionFactorCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatingPartLoadFractionCorrelationCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setMinimumHeatPumpPartLoadRatio(double minimumHeatPumpPartLoadRatio) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumHeatPumpPartLoadRatio, minimumHeatPumpPartLoadRatio);
    OS_ASSERT(result);
    return result;
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
    OS_ASSERT(result);
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatPumpWasteHeatRecovery(bool heatPumpWasteHeatRecovery) {
    return setBooleanFieldValue(OS_AirConditioner_VariableRefrigerantFlowFields::HeatPumpWasteHeatRecovery, heatPumpWasteHeatRecovery);;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setEquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode(double equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode, equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setVerticalHeightusedforPipingCorrectionFactor(double verticalHeightusedforPipingCorrectionFactor) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::VerticalHeightusedforPipingCorrectionFactor, verticalHeightusedforPipingCorrectionFactor);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setPipingCorrectionFactorforLengthinCoolingModeCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setPipingCorrectionFactorforHeightinCoolingModeCoefficient(double pipingCorrectionFactorforHeightinCoolingModeCoefficient) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforHeightinCoolingModeCoefficient, pipingCorrectionFactorforHeightinCoolingModeCoefficient);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setEquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode(double equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode, equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setPipingCorrectionFactorforLengthinHeatingModeCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setPipingCorrectionFactorforHeightinHeatingModeCoefficient(double pipingCorrectionFactorforHeightinHeatingModeCoefficient) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforHeightinHeatingModeCoefficient, pipingCorrectionFactorforHeightinHeatingModeCoefficient);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setCrankcaseHeaterPowerperCompressor(double crankcaseHeaterPowerperCompressor) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::CrankcaseHeaterPowerperCompressor, crankcaseHeaterPowerperCompressor);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setNumberofCompressors(int numberofCompressors) {
    bool result = setInt(OS_AirConditioner_VariableRefrigerantFlowFields::NumberofCompressors, numberofCompressors);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setRatioofCompressorSizetoTotalCompressorCapacity(double ratioofCompressorSizetoTotalCompressorCapacity) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatioofCompressorSizetoTotalCompressorCapacity, ratioofCompressorSizetoTotalCompressorCapacity);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setMaximumOutdoorDrybulbTemperatureforCrankcaseHeater(double maximumOutdoorDrybulbTemperatureforCrankcaseHeater) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorDrybulbTemperatureforCrankcaseHeater, maximumOutdoorDrybulbTemperatureforCrankcaseHeater);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setDefrostStrategy(std::string defrostStrategy) {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostStrategy, defrostStrategy);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setDefrostControl(std::string defrostControl) {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostControl, defrostControl);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setMaximumOutdoorDrybulbTemperatureforDefrostOperation(double maximumOutdoorDrybulbTemperatureforDefrostOperation) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation, maximumOutdoorDrybulbTemperatureforDefrostOperation);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setWaterCondenserVolumeFlowRate(boost::optional<double> waterCondenserVolumeFlowRate) {
    bool result(false);
    if (waterCondenserVolumeFlowRate) {
      result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::WaterCondenserVolumeFlowRate, waterCondenserVolumeFlowRate.get());
    }
    OS_ASSERT(result);
    return result;
  }

  void AirConditionerVariableRefrigerantFlow_Impl::autosizeWaterCondenserVolumeFlowRate() {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::WaterCondenserVolumeFlowRate, "autosize");
    OS_ASSERT(result);
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
    OS_ASSERT(result);
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
    OS_ASSERT(result);
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setFuelType(std::string fuelType) {
    bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::FuelType, fuelType);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setMinimumOutdoorTemperatureinHeatRecoveryMode(double minimumOutdoorTemperatureinHeatRecoveryMode) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumOutdoorTemperatureinHeatRecoveryMode, minimumOutdoorTemperatureinHeatRecoveryMode);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setMaximumOutdoorTemperatureinHeatRecoveryMode(double maximumOutdoorTemperatureinHeatRecoveryMode) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorTemperatureinHeatRecoveryMode, maximumOutdoorTemperatureinHeatRecoveryMode);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatRecoveryCoolingCapacityModifierCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setInitialHeatRecoveryCoolingCapacityFraction(double initialHeatRecoveryCoolingCapacityFraction) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingCapacityFraction, initialHeatRecoveryCoolingCapacityFraction);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatRecoveryCoolingCapacityTimeConstant(double heatRecoveryCoolingCapacityTimeConstant) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityTimeConstant, heatRecoveryCoolingCapacityTimeConstant);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatRecoveryCoolingEnergyModifierCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setInitialHeatRecoveryCoolingEnergyFraction(double initialHeatRecoveryCoolingEnergyFraction) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingEnergyFraction, initialHeatRecoveryCoolingEnergyFraction);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatRecoveryCoolingEnergyTimeConstant(double heatRecoveryCoolingEnergyTimeConstant) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyTimeConstant, heatRecoveryCoolingEnergyTimeConstant);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatRecoveryHeatingCapacityModifierCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setInitialHeatRecoveryHeatingCapacityFraction(double initialHeatRecoveryHeatingCapacityFraction) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryHeatingCapacityFraction, initialHeatRecoveryHeatingCapacityFraction);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatRecoveryHeatingCapacityTimeConstant(double heatRecoveryHeatingCapacityTimeConstant) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityTimeConstant, heatRecoveryHeatingCapacityTimeConstant);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatRecoveryHeatingEnergyModifierCurve(const boost::optional<Curve>& curve) {
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
    OS_ASSERT(result);
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setInitialHeatRecoveryHeatingEnergyFraction(double initialHeatRecoveryHeatingEnergyFraction) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryHeatingEnergyFraction, initialHeatRecoveryHeatingEnergyFraction);
    OS_ASSERT(result);
    return result;
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setHeatRecoveryHeatingEnergyTimeConstant(double heatRecoveryHeatingEnergyTimeConstant) {
    bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyTimeConstant, heatRecoveryHeatingEnergyTimeConstant);
    OS_ASSERT(result);
    return result;
  }

  boost::optional<Schedule> AirConditionerVariableRefrigerantFlow_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirConditioner_VariableRefrigerantFlowFields::AvailabilitySchedule);
  }

  unsigned AirConditionerVariableRefrigerantFlow_Impl::inletPort() const
  {
    return OS_AirConditioner_VariableRefrigerantFlowFields::CondenserInletNode;
  }

  unsigned AirConditionerVariableRefrigerantFlow_Impl::outletPort() const
  {
    return OS_AirConditioner_VariableRefrigerantFlowFields::CondenserOutletNode;
  }

  ModelObjectList AirConditionerVariableRefrigerantFlow_Impl::vrfModelObjectList() const
  {
    boost::optional<ModelObjectList> mo = getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_AirConditioner_VariableRefrigerantFlowFields::ZoneTerminalUnitList);

    Q_ASSERT(mo);

    return mo.get();
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::setVRFModelObjectList(const ModelObjectList & modelObjectList)
  {
    return setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::ZoneTerminalUnitList,modelObjectList.handle());
  }

  void AirConditionerVariableRefrigerantFlow_Impl::addTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow & vrf)
  {
    vrfModelObjectList().addModelObject(vrf);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::removeTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow & vrf)
  {
    vrfModelObjectList().removeModelObject(vrf);
  }

  void AirConditionerVariableRefrigerantFlow_Impl::removeAllTerminals()
  {
    vrfModelObjectList().removeAllModelObjects();
  }

  std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> AirConditionerVariableRefrigerantFlow_Impl::terminals() const
  {
    return subsetCastVector<ZoneHVACTerminalUnitVariableRefrigerantFlow>(vrfModelObjectList().modelObjects());
  }

  std::vector<ModelObject> AirConditionerVariableRefrigerantFlow_Impl::children() const
  {
    std::vector<ModelObject> result;

    boost::optional<ModelObject> curve;

    curve = coolingCapacityRatioModifierFunctionofLowTemperatureCurve();
    if(curve)
    {
      result.push_back(curve.get());
    }

    curve = coolingCapacityRatioBoundaryCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = coolingCapacityRatioModifierFunctionofHighTemperatureCurve();
    if( curve)
    {
      result.push_back(curve.get());
    }

    curve = coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = coolingEnergyInputRatioBoundaryCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = coolingCombinationRatioCorrectionFactorCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = coolingPartLoadFractionCorrelationCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = heatingCapacityRatioModifierFunctionofLowTemperatureCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = heatingCapacityRatioBoundaryCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = heatingCapacityRatioModifierFunctionofHighTemperatureCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = heatingEnergyInputRatioBoundaryCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = heatingCombinationRatioCorrectionFactorCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = heatingPartLoadFractionCorrelationCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = pipingCorrectionFactorforLengthinCoolingModeCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = pipingCorrectionFactorforLengthinHeatingModeCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = heatRecoveryCoolingCapacityModifierCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = heatRecoveryCoolingEnergyModifierCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = heatRecoveryHeatingCapacityModifierCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    curve = heatRecoveryHeatingEnergyModifierCurve();
    if( curve )
    {
      result.push_back(curve.get());
    }

    return result;
  }

  ModelObject AirConditionerVariableRefrigerantFlow_Impl::clone(Model model) const
  {
    AirConditionerVariableRefrigerantFlow airConditionerClone = StraightComponent_Impl::clone(model).cast<AirConditionerVariableRefrigerantFlow>();

    ModelObjectList modelObjectList(model);
    airConditionerClone.getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setVRFModelObjectList(modelObjectList);

    if( auto curve = coolingCapacityRatioModifierFunctionofLowTemperatureCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setCoolingCapacityRatioModifierFunctionofLowTemperatureCurve(clone);
    }

    if( auto curve = coolingCapacityRatioBoundaryCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setCoolingCapacityRatioBoundaryCurve(clone);
    }

    if( auto curve = coolingCapacityRatioModifierFunctionofHighTemperatureCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setCoolingCapacityRatioModifierFunctionofHighTemperatureCurve(clone);
    }

    if( auto curve = coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve(clone);
    }

    if( auto curve = coolingEnergyInputRatioBoundaryCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setCoolingEnergyInputRatioBoundaryCurve(clone);
    }

    if( auto curve = coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve(clone);
    }

    if( auto curve = coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(clone);
    }

    if( auto curve = coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(clone);
    }

    if( auto curve = coolingCombinationRatioCorrectionFactorCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setCoolingCombinationRatioCorrectionFactorCurve(clone);
    }

    if( auto curve = coolingPartLoadFractionCorrelationCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setCoolingPartLoadFractionCorrelationCurve(clone);
    }

    if( auto curve = heatingCapacityRatioModifierFunctionofLowTemperatureCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setHeatingCapacityRatioModifierFunctionofLowTemperatureCurve(clone);
    }

    if( auto curve = heatingCapacityRatioBoundaryCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setHeatingCapacityRatioBoundaryCurve(clone);
    }

    if( auto curve = heatingCapacityRatioModifierFunctionofHighTemperatureCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setHeatingCapacityRatioModifierFunctionofHighTemperatureCurve(clone);
    }

    if( auto curve = heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve(clone);
    }

    if( auto curve = heatingEnergyInputRatioBoundaryCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setHeatingEnergyInputRatioBoundaryCurve(clone);
    }

    if( auto curve = heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve(clone);
    }

    if( auto curve = heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(clone);
    }

    if( auto curve = heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(clone);
    }

    if( auto curve = heatingCombinationRatioCorrectionFactorCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setHeatingCombinationRatioCorrectionFactorCurve(clone);
    }

    if( auto curve = heatingPartLoadFractionCorrelationCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setHeatingPartLoadFractionCorrelationCurve(clone);
    }

    if( auto curve = pipingCorrectionFactorforLengthinCoolingModeCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setPipingCorrectionFactorforLengthinCoolingModeCurve(clone);
    }

    if( auto curve = pipingCorrectionFactorforLengthinHeatingModeCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setPipingCorrectionFactorforLengthinHeatingModeCurve(clone);
    }

    if( auto curve = heatRecoveryCoolingCapacityModifierCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setHeatRecoveryCoolingCapacityModifierCurve(clone);
    }

    if( auto curve = heatRecoveryCoolingEnergyModifierCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setHeatRecoveryCoolingEnergyModifierCurve(clone);
    }

    if( auto curve = heatRecoveryHeatingCapacityModifierCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setHeatRecoveryHeatingCapacityModifierCurve(clone);
    }

    if( auto curve = heatRecoveryHeatingEnergyModifierCurve() ) {
      auto clone = curve->clone(model).cast<Curve>();
      airConditionerClone.setHeatRecoveryHeatingEnergyModifierCurve(clone);
    }

    return airConditionerClone;
  }

  std::vector<openstudio::IdfObject> AirConditionerVariableRefrigerantFlow_Impl::remove()
  {
    vrfModelObjectList().remove();

    boost::optional<Curve> curve;

    curve = coolingCapacityRatioModifierFunctionofLowTemperatureCurve();
    if(curve)
    {
      curve->remove();
    }

    curve = coolingCapacityRatioBoundaryCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = coolingCapacityRatioModifierFunctionofHighTemperatureCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = coolingEnergyInputRatioBoundaryCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = coolingCombinationRatioCorrectionFactorCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = coolingPartLoadFractionCorrelationCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = heatingCapacityRatioModifierFunctionofLowTemperatureCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = heatingCapacityRatioBoundaryCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = heatingCapacityRatioModifierFunctionofHighTemperatureCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = heatingEnergyInputRatioBoundaryCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = heatingCombinationRatioCorrectionFactorCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = heatingPartLoadFractionCorrelationCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = pipingCorrectionFactorforLengthinCoolingModeCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = pipingCorrectionFactorforLengthinCoolingModeCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = heatRecoveryCoolingCapacityModifierCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = heatRecoveryCoolingEnergyModifierCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = heatRecoveryHeatingCapacityModifierCurve();
    if( curve )
    {
      curve->remove();
    }

    curve = heatRecoveryHeatingEnergyModifierCurve();
    if( curve )
    {
      curve->remove();
    }

    return StraightComponent_Impl::remove();
  }

  bool AirConditionerVariableRefrigerantFlow_Impl::addToNode(Node & node)
  {
    // At this time we cannot liquid cool
    return false;
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlow_Impl::autosizedRatedTotalCoolingCapacity() const {
    return getAutosizedValue("Design Size Rated Total Cooling Capacity (gross)", "W");
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlow_Impl::autosizedRatedTotalHeatingCapacity() const {
    return getAutosizedValue("Design Size Rated Total Heating Capacity", "W");
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlow_Impl::autosizedResistiveDefrostHeaterCapacity() const {
    return getAutosizedValue("Design Size Resistive Defrost Heater Capacity", "");
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlow_Impl::autosizedWaterCondenserVolumeFlowRate() const {
    return getAutosizedValue("Design Size Water Condenser Volume Flow Rate", "m3/s");
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlow_Impl::autosizedEvaporativeCondenserAirFlowRate() const {
    return getAutosizedValue("Design Size Evaporative Condenser Air Flow Rate", "m3/s");
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlow_Impl::autosizedEvaporativeCondenserPumpRatedPowerConsumption() const {
    return getAutosizedValue("Design Size Evaporative Condenser Pump Rated Power Consumption", "W");
  }

  void AirConditionerVariableRefrigerantFlow_Impl::autosize() {
    autosizeRatedTotalCoolingCapacity();
    autosizeRatedTotalHeatingCapacity();
    autosizeResistiveDefrostHeaterCapacity();
    autosizeWaterCondenserVolumeFlowRate();
    autosizeEvaporativeCondenserAirFlowRate();
    autosizeEvaporativeCondenserPumpRatedPowerConsumption();
  }

  void AirConditionerVariableRefrigerantFlow_Impl::applySizingValues() {
    boost::optional<double> val;
    val = autosizedRatedTotalCoolingCapacity();
    if (val) {
      setRatedTotalCoolingCapacity(val.get());
    }

    val = autosizedRatedTotalHeatingCapacity();
    if (val) {
      setRatedTotalHeatingCapacity(val.get());
    }

    val = autosizedResistiveDefrostHeaterCapacity();
    if (val) {
      setResistiveDefrostHeaterCapacity(val.get());
    }

    val = autosizedWaterCondenserVolumeFlowRate();
    if (val) {
      setWaterCondenserVolumeFlowRate(val.get());
    }

    val = autosizedEvaporativeCondenserAirFlowRate();
    if (val) {
      setEvaporativeCondenserAirFlowRate(val.get());
    }

    val = autosizedEvaporativeCondenserPumpRatedPowerConsumption();
    if (val) {
      setEvaporativeCondenserPumpRatedPowerConsumption(val.get());
    }

  }

  std::vector<EMSActuatorNames> AirConditionerVariableRefrigerantFlow_Impl::emsActuatorNames() const {
    std::vector<EMSActuatorNames> actuators{ { "Variable Refrigerant Flow Heat Pump", "Operating Mode" } };
    return actuators;
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlow_Impl::emsInternalVariableNames() const {
    std::vector<std::string> types;
    return types;
  }

} // detail


AirConditionerVariableRefrigerantFlow::AirConditionerVariableRefrigerantFlow(const Model& model)
  : StraightComponent(AirConditionerVariableRefrigerantFlow::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>());

  Schedule schedule = model.alwaysOnDiscreteSchedule();

  setAvailabilitySchedule(schedule);

  autosizeRatedTotalCoolingCapacity();

  setRatedCoolingCOP(3.3);

  setMinimumOutdoorTemperatureinCoolingMode(-5.0);

  setMaximumOutdoorTemperatureinCoolingMode(43.0);

  autosizeRatedTotalHeatingCapacity();

  setRatedTotalHeatingCapacitySizingRatio(1.0);

  setRatedHeatingCOP(3.5);

  setMinimumOutdoorTemperatureinHeatingMode(-20.0);

  setMaximumOutdoorTemperatureinHeatingMode(30.0);

  setHeatingPerformanceCurveOutdoorTemperatureType("WetBulbTemperature");

  setMinimumHeatPumpPartLoadRatio(0.25);

  setMasterThermostatPriorityControlType("LoadPriority");

  setHeatPumpWasteHeatRecovery(false);

  setEquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode(30.0);

  setVerticalHeightusedforPipingCorrectionFactor(10.0);

  setPipingCorrectionFactorforHeightinCoolingModeCoefficient(-0.000386);

  setEquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode(30.0);

  setPipingCorrectionFactorforHeightinHeatingModeCoefficient(0.0);

  setCrankcaseHeaterPowerperCompressor(15.0);

  setNumberofCompressors(3);

  setRatioofCompressorSizetoTotalCompressorCapacity(0.33);

  setMaximumOutdoorDrybulbTemperatureforCrankcaseHeater(7.0);

  setDefrostStrategy("Resistive");

  setDefrostControl("Timed");

  setDefrostTimePeriodFraction(0.058333);

  setResistiveDefrostHeaterCapacity(0.0000001);

  setMaximumOutdoorDrybulbTemperatureforDefrostOperation(7.0);

  autosizeWaterCondenserVolumeFlowRate();

  setEvaporativeCondenserEffectiveness(0.9);

  autosizeEvaporativeCondenserAirFlowRate();

  autosizeEvaporativeCondenserPumpRatedPowerConsumption();

  setBasinHeaterCapacity(0.0);

  setBasinHeaterSetpointTemperature(2.0);

  setFuelType("Electricity");

  setMinimumOutdoorTemperatureinHeatRecoveryMode(0.0);

  setMaximumOutdoorTemperatureinHeatRecoveryMode(20.0);

  setInitialHeatRecoveryCoolingCapacityFraction(0.5);

  setHeatRecoveryCoolingCapacityTimeConstant(0.083);

  setInitialHeatRecoveryCoolingEnergyFraction(1.0);

  setHeatRecoveryCoolingEnergyTimeConstant(0.0);

  setInitialHeatRecoveryHeatingCapacityFraction(0.5);

  setHeatRecoveryHeatingCapacityTimeConstant(0.083);

  setInitialHeatRecoveryHeatingEnergyFraction(0.5);

  setHeatRecoveryHeatingEnergyTimeConstant(0.0);

  CurveBiquadratic vrfCoolCapFT(model);
  vrfCoolCapFT.setName(name().get() + " VRFCoolCapFT");
  vrfCoolCapFT.setCoefficient1Constant(0.576882692);
  vrfCoolCapFT.setCoefficient2x(0.017447952);
  vrfCoolCapFT.setCoefficient3xPOW2(0.000583269);
  vrfCoolCapFT.setCoefficient4y(-1.76324E-06);
  vrfCoolCapFT.setCoefficient5yPOW2(-7.474E-09);
  vrfCoolCapFT.setCoefficient6xTIMESY(-1.30413E-07);
  vrfCoolCapFT.setMinimumValueofx(15.0);
  vrfCoolCapFT.setMaximumValueofx(24.0);
  vrfCoolCapFT.setMinimumValueofy(-5.0);
  vrfCoolCapFT.setMaximumValueofy(23);
  setCoolingCapacityRatioModifierFunctionofLowTemperatureCurve(vrfCoolCapFT);

  CurveCubic vrfCoolCapFTBoundary(model);
  vrfCoolCapFTBoundary.setName(name().get() + " VRFCoolCapFTBoundary");
  vrfCoolCapFTBoundary.setCoefficient1Constant(25.73);
  vrfCoolCapFTBoundary.setCoefficient2x(-0.03150043);
  vrfCoolCapFTBoundary.setCoefficient3xPOW2(-0.01416595);
  vrfCoolCapFTBoundary.setCoefficient4xPOW3(0);
  vrfCoolCapFTBoundary.setMinimumValueofx(11);
  vrfCoolCapFTBoundary.setMaximumValueofx(30);
  setCoolingCapacityRatioBoundaryCurve(vrfCoolCapFTBoundary);

  CurveBiquadratic vrfCoolCapFTHi(model);
  vrfCoolCapFTHi.setName(name().get() +  " VRFCoolCapFTHi");
  vrfCoolCapFTHi.setCoefficient1Constant(0.6867358);
  vrfCoolCapFTHi.setCoefficient2x(0.0207631);
  vrfCoolCapFTHi.setCoefficient3xPOW2(0.0005447);
  vrfCoolCapFTHi.setCoefficient4y(-0.0016218);
  vrfCoolCapFTHi.setCoefficient5yPOW2(-4.259E-07);
  vrfCoolCapFTHi.setCoefficient6xTIMESY(-0.0003392);
  vrfCoolCapFTHi.setMinimumValueofx(15);
  vrfCoolCapFTHi.setMaximumValueofx(24);
  vrfCoolCapFTHi.setMinimumValueofy(16);
  vrfCoolCapFTHi.setMaximumValueofy(43);
  setCoolingCapacityRatioModifierFunctionofHighTemperatureCurve(vrfCoolCapFTHi);

  CurveBiquadratic vrfCoolEIRFT(model);
  vrfCoolEIRFT.setName(name().get() +  " VRFCoolEIRFT");
  vrfCoolEIRFT.setCoefficient1Constant(0.989010541);
  vrfCoolEIRFT.setCoefficient2x(-0.02347967);
  vrfCoolEIRFT.setCoefficient3xPOW2(0.000199711);
  vrfCoolEIRFT.setCoefficient4y(0.005968336);
  vrfCoolEIRFT.setCoefficient5yPOW2(-1.0289E-07);
  vrfCoolEIRFT.setCoefficient6xTIMESY(-0.00015686);
  vrfCoolEIRFT.setMinimumValueofx(15);
  vrfCoolEIRFT.setMaximumValueofx(24);
  vrfCoolEIRFT.setMinimumValueofy(-5);
  vrfCoolEIRFT.setMaximumValueofy(23);
  setCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve(vrfCoolEIRFT);

  CurveCubic vrfCoolEIRFTBoundary(model);
  vrfCoolEIRFTBoundary.setName(name().get() + " VRFCoolEIRFTBoundary");
  vrfCoolEIRFTBoundary.setCoefficient1Constant(25.73473775);
  vrfCoolEIRFTBoundary.setCoefficient2x(-0.03150043);
  vrfCoolEIRFTBoundary.setCoefficient3xPOW2(-0.01416595);
  vrfCoolEIRFTBoundary.setCoefficient4xPOW3(0);
  vrfCoolEIRFTBoundary.setMinimumValueofx(15);
  vrfCoolEIRFTBoundary.setMaximumValueofx(24);
  setCoolingEnergyInputRatioBoundaryCurve(vrfCoolEIRFTBoundary);

  CurveBiquadratic vrfCoolEIRFTHi(model);
  vrfCoolEIRFTHi.setName(name().get() + " VRFCoolEIRFTHi");
  vrfCoolEIRFTHi.setCoefficient1Constant(-1.4395110176);
  vrfCoolEIRFTHi.setCoefficient2x(0.1619850459);
  vrfCoolEIRFTHi.setCoefficient3xPOW2(-0.0034911781);
  vrfCoolEIRFTHi.setCoefficient4y(0.0269442645);
  vrfCoolEIRFTHi.setCoefficient5yPOW2(0.0001346163);
  vrfCoolEIRFTHi.setCoefficient6xTIMESY(-0.0006714941);
  vrfCoolEIRFTHi.setMinimumValueofx(15);
  vrfCoolEIRFTHi.setMaximumValueofx(23.9);
  vrfCoolEIRFTHi.setMinimumValueofy(16.8);
  vrfCoolEIRFTHi.setMaximumValueofy(43.3);
  setCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve(vrfCoolEIRFTHi);

  CurveCubic coolingEIRLowPLR(model);
  coolingEIRLowPLR.setName(name().get() + " CoolingEIRLowPLR");
  coolingEIRLowPLR.setCoefficient1Constant(0.4541226192);
  coolingEIRLowPLR.setCoefficient2x(-0.1729687081);
  coolingEIRLowPLR.setCoefficient3xPOW2(1.0828661347);
  coolingEIRLowPLR.setCoefficient4xPOW3(-0.3618480897);
  coolingEIRLowPLR.setMinimumValueofx(0.5);
  coolingEIRLowPLR.setMaximumValueofx(1.0);
  setCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(coolingEIRLowPLR);

  CurveCubic coolingEIRHiPLR(model);
  coolingEIRHiPLR.setName(name().get() + " CoolingEIRHiPLR");
  coolingEIRHiPLR.setCoefficient1Constant(1.0);
  coolingEIRHiPLR.setCoefficient2x(0.0);
  coolingEIRHiPLR.setCoefficient3xPOW2(0.0);
  coolingEIRHiPLR.setCoefficient4xPOW3(0.0);
  coolingEIRHiPLR.setMinimumValueofx(1.0);
  coolingEIRHiPLR.setMaximumValueofx(1.5);
  setCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(coolingEIRHiPLR);

  CurveCubic coolingCombRatio(model);
  coolingCombRatio.setName(name().get() + " CoolingCombRatio");
  coolingCombRatio.setCoefficient1Constant(0.576593263);
  coolingCombRatio.setCoefficient2x(0.6349408697);
  coolingCombRatio.setCoefficient3xPOW2(-0.3076093963);
  coolingCombRatio.setCoefficient4xPOW3(0.0960752636);
  coolingCombRatio.setMinimumValueofx(1);
  coolingCombRatio.setMaximumValueofx(1.5);
  setCoolingCombinationRatioCorrectionFactorCurve(coolingCombRatio);

  CurveCubic vrfCPLFFPLR(model);
  vrfCPLFFPLR.setName(name().get() + " VRFCPLFFPLR");
  vrfCPLFFPLR.setCoefficient1Constant(0.85);
  vrfCPLFFPLR.setCoefficient2x(0.15);
  vrfCPLFFPLR.setCoefficient3xPOW2(0.0);
  vrfCPLFFPLR.setCoefficient4xPOW3(0.0);
  vrfCPLFFPLR.setMinimumValueofx(0.0);
  vrfCPLFFPLR.setMinimumValueofx(1.0);
  setCoolingPartLoadFractionCorrelationCurve(vrfCPLFFPLR);

  CurveBiquadratic vrfHeatCapFT(model);
  vrfHeatCapFT.setName(name().get() + " VRFHeatCapFT");
  vrfHeatCapFT.setCoefficient1Constant(1.012090154);
  vrfHeatCapFT.setCoefficient2x(-0.0012467553);
  vrfHeatCapFT.setCoefficient3xPOW2(-0.0001271847);
  vrfHeatCapFT.setCoefficient4y(0.0267564328);
  vrfHeatCapFT.setCoefficient5yPOW2(-0.0000004986);
  vrfHeatCapFT.setCoefficient6xTIMESY(-0.0002635239);
  vrfHeatCapFT.setMinimumValueofx(21.1);
  vrfHeatCapFT.setMaximumValueofx(27.2);
  vrfHeatCapFT.setMinimumValueofy(-20);
  vrfHeatCapFT.setMaximumValueofy(3.33);
  setHeatingCapacityRatioModifierFunctionofLowTemperatureCurve(vrfHeatCapFT);

  CurveCubic vrfHeatCapFTBoundary(model);
  vrfHeatCapFTBoundary.setName(name().get() + " VRFHeatCapFTBoundary");
  vrfHeatCapFTBoundary.setCoefficient1Constant(58.5770);
  vrfHeatCapFTBoundary.setCoefficient2x(-3.0255);
  vrfHeatCapFTBoundary.setCoefficient3xPOW2(0.0193);
  vrfHeatCapFTBoundary.setCoefficient4xPOW3(0.0);
  vrfHeatCapFTBoundary.setMinimumValueofx(15);
  vrfHeatCapFTBoundary.setMaximumValueofx(23.9);
  setHeatingCapacityRatioBoundaryCurve(vrfHeatCapFTBoundary);

  CurveBiquadratic vrfHeatCapFTHi(model);
  vrfHeatCapFTHi.setName(name().get() + " VRFHeatCapFTHi");
  vrfHeatCapFTHi.setCoefficient1Constant(2.5859872368);
  vrfHeatCapFTHi.setCoefficient2x(-0.0953227101);
  vrfHeatCapFTHi.setCoefficient3xPOW2(0.0009553288);
  vrfHeatCapFTHi.setCoefficient4y(0);
  vrfHeatCapFTHi.setCoefficient5yPOW2(0);
  vrfHeatCapFTHi.setCoefficient6xTIMESY(0);
  vrfHeatCapFTHi.setMinimumValueofx(21.1);
  vrfHeatCapFTHi.setMaximumValueofx(27.2);
  vrfHeatCapFTHi.setMinimumValueofy(-9.44);
  vrfHeatCapFTHi.setMaximumValueofy(15);
  setHeatingCapacityRatioModifierFunctionofHighTemperatureCurve(vrfHeatCapFTHi);

  CurveBiquadratic vrfHeatEIRFT(model);
  vrfHeatEIRFT.setName(name().get() + " VRFHeatEIRFT");
  vrfHeatEIRFT.setCoefficient1Constant(0.7224292683);
  vrfHeatEIRFT.setCoefficient2x(0.0034566628);
  vrfHeatEIRFT.setCoefficient3xPOW2(0.0006507028);
  vrfHeatEIRFT.setCoefficient4y(-0.0026435362);
  vrfHeatEIRFT.setCoefficient5yPOW2(0.0012464766);
  vrfHeatEIRFT.setCoefficient6xTIMESY(-0.0001009161);
  vrfHeatEIRFT.setMinimumValueofx(21.1);
  vrfHeatEIRFT.setMaximumValueofx(27.2);
  vrfHeatEIRFT.setMinimumValueofy(-20);
  vrfHeatEIRFT.setMaximumValueofy(3.33);
  setHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve(vrfHeatEIRFT);

  CurveCubic vrfHeatEIRFTBoundary(model);
  vrfHeatEIRFTBoundary.setName(name().get() + " VRFHeatEIRFTBoundary");
  vrfHeatEIRFTBoundary.setCoefficient1Constant(58.5770);
  vrfHeatEIRFTBoundary.setCoefficient2x(-3.0255);
  vrfHeatEIRFTBoundary.setCoefficient3xPOW2(0.0193);
  vrfHeatEIRFTBoundary.setCoefficient4xPOW3(0.0);
  vrfHeatEIRFTBoundary.setMinimumValueofx(15);
  vrfHeatEIRFTBoundary.setMaximumValueofx(23.9);
  setHeatingEnergyInputRatioBoundaryCurve(vrfHeatEIRFTBoundary);

  CurveBiquadratic vrfHeatEIRFTHi(model);
  vrfHeatEIRFTHi.setName(name().get() + " VRFHeatEIRFTHi");
  vrfHeatEIRFTHi.setCoefficient1Constant(1.3885703646);
  vrfHeatEIRFTHi.setCoefficient2x(-0.0229771462);
  vrfHeatEIRFTHi.setCoefficient3xPOW2(0.000537274);
  vrfHeatEIRFTHi.setCoefficient4y(-0.0273936962);
  vrfHeatEIRFTHi.setCoefficient5yPOW2(0.0004030426);
  vrfHeatEIRFTHi.setCoefficient6xTIMESY(-0.000059786);
  vrfHeatEIRFTHi.setMinimumValueofx(21.1);
  vrfHeatEIRFTHi.setMaximumValueofx(27.2);
  setHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve(vrfHeatEIRFTHi);

  CurveCubic heatingEIRLowPLR(model);
  heatingEIRLowPLR.setName(name().get() + " HeatingEIRLowPLR");
  heatingEIRLowPLR.setCoefficient1Constant(0.3924742025);
  heatingEIRLowPLR.setCoefficient2x(0.076016374);
  heatingEIRLowPLR.setCoefficient3xPOW2(0.6983235783);
  heatingEIRLowPLR.setCoefficient4xPOW3(-0.1688407813);
  heatingEIRLowPLR.setMinimumValueofx(0.5);
  heatingEIRLowPLR.setMaximumValueofx(1);
  setHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(heatingEIRLowPLR);

  CurveCubic heatingEIRHiPLR(model);
  heatingEIRHiPLR.setName(name().get() + " HeatingEIRHiPLR");
  heatingEIRHiPLR.setCoefficient1Constant(1.0);
  heatingEIRHiPLR.setCoefficient2x(0.0);
  heatingEIRHiPLR.setCoefficient3xPOW2(0.0);
  heatingEIRHiPLR.setCoefficient4xPOW3(0.0);
  heatingEIRHiPLR.setMinimumValueofx(1.0);
  heatingEIRHiPLR.setMaximumValueofx(1.5);
  setHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(heatingEIRHiPLR);

  CurveCubic heatingCombRatio(model);
  heatingCombRatio.setName(name().get() + " HeatingCombRatio");
  heatingCombRatio.setCoefficient1Constant(0.7667196604);
  heatingCombRatio.setCoefficient2x(0.2617302019);
  heatingCombRatio.setCoefficient3xPOW2(-0.0159110245);
  heatingCombRatio.setCoefficient4xPOW3(-0.0125388376);
  heatingCombRatio.setMinimumValueofx(1);
  heatingCombRatio.setMaximumValueofx(1.5);
  setHeatingCombinationRatioCorrectionFactorCurve(heatingCombRatio);

  setHeatingPartLoadFractionCorrelationCurve(vrfCPLFFPLR);

  CurveBiquadratic coolingLengthCorrectionFactor(model);
  coolingLengthCorrectionFactor.setName(name().get() + " CoolingLengthCorrectionFactor");
  coolingLengthCorrectionFactor.setCoefficient1Constant(2.0388158625);
  coolingLengthCorrectionFactor.setCoefficient2x(-0.0024260645);
  coolingLengthCorrectionFactor.setCoefficient3xPOW2(0.0000035512);
  coolingLengthCorrectionFactor.setCoefficient4y(-1.6858129772);
  coolingLengthCorrectionFactor.setCoefficient5yPOW2(0.668703358);
  coolingLengthCorrectionFactor.setCoefficient6xTIMESY(-0.000045706);
  coolingLengthCorrectionFactor.setMinimumValueofx(7.62);
  coolingLengthCorrectionFactor.setMaximumValueofx(182.88);
  coolingLengthCorrectionFactor.setMinimumValueofy(0.8);
  coolingLengthCorrectionFactor.setMaximumValueofy(1.5);
  setPipingCorrectionFactorforLengthinCoolingModeCurve(coolingLengthCorrectionFactor);

  ModelObjectList vrfModelObjectList(model);
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setVRFModelObjectList(vrfModelObjectList);
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

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::coolingCapacityRatioModifierFunctionofLowTemperatureCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->coolingCapacityRatioModifierFunctionofLowTemperatureCurve();
}

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::coolingCapacityRatioBoundaryCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->coolingCapacityRatioBoundaryCurve();
}

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::coolingCapacityRatioModifierFunctionofHighTemperatureCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->coolingCapacityRatioModifierFunctionofHighTemperatureCurve();
}

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
}

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::coolingEnergyInputRatioBoundaryCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->coolingEnergyInputRatioBoundaryCurve();
}

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
}

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
}

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
}

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::coolingCombinationRatioCorrectionFactorCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->coolingCombinationRatioCorrectionFactorCurve();
}

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::coolingPartLoadFractionCorrelationCurve() const {
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

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::heatingCapacityRatioModifierFunctionofLowTemperatureCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatingCapacityRatioModifierFunctionofLowTemperatureCurve();
}

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::heatingCapacityRatioBoundaryCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatingCapacityRatioBoundaryCurve();
}

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::heatingCapacityRatioModifierFunctionofHighTemperatureCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatingCapacityRatioModifierFunctionofHighTemperatureCurve();
}

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
}

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::heatingEnergyInputRatioBoundaryCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatingEnergyInputRatioBoundaryCurve();
}

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
}

std::string AirConditionerVariableRefrigerantFlow::heatingPerformanceCurveOutdoorTemperatureType() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatingPerformanceCurveOutdoorTemperatureType();
}

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
}

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
}

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::heatingCombinationRatioCorrectionFactorCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatingCombinationRatioCorrectionFactorCurve();
}

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::heatingPartLoadFractionCorrelationCurve() const {
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

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::pipingCorrectionFactorforLengthinCoolingModeCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->pipingCorrectionFactorforLengthinCoolingModeCurve();
}

double AirConditionerVariableRefrigerantFlow::pipingCorrectionFactorforHeightinCoolingModeCoefficient() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->pipingCorrectionFactorforHeightinCoolingModeCoefficient();
}

double AirConditionerVariableRefrigerantFlow::equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode();
}

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::pipingCorrectionFactorforLengthinHeatingModeCurve() const {
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

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::defrostEnergyInputRatioModifierFunctionofTemperatureCurve() const {
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

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::heatRecoveryCoolingCapacityModifierCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatRecoveryCoolingCapacityModifierCurve();
}

double AirConditionerVariableRefrigerantFlow::initialHeatRecoveryCoolingCapacityFraction() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->initialHeatRecoveryCoolingCapacityFraction();
}

double AirConditionerVariableRefrigerantFlow::heatRecoveryCoolingCapacityTimeConstant() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatRecoveryCoolingCapacityTimeConstant();
}

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::heatRecoveryCoolingEnergyModifierCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatRecoveryCoolingEnergyModifierCurve();
}

double AirConditionerVariableRefrigerantFlow::initialHeatRecoveryCoolingEnergyFraction() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->initialHeatRecoveryCoolingEnergyFraction();
}

double AirConditionerVariableRefrigerantFlow::heatRecoveryCoolingEnergyTimeConstant() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatRecoveryCoolingEnergyTimeConstant();
}

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::heatRecoveryHeatingCapacityModifierCurve() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatRecoveryHeatingCapacityModifierCurve();
}

double AirConditionerVariableRefrigerantFlow::initialHeatRecoveryHeatingCapacityFraction() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->initialHeatRecoveryHeatingCapacityFraction();
}

double AirConditionerVariableRefrigerantFlow::heatRecoveryHeatingCapacityTimeConstant() const {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->heatRecoveryHeatingCapacityTimeConstant();
}

boost::optional<Curve> AirConditionerVariableRefrigerantFlow::heatRecoveryHeatingEnergyModifierCurve() const {
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

bool AirConditionerVariableRefrigerantFlow::setMinimumOutdoorTemperatureinCoolingMode(double minimumOutdoorTemperatureinCoolingMode) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setMinimumOutdoorTemperatureinCoolingMode(minimumOutdoorTemperatureinCoolingMode);
}

bool AirConditionerVariableRefrigerantFlow::setMaximumOutdoorTemperatureinCoolingMode(double maximumOutdoorTemperatureinCoolingMode) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setMaximumOutdoorTemperatureinCoolingMode(maximumOutdoorTemperatureinCoolingMode);
}

bool AirConditionerVariableRefrigerantFlow::setCoolingCapacityRatioModifierFunctionofLowTemperatureCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setCoolingCapacityRatioModifierFunctionofLowTemperatureCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetCoolingCapacityRatioModifierFunctionofLowTemperatureCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetCoolingCapacityRatioModifierFunctionofLowTemperatureCurve();
}

bool AirConditionerVariableRefrigerantFlow::setCoolingCapacityRatioBoundaryCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setCoolingCapacityRatioBoundaryCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetCoolingCapacityRatioBoundaryCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetCoolingCapacityRatioBoundaryCurve();
}

bool AirConditionerVariableRefrigerantFlow::setCoolingCapacityRatioModifierFunctionofHighTemperatureCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setCoolingCapacityRatioModifierFunctionofHighTemperatureCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetCoolingCapacityRatioModifierFunctionofHighTemperatureCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetCoolingCapacityRatioModifierFunctionofHighTemperatureCurve();
}

bool AirConditionerVariableRefrigerantFlow::setCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
}

bool AirConditionerVariableRefrigerantFlow::setCoolingEnergyInputRatioBoundaryCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setCoolingEnergyInputRatioBoundaryCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetCoolingEnergyInputRatioBoundaryCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetCoolingEnergyInputRatioBoundaryCurve();
}

bool AirConditionerVariableRefrigerantFlow::setCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
}

bool AirConditionerVariableRefrigerantFlow::setCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
}

bool AirConditionerVariableRefrigerantFlow::setCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
}

bool AirConditionerVariableRefrigerantFlow::setCoolingCombinationRatioCorrectionFactorCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setCoolingCombinationRatioCorrectionFactorCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetCoolingCombinationRatioCorrectionFactorCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetCoolingCombinationRatioCorrectionFactorCurve();
}

bool AirConditionerVariableRefrigerantFlow::setCoolingPartLoadFractionCorrelationCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setCoolingPartLoadFractionCorrelationCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetCoolingPartLoadFractionCorrelationCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetCoolingPartLoadFractionCorrelationCurve();
}

bool AirConditionerVariableRefrigerantFlow::setRatedTotalHeatingCapacity(double ratedTotalHeatingCapacity) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setRatedTotalHeatingCapacity(ratedTotalHeatingCapacity);
}

void AirConditionerVariableRefrigerantFlow::autosizeRatedTotalHeatingCapacity() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->autosizeRatedTotalHeatingCapacity();
}

bool AirConditionerVariableRefrigerantFlow::setRatedTotalHeatingCapacitySizingRatio(double ratedTotalHeatingCapacitySizingRatio) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setRatedTotalHeatingCapacitySizingRatio(ratedTotalHeatingCapacitySizingRatio);
}

bool AirConditionerVariableRefrigerantFlow::setRatedHeatingCOP(double ratedHeatingCOP) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setRatedHeatingCOP(ratedHeatingCOP);
}

bool AirConditionerVariableRefrigerantFlow::setMinimumOutdoorTemperatureinHeatingMode(double minimumOutdoorTemperatureinHeatingMode) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setMinimumOutdoorTemperatureinHeatingMode(minimumOutdoorTemperatureinHeatingMode);
}

bool AirConditionerVariableRefrigerantFlow::setMaximumOutdoorTemperatureinHeatingMode(double maximumOutdoorTemperatureinHeatingMode) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setMaximumOutdoorTemperatureinHeatingMode(maximumOutdoorTemperatureinHeatingMode);
}

bool AirConditionerVariableRefrigerantFlow::setHeatingCapacityRatioModifierFunctionofLowTemperatureCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatingCapacityRatioModifierFunctionofLowTemperatureCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatingCapacityRatioModifierFunctionofLowTemperatureCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatingCapacityRatioModifierFunctionofLowTemperatureCurve();
}

bool AirConditionerVariableRefrigerantFlow::setHeatingCapacityRatioBoundaryCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatingCapacityRatioBoundaryCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatingCapacityRatioBoundaryCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatingCapacityRatioBoundaryCurve();
}

bool AirConditionerVariableRefrigerantFlow::setHeatingCapacityRatioModifierFunctionofHighTemperatureCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatingCapacityRatioModifierFunctionofHighTemperatureCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatingCapacityRatioModifierFunctionofHighTemperatureCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatingCapacityRatioModifierFunctionofHighTemperatureCurve();
}

bool AirConditionerVariableRefrigerantFlow::setHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
}

bool AirConditionerVariableRefrigerantFlow::setHeatingEnergyInputRatioBoundaryCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatingEnergyInputRatioBoundaryCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatingEnergyInputRatioBoundaryCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatingEnergyInputRatioBoundaryCurve();
}

bool AirConditionerVariableRefrigerantFlow::setHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
}

bool AirConditionerVariableRefrigerantFlow::setHeatingPerformanceCurveOutdoorTemperatureType(std::string heatingPerformanceCurveOutdoorTemperatureType) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatingPerformanceCurveOutdoorTemperatureType(heatingPerformanceCurveOutdoorTemperatureType);
}

bool AirConditionerVariableRefrigerantFlow::setHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
}

bool AirConditionerVariableRefrigerantFlow::setHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
}

bool AirConditionerVariableRefrigerantFlow::setHeatingCombinationRatioCorrectionFactorCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatingCombinationRatioCorrectionFactorCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatingCombinationRatioCorrectionFactorCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatingCombinationRatioCorrectionFactorCurve();
}

bool AirConditionerVariableRefrigerantFlow::setHeatingPartLoadFractionCorrelationCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatingPartLoadFractionCorrelationCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatingPartLoadFractionCorrelationCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatingPartLoadFractionCorrelationCurve();
}

bool AirConditionerVariableRefrigerantFlow::setMinimumHeatPumpPartLoadRatio(double minimumHeatPumpPartLoadRatio) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setMinimumHeatPumpPartLoadRatio(minimumHeatPumpPartLoadRatio);
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

bool AirConditionerVariableRefrigerantFlow::setHeatPumpWasteHeatRecovery(bool heatPumpWasteHeatRecovery) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatPumpWasteHeatRecovery(heatPumpWasteHeatRecovery);
}

bool AirConditionerVariableRefrigerantFlow::setEquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode(double equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setEquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode(equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode);
}

bool AirConditionerVariableRefrigerantFlow::setVerticalHeightusedforPipingCorrectionFactor(double verticalHeightusedforPipingCorrectionFactor) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setVerticalHeightusedforPipingCorrectionFactor(verticalHeightusedforPipingCorrectionFactor);
}

bool AirConditionerVariableRefrigerantFlow::setPipingCorrectionFactorforLengthinCoolingModeCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setPipingCorrectionFactorforLengthinCoolingModeCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetPipingCorrectionFactorforLengthinCoolingModeCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetPipingCorrectionFactorforLengthinCoolingModeCurve();
}

bool AirConditionerVariableRefrigerantFlow::setPipingCorrectionFactorforHeightinCoolingModeCoefficient(double pipingCorrectionFactorforHeightinCoolingModeCoefficient) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setPipingCorrectionFactorforHeightinCoolingModeCoefficient(pipingCorrectionFactorforHeightinCoolingModeCoefficient);
}

bool AirConditionerVariableRefrigerantFlow::setEquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode(double equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setEquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode(equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode);
}

bool AirConditionerVariableRefrigerantFlow::setPipingCorrectionFactorforLengthinHeatingModeCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setPipingCorrectionFactorforLengthinHeatingModeCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetPipingCorrectionFactorforLengthinHeatingModeCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetPipingCorrectionFactorforLengthinHeatingModeCurve();
}

bool AirConditionerVariableRefrigerantFlow::setPipingCorrectionFactorforHeightinHeatingModeCoefficient(double pipingCorrectionFactorforHeightinHeatingModeCoefficient) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setPipingCorrectionFactorforHeightinHeatingModeCoefficient(pipingCorrectionFactorforHeightinHeatingModeCoefficient);
}

bool AirConditionerVariableRefrigerantFlow::setCrankcaseHeaterPowerperCompressor(double crankcaseHeaterPowerperCompressor) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setCrankcaseHeaterPowerperCompressor(crankcaseHeaterPowerperCompressor);
}

bool AirConditionerVariableRefrigerantFlow::setNumberofCompressors(int numberofCompressors) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setNumberofCompressors(numberofCompressors);
}

bool AirConditionerVariableRefrigerantFlow::setRatioofCompressorSizetoTotalCompressorCapacity(double ratioofCompressorSizetoTotalCompressorCapacity) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setRatioofCompressorSizetoTotalCompressorCapacity(ratioofCompressorSizetoTotalCompressorCapacity);
}

bool AirConditionerVariableRefrigerantFlow::setMaximumOutdoorDrybulbTemperatureforCrankcaseHeater(double maximumOutdoorDrybulbTemperatureforCrankcaseHeater) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setMaximumOutdoorDrybulbTemperatureforCrankcaseHeater(maximumOutdoorDrybulbTemperatureforCrankcaseHeater);
}

bool AirConditionerVariableRefrigerantFlow::setDefrostStrategy(std::string defrostStrategy) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setDefrostStrategy(defrostStrategy);
}

bool AirConditionerVariableRefrigerantFlow::setDefrostControl(std::string defrostControl) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setDefrostControl(defrostControl);
}

bool AirConditionerVariableRefrigerantFlow::setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(const Curve& curve) {
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

bool AirConditionerVariableRefrigerantFlow::setMaximumOutdoorDrybulbTemperatureforDefrostOperation(double maximumOutdoorDrybulbTemperatureforDefrostOperation) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setMaximumOutdoorDrybulbTemperatureforDefrostOperation(maximumOutdoorDrybulbTemperatureforDefrostOperation);
}

bool AirConditionerVariableRefrigerantFlow::setWaterCondenserVolumeFlowRate(double waterCondenserVolumeFlowRate) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setWaterCondenserVolumeFlowRate(waterCondenserVolumeFlowRate);
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

bool AirConditionerVariableRefrigerantFlow::setMinimumOutdoorTemperatureinHeatRecoveryMode(double minimumOutdoorTemperatureinHeatRecoveryMode) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setMinimumOutdoorTemperatureinHeatRecoveryMode(minimumOutdoorTemperatureinHeatRecoveryMode);
}

bool AirConditionerVariableRefrigerantFlow::setMaximumOutdoorTemperatureinHeatRecoveryMode(double maximumOutdoorTemperatureinHeatRecoveryMode) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setMaximumOutdoorTemperatureinHeatRecoveryMode(maximumOutdoorTemperatureinHeatRecoveryMode);
}

bool AirConditionerVariableRefrigerantFlow::setHeatRecoveryCoolingCapacityModifierCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatRecoveryCoolingCapacityModifierCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatRecoveryCoolingCapacityModifierCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatRecoveryCoolingCapacityModifierCurve();
}

bool AirConditionerVariableRefrigerantFlow::setInitialHeatRecoveryCoolingCapacityFraction(double initialHeatRecoveryCoolingCapacityFraction) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setInitialHeatRecoveryCoolingCapacityFraction(initialHeatRecoveryCoolingCapacityFraction);
}

bool AirConditionerVariableRefrigerantFlow::setHeatRecoveryCoolingCapacityTimeConstant(double heatRecoveryCoolingCapacityTimeConstant) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatRecoveryCoolingCapacityTimeConstant(heatRecoveryCoolingCapacityTimeConstant);
}

bool AirConditionerVariableRefrigerantFlow::setHeatRecoveryCoolingEnergyModifierCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatRecoveryCoolingEnergyModifierCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatRecoveryCoolingEnergyModifierCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatRecoveryCoolingEnergyModifierCurve();
}

bool AirConditionerVariableRefrigerantFlow::setInitialHeatRecoveryCoolingEnergyFraction(double initialHeatRecoveryCoolingEnergyFraction) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setInitialHeatRecoveryCoolingEnergyFraction(initialHeatRecoveryCoolingEnergyFraction);
}

bool AirConditionerVariableRefrigerantFlow::setHeatRecoveryCoolingEnergyTimeConstant(double heatRecoveryCoolingEnergyTimeConstant) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatRecoveryCoolingEnergyTimeConstant(heatRecoveryCoolingEnergyTimeConstant);
}

bool AirConditionerVariableRefrigerantFlow::setHeatRecoveryHeatingCapacityModifierCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatRecoveryHeatingCapacityModifierCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatRecoveryHeatingCapacityModifierCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatRecoveryHeatingCapacityModifierCurve();
}

bool AirConditionerVariableRefrigerantFlow::setInitialHeatRecoveryHeatingCapacityFraction(double initialHeatRecoveryHeatingCapacityFraction) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setInitialHeatRecoveryHeatingCapacityFraction(initialHeatRecoveryHeatingCapacityFraction);
}

bool AirConditionerVariableRefrigerantFlow::setHeatRecoveryHeatingCapacityTimeConstant(double heatRecoveryHeatingCapacityTimeConstant) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatRecoveryHeatingCapacityTimeConstant(heatRecoveryHeatingCapacityTimeConstant);
}

bool AirConditionerVariableRefrigerantFlow::setHeatRecoveryHeatingEnergyModifierCurve(const Curve& curve) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatRecoveryHeatingEnergyModifierCurve(curve);
}

void AirConditionerVariableRefrigerantFlow::resetHeatRecoveryHeatingEnergyModifierCurve() {
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->resetHeatRecoveryHeatingEnergyModifierCurve();
}

bool AirConditionerVariableRefrigerantFlow::setInitialHeatRecoveryHeatingEnergyFraction(double initialHeatRecoveryHeatingEnergyFraction) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setInitialHeatRecoveryHeatingEnergyFraction(initialHeatRecoveryHeatingEnergyFraction);
}

bool AirConditionerVariableRefrigerantFlow::setHeatRecoveryHeatingEnergyTimeConstant(double heatRecoveryHeatingEnergyTimeConstant) {
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->setHeatRecoveryHeatingEnergyTimeConstant(heatRecoveryHeatingEnergyTimeConstant);
}

void AirConditionerVariableRefrigerantFlow::addTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow & vrf)
{
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->addTerminal(vrf);
}

void AirConditionerVariableRefrigerantFlow::removeTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow & vrf)
{
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->removeTerminal(vrf);
}

void AirConditionerVariableRefrigerantFlow::removeAllTerminals()
{
  getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->removeAllTerminals();
}

std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> AirConditionerVariableRefrigerantFlow::terminals() const
{
  return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->terminals();
}

/// @cond
AirConditionerVariableRefrigerantFlow::AirConditionerVariableRefrigerantFlow(std::shared_ptr<detail::AirConditionerVariableRefrigerantFlow_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

  boost::optional<double> AirConditionerVariableRefrigerantFlow::autosizedRatedTotalCoolingCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->autosizedRatedTotalCoolingCapacity();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlow::autosizedRatedTotalHeatingCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->autosizedRatedTotalHeatingCapacity();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlow::autosizedResistiveDefrostHeaterCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->autosizedResistiveDefrostHeaterCapacity();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlow::autosizedWaterCondenserVolumeFlowRate() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->autosizedWaterCondenserVolumeFlowRate();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlow::autosizedEvaporativeCondenserAirFlowRate() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->autosizedEvaporativeCondenserAirFlowRate();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlow::autosizedEvaporativeCondenserPumpRatedPowerConsumption() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlow_Impl>()->autosizedEvaporativeCondenserPumpRatedPowerConsumption();
  }

} // model
} // openstudio
