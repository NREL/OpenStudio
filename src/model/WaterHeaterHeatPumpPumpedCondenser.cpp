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

#include "WaterHeaterHeatPumpPumpedCondenser.hpp"
#include "WaterHeaterHeatPumpPumpedCondenser_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "FanOnOff.hpp"
#include "FanOnOff_Impl.hpp"
// #include "FanSystemModel.hpp"
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
#include "CoilWaterHeatingAirToWaterHeatPumpPumped.hpp"
#include "CoilWaterHeatingAirToWaterHeatPumpPumped_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_WaterHeater_HeatPump_PumpedCondenser_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  WaterHeaterHeatPumpPumpedCondenser_Impl::WaterHeaterHeatPumpPumpedCondenser_Impl(const IdfObject& idfObject,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ZoneHVACComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == WaterHeaterHeatPumpPumpedCondenser::iddObjectType());
  }

  WaterHeaterHeatPumpPumpedCondenser_Impl::WaterHeaterHeatPumpPumpedCondenser_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == WaterHeaterHeatPumpPumpedCondenser::iddObjectType());
  }

  WaterHeaterHeatPumpPumpedCondenser_Impl::WaterHeaterHeatPumpPumpedCondenser_Impl(const WaterHeaterHeatPumpPumpedCondenser_Impl& other,
                                                                                     Model_Impl* model,
                                                                                     bool keepHandle)
    : ZoneHVACComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& WaterHeaterHeatPumpPumpedCondenser_Impl::outputVariableNames() const
  {
    static const std::vector<std::string> result{
      "Water Heater Compressor Part Load Ratio",
      "Water Heater On Cycle Ancillary Electricity Rate",
      "Water Heater On Cycle Ancillary Electricity Energy",
      "Water Heater Off Cycle Ancillary Electricity Rate",
      "Water Heater Off Cycle Ancillary Electricity Energy"
    };
    return result;
  }

  IddObjectType WaterHeaterHeatPumpPumpedCondenser_Impl::iddObjectType() const {
    return WaterHeaterHeatPumpPumpedCondenser::iddObjectType();
  }

  std::vector<ScheduleTypeKey> WaterHeaterHeatPumpPumpedCondenser_Impl::getScheduleTypeKeys(const Schedule& schedule) const
  {
    std::vector<ScheduleTypeKey> result;
    UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
    UnsignedVector::const_iterator b(fieldIndices.begin()), e(fieldIndices.end());
    if (std::find(b,e,OS_WaterHeater_HeatPump_PumpedCondenserFields::AvailabilityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("WaterHeaterHeatPumpPumpedCondenser","Availability"));
    }
    if (std::find(b,e,OS_WaterHeater_HeatPump_PumpedCondenserFields::CompressorSetpointTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("WaterHeaterHeatPumpPumpedCondenser","Compressor Setpoint Temperature"));
    }
    if (std::find(b,e,OS_WaterHeater_HeatPump_PumpedCondenserFields::InletAirTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("WaterHeaterHeatPumpPumpedCondenser","Inlet Air Temperature"));
    }
    if (std::find(b,e,OS_WaterHeater_HeatPump_PumpedCondenserFields::InletAirHumidityScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("WaterHeaterHeatPumpPumpedCondenser","Inlet Air Humidity"));
    }
    if (std::find(b,e,OS_WaterHeater_HeatPump_PumpedCondenserFields::CompressorAmbientTemperatureScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("WaterHeaterHeatPumpPumpedCondenser","Compressor Ambient Temperature"));
    }
    if (std::find(b,e,OS_WaterHeater_HeatPump_PumpedCondenserFields::InletAirMixerScheduleName) != e)
    {
      result.push_back(ScheduleTypeKey("WaterHeaterHeatPumpPumpedCondenser","Inlet Air Mixer"));
    }
    return result;
  }

  boost::optional<Schedule> WaterHeaterHeatPumpPumpedCondenser_Impl::availabilitySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_HeatPump_PumpedCondenserFields::AvailabilityScheduleName);
  }

  Schedule WaterHeaterHeatPumpPumpedCondenser_Impl::compressorSetpointTemperatureSchedule() const {
    boost::optional<Schedule> value = optionalCompressorSetpointTemperatureSchedule();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Compressor Setpoint Temperature Schedule attached.");
    }
    return value.get();
  }

  double WaterHeaterHeatPumpPumpedCondenser_Impl::deadBandTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_HeatPump_PumpedCondenserFields::DeadBandTemperatureDifference,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterHeatPumpPumpedCondenser_Impl::condenserBottomLocation() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_HeatPump_PumpedCondenserFields::CondenserBottomLocation,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterHeatPumpPumpedCondenser_Impl::condenserTopLocation() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_HeatPump_PumpedCondenserFields::CondenserTopLocation,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> WaterHeaterHeatPumpPumpedCondenser_Impl::evaporatorAirFlowRate() const {
    return getDouble(OS_WaterHeater_HeatPump_PumpedCondenserFields::EvaporatorAirFlowRate,true);
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::isEvaporatorAirFlowRateAutocalculated() const {
    bool result = false;
    boost::optional<std::string> value = getString(OS_WaterHeater_HeatPump_PumpedCondenserFields::EvaporatorAirFlowRate, true);
    if (value) {
      result = openstudio::istringEqual(value.get(), "autocalculate");
    }
    return result;
  }

  std::string WaterHeaterHeatPumpPumpedCondenser_Impl::inletAirConfiguration() const {
    boost::optional<std::string> value = getString(OS_WaterHeater_HeatPump_PumpedCondenserFields::InletAirConfiguration,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> WaterHeaterHeatPumpPumpedCondenser_Impl::inletAirTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_HeatPump_PumpedCondenserFields::InletAirTemperatureScheduleName);
  }

  boost::optional<Schedule> WaterHeaterHeatPumpPumpedCondenser_Impl::inletAirHumiditySchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_HeatPump_PumpedCondenserFields::InletAirHumidityScheduleName);
  }

  HVACComponent WaterHeaterHeatPumpPumpedCondenser_Impl::tank() const {
    boost::optional<HVACComponent> value = optionalTank();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Tank attached.");
    }
    return value.get();
  }

  ModelObject WaterHeaterHeatPumpPumpedCondenser_Impl::dXCoil() const {
    boost::optional<ModelObject> value = optionalDXCoil();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an DXCoil attached.");
    }
    return value.get();
  }

  double WaterHeaterHeatPumpPumpedCondenser_Impl::minimumInletAirTemperatureforCompressorOperation() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_HeatPump_PumpedCondenserFields::MinimumInletAirTemperatureforCompressorOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterHeatPumpPumpedCondenser_Impl::maximumInletAirTemperatureforCompressorOperation() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_HeatPump_PumpedCondenserFields::MaximumInletAirTemperatureforCompressorOperation,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string WaterHeaterHeatPumpPumpedCondenser_Impl::compressorLocation() const {
    boost::optional<std::string> value = getString(OS_WaterHeater_HeatPump_PumpedCondenserFields::CompressorLocation,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> WaterHeaterHeatPumpPumpedCondenser_Impl::compressorAmbientTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_HeatPump_PumpedCondenserFields::CompressorAmbientTemperatureScheduleName);
  }

  HVACComponent WaterHeaterHeatPumpPumpedCondenser_Impl::fan() const {
    boost::optional<HVACComponent> value = optionalFan();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Fan attached.");
    }
    return value.get();
  }

  std::string WaterHeaterHeatPumpPumpedCondenser_Impl::fanPlacement() const {
    boost::optional<std::string> value = getString(OS_WaterHeater_HeatPump_PumpedCondenserFields::FanPlacement,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterHeatPumpPumpedCondenser_Impl::onCycleParasiticElectricLoad() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_HeatPump_PumpedCondenserFields::OnCycleParasiticElectricLoad,true);
    OS_ASSERT(value);
    return value.get();
  }

  double WaterHeaterHeatPumpPumpedCondenser_Impl::offCycleParasiticElectricLoad() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_HeatPump_PumpedCondenserFields::OffCycleParasiticElectricLoad,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string WaterHeaterHeatPumpPumpedCondenser_Impl::parasiticHeatRejectionLocation() const {
    boost::optional<std::string> value = getString(OS_WaterHeater_HeatPump_PumpedCondenserFields::ParasiticHeatRejectionLocation,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Schedule> WaterHeaterHeatPumpPumpedCondenser_Impl::inletAirMixerSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_HeatPump_PumpedCondenserFields::InletAirMixerScheduleName);
  }

  std::string WaterHeaterHeatPumpPumpedCondenser_Impl::tankElementControlLogic() const {
    boost::optional<std::string> value = getString(OS_WaterHeater_HeatPump_PumpedCondenserFields::TankElementControlLogic,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> WaterHeaterHeatPumpPumpedCondenser_Impl::controlSensor1HeightInStratifiedTank() const {
    return getDouble(OS_WaterHeater_HeatPump_PumpedCondenserFields::ControlSensor1HeightInStratifiedTank,true);
  }

  double WaterHeaterHeatPumpPumpedCondenser_Impl::controlSensor1Weight() const {
    boost::optional<double> value = getDouble(OS_WaterHeater_HeatPump_PumpedCondenserFields::ControlSensor1Weight,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<double> WaterHeaterHeatPumpPumpedCondenser_Impl::controlSensor2HeightInStratifiedTank() const {
    return getDouble(OS_WaterHeater_HeatPump_PumpedCondenserFields::ControlSensor2HeightInStratifiedTank,true);
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setAvailabilitySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_WaterHeater_HeatPump_PumpedCondenserFields::AvailabilityScheduleName,
                              "WaterHeaterHeatPumpPumpedCondenser",
                              "Availability",
                              schedule);
    return result;
  }

  void WaterHeaterHeatPumpPumpedCondenser_Impl::resetAvailabilitySchedule() {
    bool result = setString(OS_WaterHeater_HeatPump_PumpedCondenserFields::AvailabilityScheduleName, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setCompressorSetpointTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_WaterHeater_HeatPump_PumpedCondenserFields::CompressorSetpointTemperatureScheduleName,
                              "WaterHeaterHeatPumpPumpedCondenser",
                              "Compressor Setpoint Temperature",
                              schedule);
    return result;
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setDeadBandTemperatureDifference(double deadBandTemperatureDifference) {
    bool result = setDouble(OS_WaterHeater_HeatPump_PumpedCondenserFields::DeadBandTemperatureDifference, deadBandTemperatureDifference);
    return result;
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setCondenserBottomLocation(double condenserBottomLocation) {
    bool result = setDouble(OS_WaterHeater_HeatPump_PumpedCondenserFields::CondenserBottomLocation, condenserBottomLocation);
    return result;
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setCondenserTopLocation(double condenserTopLocation) {
    bool result = setDouble(OS_WaterHeater_HeatPump_PumpedCondenserFields::CondenserTopLocation, condenserTopLocation);
    return result;
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setEvaporatorAirFlowRate(boost::optional<double> evaporatorAirFlowRate) {
    bool result(false);
    if (evaporatorAirFlowRate) {
      result = setDouble(OS_WaterHeater_HeatPump_PumpedCondenserFields::EvaporatorAirFlowRate, evaporatorAirFlowRate.get());
    }
    return result;
  }

  void WaterHeaterHeatPumpPumpedCondenser_Impl::autocalculateEvaporatorAirFlowRate() {
    bool result = setString(OS_WaterHeater_HeatPump_PumpedCondenserFields::EvaporatorAirFlowRate, "autocalculate");
    OS_ASSERT(result);
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setInletAirConfiguration(std::string inletAirConfiguration) {
    bool result = setString(OS_WaterHeater_HeatPump_PumpedCondenserFields::InletAirConfiguration, inletAirConfiguration);
    return result;
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setInletAirTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_WaterHeater_HeatPump_PumpedCondenserFields::InletAirTemperatureScheduleName,
                              "WaterHeaterHeatPumpPumpedCondenser",
                              "Inlet Air Temperature",
                              schedule);
    return result;
  }

  void WaterHeaterHeatPumpPumpedCondenser_Impl::resetInletAirTemperatureSchedule() {
    bool result = setString(OS_WaterHeater_HeatPump_PumpedCondenserFields::InletAirTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setInletAirHumiditySchedule(Schedule& schedule) {
    bool result = setSchedule(OS_WaterHeater_HeatPump_PumpedCondenserFields::InletAirHumidityScheduleName,
                              "WaterHeaterHeatPumpPumpedCondenser",
                              "Inlet Air Humidity",
                              schedule);
    return result;
  }

  void WaterHeaterHeatPumpPumpedCondenser_Impl::resetInletAirHumiditySchedule() {
    bool result = setString(OS_WaterHeater_HeatPump_PumpedCondenserFields::InletAirHumidityScheduleName, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setTank(const HVACComponent& waterHeater) {
    bool result = setPointer(OS_WaterHeater_HeatPump_PumpedCondenserFields::TankName, waterHeater.handle());
    return result;
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setDXCoil(const ModelObject& coil) {
    bool result = setPointer(OS_WaterHeater_HeatPump_PumpedCondenserFields::DXCoilName, coil.handle());
    return result;
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setMinimumInletAirTemperatureforCompressorOperation(double minimumInletAirTemperatureforCompressorOperation) {
    bool result = setDouble(OS_WaterHeater_HeatPump_PumpedCondenserFields::MinimumInletAirTemperatureforCompressorOperation, minimumInletAirTemperatureforCompressorOperation);
    return result;
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setMaximumInletAirTemperatureforCompressorOperation(double maximumInletAirTemperatureforCompressorOperation) {
    bool result = setDouble(OS_WaterHeater_HeatPump_PumpedCondenserFields::MaximumInletAirTemperatureforCompressorOperation, maximumInletAirTemperatureforCompressorOperation);
    return result;
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setCompressorLocation(std::string compressorLocation) {
    bool result = setString(OS_WaterHeater_HeatPump_PumpedCondenserFields::CompressorLocation, compressorLocation);
    return result;
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setCompressorAmbientTemperatureSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_WaterHeater_HeatPump_PumpedCondenserFields::CompressorAmbientTemperatureScheduleName,
                              "WaterHeaterHeatPumpPumpedCondenser",
                              "Compressor Ambient Temperature",
                              schedule);
    return result;
  }

  void WaterHeaterHeatPumpPumpedCondenser_Impl::resetCompressorAmbientTemperatureSchedule() {
    bool result = setString(OS_WaterHeater_HeatPump_PumpedCondenserFields::CompressorAmbientTemperatureScheduleName, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setFan(const HVACComponent& fan) {
    bool result = setPointer(OS_WaterHeater_HeatPump_PumpedCondenserFields::FanName, fan.handle());
    return result;
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setFanPlacement(std::string fanPlacement) {
    bool result = setString(OS_WaterHeater_HeatPump_PumpedCondenserFields::FanPlacement, fanPlacement);
    return result;
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setOnCycleParasiticElectricLoad(double onCycleParasiticElectricLoad) {
    bool result = setDouble(OS_WaterHeater_HeatPump_PumpedCondenserFields::OnCycleParasiticElectricLoad, onCycleParasiticElectricLoad);
    return result;
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad) {
    bool result = setDouble(OS_WaterHeater_HeatPump_PumpedCondenserFields::OffCycleParasiticElectricLoad, offCycleParasiticElectricLoad);
    return result;
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setParasiticHeatRejectionLocation(std::string parasiticHeatRejectionLocation) {
    bool result = setString(OS_WaterHeater_HeatPump_PumpedCondenserFields::ParasiticHeatRejectionLocation, parasiticHeatRejectionLocation);
    return result;
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setInletAirMixerSchedule(Schedule& schedule) {
    bool result = setSchedule(OS_WaterHeater_HeatPump_PumpedCondenserFields::InletAirMixerScheduleName,
                              "WaterHeaterHeatPumpPumpedCondenser",
                              "Inlet Air Mixer",
                              schedule);
    return result;
  }

  void WaterHeaterHeatPumpPumpedCondenser_Impl::resetInletAirMixerSchedule() {
    bool result = setString(OS_WaterHeater_HeatPump_PumpedCondenserFields::InletAirMixerScheduleName, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setTankElementControlLogic(std::string tankElementControlLogic) {
    bool result = setString(OS_WaterHeater_HeatPump_PumpedCondenserFields::TankElementControlLogic, tankElementControlLogic);
    return result;
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setControlSensor1HeightInStratifiedTank(boost::optional<double> controlSensor1HeightInStratifiedTank) {
    bool result(false);
    if (controlSensor1HeightInStratifiedTank) {
      result = setDouble(OS_WaterHeater_HeatPump_PumpedCondenserFields::ControlSensor1HeightInStratifiedTank, controlSensor1HeightInStratifiedTank.get());
    }
    else {
      resetControlSensor1HeightInStratifiedTank();
      result = true;
    }
    return result;
  }

  void WaterHeaterHeatPumpPumpedCondenser_Impl::resetControlSensor1HeightInStratifiedTank() {
    bool result = setString(OS_WaterHeater_HeatPump_PumpedCondenserFields::ControlSensor1HeightInStratifiedTank, "");
    OS_ASSERT(result);
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setControlSensor1Weight(double controlSensor1Weight) {
    bool result = setDouble(OS_WaterHeater_HeatPump_PumpedCondenserFields::ControlSensor1Weight, controlSensor1Weight);
    return result;
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::setControlSensor2HeightInStratifiedTank(boost::optional<double> controlSensor2HeightInStratifiedTank) {
    bool result(false);
    if (controlSensor2HeightInStratifiedTank) {
      result = setDouble(OS_WaterHeater_HeatPump_PumpedCondenserFields::ControlSensor2HeightInStratifiedTank, controlSensor2HeightInStratifiedTank.get());
    }
    else {
      resetControlSensor2HeightInStratifiedTank();
      result = true;
    }
    return result;
  }

  void WaterHeaterHeatPumpPumpedCondenser_Impl::resetControlSensor2HeightInStratifiedTank() {
    bool result = setString(OS_WaterHeater_HeatPump_PumpedCondenserFields::ControlSensor2HeightInStratifiedTank, "");
    OS_ASSERT(result);
  }

  boost::optional<Schedule> WaterHeaterHeatPumpPumpedCondenser_Impl::optionalCompressorSetpointTemperatureSchedule() const {
    return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_HeatPump_PumpedCondenserFields::CompressorSetpointTemperatureScheduleName);
  }

  boost::optional<HVACComponent> WaterHeaterHeatPumpPumpedCondenser_Impl::optionalTank() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_WaterHeater_HeatPump_PumpedCondenserFields::TankName);
  }

  boost::optional<ModelObject> WaterHeaterHeatPumpPumpedCondenser_Impl::optionalDXCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<ModelObject>(OS_WaterHeater_HeatPump_PumpedCondenserFields::DXCoilName);
  }

  boost::optional<HVACComponent> WaterHeaterHeatPumpPumpedCondenser_Impl::optionalFan() const {
    return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_WaterHeater_HeatPump_PumpedCondenserFields::FanName);
  }

  unsigned WaterHeaterHeatPumpPumpedCondenser_Impl::inletPort() const {
    return OS_WaterHeater_HeatPump_PumpedCondenserFields::AirInletNodeName;
  }

  unsigned WaterHeaterHeatPumpPumpedCondenser_Impl::outletPort() const {
    return OS_WaterHeater_HeatPump_PumpedCondenserFields::AirOutletNodeName;
  }

  ModelObject WaterHeaterHeatPumpPumpedCondenser_Impl::clone(Model model) const
  {
    std::cout << "boom 1" << std::endl;
    auto newWaterHeater = ZoneHVACComponent_Impl::clone(model).cast<WaterHeaterHeatPumpPumpedCondenser>();

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

  std::vector<IdfObject> WaterHeaterHeatPumpPumpedCondenser_Impl::remove()
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

  std::vector<ModelObject> WaterHeaterHeatPumpPumpedCondenser_Impl::children() const
  {
    std::vector<ModelObject> result;

    result.push_back(tank());
    result.push_back(dXCoil());
    result.push_back(fan());

    return result;
  }

  bool WaterHeaterHeatPumpPumpedCondenser_Impl::addToThermalZone(ThermalZone & thermalZone)
  {
    bool result = false;
    auto thisObject = getObject<WaterHeaterHeatPumpPumpedCondenser>();

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

WaterHeaterHeatPumpPumpedCondenser::WaterHeaterHeatPumpPumpedCondenser(const Model& model)
  : ZoneHVACComponent(WaterHeaterHeatPumpPumpedCondenser::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>());

  CoilWaterHeatingAirToWaterHeatPumpPumped coil(model);
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

WaterHeaterHeatPumpPumpedCondenser::WaterHeaterHeatPumpPumpedCondenser(const Model& model,
  const ModelObject & dxCoil,
  const HVACComponent & tank,
  const HVACComponent & fan,
  Schedule & compressorSetpointTemperatureSchedule,
  Schedule & inletAirMixerSchedule)
  : ZoneHVACComponent(WaterHeaterHeatPumpPumpedCondenser::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>());

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

IddObjectType WaterHeaterHeatPumpPumpedCondenser::iddObjectType() {
  return IddObjectType(IddObjectType::OS_WaterHeater_HeatPump_PumpedCondenser);
}

std::vector<std::string> WaterHeaterHeatPumpPumpedCondenser::inletAirConfigurationValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WaterHeater_HeatPump_PumpedCondenserFields::InletAirConfiguration);
}

std::vector<std::string> WaterHeaterHeatPumpPumpedCondenser::compressorLocationValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WaterHeater_HeatPump_PumpedCondenserFields::CompressorLocation);
}

std::vector<std::string> WaterHeaterHeatPumpPumpedCondenser::fanPlacementValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WaterHeater_HeatPump_PumpedCondenserFields::FanPlacement);
}

std::vector<std::string> WaterHeaterHeatPumpPumpedCondenser::parasiticHeatRejectionLocationValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WaterHeater_HeatPump_PumpedCondenserFields::ParasiticHeatRejectionLocation);
}

std::vector<std::string> WaterHeaterHeatPumpPumpedCondenser::tankElementControlLogicValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_WaterHeater_HeatPump_PumpedCondenserFields::TankElementControlLogic);
}

boost::optional<Schedule> WaterHeaterHeatPumpPumpedCondenser::availabilitySchedule() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->availabilitySchedule();
}

Schedule WaterHeaterHeatPumpPumpedCondenser::compressorSetpointTemperatureSchedule() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->compressorSetpointTemperatureSchedule();
}

double WaterHeaterHeatPumpPumpedCondenser::deadBandTemperatureDifference() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->deadBandTemperatureDifference();
}

double WaterHeaterHeatPumpPumpedCondenser::condenserBottomLocation() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->condenserBottomLocation();
}

double WaterHeaterHeatPumpPumpedCondenser::condenserTopLocation() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->condenserTopLocation();
}

boost::optional<double> WaterHeaterHeatPumpPumpedCondenser::evaporatorAirFlowRate() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->evaporatorAirFlowRate();
}

bool WaterHeaterHeatPumpPumpedCondenser::isEvaporatorAirFlowRateAutocalculated() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->isEvaporatorAirFlowRateAutocalculated();
}

std::string WaterHeaterHeatPumpPumpedCondenser::inletAirConfiguration() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->inletAirConfiguration();
}

boost::optional<Schedule> WaterHeaterHeatPumpPumpedCondenser::inletAirTemperatureSchedule() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->inletAirTemperatureSchedule();
}

boost::optional<Schedule> WaterHeaterHeatPumpPumpedCondenser::inletAirHumiditySchedule() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->inletAirHumiditySchedule();
}

HVACComponent WaterHeaterHeatPumpPumpedCondenser::tank() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->tank();
}

ModelObject WaterHeaterHeatPumpPumpedCondenser::dXCoil() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->dXCoil();
}

