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

#include "WaterHeaterHeatPumpWrappedCondenser.hpp"
#include "WaterHeaterHeatPumpWrappedCondenser_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "FanOnOff.hpp"
#include "FanOnOff_Impl.hpp"
#include "WaterToWaterComponent.hpp"
#include "WaterToWaterComponent_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "WaterHeaterStratified.hpp"
#include "WaterHeaterStratified_Impl.hpp"
#include "ScheduleRuleset.hpp"
#include "ScheduleRuleset_Impl.hpp"
#include "ScheduleDay.hpp"
#include "ScheduleDay_Impl.hpp"
#include "CoilWaterHeatingAirToWaterHeatPumpWrapped.hpp"
#include "CoilWaterHeatingAirToWaterHeatPumpWrapped_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_WaterHeater_HeatPump_WrappedCondenser_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  WaterHeaterHeatPumpWrappedCondenser_Impl::WaterHeaterHeatPumpWrappedCondenser_Impl(const IdfObject& idfObject,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == WaterHeaterHeatPumpWrappedCondenser::iddObjectType());
  }

  WaterHeaterHeatPumpWrappedCondenser_Impl::WaterHeaterHeatPumpWrappedCondenser_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == WaterHeaterHeatPumpWrappedCondenser::iddObjectType());
  }

  WaterHeaterHeatPumpWrappedCondenser_Impl::WaterHeaterHeatPumpWrappedCondenser_Impl(const WaterHeaterHeatPumpWrappedCondenser_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& WaterHeaterHeatPumpWrappedCondenser_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType WaterHeaterHeatPumpWrappedCondenser_Impl::iddObjectType() const {
    return WaterHeaterHeatPumpWrappedCondenser::iddObjectType();
  }

  std::vector<ScheduleTypeKey> WaterHeaterHeatPumpWrappedCondenser_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_WaterHeater_HeatPump_WrappedCondenserFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("WaterHeaterHeatPumpWrappedCondenser","Availability"));
    }
    if (std::find(b,e,OS_WaterHeater_HeatPump_WrappedCondenserFields::CompressorSetpointTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("WaterHeaterHeatPumpWrappedCondenser","Compressor Setpoint Temperature"));
    }
    if (std::find(b,e,OS_WaterHeater_HeatPump_WrappedCondenserFields::InletAirTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("WaterHeaterHeatPumpWrappedCondenser","Inlet Air Temperature"));
    }
    if (std::find(b,e,OS_WaterHeater_HeatPump_WrappedCondenserFields::InletAirHumidityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("WaterHeaterHeatPumpWrappedCondenser","Inlet Air Humidity"));
    }
    if (std::find(b,e,OS_WaterHeater_HeatPump_WrappedCondenserFields::CompressorAmbientTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("WaterHeaterHeatPumpWrappedCondenser","Compressor Ambient Temperature"));
    }
    if (std::find(b,e,OS_WaterHeater_HeatPump_WrappedCondenserFields::InletAirMixerScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("WaterHeaterHeatPumpWrappedCondenser","Inlet Air Mixer"));
    }
    return result;
  }

  boost::optional<Schedule> WaterHeaterHeatPumpWrappedCondenser_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_HeatPump_WrappedCondenserFields::AvailabilityScheduleName);
  }

  Schedule WaterHeaterHeatPumpWrappedCondenser_Impl::compressorSetpointTemperatureSchedule() const {
    boost::optional<Schedule> value = optionalCompressorSetpointTemperatureSchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Compressor Setpoint Temperature Schedule attached.");
    }
    return value.get();
  }

  double WaterHeaterHeatPumpWrappedCondenser_Impl::deadBandTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_HeatPump_WrappedCondenserFields::DeadBandTemperatureDifference,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterHeatPumpWrappedCondenser_Impl::condenserBottomLocation() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_HeatPump_WrappedCondenserFields::CondenserBottomLocation,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterHeatPumpWrappedCondenser_Impl::condenserTopLocation() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_HeatPump_WrappedCondenserFields::CondenserTopLocation,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> WaterHeaterHeatPumpWrappedCondenser_Impl::evaporatorAirFlowRate() const {
    return getDouble(OS_WaterHeater_HeatPump_WrappedCondenserFields::EvaporatorAirFlowRate,true);
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::isEvaporatorAirFlowRateAutocalculated() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_WaterHeater_HeatPump_WrappedCondenserFields::EvaporatorAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autocalculate");
    }
    return result;
  }

  std::string WaterHeaterHeatPumpWrappedCondenser_Impl::inletAirConfiguration() const {
    boost::optional<std::string> value = getString(OS_WaterHeater_HeatPump_WrappedCondenserFields::InletAirConfiguration,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> WaterHeaterHeatPumpWrappedCondenser_Impl::inletAirTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_HeatPump_WrappedCondenserFields::InletAirTemperatureScheduleName);
  }

  boost::optional<Schedule> WaterHeaterHeatPumpWrappedCondenser_Impl::inletAirHumiditySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_HeatPump_WrappedCondenserFields::InletAirHumidityScheduleName);
  }

  HVACComponent WaterHeaterHeatPumpWrappedCondenser_Impl::tank() const {
    boost::optional<HVACComponent> value = optionalTank();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Tank attached.");
    }
    return value.get();
  }

  ModelObject WaterHeaterHeatPumpWrappedCondenser_Impl::dXCoil() const {
    boost::optional<ModelObject> value = optionalDXCoil();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an DXCoil attached.");
    }
    return value.get();
  }

  double WaterHeaterHeatPumpWrappedCondenser_Impl::minimumInletAirTemperatureforCompressorOperation() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_HeatPump_WrappedCondenserFields::MinimumInletAirTemperatureforCompressorOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterHeatPumpWrappedCondenser_Impl::maximumInletAirTemperatureforCompressorOperation() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_HeatPump_WrappedCondenserFields::MaximumInletAirTemperatureforCompressorOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string WaterHeaterHeatPumpWrappedCondenser_Impl::compressorLocation() const {
    boost::optional<std::string> value = getString(OS_WaterHeater_HeatPump_WrappedCondenserFields::CompressorLocation,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> WaterHeaterHeatPumpWrappedCondenser_Impl::compressorAmbientTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_HeatPump_WrappedCondenserFields::CompressorAmbientTemperatureScheduleName);
  }

  HVACComponent WaterHeaterHeatPumpWrappedCondenser_Impl::fan() const {
    boost::optional<HVACComponent> value = optionalFan();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Fan attached.");
    }
    return value.get();
  }

  std::string WaterHeaterHeatPumpWrappedCondenser_Impl::fanPlacement() const {
    boost::optional<std::string> value = getString(OS_WaterHeater_HeatPump_WrappedCondenserFields::FanPlacement,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterHeatPumpWrappedCondenser_Impl::onCycleParasiticElectricLoad() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_HeatPump_WrappedCondenserFields::OnCycleParasiticElectricLoad,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterHeatPumpWrappedCondenser_Impl::offCycleParasiticElectricLoad() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_HeatPump_WrappedCondenserFields::OffCycleParasiticElectricLoad,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string WaterHeaterHeatPumpWrappedCondenser_Impl::parasiticHeatRejectionLocation() const {
    boost::optional<std::string> value = getString(OS_WaterHeater_HeatPump_WrappedCondenserFields::ParasiticHeatRejectionLocation,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> WaterHeaterHeatPumpWrappedCondenser_Impl::inletAirMixerSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_HeatPump_WrappedCondenserFields::InletAirMixerScheduleName);
  }

  std::string WaterHeaterHeatPumpWrappedCondenser_Impl::tankElementControlLogic() const {
    boost::optional<std::string> value = getString(OS_WaterHeater_HeatPump_WrappedCondenserFields::TankElementControlLogic,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> WaterHeaterHeatPumpWrappedCondenser_Impl::controlSensor1HeightInStratifiedTank() const {
    return getDouble(OS_WaterHeater_HeatPump_WrappedCondenserFields::ControlSensor1HeightInStratifiedTank,true);
  }

  double WaterHeaterHeatPumpWrappedCondenser_Impl::controlSensor1Weight() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_HeatPump_WrappedCondenserFields::ControlSensor1Weight,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> WaterHeaterHeatPumpWrappedCondenser_Impl::controlSensor2HeightInStratifiedTank() const {
    return getDouble(OS_WaterHeater_HeatPump_WrappedCondenserFields::ControlSensor2HeightInStratifiedTank,true);
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_WaterHeater_HeatPump_WrappedCondenserFields::AvailabilityScheduleName,
                              "WaterHeaterHeatPumpWrappedCondenser",
                              "Availability",
                              schedule);
    return result;
  }

  void WaterHeaterHeatPumpWrappedCondenser_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_WaterHeater_HeatPump_WrappedCondenserFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setCompressorSetpointTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_WaterHeater_HeatPump_WrappedCondenserFields::CompressorSetpointTemperatureScheduleName,
                              "WaterHeaterHeatPumpWrappedCondenser",
                              "Compressor Setpoint Temperature",
                              schedule);
    return result;
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setDeadBandTemperatureDifference(double deadBandTemperatureDifference) {
    bool result = setDouble(OS_WaterHeater_HeatPump_WrappedCondenserFields::DeadBandTemperatureDifference, deadBandTemperatureDifference);
    return result;
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setCondenserBottomLocation(double condenserBottomLocation) {
    bool result = setDouble(OS_WaterHeater_HeatPump_WrappedCondenserFields::CondenserBottomLocation, condenserBottomLocation);
    return result;
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setCondenserTopLocation(double condenserTopLocation) {
    bool result = setDouble(OS_WaterHeater_HeatPump_WrappedCondenserFields::CondenserTopLocation, condenserTopLocation);
    return result;
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setEvaporatorAirFlowRate(boost::optional<double> evaporatorAirFlowRate) {
    bool result(false);
    if (evaporatorAirFlowRate) {
      result = setDouble(OS_WaterHeater_HeatPump_WrappedCondenserFields::EvaporatorAirFlowRate, evaporatorAirFlowRate.get());
    }
    return result;
  }

  void WaterHeaterHeatPumpWrappedCondenser_Impl::autocalculateEvaporatorAirFlowRate() {
    bool result = setString(OS_WaterHeater_HeatPump_WrappedCondenserFields::EvaporatorAirFlowRate, "autocalculate");
    OS_ASSERT(result);
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setInletAirConfiguration(std::string inletAirConfiguration) {
    bool result = setString(OS_WaterHeater_HeatPump_WrappedCondenserFields::InletAirConfiguration, inletAirConfiguration);
    return result;
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setInletAirTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_WaterHeater_HeatPump_WrappedCondenserFields::InletAirTemperatureScheduleName,
                              "WaterHeaterHeatPumpWrappedCondenser",
                              "Inlet Air Temperature",
                              schedule);
    return result;
  }

  void WaterHeaterHeatPumpWrappedCondenser_Impl::resetInletAirTemperatureSchedule() {
    bool result = setString(OS_WaterHeater_HeatPump_WrappedCondenserFields::InletAirTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setInletAirHumiditySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_WaterHeater_HeatPump_WrappedCondenserFields::InletAirHumidityScheduleName,
                              "WaterHeaterHeatPumpWrappedCondenser",
                              "Inlet Air Humidity",
                              schedule);
    return result;
  }

  void WaterHeaterHeatPumpWrappedCondenser_Impl::resetInletAirHumiditySchedule() {
    bool result = setString(OS_WaterHeater_HeatPump_WrappedCondenserFields::InletAirHumidityScheduleName, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setTank(const HVACComponent& waterHeater) {
    bool result = setPointer(OS_WaterHeater_HeatPump_WrappedCondenserFields::TankName, waterHeater.handle());
    return result;
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setDXCoil(const ModelObject& coil) {
    bool result = setPointer(OS_WaterHeater_HeatPump_WrappedCondenserFields::DXCoilName, coil.handle());
    return result;
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setMinimumInletAirTemperatureforCompressorOperation(double minimumInletAirTemperatureforCompressorOperation) {
    bool result = setDouble(OS_WaterHeater_HeatPump_WrappedCondenserFields::MinimumInletAirTemperatureforCompressorOperation, minimumInletAirTemperatureforCompressorOperation);
    return result;
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setMaximumInletAirTemperatureforCompressorOperation(double maximumInletAirTemperatureforCompressorOperation) {
    bool result = setDouble(OS_WaterHeater_HeatPump_WrappedCondenserFields::MaximumInletAirTemperatureforCompressorOperation, maximumInletAirTemperatureforCompressorOperation);
    return result;
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setCompressorLocation(std::string compressorLocation) {
    bool result = setString(OS_WaterHeater_HeatPump_WrappedCondenserFields::CompressorLocation, compressorLocation);
    return result;
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setCompressorAmbientTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_WaterHeater_HeatPump_WrappedCondenserFields::CompressorAmbientTemperatureScheduleName,
                              "WaterHeaterHeatPumpWrappedCondenser",
                              "Compressor Ambient Temperature",
                              schedule);
    return result;
  }

  void WaterHeaterHeatPumpWrappedCondenser_Impl::resetCompressorAmbientTemperatureSchedule() {
    bool result = setString(OS_WaterHeater_HeatPump_WrappedCondenserFields::CompressorAmbientTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setFan(const HVACComponent& fan) {
    bool result = setPointer(OS_WaterHeater_HeatPump_WrappedCondenserFields::FanName, fan.handle());
    return result;
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setFanPlacement(std::string fanPlacement) {
    bool result = setString(OS_WaterHeater_HeatPump_WrappedCondenserFields::FanPlacement, fanPlacement);
    return result;
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setOnCycleParasiticElectricLoad(double onCycleParasiticElectricLoad) {
    bool result = setDouble(OS_WaterHeater_HeatPump_WrappedCondenserFields::OnCycleParasiticElectricLoad, onCycleParasiticElectricLoad);
    return result;
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad) {
    bool result = setDouble(OS_WaterHeater_HeatPump_WrappedCondenserFields::OffCycleParasiticElectricLoad, offCycleParasiticElectricLoad);
    return result;
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setParasiticHeatRejectionLocation(std::string parasiticHeatRejectionLocation) {
    bool result = setString(OS_WaterHeater_HeatPump_WrappedCondenserFields::ParasiticHeatRejectionLocation, parasiticHeatRejectionLocation);
    return result;
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setInletAirMixerSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_WaterHeater_HeatPump_WrappedCondenserFields::InletAirMixerScheduleName,
                              "WaterHeaterHeatPumpWrappedCondenser",
                              "Inlet Air Mixer",
                              schedule);
    return result;
  }

  void WaterHeaterHeatPumpWrappedCondenser_Impl::resetInletAirMixerSchedule() {
    bool result = setString(OS_WaterHeater_HeatPump_WrappedCondenserFields::InletAirMixerScheduleName, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setTankElementControlLogic(std::string tankElementControlLogic) {
    bool result = setString(OS_WaterHeater_HeatPump_WrappedCondenserFields::TankElementControlLogic, tankElementControlLogic);
    return result;
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setControlSensor1HeightInStratifiedTank(boost::optional<double> controlSensor1HeightInStratifiedTank) {
    bool result(false);
    if (controlSensor1HeightInStratifiedTank) {
      result = setDouble(OS_WaterHeater_HeatPump_WrappedCondenserFields::ControlSensor1HeightInStratifiedTank, controlSensor1HeightInStratifiedTank.get());
    }
    else {
      resetControlSensor1HeightInStratifiedTank();
      result = true;
    }
    return result;
  }

  void WaterHeaterHeatPumpWrappedCondenser_Impl::resetControlSensor1HeightInStratifiedTank() {
    bool result = setString(OS_WaterHeater_HeatPump_WrappedCondenserFields::ControlSensor1HeightInStratifiedTank, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setControlSensor1Weight(double controlSensor1Weight) {
    bool result = setDouble(OS_WaterHeater_HeatPump_WrappedCondenserFields::ControlSensor1Weight, controlSensor1Weight);
    return result;
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::setControlSensor2HeightInStratifiedTank(boost::optional<double> controlSensor2HeightInStratifiedTank) {
    bool result(false);
    if (controlSensor2HeightInStratifiedTank) {
      result = setDouble(OS_WaterHeater_HeatPump_WrappedCondenserFields::ControlSensor2HeightInStratifiedTank, controlSensor2HeightInStratifiedTank.get());
    }
    else {
      resetControlSensor2HeightInStratifiedTank();
      result = true;
    }
    return result;
  }

  void WaterHeaterHeatPumpWrappedCondenser_Impl::resetControlSensor2HeightInStratifiedTank() {
    bool result = setString(OS_WaterHeater_HeatPump_WrappedCondenserFields::ControlSensor2HeightInStratifiedTank, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> WaterHeaterHeatPumpWrappedCondenser_Impl::optionalCompressorSetpointTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_HeatPump_WrappedCondenserFields::CompressorSetpointTemperatureScheduleName);
  }

  boost::optional<HVACComponent> WaterHeaterHeatPumpWrappedCondenser_Impl::optionalTank() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_WaterHeater_HeatPump_WrappedCondenserFields::TankName);
  }

  boost::optional<ModelObject> WaterHeaterHeatPumpWrappedCondenser_Impl::optionalDXCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObject>(OS_WaterHeater_HeatPump_WrappedCondenserFields::DXCoilName);
  }

  boost::optional<HVACComponent> WaterHeaterHeatPumpWrappedCondenser_Impl::optionalFan() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_WaterHeater_HeatPump_WrappedCondenserFields::FanName);
  }

  unsigned WaterHeaterHeatPumpWrappedCondenser_Impl::inletPort() const {
    return OS_WaterHeater_HeatPump_WrappedCondenserFields::AirInletNodeName;
  }

  unsigned WaterHeaterHeatPumpWrappedCondenser_Impl::outletPort() const {
    return OS_WaterHeater_HeatPump_WrappedCondenserFields::AirOutletNodeName;
  }

  ModelObject WaterHeaterHeatPumpWrappedCondenser_Impl::clone(Model model) const
  {
    std::cout << "boom 1" << std::endl;
    auto newWaterHeater = ZoneHVACComponent_Impl::clone(model).cast<WaterHeaterHeatPumpWrappedCondenser>();

    std::cout << "boom 2" << std::endl;
    {
      auto mo = tank().clone(model).cast<HVACComponent>();
      newWaterHeater.setTank(mo);
    }

    std::cout << "boom 3" << std::endl;
    {
      auto mo = dXCoil().clone(model).cast<ModelObject>();
      newWaterHeater.setDXCoil(mo);
    }

    std::cout << "boom 4" << std::endl;
    {
      auto mo = fan().clone(model).cast<HVACComponent>();
      newWaterHeater.setFan(mo);
    }

    return newWaterHeater;
  }

  std::vector<IdfObject> WaterHeaterHeatPumpWrappedCondenser_Impl::remove()
  {
    auto t_tank = tank();
    if( auto waterToWaterTank = t_tank.optionalCast<WaterToWaterComponent>() ) {
      waterToWaterTank->removeFromPlantLoop();
      waterToWaterTank->removeFromSecondaryPlantLoop();
    } else {
      // All tanks are WaterToWaterComponent at this time, but the api says they could be any HVACComponent,
      // so this is a little dangerous. Consider enhanced APIs to remove HVACComponent from system.
      // Something we currently don't have.
      // Ideally remove would just take care of all of this, but the way ParentObject::remove works out children remove methods
      // aren't being called cleanly.
      LOG_AND_THROW("Unsupported tank " << t_tank.briefDescription() << " attached to WaterHeaterHeatPump " << briefDescription());
    }

    return ZoneHVACComponent_Impl::remove();
  }

  std::vector<ModelObject> WaterHeaterHeatPumpWrappedCondenser_Impl::children() const
  {
    std::vector<ModelObject> result;

    result.push_back(tank());
    result.push_back(dXCoil());
    result.push_back(fan());

    return result;
  }

  bool WaterHeaterHeatPumpWrappedCondenser_Impl::addToThermalZone(ThermalZone & thermalZone)
  {
    bool result = false;
    auto thisObject = getObject<WaterHeaterHeatPumpWrappedCondenser>();

    if( (result = ZoneHVACComponent_Impl::addToThermalZone(thermalZone)) ) {
      thermalZone.setHeatingPriority(thisObject,1);
      thermalZone.setCoolingPriority(thisObject,1);
      setCompressorLocation("Zone");
      setInletAirConfiguration("ZoneAirOnly");
      auto t_tank = tank();
      if( auto waterHeaterStratified = t_tank.optionalCast<WaterHeaterStratified>() ) {
        waterHeaterStratified->setAmbientTemperatureIndicator("ThermalZone");
        waterHeaterStratified->setAmbientTemperatureThermalZone(thermalZone);
      }
    }

    return result;
  }

} // detail

WaterHeaterHeatPumpWrappedCondenser::WaterHeaterHeatPumpWrappedCondenser(const Model& model)
  : ZoneHVACComponent(WaterHeaterHeatPumpWrappedCondenser::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>());

  CoilWaterHeatingAirToWaterHeatPumpWrapped coil(model);
  setDXCoil(coil);

  WaterHeaterStratified waterHeater(model);
  setTank(waterHeater);

  FanOnOff fan(model);
  setFan(fan);

  {
    auto schedule = model.alwaysOnDiscreteSchedule();
    setAvailabilitySchedule(schedule);
  }

  {
    ScheduleRuleset schedule(model);
    schedule.defaultDaySchedule().addValue(Time(0,24,0,0),60.0);
    setCompressorSetpointTemperatureSchedule(schedule);
  }

  {
    ScheduleRuleset schedule(model);
    schedule.defaultDaySchedule().addValue(Time(0,24,0,0),19.7);
    setInletAirTemperatureSchedule(schedule);
  }

  {
    ScheduleRuleset schedule(model);
    schedule.defaultDaySchedule().addValue(Time(0,24,0,0),0.5);
    setInletAirHumiditySchedule(schedule);
  }

  {
    ScheduleRuleset schedule(model);
    schedule.defaultDaySchedule().addValue(Time(0,24,0,0),21.0);
    setCompressorAmbientTemperatureSchedule(schedule);
  }

  setDeadBandTemperatureDifference(3.89);
  setCondenserBottomLocation(0.0664166667);
  setCondenserTopLocation(0.8634166667);
  setEvaporatorAirFlowRate(0.2279);
  setInletAirConfiguration("Schedule");
  setMinimumInletAirTemperatureforCompressorOperation(7.2);
  setMaximumInletAirTemperatureforCompressorOperation(48.89);
  setCompressorLocation("Outdoors");
  setFanPlacement("DrawThrough");
  setOnCycleParasiticElectricLoad(0.0);
  setOffCycleParasiticElectricLoad(0.0);
  setParasiticHeatRejectionLocation("Outdoors");
  setTankElementControlLogic("MutuallyExclusive");
  setControlSensor1HeightInStratifiedTank(1.262);
  setControlSensor1Weight(0.75);
  setControlSensor2HeightInStratifiedTank(0.464);
}

WaterHeaterHeatPumpWrappedCondenser::WaterHeaterHeatPumpWrappedCondenser(const Model& model,
  const ModelObject & dxCoil,
  const HVACComponent & tank,
  const HVACComponent & fan,
  Schedule & compressorSetpointTemperatureSchedule,
  Schedule & inletAirMixerSchedule)
  : ZoneHVACComponent(WaterHeaterHeatPumpWrappedCondenser::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>());

  setDXCoil(dxCoil);
  setTank(tank);
  setFan(fan);
  setCompressorSetpointTemperatureSchedule(compressorSetpointTemperatureSchedule);
  setInletAirMixerSchedule(inletAirMixerSchedule);

  setDeadBandTemperatureDifference(3.89);
  setCondenserBottomLocation(0.0664166667);
  setCondenserTopLocation(0.8634166667);
  setEvaporatorAirFlowRate(0.2279);
  setInletAirConfiguration("Schedule");
  setMinimumInletAirTemperatureforCompressorOperation(7.2);
  setMaximumInletAirTemperatureforCompressorOperation(48.89);
  setCompressorLocation("Outdoors");
  setFanPlacement("DrawThrough");
  setOnCycleParasiticElectricLoad(0.0);
  setOffCycleParasiticElectricLoad(0.0);
  setParasiticHeatRejectionLocation("Outdoors");
  setTankElementControlLogic("MutuallyExclusive");
  setControlSensor1HeightInStratifiedTank(1.262);
  setControlSensor1Weight(0.75);
  setControlSensor2HeightInStratifiedTank(0.464);
}

IddObjectType WaterHeaterHeatPumpWrappedCondenser::iddObjectType() {
  return IddObjectType(IddObjectType::OS_WaterHeater_HeatPump_WrappedCondenser);
}

std::vector<std::string> WaterHeaterHeatPumpWrappedCondenser::inletAirConfigurationValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WaterHeater_HeatPump_WrappedCondenserFields::InletAirConfiguration);
}

std::vector<std::string> WaterHeaterHeatPumpWrappedCondenser::compressorLocationValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WaterHeater_HeatPump_WrappedCondenserFields::CompressorLocation);
}

std::vector<std::string> WaterHeaterHeatPumpWrappedCondenser::fanPlacementValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WaterHeater_HeatPump_WrappedCondenserFields::FanPlacement);
}

std::vector<std::string> WaterHeaterHeatPumpWrappedCondenser::parasiticHeatRejectionLocationValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WaterHeater_HeatPump_WrappedCondenserFields::ParasiticHeatRejectionLocation);
}

std::vector<std::string> WaterHeaterHeatPumpWrappedCondenser::tankElementControlLogicValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WaterHeater_HeatPump_WrappedCondenserFields::TankElementControlLogic);
}

boost::optional<Schedule> WaterHeaterHeatPumpWrappedCondenser::availabilitySchedule() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->availabilitySchedule();
}

Schedule WaterHeaterHeatPumpWrappedCondenser::compressorSetpointTemperatureSchedule() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->compressorSetpointTemperatureSchedule();
}

