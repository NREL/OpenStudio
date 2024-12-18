/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "CoilHeatingDXSingleSpeed.hpp"
#include "CoilHeatingDXSingleSpeed_Impl.hpp"
#include "CoilCoolingDXSingleSpeed.hpp"
#include "CoilCoolingDXSingleSpeed_Impl.hpp"
#include "CoilHeatingDXVariableSpeed.hpp"
#include "CoilHeatingDXVariableSpeed_Impl.hpp"
#include "CoilCoolingDXVariableSpeed.hpp"
#include "CoilCoolingDXVariableSpeed_Impl.hpp"
#include "CoilSystemCoolingDXHeatExchangerAssisted.hpp"
#include "CoilSystemCoolingDXHeatExchangerAssisted_Impl.hpp"
#include "CoilSystemIntegratedHeatPumpAirSource.hpp"
#include "CoilSystemIntegratedHeatPumpAirSource_Impl.hpp"
#include "CoilHeatingElectric.hpp"
#include "CoilHeatingElectric_Impl.hpp"
#include "CoilHeatingGas.hpp"
#include "CoilHeatingGas_Impl.hpp"
#include "CoilHeatingWater.hpp"
#include "CoilHeatingWater_Impl.hpp"
#include "FanConstantVolume.hpp"
#include "FanConstantVolume_Impl.hpp"
#include "FanOnOff.hpp"
#include "FanOnOff_Impl.hpp"
#include "FanSystemModel.hpp"
#include "FanSystemModel_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    AirLoopHVACUnitaryHeatPumpAirToAir_Impl::AirLoopHVACUnitaryHeatPumpAirToAir_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirLoopHVACUnitaryHeatPumpAirToAir::iddObjectType());
    }

    AirLoopHVACUnitaryHeatPumpAirToAir_Impl::AirLoopHVACUnitaryHeatPumpAirToAir_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirLoopHVACUnitaryHeatPumpAirToAir::iddObjectType());
    }

    AirLoopHVACUnitaryHeatPumpAirToAir_Impl::AirLoopHVACUnitaryHeatPumpAirToAir_Impl(const AirLoopHVACUnitaryHeatPumpAirToAir_Impl& other,
                                                                                     Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    ModelObject AirLoopHVACUnitaryHeatPumpAirToAir_Impl::clone(Model model) const {
      auto newUnitary = StraightComponent_Impl::clone(model).cast<AirLoopHVACUnitaryHeatPumpAirToAir>();

      auto newFan = this->supplyAirFan().clone(model).cast<HVACComponent>();

      auto newCoolingCoil = this->coolingCoil().clone(model).cast<HVACComponent>();

      auto newHeatingCoil = this->heatingCoil().clone(model).cast<HVACComponent>();

      auto newSupHeatingCoil = this->supplementalHeatingCoil().clone(model).cast<HVACComponent>();

      newUnitary.setCoolingCoil(newCoolingCoil);

      newUnitary.setHeatingCoil(newHeatingCoil);

      newUnitary.setSupplyAirFan(newFan);

      newUnitary.setSupplementalHeatingCoil(newSupHeatingCoil);

      return std::move(newUnitary);
    }

    const std::vector<std::string>& AirLoopHVACUnitaryHeatPumpAirToAir_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType AirLoopHVACUnitaryHeatPumpAirToAir_Impl::iddObjectType() const {
      return AirLoopHVACUnitaryHeatPumpAirToAir::iddObjectType();
    }

    std::vector<ScheduleTypeKey> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("AirLoopHVACUnitaryHeatPumpAirToAir", "Availability"));
      }
      if (std::find(b, e, OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFanOperatingModeScheduleName) != e) {
        result.push_back(ScheduleTypeKey("AirLoopHVACUnitaryHeatPumpAirToAir", "Supply Air Fan Operating Mode"));
      }
      return result;
    }

    std::vector<ModelObject> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::children() const {
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

    unsigned AirLoopHVACUnitaryHeatPumpAirToAir_Impl::inletPort() const {
      return OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::AirInletNodeName;
    }

    unsigned AirLoopHVACUnitaryHeatPumpAirToAir_Impl::outletPort() const {
      return OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::AirOutletNodeName;
    }

    Schedule AirLoopHVACUnitaryHeatPumpAirToAir_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value =
        getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::AvailabilityScheduleName);
      if (!value) {
        // it is an error if we get here, however we don't want to crash
        // so we hook up to global always on schedule
        LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
        value = this->model().alwaysOnDiscreteSchedule();
        OS_ASSERT(value);
        const_cast<AirLoopHVACUnitaryHeatPumpAirToAir_Impl*>(this)->setAvailabilitySchedule(*value);
        value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::AvailabilityScheduleName);
      }
      OS_ASSERT(value);
      return value.get();
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::addToNode(Node& node) {
      if (boost::optional<AirLoopHVAC> airLoop = node.airLoopHVAC()) {
        if (airLoop->supplyComponent(node.handle())) {
          return StraightComponent_Impl::addToNode(node);
        }
      }

      return false;
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::supplyAirFlowRateDuringCoolingOperation() const {
      return getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateDuringCoolingOperation, true);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::isSupplyAirFlowRateDuringCoolingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateDuringCoolingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::supplyAirFlowRateDuringHeatingOperation() const {
      return getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateDuringHeatingOperation, true);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::isSupplyAirFlowRateDuringHeatingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateDuringHeatingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
      return getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<ThermalZone> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::controllingZone() const {
      boost::optional<ThermalZone> result;

      result = getObject<ModelObject>().getModelObjectTarget<ThermalZone>(
        OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::ControllingZoneorThermostatLocation);

      return result;
    }

    HVACComponent AirLoopHVACUnitaryHeatPumpAirToAir_Impl::supplyAirFan() const {
      boost::optional<HVACComponent> value = optionalSupplyAirFan();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Supply Air Fan attached.");
      }
      return value.get();
    }

    HVACComponent AirLoopHVACUnitaryHeatPumpAirToAir_Impl::heatingCoil() const {
      boost::optional<HVACComponent> value = optionalHeatingCoil();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Heating Coil attached.");
      }
      return value.get();
    }

    HVACComponent AirLoopHVACUnitaryHeatPumpAirToAir_Impl::coolingCoil() const {
      boost::optional<HVACComponent> value = optionalCoolingCoil();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Cooling Coil attached.");
      }
      return value.get();
    }

    HVACComponent AirLoopHVACUnitaryHeatPumpAirToAir_Impl::supplementalHeatingCoil() const {
      auto value = optionalSupplementalHeatingCoil();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have a Supplemental Heating Coil attached.");
      }
      return value.get();
    }

    boost::optional<HVACComponent> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::optionalSupplyAirFan() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFanName);
    }

    boost::optional<HVACComponent> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::optionalHeatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::HeatingCoilName);
    }

    boost::optional<HVACComponent> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::optionalCoolingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::CoolingCoilName);
    }

    boost::optional<HVACComponent> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::optionalSupplementalHeatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplementalHeatingCoilName);
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::maximumSupplyAirTemperaturefromSupplementalHeater() const {
      return getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumSupplyAirTemperaturefromSupplementalHeater, true);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumSupplyAirTemperaturefromSupplementalHeater, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    double AirLoopHVACUnitaryHeatPumpAirToAir_Impl::maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const {
      boost::optional<double> value =
        getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const {
      return isEmpty(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
    }

    std::string AirLoopHVACUnitaryHeatPumpAirToAir_Impl::fanPlacement() const {
      boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::FanPlacement, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::isFanPlacementDefaulted() const {
      return isEmpty(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::FanPlacement);
    }

    boost::optional<Schedule> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::supplyAirFanOperatingModeSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFanOperatingModeScheduleName);
    }

    std::string AirLoopHVACUnitaryHeatPumpAirToAir_Impl::dehumidificationControlType() const {
      boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::DehumidificationControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::isDehumidificationControlTypeDefaulted() const {
      return isEmpty(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::DehumidificationControlType);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::AvailabilityScheduleName, "AirLoopHVACUnitaryHeatPumpAirToAir",
                                "Availability", schedule);
      return result;
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setSupplyAirFlowRateDuringCoolingOperation(
      boost::optional<double> supplyAirFlowRateDuringCoolingOperation) {
      bool result = false;
      if (supplyAirFlowRateDuringCoolingOperation) {
        result = setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateDuringCoolingOperation,
                           supplyAirFlowRateDuringCoolingOperation.get());
      } else {
        result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateDuringCoolingOperation, "");
      }
      return result;
    }

    void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::autosizeSupplyAirFlowRateDuringCoolingOperation() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateDuringCoolingOperation, "Autosize");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setSupplyAirFlowRateDuringHeatingOperation(boost::optional<double> n2) {
      bool result = false;
      if (n2) {
        result = setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateDuringHeatingOperation, n2.get());
      } else {
        result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateDuringHeatingOperation, "");
      }
      return result;
    }

    void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::autosizeSupplyAirFlowRateDuringHeatingOperation() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateDuringHeatingOperation, "Autosize");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(
      boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded) {
      bool result = false;
      if (supplyAirFlowRateWhenNoCoolingorHeatingisNeeded) {
        result = setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded,
                           supplyAirFlowRateWhenNoCoolingorHeatingisNeeded.get());
      } else {
        result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, "");
      }
      return result;
    }

    void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, "");
      OS_ASSERT(result);
    }

    void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, "Autosize");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setControllingZone(ThermalZone& zone) {
      return setPointer(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::ControllingZoneorThermostatLocation, zone.handle());
      ;
    }

    void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::resetControllingZone() {
      setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::ControllingZoneorThermostatLocation, "");
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setSupplyAirFan(HVACComponent& hvacComponent) {
      // TODO: Doesn't support Fan:SystemModel yet
      if (!hvacComponent.optionalCast<FanConstantVolume>() && !hvacComponent.optionalCast<FanOnOff>()  // &&
          // ! hvacComponent.optionalCast<FanSystemModel>()
      ) {
        return false;
      }

      return setPointer(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFanName, hvacComponent.handle());
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setHeatingCoil(HVACComponent& hvacComponent) {
      bool isTypeOK = false;

      if (hvacComponent.optionalCast<CoilHeatingDXSingleSpeed>()) {
        isTypeOK = true;
      } else if (hvacComponent.optionalCast<CoilHeatingDXVariableSpeed>()) {
        isTypeOK = true;
      } else if (hvacComponent.optionalCast<CoilSystemIntegratedHeatPumpAirSource>()) {
        isTypeOK = true;
      }

      if (isTypeOK) {
        return setPointer(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::HeatingCoilName, hvacComponent.handle());
      }
      return false;
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setCoolingCoil(HVACComponent& hvacComponent) {
      bool isTypeOK = false;

      if (hvacComponent.optionalCast<CoilCoolingDXSingleSpeed>()) {
        isTypeOK = true;
      } else if (hvacComponent.optionalCast<CoilCoolingDXVariableSpeed>()) {
        isTypeOK = true;
      } else if (hvacComponent.optionalCast<CoilSystemCoolingDXHeatExchangerAssisted>()) {
        isTypeOK = true;
      } else if (hvacComponent.optionalCast<CoilSystemIntegratedHeatPumpAirSource>()) {
        isTypeOK = true;
      }

      if (isTypeOK) {
        return setPointer(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::CoolingCoilName, hvacComponent.handle());
      }
      return false;
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setSupplementalHeatingCoil(HVACComponent& hvacComponent) {
      bool isTypeOK = false;

      if (hvacComponent.optionalCast<CoilHeatingGas>()) {
        isTypeOK = true;
      } else if (hvacComponent.optionalCast<CoilHeatingElectric>()) {
        isTypeOK = true;
      } else if (hvacComponent.optionalCast<CoilHeatingWater>()) {
        isTypeOK = true;
      }

      if (isTypeOK) {
        return setPointer(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplementalHeatingCoilName, hvacComponent.handle());
      }
      return false;
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setMaximumSupplyAirTemperaturefromSupplementalHeater(
      boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater) {
      bool result = false;
      if (maximumSupplyAirTemperaturefromSupplementalHeater) {
        result = setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumSupplyAirTemperaturefromSupplementalHeater,
                           maximumSupplyAirTemperaturefromSupplementalHeater.get());
      } else {
        result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumSupplyAirTemperaturefromSupplementalHeater, "");
      }
      OS_ASSERT(result);
      return result;
    }

    void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::autosizeMaximumSupplyAirTemperaturefromSupplementalHeater() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumSupplyAirTemperaturefromSupplementalHeater, "Autosize");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(
      double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation) {
      bool result = setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation,
                              maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
      return result;
    }

    void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setFanPlacement(const std::string& fanPlacement) {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::FanPlacement, fanPlacement);
      return result;
    }

    void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::resetFanPlacement() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::FanPlacement, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setSupplyAirFanOperatingModeSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFanOperatingModeScheduleName,
                                "AirLoopHVACUnitaryHeatPumpAirToAir", "Supply Air Fan Operating Mode", schedule);
      return result;
    }

    void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::resetSupplyAirFanOperatingModeSchedule() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFanOperatingModeScheduleName, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setDehumidificationControlType(const std::string& dehumidificationControlType) {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::DehumidificationControlType, dehumidificationControlType);
      return result;
    }

    void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::resetDehumidificationControlType() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::DehumidificationControlType, "");
      OS_ASSERT(result);
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::autosizedSupplyAirFlowRateDuringCoolingOperation() const {
      return getAutosizedValue("Supply Air Flow Rate During Cooling Operation", "m3/s");
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::autosizedSupplyAirFlowRateDuringHeatingOperation() const {
      return getAutosizedValue("Supply Air Flow Rate During Heating Operation", "m3/s");
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
      return getAutosizedValue("Supply Air Flow Rate", "m3/s");
    }

    boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::autosizedMaximumSupplyAirTemperaturefromSupplementalHeater() const {
      return getAutosizedValue("Maximum Supply Air Temperature from Supplemental Heater", "C");
    }

    void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::autosize() {
      autosizeSupplyAirFlowRateDuringCoolingOperation();
      autosizeSupplyAirFlowRateDuringHeatingOperation();
      autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
      autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();
    }

    void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedSupplyAirFlowRateDuringCoolingOperation();
      if (val) {
        setSupplyAirFlowRateDuringCoolingOperation(val.get());
      }

      val = autosizedSupplyAirFlowRateDuringHeatingOperation();
      if (val) {
        setSupplyAirFlowRateDuringHeatingOperation(val.get());
      }

      val = autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
      if (val) {
        setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(val.get());
      }

      val = autosizedMaximumSupplyAirTemperaturefromSupplementalHeater();
      if (val) {
        setMaximumSupplyAirTemperaturefromSupplementalHeater(val.get());
      }
    }

    std::vector<EMSActuatorNames> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"AirLoopHVAC:UnitaryHeatPump:AirToAir", "Autosized Supply Air Flow Rate"},
                                              {"Unitary HVAC", "Sensible Load Request"},
                                              {"Unitary HVAC", "Moisture Load Request"}};
      return actuators;
    }

    std::vector<std::string> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types{"Unitary HVAC Design Heating Capacity", "Unitary HVAC Design Cooling Capacity"};
      return types;
    }

    ComponentType AirLoopHVACUnitaryHeatPumpAirToAir_Impl::componentType() const {
      return ComponentType::Both;
    }

    std::vector<FuelType> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::coolingFuelTypes() const {
      return coolingCoil().coolingFuelTypes();
    }

    std::vector<FuelType> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::heatingFuelTypes() const {
      std::set<FuelType> result;
      for (auto ft : heatingCoil().heatingFuelTypes()) {
        result.insert(ft);
      }
      for (auto ft : supplementalHeatingCoil().heatingFuelTypes()) {
        result.insert(ft);
      }
      return {result.begin(), result.end()};
    }

    std::vector<AppGFuelType> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::appGHeatingFuelTypes() const {
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

  AirLoopHVACUnitaryHeatPumpAirToAir::AirLoopHVACUnitaryHeatPumpAirToAir(const Model& model, Schedule& availabilitySchedule, HVACComponent& supplyFan,
                                                                         HVACComponent& heatingCoil, HVACComponent& coolingCoil,
                                                                         HVACComponent& supplementalHeatingCoil)
    : StraightComponent(AirLoopHVACUnitaryHeatPumpAirToAir::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>());

    bool ok = setAvailabilitySchedule(availabilitySchedule);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s availability schedule to " << availabilitySchedule.briefDescription() << ".");
    }

    ok = setSupplyAirFan(supplyFan);
    if (!ok) {
      remove();  // Funny thing: you can't remove it, because it'll call children, which will try to get the supplyAirFan, which will not work and hit an OS_ASSERT...
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s supply fan to " << supplyFan.briefDescription() << ".");
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

    autosizeSupplyAirFlowRateDuringCoolingOperation();
    autosizeSupplyAirFlowRateDuringHeatingOperation();
    autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();
  }

  IddObjectType AirLoopHVACUnitaryHeatPumpAirToAir::iddObjectType() {
    IddObjectType result(IddObjectType::OS_AirLoopHVAC_UnitaryHeatPump_AirToAir);
    return result;
  }

  std::vector<std::string> AirLoopHVACUnitaryHeatPumpAirToAir::validFanPlacementValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::FanPlacement);
  }

  std::vector<std::string> AirLoopHVACUnitaryHeatPumpAirToAir::validDehumidificationControlTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::DehumidificationControlType);
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir::supplyAirFlowRateDuringCoolingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->supplyAirFlowRateDuringCoolingOperation();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir::isSupplyAirFlowRateDuringCoolingOperationAutosized() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->isSupplyAirFlowRateDuringCoolingOperationAutosized();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir::supplyAirFlowRateDuringHeatingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->supplyAirFlowRateDuringHeatingOperation();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir::isSupplyAirFlowRateDuringHeatingOperationAutosized() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->isSupplyAirFlowRateDuringHeatingOperationAutosized();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir::supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->supplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir::isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir::maximumSupplyAirTemperaturefromSupplementalHeater() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->maximumSupplyAirTemperaturefromSupplementalHeater();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir::isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized();
  }

  double AirLoopHVACUnitaryHeatPumpAirToAir::maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir::isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted();
  }

  std::string AirLoopHVACUnitaryHeatPumpAirToAir::fanPlacement() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->fanPlacement();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir::isFanPlacementDefaulted() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->isFanPlacementDefaulted();
  }

  std::string AirLoopHVACUnitaryHeatPumpAirToAir::dehumidificationControlType() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->dehumidificationControlType();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir::isDehumidificationControlTypeDefaulted() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->isDehumidificationControlTypeDefaulted();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir::setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setSupplyAirFlowRateDuringCoolingOperation(
      supplyAirFlowRateDuringCoolingOperation);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir::autosizeSupplyAirFlowRateDuringCoolingOperation() {
    getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->autosizeSupplyAirFlowRateDuringCoolingOperation();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir::setSupplyAirFlowRateDuringHeatingOperation(double n2) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setSupplyAirFlowRateDuringHeatingOperation(n2);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir::autosizeSupplyAirFlowRateDuringHeatingOperation() {
    getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->autosizeSupplyAirFlowRateDuringHeatingOperation();
  }

  bool
    AirLoopHVACUnitaryHeatPumpAirToAir::setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(double supplyAirFlowRateWhenNoCoolingorHeatingisNeeded) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(
      supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir::resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir::autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir::setMaximumSupplyAirTemperaturefromSupplementalHeater(
    double maximumSupplyAirTemperaturefromSupplementalHeater) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setMaximumSupplyAirTemperaturefromSupplementalHeater(
      maximumSupplyAirTemperaturefromSupplementalHeater);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir::autosizeMaximumSupplyAirTemperaturefromSupplementalHeater() {
    getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir::setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(
    double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(
      maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir::resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() {
    getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir::setFanPlacement(const std::string& fanPlacement) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setFanPlacement(fanPlacement);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir::resetFanPlacement() {
    getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->resetFanPlacement();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir::setSupplyAirFanOperatingModeSchedule(Schedule& schedule) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setSupplyAirFanOperatingModeSchedule(schedule);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir::resetSupplyAirFanOperatingModeSchedule() {
    getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->resetSupplyAirFanOperatingModeSchedule();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir::setDehumidificationControlType(const std::string& dehumidificationControlType) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setDehumidificationControlType(dehumidificationControlType);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir::resetDehumidificationControlType() {
    getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->resetDehumidificationControlType();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir::setControllingZone(ThermalZone& zone) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setControllingZone(zone);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir::resetControllingZone() {
    getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->resetControllingZone();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir::setSupplyAirFan(HVACComponent& hvacComponent) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setSupplyAirFan(hvacComponent);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir::setHeatingCoil(HVACComponent& hvacComponent) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setHeatingCoil(hvacComponent);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir::setCoolingCoil(HVACComponent& hvacComponent) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setCoolingCoil(hvacComponent);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir::setSupplementalHeatingCoil(HVACComponent& hvacComponent) {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setSupplementalHeatingCoil(hvacComponent);
  }

  boost::optional<Schedule> AirLoopHVACUnitaryHeatPumpAirToAir::supplyAirFanOperatingModeSchedule() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->supplyAirFanOperatingModeSchedule();
  }

  Schedule AirLoopHVACUnitaryHeatPumpAirToAir::availabilitySchedule() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->availabilitySchedule();
  }

  boost::optional<ThermalZone> AirLoopHVACUnitaryHeatPumpAirToAir::controllingZone() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->controllingZone();
  }

  HVACComponent AirLoopHVACUnitaryHeatPumpAirToAir::supplyAirFan() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->supplyAirFan();
  }

  HVACComponent AirLoopHVACUnitaryHeatPumpAirToAir::heatingCoil() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->heatingCoil();
  }

  HVACComponent AirLoopHVACUnitaryHeatPumpAirToAir::coolingCoil() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->coolingCoil();
  }

  HVACComponent AirLoopHVACUnitaryHeatPumpAirToAir::supplementalHeatingCoil() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->supplementalHeatingCoil();
  }

  /// @cond
  AirLoopHVACUnitaryHeatPumpAirToAir::AirLoopHVACUnitaryHeatPumpAirToAir(std::shared_ptr<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir::autosizedSupplyAirFlowRateDuringCoolingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->autosizedSupplyAirFlowRateDuringCoolingOperation();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir::autosizedSupplyAirFlowRateDuringHeatingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->autosizedSupplyAirFlowRateDuringHeatingOperation();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir::autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir::autosizedMaximumSupplyAirTemperaturefromSupplementalHeater() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->autosizedMaximumSupplyAirTemperaturefromSupplementalHeater();
  }

}  // namespace model
}  // namespace openstudio
