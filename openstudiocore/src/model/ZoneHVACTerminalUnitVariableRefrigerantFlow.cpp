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

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "FanOnOff.hpp"
#include "FanOnOff_Impl.hpp"
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include "ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "CoilHeatingDXVariableRefrigerantFlow.hpp"
#include "CoilHeatingDXVariableRefrigerantFlow_Impl.hpp"
#include "CoilCoolingDXVariableRefrigerantFlow.hpp"
#include "CoilCoolingDXVariableRefrigerantFlow_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include <utilities/idd/IddFactory.hxx>


#include <utilities/idd/OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail {

  ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl(const IdfObject& idfObject,
                                                                                                     Model_Impl* model,
                                                                                                     bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneHVACTerminalUnitVariableRefrigerantFlow::iddObjectType());
  }

  ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                     Model_Impl* model,
                                                                                                     bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneHVACTerminalUnitVariableRefrigerantFlow::iddObjectType());
  }

  ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl(const ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl& other,
                                                                                                     Model_Impl* model,
                                                                                                     bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::iddObjectType() const {
    return ZoneHVACTerminalUnitVariableRefrigerantFlow::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    // TODO: Check schedule display names.
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAvailabilityschedule) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACTerminalUnitVariableRefrigerantFlow","Terminal Unit Availability schedule"));
    }
    if (std::find(b,e,OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanOperatingModeSchedule) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACTerminalUnitVariableRefrigerantFlow","Supply Air Fan Operating Mode Schedule"));
    }
    return result;
  }

  Schedule ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::terminalUnitAvailabilityschedule() const {
    boost::optional<Schedule> value = optionalTerminalUnitAvailabilityschedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Terminal Unit Availabilityschedule attached.");
    }
    return value.get();
  }

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::supplyAirFlowRateDuringCoolingOperation() const {
    return getDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateDuringCoolingOperation,true);
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::isSupplyAirFlowRateDuringCoolingOperationAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateDuringCoolingOperation, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::supplyAirFlowRateWhenNoCoolingisNeeded() const {
    return getDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateWhenNoCoolingisNeeded,true);
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::isSupplyAirFlowRateWhenNoCoolingisNeededAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateWhenNoCoolingisNeeded, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::supplyAirFlowRateDuringHeatingOperation() const {
    return getDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateDuringHeatingOperation,true);
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::isSupplyAirFlowRateDuringHeatingOperationAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateDuringHeatingOperation, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::supplyAirFlowRateWhenNoHeatingisNeeded() const {
    return getDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateWhenNoHeatingisNeeded,true);
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::isSupplyAirFlowRateWhenNoHeatingisNeededAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateWhenNoHeatingisNeeded, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::outdoorAirFlowRateDuringCoolingOperation() const {
    return getDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateDuringCoolingOperation,true);
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::isOutdoorAirFlowRateDuringCoolingOperationAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateDuringCoolingOperation, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::outdoorAirFlowRateDuringHeatingOperation() const {
    return getDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateDuringHeatingOperation,true);
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::isOutdoorAirFlowRateDuringHeatingOperationAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateDuringHeatingOperation, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
    return getDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded,true);
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autosize");
    }
    return result;
  }

  Schedule ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::supplyAirFanOperatingModeSchedule() const {
    boost::optional<Schedule> value = optionalSupplyAirFanOperatingModeSchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Supply Air Fan Operating Mode Schedule attached.");
    }
    return value.get();
  }

  HVACComponent ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::supplyAirFan() const {
    boost::optional<HVACComponent> value = optionalSupplyAirFan();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Supply Air Fan attached.");
    }
    return value.get();
  }

  CoilCoolingDXVariableRefrigerantFlow ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::coolingCoil() const {
    boost::optional<CoilCoolingDXVariableRefrigerantFlow> result = getObject<ModelObject>().getModelObjectTarget<CoilCoolingDXVariableRefrigerantFlow>(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::CoolingCoil);

    Q_ASSERT(result);

    return result.get();
  }

  CoilHeatingDXVariableRefrigerantFlow ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::heatingCoil() const {
    boost::optional<CoilHeatingDXVariableRefrigerantFlow> result = getObject<ModelObject>().getModelObjectTarget<CoilHeatingDXVariableRefrigerantFlow>(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::HeatingCoil);

    Q_ASSERT(result);

    return result.get();
  }

  double ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::zoneTerminalUnitOnParasiticElectricEnergyUse() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::ZoneTerminalUnitOnParasiticElectricEnergyUse,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::zoneTerminalUnitOffParasiticElectricEnergyUse() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::ZoneTerminalUnitOffParasiticElectricEnergyUse,true);
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::ratedTotalHeatingCapacitySizingRatio() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::RatedTotalHeatingCapacitySizingRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setTerminalUnitAvailabilityschedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAvailabilityschedule,
                              "ZoneHVACTerminalUnitVariableRefrigerantFlow",
                              "Terminal Unit Availability schedule",
                              schedule);
    return result;
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setSupplyAirFlowRateDuringCoolingOperation(boost::optional<double> supplyAirFlowRateDuringCoolingOperation) {
    bool result(false);
    if (supplyAirFlowRateDuringCoolingOperation) {
      result = setDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateDuringCoolingOperation, supplyAirFlowRateDuringCoolingOperation.get());
    }
    return result;
  }

  void ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizeSupplyAirFlowRateDuringCoolingOperation() {
    bool result = setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateDuringCoolingOperation, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setSupplyAirFlowRateWhenNoCoolingisNeeded(boost::optional<double> supplyAirFlowRateWhenNoCoolingisNeeded) {
    bool result(false);
    if (supplyAirFlowRateWhenNoCoolingisNeeded) {
      result = setDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateWhenNoCoolingisNeeded, supplyAirFlowRateWhenNoCoolingisNeeded.get());
    }
    return result;
  }

  void ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizeSupplyAirFlowRateWhenNoCoolingisNeeded() {
    bool result = setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateWhenNoCoolingisNeeded, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setSupplyAirFlowRateDuringHeatingOperation(boost::optional<double> supplyAirFlowRateDuringHeatingOperation) {
    bool result(false);
    if (supplyAirFlowRateDuringHeatingOperation) {
      result = setDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateDuringHeatingOperation, supplyAirFlowRateDuringHeatingOperation.get());
    }
    return result;
  }

  void ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizeSupplyAirFlowRateDuringHeatingOperation() {
    bool result = setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateDuringHeatingOperation, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setSupplyAirFlowRateWhenNoHeatingisNeeded(boost::optional<double> supplyAirFlowRateWhenNoHeatingisNeeded) {
    bool result(false);
    if (supplyAirFlowRateWhenNoHeatingisNeeded) {
      result = setDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateWhenNoHeatingisNeeded, supplyAirFlowRateWhenNoHeatingisNeeded.get());
    }
    return result;
  }

  void ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizeSupplyAirFlowRateWhenNoHeatingisNeeded() {
    bool result = setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFlowRateWhenNoHeatingisNeeded, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setOutdoorAirFlowRateDuringCoolingOperation(boost::optional<double> outdoorAirFlowRateDuringCoolingOperation) {
    bool result(false);
    if (outdoorAirFlowRateDuringCoolingOperation) {
      result = setDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateDuringCoolingOperation, outdoorAirFlowRateDuringCoolingOperation.get());
    }
    return result;
  }

  void ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizeOutdoorAirFlowRateDuringCoolingOperation() {
    bool result = setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateDuringCoolingOperation, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setOutdoorAirFlowRateDuringHeatingOperation(boost::optional<double> outdoorAirFlowRateDuringHeatingOperation) {
    bool result(false);
    if (outdoorAirFlowRateDuringHeatingOperation) {
      result = setDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateDuringHeatingOperation, outdoorAirFlowRateDuringHeatingOperation.get());
    }
    return result;
  }

  void ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizeOutdoorAirFlowRateDuringHeatingOperation() {
    bool result = setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateDuringHeatingOperation, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
    bool result(false);
    if (outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
      result = setDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded.get());
    }
    return result;
  }

  void ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    bool result = setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setSupplyAirFanOperatingModeSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanOperatingModeSchedule,
                              "ZoneHVACTerminalUnitVariableRefrigerantFlow",
                              "Supply Air Fan Operating Mode Schedule",
                              schedule);
    return result;
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setZoneTerminalUnitOnParasiticElectricEnergyUse(double zoneTerminalUnitOnParasiticElectricEnergyUse) {
    bool result = setDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::ZoneTerminalUnitOnParasiticElectricEnergyUse, zoneTerminalUnitOnParasiticElectricEnergyUse);
    return result;
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setZoneTerminalUnitOffParasiticElectricEnergyUse(double zoneTerminalUnitOffParasiticElectricEnergyUse) {
    bool result = setDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::ZoneTerminalUnitOffParasiticElectricEnergyUse, zoneTerminalUnitOffParasiticElectricEnergyUse);
    return result;
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setRatedTotalHeatingCapacitySizingRatio(double ratedTotalHeatingCapacitySizingRatio) {
    bool result = setDouble(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::RatedTotalHeatingCapacitySizingRatio, ratedTotalHeatingCapacitySizingRatio);
    return result;
  }

  boost::optional<Schedule> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::optionalTerminalUnitAvailabilityschedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAvailabilityschedule);
  }

  boost::optional<Schedule> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::optionalSupplyAirFanOperatingModeSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanOperatingModeSchedule);
  }

  boost::optional<HVACComponent> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::optionalSupplyAirFan() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFan);
  }

  unsigned ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::inletPort() const
  {
    return OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAirInletNode;
  }

  unsigned ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::outletPort() const
  {
    return OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAirOutletNode;
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setSupplyAirFan(const HVACComponent & component)
  {
    return setPointer(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFan,component.handle());
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setCoolingCoil(const CoilCoolingDXVariableRefrigerantFlow & component)
  {
    return setPointer(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::CoolingCoil,component.handle());
  }

  bool ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::setHeatingCoil(const CoilHeatingDXVariableRefrigerantFlow & component)
  {
    return setPointer(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::HeatingCoil,component.handle());
  }

  ModelObject ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::clone(Model model) const
  {
    ModelObject terminalClone = ZoneHVACComponent_Impl::clone(model);

    HVACComponent fanClone = supplyAirFan().clone(model).cast<HVACComponent>();

    CoilCoolingDXVariableRefrigerantFlow coolingCoilClone = coolingCoil().clone(model).cast<CoilCoolingDXVariableRefrigerantFlow>();

    CoilHeatingDXVariableRefrigerantFlow heatingCoilClone = heatingCoil().clone(model).cast<CoilHeatingDXVariableRefrigerantFlow>();

    terminalClone.getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setSupplyAirFan(fanClone);

    terminalClone.getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setCoolingCoil(coolingCoilClone);

    terminalClone.getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setHeatingCoil(heatingCoilClone);

    // TODO Move this into base clase
    terminalClone.setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAirInletNode,"");
    terminalClone.setString(OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAirOutletNode,"");

    return terminalClone;
  }

  std::vector<ModelObject> ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl::children() const
  {
    std::vector<ModelObject> result;

    result.push_back(supplyAirFan());
    result.push_back(coolingCoil());
    result.push_back(heatingCoil());

    return result;
  }

} // detail

