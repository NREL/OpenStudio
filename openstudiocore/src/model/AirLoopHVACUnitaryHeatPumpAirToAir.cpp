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

#include "AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "CoilHeatingDXSingleSpeed.hpp"
#include "CoilHeatingDXSingleSpeed_Impl.hpp"
#include "CoilCoolingDXSingleSpeed.hpp"
#include "CoilCoolingDXSingleSpeed_Impl.hpp"
#include "CoilHeatingElectric.hpp"
#include "CoilHeatingElectric_Impl.hpp"
#include "CoilHeatingGas.hpp"
#include "CoilHeatingGas_Impl.hpp"
#include "FanConstantVolume.hpp"
#include "FanConstantVolume_Impl.hpp"
#include "FanOnOff.hpp"
#include "FanOnOff_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  AirLoopHVACUnitaryHeatPumpAirToAir_Impl::AirLoopHVACUnitaryHeatPumpAirToAir_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : StraightComponent_Impl(idfObject,model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == AirLoopHVACUnitaryHeatPumpAirToAir::iddObjectType());
  }

  AirLoopHVACUnitaryHeatPumpAirToAir_Impl::AirLoopHVACUnitaryHeatPumpAirToAir_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == AirLoopHVACUnitaryHeatPumpAirToAir::iddObjectType());
  }

  AirLoopHVACUnitaryHeatPumpAirToAir_Impl::AirLoopHVACUnitaryHeatPumpAirToAir_Impl(const AirLoopHVACUnitaryHeatPumpAirToAir_Impl& other,
                                                                                   Model_Impl* model,
                                                                                   bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  ModelObject AirLoopHVACUnitaryHeatPumpAirToAir_Impl::clone(Model model) const
  {
    AirLoopHVACUnitaryHeatPumpAirToAir newUnitary = StraightComponent_Impl::clone(model).cast<AirLoopHVACUnitaryHeatPumpAirToAir>();

    HVACComponent newFan = this->supplyAirFan().clone(model).cast<HVACComponent>();

    HVACComponent newCoolingCoil = this->coolingCoil().clone(model).cast<HVACComponent>();

    HVACComponent newHeatingCoil = this->heatingCoil().clone(model).cast<HVACComponent>();

    HVACComponent newSupHeatingCoil = this->supplementalHeatingCoil().clone(model).cast<HVACComponent>();

    newUnitary.setCoolingCoil(newCoolingCoil);

    newUnitary.setHeatingCoil(newHeatingCoil);

    newUnitary.setSupplyAirFan(newFan);

    newUnitary.setSupplementalHeatingCoil(newSupHeatingCoil);

    return newUnitary;
  }

  const std::vector<std::string>& AirLoopHVACUnitaryHeatPumpAirToAir_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType AirLoopHVACUnitaryHeatPumpAirToAir_Impl::iddObjectType() const {
    return AirLoopHVACUnitaryHeatPumpAirToAir::iddObjectType();
  }

  std::vector<ScheduleTypeKey> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AirLoopHVACUnitaryHeatPumpAirToAir","Availability"));
    }
    if (std::find(b,e,OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFanOperatingModeScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("AirLoopHVACUnitaryHeatPumpAirToAir","Supply Air Fan Operating Mode"));
    }
    return result;
  }

  std::vector<ModelObject> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::children() const
  {
    std::vector<ModelObject> result;

    result.push_back( supplyAirFan() );
    result.push_back( coolingCoil() );
    result.push_back( heatingCoil() );
    result.push_back( supplementalHeatingCoil() );

    return result;
  }

  unsigned AirLoopHVACUnitaryHeatPumpAirToAir_Impl::inletPort()
  {
    return OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::AirInletNodeName;
  }

  unsigned AirLoopHVACUnitaryHeatPumpAirToAir_Impl::outletPort()
  {
    return OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::AirOutletNodeName;
  }

  Schedule AirLoopHVACUnitaryHeatPumpAirToAir_Impl::availabilitySchedule() const
  {
    boost::optional<Schedule> value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::AvailabilityScheduleName);
    if (!value){
      // it is an error if we get here, however we don't want to crash
      // so we hook up to global always on schedule
      LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
      value = this->model().alwaysOnDiscreteSchedule();
      OS_ASSERT(value);
      const_cast<AirLoopHVACUnitaryHeatPumpAirToAir_Impl*>(this)->setAvailabilitySchedule(*value);
      value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::AvailabilityScheduleName);
    }
    OS_ASSERT(value);
    return value.get();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::addToNode(Node & node)
  {
    if( boost::optional<AirLoopHVAC> airLoop = node.airLoopHVAC() )
    {
      if( airLoop->supplyComponent(node.handle()) )
      {
        return StraightComponent_Impl::addToNode(node);
      }
    }

    return false;
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::supplyAirFlowRateDuringCoolingOperation() const {
    return getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateDuringCoolingOperation,true);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::isSupplyAirFlowRateDuringCoolingOperationAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateDuringCoolingOperation, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::supplyAirFlowRateDuringHeatingOperation() const {
    return getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateDuringHeatingOperation,true);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::isSupplyAirFlowRateDuringHeatingOperationAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateDuringHeatingOperation, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
    return getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded,true);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  boost::optional<ThermalZone> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::controllingZone() const
  {
    boost::optional<ThermalZone> result;

    result =  getObject<ModelObject>().getModelObjectTarget<ThermalZone>(
                OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::ControllingZoneorThermostatLocation);

    return result;
  }

  HVACComponent AirLoopHVACUnitaryHeatPumpAirToAir_Impl::supplyAirFan() const
  {
    boost::optional<HVACComponent> result;

    result =  getObject<ModelObject>().getModelObjectTarget<HVACComponent>(
                OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFanName);

    OS_ASSERT(result);

    return result.get();
  }

  HVACComponent AirLoopHVACUnitaryHeatPumpAirToAir_Impl::heatingCoil() const
  {
    boost::optional<HVACComponent> result;

    result =  getObject<ModelObject>().getModelObjectTarget<HVACComponent>(
                OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::HeatingCoilName);

    OS_ASSERT(result);

    return result.get();
  }

  HVACComponent AirLoopHVACUnitaryHeatPumpAirToAir_Impl::coolingCoil() const
  {
    boost::optional<HVACComponent> result;

    result =  getObject<ModelObject>().getModelObjectTarget<HVACComponent>(
                OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::CoolingCoilName);

    OS_ASSERT(result);

    return result.get();
  }

  HVACComponent AirLoopHVACUnitaryHeatPumpAirToAir_Impl::supplementalHeatingCoil() const
  {
    boost::optional<HVACComponent> result;

    result =  getObject<ModelObject>().getModelObjectTarget<HVACComponent>(
                OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplementalHeatingCoilName);

    OS_ASSERT(result);

    return result.get();
  }

  boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::maximumSupplyAirTemperaturefromSupplementalHeater() const {
    return getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumSupplyAirTemperaturefromSupplementalHeater,true);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumSupplyAirTemperaturefromSupplementalHeater, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "Autosize");
    }
    return result;
  }

  double AirLoopHVACUnitaryHeatPumpAirToAir_Impl::maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const {
    boost::optional<double> value = getDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const {
    return isEmpty(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
  }

  std::string AirLoopHVACUnitaryHeatPumpAirToAir_Impl::fanPlacement() const {
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::FanPlacement,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::isFanPlacementDefaulted() const {
    return isEmpty(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::FanPlacement);
  }

  boost::optional<Schedule> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::supplyAirFanOperatingModeSchedule() const
  {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(
             OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFanOperatingModeScheduleName);
  }

  std::string AirLoopHVACUnitaryHeatPumpAirToAir_Impl::dehumidificationControlType() const {
    boost::optional<std::string> value = getString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::DehumidificationControlType,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::isDehumidificationControlTypeDefaulted() const {
    return isEmpty(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::DehumidificationControlType);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setAvailabilitySchedule( Schedule & schedule )
  {
    bool result = setSchedule(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::AvailabilityScheduleName,
                              "AirLoopHVACUnitaryHeatPumpAirToAir",
                              "Availability",
                              schedule);
    return result;
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setSupplyAirFlowRateDuringCoolingOperation(boost::optional<double> supplyAirFlowRateDuringCoolingOperation) {
    bool result = false;
    if (supplyAirFlowRateDuringCoolingOperation) {
      result = setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateDuringCoolingOperation, supplyAirFlowRateDuringCoolingOperation.get());
    } else {
      result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateDuringCoolingOperation, "");
    }
    return result;
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::autosizeSupplyAirFlowRateDuringCoolingOperation() {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateDuringCoolingOperation, "Autosize");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setSupplyAirFlowRateDuringHeatingOperation(boost::optional<double> n2) {
    bool result = false;
    if (n2) {
      result = setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateDuringHeatingOperation, n2.get());
    } else {
      result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateDuringHeatingOperation, "");
    }
    return result;
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::autosizeSupplyAirFlowRateDuringHeatingOperation() {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateDuringHeatingOperation, "Autosize");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(boost::optional<double> supplyAirFlowRateWhenNoCoolingorHeatingisNeeded) {
    bool result = false;
    if (supplyAirFlowRateWhenNoCoolingorHeatingisNeeded) {
      result = setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, supplyAirFlowRateWhenNoCoolingorHeatingisNeeded.get());
    } else {
      result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, "");
    }
    return result;
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, "");
    OS_ASSERT(result);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded, "Autosize");
    OS_ASSERT(result);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setControllingZone( ThermalZone & zone )
  {
    setPointer(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::ControllingZoneorThermostatLocation,zone.handle());
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::resetControllingZone()
  {
    setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::ControllingZoneorThermostatLocation,"");
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setSupplyAirFan( HVACComponent & hvacComponent )
  {
    if( ! hvacComponent.optionalCast<FanConstantVolume>() && ! hvacComponent.optionalCast<FanOnOff>() ) { return; };

    setPointer(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFanName,hvacComponent.handle());
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setHeatingCoil( HVACComponent & hvacComponent )
  {
    if( ! hvacComponent.optionalCast<CoilHeatingDXSingleSpeed>() ) { return; };

    setPointer(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::HeatingCoilName,hvacComponent.handle());
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setCoolingCoil( HVACComponent & hvacComponent )
  {
    if( ! hvacComponent.optionalCast<CoilCoolingDXSingleSpeed>() ) { return; };

    setPointer(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::CoolingCoilName,hvacComponent.handle());
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setSupplementalHeatingCoil( HVACComponent & hvacComponent )
  {
    bool isTypeOK = false;

    if( hvacComponent.optionalCast<CoilHeatingGas>() )
    {
      isTypeOK = true;
    }
    else if( hvacComponent.optionalCast<CoilHeatingElectric>() )
    {
      isTypeOK = true;
    }

    if( isTypeOK )
    {
      setPointer(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplementalHeatingCoilName,hvacComponent.handle());
    }
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setMaximumSupplyAirTemperaturefromSupplementalHeater(boost::optional<double> maximumSupplyAirTemperaturefromSupplementalHeater) {
    bool result = false;
    if (maximumSupplyAirTemperaturefromSupplementalHeater) {
      result = setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumSupplyAirTemperaturefromSupplementalHeater, maximumSupplyAirTemperaturefromSupplementalHeater.get());
    } else {
      result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumSupplyAirTemperaturefromSupplementalHeater, "");
    }
    OS_ASSERT(result);
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::autosizeMaximumSupplyAirTemperaturefromSupplementalHeater() {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumSupplyAirTemperaturefromSupplementalHeater, "Autosize");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation) {
    bool result = setDouble(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation, maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
    return result;
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setFanPlacement(std::string fanPlacement) {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::FanPlacement, fanPlacement);
    return result;
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::resetFanPlacement() {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::FanPlacement, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setSupplyAirFanOperatingModeSchedule( Schedule & schedule ) {
    bool result = setSchedule(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFanOperatingModeScheduleName,
                              "AirLoopHVACUnitaryHeatPumpAirToAir",
                              "Supply Air Fan Operating Mode",
                              schedule);
    return result;
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::resetSupplyAirFanOperatingModeSchedule() {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::SupplyAirFanOperatingModeScheduleName, "");
    OS_ASSERT(result);
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setDehumidificationControlType(std::string dehumidificationControlType) {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::DehumidificationControlType, dehumidificationControlType);
    return result;
  }

  void AirLoopHVACUnitaryHeatPumpAirToAir_Impl::resetDehumidificationControlType() {
    bool result = setString(OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::DehumidificationControlType, "");
    OS_ASSERT(result);
  }

  boost::optional<ModelObject> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::availabilityScheduleAsModelObject() const {
    OptionalModelObject result = availabilitySchedule();
    return result;
  }

  boost::optional<ModelObject> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::controllingZoneAsModelObject() const {
    OptionalModelObject result;
    OptionalThermalZone intermediate = controllingZone();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  boost::optional<ModelObject> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::supplyAirFanAsModelObject() const {
    OptionalModelObject result = supplyAirFan();
    return result;
  }

  boost::optional<ModelObject> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::heatingCoilAsModelObject() const {
    OptionalModelObject result = heatingCoil();
    return result;
  }

  boost::optional<ModelObject> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::coolingCoilAsModelObject() const {
    OptionalModelObject result = coolingCoil();
    return result;
  }

  boost::optional<ModelObject> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::supplementalHeatingCoilAsModelObject() const {
    OptionalModelObject result = supplementalHeatingCoil();
    return result;
  }

  boost::optional<ModelObject> AirLoopHVACUnitaryHeatPumpAirToAir_Impl::supplyAirFanOperatingModeScheduleAsModelObject() const {
    OptionalModelObject result;
    OptionalSchedule intermediate = supplyAirFanOperatingModeSchedule();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
      if (intermediate) {
        Schedule schedule(*intermediate);
        return setAvailabilitySchedule(schedule);
      }
    }
    return false;
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setControllingZoneAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalThermalZone intermediate = modelObject->optionalCast<ThermalZone>();
      if (intermediate) {
        setControllingZone(*intermediate);
        return true;
      }
    }
    return false;
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setSupplyAirFanAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
      if (intermediate) {
        setSupplyAirFan(*intermediate);
        return true;
      }
    }
    return false;
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
      if (intermediate) {
        setHeatingCoil(*intermediate);
        return true;
      }
    }
    return false;
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setCoolingCoilAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
      if (intermediate) {
        setCoolingCoil(*intermediate);
        return true;
      }
    }
    return false;
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setSupplementalHeatingCoilAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalHVACComponent intermediate = modelObject->optionalCast<HVACComponent>();
      if (intermediate) {
        setSupplementalHeatingCoil(*intermediate);
        return true;
      }
    }
    return false;
  }

  bool AirLoopHVACUnitaryHeatPumpAirToAir_Impl::setSupplyAirFanOperatingModeScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
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

AirLoopHVACUnitaryHeatPumpAirToAir::AirLoopHVACUnitaryHeatPumpAirToAir( const Model & model,
                                                                        Schedule & availabilitySchedule,
                                                                        HVACComponent & supplyFan,
                                                                        HVACComponent & heatingCoil,
                                                                        HVACComponent & coolingCoil,
                                                                        HVACComponent & supplementalHeatingCoil )
  : StraightComponent(AirLoopHVACUnitaryHeatPumpAirToAir::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>());

  setAvailabilitySchedule( availabilitySchedule );
  setSupplyAirFan( supplyFan );
  setHeatingCoil( heatingCoil );
  setCoolingCoil( coolingCoil );
  setSupplementalHeatingCoil( supplementalHeatingCoil );

  autosizeSupplyAirFlowRateDuringCoolingOperation();

  autosizeSupplyAirFlowRateDuringHeatingOperation();

  autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();
}

IddObjectType AirLoopHVACUnitaryHeatPumpAirToAir::iddObjectType() {
  IddObjectType result(IddObjectType::OS_AirLoopHVAC_UnitaryHeatPump_AirToAir);
  return result;
}

std::vector<std::string> AirLoopHVACUnitaryHeatPumpAirToAir::validFanPlacementValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::FanPlacement);
}

