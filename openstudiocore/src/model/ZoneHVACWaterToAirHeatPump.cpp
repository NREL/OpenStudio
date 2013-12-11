/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/ZoneHVACWaterToAirHeatPump.hpp>
#include <model/ZoneHVACWaterToAirHeatPump_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ScheduleTypeLimits.hpp>
#include <model/ScheduleTypeRegistry.hpp>
#include <model/HVACComponent.hpp>
#include <model/HVACComponent_Impl.hpp>
#include <model/FanOnOff.hpp>
#include <model/FanOnOff_Impl.hpp>
#include <model/CoilHeatingWaterToAirHeatPumpEquationFit.hpp>
#include <model/CoilHeatingWaterToAirHeatPumpEquationFit_Impl.hpp>
#include <model/CoilCoolingWaterToAirHeatPumpEquationFit.hpp>
#include <model/CoilCoolingWaterToAirHeatPumpEquationFit_Impl.hpp>
#include <model/CoilHeatingElectric.hpp>
#include <model/CoilHeatingElectric_Impl.hpp>
#include <model/CoilHeatingGas.hpp>
#include <model/CoilHeatingGas_Impl.hpp>
#include <model/CoilHeatingWater.hpp>
#include <model/CoilHeatingWater_Impl.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>


#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_ZoneHVAC_WaterToAirHeatPump_FieldEnums.hxx>

#include <utilities/units/Unit.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace model {

namespace detail {