double WaterHeaterHeatPumpPumpedCondenser::minimumInletAirTemperatureforCompressorOperation() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->minimumInletAirTemperatureforCompressorOperation();
}

double WaterHeaterHeatPumpPumpedCondenser::maximumInletAirTemperatureforCompressorOperation() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->maximumInletAirTemperatureforCompressorOperation();
}

std::string WaterHeaterHeatPumpPumpedCondenser::compressorLocation() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->compressorLocation();
}

boost::optional<Schedule> WaterHeaterHeatPumpPumpedCondenser::compressorAmbientTemperatureSchedule() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->compressorAmbientTemperatureSchedule();
}

HVACComponent WaterHeaterHeatPumpPumpedCondenser::fan() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->fan();
}

std::string WaterHeaterHeatPumpPumpedCondenser::fanPlacement() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->fanPlacement();
}

double WaterHeaterHeatPumpPumpedCondenser::onCycleParasiticElectricLoad() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->onCycleParasiticElectricLoad();
}

double WaterHeaterHeatPumpPumpedCondenser::offCycleParasiticElectricLoad() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->offCycleParasiticElectricLoad();
}

std::string WaterHeaterHeatPumpPumpedCondenser::parasiticHeatRejectionLocation() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->parasiticHeatRejectionLocation();
}