ZoneHVACTerminalUnitVariableRefrigerantFlow::ZoneHVACTerminalUnitVariableRefrigerantFlow(const Model& model)
  : ZoneHVACComponent(ZoneHVACTerminalUnitVariableRefrigerantFlow::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>());

  Schedule alwaysOnSchedule = model.alwaysOnDiscreteSchedule();
  setTerminalUnitAvailabilityschedule(alwaysOnSchedule);

  autosizeSupplyAirFlowRateDuringCoolingOperation();

  autosizeSupplyAirFlowRateWhenNoCoolingisNeeded();

  autosizeSupplyAirFlowRateDuringHeatingOperation();

  autosizeSupplyAirFlowRateWhenNoHeatingisNeeded();

  autosizeOutdoorAirFlowRateDuringCoolingOperation();

  autosizeOutdoorAirFlowRateDuringHeatingOperation();

  autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

  setSupplyAirFanOperatingModeSchedule(alwaysOnSchedule);

  setZoneTerminalUnitOnParasiticElectricEnergyUse(30);

  setZoneTerminalUnitOffParasiticElectricEnergyUse(20);

  setRatedTotalHeatingCapacitySizingRatio(1.0);

  CoilCoolingDXVariableRefrigerantFlow coolingCoil(model);
  coolingCoil.setName(name().get() + " Cooling Coil");
  getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setCoolingCoil(coolingCoil);

  CoilHeatingDXVariableRefrigerantFlow heatingCoil(model);
  heatingCoil.setName(name().get() + " Heating Coil");
  getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setHeatingCoil(heatingCoil);

  FanOnOff fan(model,alwaysOnSchedule);
  fan.setName(name().get() + " Fan");
  getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setSupplyAirFan(fan);
}