  ZoneHVACWaterToAirHeatPump_Impl::ZoneHVACWaterToAirHeatPump_Impl(const IdfObject& idfObject,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZoneHVACWaterToAirHeatPump::iddObjectType());
  }

  ZoneHVACWaterToAirHeatPump_Impl::ZoneHVACWaterToAirHeatPump_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZoneHVACWaterToAirHeatPump::iddObjectType());
  }

  ZoneHVACWaterToAirHeatPump_Impl::ZoneHVACWaterToAirHeatPump_Impl(const ZoneHVACWaterToAirHeatPump_Impl& other,
                                                                   Model_Impl* model,
                                                                   bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

    ModelObject ZoneHVACWaterToAirHeatPump_Impl::clone(Model model) const
  {
    ZoneHVACWaterToAirHeatPump wahpClone = ZoneHVACComponent_Impl::clone(model).cast<ZoneHVACWaterToAirHeatPump>();

    HVACComponent supplyFanClone = this->supplyAirFan().clone(model).cast<HVACComponent>();

    HVACComponent heatingCoilClone = this->heatingCoil().clone(model).cast<HVACComponent>();

    HVACComponent coolingCoilClone = this->coolingCoil().clone(model).cast<HVACComponent>();

    HVACComponent supplementalHeatingCoilClone = this->supplementalHeatingCoil().clone(model).cast<HVACComponent>();

    wahpClone.setSupplyAirFan(supplyFanClone);

    wahpClone.setHeatingCoil(heatingCoilClone);

    wahpClone.setCoolingCoil(coolingCoilClone);

    wahpClone.setSupplementalHeatingCoil(supplementalHeatingCoilClone);

    return wahpClone;
  }

  std::vector<IdfObject> ZoneHVACWaterToAirHeatPump_Impl::remove()
  {
    if( boost::optional<CoilHeatingWaterToAirHeatPumpEquationFit> waterHeatingCoil =
          heatingCoil().optionalCast<CoilHeatingWaterToAirHeatPumpEquationFit>() )
    {
      if( boost::optional<PlantLoop> plantLoop = waterHeatingCoil->plantLoop() )
      {
        plantLoop->removeDemandBranchWithComponent( waterHeatingCoil.get() );
      }
    }
    if( boost::optional<CoilCoolingWaterToAirHeatPumpEquationFit> waterCoolingCoil =
          coolingCoil().optionalCast<CoilCoolingWaterToAirHeatPumpEquationFit>() )
    {
      if( boost::optional<PlantLoop> plantLoop = waterCoolingCoil->plantLoop() )
      {
        plantLoop->removeDemandBranchWithComponent( waterCoolingCoil.get() );
      }
    }
    return ZoneHVACComponent_Impl::remove();
  }

  const std::vector<std::string>& ZoneHVACWaterToAirHeatPump_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType ZoneHVACWaterToAirHeatPump_Impl::iddObjectType() const {
    return ZoneHVACWaterToAirHeatPump::iddObjectType();
  }

  std::vector<ScheduleTypeKey> ZoneHVACWaterToAirHeatPump_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_ZoneHVAC_WaterToAirHeatPumpFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACWaterToAirHeatPump","Availability"));
    }
    if (std::find(b,e,OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFanOperatingModeScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("ZoneHVACWaterToAirHeatPump","Supply Air Fan Operating Mode"));
    }
    return result;
  }

  std::vector<ModelObject> ZoneHVACWaterToAirHeatPump_Impl::children() const
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

  unsigned ZoneHVACWaterToAirHeatPump_Impl::inletPort()
  {
    return OS_ZoneHVAC_WaterToAirHeatPumpFields::AirInletNodeName;
  }

  unsigned ZoneHVACWaterToAirHeatPump_Impl::outletPort()
  {
    return OS_ZoneHVAC_WaterToAirHeatPumpFields::AirOutletNodeName;
  }

  Schedule ZoneHVACWaterToAirHeatPump_Impl::availabilitySchedule() const {
    boost::optional<Schedule> value = optionalAvailabilitySchedule();
    if (!value) {
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<ZoneHVACWaterToAirHeatPump_Impl*>(this)->setAvailabilitySchedule(*value);
      value = optionalAvailabilitySchedule();
    }
    OS_ASSERT(value);    
    return value.get();
  }

  boost::optional<double> ZoneHVACWaterToAirHeatPump_Impl::supplyAirFlowRateDuringCoolingOperation() const {
    return getDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateDuringCoolingOperation,true);
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::isSupplyAirFlowRateDuringCoolingOperationAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateDuringCoolingOperation, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> ZoneHVACWaterToAirHeatPump_Impl::supplyAirFlowRateDuringHeatingOperation() const {
    return getDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateDuringHeatingOperation,true);
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::isSupplyAirFlowRateDuringHeatingOperationAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateDuringHeatingOperation, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> ZoneHVACWaterToAirHeatPump_Impl::supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
    return getDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded,true);
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> ZoneHVACWaterToAirHeatPump_Impl::outdoorAirFlowRateDuringCoolingOperation() const {
    return getDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateDuringCoolingOperation,true);
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::isOutdoorAirFlowRateDuringCoolingOperationAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateDuringCoolingOperation, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> ZoneHVACWaterToAirHeatPump_Impl::outdoorAirFlowRateDuringHeatingOperation() const {
    return getDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateDuringHeatingOperation,true);
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::isOutdoorAirFlowRateDuringHeatingOperationAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateDuringHeatingOperation, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> ZoneHVACWaterToAirHeatPump_Impl::outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
    return getDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded,true);
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  HVACComponent ZoneHVACWaterToAirHeatPump_Impl::supplyAirFan() const {
    boost::optional<HVACComponent> value = optionalSupplyAirFan();
    OS_ASSERT(value);
    return value.get();
  }

  HVACComponent ZoneHVACWaterToAirHeatPump_Impl::heatingCoil() const {
    boost::optional<HVACComponent> value = optionalHeatingCoil();
    OS_ASSERT(value);
    return value.get();
  }

  HVACComponent ZoneHVACWaterToAirHeatPump_Impl::coolingCoil() const {
    boost::optional<HVACComponent> value = optionalCoolingCoil();
    OS_ASSERT(value);
    return value.get();
  }

  double ZoneHVACWaterToAirHeatPump_Impl::maximumCyclingRate() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::MaximumCyclingRate,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::isMaximumCyclingRateDefaulted() const {
    return isEmpty(OS_ZoneHVAC_WaterToAirHeatPumpFields::MaximumCyclingRate);
  }

  double ZoneHVACWaterToAirHeatPump_Impl::heatPumpTimeConstant() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::HeatPumpTimeConstant,true);
    OS_ASSERT(value);
    return value.get();
  }


  bool ZoneHVACWaterToAirHeatPump_Impl::isHeatPumpTimeConstantDefaulted() const {
    return isEmpty(OS_ZoneHVAC_WaterToAirHeatPumpFields::HeatPumpTimeConstant);
  }

  double ZoneHVACWaterToAirHeatPump_Impl::fractionofOnCyclePowerUse() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::FractionofOnCyclePowerUse,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::isFractionofOnCyclePowerUseDefaulted() const {
    return isEmpty(OS_ZoneHVAC_WaterToAirHeatPumpFields::FractionofOnCyclePowerUse);
  }

  double ZoneHVACWaterToAirHeatPump_Impl::heatPumpFanDelayTime() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::HeatPumpFanDelayTime,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::isHeatPumpFanDelayTimeDefaulted() const {
    return isEmpty(OS_ZoneHVAC_WaterToAirHeatPumpFields::HeatPumpFanDelayTime);
  }

  HVACComponent ZoneHVACWaterToAirHeatPump_Impl::supplementalHeatingCoil() const {
    boost::optional<HVACComponent> value = optionalSupplementalHeatingCoil();
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> ZoneHVACWaterToAirHeatPump_Impl::maximumSupplyAirTemperaturefromSupplementalHeater() const {
    return getDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::MaximumSupplyAirTemperaturefromSupplementalHeater,true);
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_ZoneHVAC_WaterToAirHeatPumpFields::MaximumSupplyAirTemperaturefromSupplementalHeater, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  double ZoneHVACWaterToAirHeatPump_Impl::maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const {
    boost::optional<double> value = getDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const {
    return isEmpty(OS_ZoneHVAC_WaterToAirHeatPumpFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
  }

  std::string ZoneHVACWaterToAirHeatPump_Impl::fanPlacement() const {
    boost::optional<std::string> value = getString(OS_ZoneHVAC_WaterToAirHeatPumpFields::FanPlacement,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::isFanPlacementDefaulted() const {
    return isEmpty(OS_ZoneHVAC_WaterToAirHeatPumpFields::FanPlacement);
  }

  boost::optional<Schedule> ZoneHVACWaterToAirHeatPump_Impl::supplyAirFanOperatingModeSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFanOperatingModeScheduleName);
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_WaterToAirHeatPumpFields::AvailabilityScheduleName,
                              "ZoneHVACWaterToAirHeatPump",
                              "Availability",
                              schedule);
    return result;
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setSupplyAirFlowRateDuringCoolingOperation(boost::optional<double> supplyAirFlowRateDuringCoolingOperation) {
    bool result = false;
    if (supplyAirFlowRateDuringCoolingOperation) {
      result = setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateDuringCoolingOperation, supplyAirFlowRateDuringCoolingOperation.get());
    }
    return result;
  }

  void ZoneHVACWaterToAirHeatPump_Impl::resetSupplyAirFlowRateDuringCoolingOperation() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateDuringCoolingOperation, "");
    OS_ASSERT(result);
  }

  void ZoneHVACWaterToAirHeatPump_Impl::autosizeSupplyAirFlowRateDuringCoolingOperation() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateDuringCoolingOperation, "Autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setSupplyAirFlowRateDuringHeatingOperation(boost::optional<double> supplyAirFlowRateDuringHeatingOperation) {
    bool result(false);
    if (supplyAirFlowRateDuringHeatingOperation) {
      result = setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateDuringHeatingOperation, supplyAirFlowRateDuringHeatingOperation.get());
    }
    return result;
  }
 
  void ZoneHVACWaterToAirHeatPump_Impl::resetSupplyAirFlowRateDuringHeatingOperation() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateDuringHeatingOperation, "");
    OS_ASSERT(result);
  }

  void ZoneHVACWaterToAirHeatPump_Impl::autosizeSupplyAirFlowRateDuringHeatingOperation() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateDuringHeatingOperation, "Autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded) {
    bool result(false);
    if (supplyAirFlowRateWhenNoCoolingorHeatingisNeeded) {
      result = setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, supplyAirFlowRateWhenNoCoolingorHeatingisNeeded.get());
    }
    else {
      resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
      result = true;
    }
    return result;
  }

  void ZoneHVACWaterToAirHeatPump_Impl::resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, "");
    OS_ASSERT(result);
  }

  void ZoneHVACWaterToAirHeatPump_Impl::autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setOutdoorAirFlowRateDuringCoolingOperation(boost::optional<double> outdoorAirFlowRateDuringCoolingOperation) {
    bool result(false);
    if (outdoorAirFlowRateDuringCoolingOperation) {
      result = setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateDuringCoolingOperation, outdoorAirFlowRateDuringCoolingOperation.get());
    }
    return result;
  }

  void ZoneHVACWaterToAirHeatPump_Impl::resetOutdoorAirFlowRateDuringCoolingOperation() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateDuringCoolingOperation, "");
    OS_ASSERT(result);
  }

  void ZoneHVACWaterToAirHeatPump_Impl::autosizeOutdoorAirFlowRateDuringCoolingOperation() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateDuringCoolingOperation, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setOutdoorAirFlowRateDuringHeatingOperation(boost::optional<double> outdoorAirFlowRateDuringHeatingOperation) {
    bool result(false);
    if (outdoorAirFlowRateDuringHeatingOperation) {
      result = setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateDuringHeatingOperation, outdoorAirFlowRateDuringHeatingOperation.get());
    }
    return result;
  }

  void ZoneHVACWaterToAirHeatPump_Impl::resetOutdoorAirFlowRateDuringHeatingOperation() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateDuringHeatingOperation, "");
    OS_ASSERT(result);
  }

 void ZoneHVACWaterToAirHeatPump_Impl::autosizeOutdoorAirFlowRateDuringHeatingOperation() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateDuringHeatingOperation, "autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
    bool result(false);
    if (outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
      result = setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded.get());
    }
    else {
      resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
      result = true;
    }
    return result;
  }
 
 void ZoneHVACWaterToAirHeatPump_Impl::resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, "");
    OS_ASSERT(result);
  }

 void ZoneHVACWaterToAirHeatPump_Impl::autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, "Autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setSupplyAirFan(HVACComponent& fansOnOff) {
    bool isAllowedType = false;
    if( fansOnOff.iddObjectType() == IddObjectType::OS_Fan_OnOff)
    {
      isAllowedType = true;
    }

    if( isAllowedType )
    {
      return setPointer(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFanName,fansOnOff.handle());
    }
    return false;
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setHeatingCoil(HVACComponent& heatingCoilsWaterToAirHP) 
  {
    bool isAllowedType = false;

    if( heatingCoilsWaterToAirHP.iddObjectType() == IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_EquationFit )
    {
      isAllowedType = true;
    }

    if( isAllowedType )
    {
      return setPointer(OS_ZoneHVAC_WaterToAirHeatPumpFields::HeatingCoilName,heatingCoilsWaterToAirHP.handle());
    }
    return false;
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setCoolingCoil(HVACComponent& coolingCoilsWaterToAirHP) {
    bool isAllowedType = false;

    if( coolingCoilsWaterToAirHP.iddObjectType() == IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_EquationFit )
    {
      isAllowedType = true;
    }

    if( isAllowedType )
    {
      return setPointer(OS_ZoneHVAC_WaterToAirHeatPumpFields::CoolingCoilName,coolingCoilsWaterToAirHP.handle());
    }
    return false;
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setMaximumCyclingRate(boost::optional<double> maximumCyclingRate) {
    bool result = setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::MaximumCyclingRate, maximumCyclingRate.get());
    return result;
  }

  void ZoneHVACWaterToAirHeatPump_Impl::resetMaximumCyclingRate() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::MaximumCyclingRate, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setHeatPumpTimeConstant(boost::optional<double> heatPumpTimeConstant) {
    bool result = setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::HeatPumpTimeConstant, heatPumpTimeConstant.get());
    return result;
  }

  void ZoneHVACWaterToAirHeatPump_Impl::resetHeatPumpTimeConstant() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::HeatPumpTimeConstant, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setFractionofOnCyclePowerUse(boost::optional<double> fractionofOnCyclePowerUse) {
    bool result = setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::FractionofOnCyclePowerUse, fractionofOnCyclePowerUse.get());
    return result;
  }

  void ZoneHVACWaterToAirHeatPump_Impl::resetFractionofOnCyclePowerUse() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::FractionofOnCyclePowerUse, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setHeatPumpFanDelayTime(boost::optional<double> heatPumpFanDelayTime) {
    bool result = setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::HeatPumpFanDelayTime, heatPumpFanDelayTime.get());
    return result;
  }

  void ZoneHVACWaterToAirHeatPump_Impl::resetHeatPumpFanDelayTime() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::HeatPumpFanDelayTime, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setSupplementalHeatingCoil(HVACComponent& heatingCoilName) {
    bool result = setPointer(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplementalHeatingCoilName, heatingCoilName.handle());
    return result;
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setMaximumSupplyAirTemperaturefromSupplementalHeater(boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater) {
    bool result(false);
    if (maximumSupplyAirTemperaturefromSupplementalHeater) {
      result = setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::MaximumSupplyAirTemperaturefromSupplementalHeater, maximumSupplyAirTemperaturefromSupplementalHeater.get());
    }
    return result;
  }

  void ZoneHVACWaterToAirHeatPump_Impl::resetMaximumSupplyAirTemperaturefromSupplementalHeater() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::MaximumSupplyAirTemperaturefromSupplementalHeater,"");
    OS_ASSERT(result); 
  }

  void ZoneHVACWaterToAirHeatPump_Impl::autosizeMaximumSupplyAirTemperaturefromSupplementalHeater() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::MaximumSupplyAirTemperaturefromSupplementalHeater, "Autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(boost::optional<double> maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation) {
    bool result = setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation, maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation.get());
    return result;
  }

  void ZoneHVACWaterToAirHeatPump_Impl::resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setFanPlacement(std::string fanPlacement) {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::FanPlacement, fanPlacement);
    return result;
  }

  void ZoneHVACWaterToAirHeatPump_Impl::resetFanPlacement() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::FanPlacement, "");
    OS_ASSERT(result);
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setSupplyAirFanOperatingModeSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFanOperatingModeScheduleName,
                              "ZoneHVACWaterToAirHeatPump",
                              "Supply Air Fan Operating Mode",
                              schedule);
    return result;
  }

  void ZoneHVACWaterToAirHeatPump_Impl::resetSupplyAirFanOperatingModeSchedule() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFanOperatingModeScheduleName, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> ZoneHVACWaterToAirHeatPump_Impl::optionalAvailabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_ZoneHVAC_WaterToAirHeatPumpFields::AvailabilityScheduleName);
  }

  boost::optional<HVACComponent> ZoneHVACWaterToAirHeatPump_Impl::optionalSupplyAirFan() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFanName);
  }

  boost::optional<HVACComponent> ZoneHVACWaterToAirHeatPump_Impl::optionalHeatingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_WaterToAirHeatPumpFields::HeatingCoilName);
  }

  boost::optional<HVACComponent> ZoneHVACWaterToAirHeatPump_Impl::optionalCoolingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_WaterToAirHeatPumpFields::CoolingCoilName);
  }

  boost::optional<HVACComponent> ZoneHVACWaterToAirHeatPump_Impl::optionalSupplementalHeatingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplementalHeatingCoilName);
  }

  std::vector<std::string> ZoneHVACWaterToAirHeatPump_Impl::fanPlacementValues() const {
    return ZoneHVACWaterToAirHeatPump::fanPlacementValues();
  }

  //std::vector<std::string> ZoneHVACWaterToAirHeatPump_Impl::fanPlacementValues() const {
  //  return ZoneHVACWaterToAirHeatPump::fanPlacementValues();
  //}

  boost::optional<ModelObject> ZoneHVACWaterToAirHeatPump_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  boost::optional<ModelObject> ZoneHVACWaterToAirHeatPump_Impl::supplyAirFanAsModelObject() const {
    OptionalModelObject result = supplyAirFan();
    return result;
  }

  boost::optional<ModelObject> ZoneHVACWaterToAirHeatPump_Impl::heatingCoilAsModelObject() const {
    OptionalModelObject result = heatingCoil();
    return result;
  }

  boost::optional<ModelObject> ZoneHVACWaterToAirHeatPump_Impl::coolingCoilAsModelObject() const {
    OptionalModelObject result = coolingCoil();
    return result;
  }

  boost::optional<ModelObject> ZoneHVACWaterToAirHeatPump_Impl::supplementalHeatingCoilAsModelObject() const {
    OptionalModelObject result = supplementalHeatingCoil();
    return result;
  }

  boost::optional<ModelObject> ZoneHVACWaterToAirHeatPump_Impl::supplyAirFanOperatingModeScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = supplyAirFanOperatingModeSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setSupplyAirFanAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
      if (intermediate) {
        HVACComponent fansOnOff(*intermediate);
        setSupplyAirFan(fansOnOff);
        return true;
      }
    }
    return false;
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
      if (intermediate) {
        HVACComponent heatingCoilsWaterToAirHP(*intermediate);
        setHeatingCoil(heatingCoilsWaterToAirHP);
        return true;
      }
    }
    return false;
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setCoolingCoilAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
      if (intermediate) {
        HVACComponent coolingCoilsWaterToAirHP(*intermediate);
        setCoolingCoil(coolingCoilsWaterToAirHP);
        return true;
      }
    }
    return false;
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setSupplementalHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
      if (intermediate) {
        HVACComponent heatingCoilName(*intermediate);
        setSupplementalHeatingCoil(heatingCoilName);
        return true;
      }
    }
    return false;
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setSupplyAirFanOperatingModeScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
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