boost::optional<Schedule> WaterHeaterHeatPumpPumpedCondenser::inletAirMixerSchedule() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->inletAirMixerSchedule();
}

std::string WaterHeaterHeatPumpPumpedCondenser::tankElementControlLogic() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->tankElementControlLogic();
}

boost::optional<double> WaterHeaterHeatPumpPumpedCondenser::controlSensor1HeightInStratifiedTank() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->controlSensor1HeightInStratifiedTank();
}

double WaterHeaterHeatPumpPumpedCondenser::controlSensor1Weight() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->controlSensor1Weight();
}

boost::optional<double> WaterHeaterHeatPumpPumpedCondenser::controlSensor2HeightInStratifiedTank() const {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->controlSensor2HeightInStratifiedTank();
}

bool WaterHeaterHeatPumpPumpedCondenser::setAvailabilitySchedule(Schedule& schedule) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setAvailabilitySchedule(schedule);
}

void WaterHeaterHeatPumpPumpedCondenser::resetAvailabilitySchedule() {
  getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->resetAvailabilitySchedule();
}

bool WaterHeaterHeatPumpPumpedCondenser::setCompressorSetpointTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setCompressorSetpointTemperatureSchedule(schedule);
}

bool WaterHeaterHeatPumpPumpedCondenser::setDeadBandTemperatureDifference(double deadBandTemperatureDifference) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setDeadBandTemperatureDifference(deadBandTemperatureDifference);
}

