/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "CoilCoolingDXSingleSpeedThermalStorage.hpp"
#include "CoilCoolingDXSingleSpeedThermalStorage_Impl.hpp"

// TODO: Check the following class names against object getters and setters.
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "FluidAndGlycol.hpp"
#include "FluidAndGlycol_Impl.hpp"
#include "Connection.hpp"
#include "Connection_Impl.hpp"
#include "BivariateFunctions.hpp"
#include "BivariateFunctions_Impl.hpp"
#include "UnivariateFunctions.hpp"
#include "UnivariateFunctions_Impl.hpp"
#include "TrivariateFunctions.hpp"
#include "TrivariateFunctions_Impl.hpp"
#include "WaterStorageTank.hpp"
#include "WaterStorageTank_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Coil_Cooling_DX_SingleSpeed_ThermalStorage_FieldEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    CoilCoolingDXSingleSpeedThermalStorage_Impl::CoilCoolingDXSingleSpeedThermalStorage_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                             bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == CoilCoolingDXSingleSpeedThermalStorage::iddObjectType());
    }

    CoilCoolingDXSingleSpeedThermalStorage_Impl::CoilCoolingDXSingleSpeedThermalStorage_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == CoilCoolingDXSingleSpeedThermalStorage::iddObjectType());
    }

    CoilCoolingDXSingleSpeedThermalStorage_Impl::CoilCoolingDXSingleSpeedThermalStorage_Impl(const CoilCoolingDXSingleSpeedThermalStorage_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& CoilCoolingDXSingleSpeedThermalStorage_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType CoilCoolingDXSingleSpeedThermalStorage_Impl::iddObjectType() const {
      return CoilCoolingDXSingleSpeedThermalStorage::iddObjectType();
    }

    std::vector<ScheduleTypeKey> CoilCoolingDXSingleSpeedThermalStorage_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      // TODO: Check schedule display names.
      std::vector<ScheduleTypeKey> result;
      const UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      if (std::find(fieldIndices.cbegin(), fieldIndices.cend(), OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::AvailabilitySchedule)
          != fieldIndices.cend()) {
        result.emplace_back("CoilCoolingDXSingleSpeedThermalStorage", "Availability Schedule");
      }
      if (std::find(fieldIndices.cbegin(), fieldIndices.cend(), OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::OperationModeControlSchedule)
          != fieldIndices.cend()) {
        result.emplace_back("CoilCoolingDXSingleSpeedThermalStorage", "Operation Mode Control Schedule");
      }
      if (std::find(fieldIndices.cbegin(), fieldIndices.cend(), OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::BasinHeaterAvailabilitySchedule)
          != fieldIndices.cend()) {
        result.emplace_back("CoilCoolingDXSingleSpeedThermalStorage", "Basin Heater Availability Schedule");
      }
      return result;
    }

    ComponentType CoilCoolingDXSingleSpeedThermalStorage_Impl::componentType() const {
      // TODO
      return ComponentType::None;
    }

    std::vector<FuelType> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingFuelTypes() const {
      // TODO
      return {};
    }

    std::vector<FuelType> CoilCoolingDXSingleSpeedThermalStorage_Impl::heatingFuelTypes() const {
      // TODO
      return {};
    }

    std::vector<AppGFuelType> CoilCoolingDXSingleSpeedThermalStorage_Impl::appGHeatingFuelTypes() const {

      // TODO
      return {};
    }

    Schedule CoilCoolingDXSingleSpeedThermalStorage_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
      }
      return value.get();
    }

    std::string CoilCoolingDXSingleSpeedThermalStorage_Impl::operatingModeControlMethod() const {
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::OperatingModeControlMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> CoilCoolingDXSingleSpeedThermalStorage_Impl::operationModeControlSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::OperationModeControlSchedule);
    }

    std::string CoilCoolingDXSingleSpeedThermalStorage_Impl::storageType() const {
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageType, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<FluidAndGlycol> CoilCoolingDXSingleSpeedThermalStorage_Impl::userDefinedFluidType() const {
      return getObject<ModelObject>().getModelObjectTarget<FluidAndGlycol>(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::UserDefinedFluidType);
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::fluidStorageVolume() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageVolume, true);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::isFluidStorageVolumeAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageVolume, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autocalculate");
      }
      return result;
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::iceStorageCapacity() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::IceStorageCapacity, true);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::isIceStorageCapacityAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::IceStorageCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autocalculate");
      }
      return result;
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::storageCapacitySizingFactor() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageCapacitySizingFactor, true);
    }

    Connection CoilCoolingDXSingleSpeedThermalStorage_Impl::storageTankAmbientTemperatureNode() const {
      boost::optional<Connection> value = optionalStorageTankAmbientTemperatureNode();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Storage Tank Ambient Temperature Node attached.");
      }
      return value.get();
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::fluidStorageTankRatingTemperature() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageTankRatingTemperature, true);
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::ratedEvaporatorAirFlowRate() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::RatedEvaporatorAirFlowRate, true);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::isRatedEvaporatorAirFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::RatedEvaporatorAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizedRatedEvaporatorAirFlowRate() {
      return getAutosizedValue("TODO_CHECK_SQL Rated Evaporator Air Flow Rate", "m3/s");
    }

    Connection CoilCoolingDXSingleSpeedThermalStorage_Impl::evaporatorAirInletNode() const {
      boost::optional<Connection> value = optionalEvaporatorAirInletNode();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Evaporator Air Inlet Node attached.");
      }
      return value.get();
    }

    Connection CoilCoolingDXSingleSpeedThermalStorage_Impl::evaporatorAirOutletNode() const {
      boost::optional<Connection> value = optionalEvaporatorAirOutletNode();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Evaporator Air Outlet Node attached.");
      }
      return value.get();
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingOnlyModeAvailable() const {
      return getBooleanFieldValue(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeAvailable);
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingOnlyModeRatedTotalEvaporatorCoolingCapacity() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedTotalEvaporatorCoolingCapacity, true);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::isCoolingOnlyModeRatedTotalEvaporatorCoolingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedTotalEvaporatorCoolingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizedCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity() {
      return getAutosizedValue("TODO_CHECK_SQL Cooling Only Mode Rated Total Evaporator Cooling Capacity", "W");
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingOnlyModeRatedSensibleHeatRatio() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedSensibleHeatRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingOnlyModeRatedCOP() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<BivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<BivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve);
    }

    boost::optional<UnivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve);
    }

    boost::optional<BivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<BivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve);
    }

    boost::optional<UnivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve);
    }

    boost::optional<UnivariateFunctions> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingOnlyModePartLoadFractionCorrelationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModePartLoadFractionCorrelationCurve);
    }

    boost::optional<BivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<BivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve);
    }

    boost::optional<UnivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeAvailable() const {
      return getBooleanFieldValue(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeAvailable);
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity, true);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::isCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacityAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autocalculate");
      }
      return result;
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeCapacitySizingFactor() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeCapacitySizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeRatedStorageChargingCapacity() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedStorageChargingCapacity, true);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::isCoolingAndChargeModeRatedStorageChargingCapacityAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedStorageChargingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autocalculate");
      }
      return result;
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeStorageCapacitySizingFactor() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageCapacitySizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeRatedSensibleHeatRatio() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedSensibleHeatRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeCoolingRatedCOP() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeCoolingRatedCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeChargingRatedCOP() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeChargingRatedCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<TrivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<TrivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve);
    }

    boost::optional<UnivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve);
    }

    boost::optional<TrivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<TrivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve);
    }

    boost::optional<UnivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve);
    }

    boost::optional<UnivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve);
    }

    boost::optional<TrivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<TrivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve);
    }

    boost::optional<UnivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve);
    }

    boost::optional<TrivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<TrivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve);
    }

    boost::optional<UnivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve);
    }

    boost::optional<UnivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve);
    }

    boost::optional<BivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<BivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve);
    }

    boost::optional<UnivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeAvailable() const {
      return getBooleanFieldValue(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeAvailable);
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity, true);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::isCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacityAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autocalculate");
      }
      return result;
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeEvaporatorCapacitySizingFactor() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorCapacitySizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeRatedStorageDischargingCapacity() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedStorageDischargingCapacity, true);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::isCoolingAndDischargeModeRatedStorageDischargingCapacityAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedStorageDischargingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autocalculate");
      }
      return result;
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeStorageDischargeCapacitySizingFactor() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacitySizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeRatedSensibleHeatRatio() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedSensibleHeatRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeCoolingRatedCOP() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeCoolingRatedCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeDischargingRatedCOP() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeDischargingRatedCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<TrivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<TrivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve);
    }

    boost::optional<UnivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve);
    }

    boost::optional<TrivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<TrivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve);
    }

    boost::optional<UnivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve);
    }

    boost::optional<UnivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve);
    }

    boost::optional<TrivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<TrivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve);
    }

    boost::optional<UnivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve);
    }

    boost::optional<UnivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve);
    }

    boost::optional<TrivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<TrivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve);
    }

    boost::optional<UnivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve);
    }

    boost::optional<UnivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve);
    }

    boost::optional<BivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<BivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve);
    }

    boost::optional<UnivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::chargeOnlyModeAvailable() const {
      return getBooleanFieldValue(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeAvailable);
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::chargeOnlyModeRatedStorageChargingCapacity() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeRatedStorageChargingCapacity, true);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::isChargeOnlyModeRatedStorageChargingCapacityAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeRatedStorageChargingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autocalculate");
      }
      return result;
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::chargeOnlyModeCapacitySizingFactor() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeCapacitySizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::chargeOnlyModeChargingRatedCOP() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeChargingRatedCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<BivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<BivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve);
    }

    boost::optional<BivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<BivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModeAvailable() const {
      return getBooleanFieldValue(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeAvailable);
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModeRatedStorageDischargingCapacity() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedStorageDischargingCapacity, true);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::isDischargeOnlyModeRatedStorageDischargingCapacityAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedStorageDischargingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autocalculate");
      }
      return result;
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModeCapacitySizingFactor() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeCapacitySizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModeRatedSensibleHeatRatio() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedSensibleHeatRatio, true);
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModeRatedCOP() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<BivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<BivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve);
    }

    boost::optional<UnivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve);
    }

    boost::optional<BivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<BivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve);
    }

    boost::optional<UnivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve);
    }

    boost::optional<UnivariateFunctions> CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModePartLoadFractionCorrelationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModePartLoadFractionCorrelationCurve);
    }

    boost::optional<BivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<BivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve);
    }

    boost::optional<UnivariateFunctions>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<UnivariateFunctions>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve);
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::ancillaryElectricPower() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::AncillaryElectricPower, true);
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::coldWeatherOperationMinimumOutdoorAirTemperature() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ColdWeatherOperationMinimumOutdoorAirTemperature, true);
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::coldWeatherOperationAncillaryPower() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ColdWeatherOperationAncillaryPower, true);
    }

    Connection CoilCoolingDXSingleSpeedThermalStorage_Impl::condenserAirInletNode() const {
      boost::optional<Connection> value = optionalCondenserAirInletNode();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Condenser Air Inlet Node attached.");
      }
      return value.get();
    }

    Connection CoilCoolingDXSingleSpeedThermalStorage_Impl::condenserAirOutletNode() const {
      boost::optional<Connection> value = optionalCondenserAirOutletNode();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Condenser Air Outlet Node attached.");
      }
      return value.get();
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::condenserDesignAirFlowRate() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserDesignAirFlowRate, true);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::isCondenserDesignAirFlowRateAutocalculated() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserDesignAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autocalculate");
      }
      return result;
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::condenserAirFlowSizingFactor() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserAirFlowSizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string CoilCoolingDXSingleSpeedThermalStorage_Impl::condenserType() const {
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserType, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::evaporativeCondenserEffectiveness() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporativeCondenserEffectiveness, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::evaporativeCondenserPumpRatedPowerConsumption() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporativeCondenserPumpRatedPowerConsumption, true);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporativeCondenserPumpRatedPowerConsumption, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizedEvaporativeCondenserPumpRatedPowerConsumption() {
      return getAutosizedValue("TODO_CHECK_SQL Evaporative Condenser Pump Rated Power Consumption", "W");
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::basinHeaterCapacity() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::BasinHeaterCapacity, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::basinHeaterSetpointTemperature() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::BasinHeaterSetpointTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> CoilCoolingDXSingleSpeedThermalStorage_Impl::basinHeaterAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::BasinHeaterAvailabilitySchedule);
    }

    boost::optional<WaterStorageTank> CoilCoolingDXSingleSpeedThermalStorage_Impl::supplyWaterStorageTank() const {
      return getObject<ModelObject>().getModelObjectTarget<WaterStorageTank>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::SupplyWaterStorageTank);
    }

    boost::optional<WaterStorageTank> CoilCoolingDXSingleSpeedThermalStorage_Impl::condensateCollectionWaterStorageTank() const {
      return getObject<ModelObject>().getModelObjectTarget<WaterStorageTank>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondensateCollectionWaterStorageTank);
    }

    boost::optional<Connection> CoilCoolingDXSingleSpeedThermalStorage_Impl::storageTankPlantConnectionInletNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionInletNode);
    }

    boost::optional<Connection> CoilCoolingDXSingleSpeedThermalStorage_Impl::storageTankPlantConnectionOutletNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionOutletNode);
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::storageTankPlantConnectionDesignFlowRate() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionDesignFlowRate, true);
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::storageTankPlantConnectionHeatTransferEffectiveness() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionHeatTransferEffectiveness, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::storageTankMinimumOperatingLimitFluidTemperature() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankMinimumOperatingLimitFluidTemperature, true);
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::storageTankMaximumOperatingLimitFluidTemperature() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankMaximumOperatingLimitFluidTemperature, true);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setAvailabilitySchedule(Schedule& schedule) {
      const bool result = setSchedule(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::AvailabilitySchedule,
                                      "CoilCoolingDXSingleSpeedThermalStorage", "Availability Schedule", schedule);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setOperatingModeControlMethod(const std::string& operatingModeControlMethod) {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::OperatingModeControlMethod, operatingModeControlMethod);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setOperationModeControlSchedule(Schedule& schedule) {
      const bool result = setSchedule(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::OperationModeControlSchedule,
                                      "CoilCoolingDXSingleSpeedThermalStorage", "Operation Mode Control Schedule", schedule);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetOperationModeControlSchedule() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::OperationModeControlSchedule, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setStorageType(const std::string& storageType) {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageType, storageType);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setUserDefinedFluidType(const FluidAndGlycol& fluidAndGlycol) {
      const bool result = setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::UserDefinedFluidType, fluidAndGlycol.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetUserDefinedFluidType() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::UserDefinedFluidType, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setFluidStorageVolume(double fluidStorageVolume) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageVolume, fluidStorageVolume);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetFluidStorageVolume() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageVolume, "");
      OS_ASSERT(result);
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autocalculateFluidStorageVolume() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageVolume, "autocalculate");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setIceStorageCapacity(double iceStorageCapacity) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::IceStorageCapacity, iceStorageCapacity);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetIceStorageCapacity() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::IceStorageCapacity, "");
      OS_ASSERT(result);
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autocalculateIceStorageCapacity() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::IceStorageCapacity, "autocalculate");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setStorageCapacitySizingFactor(double storageCapacitySizingFactor) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageCapacitySizingFactor, storageCapacitySizingFactor);
      OS_ASSERT(result);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetStorageCapacitySizingFactor() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageCapacitySizingFactor, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setStorageTankAmbientTemperatureNode(const Connection& connection) {
      const bool result = setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankAmbientTemperatureNode, connection.handle());
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setStorageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient(
      double storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient,
                                    storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setFluidStorageTankRatingTemperature(double fluidStorageTankRatingTemperature) {
      const bool result =
        setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageTankRatingTemperature, fluidStorageTankRatingTemperature);
      OS_ASSERT(result);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetFluidStorageTankRatingTemperature() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageTankRatingTemperature, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setRatedEvaporatorAirFlowRate(double ratedEvaporatorAirFlowRate) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::RatedEvaporatorAirFlowRate, ratedEvaporatorAirFlowRate);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizeRatedEvaporatorAirFlowRate() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::RatedEvaporatorAirFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setEvaporatorAirInletNode(const Connection& connection) {
      const bool result = setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirInletNode, connection.handle());
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setEvaporatorAirOutletNode(const Connection& connection) {
      const bool result = setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirOutletNode, connection.handle());
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingOnlyModeAvailable(bool coolingOnlyModeAvailable) {
      const bool result = setBooleanFieldValue(field.fieldEnum, field.setterArgumentName);
      OS_ASSERT(result);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity(
      double coolingOnlyModeRatedTotalEvaporatorCoolingCapacity) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedTotalEvaporatorCoolingCapacity,
                                    coolingOnlyModeRatedTotalEvaporatorCoolingCapacity);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedTotalEvaporatorCoolingCapacity, "");
      OS_ASSERT(result);
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizeCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedTotalEvaporatorCoolingCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingOnlyModeRatedSensibleHeatRatio(double coolingOnlyModeRatedSensibleHeatRatio) {
      const bool result =
        setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedSensibleHeatRatio, coolingOnlyModeRatedSensibleHeatRatio);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingOnlyModeRatedCOP(double coolingOnlyModeRatedCOP) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedCOP, coolingOnlyModeRatedCOP);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(
      const BivariateFunctions& bivariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve,
                   bivariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve,
                   univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve(
      const BivariateFunctions& bivariateFunctions) {
      const bool result = setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve,
                                     bivariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result = setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve,
                                     univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingOnlyModePartLoadFractionCorrelationCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result = setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModePartLoadFractionCorrelationCurve,
                                     univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingOnlyModePartLoadFractionCorrelationCurve() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModePartLoadFractionCorrelationCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(
      const BivariateFunctions& bivariateFunctions) {
      const bool result = setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve,
                                     bivariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result = setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve,
                                     univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeAvailable(bool coolingAndChargeModeAvailable) {
      const bool result = setBooleanFieldValue(field.fieldEnum, field.setterArgumentName);
      OS_ASSERT(result);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity(
      double coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity,
                                    coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity, "");
      OS_ASSERT(result);
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autocalculateCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity, "autocalculate");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeCapacitySizingFactor(double coolingAndChargeModeCapacitySizingFactor) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeCapacitySizingFactor,
                                    coolingAndChargeModeCapacitySizingFactor);
      OS_ASSERT(result);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeRatedStorageChargingCapacity(
      double coolingAndChargeModeRatedStorageChargingCapacity) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedStorageChargingCapacity,
                                    coolingAndChargeModeRatedStorageChargingCapacity);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeRatedStorageChargingCapacity() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedStorageChargingCapacity, "");
      OS_ASSERT(result);
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autocalculateCoolingAndChargeModeRatedStorageChargingCapacity() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedStorageChargingCapacity, "autocalculate");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeStorageCapacitySizingFactor(
      double coolingAndChargeModeStorageCapacitySizingFactor) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageCapacitySizingFactor,
                                    coolingAndChargeModeStorageCapacitySizingFactor);
      OS_ASSERT(result);
      return result;
    }

    bool
      CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeRatedSensibleHeatRatio(double coolingAndChargeModeRatedSensibleHeatRatio) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedSensibleHeatRatio,
                                    coolingAndChargeModeRatedSensibleHeatRatio);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeCoolingRatedCOP(double coolingAndChargeModeCoolingRatedCOP) {
      const bool result =
        setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeCoolingRatedCOP, coolingAndChargeModeCoolingRatedCOP);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeChargingRatedCOP(double coolingAndChargeModeChargingRatedCOP) {
      const bool result =
        setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeChargingRatedCOP, coolingAndChargeModeChargingRatedCOP);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(
      const TrivariateFunctions& trivariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve,
                   trivariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() {
      const bool result = setString(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve,
                   univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() {
      const bool result = setString(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(
      const TrivariateFunctions& trivariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve,
                   trivariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve,
                   univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve,
                   univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve(
      const TrivariateFunctions& trivariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve,
                   trivariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve,
                   univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve() {
      const bool result = setString(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(
      const TrivariateFunctions& trivariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve,
                   trivariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve,
                   univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve,
                   univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve(
      const BivariateFunctions& bivariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve,
                   bivariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve,
                   univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeAvailable(bool coolingAndDischargeModeAvailable) {
      const bool result = setBooleanFieldValue(field.fieldEnum, field.setterArgumentName);
      OS_ASSERT(result);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity(
      double coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity,
                                    coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity, "");
      OS_ASSERT(result);
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autocalculateCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity, "autocalculate");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeEvaporatorCapacitySizingFactor(
      double coolingAndDischargeModeEvaporatorCapacitySizingFactor) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorCapacitySizingFactor,
                                    coolingAndDischargeModeEvaporatorCapacitySizingFactor);
      OS_ASSERT(result);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeRatedStorageDischargingCapacity(
      double coolingAndDischargeModeRatedStorageDischargingCapacity) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedStorageDischargingCapacity,
                                    coolingAndDischargeModeRatedStorageDischargingCapacity);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeRatedStorageDischargingCapacity() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedStorageDischargingCapacity, "");
      OS_ASSERT(result);
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autocalculateCoolingAndDischargeModeRatedStorageDischargingCapacity() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedStorageDischargingCapacity, "autocalculate");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeStorageDischargeCapacitySizingFactor(
      double coolingAndDischargeModeStorageDischargeCapacitySizingFactor) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacitySizingFactor,
                                    coolingAndDischargeModeStorageDischargeCapacitySizingFactor);
      OS_ASSERT(result);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeRatedSensibleHeatRatio(
      double coolingAndDischargeModeRatedSensibleHeatRatio) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedSensibleHeatRatio,
                                    coolingAndDischargeModeRatedSensibleHeatRatio);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeCoolingRatedCOP(double coolingAndDischargeModeCoolingRatedCOP) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeCoolingRatedCOP,
                                    coolingAndDischargeModeCoolingRatedCOP);
      return result;
    }

    bool
      CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeDischargingRatedCOP(double coolingAndDischargeModeDischargingRatedCOP) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeDischargingRatedCOP,
                                    coolingAndDischargeModeDischargingRatedCOP);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(
      const TrivariateFunctions& trivariateFunctions) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve,
        trivariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() {
      const bool result = setString(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve,
        univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() {
      const bool result = setString(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(
      const TrivariateFunctions& trivariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve,
                   trivariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve() {
      const bool result = setString(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve,
                   univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve() {
      const bool result = setString(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve,
                   univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve(
      const TrivariateFunctions& trivariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve,
                   trivariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve,
                   univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve() {
      const bool result = setString(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve,
        univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve() {
      const bool result = setString(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(
      const TrivariateFunctions& trivariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve,
                   trivariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve,
                   univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve,
                   univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve(
      const BivariateFunctions& bivariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve,
                   bivariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve,
                   univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setChargeOnlyModeAvailable(bool chargeOnlyModeAvailable) {
      const bool result = setBooleanFieldValue(field.fieldEnum, field.setterArgumentName);
      OS_ASSERT(result);
      return result;
    }

    bool
      CoilCoolingDXSingleSpeedThermalStorage_Impl::setChargeOnlyModeRatedStorageChargingCapacity(double chargeOnlyModeRatedStorageChargingCapacity) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeRatedStorageChargingCapacity,
                                    chargeOnlyModeRatedStorageChargingCapacity);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetChargeOnlyModeRatedStorageChargingCapacity() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeRatedStorageChargingCapacity, "");
      OS_ASSERT(result);
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autocalculateChargeOnlyModeRatedStorageChargingCapacity() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeRatedStorageChargingCapacity, "autocalculate");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setChargeOnlyModeCapacitySizingFactor(double chargeOnlyModeCapacitySizingFactor) {
      const bool result =
        setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeCapacitySizingFactor, chargeOnlyModeCapacitySizingFactor);
      OS_ASSERT(result);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setChargeOnlyModeChargingRatedCOP(double chargeOnlyModeChargingRatedCOP) {
      const bool result =
        setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeChargingRatedCOP, chargeOnlyModeChargingRatedCOP);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve(
      const BivariateFunctions& bivariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve,
                   bivariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve(
      const BivariateFunctions& bivariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve,
                   bivariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setDischargeOnlyModeAvailable(bool dischargeOnlyModeAvailable) {
      const bool result = setBooleanFieldValue(field.fieldEnum, field.setterArgumentName);
      OS_ASSERT(result);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setDischargeOnlyModeRatedStorageDischargingCapacity(
      double dischargeOnlyModeRatedStorageDischargingCapacity) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedStorageDischargingCapacity,
                                    dischargeOnlyModeRatedStorageDischargingCapacity);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetDischargeOnlyModeRatedStorageDischargingCapacity() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedStorageDischargingCapacity, "");
      OS_ASSERT(result);
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autocalculateDischargeOnlyModeRatedStorageDischargingCapacity() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedStorageDischargingCapacity, "autocalculate");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setDischargeOnlyModeCapacitySizingFactor(double dischargeOnlyModeCapacitySizingFactor) {
      const bool result =
        setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeCapacitySizingFactor, dischargeOnlyModeCapacitySizingFactor);
      OS_ASSERT(result);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setDischargeOnlyModeRatedSensibleHeatRatio(double dischargeOnlyModeRatedSensibleHeatRatio) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedSensibleHeatRatio,
                                    dischargeOnlyModeRatedSensibleHeatRatio);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetDischargeOnlyModeRatedSensibleHeatRatio() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedSensibleHeatRatio, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setDischargeOnlyModeRatedCOP(double dischargeOnlyModeRatedCOP) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedCOP, dischargeOnlyModeRatedCOP);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setDischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve(
      const BivariateFunctions& bivariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve,
                   bivariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetDischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setDischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve,
                   univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetDischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setDischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve(
      const BivariateFunctions& bivariateFunctions) {
      const bool result = setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve,
                                     bivariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetDischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setDischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve,
                   univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetDischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setDischargeOnlyModePartLoadFractionCorrelationCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result = setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModePartLoadFractionCorrelationCurve,
                                     univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetDischargeOnlyModePartLoadFractionCorrelationCurve() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModePartLoadFractionCorrelationCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setDischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(
      const BivariateFunctions& bivariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve,
                   bivariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetDischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setDischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(
      const UnivariateFunctions& univariateFunctions) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve,
                   univariateFunctions.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetDischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setAncillaryElectricPower(double ancillaryElectricPower) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::AncillaryElectricPower, ancillaryElectricPower);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetAncillaryElectricPower() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::AncillaryElectricPower, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setColdWeatherOperationMinimumOutdoorAirTemperature(
      double coldWeatherOperationMinimumOutdoorAirTemperature) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ColdWeatherOperationMinimumOutdoorAirTemperature,
                                    coldWeatherOperationMinimumOutdoorAirTemperature);
      OS_ASSERT(result);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetColdWeatherOperationMinimumOutdoorAirTemperature() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ColdWeatherOperationMinimumOutdoorAirTemperature, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setColdWeatherOperationAncillaryPower(double coldWeatherOperationAncillaryPower) {
      const bool result =
        setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ColdWeatherOperationAncillaryPower, coldWeatherOperationAncillaryPower);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetColdWeatherOperationAncillaryPower() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ColdWeatherOperationAncillaryPower, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCondenserAirInletNode(const Connection& connection) {
      const bool result = setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserAirInletNode, connection.handle());
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCondenserAirOutletNode(const Connection& connection) {
      const bool result = setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserAirOutletNode, connection.handle());
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCondenserDesignAirFlowRate(double condenserDesignAirFlowRate) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserDesignAirFlowRate, condenserDesignAirFlowRate);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autocalculateCondenserDesignAirFlowRate() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserDesignAirFlowRate, "autocalculate");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCondenserAirFlowSizingFactor(double condenserAirFlowSizingFactor) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserAirFlowSizingFactor, condenserAirFlowSizingFactor);
      OS_ASSERT(result);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCondenserType(const std::string& condenserType) {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserType, condenserType);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness) {
      const bool result =
        setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporativeCondenserEffectiveness, evaporativeCondenserEffectiveness);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setEvaporativeCondenserPumpRatedPowerConsumption(
      double evaporativeCondenserPumpRatedPowerConsumption) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporativeCondenserPumpRatedPowerConsumption,
                                    evaporativeCondenserPumpRatedPowerConsumption);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizeEvaporativeCondenserPumpRatedPowerConsumption() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporativeCondenserPumpRatedPowerConsumption, "autosize");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setBasinHeaterCapacity(double basinHeaterCapacity) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::BasinHeaterCapacity, basinHeaterCapacity);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
      const bool result =
        setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::BasinHeaterSetpointTemperature, basinHeaterSetpointTemperature);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setBasinHeaterAvailabilitySchedule(Schedule& schedule) {
      const bool result = setSchedule(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::BasinHeaterAvailabilitySchedule,
                                      "CoilCoolingDXSingleSpeedThermalStorage", "Basin Heater Availability Schedule", schedule);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetBasinHeaterAvailabilitySchedule() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::BasinHeaterAvailabilitySchedule, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setSupplyWaterStorageTank(const WaterStorageTank& waterStorageTank) {
      const bool result = setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::SupplyWaterStorageTank, waterStorageTank.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetSupplyWaterStorageTank() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::SupplyWaterStorageTank, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCondensateCollectionWaterStorageTank(const WaterStorageTank& waterStorageTank) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondensateCollectionWaterStorageTank, waterStorageTank.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCondensateCollectionWaterStorageTank() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondensateCollectionWaterStorageTank, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setStorageTankPlantConnectionInletNode(const Connection& connection) {
      const bool result = setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionInletNode, connection.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetStorageTankPlantConnectionInletNode() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionInletNode, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setStorageTankPlantConnectionOutletNode(const Connection& connection) {
      const bool result = setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionOutletNode, connection.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetStorageTankPlantConnectionOutletNode() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionOutletNode, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setStorageTankPlantConnectionDesignFlowRate(double storageTankPlantConnectionDesignFlowRate) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionDesignFlowRate,
                                    storageTankPlantConnectionDesignFlowRate);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetStorageTankPlantConnectionDesignFlowRate() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionDesignFlowRate, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setStorageTankPlantConnectionHeatTransferEffectiveness(
      double storageTankPlantConnectionHeatTransferEffectiveness) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionHeatTransferEffectiveness,
                                    storageTankPlantConnectionHeatTransferEffectiveness);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setStorageTankMinimumOperatingLimitFluidTemperature(
      double storageTankMinimumOperatingLimitFluidTemperature) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankMinimumOperatingLimitFluidTemperature,
                                    storageTankMinimumOperatingLimitFluidTemperature);
      OS_ASSERT(result);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetStorageTankMinimumOperatingLimitFluidTemperature() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankMinimumOperatingLimitFluidTemperature, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setStorageTankMaximumOperatingLimitFluidTemperature(
      double storageTankMaximumOperatingLimitFluidTemperature) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankMaximumOperatingLimitFluidTemperature,
                                    storageTankMaximumOperatingLimitFluidTemperature);
      OS_ASSERT(result);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetStorageTankMaximumOperatingLimitFluidTemperature() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankMaximumOperatingLimitFluidTemperature, "");
      OS_ASSERT(result);
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autosize() {
      autosizeRatedEvaporatorAirFlowRate();
      autosizeCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity();
      autosizeEvaporativeCondenserPumpRatedPowerConsumption();
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::applySizingValues() {
      if (boost::optional<double> val_ = autosizedRatedEvaporatorAirFlowRate()) {
        setRatedEvaporatorAirFlowRate(val.get());
      }

      if (boost::optional<double> val_ = autosizedCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity()) {
        setCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity(val.get());
      }

      if (boost::optional<double> val_ = autosizedEvaporativeCondenserPumpRatedPowerConsumption()) {
        setEvaporativeCondenserPumpRatedPowerConsumption(val.get());
      }
    }

    boost::optional<Schedule> CoilCoolingDXSingleSpeedThermalStorage_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::AvailabilitySchedule);
    }

    boost::optional<Connection> CoilCoolingDXSingleSpeedThermalStorage_Impl::optionalStorageTankAmbientTemperatureNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankAmbientTemperatureNode);
    }

    boost::optional<Connection> CoilCoolingDXSingleSpeedThermalStorage_Impl::optionalEvaporatorAirInletNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirInletNode);
    }

    boost::optional<Connection> CoilCoolingDXSingleSpeedThermalStorage_Impl::optionalEvaporatorAirOutletNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirOutletNode);
    }

    boost::optional<Connection> CoilCoolingDXSingleSpeedThermalStorage_Impl::optionalCondenserAirInletNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserAirInletNode);
    }

    boost::optional<Connection> CoilCoolingDXSingleSpeedThermalStorage_Impl::optionalCondenserAirOutletNode() const {
      return getObject<ModelObject>().getModelObjectTarget<Connection>(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserAirOutletNode);
    }

  }  // namespace detail

  CoilCoolingDXSingleSpeedThermalStorage::CoilCoolingDXSingleSpeedThermalStorage(const Model& model)
    : StraightComponent(CoilCoolingDXSingleSpeedThermalStorage::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>());

    // TODO: consider adding (overloaded or not) explicit ctors taking required objects as argument

    // TODO: Appropriately handle the following required object-list fields.
    //     OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::AvailabilitySchedule
    //     OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankAmbientTemperatureNode
    //     OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirInletNode
    //     OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirOutletNode
    //     OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserAirInletNode
    //     OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserAirOutletNode
    bool ok = true;
    // ok = setAvailabilitySchedule();
    OS_ASSERT(ok);
    // ok = setOperatingModeControlMethod();
    OS_ASSERT(ok);
    // ok = setStorageType();
    OS_ASSERT(ok);
    // ok = setStorageTankAmbientTemperatureNode();
    OS_ASSERT(ok);
    // ok = setStorageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient();
    OS_ASSERT(ok);
    // ok = setRatedEvaporatorAirFlowRate();
    OS_ASSERT(ok);
    // ok = setEvaporatorAirInletNode();
    OS_ASSERT(ok);
    // ok = setEvaporatorAirOutletNode();
    OS_ASSERT(ok);
    // setCoolingOnlyModeAvailable();
    // ok = setCoolingOnlyModeRatedSensibleHeatRatio();
    OS_ASSERT(ok);
    // ok = setCoolingOnlyModeRatedCOP();
    OS_ASSERT(ok);
    // setCoolingAndChargeModeAvailable();
    // setCoolingAndChargeModeCapacitySizingFactor();
    // setCoolingAndChargeModeStorageCapacitySizingFactor();
    // ok = setCoolingAndChargeModeRatedSensibleHeatRatio();
    OS_ASSERT(ok);
    // ok = setCoolingAndChargeModeCoolingRatedCOP();
    OS_ASSERT(ok);
    // ok = setCoolingAndChargeModeChargingRatedCOP();
    OS_ASSERT(ok);
    // setCoolingAndDischargeModeAvailable();
    // setCoolingAndDischargeModeEvaporatorCapacitySizingFactor();
    // setCoolingAndDischargeModeStorageDischargeCapacitySizingFactor();
    // ok = setCoolingAndDischargeModeRatedSensibleHeatRatio();
    OS_ASSERT(ok);
    // ok = setCoolingAndDischargeModeCoolingRatedCOP();
    OS_ASSERT(ok);
    // ok = setCoolingAndDischargeModeDischargingRatedCOP();
    OS_ASSERT(ok);
    // setChargeOnlyModeAvailable();
    // setChargeOnlyModeCapacitySizingFactor();
    // ok = setChargeOnlyModeChargingRatedCOP();
    OS_ASSERT(ok);
    // setDischargeOnlyModeAvailable();
    // setDischargeOnlyModeCapacitySizingFactor();
    // ok = setDischargeOnlyModeRatedCOP();
    OS_ASSERT(ok);
    // ok = setCondenserAirInletNode();
    OS_ASSERT(ok);
    // ok = setCondenserAirOutletNode();
    OS_ASSERT(ok);
    // ok = setCondenserDesignAirFlowRate();
    OS_ASSERT(ok);
    // setCondenserAirFlowSizingFactor();
    // ok = setCondenserType();
    OS_ASSERT(ok);
    // ok = setEvaporativeCondenserEffectiveness();
    OS_ASSERT(ok);
    // ok = setEvaporativeCondenserPumpRatedPowerConsumption();
    OS_ASSERT(ok);
    // ok = setBasinHeaterCapacity();
    OS_ASSERT(ok);
    // ok = setBasinHeaterSetpointTemperature();
    OS_ASSERT(ok);
    // ok = setStorageTankPlantConnectionHeatTransferEffectiveness();
    OS_ASSERT(ok);
  }

  IddObjectType CoilCoolingDXSingleSpeedThermalStorage::iddObjectType() {
    return {IddObjectType::OS_Coil_Cooling_DX_SingleSpeed_ThermalStorage};
  }

  std::vector<std::string> CoilCoolingDXSingleSpeedThermalStorage::operatingModeControlMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::OperatingModeControlMethod);
  }

  std::vector<std::string> CoilCoolingDXSingleSpeedThermalStorage::storageTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageType);
  }

  std::vector<std::string> CoilCoolingDXSingleSpeedThermalStorage::condenserTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserType);
  }

  Schedule CoilCoolingDXSingleSpeedThermalStorage::availabilitySchedule() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->availabilitySchedule();
  }

  std::string CoilCoolingDXSingleSpeedThermalStorage::operatingModeControlMethod() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->operatingModeControlMethod();
  }

  boost::optional<Schedule> CoilCoolingDXSingleSpeedThermalStorage::operationModeControlSchedule() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->operationModeControlSchedule();
  }

  std::string CoilCoolingDXSingleSpeedThermalStorage::storageType() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->storageType();
  }

  boost::optional<FluidAndGlycol> CoilCoolingDXSingleSpeedThermalStorage::userDefinedFluidType() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->userDefinedFluidType();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::fluidStorageVolume() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->fluidStorageVolume();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::isFluidStorageVolumeAutocalculated() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->isFluidStorageVolumeAutocalculated();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::iceStorageCapacity() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->iceStorageCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::isIceStorageCapacityAutocalculated() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->isIceStorageCapacityAutocalculated();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::storageCapacitySizingFactor() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->storageCapacitySizingFactor();
  }

  Connection CoilCoolingDXSingleSpeedThermalStorage::storageTankAmbientTemperatureNode() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->storageTankAmbientTemperatureNode();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::fluidStorageTankRatingTemperature() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->fluidStorageTankRatingTemperature();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::ratedEvaporatorAirFlowRate() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->ratedEvaporatorAirFlowRate();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::isRatedEvaporatorAirFlowRateAutosized() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->isRatedEvaporatorAirFlowRateAutosized();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::autosizedRatedEvaporatorAirFlowRate() {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizedRatedEvaporatorAirFlowRate();
  }

  Connection CoilCoolingDXSingleSpeedThermalStorage::evaporatorAirInletNode() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->evaporatorAirInletNode();
  }

  Connection CoilCoolingDXSingleSpeedThermalStorage::evaporatorAirOutletNode() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->evaporatorAirOutletNode();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::coolingOnlyModeAvailable() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingOnlyModeAvailable();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::coolingOnlyModeRatedTotalEvaporatorCoolingCapacity() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingOnlyModeRatedTotalEvaporatorCoolingCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::isCoolingOnlyModeRatedTotalEvaporatorCoolingCapacityAutosized() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->isCoolingOnlyModeRatedTotalEvaporatorCoolingCapacityAutosized();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::autosizedCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity() {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizedCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::coolingOnlyModeRatedSensibleHeatRatio() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingOnlyModeRatedSensibleHeatRatio();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::coolingOnlyModeRatedCOP() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingOnlyModeRatedCOP();
  }

  boost::optional<BivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();
  }

  boost::optional<UnivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();
  }

  boost::optional<BivariateFunctions> CoilCoolingDXSingleSpeedThermalStorage::coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve();
  }

  boost::optional<UnivariateFunctions> CoilCoolingDXSingleSpeedThermalStorage::coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve();
  }

  boost::optional<UnivariateFunctions> CoilCoolingDXSingleSpeedThermalStorage::coolingOnlyModePartLoadFractionCorrelationCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingOnlyModePartLoadFractionCorrelationCurve();
  }

  boost::optional<BivariateFunctions> CoilCoolingDXSingleSpeedThermalStorage::coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve();
  }

  boost::optional<UnivariateFunctions> CoilCoolingDXSingleSpeedThermalStorage::coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeAvailable() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeAvailable();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::isCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacityAutocalculated() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->isCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacityAutocalculated();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeCapacitySizingFactor() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeCapacitySizingFactor();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeRatedStorageChargingCapacity() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeRatedStorageChargingCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::isCoolingAndChargeModeRatedStorageChargingCapacityAutocalculated() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->isCoolingAndChargeModeRatedStorageChargingCapacityAutocalculated();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeStorageCapacitySizingFactor() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeStorageCapacitySizingFactor();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeRatedSensibleHeatRatio() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeRatedSensibleHeatRatio();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeCoolingRatedCOP() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeCoolingRatedCOP();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeChargingRatedCOP() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeChargingRatedCOP();
  }

  boost::optional<TrivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();
  }

  boost::optional<UnivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();
  }

  boost::optional<TrivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve();
  }

  boost::optional<UnivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve();
  }

  boost::optional<UnivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve();
  }

  boost::optional<TrivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve();
  }

  boost::optional<UnivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve();
  }

  boost::optional<TrivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve();
  }

  boost::optional<UnivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve();
  }

  boost::optional<UnivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve();
  }

  boost::optional<BivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve();
  }

  boost::optional<UnivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeAvailable() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndDischargeModeAvailable();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::isCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacityAutocalculated() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->isCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacityAutocalculated();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeEvaporatorCapacitySizingFactor() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndDischargeModeEvaporatorCapacitySizingFactor();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeRatedStorageDischargingCapacity() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndDischargeModeRatedStorageDischargingCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::isCoolingAndDischargeModeRatedStorageDischargingCapacityAutocalculated() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->isCoolingAndDischargeModeRatedStorageDischargingCapacityAutocalculated();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeStorageDischargeCapacitySizingFactor() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndDischargeModeStorageDischargeCapacitySizingFactor();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeRatedSensibleHeatRatio() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndDischargeModeRatedSensibleHeatRatio();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeCoolingRatedCOP() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndDischargeModeCoolingRatedCOP();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeDischargingRatedCOP() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndDischargeModeDischargingRatedCOP();
  }

  boost::optional<TrivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();
  }

  boost::optional<UnivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();
  }

  boost::optional<TrivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve();
  }

  boost::optional<UnivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve();
  }

  boost::optional<UnivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve();
  }

  boost::optional<TrivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve();
  }

  boost::optional<UnivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve();
  }

  boost::optional<UnivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve();
  }

  boost::optional<TrivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve();
  }

  boost::optional<UnivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve();
  }

  boost::optional<UnivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve();
  }

  boost::optional<BivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve();
  }

  boost::optional<UnivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::chargeOnlyModeAvailable() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->chargeOnlyModeAvailable();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::chargeOnlyModeRatedStorageChargingCapacity() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->chargeOnlyModeRatedStorageChargingCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::isChargeOnlyModeRatedStorageChargingCapacityAutocalculated() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->isChargeOnlyModeRatedStorageChargingCapacityAutocalculated();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::chargeOnlyModeCapacitySizingFactor() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->chargeOnlyModeCapacitySizingFactor();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::chargeOnlyModeChargingRatedCOP() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->chargeOnlyModeChargingRatedCOP();
  }

  boost::optional<BivariateFunctions> CoilCoolingDXSingleSpeedThermalStorage::chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve();
  }

  boost::optional<BivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModeAvailable() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModeAvailable();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModeRatedStorageDischargingCapacity() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModeRatedStorageDischargingCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::isDischargeOnlyModeRatedStorageDischargingCapacityAutocalculated() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->isDischargeOnlyModeRatedStorageDischargingCapacityAutocalculated();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModeCapacitySizingFactor() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModeCapacitySizingFactor();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModeRatedSensibleHeatRatio() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModeRatedSensibleHeatRatio();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModeRatedCOP() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModeRatedCOP();
  }

  boost::optional<BivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve();
  }

  boost::optional<UnivariateFunctions>
    CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve();
  }

  boost::optional<BivariateFunctions> CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve();
  }

  boost::optional<UnivariateFunctions> CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve();
  }

  boost::optional<UnivariateFunctions> CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModePartLoadFractionCorrelationCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModePartLoadFractionCorrelationCurve();
  }

  boost::optional<BivariateFunctions> CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve();
  }

  boost::optional<UnivariateFunctions> CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::ancillaryElectricPower() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->ancillaryElectricPower();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::coldWeatherOperationMinimumOutdoorAirTemperature() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coldWeatherOperationMinimumOutdoorAirTemperature();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::coldWeatherOperationAncillaryPower() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coldWeatherOperationAncillaryPower();
  }

  Connection CoilCoolingDXSingleSpeedThermalStorage::condenserAirInletNode() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->condenserAirInletNode();
  }

  Connection CoilCoolingDXSingleSpeedThermalStorage::condenserAirOutletNode() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->condenserAirOutletNode();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::condenserDesignAirFlowRate() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->condenserDesignAirFlowRate();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::isCondenserDesignAirFlowRateAutocalculated() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->isCondenserDesignAirFlowRateAutocalculated();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::condenserAirFlowSizingFactor() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->condenserAirFlowSizingFactor();
  }

  std::string CoilCoolingDXSingleSpeedThermalStorage::condenserType() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->condenserType();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::evaporativeCondenserEffectiveness() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->evaporativeCondenserEffectiveness();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::evaporativeCondenserPumpRatedPowerConsumption() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->evaporativeCondenserPumpRatedPowerConsumption();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::isEvaporativeCondenserPumpRatedPowerConsumptionAutosized() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->isEvaporativeCondenserPumpRatedPowerConsumptionAutosized();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::autosizedEvaporativeCondenserPumpRatedPowerConsumption() {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizedEvaporativeCondenserPumpRatedPowerConsumption();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::basinHeaterCapacity() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->basinHeaterCapacity();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::basinHeaterSetpointTemperature() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->basinHeaterSetpointTemperature();
  }

  boost::optional<Schedule> CoilCoolingDXSingleSpeedThermalStorage::basinHeaterAvailabilitySchedule() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->basinHeaterAvailabilitySchedule();
  }

  boost::optional<WaterStorageTank> CoilCoolingDXSingleSpeedThermalStorage::supplyWaterStorageTank() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->supplyWaterStorageTank();
  }

  boost::optional<WaterStorageTank> CoilCoolingDXSingleSpeedThermalStorage::condensateCollectionWaterStorageTank() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->condensateCollectionWaterStorageTank();
  }

  boost::optional<Connection> CoilCoolingDXSingleSpeedThermalStorage::storageTankPlantConnectionInletNode() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->storageTankPlantConnectionInletNode();
  }

  boost::optional<Connection> CoilCoolingDXSingleSpeedThermalStorage::storageTankPlantConnectionOutletNode() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->storageTankPlantConnectionOutletNode();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::storageTankPlantConnectionDesignFlowRate() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->storageTankPlantConnectionDesignFlowRate();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::storageTankPlantConnectionHeatTransferEffectiveness() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->storageTankPlantConnectionHeatTransferEffectiveness();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::storageTankMinimumOperatingLimitFluidTemperature() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->storageTankMinimumOperatingLimitFluidTemperature();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::storageTankMaximumOperatingLimitFluidTemperature() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->storageTankMaximumOperatingLimitFluidTemperature();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setOperatingModeControlMethod(const std::string& operatingModeControlMethod) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setOperatingModeControlMethod(operatingModeControlMethod);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setOperationModeControlSchedule(Schedule& schedule) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setOperationModeControlSchedule(schedule);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetOperationModeControlSchedule() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetOperationModeControlSchedule();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setStorageType(const std::string& storageType) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setStorageType(storageType);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setUserDefinedFluidType(const FluidAndGlycol& fluidAndGlycol) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setUserDefinedFluidType(fluidAndGlycol);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetUserDefinedFluidType() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetUserDefinedFluidType();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setFluidStorageVolume(double fluidStorageVolume) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setFluidStorageVolume(fluidStorageVolume);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetFluidStorageVolume() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetFluidStorageVolume();
  }

  void CoilCoolingDXSingleSpeedThermalStorage::autocalculateFluidStorageVolume() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autocalculateFluidStorageVolume();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setIceStorageCapacity(double iceStorageCapacity) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setIceStorageCapacity(iceStorageCapacity);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetIceStorageCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetIceStorageCapacity();
  }

  void CoilCoolingDXSingleSpeedThermalStorage::autocalculateIceStorageCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autocalculateIceStorageCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setStorageCapacitySizingFactor(double storageCapacitySizingFactor) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setStorageCapacitySizingFactor(storageCapacitySizingFactor);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetStorageCapacitySizingFactor() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetStorageCapacitySizingFactor();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setStorageTankAmbientTemperatureNode(const Connection& connection) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setStorageTankAmbientTemperatureNode(connection);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setStorageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient(
    double storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setStorageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient(
      storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setFluidStorageTankRatingTemperature(double fluidStorageTankRatingTemperature) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setFluidStorageTankRatingTemperature(fluidStorageTankRatingTemperature);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetFluidStorageTankRatingTemperature() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetFluidStorageTankRatingTemperature();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setRatedEvaporatorAirFlowRate(double ratedEvaporatorAirFlowRate) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setRatedEvaporatorAirFlowRate(ratedEvaporatorAirFlowRate);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::autosizeRatedEvaporatorAirFlowRate() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizeRatedEvaporatorAirFlowRate();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setEvaporatorAirInletNode(const Connection& connection) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setEvaporatorAirInletNode(connection);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setEvaporatorAirOutletNode(const Connection& connection) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setEvaporatorAirOutletNode(connection);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingOnlyModeAvailable(bool coolingOnlyModeAvailable) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingOnlyModeAvailable(coolingOnlyModeAvailable);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity(
    double coolingOnlyModeRatedTotalEvaporatorCoolingCapacity) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity(
      coolingOnlyModeRatedTotalEvaporatorCoolingCapacity);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity();
  }

  void CoilCoolingDXSingleSpeedThermalStorage::autosizeCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizeCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingOnlyModeRatedSensibleHeatRatio(double coolingOnlyModeRatedSensibleHeatRatio) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingOnlyModeRatedSensibleHeatRatio(
      coolingOnlyModeRatedSensibleHeatRatio);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingOnlyModeRatedCOP(double coolingOnlyModeRatedCOP) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingOnlyModeRatedCOP(coolingOnlyModeRatedCOP);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(
    const BivariateFunctions& bivariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(
      bivariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve(
    const BivariateFunctions& bivariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve(
      bivariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(
      univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingOnlyModePartLoadFractionCorrelationCurve(const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingOnlyModePartLoadFractionCorrelationCurve(univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingOnlyModePartLoadFractionCorrelationCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingOnlyModePartLoadFractionCorrelationCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(
    const BivariateFunctions& bivariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(
      bivariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(
      univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeAvailable(bool coolingAndChargeModeAvailable) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndChargeModeAvailable(coolingAndChargeModeAvailable);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity(
    double coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity(
      coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity();
  }

  void CoilCoolingDXSingleSpeedThermalStorage::autocalculateCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autocalculateCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeCapacitySizingFactor(double coolingAndChargeModeCapacitySizingFactor) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndChargeModeCapacitySizingFactor(
      coolingAndChargeModeCapacitySizingFactor);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeRatedStorageChargingCapacity(
    double coolingAndChargeModeRatedStorageChargingCapacity) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndChargeModeRatedStorageChargingCapacity(
      coolingAndChargeModeRatedStorageChargingCapacity);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeRatedStorageChargingCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndChargeModeRatedStorageChargingCapacity();
  }

  void CoilCoolingDXSingleSpeedThermalStorage::autocalculateCoolingAndChargeModeRatedStorageChargingCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autocalculateCoolingAndChargeModeRatedStorageChargingCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeStorageCapacitySizingFactor(
    double coolingAndChargeModeStorageCapacitySizingFactor) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndChargeModeStorageCapacitySizingFactor(
      coolingAndChargeModeStorageCapacitySizingFactor);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeRatedSensibleHeatRatio(double coolingAndChargeModeRatedSensibleHeatRatio) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndChargeModeRatedSensibleHeatRatio(
      coolingAndChargeModeRatedSensibleHeatRatio);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeCoolingRatedCOP(double coolingAndChargeModeCoolingRatedCOP) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndChargeModeCoolingRatedCOP(
      coolingAndChargeModeCoolingRatedCOP);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeChargingRatedCOP(double coolingAndChargeModeChargingRatedCOP) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndChargeModeChargingRatedCOP(
      coolingAndChargeModeChargingRatedCOP);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(
    const TrivariateFunctions& trivariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(trivariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->resetCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->resetCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(
    const TrivariateFunctions& trivariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(trivariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve(
      univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve(
    const TrivariateFunctions& trivariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve(
      trivariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve(univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(
    const TrivariateFunctions& trivariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(
      trivariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(
      univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve(
      univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve(
    const BivariateFunctions& bivariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve(
      bivariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve(
      univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeAvailable(bool coolingAndDischargeModeAvailable) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndDischargeModeAvailable(coolingAndDischargeModeAvailable);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity(
    double coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity(
      coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity();
  }

  void CoilCoolingDXSingleSpeedThermalStorage::autocalculateCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autocalculateCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeEvaporatorCapacitySizingFactor(
    double coolingAndDischargeModeEvaporatorCapacitySizingFactor) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndDischargeModeEvaporatorCapacitySizingFactor(
      coolingAndDischargeModeEvaporatorCapacitySizingFactor);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeRatedStorageDischargingCapacity(
    double coolingAndDischargeModeRatedStorageDischargingCapacity) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndDischargeModeRatedStorageDischargingCapacity(
      coolingAndDischargeModeRatedStorageDischargingCapacity);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeRatedStorageDischargingCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndDischargeModeRatedStorageDischargingCapacity();
  }

  void CoilCoolingDXSingleSpeedThermalStorage::autocalculateCoolingAndDischargeModeRatedStorageDischargingCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autocalculateCoolingAndDischargeModeRatedStorageDischargingCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeStorageDischargeCapacitySizingFactor(
    double coolingAndDischargeModeStorageDischargeCapacitySizingFactor) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndDischargeModeStorageDischargeCapacitySizingFactor(
      coolingAndDischargeModeStorageDischargeCapacitySizingFactor);
  }

  bool
    CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeRatedSensibleHeatRatio(double coolingAndDischargeModeRatedSensibleHeatRatio) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndDischargeModeRatedSensibleHeatRatio(
      coolingAndDischargeModeRatedSensibleHeatRatio);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeCoolingRatedCOP(double coolingAndDischargeModeCoolingRatedCOP) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndDischargeModeCoolingRatedCOP(
      coolingAndDischargeModeCoolingRatedCOP);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeDischargingRatedCOP(double coolingAndDischargeModeDischargingRatedCOP) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndDischargeModeDischargingRatedCOP(
      coolingAndDischargeModeDischargingRatedCOP);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(
    const TrivariateFunctions& trivariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(trivariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->resetCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->resetCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(
    const TrivariateFunctions& trivariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(trivariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->resetCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->resetCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve(
      univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve(
    const TrivariateFunctions& trivariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve(trivariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve(univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve(univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(
    const TrivariateFunctions& trivariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(trivariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve(
      univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve(
    const BivariateFunctions& bivariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve(
      bivariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve(
      univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setChargeOnlyModeAvailable(bool chargeOnlyModeAvailable) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setChargeOnlyModeAvailable(chargeOnlyModeAvailable);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setChargeOnlyModeRatedStorageChargingCapacity(double chargeOnlyModeRatedStorageChargingCapacity) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setChargeOnlyModeRatedStorageChargingCapacity(
      chargeOnlyModeRatedStorageChargingCapacity);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetChargeOnlyModeRatedStorageChargingCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetChargeOnlyModeRatedStorageChargingCapacity();
  }

  void CoilCoolingDXSingleSpeedThermalStorage::autocalculateChargeOnlyModeRatedStorageChargingCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autocalculateChargeOnlyModeRatedStorageChargingCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setChargeOnlyModeCapacitySizingFactor(double chargeOnlyModeCapacitySizingFactor) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setChargeOnlyModeCapacitySizingFactor(chargeOnlyModeCapacitySizingFactor);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setChargeOnlyModeChargingRatedCOP(double chargeOnlyModeChargingRatedCOP) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setChargeOnlyModeChargingRatedCOP(chargeOnlyModeChargingRatedCOP);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve(
    const BivariateFunctions& bivariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve(
      bivariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve(
    const BivariateFunctions& bivariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve(
      bivariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setDischargeOnlyModeAvailable(bool dischargeOnlyModeAvailable) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setDischargeOnlyModeAvailable(dischargeOnlyModeAvailable);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setDischargeOnlyModeRatedStorageDischargingCapacity(
    double dischargeOnlyModeRatedStorageDischargingCapacity) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setDischargeOnlyModeRatedStorageDischargingCapacity(
      dischargeOnlyModeRatedStorageDischargingCapacity);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetDischargeOnlyModeRatedStorageDischargingCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetDischargeOnlyModeRatedStorageDischargingCapacity();
  }

  void CoilCoolingDXSingleSpeedThermalStorage::autocalculateDischargeOnlyModeRatedStorageDischargingCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autocalculateDischargeOnlyModeRatedStorageDischargingCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setDischargeOnlyModeCapacitySizingFactor(double dischargeOnlyModeCapacitySizingFactor) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setDischargeOnlyModeCapacitySizingFactor(
      dischargeOnlyModeCapacitySizingFactor);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setDischargeOnlyModeRatedSensibleHeatRatio(double dischargeOnlyModeRatedSensibleHeatRatio) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setDischargeOnlyModeRatedSensibleHeatRatio(
      dischargeOnlyModeRatedSensibleHeatRatio);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetDischargeOnlyModeRatedSensibleHeatRatio() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetDischargeOnlyModeRatedSensibleHeatRatio();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setDischargeOnlyModeRatedCOP(double dischargeOnlyModeRatedCOP) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setDischargeOnlyModeRatedCOP(dischargeOnlyModeRatedCOP);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setDischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve(
    const BivariateFunctions& bivariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setDischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve(
      bivariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetDischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetDischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setDischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setDischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve(
      univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetDischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetDischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setDischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve(
    const BivariateFunctions& bivariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setDischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve(
      bivariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetDischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetDischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setDischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setDischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(
      univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetDischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetDischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setDischargeOnlyModePartLoadFractionCorrelationCurve(const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setDischargeOnlyModePartLoadFractionCorrelationCurve(univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetDischargeOnlyModePartLoadFractionCorrelationCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetDischargeOnlyModePartLoadFractionCorrelationCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setDischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(
    const BivariateFunctions& bivariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setDischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(
      bivariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetDischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetDischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setDischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(
    const UnivariateFunctions& univariateFunctions) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setDischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(
      univariateFunctions);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetDischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetDischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setAncillaryElectricPower(double ancillaryElectricPower) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setAncillaryElectricPower(ancillaryElectricPower);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetAncillaryElectricPower() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetAncillaryElectricPower();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setColdWeatherOperationMinimumOutdoorAirTemperature(
    double coldWeatherOperationMinimumOutdoorAirTemperature) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setColdWeatherOperationMinimumOutdoorAirTemperature(
      coldWeatherOperationMinimumOutdoorAirTemperature);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetColdWeatherOperationMinimumOutdoorAirTemperature() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetColdWeatherOperationMinimumOutdoorAirTemperature();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setColdWeatherOperationAncillaryPower(double coldWeatherOperationAncillaryPower) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setColdWeatherOperationAncillaryPower(coldWeatherOperationAncillaryPower);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetColdWeatherOperationAncillaryPower() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetColdWeatherOperationAncillaryPower();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCondenserAirInletNode(const Connection& connection) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCondenserAirInletNode(connection);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCondenserAirOutletNode(const Connection& connection) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCondenserAirOutletNode(connection);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCondenserDesignAirFlowRate(double condenserDesignAirFlowRate) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCondenserDesignAirFlowRate(condenserDesignAirFlowRate);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::autocalculateCondenserDesignAirFlowRate() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autocalculateCondenserDesignAirFlowRate();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCondenserAirFlowSizingFactor(double condenserAirFlowSizingFactor) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCondenserAirFlowSizingFactor(condenserAirFlowSizingFactor);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCondenserType(const std::string& condenserType) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCondenserType(condenserType);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setEvaporativeCondenserEffectiveness(evaporativeCondenserEffectiveness);
  }

  bool
    CoilCoolingDXSingleSpeedThermalStorage::setEvaporativeCondenserPumpRatedPowerConsumption(double evaporativeCondenserPumpRatedPowerConsumption) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setEvaporativeCondenserPumpRatedPowerConsumption(
      evaporativeCondenserPumpRatedPowerConsumption);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::autosizeEvaporativeCondenserPumpRatedPowerConsumption() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizeEvaporativeCondenserPumpRatedPowerConsumption();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setBasinHeaterCapacity(double basinHeaterCapacity) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setBasinHeaterCapacity(basinHeaterCapacity);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setBasinHeaterSetpointTemperature(basinHeaterSetpointTemperature);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setBasinHeaterAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setBasinHeaterAvailabilitySchedule(schedule);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetBasinHeaterAvailabilitySchedule() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetBasinHeaterAvailabilitySchedule();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setSupplyWaterStorageTank(const WaterStorageTank& waterStorageTank) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setSupplyWaterStorageTank(waterStorageTank);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetSupplyWaterStorageTank() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetSupplyWaterStorageTank();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCondensateCollectionWaterStorageTank(const WaterStorageTank& waterStorageTank) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCondensateCollectionWaterStorageTank(waterStorageTank);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCondensateCollectionWaterStorageTank() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCondensateCollectionWaterStorageTank();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setStorageTankPlantConnectionInletNode(const Connection& connection) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setStorageTankPlantConnectionInletNode(connection);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetStorageTankPlantConnectionInletNode() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetStorageTankPlantConnectionInletNode();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setStorageTankPlantConnectionOutletNode(const Connection& connection) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setStorageTankPlantConnectionOutletNode(connection);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetStorageTankPlantConnectionOutletNode() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetStorageTankPlantConnectionOutletNode();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setStorageTankPlantConnectionDesignFlowRate(double storageTankPlantConnectionDesignFlowRate) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setStorageTankPlantConnectionDesignFlowRate(
      storageTankPlantConnectionDesignFlowRate);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetStorageTankPlantConnectionDesignFlowRate() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetStorageTankPlantConnectionDesignFlowRate();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setStorageTankPlantConnectionHeatTransferEffectiveness(
    double storageTankPlantConnectionHeatTransferEffectiveness) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setStorageTankPlantConnectionHeatTransferEffectiveness(
      storageTankPlantConnectionHeatTransferEffectiveness);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setStorageTankMinimumOperatingLimitFluidTemperature(
    double storageTankMinimumOperatingLimitFluidTemperature) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setStorageTankMinimumOperatingLimitFluidTemperature(
      storageTankMinimumOperatingLimitFluidTemperature);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetStorageTankMinimumOperatingLimitFluidTemperature() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetStorageTankMinimumOperatingLimitFluidTemperature();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setStorageTankMaximumOperatingLimitFluidTemperature(
    double storageTankMaximumOperatingLimitFluidTemperature) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setStorageTankMaximumOperatingLimitFluidTemperature(
      storageTankMaximumOperatingLimitFluidTemperature);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetStorageTankMaximumOperatingLimitFluidTemperature() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetStorageTankMaximumOperatingLimitFluidTemperature();
  }

  /// @cond
  CoilCoolingDXSingleSpeedThermalStorage::CoilCoolingDXSingleSpeedThermalStorage(
    std::shared_ptr<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio