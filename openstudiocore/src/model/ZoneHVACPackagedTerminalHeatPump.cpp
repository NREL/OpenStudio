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

#include "ZoneHVACPackagedTerminalHeatPump.hpp"
#include "ZoneHVACPackagedTerminalHeatPump_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "WaterToAirComponent.hpp"
#include "WaterToAirComponent_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>


#include <utilities/idd/OS_ZoneHVAC_PackagedTerminalHeatPump_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  ZoneHVACPackagedTerminalHeatPump_Impl::ZoneHVACPackagedTerminalHeatPump_Impl(
      const IdfObject& idfObject,
      Model_Impl* model,
      bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneHVACPackagedTerminalHeatPump::iddObjectType());
  }

  ZoneHVACPackagedTerminalHeatPump_Impl::ZoneHVACPackagedTerminalHeatPump_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneHVACPackagedTerminalHeatPump::iddObjectType());
  }

  ZoneHVACPackagedTerminalHeatPump_Impl::ZoneHVACPackagedTerminalHeatPump_Impl(const ZoneHVACPackagedTerminalHeatPump_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

  ModelObject ZoneHVACPackagedTerminalHeatPump_Impl::clone(Model model) const
  {
    ZoneHVACPackagedTerminalHeatPump pthpClone = ZoneHVACComponent_Impl::clone(model).cast<ZoneHVACPackagedTerminalHeatPump>();

    //Schedule scheduleClone = this->availabilitySchedule().clone(model).cast<Schedule>();

    HVACComponent supplyFanClone = this->supplyAirFan().clone(model).cast<HVACComponent>();

    auto t_heatingCoil = heatingCoil();
    HVACComponent heatingCoilClone = t_heatingCoil.clone(model).cast<HVACComponent>();

    auto t_coolingCoil = coolingCoil();
    HVACComponent coolingCoilClone = t_coolingCoil.clone(model).cast<HVACComponent>();

    HVACComponent supplementalHeatingCoilClone = this->supplementalHeatingCoil().clone(model).cast<HVACComponent>();

    //pthpClone.setAvailabilitySchedule(scheduleClone);

    pthpClone.setSupplyAirFan(supplyFanClone);

    pthpClone.setHeatingCoil(heatingCoilClone);

    pthpClone.setCoolingCoil(coolingCoilClone);

    pthpClone.setSupplementalHeatingCoil(supplementalHeatingCoilClone);

    if( model == this->model() ) {
      if( auto waterToAirComponent = t_coolingCoil.optionalCast<WaterToAirComponent>() ) {
        if( auto plant = waterToAirComponent->plantLoop() ) {
          plant->addDemandBranchForComponent(coolingCoilClone);
        }
      }
      if( auto waterToAirComponent = t_heatingCoil.optionalCast<WaterToAirComponent>() ) {
        if( auto plant = waterToAirComponent->plantLoop() ) {
          plant->addDemandBranchForComponent(heatingCoilClone);
        }
      }
    }

    return pthpClone;
  }

  const std::vector<std::string>& ZoneHVACPackagedTerminalHeatPump_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ZoneHVACPackagedTerminalHeatPump_Impl::iddObjectType() const {
    return ZoneHVACPackagedTerminalHeatPump::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneHVACPackagedTerminalHeatPump_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneHVAC_PackagedTerminalHeatPumpFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACPackagedTerminalHeatPump","Availability"));
    }
    if (std::find(b,e,OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFanOperatingModeScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACPackagedTerminalHeatPump","Supply Air Fan Operating Mode"));
    }
    return result;
  }

  std::vector<ModelObject> ZoneHVACPackagedTerminalHeatPump_Impl::children() const
  {
    std::vector<ModelObject> result;
    if (OptionalHVACComponent intermediate = optionalSupplyAirFan()) {
      result.push_back(*intermediate);
    }
    if (OptionalHVACComponent intermediate = optionalHeatingCoil()) {
      result.push_back(*intermediate);
    }
    if (OptionalHVACComponent intermediate = optionalCoolingCoil()) {
      result.push_back(*intermediate);
    }
    if (OptionalHVACComponent intermediate = optionalSupplementalHeatingCoil()) {
      result.push_back(*intermediate);
    }
    return result;
  }

  unsigned ZoneHVACPackagedTerminalHeatPump_Impl::inletPort() const
  {
    return OS_ZoneHVAC_PackagedTerminalHeatPumpFields::AirInletNodeName;
  }

  unsigned ZoneHVACPackagedTerminalHeatPump_Impl::outletPort() const
  {
    return OS_ZoneHVAC_PackagedTerminalHeatPumpFields::AirOutletNodeName;
  }

  Schedule ZoneHVACPackagedTerminalHeatPump_Impl::availabilitySchedule() const
  {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<ZoneHVACPackagedTerminalHeatPump_Impl*>(this)->setAvailabilitySchedule(*value);
      value = optionalAvailabilitySchedule();
    }
    OS_ASSERT(value);
    return value.get();
  }

  std::string ZoneHVACPackagedTerminalHeatPump_Impl::outdoorAirMixerObjectType() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirMixerObjectType,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ZoneHVACPackagedTerminalHeatPump_Impl::outdoorAirMixerName() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirMixerName,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> ZoneHVACPackagedTerminalHeatPump_Impl::supplyAirFlowRateDuringCoolingOperation() const {
    return getDouble(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFlowRateDuringCoolingOperation,true);
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::isSupplyAirFlowRateDuringCoolingOperationAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFlowRateDuringCoolingOperation, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> ZoneHVACPackagedTerminalHeatPump_Impl::supplyAirFlowRateDuringHeatingOperation() const {
    return getDouble(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFlowRateDuringHeatingOperation,true);
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::isSupplyAirFlowRateDuringHeatingOperationAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFlowRateDuringHeatingOperation, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> ZoneHVACPackagedTerminalHeatPump_Impl::supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
    return getDouble(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded,true);
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> ZoneHVACPackagedTerminalHeatPump_Impl::outdoorAirFlowRateDuringCoolingOperation() const {
    return getDouble(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirFlowRateDuringCoolingOperation,true);
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::isOutdoorAirFlowRateDuringCoolingOperationAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirFlowRateDuringCoolingOperation, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> ZoneHVACPackagedTerminalHeatPump_Impl::outdoorAirFlowRateDuringHeatingOperation() const {
    return getDouble(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirFlowRateDuringHeatingOperation,true);
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::isOutdoorAirFlowRateDuringHeatingOperationAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirFlowRateDuringHeatingOperation, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> ZoneHVACPackagedTerminalHeatPump_Impl::outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
    return getDouble(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded,true);
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  HVACComponent ZoneHVACPackagedTerminalHeatPump_Impl::supplyAirFan() const
  {
    boost::optional<HVACComponent> result = optionalSupplyAirFan();
    OS_ASSERT(result);
    return result.get();
  }

  HVACComponent ZoneHVACPackagedTerminalHeatPump_Impl::heatingCoil() const
  {
    boost::optional<HVACComponent> result = optionalHeatingCoil();
    OS_ASSERT(result);
    return result.get();
  }

  double ZoneHVACPackagedTerminalHeatPump_Impl::heatingConvergenceTolerance() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingConvergenceTolerance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::isHeatingConvergenceToleranceDefaulted() const {
    return isEmpty(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingConvergenceTolerance);
  }

  double ZoneHVACPackagedTerminalHeatPump_Impl::minimumOutdoorDryBulbTemperatureforCompressorOperation() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::isMinimumOutdoorDryBulbTemperatureforCompressorOperationDefaulted() const {
    return isEmpty(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation);
  }

  HVACComponent ZoneHVACPackagedTerminalHeatPump_Impl::coolingCoil() const
  {
    boost::optional<HVACComponent> result = optionalCoolingCoil();
    OS_ASSERT(result);
    return result.get();
  }

  double ZoneHVACPackagedTerminalHeatPump_Impl::coolingConvergenceTolerance() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingConvergenceTolerance,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::isCoolingConvergenceToleranceDefaulted() const {
    return isEmpty(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingConvergenceTolerance);
  }

  HVACComponent ZoneHVACPackagedTerminalHeatPump_Impl::supplementalHeatingCoil() const
  {
    boost::optional<HVACComponent> result = optionalSupplementalHeatingCoil();
    OS_ASSERT(result);
    return result.get();
  }

  boost::optional<double> ZoneHVACPackagedTerminalHeatPump_Impl::maximumSupplyAirTemperaturefromSupplementalHeater() const {
    return getDouble(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::MaximumSupplyAirTemperaturefromSupplementalHeater,true);
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::MaximumSupplyAirTemperaturefromSupplementalHeater, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  double ZoneHVACPackagedTerminalHeatPump_Impl::maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const {
    return isEmpty(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
  }

  std::string ZoneHVACPackagedTerminalHeatPump_Impl::fanPlacement() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::FanPlacement,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::isFanPlacementDefaulted() const {
    return isEmpty(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::FanPlacement);
  }

  boost::optional<Schedule> ZoneHVACPackagedTerminalHeatPump_Impl::supplyAirFanOperatingModeSchedule() const
  {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFanOperatingModeScheduleName);
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::AvailabilityScheduleName,
                              "ZoneHVACPackagedTerminalHeatPump",
                              "Availability",
                              schedule);
    return result;
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::setOutdoorAirMixerObjectType(std::string outdoorAirMixerObjectType) {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirMixerObjectType, outdoorAirMixerObjectType);
    return result;
  }

  void ZoneHVACPackagedTerminalHeatPump_Impl::setOutdoorAirMixerName(std::string outdoorAirMixerName) {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirMixerName, outdoorAirMixerName);
    OS_ASSERT(result);
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::setSupplyAirFlowRateDuringCoolingOperation(boost::optional<double> supplyAirFlowRateDuringCoolingOperation) {
    bool result = false;
    if (supplyAirFlowRateDuringCoolingOperation) {
      result = setDouble(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFlowRateDuringCoolingOperation, supplyAirFlowRateDuringCoolingOperation.get());
    } else {
      result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFlowRateDuringCoolingOperation, "");
    }
    return result;
  }

  void ZoneHVACPackagedTerminalHeatPump_Impl::autosizeSupplyAirFlowRateDuringCoolingOperation() {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFlowRateDuringCoolingOperation, "Autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::setSupplyAirFlowRateDuringHeatingOperation(boost::optional<double> supplyAirFlowRateDuringHeatingOperation) {
    bool result = false;
    if (supplyAirFlowRateDuringHeatingOperation) {
      result = setDouble(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFlowRateDuringHeatingOperation, supplyAirFlowRateDuringHeatingOperation.get());
    } else {
      result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFlowRateDuringHeatingOperation, "");
    }
    return result;
  }

  void ZoneHVACPackagedTerminalHeatPump_Impl::autosizeSupplyAirFlowRateDuringHeatingOperation() {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFlowRateDuringHeatingOperation, "Autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded) {
    bool result = false;
    if (supplyAirFlowRateWhenNoCoolingorHeatingisNeeded) {
      result = setDouble(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, supplyAirFlowRateWhenNoCoolingorHeatingisNeeded.get());
    } else {
      result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, "");
    }
    return result;
  }

  void ZoneHVACPackagedTerminalHeatPump_Impl::resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, "");
    OS_ASSERT(result);
  }

  void ZoneHVACPackagedTerminalHeatPump_Impl::autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, "Autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::setOutdoorAirFlowRateDuringCoolingOperation(boost::optional<double> outdoorAirFlowRateDuringCoolingOperation) {
    bool result = false;
    if (outdoorAirFlowRateDuringCoolingOperation) {
      result = setDouble(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirFlowRateDuringCoolingOperation, outdoorAirFlowRateDuringCoolingOperation.get());
    } else {
      result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirFlowRateDuringCoolingOperation, "");
    }
    return result;
  }

  void ZoneHVACPackagedTerminalHeatPump_Impl::autosizeOutdoorAirFlowRateDuringCoolingOperation() {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirFlowRateDuringCoolingOperation, "Autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::setOutdoorAirFlowRateDuringHeatingOperation(boost::optional<double> outdoorAirFlowRateDuringHeatingOperation) {
    bool result = false;
    if (outdoorAirFlowRateDuringHeatingOperation) {
      result = setDouble(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirFlowRateDuringHeatingOperation, outdoorAirFlowRateDuringHeatingOperation.get());
    } else {
      result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirFlowRateDuringHeatingOperation, "");
    }
    return result;
  }

  void ZoneHVACPackagedTerminalHeatPump_Impl::autosizeOutdoorAirFlowRateDuringHeatingOperation() {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirFlowRateDuringHeatingOperation, "Autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
    bool result = false;
    if (outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
      result = setDouble(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded.get());
    } else {
      result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, "");
    }
    return result;
  }

  void ZoneHVACPackagedTerminalHeatPump_Impl::resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, "");
    OS_ASSERT(result);
  }

  void ZoneHVACPackagedTerminalHeatPump_Impl::autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, "Autosize");
    OS_ASSERT(result);
  }

  void ZoneHVACPackagedTerminalHeatPump_Impl::setSupplyAirFan( HVACComponent & hvacComponent )
  {
    bool isAllowedType = false;

    if( hvacComponent.iddObjectType() == IddObjectType::OS_Fan_ConstantVolume )
    {
      isAllowedType = true;
    }
    else if( hvacComponent.iddObjectType() == IddObjectType::OS_Fan_OnOff )
    {
      isAllowedType = true;
    }

    if( isAllowedType )
    {
      setPointer(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFanName,hvacComponent.handle());
    }
  }

  void ZoneHVACPackagedTerminalHeatPump_Impl::setHeatingCoil( HVACComponent & hvacComponent )
  {
    bool isAllowedType = false;

    if( hvacComponent.iddObjectType() == IddObjectType::OS_Coil_Heating_DX_SingleSpeed )
    {
      isAllowedType = true;
    }

    if( isAllowedType )
    {
      setPointer(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingCoilName,hvacComponent.handle());
    }
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::setHeatingConvergenceTolerance(double heatingConvergenceTolerance) {
    bool result = setDouble(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingConvergenceTolerance, heatingConvergenceTolerance);
    return result;
  }

  void ZoneHVACPackagedTerminalHeatPump_Impl::resetHeatingConvergenceTolerance() {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingConvergenceTolerance, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation) {
    bool result = setDouble(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation, minimumOutdoorDryBulbTemperatureforCompressorOperation);
    return result;
  }

  void ZoneHVACPackagedTerminalHeatPump_Impl::resetMinimumOutdoorDryBulbTemperatureforCompressorOperation() {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation, "");
    OS_ASSERT(result);
  }

  void ZoneHVACPackagedTerminalHeatPump_Impl::setCoolingCoil( HVACComponent & hvacComponent )
  {
    bool isAllowedType = false;

    if( hvacComponent.iddObjectType() == IddObjectType::OS_Coil_Cooling_DX_SingleSpeed )
    {
      isAllowedType = true;
    }

    if( isAllowedType )
    {
      setPointer(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingCoilName,hvacComponent.handle());
    }
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::setCoolingConvergenceTolerance(double coolingConvergenceTolerance) {
    bool result = setDouble(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingConvergenceTolerance, coolingConvergenceTolerance);
    return result;
  }

  void ZoneHVACPackagedTerminalHeatPump_Impl::resetCoolingConvergenceTolerance() {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingConvergenceTolerance, "");
    OS_ASSERT(result);
  }

  void ZoneHVACPackagedTerminalHeatPump_Impl::setSupplementalHeatingCoil( HVACComponent & hvacComponent )
  {
    setPointer(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplementalHeatingCoilName,hvacComponent.handle());
  }

  void ZoneHVACPackagedTerminalHeatPump_Impl::setMaximumSupplyAirTemperaturefromSupplementalHeater(boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater) {
    bool result = false;
    if (maximumSupplyAirTemperaturefromSupplementalHeater) {
      result = setDouble(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::MaximumSupplyAirTemperaturefromSupplementalHeater, maximumSupplyAirTemperaturefromSupplementalHeater.get());
    } else {
      result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::MaximumSupplyAirTemperaturefromSupplementalHeater, "");
    }
    OS_ASSERT(result);
  }

  void ZoneHVACPackagedTerminalHeatPump_Impl::autosizeMaximumSupplyAirTemperaturefromSupplementalHeater() {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::MaximumSupplyAirTemperaturefromSupplementalHeater, "Autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation) {
    bool result = setDouble(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation, maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
    return result;
  }

  void ZoneHVACPackagedTerminalHeatPump_Impl::resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::setFanPlacement(std::string fanPlacement) {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::FanPlacement, fanPlacement);
    return result;
  }

  void ZoneHVACPackagedTerminalHeatPump_Impl::resetFanPlacement() {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::FanPlacement, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::setSupplyAirFanOperatingModeSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFanOperatingModeScheduleName,
                              "ZoneHVACPackagedTerminalHeatPump",
                              "Supply Air Fan Operating Mode",
                              schedule);
    return result;
  }

  void ZoneHVACPackagedTerminalHeatPump_Impl::resetSupplyAirFanOperatingModeSchedule() {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFanOperatingModeScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> ZoneHVACPackagedTerminalHeatPump_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::AvailabilityScheduleName);
  }
  boost::optional<HVACComponent> ZoneHVACPackagedTerminalHeatPump_Impl::optionalSupplyAirFan() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFanName);
  }
  boost::optional<HVACComponent> ZoneHVACPackagedTerminalHeatPump_Impl::optionalHeatingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingCoilName);
  }
  boost::optional<HVACComponent> ZoneHVACPackagedTerminalHeatPump_Impl::optionalCoolingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingCoilName);
  }
  boost::optional<HVACComponent> ZoneHVACPackagedTerminalHeatPump_Impl::optionalSupplementalHeatingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_PackagedTerminalHeatPumpFields::SupplementalHeatingCoilName);
  }

  boost::optional<ModelObject> ZoneHVACPackagedTerminalHeatPump_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  boost::optional<ModelObject> ZoneHVACPackagedTerminalHeatPump_Impl::supplyAirFanAsModelObject() const {
    OptionalModelObject result = supplyAirFan();
    return result;
  }

  boost::optional<ModelObject> ZoneHVACPackagedTerminalHeatPump_Impl::heatingCoilAsModelObject() const {
    OptionalModelObject result = heatingCoil();
    return result;
  }

  boost::optional<ModelObject> ZoneHVACPackagedTerminalHeatPump_Impl::coolingCoilAsModelObject() const {
    OptionalModelObject result = coolingCoil();
    return result;
  }

  boost::optional<ModelObject> ZoneHVACPackagedTerminalHeatPump_Impl::supplementalHeatingCoilAsModelObject() const {
    OptionalModelObject result = supplementalHeatingCoil();
    return result;
  }

  boost::optional<ModelObject> ZoneHVACPackagedTerminalHeatPump_Impl::supplyAirFanOperatingModeScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = supplyAirFanOperatingModeSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::setSupplyAirFanAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
      if (intermediate) {
        setSupplyAirFan(*intermediate);
        return true;
      }
    }
    return false;
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::setHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
      if (intermediate) {
        setHeatingCoil(*intermediate);
        return true;
      }
    }
    return false;
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::setCoolingCoilAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
      if (intermediate) {
        setCoolingCoil(*intermediate);
        return true;
      }
    }
    return false;
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::setSupplementalHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
      if (intermediate) {
        setSupplementalHeatingCoil(*intermediate);
        return true;
      }
    }
    return false;
  }

  bool ZoneHVACPackagedTerminalHeatPump_Impl::setSupplyAirFanOperatingModeScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setSupplyAirFanOperatingModeSchedule(schedule);
      }
      else {
        return false;
      }
    }
    else {
      resetSupplyAirFanOperatingModeSchedule();
    }
    return true;
  }

} // detail