bool WaterHeaterHeatPumpPumpedCondenser::setCondenserBottomLocation(double condenserBottomLocation) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setCondenserBottomLocation(condenserBottomLocation);
}

bool WaterHeaterHeatPumpPumpedCondenser::setCondenserTopLocation(double condenserTopLocation) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setCondenserTopLocation(condenserTopLocation);
}

bool WaterHeaterHeatPumpPumpedCondenser::setEvaporatorAirFlowRate(double evaporatorAirFlowRate) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setEvaporatorAirFlowRate(evaporatorAirFlowRate);
}

void WaterHeaterHeatPumpPumpedCondenser::autocalculateEvaporatorAirFlowRate() {
  getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->autocalculateEvaporatorAirFlowRate();
}

bool WaterHeaterHeatPumpPumpedCondenser::setInletAirConfiguration(std::string inletAirConfiguration) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setInletAirConfiguration(inletAirConfiguration);
}

bool WaterHeaterHeatPumpPumpedCondenser::setInletAirTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setInletAirTemperatureSchedule(schedule);
}

void WaterHeaterHeatPumpPumpedCondenser::resetInletAirTemperatureSchedule() {
  getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->resetInletAirTemperatureSchedule();
}

bool WaterHeaterHeatPumpPumpedCondenser::setInletAirHumiditySchedule(Schedule& schedule) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setInletAirHumiditySchedule(schedule);
}