double WaterHeaterHeatPumpWrappedCondenser::deadBandTemperatureDifference() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->deadBandTemperatureDifference();
}

double WaterHeaterHeatPumpWrappedCondenser::condenserBottomLocation() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->condenserBottomLocation();
}

double WaterHeaterHeatPumpWrappedCondenser::condenserTopLocation() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->condenserTopLocation();
}

boost::optional<double> WaterHeaterHeatPumpWrappedCondenser::evaporatorAirFlowRate() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->evaporatorAirFlowRate();
}

bool WaterHeaterHeatPumpWrappedCondenser::isEvaporatorAirFlowRateAutocalculated() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->isEvaporatorAirFlowRateAutocalculated();
}

std::string WaterHeaterHeatPumpWrappedCondenser::inletAirConfiguration() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->inletAirConfiguration();
}

boost::optional<Schedule> WaterHeaterHeatPumpWrappedCondenser::inletAirTemperatureSchedule() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->inletAirTemperatureSchedule();
}

boost::optional<Schedule> WaterHeaterHeatPumpWrappedCondenser::inletAirHumiditySchedule() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->inletAirHumiditySchedule();
}

HVACComponent WaterHeaterHeatPumpWrappedCondenser::tank() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->tank();
}

ModelObject WaterHeaterHeatPumpWrappedCondenser::dXCoil() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->dXCoil();
}

