/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"

#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "UnitarySystemPerformanceMultispeed.hpp"
#include "UnitarySystemPerformanceMultispeed_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"
#include "utilities/core/Exception.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_AirLoopHVAC_UnitarySystem_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

// Remove when deprecated methods (maximumCyclingRate and co) are removed
#include "../utilities/core/DeprecatedHelpers.hpp"
#include "CoilCoolingDXVariableSpeed.hpp"
#include "CoilCoolingDXVariableSpeed_Impl.hpp"
#include "CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "CoilCoolingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include "CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit.hpp"
#include "CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    AirLoopHVACUnitarySystem_Impl::AirLoopHVACUnitarySystem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirLoopHVACUnitarySystem::iddObjectType());
    }

    AirLoopHVACUnitarySystem_Impl::AirLoopHVACUnitarySystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirLoopHVACUnitarySystem::iddObjectType());
    }

    AirLoopHVACUnitarySystem_Impl::AirLoopHVACUnitarySystem_Impl(const AirLoopHVACUnitarySystem_Impl& other, Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirLoopHVACUnitarySystem_Impl::outputVariableNames() const {
      // TODO: make this non static, and do implement the test cases below
      static const std::vector<std::string> result{
        "Unitary System Fan Part Load Ratio", "Unitary System Compressor Part Load Ratio", "Unitary System Total Cooling Rate",
        "Unitary System Total Heating Rate", "Unitary System Sensible Cooling Rate", "Unitary System Sensible Heating Rate",
        "Unitary System Latent Cooling Rate", "Unitary System Latent Heating Rate", "Unitary System Ancillary Electricity Rate",

        // Load based and SingleZoneVAV control outputs
        "Unitary System Predicted Sensible Load to Setpoint Heat Transfer Rate",
        "Unitary System Predicted Moisture Load to Setpoint Heat Transfer Rate",

        // Two speed coil outputs
        "Unitary System Cycling Ratio", "Unitary System Compressor Speed Ratio",

        // Multi speed coil outputs
        "Unitary System DX Coil Cycling Ratio", "Unitary System DX Coil Speed Ratio", "Unitary System DX Coil Speed Level",
        "Unitary System Electricity Rate", "Unitary System Electricity Energy", "Unitary System Cooling Ancillary Electricity Energy",
        "Unitary System Heating Ancillary Electricity Energy",

        // Multi speed coil outputs(If heat recovery is specified)
        "Unitary System Heat Recovery Rate", "Unitary System Heat Recovery Inlet Temperature", "Unitary System Heat Recovery Outlet Temperature",
        "Unitary System Heat Recovery Fluid Mass Flow Rate", "Unitary System Heat Recovery Energy",

        // Variable speed coils
        "Unitary System Requested Sensible Cooling Rate", "Unitary System Requested Latent Cooling Rate",

        // Water to air heat pump outputs
        "Unitary System Requested Sensible Cooling Rate", "Unitary System Requested Latent Cooling Rate", "Unitary System Requested Heating Rate",
        "Unitary System Water Coil Cycling Ratio", "Unitary System Water Coil Speed Ratio", "Unitary System Water Coil Speed Level"};
      return result;
    }

    IddObjectType AirLoopHVACUnitarySystem_Impl::iddObjectType() const {
      return AirLoopHVACUnitarySystem::iddObjectType();
    }

    std::vector<ScheduleTypeKey> AirLoopHVACUnitarySystem_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      // TODO: Check schedule display names.
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_AirLoopHVAC_UnitarySystemFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("AirLoopHVACUnitarySystem", "Availability"));
      }
      if (std::find(b, e, OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFanOperatingModeScheduleName) != e) {
        result.push_back(ScheduleTypeKey("AirLoopHVACUnitarySystem", "Supply Air Fan Operating Mode"));
      }
      return result;
    }

    ModelObject AirLoopHVACUnitarySystem_Impl::clone(Model model) const {
      // Mimic what StraightComponent_Impl::clone would do (we inherit ZoneHVACComponent here...)
      auto modelObjectClone = ModelObject_Impl::clone(model).cast<AirLoopHVACUnitarySystem>();
      modelObjectClone.setString(modelObjectClone.inletPort(), "");
      modelObjectClone.setString(modelObjectClone.outletPort(), "");

      if (boost::optional<HVACComponent> supplyFan = this->supplyFan()) {
        modelObjectClone.setSupplyFan(supplyFan->clone(model).cast<HVACComponent>());
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
      if (auto designSpec = designSpecificationMultispeedObject()) {
        modelObjectClone.setDesignSpecificationMultispeedObject(designSpec->clone(model).cast<UnitarySystemPerformanceMultispeed>());
      }

      return std::move(modelObjectClone);
    }

    std::vector<ModelObject> AirLoopHVACUnitarySystem_Impl::children() const {
      std::vector<ModelObject> result;

      if (boost::optional<HVACComponent> supplyFan = this->supplyFan()) {
        result.push_back(*supplyFan);
      }
      if (boost::optional<HVACComponent> coolingCoil = this->coolingCoil()) {
        result.push_back(*coolingCoil);
      }
      if (boost::optional<HVACComponent> heatingCoil = this->heatingCoil()) {
        result.push_back(*heatingCoil);
      }
      if (boost::optional<HVACComponent> supplementalHeatingCoil = this->supplementalHeatingCoil()) {
        result.push_back(*supplementalHeatingCoil);
      }
      if (auto designSpec = designSpecificationMultispeedObject()) {
        result.push_back(*designSpec);
      }

      return result;
    }

    std::vector<IdfObject> AirLoopHVACUnitarySystem_Impl::remove() {
      std::vector<IdfObject> result;

      if (boost::optional<HVACComponent> _supplyFan = this->supplyFan()) {
        std::vector<IdfObject> removedFans = _supplyFan->remove();
        result.insert(result.end(), removedFans.begin(), removedFans.end());
      }

      if (boost::optional<HVACComponent> _coolingCoil = this->coolingCoil()) {
        if (boost::optional<PlantLoop> loop = _coolingCoil->plantLoop()) {
          loop->removeDemandBranchWithComponent(*_coolingCoil);
        }
        std::vector<IdfObject> removedCoolingCoils = _coolingCoil->remove();
        result.insert(result.end(), removedCoolingCoils.begin(), removedCoolingCoils.end());
      }

      if (boost::optional<HVACComponent> _heatingCoil = this->heatingCoil()) {
        if (boost::optional<PlantLoop> loop = _heatingCoil->plantLoop()) {
          loop->removeDemandBranchWithComponent(*_heatingCoil);
        }
        std::vector<IdfObject> removedHeatingCoils = _heatingCoil->remove();
        result.insert(result.end(), removedHeatingCoils.begin(), removedHeatingCoils.end());
      }

      if (boost::optional<HVACComponent> _supplementalHeatingCoil = this->supplementalHeatingCoil()) {
        if (boost::optional<PlantLoop> loop = _supplementalHeatingCoil->plantLoop()) {
          loop->removeDemandBranchWithComponent(*_supplementalHeatingCoil);
        }
        std::vector<IdfObject> removedSuppHeatingCoils = _supplementalHeatingCoil->remove();
        result.insert(result.end(), removedSuppHeatingCoils.begin(), removedSuppHeatingCoils.end());
      }

      if (auto designSpec = designSpecificationMultispeedObject()) {
        auto removed = designSpec->remove();
        result.insert(result.end(), removed.begin(), removed.end());
      }

      std::vector<IdfObject> removedUnitarySystem = ZoneHVACComponent_Impl::remove();
      result.insert(result.end(), removedUnitarySystem.begin(), removedUnitarySystem.end());

      return result;
    }

    bool AirLoopHVACUnitarySystem_Impl::addToNode(Node& node) {
      bool result = ZoneHVACComponent_Impl::addToNode(node);

      if (!result) {
        auto _model = node.model();
        auto thisModelObject = getObject<ModelObject>();
        auto t_airLoop = node.airLoopHVAC();
        auto t_oaSystem = node.airLoopHVACOutdoorAirSystem();

        boost::optional<unsigned> componentInletPort;
        boost::optional<unsigned> componentOutletPort;

        boost::optional<HVACComponent> systemStartComponent;
        boost::optional<HVACComponent> systemEndComponent;

        if (node.getImpl<Node_Impl>()->isConnected(thisModelObject)) {
          return false;
        }

        if (t_airLoop && !t_oaSystem) {
          if (t_airLoop->demandComponent(node.handle())) {
            return false;
          }

          systemStartComponent = t_airLoop->supplyInletNode();
          auto nodes = t_airLoop->supplyOutletNodes();
          OS_ASSERT(!nodes.empty());
          if ((nodes.size() == 2u) && (!t_airLoop->supplyComponents(node, nodes[1]).empty())) {
            systemEndComponent = nodes[1];
          } else {
            systemEndComponent = nodes[0];
          }
          OS_ASSERT(systemEndComponent);
          componentInletPort = inletPort();
          componentOutletPort = outletPort();

          removeFromAirLoopHVAC();
        } else if (t_oaSystem) {
          if (t_oaSystem->oaComponent(node.handle())) {
            systemStartComponent = t_oaSystem->outboardOANode();
            systemEndComponent = t_oaSystem.get();
            componentInletPort = inletPort();
            componentOutletPort = outletPort();
          } else if (t_oaSystem->reliefComponent(node.handle())) {
            systemStartComponent = t_oaSystem.get();
            systemEndComponent = t_oaSystem->outboardReliefNode();
            componentInletPort = inletPort();
            componentOutletPort = outletPort();
          }
          removeFromAirLoopHVAC();
        }

        if (systemStartComponent && systemEndComponent && componentOutletPort && componentInletPort) {
          result = HVACComponent_Impl::addToNode(node, systemStartComponent.get(), systemEndComponent.get(), componentInletPort.get(),
                                                 componentOutletPort.get());
        }
      }

      return result;
    }

    unsigned AirLoopHVACUnitarySystem_Impl::inletPort() const {
      return OS_AirLoopHVAC_UnitarySystemFields::AirInletNodeName;
    }

    unsigned AirLoopHVACUnitarySystem_Impl::outletPort() const {
      return OS_AirLoopHVAC_UnitarySystemFields::AirOutletNodeName;
    }

    std::string AirLoopHVACUnitarySystem_Impl::controlType() const {
      boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitarySystemFields::ControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirLoopHVACUnitarySystem_Impl::isControlTypeDefaulted() const {
      return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::ControlType);
    }

    boost::optional<ThermalZone> AirLoopHVACUnitarySystem_Impl::controllingZoneorThermostatLocation() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_AirLoopHVAC_UnitarySystemFields::ControllingZoneorThermostatLocation);
    }

    std::string AirLoopHVACUnitarySystem_Impl::dehumidificationControlType() const {
      boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitarySystemFields::DehumidificationControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirLoopHVACUnitarySystem_Impl::isDehumidificationControlTypeDefaulted() const {
      return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::DehumidificationControlType);
    }

    boost::optional<Schedule> AirLoopHVACUnitarySystem_Impl::availabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirLoopHVAC_UnitarySystemFields::AvailabilityScheduleName);
    }

    boost::optional<HVACComponent> AirLoopHVACUnitarySystem_Impl::supplyFan() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirLoopHVAC_UnitarySystemFields::SupplyFanName);
    }

    boost::optional<std::string> AirLoopHVACUnitarySystem_Impl::fanPlacement() const {
      // No default, and return uninitialized if empty
      return getString(OS_AirLoopHVAC_UnitarySystemFields::FanPlacement, false, true);
    }

    boost::optional<Schedule> AirLoopHVACUnitarySystem_Impl::supplyAirFanOperatingModeSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFanOperatingModeScheduleName);
    }

    bool AirLoopHVACUnitarySystem_Impl::hasHeatingCoil() const {
      return !isEmpty(OS_AirLoopHVAC_UnitarySystemFields::HeatingCoilName);
    }

    boost::optional<HVACComponent> AirLoopHVACUnitarySystem_Impl::heatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirLoopHVAC_UnitarySystemFields::HeatingCoilName);
    }

    double AirLoopHVACUnitarySystem_Impl::dXHeatingCoilSizingRatio() const {
      boost::optional<double> value = getDouble(OS_AirLoopHVAC_UnitarySystemFields::DXHeatingCoilSizingRatio, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirLoopHVACUnitarySystem_Impl::isDXHeatingCoilSizingRatioDefaulted() const {
      return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::DXHeatingCoilSizingRatio);
    }

    bool AirLoopHVACUnitarySystem_Impl::hasCoolingCoil() const {
      return !isEmpty(OS_AirLoopHVAC_UnitarySystemFields::CoolingCoilName);
    }

    boost::optional<HVACComponent> AirLoopHVACUnitarySystem_Impl::coolingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirLoopHVAC_UnitarySystemFields::CoolingCoilName);
    }

    bool AirLoopHVACUnitarySystem_Impl::useDOASDXCoolingCoil() const {
      boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitarySystemFields::UseDOASDXCoolingCoil, true);
      OS_ASSERT(value);
      return openstudio::istringEqual(value.get(), "Yes");
    }

    bool AirLoopHVACUnitarySystem_Impl::isUseDOASDXCoolingCoilDefaulted() const {
      return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::UseDOASDXCoolingCoil);
    }

    double AirLoopHVACUnitarySystem_Impl::dOASDXCoolingCoilLeavingMinimumAirTemperature() const {
      boost::optional<double> value = getDouble(OS_AirLoopHVAC_UnitarySystemFields::DOASDXCoolingCoilLeavingMinimumAirTemperature, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirLoopHVACUnitarySystem_Impl::isDOASDXCoolingCoilLeavingMinimumAirTemperatureDefaulted() const {
      return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::DOASDXCoolingCoilLeavingMinimumAirTemperature);
    }

    bool AirLoopHVACUnitarySystem_Impl::isDOASDXCoolingCoilLeavingMinimumAirTemperatureAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitarySystemFields::DOASDXCoolingCoilLeavingMinimumAirTemperature, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    std::string AirLoopHVACUnitarySystem_Impl::latentLoadControl() const {
      boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitarySystemFields::LatentLoadControl, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirLoopHVACUnitarySystem_Impl::isLatentLoadControlDefaulted() const {
      return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::LatentLoadControl);
    }

    boost::optional<HVACComponent> AirLoopHVACUnitarySystem_Impl::supplementalHeatingCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirLoopHVAC_UnitarySystemFields::SupplementalHeatingCoilName);
    }

    std::string AirLoopHVACUnitarySystem_Impl::supplyAirFlowRateMethodDuringCoolingOperation() const {
      boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateMethodDuringCoolingOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> AirLoopHVACUnitarySystem_Impl::supplyAirFlowRateDuringCoolingOperation() const {
      return getDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringCoolingOperation, true);
    }

    bool AirLoopHVACUnitarySystem_Impl::isSupplyAirFlowRateDuringCoolingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringCoolingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> AirLoopHVACUnitarySystem_Impl::supplyAirFlowRatePerFloorAreaDuringCoolingOperation() const {
      return getDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRatePerFloorAreaDuringCoolingOperation, true);
    }

    boost::optional<double> AirLoopHVACUnitarySystem_Impl::fractionofAutosizedDesignCoolingSupplyAirFlowRate() const {
      return getDouble(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignCoolingSupplyAirFlowRate, true);
    }

    boost::optional<double> AirLoopHVACUnitarySystem_Impl::designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation() const {
      return getDouble(OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation, true);
    }

    std::string AirLoopHVACUnitarySystem_Impl::supplyAirFlowRateMethodDuringHeatingOperation() const {
      boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateMethodDuringHeatingOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> AirLoopHVACUnitarySystem_Impl::supplyAirFlowRateDuringHeatingOperation() const {
      return getDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringHeatingOperation, true);
    }

    bool AirLoopHVACUnitarySystem_Impl::isSupplyAirFlowRateDuringHeatingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringHeatingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> AirLoopHVACUnitarySystem_Impl::supplyAirFlowRatePerFloorAreaduringHeatingOperation() const {
      return getDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRatePerFloorAreaduringHeatingOperation, true);
    }

    boost::optional<double> AirLoopHVACUnitarySystem_Impl::fractionofAutosizedDesignHeatingSupplyAirFlowRate() const {
      return getDouble(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignHeatingSupplyAirFlowRate, true);
    }

    boost::optional<double> AirLoopHVACUnitarySystem_Impl::designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation() const {
      return getDouble(OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation, true);
    }

    boost::optional<std::string> AirLoopHVACUnitarySystem_Impl::supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired() const {
      // No default, and return uninitialized if empty
      return getString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired, false, true);
    }

    boost::optional<double> AirLoopHVACUnitarySystem_Impl::supplyAirFlowRateWhenNoCoolingorHeatingisRequired() const {
      return getDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateWhenNoCoolingorHeatingisRequired, true);
    }

    bool AirLoopHVACUnitarySystem_Impl::isSupplyAirFlowRateWhenNoCoolingorHeatingisRequiredAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateWhenNoCoolingorHeatingisRequired, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    boost::optional<double> AirLoopHVACUnitarySystem_Impl::supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired() const {
      return getDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired, true);
    }

    boost::optional<double> AirLoopHVACUnitarySystem_Impl::fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() const {
      return getDouble(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired, true);
    }

    boost::optional<double> AirLoopHVACUnitarySystem_Impl::fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() const {
      return getDouble(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired, true);
    }

    boost::optional<double>
      AirLoopHVACUnitarySystem_Impl::designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired() const {
      return getDouble(
        OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired, true);
    }

    boost::optional<double>
      AirLoopHVACUnitarySystem_Impl::designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired() const {
      return getDouble(
        OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired, true);
    }

    boost::optional<double> AirLoopHVACUnitarySystem_Impl::maximumSupplyAirTemperature() const {
      return getDouble(OS_AirLoopHVAC_UnitarySystemFields::MaximumSupplyAirTemperature, true);
    }

    bool AirLoopHVACUnitarySystem_Impl::isMaximumSupplyAirTemperatureDefaulted() const {
      return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::MaximumSupplyAirTemperature);
    }

    bool AirLoopHVACUnitarySystem_Impl::isMaximumSupplyAirTemperatureAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitarySystemFields::MaximumSupplyAirTemperature, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    double AirLoopHVACUnitarySystem_Impl::maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const {
      boost::optional<double> value =
        getDouble(OS_AirLoopHVAC_UnitarySystemFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirLoopHVACUnitarySystem_Impl::isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const {
      return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
    }

    boost::optional<std::string> AirLoopHVACUnitarySystem_Impl::outdoorDryBulbTemperatureSensorNodeName() const {
      return getString(OS_AirLoopHVAC_UnitarySystemFields::OutdoorDryBulbTemperatureSensorNodeName, true);
    }

    double AirLoopHVACUnitarySystem_Impl::ancilliaryOnCycleElectricPower() const {
      boost::optional<double> value = getDouble(OS_AirLoopHVAC_UnitarySystemFields::AncilliaryOnCycleElectricPower, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirLoopHVACUnitarySystem_Impl::isAncilliaryOnCycleElectricPowerDefaulted() const {
      return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::AncilliaryOnCycleElectricPower);
    }

    double AirLoopHVACUnitarySystem_Impl::ancilliaryOffCycleElectricPower() const {
      boost::optional<double> value = getDouble(OS_AirLoopHVAC_UnitarySystemFields::AncilliaryOffCycleElectricPower, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirLoopHVACUnitarySystem_Impl::isAncilliaryOffCycleElectricPowerDefaulted() const {
      return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::AncilliaryOffCycleElectricPower);
    }

    // double AirLoopHVACUnitarySystem_Impl::designHeatRecoveryWaterFlowRate() const {
    //   boost::optional<double> value = getDouble(OS_AirLoopHVAC_UnitarySystemFields::DesignHeatRecoveryWaterFlowRate,true);
    //   OS_ASSERT(value);
    //   return value.get();
    // }

    // bool AirLoopHVACUnitarySystem_Impl::isDesignHeatRecoveryWaterFlowRateDefaulted() const {
    //   return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::DesignHeatRecoveryWaterFlowRate);
    // }

    // double AirLoopHVACUnitarySystem_Impl::maximumTemperatureforHeatRecovery() const {
    //   boost::optional<double> value = getDouble(OS_AirLoopHVAC_UnitarySystemFields::MaximumTemperatureforHeatRecovery,true);
    //   OS_ASSERT(value);
    //   return value.get();
    // }

    // bool AirLoopHVACUnitarySystem_Impl::isMaximumTemperatureforHeatRecoveryDefaulted() const {
    //   return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::MaximumTemperatureforHeatRecovery);
    // }

    boost::optional<UnitarySystemPerformanceMultispeed> AirLoopHVACUnitarySystem_Impl::designSpecificationMultispeedObject() const {
      return getObject<ModelObject>().getModelObjectTarget<UnitarySystemPerformanceMultispeed>(
        OS_AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectName);
    }

    bool AirLoopHVACUnitarySystem_Impl::setControlType(const std::string& controlType) {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::ControlType, controlType);
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetControlType() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::ControlType, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setControllingZoneorThermostatLocation(const boost::optional<ThermalZone>& thermalZone) {
      bool result(false);
      if (thermalZone) {
        result = setPointer(OS_AirLoopHVAC_UnitarySystemFields::ControllingZoneorThermostatLocation, thermalZone.get().handle());
      } else {
        resetControllingZoneorThermostatLocation();
        result = true;
      }
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetControllingZoneorThermostatLocation() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::ControllingZoneorThermostatLocation, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setDehumidificationControlType(const std::string& dehumidificationControlType) {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::DehumidificationControlType, dehumidificationControlType);
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetDehumidificationControlType() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::DehumidificationControlType, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirLoopHVAC_UnitarySystemFields::AvailabilityScheduleName, "AirLoopHVACUnitarySystem", "Availability", schedule);
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetAvailabilitySchedule() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::AvailabilityScheduleName, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setSupplyFan(const boost::optional<HVACComponent>& supplyFan) {
      bool result(false);
      if (supplyFan) {
        result = setPointer(OS_AirLoopHVAC_UnitarySystemFields::SupplyFanName, supplyFan.get().handle());
      } else {
        resetSupplyFan();
        result = true;
      }
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetSupplyFan() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyFanName, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setFanPlacement(boost::optional<std::string> fanPlacement) {
      bool result(false);
      if (fanPlacement) {
        result = setString(OS_AirLoopHVAC_UnitarySystemFields::FanPlacement, fanPlacement.get());
      } else {
        resetFanPlacement();
        result = true;
      }
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetFanPlacement() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::FanPlacement, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setSupplyAirFanOperatingModeSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFanOperatingModeScheduleName, "AirLoopHVACUnitarySystem",
                                "Supply Air Fan Operating Mode", schedule);
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetSupplyAirFanOperatingModeSchedule() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFanOperatingModeScheduleName, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setHeatingCoil(const HVACComponent& heatingCoil) {
      const bool result = setPointer(OS_AirLoopHVAC_UnitarySystemFields::HeatingCoilName, heatingCoil.handle());
      if (openstudio::istringEqual("None", supplyAirFlowRateMethodDuringHeatingOperation())) {
        autosizeSupplyAirFlowRateDuringHeatingOperation();
        OS_ASSERT(setSupplyAirFlowRateMethodDuringHeatingOperation("SupplyAirFlowRate"));
      }
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetHeatingCoil() {
      const bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::HeatingCoilName, "");
      OS_ASSERT(result);
      OS_ASSERT(setSupplyAirFlowRateMethodDuringHeatingOperation("None"));
      resetSupplyAirFlowRateDuringHeatingOperation();
      resetSupplyAirFlowRatePerFloorAreaduringHeatingOperation();
      resetFractionofAutosizedDesignHeatingSupplyAirFlowRate();
      resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation();
    }

    bool AirLoopHVACUnitarySystem_Impl::setDXHeatingCoilSizingRatio(double dXHeatingCoilSizingRatio) {
      bool result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::DXHeatingCoilSizingRatio, dXHeatingCoilSizingRatio);
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetDXHeatingCoilSizingRatio() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::DXHeatingCoilSizingRatio, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setCoolingCoil(const HVACComponent& coolingCoil) {
      const bool result = setPointer(OS_AirLoopHVAC_UnitarySystemFields::CoolingCoilName, coolingCoil.handle());
      if (openstudio::istringEqual("None", supplyAirFlowRateMethodDuringCoolingOperation())) {
        autosizeSupplyAirFlowRateDuringCoolingOperation();
        OS_ASSERT(setSupplyAirFlowRateMethodDuringCoolingOperation("SupplyAirFlowRate"));
      }
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetCoolingCoil() {
      const bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::CoolingCoilName, "");
      OS_ASSERT(result);
      OS_ASSERT(setSupplyAirFlowRateMethodDuringCoolingOperation("None"));
      resetSupplyAirFlowRateDuringCoolingOperation();
      resetSupplyAirFlowRatePerFloorAreaDuringCoolingOperation();
      resetFractionofAutosizedDesignCoolingSupplyAirFlowRate();
      resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation();
    }

    bool AirLoopHVACUnitarySystem_Impl::setUseDOASDXCoolingCoil(bool useDOASDXCoolingCoil) {
      return setBooleanFieldValue(OS_AirLoopHVAC_UnitarySystemFields::UseDOASDXCoolingCoil, useDOASDXCoolingCoil);
    }

    void AirLoopHVACUnitarySystem_Impl::resetUseDOASDXCoolingCoil() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::UseDOASDXCoolingCoil, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setDOASDXCoolingCoilLeavingMinimumAirTemperature(double dOASDXCoolingCoilLeavingMinimumAirTemperature) {
      bool result =
        setDouble(OS_AirLoopHVAC_UnitarySystemFields::DOASDXCoolingCoilLeavingMinimumAirTemperature, dOASDXCoolingCoilLeavingMinimumAirTemperature);
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetDOASDXCoolingCoilLeavingMinimumAirTemperature() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::DOASDXCoolingCoilLeavingMinimumAirTemperature, "");
      OS_ASSERT(result);
    }

    void AirLoopHVACUnitarySystem_Impl::autosizeDOASDXCoolingCoilLeavingMinimumAirTemperature() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::DOASDXCoolingCoilLeavingMinimumAirTemperature, "Autosize");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setLatentLoadControl(const std::string& latentLoadControl) {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::LatentLoadControl, latentLoadControl);
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetLatentLoadControl() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::LatentLoadControl, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setSupplementalHeatingCoil(const boost::optional<HVACComponent>& supplementalHeatingCoil) {
      bool result(false);
      if (supplementalHeatingCoil) {
        result = setPointer(OS_AirLoopHVAC_UnitarySystemFields::SupplementalHeatingCoilName, supplementalHeatingCoil.get().handle());
      } else {
        resetSupplementalHeatingCoil();
        result = true;
      }
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetSupplementalHeatingCoil() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplementalHeatingCoilName, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setSupplyAirFlowRateMethodDuringCoolingOperation(
      const std::string& supplyAirFlowRateMethodDuringCoolingOperation) {
      const bool result =
        setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateMethodDuringCoolingOperation, supplyAirFlowRateMethodDuringCoolingOperation);
      return result;
    }

    bool AirLoopHVACUnitarySystem_Impl::setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation) {
      const bool result =
        setDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringCoolingOperation, supplyAirFlowRateDuringCoolingOperation);
      OS_ASSERT(setSupplyAirFlowRateMethodDuringCoolingOperation("SupplyAirFlowRate"));
      // resetSupplyAirFlowRateDuringCoolingOperation();
      resetSupplyAirFlowRatePerFloorAreaDuringCoolingOperation();
      resetFractionofAutosizedDesignCoolingSupplyAirFlowRate();
      resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation();
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetSupplyAirFlowRateDuringCoolingOperation() {
      const bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringCoolingOperation, "");
      OS_ASSERT(result);
    }

    void AirLoopHVACUnitarySystem_Impl::autosizeSupplyAirFlowRateDuringCoolingOperation() {
      const bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringCoolingOperation, "Autosize");
      OS_ASSERT(result);
      OS_ASSERT(setSupplyAirFlowRateMethodDuringCoolingOperation("SupplyAirFlowRate"));
      // resetSupplyAirFlowRateDuringCoolingOperation();
      resetSupplyAirFlowRatePerFloorAreaDuringCoolingOperation();
      resetFractionofAutosizedDesignCoolingSupplyAirFlowRate();
      resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation();
    }

    bool AirLoopHVACUnitarySystem_Impl::setSupplyAirFlowRatePerFloorAreaDuringCoolingOperation(
      double supplyAirFlowRatePerFloorAreaDuringCoolingOperation) {
      const bool result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRatePerFloorAreaDuringCoolingOperation,
                                    supplyAirFlowRatePerFloorAreaDuringCoolingOperation);
      OS_ASSERT(setSupplyAirFlowRateMethodDuringCoolingOperation("FlowPerFloorArea"));
      resetSupplyAirFlowRateDuringCoolingOperation();
      // resetSupplyAirFlowRatePerFloorAreaDuringCoolingOperation();
      resetFractionofAutosizedDesignCoolingSupplyAirFlowRate();
      resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation();
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetSupplyAirFlowRatePerFloorAreaDuringCoolingOperation() {
      const bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRatePerFloorAreaDuringCoolingOperation, "");
      OS_ASSERT(result);
    }

    bool
      AirLoopHVACUnitarySystem_Impl::setFractionofAutosizedDesignCoolingSupplyAirFlowRate(double fractionofAutosizedDesignCoolingSupplyAirFlowRate) {
      const bool result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignCoolingSupplyAirFlowRate,
                                    fractionofAutosizedDesignCoolingSupplyAirFlowRate);
      OS_ASSERT(setSupplyAirFlowRateMethodDuringCoolingOperation("FractionOfAutosizedCoolingValue"));
      resetSupplyAirFlowRateDuringCoolingOperation();
      resetSupplyAirFlowRatePerFloorAreaDuringCoolingOperation();
      // resetFractionofAutosizedDesignCoolingSupplyAirFlowRate();
      resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation();
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetFractionofAutosizedDesignCoolingSupplyAirFlowRate() {
      const bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignCoolingSupplyAirFlowRate, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation(
      double designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation) {
      const bool result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation,
                                    designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation);
      OS_ASSERT(setSupplyAirFlowRateMethodDuringCoolingOperation("FlowPerCoolingCapacity"));
      resetSupplyAirFlowRateDuringCoolingOperation();
      resetSupplyAirFlowRatePerFloorAreaDuringCoolingOperation();
      resetFractionofAutosizedDesignCoolingSupplyAirFlowRate();
      // resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation();
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation() {
      const bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setSupplyAirFlowRateMethodDuringHeatingOperation(
      const std::string& supplyAirFlowRateMethodDuringHeatingOperation) {
      const bool result =
        setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateMethodDuringHeatingOperation, supplyAirFlowRateMethodDuringHeatingOperation);
      return result;
    }

    bool AirLoopHVACUnitarySystem_Impl::setSupplyAirFlowRateDuringHeatingOperation(double supplyAirFlowRateDuringHeatingOperation) {
      const bool result =
        setDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringHeatingOperation, supplyAirFlowRateDuringHeatingOperation);
      OS_ASSERT(setSupplyAirFlowRateMethodDuringHeatingOperation("SupplyAirFlowRate"));
      // resetSupplyAirFlowRateDuringHeatingOperation();
      resetSupplyAirFlowRatePerFloorAreaduringHeatingOperation();
      resetFractionofAutosizedDesignHeatingSupplyAirFlowRate();
      resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation();
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetSupplyAirFlowRateDuringHeatingOperation() {
      const bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringHeatingOperation, "");
      OS_ASSERT(result);
    }

    void AirLoopHVACUnitarySystem_Impl::autosizeSupplyAirFlowRateDuringHeatingOperation() {
      const bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringHeatingOperation, "Autosize");
      OS_ASSERT(result);
      OS_ASSERT(setSupplyAirFlowRateMethodDuringHeatingOperation("SupplyAirFlowRate"));
      // resetSupplyAirFlowRateDuringHeatingOperation();
      resetSupplyAirFlowRatePerFloorAreaduringHeatingOperation();
      resetFractionofAutosizedDesignHeatingSupplyAirFlowRate();
      resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation();
    }

    bool AirLoopHVACUnitarySystem_Impl::setSupplyAirFlowRatePerFloorAreaduringHeatingOperation(
      double supplyAirFlowRatePerFloorAreaDuringHeatingOperation) {
      const bool result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRatePerFloorAreaduringHeatingOperation,
                                    supplyAirFlowRatePerFloorAreaDuringHeatingOperation);
      OS_ASSERT(setSupplyAirFlowRateMethodDuringHeatingOperation("FlowPerFloorArea"));
      resetSupplyAirFlowRateDuringHeatingOperation();
      // resetSupplyAirFlowRatePerFloorAreaduringHeatingOperation();
      resetFractionofAutosizedDesignHeatingSupplyAirFlowRate();
      resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation();
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetSupplyAirFlowRatePerFloorAreaduringHeatingOperation() {
      const bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRatePerFloorAreaduringHeatingOperation, "");
      OS_ASSERT(result);
    }

    bool
      AirLoopHVACUnitarySystem_Impl::setFractionofAutosizedDesignHeatingSupplyAirFlowRate(double fractionofAutosizedDesignHeatingSupplyAirFlowRate) {
      const bool result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignHeatingSupplyAirFlowRate,
                                    fractionofAutosizedDesignHeatingSupplyAirFlowRate);
      OS_ASSERT(setSupplyAirFlowRateMethodDuringHeatingOperation("FractionOfAutosizedHeatingValue"));
      resetSupplyAirFlowRateDuringHeatingOperation();
      resetSupplyAirFlowRatePerFloorAreaduringHeatingOperation();
      // resetFractionofAutosizedDesignHeatingSupplyAirFlowRate();
      resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation();
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetFractionofAutosizedDesignHeatingSupplyAirFlowRate() {
      const bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignHeatingSupplyAirFlowRate, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation(
      double designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation) {
      const bool result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation,
                                    designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation);
      OS_ASSERT(setSupplyAirFlowRateMethodDuringHeatingOperation("FlowPerHeatingCapacity"));
      resetSupplyAirFlowRateDuringHeatingOperation();
      resetSupplyAirFlowRatePerFloorAreaduringHeatingOperation();
      resetFractionofAutosizedDesignHeatingSupplyAirFlowRate();
      // resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation();
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation() {
      const bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired(
      boost::optional<std::string> supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired) {
      bool result(false);
      if (supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired) {
        result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired,
                           supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired.get());
      } else {
        resetSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired();
        result = true;
      }
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(
      boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisRequired) {
      bool result(false);
      if (supplyAirFlowRateWhenNoCoolingorHeatingisRequired) {
        result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateWhenNoCoolingorHeatingisRequired,
                           supplyAirFlowRateWhenNoCoolingorHeatingisRequired.get());
        boost::optional<std::string> supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired("SupplyAirFlowRate");
        result = result && setSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired(supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired);
      } else {
        resetSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
        result = true;
      }
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateWhenNoCoolingorHeatingisRequired, "");
      OS_ASSERT(result);
    }

    void AirLoopHVACUnitarySystem_Impl::autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateWhenNoCoolingorHeatingisRequired, "Autosize");
      boost::optional<std::string> supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired("SupplyAirFlowRate");
      result = result && setSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired(supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired);
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setSupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired(
      boost::optional<double> supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired) {
      bool result(false);
      if (supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired) {
        result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired,
                           supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired.get());
        boost::optional<std::string> supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired("FlowPerFloorArea");
        result = result && setSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired(supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired);
      } else {
        resetSupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired();
        result = true;
      }
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetSupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setFractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(
      boost::optional<double> fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired) {
      bool result(false);
      if (fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired) {
        result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired,
                           fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired.get());
        boost::optional<std::string> supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired("FractionOfAutosizedCoolingValue");
        result = result && setSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired(supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired);
      } else {
        resetFractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
        result = true;
      }
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetFractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() {
      bool result =
        setString(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(
      boost::optional<double> fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired) {
      bool result(false);
      if (fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired) {
        result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired,
                           fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired.get());
        boost::optional<std::string> supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired("FractionOfAutosizedHeatingValue");
        result = result && setSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired(supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired);
      } else {
        resetFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
        result = true;
      }
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() {
      bool result =
        setString(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired(
      boost::optional<double> designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired) {
      bool result(false);
      if (designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired) {
        result = setDouble(
          OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired,
          designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired.get());
        boost::optional<std::string> supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired("FlowPerCoolingCapacity");
        result = result && setSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired(supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired);
      } else {
        resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired();
        result = true;
      }
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired() {
      bool result = setString(
        OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired(
      boost::optional<double> designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired) {
      bool result(false);
      if (designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired) {
        result = setDouble(
          OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired,
          designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired.get());
        boost::optional<std::string> supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired("FlowPerHeatingCapacity");
        result = result && setSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired(supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired);
      } else {
        resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired();
        result = true;
      }
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired() {
      bool result = setString(
        OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setMaximumSupplyAirTemperature(boost::optional<double> maximumSupplyAirTemperature) {
      bool result(false);
      if (maximumSupplyAirTemperature) {
        result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::MaximumSupplyAirTemperature, maximumSupplyAirTemperature.get());
      } else {
        resetMaximumSupplyAirTemperature();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetMaximumSupplyAirTemperature() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::MaximumSupplyAirTemperature, "");
      OS_ASSERT(result);
    }

    void AirLoopHVACUnitarySystem_Impl::autosizeMaximumSupplyAirTemperature() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::MaximumSupplyAirTemperature, "Autosize");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(
      double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation) {
      bool result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation,
                              maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
      OS_ASSERT(result);
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setOutdoorDryBulbTemperatureSensorNodeName(
      boost::optional<std::string> outdoorDryBulbTemperatureSensorNodeName) {
      bool result(false);
      if (outdoorDryBulbTemperatureSensorNodeName) {
        result =
          setString(OS_AirLoopHVAC_UnitarySystemFields::OutdoorDryBulbTemperatureSensorNodeName, outdoorDryBulbTemperatureSensorNodeName.get());
      } else {
        resetOutdoorDryBulbTemperatureSensorNodeName();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetOutdoorDryBulbTemperatureSensorNodeName() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::OutdoorDryBulbTemperatureSensorNodeName, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setAncilliaryOnCycleElectricPower(double ancilliaryOnCycleElectricPower) {
      bool result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::AncilliaryOnCycleElectricPower, ancilliaryOnCycleElectricPower);
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetAncilliaryOnCycleElectricPower() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::AncilliaryOnCycleElectricPower, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitarySystem_Impl::setAncilliaryOffCycleElectricPower(double ancilliaryOffCycleElectricPower) {
      bool result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::AncilliaryOffCycleElectricPower, ancilliaryOffCycleElectricPower);
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetAncilliaryOffCycleElectricPower() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::AncilliaryOffCycleElectricPower, "");
      OS_ASSERT(result);
    }

    // bool AirLoopHVACUnitarySystem_Impl::setDesignHeatRecoveryWaterFlowRate(double designHeatRecoveryWaterFlowRate) {
    //   bool result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::DesignHeatRecoveryWaterFlowRate, designHeatRecoveryWaterFlowRate);
    //   return result;
    // }

    // void AirLoopHVACUnitarySystem_Impl::resetDesignHeatRecoveryWaterFlowRate() {
    //   bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::DesignHeatRecoveryWaterFlowRate, "");
    //   OS_ASSERT(result);
    // }

    // bool AirLoopHVACUnitarySystem_Impl::setMaximumTemperatureforHeatRecovery(double maximumTemperatureforHeatRecovery) {
    //   bool result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::MaximumTemperatureforHeatRecovery, maximumTemperatureforHeatRecovery);
    //   return result;
    // }

    // void AirLoopHVACUnitarySystem_Impl::resetMaximumTemperatureforHeatRecovery() {
    //   bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::MaximumTemperatureforHeatRecovery, "");
    //   OS_ASSERT(result);
    // }

    bool AirLoopHVACUnitarySystem_Impl::setDesignSpecificationMultispeedObject(
      const boost::optional<UnitarySystemPerformanceMultispeed>& unitarySystemPerformace) {
      bool result(false);
      if (unitarySystemPerformace) {
        result = setPointer(OS_AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectName, unitarySystemPerformace.get().handle());
      } else {
        resetDesignSpecificationMultispeedObject();
        result = true;
      }
      return result;
    }

    void AirLoopHVACUnitarySystem_Impl::resetDesignSpecificationMultispeedObject() {
      bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectName, "");
      OS_ASSERT(result);
    }

    boost::optional<double> AirLoopHVACUnitarySystem_Impl::autosizedSupplyAirFlowRateDuringCoolingOperation() const {
      return getAutosizedValue("Design Size Cooling Supply Air Flow Rate", "m3/s");
    }

    boost::optional<double> AirLoopHVACUnitarySystem_Impl::autosizedSupplyAirFlowRateDuringHeatingOperation() const {
      return getAutosizedValue("Design Size Heating Supply Air Flow Rate", "m3/s");
    }

    boost::optional<double> AirLoopHVACUnitarySystem_Impl::autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() const {
      return getAutosizedValue("Design Size No Load Supply Air Flow Rate", "m3/s");
    }

    boost::optional<double> AirLoopHVACUnitarySystem_Impl::autosizedMaximumSupplyAirTemperature() const {
      return getAutosizedValue("Design Size Maximum Supply Air Temperature", "C");
    }

    boost::optional<double> AirLoopHVACUnitarySystem_Impl::autosizedDOASDXCoolingCoilLeavingMinimumAirTemperature() const {
      return getAutosizedValue("Design Size Minimum Supply Air Temperature", "C");
    }

    void AirLoopHVACUnitarySystem_Impl::autosize() {
      autosizeSupplyAirFlowRateDuringCoolingOperation();
      autosizeSupplyAirFlowRateDuringHeatingOperation();
      autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
      autosizeMaximumSupplyAirTemperature();
    }

    void AirLoopHVACUnitarySystem_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedSupplyAirFlowRateDuringCoolingOperation();
      if (val) {
        setSupplyAirFlowRateDuringCoolingOperation(val.get());
      }

      val = autosizedSupplyAirFlowRateDuringHeatingOperation();
      if (val) {
        setSupplyAirFlowRateDuringHeatingOperation(val.get());
      }

      val = autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
      if (val) {
        setSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(val.get());
      }

      val = autosizedMaximumSupplyAirTemperature();
      if (val) {
        setMaximumSupplyAirTemperature(val.get());
      }
    }

    std::vector<EMSActuatorNames> AirLoopHVACUnitarySystem_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{{"AirLoopHVAC:UnitarySystem", "Autosized Supply Air Flow Rate"},
                                              {"AirLoopHVAC:UnitarySystem", "Autosized Supply Air Flow Rate During Cooling Operation"},
                                              {"AirLoopHVAC:UnitarySystem", "Autosized Supply Air Flow Rate During Heating Operation"},
                                              {"AirLoopHVAC:UnitarySystem", "Autosized Supply Air Flow Rate During No Heating or Cooling Operation"},
                                              {"Unitary HVAC", "Sensible Load Request"},
                                              {"Unitary HVAC", "Moisture Load Request"}};
      return actuators;
    }

    std::vector<std::string> AirLoopHVACUnitarySystem_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types{"Unitary System Control Zone Mass Flow Fraction", "Unitary HVAC Design Heating Capacity",
                                     "Unitary HVAC Design Cooling Capacity"};
      return types;
    }

    ComponentType AirLoopHVACUnitarySystem_Impl::componentType() const {
      const bool has_cooling = coolingCoil().is_initialized();
      const bool has_heating = heatingCoil().is_initialized();

      if (has_cooling && has_heating) {
        return ComponentType::Both;
      } else if (has_cooling) {
        return ComponentType::Cooling;
      } else if (has_heating) {
        return ComponentType::Heating;
      }
      return ComponentType::None;
    }

    std::vector<FuelType> AirLoopHVACUnitarySystem_Impl::coolingFuelTypes() const {
      std::set<FuelType> result;
      if (auto cc_ = coolingCoil()) {
        for (auto ft : cc_->coolingFuelTypes()) {
          result.insert(ft);
        }
      }

      return {result.begin(), result.end()};
    }

    std::vector<FuelType> AirLoopHVACUnitarySystem_Impl::heatingFuelTypes() const {
      std::set<FuelType> result;
      if (auto hc_ = heatingCoil()) {
        for (auto ft : hc_->heatingFuelTypes()) {
          result.insert(ft);
        }
      }
      if (auto supHC_ = supplementalHeatingCoil()) {
        for (auto ft : supHC_->heatingFuelTypes()) {
          result.insert(ft);
        }
      }

      return {result.begin(), result.end()};
    }

    std::vector<AppGFuelType> AirLoopHVACUnitarySystem_Impl::appGHeatingFuelTypes() const {
      std::set<AppGFuelType> result;
      if (auto hc_ = heatingCoil()) {
        for (auto ft : hc_->appGHeatingFuelTypes()) {
          result.insert(ft);
        }
      }
      if (auto supHC_ = supplementalHeatingCoil()) {
        for (auto ft : supHC_->appGHeatingFuelTypes()) {
          result.insert(ft);
        }
      }
      return {result.begin(), result.end()};
    }

  }  // namespace detail

  AirLoopHVACUnitarySystem::AirLoopHVACUnitarySystem(const Model& model) : ZoneHVACComponent(AirLoopHVACUnitarySystem::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirLoopHVACUnitarySystem_Impl>());

    bool ok = true;
    ok = getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setControlType("Load");
    OS_ASSERT(ok);
    ok = setDehumidificationControlType("None");
    OS_ASSERT(ok);
    ok = setDXHeatingCoilSizingRatio(1.0);
    OS_ASSERT(ok);
    setUseDOASDXCoolingCoil(false);
    ok = setDOASDXCoolingCoilLeavingMinimumAirTemperature(2.0);
    OS_ASSERT(ok);
    ok = setLatentLoadControl("SensibleOnlyLoadControl");
    OS_ASSERT(ok);
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setSupplyAirFlowRateMethodDuringCoolingOperation("None");
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setSupplyAirFlowRateMethodDuringHeatingOperation("None");
    autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
    setMaximumSupplyAirTemperature(80.0);
    setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(21.0);
    ok = setAncilliaryOnCycleElectricPower(0.0);
    OS_ASSERT(ok);
    ok = setAncilliaryOffCycleElectricPower(0.0);
    OS_ASSERT(ok);
    // ok = setMaximumTemperatureforHeatRecovery(80.0);
    // OS_ASSERT(ok);
  }

  IddObjectType AirLoopHVACUnitarySystem::iddObjectType() {
    return {IddObjectType::OS_AirLoopHVAC_UnitarySystem};
  }

  std::vector<std::string> AirLoopHVACUnitarySystem::controlTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_AirLoopHVAC_UnitarySystemFields::ControlType);
  }

  std::vector<std::string> AirLoopHVACUnitarySystem::dehumidificationControlTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_AirLoopHVAC_UnitarySystemFields::DehumidificationControlType);
  }

  std::vector<std::string> AirLoopHVACUnitarySystem::fanPlacementValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_AirLoopHVAC_UnitarySystemFields::FanPlacement);
  }

  std::vector<std::string> AirLoopHVACUnitarySystem::latentLoadControlValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_AirLoopHVAC_UnitarySystemFields::LatentLoadControl);
  }

  std::vector<std::string> AirLoopHVACUnitarySystem::supplyAirFlowRateMethodDuringCoolingOperationValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateMethodDuringCoolingOperation);
  }

  std::vector<std::string> AirLoopHVACUnitarySystem::supplyAirFlowRateMethodDuringHeatingOperationValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateMethodDuringHeatingOperation);
  }

  std::vector<std::string> AirLoopHVACUnitarySystem::supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequiredValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired);
  }

  std::string AirLoopHVACUnitarySystem::controlType() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->controlType();
  }

  bool AirLoopHVACUnitarySystem::isControlTypeDefaulted() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isControlTypeDefaulted();
  }

  boost::optional<ThermalZone> AirLoopHVACUnitarySystem::controllingZoneorThermostatLocation() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->controllingZoneorThermostatLocation();
  }

  std::string AirLoopHVACUnitarySystem::dehumidificationControlType() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->dehumidificationControlType();
  }

  bool AirLoopHVACUnitarySystem::isDehumidificationControlTypeDefaulted() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isDehumidificationControlTypeDefaulted();
  }

  boost::optional<Schedule> AirLoopHVACUnitarySystem::availabilitySchedule() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->availabilitySchedule();
  }

  boost::optional<HVACComponent> AirLoopHVACUnitarySystem::supplyFan() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->supplyFan();
  }

  boost::optional<std::string> AirLoopHVACUnitarySystem::fanPlacement() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->fanPlacement();
  }

  boost::optional<Schedule> AirLoopHVACUnitarySystem::supplyAirFanOperatingModeSchedule() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->supplyAirFanOperatingModeSchedule();
  }

  boost::optional<HVACComponent> AirLoopHVACUnitarySystem::heatingCoil() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->heatingCoil();
  }

  double AirLoopHVACUnitarySystem::dXHeatingCoilSizingRatio() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->dXHeatingCoilSizingRatio();
  }

  bool AirLoopHVACUnitarySystem::isDXHeatingCoilSizingRatioDefaulted() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isDXHeatingCoilSizingRatioDefaulted();
  }

  boost::optional<HVACComponent> AirLoopHVACUnitarySystem::coolingCoil() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->coolingCoil();
  }

  bool AirLoopHVACUnitarySystem::useDOASDXCoolingCoil() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->useDOASDXCoolingCoil();
  }

  bool AirLoopHVACUnitarySystem::isUseDOASDXCoolingCoilDefaulted() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isUseDOASDXCoolingCoilDefaulted();
  }

  double AirLoopHVACUnitarySystem::dOASDXCoolingCoilLeavingMinimumAirTemperature() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->dOASDXCoolingCoilLeavingMinimumAirTemperature();
  }

  bool AirLoopHVACUnitarySystem::isDOASDXCoolingCoilLeavingMinimumAirTemperatureDefaulted() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isDOASDXCoolingCoilLeavingMinimumAirTemperatureDefaulted();
  }

  bool AirLoopHVACUnitarySystem::isDOASDXCoolingCoilLeavingMinimumAirTemperatureAutosized() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isDOASDXCoolingCoilLeavingMinimumAirTemperatureAutosized();
  }

  std::string AirLoopHVACUnitarySystem::latentLoadControl() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->latentLoadControl();
  }

  bool AirLoopHVACUnitarySystem::isLatentLoadControlDefaulted() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isLatentLoadControlDefaulted();
  }

  boost::optional<HVACComponent> AirLoopHVACUnitarySystem::supplementalHeatingCoil() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->supplementalHeatingCoil();
  }

  std::string AirLoopHVACUnitarySystem::supplyAirFlowRateMethodDuringCoolingOperation() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->supplyAirFlowRateMethodDuringCoolingOperation();
  }

  boost::optional<double> AirLoopHVACUnitarySystem::supplyAirFlowRateDuringCoolingOperation() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->supplyAirFlowRateDuringCoolingOperation();
  }

  bool AirLoopHVACUnitarySystem::isSupplyAirFlowRateDuringCoolingOperationAutosized() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isSupplyAirFlowRateDuringCoolingOperationAutosized();
  }

  boost::optional<double> AirLoopHVACUnitarySystem::supplyAirFlowRatePerFloorAreaDuringCoolingOperation() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->supplyAirFlowRatePerFloorAreaDuringCoolingOperation();
  }

  boost::optional<double> AirLoopHVACUnitarySystem::fractionofAutosizedDesignCoolingSupplyAirFlowRate() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->fractionofAutosizedDesignCoolingSupplyAirFlowRate();
  }

  boost::optional<double> AirLoopHVACUnitarySystem::designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation();
  }

  std::string AirLoopHVACUnitarySystem::supplyAirFlowRateMethodDuringHeatingOperation() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->supplyAirFlowRateMethodDuringHeatingOperation();
  }

  boost::optional<double> AirLoopHVACUnitarySystem::supplyAirFlowRateDuringHeatingOperation() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->supplyAirFlowRateDuringHeatingOperation();
  }

  bool AirLoopHVACUnitarySystem::isSupplyAirFlowRateDuringHeatingOperationAutosized() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isSupplyAirFlowRateDuringHeatingOperationAutosized();
  }

  boost::optional<double> AirLoopHVACUnitarySystem::supplyAirFlowRatePerFloorAreaduringHeatingOperation() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->supplyAirFlowRatePerFloorAreaduringHeatingOperation();
  }

  boost::optional<double> AirLoopHVACUnitarySystem::fractionofAutosizedDesignHeatingSupplyAirFlowRate() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->fractionofAutosizedDesignHeatingSupplyAirFlowRate();
  }

  boost::optional<double> AirLoopHVACUnitarySystem::designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation();
  }

  boost::optional<std::string> AirLoopHVACUnitarySystem::supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired();
  }

  boost::optional<double> AirLoopHVACUnitarySystem::supplyAirFlowRateWhenNoCoolingorHeatingisRequired() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->supplyAirFlowRateWhenNoCoolingorHeatingisRequired();
  }

  bool AirLoopHVACUnitarySystem::isSupplyAirFlowRateWhenNoCoolingorHeatingisRequiredAutosized() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isSupplyAirFlowRateWhenNoCoolingorHeatingisRequiredAutosized();
  }

  boost::optional<double> AirLoopHVACUnitarySystem::supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired();
  }

  boost::optional<double> AirLoopHVACUnitarySystem::fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
  }

  boost::optional<double> AirLoopHVACUnitarySystem::fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
  }

  boost::optional<double>
    AirLoopHVACUnitarySystem::designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()
      ->designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired();
  }

  boost::optional<double>
    AirLoopHVACUnitarySystem::designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()
      ->designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired();
  }

  boost::optional<double> AirLoopHVACUnitarySystem::maximumSupplyAirTemperature() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->maximumSupplyAirTemperature();
  }

  bool AirLoopHVACUnitarySystem::isMaximumSupplyAirTemperatureDefaulted() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isMaximumSupplyAirTemperatureDefaulted();
  }

  bool AirLoopHVACUnitarySystem::isMaximumSupplyAirTemperatureAutosized() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isMaximumSupplyAirTemperatureAutosized();
  }

  double AirLoopHVACUnitarySystem::maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();
  }

  bool AirLoopHVACUnitarySystem::isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted();
  }

  boost::optional<std::string> AirLoopHVACUnitarySystem::outdoorDryBulbTemperatureSensorNodeName() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->outdoorDryBulbTemperatureSensorNodeName();
  }

  double AirLoopHVACUnitarySystem::ancilliaryOnCycleElectricPower() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->ancilliaryOnCycleElectricPower();
  }

  bool AirLoopHVACUnitarySystem::isAncilliaryOnCycleElectricPowerDefaulted() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isAncilliaryOnCycleElectricPowerDefaulted();
  }

  double AirLoopHVACUnitarySystem::ancilliaryOffCycleElectricPower() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->ancilliaryOffCycleElectricPower();
  }

  bool AirLoopHVACUnitarySystem::isAncilliaryOffCycleElectricPowerDefaulted() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isAncilliaryOffCycleElectricPowerDefaulted();
  }

  // TODO: @kbenne, why is this not exposed?
  // double AirLoopHVACUnitarySystem::designHeatRecoveryWaterFlowRate() const {
  //   return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->designHeatRecoveryWaterFlowRate();
  // }

  // bool AirLoopHVACUnitarySystem::isDesignHeatRecoveryWaterFlowRateDefaulted() const {
  //   return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isDesignHeatRecoveryWaterFlowRateDefaulted();
  // }

  // double AirLoopHVACUnitarySystem::maximumTemperatureforHeatRecovery() const {
  //   return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->maximumTemperatureforHeatRecovery();
  // }

  // bool AirLoopHVACUnitarySystem::isMaximumTemperatureforHeatRecoveryDefaulted() const {
  //   return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isMaximumTemperatureforHeatRecoveryDefaulted();
  // }

  boost::optional<UnitarySystemPerformanceMultispeed> AirLoopHVACUnitarySystem::designSpecificationMultispeedObject() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->designSpecificationMultispeedObject();
  }

  bool AirLoopHVACUnitarySystem::setControlType(const std::string& controlType) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setControlType(controlType);
  }

  void AirLoopHVACUnitarySystem::resetControlType() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetControlType();
  }

  bool AirLoopHVACUnitarySystem::setControllingZoneorThermostatLocation(const ThermalZone& thermalZone) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setControllingZoneorThermostatLocation(thermalZone);
  }

  void AirLoopHVACUnitarySystem::resetControllingZoneorThermostatLocation() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetControllingZoneorThermostatLocation();
  }

  bool AirLoopHVACUnitarySystem::setDehumidificationControlType(const std::string& dehumidificationControlType) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setDehumidificationControlType(dehumidificationControlType);
  }

  void AirLoopHVACUnitarySystem::resetDehumidificationControlType() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetDehumidificationControlType();
  }

  bool AirLoopHVACUnitarySystem::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setAvailabilitySchedule(schedule);
  }

  void AirLoopHVACUnitarySystem::resetAvailabilitySchedule() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetAvailabilitySchedule();
  }

  bool AirLoopHVACUnitarySystem::setSupplyFan(const HVACComponent& supplyFan) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setSupplyFan(supplyFan);
  }

  void AirLoopHVACUnitarySystem::resetSupplyFan() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetSupplyFan();
  }

  bool AirLoopHVACUnitarySystem::setFanPlacement(const std::string& fanPlacement) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setFanPlacement(fanPlacement);
  }

  void AirLoopHVACUnitarySystem::resetFanPlacement() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetFanPlacement();
  }

  bool AirLoopHVACUnitarySystem::setSupplyAirFanOperatingModeSchedule(Schedule& schedule) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setSupplyAirFanOperatingModeSchedule(schedule);
  }

  void AirLoopHVACUnitarySystem::resetSupplyAirFanOperatingModeSchedule() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetSupplyAirFanOperatingModeSchedule();
  }

  bool AirLoopHVACUnitarySystem::setHeatingCoil(const HVACComponent& heatingCoil) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setHeatingCoil(heatingCoil);
  }

  void AirLoopHVACUnitarySystem::resetHeatingCoil() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetHeatingCoil();
  }

  bool AirLoopHVACUnitarySystem::setDXHeatingCoilSizingRatio(double dXHeatingCoilSizingRatio) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setDXHeatingCoilSizingRatio(dXHeatingCoilSizingRatio);
  }

  void AirLoopHVACUnitarySystem::resetDXHeatingCoilSizingRatio() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetDXHeatingCoilSizingRatio();
  }

  bool AirLoopHVACUnitarySystem::setCoolingCoil(const HVACComponent& coolingCoil) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setCoolingCoil(coolingCoil);
  }

  void AirLoopHVACUnitarySystem::resetCoolingCoil() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetCoolingCoil();
  }

  bool AirLoopHVACUnitarySystem::setUseDOASDXCoolingCoil(bool useDOASDXCoolingCoil) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setUseDOASDXCoolingCoil(useDOASDXCoolingCoil);
  }

  void AirLoopHVACUnitarySystem::resetUseDOASDXCoolingCoil() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetUseDOASDXCoolingCoil();
  }

  bool AirLoopHVACUnitarySystem::setDOASDXCoolingCoilLeavingMinimumAirTemperature(double dOASDXCoolingCoilLeavingMinimumAirTemperature) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setDOASDXCoolingCoilLeavingMinimumAirTemperature(
      dOASDXCoolingCoilLeavingMinimumAirTemperature);
  }

  void AirLoopHVACUnitarySystem::resetDOASDXCoolingCoilLeavingMinimumAirTemperature() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetDOASDXCoolingCoilLeavingMinimumAirTemperature();
  }

  void AirLoopHVACUnitarySystem::autosizeDOASDXCoolingCoilLeavingMinimumAirTemperature() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->autosizeDOASDXCoolingCoilLeavingMinimumAirTemperature();
  }

  bool AirLoopHVACUnitarySystem::setLatentLoadControl(const std::string& latentLoadControl) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setLatentLoadControl(latentLoadControl);
  }

  void AirLoopHVACUnitarySystem::resetLatentLoadControl() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetLatentLoadControl();
  }

  bool AirLoopHVACUnitarySystem::setSupplementalHeatingCoil(const HVACComponent& supplementalHeatingCoil) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setSupplementalHeatingCoil(supplementalHeatingCoil);
  }

  void AirLoopHVACUnitarySystem::resetSupplementalHeatingCoil() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetSupplementalHeatingCoil();
  }

  bool AirLoopHVACUnitarySystem::setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setSupplyAirFlowRateDuringCoolingOperation(supplyAirFlowRateDuringCoolingOperation);
  }

  void AirLoopHVACUnitarySystem::autosizeSupplyAirFlowRateDuringCoolingOperation() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->autosizeSupplyAirFlowRateDuringCoolingOperation();
  }

  bool AirLoopHVACUnitarySystem::setSupplyAirFlowRatePerFloorAreaDuringCoolingOperation(double supplyAirFlowRatePerFloorAreaDuringCoolingOperation) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setSupplyAirFlowRatePerFloorAreaDuringCoolingOperation(
      supplyAirFlowRatePerFloorAreaDuringCoolingOperation);
  }

  bool AirLoopHVACUnitarySystem::setFractionofAutosizedDesignCoolingSupplyAirFlowRate(double fractionofAutosizedDesignCoolingSupplyAirFlowRate) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setFractionofAutosizedDesignCoolingSupplyAirFlowRate(
      fractionofAutosizedDesignCoolingSupplyAirFlowRate);
  }

  bool AirLoopHVACUnitarySystem::setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation(
    double designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation(
      designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation);
  }

  bool AirLoopHVACUnitarySystem::setSupplyAirFlowRateDuringHeatingOperation(double supplyAirFlowRateDuringHeatingOperation) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setSupplyAirFlowRateDuringHeatingOperation(supplyAirFlowRateDuringHeatingOperation);
  }

  void AirLoopHVACUnitarySystem::autosizeSupplyAirFlowRateDuringHeatingOperation() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->autosizeSupplyAirFlowRateDuringHeatingOperation();
  }

  bool AirLoopHVACUnitarySystem::setSupplyAirFlowRatePerFloorAreaduringHeatingOperation(double supplyAirFlowRatePerFloorAreaduringHeatingOperation) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setSupplyAirFlowRatePerFloorAreaduringHeatingOperation(
      supplyAirFlowRatePerFloorAreaduringHeatingOperation);
  }

  bool AirLoopHVACUnitarySystem::setFractionofAutosizedDesignHeatingSupplyAirFlowRate(double fractionofAutosizedDesignHeatingSupplyAirFlowRate) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setFractionofAutosizedDesignHeatingSupplyAirFlowRate(
      fractionofAutosizedDesignHeatingSupplyAirFlowRate);
  }

  bool AirLoopHVACUnitarySystem::setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation(
    double designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation(
      designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation);
  }

  bool AirLoopHVACUnitarySystem::setSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired(
    const std::string& supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired(
      supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired);
  }

  void AirLoopHVACUnitarySystem::resetSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired();
  }

  bool AirLoopHVACUnitarySystem::setSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(double supplyAirFlowRateWhenNoCoolingorHeatingisRequired) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(
      supplyAirFlowRateWhenNoCoolingorHeatingisRequired);
  }

  void AirLoopHVACUnitarySystem::resetSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
  }

  void AirLoopHVACUnitarySystem::autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
  }

  bool AirLoopHVACUnitarySystem::setSupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired(
    double supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setSupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired(
      supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired);
  }

  void AirLoopHVACUnitarySystem::resetSupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetSupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired();
  }

  bool AirLoopHVACUnitarySystem::setFractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(
    double fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setFractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(
      fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired);
  }

  void AirLoopHVACUnitarySystem::resetFractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetFractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
  }

  bool AirLoopHVACUnitarySystem::setFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(
    double fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(
      fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired);
  }

  void AirLoopHVACUnitarySystem::resetFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
  }

  bool AirLoopHVACUnitarySystem::setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired(
    double designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()
      ->setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired(
        designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired);
  }

  void AirLoopHVACUnitarySystem::resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()
      ->resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired();
  }

  bool AirLoopHVACUnitarySystem::setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired(
    double designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()
      ->setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired(
        designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired);
  }

  void AirLoopHVACUnitarySystem::resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()
      ->resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired();
  }

  bool AirLoopHVACUnitarySystem::setMaximumSupplyAirTemperature(double maximumSupplyAirTemperature) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setMaximumSupplyAirTemperature(maximumSupplyAirTemperature);
  }

  void AirLoopHVACUnitarySystem::resetMaximumSupplyAirTemperature() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetMaximumSupplyAirTemperature();
  }

  void AirLoopHVACUnitarySystem::autosizeMaximumSupplyAirTemperature() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->autosizeMaximumSupplyAirTemperature();
  }

  bool AirLoopHVACUnitarySystem::setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(
    double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(
      maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
  }

  void AirLoopHVACUnitarySystem::resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();
  }

  bool AirLoopHVACUnitarySystem::setOutdoorDryBulbTemperatureSensorNodeName(const std::string& outdoorDryBulbTemperatureSensorNodeName) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setOutdoorDryBulbTemperatureSensorNodeName(outdoorDryBulbTemperatureSensorNodeName);
  }

  void AirLoopHVACUnitarySystem::resetOutdoorDryBulbTemperatureSensorNodeName() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetOutdoorDryBulbTemperatureSensorNodeName();
  }

  bool AirLoopHVACUnitarySystem::setAncilliaryOnCycleElectricPower(double ancilliaryOnCycleElectricPower) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setAncilliaryOnCycleElectricPower(ancilliaryOnCycleElectricPower);
  }

  void AirLoopHVACUnitarySystem::resetAncilliaryOnCycleElectricPower() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetAncilliaryOnCycleElectricPower();
  }

  bool AirLoopHVACUnitarySystem::setAncilliaryOffCycleElectricPower(double ancilliaryOffCycleElectricPower) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setAncilliaryOffCycleElectricPower(ancilliaryOffCycleElectricPower);
  }

  void AirLoopHVACUnitarySystem::resetAncilliaryOffCycleElectricPower() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetAncilliaryOffCycleElectricPower();
  }

  // bool AirLoopHVACUnitarySystem::setDesignHeatRecoveryWaterFlowRate(double designHeatRecoveryWaterFlowRate) {
  //   return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setDesignHeatRecoveryWaterFlowRate(designHeatRecoveryWaterFlowRate);
  // }

  // void AirLoopHVACUnitarySystem::resetDesignHeatRecoveryWaterFlowRate() {
  //   getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetDesignHeatRecoveryWaterFlowRate();
  // }

  // bool AirLoopHVACUnitarySystem::setMaximumTemperatureforHeatRecovery(double maximumTemperatureforHeatRecovery) {
  //   return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setMaximumTemperatureforHeatRecovery(maximumTemperatureforHeatRecovery);
  // }

  // void AirLoopHVACUnitarySystem::resetMaximumTemperatureforHeatRecovery() {
  //   getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetMaximumTemperatureforHeatRecovery();
  // }

  bool AirLoopHVACUnitarySystem::setDesignSpecificationMultispeedObject(const UnitarySystemPerformanceMultispeed& unitarySystemPerformace) {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setDesignSpecificationMultispeedObject(unitarySystemPerformace);
  }

  void AirLoopHVACUnitarySystem::resetDesignSpecificationMultispeedObject() {
    getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetDesignSpecificationMultispeedObject();
  }

  boost::optional<double> AirLoopHVACUnitarySystem::autosizedSupplyAirFlowRateDuringCoolingOperation() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->autosizedSupplyAirFlowRateDuringCoolingOperation();
  }

  boost::optional<double> AirLoopHVACUnitarySystem::autosizedSupplyAirFlowRateDuringHeatingOperation() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->autosizedSupplyAirFlowRateDuringHeatingOperation();
  }

  boost::optional<double> AirLoopHVACUnitarySystem::autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->autosizedSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
  }

  boost::optional<double> AirLoopHVACUnitarySystem::autosizedMaximumSupplyAirTemperature() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->autosizedMaximumSupplyAirTemperature();
  }

  boost::optional<double> AirLoopHVACUnitarySystem::autosizedDOASDXCoolingCoilLeavingMinimumAirTemperature() const {
    return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->autosizedDOASDXCoolingCoilLeavingMinimumAirTemperature();
  }

  /// @cond
  AirLoopHVACUnitarySystem::AirLoopHVACUnitarySystem(std::shared_ptr<detail::AirLoopHVACUnitarySystem_Impl> impl)
    : ZoneHVACComponent(std::move(impl)) {}

  // DEPRECATED
  bool AirLoopHVACUnitarySystem::setSupplyAirFlowRateMethodDuringCoolingOperation(const std::string& supplyAirFlowRateMethodDuringCoolingOperation) {
    DEPRECATED_AT_MSG(3, 7, 0,
                      "Use setSupplyAirFlowRateDuringCoolingOperation, autosizeSupplyAirFlowRateDuringCoolingOperation, "
                      "setSupplyAirFlowRatePerFloorAreaDuringCoolingOperation, setFractionofAutosizedDesignCoolingSupplyAirFlowRate, or "
                      "setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation instead.");
    return false;
  }

  void AirLoopHVACUnitarySystem::resetSupplyAirFlowRateMethodDuringCoolingOperation() {
    DEPRECATED_AT_MSG(3, 7, 0,
                      "Use setSupplyAirFlowRateDuringCoolingOperation, autosizeSupplyAirFlowRateDuringCoolingOperation, "
                      "setSupplyAirFlowRatePerFloorAreaDuringCoolingOperation, setFractionofAutosizedDesignCoolingSupplyAirFlowRate, or "
                      "setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation instead.");
  }

  void AirLoopHVACUnitarySystem::resetSupplyAirFlowRateDuringCoolingOperation() {
    DEPRECATED_AT_MSG(3, 7, 0,
                      "Use setSupplyAirFlowRatePerFloorAreaDuringCoolingOperation, setFractionofAutosizedDesignCoolingSupplyAirFlowRate, or "
                      "setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation instead.");
  }

  void AirLoopHVACUnitarySystem::resetSupplyAirFlowRatePerFloorAreaDuringCoolingOperation() {
    DEPRECATED_AT_MSG(
      3, 7, 0,
      "Use setSupplyAirFlowRateDuringCoolingOperation, autosizeSupplyAirFlowRateDuringCoolingOperation, "
      "setFractionofAutosizedDesignCoolingSupplyAirFlowRate, or setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation instead.");
  }

  void AirLoopHVACUnitarySystem::resetFractionofAutosizedDesignCoolingSupplyAirFlowRate() {
    DEPRECATED_AT_MSG(
      3, 7, 0,
      "Use setSupplyAirFlowRateDuringCoolingOperation, autosizeSupplyAirFlowRateDuringCoolingOperation, "
      "setSupplyAirFlowRatePerFloorAreaDuringCoolingOperation, or setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation instead.");
  }

  void AirLoopHVACUnitarySystem::resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation() {
    DEPRECATED_AT_MSG(3, 7, 0,
                      "Use setSupplyAirFlowRateDuringCoolingOperation, autosizeSupplyAirFlowRateDuringCoolingOperation, "
                      "setSupplyAirFlowRatePerFloorAreaDuringCoolingOperation, or setFractionofAutosizedDesignCoolingSupplyAirFlowRate instead.");
  }

  bool AirLoopHVACUnitarySystem::setSupplyAirFlowRateMethodDuringHeatingOperation(const std::string& supplyAirFlowRateMethodDuringHeatingOperation) {
    DEPRECATED_AT_MSG(3, 7, 0,
                      "Use setSupplyAirFlowRateDuringHeatingOperation, autosizeSupplyAirFlowRateDuringHeatingOperation, "
                      "setSupplyAirFlowRatePerFloorAreaduringHeatingOperation, setFractionofAutosizedDesignHeatingSupplyAirFlowRate, or "
                      "setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation instead.");
    return false;
  }

  void AirLoopHVACUnitarySystem::resetSupplyAirFlowRateMethodDuringHeatingOperation() {
    DEPRECATED_AT_MSG(3, 7, 0,
                      "Use setSupplyAirFlowRateDuringHeatingOperation, autosizeSupplyAirFlowRateDuringHeatingOperation, "
                      "setSupplyAirFlowRatePerFloorAreaduringHeatingOperation, setFractionofAutosizedDesignHeatingSupplyAirFlowRate, or "
                      "setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation instead.");
  }

  void AirLoopHVACUnitarySystem::resetSupplyAirFlowRateDuringHeatingOperation() {
    DEPRECATED_AT_MSG(3, 7, 0,
                      "Use setSupplyAirFlowRatePerFloorAreaduringHeatingOperation, setFractionofAutosizedDesignHeatingSupplyAirFlowRate, or "
                      "setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation instead.");
  }

  void AirLoopHVACUnitarySystem::resetSupplyAirFlowRatePerFloorAreaduringHeatingOperation() {
    DEPRECATED_AT_MSG(
      3, 7, 0,
      "Use setSupplyAirFlowRateDuringHeatingOperation, autosizeSupplyAirFlowRateDuringHeatingOperation, "
      "setFractionofAutosizedDesignHeatingSupplyAirFlowRate, or setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation instead.");
  }

  void AirLoopHVACUnitarySystem::resetFractionofAutosizedDesignHeatingSupplyAirFlowRate() {
    DEPRECATED_AT_MSG(
      3, 7, 0,
      "Use setSupplyAirFlowRateDuringHeatingOperation, autosizeSupplyAirFlowRateDuringHeatingOperation, "
      "setSupplyAirFlowRatePerFloorAreaduringHeatingOperation, or setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation instead.");
  }

  void AirLoopHVACUnitarySystem::resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation() {
    DEPRECATED_AT_MSG(3, 7, 0,
                      "Use setSupplyAirFlowRateDuringHeatingOperation, autosizeSupplyAirFlowRateDuringHeatingOperation, "
                      "setSupplyAirFlowRatePerFloorAreaduringHeatingOperation, or setFractionofAutosizedDesignHeatingSupplyAirFlowRate instead.");
  }

  double AirLoopHVACUnitarySystem::maximumCyclingRate() const {
    DEPRECATED_AT_MSG(3, 7, 0,
                      "As of EnergyPlus 23.2.0, this property is on the child cooling coil. Use coolingCoil()->maximumCyclingRate() instead.");
    double result = 2.5;  // former default
    if (auto c_ = coolingCoil()) {
      if (auto c_eq_ = c_->optionalCast<CoilCoolingWaterToAirHeatPumpEquationFit>()) {
        result = c_eq_->maximumCyclingRate();
      } else if (auto c_vs_ = c_->optionalCast<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit>()) {
        result = c_vs_->maximumCyclingRate();
      } else if (auto c_dx_ = c_->optionalCast<CoilCoolingDXVariableSpeed>()) {
        result = c_dx_->maximumCyclingRate();
      }
    }
    return result;
  }

  bool AirLoopHVACUnitarySystem::isMaximumCyclingRateDefaulted() const {
    DEPRECATED_AT_MSG(3, 7, 0, "As of EnergyPlus 23.2.0, this property is on the child cooling coil and it's a required-field");
    return false;
  }

  bool AirLoopHVACUnitarySystem::setMaximumCyclingRate(double maximumCyclingRate) {
    DEPRECATED_AT_MSG(3, 7, 0,
                      "As of EnergyPlus 23.2.0, this property is on the child cooling coil. Use coolingCoil()->setMaximumCyclingRate() instead.");
    bool result = false;
    if (auto c_ = coolingCoil()) {
      if (auto c_eq_ = c_->optionalCast<CoilCoolingWaterToAirHeatPumpEquationFit>()) {
        result = c_eq_->setMaximumCyclingRate(maximumCyclingRate);
      } else if (auto c_vs_ = c_->optionalCast<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit>()) {
        result = c_vs_->setMaximumCyclingRate(maximumCyclingRate);
      } else if (auto c_dx_ = c_->optionalCast<CoilCoolingDXVariableSpeed>()) {
        result = c_dx_->setMaximumCyclingRate(maximumCyclingRate);
      }
    }
    return result;
  }

  void AirLoopHVACUnitarySystem::resetMaximumCyclingRate() {
    DEPRECATED_AT_MSG(3, 7, 0, "As of EnergyPlus 23.2.0, this property is on the child cooling coil and it's a required-field");
  }

  double AirLoopHVACUnitarySystem::heatPumpTimeConstant() const {
    DEPRECATED_AT_MSG(
      3, 7, 0, "As of EnergyPlus 23.2.0, this property is on the child cooling coil. Use coolingCoil()->latentCapacityTimeConstant() instead.");
    double result = 60;  // former default
    if (auto c_ = coolingCoil()) {
      if (auto c_eq_ = c_->optionalCast<CoilCoolingWaterToAirHeatPumpEquationFit>()) {
        result = c_eq_->latentCapacityTimeConstant();
      } else if (auto c_vs_ = c_->optionalCast<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit>()) {
        result = c_vs_->latentCapacityTimeConstant();
      } else if (auto c_dx_ = c_->optionalCast<CoilCoolingDXVariableSpeed>()) {
        result = c_dx_->latentCapacityTimeConstant();
      }
    }
    return result;
  }

  bool AirLoopHVACUnitarySystem::isHeatPumpTimeConstantDefaulted() const {
    DEPRECATED_AT_MSG(3, 7, 0, "As of EnergyPlus 23.2.0, this property is on the child cooling coil and it's a required-field");
    return false;
  }

  bool AirLoopHVACUnitarySystem::setHeatPumpTimeConstant(double heatPumpTimeConstant) {
    DEPRECATED_AT_MSG(
      3, 7, 0, "As of EnergyPlus 23.2.0, this property is on the child cooling coil. Use coolingCoil()->setLatentCapacityTimeConstant() instead.");
    bool result = false;
    if (auto c_ = coolingCoil()) {
      if (auto c_eq_ = c_->optionalCast<CoilCoolingWaterToAirHeatPumpEquationFit>()) {
        result = c_eq_->setLatentCapacityTimeConstant(heatPumpTimeConstant);
      } else if (auto c_vs_ = c_->optionalCast<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit>()) {
        result = c_vs_->setLatentCapacityTimeConstant(heatPumpTimeConstant);
      } else if (auto c_dx_ = c_->optionalCast<CoilCoolingDXVariableSpeed>()) {
        result = c_dx_->setLatentCapacityTimeConstant(heatPumpTimeConstant);
      }
    }
    return result;
  }

  void AirLoopHVACUnitarySystem::resetHeatPumpTimeConstant() {
    DEPRECATED_AT_MSG(3, 7, 0, "As of EnergyPlus 23.2.0, this property is on the child cooling coil and it's a required-field");
  }

  double AirLoopHVACUnitarySystem::fractionofOnCyclePowerUse() const {
    DEPRECATED_AT_MSG(3, 7, 0, "As of EnergyPlus 23.2.0, this property is no longer used.");
    const double result = 0.01;  // former default
    return result;
  }

  bool AirLoopHVACUnitarySystem::isFractionofOnCyclePowerUseDefaulted() const {
    DEPRECATED_AT_MSG(3, 7, 0, "As of EnergyPlus 23.2.0, this property is no longer used.");
    return false;
  }

  bool AirLoopHVACUnitarySystem::setFractionofOnCyclePowerUse(double fractionofOnCyclePowerUse) {
    DEPRECATED_AT_MSG(3, 7, 0, "As of EnergyPlus 23.2.0, this property is no longer used.");
    return false;
  }

  void AirLoopHVACUnitarySystem::resetFractionofOnCyclePowerUse() {
    DEPRECATED_AT_MSG(3, 7, 0, "As of EnergyPlus 23.2.0, this property is no longer used.");
  }

  double AirLoopHVACUnitarySystem::heatPumpFanDelayTime() const {
    DEPRECATED_AT_MSG(3, 7, 0, "As of EnergyPlus 23.2.0, this property is on the child cooling coil. Use coolingCoil()->fanDelayTime() instead.");
    double result = 60;  // former default
    if (auto c_ = coolingCoil()) {
      if (auto c_eq_ = c_->optionalCast<CoilCoolingWaterToAirHeatPumpEquationFit>()) {
        result = c_eq_->fanDelayTime();
      } else if (auto c_vs_ = c_->optionalCast<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit>()) {
        result = c_vs_->fanDelayTime();
      } else if (auto c_dx_ = c_->optionalCast<CoilCoolingDXVariableSpeed>()) {
        result = c_dx_->fanDelayTime();
      }
    }
    return result;
  }

  bool AirLoopHVACUnitarySystem::isHeatPumpFanDelayTimeDefaulted() const {
    DEPRECATED_AT_MSG(3, 7, 0, "As of EnergyPlus 23.2.0, this property is on the child cooling coil and it's a required-field");
    return false;
  }

  bool AirLoopHVACUnitarySystem::setHeatPumpFanDelayTime(double heatPumpFanDelayTime) {
    DEPRECATED_AT_MSG(3, 7, 0, "As of EnergyPlus 23.2.0, this property is on the child cooling coil. Use coolingCoil()->setFanDelayTime() instead.");
    bool result = false;
    if (auto c_ = coolingCoil()) {
      if (auto c_eq_ = c_->optionalCast<CoilCoolingWaterToAirHeatPumpEquationFit>()) {
        result = c_eq_->setFanDelayTime(heatPumpFanDelayTime);
      } else if (auto c_vs_ = c_->optionalCast<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit>()) {
        result = c_vs_->setFanDelayTime(heatPumpFanDelayTime);
      } else if (auto c_dx_ = c_->optionalCast<CoilCoolingDXVariableSpeed>()) {
        result = c_dx_->setFanDelayTime(heatPumpFanDelayTime);
      }
    }
    return result;
  }

  void AirLoopHVACUnitarySystem::resetHeatPumpFanDelayTime() {
    DEPRECATED_AT_MSG(3, 7, 0, "As of EnergyPlus 23.2.0, this property is on the child cooling coil and it's a required-field");
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
