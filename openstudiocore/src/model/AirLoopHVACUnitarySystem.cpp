/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_AirLoopHVAC_UnitarySystem_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirLoopHVACUnitarySystem_Impl::AirLoopHVACUnitarySystem_Impl(const IdfObject& idfObject,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : WaterToAirComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirLoopHVACUnitarySystem::iddObjectType());
  }

  AirLoopHVACUnitarySystem_Impl::AirLoopHVACUnitarySystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : WaterToAirComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirLoopHVACUnitarySystem::iddObjectType());
  }

  AirLoopHVACUnitarySystem_Impl::AirLoopHVACUnitarySystem_Impl(const AirLoopHVACUnitarySystem_Impl& other,
                                                               Model_Impl* model,
                                                               bool keepHandle)
    : WaterToAirComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirLoopHVACUnitarySystem_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirLoopHVACUnitarySystem_Impl::iddObjectType() const {
    return AirLoopHVACUnitarySystem::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AirLoopHVACUnitarySystem_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AirLoopHVAC_UnitarySystemFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AirLoopHVACUnitarySystem","Availability"));
    }
    if (std::find(b,e,OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFanOperatingModeScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AirLoopHVACUnitarySystem","Supply Air Fan Operating Mode"));
    }
    return result;
  }

  ModelObject AirLoopHVACUnitarySystem_Impl::clone(Model model) const
  {
    AirLoopHVACUnitarySystem modelObjectClone = ModelObject_Impl::clone(model).cast<AirLoopHVACUnitarySystem>();

    if( boost::optional<HVACComponent> supplyFan = this->supplyFan()) {
      modelObjectClone.setSupplyFan(supplyFan->clone(model).cast<HVACComponent>());
    }
    if( boost::optional<HVACComponent> coolingCoil = this->coolingCoil()) {
      modelObjectClone.setCoolingCoil(coolingCoil->clone(model).cast<HVACComponent>());
    }
    if( boost::optional<HVACComponent> heatingCoil = this->heatingCoil()) {
      modelObjectClone.setHeatingCoil(heatingCoil->clone(model).cast<HVACComponent>());
    }
    if( boost::optional<HVACComponent> supplementalHeatingCoil = this->supplementalHeatingCoil()) {
      modelObjectClone.setSupplementalHeatingCoil(supplementalHeatingCoil->clone(model).cast<HVACComponent>());
    }
    if( auto designSpec = designSpecificationMultispeedObject() ) {
      modelObjectClone.setDesignSpecificationMultispeedObject(designSpec->clone(model).cast<UnitarySystemPerformanceMultispeed>());
    }

    return modelObjectClone;
  }

  std::vector<ModelObject> AirLoopHVACUnitarySystem_Impl::children() const
  {
    std::vector<ModelObject> result;

    if( boost::optional<HVACComponent> supplyFan = this->supplyFan()) {
      result.push_back( *supplyFan );
    }
    if( boost::optional<HVACComponent> coolingCoil = this->coolingCoil()) {
      result.push_back( *coolingCoil );
    }
    if( boost::optional<HVACComponent> heatingCoil = this->heatingCoil()) {
      result.push_back( *heatingCoil );
    }
    if( boost::optional<HVACComponent> supplementalHeatingCoil = this->supplementalHeatingCoil()) {
      result.push_back( *supplementalHeatingCoil );
    }
    if( auto designSpec = designSpecificationMultispeedObject() ) {
      result.push_back( *designSpec );
    }

    return result;
  }

  std::vector<IdfObject> AirLoopHVACUnitarySystem_Impl::remove()
  {
    std::vector<IdfObject> result;

    if ( boost::optional<HVACComponent> _supplyFan = this->supplyFan() ) {
      std::vector<IdfObject> removedFans = _supplyFan->remove();
      result.insert(result.end(), removedFans.begin(), removedFans.end());
    }

    if (boost::optional<HVACComponent> _coolingCoil = this->coolingCoil()) {
      if( boost::optional<PlantLoop> loop = _coolingCoil->plantLoop() )
      {
        loop->removeDemandBranchWithComponent(*_coolingCoil);
      }
      std::vector<IdfObject> removedCoolingCoils = _coolingCoil->remove();
      result.insert(result.end(), removedCoolingCoils.begin(), removedCoolingCoils.end());
    }

    if (boost::optional<HVACComponent> _heatingCoil = this->heatingCoil()) {
      if( boost::optional<PlantLoop> loop = _heatingCoil->plantLoop() )
      {
        loop->removeDemandBranchWithComponent(*_heatingCoil);
      }
      std::vector<IdfObject> removedHeatingCoils = _heatingCoil->remove();
      result.insert(result.end(), removedHeatingCoils.begin(), removedHeatingCoils.end());
    }

    if (boost::optional<HVACComponent> _supplementalHeatingCoil = this->supplementalHeatingCoil()) {
      if( boost::optional<PlantLoop> loop = _supplementalHeatingCoil->plantLoop() )
      {
        loop->removeDemandBranchWithComponent(*_supplementalHeatingCoil);
      }
      std::vector<IdfObject> removedSuppHeatingCoils = _supplementalHeatingCoil->remove();
      result.insert(result.end(), removedSuppHeatingCoils.begin(), removedSuppHeatingCoils.end());
    }

    if( auto designSpec = designSpecificationMultispeedObject() ) {
      auto removed = designSpec->remove();
      result.insert(result.end(), removed.begin(), removed.end());
    }

    std::vector<IdfObject> removedUnitarySystem = WaterToAirComponent_Impl::remove();
    result.insert(result.end(), removedUnitarySystem.begin(), removedUnitarySystem.end());

    return result;
  }

  bool AirLoopHVACUnitarySystem_Impl::addToNode(Node & node)
  {
    // remove once heat recovery is re-enabled in future
    if( node.plantLoop() ) {
      return false;
    }
    return WaterToAirComponent_Impl::addToNode(node);
  }

  unsigned AirLoopHVACUnitarySystem_Impl::airInletPort()
  {
    return OS_AirLoopHVAC_UnitarySystemFields::AirInletNodeName;
  }

  unsigned AirLoopHVACUnitarySystem_Impl::airOutletPort()
  {
    return OS_AirLoopHVAC_UnitarySystemFields::AirOutletNodeName;
  }

  unsigned AirLoopHVACUnitarySystem_Impl::waterInletPort()
  {
    return OS_AirLoopHVAC_UnitarySystemFields::HeatRecoveryWaterInletNodeName;
  }

  unsigned AirLoopHVACUnitarySystem_Impl::waterOutletPort()
  {
    return OS_AirLoopHVAC_UnitarySystemFields::HeatRecoveryWaterOutletNodeName;
  }

  std::string AirLoopHVACUnitarySystem_Impl::controlType() const {
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitarySystemFields::ControlType,true);
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
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitarySystemFields::DehumidificationControlType,true);
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
    return getString(OS_AirLoopHVAC_UnitarySystemFields::FanPlacement,true);
  }

  boost::optional<Schedule> AirLoopHVACUnitarySystem_Impl::supplyAirFanOperatingModeSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFanOperatingModeScheduleName);
  }

  boost::optional<HVACComponent> AirLoopHVACUnitarySystem_Impl::heatingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirLoopHVAC_UnitarySystemFields::HeatingCoilName);
  }

  double AirLoopHVACUnitarySystem_Impl::dXHeatingCoilSizingRatio() const {
    boost::optional<double> value = getDouble(OS_AirLoopHVAC_UnitarySystemFields::DXHeatingCoilSizingRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirLoopHVACUnitarySystem_Impl::isDXHeatingCoilSizingRatioDefaulted() const {
    return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::DXHeatingCoilSizingRatio);
  }

  boost::optional<HVACComponent> AirLoopHVACUnitarySystem_Impl::coolingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirLoopHVAC_UnitarySystemFields::CoolingCoilName);
  }

  bool AirLoopHVACUnitarySystem_Impl::useDOASDXCoolingCoil() const {
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitarySystemFields::UseDOASDXCoolingCoil,true);
    OS_ASSERT(value);
    return openstudio::istringEqual(value.get(), "Yes");
  }

  bool AirLoopHVACUnitarySystem_Impl::isUseDOASDXCoolingCoilDefaulted() const {
    return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::UseDOASDXCoolingCoil);
  }

  double AirLoopHVACUnitarySystem_Impl::dOASDXCoolingCoilLeavingMinimumAirTemperature() const {
    boost::optional<double> value = getDouble(OS_AirLoopHVAC_UnitarySystemFields::DOASDXCoolingCoilLeavingMinimumAirTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirLoopHVACUnitarySystem_Impl::isDOASDXCoolingCoilLeavingMinimumAirTemperatureDefaulted() const {
    return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::DOASDXCoolingCoilLeavingMinimumAirTemperature);
  }

  std::string AirLoopHVACUnitarySystem_Impl::latentLoadControl() const {
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitarySystemFields::LatentLoadControl,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirLoopHVACUnitarySystem_Impl::isLatentLoadControlDefaulted() const {
    return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::LatentLoadControl);
  }

  boost::optional<HVACComponent> AirLoopHVACUnitarySystem_Impl::supplementalHeatingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirLoopHVAC_UnitarySystemFields::SupplementalHeatingCoilName);
  }

  boost::optional<std::string> AirLoopHVACUnitarySystem_Impl::supplyAirFlowRateMethodDuringCoolingOperation() const {
    return getString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateMethodDuringCoolingOperation,true);
  }

  boost::optional<double> AirLoopHVACUnitarySystem_Impl::supplyAirFlowRateDuringCoolingOperation() const {
    return getDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringCoolingOperation,true);
  }

  bool AirLoopHVACUnitarySystem_Impl::isSupplyAirFlowRateDuringCoolingOperationAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringCoolingOperation, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> AirLoopHVACUnitarySystem_Impl::supplyAirFlowRatePerFloorAreaDuringCoolingOperation() const {
    return getDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRatePerFloorAreaDuringCoolingOperation,true);
  }

  boost::optional<double> AirLoopHVACUnitarySystem_Impl::fractionofAutosizedDesignCoolingSupplyAirFlowRate() const {
    return getDouble(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignCoolingSupplyAirFlowRate,true);
  }

  boost::optional<double> AirLoopHVACUnitarySystem_Impl::designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation() const {
    return getDouble(OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation,true);
  }

  boost::optional<std::string> AirLoopHVACUnitarySystem_Impl::supplyAirFlowRateMethodDuringHeatingOperation() const {
    return getString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateMethodDuringHeatingOperation,true);
  }

  boost::optional<double> AirLoopHVACUnitarySystem_Impl::supplyAirFlowRateDuringHeatingOperation() const {
    return getDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringHeatingOperation,true);
  }

  bool AirLoopHVACUnitarySystem_Impl::isSupplyAirFlowRateDuringHeatingOperationAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringHeatingOperation, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> AirLoopHVACUnitarySystem_Impl::supplyAirFlowRatePerFloorAreaduringHeatingOperation() const {
    return getDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRatePerFloorAreaduringHeatingOperation,true);
  }

  boost::optional<double> AirLoopHVACUnitarySystem_Impl::fractionofAutosizedDesignHeatingSupplyAirFlowRate() const {
    return getDouble(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignHeatingSupplyAirFlowRate,true);
  }

  boost::optional<double> AirLoopHVACUnitarySystem_Impl::designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation() const {
    return getDouble(OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation,true);
  }

  boost::optional<std::string> AirLoopHVACUnitarySystem_Impl::supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired() const {
    return getString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired,true);
  }

  boost::optional<double> AirLoopHVACUnitarySystem_Impl::supplyAirFlowRateWhenNoCoolingorHeatingisRequired() const {
    return getDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateWhenNoCoolingorHeatingisRequired,true);
  }

  bool AirLoopHVACUnitarySystem_Impl::isSupplyAirFlowRateWhenNoCoolingorHeatingisRequiredAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateWhenNoCoolingorHeatingisRequired, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> AirLoopHVACUnitarySystem_Impl::supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired() const {
    return getDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired,true);
  }

  boost::optional<double> AirLoopHVACUnitarySystem_Impl::fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() const {
    return getDouble(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired,true);
  }

  boost::optional<double> AirLoopHVACUnitarySystem_Impl::fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() const {
    return getDouble(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired,true);
  }

  boost::optional<double> AirLoopHVACUnitarySystem_Impl::designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired() const {
    return getDouble(OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired,true);
  }

  boost::optional<double> AirLoopHVACUnitarySystem_Impl::designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired() const {
    return getDouble(OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired,true);
  }

  boost::optional<double> AirLoopHVACUnitarySystem_Impl::maximumSupplyAirTemperature() const {
    return getDouble(OS_AirLoopHVAC_UnitarySystemFields::MaximumSupplyAirTemperature,true);
  }

  bool AirLoopHVACUnitarySystem_Impl::isMaximumSupplyAirTemperatureDefaulted() const {
    return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::MaximumSupplyAirTemperature);
  }

  bool AirLoopHVACUnitarySystem_Impl::isMaximumSupplyAirTemperatureAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitarySystemFields::MaximumSupplyAirTemperature, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  double AirLoopHVACUnitarySystem_Impl::maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const {
    boost::optional<double> value = getDouble(OS_AirLoopHVAC_UnitarySystemFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirLoopHVACUnitarySystem_Impl::isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const {
    return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
  }

  boost::optional<std::string> AirLoopHVACUnitarySystem_Impl::outdoorDryBulbTemperatureSensorNodeName() const {
    return getString(OS_AirLoopHVAC_UnitarySystemFields::OutdoorDryBulbTemperatureSensorNodeName,true);
  }

  double AirLoopHVACUnitarySystem_Impl::maximumCyclingRate() const {
    boost::optional<double> value = getDouble(OS_AirLoopHVAC_UnitarySystemFields::MaximumCyclingRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirLoopHVACUnitarySystem_Impl::isMaximumCyclingRateDefaulted() const {
    return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::MaximumCyclingRate);
  }

  double AirLoopHVACUnitarySystem_Impl::heatPumpTimeConstant() const {
    boost::optional<double> value = getDouble(OS_AirLoopHVAC_UnitarySystemFields::HeatPumpTimeConstant,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirLoopHVACUnitarySystem_Impl::isHeatPumpTimeConstantDefaulted() const {
    return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::HeatPumpTimeConstant);
  }

  double AirLoopHVACUnitarySystem_Impl::fractionofOnCyclePowerUse() const {
    boost::optional<double> value = getDouble(OS_AirLoopHVAC_UnitarySystemFields::FractionofOnCyclePowerUse,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirLoopHVACUnitarySystem_Impl::isFractionofOnCyclePowerUseDefaulted() const {
    return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::FractionofOnCyclePowerUse);
  }

  double AirLoopHVACUnitarySystem_Impl::heatPumpFanDelayTime() const {
    boost::optional<double> value = getDouble(OS_AirLoopHVAC_UnitarySystemFields::HeatPumpFanDelayTime,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirLoopHVACUnitarySystem_Impl::isHeatPumpFanDelayTimeDefaulted() const {
    return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::HeatPumpFanDelayTime);
  }

  double AirLoopHVACUnitarySystem_Impl::ancilliaryOnCycleElectricPower() const {
    boost::optional<double> value = getDouble(OS_AirLoopHVAC_UnitarySystemFields::AncilliaryOnCycleElectricPower,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirLoopHVACUnitarySystem_Impl::isAncilliaryOnCycleElectricPowerDefaulted() const {
    return isEmpty(OS_AirLoopHVAC_UnitarySystemFields::AncilliaryOnCycleElectricPower);
  }

  double AirLoopHVACUnitarySystem_Impl::ancilliaryOffCycleElectricPower() const {
    boost::optional<double> value = getDouble(OS_AirLoopHVAC_UnitarySystemFields::AncilliaryOffCycleElectricPower,true);
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
    return getObject<ModelObject>().getModelObjectTarget<UnitarySystemPerformanceMultispeed>(OS_AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectName);
  }

  bool AirLoopHVACUnitarySystem_Impl::setControlType(std::string controlType) {
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
    }
    else {
      resetControllingZoneorThermostatLocation();
      result = true;
    }
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetControllingZoneorThermostatLocation() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::ControllingZoneorThermostatLocation, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setDehumidificationControlType(std::string dehumidificationControlType) {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::DehumidificationControlType, dehumidificationControlType);
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetDehumidificationControlType() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::DehumidificationControlType, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirLoopHVAC_UnitarySystemFields::AvailabilityScheduleName,
                              "AirLoopHVACUnitarySystem",
                              "Availability",
                              schedule);
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
    }
    else {
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
    }
    else {
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
    bool result = setSchedule(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFanOperatingModeScheduleName,
                              "AirLoopHVACUnitarySystem",
                              "Supply Air Fan Operating Mode",
                              schedule);
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetSupplyAirFanOperatingModeSchedule() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFanOperatingModeScheduleName, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setHeatingCoil(const boost::optional<HVACComponent>& heatingCoil) {
    bool result(false);
    if (heatingCoil) {
      result = setPointer(OS_AirLoopHVAC_UnitarySystemFields::HeatingCoilName, heatingCoil.get().handle());
    }
    else {
      resetHeatingCoil();
      result = true;
    }
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetHeatingCoil() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::HeatingCoilName, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setDXHeatingCoilSizingRatio(double dXHeatingCoilSizingRatio) {
    bool result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::DXHeatingCoilSizingRatio, dXHeatingCoilSizingRatio);
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetDXHeatingCoilSizingRatio() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::DXHeatingCoilSizingRatio, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setCoolingCoil(const boost::optional<HVACComponent>& coolingCoil) {
    bool result(false);
    if (coolingCoil) {
      result = setPointer(OS_AirLoopHVAC_UnitarySystemFields::CoolingCoilName, coolingCoil.get().handle());
    }
    else {
      resetCoolingCoil();
      result = true;
    }
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetCoolingCoil() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::CoolingCoilName, "");
    OS_ASSERT(result);
  }

  void AirLoopHVACUnitarySystem_Impl::setUseDOASDXCoolingCoil(bool useDOASDXCoolingCoil) {
    setBooleanFieldValue(OS_AirLoopHVAC_UnitarySystemFields::UseDOASDXCoolingCoil, useDOASDXCoolingCoil);
  }

  void AirLoopHVACUnitarySystem_Impl::resetUseDOASDXCoolingCoil() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::UseDOASDXCoolingCoil, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setDOASDXCoolingCoilLeavingMinimumAirTemperature(double dOASDXCoolingCoilLeavingMinimumAirTemperature) {
    bool result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::DOASDXCoolingCoilLeavingMinimumAirTemperature, dOASDXCoolingCoilLeavingMinimumAirTemperature);
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetDOASDXCoolingCoilLeavingMinimumAirTemperature() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::DOASDXCoolingCoilLeavingMinimumAirTemperature, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setLatentLoadControl(std::string latentLoadControl) {
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
    }
    else {
      resetSupplementalHeatingCoil();
      result = true;
    }
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetSupplementalHeatingCoil() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplementalHeatingCoilName, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setSupplyAirFlowRateMethodDuringCoolingOperation(boost::optional<std::string> supplyAirFlowRateMethodDuringCoolingOperation) {
    bool result(false);
    if (supplyAirFlowRateMethodDuringCoolingOperation) {
      result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateMethodDuringCoolingOperation, supplyAirFlowRateMethodDuringCoolingOperation.get());
    }
    else {
      resetSupplyAirFlowRateMethodDuringCoolingOperation();
      result = true;
    }
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetSupplyAirFlowRateMethodDuringCoolingOperation() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateMethodDuringCoolingOperation, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setSupplyAirFlowRateDuringCoolingOperation(boost::optional<double> supplyAirFlowRateDuringCoolingOperation) {
    bool result(false);
    if (supplyAirFlowRateDuringCoolingOperation) {
      result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringCoolingOperation, supplyAirFlowRateDuringCoolingOperation.get());
    }
    else {
      resetSupplyAirFlowRateDuringCoolingOperation();
      result = true;
    }
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetSupplyAirFlowRateDuringCoolingOperation() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringCoolingOperation, "");
    OS_ASSERT(result);
  }

  void AirLoopHVACUnitarySystem_Impl::autosizeSupplyAirFlowRateDuringCoolingOperation() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringCoolingOperation, "autosize");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setSupplyAirFlowRatePerFloorAreaDuringCoolingOperation(boost::optional<double> supplyAirFlowRatePerFloorAreaDuringCoolingOperation) {
    bool result(false);
    if (supplyAirFlowRatePerFloorAreaDuringCoolingOperation) {
      result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRatePerFloorAreaDuringCoolingOperation, supplyAirFlowRatePerFloorAreaDuringCoolingOperation.get());
    }
    else {
      resetSupplyAirFlowRatePerFloorAreaDuringCoolingOperation();
      result = true;
    }
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetSupplyAirFlowRatePerFloorAreaDuringCoolingOperation() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRatePerFloorAreaDuringCoolingOperation, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setFractionofAutosizedDesignCoolingSupplyAirFlowRate(boost::optional<double> fractionofAutosizedDesignCoolingSupplyAirFlowRate) {
    bool result(false);
    if (fractionofAutosizedDesignCoolingSupplyAirFlowRate) {
      result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignCoolingSupplyAirFlowRate, fractionofAutosizedDesignCoolingSupplyAirFlowRate.get());
    }
    else {
      resetFractionofAutosizedDesignCoolingSupplyAirFlowRate();
      result = true;
    }
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetFractionofAutosizedDesignCoolingSupplyAirFlowRate() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignCoolingSupplyAirFlowRate, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation(boost::optional<double> designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation) {
    bool result(false);
    if (designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation) {
      result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation, designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation.get());
    }
    else {
      resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation();
      result = true;
    }
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setSupplyAirFlowRateMethodDuringHeatingOperation(boost::optional<std::string> supplyAirFlowRateMethodDuringHeatingOperation) {
    bool result(false);
    if (supplyAirFlowRateMethodDuringHeatingOperation) {
      result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateMethodDuringHeatingOperation, supplyAirFlowRateMethodDuringHeatingOperation.get());
    }
    else {
      resetSupplyAirFlowRateMethodDuringHeatingOperation();
      result = true;
    }
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetSupplyAirFlowRateMethodDuringHeatingOperation() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateMethodDuringHeatingOperation, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setSupplyAirFlowRateDuringHeatingOperation(boost::optional<double> supplyAirFlowRateDuringHeatingOperation) {
    bool result(false);
    if (supplyAirFlowRateDuringHeatingOperation) {
      result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringHeatingOperation, supplyAirFlowRateDuringHeatingOperation.get());
    }
    else {
      resetSupplyAirFlowRateDuringHeatingOperation();
      result = true;
    }
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetSupplyAirFlowRateDuringHeatingOperation() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringHeatingOperation, "");
    OS_ASSERT(result);
  }

  void AirLoopHVACUnitarySystem_Impl::autosizeSupplyAirFlowRateDuringHeatingOperation() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringHeatingOperation, "autosize");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setSupplyAirFlowRatePerFloorAreaduringHeatingOperation(boost::optional<double> supplyAirFlowRatePerFloorAreaduringHeatingOperation) {
    bool result(false);
    if (supplyAirFlowRatePerFloorAreaduringHeatingOperation) {
      result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRatePerFloorAreaduringHeatingOperation, supplyAirFlowRatePerFloorAreaduringHeatingOperation.get());
    }
    else {
      resetSupplyAirFlowRatePerFloorAreaduringHeatingOperation();
      result = true;
    }
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetSupplyAirFlowRatePerFloorAreaduringHeatingOperation() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRatePerFloorAreaduringHeatingOperation, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setFractionofAutosizedDesignHeatingSupplyAirFlowRate(boost::optional<double> fractionofAutosizedDesignHeatingSupplyAirFlowRate) {
    bool result(false);
    if (fractionofAutosizedDesignHeatingSupplyAirFlowRate) {
      result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignHeatingSupplyAirFlowRate, fractionofAutosizedDesignHeatingSupplyAirFlowRate.get());
    }
    else {
      resetFractionofAutosizedDesignHeatingSupplyAirFlowRate();
      result = true;
    }
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetFractionofAutosizedDesignHeatingSupplyAirFlowRate() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignHeatingSupplyAirFlowRate, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation(boost::optional<double> designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation) {
    bool result(false);
    if (designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation) {
      result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation, designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation.get());
    }
    else {
      resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation();
      result = true;
    }
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired(boost::optional<std::string> supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired) {
    bool result(false);
    if (supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired) {
      result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired, supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired.get());
    }
    else {
      resetSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired();
      result = true;
    }
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisRequired) {
    bool result(false);
    if (supplyAirFlowRateWhenNoCoolingorHeatingisRequired) {
      result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateWhenNoCoolingorHeatingisRequired, supplyAirFlowRateWhenNoCoolingorHeatingisRequired.get());
    }
    else {
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
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateWhenNoCoolingorHeatingisRequired, "autosize");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setSupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired(boost::optional<double> supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired) {
    bool result(false);
    if (supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired) {
      result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired, supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired.get());
    }
    else {
      resetSupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired();
      result = true;
    }
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetSupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setFractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(boost::optional<double> fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired) {
    bool result(false);
    if (fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired) {
      result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired, fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired.get());
    }
    else {
      resetFractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
      result = true;
    }
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetFractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(boost::optional<double> fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired) {
    bool result(false);
    if (fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired) {
      result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired, fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired.get());
    }
    else {
      resetFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
      result = true;
    }
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired(boost::optional<double> designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired) {
    bool result(false);
    if (designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired) {
      result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired, designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired.get());
    }
    else {
      resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired();
      result = true;
    }
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired(boost::optional<double> designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired) {
    bool result(false);
    if (designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired) {
      result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired, designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired.get());
    }
    else {
      resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired();
      result = true;
    }
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired, "");
    OS_ASSERT(result);
  }

  void AirLoopHVACUnitarySystem_Impl::setMaximumSupplyAirTemperature(boost::optional<double> maximumSupplyAirTemperature) {
    bool result(false);
    if (maximumSupplyAirTemperature) {
      result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::MaximumSupplyAirTemperature, maximumSupplyAirTemperature.get());
    }
    else {
      resetMaximumSupplyAirTemperature();
      result = true;
    }
    OS_ASSERT(result);
  }

  void AirLoopHVACUnitarySystem_Impl::resetMaximumSupplyAirTemperature() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::MaximumSupplyAirTemperature, "");
    OS_ASSERT(result);
  }

  void AirLoopHVACUnitarySystem_Impl::autosizeMaximumSupplyAirTemperature() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::MaximumSupplyAirTemperature, "autosize");
    OS_ASSERT(result);
  }

  void AirLoopHVACUnitarySystem_Impl::setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation) {
    bool result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation, maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
    OS_ASSERT(result);
  }

  void AirLoopHVACUnitarySystem_Impl::resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation, "");
    OS_ASSERT(result);
  }

  void AirLoopHVACUnitarySystem_Impl::setOutdoorDryBulbTemperatureSensorNodeName(boost::optional<std::string> outdoorDryBulbTemperatureSensorNodeName) {
    bool result(false);
    if (outdoorDryBulbTemperatureSensorNodeName) {
      result = setString(OS_AirLoopHVAC_UnitarySystemFields::OutdoorDryBulbTemperatureSensorNodeName, outdoorDryBulbTemperatureSensorNodeName.get());
    }
    else {
      resetOutdoorDryBulbTemperatureSensorNodeName();
      result = true;
    }
    OS_ASSERT(result);
  }

  void AirLoopHVACUnitarySystem_Impl::resetOutdoorDryBulbTemperatureSensorNodeName() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::OutdoorDryBulbTemperatureSensorNodeName, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setMaximumCyclingRate(double maximumCyclingRate) {
    bool result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::MaximumCyclingRate, maximumCyclingRate);
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetMaximumCyclingRate() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::MaximumCyclingRate, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setHeatPumpTimeConstant(double heatPumpTimeConstant) {
    bool result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::HeatPumpTimeConstant, heatPumpTimeConstant);
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetHeatPumpTimeConstant() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::HeatPumpTimeConstant, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setFractionofOnCyclePowerUse(double fractionofOnCyclePowerUse) {
    bool result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::FractionofOnCyclePowerUse, fractionofOnCyclePowerUse);
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetFractionofOnCyclePowerUse() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::FractionofOnCyclePowerUse, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitarySystem_Impl::setHeatPumpFanDelayTime(double heatPumpFanDelayTime) {
    bool result = setDouble(OS_AirLoopHVAC_UnitarySystemFields::HeatPumpFanDelayTime, heatPumpFanDelayTime);
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetHeatPumpFanDelayTime() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::HeatPumpFanDelayTime, "");
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

  bool AirLoopHVACUnitarySystem_Impl::setDesignSpecificationMultispeedObject(const boost::optional<UnitarySystemPerformanceMultispeed>& unitarySystemPerformace) {
    bool result(false);
    if (unitarySystemPerformace) {
      result = setPointer(OS_AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectName, unitarySystemPerformace.get().handle());
    }
    else {
      resetDesignSpecificationMultispeedObject();
      result = true;
    }
    return result;
  }

  void AirLoopHVACUnitarySystem_Impl::resetDesignSpecificationMultispeedObject() {
    bool result = setString(OS_AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectName, "");
    OS_ASSERT(result);
  }

} // detail

