/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "AirConditionerVariableRefrigerantFlowFluidTemperatureControl.hpp"
#include "AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl.hpp"
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
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"
#include "PlantLoop_Impl.hpp"
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

    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirConditionerVariableRefrigerantFlowFluidTemperatureControl::iddObjectType());
    }

    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirConditionerVariableRefrigerantFlowFluidTemperatureControl::iddObjectType());
    }

    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl(
      const AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "VRF Heat Pump Total Cooling Rate", "VRF Heat Pump Total Heating Rate", "VRF Heat Pump Cooling COP", "VRF Heat Pump Heating COP",
        "VRF Heat Pump COP", "VRF Heat Pump Part Load Ratio", "VRF Heat Pump Runtime Fraction", "VRF Heat Pump Cycling Ratio",
        "VRF Heat Pump Operating Mode", "VRF Heat Pump Condenser Inlet Temperature", "VRF Heat Pump Maximum Capacity Cooling Rate",
        "VRF Heat Pump Maximum Capacity Heating Rate", "VRF Heat Pump Crankcase Heater Electricity Rate",
        "VRF Heat Pump Crankcase Heater Electricity Energy", "VRF Heat Pump Terminal Unit Heating Load Rate",
        "VRF Heat Pump Terminal Unit Cooling Load Rate",

        // TODO: add proper tests once the ModelObject return type is changed.
        // For now include all
        // Heat Recovery:
        "VRF Heat Pump Heat Recovery Status Change Multiplier", "VRF Heat Pump Simultaneous Cooling and Heating Efficiency",
        // Evap-cooled:
        "VRF Heat Pump Evaporative Condenser Water Use Volume", "VRF Heat Pump Evaporative Condenser Pump Electricity Rate",
        "VRF Heat Pump Evaporative Condenser Pump Electricity Energy", "VRF Heat Pump Basin Heater Electricity Rate",
        "VRF Heat Pump Basin Heater Electricity Energy", "VRF Heat Pump Heat Recovery Status Change Multiplier",
        // Water-cooled:
        "VRF Heat Pump Condenser Outlet Temperature", "VRF Heat Pump Condenser Mass Flow Rate", "VRF Heat Pump Condenser Heat Transfer Energy",
        "VRF Heat Pump Condenser Heat Transfer Rate",
        // Electric Fuel type (default):
        "VRF Heat Pump Cooling Electricity Rate", "VRF Heat Pump Cooling Electricity Energy", "VRF Heat Pump Heating Electricity Rate",
        "VRF Heat Pump Heating Electricity Energy",
        // Electric defrost always used for Defrost Strategy = Resistive regardless of fuel type
        "VRF Heat Pump Defrost Electricity Rate", "VRF Heat Pump Defrost Electricity Energy"
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

    IddObjectType AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::iddObjectType() const {
      return AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::iddObjectType();
    }

    std::vector<ScheduleTypeKey>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_AirConditioner_VariableRefrigerantFlowFields::AvailabilitySchedule) != e) {
        result.push_back(ScheduleTypeKey("AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR", "Availability Schedule"));
      }
      if (std::find(b, e, OS_AirConditioner_VariableRefrigerantFlowFields::ThermostatPrioritySchedule) != e) {
        result.push_back(ScheduleTypeKey("AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR", "Thermostat Priority Schedule"));
      }
      if (std::find(b, e, OS_AirConditioner_VariableRefrigerantFlowFields::BasinHeaterOperatingSchedule) != e) {
        result.push_back(ScheduleTypeKey("AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR", "Basin Heater Operating Schedule"));
      }
      return result;
    }

    Schedule AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
      }
      return value.get();
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::grossRatedTotalCoolingCapacity() const {
      return getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::GrossRatedTotalCoolingCapacity, true);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::isGrossRatedTotalCoolingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::GrossRatedTotalCoolingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::grossRatedCoolingCOP() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::GrossRatedCoolingCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::minimumOutdoorTemperatureinCoolingMode() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumOutdoorTemperatureinCoolingMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::maximumOutdoorTemperatureinCoolingMode() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorTemperatureinCoolingMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::coolingCapacityRatioModifierFunctionofLowTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofLowTemperatureCurveName);
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::coolingCapacityRatioBoundaryCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioBoundaryCurveName);
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::coolingCapacityRatioModifierFunctionofHighTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofHighTemperatureCurveName);
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofLowTemperatureCurveName);
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::coolingEnergyInputRatioBoundaryCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioBoundaryCurveName);
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofHighTemperatureCurveName);
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName);
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName);
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::coolingCombinationRatioCorrectionFactorCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCombinationRatioCorrectionFactorCurveName);
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::coolingPartLoadFractionCorrelationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::CoolingPartLoadFractionCorrelationCurveName);
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::grossRatedHeatingCapacity() const {
      return getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::GrossRatedHeatingCapacity, true);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::isGrossRatedHeatingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::GrossRatedHeatingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::ratedHeatingCapacitySizingRatio() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatedHeatingCapacitySizingRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::ratedHeatingCOP() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatedHeatingCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::minimumOutdoorTemperatureinHeatingMode() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumOutdoorTemperatureinHeatingMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::maximumOutdoorTemperatureinHeatingMode() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorTemperatureinHeatingMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatingCapacityRatioModifierFunctionofLowTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofLowTemperatureCurveName);
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatingCapacityRatioBoundaryCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioBoundaryCurveName);
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatingCapacityRatioModifierFunctionofHighTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofHighTemperatureCurveName);
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofLowTemperatureCurveName);
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatingEnergyInputRatioBoundaryCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioBoundaryCurveName);
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofHighTemperatureCurveName);
    }

    std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatingPerformanceCurveOutdoorTemperatureType() const {
      boost::optional<std::string> value =
        getString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingPerformanceCurveOutdoorTemperatureType, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName);
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName);
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatingCombinationRatioCorrectionFactorCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCombinationRatioCorrectionFactorCurveName);
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatingPartLoadFractionCorrelationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::HeatingPartLoadFractionCorrelationCurveName);
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::minimumHeatPumpPartLoadRatio() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumHeatPumpPartLoadRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<ThermalZone> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::zoneforMasterThermostatLocation() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(
        OS_AirConditioner_VariableRefrigerantFlowFields::ZoneNameforMasterThermostatLocation);
    }

    std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::masterThermostatPriorityControlType() const {
      boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::MasterThermostatPriorityControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::thermostatPrioritySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirConditioner_VariableRefrigerantFlowFields::ThermostatPrioritySchedule);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatPumpWasteHeatRecovery() const {
      boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatPumpWasteHeatRecovery, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    double
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::verticalHeightusedforPipingCorrectionFactor() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::VerticalHeightusedforPipingCorrectionFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::pipingCorrectionFactorforLengthinCoolingModeCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforLengthinCoolingModeCurveName);
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::pipingCorrectionFactorforHeightinCoolingModeCoefficient() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforHeightinCoolingModeCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    double
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::pipingCorrectionFactorforLengthinHeatingModeCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforLengthinHeatingModeCurveName);
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::pipingCorrectionFactorforHeightinHeatingModeCoefficient() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforHeightinHeatingModeCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::crankcaseHeaterPowerperCompressor() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::CrankcaseHeaterPowerperCompressor, true);
      OS_ASSERT(value);
      return value.get();
    }

    int AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::numberofCompressors() const {
      boost::optional<int> value = getInt(OS_AirConditioner_VariableRefrigerantFlowFields::NumberofCompressors, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::ratioofCompressorSizetoTotalCompressorCapacity() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatioofCompressorSizetoTotalCompressorCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::maximumOutdoorDrybulbTemperatureforCrankcaseHeater() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorDrybulbTemperatureforCrankcaseHeater, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::defrostStrategy() const {
      boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostStrategy, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::defrostControl() const {
      boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostControl, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::defrostEnergyInputRatioModifierFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName);
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::defrostTimePeriodFraction() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostTimePeriodFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resistiveDefrostHeaterCapacity() const {
      return getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::ResistiveDefrostHeaterCapacity, true);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::isResistiveDefrostHeaterCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::ResistiveDefrostHeaterCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::maximumOutdoorDrybulbTemperatureforDefrostOperation() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::waterCondenserVolumeFlowRate() const {
      return getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::WaterCondenserVolumeFlowRate, true);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::isWaterCondenserVolumeFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::WaterCondenserVolumeFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::evaporativeCondenserEffectiveness() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserEffectiveness, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::evaporativeCondenserAirFlowRate() const {
      return getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserAirFlowRate, true);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::isEvaporativeCondenserAirFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::evaporativeCondenserPumpRatedPowerConsumption() const {
      return getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserPumpRatedPowerConsumption, true);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserPumpRatedPowerConsumption, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::basinHeaterCapacity() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::BasinHeaterCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::basinHeaterSetpointTemperature() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::BasinHeaterSetpointTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::basinHeaterOperatingSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirConditioner_VariableRefrigerantFlowFields::BasinHeaterOperatingSchedule);
    }

    std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::fuelType() const {
      boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::FuelType, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::minimumOutdoorTemperatureinHeatRecoveryMode() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumOutdoorTemperatureinHeatRecoveryMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::maximumOutdoorTemperatureinHeatRecoveryMode() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorTemperatureinHeatRecoveryMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatRecoveryCoolingCapacityModifierCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityModifierCurveName);
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::initialHeatRecoveryCoolingCapacityFraction() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingCapacityFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatRecoveryCoolingCapacityTimeConstant() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityTimeConstant, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatRecoveryCoolingEnergyModifierCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyModifierCurveName);
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::initialHeatRecoveryCoolingEnergyFraction() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingEnergyFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatRecoveryCoolingEnergyTimeConstant() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyTimeConstant, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatRecoveryHeatingCapacityModifierCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityModifierCurveName);
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::initialHeatRecoveryHeatingCapacityFraction() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryHeatingCapacityFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatRecoveryHeatingCapacityTimeConstant() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityTimeConstant, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatRecoveryHeatingEnergyModifierCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyModifierCurveName);
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::initialHeatRecoveryHeatingEnergyFraction() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryHeatingEnergyFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::heatRecoveryHeatingEnergyTimeConstant() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyTimeConstant, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirConditioner_VariableRefrigerantFlowFields::AvailabilitySchedule,
                                "AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR", "Availability Schedule", schedule);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setGrossRatedTotalCoolingCapacity(
      boost::optional<double> grossRatedTotalCoolingCapacity) {
      bool result(false);
      if (grossRatedTotalCoolingCapacity) {
        result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::GrossRatedTotalCoolingCapacity, grossRatedTotalCoolingCapacity.get());
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::autosizeGrossRatedTotalCoolingCapacity() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::GrossRatedTotalCoolingCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setGrossRatedCoolingCOP(double grossRatedCoolingCOP) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::GrossRatedCoolingCOP, grossRatedCoolingCOP);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setMinimumOutdoorTemperatureinCoolingMode(
      double minimumOutdoorTemperatureinCoolingMode) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumOutdoorTemperatureinCoolingMode, minimumOutdoorTemperatureinCoolingMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setMaximumOutdoorTemperatureinCoolingMode(
      double maximumOutdoorTemperatureinCoolingMode) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorTemperatureinCoolingMode, maximumOutdoorTemperatureinCoolingMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setCoolingCapacityRatioModifierFunctionofLowTemperatureCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofLowTemperatureCurveName,
                            curve.get().handle());
      } else {
        resetCoolingCapacityRatioModifierFunctionofLowTemperatureCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetCoolingCapacityRatioModifierFunctionofLowTemperatureCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofLowTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setCoolingCapacityRatioBoundaryCurve(const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioBoundaryCurveName, curve.get().handle());
      } else {
        resetCoolingCapacityRatioBoundaryCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetCoolingCapacityRatioBoundaryCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioBoundaryCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setCoolingCapacityRatioModifierFunctionofHighTemperatureCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofHighTemperatureCurveName,
                            curve.get().handle());
      } else {
        resetCoolingCapacityRatioModifierFunctionofHighTemperatureCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetCoolingCapacityRatioModifierFunctionofHighTemperatureCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCapacityRatioModifierFunctionofHighTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofLowTemperatureCurveName,
                            curve.get().handle());
      } else {
        resetCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofLowTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setCoolingEnergyInputRatioBoundaryCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioBoundaryCurveName, curve.get().handle());
      } else {
        resetCoolingEnergyInputRatioBoundaryCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetCoolingEnergyInputRatioBoundaryCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioBoundaryCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofHighTemperatureCurveName,
                            curve.get().handle());
      } else {
        resetCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofHighTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName,
                            curve.get().handle());
      } else {
        resetCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() {
      bool result =
        setString(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName,
                            curve.get().handle());
      } else {
        resetCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() {
      bool result =
        setString(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setCoolingCombinationRatioCorrectionFactorCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCombinationRatioCorrectionFactorCurveName, curve.get().handle());
      } else {
        resetCoolingCombinationRatioCorrectionFactorCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetCoolingCombinationRatioCorrectionFactorCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingCombinationRatioCorrectionFactorCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setCoolingPartLoadFractionCorrelationCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingPartLoadFractionCorrelationCurveName, curve.get().handle());
      } else {
        resetCoolingPartLoadFractionCorrelationCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetCoolingPartLoadFractionCorrelationCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::CoolingPartLoadFractionCorrelationCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setGrossRatedHeatingCapacity(
      boost::optional<double> grossRatedHeatingCapacity) {
      bool result(false);
      if (grossRatedHeatingCapacity) {
        result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::GrossRatedHeatingCapacity, grossRatedHeatingCapacity.get());
      }
      OS_ASSERT(result);
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::autosizeGrossRatedHeatingCapacity() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::GrossRatedHeatingCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setRatedHeatingCapacitySizingRatio(
      double ratedHeatingCapacitySizingRatio) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatedHeatingCapacitySizingRatio, ratedHeatingCapacitySizingRatio);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setRatedHeatingCOP(double ratedHeatingCOP) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatedHeatingCOP, ratedHeatingCOP);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setMinimumOutdoorTemperatureinHeatingMode(
      double minimumOutdoorTemperatureinHeatingMode) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumOutdoorTemperatureinHeatingMode, minimumOutdoorTemperatureinHeatingMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setMaximumOutdoorTemperatureinHeatingMode(
      double maximumOutdoorTemperatureinHeatingMode) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorTemperatureinHeatingMode, maximumOutdoorTemperatureinHeatingMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatingCapacityRatioModifierFunctionofLowTemperatureCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofLowTemperatureCurveName,
                            curve.get().handle());
      } else {
        resetHeatingCapacityRatioModifierFunctionofLowTemperatureCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetHeatingCapacityRatioModifierFunctionofLowTemperatureCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofLowTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatingCapacityRatioBoundaryCurve(const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioBoundaryCurveName, curve.get().handle());
      } else {
        resetHeatingCapacityRatioBoundaryCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetHeatingCapacityRatioBoundaryCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioBoundaryCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatingCapacityRatioModifierFunctionofHighTemperatureCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofHighTemperatureCurveName,
                            curve.get().handle());
      } else {
        resetHeatingCapacityRatioModifierFunctionofHighTemperatureCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetHeatingCapacityRatioModifierFunctionofHighTemperatureCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCapacityRatioModifierFunctionofHighTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofLowTemperatureCurveName,
                            curve.get().handle());
      } else {
        resetHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofLowTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatingEnergyInputRatioBoundaryCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioBoundaryCurveName, curve.get().handle());
      } else {
        resetHeatingEnergyInputRatioBoundaryCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetHeatingEnergyInputRatioBoundaryCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioBoundaryCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofHighTemperatureCurveName,
                            curve.get().handle());
      } else {
        resetHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofHighTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatingPerformanceCurveOutdoorTemperatureType(
      std::string heatingPerformanceCurveOutdoorTemperatureType) {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingPerformanceCurveOutdoorTemperatureType,
                              heatingPerformanceCurveOutdoorTemperatureType);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName,
                            curve.get().handle());
      } else {
        resetHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() {
      bool result =
        setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName,
                            curve.get().handle());
      } else {
        resetHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() {
      bool result =
        setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatingCombinationRatioCorrectionFactorCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCombinationRatioCorrectionFactorCurveName, curve.get().handle());
      } else {
        resetHeatingCombinationRatioCorrectionFactorCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetHeatingCombinationRatioCorrectionFactorCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingCombinationRatioCorrectionFactorCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatingPartLoadFractionCorrelationCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingPartLoadFractionCorrelationCurveName, curve.get().handle());
      } else {
        resetHeatingPartLoadFractionCorrelationCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetHeatingPartLoadFractionCorrelationCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatingPartLoadFractionCorrelationCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setMinimumHeatPumpPartLoadRatio(double minimumHeatPumpPartLoadRatio) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumHeatPumpPartLoadRatio, minimumHeatPumpPartLoadRatio);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setZoneforMasterThermostatLocation(
      const boost::optional<ThermalZone>& zone) {
      bool result(false);
      if (zone) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::ZoneNameforMasterThermostatLocation, zone.get().handle());
      } else {
        resetZoneforMasterThermostatLocation();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetZoneforMasterThermostatLocation() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::ZoneNameforMasterThermostatLocation, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setMasterThermostatPriorityControlType(
      std::string masterThermostatPriorityControlType) {
      bool result =
        setString(OS_AirConditioner_VariableRefrigerantFlowFields::MasterThermostatPriorityControlType, masterThermostatPriorityControlType);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setThermostatPrioritySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirConditioner_VariableRefrigerantFlowFields::ThermostatPrioritySchedule,
                                "AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR", "Thermostat Priority Schedule", schedule);
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetThermostatPrioritySchedule() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::ThermostatPrioritySchedule, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatPumpWasteHeatRecovery(bool heatPumpWasteHeatRecovery) {
      return setBooleanFieldValue(OS_AirConditioner_VariableRefrigerantFlowFields::HeatPumpWasteHeatRecovery, heatPumpWasteHeatRecovery);
      ;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setEquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode(
      double equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode,
                              equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setVerticalHeightusedforPipingCorrectionFactor(
      double verticalHeightusedforPipingCorrectionFactor) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::VerticalHeightusedforPipingCorrectionFactor,
                              verticalHeightusedforPipingCorrectionFactor);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setPipingCorrectionFactorforLengthinCoolingModeCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result =
          setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforLengthinCoolingModeCurveName, curve.get().handle());
      } else {
        resetPipingCorrectionFactorforLengthinCoolingModeCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetPipingCorrectionFactorforLengthinCoolingModeCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforLengthinCoolingModeCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setPipingCorrectionFactorforHeightinCoolingModeCoefficient(
      double pipingCorrectionFactorforHeightinCoolingModeCoefficient) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforHeightinCoolingModeCoefficient,
                              pipingCorrectionFactorforHeightinCoolingModeCoefficient);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setEquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode(
      double equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode,
                              equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setPipingCorrectionFactorforLengthinHeatingModeCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result =
          setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforLengthinHeatingModeCurveName, curve.get().handle());
      } else {
        resetPipingCorrectionFactorforLengthinHeatingModeCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetPipingCorrectionFactorforLengthinHeatingModeCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforLengthinHeatingModeCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setPipingCorrectionFactorforHeightinHeatingModeCoefficient(
      double pipingCorrectionFactorforHeightinHeatingModeCoefficient) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::PipingCorrectionFactorforHeightinHeatingModeCoefficient,
                              pipingCorrectionFactorforHeightinHeatingModeCoefficient);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setCrankcaseHeaterPowerperCompressor(
      double crankcaseHeaterPowerperCompressor) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::CrankcaseHeaterPowerperCompressor, crankcaseHeaterPowerperCompressor);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setNumberofCompressors(int numberofCompressors) {
      bool result = setInt(OS_AirConditioner_VariableRefrigerantFlowFields::NumberofCompressors, numberofCompressors);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setRatioofCompressorSizetoTotalCompressorCapacity(
      double ratioofCompressorSizetoTotalCompressorCapacity) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::RatioofCompressorSizetoTotalCompressorCapacity,
                              ratioofCompressorSizetoTotalCompressorCapacity);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setMaximumOutdoorDrybulbTemperatureforCrankcaseHeater(
      double maximumOutdoorDrybulbTemperatureforCrankcaseHeater) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorDrybulbTemperatureforCrankcaseHeater,
                              maximumOutdoorDrybulbTemperatureforCrankcaseHeater);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setDefrostStrategy(std::string defrostStrategy) {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostStrategy, defrostStrategy);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setDefrostControl(std::string defrostControl) {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostControl, defrostControl);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName,
                            curve.get().handle());
      } else {
        resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setDefrostTimePeriodFraction(double defrostTimePeriodFraction) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::DefrostTimePeriodFraction, defrostTimePeriodFraction);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setResistiveDefrostHeaterCapacity(
      boost::optional<double> resistiveDefrostHeaterCapacity) {
      bool result(false);
      if (resistiveDefrostHeaterCapacity) {
        result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::ResistiveDefrostHeaterCapacity, resistiveDefrostHeaterCapacity.get());
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::autosizeResistiveDefrostHeaterCapacity() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::ResistiveDefrostHeaterCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setMaximumOutdoorDrybulbTemperatureforDefrostOperation(
      double maximumOutdoorDrybulbTemperatureforDefrostOperation) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation,
                              maximumOutdoorDrybulbTemperatureforDefrostOperation);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setWaterCondenserVolumeFlowRate(
      boost::optional<double> waterCondenserVolumeFlowRate) {
      bool result(false);
      if (waterCondenserVolumeFlowRate) {
        result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::WaterCondenserVolumeFlowRate, waterCondenserVolumeFlowRate.get());
      }
      OS_ASSERT(result);
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::autosizeWaterCondenserVolumeFlowRate() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::WaterCondenserVolumeFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setEvaporativeCondenserEffectiveness(
      double evaporativeCondenserEffectiveness) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserEffectiveness, evaporativeCondenserEffectiveness);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setEvaporativeCondenserAirFlowRate(
      boost::optional<double> evaporativeCondenserAirFlowRate) {
      bool result(false);
      if (evaporativeCondenserAirFlowRate) {
        result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserAirFlowRate, evaporativeCondenserAirFlowRate.get());
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::autosizeEvaporativeCondenserAirFlowRate() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserAirFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setEvaporativeCondenserPumpRatedPowerConsumption(
      boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption) {
      bool result(false);
      if (evaporativeCondenserPumpRatedPowerConsumption) {
        result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserPumpRatedPowerConsumption,
                           evaporativeCondenserPumpRatedPowerConsumption.get());
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::autosizeEvaporativeCondenserPumpRatedPowerConsumption() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::EvaporativeCondenserPumpRatedPowerConsumption, "autosize");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setBasinHeaterCapacity(double basinHeaterCapacity) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::BasinHeaterCapacity, basinHeaterCapacity);
      return result;
    }

    bool
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::BasinHeaterSetpointTemperature, basinHeaterSetpointTemperature);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setBasinHeaterOperatingSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirConditioner_VariableRefrigerantFlowFields::BasinHeaterOperatingSchedule,
                                "AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR", "Basin Heater Operating Schedule", schedule);
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetBasinHeaterOperatingSchedule() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::BasinHeaterOperatingSchedule, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setFuelType(std::string fuelType) {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::FuelType, fuelType);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setMinimumOutdoorTemperatureinHeatRecoveryMode(
      double minimumOutdoorTemperatureinHeatRecoveryMode) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MinimumOutdoorTemperatureinHeatRecoveryMode,
                              minimumOutdoorTemperatureinHeatRecoveryMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setMaximumOutdoorTemperatureinHeatRecoveryMode(
      double maximumOutdoorTemperatureinHeatRecoveryMode) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::MaximumOutdoorTemperatureinHeatRecoveryMode,
                              maximumOutdoorTemperatureinHeatRecoveryMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatRecoveryCoolingCapacityModifierCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityModifierCurveName, curve.get().handle());
      } else {
        resetHeatRecoveryCoolingCapacityModifierCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetHeatRecoveryCoolingCapacityModifierCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityModifierCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setInitialHeatRecoveryCoolingCapacityFraction(
      double initialHeatRecoveryCoolingCapacityFraction) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingCapacityFraction,
                              initialHeatRecoveryCoolingCapacityFraction);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatRecoveryCoolingCapacityTimeConstant(
      double heatRecoveryCoolingCapacityTimeConstant) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingCapacityTimeConstant, heatRecoveryCoolingCapacityTimeConstant);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatRecoveryCoolingEnergyModifierCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyModifierCurveName, curve.get().handle());
      } else {
        resetHeatRecoveryCoolingEnergyModifierCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetHeatRecoveryCoolingEnergyModifierCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyModifierCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setInitialHeatRecoveryCoolingEnergyFraction(
      double initialHeatRecoveryCoolingEnergyFraction) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingEnergyFraction,
                              initialHeatRecoveryCoolingEnergyFraction);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatRecoveryCoolingEnergyTimeConstant(
      double heatRecoveryCoolingEnergyTimeConstant) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryCoolingEnergyTimeConstant, heatRecoveryCoolingEnergyTimeConstant);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatRecoveryHeatingCapacityModifierCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityModifierCurveName, curve.get().handle());
      } else {
        resetHeatRecoveryHeatingCapacityModifierCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetHeatRecoveryHeatingCapacityModifierCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityModifierCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setInitialHeatRecoveryHeatingCapacityFraction(
      double initialHeatRecoveryHeatingCapacityFraction) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryHeatingCapacityFraction,
                              initialHeatRecoveryHeatingCapacityFraction);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatRecoveryHeatingCapacityTimeConstant(
      double heatRecoveryHeatingCapacityTimeConstant) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingCapacityTimeConstant, heatRecoveryHeatingCapacityTimeConstant);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatRecoveryHeatingEnergyModifierCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyModifierCurveName, curve.get().handle());
      } else {
        resetHeatRecoveryHeatingEnergyModifierCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetHeatRecoveryHeatingEnergyModifierCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyModifierCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setInitialHeatRecoveryHeatingEnergyFraction(
      double initialHeatRecoveryHeatingEnergyFraction) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryHeatingEnergyFraction,
                              initialHeatRecoveryHeatingEnergyFraction);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setHeatRecoveryHeatingEnergyTimeConstant(
      double heatRecoveryHeatingEnergyTimeConstant) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlowFields::HeatRecoveryHeatingEnergyTimeConstant, heatRecoveryHeatingEnergyTimeConstant);
      OS_ASSERT(result);
      return result;
    }

    boost::optional<Schedule> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirConditioner_VariableRefrigerantFlowFields::AvailabilitySchedule);
    }

    unsigned AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::inletPort() const {
      return OS_AirConditioner_VariableRefrigerantFlowFields::CondenserInletNode;
    }

    unsigned AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::outletPort() const {
      return OS_AirConditioner_VariableRefrigerantFlowFields::CondenserOutletNode;
    }

    ModelObjectList AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::vrfModelObjectList() const {
      boost::optional<ModelObjectList> mo =
        getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(OS_AirConditioner_VariableRefrigerantFlowFields::ZoneTerminalUnitList);

      OS_ASSERT(mo);

      return mo.get();
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setVRFModelObjectList(const ModelObjectList& modelObjectList) {
      return setPointer(OS_AirConditioner_VariableRefrigerantFlowFields::ZoneTerminalUnitList, modelObjectList.handle());
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::addTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf) {
      vrfModelObjectList().addModelObject(vrf);
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::removeTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf) {
      vrfModelObjectList().removeModelObject(vrf);
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::removeAllTerminals() {
      vrfModelObjectList().removeAllModelObjects();
    }

    std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::terminals() const {
      return subsetCastVector<ZoneHVACTerminalUnitVariableRefrigerantFlow>(vrfModelObjectList().modelObjects());
    }

    std::vector<ModelObject> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::children() const {
      std::vector<ModelObject> result;

      boost::optional<ModelObject> curve;

      curve = coolingCapacityRatioModifierFunctionofLowTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingCapacityRatioBoundaryCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingCapacityRatioModifierFunctionofHighTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingEnergyInputRatioBoundaryCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingCombinationRatioCorrectionFactorCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingPartLoadFractionCorrelationCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = heatingCapacityRatioModifierFunctionofLowTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = heatingCapacityRatioBoundaryCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = heatingCapacityRatioModifierFunctionofHighTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = heatingEnergyInputRatioBoundaryCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = heatingCombinationRatioCorrectionFactorCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = heatingPartLoadFractionCorrelationCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = pipingCorrectionFactorforLengthinCoolingModeCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = pipingCorrectionFactorforLengthinHeatingModeCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = heatRecoveryCoolingCapacityModifierCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = heatRecoveryCoolingEnergyModifierCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = heatRecoveryHeatingCapacityModifierCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = heatRecoveryHeatingEnergyModifierCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      return result;
    }

    ModelObject AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::clone(Model model) const {
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR airConditionerClone =
        StraightComponent_Impl::clone(model).cast<AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR>();

      ModelObjectList modelObjectList(model);
      airConditionerClone.getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setVRFModelObjectList(
        modelObjectList);

      if (auto curve = coolingCapacityRatioModifierFunctionofLowTemperatureCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setCoolingCapacityRatioModifierFunctionofLowTemperatureCurve(clone);
      }

      if (auto curve = coolingCapacityRatioBoundaryCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setCoolingCapacityRatioBoundaryCurve(clone);
      }

      if (auto curve = coolingCapacityRatioModifierFunctionofHighTemperatureCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setCoolingCapacityRatioModifierFunctionofHighTemperatureCurve(clone);
      }

      if (auto curve = coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve(clone);
      }

      if (auto curve = coolingEnergyInputRatioBoundaryCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setCoolingEnergyInputRatioBoundaryCurve(clone);
      }

      if (auto curve = coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve(clone);
      }

      if (auto curve = coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(clone);
      }

      if (auto curve = coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(clone);
      }

      if (auto curve = coolingCombinationRatioCorrectionFactorCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setCoolingCombinationRatioCorrectionFactorCurve(clone);
      }

      if (auto curve = coolingPartLoadFractionCorrelationCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setCoolingPartLoadFractionCorrelationCurve(clone);
      }

      if (auto curve = heatingCapacityRatioModifierFunctionofLowTemperatureCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setHeatingCapacityRatioModifierFunctionofLowTemperatureCurve(clone);
      }

      if (auto curve = heatingCapacityRatioBoundaryCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setHeatingCapacityRatioBoundaryCurve(clone);
      }

      if (auto curve = heatingCapacityRatioModifierFunctionofHighTemperatureCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setHeatingCapacityRatioModifierFunctionofHighTemperatureCurve(clone);
      }

      if (auto curve = heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve(clone);
      }

      if (auto curve = heatingEnergyInputRatioBoundaryCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setHeatingEnergyInputRatioBoundaryCurve(clone);
      }

      if (auto curve = heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve(clone);
      }

      if (auto curve = heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(clone);
      }

      if (auto curve = heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(clone);
      }

      if (auto curve = heatingCombinationRatioCorrectionFactorCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setHeatingCombinationRatioCorrectionFactorCurve(clone);
      }

      if (auto curve = heatingPartLoadFractionCorrelationCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setHeatingPartLoadFractionCorrelationCurve(clone);
      }

      if (auto curve = pipingCorrectionFactorforLengthinCoolingModeCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setPipingCorrectionFactorforLengthinCoolingModeCurve(clone);
      }

      if (auto curve = pipingCorrectionFactorforLengthinHeatingModeCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setPipingCorrectionFactorforLengthinHeatingModeCurve(clone);
      }

      if (auto curve = heatRecoveryCoolingCapacityModifierCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setHeatRecoveryCoolingCapacityModifierCurve(clone);
      }

      if (auto curve = heatRecoveryCoolingEnergyModifierCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setHeatRecoveryCoolingEnergyModifierCurve(clone);
      }

      if (auto curve = heatRecoveryHeatingCapacityModifierCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setHeatRecoveryHeatingCapacityModifierCurve(clone);
      }

      if (auto curve = heatRecoveryHeatingEnergyModifierCurve()) {
        auto clone = curve->clone(model).cast<Curve>();
        airConditionerClone.setHeatRecoveryHeatingEnergyModifierCurve(clone);
      }

      return airConditionerClone;
    }

    std::vector<openstudio::IdfObject> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::remove() {
      vrfModelObjectList().remove();

      boost::optional<Curve> curve;

      curve = coolingCapacityRatioModifierFunctionofLowTemperatureCurve();
      if (curve) {
        curve->remove();
      }

      curve = coolingCapacityRatioBoundaryCurve();
      if (curve) {
        curve->remove();
      }

      curve = coolingCapacityRatioModifierFunctionofHighTemperatureCurve();
      if (curve) {
        curve->remove();
      }

      curve = coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
      if (curve) {
        curve->remove();
      }

      curve = coolingEnergyInputRatioBoundaryCurve();
      if (curve) {
        curve->remove();
      }

      curve = coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
      if (curve) {
        curve->remove();
      }

      curve = coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
      if (curve) {
        curve->remove();
      }

      curve = coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
      if (curve) {
        curve->remove();
      }

      curve = coolingCombinationRatioCorrectionFactorCurve();
      if (curve) {
        curve->remove();
      }

      curve = coolingPartLoadFractionCorrelationCurve();
      if (curve) {
        curve->remove();
      }

      curve = heatingCapacityRatioModifierFunctionofLowTemperatureCurve();
      if (curve) {
        curve->remove();
      }

      curve = heatingCapacityRatioBoundaryCurve();
      if (curve) {
        curve->remove();
      }

      curve = heatingCapacityRatioModifierFunctionofHighTemperatureCurve();
      if (curve) {
        curve->remove();
      }

      curve = heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
      if (curve) {
        curve->remove();
      }

      curve = heatingEnergyInputRatioBoundaryCurve();
      if (curve) {
        curve->remove();
      }

      curve = heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
      if (curve) {
        curve->remove();
      }

      curve = heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
      if (curve) {
        curve->remove();
      }

      curve = heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
      if (curve) {
        curve->remove();
      }

      curve = heatingCombinationRatioCorrectionFactorCurve();
      if (curve) {
        curve->remove();
      }

      curve = heatingPartLoadFractionCorrelationCurve();
      if (curve) {
        curve->remove();
      }

      curve = pipingCorrectionFactorforLengthinCoolingModeCurve();
      if (curve) {
        curve->remove();
      }

      curve = pipingCorrectionFactorforLengthinCoolingModeCurve();
      if (curve) {
        curve->remove();
      }

      curve = heatRecoveryCoolingCapacityModifierCurve();
      if (curve) {
        curve->remove();
      }

      curve = heatRecoveryCoolingEnergyModifierCurve();
      if (curve) {
        curve->remove();
      }

      curve = heatRecoveryHeatingCapacityModifierCurve();
      if (curve) {
        curve->remove();
      }

      curve = heatRecoveryHeatingEnergyModifierCurve();
      if (curve) {
        curve->remove();
      }

      return StraightComponent_Impl::remove();
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::autosizedGrossRatedTotalCoolingCapacity() const {
      return getAutosizedValue("Design Size Rated Total Cooling Capacity (gross)", "W");
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::autosizedGrossRatedHeatingCapacity() const {
      return getAutosizedValue("Design Size Rated Total Heating Capacity", "W");
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::autosizedResistiveDefrostHeaterCapacity() const {
      return getAutosizedValue("Design Size Resistive Defrost Heater Capacity", "");
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::autosizedWaterCondenserVolumeFlowRate() const {
      return getAutosizedValue("Design Size Water Condenser Volume Flow Rate", "m3/s");
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::autosizedEvaporativeCondenserAirFlowRate() const {
      return getAutosizedValue("Design Size Evaporative Condenser Air Flow Rate", "m3/s");
    }

    boost::optional<double>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::autosizedEvaporativeCondenserPumpRatedPowerConsumption() const {
      return getAutosizedValue("Design Size Evaporative Condenser Pump Rated Power Consumption", "W");
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::autosize() {
      autosizeGrossRatedTotalCoolingCapacity();
      autosizeGrossRatedHeatingCapacity();
      autosizeResistiveDefrostHeaterCapacity();
      autosizeWaterCondenserVolumeFlowRate();
      autosizeEvaporativeCondenserAirFlowRate();
      autosizeEvaporativeCondenserPumpRatedPowerConsumption();
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedGrossRatedTotalCoolingCapacity();
      if (val) {
        setGrossRatedTotalCoolingCapacity(val.get());
      }

      val = autosizedGrossRatedHeatingCapacity();
      if (val) {
        setGrossRatedHeatingCapacity(val.get());
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

    std::vector<EMSActuatorNames> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"Variable Refrigerant Flow Heat Pump", "Operating Mode"}};
      return actuators;
    }

    std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types;
      return types;
    }

    std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::condenserType() const {
      // Note JM 2019-09-02: This looks weird / unusual, but it's because it was decided to not put any logic here and move it to FT
      // In order to preserve backward compat, the return type was left to std::string, so we basically do this:
      // * If hardset, return that value
      // * If empty (defaulted), return the default: VRF connected to a PlantLoop => WaterCooled, else AirCooled.
      std::string condenserType;

      if (isCondenserTypeDefaulted()) {
        // Default like FT
        if (this->plantLoop()) {
          condenserType = "WaterCooled";
        } else {
          condenserType = "AirCooled";
        }
      } else {
        boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlowFields::CondenserType, false);
        condenserType = value.get();
      }
      return condenserType;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::setCondenserType(const std::string& condenserType) {

      // If this doesn't agree with the current conditions, we warn...
      if ((openstudio::istringEqual("AirCooled", condenserType) || openstudio::istringEqual("EvaporativelyCooled", condenserType))
          && (this->plantLoop())) {
        LOG(Warn, "Setting the Condenser Type to '" << condenserType << "', you should disconnect from its PlantLoop. "
                                                    << "Occurred for " << briefDescription());
      } else if (istringEqual("WaterCooled", condenserType) && !(this->plantLoop())) {
        LOG(Warn, "Setting the Condenser Type to 'WaterCooled', you should connect it to a PlantLoop. "
                    << "Occurred for " << briefDescription());
      }

      // ... but we still do it...
      return setString(OS_AirConditioner_VariableRefrigerantFlowFields::CondenserType, condenserType);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::isCondenserTypeDefaulted() const {
      return isEmpty(OS_AirConditioner_VariableRefrigerantFlowFields::CondenserType);
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::resetCondenserType() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlowFields::CondenserType, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::addToNode(Node& node) {
      // Only accept the demand side of a PlantLoop
      if (boost::optional<PlantLoop> plant = node.plantLoop()) {
        if (plant->demandComponent(node.handle())) {
          // bool success = StraightComponent_Impl::addToNode(node);
          // if (success) {
          // // If everything went well, then switch the condenser type
          //   setCondenserType("WaterCooled");
          //   return success;
          // }
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    // This override is rendered moot now.
    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl::removeFromLoop() {
      // Disconnect the component
      bool ok = StraightComponent_Impl::removeFromLoop();

      // Don't Switch the condenser type to "AirCooled"
      // this->setCondenserType("AirCooled");
      return ok;
    }

  }  // namespace detail

  AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR(const Model& model)
    : StraightComponent(AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>());

    Schedule schedule = model.alwaysOnDiscreteSchedule();

    setAvailabilitySchedule(schedule);

    autosizeGrossRatedTotalCoolingCapacity();

    setGrossRatedCoolingCOP(3.3);

    setMinimumOutdoorTemperatureinCoolingMode(-5.0);

    setMaximumOutdoorTemperatureinCoolingMode(43.0);

    autosizeGrossRatedHeatingCapacity();

    setRatedHeatingCapacitySizingRatio(1.0);

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
    vrfCoolCapFTHi.setName(name().get() + " VRFCoolCapFTHi");
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
    vrfCoolEIRFT.setName(name().get() + " VRFCoolEIRFT");
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
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setVRFModelObjectList(vrfModelObjectList);
  }

  IddObjectType AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::iddObjectType() {
    return IddObjectType(IddObjectType::OS_AirConditioner_VariableRefrigerantFlow);
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatingPerformanceCurveOutdoorTemperatureTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirConditioner_VariableRefrigerantFlowFields::HeatingPerformanceCurveOutdoorTemperatureType);
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::masterThermostatPriorityControlTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirConditioner_VariableRefrigerantFlowFields::MasterThermostatPriorityControlType);
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::defrostStrategyValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_AirConditioner_VariableRefrigerantFlowFields::DefrostStrategy);
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::defrostControlValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_AirConditioner_VariableRefrigerantFlowFields::DefrostControl);
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::fuelTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_AirConditioner_VariableRefrigerantFlowFields::FuelType);
  }

  Schedule AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::availabilitySchedule() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->availabilitySchedule();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::minimumOutdoorTemperatureinCoolingMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->minimumOutdoorTemperatureinCoolingMode();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::maximumOutdoorTemperatureinCoolingMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->maximumOutdoorTemperatureinCoolingMode();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::coolingCapacityRatioModifierFunctionofLowTemperatureCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->coolingCapacityRatioModifierFunctionofLowTemperatureCurve();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::coolingCapacityRatioBoundaryCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->coolingCapacityRatioBoundaryCurve();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::coolingCapacityRatioModifierFunctionofHighTemperatureCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->coolingCapacityRatioModifierFunctionofHighTemperatureCurve();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::coolingEnergyInputRatioBoundaryCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->coolingEnergyInputRatioBoundaryCurve();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::coolingCombinationRatioCorrectionFactorCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->coolingCombinationRatioCorrectionFactorCurve();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::coolingPartLoadFractionCorrelationCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->coolingPartLoadFractionCorrelationCurve();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::ratedHeatingCOP() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->ratedHeatingCOP();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::minimumOutdoorTemperatureinHeatingMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->minimumOutdoorTemperatureinHeatingMode();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::maximumOutdoorTemperatureinHeatingMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->maximumOutdoorTemperatureinHeatingMode();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatingCapacityRatioModifierFunctionofLowTemperatureCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->heatingCapacityRatioModifierFunctionofLowTemperatureCurve();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatingCapacityRatioBoundaryCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->heatingCapacityRatioBoundaryCurve();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatingCapacityRatioModifierFunctionofHighTemperatureCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->heatingCapacityRatioModifierFunctionofHighTemperatureCurve();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatingEnergyInputRatioBoundaryCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->heatingEnergyInputRatioBoundaryCurve();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
  }

  std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatingPerformanceCurveOutdoorTemperatureType() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->heatingPerformanceCurveOutdoorTemperatureType();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatingCombinationRatioCorrectionFactorCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->heatingCombinationRatioCorrectionFactorCurve();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatingPartLoadFractionCorrelationCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->heatingPartLoadFractionCorrelationCurve();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::minimumHeatPumpPartLoadRatio() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->minimumHeatPumpPartLoadRatio();
  }

  boost::optional<ThermalZone> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::zoneforMasterThermostatLocation() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->zoneforMasterThermostatLocation();
  }

  std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::masterThermostatPriorityControlType() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->masterThermostatPriorityControlType();
  }

  boost::optional<Schedule> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::thermostatPrioritySchedule() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->thermostatPrioritySchedule();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatPumpWasteHeatRecovery() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->heatPumpWasteHeatRecovery();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::verticalHeightusedforPipingCorrectionFactor() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->verticalHeightusedforPipingCorrectionFactor();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::pipingCorrectionFactorforLengthinCoolingModeCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->pipingCorrectionFactorforLengthinCoolingModeCurve();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::pipingCorrectionFactorforHeightinCoolingModeCoefficient() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->pipingCorrectionFactorforHeightinCoolingModeCoefficient();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::pipingCorrectionFactorforLengthinHeatingModeCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->pipingCorrectionFactorforLengthinHeatingModeCurve();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::pipingCorrectionFactorforHeightinHeatingModeCoefficient() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->pipingCorrectionFactorforHeightinHeatingModeCoefficient();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::crankcaseHeaterPowerperCompressor() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->crankcaseHeaterPowerperCompressor();
  }

  int AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::numberofCompressors() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->numberofCompressors();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::ratioofCompressorSizetoTotalCompressorCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->ratioofCompressorSizetoTotalCompressorCapacity();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::maximumOutdoorDrybulbTemperatureforCrankcaseHeater() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->maximumOutdoorDrybulbTemperatureforCrankcaseHeater();
  }

  std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::defrostStrategy() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->defrostStrategy();
  }

  std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::defrostControl() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->defrostControl();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::defrostEnergyInputRatioModifierFunctionofTemperatureCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->defrostEnergyInputRatioModifierFunctionofTemperatureCurve();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::defrostTimePeriodFraction() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->defrostTimePeriodFraction();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resistiveDefrostHeaterCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->resistiveDefrostHeaterCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::isResistiveDefrostHeaterCapacityAutosized() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->isResistiveDefrostHeaterCapacityAutosized();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::maximumOutdoorDrybulbTemperatureforDefrostOperation() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->maximumOutdoorDrybulbTemperatureforDefrostOperation();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::waterCondenserVolumeFlowRate() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->waterCondenserVolumeFlowRate();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::isWaterCondenserVolumeFlowRateAutosized() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->isWaterCondenserVolumeFlowRateAutosized();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::evaporativeCondenserEffectiveness() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->evaporativeCondenserEffectiveness();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::evaporativeCondenserAirFlowRate() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->evaporativeCondenserAirFlowRate();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::isEvaporativeCondenserAirFlowRateAutosized() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->isEvaporativeCondenserAirFlowRateAutosized();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::evaporativeCondenserPumpRatedPowerConsumption() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->evaporativeCondenserPumpRatedPowerConsumption();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->isEvaporativeCondenserPumpRatedPowerConsumptionAutosized();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::basinHeaterCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->basinHeaterCapacity();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::basinHeaterSetpointTemperature() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->basinHeaterSetpointTemperature();
  }

  boost::optional<Schedule> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::basinHeaterOperatingSchedule() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->basinHeaterOperatingSchedule();
  }

  std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::fuelType() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->fuelType();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::minimumOutdoorTemperatureinHeatRecoveryMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->minimumOutdoorTemperatureinHeatRecoveryMode();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::maximumOutdoorTemperatureinHeatRecoveryMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->maximumOutdoorTemperatureinHeatRecoveryMode();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatRecoveryCoolingCapacityModifierCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->heatRecoveryCoolingCapacityModifierCurve();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::initialHeatRecoveryCoolingCapacityFraction() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->initialHeatRecoveryCoolingCapacityFraction();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatRecoveryCoolingCapacityTimeConstant() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->heatRecoveryCoolingCapacityTimeConstant();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatRecoveryCoolingEnergyModifierCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->heatRecoveryCoolingEnergyModifierCurve();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::initialHeatRecoveryCoolingEnergyFraction() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->initialHeatRecoveryCoolingEnergyFraction();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatRecoveryCoolingEnergyTimeConstant() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->heatRecoveryCoolingEnergyTimeConstant();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatRecoveryHeatingCapacityModifierCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->heatRecoveryHeatingCapacityModifierCurve();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::initialHeatRecoveryHeatingCapacityFraction() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->initialHeatRecoveryHeatingCapacityFraction();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatRecoveryHeatingCapacityTimeConstant() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->heatRecoveryHeatingCapacityTimeConstant();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatRecoveryHeatingEnergyModifierCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->heatRecoveryHeatingEnergyModifierCurve();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::initialHeatRecoveryHeatingEnergyFraction() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->initialHeatRecoveryHeatingEnergyFraction();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::heatRecoveryHeatingEnergyTimeConstant() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->heatRecoveryHeatingEnergyTimeConstant();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setMinimumOutdoorTemperatureinCoolingMode(
    double minimumOutdoorTemperatureinCoolingMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setMinimumOutdoorTemperatureinCoolingMode(
      minimumOutdoorTemperatureinCoolingMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setMaximumOutdoorTemperatureinCoolingMode(
    double maximumOutdoorTemperatureinCoolingMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setMaximumOutdoorTemperatureinCoolingMode(
      maximumOutdoorTemperatureinCoolingMode);
  }

  bool
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setCoolingCapacityRatioModifierFunctionofLowTemperatureCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setCoolingCapacityRatioModifierFunctionofLowTemperatureCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetCoolingCapacityRatioModifierFunctionofLowTemperatureCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->resetCoolingCapacityRatioModifierFunctionofLowTemperatureCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setCoolingCapacityRatioBoundaryCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setCoolingCapacityRatioBoundaryCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetCoolingCapacityRatioBoundaryCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->resetCoolingCapacityRatioBoundaryCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setCoolingCapacityRatioModifierFunctionofHighTemperatureCurve(
    const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setCoolingCapacityRatioModifierFunctionofHighTemperatureCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetCoolingCapacityRatioModifierFunctionofHighTemperatureCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->resetCoolingCapacityRatioModifierFunctionofHighTemperatureCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve(
    const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->resetCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setCoolingEnergyInputRatioBoundaryCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setCoolingEnergyInputRatioBoundaryCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetCoolingEnergyInputRatioBoundaryCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->resetCoolingEnergyInputRatioBoundaryCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve(
    const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->resetCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(
    const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->resetCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(
    const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->resetCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setCoolingCombinationRatioCorrectionFactorCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setCoolingCombinationRatioCorrectionFactorCurve(
      curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetCoolingCombinationRatioCorrectionFactorCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->resetCoolingCombinationRatioCorrectionFactorCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setCoolingPartLoadFractionCorrelationCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setCoolingPartLoadFractionCorrelationCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetCoolingPartLoadFractionCorrelationCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->resetCoolingPartLoadFractionCorrelationCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setRatedHeatingCOP(double ratedHeatingCOP) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setRatedHeatingCOP(ratedHeatingCOP);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setMinimumOutdoorTemperatureinHeatingMode(
    double minimumOutdoorTemperatureinHeatingMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setMinimumOutdoorTemperatureinHeatingMode(
      minimumOutdoorTemperatureinHeatingMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setMaximumOutdoorTemperatureinHeatingMode(
    double maximumOutdoorTemperatureinHeatingMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setMaximumOutdoorTemperatureinHeatingMode(
      maximumOutdoorTemperatureinHeatingMode);
  }

  bool
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatingCapacityRatioModifierFunctionofLowTemperatureCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setHeatingCapacityRatioModifierFunctionofLowTemperatureCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetHeatingCapacityRatioModifierFunctionofLowTemperatureCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->resetHeatingCapacityRatioModifierFunctionofLowTemperatureCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatingCapacityRatioBoundaryCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setHeatingCapacityRatioBoundaryCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetHeatingCapacityRatioBoundaryCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->resetHeatingCapacityRatioBoundaryCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatingCapacityRatioModifierFunctionofHighTemperatureCurve(
    const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setHeatingCapacityRatioModifierFunctionofHighTemperatureCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetHeatingCapacityRatioModifierFunctionofHighTemperatureCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->resetHeatingCapacityRatioModifierFunctionofHighTemperatureCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve(
    const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->resetHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatingEnergyInputRatioBoundaryCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setHeatingEnergyInputRatioBoundaryCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetHeatingEnergyInputRatioBoundaryCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->resetHeatingEnergyInputRatioBoundaryCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve(
    const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->resetHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatingPerformanceCurveOutdoorTemperatureType(
    std::string heatingPerformanceCurveOutdoorTemperatureType) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setHeatingPerformanceCurveOutdoorTemperatureType(
      heatingPerformanceCurveOutdoorTemperatureType);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(
    const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->resetHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(
    const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->resetHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatingCombinationRatioCorrectionFactorCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setHeatingCombinationRatioCorrectionFactorCurve(
      curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetHeatingCombinationRatioCorrectionFactorCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->resetHeatingCombinationRatioCorrectionFactorCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatingPartLoadFractionCorrelationCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setHeatingPartLoadFractionCorrelationCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetHeatingPartLoadFractionCorrelationCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->resetHeatingPartLoadFractionCorrelationCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setMinimumHeatPumpPartLoadRatio(double minimumHeatPumpPartLoadRatio) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setMinimumHeatPumpPartLoadRatio(
      minimumHeatPumpPartLoadRatio);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setZoneforMasterThermostatLocation(const ThermalZone& zone) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setZoneforMasterThermostatLocation(zone);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetZoneforMasterThermostatLocation() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->resetZoneforMasterThermostatLocation();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setMasterThermostatPriorityControlType(
    std::string masterThermostatPriorityControlType) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setMasterThermostatPriorityControlType(
      masterThermostatPriorityControlType);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setThermostatPrioritySchedule(Schedule& schedule) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setThermostatPrioritySchedule(schedule);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetThermostatPrioritySchedule() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->resetThermostatPrioritySchedule();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatPumpWasteHeatRecovery(bool heatPumpWasteHeatRecovery) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setHeatPumpWasteHeatRecovery(
      heatPumpWasteHeatRecovery);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setEquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode(
    double equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setEquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode(equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setVerticalHeightusedforPipingCorrectionFactor(
    double verticalHeightusedforPipingCorrectionFactor) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setVerticalHeightusedforPipingCorrectionFactor(
      verticalHeightusedforPipingCorrectionFactor);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setPipingCorrectionFactorforLengthinCoolingModeCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setPipingCorrectionFactorforLengthinCoolingModeCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetPipingCorrectionFactorforLengthinCoolingModeCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->resetPipingCorrectionFactorforLengthinCoolingModeCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setPipingCorrectionFactorforHeightinCoolingModeCoefficient(
    double pipingCorrectionFactorforHeightinCoolingModeCoefficient) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setPipingCorrectionFactorforHeightinCoolingModeCoefficient(pipingCorrectionFactorforHeightinCoolingModeCoefficient);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setEquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode(
    double equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setEquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode(equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setPipingCorrectionFactorforLengthinHeatingModeCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setPipingCorrectionFactorforLengthinHeatingModeCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetPipingCorrectionFactorforLengthinHeatingModeCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->resetPipingCorrectionFactorforLengthinHeatingModeCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setPipingCorrectionFactorforHeightinHeatingModeCoefficient(
    double pipingCorrectionFactorforHeightinHeatingModeCoefficient) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setPipingCorrectionFactorforHeightinHeatingModeCoefficient(pipingCorrectionFactorforHeightinHeatingModeCoefficient);
  }

  bool
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setCrankcaseHeaterPowerperCompressor(double crankcaseHeaterPowerperCompressor) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setCrankcaseHeaterPowerperCompressor(
      crankcaseHeaterPowerperCompressor);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setNumberofCompressors(int numberofCompressors) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setNumberofCompressors(numberofCompressors);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setRatioofCompressorSizetoTotalCompressorCapacity(
    double ratioofCompressorSizetoTotalCompressorCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setRatioofCompressorSizetoTotalCompressorCapacity(
      ratioofCompressorSizetoTotalCompressorCapacity);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setMaximumOutdoorDrybulbTemperatureforCrankcaseHeater(
    double maximumOutdoorDrybulbTemperatureforCrankcaseHeater) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setMaximumOutdoorDrybulbTemperatureforCrankcaseHeater(maximumOutdoorDrybulbTemperatureforCrankcaseHeater);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setDefrostStrategy(std::string defrostStrategy) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setDefrostStrategy(defrostStrategy);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setDefrostControl(std::string defrostControl) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setDefrostControl(defrostControl);
  }

  bool
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setDefrostTimePeriodFraction(double defrostTimePeriodFraction) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setDefrostTimePeriodFraction(
      defrostTimePeriodFraction);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setResistiveDefrostHeaterCapacity(
      resistiveDefrostHeaterCapacity);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::autosizeResistiveDefrostHeaterCapacity() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->autosizeResistiveDefrostHeaterCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setMaximumOutdoorDrybulbTemperatureforDefrostOperation(
    double maximumOutdoorDrybulbTemperatureforDefrostOperation) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->setMaximumOutdoorDrybulbTemperatureforDefrostOperation(maximumOutdoorDrybulbTemperatureforDefrostOperation);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setWaterCondenserVolumeFlowRate(double waterCondenserVolumeFlowRate) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setWaterCondenserVolumeFlowRate(
      waterCondenserVolumeFlowRate);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::autosizeWaterCondenserVolumeFlowRate() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->autosizeWaterCondenserVolumeFlowRate();
  }

  bool
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setEvaporativeCondenserEffectiveness(
      evaporativeCondenserEffectiveness);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setEvaporativeCondenserAirFlowRate(double evaporativeCondenserAirFlowRate) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setEvaporativeCondenserAirFlowRate(
      evaporativeCondenserAirFlowRate);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::autosizeEvaporativeCondenserAirFlowRate() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->autosizeEvaporativeCondenserAirFlowRate();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setEvaporativeCondenserPumpRatedPowerConsumption(
    double evaporativeCondenserPumpRatedPowerConsumption) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setEvaporativeCondenserPumpRatedPowerConsumption(
      evaporativeCondenserPumpRatedPowerConsumption);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::autosizeEvaporativeCondenserPumpRatedPowerConsumption() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->autosizeEvaporativeCondenserPumpRatedPowerConsumption();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setBasinHeaterCapacity(double basinHeaterCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setBasinHeaterCapacity(basinHeaterCapacity);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setBasinHeaterSetpointTemperature(
      basinHeaterSetpointTemperature);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setBasinHeaterOperatingSchedule(Schedule& schedule) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setBasinHeaterOperatingSchedule(schedule);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetBasinHeaterOperatingSchedule() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->resetBasinHeaterOperatingSchedule();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setFuelType(std::string fuelType) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setFuelType(fuelType);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setMinimumOutdoorTemperatureinHeatRecoveryMode(
    double minimumOutdoorTemperatureinHeatRecoveryMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setMinimumOutdoorTemperatureinHeatRecoveryMode(
      minimumOutdoorTemperatureinHeatRecoveryMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setMaximumOutdoorTemperatureinHeatRecoveryMode(
    double maximumOutdoorTemperatureinHeatRecoveryMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setMaximumOutdoorTemperatureinHeatRecoveryMode(
      maximumOutdoorTemperatureinHeatRecoveryMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatRecoveryCoolingCapacityModifierCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setHeatRecoveryCoolingCapacityModifierCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetHeatRecoveryCoolingCapacityModifierCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->resetHeatRecoveryCoolingCapacityModifierCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setInitialHeatRecoveryCoolingCapacityFraction(
    double initialHeatRecoveryCoolingCapacityFraction) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setInitialHeatRecoveryCoolingCapacityFraction(
      initialHeatRecoveryCoolingCapacityFraction);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatRecoveryCoolingCapacityTimeConstant(
    double heatRecoveryCoolingCapacityTimeConstant) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setHeatRecoveryCoolingCapacityTimeConstant(
      heatRecoveryCoolingCapacityTimeConstant);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatRecoveryCoolingEnergyModifierCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setHeatRecoveryCoolingEnergyModifierCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetHeatRecoveryCoolingEnergyModifierCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->resetHeatRecoveryCoolingEnergyModifierCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setInitialHeatRecoveryCoolingEnergyFraction(
    double initialHeatRecoveryCoolingEnergyFraction) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setInitialHeatRecoveryCoolingEnergyFraction(
      initialHeatRecoveryCoolingEnergyFraction);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatRecoveryCoolingEnergyTimeConstant(
    double heatRecoveryCoolingEnergyTimeConstant) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setHeatRecoveryCoolingEnergyTimeConstant(
      heatRecoveryCoolingEnergyTimeConstant);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatRecoveryHeatingCapacityModifierCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setHeatRecoveryHeatingCapacityModifierCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetHeatRecoveryHeatingCapacityModifierCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->resetHeatRecoveryHeatingCapacityModifierCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setInitialHeatRecoveryHeatingCapacityFraction(
    double initialHeatRecoveryHeatingCapacityFraction) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setInitialHeatRecoveryHeatingCapacityFraction(
      initialHeatRecoveryHeatingCapacityFraction);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatRecoveryHeatingCapacityTimeConstant(
    double heatRecoveryHeatingCapacityTimeConstant) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setHeatRecoveryHeatingCapacityTimeConstant(
      heatRecoveryHeatingCapacityTimeConstant);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatRecoveryHeatingEnergyModifierCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setHeatRecoveryHeatingEnergyModifierCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetHeatRecoveryHeatingEnergyModifierCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->resetHeatRecoveryHeatingEnergyModifierCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setInitialHeatRecoveryHeatingEnergyFraction(
    double initialHeatRecoveryHeatingEnergyFraction) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setInitialHeatRecoveryHeatingEnergyFraction(
      initialHeatRecoveryHeatingEnergyFraction);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setHeatRecoveryHeatingEnergyTimeConstant(
    double heatRecoveryHeatingEnergyTimeConstant) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setHeatRecoveryHeatingEnergyTimeConstant(
      heatRecoveryHeatingEnergyTimeConstant);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::addTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf) {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->addTerminal(vrf);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::removeTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf) {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->removeTerminal(vrf);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::removeAllTerminals() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->removeAllTerminals();
  }

  std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::terminals() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->terminals();
  }

  /// @cond
  AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR(
    std::shared_ptr<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::autosizedResistiveDefrostHeaterCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->autosizedResistiveDefrostHeaterCapacity();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::autosizedWaterCondenserVolumeFlowRate() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->autosizedWaterCondenserVolumeFlowRate();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::autosizedEvaporativeCondenserAirFlowRate() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->autosizedEvaporativeCondenserAirFlowRate();
  }

  boost::optional<double>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::autosizedEvaporativeCondenserPumpRatedPowerConsumption() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()
      ->autosizedEvaporativeCondenserPumpRatedPowerConsumption();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setGrossRatedTotalCoolingCapacity(double grossRatedTotalCoolingCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setGrossRatedTotalCoolingCapacity(
      grossRatedTotalCoolingCapacity);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::autosizeGrossRatedTotalCoolingCapacity() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->autosizeGrossRatedTotalCoolingCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setGrossRatedCoolingCOP(double grossRatedCoolingCOP) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setGrossRatedCoolingCOP(grossRatedCoolingCOP);
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::grossRatedTotalCoolingCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->grossRatedTotalCoolingCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::isGrossRatedTotalCoolingCapacityAutosized() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->isGrossRatedTotalCoolingCapacityAutosized();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::grossRatedCoolingCOP() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->grossRatedCoolingCOP();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::grossRatedHeatingCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->grossRatedHeatingCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::isGrossRatedHeatingCapacityAutosized() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->isGrossRatedHeatingCapacityAutosized();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::ratedHeatingCapacitySizingRatio() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->ratedHeatingCapacitySizingRatio();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setGrossRatedHeatingCapacity(double grossRatedHeatingCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setGrossRatedHeatingCapacity(
      grossRatedHeatingCapacity);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::autosizeGrossRatedHeatingCapacity() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->autosizeGrossRatedHeatingCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setRatedHeatingCapacitySizingRatio(double ratedHeatingCapacitySizingRatio) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setRatedHeatingCapacitySizingRatio(
      ratedHeatingCapacitySizingRatio);
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::autosizedGrossRatedTotalCoolingCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->autosizedGrossRatedTotalCoolingCapacity();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::autosizedGrossRatedHeatingCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->autosizedGrossRatedHeatingCapacity();
  }

  std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::condenserType() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->condenserType();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setCondenserType(const std::string& condenserType) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setCondenserType(condenserType);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::isCondenserTypeDefaulted() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->isCondenserTypeDefaulted();
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::resetCondenserType() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->resetCondenserType();
  }

  // DEPRECATED
  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setRatedTotalCoolingCapacity(double grossRatedTotalCoolingCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setGrossRatedTotalCoolingCapacity(
      grossRatedTotalCoolingCapacity);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::autosizeRatedTotalCoolingCapacity() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->autosizeGrossRatedTotalCoolingCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setRatedCoolingCOP(double grossRatedCoolingCOP) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setGrossRatedCoolingCOP(grossRatedCoolingCOP);
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::ratedTotalCoolingCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->grossRatedTotalCoolingCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::isRatedTotalCoolingCapacityAutosized() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->isGrossRatedTotalCoolingCapacityAutosized();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::ratedCoolingCOP() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->grossRatedCoolingCOP();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::ratedTotalHeatingCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->grossRatedHeatingCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::isRatedTotalHeatingCapacityAutosized() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->isGrossRatedHeatingCapacityAutosized();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::ratedTotalHeatingCapacitySizingRatio() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->ratedHeatingCapacitySizingRatio();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setRatedTotalHeatingCapacity(double grossRatedHeatingCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setGrossRatedHeatingCapacity(
      grossRatedHeatingCapacity);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::autosizeRatedTotalHeatingCapacity() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->autosizeGrossRatedHeatingCapacity();
  }

  bool
    AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::setRatedTotalHeatingCapacitySizingRatio(double ratedHeatingCapacitySizingRatio) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->setRatedHeatingCapacitySizingRatio(
      ratedHeatingCapacitySizingRatio);
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::autosizedRatedTotalCoolingCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->autosizedGrossRatedTotalCoolingCapacity();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR::autosizedRatedTotalHeatingCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR_Impl>()->autosizedGrossRatedHeatingCapacity();
  }

}  // namespace model
}  // namespace openstudio