std::vector<std::string> AirLoopHVACUnitaryHeatPumpAirToAir::validDehumidificationControlTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_AirLoopHVAC_UnitaryHeatPump_AirToAirFields::DehumidificationControlType);
}

boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir::supplyAirFlowRateDuringCoolingOperation() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->supplyAirFlowRateDuringCoolingOperation();
}

bool AirLoopHVACUnitaryHeatPumpAirToAir::isSupplyAirFlowRateDuringCoolingOperationAutosized() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->isSupplyAirFlowRateDuringCoolingOperationAutosized();
}

boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir::supplyAirFlowRateDuringHeatingOperation() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->supplyAirFlowRateDuringHeatingOperation();
}

bool AirLoopHVACUnitaryHeatPumpAirToAir::isSupplyAirFlowRateDuringHeatingOperationAutosized() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->isSupplyAirFlowRateDuringHeatingOperationAutosized();
}

boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir::supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->supplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

bool AirLoopHVACUnitaryHeatPumpAirToAir::isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized();
}

boost::optional<double> AirLoopHVACUnitaryHeatPumpAirToAir::maximumSupplyAirTemperaturefromSupplementalHeater() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->maximumSupplyAirTemperaturefromSupplementalHeater();
}

bool AirLoopHVACUnitaryHeatPumpAirToAir::isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized();
}