double WaterHeaterHeatPumpWrappedCondenser::minimumInletAirTemperatureforCompressorOperation() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->minimumInletAirTemperatureforCompressorOperation();
}

double WaterHeaterHeatPumpWrappedCondenser::maximumInletAirTemperatureforCompressorOperation() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->maximumInletAirTemperatureforCompressorOperation();
}

std::string WaterHeaterHeatPumpWrappedCondenser::compressorLocation() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->compressorLocation();
}

boost::optional<Schedule> WaterHeaterHeatPumpWrappedCondenser::compressorAmbientTemperatureSchedule() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->compressorAmbientTemperatureSchedule();
}

HVACComponent WaterHeaterHeatPumpWrappedCondenser::fan() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->fan();
}

std::string WaterHeaterHeatPumpWrappedCondenser::fanPlacement() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->fanPlacement();
}

double WaterHeaterHeatPumpWrappedCondenser::onCycleParasiticElectricLoad() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->onCycleParasiticElectricLoad();
}

double WaterHeaterHeatPumpWrappedCondenser::offCycleParasiticElectricLoad() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->offCycleParasiticElectricLoad();
}

std::string WaterHeaterHeatPumpWrappedCondenser::parasiticHeatRejectionLocation() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->parasiticHeatRejectionLocation();
}

