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

#include "ZoneHVACWaterToAirHeatPump.hpp"
#include "ZoneHVACWaterToAirHeatPump_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "WaterToAirComponent.hpp"
#include "WaterToAirComponent_Impl.hpp"
#include "FanOnOff.hpp"
#include "FanOnOff_Impl.hpp"
#include "CoilHeatingWaterToAirHeatPumpEquationFit.hpp"
#include "CoilHeatingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include "CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "CoilCoolingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include "CoilHeatingElectric.hpp"
#include "CoilHeatingElectric_Impl.hpp"
#include "CoilHeatingGas.hpp"
#include "CoilHeatingGas_Impl.hpp"
#include "CoilHeatingWater.hpp"
#include "CoilHeatingWater_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"


#include <utilities/idd/IddFactory.hxx>


#include <utilities/idd/OS_ZoneHVAC_WaterToAirHeatPump_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

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

    auto t_heatingCoil = heatingCoil();
    HVACComponent heatingCoilClone = t_heatingCoil.clone(model).cast<HVACComponent>();

    auto t_coolingCoil = coolingCoil();
    HVACComponent coolingCoilClone = t_coolingCoil.clone(model).cast<HVACComponent>();

    auto t_supplementalHeatingCoil = supplementalHeatingCoil();
    HVACComponent supplementalHeatingCoilClone = t_supplementalHeatingCoil.clone(model).cast<HVACComponent>();

    wahpClone.setSupplyAirFan(supplyFanClone);

    wahpClone.setHeatingCoil(heatingCoilClone);

    wahpClone.setCoolingCoil(coolingCoilClone);

    wahpClone.setSupplementalHeatingCoil(supplementalHeatingCoilClone);

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
      if( auto waterToAirComponent = t_supplementalHeatingCoil.optionalCast<WaterToAirComponent>() ) {
        if( auto plant = waterToAirComponent->plantLoop() ) {
          plant->addDemandBranchForComponent(supplementalHeatingCoilClone);
        }
      }
    }

    return wahpClone;
  }

  std::vector<IdfObject> ZoneHVACWaterToAirHeatPump_Impl::remove()
  {
    // This plant disconnect needs to be done because virtual ModelObject::remove
    // is not called by ParentObjects bulk children remove.  Normally WaterToAirComponent::remove
    // handles this.
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
    if( boost::optional<CoilHeatingWater> t_supplementalHeatingCoil =
          supplementalHeatingCoil().optionalCast<CoilHeatingWater>() )
    {
      if( boost::optional<PlantLoop> plantLoop = t_supplementalHeatingCoil->plantLoop() )
      {
        plantLoop->removeDemandBranchWithComponent( t_supplementalHeatingCoil.get() );
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

  unsigned ZoneHVACWaterToAirHeatPump_Impl::inletPort() const
  {
    return OS_ZoneHVAC_WaterToAirHeatPumpFields::AirInletNodeName;
  }

  unsigned ZoneHVACWaterToAirHeatPump_Impl::outletPort() const
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
  bool ZoneHVACWaterToAirHeatPump_Impl::setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation)   {
    return setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateDuringCoolingOperation, supplyAirFlowRateDuringCoolingOperation);
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
  bool ZoneHVACWaterToAirHeatPump_Impl::setSupplyAirFlowRateDuringHeatingOperation(double supplyAirFlowRateDuringHeatingOperation)   {
    return setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateDuringHeatingOperation, supplyAirFlowRateDuringHeatingOperation);
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
  bool ZoneHVACWaterToAirHeatPump_Impl::setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(double supplyAirFlowRateWhenNoCoolingorHeatingisNeeded)   {
    return setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);
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
  bool ZoneHVACWaterToAirHeatPump_Impl::setOutdoorAirFlowRateDuringCoolingOperation(double outdoorAirFlowRateDuringCoolingOperation) {
    return setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateDuringCoolingOperation, outdoorAirFlowRateDuringCoolingOperation);
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
  bool ZoneHVACWaterToAirHeatPump_Impl::setOutdoorAirFlowRateDuringHeatingOperation(double outdoorAirFlowRateDuringHeatingOperation) {
    return setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateDuringHeatingOperation, outdoorAirFlowRateDuringHeatingOperation);
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
  bool ZoneHVACWaterToAirHeatPump_Impl::setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(double outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded)  {
    return setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded);
  }


 void ZoneHVACWaterToAirHeatPump_Impl::resetOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, "");
    OS_ASSERT(result);
  }

 void ZoneHVACWaterToAirHeatPump_Impl::autosizeOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    bool result = setString(OS_ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded, "Autosize");
    OS_ASSERT(result);
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setSupplyAirFan(HVACComponent& fan) {
    return setPointer(OS_ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFanName,fan.handle());
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setHeatingCoil(HVACComponent& heatingCoilsWaterToAirHP)
  {
    return setPointer(OS_ZoneHVAC_WaterToAirHeatPumpFields::HeatingCoilName,heatingCoilsWaterToAirHP.handle());
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setCoolingCoil(HVACComponent& coolingCoilsWaterToAirHP) {
    return setPointer(OS_ZoneHVAC_WaterToAirHeatPumpFields::CoolingCoilName,coolingCoilsWaterToAirHP.handle());
  }

  bool ZoneHVACWaterToAirHeatPump_Impl::setMaximumCyclingRate(boost::optional<double> maximumCyclingRate) {
    bool result = setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::MaximumCyclingRate, maximumCyclingRate.get());
    return result;
  }
  bool ZoneHVACWaterToAirHeatPump_Impl::setMaximumCyclingRate(double maximumCyclingRate) {
    bool result = setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::MaximumCyclingRate, maximumCyclingRate);
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
  bool ZoneHVACWaterToAirHeatPump_Impl::setHeatPumpTimeConstant(double heatPumpTimeConstant) {
    bool result = setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::HeatPumpTimeConstant, heatPumpTimeConstant);
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
  bool ZoneHVACWaterToAirHeatPump_Impl::setFractionofOnCyclePowerUse(double fractionofOnCyclePowerUse) {
    bool result = setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::FractionofOnCyclePowerUse, fractionofOnCyclePowerUse);
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
  bool ZoneHVACWaterToAirHeatPump_Impl::setHeatPumpFanDelayTime(double heatPumpFanDelayTime) {
    bool result = setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::HeatPumpFanDelayTime, heatPumpFanDelayTime);
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
  bool ZoneHVACWaterToAirHeatPump_Impl::setMaximumSupplyAirTemperaturefromSupplementalHeater(double maximumSupplyAirTemperaturefromSupplementalHeater) {
    return setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::MaximumSupplyAirTemperaturefromSupplementalHeater, maximumSupplyAirTemperaturefromSupplementalHeater);
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
  bool ZoneHVACWaterToAirHeatPump_Impl::setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation) {
    bool result = setDouble(OS_ZoneHVAC_WaterToAirHeatPumpFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation, maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
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
bool ZoneHVACWaterToAirHeatPump::setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation) {
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
bool ZoneHVACWaterToAirHeatPump::setSupplyAirFlowRateDuringHeatingOperation(double supplyAirFlowRateDuringHeatingOperation) {
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
bool ZoneHVACWaterToAirHeatPump::setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(double supplyAirFlowRateWhenNoCoolingorHeatingisNeeded) {
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
bool ZoneHVACWaterToAirHeatPump::setOutdoorAirFlowRateDuringCoolingOperation(double outdoorAirFlowRateDuringCoolingOperation) {
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
bool ZoneHVACWaterToAirHeatPump::setOutdoorAirFlowRateDuringHeatingOperation(double outdoorAirFlowRateDuringHeatingOperation) {
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
bool ZoneHVACWaterToAirHeatPump::setOutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded(double outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded) {
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
bool ZoneHVACWaterToAirHeatPump::setMaximumCyclingRate(double maximumCyclingRate) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setMaximumCyclingRate(maximumCyclingRate);
}


void ZoneHVACWaterToAirHeatPump::resetMaximumCyclingRate() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->resetMaximumCyclingRate();
}

bool ZoneHVACWaterToAirHeatPump::setHeatPumpTimeConstant(boost::optional<double> heatPumpTimeConstant) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setHeatPumpTimeConstant(heatPumpTimeConstant);
}
bool ZoneHVACWaterToAirHeatPump::setHeatPumpTimeConstant(double heatPumpTimeConstant) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setHeatPumpTimeConstant(heatPumpTimeConstant);
}


void ZoneHVACWaterToAirHeatPump::resetHeatPumpTimeConstant() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->resetHeatPumpTimeConstant();
}

bool ZoneHVACWaterToAirHeatPump::setFractionofOnCyclePowerUse(boost::optional<double> fractionofOnCyclePowerUse) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setFractionofOnCyclePowerUse(fractionofOnCyclePowerUse);
}
bool ZoneHVACWaterToAirHeatPump::setFractionofOnCyclePowerUse(double fractionofOnCyclePowerUse) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setFractionofOnCyclePowerUse(fractionofOnCyclePowerUse);
}


void ZoneHVACWaterToAirHeatPump::resetFractionofOnCyclePowerUse() {
  getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->resetFractionofOnCyclePowerUse();
}

bool ZoneHVACWaterToAirHeatPump::setHeatPumpFanDelayTime(boost::optional<double> heatPumpFanDelayTime) {
  return getImpl<detail::ZoneHVACWaterToAirHeatPump_Impl>()->setHeatPumpFanDelayTime(heatPumpFanDelayTime);
}
bool ZoneHVACWaterToAirHeatPump::setHeatPumpFanDelayTime(double heatPumpFanDelayTime) {
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
bool ZoneHVACWaterToAirHeatPump::setMaximumSupplyAirTemperaturefromSupplementalHeater(double maximumSupplyAirTemperaturefromSupplementalHeater) {
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
bool ZoneHVACWaterToAirHeatPump::setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation) {
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
ZoneHVACWaterToAirHeatPump::ZoneHVACWaterToAirHeatPump(std::shared_ptr<detail::ZoneHVACWaterToAirHeatPump_Impl> impl)
  : ZoneHVACComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