ZoneHVACTerminalUnitVariableRefrigerantFlow::ZoneHVACTerminalUnitVariableRefrigerantFlow(const Model& model,
  const CoilCoolingDXVariableRefrigerantFlow & coolingCoil,
  const CoilHeatingDXVariableRefrigerantFlow & heatingCoil,
  const HVACComponent & fan)
  : ZoneHVACComponent(ZoneHVACTerminalUnitVariableRefrigerantFlow::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>());

  Schedule alwaysOnSchedule = model.alwaysOnDiscreteSchedule();
  setTerminalUnitAvailabilityschedule(alwaysOnSchedule);

  autosizeSupplyAirFlowRateDuringCoolingOperation();

  autosizeSupplyAirFlowRateWhenNoCoolingisNeeded();

  autosizeSupplyAirFlowRateDuringHeatingOperation();

  autosizeSupplyAirFlowRateWhenNoHeatingisNeeded();

  autosizeOutdoorAirFlowRateDuringCoolingOperation();

  autosizeOutdoorAirFlowRateDuringHeatingOperation();

  autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();

  setSupplyAirFanOperatingModeSchedule(alwaysOnSchedule);

  setZoneTerminalUnitOnParasiticElectricEnergyUse(30);

  setZoneTerminalUnitOffParasiticElectricEnergyUse(20);

  setRatedTotalHeatingCapacitySizingRatio(1.0);

  getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setCoolingCoil(coolingCoil);

  getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setHeatingCoil(heatingCoil);

  getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setSupplyAirFan(fan);
}