boost::optional<Schedule> WaterHeaterHeatPumpWrappedCondenser::inletAirMixerSchedule() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->inletAirMixerSchedule();
}

std::string WaterHeaterHeatPumpWrappedCondenser::tankElementControlLogic() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->tankElementControlLogic();
}

boost::optional<double> WaterHeaterHeatPumpWrappedCondenser::controlSensor1HeightInStratifiedTank() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->controlSensor1HeightInStratifiedTank();
}

double WaterHeaterHeatPumpWrappedCondenser::controlSensor1Weight() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->controlSensor1Weight();
}

boost::optional<double> WaterHeaterHeatPumpWrappedCondenser::controlSensor2HeightInStratifiedTank() const {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->controlSensor2HeightInStratifiedTank();
}

bool WaterHeaterHeatPumpWrappedCondenser::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setAvailabilitySchedule(schedule);
}

void WaterHeaterHeatPumpWrappedCondenser::resetAvailabilitySchedule() {
  getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->resetAvailabilitySchedule();
}

bool WaterHeaterHeatPumpWrappedCondenser::setCompressorSetpointTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setCompressorSetpointTemperatureSchedule(schedule);
}

bool WaterHeaterHeatPumpWrappedCondenser::setDeadBandTemperatureDifference(double deadBandTemperatureDifference) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setDeadBandTemperatureDifference(deadBandTemperatureDifference);
}