AirLoopHVACUnitarySystem::AirLoopHVACUnitarySystem(const Model& model)
  : WaterToAirComponent(AirLoopHVACUnitarySystem::iddObjectType(),model)
{
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
  autosizeSupplyAirFlowRateDuringCoolingOperation();
  autosizeSupplyAirFlowRateDuringHeatingOperation();
  autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
  setMaximumSupplyAirTemperature(80.0);
  setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(21.0);
  ok = setMaximumCyclingRate(2.5);
  OS_ASSERT(ok);
  ok = setHeatPumpTimeConstant(60.0);
  OS_ASSERT(ok);
  ok = setFractionofOnCyclePowerUse(0.01);
  OS_ASSERT(ok);
  ok = setHeatPumpFanDelayTime(60);
  OS_ASSERT(ok);
  ok = setAncilliaryOnCycleElectricPower(0.0);
  OS_ASSERT(ok);
  ok = setAncilliaryOffCycleElectricPower(0.0);
  OS_ASSERT(ok);
  // ok = setMaximumTemperatureforHeatRecovery(80.0);
  // OS_ASSERT(ok);
}

IddObjectType AirLoopHVACUnitarySystem::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirLoopHVAC_UnitarySystem);
}

std::vector<std::string> AirLoopHVACUnitarySystem::controlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_AirLoopHVAC_UnitarySystemFields::ControlType);
}