ZoneHVACWaterToAirHeatPump::ZoneHVACWaterToAirHeatPump(const Model& model,
                                                       Schedule & availabilitySchedule,
                                                       HVACComponent & supplyAirFan,
                                                       HVACComponent & heatingCoil,
                                                       HVACComponent & coolingCoil,
                                                       HVACComponent & supplementalHeatingCoil)
  : ZoneHVACComponent(ZoneHVACWaterToAirHeatPump::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>());

  bool ok = setAvailabilitySchedule(availabilitySchedule);
  if (!ok) {
    remove();
    LOG_AND_THROW("Unable to set " << briefDescription() << "'s availability schedule to "
                  << availabilitySchedule.briefDescription() << ".");
  }

  ok = setSupplyAirFan(supplyAirFan);
  OS_ASSERT(ok);
  ok = setHeatingCoil(heatingCoil);
  OS_ASSERT(ok);
  ok = setCoolingCoil(coolingCoil);
  OS_ASSERT(ok);
  ok = setSupplementalHeatingCoil(supplementalHeatingCoil);
  OS_ASSERT(ok);

  autosizeSupplyAirFlowRateDuringCoolingOperation();
  autosizeSupplyAirFlowRateDuringHeatingOperation();
  autosizeOutdoorAirFlowRateDuringCoolingOperation();
  autosizeOutdoorAirFlowRateDuringHeatingOperation();
  autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();
  autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
  autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
  setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::AvailabilityManagerListName,"");
}

