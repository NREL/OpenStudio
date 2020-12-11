/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "WaterHeaterMixed.hpp"
#include "WaterHeaterMixed_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "WaterHeaterHeatPump.hpp"
#include "WaterHeaterHeatPump_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleDay.hpp"
#include "ScheduleDay_Impl.hpp"
#include "ScheduleRuleset.hpp"
#include "ScheduleRuleset_Impl.hpp"
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_WaterHeater_Mixed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    WaterHeaterMixed_Impl::WaterHeaterMixed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == WaterHeaterMixed::iddObjectType());
    }

    WaterHeaterMixed_Impl::WaterHeaterMixed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == WaterHeaterMixed::iddObjectType());
    }

    WaterHeaterMixed_Impl::WaterHeaterMixed_Impl(const WaterHeaterMixed_Impl& other, Model_Impl* model, bool keepHandle)
      : WaterToWaterComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& WaterHeaterMixed_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "Water Heater Tank Temperature", "Water Heater Final Tank Temperature", "Water Heater Heat Loss Rate", "Water Heater Heat Loss Energy",
        "Water Heater Use Side Mass Flow Rate", "Water Heater Use Side Inlet Temperature", "Water Heater Use Side Outlet Temperature",
        "Water Heater Use Side Heat Transfer Rate", "Water Heater Use Side Heat Transfer Energy", "Water Heater Source Side Mass Flow Rate",
        "Water Heater Source Side Inlet Temperature", "Water Heater Source Side Outlet Temperature", "Water Heater Source Side Heat Transfer Rate",
        "Water Heater Source Side Heat Transfer Energy", "Water Heater Off Cycle Parasitic Tank Heat Transfer Rate",
        "Water Heater Off Cycle Parasitic Tank Heat Transfer Energy", "Water Heater On Cycle Parasitic Tank Heat Transfer Rate",
        "Water Heater On Cycle Parasitic Tank Heat Transfer Energy", "Water Heater Total Demand Heat Transfer Rate",
        "Water Heater Total Demand Energy", "Water Heater Heating Rate", "Water Heater Heating Energy",
        "Water Heater Unmet Demand Heat Transfer Rate", "Water Heater Unmet Demand Heat Transfer Energy", "Water Heater Venting Heat Transfer Rate",
        "Water Heater Venting Heat Transfer Energy", "Water Heater Net Heat Transfer Rate", "Water Heater Net Heat Transfer Energy",
        "Water Heater Cycle On Count", "Water Heater Runtime Fraction", "Water Heater Part Load Ratio", "Water Heater Electricity Rate",

        // This should be based on fuel type

        //"Water Heater <Fuel Type> Rate",
        //"Water Heater <Fuel Type> Energy",
        // Fuel type specific
        // TODO: DLM: the return type of this method needs to change to std::vector<std::string> in ModelObject
        // until then, make this include all possible outputVariableNames for class regardless of fuelType
        // std::string fuelType = this->fuelType();
        // if (fuelType == "Electricity") {
        "Water Heater Electricity Rate", "Water Heater Electricity Energy",
        // } else if (fuelType == "NaturalGas") {
        "Water Heater NaturalGas Rate", "Water Heater NaturalGas Energy",
        // } else if (fuelType == "Propane") {
        "Water Heater Propane Rate", "Water Heater Propane Energy",
        // } else if (fuelType == "FuelOilNo1") {
        "Water Heater FuelOilNo1 Rate", "Water Heater FuelOilNo1 Energy",
        // } else if (fuelType == "FuelOilNo2") {
        "Water Heater FuelOilNo2 Rate", "Water Heater FuelOilNo2 Energy",
        // } else if (fuelType == "Coal") {
        "Water Heater Coal Rate", "Water Heater Coal Energy",
        // } else if (fuelType == "Diesel") {
        "Water Heater Diesel Rate", "Water Heater Diesel Energy",
        // } else if (fuelType == "Gasoline") {
        "Water Heater Gasoline Rate", "Water Heater Gasoline Energy",
        // } else if (fuelType == "OtherFuel1") {
        "Water Heater OtherFuel1 Rate", "Water Heater OtherFuel1 Energy",
        // } else if (fuelType == "OtherFuel2") {
        "Water Heater OtherFuel2 Rate", "Water Heater OtherFuel2 Energy",
        // }

        // TODO: implement these too...
        //"Water Heater Off Cycle Parasitic <Fuel Type> Rate",
        //"Water Heater Off Cycle Parasitic <Fuel Type> Energy",
        //"Water Heater On Cycle Parasitic <Fuel Type> Rate",
        // "Water Heater On Cycle Parasitic <Fuel Type> Energy",
        //
        "Water Heater Water Volume Flow Rate", "Water Heater Water Volume"

      };
      return result;
    }

    IddObjectType WaterHeaterMixed_Impl::iddObjectType() const {
      return WaterHeaterMixed::iddObjectType();
    }

    std::vector<ScheduleTypeKey> WaterHeaterMixed_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_WaterHeater_MixedFields::SetpointTemperatureScheduleName) != e) {
        result.push_back(ScheduleTypeKey("WaterHeaterMixed", "Setpoint Temperature"));
      }
      if (std::find(b, e, OS_WaterHeater_MixedFields::AmbientTemperatureScheduleName) != e) {
        result.push_back(ScheduleTypeKey("WaterHeaterMixed", "Ambient Temperature"));
      }
      if (std::find(b, e, OS_WaterHeater_MixedFields::UseFlowRateFractionScheduleName) != e) {
        result.push_back(ScheduleTypeKey("WaterHeaterMixed", "Use Flow Rate Fraction"));
      }
      if (std::find(b, e, OS_WaterHeater_MixedFields::ColdWaterSupplyTemperatureScheduleName) != e) {
        result.push_back(ScheduleTypeKey("WaterHeaterMixed", "Cold Water Supply Temperature"));
      }
      if (std::find(b, e, OS_WaterHeater_MixedFields::IndirectAlternateSetpointTemperatureScheduleName) != e) {
        result.push_back(ScheduleTypeKey("WaterHeaterMixed", "Indirect Alternate Setpoint Temperature"));
      }
      return result;
    }

    boost::optional<double> WaterHeaterMixed_Impl::tankVolume() const {
      return getDouble(OS_WaterHeater_MixedFields::TankVolume, true);
    }

    bool WaterHeaterMixed_Impl::isTankVolumeDefaulted() const {
      return isEmpty(OS_WaterHeater_MixedFields::TankVolume);
    }

    bool WaterHeaterMixed_Impl::isTankVolumeAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_WaterHeater_MixedFields::TankVolume, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<Schedule> WaterHeaterMixed_Impl::setpointTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_MixedFields::SetpointTemperatureScheduleName);
    }

    double WaterHeaterMixed_Impl::deadbandTemperatureDifference() const {
      boost::optional<double> value = getDouble(OS_WaterHeater_MixedFields::DeadbandTemperatureDifference, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool WaterHeaterMixed_Impl::isDeadbandTemperatureDifferenceDefaulted() const {
      return isEmpty(OS_WaterHeater_MixedFields::DeadbandTemperatureDifference);
    }

    boost::optional<double> WaterHeaterMixed_Impl::maximumTemperatureLimit() const {
      return getDouble(OS_WaterHeater_MixedFields::MaximumTemperatureLimit, true);
    }

    std::string WaterHeaterMixed_Impl::heaterControlType() const {
      boost::optional<std::string> value = getString(OS_WaterHeater_MixedFields::HeaterControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool WaterHeaterMixed_Impl::isHeaterControlTypeDefaulted() const {
      return isEmpty(OS_WaterHeater_MixedFields::HeaterControlType);
    }

    boost::optional<double> WaterHeaterMixed_Impl::heaterMaximumCapacity() const {
      return getDouble(OS_WaterHeater_MixedFields::HeaterMaximumCapacity, true);
    }

    bool WaterHeaterMixed_Impl::isHeaterMaximumCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_WaterHeater_MixedFields::HeaterMaximumCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> WaterHeaterMixed_Impl::heaterMinimumCapacity() const {
      return getDouble(OS_WaterHeater_MixedFields::HeaterMinimumCapacity, true);
    }

    double WaterHeaterMixed_Impl::heaterIgnitionMinimumFlowRate() const {
      boost::optional<double> value = getDouble(OS_WaterHeater_MixedFields::HeaterIgnitionMinimumFlowRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool WaterHeaterMixed_Impl::isHeaterIgnitionMinimumFlowRateDefaulted() const {
      return isEmpty(OS_WaterHeater_MixedFields::HeaterIgnitionMinimumFlowRate);
    }

    double WaterHeaterMixed_Impl::heaterIgnitionDelay() const {
      boost::optional<double> value = getDouble(OS_WaterHeater_MixedFields::HeaterIgnitionDelay, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool WaterHeaterMixed_Impl::isHeaterIgnitionDelayDefaulted() const {
      return isEmpty(OS_WaterHeater_MixedFields::HeaterIgnitionDelay);
    }

    std::string WaterHeaterMixed_Impl::heaterFuelType() const {
      boost::optional<std::string> value = getString(OS_WaterHeater_MixedFields::HeaterFuelType, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> WaterHeaterMixed_Impl::heaterThermalEfficiency() const {
      return getDouble(OS_WaterHeater_MixedFields::HeaterThermalEfficiency, true);
    }

    boost::optional<CurveCubic> WaterHeaterMixed_Impl::partLoadFactorCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<CurveCubic>(OS_WaterHeater_MixedFields::PartLoadFactorCurveName);
    }

    double WaterHeaterMixed_Impl::offCycleParasiticFuelConsumptionRate() const {
      boost::optional<double> value = getDouble(OS_WaterHeater_MixedFields::OffCycleParasiticFuelConsumptionRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool WaterHeaterMixed_Impl::isOffCycleParasiticFuelConsumptionRateDefaulted() const {
      return isEmpty(OS_WaterHeater_MixedFields::OffCycleParasiticFuelConsumptionRate);
    }

    boost::optional<std::string> WaterHeaterMixed_Impl::offCycleParasiticFuelType() const {
      return getString(OS_WaterHeater_MixedFields::OffCycleParasiticFuelType, true);
    }

    double WaterHeaterMixed_Impl::offCycleParasiticHeatFractiontoTank() const {
      boost::optional<double> value = getDouble(OS_WaterHeater_MixedFields::OffCycleParasiticHeatFractiontoTank, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool WaterHeaterMixed_Impl::isOffCycleParasiticHeatFractiontoTankDefaulted() const {
      return isEmpty(OS_WaterHeater_MixedFields::OffCycleParasiticHeatFractiontoTank);
    }

    double WaterHeaterMixed_Impl::onCycleParasiticFuelConsumptionRate() const {
      boost::optional<double> value = getDouble(OS_WaterHeater_MixedFields::OnCycleParasiticFuelConsumptionRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool WaterHeaterMixed_Impl::isOnCycleParasiticFuelConsumptionRateDefaulted() const {
      return isEmpty(OS_WaterHeater_MixedFields::OnCycleParasiticFuelConsumptionRate);
    }

    boost::optional<std::string> WaterHeaterMixed_Impl::onCycleParasiticFuelType() const {
      return getString(OS_WaterHeater_MixedFields::OnCycleParasiticFuelType, true);
    }

    double WaterHeaterMixed_Impl::onCycleParasiticHeatFractiontoTank() const {
      boost::optional<double> value = getDouble(OS_WaterHeater_MixedFields::OnCycleParasiticHeatFractiontoTank, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool WaterHeaterMixed_Impl::isOnCycleParasiticHeatFractiontoTankDefaulted() const {
      return isEmpty(OS_WaterHeater_MixedFields::OnCycleParasiticHeatFractiontoTank);
    }

    std::string WaterHeaterMixed_Impl::ambientTemperatureIndicator() const {
      boost::optional<std::string> value = getString(OS_WaterHeater_MixedFields::AmbientTemperatureIndicator, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> WaterHeaterMixed_Impl::ambientTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_MixedFields::AmbientTemperatureScheduleName);
    }

    boost::optional<ThermalZone> WaterHeaterMixed_Impl::ambientTemperatureThermalZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_WaterHeater_MixedFields::AmbientTemperatureThermalZoneName);
    }

    boost::optional<std::string> WaterHeaterMixed_Impl::ambientTemperatureOutdoorAirNodeName() const {
      return getString(OS_WaterHeater_MixedFields::AmbientTemperatureOutdoorAirNodeName, true);
    }

    boost::optional<double> WaterHeaterMixed_Impl::offCycleLossCoefficienttoAmbientTemperature() const {
      return getDouble(OS_WaterHeater_MixedFields::OffCycleLossCoefficienttoAmbientTemperature, true);
    }

    double WaterHeaterMixed_Impl::offCycleLossFractiontoThermalZone() const {
      boost::optional<double> value = getDouble(OS_WaterHeater_MixedFields::OffCycleLossFractiontoThermalZone, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool WaterHeaterMixed_Impl::isOffCycleLossFractiontoThermalZoneDefaulted() const {
      return isEmpty(OS_WaterHeater_MixedFields::OffCycleLossFractiontoThermalZone);
    }

    boost::optional<double> WaterHeaterMixed_Impl::onCycleLossCoefficienttoAmbientTemperature() const {
      return getDouble(OS_WaterHeater_MixedFields::OnCycleLossCoefficienttoAmbientTemperature, true);
    }

    double WaterHeaterMixed_Impl::onCycleLossFractiontoThermalZone() const {
      boost::optional<double> value = getDouble(OS_WaterHeater_MixedFields::OnCycleLossFractiontoThermalZone, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool WaterHeaterMixed_Impl::isOnCycleLossFractiontoThermalZoneDefaulted() const {
      return isEmpty(OS_WaterHeater_MixedFields::OnCycleLossFractiontoThermalZone);
    }

    boost::optional<double> WaterHeaterMixed_Impl::peakUseFlowRate() const {
      return getDouble(OS_WaterHeater_MixedFields::PeakUseFlowRate, true);
    }

    boost::optional<Schedule> WaterHeaterMixed_Impl::useFlowRateFractionSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_MixedFields::UseFlowRateFractionScheduleName);
    }

    boost::optional<Schedule> WaterHeaterMixed_Impl::coldWaterSupplyTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_MixedFields::ColdWaterSupplyTemperatureScheduleName);
    }

    double WaterHeaterMixed_Impl::useSideEffectiveness() const {
      boost::optional<double> value = getDouble(OS_WaterHeater_MixedFields::UseSideEffectiveness, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool WaterHeaterMixed_Impl::isUseSideEffectivenessDefaulted() const {
      return isEmpty(OS_WaterHeater_MixedFields::UseSideEffectiveness);
    }

    double WaterHeaterMixed_Impl::sourceSideEffectiveness() const {
      boost::optional<double> value = getDouble(OS_WaterHeater_MixedFields::SourceSideEffectiveness, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool WaterHeaterMixed_Impl::isSourceSideEffectivenessDefaulted() const {
      return isEmpty(OS_WaterHeater_MixedFields::SourceSideEffectiveness);
    }

    boost::optional<double> WaterHeaterMixed_Impl::useSideDesignFlowRate() const {
      return getDouble(OS_WaterHeater_MixedFields::UseSideDesignFlowRate, true);
    }

    bool WaterHeaterMixed_Impl::isUseSideDesignFlowRateDefaulted() const {
      return isEmpty(OS_WaterHeater_MixedFields::UseSideDesignFlowRate);
    }

    bool WaterHeaterMixed_Impl::isUseSideDesignFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_WaterHeater_MixedFields::UseSideDesignFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> WaterHeaterMixed_Impl::sourceSideDesignFlowRate() const {
      return getDouble(OS_WaterHeater_MixedFields::SourceSideDesignFlowRate, true);
    }

    bool WaterHeaterMixed_Impl::isSourceSideDesignFlowRateDefaulted() const {
      return isEmpty(OS_WaterHeater_MixedFields::SourceSideDesignFlowRate);
    }

    bool WaterHeaterMixed_Impl::isSourceSideDesignFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_WaterHeater_MixedFields::SourceSideDesignFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double WaterHeaterMixed_Impl::indirectWaterHeatingRecoveryTime() const {
      boost::optional<double> value = getDouble(OS_WaterHeater_MixedFields::IndirectWaterHeatingRecoveryTime, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool WaterHeaterMixed_Impl::isIndirectWaterHeatingRecoveryTimeDefaulted() const {
      return isEmpty(OS_WaterHeater_MixedFields::IndirectWaterHeatingRecoveryTime);
    }

    bool WaterHeaterMixed_Impl::setTankVolume(boost::optional<double> tankVolume) {
      bool result = false;
      if (tankVolume) {
        result = setDouble(OS_WaterHeater_MixedFields::TankVolume, tankVolume.get());
      } else {
        result = setString(OS_WaterHeater_MixedFields::TankVolume, "");
      }
      return result;
    }

    void WaterHeaterMixed_Impl::resetTankVolume() {
      bool result = setString(OS_WaterHeater_MixedFields::TankVolume, "");
      OS_ASSERT(result);
    }

    void WaterHeaterMixed_Impl::autosizeTankVolume() {
      bool result = setString(OS_WaterHeater_MixedFields::TankVolume, "autosize");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setSetpointTemperatureSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_WaterHeater_MixedFields::SetpointTemperatureScheduleName, "WaterHeaterMixed", "Setpoint Temperature", schedule);
      return result;
    }

    void WaterHeaterMixed_Impl::resetSetpointTemperatureSchedule() {
      bool result = setString(OS_WaterHeater_MixedFields::SetpointTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setDeadbandTemperatureDifference(double deadbandTemperatureDifference) {
      bool result = setDouble(OS_WaterHeater_MixedFields::DeadbandTemperatureDifference, deadbandTemperatureDifference);
      return result;
    }

    void WaterHeaterMixed_Impl::resetDeadbandTemperatureDifference() {
      bool result = setString(OS_WaterHeater_MixedFields::DeadbandTemperatureDifference, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setMaximumTemperatureLimit(boost::optional<double> maximumTemperatureLimit) {
      bool result = false;
      if (maximumTemperatureLimit) {
        result = setDouble(OS_WaterHeater_MixedFields::MaximumTemperatureLimit, maximumTemperatureLimit.get());
      } else {
        result = setString(OS_WaterHeater_MixedFields::MaximumTemperatureLimit, "");
      }
      OS_ASSERT(result);
      return result;
    }

    void WaterHeaterMixed_Impl::resetMaximumTemperatureLimit() {
      bool result = setString(OS_WaterHeater_MixedFields::MaximumTemperatureLimit, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setHeaterControlType(std::string heaterControlType) {
      bool result = setString(OS_WaterHeater_MixedFields::HeaterControlType, heaterControlType);
      return result;
    }

    void WaterHeaterMixed_Impl::resetHeaterControlType() {
      bool result = setString(OS_WaterHeater_MixedFields::HeaterControlType, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setHeaterMaximumCapacity(boost::optional<double> heaterMaximumCapacity) {
      bool result = false;
      if (heaterMaximumCapacity) {
        result = setDouble(OS_WaterHeater_MixedFields::HeaterMaximumCapacity, heaterMaximumCapacity.get());
      } else {
        result = setString(OS_WaterHeater_MixedFields::HeaterMaximumCapacity, "");
      }
      return result;
    }

    void WaterHeaterMixed_Impl::resetHeaterMaximumCapacity() {
      bool result = setString(OS_WaterHeater_MixedFields::HeaterMaximumCapacity, "");
      OS_ASSERT(result);
    }

    void WaterHeaterMixed_Impl::autosizeHeaterMaximumCapacity() {
      bool result = setString(OS_WaterHeater_MixedFields::HeaterMaximumCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setHeaterMinimumCapacity(boost::optional<double> heaterMinimumCapacity) {
      bool result = false;
      if (heaterMinimumCapacity) {
        result = setDouble(OS_WaterHeater_MixedFields::HeaterMinimumCapacity, heaterMinimumCapacity.get());
      } else {
        result = setString(OS_WaterHeater_MixedFields::HeaterMinimumCapacity, "");
      }
      return result;
    }

    void WaterHeaterMixed_Impl::resetHeaterMinimumCapacity() {
      bool result = setString(OS_WaterHeater_MixedFields::HeaterMinimumCapacity, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setHeaterIgnitionMinimumFlowRate(double heaterIgnitionMinimumFlowRate) {
      bool result = setDouble(OS_WaterHeater_MixedFields::HeaterIgnitionMinimumFlowRate, heaterIgnitionMinimumFlowRate);
      return result;
    }

    void WaterHeaterMixed_Impl::resetHeaterIgnitionMinimumFlowRate() {
      bool result = setString(OS_WaterHeater_MixedFields::HeaterIgnitionMinimumFlowRate, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setHeaterIgnitionDelay(double heaterIgnitionDelay) {
      bool result = setDouble(OS_WaterHeater_MixedFields::HeaterIgnitionDelay, heaterIgnitionDelay);
      return result;
    }

    void WaterHeaterMixed_Impl::resetHeaterIgnitionDelay() {
      bool result = setString(OS_WaterHeater_MixedFields::HeaterIgnitionDelay, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setHeaterFuelType(std::string heaterFuelType) {
      bool result = setString(OS_WaterHeater_MixedFields::HeaterFuelType, heaterFuelType);
      return result;
    }

    bool WaterHeaterMixed_Impl::setHeaterThermalEfficiency(boost::optional<double> heaterThermalEfficiency) {
      bool result = false;
      if (heaterThermalEfficiency) {
        result = setDouble(OS_WaterHeater_MixedFields::HeaterThermalEfficiency, heaterThermalEfficiency.get());
      } else {
        result = setString(OS_WaterHeater_MixedFields::HeaterThermalEfficiency, "");
      }
      return result;
    }

    void WaterHeaterMixed_Impl::resetHeaterThermalEfficiency() {
      bool result = setString(OS_WaterHeater_MixedFields::HeaterThermalEfficiency, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setPartLoadFactorCurve(const boost::optional<CurveCubic>& partLoadFactorCurve) {
      bool result = false;
      if (partLoadFactorCurve) {
        result = setPointer(OS_WaterHeater_MixedFields::PartLoadFactorCurveName, partLoadFactorCurve.get().handle());
      } else {
        result = setString(OS_WaterHeater_MixedFields::PartLoadFactorCurveName, "");
      }
      return result;
    }

    void WaterHeaterMixed_Impl::resetPartLoadFactorCurve() {
      bool result = setString(OS_WaterHeater_MixedFields::PartLoadFactorCurveName, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setOffCycleParasiticFuelConsumptionRate(double offCycleParasiticFuelConsumptionRate) {
      bool result = setDouble(OS_WaterHeater_MixedFields::OffCycleParasiticFuelConsumptionRate, offCycleParasiticFuelConsumptionRate);
      return result;
    }

    void WaterHeaterMixed_Impl::resetOffCycleParasiticFuelConsumptionRate() {
      bool result = setString(OS_WaterHeater_MixedFields::OffCycleParasiticFuelConsumptionRate, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setOffCycleParasiticFuelType(boost::optional<std::string> offCycleParasiticFuelType) {
      bool result = false;
      if (offCycleParasiticFuelType) {
        result = setString(OS_WaterHeater_MixedFields::OffCycleParasiticFuelType, offCycleParasiticFuelType.get());
      } else {
        result = setString(OS_WaterHeater_MixedFields::OffCycleParasiticFuelType, "");
      }
      return result;
    }

    void WaterHeaterMixed_Impl::resetOffCycleParasiticFuelType() {
      bool result = setString(OS_WaterHeater_MixedFields::OffCycleParasiticFuelType, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setOffCycleParasiticHeatFractiontoTank(double offCycleParasiticHeatFractiontoTank) {
      bool result = setDouble(OS_WaterHeater_MixedFields::OffCycleParasiticHeatFractiontoTank, offCycleParasiticHeatFractiontoTank);
      return result;
    }

    void WaterHeaterMixed_Impl::resetOffCycleParasiticHeatFractiontoTank() {
      bool result = setString(OS_WaterHeater_MixedFields::OffCycleParasiticHeatFractiontoTank, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setOnCycleParasiticFuelConsumptionRate(double onCycleParasiticFuelConsumptionRate) {
      bool result = setDouble(OS_WaterHeater_MixedFields::OnCycleParasiticFuelConsumptionRate, onCycleParasiticFuelConsumptionRate);
      return result;
    }

    void WaterHeaterMixed_Impl::resetOnCycleParasiticFuelConsumptionRate() {
      bool result = setString(OS_WaterHeater_MixedFields::OnCycleParasiticFuelConsumptionRate, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setOnCycleParasiticFuelType(boost::optional<std::string> onCycleParasiticFuelType) {
      bool result = false;
      if (onCycleParasiticFuelType) {
        result = setString(OS_WaterHeater_MixedFields::OnCycleParasiticFuelType, onCycleParasiticFuelType.get());
      } else {
        result = setString(OS_WaterHeater_MixedFields::OnCycleParasiticFuelType, "");
      }
      return result;
    }

    void WaterHeaterMixed_Impl::resetOnCycleParasiticFuelType() {
      bool result = setString(OS_WaterHeater_MixedFields::OnCycleParasiticFuelType, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setOnCycleParasiticHeatFractiontoTank(double onCycleParasiticHeatFractiontoTank) {
      bool result = setDouble(OS_WaterHeater_MixedFields::OnCycleParasiticHeatFractiontoTank, onCycleParasiticHeatFractiontoTank);
      return result;
    }

    void WaterHeaterMixed_Impl::resetOnCycleParasiticHeatFractiontoTank() {
      bool result = setString(OS_WaterHeater_MixedFields::OnCycleParasiticHeatFractiontoTank, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setAmbientTemperatureIndicator(std::string ambientTemperatureIndicator) {
      bool result = setString(OS_WaterHeater_MixedFields::AmbientTemperatureIndicator, ambientTemperatureIndicator);
      return result;
    }

    bool WaterHeaterMixed_Impl::setAmbientTemperatureSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_WaterHeater_MixedFields::AmbientTemperatureScheduleName, "WaterHeaterMixed", "Ambient Temperature", schedule);
      return result;
    }

    void WaterHeaterMixed_Impl::resetAmbientTemperatureSchedule() {
      bool result = setString(OS_WaterHeater_MixedFields::AmbientTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setAmbientTemperatureThermalZone(const boost::optional<ThermalZone>& ambientTemperatureThermalZone) {
      bool result = false;
      if (ambientTemperatureThermalZone) {
        result = setPointer(OS_WaterHeater_MixedFields::AmbientTemperatureThermalZoneName, ambientTemperatureThermalZone.get().handle());
      } else {
        result = setString(OS_WaterHeater_MixedFields::AmbientTemperatureThermalZoneName, "");
      }
      return result;
    }

    void WaterHeaterMixed_Impl::resetAmbientTemperatureThermalZone() {
      bool result = setString(OS_WaterHeater_MixedFields::AmbientTemperatureThermalZoneName, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setAmbientTemperatureOutdoorAirNodeName(boost::optional<std::string> ambientTemperatureOutdoorAirNodeName) {
      bool result = false;
      if (ambientTemperatureOutdoorAirNodeName) {
        result = setString(OS_WaterHeater_MixedFields::AmbientTemperatureOutdoorAirNodeName, ambientTemperatureOutdoorAirNodeName.get());
      } else {
        result = setString(OS_WaterHeater_MixedFields::AmbientTemperatureOutdoorAirNodeName, "");
      }
      OS_ASSERT(result);
      return result;
    }

    void WaterHeaterMixed_Impl::resetAmbientTemperatureOutdoorAirNodeName() {
      bool result = setString(OS_WaterHeater_MixedFields::AmbientTemperatureOutdoorAirNodeName, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setOffCycleLossCoefficienttoAmbientTemperature(boost::optional<double> offCycleLossCoefficienttoAmbientTemperature) {
      bool result = false;
      if (offCycleLossCoefficienttoAmbientTemperature) {
        result =
          setDouble(OS_WaterHeater_MixedFields::OffCycleLossCoefficienttoAmbientTemperature, offCycleLossCoefficienttoAmbientTemperature.get());
      } else {
        result = setString(OS_WaterHeater_MixedFields::OffCycleLossCoefficienttoAmbientTemperature, "");
      }
      return result;
    }

    void WaterHeaterMixed_Impl::resetOffCycleLossCoefficienttoAmbientTemperature() {
      bool result = setString(OS_WaterHeater_MixedFields::OffCycleLossCoefficienttoAmbientTemperature, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setOffCycleLossFractiontoThermalZone(double offCycleLossFractiontoThermalZone) {
      bool result = setDouble(OS_WaterHeater_MixedFields::OffCycleLossFractiontoThermalZone, offCycleLossFractiontoThermalZone);
      return result;
    }

    void WaterHeaterMixed_Impl::resetOffCycleLossFractiontoThermalZone() {
      bool result = setString(OS_WaterHeater_MixedFields::OffCycleLossFractiontoThermalZone, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setOnCycleLossCoefficienttoAmbientTemperature(boost::optional<double> onCycleLossCoefficienttoAmbientTemperature) {
      bool result = false;
      if (onCycleLossCoefficienttoAmbientTemperature) {
        result = setDouble(OS_WaterHeater_MixedFields::OnCycleLossCoefficienttoAmbientTemperature, onCycleLossCoefficienttoAmbientTemperature.get());
      } else {
        result = setString(OS_WaterHeater_MixedFields::OnCycleLossCoefficienttoAmbientTemperature, "");
      }
      return result;
    }

    void WaterHeaterMixed_Impl::resetOnCycleLossCoefficienttoAmbientTemperature() {
      bool result = setString(OS_WaterHeater_MixedFields::OnCycleLossCoefficienttoAmbientTemperature, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setOnCycleLossFractiontoThermalZone(double onCycleLossFractiontoThermalZone) {
      bool result = setDouble(OS_WaterHeater_MixedFields::OnCycleLossFractiontoThermalZone, onCycleLossFractiontoThermalZone);
      return result;
    }

    void WaterHeaterMixed_Impl::resetOnCycleLossFractiontoThermalZone() {
      bool result = setString(OS_WaterHeater_MixedFields::OnCycleLossFractiontoThermalZone, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setPeakUseFlowRate(boost::optional<double> peakUseFlowRate) {
      bool result = false;
      if (peakUseFlowRate) {
        result = setDouble(OS_WaterHeater_MixedFields::PeakUseFlowRate, peakUseFlowRate.get());
      } else {
        result = setString(OS_WaterHeater_MixedFields::PeakUseFlowRate, "");
      }
      return result;
    }

    void WaterHeaterMixed_Impl::resetPeakUseFlowRate() {
      bool result = setString(OS_WaterHeater_MixedFields::PeakUseFlowRate, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setUseFlowRateFractionSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_WaterHeater_MixedFields::UseFlowRateFractionScheduleName, "WaterHeaterMixed", "Use Flow Rate Fraction", schedule);
      return result;
    }

    void WaterHeaterMixed_Impl::resetUseFlowRateFractionSchedule() {
      bool result = setString(OS_WaterHeater_MixedFields::UseFlowRateFractionScheduleName, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setColdWaterSupplyTemperatureSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_WaterHeater_MixedFields::ColdWaterSupplyTemperatureScheduleName, "WaterHeaterMixed",
                                "Cold Water Supply Temperature", schedule);
      return result;
    }

    void WaterHeaterMixed_Impl::resetColdWaterSupplyTemperatureSchedule() {
      bool result = setString(OS_WaterHeater_MixedFields::ColdWaterSupplyTemperatureScheduleName, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setUseSideEffectiveness(double useSideEffectiveness) {
      bool result = setDouble(OS_WaterHeater_MixedFields::UseSideEffectiveness, useSideEffectiveness);
      return result;
    }

    void WaterHeaterMixed_Impl::resetUseSideEffectiveness() {
      bool result = setString(OS_WaterHeater_MixedFields::UseSideEffectiveness, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setSourceSideEffectiveness(double sourceSideEffectiveness) {
      bool result = setDouble(OS_WaterHeater_MixedFields::SourceSideEffectiveness, sourceSideEffectiveness);
      return result;
    }

    void WaterHeaterMixed_Impl::resetSourceSideEffectiveness() {
      bool result = setString(OS_WaterHeater_MixedFields::SourceSideEffectiveness, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setUseSideDesignFlowRate(boost::optional<double> useSideDesignFlowRate) {
      bool result = false;
      if (useSideDesignFlowRate) {
        result = setDouble(OS_WaterHeater_MixedFields::UseSideDesignFlowRate, useSideDesignFlowRate.get());
      } else {
        result = setString(OS_WaterHeater_MixedFields::UseSideDesignFlowRate, "");
      }
      return result;
    }

    void WaterHeaterMixed_Impl::resetUseSideDesignFlowRate() {
      bool result = setString(OS_WaterHeater_MixedFields::UseSideDesignFlowRate, "");
      OS_ASSERT(result);
    }

    void WaterHeaterMixed_Impl::autosizeUseSideDesignFlowRate() {
      bool result = setString(OS_WaterHeater_MixedFields::UseSideDesignFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setSourceSideDesignFlowRate(boost::optional<double> sourceSideDesignFlowRate) {
      bool result = false;
      if (sourceSideDesignFlowRate) {
        result = setDouble(OS_WaterHeater_MixedFields::SourceSideDesignFlowRate, sourceSideDesignFlowRate.get());
      } else {
        result = setString(OS_WaterHeater_MixedFields::SourceSideDesignFlowRate, "");
      }
      return result;
    }

    void WaterHeaterMixed_Impl::resetSourceSideDesignFlowRate() {
      bool result = setString(OS_WaterHeater_MixedFields::SourceSideDesignFlowRate, "");
      OS_ASSERT(result);
    }

    void WaterHeaterMixed_Impl::autosizeSourceSideDesignFlowRate() {
      bool result = setString(OS_WaterHeater_MixedFields::SourceSideDesignFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool WaterHeaterMixed_Impl::setIndirectWaterHeatingRecoveryTime(double indirectWaterHeatingRecoveryTime) {
      bool result = setDouble(OS_WaterHeater_MixedFields::IndirectWaterHeatingRecoveryTime, indirectWaterHeatingRecoveryTime);
      return result;
    }

    void WaterHeaterMixed_Impl::resetIndirectWaterHeatingRecoveryTime() {
      bool result = setString(OS_WaterHeater_MixedFields::IndirectWaterHeatingRecoveryTime, "");
      OS_ASSERT(result);
    }

    std::vector<std::string> WaterHeaterMixed_Impl::heaterControlTypeValues() const {
      return WaterHeaterMixed::heaterControlTypeValues();
    }

    std::vector<std::string> WaterHeaterMixed_Impl::heaterFuelTypeValues() const {
      return WaterHeaterMixed::heaterFuelTypeValues();
    }

    std::vector<std::string> WaterHeaterMixed_Impl::offCycleParasiticFuelTypeValues() const {
      return WaterHeaterMixed::offCycleParasiticFuelTypeValues();
    }

    std::vector<std::string> WaterHeaterMixed_Impl::onCycleParasiticFuelTypeValues() const {
      return WaterHeaterMixed::onCycleParasiticFuelTypeValues();
    }

    std::vector<std::string> WaterHeaterMixed_Impl::ambientTemperatureIndicatorValues() const {
      return WaterHeaterMixed::ambientTemperatureIndicatorValues();
    }

    boost::optional<ModelObject> WaterHeaterMixed_Impl::setpointTemperatureScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = setpointTemperatureSchedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> WaterHeaterMixed_Impl::partLoadFactorCurveAsModelObject() const {
      OptionalModelObject result;
      OptionalCurveCubic intermediate = partLoadFactorCurve();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> WaterHeaterMixed_Impl::ambientTemperatureScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = ambientTemperatureSchedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> WaterHeaterMixed_Impl::ambientTemperatureThermalZoneAsModelObject() const {
      OptionalModelObject result;
      OptionalThermalZone intermediate = ambientTemperatureThermalZone();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> WaterHeaterMixed_Impl::useFlowRateFractionScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = useFlowRateFractionSchedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    boost::optional<ModelObject> WaterHeaterMixed_Impl::coldWaterSupplyTemperatureScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = coldWaterSupplyTemperatureSchedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool WaterHeaterMixed_Impl::setSetpointTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setSetpointTemperatureSchedule(schedule);
        } else {
          return false;
        }
      } else {
        resetSetpointTemperatureSchedule();
      }
      return true;
    }

    bool WaterHeaterMixed_Impl::setPartLoadFactorCurveAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalCurveCubic intermediate = modelObject->optionalCast<CurveCubic>();
        if (intermediate) {
          return setPartLoadFactorCurve(*intermediate);
        } else {
          return false;
        }
      } else {
        resetPartLoadFactorCurve();
      }
      return true;
    }

    bool WaterHeaterMixed_Impl::setAmbientTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setAmbientTemperatureSchedule(schedule);
        } else {
          return false;
        }
      } else {
        resetAmbientTemperatureSchedule();
      }
      return true;
    }

    bool WaterHeaterMixed_Impl::setAmbientTemperatureThermalZoneAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalThermalZone intermediate = modelObject->optionalCast<ThermalZone>();
        if (intermediate) {
          return setAmbientTemperatureThermalZone(*intermediate);
        } else {
          return false;
        }
      } else {
        resetAmbientTemperatureThermalZone();
      }
      return true;
    }

    bool WaterHeaterMixed_Impl::setUseFlowRateFractionScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setUseFlowRateFractionSchedule(schedule);
        } else {
          return false;
        }
      } else {
        resetUseFlowRateFractionSchedule();
      }
      return true;
    }

    bool WaterHeaterMixed_Impl::setColdWaterSupplyTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          return setColdWaterSupplyTemperatureSchedule(*intermediate);
        } else {
          return false;
        }
      } else {
        resetColdWaterSupplyTemperatureSchedule();
      }
      return true;
    }

    unsigned WaterHeaterMixed_Impl::supplyInletPort() const {
      return OS_WaterHeater_MixedFields::UseSideInletNodeName;
    }

    unsigned WaterHeaterMixed_Impl::supplyOutletPort() const {
      return OS_WaterHeater_MixedFields::UseSideOutletNodeName;
    }

    unsigned WaterHeaterMixed_Impl::demandInletPort() const {
      return OS_WaterHeater_MixedFields::SourceSideInletNodeName;
    }

    unsigned WaterHeaterMixed_Impl::demandOutletPort() const {
      return OS_WaterHeater_MixedFields::SourceSideOutletNodeName;
    }

    boost::optional<ZoneHVACComponent> WaterHeaterMixed_Impl::containingZoneHVACComponent() const {
      auto hpwhs = model().getConcreteModelObjects<model::WaterHeaterHeatPump>();
      auto t_Handle = handle();

      for (const auto& hpwh : hpwhs) {
        if (hpwh.tank().handle() == t_Handle) {
          return hpwh;
        }
      }

      return boost::none;
    }

    boost::optional<double> WaterHeaterMixed_Impl::autosizedTankVolume() const {
      return getAutosizedValue("Design Size Tank Volume", "m3");
    }

    boost::optional<double> WaterHeaterMixed_Impl::autosizedHeaterMaximumCapacity() const {
      return getAutosizedValue("Design Size Heater Maximum Capacity", "W");
    }

    boost::optional<double> WaterHeaterMixed_Impl::autosizedUseSideDesignFlowRate() const {
      return getAutosizedValue("Design Size Use Side Design Flow Rate", "m3/s");
    }

    boost::optional<double> WaterHeaterMixed_Impl::autosizedSourceSideDesignFlowRate() const {
      return getAutosizedValue("Design Size Source Side Design Flow Rate", "m3/s");
    }

    void WaterHeaterMixed_Impl::autosize() {
      autosizeTankVolume();
      autosizeHeaterMaximumCapacity();
      autosizeUseSideDesignFlowRate();
      autosizeSourceSideDesignFlowRate();
    }

    void WaterHeaterMixed_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedTankVolume();
      if (val) {
        setTankVolume(val.get());
      }

      val = autosizedHeaterMaximumCapacity();
      if (val) {
        setHeaterMaximumCapacity(val.get());
      }

      val = autosizedUseSideDesignFlowRate();
      if (val) {
        setUseSideDesignFlowRate(val.get());
      }

      val = autosizedSourceSideDesignFlowRate();
      if (val) {
        setSourceSideDesignFlowRate(val.get());
      }
    }

    std::string WaterHeaterMixed_Impl::endUseSubcategory() const {
      auto value = getString(OS_WaterHeater_MixedFields::EndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool WaterHeaterMixed_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
      return setString(OS_WaterHeater_MixedFields::EndUseSubcategory, endUseSubcategory);
    }

    std::vector<std::string> WaterHeaterMixed_Impl::sourceSideFlowControlModeValues() const {
      return WaterHeaterMixed::sourceSideFlowControlModeValues();
    }

    std::string WaterHeaterMixed_Impl::sourceSideFlowControlMode() const {
      boost::optional<std::string> value = getString(OS_WaterHeater_MixedFields::SourceSideFlowControlMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool WaterHeaterMixed_Impl::setSourceSideFlowControlMode(const std::string& sourceSideFlowControlMode) {

      bool result = false;

      // Do not accept IndirectHeatAlternateSetpoint unless there is already a schedule that is set
      if (openstudio::istringEqual("IndirectHeatAlternateSetpoint", sourceSideFlowControlMode)) {
        if (!indirectAlternateSetpointTemperatureSchedule()) {
          LOG(Warn, "If you want to use a Source Side Flow Control Mode of 'IndirectHeatAlternateSetpoint', "
                    "use setIndirectAlternateSetpointTemperatureSchedule(schedule) instead for "
                      << briefDescription());
          return false;
        }
      }
      // If other than IndirectHeatAlternateSetpoint, Reset the indirect alternate setpoint temp schedule
      else {
        // Have to do this before resetting the schedule, in case a bad (per IDD) value other than 'IndirectHeatAlternateSetpoint' is provided
        result = setString(OS_WaterHeater_MixedFields::SourceSideFlowControlMode, sourceSideFlowControlMode);

        if (result && indirectAlternateSetpointTemperatureSchedule()) {
          LOG(Info, "Resetting the 'Indirect Alternate Setpoint Temperature Schedule Name' for " << briefDescription());
          setString(OS_WaterHeater_MixedFields::IndirectAlternateSetpointTemperatureScheduleName, "");
        }
      }

      return result;
    }

    boost::optional<Schedule> WaterHeaterMixed_Impl::indirectAlternateSetpointTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_MixedFields::IndirectAlternateSetpointTemperatureScheduleName);
    }

    bool WaterHeaterMixed_Impl::setIndirectAlternateSetpointTemperatureSchedule(Schedule& indirectAlternateSetpointTemperatureSchedule) {
      bool result = setSchedule(OS_WaterHeater_MixedFields::IndirectAlternateSetpointTemperatureScheduleName, "WaterHeaterMixed",
                                "Indirect Alternate Setpoint Temperature", indirectAlternateSetpointTemperatureSchedule);
      // Also set the source Side Flow Control Mode accordingly
      if (result && !openstudio::istringEqual("IndirectHeatAlternateSetpoint", sourceSideFlowControlMode())) {
        LOG(Info, "Setting the Source Side Flow Control Mode to 'IndirectHeatAlternateSetpoint' for " << briefDescription());
        result = setString(OS_WaterHeater_MixedFields::SourceSideFlowControlMode, "IndirectHeatAlternateSetpoint");
      }
      return result;
    }

    void WaterHeaterMixed_Impl::resetIndirectAlternateSetpointTemperatureSchedule() {
      bool result = setString(OS_WaterHeater_MixedFields::IndirectAlternateSetpointTemperatureScheduleName, "");
      OS_ASSERT(result);
      // Reset the Source Side Flow Control Mode to the default "IndirectHeatPrimarySetpoint"
      if (openstudio::istringEqual("IndirectHeatAlternateSetpoint", sourceSideFlowControlMode())) {
        LOG(Info, "Resetting the Source Side Flow Control Mode to the default 'IndirectHeatPrimarySetpoint' for " << briefDescription());
        result = setString(OS_WaterHeater_MixedFields::SourceSideFlowControlMode, "IndirectHeatPrimarySetpoint");
      }
      OS_ASSERT(result);
    }

    boost::optional<ModelObject> WaterHeaterMixed_Impl::indirectAlternateSetpointTemperatureScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = indirectAlternateSetpointTemperatureSchedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool WaterHeaterMixed_Impl::setIndirectAlternateSetpointTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setIndirectAlternateSetpointTemperatureSchedule(schedule);
        } else {
          return false;
        }
      } else {
        resetIndirectAlternateSetpointTemperatureSchedule();
      }
      return true;
    }

  }  // namespace detail

  WaterHeaterMixed::WaterHeaterMixed(const Model& model) : WaterToWaterComponent(WaterHeaterMixed::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::WaterHeaterMixed_Impl>());

    setTankVolume(0.3785);
    setDeadbandTemperatureDifference(2.0);
    setMaximumTemperatureLimit(82.22);
    setHeaterControlType("Cycle");
    setHeaterMaximumCapacity(845000);
    setHeaterFuelType("NaturalGas");
    setHeaterThermalEfficiency(0.8);
    setOffCycleParasiticFuelConsumptionRate(20.0);
    setOffCycleParasiticFuelType("NaturalGas");
    setOffCycleParasiticHeatFractiontoTank(0.8);
    setOnCycleParasiticFuelType("NaturalGas");
    setOffCycleLossCoefficienttoAmbientTemperature(6.0);
    setOnCycleLossCoefficienttoAmbientTemperature(6.0);
    setUseSideEffectiveness(1.0);
    setSourceSideEffectiveness(1.0);
    autosizeUseSideDesignFlowRate();
    autosizeSourceSideDesignFlowRate();
    setIndirectWaterHeatingRecoveryTime(1.5);

    setAmbientTemperatureIndicator("Schedule");
    ScheduleRuleset amb_schedule(model);
    amb_schedule.defaultDaySchedule().addValue(Time(0, 24, 0, 0), 22.0);
    setAmbientTemperatureSchedule(amb_schedule);

    ScheduleRuleset setpoint_schedule(model);
    setpoint_schedule.defaultDaySchedule().addValue(Time(0, 24, 0, 0), 60.0);
    setSetpointTemperatureSchedule(setpoint_schedule);

    setSourceSideFlowControlMode("IndirectHeatPrimarySetpoint");
    setEndUseSubcategory("General");
  }

  IddObjectType WaterHeaterMixed::iddObjectType() {
    return IddObjectType(IddObjectType::OS_WaterHeater_Mixed);
  }

  std::vector<std::string> WaterHeaterMixed::heaterControlTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_WaterHeater_MixedFields::HeaterControlType);
  }

  std::vector<std::string> WaterHeaterMixed::heaterFuelTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_WaterHeater_MixedFields::HeaterFuelType);
  }

  std::vector<std::string> WaterHeaterMixed::offCycleParasiticFuelTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_WaterHeater_MixedFields::OffCycleParasiticFuelType);
  }

  std::vector<std::string> WaterHeaterMixed::onCycleParasiticFuelTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_WaterHeater_MixedFields::OnCycleParasiticFuelType);
  }

  std::vector<std::string> WaterHeaterMixed::ambientTemperatureIndicatorValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_WaterHeater_MixedFields::AmbientTemperatureIndicator);
  }

  boost::optional<double> WaterHeaterMixed::tankVolume() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->tankVolume();
  }

  bool WaterHeaterMixed::isTankVolumeDefaulted() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->isTankVolumeDefaulted();
  }

  bool WaterHeaterMixed::isTankVolumeAutosized() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->isTankVolumeAutosized();
  }

  boost::optional<Schedule> WaterHeaterMixed::setpointTemperatureSchedule() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setpointTemperatureSchedule();
  }

  double WaterHeaterMixed::deadbandTemperatureDifference() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->deadbandTemperatureDifference();
  }

  bool WaterHeaterMixed::isDeadbandTemperatureDifferenceDefaulted() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->isDeadbandTemperatureDifferenceDefaulted();
  }

  boost::optional<double> WaterHeaterMixed::maximumTemperatureLimit() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->maximumTemperatureLimit();
  }

  std::string WaterHeaterMixed::heaterControlType() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->heaterControlType();
  }

  bool WaterHeaterMixed::isHeaterControlTypeDefaulted() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->isHeaterControlTypeDefaulted();
  }

  boost::optional<double> WaterHeaterMixed::heaterMaximumCapacity() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->heaterMaximumCapacity();
  }

  bool WaterHeaterMixed::isHeaterMaximumCapacityAutosized() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->isHeaterMaximumCapacityAutosized();
  }

  boost::optional<double> WaterHeaterMixed::heaterMinimumCapacity() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->heaterMinimumCapacity();
  }

  double WaterHeaterMixed::heaterIgnitionMinimumFlowRate() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->heaterIgnitionMinimumFlowRate();
  }

  bool WaterHeaterMixed::isHeaterIgnitionMinimumFlowRateDefaulted() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->isHeaterIgnitionMinimumFlowRateDefaulted();
  }

  double WaterHeaterMixed::heaterIgnitionDelay() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->heaterIgnitionDelay();
  }

  bool WaterHeaterMixed::isHeaterIgnitionDelayDefaulted() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->isHeaterIgnitionDelayDefaulted();
  }

  std::string WaterHeaterMixed::heaterFuelType() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->heaterFuelType();
  }

  boost::optional<double> WaterHeaterMixed::heaterThermalEfficiency() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->heaterThermalEfficiency();
  }

  boost::optional<CurveCubic> WaterHeaterMixed::partLoadFactorCurve() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->partLoadFactorCurve();
  }

  double WaterHeaterMixed::offCycleParasiticFuelConsumptionRate() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->offCycleParasiticFuelConsumptionRate();
  }

  bool WaterHeaterMixed::isOffCycleParasiticFuelConsumptionRateDefaulted() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->isOffCycleParasiticFuelConsumptionRateDefaulted();
  }

  boost::optional<std::string> WaterHeaterMixed::offCycleParasiticFuelType() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->offCycleParasiticFuelType();
  }

  double WaterHeaterMixed::offCycleParasiticHeatFractiontoTank() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->offCycleParasiticHeatFractiontoTank();
  }

  bool WaterHeaterMixed::isOffCycleParasiticHeatFractiontoTankDefaulted() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->isOffCycleParasiticHeatFractiontoTankDefaulted();
  }

  double WaterHeaterMixed::onCycleParasiticFuelConsumptionRate() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->onCycleParasiticFuelConsumptionRate();
  }

  bool WaterHeaterMixed::isOnCycleParasiticFuelConsumptionRateDefaulted() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->isOnCycleParasiticFuelConsumptionRateDefaulted();
  }

  boost::optional<std::string> WaterHeaterMixed::onCycleParasiticFuelType() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->onCycleParasiticFuelType();
  }

  double WaterHeaterMixed::onCycleParasiticHeatFractiontoTank() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->onCycleParasiticHeatFractiontoTank();
  }

  bool WaterHeaterMixed::isOnCycleParasiticHeatFractiontoTankDefaulted() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->isOnCycleParasiticHeatFractiontoTankDefaulted();
  }

  std::string WaterHeaterMixed::ambientTemperatureIndicator() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->ambientTemperatureIndicator();
  }

  boost::optional<Schedule> WaterHeaterMixed::ambientTemperatureSchedule() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->ambientTemperatureSchedule();
  }

  boost::optional<ThermalZone> WaterHeaterMixed::ambientTemperatureThermalZone() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->ambientTemperatureThermalZone();
  }

  boost::optional<std::string> WaterHeaterMixed::ambientTemperatureOutdoorAirNodeName() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->ambientTemperatureOutdoorAirNodeName();
  }

  boost::optional<double> WaterHeaterMixed::offCycleLossCoefficienttoAmbientTemperature() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->offCycleLossCoefficienttoAmbientTemperature();
  }

  double WaterHeaterMixed::offCycleLossFractiontoThermalZone() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->offCycleLossFractiontoThermalZone();
  }

  bool WaterHeaterMixed::isOffCycleLossFractiontoThermalZoneDefaulted() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->isOffCycleLossFractiontoThermalZoneDefaulted();
  }

  boost::optional<double> WaterHeaterMixed::onCycleLossCoefficienttoAmbientTemperature() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->onCycleLossCoefficienttoAmbientTemperature();
  }

  double WaterHeaterMixed::onCycleLossFractiontoThermalZone() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->onCycleLossFractiontoThermalZone();
  }

  bool WaterHeaterMixed::isOnCycleLossFractiontoThermalZoneDefaulted() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->isOnCycleLossFractiontoThermalZoneDefaulted();
  }

  boost::optional<double> WaterHeaterMixed::peakUseFlowRate() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->peakUseFlowRate();
  }

  boost::optional<Schedule> WaterHeaterMixed::useFlowRateFractionSchedule() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->useFlowRateFractionSchedule();
  }

  boost::optional<Schedule> WaterHeaterMixed::coldWaterSupplyTemperatureSchedule() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->coldWaterSupplyTemperatureSchedule();
  }

  double WaterHeaterMixed::useSideEffectiveness() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->useSideEffectiveness();
  }

  bool WaterHeaterMixed::isUseSideEffectivenessDefaulted() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->isUseSideEffectivenessDefaulted();
  }

  double WaterHeaterMixed::sourceSideEffectiveness() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->sourceSideEffectiveness();
  }

  bool WaterHeaterMixed::isSourceSideEffectivenessDefaulted() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->isSourceSideEffectivenessDefaulted();
  }

  boost::optional<double> WaterHeaterMixed::useSideDesignFlowRate() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->useSideDesignFlowRate();
  }

  bool WaterHeaterMixed::isUseSideDesignFlowRateDefaulted() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->isUseSideDesignFlowRateDefaulted();
  }

  bool WaterHeaterMixed::isUseSideDesignFlowRateAutosized() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->isUseSideDesignFlowRateAutosized();
  }

  boost::optional<double> WaterHeaterMixed::sourceSideDesignFlowRate() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->sourceSideDesignFlowRate();
  }

  bool WaterHeaterMixed::isSourceSideDesignFlowRateDefaulted() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->isSourceSideDesignFlowRateDefaulted();
  }

  bool WaterHeaterMixed::isSourceSideDesignFlowRateAutosized() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->isSourceSideDesignFlowRateAutosized();
  }

  double WaterHeaterMixed::indirectWaterHeatingRecoveryTime() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->indirectWaterHeatingRecoveryTime();
  }

  bool WaterHeaterMixed::isIndirectWaterHeatingRecoveryTimeDefaulted() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->isIndirectWaterHeatingRecoveryTimeDefaulted();
  }

  bool WaterHeaterMixed::setTankVolume(double tankVolume) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setTankVolume(tankVolume);
  }

  void WaterHeaterMixed::resetTankVolume() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetTankVolume();
  }

  void WaterHeaterMixed::autosizeTankVolume() {
    getImpl<detail::WaterHeaterMixed_Impl>()->autosizeTankVolume();
  }

  bool WaterHeaterMixed::setSetpointTemperatureSchedule(Schedule& setpointTemperatureSchedule) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setSetpointTemperatureSchedule(setpointTemperatureSchedule);
  }

  void WaterHeaterMixed::resetSetpointTemperatureSchedule() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetSetpointTemperatureSchedule();
  }

  bool WaterHeaterMixed::setDeadbandTemperatureDifference(double deadbandTemperatureDifference) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setDeadbandTemperatureDifference(deadbandTemperatureDifference);
  }

  void WaterHeaterMixed::resetDeadbandTemperatureDifference() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetDeadbandTemperatureDifference();
  }

  bool WaterHeaterMixed::setMaximumTemperatureLimit(double maximumTemperatureLimit) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setMaximumTemperatureLimit(maximumTemperatureLimit);
  }

  void WaterHeaterMixed::resetMaximumTemperatureLimit() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetMaximumTemperatureLimit();
  }

  bool WaterHeaterMixed::setHeaterControlType(std::string heaterControlType) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setHeaterControlType(heaterControlType);
  }

  void WaterHeaterMixed::resetHeaterControlType() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetHeaterControlType();
  }

  bool WaterHeaterMixed::setHeaterMaximumCapacity(double heaterMaximumCapacity) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setHeaterMaximumCapacity(heaterMaximumCapacity);
  }

  void WaterHeaterMixed::resetHeaterMaximumCapacity() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetHeaterMaximumCapacity();
  }

  void WaterHeaterMixed::autosizeHeaterMaximumCapacity() {
    getImpl<detail::WaterHeaterMixed_Impl>()->autosizeHeaterMaximumCapacity();
  }

  bool WaterHeaterMixed::setHeaterMinimumCapacity(double heaterMinimumCapacity) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setHeaterMinimumCapacity(heaterMinimumCapacity);
  }

  void WaterHeaterMixed::resetHeaterMinimumCapacity() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetHeaterMinimumCapacity();
  }

  bool WaterHeaterMixed::setHeaterIgnitionMinimumFlowRate(double heaterIgnitionMinimumFlowRate) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setHeaterIgnitionMinimumFlowRate(heaterIgnitionMinimumFlowRate);
  }

  void WaterHeaterMixed::resetHeaterIgnitionMinimumFlowRate() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetHeaterIgnitionMinimumFlowRate();
  }

  bool WaterHeaterMixed::setHeaterIgnitionDelay(double heaterIgnitionDelay) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setHeaterIgnitionDelay(heaterIgnitionDelay);
  }

  void WaterHeaterMixed::resetHeaterIgnitionDelay() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetHeaterIgnitionDelay();
  }

  bool WaterHeaterMixed::setHeaterFuelType(std::string heaterFuelType) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setHeaterFuelType(heaterFuelType);
  }

  bool WaterHeaterMixed::setHeaterThermalEfficiency(double heaterThermalEfficiency) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setHeaterThermalEfficiency(heaterThermalEfficiency);
  }

  void WaterHeaterMixed::resetHeaterThermalEfficiency() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetHeaterThermalEfficiency();
  }

  bool WaterHeaterMixed::setPartLoadFactorCurve(const CurveCubic& partLoadFactorCurve) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setPartLoadFactorCurve(partLoadFactorCurve);
  }

  void WaterHeaterMixed::resetPartLoadFactorCurve() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetPartLoadFactorCurve();
  }

  bool WaterHeaterMixed::setOffCycleParasiticFuelConsumptionRate(double offCycleParasiticFuelConsumptionRate) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setOffCycleParasiticFuelConsumptionRate(offCycleParasiticFuelConsumptionRate);
  }

  void WaterHeaterMixed::resetOffCycleParasiticFuelConsumptionRate() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetOffCycleParasiticFuelConsumptionRate();
  }

  bool WaterHeaterMixed::setOffCycleParasiticFuelType(std::string offCycleParasiticFuelType) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setOffCycleParasiticFuelType(offCycleParasiticFuelType);
  }

  void WaterHeaterMixed::resetOffCycleParasiticFuelType() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetOffCycleParasiticFuelType();
  }

  bool WaterHeaterMixed::setOffCycleParasiticHeatFractiontoTank(double offCycleParasiticHeatFractiontoTank) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setOffCycleParasiticHeatFractiontoTank(offCycleParasiticHeatFractiontoTank);
  }

  void WaterHeaterMixed::resetOffCycleParasiticHeatFractiontoTank() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetOffCycleParasiticHeatFractiontoTank();
  }

  bool WaterHeaterMixed::setOnCycleParasiticFuelConsumptionRate(double onCycleParasiticFuelConsumptionRate) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setOnCycleParasiticFuelConsumptionRate(onCycleParasiticFuelConsumptionRate);
  }

  void WaterHeaterMixed::resetOnCycleParasiticFuelConsumptionRate() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetOnCycleParasiticFuelConsumptionRate();
  }

  bool WaterHeaterMixed::setOnCycleParasiticFuelType(std::string onCycleParasiticFuelType) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setOnCycleParasiticFuelType(onCycleParasiticFuelType);
  }

  void WaterHeaterMixed::resetOnCycleParasiticFuelType() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetOnCycleParasiticFuelType();
  }

  bool WaterHeaterMixed::setOnCycleParasiticHeatFractiontoTank(double onCycleParasiticHeatFractiontoTank) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setOnCycleParasiticHeatFractiontoTank(onCycleParasiticHeatFractiontoTank);
  }

  void WaterHeaterMixed::resetOnCycleParasiticHeatFractiontoTank() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetOnCycleParasiticHeatFractiontoTank();
  }

  bool WaterHeaterMixed::setAmbientTemperatureIndicator(std::string ambientTemperatureIndicator) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setAmbientTemperatureIndicator(ambientTemperatureIndicator);
  }

  bool WaterHeaterMixed::setAmbientTemperatureSchedule(Schedule& ambientTemperatureSchedule) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setAmbientTemperatureSchedule(ambientTemperatureSchedule);
  }

  void WaterHeaterMixed::resetAmbientTemperatureSchedule() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetAmbientTemperatureSchedule();
  }

  bool WaterHeaterMixed::setAmbientTemperatureThermalZone(const ThermalZone& ambientTemperatureThermalZone) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setAmbientTemperatureThermalZone(ambientTemperatureThermalZone);
  }

  void WaterHeaterMixed::resetAmbientTemperatureThermalZone() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetAmbientTemperatureThermalZone();
  }

  bool WaterHeaterMixed::setAmbientTemperatureOutdoorAirNodeName(std::string ambientTemperatureOutdoorAirNodeName) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setAmbientTemperatureOutdoorAirNodeName(ambientTemperatureOutdoorAirNodeName);
  }

  void WaterHeaterMixed::resetAmbientTemperatureOutdoorAirNodeName() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetAmbientTemperatureOutdoorAirNodeName();
  }

  bool WaterHeaterMixed::setOffCycleLossCoefficienttoAmbientTemperature(double offCycleLossCoefficienttoAmbientTemperature) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setOffCycleLossCoefficienttoAmbientTemperature(offCycleLossCoefficienttoAmbientTemperature);
  }

  void WaterHeaterMixed::resetOffCycleLossCoefficienttoAmbientTemperature() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetOffCycleLossCoefficienttoAmbientTemperature();
  }

  bool WaterHeaterMixed::setOffCycleLossFractiontoThermalZone(double offCycleLossFractiontoThermalZone) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setOffCycleLossFractiontoThermalZone(offCycleLossFractiontoThermalZone);
  }

  void WaterHeaterMixed::resetOffCycleLossFractiontoThermalZone() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetOffCycleLossFractiontoThermalZone();
  }

  bool WaterHeaterMixed::setOnCycleLossCoefficienttoAmbientTemperature(double onCycleLossCoefficienttoAmbientTemperature) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setOnCycleLossCoefficienttoAmbientTemperature(onCycleLossCoefficienttoAmbientTemperature);
  }

  void WaterHeaterMixed::resetOnCycleLossCoefficienttoAmbientTemperature() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetOnCycleLossCoefficienttoAmbientTemperature();
  }

  bool WaterHeaterMixed::setOnCycleLossFractiontoThermalZone(double onCycleLossFractiontoThermalZone) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setOnCycleLossFractiontoThermalZone(onCycleLossFractiontoThermalZone);
  }

  void WaterHeaterMixed::resetOnCycleLossFractiontoThermalZone() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetOnCycleLossFractiontoThermalZone();
  }

  bool WaterHeaterMixed::setPeakUseFlowRate(double peakUseFlowRate) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setPeakUseFlowRate(peakUseFlowRate);
  }

  void WaterHeaterMixed::resetPeakUseFlowRate() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetPeakUseFlowRate();
  }

  bool WaterHeaterMixed::setUseFlowRateFractionSchedule(Schedule& useFlowRateFractionSchedule) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setUseFlowRateFractionSchedule(useFlowRateFractionSchedule);
  }

  void WaterHeaterMixed::resetUseFlowRateFractionSchedule() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetUseFlowRateFractionSchedule();
  }

  bool WaterHeaterMixed::setColdWaterSupplyTemperatureSchedule(Schedule& coldWaterSupplyTemperatureSchedule) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setColdWaterSupplyTemperatureSchedule(coldWaterSupplyTemperatureSchedule);
  }

  void WaterHeaterMixed::resetColdWaterSupplyTemperatureSchedule() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetColdWaterSupplyTemperatureSchedule();
  }

  bool WaterHeaterMixed::setUseSideEffectiveness(double useSideEffectiveness) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setUseSideEffectiveness(useSideEffectiveness);
  }

  void WaterHeaterMixed::resetUseSideEffectiveness() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetUseSideEffectiveness();
  }

  bool WaterHeaterMixed::setSourceSideEffectiveness(double sourceSideEffectiveness) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setSourceSideEffectiveness(sourceSideEffectiveness);
  }

  void WaterHeaterMixed::resetSourceSideEffectiveness() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetSourceSideEffectiveness();
  }

  bool WaterHeaterMixed::setUseSideDesignFlowRate(double useSideDesignFlowRate) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setUseSideDesignFlowRate(useSideDesignFlowRate);
  }

  void WaterHeaterMixed::resetUseSideDesignFlowRate() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetUseSideDesignFlowRate();
  }

  void WaterHeaterMixed::autosizeUseSideDesignFlowRate() {
    getImpl<detail::WaterHeaterMixed_Impl>()->autosizeUseSideDesignFlowRate();
  }

  bool WaterHeaterMixed::setSourceSideDesignFlowRate(double sourceSideDesignFlowRate) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setSourceSideDesignFlowRate(sourceSideDesignFlowRate);
  }

  void WaterHeaterMixed::resetSourceSideDesignFlowRate() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetSourceSideDesignFlowRate();
  }

  void WaterHeaterMixed::autosizeSourceSideDesignFlowRate() {
    getImpl<detail::WaterHeaterMixed_Impl>()->autosizeSourceSideDesignFlowRate();
  }

  bool WaterHeaterMixed::setIndirectWaterHeatingRecoveryTime(double indirectWaterHeatingRecoveryTime) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setIndirectWaterHeatingRecoveryTime(indirectWaterHeatingRecoveryTime);
  }

  void WaterHeaterMixed::resetIndirectWaterHeatingRecoveryTime() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetIndirectWaterHeatingRecoveryTime();
  }

  std::vector<std::string> WaterHeaterMixed::sourceSideFlowControlModeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_WaterHeater_MixedFields::SourceSideFlowControlMode);
  }

  std::string WaterHeaterMixed::sourceSideFlowControlMode() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->sourceSideFlowControlMode();
  }

  bool WaterHeaterMixed::setSourceSideFlowControlMode(const std::string& sourceSideFlowControlMode) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setSourceSideFlowControlMode(sourceSideFlowControlMode);
  }

  boost::optional<Schedule> WaterHeaterMixed::indirectAlternateSetpointTemperatureSchedule() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->indirectAlternateSetpointTemperatureSchedule();
  }

  bool WaterHeaterMixed::setIndirectAlternateSetpointTemperatureSchedule(Schedule& indirectAlternateSetpointTemperatureSchedule) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setIndirectAlternateSetpointTemperatureSchedule(indirectAlternateSetpointTemperatureSchedule);
  }
  void WaterHeaterMixed::resetIndirectAlternateSetpointTemperatureSchedule() {
    getImpl<detail::WaterHeaterMixed_Impl>()->resetIndirectAlternateSetpointTemperatureSchedule();
  }

  std::string WaterHeaterMixed::endUseSubcategory() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->endUseSubcategory();
  }

  bool WaterHeaterMixed::setEndUseSubcategory(const std::string& endUseSubcategory) {
    return getImpl<detail::WaterHeaterMixed_Impl>()->setEndUseSubcategory(endUseSubcategory);
  }

  /// @cond
  WaterHeaterMixed::WaterHeaterMixed(std::shared_ptr<detail::WaterHeaterMixed_Impl> impl) : WaterToWaterComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> WaterHeaterMixed::autosizedTankVolume() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->autosizedTankVolume();
  }

  boost::optional<double> WaterHeaterMixed::autosizedHeaterMaximumCapacity() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->autosizedHeaterMaximumCapacity();
  }

  boost::optional<double> WaterHeaterMixed::autosizedUseSideDesignFlowRate() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->autosizedUseSideDesignFlowRate();
  }

  boost::optional<double> WaterHeaterMixed::autosizedSourceSideDesignFlowRate() const {
    return getImpl<detail::WaterHeaterMixed_Impl>()->autosizedSourceSideDesignFlowRate();
  }

}  // namespace model
}  // namespace openstudio