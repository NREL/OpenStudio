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

#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl(const IdfObject& idfObject,
                                                                                                         Model_Impl* model,
                                                                                                         bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::iddObjectType());
  }

  AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                         Model_Impl* model,
                                                                                                         bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::iddObjectType());
  }

  AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl(const AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl& other,
                                                                                                         Model_Impl* model,
                                                                                                         bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::iddObjectType() const {
    return AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::AvailabilitySchedule) != e)
    {
      result.push_back(ScheduleTypeKey("AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass","Availability Schedule"));
    }
    if (std::find(b,e,OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateMultiplierSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass","Outdoor Air Flow Rate Multiplier Schedule"));
    }
    if (std::find(b,e,OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SupplyAirFanOperatingModeSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass","Supply Air Fan Operating Mode Schedule"));
    }
    return result;
  }

  boost::optional<Schedule> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::AvailabilitySchedule);
  }

  boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::systemAirFlowRateDuringCoolingOperation() const {
    return getDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateDuringCoolingOperation,true);
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::isSystemAirFlowRateDuringCoolingOperationAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateDuringCoolingOperation, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::systemAirFlowRateDuringHeatingOperation() const {
    return getDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateDuringHeatingOperation,true);
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::isSystemAirFlowRateDuringHeatingOperationAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateDuringHeatingOperation, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::systemAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
    return getDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateWhenNoCoolingorHeatingisNeeded,true);
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::isSystemAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::outdoorAirFlowRateDuringCoolingOperation() const {
    return getDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateDuringCoolingOperation,true);
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::isOutdoorAirFlowRateDuringCoolingOperationAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateDuringCoolingOperation, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::outdoorAirFlowRateDuringHeatingOperation() const {
    return getDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateDuringHeatingOperation,true);
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::isOutdoorAirFlowRateDuringHeatingOperationAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateDuringHeatingOperation, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
    return getDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded,true);
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<Schedule> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::outdoorAirFlowRateMultiplierSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateMultiplierSchedule);
  }

  HVACComponent AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::supplyAirFan() const {
    boost::optional<HVACComponent> value = optionalSupplyAirFan();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Supply Air Fan attached.");
    }
    return value.get();
  }

  std::string AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::supplyAirFanPlacement() const {
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SupplyAirFanPlacement,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::supplyAirFanOperatingModeSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SupplyAirFanOperatingModeSchedule);
  }

  HVACComponent AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::coolingCoil() const {
    boost::optional<HVACComponent> value = optionalCoolingCoil();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Cooling Coil attached.");
    }
    return value.get();
  }

  HVACComponent AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::heatingCoil() const {
    boost::optional<HVACComponent> value = optionalHeatingCoil();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heating Coil attached.");
    }
    return value.get();
  }

  std::string AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::priorityControlMode() const {
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::PriorityControlMode,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::minimumOutletAirTemperatureDuringCoolingOperation() const {
    boost::optional<double> value = getDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::MinimumOutletAirTemperatureDuringCoolingOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  double AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::maximumOutletAirTemperatureDuringHeatingOperation() const {
    boost::optional<double> value = getDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::MaximumOutletAirTemperatureDuringHeatingOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::dehumidificationControlType() const {
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::DehumidificationControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::AvailabilitySchedule,
                              "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass",
                              "Availability Schedule",
                              schedule);
    return result;
  }

  void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::AvailabilitySchedule, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setSystemAirFlowRateDuringCoolingOperation(boost::optional<double> systemAirFlowRateDuringCoolingOperation) {
    bool result(false);
    if (systemAirFlowRateDuringCoolingOperation) {
      result = setDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateDuringCoolingOperation, systemAirFlowRateDuringCoolingOperation.get());
    }
    return result;
  }

  void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::autosizeSystemAirFlowRateDuringCoolingOperation() {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateDuringCoolingOperation, "autosize");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setSystemAirFlowRateDuringHeatingOperation(boost::optional<double> systemAirFlowRateDuringHeatingOperation) {
    bool result(false);
    if (systemAirFlowRateDuringHeatingOperation) {
      result = setDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateDuringHeatingOperation, systemAirFlowRateDuringHeatingOperation.get());
    }
    return result;
  }

  void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::autosizeSystemAirFlowRateDuringHeatingOperation() {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateDuringHeatingOperation, "autosize");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setSystemAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> systemAirFlowRateWhenNoCoolingorHeatingisNeeded) {
    bool result(false);
    if (systemAirFlowRateWhenNoCoolingorHeatingisNeeded) {
      result = setDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateWhenNoCoolingorHeatingisNeeded, systemAirFlowRateWhenNoCoolingorHeatingisNeeded.get());
    }
    return result;
  }

  void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::autosizeSystemAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateWhenNoCoolingorHeatingisNeeded, "autosize");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setOutdoorAirFlowRateDuringCoolingOperation(boost::optional<double> outdoorAirFlowRateDuringCoolingOperation) {
    bool result(false);
    if (outdoorAirFlowRateDuringCoolingOperation) {
      result = setDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateDuringCoolingOperation, outdoorAirFlowRateDuringCoolingOperation.get());
    }
    return result;
  }

  void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::autosizeOutdoorAirFlowRateDuringCoolingOperation() {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateDuringCoolingOperation, "autosize");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setOutdoorAirFlowRateDuringHeatingOperation(boost::optional<double> outdoorAirFlowRateDuringHeatingOperation) {
    bool result(false);
    if (outdoorAirFlowRateDuringHeatingOperation) {
      result = setDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateDuringHeatingOperation, outdoorAirFlowRateDuringHeatingOperation.get());
    }
    return result;
  }

  void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::autosizeOutdoorAirFlowRateDuringHeatingOperation() {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateDuringHeatingOperation, "autosize");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
    bool result(false);
    if (outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
      result = setDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded.get());
    }
    return result;
  }

  void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, "autosize");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setOutdoorAirFlowRateMultiplierSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateMultiplierSchedule,
                              "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass",
                              "Outdoor Air Flow Rate Multiplier Schedule",
                              schedule);
    return result;
  }

  void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::resetOutdoorAirFlowRateMultiplierSchedule() {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateMultiplierSchedule, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setSupplyAirFan(const HVACComponent& fansCVandOnOff) {
    bool result = setPointer(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SupplyAirFan, fansCVandOnOff.handle());
    return result;
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setSupplyAirFanPlacement(std::string supplyAirFanPlacement) {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SupplyAirFanPlacement, supplyAirFanPlacement);
    return result;
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setSupplyAirFanOperatingModeSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SupplyAirFanOperatingModeSchedule,
                              "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass",
                              "Supply Air Fan Operating Mode Schedule",
                              schedule);
    return result;
  }

  void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::resetSupplyAirFanOperatingModeSchedule() {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SupplyAirFanOperatingModeSchedule, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setCoolingCoil(const HVACComponent& coolingCoilsDXMultiModeOrSingleSpeed) {
    bool result = setPointer(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::CoolingCoil, coolingCoilsDXMultiModeOrSingleSpeed.handle());
    return result;
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setHeatingCoil(const HVACComponent& heatingCoilName) {
    bool result = setPointer(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::HeatingCoil, heatingCoilName.handle());
    return result;
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setPriorityControlMode(std::string priorityControlMode) {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::PriorityControlMode, priorityControlMode);
    return result;
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setMinimumOutletAirTemperatureDuringCoolingOperation(double minimumOutletAirTemperatureDuringCoolingOperation) {
    bool result = setDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::MinimumOutletAirTemperatureDuringCoolingOperation, minimumOutletAirTemperatureDuringCoolingOperation);
    return result;
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setMaximumOutletAirTemperatureDuringHeatingOperation(double maximumOutletAirTemperatureDuringHeatingOperation) {
    bool result = setDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::MaximumOutletAirTemperatureDuringHeatingOperation, maximumOutletAirTemperatureDuringHeatingOperation);
    return result;
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setDehumidificationControlType(std::string dehumidificationControlType) {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::DehumidificationControlType, dehumidificationControlType);
    return result;
  }

  boost::optional<HVACComponent> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::optionalSupplyAirFan() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SupplyAirFan);
  }

  boost::optional<HVACComponent> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::optionalCoolingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::CoolingCoil);
  }

  boost::optional<HVACComponent> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::optionalHeatingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::HeatingCoil);
  }

  unsigned AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::inletPort()
  {
    return OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::AirInletNode;
  }

  unsigned AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::outletPort()
  {
    return OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::AirOutletNode;
  }

  ModelObject AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::clone(Model model) const
  {
    AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass unitaryClone = StraightComponent_Impl::clone(model).cast<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass>();

    HVACComponent supplyFanClone = this->supplyAirFan().clone(model).cast<HVACComponent>();
    HVACComponent heatingCoilClone = this->heatingCoil().clone(model).cast<HVACComponent>();
    HVACComponent coolingCoilClone = this->coolingCoil().clone(model).cast<HVACComponent>();

    unitaryClone.setSupplyAirFan(supplyFanClone);
    unitaryClone.setHeatingCoil(heatingCoilClone);
    unitaryClone.setCoolingCoil(coolingCoilClone);

    return unitaryClone;
  }

  std::vector<IdfObject> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::remove()
  {
    std::vector<IdfObject> result;

    if ( boost::optional<HVACComponent> _supplyFan = this->supplyAirFan() ) {
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

    std::vector<IdfObject> removedUnitarySystem = StraightComponent_Impl::remove();
    result.insert(result.end(), removedUnitarySystem.begin(), removedUnitarySystem.end());

    return result;
  }

  std::vector<ModelObject> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::children() const
  {
    std::vector<ModelObject> result;

    if( boost::optional<HVACComponent> supplyFan = this->supplyAirFan()) {
      result.push_back( *supplyFan );
    }
    if( boost::optional<HVACComponent> coolingCoil = this->coolingCoil()) {
      result.push_back( *coolingCoil );
    }
    if( boost::optional<HVACComponent> heatingCoil = this->heatingCoil()) {
      result.push_back( *heatingCoil );
    }

    return result;
  }

} // detail

AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass(const Model& model,
  const HVACComponent& fan,
  const HVACComponent& coolingCoil,
  const HVACComponent& heatingCoil)
  : StraightComponent(AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>());

  bool ok;
  ok = setCoolingCoil(coolingCoil);
  if( ! ok ) {
    LOG_AND_THROW("Unable to set cooling coil");
  }
  ok = setHeatingCoil(heatingCoil);
  if( ! ok ) {
    LOG_AND_THROW("Unable to set heating coil");
  }
  ok = setSupplyAirFan(fan);
  if( ! ok ) {
    LOG_AND_THROW("Unable to set fan");
  }

  autosizeSystemAirFlowRateDuringCoolingOperation();
  autosizeSystemAirFlowRateDuringHeatingOperation();
  autosizeSystemAirFlowRateWhenNoCoolingorHeatingisNeeded();
  autosizeOutdoorAirFlowRateDuringCoolingOperation();
  autosizeOutdoorAirFlowRateDuringHeatingOperation();
  autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
  setSupplyAirFanPlacement("DrawThrough");
  setPriorityControlMode("ZonePriority");
  setMinimumOutletAirTemperatureDuringCoolingOperation(8.0);
  setMaximumOutletAirTemperatureDuringHeatingOperation(50.0);
  setDehumidificationControlType("None");
}