IddObjectType ZoneHVACWaterToAirHeatPump::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneHVAC_WaterToAirHeatPump);
}

std::vector<std::string> ZoneHVACWaterToAirHeatPump::fanPlacementValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_ZoneHVAC_WaterToAirHeatPumpFields::FanPlacement);
}

Schedule ZoneHVACWaterToAirHeatPump::availabilitySchedule() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->availabilitySchedule();
}

boost::optional<double> ZoneHVACWaterToAirHeatPump::supplyAirFlowRateDuringCoolingOperation() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->supplyAirFlowRateDuringCoolingOperation();
}

bool ZoneHVACWaterToAirHeatPump::isSupplyAirFlowRateDuringCoolingOperationAutosized() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->isSupplyAirFlowRateDuringCoolingOperationAutosized();
}

boost::optional<double> ZoneHVACWaterToAirHeatPump::supplyAirFlowRateDuringHeatingOperation() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->supplyAirFlowRateDuringHeatingOperation();
}

bool ZoneHVACWaterToAirHeatPump::isSupplyAirFlowRateDuringHeatingOperationAutosized() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->isSupplyAirFlowRateDuringHeatingOperationAutosized();
}

boost::optional<double> ZoneHVACWaterToAirHeatPump::supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->supplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

bool ZoneHVACWaterToAirHeatPump::isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized();
}