ZoneHVACPackagedTerminalHeatPump::ZoneHVACPackagedTerminalHeatPump( const Model& model,
                                                                    Schedule & availabilitySchedule,
                                                                    HVACComponent & supplyAirFan,
                                                                    HVACComponent & heatingCoil,
                                                                    HVACComponent & coolingCoil,
                                                                    HVACComponent & supplementalHeatingCoil )
  : ZoneHVACComponent(ZoneHVACPackagedTerminalHeatPump::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>());

  bool ok = setAvailabilitySchedule(availabilitySchedule);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s availability schedule to "
                  << availabilitySchedule.briefDescription() << ".");
  }
  setOutdoorAirMixerObjectType("OutdoorAir:Mixer");
  setSupplyAirFan(supplyAirFan);
  setHeatingCoil(heatingCoil);
  setCoolingCoil(coolingCoil);
  setSupplementalHeatingCoil(supplementalHeatingCoil);

  autosizeSupplyAirFlowRateDuringCoolingOperation();
  autosizeSupplyAirFlowRateDuringHeatingOperation();
  autosizeOutdoorAirFlowRateDuringCoolingOperation();
  autosizeOutdoorAirFlowRateDuringHeatingOperation();
  autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();
  autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
  autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

IddObjectType ZoneHVACPackagedTerminalHeatPump::iddObjectType() {
  IddObjectType result(IddObjectType::OS_ZoneHVAC_PackagedTerminalHeatPump);
  return result;
}

