/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include <model/ZoneHVACPackagedTerminalAirConditioner.hpp>
#include <model/ZoneHVACPackagedTerminalAirConditioner_Impl.hpp>

#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/HVACComponent.hpp>
#include <model/HVACComponent_Impl.hpp>
#include <model/CoilHeatingWater.hpp>
#include <model/CoilHeatingWater_Impl.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ZoneHVAC_PackagedTerminalAirConditioner_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  ZoneHVACPackagedTerminalAirConditioner_Impl::ZoneHVACPackagedTerminalAirConditioner_Impl(
      const IdfObject& idfObject, 
      Model_Impl* model, 
      bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneHVACPackagedTerminalAirConditioner::iddObjectType());
  }

  ZoneHVACPackagedTerminalAirConditioner_Impl::ZoneHVACPackagedTerminalAirConditioner_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneHVACPackagedTerminalAirConditioner::iddObjectType());
  }

  ZoneHVACPackagedTerminalAirConditioner_Impl::ZoneHVACPackagedTerminalAirConditioner_Impl(const ZoneHVACPackagedTerminalAirConditioner_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

  ModelObject ZoneHVACPackagedTerminalAirConditioner_Impl::clone(Model model) const
  {
    ZoneHVACPackagedTerminalAirConditioner ptacClone = ZoneHVACComponent_Impl::clone(model).cast<ZoneHVACPackagedTerminalAirConditioner>();

    HVACComponent supplyFanClone = this->supplyAirFan().clone(model).cast<HVACComponent>();

    HVACComponent heatingCoilClone = this->heatingCoil().clone(model).cast<HVACComponent>();

    HVACComponent coolingCoilClone = this->coolingCoil().clone(model).cast<HVACComponent>();

    ptacClone.setSupplyAirFan(supplyFanClone);

    ptacClone.setHeatingCoil(heatingCoilClone);

    ptacClone.setCoolingCoil(coolingCoilClone);

    return ptacClone;
  }

  std::vector<IdfObject> ZoneHVACPackagedTerminalAirConditioner_Impl::remove()
  {
    if( boost::optional<CoilHeatingWater> waterHeatingCoil =
          heatingCoil().optionalCast<CoilHeatingWater>() )
    {
      if( boost::optional<PlantLoop> plantLoop = waterHeatingCoil->plantLoop() )
      {
        plantLoop->removeDemandBranchWithComponent( waterHeatingCoil.get() );
      }
    }

    return ZoneHVACComponent_Impl::remove();
  }

  const std::vector<std::string>& ZoneHVACPackagedTerminalAirConditioner_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ZoneHVACPackagedTerminalAirConditioner_Impl::iddObjectType() const {
    return ZoneHVACPackagedTerminalAirConditioner::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneHVACPackagedTerminalAirConditioner_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneHVAC_PackagedTerminalAirConditionerFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACPackagedTerminalAirConditioner","Availability"));
    }
    if (std::find(b,e,OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFanOperatingModeScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACPackagedTerminalAirConditioner","Supply Air Fan Operating Mode"));
    }
    return result;
  }

  std::vector<ModelObject> ZoneHVACPackagedTerminalAirConditioner_Impl::children() const
  {
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

  unsigned ZoneHVACPackagedTerminalAirConditioner_Impl::inletPort()
  {
    return OS_ZoneHVAC_PackagedTerminalAirConditionerFields::AirInletNodeName;
  }

  unsigned ZoneHVACPackagedTerminalAirConditioner_Impl::outletPort()
  {
    return OS_ZoneHVAC_PackagedTerminalAirConditionerFields::AirOutletNodeName;
  }

  Schedule ZoneHVACPackagedTerminalAirConditioner_Impl::availabilitySchedule() const
  {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<ZoneHVACPackagedTerminalAirConditioner_Impl*>(this)->setAvailabilitySchedule(*value);
      value = optionalAvailabilitySchedule();
    }
    OS_ASSERT(value);
    return value.get();
  }

  std::string ZoneHVACPackagedTerminalAirConditioner_Impl::outdoorAirMixerObjectType() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirMixerObjectType,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string ZoneHVACPackagedTerminalAirConditioner_Impl::outdoorAirMixerName() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirMixerName,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> ZoneHVACPackagedTerminalAirConditioner_Impl::supplyAirFlowRateDuringCoolingOperation() const {
    return getDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateDuringCoolingOperation,true);
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
    return getDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateDuringHeatingOperation,true);
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
    return getDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded,true);
  }

  bool ZoneHVACPackagedTerminalAirConditioner_Impl::isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> ZoneHVACPackagedTerminalAirConditioner_Impl::outdoorAirFlowRateDuringCoolingOperation() const {
    return getDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringCoolingOperation,true);
  }

  bool ZoneHVACPackagedTerminalAirConditioner_Impl::isOutdoorAirFlowRateDuringCoolingOperationAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringCoolingOperation, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> ZoneHVACPackagedTerminalAirConditioner_Impl::outdoorAirFlowRateDuringHeatingOperation() const {
    return getDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringHeatingOperation,true);
  }

  bool ZoneHVACPackagedTerminalAirConditioner_Impl::isOutdoorAirFlowRateDuringHeatingOperationAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringHeatingOperation, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> ZoneHVACPackagedTerminalAirConditioner_Impl::outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
    return getDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded,true);
  }

  bool ZoneHVACPackagedTerminalAirConditioner_Impl::isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  HVACComponent ZoneHVACPackagedTerminalAirConditioner_Impl::supplyAirFan() const
  {
    boost::optional<HVACComponent> fan = optionalSupplyAirFan();
    OS_ASSERT(fan);
    return fan.get();
  }

  HVACComponent ZoneHVACPackagedTerminalAirConditioner_Impl::heatingCoil() const
  {
    boost::optional<HVACComponent> coil = optionalHeatingCoil();
    OS_ASSERT(coil);
    return coil.get();
  }

  HVACComponent ZoneHVACPackagedTerminalAirConditioner_Impl::coolingCoil() const
  {
    boost::optional<HVACComponent> coil = optionalCoolingCoil();
    OS_ASSERT(coil);
    return coil.get();
  }

  std::string ZoneHVACPackagedTerminalAirConditioner_Impl::fanPlacement() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::FanPlacement,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACPackagedTerminalAirConditioner_Impl::isFanPlacementDefaulted() const {
    return isEmpty(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::FanPlacement);
  }

  boost::optional<Schedule> ZoneHVACPackagedTerminalAirConditioner_Impl::supplyAirFanOperatingModeSchedule() const
  {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(
             OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFanOperatingModeScheduleName);
  }

  bool ZoneHVACPackagedTerminalAirConditioner_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::AvailabilityScheduleName,
                              "ZoneHVACPackagedTerminalAirConditioner",
                              "Availability",
                              schedule);
    return result;
  }

  bool ZoneHVACPackagedTerminalAirConditioner_Impl::setOutdoorAirMixerObjectType(std::string outdoorAirMixerObjectType) {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirMixerObjectType, outdoorAirMixerObjectType);
    return result;
  }

  void ZoneHVACPackagedTerminalAirConditioner_Impl::setOutdoorAirMixerName(std::string outdoorAirMixerName) {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirMixerName, outdoorAirMixerName);
    OS_ASSERT(result);
  }

  bool ZoneHVACPackagedTerminalAirConditioner_Impl::setSupplyAirFlowRateDuringCoolingOperation(boost::optional<double> supplyAirFlowRateDuringCoolingOperation) {
    bool result = false;
    if (supplyAirFlowRateDuringCoolingOperation) {
      result = setDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateDuringCoolingOperation, supplyAirFlowRateDuringCoolingOperation.get());
    } else {
      result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateDuringCoolingOperation, "");
    }
    return result;
  }

  void ZoneHVACPackagedTerminalAirConditioner_Impl::autosizeSupplyAirFlowRateDuringCoolingOperation() {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateDuringCoolingOperation, "Autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACPackagedTerminalAirConditioner_Impl::setSupplyAirFlowRateDuringHeatingOperation(boost::optional<double> supplyAirFlowRateDuringHeatingOperation) {
    bool result = false;
    if (supplyAirFlowRateDuringHeatingOperation) {
      result = setDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateDuringHeatingOperation, supplyAirFlowRateDuringHeatingOperation.get());
    } else {
      result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateDuringHeatingOperation, "");
    }
    return result;
  }

  void ZoneHVACPackagedTerminalAirConditioner_Impl::autosizeSupplyAirFlowRateDuringHeatingOperation() {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateDuringHeatingOperation, "Autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACPackagedTerminalAirConditioner_Impl::setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded) {
    bool result = false;
    if (supplyAirFlowRateWhenNoCoolingorHeatingisNeeded) {
      result = setDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, supplyAirFlowRateWhenNoCoolingorHeatingisNeeded.get());
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

  bool ZoneHVACPackagedTerminalAirConditioner_Impl::setOutdoorAirFlowRateDuringCoolingOperation(boost::optional<double> outdoorAirFlowRateDuringCoolingOperation) {
    bool result = false;
    if (outdoorAirFlowRateDuringCoolingOperation) {
      result = setDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringCoolingOperation, outdoorAirFlowRateDuringCoolingOperation.get());
    } else {
      result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringCoolingOperation, "");
    }
    return result;
  }

  void ZoneHVACPackagedTerminalAirConditioner_Impl::autosizeOutdoorAirFlowRateDuringCoolingOperation() {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringCoolingOperation, "Autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACPackagedTerminalAirConditioner_Impl::setOutdoorAirFlowRateDuringHeatingOperation(boost::optional<double> outdoorAirFlowRateDuringHeatingOperation) {
    bool result = false;
    if (outdoorAirFlowRateDuringHeatingOperation) {
      result = setDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringHeatingOperation, outdoorAirFlowRateDuringHeatingOperation.get());
    } else {
      result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringHeatingOperation, "");
    }
    return result;
  }

  void ZoneHVACPackagedTerminalAirConditioner_Impl::autosizeOutdoorAirFlowRateDuringHeatingOperation() {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringHeatingOperation, "Autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACPackagedTerminalAirConditioner_Impl::setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
    bool result = false;
    if (outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
      result = setDouble(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded.get());
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

  void ZoneHVACPackagedTerminalAirConditioner_Impl::setSupplyAirFan( HVACComponent & fan )
  {
    bool isAllowedType = false;

    if( fan.iddObjectType() == IddObjectType::OS_Fan_ConstantVolume )
    {
      isAllowedType = true;
    }
    else if( fan.iddObjectType() == IddObjectType::OS_Fan_OnOff )
    {
      isAllowedType = true;
    }

    if( isAllowedType )
    {
      setPointer(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFanName,fan.handle());
    }
  }

  void ZoneHVACPackagedTerminalAirConditioner_Impl::setHeatingCoil( HVACComponent & heatingCoil )
  {
    bool isAllowedType = false;

    if( heatingCoil.iddObjectType() == IddObjectType::OS_Coil_Heating_Gas )
    {
      isAllowedType = true;
    }
    else if( heatingCoil.iddObjectType() == IddObjectType::OS_Coil_Heating_Electric )
    {
      isAllowedType = true;
    }
    else if( heatingCoil.iddObjectType() == IddObjectType::OS_Coil_Heating_Water )
    {
      isAllowedType = true;
    }

    if( isAllowedType )
    {
      setPointer(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::HeatingCoilName,heatingCoil.handle());
    }
  }

  void ZoneHVACPackagedTerminalAirConditioner_Impl::setCoolingCoil( HVACComponent & coolingCoil )
  {
    bool isAllowedType = false;

    if( coolingCoil.iddObjectType() == IddObjectType::OS_Coil_Cooling_DX_SingleSpeed )
    {
      isAllowedType = true;
    }

    if( isAllowedType )
    {
      setPointer(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::CoolingCoilName,coolingCoil.handle());
    }
  }

  bool ZoneHVACPackagedTerminalAirConditioner_Impl::setFanPlacement(std::string fanPlacement) {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::FanPlacement, fanPlacement);
    return result;
  }

  void ZoneHVACPackagedTerminalAirConditioner_Impl::resetFanPlacement() {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::FanPlacement, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACPackagedTerminalAirConditioner_Impl::setSupplyAirFanOperatingModeSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFanOperatingModeScheduleName,
                              "ZoneHVACPackagedTerminalAirConditioner",
                              "Supply Air Fan Operating Mode",
                              schedule);
    return result;
  }

  void ZoneHVACPackagedTerminalAirConditioner_Impl::resetSupplyAirFanOperatingModeSchedule() {
    bool result = setString(OS_ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFanOperatingModeScheduleName, "");
    OS_ASSERT(result);
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

  bool ZoneHVACPackagedTerminalAirConditioner_Impl::setSupplyAirFanOperatingModeScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
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

ZoneHVACPackagedTerminalAirConditioner::ZoneHVACPackagedTerminalAirConditioner( const Model& model,
                                                                                Schedule & availabilitySchedule,
                                                                                HVACComponent & supplyAirFan,
                                                                                HVACComponent & heatingCoil,
                                                                                HVACComponent & coolingCoil )
  : ZoneHVACComponent(ZoneHVACPackagedTerminalAirConditioner::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>());

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
  setFanPlacement("DrawThrough");
  resetSupplyAirFanOperatingModeSchedule();

  autosizeSupplyAirFlowRateDuringCoolingOperation();
  autosizeSupplyAirFlowRateDuringHeatingOperation();
  autosizeOutdoorAirFlowRateDuringCoolingOperation();
  autosizeOutdoorAirFlowRateDuringHeatingOperation();
  autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
  autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

IddObjectType ZoneHVACPackagedTerminalAirConditioner::iddObjectType() {
  IddObjectType result(IddObjectType::OS_ZoneHVAC_PackagedTerminalAirConditioner);
  return result;
}

std::vector<std::string> ZoneHVACPackagedTerminalAirConditioner::outdoorAirMixerObjectTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirMixerObjectType);
}