void WaterHeaterHeatPumpPumpedCondenser::resetInletAirHumiditySchedule() {
  getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->resetInletAirHumiditySchedule();
}

bool WaterHeaterHeatPumpPumpedCondenser::setTank(const HVACComponent& tank) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setTank(tank);
}

bool WaterHeaterHeatPumpPumpedCondenser::setDXCoil(const ModelObject& coil) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setDXCoil(coil);
}

bool WaterHeaterHeatPumpPumpedCondenser::setMinimumInletAirTemperatureforCompressorOperation(double minimumInletAirTemperatureforCompressorOperation) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setMinimumInletAirTemperatureforCompressorOperation(minimumInletAirTemperatureforCompressorOperation);
}

bool WaterHeaterHeatPumpPumpedCondenser::setMaximumInletAirTemperatureforCompressorOperation(double maximumInletAirTemperatureforCompressorOperation) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setMaximumInletAirTemperatureforCompressorOperation(maximumInletAirTemperatureforCompressorOperation);
}

bool WaterHeaterHeatPumpPumpedCondenser::setCompressorLocation(std::string compressorLocation) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setCompressorLocation(compressorLocation);
}

bool WaterHeaterHeatPumpPumpedCondenser::setCompressorAmbientTemperatureSchedule(Schedule& schedule) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setCompressorAmbientTemperatureSchedule(schedule);
}

