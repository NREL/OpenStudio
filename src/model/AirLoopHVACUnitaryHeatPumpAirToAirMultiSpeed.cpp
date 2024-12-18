/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "Node.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl(const IdfObject& idfObject,
                                                                                                         Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::iddObjectType());
    }

    AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::iddObjectType());
    }

    AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl(
      const AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::iddObjectType() const {
      return AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::iddObjectType();
    }

    std::vector<ScheduleTypeKey> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::AvailabilitySchedule) != e) {
        result.push_back(ScheduleTypeKey("AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed", "Availability Schedule"));
      }
      if (std::find(b, e, OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplyAirFanOperatingModeSchedule) != e) {
        result.push_back(ScheduleTypeKey("AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed", "Supply Air Fan Operating Mode Schedule"));
      }
      return result;
    }

    boost::optional<Schedule> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::availabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::AvailabilitySchedule);
    }

    boost::optional<ThermalZone> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::controllingZoneorThermostatLocation() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(
        OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::ControllingZoneorThermostatLocation);
    }

    HVACComponent AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::supplyAirFan() const {
      boost::optional<HVACComponent> value = optionalSupplyAirFan();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Supply Air Fan attached.");
      }
      return value.get();
    }

    std::string AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::supplyAirFanPlacement() const {
      boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplyAirFanPlacement, true);
      OS_ASSERT(value);
      return value.get();
    }

    Schedule AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::supplyAirFanOperatingModeSchedule() const {
      auto value = getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplyAirFanOperatingModeSchedule);
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Air Fan Operating Mode Schedule attached.");
      }
      return value.get();
    }

    HVACComponent AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::heatingCoil() const {
      boost::optional<HVACComponent> value = optionalHeatingCoil();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Heating Coil attached.");
      }
      return value.get();
    }

    double AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::minimumOutdoorDryBulbTemperatureforCompressorOperation() const {
      boost::optional<double> value =
        getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    HVACComponent AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::coolingCoil() const {
      boost::optional<HVACComponent> value = optionalCoolingCoil();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Cooling Coil attached.");
      }
      return value.get();
    }

    HVACComponent AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::supplementalHeatingCoil() const {
      auto value = optionalSupplementalHeatingCoil();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a Supplemental Heating Coil attached.");
      }
      return value.get();
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::maximumSupplyAirTemperaturefromSupplementalHeater() const {
      return getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::MaximumSupplyAirTemperaturefromSupplementalHeater, true);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::MaximumSupplyAirTemperaturefromSupplementalHeater, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const {
      boost::optional<double> value =
        getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::auxiliaryOnCycleElectricPower() const {
      boost::optional<double> value = getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::AuxiliaryOnCycleElectricPower, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::auxiliaryOffCycleElectricPower() const {
      boost::optional<double> value = getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::AuxiliaryOffCycleElectricPower, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::designHeatRecoveryWaterFlowRate() const {
      boost::optional<double> value = getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::DesignHeatRecoveryWaterFlowRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::maximumTemperatureforHeatRecovery() const {
      boost::optional<double> value = getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::MaximumTemperatureforHeatRecovery, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
      return getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    int AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::numberofSpeedsforHeating() const {
      boost::optional<int> value = getInt(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::NumberofSpeedsforHeating, true);
      OS_ASSERT(value);
      return value.get();
    }

    int AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::numberofSpeedsforCooling() const {
      boost::optional<int> value = getInt(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::NumberofSpeedsforCooling, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::speed1SupplyAirFlowRateDuringHeatingOperation() const {
      return getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed1SupplyAirFlowRateDuringHeatingOperation, true);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::isSpeed1SupplyAirFlowRateDuringHeatingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed1SupplyAirFlowRateDuringHeatingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::speed2SupplyAirFlowRateDuringHeatingOperation() const {
      return getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed2SupplyAirFlowRateDuringHeatingOperation, true);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::isSpeed2SupplyAirFlowRateDuringHeatingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed2SupplyAirFlowRateDuringHeatingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::speed3SupplyAirFlowRateDuringHeatingOperation() const {
      return getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed3SupplyAirFlowRateDuringHeatingOperation, true);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::isSpeed3SupplyAirFlowRateDuringHeatingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed3SupplyAirFlowRateDuringHeatingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::speed4SupplyAirFlowRateDuringHeatingOperation() const {
      return getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed4SupplyAirFlowRateDuringHeatingOperation, true);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::isSpeed4SupplyAirFlowRateDuringHeatingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed4SupplyAirFlowRateDuringHeatingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::speed1SupplyAirFlowRateDuringCoolingOperation() const {
      return getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed1SupplyAirFlowRateDuringCoolingOperation, true);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::isSpeed1SupplyAirFlowRateDuringCoolingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed1SupplyAirFlowRateDuringCoolingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::speed2SupplyAirFlowRateDuringCoolingOperation() const {
      return getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed2SupplyAirFlowRateDuringCoolingOperation, true);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::isSpeed2SupplyAirFlowRateDuringCoolingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed2SupplyAirFlowRateDuringCoolingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::speed3SupplyAirFlowRateDuringCoolingOperation() const {
      return getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed3SupplyAirFlowRateDuringCoolingOperation, true);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::isSpeed3SupplyAirFlowRateDuringCoolingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed3SupplyAirFlowRateDuringCoolingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::speed4SupplyAirFlowRateDuringCoolingOperation() const {
      return getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed4SupplyAirFlowRateDuringCoolingOperation, true);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::isSpeed4SupplyAirFlowRateDuringCoolingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed4SupplyAirFlowRateDuringCoolingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::AvailabilitySchedule,
                                "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed", "Availability Schedule", schedule);
      return result;
    }

    void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::resetAvailabilitySchedule() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::AvailabilitySchedule, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setControllingZoneorThermostatLocation(const boost::optional<ThermalZone>& thermalZone) {
      bool result(false);
      if (thermalZone) {
        result =
          setPointer(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::ControllingZoneorThermostatLocation, thermalZone.get().handle());
      } else {
        resetControllingZoneorThermostatLocation();
        result = true;
      }
      return result;
    }

    void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::resetControllingZoneorThermostatLocation() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::ControllingZoneorThermostatLocation, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setSupplyAirFan(const HVACComponent& fan) {
      bool result = setPointer(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplyAirFan, fan.handle());
      return result;
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setSupplyAirFanPlacement(const std::string& supplyAirFanPlacement) {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplyAirFanPlacement, supplyAirFanPlacement);
      return result;
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setSupplyAirFanOperatingModeSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplyAirFanOperatingModeSchedule,
                                "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed", "Supply Air Fan Operating Mode Schedule", schedule);
      return result;
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setHeatingCoil(const HVACComponent& coil) {
      bool result = setPointer(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::HeatingCoil, coil.handle());
      return result;
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setMinimumOutdoorDryBulbTemperatureforCompressorOperation(
      double minimumOutdoorDryBulbTemperatureforCompressorOperation) {
      bool result = setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation,
                              minimumOutdoorDryBulbTemperatureforCompressorOperation);
      return result;
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setCoolingCoil(const HVACComponent& coil) {
      bool result = setPointer(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::CoolingCoil, coil.handle());
      return result;
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setSupplementalHeatingCoil(const HVACComponent& coil) {
      bool result = setPointer(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplementalHeatingCoil, coil.handle());
      return result;
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setMaximumSupplyAirTemperaturefromSupplementalHeater(
      boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater) {
      bool result(false);
      if (maximumSupplyAirTemperaturefromSupplementalHeater) {
        result = setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::MaximumSupplyAirTemperaturefromSupplementalHeater,
                           maximumSupplyAirTemperaturefromSupplementalHeater.get());
      }
      OS_ASSERT(result);
      return result;
    }

    void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::autosizeMaximumSupplyAirTemperaturefromSupplementalHeater() {
      bool result =
        setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::MaximumSupplyAirTemperaturefromSupplementalHeater, "autosize");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(
      double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation) {
      bool result =
        setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation,
                  maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
      return result;
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setAuxiliaryOnCycleElectricPower(double auxiliaryOnCycleElectricPower) {
      bool result = setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::AuxiliaryOnCycleElectricPower, auxiliaryOnCycleElectricPower);
      return result;
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setAuxiliaryOffCycleElectricPower(double auxiliaryOffCycleElectricPower) {
      bool result =
        setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::AuxiliaryOffCycleElectricPower, auxiliaryOffCycleElectricPower);
      return result;
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setDesignHeatRecoveryWaterFlowRate(double designHeatRecoveryWaterFlowRate) {
      bool result =
        setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::DesignHeatRecoveryWaterFlowRate, designHeatRecoveryWaterFlowRate);
      return result;
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setMaximumTemperatureforHeatRecovery(double maximumTemperatureforHeatRecovery) {
      bool result =
        setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::MaximumTemperatureforHeatRecovery, maximumTemperatureforHeatRecovery);
      return result;
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(
      boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded) {
      bool result(false);
      if (supplyAirFlowRateWhenNoCoolingorHeatingisNeeded) {
        result = setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded,
                           supplyAirFlowRateWhenNoCoolingorHeatingisNeeded.get());
      }
      return result;
    }

    void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, "autosize");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setNumberofSpeedsforHeating(int numberofSpeedsforHeating) {
      bool result = setInt(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::NumberofSpeedsforHeating, numberofSpeedsforHeating);
      return result;
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setNumberofSpeedsforCooling(int numberofSpeedsforCooling) {
      bool result = setInt(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::NumberofSpeedsforCooling, numberofSpeedsforCooling);
      return result;
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setSpeed1SupplyAirFlowRateDuringHeatingOperation(
      boost::optional<double> speed1SupplyAirFlowRateDuringHeatingOperation) {
      bool result(false);
      if (speed1SupplyAirFlowRateDuringHeatingOperation) {
        result = setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed1SupplyAirFlowRateDuringHeatingOperation,
                           speed1SupplyAirFlowRateDuringHeatingOperation.get());
      }
      return result;
    }

    void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::autosizeSpeed1SupplyAirFlowRateDuringHeatingOperation() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed1SupplyAirFlowRateDuringHeatingOperation, "autosize");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setSpeed2SupplyAirFlowRateDuringHeatingOperation(
      boost::optional<double> speed2SupplyAirFlowRateDuringHeatingOperation) {
      bool result(false);
      if (speed2SupplyAirFlowRateDuringHeatingOperation) {
        result = setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed2SupplyAirFlowRateDuringHeatingOperation,
                           speed2SupplyAirFlowRateDuringHeatingOperation.get());
      }
      return result;
    }

    void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::autosizeSpeed2SupplyAirFlowRateDuringHeatingOperation() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed2SupplyAirFlowRateDuringHeatingOperation, "autosize");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setSpeed3SupplyAirFlowRateDuringHeatingOperation(
      boost::optional<double> speed3SupplyAirFlowRateDuringHeatingOperation) {
      bool result(false);
      if (speed3SupplyAirFlowRateDuringHeatingOperation) {
        result = setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed3SupplyAirFlowRateDuringHeatingOperation,
                           speed3SupplyAirFlowRateDuringHeatingOperation.get());
      }
      return result;
    }

    void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::autosizeSpeed3SupplyAirFlowRateDuringHeatingOperation() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed3SupplyAirFlowRateDuringHeatingOperation, "autosize");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setSpeed4SupplyAirFlowRateDuringHeatingOperation(
      boost::optional<double> speed4SupplyAirFlowRateDuringHeatingOperation) {
      bool result(false);
      if (speed4SupplyAirFlowRateDuringHeatingOperation) {
        result = setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed4SupplyAirFlowRateDuringHeatingOperation,
                           speed4SupplyAirFlowRateDuringHeatingOperation.get());
      }
      return result;
    }

    void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::autosizeSpeed4SupplyAirFlowRateDuringHeatingOperation() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed4SupplyAirFlowRateDuringHeatingOperation, "autosize");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setSpeed1SupplyAirFlowRateDuringCoolingOperation(
      boost::optional<double> speed1SupplyAirFlowRateDuringCoolingOperation) {
      bool result(false);
      if (speed1SupplyAirFlowRateDuringCoolingOperation) {
        result = setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed1SupplyAirFlowRateDuringCoolingOperation,
                           speed1SupplyAirFlowRateDuringCoolingOperation.get());
      }
      return result;
    }

    void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::autosizeSpeed1SupplyAirFlowRateDuringCoolingOperation() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed1SupplyAirFlowRateDuringCoolingOperation, "autosize");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setSpeed2SupplyAirFlowRateDuringCoolingOperation(
      boost::optional<double> speed2SupplyAirFlowRateDuringCoolingOperation) {
      bool result(false);
      if (speed2SupplyAirFlowRateDuringCoolingOperation) {
        result = setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed2SupplyAirFlowRateDuringCoolingOperation,
                           speed2SupplyAirFlowRateDuringCoolingOperation.get());
      }
      return result;
    }

    void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::autosizeSpeed2SupplyAirFlowRateDuringCoolingOperation() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed2SupplyAirFlowRateDuringCoolingOperation, "autosize");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setSpeed3SupplyAirFlowRateDuringCoolingOperation(
      boost::optional<double> speed3SupplyAirFlowRateDuringCoolingOperation) {
      bool result(false);
      if (speed3SupplyAirFlowRateDuringCoolingOperation) {
        result = setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed3SupplyAirFlowRateDuringCoolingOperation,
                           speed3SupplyAirFlowRateDuringCoolingOperation.get());
      }
      return result;
    }

    void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::autosizeSpeed3SupplyAirFlowRateDuringCoolingOperation() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed3SupplyAirFlowRateDuringCoolingOperation, "autosize");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::setSpeed4SupplyAirFlowRateDuringCoolingOperation(
      boost::optional<double> speed4SupplyAirFlowRateDuringCoolingOperation) {
      bool result(false);
      if (speed4SupplyAirFlowRateDuringCoolingOperation) {
        result = setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed4SupplyAirFlowRateDuringCoolingOperation,
                           speed4SupplyAirFlowRateDuringCoolingOperation.get());
      }
      return result;
    }

    void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::autosizeSpeed4SupplyAirFlowRateDuringCoolingOperation() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed4SupplyAirFlowRateDuringCoolingOperation, "autosize");
      OS_ASSERT(result);
    }

    boost::optional<HVACComponent> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::optionalSupplyAirFan() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplyAirFan);
    }

    boost::optional<HVACComponent> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::optionalHeatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::HeatingCoil);
    }

    boost::optional<HVACComponent> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::optionalCoolingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::CoolingCoil);
    }

    boost::optional<HVACComponent> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::optionalSupplementalHeatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(
        OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplementalHeatingCoil);
    }

    unsigned AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::inletPort() const {
      return OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::AirInletNode;
    }

    unsigned AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::outletPort() const {
      return OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::AirOutletNode;
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::addToNode(Node& node) {
      if (boost::optional<AirLoopHVAC> airLoop = node.airLoopHVAC()) {
        if (airLoop->supplyComponent(node.handle())) {
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    ModelObject AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::clone(Model model) const {
      auto modelObjectClone = StraightComponent_Impl::clone(model).cast<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed>();

      if (boost::optional<HVACComponent> supplyFan = this->supplyAirFan()) {
        modelObjectClone.setSupplyAirFan(supplyFan->clone(model).cast<HVACComponent>());
      }
      if (boost::optional<HVACComponent> coolingCoil = this->coolingCoil()) {
        modelObjectClone.setCoolingCoil(coolingCoil->clone(model).cast<HVACComponent>());
      }
      if (boost::optional<HVACComponent> heatingCoil = this->heatingCoil()) {
        modelObjectClone.setHeatingCoil(heatingCoil->clone(model).cast<HVACComponent>());
      }
      if (boost::optional<HVACComponent> supplementalHeatingCoil = this->supplementalHeatingCoil()) {
        modelObjectClone.setSupplementalHeatingCoil(supplementalHeatingCoil->clone(model).cast<HVACComponent>());
      }

      return std::move(modelObjectClone);
    }

    std::vector<ModelObject> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::children() const {
      std::vector<ModelObject> result;

      // Avoid crashing when calling remove() in Ctor when failing to set one of the child component by calling the optional one
      if (boost::optional<HVACComponent> supplyFan = this->optionalSupplyAirFan()) {
        result.push_back(std::move(*supplyFan));
      }
      if (boost::optional<HVACComponent> coolingCoil = this->optionalCoolingCoil()) {
        result.push_back(std::move(*coolingCoil));
      }
      if (boost::optional<HVACComponent> heatingCoil = this->optionalHeatingCoil()) {
        result.push_back(std::move(*heatingCoil));
      }
      if (boost::optional<HVACComponent> supplementalHeatingCoil = this->optionalSupplementalHeatingCoil()) {
        result.push_back(std::move(*supplementalHeatingCoil));
      }

      return result;
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::autosizedMaximumSupplyAirTemperaturefromSupplementalHeater() const {
      return getAutosizedValue("Maximum Supply Air Temperature from Supplemental Heater", "C");
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
      return getAutosizedValue("Supply Air Flow Rate When No Cooling or Heating is Needed", "m3/s");
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::autosizedSpeed1SupplyAirFlowRateDuringHeatingOperation() const {
      return getAutosizedValue("Speed1Supply Air Flow Rate During Heating Operation", "m3/s");
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::autosizedSpeed2SupplyAirFlowRateDuringHeatingOperation() const {
      return getAutosizedValue("Speed2Supply Air Flow Rate During Heating Operation", "m3/s");
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::autosizedSpeed3SupplyAirFlowRateDuringHeatingOperation() const {
      return getAutosizedValue("Speed3Supply Air Flow Rate During Heating Operation", "m3/s");
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::autosizedSpeed4SupplyAirFlowRateDuringHeatingOperation() const {
      return getAutosizedValue("Speed4Supply Air Flow Rate During Heating Operation", "m3/s");
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::autosizedSpeed1SupplyAirFlowRateDuringCoolingOperation() const {
      return getAutosizedValue("Speed 1 Supply Air Flow Rate During Cooling Operation", "m3/s");
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::autosizedSpeed2SupplyAirFlowRateDuringCoolingOperation() const {
      return getAutosizedValue("Speed 2 Supply Air Flow Rate During Cooling Operation", "m3/s");
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::autosizedSpeed3SupplyAirFlowRateDuringCoolingOperation() const {
      return getAutosizedValue("Speed 3 Supply Air Flow Rate During Cooling Operation", "m3/s");
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::autosizedSpeed4SupplyAirFlowRateDuringCoolingOperation() const {
      return getAutosizedValue("Speed 4 Supply Air Flow Rate During Cooling Operation", "m3/s");
    }

    void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::autosize() {
      autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();
      autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
      autosizeSpeed1SupplyAirFlowRateDuringHeatingOperation();
      autosizeSpeed2SupplyAirFlowRateDuringHeatingOperation();
      autosizeSpeed3SupplyAirFlowRateDuringHeatingOperation();
      autosizeSpeed4SupplyAirFlowRateDuringHeatingOperation();
      autosizeSpeed1SupplyAirFlowRateDuringCoolingOperation();
      autosizeSpeed2SupplyAirFlowRateDuringCoolingOperation();
      autosizeSpeed3SupplyAirFlowRateDuringCoolingOperation();
      autosizeSpeed4SupplyAirFlowRateDuringCoolingOperation();
    }

    void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedMaximumSupplyAirTemperaturefromSupplementalHeater();
      if (val) {
        setMaximumSupplyAirTemperaturefromSupplementalHeater(val.get());
      }

      val = autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
      if (val) {
        setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(val.get());
      }

      val = autosizedSpeed1SupplyAirFlowRateDuringHeatingOperation();
      if (val) {
        setSpeed1SupplyAirFlowRateDuringHeatingOperation(val.get());
      }

      val = autosizedSpeed2SupplyAirFlowRateDuringHeatingOperation();
      if (val) {
        setSpeed2SupplyAirFlowRateDuringHeatingOperation(val.get());
      }

      val = autosizedSpeed3SupplyAirFlowRateDuringHeatingOperation();
      if (val) {
        setSpeed3SupplyAirFlowRateDuringHeatingOperation(val.get());
      }

      val = autosizedSpeed4SupplyAirFlowRateDuringHeatingOperation();
      if (val) {
        setSpeed4SupplyAirFlowRateDuringHeatingOperation(val.get());
      }

      val = autosizedSpeed1SupplyAirFlowRateDuringCoolingOperation();
      if (val) {
        setSpeed1SupplyAirFlowRateDuringCoolingOperation(val.get());
      }

      val = autosizedSpeed2SupplyAirFlowRateDuringCoolingOperation();
      if (val) {
        setSpeed2SupplyAirFlowRateDuringCoolingOperation(val.get());
      }

      val = autosizedSpeed3SupplyAirFlowRateDuringCoolingOperation();
      if (val) {
        setSpeed3SupplyAirFlowRateDuringCoolingOperation(val.get());
      }

      val = autosizedSpeed4SupplyAirFlowRateDuringCoolingOperation();
      if (val) {
        setSpeed4SupplyAirFlowRateDuringCoolingOperation(val.get());
      }
    }

    std::vector<EMSActuatorNames> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"AirLoopHVAC:UnitaryHeatPump:AirToAir", "Autosized Supply Air Flow Rate"},
                                              {"Unitary HVAC", "Sensible Load Request"},
                                              {"Unitary HVAC", "Moisture Load Request"}};
      return actuators;
    }

    std::vector<std::string> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types{"Unitary HVAC Design Heating Capacity", "Unitary HVAC Design Cooling Capacity"};
      return types;
    }

    ComponentType AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::componentType() const {

      return ComponentType::Both;
    }

    std::vector<FuelType> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::coolingFuelTypes() const {
      return coolingCoil().coolingFuelTypes();
    }

    std::vector<FuelType> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::heatingFuelTypes() const {
      std::set<FuelType> result;
      for (auto ft : heatingCoil().heatingFuelTypes()) {
        result.insert(ft);
      }
      for (auto ft : supplementalHeatingCoil().heatingFuelTypes()) {
        result.insert(ft);
      }
      return {result.begin(), result.end()};
    }

    std::vector<AppGFuelType> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl::appGHeatingFuelTypes() const {
      std::set<AppGFuelType> result;
      for (auto ft : heatingCoil().appGHeatingFuelTypes()) {
        result.insert(ft);
      }
      for (auto ft : supplementalHeatingCoil().appGHeatingFuelTypes()) {
        result.insert(ft);
      }
      return {result.begin(), result.end()};
    }

  }  // namespace detail

  AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed(const Model& model, const HVACComponent& fan,
                                                                                             const HVACComponent& heatingCoil,
                                                                                             const HVACComponent& coolingCoil,
                                                                                             const HVACComponent& supplementalHeatingCoil)
    : StraightComponent(AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>());

    bool ok = setSupplyAirFan(fan);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s supply fan to " << fan.briefDescription() << ".");
    }

    ok = setHeatingCoil(heatingCoil);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s heating coil to " << heatingCoil.briefDescription() << ".");
    }

    ok = setCoolingCoil(coolingCoil);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s cooling coil to " << coolingCoil.briefDescription() << ".");
    }

    ok = setSupplementalHeatingCoil(supplementalHeatingCoil);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s supplemental heating coil to " << supplementalHeatingCoil.briefDescription()
                                     << ".");
    }

    setSupplyAirFanPlacement("DrawThrough");
    setMinimumOutdoorDryBulbTemperatureforCompressorOperation(-8.0);
    setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(21.0);

    autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();
    setAuxiliaryOnCycleElectricPower(0.0);
    setAuxiliaryOffCycleElectricPower(0.0);
    setDesignHeatRecoveryWaterFlowRate(0.0);
    setMaximumTemperatureforHeatRecovery(80.0);
    autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
    setNumberofSpeedsforHeating(2);
    setNumberofSpeedsforCooling(2);
    autosizeSpeed1SupplyAirFlowRateDuringHeatingOperation();
    autosizeSpeed2SupplyAirFlowRateDuringHeatingOperation();
    autosizeSpeed3SupplyAirFlowRateDuringHeatingOperation();
    autosizeSpeed4SupplyAirFlowRateDuringHeatingOperation();
    autosizeSpeed1SupplyAirFlowRateDuringCoolingOperation();
    autosizeSpeed2SupplyAirFlowRateDuringCoolingOperation();
    autosizeSpeed3SupplyAirFlowRateDuringCoolingOperation();
    autosizeSpeed4SupplyAirFlowRateDuringCoolingOperation();
    auto schedule = model.alwaysOnDiscreteSchedule();
    setSupplyAirFanOperatingModeSchedule(schedule);
  }

  IddObjectType AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::iddObjectType() {
    return {IddObjectType::OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeed};
  }

  std::vector<std::string> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::supplyAirFanPlacementValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplyAirFanPlacement);
  }

  boost::optional<Schedule> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::availabilitySchedule() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->availabilitySchedule();
  }

  boost::optional<ThermalZone> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::controllingZoneorThermostatLocation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->controllingZoneorThermostatLocation();
  }

  HVACComponent AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::supplyAirFan() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->supplyAirFan();
  }

  std::string AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::supplyAirFanPlacement() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->supplyAirFanPlacement();
  }

  Schedule AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::supplyAirFanOperatingModeSchedule() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->supplyAirFanOperatingModeSchedule();
  }

  HVACComponent AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::heatingCoil() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->heatingCoil();
  }

  double AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::minimumOutdoorDryBulbTemperatureforCompressorOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->minimumOutdoorDryBulbTemperatureforCompressorOperation();
  }

  HVACComponent AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::coolingCoil() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->coolingCoil();
  }

  HVACComponent AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::supplementalHeatingCoil() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->supplementalHeatingCoil();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::maximumSupplyAirTemperaturefromSupplementalHeater() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->maximumSupplyAirTemperaturefromSupplementalHeater();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized();
  }

  double AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();
  }

  double AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::auxiliaryOnCycleElectricPower() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->auxiliaryOnCycleElectricPower();
  }

  double AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::auxiliaryOffCycleElectricPower() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->auxiliaryOffCycleElectricPower();
  }

  double AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::designHeatRecoveryWaterFlowRate() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->designHeatRecoveryWaterFlowRate();
  }

  double AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::maximumTemperatureforHeatRecovery() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->maximumTemperatureforHeatRecovery();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->supplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized();
  }

  int AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::numberofSpeedsforHeating() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->numberofSpeedsforHeating();
  }

  int AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::numberofSpeedsforCooling() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->numberofSpeedsforCooling();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::speed1SupplyAirFlowRateDuringHeatingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->speed1SupplyAirFlowRateDuringHeatingOperation();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::isSpeed1SupplyAirFlowRateDuringHeatingOperationAutosized() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->isSpeed1SupplyAirFlowRateDuringHeatingOperationAutosized();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::speed2SupplyAirFlowRateDuringHeatingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->speed2SupplyAirFlowRateDuringHeatingOperation();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::isSpeed2SupplyAirFlowRateDuringHeatingOperationAutosized() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->isSpeed2SupplyAirFlowRateDuringHeatingOperationAutosized();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::speed3SupplyAirFlowRateDuringHeatingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->speed3SupplyAirFlowRateDuringHeatingOperation();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::isSpeed3SupplyAirFlowRateDuringHeatingOperationAutosized() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->isSpeed3SupplyAirFlowRateDuringHeatingOperationAutosized();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::speed4SupplyAirFlowRateDuringHeatingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->speed4SupplyAirFlowRateDuringHeatingOperation();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::isSpeed4SupplyAirFlowRateDuringHeatingOperationAutosized() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->isSpeed4SupplyAirFlowRateDuringHeatingOperationAutosized();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::speed1SupplyAirFlowRateDuringCoolingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->speed1SupplyAirFlowRateDuringCoolingOperation();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::isSpeed1SupplyAirFlowRateDuringCoolingOperationAutosized() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->isSpeed1SupplyAirFlowRateDuringCoolingOperationAutosized();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::speed2SupplyAirFlowRateDuringCoolingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->speed2SupplyAirFlowRateDuringCoolingOperation();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::isSpeed2SupplyAirFlowRateDuringCoolingOperationAutosized() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->isSpeed2SupplyAirFlowRateDuringCoolingOperationAutosized();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::speed3SupplyAirFlowRateDuringCoolingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->speed3SupplyAirFlowRateDuringCoolingOperation();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::isSpeed3SupplyAirFlowRateDuringCoolingOperationAutosized() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->isSpeed3SupplyAirFlowRateDuringCoolingOperationAutosized();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::speed4SupplyAirFlowRateDuringCoolingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->speed4SupplyAirFlowRateDuringCoolingOperation();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::isSpeed4SupplyAirFlowRateDuringCoolingOperationAutosized() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->isSpeed4SupplyAirFlowRateDuringCoolingOperationAutosized();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setAvailabilitySchedule(schedule);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::resetAvailabilitySchedule() {
    getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->resetAvailabilitySchedule();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setControllingZoneorThermostatLocation(const ThermalZone& thermalZone) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setControllingZoneorThermostatLocation(thermalZone);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::resetControllingZoneorThermostatLocation() {
    getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->resetControllingZoneorThermostatLocation();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setSupplyAirFan(const HVACComponent& fan) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setSupplyAirFan(fan);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setSupplyAirFanPlacement(const std::string& supplyAirFanPlacement) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setSupplyAirFanPlacement(supplyAirFanPlacement);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setSupplyAirFanOperatingModeSchedule(Schedule& schedule) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setSupplyAirFanOperatingModeSchedule(schedule);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setHeatingCoil(const HVACComponent& coil) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setHeatingCoil(coil);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setMinimumOutdoorDryBulbTemperatureforCompressorOperation(
    double minimumOutdoorDryBulbTemperatureforCompressorOperation) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setMinimumOutdoorDryBulbTemperatureforCompressorOperation(
      minimumOutdoorDryBulbTemperatureforCompressorOperation);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setCoolingCoil(const HVACComponent& coil) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setCoolingCoil(coil);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setSupplementalHeatingCoil(const HVACComponent& coil) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setSupplementalHeatingCoil(coil);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setMaximumSupplyAirTemperaturefromSupplementalHeater(
    double maximumSupplyAirTemperaturefromSupplementalHeater) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setMaximumSupplyAirTemperaturefromSupplementalHeater(
      maximumSupplyAirTemperaturefromSupplementalHeater);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::autosizeMaximumSupplyAirTemperaturefromSupplementalHeater() {
    getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(
    double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(
      maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setAuxiliaryOnCycleElectricPower(double auxiliaryOnCycleElectricPower) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setAuxiliaryOnCycleElectricPower(auxiliaryOnCycleElectricPower);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setAuxiliaryOffCycleElectricPower(double auxiliaryOffCycleElectricPower) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setAuxiliaryOffCycleElectricPower(auxiliaryOffCycleElectricPower);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setDesignHeatRecoveryWaterFlowRate(double designHeatRecoveryWaterFlowRate) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setDesignHeatRecoveryWaterFlowRate(designHeatRecoveryWaterFlowRate);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setMaximumTemperatureforHeatRecovery(double maximumTemperatureforHeatRecovery) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setMaximumTemperatureforHeatRecovery(
      maximumTemperatureforHeatRecovery);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(
    double supplyAirFlowRateWhenNoCoolingorHeatingisNeeded) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(
      supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setNumberofSpeedsforHeating(int numberofSpeedsforHeating) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setNumberofSpeedsforHeating(numberofSpeedsforHeating);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setNumberofSpeedsforCooling(int numberofSpeedsforCooling) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setNumberofSpeedsforCooling(numberofSpeedsforCooling);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setSpeed1SupplyAirFlowRateDuringHeatingOperation(
    double speed1SupplyAirFlowRateDuringHeatingOperation) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setSpeed1SupplyAirFlowRateDuringHeatingOperation(
      speed1SupplyAirFlowRateDuringHeatingOperation);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::autosizeSpeed1SupplyAirFlowRateDuringHeatingOperation() {
    getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->autosizeSpeed1SupplyAirFlowRateDuringHeatingOperation();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setSpeed2SupplyAirFlowRateDuringHeatingOperation(
    double speed2SupplyAirFlowRateDuringHeatingOperation) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setSpeed2SupplyAirFlowRateDuringHeatingOperation(
      speed2SupplyAirFlowRateDuringHeatingOperation);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::autosizeSpeed2SupplyAirFlowRateDuringHeatingOperation() {
    getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->autosizeSpeed2SupplyAirFlowRateDuringHeatingOperation();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setSpeed3SupplyAirFlowRateDuringHeatingOperation(
    double speed3SupplyAirFlowRateDuringHeatingOperation) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setSpeed3SupplyAirFlowRateDuringHeatingOperation(
      speed3SupplyAirFlowRateDuringHeatingOperation);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::autosizeSpeed3SupplyAirFlowRateDuringHeatingOperation() {
    getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->autosizeSpeed3SupplyAirFlowRateDuringHeatingOperation();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setSpeed4SupplyAirFlowRateDuringHeatingOperation(
    double speed4SupplyAirFlowRateDuringHeatingOperation) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setSpeed4SupplyAirFlowRateDuringHeatingOperation(
      speed4SupplyAirFlowRateDuringHeatingOperation);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::autosizeSpeed4SupplyAirFlowRateDuringHeatingOperation() {
    getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->autosizeSpeed4SupplyAirFlowRateDuringHeatingOperation();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setSpeed1SupplyAirFlowRateDuringCoolingOperation(
    double speed1SupplyAirFlowRateDuringCoolingOperation) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setSpeed1SupplyAirFlowRateDuringCoolingOperation(
      speed1SupplyAirFlowRateDuringCoolingOperation);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::autosizeSpeed1SupplyAirFlowRateDuringCoolingOperation() {
    getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->autosizeSpeed1SupplyAirFlowRateDuringCoolingOperation();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setSpeed2SupplyAirFlowRateDuringCoolingOperation(
    double speed2SupplyAirFlowRateDuringCoolingOperation) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setSpeed2SupplyAirFlowRateDuringCoolingOperation(
      speed2SupplyAirFlowRateDuringCoolingOperation);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::autosizeSpeed2SupplyAirFlowRateDuringCoolingOperation() {
    getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->autosizeSpeed2SupplyAirFlowRateDuringCoolingOperation();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setSpeed3SupplyAirFlowRateDuringCoolingOperation(
    double speed3SupplyAirFlowRateDuringCoolingOperation) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setSpeed3SupplyAirFlowRateDuringCoolingOperation(
      speed3SupplyAirFlowRateDuringCoolingOperation);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::autosizeSpeed3SupplyAirFlowRateDuringCoolingOperation() {
    getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->autosizeSpeed3SupplyAirFlowRateDuringCoolingOperation();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::setSpeed4SupplyAirFlowRateDuringCoolingOperation(
    double speed4SupplyAirFlowRateDuringCoolingOperation) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->setSpeed4SupplyAirFlowRateDuringCoolingOperation(
      speed4SupplyAirFlowRateDuringCoolingOperation);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::autosizeSpeed4SupplyAirFlowRateDuringCoolingOperation() {
    getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->autosizeSpeed4SupplyAirFlowRateDuringCoolingOperation();
  }

  /// @cond
  AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed(
    std::shared_ptr<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::autosizedMaximumSupplyAirTemperaturefromSupplementalHeater() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->autosizedMaximumSupplyAirTemperaturefromSupplementalHeater();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::autosizedSpeed1SupplyAirFlowRateDuringHeatingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->autosizedSpeed1SupplyAirFlowRateDuringHeatingOperation();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::autosizedSpeed2SupplyAirFlowRateDuringHeatingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->autosizedSpeed2SupplyAirFlowRateDuringHeatingOperation();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::autosizedSpeed3SupplyAirFlowRateDuringHeatingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->autosizedSpeed3SupplyAirFlowRateDuringHeatingOperation();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::autosizedSpeed4SupplyAirFlowRateDuringHeatingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->autosizedSpeed4SupplyAirFlowRateDuringHeatingOperation();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::autosizedSpeed1SupplyAirFlowRateDuringCoolingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->autosizedSpeed1SupplyAirFlowRateDuringCoolingOperation();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::autosizedSpeed2SupplyAirFlowRateDuringCoolingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->autosizedSpeed2SupplyAirFlowRateDuringCoolingOperation();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::autosizedSpeed3SupplyAirFlowRateDuringCoolingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->autosizedSpeed3SupplyAirFlowRateDuringCoolingOperation();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed::autosizedSpeed4SupplyAirFlowRateDuringCoolingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl>()->autosizedSpeed4SupplyAirFlowRateDuringCoolingOperation();
  }

}  // namespace model
}  // namespace openstudio