std::vector<std::string> AirLoopHVACUnitarySystem::dehumidificationControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_AirLoopHVAC_UnitarySystemFields::DehumidificationControlType);
}

std::vector<std::string> AirLoopHVACUnitarySystem::fanPlacementValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_AirLoopHVAC_UnitarySystemFields::FanPlacement);
}

std::vector<std::string> AirLoopHVACUnitarySystem::latentLoadControlValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_AirLoopHVAC_UnitarySystemFields::LatentLoadControl);
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

//std::string AirLoopHVACUnitarySystem::controlType() const {
//  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->controlType();
//}
//
//bool AirLoopHVACUnitarySystem::isControlTypeDefaulted() const {
//  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isControlTypeDefaulted();
//}

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

std::string AirLoopHVACUnitarySystem::latentLoadControl() const {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->latentLoadControl();
}

bool AirLoopHVACUnitarySystem::isLatentLoadControlDefaulted() const {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isLatentLoadControlDefaulted();
}

boost::optional<HVACComponent> AirLoopHVACUnitarySystem::supplementalHeatingCoil() const {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->supplementalHeatingCoil();
}

boost::optional<std::string> AirLoopHVACUnitarySystem::supplyAirFlowRateMethodDuringCoolingOperation() const {
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

boost::optional<std::string> AirLoopHVACUnitarySystem::supplyAirFlowRateMethodDuringHeatingOperation() const {
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

boost::optional<double> AirLoopHVACUnitarySystem::designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired() const {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired();
}

boost::optional<double> AirLoopHVACUnitarySystem::designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired() const {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired();
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

double AirLoopHVACUnitarySystem::maximumCyclingRate() const {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->maximumCyclingRate();
}

bool AirLoopHVACUnitarySystem::isMaximumCyclingRateDefaulted() const {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isMaximumCyclingRateDefaulted();
}

double AirLoopHVACUnitarySystem::heatPumpTimeConstant() const {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->heatPumpTimeConstant();
}

bool AirLoopHVACUnitarySystem::isHeatPumpTimeConstantDefaulted() const {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isHeatPumpTimeConstantDefaulted();
}

double AirLoopHVACUnitarySystem::fractionofOnCyclePowerUse() const {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->fractionofOnCyclePowerUse();
}

bool AirLoopHVACUnitarySystem::isFractionofOnCyclePowerUseDefaulted() const {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isFractionofOnCyclePowerUseDefaulted();
}

double AirLoopHVACUnitarySystem::heatPumpFanDelayTime() const {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->heatPumpFanDelayTime();
}

bool AirLoopHVACUnitarySystem::isHeatPumpFanDelayTimeDefaulted() const {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->isHeatPumpFanDelayTimeDefaulted();
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

//bool AirLoopHVACUnitarySystem::setControlType(std::string controlType) {
//  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setControlType(controlType);
//}
//
//void AirLoopHVACUnitarySystem::resetControlType() {
//  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetControlType();
//}

bool AirLoopHVACUnitarySystem::setControllingZoneorThermostatLocation(const ThermalZone& thermalZone) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setControllingZoneorThermostatLocation(thermalZone);
}

void AirLoopHVACUnitarySystem::resetControllingZoneorThermostatLocation() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetControllingZoneorThermostatLocation();
}

bool AirLoopHVACUnitarySystem::setDehumidificationControlType(std::string dehumidificationControlType) {
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

bool AirLoopHVACUnitarySystem::setFanPlacement(std::string fanPlacement) {
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

void AirLoopHVACUnitarySystem::setUseDOASDXCoolingCoil(bool useDOASDXCoolingCoil) {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setUseDOASDXCoolingCoil(useDOASDXCoolingCoil);
}

void AirLoopHVACUnitarySystem::resetUseDOASDXCoolingCoil() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetUseDOASDXCoolingCoil();
}

bool AirLoopHVACUnitarySystem::setDOASDXCoolingCoilLeavingMinimumAirTemperature(double dOASDXCoolingCoilLeavingMinimumAirTemperature) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setDOASDXCoolingCoilLeavingMinimumAirTemperature(dOASDXCoolingCoilLeavingMinimumAirTemperature);
}

void AirLoopHVACUnitarySystem::resetDOASDXCoolingCoilLeavingMinimumAirTemperature() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetDOASDXCoolingCoilLeavingMinimumAirTemperature();
}

bool AirLoopHVACUnitarySystem::setLatentLoadControl(std::string latentLoadControl) {
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

bool AirLoopHVACUnitarySystem::setSupplyAirFlowRateMethodDuringCoolingOperation(std::string supplyAirFlowRateMethodDuringCoolingOperation) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setSupplyAirFlowRateMethodDuringCoolingOperation(supplyAirFlowRateMethodDuringCoolingOperation);
}

void AirLoopHVACUnitarySystem::resetSupplyAirFlowRateMethodDuringCoolingOperation() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetSupplyAirFlowRateMethodDuringCoolingOperation();
}