std::vector<std::string> ZoneHVACPackagedTerminalHeatPump::outdoorAirMixerObjectTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirMixerObjectType);
}

std::vector<std::string> ZoneHVACPackagedTerminalHeatPump::validOutdoorAirMixerObjectTypeValues() {
  return ZoneHVACPackagedTerminalHeatPump::outdoorAirMixerObjectTypeValues();
}

std::vector<std::string> ZoneHVACPackagedTerminalHeatPump::fanPlacementValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_PackagedTerminalHeatPumpFields::FanPlacement);
}

std::vector<std::string> ZoneHVACPackagedTerminalHeatPump::validFanPlacementValues() {
  return ZoneHVACPackagedTerminalHeatPump::fanPlacementValues();
}

std::string ZoneHVACPackagedTerminalHeatPump::outdoorAirMixerObjectType() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->outdoorAirMixerObjectType();
}

std::string ZoneHVACPackagedTerminalHeatPump::outdoorAirMixerName() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->outdoorAirMixerName();
}

boost::optional<double> ZoneHVACPackagedTerminalHeatPump::supplyAirFlowRateDuringCoolingOperation() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->supplyAirFlowRateDuringCoolingOperation();
}

bool ZoneHVACPackagedTerminalHeatPump::isSupplyAirFlowRateDuringCoolingOperationAutosized() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->isSupplyAirFlowRateDuringCoolingOperationAutosized();
}