boost::optional<double> ZoneHVACWaterToAirHeatPump::outdoorAirFlowRateDuringCoolingOperation() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->outdoorAirFlowRateDuringCoolingOperation();
}

bool ZoneHVACWaterToAirHeatPump::isOutdoorAirFlowRateDuringCoolingOperationAutosized() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->isOutdoorAirFlowRateDuringCoolingOperationAutosized();
}

boost::optional<double> ZoneHVACWaterToAirHeatPump::outdoorAirFlowRateDuringHeatingOperation() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->outdoorAirFlowRateDuringHeatingOperation();
}

bool ZoneHVACWaterToAirHeatPump::isOutdoorAirFlowRateDuringHeatingOperationAutosized() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->isOutdoorAirFlowRateDuringHeatingOperationAutosized();
}

boost::optional<double> ZoneHVACWaterToAirHeatPump::outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

bool ZoneHVACWaterToAirHeatPump::isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized();
}

HVACComponent ZoneHVACWaterToAirHeatPump::supplyAirFan() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->supplyAirFan();
}

HVACComponent ZoneHVACWaterToAirHeatPump::heatingCoil() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->heatingCoil();
}

HVACComponent ZoneHVACWaterToAirHeatPump::coolingCoil() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->coolingCoil();
}

double ZoneHVACWaterToAirHeatPump::maximumCyclingRate() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->maximumCyclingRate();
}