void WaterHeaterHeatPumpPumpedCondenser::resetCompressorAmbientTemperatureSchedule() {
  getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->resetCompressorAmbientTemperatureSchedule();
}

bool WaterHeaterHeatPumpPumpedCondenser::setFan(const HVACComponent& fan) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setFan(fan);
}

bool WaterHeaterHeatPumpPumpedCondenser::setFanPlacement(std::string fanPlacement) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setFanPlacement(fanPlacement);
}

bool WaterHeaterHeatPumpPumpedCondenser::setOnCycleParasiticElectricLoad(double onCycleParasiticElectricLoad) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setOnCycleParasiticElectricLoad(onCycleParasiticElectricLoad);
}

bool WaterHeaterHeatPumpPumpedCondenser::setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setOffCycleParasiticElectricLoad(offCycleParasiticElectricLoad);
}

bool WaterHeaterHeatPumpPumpedCondenser::setParasiticHeatRejectionLocation(std::string parasiticHeatRejectionLocation) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setParasiticHeatRejectionLocation(parasiticHeatRejectionLocation);
}

bool WaterHeaterHeatPumpPumpedCondenser::setInletAirMixerSchedule(Schedule& schedule) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setInletAirMixerSchedule(schedule);
}

void WaterHeaterHeatPumpPumpedCondenser::resetInletAirMixerSchedule() {
  getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->resetInletAirMixerSchedule();
}