std::vector<std::string> ZoneHVACPackagedTerminalAirConditioner::validOutdoorAirMixerObjectTypeValues() {
  return ZoneHVACPackagedTerminalAirConditioner::outdoorAirMixerObjectTypeValues();
}

std::vector<std::string> ZoneHVACPackagedTerminalAirConditioner::fanPlacementValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_PackagedTerminalAirConditionerFields::FanPlacement);
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

bool ZoneHVACPackagedTerminalAirConditioner::setOutdoorAirMixerObjectType(std::string outdoorAirMixerObjectType) {
  return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setOutdoorAirMixerObjectType(outdoorAirMixerObjectType);
}

void ZoneHVACPackagedTerminalAirConditioner::setOutdoorAirMixerName(std::string outdoorAirMixerName) {
  getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setOutdoorAirMixerName(outdoorAirMixerName);
}

bool ZoneHVACPackagedTerminalAirConditioner::setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation) {
  return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setSupplyAirFlowRateDuringCoolingOperation(supplyAirFlowRateDuringCoolingOperation);
}

void ZoneHVACPackagedTerminalAirConditioner::autosizeSupplyAirFlowRateDuringCoolingOperation() {
  getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->autosizeSupplyAirFlowRateDuringCoolingOperation();
}

bool ZoneHVACPackagedTerminalAirConditioner::setSupplyAirFlowRateDuringHeatingOperation(double supplyAirFlowRateDuringHeatingOperation) {
  return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setSupplyAirFlowRateDuringHeatingOperation(supplyAirFlowRateDuringHeatingOperation);
}