bool WaterHeaterHeatPumpWrappedCondenser::setCondenserBottomLocation(double condenserBottomLocation) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setCondenserBottomLocation(condenserBottomLocation);
}

bool WaterHeaterHeatPumpWrappedCondenser::setCondenserTopLocation(double condenserTopLocation) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setCondenserTopLocation(condenserTopLocation);
}

bool WaterHeaterHeatPumpWrappedCondenser::setEvaporatorAirFlowRate(double evaporatorAirFlowRate) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setEvaporatorAirFlowRate(evaporatorAirFlowRate);
}

void WaterHeaterHeatPumpWrappedCondenser::autocalculateEvaporatorAirFlowRate() {
  getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->autocalculateEvaporatorAirFlowRate();
}

bool WaterHeaterHeatPumpWrappedCondenser::setInletAirConfiguration(std::string inletAirConfiguration) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setInletAirConfiguration(inletAirConfiguration);
}

bool WaterHeaterHeatPumpWrappedCondenser::setInletAirTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setInletAirTemperatureSchedule(schedule);
}

void WaterHeaterHeatPumpWrappedCondenser::resetInletAirTemperatureSchedule() {
  getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->resetInletAirTemperatureSchedule();
}

bool WaterHeaterHeatPumpWrappedCondenser::setInletAirHumiditySchedule(Schedule& schedule) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setInletAirHumiditySchedule(schedule);
}