bool ZoneHVACWaterToAirHeatPump::isMaximumCyclingRateDefaulted() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->isMaximumCyclingRateDefaulted();
}

double ZoneHVACWaterToAirHeatPump::heatPumpTimeConstant() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->heatPumpTimeConstant();
}

bool ZoneHVACWaterToAirHeatPump::isHeatPumpTimeConstantDefaulted() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->isHeatPumpTimeConstantDefaulted();
}

double ZoneHVACWaterToAirHeatPump::fractionofOnCyclePowerUse() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->fractionofOnCyclePowerUse();
}

bool ZoneHVACWaterToAirHeatPump::isFractionofOnCyclePowerUseDefaulted() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->isFractionofOnCyclePowerUseDefaulted();
}

double ZoneHVACWaterToAirHeatPump::heatPumpFanDelayTime() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->heatPumpFanDelayTime();
}

bool ZoneHVACWaterToAirHeatPump::isHeatPumpFanDelayTimeDefaulted() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->isHeatPumpFanDelayTimeDefaulted();
}

HVACComponent ZoneHVACWaterToAirHeatPump::supplementalHeatingCoil() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->supplementalHeatingCoil();
}

boost::optional<double> ZoneHVACWaterToAirHeatPump::maximumSupplyAirTemperaturefromSupplementalHeater() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->maximumSupplyAirTemperaturefromSupplementalHeater();
}

