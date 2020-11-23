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

#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Mixer.hpp"
#include "Mixer_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
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
                                                                                                           Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::iddObjectType());
    }

    AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::iddObjectType());
    }

    AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl(
      const AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::iddObjectType() const {
      return AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::iddObjectType();
    }

    std::vector<ScheduleTypeKey> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
      if (std::find(b, e, OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::AvailabilitySchedule) != e) {
        result.push_back(ScheduleTypeKey("AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass", "Availability Schedule"));
      }
      if (std::find(b, e, OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateMultiplierSchedule) != e) {
        result.push_back(ScheduleTypeKey("AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass", "Outdoor Air Flow Rate Multiplier Schedule"));
      }
      if (std::find(b, e, OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SupplyAirFanOperatingModeSchedule) != e) {
        result.push_back(ScheduleTypeKey("AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass", "Supply Air Fan Operating Mode Schedule"));
      }
      return result;
    }

    boost::optional<Schedule> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::availabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::AvailabilitySchedule);
    }

    boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::systemAirFlowRateDuringCoolingOperation() const {
      return getDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateDuringCoolingOperation, true);
    }

    bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::isSystemAirFlowRateDuringCoolingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateDuringCoolingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::systemAirFlowRateDuringHeatingOperation() const {
      return getDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateDuringHeatingOperation, true);
    }

    bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::isSystemAirFlowRateDuringHeatingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateDuringHeatingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::systemAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
      return getDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
    }

    bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::isSystemAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::outdoorAirFlowRateDuringCoolingOperation() const {
      return getDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateDuringCoolingOperation, true);
    }

    bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::isOutdoorAirFlowRateDuringCoolingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateDuringCoolingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::outdoorAirFlowRateDuringHeatingOperation() const {
      return getDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateDuringHeatingOperation, true);
    }

    bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::isOutdoorAirFlowRateDuringHeatingOperationAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateDuringHeatingOperation, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
      return getDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
    }

    bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
      bool result = false;
      boost::optional<std::string> value =
        getString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<Schedule> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::outdoorAirFlowRateMultiplierSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateMultiplierSchedule);
    }

    HVACComponent AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::supplyAirFan() const {
      boost::optional<HVACComponent> value = optionalSupplyAirFan();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Supply Air Fan attached.");
      }
      return value.get();
    }

    std::string AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::supplyAirFanPlacement() const {
      boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SupplyAirFanPlacement, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::supplyAirFanOperatingModeSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(
        OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SupplyAirFanOperatingModeSchedule);
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
      boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::PriorityControlMode, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::minimumOutletAirTemperatureDuringCoolingOperation() const {
      boost::optional<double> value =
        getDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::MinimumOutletAirTemperatureDuringCoolingOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::maximumOutletAirTemperatureDuringHeatingOperation() const {
      boost::optional<double> value =
        getDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::MaximumOutletAirTemperatureDuringHeatingOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::dehumidificationControlType() const {
      boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::DehumidificationControlType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::AvailabilitySchedule,
                                "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass", "Availability Schedule", schedule);
      return result;
    }

    void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::resetAvailabilitySchedule() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::AvailabilitySchedule, "");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setSystemAirFlowRateDuringCoolingOperation(
      boost::optional<double> systemAirFlowRateDuringCoolingOperation) {
      bool result(false);
      if (systemAirFlowRateDuringCoolingOperation) {
        result = setDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateDuringCoolingOperation,
                           systemAirFlowRateDuringCoolingOperation.get());
      }
      return result;
    }

    void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::autosizeSystemAirFlowRateDuringCoolingOperation() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateDuringCoolingOperation, "autosize");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setSystemAirFlowRateDuringHeatingOperation(
      boost::optional<double> systemAirFlowRateDuringHeatingOperation) {
      bool result(false);
      if (systemAirFlowRateDuringHeatingOperation) {
        result = setDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateDuringHeatingOperation,
                           systemAirFlowRateDuringHeatingOperation.get());
      }
      return result;
    }

    void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::autosizeSystemAirFlowRateDuringHeatingOperation() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateDuringHeatingOperation, "autosize");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setSystemAirFlowRateWhenNoCoolingorHeatingisNeeded(
      boost::optional<double> systemAirFlowRateWhenNoCoolingorHeatingisNeeded) {
      bool result(false);
      if (systemAirFlowRateWhenNoCoolingorHeatingisNeeded) {
        result = setDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateWhenNoCoolingorHeatingisNeeded,
                           systemAirFlowRateWhenNoCoolingorHeatingisNeeded.get());
      }
      return result;
    }

    void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::autosizeSystemAirFlowRateWhenNoCoolingorHeatingisNeeded() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SystemAirFlowRateWhenNoCoolingorHeatingisNeeded, "autosize");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setOutdoorAirFlowRateDuringCoolingOperation(
      boost::optional<double> outdoorAirFlowRateDuringCoolingOperation) {
      bool result(false);
      if (outdoorAirFlowRateDuringCoolingOperation) {
        result = setDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateDuringCoolingOperation,
                           outdoorAirFlowRateDuringCoolingOperation.get());
      }
      return result;
    }

    void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::autosizeOutdoorAirFlowRateDuringCoolingOperation() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateDuringCoolingOperation, "autosize");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setOutdoorAirFlowRateDuringHeatingOperation(
      boost::optional<double> outdoorAirFlowRateDuringHeatingOperation) {
      bool result(false);
      if (outdoorAirFlowRateDuringHeatingOperation) {
        result = setDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateDuringHeatingOperation,
                           outdoorAirFlowRateDuringHeatingOperation.get());
      }
      return result;
    }

    void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::autosizeOutdoorAirFlowRateDuringHeatingOperation() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateDuringHeatingOperation, "autosize");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(
      boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
      bool result(false);
      if (outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
        result = setDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded,
                           outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded.get());
      }
      return result;
    }

    void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
      bool result = setString(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, "autosize");
      OS_ASSERT(result);
    }

    bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setOutdoorAirFlowRateMultiplierSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateMultiplierSchedule,
                                "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass", "Outdoor Air Flow Rate Multiplier Schedule", schedule);
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
                                "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass", "Supply Air Fan Operating Mode Schedule", schedule);
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

    bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setMinimumOutletAirTemperatureDuringCoolingOperation(
      double minimumOutletAirTemperatureDuringCoolingOperation) {
      bool result = setDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::MinimumOutletAirTemperatureDuringCoolingOperation,
                              minimumOutletAirTemperatureDuringCoolingOperation);
      return result;
    }

    bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setMaximumOutletAirTemperatureDuringHeatingOperation(
      double maximumOutletAirTemperatureDuringHeatingOperation) {
      bool result = setDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::MaximumOutletAirTemperatureDuringHeatingOperation,
                              maximumOutletAirTemperatureDuringHeatingOperation);
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

    unsigned AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::inletPort() const {
      return OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::AirInletNode;
    }

    unsigned AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::outletPort() const {
      return OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::AirOutletNode;
    }

    ModelObject AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::clone(Model model) const {
      AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass unitaryClone =
        StraightComponent_Impl::clone(model).cast<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass>();

      HVACComponent supplyFanClone = this->supplyAirFan().clone(model).cast<HVACComponent>();
      HVACComponent heatingCoilClone = this->heatingCoil().clone(model).cast<HVACComponent>();
      HVACComponent coolingCoilClone = this->coolingCoil().clone(model).cast<HVACComponent>();

      unitaryClone.setSupplyAirFan(supplyFanClone);
      unitaryClone.setHeatingCoil(heatingCoilClone);
      unitaryClone.setCoolingCoil(coolingCoilClone);

      // We need this because "connect" is first going to try to disconnect from anything
      // currently attached.  At this point unitaryClone is left pointing (through a connection) to the old air node,
      // (because of ModelObject::clone behavior) so connecting to the new node will remove the connection joining
      // the original unitary and the original node.
      unitaryClone.setString(this->plenumorMixerAirPort(), "");
      // Create a node for the Plenum or Mixer Air
      Node node(model);
      model.connect(unitaryClone, this->plenumorMixerAirPort(), node, node.inletPort());

      return unitaryClone;
    }

    std::vector<IdfObject> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::remove() {
      std::vector<IdfObject> result;

      if (boost::optional<HVACComponent> _supplyFan = this->supplyAirFan()) {
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

      // Removes branch on Plenum corresponding to this object
      resetPlenumorMixer();
      // delete the node
      Node mixerNode = plenumorMixerNode();
      mixerNode.disconnect();
      std::vector<IdfObject> removedNode = mixerNode.remove();
      result.insert(result.end(), removedNode.begin(), removedNode.end());

      std::vector<IdfObject> removedUnitarySystem = StraightComponent_Impl::remove();
      result.insert(result.end(), removedUnitarySystem.begin(), removedUnitarySystem.end());

      return result;
    }

    std::vector<ModelObject> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::children() const {
      std::vector<ModelObject> result;

      if (boost::optional<HVACComponent> supplyFan = this->supplyAirFan()) {
        result.push_back(*supplyFan);
      }
      if (boost::optional<HVACComponent> coolingCoil = this->coolingCoil()) {
        result.push_back(*coolingCoil);
      }
      if (boost::optional<HVACComponent> heatingCoil = this->heatingCoil()) {
        result.push_back(*heatingCoil);
      }

      return result;
    }

    double AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::minimumRuntimeBeforeOperatingModeChange() const {
      boost::optional<double> value =
        getDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::MinimumRuntimeBeforeOperatingModeChange, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setMinimumRuntimeBeforeOperatingModeChange(double runtime) {
      bool result = setDouble(OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::MinimumRuntimeBeforeOperatingModeChange, runtime);
      return result;
    }

    unsigned AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::plenumorMixerAirPort() const {
      return OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::PlenumorMixerInletNodeName;
    }

    Node AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::plenumorMixerNode() const {
      return this->connectedObject(this->plenumorMixerAirPort())->cast<Node>();
    }

    boost::optional<Mixer> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::plenumorMixer() const {
      boost::optional<Mixer> result;

      if (boost::optional<ModelObject> _mo = plenumorMixerNode().outletModelObject()) {
        result = _mo->optionalCast<Mixer>();
        OS_ASSERT(result);
      }

      return result;
    }

    bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::setPlenumorMixer(const Mixer& returnPathComponent) {

      if ((returnPathComponent.iddObjectType() != IddObjectType::OS_AirLoopHVAC_ZoneMixer)
          && (returnPathComponent.iddObjectType() != IddObjectType::OS_AirLoopHVAC_ReturnPlenum)) {
        LOG(Error, briefDescription() << " can only be connected to an AirLoopHVACZoneMixer or an AirLoopHVACReturnPlenum.");
        return false;
      }

      bool result = false;

      if (boost::optional<AirLoopHVAC> mixerLoop = returnPathComponent.airLoopHVAC()) {
        if (boost::optional<AirLoopHVAC> thisLoop = this->airLoopHVAC()) {
          if (mixerLoop == thisLoop) {
            result = true;
          }
        }
      }

      if (result) {
        // Reset any existing plenum or mixer
        resetPlenumorMixer();
        Node mixerNode = plenumorMixerNode();
        this->model().connect(mixerNode, mixerNode.outletPort(), returnPathComponent, returnPathComponent.nextInletPort());
      } else {
        LOG(Warn, briefDescription() << " cannot be connected with a " << returnPathComponent.briefDescription()
                                     << " unless they are both on the same AirLoopHVAC.")
      }

      return result;
    }

    void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::resetPlenumorMixer() {
      Node mixerNode = plenumorMixerNode();
      if (boost::optional<Mixer> existingMixer = plenumorMixer()) {
        existingMixer->removePortForBranch(existingMixer->branchIndexForInletModelObject(mixerNode));
      }
      this->model().disconnect(mixerNode, mixerNode.outletPort());
    }

    boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::autosizedSystemAirFlowRateDuringCoolingOperation() const {
      return getAutosizedValue("maximum cooling air flow rate", "m3/s");
    }

    boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::autosizedSystemAirFlowRateDuringHeatingOperation() const {
      return getAutosizedValue("maximum heating air flow rate", "m3/s");
    }

    boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::autosizedSystemAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
      return getAutosizedValue("maximum air flow rate when compressor/coil is off", "m3/s");
    }

    boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::autosizedOutdoorAirFlowRateDuringCoolingOperation() const {
      return getAutosizedValue("maximum outside air flow rate in cooling", "m3/s");
    }

    boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::autosizedOutdoorAirFlowRateDuringHeatingOperation() const {
      return getAutosizedValue("maximum outdoor air flow rate in heating", "m3/s");
    }

    boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::autosizedOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
      return getAutosizedValue("maximum outdoor air flow rate when compressor is off", "m3/s");
    }

    void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::autosize() {
      autosizeSystemAirFlowRateDuringCoolingOperation();
      autosizeSystemAirFlowRateDuringHeatingOperation();
      autosizeSystemAirFlowRateWhenNoCoolingorHeatingisNeeded();
      autosizeOutdoorAirFlowRateDuringCoolingOperation();
      autosizeOutdoorAirFlowRateDuringHeatingOperation();
      autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
    }

    void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedSystemAirFlowRateDuringCoolingOperation();
      if (val) {
        setSystemAirFlowRateDuringCoolingOperation(val.get());
      }

      val = autosizedSystemAirFlowRateDuringHeatingOperation();
      if (val) {
        setSystemAirFlowRateDuringHeatingOperation(val.get());
      }

      val = autosizedSystemAirFlowRateWhenNoCoolingorHeatingisNeeded();
      if (val) {
        setSystemAirFlowRateWhenNoCoolingorHeatingisNeeded(val.get());
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

    std::vector<EMSActuatorNames> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{
        {"AirLoopHVAC:UnitaryHeatCool", "Autosized Supply Air Flow Rate"},
        {"AirLoopHVAC:UnitaryHeatCool", "Autosized Supply Air Flow Rate During Cooling Operation"},
        {"AirLoopHVAC:UnitaryHeatCool", "Autosized Supply Air Flow Rate During Heating Operation"},
        {"AirLoopHVAC:UnitaryHeatCool", "Autosized Supply Air Flow Rate During No Heating or Cooling Operation"},
        {"Unitary HVAC", "Sensible Load Request"},
        {"Unitary HVAC", "Moisture Load Request"}};
      return actuators;
    }

    std::vector<std::string> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types{"Unitary HVAC Design Heating Capacity", "Unitary HVAC Design Cooling Capacity"};
      return types;
    }

  }  // namespace detail

  AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass(const Model& model, const HVACComponent& fan,
                                                                                               const HVACComponent& coolingCoil,
                                                                                               const HVACComponent& heatingCoil)
    : StraightComponent(AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>());

    bool ok;
    ok = setCoolingCoil(coolingCoil);
    if (!ok) {
      LOG_AND_THROW("Unable to set cooling coil");
    }
    ok = setHeatingCoil(heatingCoil);
    if (!ok) {
      LOG_AND_THROW("Unable to set heating coil");
    }
    ok = setSupplyAirFan(fan);
    if (!ok) {
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
    // This field is a bit weird, in the sense that if it's not present in the IDF it's 0, if it's present and blank it's 0.25
    // In order to try to maintain historical behavior, default to 0
    setMinimumRuntimeBeforeOperatingModeChange(0.0);

    // Create a node for the Plenum or Mixer Air
    Node node(model);
    model.connect(*this, this->plenumorMixerAirPort(), node, node.inletPort());
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
    return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setSystemAirFlowRateDuringCoolingOperation(
      systemAirFlowRateDuringCoolingOperation);
  }

  void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::autosizeSystemAirFlowRateDuringCoolingOperation() {
    getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->autosizeSystemAirFlowRateDuringCoolingOperation();
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setSystemAirFlowRateDuringHeatingOperation(double systemAirFlowRateDuringHeatingOperation) {
    return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setSystemAirFlowRateDuringHeatingOperation(
      systemAirFlowRateDuringHeatingOperation);
  }

  void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::autosizeSystemAirFlowRateDuringHeatingOperation() {
    getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->autosizeSystemAirFlowRateDuringHeatingOperation();
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setSystemAirFlowRateWhenNoCoolingorHeatingisNeeded(
    double systemAirFlowRateWhenNoCoolingorHeatingisNeeded) {
    return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setSystemAirFlowRateWhenNoCoolingorHeatingisNeeded(
      systemAirFlowRateWhenNoCoolingorHeatingisNeeded);
  }

  void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::autosizeSystemAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->autosizeSystemAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setOutdoorAirFlowRateDuringCoolingOperation(double outdoorAirFlowRateDuringCoolingOperation) {
    return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setOutdoorAirFlowRateDuringCoolingOperation(
      outdoorAirFlowRateDuringCoolingOperation);
  }

  void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::autosizeOutdoorAirFlowRateDuringCoolingOperation() {
    getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->autosizeOutdoorAirFlowRateDuringCoolingOperation();
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setOutdoorAirFlowRateDuringHeatingOperation(double outdoorAirFlowRateDuringHeatingOperation) {
    return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setOutdoorAirFlowRateDuringHeatingOperation(
      outdoorAirFlowRateDuringHeatingOperation);
  }

  void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::autosizeOutdoorAirFlowRateDuringHeatingOperation() {
    getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->autosizeOutdoorAirFlowRateDuringHeatingOperation();
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(
    double outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
    return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(
      outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);
  }

  void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setOutdoorAirFlowRateMultiplierSchedule(Schedule& outdoorAirFlowRateMultiplierSchedule) {
    return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setOutdoorAirFlowRateMultiplierSchedule(
      outdoorAirFlowRateMultiplierSchedule);
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

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setMinimumOutletAirTemperatureDuringCoolingOperation(
    double minimumOutletAirTemperatureDuringCoolingOperation) {
    return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setMinimumOutletAirTemperatureDuringCoolingOperation(
      minimumOutletAirTemperatureDuringCoolingOperation);
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setMaximumOutletAirTemperatureDuringHeatingOperation(
    double maximumOutletAirTemperatureDuringHeatingOperation) {
    return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setMaximumOutletAirTemperatureDuringHeatingOperation(
      maximumOutletAirTemperatureDuringHeatingOperation);
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setDehumidificationControlType(std::string dehumidificationControlType) {
    return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setDehumidificationControlType(dehumidificationControlType);
  }

  double AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::minimumRuntimeBeforeOperatingModeChange() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->minimumRuntimeBeforeOperatingModeChange();
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setMinimumRuntimeBeforeOperatingModeChange(double runtime) {
    return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setMinimumRuntimeBeforeOperatingModeChange(runtime);
  }

  unsigned AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::plenumorMixerAirPort() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->plenumorMixerAirPort();
  }

  Node AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::plenumorMixerNode() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->plenumorMixerNode();
  }

  boost::optional<Mixer> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::plenumorMixer() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->plenumorMixer();
  }

  bool AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setPlenumorMixer(const Mixer& returnPathComponent) {
    return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->setPlenumorMixer(returnPathComponent);
  }

  void AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::resetPlenumorMixer() {
    getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->resetPlenumorMixer();
  }

  /// @cond
  AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass(
    std::shared_ptr<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl> impl)
    : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::autosizedSystemAirFlowRateDuringCoolingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->autosizedSystemAirFlowRateDuringCoolingOperation();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::autosizedSystemAirFlowRateDuringHeatingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->autosizedSystemAirFlowRateDuringHeatingOperation();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::autosizedSystemAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->autosizedSystemAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::autosizedOutdoorAirFlowRateDuringCoolingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->autosizedOutdoorAirFlowRateDuringCoolingOperation();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::autosizedOutdoorAirFlowRateDuringHeatingOperation() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->autosizedOutdoorAirFlowRateDuringHeatingOperation();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::autosizedOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
    return getImpl<detail::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl>()->autosizedOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
  }

}  // namespace model
}  // namespace openstudio