bool WaterHeaterHeatPumpPumpedCondenser::setTankElementControlLogic(std::string tankElementControlLogic) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setTankElementControlLogic(tankElementControlLogic);
}

bool WaterHeaterHeatPumpPumpedCondenser::setControlSensor1HeightInStratifiedTank(double controlSensor1HeightInStratifiedTank) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setControlSensor1HeightInStratifiedTank(controlSensor1HeightInStratifiedTank);
}

void WaterHeaterHeatPumpPumpedCondenser::resetControlSensor1HeightInStratifiedTank() {
  getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->resetControlSensor1HeightInStratifiedTank();
}

bool WaterHeaterHeatPumpPumpedCondenser::setControlSensor1Weight(double controlSensor1Weight) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setControlSensor1Weight(controlSensor1Weight);
}

bool WaterHeaterHeatPumpPumpedCondenser::setControlSensor2HeightInStratifiedTank(double controlSensor2HeightInStratifiedTank) {
  return getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->setControlSensor2HeightInStratifiedTank(controlSensor2HeightInStratifiedTank);
}

void WaterHeaterHeatPumpPumpedCondenser::resetControlSensor2HeightInStratifiedTank() {
  getImpl<detail::WaterHeaterHeatPumpPumpedCondenser_Impl>()->resetControlSensor2HeightInStratifiedTank();
}

/// @cond
WaterHeaterHeatPumpPumpedCondenser::WaterHeaterHeatPumpPumpedCondenser(std::shared_ptr<detail::WaterHeaterHeatPumpPumpedCondenser_Impl> impl)
  : ZoneHVACComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