bool ZoneHVACWaterToAirHeatPump::isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized();
}

double ZoneHVACWaterToAirHeatPump::maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();
}

bool ZoneHVACWaterToAirHeatPump::isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted();
}

std::string ZoneHVACWaterToAirHeatPump::fanPlacement() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->fanPlacement();
}

bool ZoneHVACWaterToAirHeatPump::isFanPlacementDefaulted() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->isFanPlacementDefaulted();
}

boost::optional<Schedule> ZoneHVACWaterToAirHeatPump::supplyAirFanOperatingModeSchedule() const {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->supplyAirFanOperatingModeSchedule();
}

bool ZoneHVACWaterToAirHeatPump::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setAvailabilitySchedule(schedule);
}

bool ZoneHVACWaterToAirHeatPump::setSupplyAirFlowRateDuringCoolingOperation(boost::optional<double> supplyAirFlowRateDuringCoolingOperation) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setSupplyAirFlowRateDuringCoolingOperation(supplyAirFlowRateDuringCoolingOperation);
}

void ZoneHVACWaterToAirHeatPump::resetSupplyAirFlowRateDuringCoolingOperation() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->resetSupplyAirFlowRateDuringCoolingOperation();
}

void ZoneHVACWaterToAirHeatPump::autosizeSupplyAirFlowRateDuringCoolingOperation() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->autosizeSupplyAirFlowRateDuringCoolingOperation();
}

bool ZoneHVACWaterToAirHeatPump::setSupplyAirFlowRateDuringHeatingOperation(boost::optional<double> supplyAirFlowRateDuringHeatingOperation) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setSupplyAirFlowRateDuringHeatingOperation(supplyAirFlowRateDuringHeatingOperation);
}

void ZoneHVACWaterToAirHeatPump::resetSupplyAirFlowRateDuringHeatingOperation() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->resetSupplyAirFlowRateDuringHeatingOperation();
}


void ZoneHVACWaterToAirHeatPump::autosizeSupplyAirFlowRateDuringHeatingOperation() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->autosizeSupplyAirFlowRateDuringHeatingOperation();
}

bool ZoneHVACWaterToAirHeatPump::setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);
}

void ZoneHVACWaterToAirHeatPump::resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

void ZoneHVACWaterToAirHeatPump::autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

bool ZoneHVACWaterToAirHeatPump::setOutdoorAirFlowRateDuringCoolingOperation(boost::optional<double> outdoorAirFlowRateDuringCoolingOperation) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setOutdoorAirFlowRateDuringCoolingOperation(outdoorAirFlowRateDuringCoolingOperation);
}

void ZoneHVACWaterToAirHeatPump::resetOutdoorAirFlowRateDuringCoolingOperation() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->resetOutdoorAirFlowRateDuringCoolingOperation();
}

void ZoneHVACWaterToAirHeatPump::autosizeOutdoorAirFlowRateDuringCoolingOperation() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->autosizeOutdoorAirFlowRateDuringCoolingOperation();
}

bool ZoneHVACWaterToAirHeatPump::setOutdoorAirFlowRateDuringHeatingOperation(boost::optional<double> outdoorAirFlowRateDuringHeatingOperation) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setOutdoorAirFlowRateDuringHeatingOperation(outdoorAirFlowRateDuringHeatingOperation);
}