IddObjectType ZoneHVACTerminalUnitVariableRefrigerantFlow::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlow);
}

std::vector<std::string> ZoneHVACTerminalUnitVariableRefrigerantFlow::supplyAirFanplacementValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanplacement);
}

Schedule ZoneHVACTerminalUnitVariableRefrigerantFlow::terminalUnitAvailabilityschedule() const {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->terminalUnitAvailabilityschedule();
}

boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::supplyAirFlowRateDuringCoolingOperation() const {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->supplyAirFlowRateDuringCoolingOperation();
}

bool ZoneHVACTerminalUnitVariableRefrigerantFlow::isSupplyAirFlowRateDuringCoolingOperationAutosized() const {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->isSupplyAirFlowRateDuringCoolingOperationAutosized();
}

boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::supplyAirFlowRateWhenNoCoolingisNeeded() const {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->supplyAirFlowRateWhenNoCoolingisNeeded();
}

bool ZoneHVACTerminalUnitVariableRefrigerantFlow::isSupplyAirFlowRateWhenNoCoolingisNeededAutosized() const {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->isSupplyAirFlowRateWhenNoCoolingisNeededAutosized();
}

boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::supplyAirFlowRateDuringHeatingOperation() const {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->supplyAirFlowRateDuringHeatingOperation();
}

bool ZoneHVACTerminalUnitVariableRefrigerantFlow::isSupplyAirFlowRateDuringHeatingOperationAutosized() const {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->isSupplyAirFlowRateDuringHeatingOperationAutosized();
}

boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::supplyAirFlowRateWhenNoHeatingisNeeded() const {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->supplyAirFlowRateWhenNoHeatingisNeeded();
}

bool ZoneHVACTerminalUnitVariableRefrigerantFlow::isSupplyAirFlowRateWhenNoHeatingisNeededAutosized() const {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->isSupplyAirFlowRateWhenNoHeatingisNeededAutosized();
}

boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::outdoorAirFlowRateDuringCoolingOperation() const {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->outdoorAirFlowRateDuringCoolingOperation();
}

bool ZoneHVACTerminalUnitVariableRefrigerantFlow::isOutdoorAirFlowRateDuringCoolingOperationAutosized() const {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->isOutdoorAirFlowRateDuringCoolingOperationAutosized();
}

boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::outdoorAirFlowRateDuringHeatingOperation() const {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->outdoorAirFlowRateDuringHeatingOperation();
}

bool ZoneHVACTerminalUnitVariableRefrigerantFlow::isOutdoorAirFlowRateDuringHeatingOperationAutosized() const {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->isOutdoorAirFlowRateDuringHeatingOperationAutosized();
}

boost::optional<double> ZoneHVACTerminalUnitVariableRefrigerantFlow::outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

bool ZoneHVACTerminalUnitVariableRefrigerantFlow::isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized();
}

Schedule ZoneHVACTerminalUnitVariableRefrigerantFlow::supplyAirFanOperatingModeSchedule() const {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->supplyAirFanOperatingModeSchedule();
}

HVACComponent ZoneHVACTerminalUnitVariableRefrigerantFlow::supplyAirFan() const {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->supplyAirFan();
}

CoilCoolingDXVariableRefrigerantFlow ZoneHVACTerminalUnitVariableRefrigerantFlow::coolingCoil() const {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->coolingCoil();
}

CoilHeatingDXVariableRefrigerantFlow ZoneHVACTerminalUnitVariableRefrigerantFlow::heatingCoil() const {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->heatingCoil();
}