void WaterHeaterHeatPumpWrappedCondenser::resetInletAirHumiditySchedule() {
  getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->resetInletAirHumiditySchedule();
}

bool WaterHeaterHeatPumpWrappedCondenser::setTank(const HVACComponent& tank) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setTank(tank);
}

bool WaterHeaterHeatPumpWrappedCondenser::setDXCoil(const ModelObject& coil) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setDXCoil(coil);
}

bool WaterHeaterHeatPumpWrappedCondenser::setMinimumInletAirTemperatureforCompressorOperation(double minimumInletAirTemperatureforCompressorOperation) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setMinimumInletAirTemperatureforCompressorOperation(minimumInletAirTemperatureforCompressorOperation);
}

bool WaterHeaterHeatPumpWrappedCondenser::setMaximumInletAirTemperatureforCompressorOperation(double maximumInletAirTemperatureforCompressorOperation) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setMaximumInletAirTemperatureforCompressorOperation(maximumInletAirTemperatureforCompressorOperation);
}

bool WaterHeaterHeatPumpWrappedCondenser::setCompressorLocation(std::string compressorLocation) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setCompressorLocation(compressorLocation);
}

bool WaterHeaterHeatPumpWrappedCondenser::setCompressorAmbientTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setCompressorAmbientTemperatureSchedule(schedule);
}