void ZoneHVACWaterToAirHeatPump::resetOutdoorAirFlowRateDuringHeatingOperation() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->resetOutdoorAirFlowRateDuringHeatingOperation();
}

void ZoneHVACWaterToAirHeatPump::autosizeOutdoorAirFlowRateDuringHeatingOperation() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->autosizeOutdoorAirFlowRateDuringHeatingOperation();
}

bool ZoneHVACWaterToAirHeatPump::setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);
}

void ZoneHVACWaterToAirHeatPump::resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

void ZoneHVACWaterToAirHeatPump::autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

bool ZoneHVACWaterToAirHeatPump::setSupplyAirFan(HVACComponent& fansOnOff) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setSupplyAirFan(fansOnOff);
}

bool ZoneHVACWaterToAirHeatPump::setHeatingCoil(HVACComponent& heatingCoilsWaterToAirHP) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setHeatingCoil(heatingCoilsWaterToAirHP);
}

bool ZoneHVACWaterToAirHeatPump::setCoolingCoil(HVACComponent& coolingCoilsWaterToAirHP) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setCoolingCoil(coolingCoilsWaterToAirHP);
}

bool ZoneHVACWaterToAirHeatPump::setMaximumCyclingRate(boost::optional<double> maximumCyclingRate) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setMaximumCyclingRate(maximumCyclingRate);
}

void ZoneHVACWaterToAirHeatPump::resetMaximumCyclingRate() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->resetMaximumCyclingRate();
}

bool ZoneHVACWaterToAirHeatPump::setHeatPumpTimeConstant(boost::optional<double> heatPumpTimeConstant) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setHeatPumpTimeConstant(heatPumpTimeConstant);
}

void ZoneHVACWaterToAirHeatPump::resetHeatPumpTimeConstant() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->resetHeatPumpTimeConstant();
}

bool ZoneHVACWaterToAirHeatPump::setFractionofOnCyclePowerUse(boost::optional<double> fractionofOnCyclePowerUse) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setFractionofOnCyclePowerUse(fractionofOnCyclePowerUse);
}

void ZoneHVACWaterToAirHeatPump::resetFractionofOnCyclePowerUse() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->resetFractionofOnCyclePowerUse();
}

bool ZoneHVACWaterToAirHeatPump::setHeatPumpFanDelayTime(boost::optional<double> heatPumpFanDelayTime) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setHeatPumpFanDelayTime(heatPumpFanDelayTime);
}

void ZoneHVACWaterToAirHeatPump::resetHeatPumpFanDelayTime() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->resetHeatPumpFanDelayTime();
}

bool ZoneHVACWaterToAirHeatPump::setSupplementalHeatingCoil(HVACComponent& heatingCoilName) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setSupplementalHeatingCoil(heatingCoilName);
}

bool ZoneHVACWaterToAirHeatPump::setMaximumSupplyAirTemperaturefromSupplementalHeater(boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setMaximumSupplyAirTemperaturefromSupplementalHeater(maximumSupplyAirTemperaturefromSupplementalHeater);
}

void ZoneHVACWaterToAirHeatPump::resetMaximumSupplyAirTemperaturefromSupplementalHeater() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->resetMaximumSupplyAirTemperaturefromSupplementalHeater();
}

void ZoneHVACWaterToAirHeatPump::autosizeMaximumSupplyAirTemperaturefromSupplementalHeater() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();
}

bool ZoneHVACWaterToAirHeatPump::setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(boost::optional<double> maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
}

void ZoneHVACWaterToAirHeatPump::resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();
}

bool ZoneHVACWaterToAirHeatPump::setFanPlacement(std::string fanPlacement) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setFanPlacement(fanPlacement);
}

void ZoneHVACWaterToAirHeatPump::resetFanPlacement() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->resetFanPlacement();
}

bool ZoneHVACWaterToAirHeatPump::setSupplyAirFanOperatingModeSchedule(Schedule& schedule) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setSupplyAirFanOperatingModeSchedule(schedule);
}

void ZoneHVACWaterToAirHeatPump::resetSupplyAirFanOperatingModeSchedule() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->resetSupplyAirFanOperatingModeSchedule();
}

/// @cond
ZoneHVACWaterToAirHeatPump::ZoneHVACWaterToAirHeatPump(boost::shared_ptr<detail::ZoneHVACWaterToAirHeatPump_Impl> impl)
  : ZoneHVACComponent(impl)
{}
/// @endcond

} // model
} // openstudio