boost::optional<double> ZoneHVACPackagedTerminalHeatPump::supplyAirFlowRateDuringHeatingOperation() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->supplyAirFlowRateDuringHeatingOperation();
}

bool ZoneHVACPackagedTerminalHeatPump::isSupplyAirFlowRateDuringHeatingOperationAutosized() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->isSupplyAirFlowRateDuringHeatingOperationAutosized();
}

boost::optional<double> ZoneHVACPackagedTerminalHeatPump::supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->supplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

bool ZoneHVACPackagedTerminalHeatPump::isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized();
}

boost::optional<double> ZoneHVACPackagedTerminalHeatPump::outdoorAirFlowRateDuringCoolingOperation() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->outdoorAirFlowRateDuringCoolingOperation();
}

bool ZoneHVACPackagedTerminalHeatPump::isOutdoorAirFlowRateDuringCoolingOperationAutosized() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->isOutdoorAirFlowRateDuringCoolingOperationAutosized();
}

boost::optional<double> ZoneHVACPackagedTerminalHeatPump::outdoorAirFlowRateDuringHeatingOperation() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->outdoorAirFlowRateDuringHeatingOperation();
}

bool ZoneHVACPackagedTerminalHeatPump::isOutdoorAirFlowRateDuringHeatingOperationAutosized() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->isOutdoorAirFlowRateDuringHeatingOperationAutosized();
}