bool AirLoopHVACUnitarySystem::setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setSupplyAirFlowRateDuringCoolingOperation(supplyAirFlowRateDuringCoolingOperation);
}

void AirLoopHVACUnitarySystem::resetSupplyAirFlowRateDuringCoolingOperation() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetSupplyAirFlowRateDuringCoolingOperation();
}

void AirLoopHVACUnitarySystem::autosizeSupplyAirFlowRateDuringCoolingOperation() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->autosizeSupplyAirFlowRateDuringCoolingOperation();
}

bool AirLoopHVACUnitarySystem::setSupplyAirFlowRatePerFloorAreaDuringCoolingOperation(double supplyAirFlowRatePerFloorAreaDuringCoolingOperation) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setSupplyAirFlowRatePerFloorAreaDuringCoolingOperation(supplyAirFlowRatePerFloorAreaDuringCoolingOperation);
}

void AirLoopHVACUnitarySystem::resetSupplyAirFlowRatePerFloorAreaDuringCoolingOperation() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetSupplyAirFlowRatePerFloorAreaDuringCoolingOperation();
}

bool AirLoopHVACUnitarySystem::setFractionofAutosizedDesignCoolingSupplyAirFlowRate(double fractionofAutosizedDesignCoolingSupplyAirFlowRate) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setFractionofAutosizedDesignCoolingSupplyAirFlowRate(fractionofAutosizedDesignCoolingSupplyAirFlowRate);
}