double AirLoopHVACUnitaryHeatPumpAirToAir::maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();
}

bool AirLoopHVACUnitaryHeatPumpAirToAir::isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->isMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperationDefaulted();
}

std::string AirLoopHVACUnitaryHeatPumpAirToAir::fanPlacement() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->fanPlacement();
}

bool AirLoopHVACUnitaryHeatPumpAirToAir::isFanPlacementDefaulted() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->isFanPlacementDefaulted();
}

std::string AirLoopHVACUnitaryHeatPumpAirToAir::dehumidificationControlType() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->dehumidificationControlType();
}

bool AirLoopHVACUnitaryHeatPumpAirToAir::isDehumidificationControlTypeDefaulted() const {
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->isDehumidificationControlTypeDefaulted();
}

bool AirLoopHVACUnitaryHeatPumpAirToAir::setSupplyAirFlowRateDuringCoolingOperation(double supplyAirFlowRateDuringCoolingOperation) {
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setSupplyAirFlowRateDuringCoolingOperation(supplyAirFlowRateDuringCoolingOperation);
}

void AirLoopHVACUnitaryHeatPumpAirToAir::autosizeSupplyAirFlowRateDuringCoolingOperation() {
  getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->autosizeSupplyAirFlowRateDuringCoolingOperation();
}

