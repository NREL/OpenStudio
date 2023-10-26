/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "WaterToAirComponent.hpp"
#include "WaterToAirComponent_Impl.hpp"
#include "CoilHeatingWater.hpp"
#include "CoilHeatingWater_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_ZoneHVAC_PackagedTerminalAirConditioner_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    ZoneHVACPackagedTerminalAirConditioner_Impl::ZoneHVACPackagedTerminalAirConditioner_Impl(const IdfObject& idfObject, Model_Impl* model,
                                                                                             bool keepHandle)
      : ZoneHVACComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ZoneHVACPackagedTerminalAirConditioner::iddObjectType());
    }

    ZoneHVACPackagedTerminalAirConditioner_Impl::ZoneHVACPackagedTerminalAirConditioner_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ZoneHVACPackagedTerminalAirConditioner::iddObjectType());
    }

    ZoneHVACPackagedTerminalAirConditioner_Impl::ZoneHVACPackagedTerminalAirConditioner_Impl(const ZoneHVACPackagedTerminalAirConditioner_Impl& other,
                                                                                             Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {}

    ModelObject ZoneHVACPackagedTerminalAirConditioner_Impl::clone(Model model) const {
      auto ptacClone = ZoneHVACComponent_Impl::clone(model).cast<ZoneHVACPackagedTerminalAirConditioner>();

      auto supplyFanClone = this->supplyAirFan().clone(model).cast<HVACComponent>();

      auto t_heatingCoil = heatingCoil();
      auto heatingCoilClone = t_heatingCoil.clone(model).cast<HVACComponent>();

      auto t_coolingCoil = coolingCoil();
      auto coolingCoilClone = t_coolingCoil.clone(model).cast<HVACComponent>();

      ptacClone.setSupplyAirFan(supplyFanClone);

      ptacClone.setHeatingCoil(heatingCoilClone);

      ptacClone.setCoolingCoil(coolingCoilClone);

      if (model == this->model()) {
        if (auto waterToAirComponent = t_coolingCoil.optionalCast<WaterToAirComponent>()) {
          if (auto plant = waterToAirComponent->plantLoop()) {
            plant->addDemandBranchForComponent(coolingCoilClone);
          }
        }
        if (auto waterToAirComponent = t_heatingCoil.optionalCast<WaterToAirComponent>()) {
          if (auto plant = waterToAirComponent->plantLoop()) {
            plant->addDemandBranchForComponent(heatingCoilClone);
          }
        }
      }

      return std::move(ptacClone);
    }

    std::vector<IdfObject> ZoneHVACPackagedTerminalAirConditioner_Impl::remove() {
      if (boost::optional<CoilHeatingWater> waterHeatingCoil = heatingCoil().optionalCast<CoilHeatingWater>()) {
        if (boost::optional<PlantLoop> plantLoop = waterHeatingCoil->plantLoop()) {
          plantLoop->removeDemandBranchWithComponent(waterHeatingCoil.get());
        }
      }

      return ZoneHVACComponent_Impl::remove();
    }

    const std::vector<std::string>& ZoneHVACPackagedTerminalAirConditioner_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{
        "Zone Packaged Terminal Air Conditioner Total Heating Rate",     "Zone Packaged Terminal Air Conditioner Total Heating Energy",
        "Zone Packaged Terminal Air Conditioner Total Cooling Rate",     "Zone Packaged Terminal Air Conditioner Total Cooling Energy",
        "Zone Packaged Terminal Air Conditioner Sensible Heating Rate",  "Zone Packaged Terminal Air Conditioner Sensible Heating Energy",
        "Zone Packaged Terminal Air Conditioner Sensible Cooling Rate",  "Zone Packaged Terminal Air Conditioner Sensible Cooling Energy",
        "Zone Packaged Terminal Air Conditioner Latent Heating Rate",    "Zone Packaged Terminal Air Conditioner Latent Heating Energy",
        "Zone Packaged Terminal Air Conditioner Latent Cooling Rate",    "Zone Packaged Terminal Air Conditioner Latent Cooling Energy",
        "Zone Packaged Terminal Air Conditioner Electricity Rate",       "Zone Packaged Terminal Air Conditioner Electricity Energy",
        "Zone Packaged Terminal Air Conditioner Fan Part Load Ratio",    "Zone Packaged Terminal Air Conditioner Compressor Part Load Ratio",
        "Zone Packaged Terminal Air Conditioner Fan Availability Status"};
      return result;
    }

    IddObjectType ZoneHVACPackagedTerminalAirConditioner_Impl::iddObjectType() const {
      return ZoneHVACPackagedTerminalAirConditioner::iddObjectType();
    }

    std::vector<ScheduleTypeKey> ZoneHVACPackagedTerminalAirConditioner_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_ZoneHVAC_PackagedTerminalAirConditionerFields::AvailabilityScheduleName) != e) {
        result.emplace_back("ZoneHVACPackagedTerminalAirConditioner", "Availability");
      }
      if (std::find(b, e, OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFanOperatingModeScheduleName) != e) {
        result.emplace_back("ZoneHVACPackagedTerminalAirConditioner", "Supply Air Fan Operating Mode");
      }
      return result;
    }

    std::vector<ModelObject> ZoneHVACPackagedTerminalAirConditioner_Impl::children() const {
      std::vector<ModelObject> result;
      if (OptionalHVACComponent intermediate = optionalHeatingCoil()) {
        result.push_back(*intermediate);
      }
      if (OptionalHVACComponent intermediate = optionalSupplyAirFan()) {
        result.push_back(*intermediate);
      }
      if (OptionalHVACComponent intermediate = optionalCoolingCoil()) {
        result.push_back(*intermediate);
      }
      return result;
    }

    unsigned ZoneHVACPackagedTerminalAirConditioner_Impl::inletPort() const {
      return OS_ZoneHVAC_PackagedTerminalAirConditionerFields::AirInletNodeName;
    }

    unsigned ZoneHVACPackagedTerminalAirConditioner_Impl::outletPort() const {
      return OS_ZoneHVAC_PackagedTerminalAirConditionerFields::AirOutletNodeName;
    }

    Schedule ZoneHVACPackagedTerminalAirConditioner_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        // it is an error if we get here, however we don't want to crash
        // so we hook up to global always on schedule
        LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
        value = this->model().alwaysOnDiscreteSchedule();
        OS_ASSERT(value);
        const_cast<ZoneHVACPackagedTerminalAirConditioner_Impl*>(this)->setAvailabilitySchedule(*value);  // NOLINT
        value = optionalAvailabilitySchedule();
      }
      OS_ASSERT(value);
      return value.get();
    }

    std::string ZoneHVACPackagedTerminalAirConditioner_Impl::outdoorAirMixerObjectType() const {
      boost::optional<std::string> value = getString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirMixerObjectType, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string ZoneHVACPackagedTerminalAirConditioner_Impl::outdoorAirMixerName() const {
      boost::optional<std::string> value = getString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirMixerName, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> ZoneHVACPackagedTerminalAirConditioner_Impl::supplyAirFlowRateDuringCoolingOperation() const {
      return getDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateDuringCoolingOperation, true);
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::isSupplyAirFlowRateDuringCoolingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateDuringCoolingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> ZoneHVACPackagedTerminalAirConditioner_Impl::supplyAirFlowRateDuringHeatingOperation() const {
      return getDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateDuringHeatingOperation, true);
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::isSupplyAirFlowRateDuringHeatingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateDuringHeatingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> ZoneHVACPackagedTerminalAirConditioner_Impl::supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
      return getDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> ZoneHVACPackagedTerminalAirConditioner_Impl::outdoorAirFlowRateDuringCoolingOperation() const {
      return getDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringCoolingOperation, true);
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::isOutdoorAirFlowRateDuringCoolingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringCoolingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> ZoneHVACPackagedTerminalAirConditioner_Impl::outdoorAirFlowRateDuringHeatingOperation() const {
      return getDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringHeatingOperation, true);
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::isOutdoorAirFlowRateDuringHeatingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringHeatingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> ZoneHVACPackagedTerminalAirConditioner_Impl::outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
      return getDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    HVACComponent ZoneHVACPackagedTerminalAirConditioner_Impl::supplyAirFan() const {
      boost::optional<HVACComponent> fan = optionalSupplyAirFan();
      OS_ASSERT(fan);
      return fan.get();
    }

    HVACComponent ZoneHVACPackagedTerminalAirConditioner_Impl::heatingCoil() const {
      boost::optional<HVACComponent> coil = optionalHeatingCoil();
      OS_ASSERT(coil);
      return coil.get();
    }

    HVACComponent ZoneHVACPackagedTerminalAirConditioner_Impl::coolingCoil() const {
      boost::optional<HVACComponent> coil = optionalCoolingCoil();
      OS_ASSERT(coil);
      return coil.get();
    }

    std::string ZoneHVACPackagedTerminalAirConditioner_Impl::fanPlacement() const {
      boost::optional<std::string> value = getString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::FanPlacement, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::isFanPlacementDefaulted() const {
      return isEmpty(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::FanPlacement);
    }

    Schedule ZoneHVACPackagedTerminalAirConditioner_Impl::supplyAirFanOperatingModeSchedule() const {
      boost::optional<Schedule> value = optionalSupplyAirFanOperatingModeSchedule();
      if (!value) {
        // it is an error if we get here, however we don't want to crash
        // so we hook up to global always on schedule
        LOG(Error, "Required availability schedule not set, using 'Always Off' schedule");
        value = this->model().alwaysOffDiscreteSchedule();
        OS_ASSERT(value);
        const_cast<ZoneHVACPackagedTerminalAirConditioner_Impl*>(this)->setSupplyAirFanOperatingModeSchedule(*value);  // NOLINT
        value = optionalSupplyAirFanOperatingModeSchedule();
      }
      OS_ASSERT(value);
      return value.get();
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::AvailabilityScheduleName, "ZoneHVACPackagedTerminalAirConditioner",
                                "Availability", schedule);
      return result;
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::setOutdoorAirMixerObjectType(const std::string& outdoorAirMixerObjectType) {
      bool result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirMixerObjectType, outdoorAirMixerObjectType);
      return result;
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::setOutdoorAirMixerName(const std::string& outdoorAirMixerName) {
      bool result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirMixerName, outdoorAirMixerName);
      OS_ASSERT(result);
      return result;
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::setSupplyAirFlowRateDuringCoolingOperation(
      boost::optional<double> supplyAirFlowRateDuringCoolingOperation) {
      bool result = false;
      if (supplyAirFlowRateDuringCoolingOperation) {
        result = setDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateDuringCoolingOperation,
                           supplyAirFlowRateDuringCoolingOperation.get());
      } else {
        result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateDuringCoolingOperation, "");
      }
      return result;
    }

    void ZoneHVACPackagedTerminalAirConditioner_Impl::autosizeSupplyAirFlowRateDuringCoolingOperation() {
      bool result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateDuringCoolingOperation, "Autosize");
      OS_ASSERT(result);
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::setSupplyAirFlowRateDuringHeatingOperation(
      boost::optional<double> supplyAirFlowRateDuringHeatingOperation) {
      bool result = false;
      if (supplyAirFlowRateDuringHeatingOperation) {
        result = setDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateDuringHeatingOperation,
                           supplyAirFlowRateDuringHeatingOperation.get());
      } else {
        result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateDuringHeatingOperation, "");
      }
      return result;
    }

    void ZoneHVACPackagedTerminalAirConditioner_Impl::autosizeSupplyAirFlowRateDuringHeatingOperation() {
      bool result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateDuringHeatingOperation, "Autosize");
      OS_ASSERT(result);
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(
      boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded) {
      bool result = false;
      if (supplyAirFlowRateWhenNoCoolingorHeatingisNeeded) {
        result = setDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded,
                           supplyAirFlowRateWhenNoCoolingorHeatingisNeeded.get());
      } else {
        result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, "");
      }
      return result;
    }

    void ZoneHVACPackagedTerminalAirConditioner_Impl::resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
      bool result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, "");
      OS_ASSERT(result);
    }

    void ZoneHVACPackagedTerminalAirConditioner_Impl::autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
      bool result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, "Autosize");
      OS_ASSERT(result);
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::setOutdoorAirFlowRateDuringCoolingOperation(
      boost::optional<double> outdoorAirFlowRateDuringCoolingOperation) {
      bool result = false;
      if (outdoorAirFlowRateDuringCoolingOperation) {
        result = setDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringCoolingOperation,
                           outdoorAirFlowRateDuringCoolingOperation.get());
      } else {
        result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringCoolingOperation, "");
      }
      return result;
    }

    void ZoneHVACPackagedTerminalAirConditioner_Impl::autosizeOutdoorAirFlowRateDuringCoolingOperation() {
      bool result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringCoolingOperation, "Autosize");
      OS_ASSERT(result);
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::setOutdoorAirFlowRateDuringHeatingOperation(
      boost::optional<double> outdoorAirFlowRateDuringHeatingOperation) {
      bool result = false;
      if (outdoorAirFlowRateDuringHeatingOperation) {
        result = setDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringHeatingOperation,
                           outdoorAirFlowRateDuringHeatingOperation.get());
      } else {
        result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringHeatingOperation, "");
      }
      return result;
    }

    void ZoneHVACPackagedTerminalAirConditioner_Impl::autosizeOutdoorAirFlowRateDuringHeatingOperation() {
      bool result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringHeatingOperation, "Autosize");
      OS_ASSERT(result);
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(
      boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
      bool result = false;
      if (outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
        result = setDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded,
                           outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded.get());
      } else {
        result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, "");
      }
      return result;
    }

    void ZoneHVACPackagedTerminalAirConditioner_Impl::resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
      bool result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, "");
      OS_ASSERT(result);
    }

    void ZoneHVACPackagedTerminalAirConditioner_Impl::autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
      bool result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, "Autosize");
      OS_ASSERT(result);
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::setSupplyAirFan(HVACComponent& fan) {
      if ((fan.iddObjectType() == IddObjectType::OS_Fan_ConstantVolume) || (fan.iddObjectType() == IddObjectType::OS_Fan_OnOff)
          || (fan.iddObjectType() == IddObjectType::OS_Fan_SystemModel)) {
        return setPointer(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFanName, fan.handle());
      } else {
        LOG(Warn, "Invalid Fan Type (expected FanConstantVolume, FanOnOff or FanSystemModel, not '" << fan.iddObjectType().valueName() << "') for "
                                                                                                    << briefDescription());
        return false;
      }
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::setHeatingCoil(HVACComponent& heatingCoil) {
      bool isAllowedType = false;

      auto iddObjectType = heatingCoil.iddObjectType();
      if ((iddObjectType == IddObjectType::OS_Coil_Heating_Gas) || (iddObjectType == IddObjectType::OS_Coil_Heating_Electric)
          || (iddObjectType == IddObjectType::OS_Coil_Heating_Water)) {
        isAllowedType = true;
      }

      if (isAllowedType) {
        return setPointer(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::HeatingCoilName, heatingCoil.handle());
      } else {
        LOG(Warn, "Invalid Heating Coil Type (expected CoilHeatingGas, CoilHeatingElectric, or CoilHeatingWater, not '"
                    << heatingCoil.iddObjectType().valueName() << "')  for " << briefDescription());
        return false;
      }
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::setCoolingCoil(HVACComponent& coolingCoil) {
      bool isAllowedType = false;

      auto iddObjectType = coolingCoil.iddObjectType();
      if ((iddObjectType == IddObjectType::OS_Coil_Cooling_DX_SingleSpeed) || (iddObjectType == IddObjectType::OS_Coil_Cooling_DX_VariableSpeed)
          || (iddObjectType == IddObjectType::OS_CoilSystem_Cooling_DX_HeatExchangerAssisted)
          || (iddObjectType == IddObjectType::OS_Coil_Cooling_DX)) {
        isAllowedType = true;
      }

      if (isAllowedType) {
        return setPointer(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::CoolingCoilName, coolingCoil.handle());
      } else {
        LOG(Warn, "Invalid Cooling Coil Type (expected CoilCoolingDXSingleSpeed or CoilSystemCoolingDXHeatExchangerAssisted or "
                  "CoilCoolingDXVariableSpeed or CoilCoolingDX, not '"
                    << coolingCoil.iddObjectType().valueName() << "') for " << briefDescription());
        return false;
      }
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::setFanPlacement(const std::string& fanPlacement) {
      bool result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::FanPlacement, fanPlacement);
      return result;
    }

    void ZoneHVACPackagedTerminalAirConditioner_Impl::resetFanPlacement() {
      bool result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::FanPlacement, "");
      OS_ASSERT(result);
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::setSupplyAirFanOperatingModeSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFanOperatingModeScheduleName,
                                "ZoneHVACPackagedTerminalAirConditioner", "Supply Air Fan Operating Mode", schedule);
      return result;
    }

    boost::optional<Schedule> ZoneHVACPackagedTerminalAirConditioner_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::AvailabilityScheduleName);
    }
    boost::optional<HVACComponent> ZoneHVACPackagedTerminalAirConditioner_Impl::optionalSupplyAirFan() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFanName);
    }
    boost::optional<HVACComponent> ZoneHVACPackagedTerminalAirConditioner_Impl::optionalHeatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::HeatingCoilName);
    }
    boost::optional<HVACComponent> ZoneHVACPackagedTerminalAirConditioner_Impl::optionalCoolingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::CoolingCoilName);
    }

    boost::optional<Schedule> ZoneHVACPackagedTerminalAirConditioner_Impl::optionalSupplyAirFanOperatingModeSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFanOperatingModeScheduleName);
    }

    boost::optional<ModelObject> ZoneHVACPackagedTerminalAirConditioner_Impl::availabilityScheduleAsModelObject() const {
      OptionalModelObject result = availabilitySchedule();
      return result;
    }

    boost::optional<ModelObject> ZoneHVACPackagedTerminalAirConditioner_Impl::supplyAirFanAsModelObject() const {
      OptionalModelObject result = supplyAirFan();
      return result;
    }

    boost::optional<ModelObject> ZoneHVACPackagedTerminalAirConditioner_Impl::heatingCoilAsModelObject() const {
      OptionalModelObject result = heatingCoil();
      return result;
    }

    boost::optional<ModelObject> ZoneHVACPackagedTerminalAirConditioner_Impl::coolingCoilAsModelObject() const {
      OptionalModelObject result = coolingCoil();
      return result;
    }

    boost::optional<ModelObject> ZoneHVACPackagedTerminalAirConditioner_Impl::supplyAirFanOperatingModeScheduleAsModelObject() const {
      OptionalModelObject result;
      OptionalSchedule intermediate = supplyAirFanOperatingModeSchedule();
      if (intermediate) {
        result = *intermediate;
      }
      return result;
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setAvailabilitySchedule(schedule);
        }
      }
      return false;
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::setSupplyAirFanAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
        if (intermediate) {
          setSupplyAirFan(*intermediate);
          return true;
        }
      }
      return false;
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::setHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
        if (intermediate) {
          setHeatingCoil(*intermediate);
          return true;
        }
      }
      return false;
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::setCoolingCoilAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
        if (intermediate) {
          setCoolingCoil(*intermediate);
          return true;
        }
      }
      return false;
    }

    bool ZoneHVACPackagedTerminalAirConditioner_Impl::setSupplyAirFanOperatingModeScheduleAsModelObject(
      const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setSupplyAirFanOperatingModeSchedule(schedule);
        }
      }
      return false;
    }

    boost::optional<double> ZoneHVACPackagedTerminalAirConditioner_Impl::autosizedSupplyAirFlowRateDuringCoolingOperation() const {
      return getAutosizedValue("Design Size Cooling Supply Air Flow Rate", "m3/s");
    }

    boost::optional<double> ZoneHVACPackagedTerminalAirConditioner_Impl::autosizedSupplyAirFlowRateDuringHeatingOperation() const {
      return getAutosizedValue("Design Size Heating Supply Air Flow Rate", "m3/s");
    }

    boost::optional<double> ZoneHVACPackagedTerminalAirConditioner_Impl::autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
      return getAutosizedValue("Design Size No Load Supply Air Flow Rate", "m3/s");
    }

    boost::optional<double> ZoneHVACPackagedTerminalAirConditioner_Impl::autosizedOutdoorAirFlowRateDuringCoolingOperation() const {
      return getAutosizedValue("Design Size Outdoor Air Flow Rate During Cooling Operation", "m3/s");
    }

    boost::optional<double> ZoneHVACPackagedTerminalAirConditioner_Impl::autosizedOutdoorAirFlowRateDuringHeatingOperation() const {
      return getAutosizedValue("Design Size Outdoor Air Flow Rate During Heating Operation", "m3/s");
    }

    boost::optional<double> ZoneHVACPackagedTerminalAirConditioner_Impl::autosizedOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
      return getAutosizedValue("Design Size Outdoor Air Flow Rate When No Cooling or Heating is Needed", "m3/s");
    }

    void ZoneHVACPackagedTerminalAirConditioner_Impl::autosize() {
      autosizeSupplyAirFlowRateDuringCoolingOperation();
      autosizeSupplyAirFlowRateDuringHeatingOperation();
      autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
      autosizeOutdoorAirFlowRateDuringCoolingOperation();
      autosizeOutdoorAirFlowRateDuringHeatingOperation();
      autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
    }

    void ZoneHVACPackagedTerminalAirConditioner_Impl::applySizingValues() {
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

      val = autosizedOutdoorAirFlowRateDuringCoolingOperation();
      if (val) {
        setOutdoorAirFlowRateDuringCoolingOperation(val.get());
      }

      val = autosizedOutdoorAirFlowRateDuringHeatingOperation();
      if (val) {
        setOutdoorAirFlowRateDuringHeatingOperation(val.get());
      }

      val = autosizedOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
      if (val) {
        setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(val.get());
      }
    }

    ComponentType ZoneHVACPackagedTerminalAirConditioner_Impl::componentType() const {
      return ComponentType::Both;
    }

    std::vector<FuelType> ZoneHVACPackagedTerminalAirConditioner_Impl::coolingFuelTypes() const {
      std::set<FuelType> result;
      for (auto ft : coolingCoil().coolingFuelTypes()) {
        result.insert(ft);
      }
      return {result.begin(), result.end()};
    }

    std::vector<FuelType> ZoneHVACPackagedTerminalAirConditioner_Impl::heatingFuelTypes() const {
      std::set<FuelType> result;
      for (auto ft : heatingCoil().heatingFuelTypes()) {
        result.insert(ft);
      }
      return {result.begin(), result.end()};
    }

    std::vector<AppGFuelType> ZoneHVACPackagedTerminalAirConditioner_Impl::appGHeatingFuelTypes() const {
      std::set<AppGFuelType> result;
      for (auto ft : heatingCoil().appGHeatingFuelTypes()) {
        result.insert(ft);
      }
      return {result.begin(), result.end()};
    }

  }  // namespace detail

  ZoneHVACPackagedTerminalAirConditioner::ZoneHVACPackagedTerminalAirConditioner(const Model& model, Schedule& availabilitySchedule,
                                                                                 HVACComponent& supplyAirFan, HVACComponent& heatingCoil,
                                                                                 HVACComponent& coolingCoil)
    : ZoneHVACComponent(ZoneHVACPackagedTerminalAirConditioner::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>());

    bool ok = setAvailabilitySchedule(availabilitySchedule);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s availability schedule to " << availabilitySchedule.briefDescription() << ".");
    }
    setOutdoorAirMixerObjectType("OutdoorAir:Mixer");
    setSupplyAirFan(supplyAirFan);
    setHeatingCoil(heatingCoil);
    setCoolingCoil(coolingCoil);
    setFanPlacement("DrawThrough");
    // When Blank, E+ defaults to 0 (cycling). If it's a FanConstantVolume though, you must provide an always On or E+ will Fatal.
    if (supplyAirFan.iddObjectType() == IddObjectType::OS_Fan_ConstantVolume) {
      auto alwaysOn = model.alwaysOnDiscreteSchedule();
      ok = setSupplyAirFanOperatingModeSchedule(alwaysOn);
      OS_ASSERT(ok);
    } else {
      auto alwaysOff = model.alwaysOffDiscreteSchedule();
      ok = setSupplyAirFanOperatingModeSchedule(alwaysOff);
      OS_ASSERT(ok);
    }

    autosizeSupplyAirFlowRateDuringCoolingOperation();
    autosizeSupplyAirFlowRateDuringHeatingOperation();
    autosizeOutdoorAirFlowRateDuringCoolingOperation();
    autosizeOutdoorAirFlowRateDuringHeatingOperation();
    autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
    autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

  IddObjectType ZoneHVACPackagedTerminalAirConditioner::iddObjectType() {
    return IddObjectType::OS_ZoneHVAC_PackagedTerminalAirConditioner;
  }

  std::vector<std::string> ZoneHVACPackagedTerminalAirConditioner::outdoorAirMixerObjectTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirMixerObjectType);
  }

  std::vector<std::string> ZoneHVACPackagedTerminalAirConditioner::validOutdoorAirMixerObjectTypeValues() {
    return ZoneHVACPackagedTerminalAirConditioner::outdoorAirMixerObjectTypeValues();
  }

  std::vector<std::string> ZoneHVACPackagedTerminalAirConditioner::fanPlacementValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_ZoneHVAC_PackagedTerminalAirConditionerFields::FanPlacement);
  }

  std::vector<std::string> ZoneHVACPackagedTerminalAirConditioner::validFanPlacementValues() {
    return ZoneHVACPackagedTerminalAirConditioner::fanPlacementValues();
  }

  std::string ZoneHVACPackagedTerminalAirConditioner::outdoorAirMixerObjectType() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->outdoorAirMixerObjectType();
  }

  std::string ZoneHVACPackagedTerminalAirConditioner::outdoorAirMixerName() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->outdoorAirMixerName();
  }

  boost::optional<double> ZoneHVACPackagedTerminalAirConditioner::supplyAirFlowRateDuringCoolingOperation() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->supplyAirFlowRateDuringCoolingOperation();
  }

  bool ZoneHVACPackagedTerminalAirConditioner::isSupplyAirFlowRateDuringCoolingOperationAutosized() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->isSupplyAirFlowRateDuringCoolingOperationAutosized();
  }

  boost::optional<double> ZoneHVACPackagedTerminalAirConditioner::supplyAirFlowRateDuringHeatingOperation() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->supplyAirFlowRateDuringHeatingOperation();
  }

  bool ZoneHVACPackagedTerminalAirConditioner::isSupplyAirFlowRateDuringHeatingOperationAutosized() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->isSupplyAirFlowRateDuringHeatingOperationAutosized();
  }

  boost::optional<double> ZoneHVACPackagedTerminalAirConditioner::supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->supplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

  bool ZoneHVACPackagedTerminalAirConditioner::isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized();
  }

  boost::optional<double> ZoneHVACPackagedTerminalAirConditioner::outdoorAirFlowRateDuringCoolingOperation() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->outdoorAirFlowRateDuringCoolingOperation();
  }

  bool ZoneHVACPackagedTerminalAirConditioner::isOutdoorAirFlowRateDuringCoolingOperationAutosized() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->isOutdoorAirFlowRateDuringCoolingOperationAutosized();
  }

  boost::optional<double> ZoneHVACPackagedTerminalAirConditioner::outdoorAirFlowRateDuringHeatingOperation() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->outdoorAirFlowRateDuringHeatingOperation();
  }

  bool ZoneHVACPackagedTerminalAirConditioner::isOutdoorAirFlowRateDuringHeatingOperationAutosized() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->isOutdoorAirFlowRateDuringHeatingOperationAutosized();
  }

  boost::optional<double> ZoneHVACPackagedTerminalAirConditioner::outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

  bool ZoneHVACPackagedTerminalAirConditioner::isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized();
  }

  std::string ZoneHVACPackagedTerminalAirConditioner::fanPlacement() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->fanPlacement();
  }

  bool ZoneHVACPackagedTerminalAirConditioner::isFanPlacementDefaulted() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->isFanPlacementDefaulted();
  }

  bool ZoneHVACPackagedTerminalAirConditioner::setOutdoorAirMixerObjectType(const std::string& outdoorAirMixerObjectType) {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setOutdoorAirMixerObjectType(outdoorAirMixerObjectType);
  }

  bool ZoneHVACPackagedTerminalAirConditioner::setOutdoorAirMixerName(const std::string& outdoorAirMixerName) {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setOutdoorAirMixerName(outdoorAirMixerName);
  }

  bool ZoneHVACPackagedTerminalAirConditioner::setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation) {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setSupplyAirFlowRateDuringCoolingOperation(
      supplyAirFlowRateDuringCoolingOperation);
  }

  void ZoneHVACPackagedTerminalAirConditioner::autosizeSupplyAirFlowRateDuringCoolingOperation() {
    getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->autosizeSupplyAirFlowRateDuringCoolingOperation();
  }

  bool ZoneHVACPackagedTerminalAirConditioner::setSupplyAirFlowRateDuringHeatingOperation(double supplyAirFlowRateDuringHeatingOperation) {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setSupplyAirFlowRateDuringHeatingOperation(
      supplyAirFlowRateDuringHeatingOperation);
  }

  void ZoneHVACPackagedTerminalAirConditioner::autosizeSupplyAirFlowRateDuringHeatingOperation() {
    getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->autosizeSupplyAirFlowRateDuringHeatingOperation();
  }

  bool ZoneHVACPackagedTerminalAirConditioner::setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(
    double supplyAirFlowRateWhenNoCoolingorHeatingisNeeded) {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(
      supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);
  }

  void ZoneHVACPackagedTerminalAirConditioner::resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

  void ZoneHVACPackagedTerminalAirConditioner::autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

  bool ZoneHVACPackagedTerminalAirConditioner::setOutdoorAirFlowRateDuringCoolingOperation(double outdoorAirFlowRateDuringCoolingOperation) {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setOutdoorAirFlowRateDuringCoolingOperation(
      outdoorAirFlowRateDuringCoolingOperation);
  }

  void ZoneHVACPackagedTerminalAirConditioner::autosizeOutdoorAirFlowRateDuringCoolingOperation() {
    getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->autosizeOutdoorAirFlowRateDuringCoolingOperation();
  }

  bool ZoneHVACPackagedTerminalAirConditioner::setOutdoorAirFlowRateDuringHeatingOperation(double outdoorAirFlowRateDuringHeatingOperation) {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setOutdoorAirFlowRateDuringHeatingOperation(
      outdoorAirFlowRateDuringHeatingOperation);
  }

  void ZoneHVACPackagedTerminalAirConditioner::autosizeOutdoorAirFlowRateDuringHeatingOperation() {
    getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->autosizeOutdoorAirFlowRateDuringHeatingOperation();
  }

  bool ZoneHVACPackagedTerminalAirConditioner::setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(
    double outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(
      outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);
  }

  void ZoneHVACPackagedTerminalAirConditioner::resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

  void ZoneHVACPackagedTerminalAirConditioner::autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

  bool ZoneHVACPackagedTerminalAirConditioner::setFanPlacement(const std::string& fanPlacement) {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setFanPlacement(fanPlacement);
  }

  void ZoneHVACPackagedTerminalAirConditioner::resetFanPlacement() {
    getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->resetFanPlacement();
  }

  ZoneHVACPackagedTerminalAirConditioner::ZoneHVACPackagedTerminalAirConditioner(
    std::shared_ptr<detail::ZoneHVACPackagedTerminalAirConditioner_Impl> impl)
    : ZoneHVACComponent(std::move(impl)) {}

  Schedule ZoneHVACPackagedTerminalAirConditioner::supplyAirFanOperatingModeSchedule() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->supplyAirFanOperatingModeSchedule();
  }

  bool ZoneHVACPackagedTerminalAirConditioner::setSupplyAirFanOperatingModeSchedule(Schedule& schedule) {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setSupplyAirFanOperatingModeSchedule(schedule);
  }

  Schedule ZoneHVACPackagedTerminalAirConditioner::availabilitySchedule() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->availabilitySchedule();
  }

  bool ZoneHVACPackagedTerminalAirConditioner::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setAvailabilitySchedule(schedule);
  }

  HVACComponent ZoneHVACPackagedTerminalAirConditioner::supplyAirFan() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->supplyAirFan();
  }

  bool ZoneHVACPackagedTerminalAirConditioner::setSupplyAirFan(HVACComponent& fan) {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setSupplyAirFan(fan);
  }

  HVACComponent ZoneHVACPackagedTerminalAirConditioner::heatingCoil() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->heatingCoil();
  }

  bool ZoneHVACPackagedTerminalAirConditioner::setHeatingCoil(HVACComponent& heatingCoil) {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setHeatingCoil(heatingCoil);
  }

  HVACComponent ZoneHVACPackagedTerminalAirConditioner::coolingCoil() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->coolingCoil();
  }

  bool ZoneHVACPackagedTerminalAirConditioner::setCoolingCoil(HVACComponent& coolingCoil) {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setCoolingCoil(coolingCoil);
  }

  boost::optional<double> ZoneHVACPackagedTerminalAirConditioner::autosizedSupplyAirFlowRateDuringCoolingOperation() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->autosizedSupplyAirFlowRateDuringCoolingOperation();
  }

  boost::optional<double> ZoneHVACPackagedTerminalAirConditioner::autosizedSupplyAirFlowRateDuringHeatingOperation() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->autosizedSupplyAirFlowRateDuringHeatingOperation();
  }

  boost::optional<double> ZoneHVACPackagedTerminalAirConditioner::autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

  boost::optional<double> ZoneHVACPackagedTerminalAirConditioner::autosizedOutdoorAirFlowRateDuringCoolingOperation() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->autosizedOutdoorAirFlowRateDuringCoolingOperation();
  }

  boost::optional<double> ZoneHVACPackagedTerminalAirConditioner::autosizedOutdoorAirFlowRateDuringHeatingOperation() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->autosizedOutdoorAirFlowRateDuringHeatingOperation();
  }

  boost::optional<double> ZoneHVACPackagedTerminalAirConditioner::autosizedOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
    return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->autosizedOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

}  // namespace model
}  // namespace openstudio