void AirLoopHVACUnitarySystem::resetFractionofAutosizedDesignCoolingSupplyAirFlowRate() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetFractionofAutosizedDesignCoolingSupplyAirFlowRate();
}

bool AirLoopHVACUnitarySystem::setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation(double designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation(designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation);
}

void AirLoopHVACUnitarySystem::resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation();
}

bool AirLoopHVACUnitarySystem::setSupplyAirFlowRateMethodDuringHeatingOperation(std::string supplyAirFlowRateMethodDuringHeatingOperation) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setSupplyAirFlowRateMethodDuringHeatingOperation(supplyAirFlowRateMethodDuringHeatingOperation);
}

void AirLoopHVACUnitarySystem::resetSupplyAirFlowRateMethodDuringHeatingOperation() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetSupplyAirFlowRateMethodDuringHeatingOperation();
}

bool AirLoopHVACUnitarySystem::setSupplyAirFlowRateDuringHeatingOperation(double supplyAirFlowRateDuringHeatingOperation) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setSupplyAirFlowRateDuringHeatingOperation(supplyAirFlowRateDuringHeatingOperation);
}

void AirLoopHVACUnitarySystem::resetSupplyAirFlowRateDuringHeatingOperation() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetSupplyAirFlowRateDuringHeatingOperation();
}

void AirLoopHVACUnitarySystem::autosizeSupplyAirFlowRateDuringHeatingOperation() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->autosizeSupplyAirFlowRateDuringHeatingOperation();
}

bool AirLoopHVACUnitarySystem::setSupplyAirFlowRatePerFloorAreaduringHeatingOperation(double supplyAirFlowRatePerFloorAreaduringHeatingOperation) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setSupplyAirFlowRatePerFloorAreaduringHeatingOperation(supplyAirFlowRatePerFloorAreaduringHeatingOperation);
}

void AirLoopHVACUnitarySystem::resetSupplyAirFlowRatePerFloorAreaduringHeatingOperation() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetSupplyAirFlowRatePerFloorAreaduringHeatingOperation();
}

bool AirLoopHVACUnitarySystem::setFractionofAutosizedDesignHeatingSupplyAirFlowRate(double fractionofAutosizedDesignHeatingSupplyAirFlowRate) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setFractionofAutosizedDesignHeatingSupplyAirFlowRate(fractionofAutosizedDesignHeatingSupplyAirFlowRate);
}

void AirLoopHVACUnitarySystem::resetFractionofAutosizedDesignHeatingSupplyAirFlowRate() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetFractionofAutosizedDesignHeatingSupplyAirFlowRate();
}