bool AirLoopHVACUnitaryHeatPumpAirToAir::setSupplyAirFlowRateDuringHeatingOperation(double n2) {
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setSupplyAirFlowRateDuringHeatingOperation(n2);
}

void AirLoopHVACUnitaryHeatPumpAirToAir::autosizeSupplyAirFlowRateDuringHeatingOperation() {
  getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->autosizeSupplyAirFlowRateDuringHeatingOperation();
}

bool AirLoopHVACUnitaryHeatPumpAirToAir::setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(double supplyAirFlowRateWhenNoCoolingorHeatingisNeeded) {
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded(supplyAirFlowRateWhenNoCoolingorHeatingisNeeded);
}

void AirLoopHVACUnitaryHeatPumpAirToAir::resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
  getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->resetSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

void AirLoopHVACUnitaryHeatPumpAirToAir::autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded() {
  getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->autosizeSupplyAirFlowRateWhenNoCoolingorHeatingisNeeded();
}

void AirLoopHVACUnitaryHeatPumpAirToAir::setMaximumSupplyAirTemperaturefromSupplementalHeater(double maximumSupplyAirTemperaturefromSupplementalHeater) {
  getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setMaximumSupplyAirTemperaturefromSupplementalHeater(maximumSupplyAirTemperaturefromSupplementalHeater);
}