boost::optional<double> ZoneHVACPackagedTerminalHeatPump::outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

bool ZoneHVACPackagedTerminalHeatPump::isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized();
}

double ZoneHVACPackagedTerminalHeatPump::heatingConvergenceTolerance() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->heatingConvergenceTolerance();
}

bool ZoneHVACPackagedTerminalHeatPump::isHeatingConvergenceToleranceDefaulted() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->isHeatingConvergenceToleranceDefaulted();
}

double ZoneHVACPackagedTerminalHeatPump::minimumOutdoorDryBulbTemperatureforCompressorOperation() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->minimumOutdoorDryBulbTemperatureforCompressorOperation();
}

bool ZoneHVACPackagedTerminalHeatPump::isMinimumOutdoorDryBulbTemperatureforCompressorOperationDefaulted() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->isMinimumOutdoorDryBulbTemperatureforCompressorOperationDefaulted();
}

double ZoneHVACPackagedTerminalHeatPump::coolingConvergenceTolerance() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->coolingConvergenceTolerance();
}

bool ZoneHVACPackagedTerminalHeatPump::isCoolingConvergenceToleranceDefaulted() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->isCoolingConvergenceToleranceDefaulted();
}

boost::optional<double> ZoneHVACPackagedTerminalHeatPump::maximumSupplyAirTemperaturefromSupplementalHeater() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->maximumSupplyAirTemperaturefromSupplementalHeater();
}

