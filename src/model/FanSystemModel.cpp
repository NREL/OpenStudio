/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "FanSystemModel.hpp"
#include "FanSystemModel_Impl.hpp"

#include "Model.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"

// containing HVAC Component
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl.hpp"
//#include "AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
//#include "AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"
//#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
//#include "AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "AirTerminalSingleDuctParallelPIUReheat.hpp"
#include "AirTerminalSingleDuctParallelPIUReheat_Impl.hpp"
#include "AirTerminalSingleDuctSeriesPIUReheat.hpp"
#include "AirTerminalSingleDuctSeriesPIUReheat_Impl.hpp"

// containing ZoneHVAC Component
#include "ZoneHVACEnergyRecoveryVentilator.hpp"
#include "ZoneHVACEnergyRecoveryVentilator_Impl.hpp"
#include "ZoneHVACFourPipeFanCoil.hpp"
#include "ZoneHVACFourPipeFanCoil_Impl.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner_Impl.hpp"
#include "ZoneHVACPackagedTerminalHeatPump.hpp"
#include "ZoneHVACPackagedTerminalHeatPump_Impl.hpp"
#include "ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl.hpp"
#include "ZoneHVACUnitHeater.hpp"
#include "ZoneHVACUnitHeater_Impl.hpp"
#include "ZoneHVACUnitVentilator.hpp"
#include "ZoneHVACUnitVentilator_Impl.hpp"
#include "ZoneHVACEvaporativeCoolerUnit.hpp"
#include "ZoneHVACEvaporativeCoolerUnit_Impl.hpp"
#include "ZoneHVACWaterToAirHeatPump.hpp"
#include "ZoneHVACWaterToAirHeatPump_Impl.hpp"
// These are supposed to be ZoneHVACComponents
#include "WaterHeaterHeatPump.hpp"
#include "WaterHeaterHeatPump_Impl.hpp"
#include "WaterHeaterHeatPumpWrappedCondenser.hpp"
#include "WaterHeaterHeatPumpWrappedCondenser_Impl.hpp"

#include "AirflowNetworkFan.hpp"
#include "AirflowNetworkFan_Impl.hpp"

#include "SetpointManagerMixedAir.hpp"

#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include "../utilities/idf/IdfExtensibleGroup.hpp"
#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Fan_SystemModel_FieldEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

#include <algorithm>

namespace openstudio {
namespace model {

  FanSystemModelSpeed::FanSystemModelSpeed(double t_flowFraction) : m_flowFraction(t_flowFraction), m_electricPowerFraction(boost::none) {

    if ((m_flowFraction < 0) || (m_flowFraction > 1)) {
      LOG_AND_THROW("Unable to create FanSystemModelSpeed, Flow Fraction (" << m_flowFraction << ") is outside the range [0, 1]");
    }
  }

  FanSystemModelSpeed::FanSystemModelSpeed(double t_flowFraction, double t_electricPowerFraction)
    : m_flowFraction(t_flowFraction), m_electricPowerFraction(t_electricPowerFraction) {

    if ((m_flowFraction < 0) || (m_flowFraction > 1)) {
      LOG_AND_THROW("Unable to create FanSystemModelSpeed, Flow Fraction (" << m_flowFraction << ") is outside the range [0, 1]");
    }
    if ((t_electricPowerFraction < 0) || (t_electricPowerFraction > 1)) {
      LOG_AND_THROW("Unable to create FanSystemModelSpeed, Electric Power Fraction (" << t_electricPowerFraction << ") is outside the range [0, 1]");
    }
  }

  double FanSystemModelSpeed::flowFraction() const {
    return m_flowFraction;
  }

  boost::optional<double> FanSystemModelSpeed::electricPowerFraction() const {
    return m_electricPowerFraction;
  }

  bool FanSystemModelSpeed::operator<(const FanSystemModelSpeed& other) const {
    return (m_flowFraction < other.m_flowFraction);
  }

  std::ostream& operator<<(std::ostream& out, const openstudio::model::FanSystemModelSpeed& speed) {
    if (auto efp_ = speed.electricPowerFraction()) {
      out << "[" << speed.flowFraction() << ", " << efp_.get() << "]";
    } else {
      out << "[" << speed.flowFraction() << ", None]";
    }
    return out;
  }

  namespace detail {

    FanSystemModel_Impl::FanSystemModel_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == FanSystemModel::iddObjectType());
    }