void AirLoopHVACUnitaryHeatPumpAirToAir::autosizeMaximumSupplyAirTemperaturefromSupplementalHeater() {
  getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->autosizeMaximumSupplyAirTemperaturefromSupplementalHeater();
}

bool AirLoopHVACUnitaryHeatPumpAirToAir::setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(double maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation) {
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation(maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation);
}

void AirLoopHVACUnitaryHeatPumpAirToAir::resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() {
  getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->resetMaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();
}

bool AirLoopHVACUnitaryHeatPumpAirToAir::setFanPlacement(std::string fanPlacement) {
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setFanPlacement(fanPlacement);
}

void AirLoopHVACUnitaryHeatPumpAirToAir::resetFanPlacement() {
  getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->resetFanPlacement();
}

void AirLoopHVACUnitaryHeatPumpAirToAir::setSupplyAirFanOperatingModeSchedule( Schedule & schedule )
{
  getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setSupplyAirFanOperatingModeSchedule(schedule);
}

void AirLoopHVACUnitaryHeatPumpAirToAir::resetSupplyAirFanOperatingModeSchedule() {
  getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->resetSupplyAirFanOperatingModeSchedule();
}

bool AirLoopHVACUnitaryHeatPumpAirToAir::setDehumidificationControlType(std::string dehumidificationControlType) {
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setDehumidificationControlType(dehumidificationControlType);
}