bool ZoneHVACPackagedTerminalHeatPump::isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized();
}

double ZoneHVACPackagedTerminalHeatPump::maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();
}

bool ZoneHVACPackagedTerminalHeatPump::isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted();
}

std::string ZoneHVACPackagedTerminalHeatPump::fanPlacement() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->fanPlacement();
}

bool ZoneHVACPackagedTerminalHeatPump::isFanPlacementDefaulted() const {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->isFanPlacementDefaulted();
}

bool ZoneHVACPackagedTerminalHeatPump::setOutdoorAirMixerObjectType(std::string outdoorAirMixerObjectType) {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->setOutdoorAirMixerObjectType(outdoorAirMixerObjectType);
}

void ZoneHVACPackagedTerminalHeatPump::setOutdoorAirMixerName(std::string outdoorAirMixerName) {
  getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->setOutdoorAirMixerName(outdoorAirMixerName);
}

bool ZoneHVACPackagedTerminalHeatPump::setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation) {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->setSupplyAirFlowRateDuringCoolingOperation(supplyAirFlowRateDuringCoolingOperation);
}

void ZoneHVACPackagedTerminalHeatPump::autosizeSupplyAirFlowRateDuringCoolingOperation() {
  getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->autosizeSupplyAirFlowRateDuringCoolingOperation();
}