    FanSystemModel_Impl::FanSystemModel_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == FanSystemModel::iddObjectType());
    }

    FanSystemModel_Impl::FanSystemModel_Impl(const FanSystemModel_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& FanSystemModel_Impl::outputVariableNames() const {

      static const std::vector<std::string> results{"Fan Electricity Rate", "Fan Rise in Air Temperature", "Fan Electricity Energy",
                                                    "Fan Air Mass Flow Rate"};
      return results;
    }

    IddObjectType FanSystemModel_Impl::iddObjectType() const {
      return FanSystemModel::iddObjectType();
    }

    std::vector<ScheduleTypeKey> FanSystemModel_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Fan_SystemModelFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("FanSystemModel", "Availability"));
      }
      return result;
    }

    std::vector<ModelObject> FanSystemModel_Impl::children() const {
      std::vector<ModelObject> result;
      if (boost::optional<Curve> intermediate = electricPowerFunctionofFlowFractionCurve()) {
        result.push_back(*intermediate);
      }
      return result;
    }

    Schedule FanSystemModel_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        LOG_AND_THROW(briefDescription() << " does not have an Availability Schedule attached.");
      }
      return value.get();
    }

    boost::optional<Schedule> FanSystemModel_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Fan_SystemModelFields::AvailabilityScheduleName);
    }

    boost::optional<double> FanSystemModel_Impl::designMaximumAirFlowRate() const {
      return getDouble(OS_Fan_SystemModelFields::DesignMaximumAirFlowRate, true);
    }

    bool FanSystemModel_Impl::isDesignMaximumAirFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Fan_SystemModelFields::DesignMaximumAirFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> FanSystemModel_Impl::autosizedDesignMaximumAirFlowRate() {
      return getAutosizedValue("Design Size Design Maximum Air Flow Rate", "m3/s");
    }

    std::string FanSystemModel_Impl::speedControlMethod() const {
      boost::optional<std::string> value = getString(OS_Fan_SystemModelFields::SpeedControlMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    double FanSystemModel_Impl::electricPowerMinimumFlowRateFraction() const {
      boost::optional<double> value = getDouble(OS_Fan_SystemModelFields::ElectricPowerMinimumFlowRateFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    double FanSystemModel_Impl::designPressureRise() const {
      boost::optional<double> value = getDouble(OS_Fan_SystemModelFields::DesignPressureRise, true);
      OS_ASSERT(value);
      return value.get();
    }

    double FanSystemModel_Impl::motorEfficiency() const {
      boost::optional<double> value = getDouble(OS_Fan_SystemModelFields::MotorEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    double FanSystemModel_Impl::motorInAirStreamFraction() const {
      boost::optional<double> value = getDouble(OS_Fan_SystemModelFields::MotorInAirStreamFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> FanSystemModel_Impl::designElectricPowerConsumption() const {
      return getDouble(OS_Fan_SystemModelFields::DesignElectricPowerConsumption, true);
    }

    bool FanSystemModel_Impl::isDesignElectricPowerConsumptionAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Fan_SystemModelFields::DesignElectricPowerConsumption, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    boost::optional<double> FanSystemModel_Impl::autosizedDesignElectricPowerConsumption() {
      return getAutosizedValue("Design Electric Power Consumption", "W");
    }

    std::string FanSystemModel_Impl::designPowerSizingMethod() const {
      boost::optional<std::string> value = getString(OS_Fan_SystemModelFields::DesignPowerSizingMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    double FanSystemModel_Impl::electricPowerPerUnitFlowRate() const {
      boost::optional<double> value = getDouble(OS_Fan_SystemModelFields::ElectricPowerPerUnitFlowRate, true);
      OS_ASSERT(value);
      return value.get();
    }

    double FanSystemModel_Impl::electricPowerPerUnitFlowRatePerUnitPressure() const {
      boost::optional<double> value = getDouble(OS_Fan_SystemModelFields::ElectricPowerPerUnitFlowRatePerUnitPressure, true);
      OS_ASSERT(value);
      return value.get();
    }

    double FanSystemModel_Impl::fanTotalEfficiency() const {
      boost::optional<double> value = getDouble(OS_Fan_SystemModelFields::FanTotalEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Curve> FanSystemModel_Impl::electricPowerFunctionofFlowFractionCurve() const {
      return getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_SystemModelFields::ElectricPowerFunctionofFlowFractionCurveName);
    }

    boost::optional<double> FanSystemModel_Impl::nightVentilationModePressureRise() const {
      return getDouble(OS_Fan_SystemModelFields::NightVentilationModePressureRise, true);
    }

    boost::optional<double> FanSystemModel_Impl::nightVentilationModeFlowFraction() const {
      return getDouble(OS_Fan_SystemModelFields::NightVentilationModeFlowFraction, true);
    }

    boost::optional<ThermalZone> FanSystemModel_Impl::motorLossZone() const {
      return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_Fan_SystemModelFields::MotorLossZoneName);
    }

    double FanSystemModel_Impl::motorLossRadiativeFraction() const {
      boost::optional<double> value = getDouble(OS_Fan_SystemModelFields::MotorLossRadiativeFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string FanSystemModel_Impl::endUseSubcategory() const {
      boost::optional<std::string> value = getString(OS_Fan_SystemModelFields::EndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool FanSystemModel_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Fan_SystemModelFields::AvailabilityScheduleName, "FanSystemModel", "Availability", schedule);
      return result;
    }

    unsigned FanSystemModel_Impl::inletPort() const {
      return OS_Fan_SystemModelFields::AirInletNodeName;
    }

    unsigned FanSystemModel_Impl::outletPort() const {
      return OS_Fan_SystemModelFields::AirOutletNodeName;
    }

    bool FanSystemModel_Impl::addToNode(Node& node) {
      auto oaSystem = node.airLoopHVACOutdoorAirSystem();
      auto airLoop = node.airLoopHVAC();

      if ((airLoop && airLoop->supplyComponent(node.handle())) || (oaSystem && oaSystem->component(node.handle()))) {
        if (StraightComponent_Impl::addToNode(node)) {
          if (airLoop) {
            SetpointManagerMixedAir::updateFanInletOutletNodes(airLoop.get());
          }
          return true;
        }
      }

      return false;
    }

    boost::optional<HVACComponent> FanSystemModel_Impl::containingHVACComponent() const {
      auto t_handle = handle();

      // AirLoopHVACUnitarySystem
      std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

      for (const auto& airLoopHVACUnitarySystem : airLoopHVACUnitarySystems) {
        if (boost::optional<HVACComponent> fan = airLoopHVACUnitarySystem.supplyFan()) {
          if (fan->handle() == t_handle) {
            return airLoopHVACUnitarySystem;
          }
        }
      }

      // AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass
      std::vector<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass> bypassSystems =
        this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass>();

      for (const auto& bypassSystem : bypassSystems) {
        if (boost::optional<HVACComponent> fan = bypassSystem.supplyAirFan()) {
          if (fan->handle() == t_handle) {
            return bypassSystem;
          }
        }
      }

      // TODO: Energy+.idd currently won't let that actually happen right now (cf https://github.com/NREL/EnergyPlus/issues/7697)
      // But it can't hurt to have that code in already
      // AirLoopHVACUnitaryHeatPumpAirToAir
      //std::vector<AirLoopHVACUnitaryHeatPumpAirToAir> airLoopHVACUnitaryHeatPumpAirToAirs = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAir>();

      //for( const auto & airLoopHVACUnitaryHeatPumpAirToAir : airLoopHVACUnitaryHeatPumpAirToAirs )
      //{
      //if( boost::optional<HVACComponent> fan = airLoopHVACUnitaryHeatPumpAirToAir.supplyAirFan() )
      //{
      //if( fan->handle() == this->handle() )
      //{
      //return airLoopHVACUnitaryHeatPumpAirToAir;
      //}
      //}
      //}

      // TODO: Energy+.idd currently won't let that actually happen
      // AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed
      //std::vector<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed> airLoopHVACUnitaryHeatPumpAirToAirMultiSpeeds = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed>();

      //for( const auto & airLoopHVACUnitaryHeatPumpAirToAirMultiSpeed : airLoopHVACUnitaryHeatPumpAirToAirMultiSpeeds )
      //{
      //if( boost::optional<HVACComponent> fan = airLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.supplyAirFan() )
      //{
      //if( fan->handle() == this->handle() )
      //{
      //return airLoopHVACUnitaryHeatPumpAirToAirMultiSpeed;
      //}
      //}
      //}

      // AirTerminalSingleDuctParallelPIUReheat
      std::vector<AirTerminalSingleDuctParallelPIUReheat> airTerminalSingleDuctParallelPIUReheatObjects;

      airTerminalSingleDuctParallelPIUReheatObjects = this->model().getConcreteModelObjects<AirTerminalSingleDuctParallelPIUReheat>();

      for (const auto& airTerminalSingleDuctParallelPIUReheatObject : airTerminalSingleDuctParallelPIUReheatObjects) {
        if (boost::optional<HVACComponent> fan = airTerminalSingleDuctParallelPIUReheatObject.fan()) {
          if (fan->handle() == this->handle()) {
            return airTerminalSingleDuctParallelPIUReheatObject;
          }
        }
      }

      // AirTerminalSingleDuctSeriesPIUReheat
      std::vector<AirTerminalSingleDuctSeriesPIUReheat> airTerminalSingleDuctSeriesPIUReheatObjects;

      airTerminalSingleDuctSeriesPIUReheatObjects = this->model().getConcreteModelObjects<AirTerminalSingleDuctSeriesPIUReheat>();

      for (const auto& airTerminalSingleDuctSeriesPIUReheatObject : airTerminalSingleDuctSeriesPIUReheatObjects) {
        if (boost::optional<HVACComponent> fan = airTerminalSingleDuctSeriesPIUReheatObject.fan()) {
          if (fan->handle() == this->handle()) {
            return airTerminalSingleDuctSeriesPIUReheatObject;
          }
        }
      }

      // WaterHeaterHeatPump: That's a ZoneHVAC technically...
      //{
      //auto hpwhs = model().getConcreteModelObjects<WaterHeaterHeatPump>();
      //for( const auto & hpwh : hpwhs ) {
      //if( hpwh.fan().handle() == t_handle ) {
      //return hpwh;
      //}
      //}
      //}

      return boost::none;
    }

    boost::optional<ZoneHVACComponent> FanSystemModel_Impl::containingZoneHVACComponent() const {

      std::vector<ZoneHVACComponent> zoneHVACComponent = this->model().getModelObjects<ZoneHVACComponent>();
      for (const auto& elem : zoneHVACComponent) {
        switch (elem.iddObject().type().value()) {

          // Technically these two **are** ZoneHVACComponents
          case openstudio::IddObjectType::OS_WaterHeater_HeatPump: {
            auto component = elem.cast<WaterHeaterHeatPump>();
            if (component.fan().handle() == this->handle()) {
              return elem;
            }
            break;
          }
          case openstudio::IddObjectType::OS_WaterHeater_HeatPump_WrappedCondenser: {
            auto component = elem.cast<WaterHeaterHeatPumpWrappedCondenser>();
            if (component.fan().handle() == this->handle()) {
              return elem;
            }
            break;
          }

          case openstudio::IddObjectType::OS_ZoneHVAC_EnergyRecoveryVentilator: {
            auto component = elem.cast<ZoneHVACEnergyRecoveryVentilator>();
            if (component.supplyAirFan().handle() == this->handle()) {
              return elem;
            }
            break;
          }
          case openstudio::IddObjectType::OS_ZoneHVAC_FourPipeFanCoil: {
            auto component = elem.cast<ZoneHVACFourPipeFanCoil>();
            if (component.supplyAirFan().handle() == this->handle()) {
              return elem;
            }
            break;
          }
          case openstudio::IddObjectType::OS_ZoneHVAC_PackagedTerminalAirConditioner: {
            auto component = elem.cast<ZoneHVACPackagedTerminalAirConditioner>();
            if (component.supplyAirFan().handle() == this->handle()) {
              return elem;
            }
            break;
          }
          case openstudio::IddObjectType::OS_ZoneHVAC_PackagedTerminalHeatPump: {
            auto component = elem.cast<ZoneHVACPackagedTerminalHeatPump>();
            if (component.supplyAirFan().handle() == this->handle()) {
              return elem;
            }
            break;
          }
          case openstudio::IddObjectType::OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlow: {
            auto component = elem.cast<ZoneHVACTerminalUnitVariableRefrigerantFlow>();
            if (component.supplyAirFan().handle() == this->handle()) {
              return elem;
            }
            break;
          }
          case openstudio::IddObjectType::OS_ZoneHVAC_UnitHeater: {
            auto component = elem.cast<ZoneHVACUnitHeater>();
            if (component.supplyAirFan().handle() == this->handle()) {
              return elem;
            }
            break;
          }
          case openstudio::IddObjectType::OS_ZoneHVAC_UnitVentilator: {
            auto component = elem.cast<ZoneHVACUnitVentilator>();
            if (component.supplyAirFan().handle() == this->handle()) {
              return elem;
            }
            break;
          }
          case openstudio::IddObjectType::OS_ZoneHVAC_EvaporativeCoolerUnit: {
            auto component = elem.cast<ZoneHVACEvaporativeCoolerUnit>();
            if (component.supplyAirFan().handle() == this->handle()) {
              return elem;
            }
            break;
          }
          case openstudio::IddObjectType::OS_ZoneHVAC_WaterToAirHeatPump: {
            auto component = elem.cast<ZoneHVACWaterToAirHeatPump>();
            if (component.supplyAirFan().handle() == this->handle()) {
              return elem;
            }
            break;
          }
          default: {
            break;
          }
        }
      }
      return boost::none;
    }

    bool FanSystemModel_Impl::setDesignMaximumAirFlowRate(double designMaximumAirFlowRate) {
      bool result = setDouble(OS_Fan_SystemModelFields::DesignMaximumAirFlowRate, designMaximumAirFlowRate);
      return result;
    }

    void FanSystemModel_Impl::autosizeDesignMaximumAirFlowRate() {
      bool result = setString(OS_Fan_SystemModelFields::DesignMaximumAirFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool FanSystemModel_Impl::setSpeedControlMethod(const std::string& speedControlMethod) {
      bool result = setString(OS_Fan_SystemModelFields::SpeedControlMethod, speedControlMethod);
      return result;
    }

    bool FanSystemModel_Impl::setElectricPowerMinimumFlowRateFraction(double electricPowerMinimumFlowRateFraction) {
      bool result = setDouble(OS_Fan_SystemModelFields::ElectricPowerMinimumFlowRateFraction, electricPowerMinimumFlowRateFraction);
      return result;
    }

    bool FanSystemModel_Impl::setDesignPressureRise(double designPressureRise) {
      bool result = setDouble(OS_Fan_SystemModelFields::DesignPressureRise, designPressureRise);
      return result;
    }

    bool FanSystemModel_Impl::setMotorEfficiency(double motorEfficiency) {
      bool result = setDouble(OS_Fan_SystemModelFields::MotorEfficiency, motorEfficiency);
      return result;
    }

    bool FanSystemModel_Impl::setMotorInAirStreamFraction(double motorInAirStreamFraction) {
      bool result = setDouble(OS_Fan_SystemModelFields::MotorInAirStreamFraction, motorInAirStreamFraction);
      return result;
    }

    bool FanSystemModel_Impl::setDesignElectricPowerConsumption(double designElectricPowerConsumption) {
      bool result = setDouble(OS_Fan_SystemModelFields::DesignElectricPowerConsumption, designElectricPowerConsumption);
      // OS_ASSERT(result); // shouldn't accept < 0
      return result;
    }

    void FanSystemModel_Impl::autosizeDesignElectricPowerConsumption() {
      bool result = setString(OS_Fan_SystemModelFields::DesignElectricPowerConsumption, "autosize");
      OS_ASSERT(result);
    }

    bool FanSystemModel_Impl::setDesignPowerSizingMethod(const std::string& designPowerSizingMethod) {
      bool result = setString(OS_Fan_SystemModelFields::DesignPowerSizingMethod, designPowerSizingMethod);
      return result;
    }

    bool FanSystemModel_Impl::setElectricPowerPerUnitFlowRate(double electricPowerPerUnitFlowRate) {
      bool result = setDouble(OS_Fan_SystemModelFields::ElectricPowerPerUnitFlowRate, electricPowerPerUnitFlowRate);
      // OS_ASSERT(result); // shouldn't accept < 0
      return result;
    }

    bool FanSystemModel_Impl::setElectricPowerPerUnitFlowRatePerUnitPressure(double electricPowerPerUnitFlowRatePerUnitPressure) {
      bool result = setDouble(OS_Fan_SystemModelFields::ElectricPowerPerUnitFlowRatePerUnitPressure, electricPowerPerUnitFlowRatePerUnitPressure);
      // OS_ASSERT(result); // shouldn't accept < 0
      return result;
    }

    bool FanSystemModel_Impl::setFanTotalEfficiency(double fanTotalEfficiency) {
      bool result = setDouble(OS_Fan_SystemModelFields::FanTotalEfficiency, fanTotalEfficiency);
      return result;
    }

    bool FanSystemModel_Impl::setElectricPowerFunctionofFlowFractionCurve(const Curve& curve) {
      bool result = setPointer(OS_Fan_SystemModelFields::ElectricPowerFunctionofFlowFractionCurveName, curve.handle());
      return result;
    }

    void FanSystemModel_Impl::resetElectricPowerFunctionofFlowFractionCurve() {
      bool result = setString(OS_Fan_SystemModelFields::ElectricPowerFunctionofFlowFractionCurveName, "");
      OS_ASSERT(result);
    }

    bool FanSystemModel_Impl::setNightVentilationModePressureRise(double nightVentilationModePressureRise) {
      bool result = setDouble(OS_Fan_SystemModelFields::NightVentilationModePressureRise, nightVentilationModePressureRise);
      // OS_ASSERT(result); // shouldn't accept < 0
      return result;
    }

    void FanSystemModel_Impl::resetNightVentilationModePressureRise() {
      bool result = setString(OS_Fan_SystemModelFields::NightVentilationModePressureRise, "");
      OS_ASSERT(result);
    }

    bool FanSystemModel_Impl::setNightVentilationModeFlowFraction(double nightVentilationModeFlowFraction) {
      bool result = setDouble(OS_Fan_SystemModelFields::NightVentilationModeFlowFraction, nightVentilationModeFlowFraction);
      return result;
    }

    void FanSystemModel_Impl::resetNightVentilationModeFlowFraction() {
      bool result = setString(OS_Fan_SystemModelFields::NightVentilationModeFlowFraction, "");
      OS_ASSERT(result);
    }

    bool FanSystemModel_Impl::setMotorLossZone(const ThermalZone& thermalZone) {
      bool result = setPointer(OS_Fan_SystemModelFields::MotorLossZoneName, thermalZone.handle());
      return result;
    }

    void FanSystemModel_Impl::resetMotorLossZone() {
      bool result = setString(OS_Fan_SystemModelFields::MotorLossZoneName, "");
      OS_ASSERT(result);
    }

    bool FanSystemModel_Impl::setMotorLossRadiativeFraction(double motorLossRadiativeFraction) {
      bool result = setDouble(OS_Fan_SystemModelFields::MotorLossRadiativeFraction, motorLossRadiativeFraction);
      return result;
    }

    bool FanSystemModel_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
      bool result = setString(OS_Fan_SystemModelFields::EndUseSubcategory, endUseSubcategory);
      OS_ASSERT(result);
      return result;
    }

    //bool FanSystemModel_Impl::setNumberofSpeeds(int numberofSpeeds) {
    //bool result = setInt(OS_Fan_SystemModelFields::NumberofSpeeds, numberofSpeeds);
    //// OS_ASSERT(result); // shouldn't accept < 0
    //return result;
    //}

    unsigned FanSystemModel_Impl::numberofSpeeds() const {
      // number of different speed levels available when Speed Control Method is set to Discrete
      // note Speed need to be arranged in increasing order in remaining field sets.
      // If set to 1, or omitted, and Speed Control Method is Discrete then constant fan speed is the design maximum.
      return std::max<unsigned>(1, numExtensibleGroups());
    }

    boost::optional<unsigned> FanSystemModel_Impl::speedIndex(const FanSystemModelSpeed& t_speed) const {
      boost::optional<unsigned> result;

      // We do it with extensibleGroups() (rather than speeds()) and getString to avoid overhead
      // of manipulating actual model objects and speed up the routine
      auto egs = castVector<WorkspaceExtensibleGroup>(extensibleGroups());
      auto flowFraction = toString(t_speed.flowFraction());
      auto it = std::find_if(egs.begin(), egs.end(), [&](const WorkspaceExtensibleGroup& eg) {
        return (eg.getField(OS_Fan_SystemModelExtensibleFields::SpeedFlowFraction).get() == flowFraction);
      });

      // If found, we compute the index by using std::distance between the start of vector and the iterator returned by std::find_if
      if (it != egs.end()) {
        result = std::distance(egs.begin(), it);
      }

      return result;
    }

    std::vector<FanSystemModelSpeed> FanSystemModel_Impl::speeds() const {

      std::vector<FanSystemModelSpeed> result;

      for (const auto& group : extensibleGroups()) {
        boost::optional<double> _flowFraction = group.getDouble(OS_Fan_SystemModelExtensibleFields::SpeedFlowFraction);
        boost::optional<double> _electricPowerFraction = group.getDouble(OS_Fan_SystemModelExtensibleFields::SpeedElectricPowerFraction);
        if (_flowFraction.has_value()) {
          if (_electricPowerFraction.has_value()) {
            result.push_back(FanSystemModelSpeed(_flowFraction.get(), _electricPowerFraction.get()));
          } else {
            result.push_back(FanSystemModelSpeed(_flowFraction.get()));
          }
        } else {
          // Shouldn't happen
          OS_ASSERT(false);
        }
      }

      return result;
    }

    boost::optional<FanSystemModelSpeed> FanSystemModel_Impl::getSpeed(unsigned speedIndex) const {

      boost::optional<FanSystemModelSpeed> result;

      unsigned n = numberofSpeeds();
      if (speedIndex >= n) {
        LOG(Error, "Asked to get FanSystemModelSpeed with index " << speedIndex << ", but " << briefDescription() << " has just " << n << " Speeds.");
        return result;
      }

      IdfExtensibleGroup group = getExtensibleGroup(speedIndex);
      boost::optional<double> _flowFraction = group.getDouble(OS_Fan_SystemModelExtensibleFields::SpeedFlowFraction);
      boost::optional<double> _electricPowerFraction = group.getDouble(OS_Fan_SystemModelExtensibleFields::SpeedElectricPowerFraction);
      if (_flowFraction.has_value()) {
        if (_electricPowerFraction.has_value()) {
          result = FanSystemModelSpeed(_flowFraction.get(), _electricPowerFraction.get());
        } else {
          result = FanSystemModelSpeed(_flowFraction.get());
        }
      } else {
        // Shouldn't happen
        OS_ASSERT(false);
      }

      return result;
    }

    bool FanSystemModel_Impl::addSpeedPrivate(double flowFraction, boost::optional<double> electricPowerFraction) {
      bool result;
      // Push an extensible group
      auto eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
      bool temp = eg.setDouble(OS_Fan_SystemModelExtensibleFields::SpeedFlowFraction, flowFraction);
      bool ok = electricPowerFraction.is_initialized()
                  ? eg.setDouble(OS_Fan_SystemModelExtensibleFields::SpeedElectricPowerFraction, electricPowerFraction.get())
                  : true;
      if (temp && ok) {
        result = true;
      } else {
        // Something went wrong, which shouldn't happen really.
        getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
        result = false;
      }
      return result;
    }

    bool FanSystemModel_Impl::addSpeed(const FanSystemModelSpeed& speed) {
      // Speeds need to be sorted...
      std::vector<FanSystemModelSpeed> s = speeds();
      s.push_back(speed);
      return setSpeeds(s);
    }

    bool FanSystemModel_Impl::addSpeed(double flowFraction) {
      return addSpeed(FanSystemModelSpeed(flowFraction));
    }

    // Overloads, it creates a FanSystemModelSpeed wrapper, then call `addSpeed(const FanSystemModelSpeed&)`
    bool FanSystemModel_Impl::addSpeed(double flowFraction, double electricPowerFraction) {
      // Make a speed (which will check for validity), and then call the above function
      return addSpeed(FanSystemModelSpeed(flowFraction, electricPowerFraction));
    }

    bool FanSystemModel_Impl::removeSpeed(unsigned speedIndex) {
      bool result;

      unsigned n = numberofSpeeds();
      if (speedIndex < n) {
        getObject<ModelObject>().eraseExtensibleGroup(speedIndex);
        result = true;
      } else {
        result = false;
      }
      return result;
    }

    void FanSystemModel_Impl::removeAllSpeeds() {
      getObject<ModelObject>().clearExtensibleGroups();
    }

    // Directly set the speeds from a vector, will delete any existing speeds, will sort the speeds by flow fraction too as required
    bool FanSystemModel_Impl::setSpeeds(const std::vector<FanSystemModelSpeed>& speeds) {

      // Ok, now we clear
      this->clearExtensibleGroups();

      std::vector<std::pair<double, boost::optional<double>>> speedVecPair;
      for (const FanSystemModelSpeed& sp : speeds) {
        speedVecPair.push_back(std::make_pair(sp.flowFraction(), sp.electricPowerFraction()));
      }
      std::sort(speedVecPair.begin(), speedVecPair.end());
      bool result = true;
      for (const auto& speed : speedVecPair) {
        result &= addSpeedPrivate(speed.first, speed.second);
      }

      if (!electricPowerFunctionofFlowFractionCurve()) {

        // We do it with extensibleGroups() (rather than speeds()) and isEmpty to avoid overhead
        // of manipulating actual model objects and speed up the routine
        auto egs = castVector<WorkspaceExtensibleGroup>(extensibleGroups());
        auto hasBlankEPF = [](const WorkspaceExtensibleGroup& eg) {
          return eg.isEmpty(OS_Fan_SystemModelExtensibleFields::SpeedElectricPowerFraction);
        };
        if (std::any_of(egs.begin(), egs.end(), hasBlankEPF)) {
          LOG(Warn, "For" << briefDescription() << ", you have speeds with blank ElectricPowerFraction "
                          << "but you did not assign an Electric Power Function of Flow Fraction Curve.");
        }
      }

      return result;
    }

    void FanSystemModel_Impl::autosize() {
      autosizedDesignMaximumAirFlowRate();
      autosizedDesignElectricPowerConsumption();
    }

    void FanSystemModel_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedDesignMaximumAirFlowRate();
      if (val) {
        setDesignMaximumAirFlowRate(val.get());
      }

      val = autosizedDesignElectricPowerConsumption();
      if (val) {
        setDesignElectricPowerConsumption(val.get());
      }
    }

    AirflowNetworkFan FanSystemModel_Impl::getAirflowNetworkFan() {
      auto opt = airflowNetworkFan();
      if (opt) {
        return opt.get();
      }
      return AirflowNetworkFan(model(), handle());
    }

    boost::optional<AirflowNetworkFan> FanSystemModel_Impl::airflowNetworkFan() const {
      std::vector<AirflowNetworkFan> myAFNitems =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkFan>(AirflowNetworkFan::iddObjectType());
      auto count = myAFNitems.size();
      if (count == 1) {
        return myAFNitems[0];
      } else if (count > 1) {
        LOG(Warn, briefDescription() << " has more than one AirflowNetworkFan attached, returning first.");
        return myAFNitems[0];
      }
      return boost::none;
    }

    ComponentType FanSystemModel_Impl::componentType() const {
      return ComponentType::None;
    }

    std::vector<FuelType> FanSystemModel_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> FanSystemModel_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> FanSystemModel_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  FanSystemModel::FanSystemModel(const Model& model) : StraightComponent(FanSystemModel::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::FanSystemModel_Impl>());

    bool ok = true;

    auto availabilitySchedule = model.alwaysOnDiscreteSchedule();
    ok = setAvailabilitySchedule(availabilitySchedule);
    OS_ASSERT(ok);

    autosizeDesignMaximumAirFlowRate();

    ok = setDesignPressureRise(500.0);  // Completely abritrary
    OS_ASSERT(ok);
    ok = setSpeedControlMethod("Discrete");
    OS_ASSERT(ok);
    ok = setElectricPowerMinimumFlowRateFraction(0.2);
    OS_ASSERT(ok);
    ok = setMotorEfficiency(0.9);
    OS_ASSERT(ok);
    ok = setMotorInAirStreamFraction(1.0);
    OS_ASSERT(ok);

    autosizeDesignElectricPowerConsumption();

    ok = setElectricPowerPerUnitFlowRate(840.0);  // TODO
    OS_ASSERT(ok);
    ok = setElectricPowerPerUnitFlowRatePerUnitPressure(1.66667);
    OS_ASSERT(ok);
    ok = setDesignPowerSizingMethod("PowerPerFlowPerPressure");
    OS_ASSERT(ok);
    ok = setFanTotalEfficiency(0.7);
    OS_ASSERT(ok);
    ok = setMotorLossRadiativeFraction(0.0);
    OS_ASSERT(ok);
    ok = setEndUseSubcategory("General");
    OS_ASSERT(ok);
  }

  IddObjectType FanSystemModel::iddObjectType() {
    return {IddObjectType::OS_Fan_SystemModel};
  }

  std::vector<std::string> FanSystemModel::speedControlMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Fan_SystemModelFields::SpeedControlMethod);
  }

  std::vector<std::string> FanSystemModel::designPowerSizingMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Fan_SystemModelFields::DesignPowerSizingMethod);
  }

  Schedule FanSystemModel::availabilitySchedule() const {
    return getImpl<detail::FanSystemModel_Impl>()->availabilitySchedule();
  }

  boost::optional<double> FanSystemModel::designMaximumAirFlowRate() const {
    return getImpl<detail::FanSystemModel_Impl>()->designMaximumAirFlowRate();
  }

  bool FanSystemModel::isDesignMaximumAirFlowRateAutosized() const {
    return getImpl<detail::FanSystemModel_Impl>()->isDesignMaximumAirFlowRateAutosized();
  }

  boost::optional<double> FanSystemModel::autosizedDesignMaximumAirFlowRate() {
    return getImpl<detail::FanSystemModel_Impl>()->autosizedDesignMaximumAirFlowRate();
  }

  std::string FanSystemModel::speedControlMethod() const {
    return getImpl<detail::FanSystemModel_Impl>()->speedControlMethod();
  }

  double FanSystemModel::electricPowerMinimumFlowRateFraction() const {
    return getImpl<detail::FanSystemModel_Impl>()->electricPowerMinimumFlowRateFraction();
  }

  double FanSystemModel::designPressureRise() const {
    return getImpl<detail::FanSystemModel_Impl>()->designPressureRise();
  }

  double FanSystemModel::motorEfficiency() const {
    return getImpl<detail::FanSystemModel_Impl>()->motorEfficiency();
  }

  double FanSystemModel::motorInAirStreamFraction() const {
    return getImpl<detail::FanSystemModel_Impl>()->motorInAirStreamFraction();
  }

  boost::optional<double> FanSystemModel::designElectricPowerConsumption() const {
    return getImpl<detail::FanSystemModel_Impl>()->designElectricPowerConsumption();
  }

  bool FanSystemModel::isDesignElectricPowerConsumptionAutosized() const {
    return getImpl<detail::FanSystemModel_Impl>()->isDesignElectricPowerConsumptionAutosized();
  }

  boost::optional<double> FanSystemModel::autosizedDesignElectricPowerConsumption() {
    return getImpl<detail::FanSystemModel_Impl>()->autosizedDesignElectricPowerConsumption();
  }

  std::string FanSystemModel::designPowerSizingMethod() const {
    return getImpl<detail::FanSystemModel_Impl>()->designPowerSizingMethod();
  }

  double FanSystemModel::electricPowerPerUnitFlowRate() const {
    return getImpl<detail::FanSystemModel_Impl>()->electricPowerPerUnitFlowRate();
  }

  double FanSystemModel::electricPowerPerUnitFlowRatePerUnitPressure() const {
    return getImpl<detail::FanSystemModel_Impl>()->electricPowerPerUnitFlowRatePerUnitPressure();
  }

  double FanSystemModel::fanTotalEfficiency() const {
    return getImpl<detail::FanSystemModel_Impl>()->fanTotalEfficiency();
  }

  boost::optional<Curve> FanSystemModel::electricPowerFunctionofFlowFractionCurve() const {
    return getImpl<detail::FanSystemModel_Impl>()->electricPowerFunctionofFlowFractionCurve();
  }

  boost::optional<double> FanSystemModel::nightVentilationModePressureRise() const {
    return getImpl<detail::FanSystemModel_Impl>()->nightVentilationModePressureRise();
  }

  boost::optional<double> FanSystemModel::nightVentilationModeFlowFraction() const {
    return getImpl<detail::FanSystemModel_Impl>()->nightVentilationModeFlowFraction();
  }

  boost::optional<ThermalZone> FanSystemModel::motorLossZone() const {
    return getImpl<detail::FanSystemModel_Impl>()->motorLossZone();
  }

  double FanSystemModel::motorLossRadiativeFraction() const {
    return getImpl<detail::FanSystemModel_Impl>()->motorLossRadiativeFraction();
  }

  std::string FanSystemModel::endUseSubcategory() const {
    return getImpl<detail::FanSystemModel_Impl>()->endUseSubcategory();
  }

  bool FanSystemModel::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::FanSystemModel_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool FanSystemModel::setDesignMaximumAirFlowRate(double designMaximumAirFlowRate) {
    return getImpl<detail::FanSystemModel_Impl>()->setDesignMaximumAirFlowRate(designMaximumAirFlowRate);
  }

  void FanSystemModel::autosizeDesignMaximumAirFlowRate() {
    getImpl<detail::FanSystemModel_Impl>()->autosizeDesignMaximumAirFlowRate();
  }

  bool FanSystemModel::setSpeedControlMethod(const std::string& speedControlMethod) {
    return getImpl<detail::FanSystemModel_Impl>()->setSpeedControlMethod(speedControlMethod);
  }

  bool FanSystemModel::setElectricPowerMinimumFlowRateFraction(double electricPowerMinimumFlowRateFraction) {
    return getImpl<detail::FanSystemModel_Impl>()->setElectricPowerMinimumFlowRateFraction(electricPowerMinimumFlowRateFraction);
  }

  bool FanSystemModel::setDesignPressureRise(double designPressureRise) {
    return getImpl<detail::FanSystemModel_Impl>()->setDesignPressureRise(designPressureRise);
  }

  bool FanSystemModel::setMotorEfficiency(double motorEfficiency) {
    return getImpl<detail::FanSystemModel_Impl>()->setMotorEfficiency(motorEfficiency);
  }

  bool FanSystemModel::setMotorInAirStreamFraction(double motorInAirStreamFraction) {
    return getImpl<detail::FanSystemModel_Impl>()->setMotorInAirStreamFraction(motorInAirStreamFraction);
  }

  bool FanSystemModel::setDesignElectricPowerConsumption(double designElectricPowerConsumption) {
    return getImpl<detail::FanSystemModel_Impl>()->setDesignElectricPowerConsumption(designElectricPowerConsumption);
  }

  void FanSystemModel::autosizeDesignElectricPowerConsumption() {
    getImpl<detail::FanSystemModel_Impl>()->autosizeDesignElectricPowerConsumption();
  }

  bool FanSystemModel::setDesignPowerSizingMethod(const std::string& designPowerSizingMethod) {
    return getImpl<detail::FanSystemModel_Impl>()->setDesignPowerSizingMethod(designPowerSizingMethod);
  }

  bool FanSystemModel::setElectricPowerPerUnitFlowRate(double electricPowerPerUnitFlowRate) {
    return getImpl<detail::FanSystemModel_Impl>()->setElectricPowerPerUnitFlowRate(electricPowerPerUnitFlowRate);
  }

  bool FanSystemModel::setElectricPowerPerUnitFlowRatePerUnitPressure(double electricPowerPerUnitFlowRatePerUnitPressure) {
    return getImpl<detail::FanSystemModel_Impl>()->setElectricPowerPerUnitFlowRatePerUnitPressure(electricPowerPerUnitFlowRatePerUnitPressure);
  }

  bool FanSystemModel::setFanTotalEfficiency(double fanTotalEfficiency) {
    return getImpl<detail::FanSystemModel_Impl>()->setFanTotalEfficiency(fanTotalEfficiency);
  }

  bool FanSystemModel::setElectricPowerFunctionofFlowFractionCurve(const Curve& curve) {
    return getImpl<detail::FanSystemModel_Impl>()->setElectricPowerFunctionofFlowFractionCurve(curve);
  }

  void FanSystemModel::resetElectricPowerFunctionofFlowFractionCurve() {
    getImpl<detail::FanSystemModel_Impl>()->resetElectricPowerFunctionofFlowFractionCurve();
  }

  bool FanSystemModel::setNightVentilationModePressureRise(double nightVentilationModePressureRise) {
    return getImpl<detail::FanSystemModel_Impl>()->setNightVentilationModePressureRise(nightVentilationModePressureRise);
  }

  void FanSystemModel::resetNightVentilationModePressureRise() {
    getImpl<detail::FanSystemModel_Impl>()->resetNightVentilationModePressureRise();
  }

  bool FanSystemModel::setNightVentilationModeFlowFraction(double nightVentilationModeFlowFraction) {
    return getImpl<detail::FanSystemModel_Impl>()->setNightVentilationModeFlowFraction(nightVentilationModeFlowFraction);
  }

  void FanSystemModel::resetNightVentilationModeFlowFraction() {
    getImpl<detail::FanSystemModel_Impl>()->resetNightVentilationModeFlowFraction();
  }

  bool FanSystemModel::setMotorLossZone(const ThermalZone& thermalZone) {
    return getImpl<detail::FanSystemModel_Impl>()->setMotorLossZone(thermalZone);
  }

  void FanSystemModel::resetMotorLossZone() {
    getImpl<detail::FanSystemModel_Impl>()->resetMotorLossZone();
  }

  bool FanSystemModel::setMotorLossRadiativeFraction(double motorLossRadiativeFraction) {
    return getImpl<detail::FanSystemModel_Impl>()->setMotorLossRadiativeFraction(motorLossRadiativeFraction);
  }

  bool FanSystemModel::setEndUseSubcategory(const std::string& endUseSubcategory) {
    return getImpl<detail::FanSystemModel_Impl>()->setEndUseSubcategory(endUseSubcategory);
  }

  //bool FanSystemModel::setNumberofSpeeds(int numberofSpeeds) {
  //return getImpl<detail::FanSystemModel_Impl>()->setNumberofSpeeds(numberofSpeeds);
  //}

  unsigned FanSystemModel::numberofSpeeds() const {
    return getImpl<detail::FanSystemModel_Impl>()->numberofSpeeds();
  }

  std::vector<FanSystemModelSpeed> FanSystemModel::speeds() const {
    return getImpl<detail::FanSystemModel_Impl>()->speeds();
  }

  boost::optional<unsigned> FanSystemModel::speedIndex(const FanSystemModelSpeed& speed) const {
    return getImpl<detail::FanSystemModel_Impl>()->speedIndex(speed);
  }

  boost::optional<FanSystemModelSpeed> FanSystemModel::getSpeed(unsigned speedIndex) const {
    return getImpl<detail::FanSystemModel_Impl>()->getSpeed(speedIndex);
  }

  bool FanSystemModel::addSpeed(const FanSystemModelSpeed& speed) {
    return getImpl<detail::FanSystemModel_Impl>()->addSpeed(speed);
  }

  // Overloads, it creates a FanSystemModelSpeed wrapper, then call `addSpeed(const FanSystemModelSpeed&)`
  bool FanSystemModel::addSpeed(double flowFraction) {
    return getImpl<detail::FanSystemModel_Impl>()->addSpeed(flowFraction);
  }

  bool FanSystemModel::addSpeed(double flowFraction, double electricPowerFraction) {
    return getImpl<detail::FanSystemModel_Impl>()->addSpeed(flowFraction, electricPowerFraction);
  }

  bool FanSystemModel::removeSpeed(unsigned speedIndex) {
    return getImpl<detail::FanSystemModel_Impl>()->removeSpeed(speedIndex);
  }

  void FanSystemModel::removeAllSpeeds() {
    getImpl<detail::FanSystemModel_Impl>()->removeAllSpeeds();
  }

  bool FanSystemModel::setSpeeds(const std::vector<FanSystemModelSpeed>& speeds) {
    return getImpl<detail::FanSystemModel_Impl>()->setSpeeds(speeds);
  }

  AirflowNetworkFan FanSystemModel::getAirflowNetworkFan() {
    return getImpl<detail::FanSystemModel_Impl>()->getAirflowNetworkFan();
  }

  boost::optional<AirflowNetworkFan> FanSystemModel::airflowNetworkFan() const {
    return getImpl<detail::FanSystemModel_Impl>()->airflowNetworkFan();
  }

  /// @cond
  FanSystemModel::FanSystemModel(std::shared_ptr<detail::FanSystemModel_Impl> impl) : StraightComponent(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