double ZoneHVACTerminalUnitVariableRefrigerantFlow::zoneTerminalUnitOnParasiticElectricEnergyUse() const {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->zoneTerminalUnitOnParasiticElectricEnergyUse();
}

double ZoneHVACTerminalUnitVariableRefrigerantFlow::zoneTerminalUnitOffParasiticElectricEnergyUse() const {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->zoneTerminalUnitOffParasiticElectricEnergyUse();
}

double ZoneHVACTerminalUnitVariableRefrigerantFlow::ratedTotalHeatingCapacitySizingRatio() const {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->ratedTotalHeatingCapacitySizingRatio();
}

bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setTerminalUnitAvailabilityschedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setTerminalUnitAvailabilityschedule(schedule);
}

bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation) {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setSupplyAirFlowRateDuringCoolingOperation(supplyAirFlowRateDuringCoolingOperation);
}

void ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizeSupplyAirFlowRateDuringCoolingOperation() {
  getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizeSupplyAirFlowRateDuringCoolingOperation();
}

bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setSupplyAirFlowRateWhenNoCoolingisNeeded(double supplyAirFlowRateWhenNoCoolingisNeeded) {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setSupplyAirFlowRateWhenNoCoolingisNeeded(supplyAirFlowRateWhenNoCoolingisNeeded);
}

void ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizeSupplyAirFlowRateWhenNoCoolingisNeeded() {
  getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizeSupplyAirFlowRateWhenNoCoolingisNeeded();
}

bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setSupplyAirFlowRateDuringHeatingOperation(double supplyAirFlowRateDuringHeatingOperation) {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setSupplyAirFlowRateDuringHeatingOperation(supplyAirFlowRateDuringHeatingOperation);
}

void ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizeSupplyAirFlowRateDuringHeatingOperation() {
  getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizeSupplyAirFlowRateDuringHeatingOperation();
}

bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setSupplyAirFlowRateWhenNoHeatingisNeeded(double supplyAirFlowRateWhenNoHeatingisNeeded) {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setSupplyAirFlowRateWhenNoHeatingisNeeded(supplyAirFlowRateWhenNoHeatingisNeeded);
}

void ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizeSupplyAirFlowRateWhenNoHeatingisNeeded() {
  getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizeSupplyAirFlowRateWhenNoHeatingisNeeded();
}

bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setOutdoorAirFlowRateDuringCoolingOperation(double outdoorAirFlowRateDuringCoolingOperation) {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setOutdoorAirFlowRateDuringCoolingOperation(outdoorAirFlowRateDuringCoolingOperation);
}

void ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizeOutdoorAirFlowRateDuringCoolingOperation() {
  getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizeOutdoorAirFlowRateDuringCoolingOperation();
}

bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setOutdoorAirFlowRateDuringHeatingOperation(double outdoorAirFlowRateDuringHeatingOperation) {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setOutdoorAirFlowRateDuringHeatingOperation(outdoorAirFlowRateDuringHeatingOperation);
}

void ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizeOutdoorAirFlowRateDuringHeatingOperation() {
  getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizeOutdoorAirFlowRateDuringHeatingOperation();
}

bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(double outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);
}

void ZoneHVACTerminalUnitVariableRefrigerantFlow::autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
  getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setSupplyAirFanOperatingModeSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setSupplyAirFanOperatingModeSchedule(schedule);
}

bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setZoneTerminalUnitOnParasiticElectricEnergyUse(double zoneTerminalUnitOnParasiticElectricEnergyUse) {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setZoneTerminalUnitOnParasiticElectricEnergyUse(zoneTerminalUnitOnParasiticElectricEnergyUse);
}

bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setZoneTerminalUnitOffParasiticElectricEnergyUse(double zoneTerminalUnitOffParasiticElectricEnergyUse) {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setZoneTerminalUnitOffParasiticElectricEnergyUse(zoneTerminalUnitOffParasiticElectricEnergyUse);
}

bool ZoneHVACTerminalUnitVariableRefrigerantFlow::setRatedTotalHeatingCapacitySizingRatio(double ratedTotalHeatingCapacitySizingRatio) {
  return getImpl<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl>()->setRatedTotalHeatingCapacitySizingRatio(ratedTotalHeatingCapacitySizingRatio);
}

/// @cond
ZoneHVACTerminalUnitVariableRefrigerantFlow::ZoneHVACTerminalUnitVariableRefrigerantFlow(std::shared_ptr<detail::ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl> impl)
  : ZoneHVACComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