bool ZoneHVACPackagedTerminalHeatPump::setSupplyAirFlowRateDuringHeatingOperation(double supplyAirFlowRateDuringHeatingOperation) {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->setSupplyAirFlowRateDuringHeatingOperation(supplyAirFlowRateDuringHeatingOperation);
}

void ZoneHVACPackagedTerminalHeatPump::autosizeSupplyAirFlowRateDuringHeatingOperation() {
  getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->autosizeSupplyAirFlowRateDuringHeatingOperation();
}

bool ZoneHVACPackagedTerminalHeatPump::setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(double supplyAirFlowRateWhenNoCoolingorHeatingisNeeded) {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);
}

void ZoneHVACPackagedTerminalHeatPump::resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
  getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

void ZoneHVACPackagedTerminalHeatPump::autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
  getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

bool ZoneHVACPackagedTerminalHeatPump::setOutdoorAirFlowRateDuringCoolingOperation(double outdoorAirFlowRateDuringCoolingOperation) {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->setOutdoorAirFlowRateDuringCoolingOperation(outdoorAirFlowRateDuringCoolingOperation);
}

void ZoneHVACPackagedTerminalHeatPump::autosizeOutdoorAirFlowRateDuringCoolingOperation() {
  getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->autosizeOutdoorAirFlowRateDuringCoolingOperation();
}

bool ZoneHVACPackagedTerminalHeatPump::setOutdoorAirFlowRateDuringHeatingOperation(double outdoorAirFlowRateDuringHeatingOperation) {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->setOutdoorAirFlowRateDuringHeatingOperation(outdoorAirFlowRateDuringHeatingOperation);
}

void ZoneHVACPackagedTerminalHeatPump::autosizeOutdoorAirFlowRateDuringHeatingOperation() {
  getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->autosizeOutdoorAirFlowRateDuringHeatingOperation();
}

bool ZoneHVACPackagedTerminalHeatPump::setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(double outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);
}

void ZoneHVACPackagedTerminalHeatPump::resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
  getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

void ZoneHVACPackagedTerminalHeatPump::autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
  getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

bool ZoneHVACPackagedTerminalHeatPump::setHeatingConvergenceTolerance(double heatingConvergenceTolerance) {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->setHeatingConvergenceTolerance(heatingConvergenceTolerance);
}

void ZoneHVACPackagedTerminalHeatPump::resetHeatingConvergenceTolerance() {
  getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->resetHeatingConvergenceTolerance();
}