void ZoneHVACPackagedTerminalAirConditioner::autosizeSupplyAirFlowRateDuringHeatingOperation() {
  getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->autosizeSupplyAirFlowRateDuringHeatingOperation();
}

bool ZoneHVACPackagedTerminalAirConditioner::setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(double supplyAirFlowRateWhenNoCoolingorHeatingisNeeded) {
  return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);
}

void ZoneHVACPackagedTerminalAirConditioner::resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
  getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

void ZoneHVACPackagedTerminalAirConditioner::autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
  getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

bool ZoneHVACPackagedTerminalAirConditioner::setOutdoorAirFlowRateDuringCoolingOperation(double outdoorAirFlowRateDuringCoolingOperation) {
  return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setOutdoorAirFlowRateDuringCoolingOperation(outdoorAirFlowRateDuringCoolingOperation);
}

void ZoneHVACPackagedTerminalAirConditioner::autosizeOutdoorAirFlowRateDuringCoolingOperation() {
  getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->autosizeOutdoorAirFlowRateDuringCoolingOperation();
}

bool ZoneHVACPackagedTerminalAirConditioner::setOutdoorAirFlowRateDuringHeatingOperation(double outdoorAirFlowRateDuringHeatingOperation) {
  return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setOutdoorAirFlowRateDuringHeatingOperation(outdoorAirFlowRateDuringHeatingOperation);
}