bool AirLoopHVACUnitarySystem::setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation(double designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation(designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation);
}

void AirLoopHVACUnitarySystem::resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation();
}

bool AirLoopHVACUnitarySystem::setSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired(std::string supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired(supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired);
}

void AirLoopHVACUnitarySystem::resetSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetSupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired();
}

bool AirLoopHVACUnitarySystem::setSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(double supplyAirFlowRateWhenNoCoolingorHeatingisRequired) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(supplyAirFlowRateWhenNoCoolingorHeatingisRequired);
}

void AirLoopHVACUnitarySystem::resetSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
}

void AirLoopHVACUnitarySystem::autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
}

bool AirLoopHVACUnitarySystem::setSupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired(double supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setSupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired(supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired);
}

void AirLoopHVACUnitarySystem::resetSupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetSupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired();
}

bool AirLoopHVACUnitarySystem::setFractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(double fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setFractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired);
}

void AirLoopHVACUnitarySystem::resetFractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetFractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
}

bool AirLoopHVACUnitarySystem::setFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(double fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired(fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired);
}

void AirLoopHVACUnitarySystem::resetFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetFractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
}

bool AirLoopHVACUnitarySystem::setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired(double designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired(designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired);
}

void AirLoopHVACUnitarySystem::resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetDesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired();
}