void WaterHeaterHeatPumpWrappedCondenser::resetCompressorAmbientTemperatureSchedule() {
  getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->resetCompressorAmbientTemperatureSchedule();
}

bool WaterHeaterHeatPumpWrappedCondenser::setFan(const HVACComponent& fan) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setFan(fan);
}

bool WaterHeaterHeatPumpWrappedCondenser::setFanPlacement(std::string fanPlacement) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setFanPlacement(fanPlacement);
}

bool WaterHeaterHeatPumpWrappedCondenser::setOnCycleParasiticElectricLoad(double onCycleParasiticElectricLoad) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setOnCycleParasiticElectricLoad(onCycleParasiticElectricLoad);
}

bool WaterHeaterHeatPumpWrappedCondenser::setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setOffCycleParasiticElectricLoad(offCycleParasiticElectricLoad);
}

bool WaterHeaterHeatPumpWrappedCondenser::setParasiticHeatRejectionLocation(std::string parasiticHeatRejectionLocation) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setParasiticHeatRejectionLocation(parasiticHeatRejectionLocation);
}

bool WaterHeaterHeatPumpWrappedCondenser::setInletAirMixerSchedule(Schedule& schedule) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setInletAirMixerSchedule(schedule);
}

void WaterHeaterHeatPumpWrappedCondenser::resetInletAirMixerSchedule() {
  getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->resetInletAirMixerSchedule();
}