void ZoneHVACPackagedTerminalAirConditioner::autosizeOutdoorAirFlowRateDuringHeatingOperation() {
  getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->autosizeOutdoorAirFlowRateDuringHeatingOperation();
}

bool ZoneHVACPackagedTerminalAirConditioner::setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(double outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
  return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);
}

void ZoneHVACPackagedTerminalAirConditioner::resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
  getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

void ZoneHVACPackagedTerminalAirConditioner::autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
  getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

bool ZoneHVACPackagedTerminalAirConditioner::setFanPlacement(std::string fanPlacement) {
  return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setFanPlacement(fanPlacement);
}

void ZoneHVACPackagedTerminalAirConditioner::resetFanPlacement() {
  getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->resetFanPlacement();
}

ZoneHVACPackagedTerminalAirConditioner::ZoneHVACPackagedTerminalAirConditioner(boost::shared_ptr<detail::ZoneHVACPackagedTerminalAirConditioner_Impl> impl)
  : ZoneHVACComponent(impl)
{}

boost::optional<Schedule> ZoneHVACPackagedTerminalAirConditioner::supplyAirFanOperatingModeSchedule() const
{
  return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->supplyAirFanOperatingModeSchedule();
}

bool ZoneHVACPackagedTerminalAirConditioner::setSupplyAirFanOperatingModeSchedule( Schedule & schedule )
{
  return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setSupplyAirFanOperatingModeSchedule(schedule);
}

