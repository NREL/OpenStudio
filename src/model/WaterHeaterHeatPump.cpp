/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "WaterHeaterHeatPump.hpp"
#include "WaterHeaterHeatPump_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "WaterHeaterMixed.hpp"
#include "WaterHeaterMixed_Impl.hpp"
#include "WaterHeaterStratified.hpp"
#include "WaterHeaterStratified_Impl.hpp"
#include "FanOnOff.hpp"
#include "FanOnOff_Impl.hpp"
// #include "FanSystemModel.hpp"
#include "CoilWaterHeatingAirToWaterHeatPump.hpp"
#include "CoilWaterHeatingAirToWaterHeatPump_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleDay.hpp"
#include "ScheduleDay_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "HVACComponent.hpp"
#include "HVACComponent_Impl.hpp"
#include "ScheduleRuleset.hpp"
#include "ScheduleRuleset_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_WaterHeater_HeatPump_FieldEnums.hxx>

namespace openstudio {
namespace model {

  namespace detail {

    WaterHeaterHeatPump_Impl::WaterHeaterHeatPump_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == WaterHeaterHeatPump::iddObjectType());
    }

    WaterHeaterHeatPump_Impl::WaterHeaterHeatPump_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == WaterHeaterHeatPump::iddObjectType());
    }

    WaterHeaterHeatPump_Impl::WaterHeaterHeatPump_Impl(const WaterHeaterHeatPump_Impl& other, Model_Impl* model, bool keepHandle)
      : ZoneHVACComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& WaterHeaterHeatPump_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{// WaterHeater:HeatPump:PumpedCondenser
                                                   "Water Heater Compressor Part Load Ratio", "Water Heater On Cycle Ancillary Electricity Rate",
                                                   "Water Heater On Cycle Ancillary Electricity Energy",
                                                   "Water Heater Off Cycle Ancillary Electricity Rate",
                                                   "Water Heater Off Cycle Ancillary Electricity Energy"};
      return result;
    }

    IddObjectType WaterHeaterHeatPump_Impl::iddObjectType() const {
      return WaterHeaterHeatPump::iddObjectType();
    }

    std::vector<ScheduleTypeKey> WaterHeaterHeatPump_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      // TODO: Check schedule display names.
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_WaterHeater_HeatPumpFields::AvailabilitySchedule) != e) {
        result.push_back(ScheduleTypeKey("WaterHeaterHeatPump", "Availability Schedule"));
      }
      if (std::find(b, e, OS_WaterHeater_HeatPumpFields::CompressorSetpointTemperatureSchedule) != e) {
        result.push_back(ScheduleTypeKey("WaterHeaterHeatPump", "Compressor Setpoint Temperature Schedule"));
      }
      if (std::find(b, e, OS_WaterHeater_HeatPumpFields::InletAirTemperatureSchedule) != e) {
        result.push_back(ScheduleTypeKey("WaterHeaterHeatPump", "Inlet Air Temperature Schedule"));
      }
      if (std::find(b, e, OS_WaterHeater_HeatPumpFields::InletAirHumiditySchedule) != e) {
        result.push_back(ScheduleTypeKey("WaterHeaterHeatPump", "Inlet Air Humidity Schedule"));
      }
      if (std::find(b, e, OS_WaterHeater_HeatPumpFields::CompressorAmbientTemperatureSchedule) != e) {
        result.push_back(ScheduleTypeKey("WaterHeaterHeatPump", "Compressor Ambient Temperature Schedule"));
      }
      if (std::find(b, e, OS_WaterHeater_HeatPumpFields::InletAirMixerSchedule) != e) {
        result.push_back(ScheduleTypeKey("WaterHeaterHeatPump", "Inlet Air Mixer Schedule"));
      }
      return result;
    }

    boost::optional<Schedule> WaterHeaterHeatPump_Impl::availabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_HeatPumpFields::AvailabilitySchedule);
    }

    Schedule WaterHeaterHeatPump_Impl::compressorSetpointTemperatureSchedule() const {
      boost::optional<Schedule> value = optionalCompressorSetpointTemperatureSchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Compressor Setpoint Temperature Schedule attached.");
      }
      return value.get();
    }

    double WaterHeaterHeatPump_Impl::deadBandTemperatureDifference() const {
      boost::optional<double> value = getDouble(OS_WaterHeater_HeatPumpFields::DeadBandTemperatureDifference, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> WaterHeaterHeatPump_Impl::condenserWaterFlowRate() const {
      return getDouble(OS_WaterHeater_HeatPumpFields::CondenserWaterFlowRate, true);
    }

    bool WaterHeaterHeatPump_Impl::isCondenserWaterFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_WaterHeater_HeatPumpFields::CondenserWaterFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> WaterHeaterHeatPump_Impl::evaporatorAirFlowRate() const {
      return getDouble(OS_WaterHeater_HeatPumpFields::EvaporatorAirFlowRate, true);
    }

    bool WaterHeaterHeatPump_Impl::isEvaporatorAirFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_WaterHeater_HeatPumpFields::EvaporatorAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    std::string WaterHeaterHeatPump_Impl::inletAirConfiguration() const {
      boost::optional<std::string> value = getString(OS_WaterHeater_HeatPumpFields::InletAirConfiguration, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> WaterHeaterHeatPump_Impl::inletAirTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_HeatPumpFields::InletAirTemperatureSchedule);
    }

    boost::optional<Schedule> WaterHeaterHeatPump_Impl::inletAirHumiditySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_HeatPumpFields::InletAirHumiditySchedule);
    }

    HVACComponent WaterHeaterHeatPump_Impl::tank() const {
      auto value = optionalTank();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Tank attached.");
      }
      return value.get();
    }

    ModelObject WaterHeaterHeatPump_Impl::dXCoil() const {
      auto value = optionalDXCoil();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an DXCoil attached.");
      }
      return value.get();
    }

    double WaterHeaterHeatPump_Impl::minimumInletAirTemperatureforCompressorOperation() const {
      boost::optional<double> value = getDouble(OS_WaterHeater_HeatPumpFields::MinimumInletAirTemperatureforCompressorOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    double WaterHeaterHeatPump_Impl::maximumInletAirTemperatureforCompressorOperation() const {
      boost::optional<double> value = getDouble(OS_WaterHeater_HeatPumpFields::MaximumInletAirTemperatureforCompressorOperation, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string WaterHeaterHeatPump_Impl::compressorLocation() const {
      boost::optional<std::string> value = getString(OS_WaterHeater_HeatPumpFields::CompressorLocation, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> WaterHeaterHeatPump_Impl::compressorAmbientTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_HeatPumpFields::CompressorAmbientTemperatureSchedule);
    }

    HVACComponent WaterHeaterHeatPump_Impl::fan() const {
      auto value = optionalFan();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Fan attached.");
      }
      return value.get();
    }

    std::string WaterHeaterHeatPump_Impl::fanPlacement() const {
      boost::optional<std::string> value = getString(OS_WaterHeater_HeatPumpFields::FanPlacement, true);
      OS_ASSERT(value);
      return value.get();
    }

    double WaterHeaterHeatPump_Impl::onCycleParasiticElectricLoad() const {
      boost::optional<double> value = getDouble(OS_WaterHeater_HeatPumpFields::OnCycleParasiticElectricLoad, true);
      OS_ASSERT(value);
      return value.get();
    }

    double WaterHeaterHeatPump_Impl::offCycleParasiticElectricLoad() const {
      boost::optional<double> value = getDouble(OS_WaterHeater_HeatPumpFields::OffCycleParasiticElectricLoad, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string WaterHeaterHeatPump_Impl::parasiticHeatRejectionLocation() const {
      boost::optional<std::string> value = getString(OS_WaterHeater_HeatPumpFields::ParasiticHeatRejectionLocation, true);
      OS_ASSERT(value);
      return value.get();
    }

    Schedule WaterHeaterHeatPump_Impl::inletAirMixerSchedule() const {
      boost::optional<Schedule> value = optionalInletAirMixerSchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Inlet Air Mixer Schedule attached.");
      }
      return value.get();
    }

    std::string WaterHeaterHeatPump_Impl::tankElementControlLogic() const {
      boost::optional<std::string> value = getString(OS_WaterHeater_HeatPumpFields::TankElementControlLogic, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string WaterHeaterHeatPump_Impl::controlSensorLocationInStratifiedTank() const {
      boost::optional<std::string> value = getString(OS_WaterHeater_HeatPumpFields::ControlSensorLocationInStratifiedTank, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool WaterHeaterHeatPump_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_WaterHeater_HeatPumpFields::AvailabilitySchedule, "WaterHeaterHeatPump", "Availability Schedule", schedule);
      return result;
    }

    void WaterHeaterHeatPump_Impl::resetAvailabilitySchedule() {
      bool result = setString(OS_WaterHeater_HeatPumpFields::AvailabilitySchedule, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterHeatPump_Impl::setCompressorSetpointTemperatureSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_WaterHeater_HeatPumpFields::CompressorSetpointTemperatureSchedule, "WaterHeaterHeatPump",
                                "Compressor Setpoint Temperature Schedule", schedule);
      return result;
    }

    bool WaterHeaterHeatPump_Impl::setDeadBandTemperatureDifference(double deadBandTemperatureDifference) {
      bool result = setDouble(OS_WaterHeater_HeatPumpFields::DeadBandTemperatureDifference, deadBandTemperatureDifference);
      return result;
    }

    bool WaterHeaterHeatPump_Impl::setCondenserWaterFlowRate(boost::optional<double> condenserWaterFlowRate) {
      bool result(false);
      if (condenserWaterFlowRate) {
        result = setDouble(OS_WaterHeater_HeatPumpFields::CondenserWaterFlowRate, condenserWaterFlowRate.get());
      } else {
        resetCondenserWaterFlowRate();
        result = true;
      }
      return result;
    }

    void WaterHeaterHeatPump_Impl::resetCondenserWaterFlowRate() {
      bool result = setString(OS_WaterHeater_HeatPumpFields::CondenserWaterFlowRate, "");
      OS_ASSERT(result);
    }

    void WaterHeaterHeatPump_Impl::autosizeCondenserWaterFlowRate() {
      bool result = setString(OS_WaterHeater_HeatPumpFields::CondenserWaterFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool WaterHeaterHeatPump_Impl::setEvaporatorAirFlowRate(boost::optional<double> evaporatorAirFlowRate) {
      bool result(false);
      if (evaporatorAirFlowRate) {
        result = setDouble(OS_WaterHeater_HeatPumpFields::EvaporatorAirFlowRate, evaporatorAirFlowRate.get());
      } else {
        resetEvaporatorAirFlowRate();
        result = true;
      }
      return result;
    }

    void WaterHeaterHeatPump_Impl::resetEvaporatorAirFlowRate() {
      bool result = setString(OS_WaterHeater_HeatPumpFields::EvaporatorAirFlowRate, "");
      OS_ASSERT(result);
    }

    void WaterHeaterHeatPump_Impl::autosizeEvaporatorAirFlowRate() {
      bool result = setString(OS_WaterHeater_HeatPumpFields::EvaporatorAirFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool WaterHeaterHeatPump_Impl::setInletAirConfiguration(const std::string& inletAirConfiguration) {
      bool result = setString(OS_WaterHeater_HeatPumpFields::InletAirConfiguration, inletAirConfiguration);
      return result;
    }

    bool WaterHeaterHeatPump_Impl::setInletAirTemperatureSchedule(Schedule& schedule) {
      bool result =
        setSchedule(OS_WaterHeater_HeatPumpFields::InletAirTemperatureSchedule, "WaterHeaterHeatPump", "Inlet Air Temperature Schedule", schedule);
      return result;
    }

    void WaterHeaterHeatPump_Impl::resetInletAirTemperatureSchedule() {
      bool result = setString(OS_WaterHeater_HeatPumpFields::InletAirTemperatureSchedule, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterHeatPump_Impl::setInletAirHumiditySchedule(Schedule& schedule) {
      bool result =
        setSchedule(OS_WaterHeater_HeatPumpFields::InletAirHumiditySchedule, "WaterHeaterHeatPump", "Inlet Air Humidity Schedule", schedule);
      return result;
    }

    void WaterHeaterHeatPump_Impl::resetInletAirHumiditySchedule() {
      bool result = setString(OS_WaterHeater_HeatPumpFields::InletAirHumiditySchedule, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterHeatPump_Impl::setTank(const HVACComponent& waterHeater) {
      bool result = setPointer(OS_WaterHeater_HeatPumpFields::Tank, waterHeater.handle());
      return result;
    }

    bool WaterHeaterHeatPump_Impl::setDXCoil(const ModelObject& heatPumpWaterHeaterDXCoils) {
      bool result = setPointer(OS_WaterHeater_HeatPumpFields::DXCoil, heatPumpWaterHeaterDXCoils.handle());
      return result;
    }

    bool WaterHeaterHeatPump_Impl::setMinimumInletAirTemperatureforCompressorOperation(double minimumInletAirTemperatureforCompressorOperation) {
      bool result =
        setDouble(OS_WaterHeater_HeatPumpFields::MinimumInletAirTemperatureforCompressorOperation, minimumInletAirTemperatureforCompressorOperation);
      return result;
    }

    bool WaterHeaterHeatPump_Impl::setMaximumInletAirTemperatureforCompressorOperation(double maximumInletAirTemperatureforCompressorOperation) {
      bool result =
        setDouble(OS_WaterHeater_HeatPumpFields::MaximumInletAirTemperatureforCompressorOperation, maximumInletAirTemperatureforCompressorOperation);
      return result;
    }

    bool WaterHeaterHeatPump_Impl::setCompressorLocation(const std::string& compressorLocation) {
      bool result = setString(OS_WaterHeater_HeatPumpFields::CompressorLocation, compressorLocation);
      return result;
    }

    bool WaterHeaterHeatPump_Impl::setCompressorAmbientTemperatureSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_WaterHeater_HeatPumpFields::CompressorAmbientTemperatureSchedule, "WaterHeaterHeatPump",
                                "Compressor Ambient Temperature Schedule", schedule);
      return result;
    }

    void WaterHeaterHeatPump_Impl::resetCompressorAmbientTemperatureSchedule() {
      bool result = setString(OS_WaterHeater_HeatPumpFields::CompressorAmbientTemperatureSchedule, "");
      OS_ASSERT(result);
    }

    bool WaterHeaterHeatPump_Impl::setFan(const HVACComponent& fansOnOff) {
      bool result = setPointer(OS_WaterHeater_HeatPumpFields::Fan, fansOnOff.handle());
      return result;
    }

    bool WaterHeaterHeatPump_Impl::setFanPlacement(const std::string& fanPlacement) {
      bool result = setString(OS_WaterHeater_HeatPumpFields::FanPlacement, fanPlacement);
      return result;
    }

    bool WaterHeaterHeatPump_Impl::setOnCycleParasiticElectricLoad(double onCycleParasiticElectricLoad) {
      bool result = setDouble(OS_WaterHeater_HeatPumpFields::OnCycleParasiticElectricLoad, onCycleParasiticElectricLoad);
      return result;
    }

    bool WaterHeaterHeatPump_Impl::setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad) {
      bool result = setDouble(OS_WaterHeater_HeatPumpFields::OffCycleParasiticElectricLoad, offCycleParasiticElectricLoad);
      return result;
    }

    bool WaterHeaterHeatPump_Impl::setParasiticHeatRejectionLocation(const std::string& parasiticHeatRejectionLocation) {
      bool result = setString(OS_WaterHeater_HeatPumpFields::ParasiticHeatRejectionLocation, parasiticHeatRejectionLocation);
      return result;
    }

    bool WaterHeaterHeatPump_Impl::setInletAirMixerSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_WaterHeater_HeatPumpFields::InletAirMixerSchedule, "WaterHeaterHeatPump", "Inlet Air Mixer Schedule", schedule);
      return result;
    }

    bool WaterHeaterHeatPump_Impl::setTankElementControlLogic(const std::string& tankElementControlLogic) {
      bool result = setString(OS_WaterHeater_HeatPumpFields::TankElementControlLogic, tankElementControlLogic);
      return result;
    }

    bool WaterHeaterHeatPump_Impl::setControlSensorLocationInStratifiedTank(const std::string& controlSensorLocationInStratifiedTank) {
      bool result = setString(OS_WaterHeater_HeatPumpFields::ControlSensorLocationInStratifiedTank, controlSensorLocationInStratifiedTank);
      return result;
    }

    boost::optional<Schedule> WaterHeaterHeatPump_Impl::optionalCompressorSetpointTemperatureSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_HeatPumpFields::CompressorSetpointTemperatureSchedule);
    }

    boost::optional<HVACComponent> WaterHeaterHeatPump_Impl::optionalTank() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_WaterHeater_HeatPumpFields::Tank);
    }

    boost::optional<ModelObject> WaterHeaterHeatPump_Impl::optionalDXCoil() const {
      return getObject<ModelObject>().getModelObjectTarget<ModelObject>(OS_WaterHeater_HeatPumpFields::DXCoil);
    }

    boost::optional<HVACComponent> WaterHeaterHeatPump_Impl::optionalFan() const {
      return getObject<ModelObject>().getModelObjectTarget<HVACComponent>(OS_WaterHeater_HeatPumpFields::Fan);
    }

    boost::optional<Schedule> WaterHeaterHeatPump_Impl::optionalInletAirMixerSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_WaterHeater_HeatPumpFields::InletAirMixerSchedule);
    }

    unsigned WaterHeaterHeatPump_Impl::inletPort() const {
      return OS_WaterHeater_HeatPumpFields::AirInletNode;
    }

    unsigned WaterHeaterHeatPump_Impl::outletPort() const {
      return OS_WaterHeater_HeatPumpFields::AirOutletNode;
    }

    ModelObject WaterHeaterHeatPump_Impl::clone(Model model) const {
      auto newWaterHeater = ZoneHVACComponent_Impl::clone(model).cast<WaterHeaterHeatPump>();

      {
        auto mo = tank().clone(model).cast<HVACComponent>();
        newWaterHeater.setTank(mo);
      }

      {
        auto mo = dXCoil().clone(model).cast<ModelObject>();
        newWaterHeater.setDXCoil(mo);
      }

      {
        auto mo = fan().clone(model).cast<HVACComponent>();
        newWaterHeater.setFan(mo);
      }

      return std::move(newWaterHeater);
    }

    std::vector<IdfObject> WaterHeaterHeatPump_Impl::remove() {
      auto t_tank = tank();
      if (auto waterToWaterTank = t_tank.optionalCast<WaterToWaterComponent>()) {
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

    std::vector<ModelObject> WaterHeaterHeatPump_Impl::children() const {
      std::vector<ModelObject> result;

      result.push_back(tank());
      result.push_back(dXCoil());
      result.push_back(fan());

      return result;
    }

    bool WaterHeaterHeatPump_Impl::addToThermalZone(ThermalZone& thermalZone) {
      bool result = false;
      auto thisObject = getObject<WaterHeaterHeatPump>();

      if ((result = ZoneHVACComponent_Impl::addToThermalZone(thermalZone))) {
        thermalZone.setHeatingPriority(thisObject, 1);
        thermalZone.setCoolingPriority(thisObject, 1);
        setCompressorLocation("Zone");
        setInletAirConfiguration("ZoneAirOnly");
        auto t_tank = tank();
        if (auto waterHeaterMixed = t_tank.optionalCast<WaterHeaterMixed>()) {
          waterHeaterMixed->setAmbientTemperatureIndicator("ThermalZone");
          waterHeaterMixed->setAmbientTemperatureThermalZone(thermalZone);
        } else if (auto waterHeaterStratified = t_tank.optionalCast<WaterHeaterStratified>()) {
          waterHeaterStratified->setAmbientTemperatureIndicator("ThermalZone");
          waterHeaterStratified->setAmbientTemperatureThermalZone(thermalZone);
        }
      }

      return result;
    }

    boost::optional<double> WaterHeaterHeatPump_Impl::autosizedCondenserWaterFlowRate() const {
      return getAutosizedValue("Design Size Condenser Water Flow Rate", "m3/s");
    }

    boost::optional<double> WaterHeaterHeatPump_Impl::autosizedEvaporatorAirFlowRate() const {
      return getAutosizedValue("Design Size Evaporator Air Flow Rate", "m3/s");
    }

    void WaterHeaterHeatPump_Impl::autosize() {
      autosizeCondenserWaterFlowRate();
      autosizeEvaporatorAirFlowRate();
    }

    void WaterHeaterHeatPump_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedCondenserWaterFlowRate();
      if (val) {
        setCondenserWaterFlowRate(val.get());
      }

      val = autosizedEvaporatorAirFlowRate();
      if (val) {
        setEvaporatorAirFlowRate(val.get());
      }
    }

    // NOTE: The WaterHeaterHeatPump is listed as a ThermalZone equipment, but it's not a zone load.
    // So we set everything to None/Empty here. It has a child Tank (WaterHeater:Mixed for eg), which WILL check if it's part of a HPWH
    // so the PlantLoop's methods are affected accordingly though.
    ComponentType WaterHeaterHeatPump_Impl::componentType() const {
      return ComponentType::None;
    }

    std::vector<FuelType> WaterHeaterHeatPump_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> WaterHeaterHeatPump_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> WaterHeaterHeatPump_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  WaterHeaterHeatPump::WaterHeaterHeatPump(const Model& model, const ModelObject& dxCoil, const HVACComponent& tank, const HVACComponent& fan,
                                           Schedule& compressorSetpointTemperatureSchedule, Schedule& inletAirMixerSchedule)
    : ZoneHVACComponent(WaterHeaterHeatPump::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::WaterHeaterHeatPump_Impl>());

    setDXCoil(dxCoil);
    setTank(tank);
    setFan(fan);
    setCompressorSetpointTemperatureSchedule(compressorSetpointTemperatureSchedule);
    setInletAirMixerSchedule(inletAirMixerSchedule);

    setDeadBandTemperatureDifference(5.0);
    autosizeCondenserWaterFlowRate();
    autosizeEvaporatorAirFlowRate();
    setInletAirConfiguration("Schedule");
    setMinimumInletAirTemperatureforCompressorOperation(10.0);
    setMaximumInletAirTemperatureforCompressorOperation(48.89);
    setCompressorLocation("Schedule");
    setFanPlacement("DrawThrough");
    setOnCycleParasiticElectricLoad(0.0);
    setOffCycleParasiticElectricLoad(0.0);
    setParasiticHeatRejectionLocation("Outdoors");
    setTankElementControlLogic("Simultaneous");
    setControlSensorLocationInStratifiedTank("Heater1");
  }

  WaterHeaterHeatPump::WaterHeaterHeatPump(const Model& model) : ZoneHVACComponent(WaterHeaterHeatPump::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::WaterHeaterHeatPump_Impl>());

    CoilWaterHeatingAirToWaterHeatPump coil(model);
    setDXCoil(coil);

    WaterHeaterMixed waterHeater(model);
    setTank(waterHeater);

    FanOnOff fan(model);
    setFan(fan);

    {
      ScheduleRuleset schedule(model);
      schedule.defaultDaySchedule().addValue(Time(0, 24, 0, 0), 60.0);
      setCompressorSetpointTemperatureSchedule(schedule);
    }

    {
      ScheduleRuleset schedule(model);
      schedule.defaultDaySchedule().addValue(Time(0, 24, 0, 0), 0.2);
      setInletAirMixerSchedule(schedule);
    }

    {
      ScheduleRuleset schedule(model);
      schedule.defaultDaySchedule().addValue(Time(0, 24, 0, 0), 19.7);
      setInletAirTemperatureSchedule(schedule);
    }

    {
      ScheduleRuleset schedule(model);
      schedule.defaultDaySchedule().addValue(Time(0, 24, 0, 0), 0.5);
      setInletAirHumiditySchedule(schedule);
    }

    {
      ScheduleRuleset schedule(model);
      schedule.defaultDaySchedule().addValue(Time(0, 24, 0, 0), 21.0);
      setCompressorAmbientTemperatureSchedule(schedule);
    }

    setDeadBandTemperatureDifference(5.0);
    autosizeCondenserWaterFlowRate();
    autosizeEvaporatorAirFlowRate();
    setInletAirConfiguration("Schedule");
    setMinimumInletAirTemperatureforCompressorOperation(10.0);
    setMaximumInletAirTemperatureforCompressorOperation(48.89);
    setCompressorLocation("Schedule");
    setFanPlacement("DrawThrough");
    setOnCycleParasiticElectricLoad(0.0);
    setOffCycleParasiticElectricLoad(0.0);
    setParasiticHeatRejectionLocation("Outdoors");
    setTankElementControlLogic("Simultaneous");
    setControlSensorLocationInStratifiedTank("Heater1");
  }

  IddObjectType WaterHeaterHeatPump::iddObjectType() {
    return {IddObjectType::OS_WaterHeater_HeatPump};
  }

  std::vector<std::string> WaterHeaterHeatPump::inletAirConfigurationValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_WaterHeater_HeatPumpFields::InletAirConfiguration);
  }

  std::vector<std::string> WaterHeaterHeatPump::compressorLocationValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_WaterHeater_HeatPumpFields::CompressorLocation);
  }

  std::vector<std::string> WaterHeaterHeatPump::fanPlacementValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_WaterHeater_HeatPumpFields::FanPlacement);
  }

  std::vector<std::string> WaterHeaterHeatPump::parasiticHeatRejectionLocationValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_WaterHeater_HeatPumpFields::ParasiticHeatRejectionLocation);
  }

  std::vector<std::string> WaterHeaterHeatPump::controlSensorLocationInStratifiedTankValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                          OS_WaterHeater_HeatPumpFields::ControlSensorLocationInStratifiedTank);
  }

  std::vector<std::string> WaterHeaterHeatPump::tankElementControlLogicValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_WaterHeater_HeatPumpFields::TankElementControlLogic);
  }

  boost::optional<Schedule> WaterHeaterHeatPump::availabilitySchedule() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->availabilitySchedule();
  }

  Schedule WaterHeaterHeatPump::compressorSetpointTemperatureSchedule() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->compressorSetpointTemperatureSchedule();
  }

  double WaterHeaterHeatPump::deadBandTemperatureDifference() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->deadBandTemperatureDifference();
  }

  boost::optional<double> WaterHeaterHeatPump::condenserWaterFlowRate() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->condenserWaterFlowRate();
  }

  bool WaterHeaterHeatPump::isCondenserWaterFlowRateAutosized() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->isCondenserWaterFlowRateAutosized();
  }

  boost::optional<double> WaterHeaterHeatPump::evaporatorAirFlowRate() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->evaporatorAirFlowRate();
  }

  bool WaterHeaterHeatPump::isEvaporatorAirFlowRateAutosized() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->isEvaporatorAirFlowRateAutosized();
  }

  std::string WaterHeaterHeatPump::inletAirConfiguration() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->inletAirConfiguration();
  }

  boost::optional<Schedule> WaterHeaterHeatPump::inletAirTemperatureSchedule() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->inletAirTemperatureSchedule();
  }

  boost::optional<Schedule> WaterHeaterHeatPump::inletAirHumiditySchedule() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->inletAirHumiditySchedule();
  }

  HVACComponent WaterHeaterHeatPump::tank() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->tank();
  }

  ModelObject WaterHeaterHeatPump::dXCoil() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->dXCoil();
  }

  double WaterHeaterHeatPump::minimumInletAirTemperatureforCompressorOperation() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->minimumInletAirTemperatureforCompressorOperation();
  }

  double WaterHeaterHeatPump::maximumInletAirTemperatureforCompressorOperation() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->maximumInletAirTemperatureforCompressorOperation();
  }

  std::string WaterHeaterHeatPump::compressorLocation() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->compressorLocation();
  }

  boost::optional<Schedule> WaterHeaterHeatPump::compressorAmbientTemperatureSchedule() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->compressorAmbientTemperatureSchedule();
  }

  HVACComponent WaterHeaterHeatPump::fan() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->fan();
  }

  std::string WaterHeaterHeatPump::fanPlacement() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->fanPlacement();
  }

  double WaterHeaterHeatPump::onCycleParasiticElectricLoad() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->onCycleParasiticElectricLoad();
  }

  double WaterHeaterHeatPump::offCycleParasiticElectricLoad() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->offCycleParasiticElectricLoad();
  }

  std::string WaterHeaterHeatPump::parasiticHeatRejectionLocation() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->parasiticHeatRejectionLocation();
  }

  Schedule WaterHeaterHeatPump::inletAirMixerSchedule() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->inletAirMixerSchedule();
  }

  std::string WaterHeaterHeatPump::tankElementControlLogic() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->tankElementControlLogic();
  }

  std::string WaterHeaterHeatPump::controlSensorLocationInStratifiedTank() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->controlSensorLocationInStratifiedTank();
  }

  bool WaterHeaterHeatPump::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->setAvailabilitySchedule(schedule);
  }

  void WaterHeaterHeatPump::resetAvailabilitySchedule() {
    getImpl<detail::WaterHeaterHeatPump_Impl>()->resetAvailabilitySchedule();
  }

  bool WaterHeaterHeatPump::setCompressorSetpointTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->setCompressorSetpointTemperatureSchedule(schedule);
  }

  bool WaterHeaterHeatPump::setDeadBandTemperatureDifference(double deadBandTemperatureDifference) {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->setDeadBandTemperatureDifference(deadBandTemperatureDifference);
  }

  bool WaterHeaterHeatPump::setCondenserWaterFlowRate(double condenserWaterFlowRate) {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->setCondenserWaterFlowRate(condenserWaterFlowRate);
  }

  void WaterHeaterHeatPump::resetCondenserWaterFlowRate() {
    getImpl<detail::WaterHeaterHeatPump_Impl>()->resetCondenserWaterFlowRate();
  }

  void WaterHeaterHeatPump::autosizeCondenserWaterFlowRate() {
    getImpl<detail::WaterHeaterHeatPump_Impl>()->autosizeCondenserWaterFlowRate();
  }

  bool WaterHeaterHeatPump::setEvaporatorAirFlowRate(double evaporatorAirFlowRate) {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->setEvaporatorAirFlowRate(evaporatorAirFlowRate);
  }

  void WaterHeaterHeatPump::resetEvaporatorAirFlowRate() {
    getImpl<detail::WaterHeaterHeatPump_Impl>()->resetEvaporatorAirFlowRate();
  }

  void WaterHeaterHeatPump::autosizeEvaporatorAirFlowRate() {
    getImpl<detail::WaterHeaterHeatPump_Impl>()->autosizeEvaporatorAirFlowRate();
  }

  bool WaterHeaterHeatPump::setInletAirConfiguration(const std::string& inletAirConfiguration) {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->setInletAirConfiguration(inletAirConfiguration);
  }

  bool WaterHeaterHeatPump::setInletAirTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->setInletAirTemperatureSchedule(schedule);
  }

  void WaterHeaterHeatPump::resetInletAirTemperatureSchedule() {
    getImpl<detail::WaterHeaterHeatPump_Impl>()->resetInletAirTemperatureSchedule();
  }

  bool WaterHeaterHeatPump::setInletAirHumiditySchedule(Schedule& schedule) {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->setInletAirHumiditySchedule(schedule);
  }

  void WaterHeaterHeatPump::resetInletAirHumiditySchedule() {
    getImpl<detail::WaterHeaterHeatPump_Impl>()->resetInletAirHumiditySchedule();
  }

  bool WaterHeaterHeatPump::setTank(const HVACComponent& waterHeater) {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->setTank(waterHeater);
  }

  bool WaterHeaterHeatPump::setDXCoil(const ModelObject& coil) {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->setDXCoil(coil);
  }

  bool WaterHeaterHeatPump::setMinimumInletAirTemperatureforCompressorOperation(double minimumInletAirTemperatureforCompressorOperation) {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->setMinimumInletAirTemperatureforCompressorOperation(
      minimumInletAirTemperatureforCompressorOperation);
  }

  bool WaterHeaterHeatPump::setMaximumInletAirTemperatureforCompressorOperation(double maximumInletAirTemperatureforCompressorOperation) {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->setMaximumInletAirTemperatureforCompressorOperation(
      maximumInletAirTemperatureforCompressorOperation);
  }

  bool WaterHeaterHeatPump::setCompressorLocation(const std::string& compressorLocation) {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->setCompressorLocation(compressorLocation);
  }

  bool WaterHeaterHeatPump::setCompressorAmbientTemperatureSchedule(Schedule& schedule) {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->setCompressorAmbientTemperatureSchedule(schedule);
  }

  void WaterHeaterHeatPump::resetCompressorAmbientTemperatureSchedule() {
    getImpl<detail::WaterHeaterHeatPump_Impl>()->resetCompressorAmbientTemperatureSchedule();
  }

  bool WaterHeaterHeatPump::setFan(const HVACComponent& fan) {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->setFan(fan);
  }

  bool WaterHeaterHeatPump::setFanPlacement(const std::string& fanPlacement) {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->setFanPlacement(fanPlacement);
  }

  bool WaterHeaterHeatPump::setOnCycleParasiticElectricLoad(double onCycleParasiticElectricLoad) {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->setOnCycleParasiticElectricLoad(onCycleParasiticElectricLoad);
  }

  bool WaterHeaterHeatPump::setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad) {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->setOffCycleParasiticElectricLoad(offCycleParasiticElectricLoad);
  }

  bool WaterHeaterHeatPump::setParasiticHeatRejectionLocation(const std::string& parasiticHeatRejectionLocation) {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->setParasiticHeatRejectionLocation(parasiticHeatRejectionLocation);
  }

  bool WaterHeaterHeatPump::setInletAirMixerSchedule(Schedule& schedule) {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->setInletAirMixerSchedule(schedule);
  }

  bool WaterHeaterHeatPump::setTankElementControlLogic(const std::string& tankElementControlLogic) {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->setTankElementControlLogic(tankElementControlLogic);
  }

  bool WaterHeaterHeatPump::setControlSensorLocationInStratifiedTank(const std::string& controlSensorLocationInStratifiedTank) {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->setControlSensorLocationInStratifiedTank(controlSensorLocationInStratifiedTank);
  }

  /// @cond
  WaterHeaterHeatPump::WaterHeaterHeatPump(std::shared_ptr<detail::WaterHeaterHeatPump_Impl> impl) : ZoneHVACComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> WaterHeaterHeatPump::autosizedCondenserWaterFlowRate() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->autosizedCondenserWaterFlowRate();
  }

  boost::optional<double> WaterHeaterHeatPump::autosizedEvaporatorAirFlowRate() const {
    return getImpl<detail::WaterHeaterHeatPump_Impl>()->autosizedEvaporatorAirFlowRate();
  }

}  // namespace model
}  // namespace openstudio
