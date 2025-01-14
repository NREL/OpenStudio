/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "CoilCoolingDXSingleSpeedThermalStorage.hpp"
#include "CoilCoolingDXSingleSpeedThermalStorage_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "AirLoopHVACDedicatedOutdoorAirSystem.hpp"
#include "AirLoopHVACDedicatedOutdoorAirSystem_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"

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
      static const std::vector<std::string> result{
        "Cooling Coil Operating Mode Index",
        "Cooling Coil Total Cooling Rate",
        "Cooling Coil Total Cooling Energy",
        "Cooling Coil Sensible Cooling Rate",
        "Cooling Coil Sensible Cooling Energy",
        "Cooling Coil Latent Cooling Rate",
        "Cooling Coil Latent Cooling Energy",
        "Cooling Coil Electricity Rate",
        "Cooling Coil Electricity Energy",
        "Cooling Coil Runtime Fraction",
        "Cooling Coil Cold Weather Protection Electricity Energy",
        "Cooling Coil Cold Weather Protection Electricity Rate",
        "Cooling Coil Thermal Storage Mechanical Heat Transfer Rate",
        "Cooling Coil Thermal Storage Mechanical Heat Transfer Energy",
        "Cooling Coil Thermal Storage Ambient Heat Transfer Rate",
        "Cooling Coil Thermal Storage Ambient Heat Transfer Energy",
        "Cooling Coil Ice Thermal Storage End Fraction",
        "Cooling Coil Condenser Inlet Temperature",
        "Cooling Coil Evaporative Condenser Water Volume",
        "Cooling Coil Evaporative Condenser Mains Supply Water Volume",
        "Cooling Coil Evaporative Condenser Pump Electricity Rate",
        "Cooling Coil Evaporative Condenser Pump Electricity Energy",
        "Cooling Coil Basin Heater Electricity Rate",
        "Cooling Coil Basin Heater Electricity Energy",
        "Cooling Coil Thermal Storage Plant Heat Transfer Rate",
        "Cooling Coil Thermal Storage Plant Heat Transfer Energy",
        "Cooling Coil Fluid Thermal Storage End Temperature",
      };
      return result;
    }

    IddObjectType CoilCoolingDXSingleSpeedThermalStorage_Impl::iddObjectType() const {
      return CoilCoolingDXSingleSpeedThermalStorage::iddObjectType();
    }

    std::vector<ScheduleTypeKey> CoilCoolingDXSingleSpeedThermalStorage_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
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

    unsigned CoilCoolingDXSingleSpeedThermalStorage_Impl::inletPort() const {
      return OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirInletNode;
    }

    unsigned CoilCoolingDXSingleSpeedThermalStorage_Impl::outletPort() const {
      return OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirOutletNode;
    }

    boost::optional<HVACComponent> CoilCoolingDXSingleSpeedThermalStorage_Impl::containingHVACComponent() const {
      // AirLoopHVACUnitarySystem
      std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

      for (const auto& airLoopHVACUnitarySystem : airLoopHVACUnitarySystems) {
        if (boost::optional<HVACComponent> coolingCoil = airLoopHVACUnitarySystem.coolingCoil()) {
          if (coolingCoil->handle() == this->handle()) {
            return airLoopHVACUnitarySystem;
          }
        }
      }

      return boost::none;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::addToNode(Node& node) {
      if (boost::optional<AirLoopHVAC> airLoop = node.airLoopHVAC()) {
        if (!airLoop->demandComponent(node.handle())) {
          return StraightComponent_Impl::addToNode(node);
        }
      } else if (boost::optional<AirLoopHVACOutdoorAirSystem> oas = node.airLoopHVACOutdoorAirSystem()) {
        if (oas->airLoopHVACDedicatedOutdoorAirSystem()) {
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    std::vector<ModelObject> CoilCoolingDXSingleSpeedThermalStorage_Impl::children() const {
      std::vector<ModelObject> result;

      boost::optional<ModelObject> curve;

      curve = coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingOnlyModePartLoadFractionCorrelationCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = dischargeOnlyModePartLoadFractionCorrelationCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      curve = dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve();
      if (curve) {
        result.push_back(curve.get());
      }

      return result;
    }

    ComponentType CoilCoolingDXSingleSpeedThermalStorage_Impl::componentType() const {
      return ComponentType::Cooling;
    }

    std::vector<FuelType> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingFuelTypes() const {
      return {FuelType::Electricity};
    }

    std::vector<FuelType> CoilCoolingDXSingleSpeedThermalStorage_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> CoilCoolingDXSingleSpeedThermalStorage_Impl::appGHeatingFuelTypes() const {
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

    int CoilCoolingDXSingleSpeedThermalStorage_Impl::glycolConcentration() const {
      boost::optional<int> value = getInt(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::GlycolConcentration, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::fluidStorageVolume() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageVolume, true);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::isFluidStorageVolumeAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageVolume, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizedFluidStorageVolume() {
      return getAutosizedValue("Fluid Storage Volume", "m3");
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::iceStorageCapacity() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::IceStorageCapacity, true);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::isIceStorageCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::IceStorageCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizedIceStorageCapacity() {
      return getAutosizedValue("Ice Storage Capacity", "GJ");
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::storageCapacitySizingFactor() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageCapacitySizingFactor, true);
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient() const {
      boost::optional<double> value =
        getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::fluidStorageTankRatingTemperature() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageTankRatingTemperature, true);
      OS_ASSERT(value);
      return value.get();
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
      return getAutosizedValue("Rated Evaporator Air Flow Rate", "m3/s");
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
      return getAutosizedValue("Cooling Only Mode Rated Total Evaporator Cooling Capacity", "W");
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

    boost::optional<Curve>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve);
    }

    boost::optional<Curve>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve);
    }

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve);
    }

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve);
    }

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingOnlyModePartLoadFractionCorrelationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModePartLoadFractionCorrelationCurve);
    }

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve);
    }

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeAvailable() const {
      return getBooleanFieldValue(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeAvailable);
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity, true);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::isCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizedCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity() {
      return getAutosizedValue("Cooling And Charge Mode Rated Total Evaporator Cooling Capacity", "W");
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeCapacitySizingFactor() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeCapacitySizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeRatedStorageChargingCapacity() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedStorageChargingCapacity, true);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::isCoolingAndChargeModeRatedStorageChargingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedStorageChargingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizedCoolingAndChargeModeRatedStorageChargingCapacity() {
      return getAutosizedValue("Cooling And Charge Mode Rated Storage Charging Capacity", "W");
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

    boost::optional<Curve>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve);
    }

    boost::optional<Curve>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve);
    }

    boost::optional<Curve>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve);
    }

    boost::optional<Curve>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve);
    }

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve);
    }

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve);
    }

    boost::optional<Curve>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve);
    }

    boost::optional<Curve>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve);
    }

    boost::optional<Curve>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve);
    }

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve);
    }

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve);
    }

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeAvailable() const {
      return getBooleanFieldValue(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeAvailable);
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity, true);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::isCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizedCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity() {
      return getAutosizedValue("Cooling And Discharge Mode Rated Total Evaporator Cooling Capacity", "W");
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

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::isCoolingAndDischargeModeRatedStorageDischargingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedStorageDischargingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizedCoolingAndDischargeModeRatedStorageDischargingCapacity() {
      return getAutosizedValue("Cooling And Discharge Mode Rated Storage Discharging Capacity", "W");
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

    boost::optional<Curve>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve);
    }

    boost::optional<Curve>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve);
    }

    boost::optional<Curve>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve);
    }

    boost::optional<Curve>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve);
    }

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve);
    }

    boost::optional<Curve>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve);
    }

    boost::optional<Curve>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve);
    }

    boost::optional<Curve>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve);
    }

    boost::optional<Curve>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve);
    }

    boost::optional<Curve>
      CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve);
    }

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve);
    }

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve);
    }

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::chargeOnlyModeAvailable() const {
      return getBooleanFieldValue(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeAvailable);
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::chargeOnlyModeRatedStorageChargingCapacity() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeRatedStorageChargingCapacity, true);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::isChargeOnlyModeRatedStorageChargingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeRatedStorageChargingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizedChargeOnlyModeRatedStorageChargingCapacity() {
      return getAutosizedValue("Charge Only Mode Rated Storage Charging Capacity", "W");
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

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve);
    }

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModeAvailable() const {
      return getBooleanFieldValue(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeAvailable);
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModeRatedStorageDischargingCapacity() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedStorageDischargingCapacity, true);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::isDischargeOnlyModeRatedStorageDischargingCapacityAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedStorageDischargingCapacity, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizedDischargeOnlyModeRatedStorageDischargingCapacity() {
      return getAutosizedValue("Discharge Only Mode Rated Storage Discharging Capacity", "W");
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModeCapacitySizingFactor() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeCapacitySizingFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModeRatedSensibleHeatRatio() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedSensibleHeatRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    double CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModeRatedCOP() const {
      boost::optional<double> value = getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedCOP, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve);
    }

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve);
    }

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve);
    }

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve);
    }

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModePartLoadFractionCorrelationCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModePartLoadFractionCorrelationCurve);
    }

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve);
    }

    boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage_Impl::dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(
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

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::condenserDesignAirFlowRate() const {
      return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserDesignAirFlowRate, true);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::isCondenserDesignAirFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserDesignAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizedCondenserDesignAirFlowRate() {
      return getAutosizedValue("Condenser Air Flow Rate", "m3/s");
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
      return getAutosizedValue("Evaporative Condenser Pump Rated Power Consumption", "W");
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

    // boost::optional<WaterStorageTank> CoilCoolingDXSingleSpeedThermalStorage_Impl::supplyWaterStorageTank() const {
    // return getObject<ModelObject>().getModelObjectTarget<WaterStorageTank>(
    // OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::SupplyWaterStorageTank);
    // }

    // boost::optional<WaterStorageTank> CoilCoolingDXSingleSpeedThermalStorage_Impl::condensateCollectionWaterStorageTank() const {
    // return getObject<ModelObject>().getModelObjectTarget<WaterStorageTank>(
    // OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondensateCollectionWaterStorageTank);
    // }

    // boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::storageTankPlantConnectionDesignFlowRate() const {
    // return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionDesignFlowRate, true);
    // }

    // double CoilCoolingDXSingleSpeedThermalStorage_Impl::storageTankPlantConnectionHeatTransferEffectiveness() const {
    // boost::optional<double> value =
    // getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionHeatTransferEffectiveness, true);
    // OS_ASSERT(value);
    // return value.get();
    // }

    // boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::storageTankMinimumOperatingLimitFluidTemperature() const {
    // return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankMinimumOperatingLimitFluidTemperature, true);
    // }

    // boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage_Impl::storageTankMaximumOperatingLimitFluidTemperature() const {
    // return getDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankMaximumOperatingLimitFluidTemperature, true);
    // }

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

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setGlycolConcentration(int glycolConcentration) {
      const bool result = setInt(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::GlycolConcentration, glycolConcentration);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setFluidStorageVolume(double fluidStorageVolume) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageVolume, fluidStorageVolume);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetFluidStorageVolume() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageVolume, "");
      OS_ASSERT(result);
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizeFluidStorageVolume() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::FluidStorageVolume, "autosize");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setIceStorageCapacity(double iceStorageCapacity) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::IceStorageCapacity, iceStorageCapacity);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizeIceStorageCapacity() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::IceStorageCapacity, "autosize");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setStorageCapacitySizingFactor(double storageCapacitySizingFactor) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageCapacitySizingFactor, storageCapacitySizingFactor);
      OS_ASSERT(result);
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

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setRatedEvaporatorAirFlowRate(double ratedEvaporatorAirFlowRate) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::RatedEvaporatorAirFlowRate, ratedEvaporatorAirFlowRate);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizeRatedEvaporatorAirFlowRate() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::RatedEvaporatorAirFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingOnlyModeAvailable(bool coolingOnlyModeAvailable) {
      return setBooleanFieldValue(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeAvailable, coolingOnlyModeAvailable);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity(
      double coolingOnlyModeRatedTotalEvaporatorCoolingCapacity) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeRatedTotalEvaporatorCoolingCapacity,
                                    coolingOnlyModeRatedTotalEvaporatorCoolingCapacity);
      return result;
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

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(const Curve& Curve) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool
      CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(const Curve& Curve) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve,
                   Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingOnlyModePartLoadFractionCorrelationCurve(const Curve& Curve) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModePartLoadFractionCorrelationCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingOnlyModePartLoadFractionCorrelationCurve() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModePartLoadFractionCorrelationCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(const Curve& Curve) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(const Curve& Curve) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeAvailable(bool coolingAndChargeModeAvailable) {
      return setBooleanFieldValue(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeAvailable, coolingAndChargeModeAvailable);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity(
      double coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity,
                                    coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizeCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity, "autosize");
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

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizeCoolingAndChargeModeRatedStorageChargingCapacity() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeRatedStorageChargingCapacity, "autosize");
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
      const Curve& Curve) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve,
                   Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() {
      const bool result = setString(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(
      const Curve& Curve) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve,
                   Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() {
      const bool result = setString(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool
      CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve,
                   Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool
      CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve,
                   Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve(const Curve& Curve) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve(const Curve& Curve) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool
      CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve(const Curve& Curve) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve,
                   Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve() {
      const bool result = setString(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve(const Curve& Curve) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve(const Curve& Curve) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve(const Curve& Curve) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeAvailable(bool coolingAndDischargeModeAvailable) {
      return setBooleanFieldValue(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeAvailable,
                                  coolingAndDischargeModeAvailable);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity(
      double coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity,
                                    coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizeCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity, "autosize");
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

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizeCoolingAndDischargeModeRatedStorageDischargingCapacity() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeRatedStorageDischargingCapacity, "autosize");
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
      const Curve& Curve) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve,
        Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() {
      const bool result = setString(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(
      const Curve& Curve) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve,
        Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() {
      const bool result = setString(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(
      const Curve& Curve) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve,
                   Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve() {
      const bool result = setString(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(
      const Curve& Curve) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve,
                   Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve() {
      const bool result = setString(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve(const Curve& Curve) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve, "");
      OS_ASSERT(result);
    }

    bool
      CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve(const Curve& Curve) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve,
                   Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool
      CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve(const Curve& Curve) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve,
                   Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve() {
      const bool result = setString(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve(
      const Curve& Curve) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve,
        Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve() {
      const bool result = setString(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve, "");
      OS_ASSERT(result);
    }

    bool
      CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve,
                   Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool
      CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve,
                   Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve(const Curve& Curve) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve(const Curve& Curve) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve(const Curve& Curve) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setChargeOnlyModeAvailable(bool chargeOnlyModeAvailable) {
      return setBooleanFieldValue(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeAvailable, chargeOnlyModeAvailable);
    }

    bool
      CoilCoolingDXSingleSpeedThermalStorage_Impl::setChargeOnlyModeRatedStorageChargingCapacity(double chargeOnlyModeRatedStorageChargingCapacity) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeRatedStorageChargingCapacity,
                                    chargeOnlyModeRatedStorageChargingCapacity);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizeChargeOnlyModeRatedStorageChargingCapacity() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeRatedStorageChargingCapacity, "autosize");
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

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve(const Curve& Curve) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::ChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setDischargeOnlyModeAvailable(bool dischargeOnlyModeAvailable) {
      return setBooleanFieldValue(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeAvailable, dischargeOnlyModeAvailable);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setDischargeOnlyModeRatedStorageDischargingCapacity(
      double dischargeOnlyModeRatedStorageDischargingCapacity) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedStorageDischargingCapacity,
                                    dischargeOnlyModeRatedStorageDischargingCapacity);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizeDischargeOnlyModeRatedStorageDischargingCapacity() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedStorageDischargingCapacity, "autosize");
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

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setDischargeOnlyModeRatedCOP(double dischargeOnlyModeRatedCOP) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeRatedCOP, dischargeOnlyModeRatedCOP);
      return result;
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setDischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve(const Curve& Curve) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetDischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setDischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve(const Curve& Curve) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetDischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setDischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetDischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setDischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetDischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setDischargeOnlyModePartLoadFractionCorrelationCurve(const Curve& Curve) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModePartLoadFractionCorrelationCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetDischargeOnlyModePartLoadFractionCorrelationCurve() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModePartLoadFractionCorrelationCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setDischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(const Curve& Curve) {
      const bool result =
        setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve, Curve.handle());
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetDischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve() {
      const bool result =
        setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve, "");
      OS_ASSERT(result);
    }

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setDischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(const Curve& Curve) {
      const bool result = setPointer(
        OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::DischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve, Curve.handle());
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

    bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCondenserDesignAirFlowRate(double condenserDesignAirFlowRate) {
      const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserDesignAirFlowRate, condenserDesignAirFlowRate);
      return result;
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autosizeCondenserDesignAirFlowRate() {
      const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondenserDesignAirFlowRate, "autosize");
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

    // bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setSupplyWaterStorageTank(const WaterStorageTank& waterStorageTank) {
    // const bool result = setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::SupplyWaterStorageTank, waterStorageTank.handle());
    // return result;
    // }

    // void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetSupplyWaterStorageTank() {
    // const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::SupplyWaterStorageTank, "");
    // OS_ASSERT(result);
    // }

    // bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setCondensateCollectionWaterStorageTank(const WaterStorageTank& waterStorageTank) {
    // const bool result =
    // setPointer(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondensateCollectionWaterStorageTank, waterStorageTank.handle());
    // return result;
    // }

    // void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetCondensateCollectionWaterStorageTank() {
    // const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::CondensateCollectionWaterStorageTank, "");
    // OS_ASSERT(result);
    // }

    // bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setStorageTankPlantConnectionDesignFlowRate(double storageTankPlantConnectionDesignFlowRate) {
    // const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionDesignFlowRate,
    // storageTankPlantConnectionDesignFlowRate);
    // return result;
    // }

    // void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetStorageTankPlantConnectionDesignFlowRate() {
    // const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionDesignFlowRate, "");
    // OS_ASSERT(result);
    // }

    // bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setStorageTankPlantConnectionHeatTransferEffectiveness(
    // double storageTankPlantConnectionHeatTransferEffectiveness) {
    // const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankPlantConnectionHeatTransferEffectiveness,
    // storageTankPlantConnectionHeatTransferEffectiveness);
    // return result;
    // }

    // bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setStorageTankMinimumOperatingLimitFluidTemperature(
    // double storageTankMinimumOperatingLimitFluidTemperature) {
    // const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankMinimumOperatingLimitFluidTemperature,
    // storageTankMinimumOperatingLimitFluidTemperature);
    // OS_ASSERT(result);
    // return result;
    // }

    // void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetStorageTankMinimumOperatingLimitFluidTemperature() {
    // const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankMinimumOperatingLimitFluidTemperature, "");
    // OS_ASSERT(result);
    // }

    // bool CoilCoolingDXSingleSpeedThermalStorage_Impl::setStorageTankMaximumOperatingLimitFluidTemperature(
    // double storageTankMaximumOperatingLimitFluidTemperature) {
    // const bool result = setDouble(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankMaximumOperatingLimitFluidTemperature,
    // storageTankMaximumOperatingLimitFluidTemperature);
    // OS_ASSERT(result);
    // return result;
    // }

    // void CoilCoolingDXSingleSpeedThermalStorage_Impl::resetStorageTankMaximumOperatingLimitFluidTemperature() {
    // const bool result = setString(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::StorageTankMaximumOperatingLimitFluidTemperature, "");
    // OS_ASSERT(result);
    // }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::autosize() {
      autosizeRatedEvaporatorAirFlowRate();
      autosizeCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity();
      autosizeEvaporativeCondenserPumpRatedPowerConsumption();
    }

    void CoilCoolingDXSingleSpeedThermalStorage_Impl::applySizingValues() {
      if (boost::optional<double> val_ = autosizedRatedEvaporatorAirFlowRate()) {
        setRatedEvaporatorAirFlowRate(val_.get());
      }

      if (boost::optional<double> val_ = autosizedCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity()) {
        setCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity(val_.get());
      }

      if (boost::optional<double> val_ = autosizedEvaporativeCondenserPumpRatedPowerConsumption()) {
        setEvaporativeCondenserPumpRatedPowerConsumption(val_.get());
      }
    }

    boost::optional<Schedule> CoilCoolingDXSingleSpeedThermalStorage_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::AvailabilitySchedule);
    }

  }  // namespace detail

  CoilCoolingDXSingleSpeedThermalStorage::CoilCoolingDXSingleSpeedThermalStorage(const Model& model)
    : StraightComponent(CoilCoolingDXSingleSpeedThermalStorage::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>());

    bool ok = true;
    auto schedule_on = model.alwaysOnDiscreteSchedule();
    ok = setAvailabilitySchedule(schedule_on);
    OS_ASSERT(ok);
    ok = setOperatingModeControlMethod("ScheduledModes");  // intentionally NOT setting an operation mode control schedule
    OS_ASSERT(ok);
    ok = setStorageType("Ice");  // RetailPackagedTESCoil.idf
    OS_ASSERT(ok);
    ok = setGlycolConcentration(0);
    OS_ASSERT(ok);
    autosizeFluidStorageVolume();
    autosizeIceStorageCapacity();
    ok = setStorageCapacitySizingFactor(6.0);  // RetailPackagedTESCoil.idf
    OS_ASSERT(ok);
    ok = setStorageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient(7.913);  // RetailPackagedTESCoil.idf
    OS_ASSERT(ok);
    ok = setFluidStorageTankRatingTemperature(26.7);
    OS_ASSERT(ok);
    autosizeRatedEvaporatorAirFlowRate();

    // Cooling Only Mode
    ok = setCoolingOnlyModeAvailable(false);
    OS_ASSERT(ok);
    autosizeCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity();
    ok = setCoolingOnlyModeRatedSensibleHeatRatio(0.7);  // IDD default
    OS_ASSERT(ok);
    ok = setCoolingOnlyModeRatedCOP(3.0);  // IDD default
    OS_ASSERT(ok);

    // Cooling And Charge Mode
    ok = setCoolingAndChargeModeAvailable(false);
    OS_ASSERT(ok);
    autosizeCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity();
    ok = setCoolingAndChargeModeCapacitySizingFactor(0.5);  // IDD default
    OS_ASSERT(ok);
    autosizeCoolingAndChargeModeRatedStorageChargingCapacity();
    ok = setCoolingAndChargeModeStorageCapacitySizingFactor(0.5);  // IDD default
    OS_ASSERT(ok);
    ok = setCoolingAndChargeModeRatedSensibleHeatRatio(0.7);  // IDD default
    OS_ASSERT(ok);
    ok = setCoolingAndChargeModeCoolingRatedCOP(3.0);  // IDD default
    OS_ASSERT(ok);
    ok = setCoolingAndChargeModeChargingRatedCOP(3.0);  // IDD default
    OS_ASSERT(ok);

    // Cooling And Discharge Mode
    ok = setCoolingAndDischargeModeAvailable(false);
    OS_ASSERT(ok);
    autosizeCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity();
    ok = setCoolingAndDischargeModeEvaporatorCapacitySizingFactor(1.0);  // IDD default
    OS_ASSERT(ok);
    autosizeCoolingAndDischargeModeRatedStorageDischargingCapacity();
    ok = setCoolingAndDischargeModeStorageDischargeCapacitySizingFactor(1.0);  // IDD default
    OS_ASSERT(ok);
    ok = setCoolingAndDischargeModeRatedSensibleHeatRatio(0.7);  // IDD default
    OS_ASSERT(ok);
    ok = setCoolingAndDischargeModeCoolingRatedCOP(3.0);  // IDD default
    OS_ASSERT(ok);
    ok = setCoolingAndDischargeModeDischargingRatedCOP(3.0);  // IDD default
    OS_ASSERT(ok);

    // Charge Only Mode
    ok = setChargeOnlyModeAvailable(false);
    OS_ASSERT(ok);
    autosizeChargeOnlyModeRatedStorageChargingCapacity();
    ok = setChargeOnlyModeCapacitySizingFactor(1.0);  // IDD default
    OS_ASSERT(ok);
    ok = setChargeOnlyModeChargingRatedCOP(3.0);  // IDD default
    OS_ASSERT(ok);

    // Discharge Only Mode
    ok = setDischargeOnlyModeAvailable(false);
    OS_ASSERT(ok);
    autosizeDischargeOnlyModeRatedStorageDischargingCapacity();
    ok = setDischargeOnlyModeCapacitySizingFactor(1.0);  // IDD default
    OS_ASSERT(ok);
    ok = setDischargeOnlyModeRatedSensibleHeatRatio(0.7);  // Assumed; default missing in IDD
    OS_ASSERT(ok);
    ok = setDischargeOnlyModeRatedCOP(3.0);  // IDD default
    OS_ASSERT(ok);

    ok = setAncillaryElectricPower(0.0);
    OS_ASSERT(ok);
    ok = setColdWeatherOperationAncillaryPower(0.0);
    OS_ASSERT(ok);
    autosizeCondenserDesignAirFlowRate();
    ok = setCondenserAirFlowSizingFactor(1.0);  // IDD default
    OS_ASSERT(ok);
    ok = setCondenserType("AirCooled");  // IDD default
    OS_ASSERT(ok);
    ok = setEvaporativeCondenserEffectiveness(0.7);  // IDD default
    OS_ASSERT(ok);
    ok = setEvaporativeCondenserPumpRatedPowerConsumption(0.0);  // IDD default
    OS_ASSERT(ok);
    ok = setBasinHeaterCapacity(0.0);  // IDD default
    OS_ASSERT(ok);
    ok = setBasinHeaterSetpointTemperature(2.0);  // IDD default
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

  int CoilCoolingDXSingleSpeedThermalStorage::glycolConcentration() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->glycolConcentration();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::fluidStorageVolume() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->fluidStorageVolume();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::isFluidStorageVolumeAutosized() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->isFluidStorageVolumeAutosized();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::autosizedFluidStorageVolume() {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizedFluidStorageVolume();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::iceStorageCapacity() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->iceStorageCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::isIceStorageCapacityAutosized() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->isIceStorageCapacityAutosized();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::autosizedIceStorageCapacity() {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizedIceStorageCapacity();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::storageCapacitySizingFactor() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->storageCapacitySizingFactor();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::fluidStorageTankRatingTemperature() const {
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

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingOnlyModePartLoadFractionCorrelationCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingOnlyModePartLoadFractionCorrelationCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeAvailable() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeAvailable();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeRatedTotalEvaporatorCoolingCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::isCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacityAutosized() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->isCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacityAutosized();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::autosizedCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity() {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizedCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeCapacitySizingFactor() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeCapacitySizingFactor();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeRatedStorageChargingCapacity() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeRatedStorageChargingCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::isCoolingAndChargeModeRatedStorageChargingCapacityAutosized() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->isCoolingAndChargeModeRatedStorageChargingCapacityAutosized();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::autosizedCoolingAndChargeModeRatedStorageChargingCapacity() {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizedCoolingAndChargeModeRatedStorageChargingCapacity();
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

  boost::optional<Curve>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();
  }

  boost::optional<Curve>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeAvailable() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndDischargeModeAvailable();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::isCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacityAutosized() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->isCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacityAutosized();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::autosizedCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity() {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizedCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeEvaporatorCapacitySizingFactor() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndDischargeModeEvaporatorCapacitySizingFactor();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeRatedStorageDischargingCapacity() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndDischargeModeRatedStorageDischargingCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::isCoolingAndDischargeModeRatedStorageDischargingCapacityAutosized() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->isCoolingAndDischargeModeRatedStorageDischargingCapacityAutosized();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::autosizedCoolingAndDischargeModeRatedStorageDischargingCapacity() {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizedCoolingAndDischargeModeRatedStorageDischargingCapacity();
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

  boost::optional<Curve>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();
  }

  boost::optional<Curve>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve();
  }

  boost::optional<Curve>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve();
  }

  boost::optional<Curve>
    CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->coolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->coolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::chargeOnlyModeAvailable() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->chargeOnlyModeAvailable();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::chargeOnlyModeRatedStorageChargingCapacity() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->chargeOnlyModeRatedStorageChargingCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::isChargeOnlyModeRatedStorageChargingCapacityAutosized() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->isChargeOnlyModeRatedStorageChargingCapacityAutosized();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::autosizedChargeOnlyModeRatedStorageChargingCapacity() {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizedChargeOnlyModeRatedStorageChargingCapacity();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::chargeOnlyModeCapacitySizingFactor() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->chargeOnlyModeCapacitySizingFactor();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::chargeOnlyModeChargingRatedCOP() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->chargeOnlyModeChargingRatedCOP();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->chargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->chargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModeAvailable() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModeAvailable();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModeRatedStorageDischargingCapacity() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModeRatedStorageDischargingCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::isDischargeOnlyModeRatedStorageDischargingCapacityAutosized() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->isDischargeOnlyModeRatedStorageDischargingCapacityAutosized();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::autosizedDischargeOnlyModeRatedStorageDischargingCapacity() {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizedDischargeOnlyModeRatedStorageDischargingCapacity();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModeCapacitySizingFactor() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModeCapacitySizingFactor();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModeRatedSensibleHeatRatio() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModeRatedSensibleHeatRatio();
  }

  double CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModeRatedCOP() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModeRatedCOP();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModePartLoadFractionCorrelationCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModePartLoadFractionCorrelationCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->dischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve();
  }

  boost::optional<Curve> CoilCoolingDXSingleSpeedThermalStorage::dischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve() const {
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

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::condenserDesignAirFlowRate() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->condenserDesignAirFlowRate();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::isCondenserDesignAirFlowRateAutosized() const {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->isCondenserDesignAirFlowRateAutosized();
  }

  boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::autosizedCondenserDesignAirFlowRate() {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizedCondenserDesignAirFlowRate();
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

  // boost::optional<WaterStorageTank> CoilCoolingDXSingleSpeedThermalStorage::supplyWaterStorageTank() const {
  // return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->supplyWaterStorageTank();
  // }

  // boost::optional<WaterStorageTank> CoilCoolingDXSingleSpeedThermalStorage::condensateCollectionWaterStorageTank() const {
  // return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->condensateCollectionWaterStorageTank();
  // }

  // boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::storageTankPlantConnectionDesignFlowRate() const {
  // return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->storageTankPlantConnectionDesignFlowRate();
  // }

  // double CoilCoolingDXSingleSpeedThermalStorage::storageTankPlantConnectionHeatTransferEffectiveness() const {
  // return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->storageTankPlantConnectionHeatTransferEffectiveness();
  // }

  // boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::storageTankMinimumOperatingLimitFluidTemperature() const {
  // return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->storageTankMinimumOperatingLimitFluidTemperature();
  // }

  // boost::optional<double> CoilCoolingDXSingleSpeedThermalStorage::storageTankMaximumOperatingLimitFluidTemperature() const {
  // return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->storageTankMaximumOperatingLimitFluidTemperature();
  // }

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

  bool CoilCoolingDXSingleSpeedThermalStorage::setGlycolConcentration(int glycolConcentration) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setGlycolConcentration(glycolConcentration);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setFluidStorageVolume(double fluidStorageVolume) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setFluidStorageVolume(fluidStorageVolume);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetFluidStorageVolume() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetFluidStorageVolume();
  }

  void CoilCoolingDXSingleSpeedThermalStorage::autosizeFluidStorageVolume() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizeFluidStorageVolume();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setIceStorageCapacity(double iceStorageCapacity) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setIceStorageCapacity(iceStorageCapacity);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::autosizeIceStorageCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizeIceStorageCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setStorageCapacitySizingFactor(double storageCapacitySizingFactor) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setStorageCapacitySizingFactor(storageCapacitySizingFactor);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setStorageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient(
    double storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setStorageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient(
      storageTanktoAmbientUvalueTimesAreaHeatTransferCoefficient);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setFluidStorageTankRatingTemperature(double fluidStorageTankRatingTemperature) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setFluidStorageTankRatingTemperature(fluidStorageTankRatingTemperature);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setRatedEvaporatorAirFlowRate(double ratedEvaporatorAirFlowRate) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setRatedEvaporatorAirFlowRate(ratedEvaporatorAirFlowRate);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::autosizeRatedEvaporatorAirFlowRate() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizeRatedEvaporatorAirFlowRate();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingOnlyModeAvailable(bool coolingOnlyModeAvailable) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingOnlyModeAvailable(coolingOnlyModeAvailable);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity(
    double coolingOnlyModeRatedTotalEvaporatorCoolingCapacity) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingOnlyModeRatedTotalEvaporatorCoolingCapacity(
      coolingOnlyModeRatedTotalEvaporatorCoolingCapacity);
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

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(
      Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingOnlyModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingOnlyModeEnergyInputRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingOnlyModeEnergyInputRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingOnlyModePartLoadFractionCorrelationCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingOnlyModePartLoadFractionCorrelationCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingOnlyModePartLoadFractionCorrelationCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingOnlyModePartLoadFractionCorrelationCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingOnlyModeSensibleHeatRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(Curve);
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

  void CoilCoolingDXSingleSpeedThermalStorage::autosizeCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizeCoolingAndChargeModeRatedTotalEvaporatorCoolingCapacity();
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

  void CoilCoolingDXSingleSpeedThermalStorage::autosizeCoolingAndChargeModeRatedStorageChargingCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizeCoolingAndChargeModeRatedStorageChargingCapacity();
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

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->resetCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->resetCoolingAndChargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndChargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndChargeModeEvaporatorPartLoadFractionCorrelationCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve(
      Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndChargeModeStorageChargeCapacityFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndChargeModeStorageChargeCapacityFunctionofTotalEvaporatorPLRCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(
      Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndChargeModeStorageEnergyInputRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(
      Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndChargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve(
      Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndChargeModeStorageEnergyPartLoadFractionCorrelationCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndChargeModeSensibleHeatRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndChargeModeSensibleHeatRatioFunctionofFlowFractionCurve(Curve);
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

  void CoilCoolingDXSingleSpeedThermalStorage::autosizeCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizeCoolingAndDischargeModeRatedTotalEvaporatorCoolingCapacity();
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

  void CoilCoolingDXSingleSpeedThermalStorage::autosizeCoolingAndDischargeModeRatedStorageDischargingCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizeCoolingAndDischargeModeRatedStorageDischargingCapacity();
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

  bool
    CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->resetCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofTemperatureCurve();
  }

  bool
    CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->resetCoolingAndDischargeModeTotalEvaporatorCoolingCapacityFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->resetCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->resetCoolingAndDischargeModeEvaporatorEnergyInputRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve(
      Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndDischargeModeEvaporatorPartLoadFractionCorrelationCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofFlowFractionCurve();
  }

  bool
    CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->resetCoolingAndDischargeModeStorageDischargeCapacityFunctionofTotalEvaporatorPLRCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndDischargeModeStorageEnergyInputRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()
      ->setCoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndDischargeModeStorageEnergyInputRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve(
      Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndDischargeModeStorageEnergyPartLoadFractionCorrelationCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve(
      Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetCoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCoolingAndDischargeModeSensibleHeatRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setCoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCoolingAndDischargeModeSensibleHeatRatioFunctionofFlowFractionCurve(
      Curve);
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

  void CoilCoolingDXSingleSpeedThermalStorage::autosizeChargeOnlyModeRatedStorageChargingCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizeChargeOnlyModeRatedStorageChargingCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setChargeOnlyModeCapacitySizingFactor(double chargeOnlyModeCapacitySizingFactor) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setChargeOnlyModeCapacitySizingFactor(chargeOnlyModeCapacitySizingFactor);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setChargeOnlyModeChargingRatedCOP(double chargeOnlyModeChargingRatedCOP) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setChargeOnlyModeChargingRatedCOP(chargeOnlyModeChargingRatedCOP);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetChargeOnlyModeStorageChargeCapacityFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setChargeOnlyModeStorageEnergyInputRatioFunctionofTemperatureCurve(Curve);
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

  void CoilCoolingDXSingleSpeedThermalStorage::autosizeDischargeOnlyModeRatedStorageDischargingCapacity() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizeDischargeOnlyModeRatedStorageDischargingCapacity();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setDischargeOnlyModeCapacitySizingFactor(double dischargeOnlyModeCapacitySizingFactor) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setDischargeOnlyModeCapacitySizingFactor(
      dischargeOnlyModeCapacitySizingFactor);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setDischargeOnlyModeRatedSensibleHeatRatio(double dischargeOnlyModeRatedSensibleHeatRatio) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setDischargeOnlyModeRatedSensibleHeatRatio(
      dischargeOnlyModeRatedSensibleHeatRatio);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setDischargeOnlyModeRatedCOP(double dischargeOnlyModeRatedCOP) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setDischargeOnlyModeRatedCOP(dischargeOnlyModeRatedCOP);
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setDischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setDischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve(
      Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetDischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetDischargeOnlyModeStorageDischargeCapacityFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setDischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setDischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve(
      Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetDischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetDischargeOnlyModeStorageDischargeCapacityFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setDischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setDischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetDischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetDischargeOnlyModeEnergyInputRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setDischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setDischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetDischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetDischargeOnlyModeEnergyInputRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setDischargeOnlyModePartLoadFractionCorrelationCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setDischargeOnlyModePartLoadFractionCorrelationCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetDischargeOnlyModePartLoadFractionCorrelationCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetDischargeOnlyModePartLoadFractionCorrelationCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setDischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setDischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetDischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetDischargeOnlyModeSensibleHeatRatioFunctionofTemperatureCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setDischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(const Curve& Curve) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setDischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve(Curve);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::resetDischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetDischargeOnlyModeSensibleHeatRatioFunctionofFlowFractionCurve();
  }

  bool CoilCoolingDXSingleSpeedThermalStorage::setAncillaryElectricPower(double ancillaryElectricPower) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setAncillaryElectricPower(ancillaryElectricPower);
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

  bool CoilCoolingDXSingleSpeedThermalStorage::setCondenserDesignAirFlowRate(double condenserDesignAirFlowRate) {
    return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCondenserDesignAirFlowRate(condenserDesignAirFlowRate);
  }

  void CoilCoolingDXSingleSpeedThermalStorage::autosizeCondenserDesignAirFlowRate() {
    getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->autosizeCondenserDesignAirFlowRate();
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

  // bool CoilCoolingDXSingleSpeedThermalStorage::setSupplyWaterStorageTank(const WaterStorageTank& waterStorageTank) {
  // return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setSupplyWaterStorageTank(waterStorageTank);
  // }

  // void CoilCoolingDXSingleSpeedThermalStorage::resetSupplyWaterStorageTank() {
  // getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetSupplyWaterStorageTank();
  // }

  // bool CoilCoolingDXSingleSpeedThermalStorage::setCondensateCollectionWaterStorageTank(const WaterStorageTank& waterStorageTank) {
  // return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setCondensateCollectionWaterStorageTank(waterStorageTank);
  // }

  // void CoilCoolingDXSingleSpeedThermalStorage::resetCondensateCollectionWaterStorageTank() {
  // getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetCondensateCollectionWaterStorageTank();
  // }

  // bool CoilCoolingDXSingleSpeedThermalStorage::setStorageTankPlantConnectionDesignFlowRate(double storageTankPlantConnectionDesignFlowRate) {
  // return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setStorageTankPlantConnectionDesignFlowRate(
  // storageTankPlantConnectionDesignFlowRate);
  // }

  // void CoilCoolingDXSingleSpeedThermalStorage::resetStorageTankPlantConnectionDesignFlowRate() {
  // getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetStorageTankPlantConnectionDesignFlowRate();
  // }

  // bool CoilCoolingDXSingleSpeedThermalStorage::setStorageTankPlantConnectionHeatTransferEffectiveness(
  // double storageTankPlantConnectionHeatTransferEffectiveness) {
  // return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setStorageTankPlantConnectionHeatTransferEffectiveness(
  // storageTankPlantConnectionHeatTransferEffectiveness);
  // }

  // bool CoilCoolingDXSingleSpeedThermalStorage::setStorageTankMinimumOperatingLimitFluidTemperature(
  // double storageTankMinimumOperatingLimitFluidTemperature) {
  // return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setStorageTankMinimumOperatingLimitFluidTemperature(
  // storageTankMinimumOperatingLimitFluidTemperature);
  // }

  // void CoilCoolingDXSingleSpeedThermalStorage::resetStorageTankMinimumOperatingLimitFluidTemperature() {
  // getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetStorageTankMinimumOperatingLimitFluidTemperature();
  // }

  // bool CoilCoolingDXSingleSpeedThermalStorage::setStorageTankMaximumOperatingLimitFluidTemperature(
  // double storageTankMaximumOperatingLimitFluidTemperature) {
  // return getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->setStorageTankMaximumOperatingLimitFluidTemperature(
  // storageTankMaximumOperatingLimitFluidTemperature);
  // }

  // void CoilCoolingDXSingleSpeedThermalStorage::resetStorageTankMaximumOperatingLimitFluidTemperature() {
  // getImpl<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl>()->resetStorageTankMaximumOperatingLimitFluidTemperature();
  // }

  /// @cond
  CoilCoolingDXSingleSpeedThermalStorage::CoilCoolingDXSingleSpeedThermalStorage(
    std::shared_ptr<detail::CoilCoolingDXSingleSpeedThermalStorage_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