IddObjectType AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::iddObjectType() {
  return IddObjectType(IddObjectType::OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass);
}

std::vector<std::string> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::supplyAirFanPlacementValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SupplyAirFanPlacement);
}

std::vector<std::string> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::priorityControlModeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::PriorityControlMode);
}

std::vector<std::string> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::dehumidificationControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::DehumidificationControlType);
}

boost::optional<Schedule> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::availabilitySchedule() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->availabilitySchedule();
}

boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::systemAirFlowRateDuringCoolingOperation() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->systemAirFlowRateDuringCoolingOperation();
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::isSystemAirFlowRateDuringCoolingOperationAutosized() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->isSystemAirFlowRateDuringCoolingOperationAutosized();
}

boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::systemAirFlowRateDuringHeatingOperation() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->systemAirFlowRateDuringHeatingOperation();
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::isSystemAirFlowRateDuringHeatingOperationAutosized() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->isSystemAirFlowRateDuringHeatingOperationAutosized();
}

boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::systemAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->systemAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::isSystemAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->isSystemAirFlowRateWhenNoCoolingorHeatingisNeededAutosized();
}

boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::outdoorAirFlowRateDuringCoolingOperation() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->outdoorAirFlowRateDuringCoolingOperation();
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::isOutdoorAirFlowRateDuringCoolingOperationAutosized() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->isOutdoorAirFlowRateDuringCoolingOperationAutosized();
}

boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::outdoorAirFlowRateDuringHeatingOperation() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->outdoorAirFlowRateDuringHeatingOperation();
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::isOutdoorAirFlowRateDuringHeatingOperationAutosized() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->isOutdoorAirFlowRateDuringHeatingOperationAutosized();
}

boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized();
}

boost::optional<Schedule> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::outdoorAirFlowRateMultiplierSchedule() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->outdoorAirFlowRateMultiplierSchedule();
}

HVACComponent AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::supplyAirFan() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->supplyAirFan();
}

std::string AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::supplyAirFanPlacement() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->supplyAirFanPlacement();
}

boost::optional<Schedule> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::supplyAirFanOperatingModeSchedule() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->supplyAirFanOperatingModeSchedule();
}

HVACComponent AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::coolingCoil() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->coolingCoil();
}

HVACComponent AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::heatingCoil() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->heatingCoil();
}

std::string AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::priorityControlMode() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->priorityControlMode();
}

double AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::minimumOutletAirTemperatureDuringCoolingOperation() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->minimumOutletAirTemperatureDuringCoolingOperation();
}

double AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::maximumOutletAirTemperatureDuringHeatingOperation() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->maximumOutletAirTemperatureDuringHeatingOperation();
}

std::string AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::dehumidificationControlType() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->dehumidificationControlType();
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setAvailabilitySchedule(schedule);
}

void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::resetAvailabilitySchedule() {
  getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->resetAvailabilitySchedule();
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setSystemAirFlowRateDuringCoolingOperation(double systemAirFlowRateDuringCoolingOperation) {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setSystemAirFlowRateDuringCoolingOperation(systemAirFlowRateDuringCoolingOperation);
}

void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::autosizeSystemAirFlowRateDuringCoolingOperation() {
  getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->autosizeSystemAirFlowRateDuringCoolingOperation();
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setSystemAirFlowRateDuringHeatingOperation(double systemAirFlowRateDuringHeatingOperation) {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setSystemAirFlowRateDuringHeatingOperation(systemAirFlowRateDuringHeatingOperation);
}

void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::autosizeSystemAirFlowRateDuringHeatingOperation() {
  getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->autosizeSystemAirFlowRateDuringHeatingOperation();
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setSystemAirFlowRateWhenNoCoolingorHeatingisNeeded(double systemAirFlowRateWhenNoCoolingorHeatingisNeeded) {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setSystemAirFlowRateWhenNoCoolingorHeatingisNeeded(systemAirFlowRateWhenNoCoolingorHeatingisNeeded);
}

void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::autosizeSystemAirFlowRateWhenNoCoolingorHeatingisNeeded() {
  getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->autosizeSystemAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setOutdoorAirFlowRateDuringCoolingOperation(double outdoorAirFlowRateDuringCoolingOperation) {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setOutdoorAirFlowRateDuringCoolingOperation(outdoorAirFlowRateDuringCoolingOperation);
}

void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::autosizeOutdoorAirFlowRateDuringCoolingOperation() {
  getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->autosizeOutdoorAirFlowRateDuringCoolingOperation();
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setOutdoorAirFlowRateDuringHeatingOperation(double outdoorAirFlowRateDuringHeatingOperation) {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setOutdoorAirFlowRateDuringHeatingOperation(outdoorAirFlowRateDuringHeatingOperation);
}

void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::autosizeOutdoorAirFlowRateDuringHeatingOperation() {
  getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->autosizeOutdoorAirFlowRateDuringHeatingOperation();
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(double outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);
}

void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
  getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setOutdoorAirFlowRateMultiplierSchedule(Schedule& outdoorAirFlowRateMultiplierSchedule) {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setOutdoorAirFlowRateMultiplierSchedule(outdoorAirFlowRateMultiplierSchedule);
}

void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::resetOutdoorAirFlowRateMultiplierSchedule() {
  getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->resetOutdoorAirFlowRateMultiplierSchedule();
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setSupplyAirFan(const HVACComponent& fansCVandOnOff) {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setSupplyAirFan(fansCVandOnOff);
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setSupplyAirFanPlacement(std::string supplyAirFanPlacement) {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setSupplyAirFanPlacement(supplyAirFanPlacement);
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setSupplyAirFanOperatingModeSchedule(Schedule& schedule) {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setSupplyAirFanOperatingModeSchedule(schedule);
}

void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::resetSupplyAirFanOperatingModeSchedule() {
  getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->resetSupplyAirFanOperatingModeSchedule();
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setCoolingCoil(const HVACComponent& coolingCoilsDXMultiModeOrSingleSpeed) {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setCoolingCoil(coolingCoilsDXMultiModeOrSingleSpeed);
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setHeatingCoil(const HVACComponent& heatingCoilName) {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setHeatingCoil(heatingCoilName);
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setPriorityControlMode(std::string priorityControlMode) {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setPriorityControlMode(priorityControlMode);
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setMinimumOutletAirTemperatureDuringCoolingOperation(double minimumOutletAirTemperatureDuringCoolingOperation) {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setMinimumOutletAirTemperatureDuringCoolingOperation(minimumOutletAirTemperatureDuringCoolingOperation);
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setMaximumOutletAirTemperatureDuringHeatingOperation(double maximumOutletAirTemperatureDuringHeatingOperation) {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setMaximumOutletAirTemperatureDuringHeatingOperation(maximumOutletAirTemperatureDuringHeatingOperation);
}

bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setDehumidificationControlType(std::string dehumidificationControlType) {
  return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setDehumidificationControlType(dehumidificationControlType);
}

/// @cond
AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass(std::shared_ptr<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