void ZoneHVACPackagedTerminalAirConditioner::resetSupplyAirFanOperatingModeSchedule()
{
  getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->resetSupplyAirFanOperatingModeSchedule();
}

Schedule ZoneHVACPackagedTerminalAirConditioner::availabilitySchedule() const
{
  return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->availabilitySchedule();
}

bool ZoneHVACPackagedTerminalAirConditioner::setAvailabilitySchedule( Schedule & schedule )
{
  return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setAvailabilitySchedule(schedule);
}

HVACComponent ZoneHVACPackagedTerminalAirConditioner::supplyAirFan() const
{
  return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->supplyAirFan();
}

void ZoneHVACPackagedTerminalAirConditioner::setSupplyAirFan( HVACComponent & fan )
{
  getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setSupplyAirFan(fan);
}

HVACComponent ZoneHVACPackagedTerminalAirConditioner::heatingCoil() const
{
  return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->heatingCoil();
}

void ZoneHVACPackagedTerminalAirConditioner::setHeatingCoil( HVACComponent & heatingCoil )
{
  getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setHeatingCoil(heatingCoil);
}

HVACComponent ZoneHVACPackagedTerminalAirConditioner::coolingCoil() const
{
  return getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->coolingCoil();
}

void ZoneHVACPackagedTerminalAirConditioner::setCoolingCoil( HVACComponent & coolingCoil )
{
  getImpl<detail::ZoneHVACPackagedTerminalAirConditioner_Impl>()->setCoolingCoil(coolingCoil);
}

} // model
} // openstudio