void AirLoopHVACUnitaryHeatPumpAirToAir::resetDehumidificationControlType() {
  getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->resetDehumidificationControlType();
}

bool AirLoopHVACUnitaryHeatPumpAirToAir::setAvailabilitySchedule( Schedule & schedule )
{
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setAvailabilitySchedule(schedule);
}

void AirLoopHVACUnitaryHeatPumpAirToAir::setControllingZone( ThermalZone & zone )
{
  getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setControllingZone(zone);
}

void AirLoopHVACUnitaryHeatPumpAirToAir::resetControllingZone()
{
  getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->resetControllingZone();
}

void AirLoopHVACUnitaryHeatPumpAirToAir::setSupplyAirFan( HVACComponent & hvacComponent )
{
  getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setSupplyAirFan(hvacComponent);
}

void AirLoopHVACUnitaryHeatPumpAirToAir::setHeatingCoil( HVACComponent & hvacComponent )
{
  getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setHeatingCoil(hvacComponent);
}

void AirLoopHVACUnitaryHeatPumpAirToAir::setCoolingCoil( HVACComponent & hvacComponent )
{
  getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setCoolingCoil(hvacComponent);
}

void AirLoopHVACUnitaryHeatPumpAirToAir::setSupplementalHeatingCoil( HVACComponent & hvacComponent )
{
  getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->setSupplementalHeatingCoil(hvacComponent);
}

boost::optional<Schedule> AirLoopHVACUnitaryHeatPumpAirToAir::supplyAirFanOperatingModeSchedule() const
{
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->supplyAirFanOperatingModeSchedule();
}

Schedule AirLoopHVACUnitaryHeatPumpAirToAir::availabilitySchedule() const
{
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->availabilitySchedule();
}

boost::optional<ThermalZone> AirLoopHVACUnitaryHeatPumpAirToAir::controllingZone() const
{
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->controllingZone();
}

HVACComponent AirLoopHVACUnitaryHeatPumpAirToAir::supplyAirFan() const
{
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->supplyAirFan();
}

HVACComponent AirLoopHVACUnitaryHeatPumpAirToAir::heatingCoil() const
{
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->heatingCoil();
}

HVACComponent AirLoopHVACUnitaryHeatPumpAirToAir::coolingCoil() const
{
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->coolingCoil();
}

HVACComponent AirLoopHVACUnitaryHeatPumpAirToAir::supplementalHeatingCoil() const
{
  return getImpl<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl>()->supplementalHeatingCoil();
}

/// @cond
AirLoopHVACUnitaryHeatPumpAirToAir::AirLoopHVACUnitaryHeatPumpAirToAir(std::shared_ptr<detail::AirLoopHVACUnitaryHeatPumpAirToAir_Impl> impl)
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