bool ZoneHVACPackagedTerminalHeatPump::setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation) {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->setMinimumOutdoorDryBulbTemperatureforCompressorOperation(minimumOutdoorDryBulbTemperatureforCompressorOperation);
}

void ZoneHVACPackagedTerminalHeatPump::resetMinimumOutdoorDryBulbTemperatureforCompressorOperation() {
  getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->resetMinimumOutdoorDryBulbTemperatureforCompressorOperation();
}

bool ZoneHVACPackagedTerminalHeatPump::setCoolingConvergenceTolerance(double coolingConvergenceTolerance) {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->setCoolingConvergenceTolerance(coolingConvergenceTolerance);
}

void ZoneHVACPackagedTerminalHeatPump::resetCoolingConvergenceTolerance() {
  getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->resetCoolingConvergenceTolerance();
}

void ZoneHVACPackagedTerminalHeatPump::setMaximumSupplyAirTemperaturefromSupplementalHeater(double maximumSupplyAirTemperaturefromSupplementalHeater) {
  getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->setMaximumSupplyAirTemperaturefromSupplementalHeater(maximumSupplyAirTemperaturefromSupplementalHeater);
}

void ZoneHVACPackagedTerminalHeatPump::autosizeMaximumSupplyAirTemperaturefromSupplementalHeater() {
  getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();
}

bool ZoneHVACPackagedTerminalHeatPump::setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation) {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
}

void ZoneHVACPackagedTerminalHeatPump::resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() {
  getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();
}

bool ZoneHVACPackagedTerminalHeatPump::setFanPlacement(std::string fanPlacement) {
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->setFanPlacement(fanPlacement);
}

void ZoneHVACPackagedTerminalHeatPump::resetFanPlacement() {
  getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->resetFanPlacement();
}

bool ZoneHVACPackagedTerminalHeatPump::setAvailabilitySchedule( Schedule & schedule )
{
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->setAvailabilitySchedule( schedule );
}

bool ZoneHVACPackagedTerminalHeatPump::setSupplyAirFanOperatingModeSchedule( Schedule & schedule )
{
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->setSupplyAirFanOperatingModeSchedule( schedule );
}

void ZoneHVACPackagedTerminalHeatPump::resetSupplyAirFanOperatingModeSchedule() {
  getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->resetSupplyAirFanOperatingModeSchedule();
}

void ZoneHVACPackagedTerminalHeatPump::setSupplyAirFan( HVACComponent & hvacComponent )
{
  getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->setSupplyAirFan( hvacComponent );
}

void ZoneHVACPackagedTerminalHeatPump::setHeatingCoil( HVACComponent & hvacComponent )
{
  getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->setHeatingCoil( hvacComponent );
}

void ZoneHVACPackagedTerminalHeatPump::setSupplementalHeatingCoil( HVACComponent & hvacComponent )
{
  getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->setSupplementalHeatingCoil( hvacComponent );
}

void ZoneHVACPackagedTerminalHeatPump::setCoolingCoil( HVACComponent & hvacComponent )
{
  getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->setCoolingCoil( hvacComponent );
}

Schedule ZoneHVACPackagedTerminalHeatPump::availabilitySchedule() const
{
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->availabilitySchedule();
}

boost::optional<Schedule> ZoneHVACPackagedTerminalHeatPump::supplyAirFanOperatingModeSchedule() const
{
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->supplyAirFanOperatingModeSchedule();
}

HVACComponent ZoneHVACPackagedTerminalHeatPump::supplyAirFan() const
{
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->supplyAirFan();
}

HVACComponent ZoneHVACPackagedTerminalHeatPump::heatingCoil() const
{
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->heatingCoil();
}

HVACComponent ZoneHVACPackagedTerminalHeatPump::supplementalHeatingCoil() const
{
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->supplementalHeatingCoil();
}

HVACComponent ZoneHVACPackagedTerminalHeatPump::coolingCoil() const
{
  return getImpl<detail::ZoneHVACPackagedTerminalHeatPump_Impl>()->coolingCoil();
}

/// @cond
ZoneHVACPackagedTerminalHeatPump::ZoneHVACPackagedTerminalHeatPump(std::shared_ptr<detail::ZoneHVACPackagedTerminalHeatPump_Impl> impl)
  : ZoneHVACComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

