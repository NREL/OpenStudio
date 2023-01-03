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
#include "AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl.hpp"
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

#include <utilities/idd/OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControl_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Containers.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirConditionerVariableRefrigerantFlowFluidTemperatureControl::iddObjectType());
    }

    AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirConditionerVariableRefrigerantFlowFluidTemperatureControl::iddObjectType());
    }

    AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl(
      const AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::outputVariableNames() const {
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

    IddObjectType AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::iddObjectType() const {
      return AirConditionerVariableRefrigerantFlowFluidTemperatureControl::iddObjectType();
    }

    std::vector<ScheduleTypeKey>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilitySchedule) != e) {
        result.push_back(ScheduleTypeKey("AirConditionerVariableRefrigerantFlowFluidTemperatureControl", "Availability Schedule"));
      }
      if (std::find(b, e, OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ThermostatPrioritySchedule) != e) {
        result.push_back(ScheduleTypeKey("AirConditionerVariableRefrigerantFlowFluidTemperatureControl", "Thermostat Priority Schedule"));
      }
      if (std::find(b, e, OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::BasinHeaterOperatingSchedule) != e) {
        result.push_back(ScheduleTypeKey("AirConditionerVariableRefrigerantFlowFluidTemperatureControl", "Basin Heater Operating Schedule"));
      }
      return result;
    }

    Schedule AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
      }
      return value.get();
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::grossRatedTotalCoolingCapacity() const {
      return getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::GrossRatedTotalCoolingCapacity, true);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::isGrossRatedTotalCoolingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::GrossRatedTotalCoolingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::grossRatedCoolingCOP() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::GrossRatedCoolingCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::minimumOutdoorTemperatureinCoolingMode() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MinimumOutdoorTemperatureinCoolingMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::maximumOutdoorTemperatureinCoolingMode() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorTemperatureinCoolingMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::coolingCapacityRatioModifierFunctionofLowTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingCapacityRatioModifierFunctionofLowTemperatureCurveName);
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::coolingCapacityRatioBoundaryCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingCapacityRatioBoundaryCurveName);
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::coolingCapacityRatioModifierFunctionofHighTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingCapacityRatioModifierFunctionofHighTemperatureCurveName);
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingEnergyInputRatioModifierFunctionofLowTemperatureCurveName);
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::coolingEnergyInputRatioBoundaryCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingEnergyInputRatioBoundaryCurveName);
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingEnergyInputRatioModifierFunctionofHighTemperatureCurveName);
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName);
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName);
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::coolingCombinationRatioCorrectionFactorCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingCombinationRatioCorrectionFactorCurveName);
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::coolingPartLoadFractionCorrelationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingPartLoadFractionCorrelationCurveName);
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::grossRatedHeatingCapacity() const {
      return getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::GrossRatedHeatingCapacity, true);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::isGrossRatedHeatingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::GrossRatedHeatingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::ratedHeatingCapacitySizingRatio() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedHeatingCapacitySizingRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::ratedHeatingCOP() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedHeatingCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::minimumOutdoorTemperatureinHeatingMode() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MinimumOutdoorTemperatureinHeatingMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::maximumOutdoorTemperatureinHeatingMode() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorTemperatureinHeatingMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::heatingCapacityRatioModifierFunctionofLowTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingCapacityRatioModifierFunctionofLowTemperatureCurveName);
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::heatingCapacityRatioBoundaryCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingCapacityRatioBoundaryCurveName);
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::heatingCapacityRatioModifierFunctionofHighTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingCapacityRatioModifierFunctionofHighTemperatureCurveName);
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingEnergyInputRatioModifierFunctionofLowTemperatureCurveName);
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::heatingEnergyInputRatioBoundaryCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingEnergyInputRatioBoundaryCurveName);
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingEnergyInputRatioModifierFunctionofHighTemperatureCurveName);
    }

    std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::heatingPerformanceCurveOutdoorTemperatureType() const {
      boost::optional<std::string> value =
        getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingPerformanceCurveOutdoorTemperatureType, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName);
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName);
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::heatingCombinationRatioCorrectionFactorCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingCombinationRatioCorrectionFactorCurveName);
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::heatingPartLoadFractionCorrelationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingPartLoadFractionCorrelationCurveName);
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::minimumHeatPumpPartLoadRatio() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MinimumHeatPumpPartLoadRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<ThermalZone> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::zoneforMasterThermostatLocation() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ZoneNameforMasterThermostatLocation);
    }

    std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::masterThermostatPriorityControlType() const {
      boost::optional<std::string> value =
        getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MasterThermostatPriorityControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::thermostatPrioritySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ThermostatPrioritySchedule);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::heatPumpWasteHeatRecovery() const {
      boost::optional<std::string> value =
        getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatPumpWasteHeatRecovery, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    double
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode() const {
      boost::optional<double> value = getDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::EquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode,
        true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::verticalHeightusedforPipingCorrectionFactor() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::VerticalHeightusedforPipingCorrectionFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::pipingCorrectionFactorforLengthinCoolingModeCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::PipingCorrectionFactorforLengthinCoolingModeCurveName);
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::pipingCorrectionFactorforHeightinCoolingModeCoefficient() const {
      boost::optional<double> value = getDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::PipingCorrectionFactorforHeightinCoolingModeCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    double
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode() const {
      boost::optional<double> value = getDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::EquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode,
        true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::pipingCorrectionFactorforLengthinHeatingModeCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::PipingCorrectionFactorforLengthinHeatingModeCurveName);
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::pipingCorrectionFactorforHeightinHeatingModeCoefficient() const {
      boost::optional<double> value = getDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::PipingCorrectionFactorforHeightinHeatingModeCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::crankcaseHeaterPowerperCompressor() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CrankcaseHeaterPowerperCompressor, true);
      OS_ASSERT(value);
      return value.get();
    }

    int AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::numberofCompressors() const {
      boost::optional<int> value = getInt(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::NumberofCompressors, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::ratioofCompressorSizetoTotalCompressorCapacity() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatioofCompressorSizetoTotalCompressorCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::maximumOutdoorDrybulbTemperatureforCrankcaseHeater() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorDrybulbTemperatureforCrankcaseHeater, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::defrostStrategy() const {
      boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostStrategy, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::defrostControl() const {
      boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostControl, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::defrostEnergyInputRatioModifierFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName);
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::defrostTimePeriodFraction() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostTimePeriodFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resistiveDefrostHeaterCapacity() const {
      return getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ResistiveDefrostHeaterCapacity, true);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::isResistiveDefrostHeaterCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ResistiveDefrostHeaterCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::maximumOutdoorDrybulbTemperatureforDefrostOperation() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::waterCondenserVolumeFlowRate() const {
      return getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::WaterCondenserVolumeFlowRate, true);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::isWaterCondenserVolumeFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::WaterCondenserVolumeFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::evaporativeCondenserEffectiveness() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::EvaporativeCondenserEffectiveness, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::evaporativeCondenserAirFlowRate() const {
      return getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::EvaporativeCondenserAirFlowRate, true);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::isEvaporativeCondenserAirFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::EvaporativeCondenserAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::evaporativeCondenserPumpRatedPowerConsumption() const {
      return getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::EvaporativeCondenserPumpRatedPowerConsumption, true);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::EvaporativeCondenserPumpRatedPowerConsumption, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::basinHeaterCapacity() const {
      boost::optional<double> value = getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::BasinHeaterCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::basinHeaterSetpointTemperature() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::BasinHeaterSetpointTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::basinHeaterOperatingSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::BasinHeaterOperatingSchedule);
    }

    std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::fuelType() const {
      boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::FuelType, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::minimumOutdoorTemperatureinHeatRecoveryMode() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MinimumOutdoorTemperatureinHeatRecoveryMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::maximumOutdoorTemperatureinHeatRecoveryMode() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorTemperatureinHeatRecoveryMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::heatRecoveryCoolingCapacityModifierCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatRecoveryCoolingCapacityModifierCurveName);
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::initialHeatRecoveryCoolingCapacityFraction() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::InitialHeatRecoveryCoolingCapacityFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::heatRecoveryCoolingCapacityTimeConstant() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatRecoveryCoolingCapacityTimeConstant, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::heatRecoveryCoolingEnergyModifierCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatRecoveryCoolingEnergyModifierCurveName);
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::initialHeatRecoveryCoolingEnergyFraction() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::InitialHeatRecoveryCoolingEnergyFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::heatRecoveryCoolingEnergyTimeConstant() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatRecoveryCoolingEnergyTimeConstant, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::heatRecoveryHeatingCapacityModifierCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatRecoveryHeatingCapacityModifierCurveName);
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::initialHeatRecoveryHeatingCapacityFraction() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::InitialHeatRecoveryHeatingCapacityFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::heatRecoveryHeatingCapacityTimeConstant() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatRecoveryHeatingCapacityTimeConstant, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::heatRecoveryHeatingEnergyModifierCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatRecoveryHeatingEnergyModifierCurveName);
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::initialHeatRecoveryHeatingEnergyFraction() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::InitialHeatRecoveryHeatingEnergyFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::heatRecoveryHeatingEnergyTimeConstant() const {
      boost::optional<double> value =
        getDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatRecoveryHeatingEnergyTimeConstant, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilitySchedule,
                                "AirConditionerVariableRefrigerantFlowFluidTemperatureControl", "Availability Schedule", schedule);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setGrossRatedTotalCoolingCapacity(
      boost::optional<double> grossRatedTotalCoolingCapacity) {
      bool result(false);
      if (grossRatedTotalCoolingCapacity) {
        result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::GrossRatedTotalCoolingCapacity,
                           grossRatedTotalCoolingCapacity.get());
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizeGrossRatedTotalCoolingCapacity() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::GrossRatedTotalCoolingCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setGrossRatedCoolingCOP(double grossRatedCoolingCOP) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::GrossRatedCoolingCOP, grossRatedCoolingCOP);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setMinimumOutdoorTemperatureinCoolingMode(
      double minimumOutdoorTemperatureinCoolingMode) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MinimumOutdoorTemperatureinCoolingMode,
                              minimumOutdoorTemperatureinCoolingMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setMaximumOutdoorTemperatureinCoolingMode(
      double maximumOutdoorTemperatureinCoolingMode) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorTemperatureinCoolingMode,
                              maximumOutdoorTemperatureinCoolingMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setCoolingCapacityRatioModifierFunctionofLowTemperatureCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(
          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingCapacityRatioModifierFunctionofLowTemperatureCurveName,
          curve.get().handle());
      } else {
        resetCoolingCapacityRatioModifierFunctionofLowTemperatureCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetCoolingCapacityRatioModifierFunctionofLowTemperatureCurve() {
      bool result = setString(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingCapacityRatioModifierFunctionofLowTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setCoolingCapacityRatioBoundaryCurve(const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingCapacityRatioBoundaryCurveName,
                            curve.get().handle());
      } else {
        resetCoolingCapacityRatioBoundaryCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetCoolingCapacityRatioBoundaryCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingCapacityRatioBoundaryCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setCoolingCapacityRatioModifierFunctionofHighTemperatureCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(
          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingCapacityRatioModifierFunctionofHighTemperatureCurveName,
          curve.get().handle());
      } else {
        resetCoolingCapacityRatioModifierFunctionofHighTemperatureCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetCoolingCapacityRatioModifierFunctionofHighTemperatureCurve() {
      bool result = setString(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingCapacityRatioModifierFunctionofHighTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(
          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingEnergyInputRatioModifierFunctionofLowTemperatureCurveName,
          curve.get().handle());
      } else {
        resetCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve() {
      bool result = setString(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingEnergyInputRatioModifierFunctionofLowTemperatureCurveName,
        "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setCoolingEnergyInputRatioBoundaryCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingEnergyInputRatioBoundaryCurveName,
                            curve.get().handle());
      } else {
        resetCoolingEnergyInputRatioBoundaryCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetCoolingEnergyInputRatioBoundaryCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingEnergyInputRatioBoundaryCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(
          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingEnergyInputRatioModifierFunctionofHighTemperatureCurveName,
          curve.get().handle());
      } else {
        resetCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve() {
      bool result = setString(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingEnergyInputRatioModifierFunctionofHighTemperatureCurveName,
        "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(
          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName,
          curve.get().handle());
      } else {
        resetCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() {
      bool result = setString(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName,
        "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::
                              CoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName,
                            curve.get().handle());
      } else {
        resetCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() {
      bool result = setString(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName,
        "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setCoolingCombinationRatioCorrectionFactorCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingCombinationRatioCorrectionFactorCurveName,
                            curve.get().handle());
      } else {
        resetCoolingCombinationRatioCorrectionFactorCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetCoolingCombinationRatioCorrectionFactorCurve() {
      bool result =
        setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingCombinationRatioCorrectionFactorCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setCoolingPartLoadFractionCorrelationCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingPartLoadFractionCorrelationCurveName,
                            curve.get().handle());
      } else {
        resetCoolingPartLoadFractionCorrelationCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetCoolingPartLoadFractionCorrelationCurve() {
      bool result =
        setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CoolingPartLoadFractionCorrelationCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setGrossRatedHeatingCapacity(
      boost::optional<double> grossRatedHeatingCapacity) {
      bool result(false);
      if (grossRatedHeatingCapacity) {
        result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::GrossRatedHeatingCapacity,
                           grossRatedHeatingCapacity.get());
      }
      OS_ASSERT(result);
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizeGrossRatedHeatingCapacity() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::GrossRatedHeatingCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setRatedHeatingCapacitySizingRatio(double ratedHeatingCapacitySizingRatio) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedHeatingCapacitySizingRatio,
                              ratedHeatingCapacitySizingRatio);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setRatedHeatingCOP(double ratedHeatingCOP) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatedHeatingCOP, ratedHeatingCOP);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setMinimumOutdoorTemperatureinHeatingMode(
      double minimumOutdoorTemperatureinHeatingMode) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MinimumOutdoorTemperatureinHeatingMode,
                              minimumOutdoorTemperatureinHeatingMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setMaximumOutdoorTemperatureinHeatingMode(
      double maximumOutdoorTemperatureinHeatingMode) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorTemperatureinHeatingMode,
                              maximumOutdoorTemperatureinHeatingMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setHeatingCapacityRatioModifierFunctionofLowTemperatureCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(
          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingCapacityRatioModifierFunctionofLowTemperatureCurveName,
          curve.get().handle());
      } else {
        resetHeatingCapacityRatioModifierFunctionofLowTemperatureCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetHeatingCapacityRatioModifierFunctionofLowTemperatureCurve() {
      bool result = setString(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingCapacityRatioModifierFunctionofLowTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setHeatingCapacityRatioBoundaryCurve(const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingCapacityRatioBoundaryCurveName,
                            curve.get().handle());
      } else {
        resetHeatingCapacityRatioBoundaryCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetHeatingCapacityRatioBoundaryCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingCapacityRatioBoundaryCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setHeatingCapacityRatioModifierFunctionofHighTemperatureCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(
          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingCapacityRatioModifierFunctionofHighTemperatureCurveName,
          curve.get().handle());
      } else {
        resetHeatingCapacityRatioModifierFunctionofHighTemperatureCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetHeatingCapacityRatioModifierFunctionofHighTemperatureCurve() {
      bool result = setString(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingCapacityRatioModifierFunctionofHighTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(
          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingEnergyInputRatioModifierFunctionofLowTemperatureCurveName,
          curve.get().handle());
      } else {
        resetHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve() {
      bool result = setString(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingEnergyInputRatioModifierFunctionofLowTemperatureCurveName,
        "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setHeatingEnergyInputRatioBoundaryCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingEnergyInputRatioBoundaryCurveName,
                            curve.get().handle());
      } else {
        resetHeatingEnergyInputRatioBoundaryCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetHeatingEnergyInputRatioBoundaryCurve() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingEnergyInputRatioBoundaryCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(
          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingEnergyInputRatioModifierFunctionofHighTemperatureCurveName,
          curve.get().handle());
      } else {
        resetHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve() {
      bool result = setString(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingEnergyInputRatioModifierFunctionofHighTemperatureCurveName,
        "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setHeatingPerformanceCurveOutdoorTemperatureType(
      std::string heatingPerformanceCurveOutdoorTemperatureType) {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingPerformanceCurveOutdoorTemperatureType,
                              heatingPerformanceCurveOutdoorTemperatureType);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(
          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName,
          curve.get().handle());
      } else {
        resetHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() {
      bool result = setString(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurveName,
        "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::
                              HeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName,
                            curve.get().handle());
      } else {
        resetHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() {
      bool result = setString(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurveName,
        "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setHeatingCombinationRatioCorrectionFactorCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingCombinationRatioCorrectionFactorCurveName,
                            curve.get().handle());
      } else {
        resetHeatingCombinationRatioCorrectionFactorCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetHeatingCombinationRatioCorrectionFactorCurve() {
      bool result =
        setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingCombinationRatioCorrectionFactorCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setHeatingPartLoadFractionCorrelationCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingPartLoadFractionCorrelationCurveName,
                            curve.get().handle());
      } else {
        resetHeatingPartLoadFractionCorrelationCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetHeatingPartLoadFractionCorrelationCurve() {
      bool result =
        setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingPartLoadFractionCorrelationCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setMinimumHeatPumpPartLoadRatio(double minimumHeatPumpPartLoadRatio) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MinimumHeatPumpPartLoadRatio,
                              minimumHeatPumpPartLoadRatio);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setZoneforMasterThermostatLocation(
      const boost::optional<ThermalZone>& zone) {
      bool result(false);
      if (zone) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ZoneNameforMasterThermostatLocation,
                            zone.get().handle());
      } else {
        resetZoneforMasterThermostatLocation();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetZoneforMasterThermostatLocation() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ZoneNameforMasterThermostatLocation, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setMasterThermostatPriorityControlType(
      std::string masterThermostatPriorityControlType) {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MasterThermostatPriorityControlType,
                              masterThermostatPriorityControlType);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setThermostatPrioritySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ThermostatPrioritySchedule,
                                "AirConditionerVariableRefrigerantFlowFluidTemperatureControl", "Thermostat Priority Schedule", schedule);
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetThermostatPrioritySchedule() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ThermostatPrioritySchedule, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setHeatPumpWasteHeatRecovery(bool heatPumpWasteHeatRecovery) {
      return setBooleanFieldValue(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatPumpWasteHeatRecovery,
                                  heatPumpWasteHeatRecovery);
      ;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setEquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode(
      double equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode) {
      bool result = setDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::EquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode,
        equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setVerticalHeightusedforPipingCorrectionFactor(
      double verticalHeightusedforPipingCorrectionFactor) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::VerticalHeightusedforPipingCorrectionFactor,
                              verticalHeightusedforPipingCorrectionFactor);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setPipingCorrectionFactorforLengthinCoolingModeCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result =
          setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::PipingCorrectionFactorforLengthinCoolingModeCurveName,
                     curve.get().handle());
      } else {
        resetPipingCorrectionFactorforLengthinCoolingModeCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetPipingCorrectionFactorforLengthinCoolingModeCurve() {
      bool result =
        setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::PipingCorrectionFactorforLengthinCoolingModeCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setPipingCorrectionFactorforHeightinCoolingModeCoefficient(
      double pipingCorrectionFactorforHeightinCoolingModeCoefficient) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::PipingCorrectionFactorforHeightinCoolingModeCoefficient,
                  pipingCorrectionFactorforHeightinCoolingModeCoefficient);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setEquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode(
      double equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode) {
      bool result = setDouble(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::EquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode,
        equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setPipingCorrectionFactorforLengthinHeatingModeCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result =
          setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::PipingCorrectionFactorforLengthinHeatingModeCurveName,
                     curve.get().handle());
      } else {
        resetPipingCorrectionFactorforLengthinHeatingModeCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetPipingCorrectionFactorforLengthinHeatingModeCurve() {
      bool result =
        setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::PipingCorrectionFactorforLengthinHeatingModeCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setPipingCorrectionFactorforHeightinHeatingModeCoefficient(
      double pipingCorrectionFactorforHeightinHeatingModeCoefficient) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::PipingCorrectionFactorforHeightinHeatingModeCoefficient,
                  pipingCorrectionFactorforHeightinHeatingModeCoefficient);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setCrankcaseHeaterPowerperCompressor(
      double crankcaseHeaterPowerperCompressor) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CrankcaseHeaterPowerperCompressor,
                              crankcaseHeaterPowerperCompressor);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setNumberofCompressors(int numberofCompressors) {
      bool result = setInt(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::NumberofCompressors, numberofCompressors);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setRatioofCompressorSizetoTotalCompressorCapacity(
      double ratioofCompressorSizetoTotalCompressorCapacity) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::RatioofCompressorSizetoTotalCompressorCapacity,
                              ratioofCompressorSizetoTotalCompressorCapacity);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setMaximumOutdoorDrybulbTemperatureforCrankcaseHeater(
      double maximumOutdoorDrybulbTemperatureforCrankcaseHeater) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorDrybulbTemperatureforCrankcaseHeater,
                  maximumOutdoorDrybulbTemperatureforCrankcaseHeater);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setDefrostStrategy(std::string defrostStrategy) {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostStrategy, defrostStrategy);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setDefrostControl(std::string defrostControl) {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostControl, defrostControl);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(
          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName,
          curve.get().handle());
      } else {
        resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve() {
      bool result = setString(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostEnergyInputRatioModifierFunctionofTemperatureCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setDefrostTimePeriodFraction(double defrostTimePeriodFraction) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostTimePeriodFraction, defrostTimePeriodFraction);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setResistiveDefrostHeaterCapacity(
      boost::optional<double> resistiveDefrostHeaterCapacity) {
      bool result(false);
      if (resistiveDefrostHeaterCapacity) {
        result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ResistiveDefrostHeaterCapacity,
                           resistiveDefrostHeaterCapacity.get());
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizeResistiveDefrostHeaterCapacity() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ResistiveDefrostHeaterCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setMaximumOutdoorDrybulbTemperatureforDefrostOperation(
      double maximumOutdoorDrybulbTemperatureforDefrostOperation) {
      bool result =
        setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorDrybulbTemperatureforDefrostOperation,
                  maximumOutdoorDrybulbTemperatureforDefrostOperation);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setWaterCondenserVolumeFlowRate(
      boost::optional<double> waterCondenserVolumeFlowRate) {
      bool result(false);
      if (waterCondenserVolumeFlowRate) {
        result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::WaterCondenserVolumeFlowRate,
                           waterCondenserVolumeFlowRate.get());
      }
      OS_ASSERT(result);
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizeWaterCondenserVolumeFlowRate() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::WaterCondenserVolumeFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setEvaporativeCondenserEffectiveness(
      double evaporativeCondenserEffectiveness) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::EvaporativeCondenserEffectiveness,
                              evaporativeCondenserEffectiveness);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setEvaporativeCondenserAirFlowRate(
      boost::optional<double> evaporativeCondenserAirFlowRate) {
      bool result(false);
      if (evaporativeCondenserAirFlowRate) {
        result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::EvaporativeCondenserAirFlowRate,
                           evaporativeCondenserAirFlowRate.get());
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizeEvaporativeCondenserAirFlowRate() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::EvaporativeCondenserAirFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setEvaporativeCondenserPumpRatedPowerConsumption(
      boost::optional<double> evaporativeCondenserPumpRatedPowerConsumption) {
      bool result(false);
      if (evaporativeCondenserPumpRatedPowerConsumption) {
        result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::EvaporativeCondenserPumpRatedPowerConsumption,
                           evaporativeCondenserPumpRatedPowerConsumption.get());
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizeEvaporativeCondenserPumpRatedPowerConsumption() {
      bool result =
        setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::EvaporativeCondenserPumpRatedPowerConsumption, "autosize");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setBasinHeaterCapacity(double basinHeaterCapacity) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::BasinHeaterCapacity, basinHeaterCapacity);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::BasinHeaterSetpointTemperature,
                              basinHeaterSetpointTemperature);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setBasinHeaterOperatingSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::BasinHeaterOperatingSchedule,
                                "AirConditionerVariableRefrigerantFlowFluidTemperatureControl", "Basin Heater Operating Schedule", schedule);
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetBasinHeaterOperatingSchedule() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::BasinHeaterOperatingSchedule, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setFuelType(std::string fuelType) {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::FuelType, fuelType);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setMinimumOutdoorTemperatureinHeatRecoveryMode(
      double minimumOutdoorTemperatureinHeatRecoveryMode) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MinimumOutdoorTemperatureinHeatRecoveryMode,
                              minimumOutdoorTemperatureinHeatRecoveryMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setMaximumOutdoorTemperatureinHeatRecoveryMode(
      double maximumOutdoorTemperatureinHeatRecoveryMode) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MaximumOutdoorTemperatureinHeatRecoveryMode,
                              maximumOutdoorTemperatureinHeatRecoveryMode);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setHeatRecoveryCoolingCapacityModifierCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatRecoveryCoolingCapacityModifierCurveName,
                            curve.get().handle());
      } else {
        resetHeatRecoveryCoolingCapacityModifierCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetHeatRecoveryCoolingCapacityModifierCurve() {
      bool result =
        setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatRecoveryCoolingCapacityModifierCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setInitialHeatRecoveryCoolingCapacityFraction(
      double initialHeatRecoveryCoolingCapacityFraction) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::InitialHeatRecoveryCoolingCapacityFraction,
                              initialHeatRecoveryCoolingCapacityFraction);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setHeatRecoveryCoolingCapacityTimeConstant(
      double heatRecoveryCoolingCapacityTimeConstant) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatRecoveryCoolingCapacityTimeConstant,
                              heatRecoveryCoolingCapacityTimeConstant);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setHeatRecoveryCoolingEnergyModifierCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatRecoveryCoolingEnergyModifierCurveName,
                            curve.get().handle());
      } else {
        resetHeatRecoveryCoolingEnergyModifierCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetHeatRecoveryCoolingEnergyModifierCurve() {
      bool result =
        setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatRecoveryCoolingEnergyModifierCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setInitialHeatRecoveryCoolingEnergyFraction(
      double initialHeatRecoveryCoolingEnergyFraction) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::InitialHeatRecoveryCoolingEnergyFraction,
                              initialHeatRecoveryCoolingEnergyFraction);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setHeatRecoveryCoolingEnergyTimeConstant(
      double heatRecoveryCoolingEnergyTimeConstant) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatRecoveryCoolingEnergyTimeConstant,
                              heatRecoveryCoolingEnergyTimeConstant);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setHeatRecoveryHeatingCapacityModifierCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatRecoveryHeatingCapacityModifierCurveName,
                            curve.get().handle());
      } else {
        resetHeatRecoveryHeatingCapacityModifierCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetHeatRecoveryHeatingCapacityModifierCurve() {
      bool result =
        setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatRecoveryHeatingCapacityModifierCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setInitialHeatRecoveryHeatingCapacityFraction(
      double initialHeatRecoveryHeatingCapacityFraction) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::InitialHeatRecoveryHeatingCapacityFraction,
                              initialHeatRecoveryHeatingCapacityFraction);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setHeatRecoveryHeatingCapacityTimeConstant(
      double heatRecoveryHeatingCapacityTimeConstant) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatRecoveryHeatingCapacityTimeConstant,
                              heatRecoveryHeatingCapacityTimeConstant);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setHeatRecoveryHeatingEnergyModifierCurve(
      const boost::optional<Curve>& curve) {
      bool result(false);
      if (curve) {
        result = setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatRecoveryHeatingEnergyModifierCurveName,
                            curve.get().handle());
      } else {
        resetHeatRecoveryHeatingEnergyModifierCurve();
        result = true;
      }
      return result;
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetHeatRecoveryHeatingEnergyModifierCurve() {
      bool result =
        setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatRecoveryHeatingEnergyModifierCurveName, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setInitialHeatRecoveryHeatingEnergyFraction(
      double initialHeatRecoveryHeatingEnergyFraction) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::InitialHeatRecoveryHeatingEnergyFraction,
                              initialHeatRecoveryHeatingEnergyFraction);
      OS_ASSERT(result);
      return result;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setHeatRecoveryHeatingEnergyTimeConstant(
      double heatRecoveryHeatingEnergyTimeConstant) {
      bool result = setDouble(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatRecoveryHeatingEnergyTimeConstant,
                              heatRecoveryHeatingEnergyTimeConstant);
      OS_ASSERT(result);
      return result;
    }

    boost::optional<Schedule> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::AvailabilitySchedule);
    }

    unsigned AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::inletPort() const {
      return OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CondenserInletNode;
    }

    unsigned AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::outletPort() const {
      return OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CondenserOutletNode;
    }

    ModelObjectList AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::vrfModelObjectList() const {
      boost::optional<ModelObjectList> mo = getObject<ModelObject>().getModelObjectTarget<ModelObjectList>(
        OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ZoneTerminalUnitList);

      OS_ASSERT(mo);

      return mo.get();
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setVRFModelObjectList(const ModelObjectList& modelObjectList) {
      return setPointer(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::ZoneTerminalUnitList, modelObjectList.handle());
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::addTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf) {
      vrfModelObjectList().addModelObject(vrf);
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::removeTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf) {
      vrfModelObjectList().removeModelObject(vrf);
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::removeAllTerminals() {
      vrfModelObjectList().removeAllModelObjects();
    }

    std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::terminals() const {
      return subsetCastVector<ZoneHVACTerminalUnitVariableRefrigerantFlow>(vrfModelObjectList().modelObjects());
    }

    std::vector<ModelObject> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::children() const {
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

    ModelObject AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::clone(Model model) const {
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl airConditionerClone =
        StraightComponent_Impl::clone(model).cast<AirConditionerVariableRefrigerantFlowFluidTemperatureControl>();

      ModelObjectList modelObjectList(model);
      airConditionerClone.getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setVRFModelObjectList(
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

    std::vector<openstudio::IdfObject> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::remove() {
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

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizedGrossRatedTotalCoolingCapacity() const {
      return getAutosizedValue("Design Size Rated Total Cooling Capacity (gross)", "W");
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizedGrossRatedHeatingCapacity() const {
      return getAutosizedValue("Design Size Rated Total Heating Capacity", "W");
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizedResistiveDefrostHeaterCapacity() const {
      return getAutosizedValue("Design Size Resistive Defrost Heater Capacity", "");
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizedWaterCondenserVolumeFlowRate() const {
      return getAutosizedValue("Design Size Water Condenser Volume Flow Rate", "m3/s");
    }

    boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizedEvaporativeCondenserAirFlowRate() const {
      return getAutosizedValue("Design Size Evaporative Condenser Air Flow Rate", "m3/s");
    }

    boost::optional<double>
      AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::autosizedEvaporativeCondenserPumpRatedPowerConsumption() const {
      return getAutosizedValue("Design Size Evaporative Condenser Pump Rated Power Consumption", "W");
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::autosize() {
      autosizeGrossRatedTotalCoolingCapacity();
      autosizeGrossRatedHeatingCapacity();
      autosizeResistiveDefrostHeaterCapacity();
      autosizeWaterCondenserVolumeFlowRate();
      autosizeEvaporativeCondenserAirFlowRate();
      autosizeEvaporativeCondenserPumpRatedPowerConsumption();
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::applySizingValues() {
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

    std::vector<EMSActuatorNames> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"Variable Refrigerant Flow Heat Pump", "Operating Mode"}};
      return actuators;
    }

    std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types;
      return types;
    }

    std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::condenserType() const {
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
        boost::optional<std::string> value = getString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CondenserType, false);
        condenserType = value.get();
      }
      return condenserType;
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::setCondenserType(const std::string& condenserType) {

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
      return setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CondenserType, condenserType);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::isCondenserTypeDefaulted() const {
      return isEmpty(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CondenserType);
    }

    void AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::resetCondenserType() {
      bool result = setString(OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::CondenserType, "");
      OS_ASSERT(result);
    }

    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::addToNode(Node& node) {
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
    bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl::removeFromLoop() {
      // Disconnect the component
      bool ok = StraightComponent_Impl::removeFromLoop();

      // Don't Switch the condenser type to "AirCooled"
      // this->setCondenserType("AirCooled");
      return ok;
    }

  }  // namespace detail

  AirConditionerVariableRefrigerantFlowFluidTemperatureControl::AirConditionerVariableRefrigerantFlowFluidTemperatureControl(const Model& model)
    : StraightComponent(AirConditionerVariableRefrigerantFlowFluidTemperatureControl::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>());

    Schedule schedule = model.alwaysOnDiscreteSchedule();

    setAvailabilitySchedule(schedule);

    setRefrigerantType("R401A");

    setRatedEvaporativeCapacity(40000);

    setRatedCompressorPowerPerUnitofRatedEvaporativeCapacity(0.35);

    setMinimumOutdoorAirTemperatureinCoolingMode(-6.0);

    setMaximumOutdoorAirTemperatureinCoolingMode(43.0);

    setMinimumOutdoorAirTemperatureinHeatingMode(-20.0);

    setMaximumOutdoorAirTemperatureinHeatingMode(16.0);

    setReferenceOutdoorUnitSuperheating(3);

    setReferenceOutdoorUnitSubcooling(5);

    setRefrigerantTemperatureControlAlgorithmforIndoorUnit("VariableTemp");

    setReferenceEvaporatingTemperatureforIndoorUnit(6.0);

    setReferenceCondensingTemperatureforIndoorUnit(44.0);

    setVariableEvaporatingTemperatureMinimumforIndoorUnit(4.0);

    setVariableEvaporatingTemperatureMaximumforIndoorUnit(13.0);

    setVariableCondensingTemperatureMinimumforIndoorUnit(42.0);

    setVariableCondensingTemperatureMaximumforIndoorUnit(46.0);

    setOutdoorUnitFanPowerPerUnitofRatedEvaporativeCapacity(4.25E-3);

    setOutdoorUnitFanFlowRatePerUnitofRatedEvaporativeCapacity(7.50E-5);

    setOutdoorUnitEvaporatingTemperatureFunctionofSuperheatingCurve();

    setOutdoorUnitCondensingTemperatureFunctionofSubcoolingCurve();

    setDiameterofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(0.0762);

    setLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(30.0);

    setEquivalentLengthofMainPipeConnectingOutdoorUnittotheFirstBranchJoint(36.0);

    setHeightDifferenceBetweenOutdoorUnitandIndoorUnits(5.0);

    setMainPipeInsulationThickness(0.02);

    setMainPipeInsulationThermalConductivity(0.032);

    ModelObjectList vrfModelObjectList(model);
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setVRFModelObjectList(vrfModelObjectList);
  }

  IddObjectType AirConditionerVariableRefrigerantFlowFluidTemperatureControl::iddObjectType() {
    return IddObjectType(IddObjectType::OS_AirConditioner_VariableRefrigerantFlow);
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::heatingPerformanceCurveOutdoorTemperatureTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::HeatingPerformanceCurveOutdoorTemperatureType);
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::masterThermostatPriorityControlTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::MasterThermostatPriorityControlType);
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::defrostStrategyValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostStrategy);
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::defrostControlValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::DefrostControl);
  }

  std::vector<std::string> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::fuelTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirConditioner_VariableRefrigerantFlow_FluidTemperatureControlFields::FuelType);
  }

  Schedule AirConditionerVariableRefrigerantFlowFluidTemperatureControl::availabilitySchedule() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->availabilitySchedule();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::minimumOutdoorTemperatureinCoolingMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->minimumOutdoorTemperatureinCoolingMode();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::maximumOutdoorTemperatureinCoolingMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->maximumOutdoorTemperatureinCoolingMode();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::coolingCapacityRatioModifierFunctionofLowTemperatureCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->coolingCapacityRatioModifierFunctionofLowTemperatureCurve();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::coolingCapacityRatioBoundaryCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->coolingCapacityRatioBoundaryCurve();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::coolingCapacityRatioModifierFunctionofHighTemperatureCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->coolingCapacityRatioModifierFunctionofHighTemperatureCurve();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->coolingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::coolingEnergyInputRatioBoundaryCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->coolingEnergyInputRatioBoundaryCurve();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->coolingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->coolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->coolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::coolingCombinationRatioCorrectionFactorCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->coolingCombinationRatioCorrectionFactorCurve();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::coolingPartLoadFractionCorrelationCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->coolingPartLoadFractionCorrelationCurve();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::ratedHeatingCOP() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->ratedHeatingCOP();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::minimumOutdoorTemperatureinHeatingMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->minimumOutdoorTemperatureinHeatingMode();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::maximumOutdoorTemperatureinHeatingMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->maximumOutdoorTemperatureinHeatingMode();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::heatingCapacityRatioModifierFunctionofLowTemperatureCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->heatingCapacityRatioModifierFunctionofLowTemperatureCurve();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::heatingCapacityRatioBoundaryCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->heatingCapacityRatioBoundaryCurve();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::heatingCapacityRatioModifierFunctionofHighTemperatureCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->heatingCapacityRatioModifierFunctionofHighTemperatureCurve();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->heatingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::heatingEnergyInputRatioBoundaryCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->heatingEnergyInputRatioBoundaryCurve();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->heatingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
  }

  std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControl::heatingPerformanceCurveOutdoorTemperatureType() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->heatingPerformanceCurveOutdoorTemperatureType();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->heatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->heatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::heatingCombinationRatioCorrectionFactorCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->heatingCombinationRatioCorrectionFactorCurve();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::heatingPartLoadFractionCorrelationCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->heatingPartLoadFractionCorrelationCurve();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::minimumHeatPumpPartLoadRatio() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->minimumHeatPumpPartLoadRatio();
  }

  boost::optional<ThermalZone> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::zoneforMasterThermostatLocation() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->zoneforMasterThermostatLocation();
  }

  std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControl::masterThermostatPriorityControlType() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->masterThermostatPriorityControlType();
  }

  boost::optional<Schedule> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::thermostatPrioritySchedule() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->thermostatPrioritySchedule();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::heatPumpWasteHeatRecovery() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->heatPumpWasteHeatRecovery();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::verticalHeightusedforPipingCorrectionFactor() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->verticalHeightusedforPipingCorrectionFactor();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::pipingCorrectionFactorforLengthinCoolingModeCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->pipingCorrectionFactorforLengthinCoolingModeCurve();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::pipingCorrectionFactorforHeightinCoolingModeCoefficient() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->pipingCorrectionFactorforHeightinCoolingModeCoefficient();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::pipingCorrectionFactorforLengthinHeatingModeCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->pipingCorrectionFactorforLengthinHeatingModeCurve();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::pipingCorrectionFactorforHeightinHeatingModeCoefficient() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->pipingCorrectionFactorforHeightinHeatingModeCoefficient();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::crankcaseHeaterPowerperCompressor() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->crankcaseHeaterPowerperCompressor();
  }

  int AirConditionerVariableRefrigerantFlowFluidTemperatureControl::numberofCompressors() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->numberofCompressors();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::ratioofCompressorSizetoTotalCompressorCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->ratioofCompressorSizetoTotalCompressorCapacity();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::maximumOutdoorDrybulbTemperatureforCrankcaseHeater() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->maximumOutdoorDrybulbTemperatureforCrankcaseHeater();
  }

  std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControl::defrostStrategy() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->defrostStrategy();
  }

  std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControl::defrostControl() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->defrostControl();
  }

  boost::optional<Curve>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::defrostEnergyInputRatioModifierFunctionofTemperatureCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->defrostEnergyInputRatioModifierFunctionofTemperatureCurve();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::defrostTimePeriodFraction() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->defrostTimePeriodFraction();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resistiveDefrostHeaterCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resistiveDefrostHeaterCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::isResistiveDefrostHeaterCapacityAutosized() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->isResistiveDefrostHeaterCapacityAutosized();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::maximumOutdoorDrybulbTemperatureforDefrostOperation() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->maximumOutdoorDrybulbTemperatureforDefrostOperation();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::waterCondenserVolumeFlowRate() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->waterCondenserVolumeFlowRate();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::isWaterCondenserVolumeFlowRateAutosized() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->isWaterCondenserVolumeFlowRateAutosized();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::evaporativeCondenserEffectiveness() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->evaporativeCondenserEffectiveness();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::evaporativeCondenserAirFlowRate() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->evaporativeCondenserAirFlowRate();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::isEvaporativeCondenserAirFlowRateAutosized() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->isEvaporativeCondenserAirFlowRateAutosized();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::evaporativeCondenserPumpRatedPowerConsumption() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->evaporativeCondenserPumpRatedPowerConsumption();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->isEvaporativeCondenserPumpRatedPowerConsumptionAutosized();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::basinHeaterCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->basinHeaterCapacity();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::basinHeaterSetpointTemperature() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->basinHeaterSetpointTemperature();
  }

  boost::optional<Schedule> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::basinHeaterOperatingSchedule() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->basinHeaterOperatingSchedule();
  }

  std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControl::fuelType() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->fuelType();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::minimumOutdoorTemperatureinHeatRecoveryMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->minimumOutdoorTemperatureinHeatRecoveryMode();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::maximumOutdoorTemperatureinHeatRecoveryMode() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->maximumOutdoorTemperatureinHeatRecoveryMode();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::heatRecoveryCoolingCapacityModifierCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->heatRecoveryCoolingCapacityModifierCurve();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::initialHeatRecoveryCoolingCapacityFraction() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->initialHeatRecoveryCoolingCapacityFraction();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::heatRecoveryCoolingCapacityTimeConstant() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->heatRecoveryCoolingCapacityTimeConstant();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::heatRecoveryCoolingEnergyModifierCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->heatRecoveryCoolingEnergyModifierCurve();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::initialHeatRecoveryCoolingEnergyFraction() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->initialHeatRecoveryCoolingEnergyFraction();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::heatRecoveryCoolingEnergyTimeConstant() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->heatRecoveryCoolingEnergyTimeConstant();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::heatRecoveryHeatingCapacityModifierCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->heatRecoveryHeatingCapacityModifierCurve();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::initialHeatRecoveryHeatingCapacityFraction() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->initialHeatRecoveryHeatingCapacityFraction();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::heatRecoveryHeatingCapacityTimeConstant() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->heatRecoveryHeatingCapacityTimeConstant();
  }

  boost::optional<Curve> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::heatRecoveryHeatingEnergyModifierCurve() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->heatRecoveryHeatingEnergyModifierCurve();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::initialHeatRecoveryHeatingEnergyFraction() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->initialHeatRecoveryHeatingEnergyFraction();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::heatRecoveryHeatingEnergyTimeConstant() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->heatRecoveryHeatingEnergyTimeConstant();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setMinimumOutdoorTemperatureinCoolingMode(
    double minimumOutdoorTemperatureinCoolingMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setMinimumOutdoorTemperatureinCoolingMode(
      minimumOutdoorTemperatureinCoolingMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setMaximumOutdoorTemperatureinCoolingMode(
    double maximumOutdoorTemperatureinCoolingMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setMaximumOutdoorTemperatureinCoolingMode(
      maximumOutdoorTemperatureinCoolingMode);
  }

  bool
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setCoolingCapacityRatioModifierFunctionofLowTemperatureCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setCoolingCapacityRatioModifierFunctionofLowTemperatureCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetCoolingCapacityRatioModifierFunctionofLowTemperatureCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->resetCoolingCapacityRatioModifierFunctionofLowTemperatureCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setCoolingCapacityRatioBoundaryCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setCoolingCapacityRatioBoundaryCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetCoolingCapacityRatioBoundaryCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resetCoolingCapacityRatioBoundaryCurve();
  }

  bool
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setCoolingCapacityRatioModifierFunctionofHighTemperatureCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setCoolingCapacityRatioModifierFunctionofHighTemperatureCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetCoolingCapacityRatioModifierFunctionofHighTemperatureCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->resetCoolingCapacityRatioModifierFunctionofHighTemperatureCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve(
    const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->resetCoolingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setCoolingEnergyInputRatioBoundaryCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setCoolingEnergyInputRatioBoundaryCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetCoolingEnergyInputRatioBoundaryCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resetCoolingEnergyInputRatioBoundaryCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve(
    const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->resetCoolingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(
    const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->resetCoolingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(
    const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->resetCoolingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setCoolingCombinationRatioCorrectionFactorCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setCoolingCombinationRatioCorrectionFactorCurve(
      curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetCoolingCombinationRatioCorrectionFactorCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resetCoolingCombinationRatioCorrectionFactorCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setCoolingPartLoadFractionCorrelationCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setCoolingPartLoadFractionCorrelationCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetCoolingPartLoadFractionCorrelationCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resetCoolingPartLoadFractionCorrelationCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setRatedHeatingCOP(double ratedHeatingCOP) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setRatedHeatingCOP(ratedHeatingCOP);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setMinimumOutdoorTemperatureinHeatingMode(
    double minimumOutdoorTemperatureinHeatingMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setMinimumOutdoorTemperatureinHeatingMode(
      minimumOutdoorTemperatureinHeatingMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setMaximumOutdoorTemperatureinHeatingMode(
    double maximumOutdoorTemperatureinHeatingMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setMaximumOutdoorTemperatureinHeatingMode(
      maximumOutdoorTemperatureinHeatingMode);
  }

  bool
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setHeatingCapacityRatioModifierFunctionofLowTemperatureCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setHeatingCapacityRatioModifierFunctionofLowTemperatureCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetHeatingCapacityRatioModifierFunctionofLowTemperatureCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->resetHeatingCapacityRatioModifierFunctionofLowTemperatureCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setHeatingCapacityRatioBoundaryCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setHeatingCapacityRatioBoundaryCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetHeatingCapacityRatioBoundaryCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resetHeatingCapacityRatioBoundaryCurve();
  }

  bool
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setHeatingCapacityRatioModifierFunctionofHighTemperatureCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setHeatingCapacityRatioModifierFunctionofHighTemperatureCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetHeatingCapacityRatioModifierFunctionofHighTemperatureCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->resetHeatingCapacityRatioModifierFunctionofHighTemperatureCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve(
    const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->resetHeatingEnergyInputRatioModifierFunctionofLowTemperatureCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setHeatingEnergyInputRatioBoundaryCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setHeatingEnergyInputRatioBoundaryCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetHeatingEnergyInputRatioBoundaryCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resetHeatingEnergyInputRatioBoundaryCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve(
    const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->resetHeatingEnergyInputRatioModifierFunctionofHighTemperatureCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setHeatingPerformanceCurveOutdoorTemperatureType(
    std::string heatingPerformanceCurveOutdoorTemperatureType) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setHeatingPerformanceCurveOutdoorTemperatureType(
      heatingPerformanceCurveOutdoorTemperatureType);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(
    const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->resetHeatingEnergyInputRatioModifierFunctionofLowPartLoadRatioCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(
    const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->resetHeatingEnergyInputRatioModifierFunctionofHighPartLoadRatioCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setHeatingCombinationRatioCorrectionFactorCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setHeatingCombinationRatioCorrectionFactorCurve(
      curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetHeatingCombinationRatioCorrectionFactorCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resetHeatingCombinationRatioCorrectionFactorCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setHeatingPartLoadFractionCorrelationCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setHeatingPartLoadFractionCorrelationCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetHeatingPartLoadFractionCorrelationCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resetHeatingPartLoadFractionCorrelationCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setMinimumHeatPumpPartLoadRatio(double minimumHeatPumpPartLoadRatio) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setMinimumHeatPumpPartLoadRatio(
      minimumHeatPumpPartLoadRatio);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setZoneforMasterThermostatLocation(const ThermalZone& zone) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setZoneforMasterThermostatLocation(zone);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetZoneforMasterThermostatLocation() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resetZoneforMasterThermostatLocation();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setMasterThermostatPriorityControlType(
    std::string masterThermostatPriorityControlType) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setMasterThermostatPriorityControlType(
      masterThermostatPriorityControlType);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setThermostatPrioritySchedule(Schedule& schedule) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setThermostatPrioritySchedule(schedule);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetThermostatPrioritySchedule() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resetThermostatPrioritySchedule();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setHeatPumpWasteHeatRecovery(bool heatPumpWasteHeatRecovery) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setHeatPumpWasteHeatRecovery(
      heatPumpWasteHeatRecovery);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setEquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode(
    double equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setEquivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode(equivalentPipingLengthusedforPipingCorrectionFactorinCoolingMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setVerticalHeightusedforPipingCorrectionFactor(
    double verticalHeightusedforPipingCorrectionFactor) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setVerticalHeightusedforPipingCorrectionFactor(
      verticalHeightusedforPipingCorrectionFactor);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setPipingCorrectionFactorforLengthinCoolingModeCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setPipingCorrectionFactorforLengthinCoolingModeCurve(
      curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetPipingCorrectionFactorforLengthinCoolingModeCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resetPipingCorrectionFactorforLengthinCoolingModeCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setPipingCorrectionFactorforHeightinCoolingModeCoefficient(
    double pipingCorrectionFactorforHeightinCoolingModeCoefficient) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setPipingCorrectionFactorforHeightinCoolingModeCoefficient(pipingCorrectionFactorforHeightinCoolingModeCoefficient);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setEquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode(
    double equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setEquivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode(equivalentPipingLengthusedforPipingCorrectionFactorinHeatingMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setPipingCorrectionFactorforLengthinHeatingModeCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setPipingCorrectionFactorforLengthinHeatingModeCurve(
      curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetPipingCorrectionFactorforLengthinHeatingModeCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resetPipingCorrectionFactorforLengthinHeatingModeCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setPipingCorrectionFactorforHeightinHeatingModeCoefficient(
    double pipingCorrectionFactorforHeightinHeatingModeCoefficient) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setPipingCorrectionFactorforHeightinHeatingModeCoefficient(pipingCorrectionFactorforHeightinHeatingModeCoefficient);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setCrankcaseHeaterPowerperCompressor(double crankcaseHeaterPowerperCompressor) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setCrankcaseHeaterPowerperCompressor(
      crankcaseHeaterPowerperCompressor);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setNumberofCompressors(int numberofCompressors) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setNumberofCompressors(numberofCompressors);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setRatioofCompressorSizetoTotalCompressorCapacity(
    double ratioofCompressorSizetoTotalCompressorCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setRatioofCompressorSizetoTotalCompressorCapacity(
      ratioofCompressorSizetoTotalCompressorCapacity);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setMaximumOutdoorDrybulbTemperatureforCrankcaseHeater(
    double maximumOutdoorDrybulbTemperatureforCrankcaseHeater) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setMaximumOutdoorDrybulbTemperatureforCrankcaseHeater(maximumOutdoorDrybulbTemperatureforCrankcaseHeater);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setDefrostStrategy(std::string defrostStrategy) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setDefrostStrategy(defrostStrategy);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setDefrostControl(std::string defrostControl) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setDefrostControl(defrostControl);
  }

  bool
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setDefrostEnergyInputRatioModifierFunctionofTemperatureCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->resetDefrostEnergyInputRatioModifierFunctionofTemperatureCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setDefrostTimePeriodFraction(double defrostTimePeriodFraction) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setDefrostTimePeriodFraction(
      defrostTimePeriodFraction);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setResistiveDefrostHeaterCapacity(double resistiveDefrostHeaterCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setResistiveDefrostHeaterCapacity(
      resistiveDefrostHeaterCapacity);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::autosizeResistiveDefrostHeaterCapacity() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizeResistiveDefrostHeaterCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setMaximumOutdoorDrybulbTemperatureforDefrostOperation(
    double maximumOutdoorDrybulbTemperatureforDefrostOperation) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->setMaximumOutdoorDrybulbTemperatureforDefrostOperation(maximumOutdoorDrybulbTemperatureforDefrostOperation);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setWaterCondenserVolumeFlowRate(double waterCondenserVolumeFlowRate) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setWaterCondenserVolumeFlowRate(
      waterCondenserVolumeFlowRate);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::autosizeWaterCondenserVolumeFlowRate() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizeWaterCondenserVolumeFlowRate();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setEvaporativeCondenserEffectiveness(
      evaporativeCondenserEffectiveness);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setEvaporativeCondenserAirFlowRate(double evaporativeCondenserAirFlowRate) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setEvaporativeCondenserAirFlowRate(
      evaporativeCondenserAirFlowRate);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::autosizeEvaporativeCondenserAirFlowRate() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizeEvaporativeCondenserAirFlowRate();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setEvaporativeCondenserPumpRatedPowerConsumption(
    double evaporativeCondenserPumpRatedPowerConsumption) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setEvaporativeCondenserPumpRatedPowerConsumption(
      evaporativeCondenserPumpRatedPowerConsumption);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::autosizeEvaporativeCondenserPumpRatedPowerConsumption() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizeEvaporativeCondenserPumpRatedPowerConsumption();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setBasinHeaterCapacity(double basinHeaterCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setBasinHeaterCapacity(basinHeaterCapacity);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setBasinHeaterSetpointTemperature(
      basinHeaterSetpointTemperature);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setBasinHeaterOperatingSchedule(Schedule& schedule) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setBasinHeaterOperatingSchedule(schedule);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetBasinHeaterOperatingSchedule() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resetBasinHeaterOperatingSchedule();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setFuelType(std::string fuelType) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setFuelType(fuelType);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setMinimumOutdoorTemperatureinHeatRecoveryMode(
    double minimumOutdoorTemperatureinHeatRecoveryMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setMinimumOutdoorTemperatureinHeatRecoveryMode(
      minimumOutdoorTemperatureinHeatRecoveryMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setMaximumOutdoorTemperatureinHeatRecoveryMode(
    double maximumOutdoorTemperatureinHeatRecoveryMode) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setMaximumOutdoorTemperatureinHeatRecoveryMode(
      maximumOutdoorTemperatureinHeatRecoveryMode);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setHeatRecoveryCoolingCapacityModifierCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setHeatRecoveryCoolingCapacityModifierCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetHeatRecoveryCoolingCapacityModifierCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resetHeatRecoveryCoolingCapacityModifierCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setInitialHeatRecoveryCoolingCapacityFraction(
    double initialHeatRecoveryCoolingCapacityFraction) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setInitialHeatRecoveryCoolingCapacityFraction(
      initialHeatRecoveryCoolingCapacityFraction);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setHeatRecoveryCoolingCapacityTimeConstant(
    double heatRecoveryCoolingCapacityTimeConstant) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setHeatRecoveryCoolingCapacityTimeConstant(
      heatRecoveryCoolingCapacityTimeConstant);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setHeatRecoveryCoolingEnergyModifierCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setHeatRecoveryCoolingEnergyModifierCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetHeatRecoveryCoolingEnergyModifierCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resetHeatRecoveryCoolingEnergyModifierCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setInitialHeatRecoveryCoolingEnergyFraction(
    double initialHeatRecoveryCoolingEnergyFraction) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setInitialHeatRecoveryCoolingEnergyFraction(
      initialHeatRecoveryCoolingEnergyFraction);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setHeatRecoveryCoolingEnergyTimeConstant(
    double heatRecoveryCoolingEnergyTimeConstant) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setHeatRecoveryCoolingEnergyTimeConstant(
      heatRecoveryCoolingEnergyTimeConstant);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setHeatRecoveryHeatingCapacityModifierCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setHeatRecoveryHeatingCapacityModifierCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetHeatRecoveryHeatingCapacityModifierCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resetHeatRecoveryHeatingCapacityModifierCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setInitialHeatRecoveryHeatingCapacityFraction(
    double initialHeatRecoveryHeatingCapacityFraction) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setInitialHeatRecoveryHeatingCapacityFraction(
      initialHeatRecoveryHeatingCapacityFraction);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setHeatRecoveryHeatingCapacityTimeConstant(
    double heatRecoveryHeatingCapacityTimeConstant) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setHeatRecoveryHeatingCapacityTimeConstant(
      heatRecoveryHeatingCapacityTimeConstant);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setHeatRecoveryHeatingEnergyModifierCurve(const Curve& curve) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setHeatRecoveryHeatingEnergyModifierCurve(curve);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetHeatRecoveryHeatingEnergyModifierCurve() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resetHeatRecoveryHeatingEnergyModifierCurve();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setInitialHeatRecoveryHeatingEnergyFraction(
    double initialHeatRecoveryHeatingEnergyFraction) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setInitialHeatRecoveryHeatingEnergyFraction(
      initialHeatRecoveryHeatingEnergyFraction);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setHeatRecoveryHeatingEnergyTimeConstant(
    double heatRecoveryHeatingEnergyTimeConstant) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setHeatRecoveryHeatingEnergyTimeConstant(
      heatRecoveryHeatingEnergyTimeConstant);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::addTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf) {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->addTerminal(vrf);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::removeTerminal(ZoneHVACTerminalUnitVariableRefrigerantFlow& vrf) {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->removeTerminal(vrf);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::removeAllTerminals() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->removeAllTerminals();
  }

  std::vector<ZoneHVACTerminalUnitVariableRefrigerantFlow> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::terminals() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->terminals();
  }

  /// @cond
  AirConditionerVariableRefrigerantFlowFluidTemperatureControl::AirConditionerVariableRefrigerantFlowFluidTemperatureControl(
    std::shared_ptr<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::autosizedResistiveDefrostHeaterCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizedResistiveDefrostHeaterCapacity();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::autosizedWaterCondenserVolumeFlowRate() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizedWaterCondenserVolumeFlowRate();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::autosizedEvaporativeCondenserAirFlowRate() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizedEvaporativeCondenserAirFlowRate();
  }

  boost::optional<double>
    AirConditionerVariableRefrigerantFlowFluidTemperatureControl::autosizedEvaporativeCondenserPumpRatedPowerConsumption() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()
      ->autosizedEvaporativeCondenserPumpRatedPowerConsumption();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setGrossRatedTotalCoolingCapacity(double grossRatedTotalCoolingCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setGrossRatedTotalCoolingCapacity(
      grossRatedTotalCoolingCapacity);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::autosizeGrossRatedTotalCoolingCapacity() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizeGrossRatedTotalCoolingCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setGrossRatedCoolingCOP(double grossRatedCoolingCOP) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setGrossRatedCoolingCOP(grossRatedCoolingCOP);
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::grossRatedTotalCoolingCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->grossRatedTotalCoolingCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::isGrossRatedTotalCoolingCapacityAutosized() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->isGrossRatedTotalCoolingCapacityAutosized();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::grossRatedCoolingCOP() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->grossRatedCoolingCOP();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::grossRatedHeatingCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->grossRatedHeatingCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::isGrossRatedHeatingCapacityAutosized() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->isGrossRatedHeatingCapacityAutosized();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::ratedHeatingCapacitySizingRatio() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->ratedHeatingCapacitySizingRatio();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setGrossRatedHeatingCapacity(double grossRatedHeatingCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setGrossRatedHeatingCapacity(
      grossRatedHeatingCapacity);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::autosizeGrossRatedHeatingCapacity() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizeGrossRatedHeatingCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setRatedHeatingCapacitySizingRatio(double ratedHeatingCapacitySizingRatio) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setRatedHeatingCapacitySizingRatio(
      ratedHeatingCapacitySizingRatio);
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::autosizedGrossRatedTotalCoolingCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizedGrossRatedTotalCoolingCapacity();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::autosizedGrossRatedHeatingCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizedGrossRatedHeatingCapacity();
  }

  std::string AirConditionerVariableRefrigerantFlowFluidTemperatureControl::condenserType() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->condenserType();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setCondenserType(const std::string& condenserType) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setCondenserType(condenserType);
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::isCondenserTypeDefaulted() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->isCondenserTypeDefaulted();
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::resetCondenserType() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->resetCondenserType();
  }

  // DEPRECATED
  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setRatedTotalCoolingCapacity(double grossRatedTotalCoolingCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setGrossRatedTotalCoolingCapacity(
      grossRatedTotalCoolingCapacity);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::autosizeRatedTotalCoolingCapacity() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizeGrossRatedTotalCoolingCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setRatedCoolingCOP(double grossRatedCoolingCOP) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setGrossRatedCoolingCOP(grossRatedCoolingCOP);
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::ratedTotalCoolingCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->grossRatedTotalCoolingCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::isRatedTotalCoolingCapacityAutosized() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->isGrossRatedTotalCoolingCapacityAutosized();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::ratedCoolingCOP() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->grossRatedCoolingCOP();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::ratedTotalHeatingCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->grossRatedHeatingCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::isRatedTotalHeatingCapacityAutosized() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->isGrossRatedHeatingCapacityAutosized();
  }

  double AirConditionerVariableRefrigerantFlowFluidTemperatureControl::ratedTotalHeatingCapacitySizingRatio() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->ratedHeatingCapacitySizingRatio();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setRatedTotalHeatingCapacity(double grossRatedHeatingCapacity) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setGrossRatedHeatingCapacity(
      grossRatedHeatingCapacity);
  }

  void AirConditionerVariableRefrigerantFlowFluidTemperatureControl::autosizeRatedTotalHeatingCapacity() {
    getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizeGrossRatedHeatingCapacity();
  }

  bool AirConditionerVariableRefrigerantFlowFluidTemperatureControl::setRatedTotalHeatingCapacitySizingRatio(double ratedHeatingCapacitySizingRatio) {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->setRatedHeatingCapacitySizingRatio(
      ratedHeatingCapacitySizingRatio);
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::autosizedRatedTotalCoolingCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizedGrossRatedTotalCoolingCapacity();
  }

  boost::optional<double> AirConditionerVariableRefrigerantFlowFluidTemperatureControl::autosizedRatedTotalHeatingCapacity() const {
    return getImpl<detail::AirConditionerVariableRefrigerantFlowFluidTemperatureControl_Impl>()->autosizedGrossRatedHeatingCapacity();
  }

}  // namespace model
}  // namespace openstudio