bool AirLoopHVACUnitarySystem::setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired(double designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired(designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired);
}

void AirLoopHVACUnitarySystem::resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetDesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired();
}

void AirLoopHVACUnitarySystem::setMaximumSupplyAirTemperature(double maximumSupplyAirTemperature) {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setMaximumSupplyAirTemperature(maximumSupplyAirTemperature);
}

void AirLoopHVACUnitarySystem::resetMaximumSupplyAirTemperature() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetMaximumSupplyAirTemperature();
}

void AirLoopHVACUnitarySystem::autosizeMaximumSupplyAirTemperature() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->autosizeMaximumSupplyAirTemperature();
}

void AirLoopHVACUnitarySystem::setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation) {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
}

void AirLoopHVACUnitarySystem::resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();
}

void AirLoopHVACUnitarySystem::setOutdoorDryBulbTemperatureSensorNodeName(std::string outdoorDryBulbTemperatureSensorNodeName) {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setOutdoorDryBulbTemperatureSensorNodeName(outdoorDryBulbTemperatureSensorNodeName);
}

void AirLoopHVACUnitarySystem::resetOutdoorDryBulbTemperatureSensorNodeName() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetOutdoorDryBulbTemperatureSensorNodeName();
}

bool AirLoopHVACUnitarySystem::setMaximumCyclingRate(double maximumCyclingRate) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setMaximumCyclingRate(maximumCyclingRate);
}

void AirLoopHVACUnitarySystem::resetMaximumCyclingRate() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetMaximumCyclingRate();
}

bool AirLoopHVACUnitarySystem::setHeatPumpTimeConstant(double heatPumpTimeConstant) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setHeatPumpTimeConstant(heatPumpTimeConstant);
}

void AirLoopHVACUnitarySystem::resetHeatPumpTimeConstant() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetHeatPumpTimeConstant();
}

bool AirLoopHVACUnitarySystem::setFractionofOnCyclePowerUse(double fractionofOnCyclePowerUse) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setFractionofOnCyclePowerUse(fractionofOnCyclePowerUse);
}

void AirLoopHVACUnitarySystem::resetFractionofOnCyclePowerUse() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetFractionofOnCyclePowerUse();
}

bool AirLoopHVACUnitarySystem::setHeatPumpFanDelayTime(double heatPumpFanDelayTime) {
  return getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->setHeatPumpFanDelayTime(heatPumpFanDelayTime);
}

void AirLoopHVACUnitarySystem::resetHeatPumpFanDelayTime() {
  getImpl<detail::AirLoopHVACUnitarySystem_Impl>()->resetHeatPumpFanDelayTime();
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

/// @cond
AirLoopHVACUnitarySystem::AirLoopHVACUnitarySystem(std::shared_ptr<detail::AirLoopHVACUnitarySystem_Impl> impl)
  : WaterToAirComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