bool WaterHeaterHeatPumpWrappedCondenser::setTankElementControlLogic(std::string tankElementControlLogic) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setTankElementControlLogic(tankElementControlLogic);
}

bool WaterHeaterHeatPumpWrappedCondenser::setControlSensor1HeightInStratifiedTank(double controlSensor1HeightInStratifiedTank) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setControlSensor1HeightInStratifiedTank(controlSensor1HeightInStratifiedTank);
}

void WaterHeaterHeatPumpWrappedCondenser::resetControlSensor1HeightInStratifiedTank() {
  getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->resetControlSensor1HeightInStratifiedTank();
}

bool WaterHeaterHeatPumpWrappedCondenser::setControlSensor1Weight(double controlSensor1Weight) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setControlSensor1Weight(controlSensor1Weight);
}

bool WaterHeaterHeatPumpWrappedCondenser::setControlSensor2HeightInStratifiedTank(double controlSensor2HeightInStratifiedTank) {
  return getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->setControlSensor2HeightInStratifiedTank(controlSensor2HeightInStratifiedTank);
}

void WaterHeaterHeatPumpWrappedCondenser::resetControlSensor2HeightInStratifiedTank() {
  getImpl<detail::WaterHeaterHeatPumpWrappedCondenser_Impl>()->resetControlSensor2HeightInStratifiedTank();
}

/// @cond
WaterHeaterHeatPumpWrappedCondenser::WaterHeaterHeatPumpWrappedCondenser(std::shared_ptr<detail::WaterHeaterHeatPumpWrappedCondenser_Impl> impl)
  : ZoneHVACComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

