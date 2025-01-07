/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "FanOnOff.hpp"
#include "FanOnOff_Impl.hpp"
#include "WaterHeaterHeatPump.hpp"
#include "WaterHeaterHeatPump_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"
#include "Loop.hpp"
#include "StraightComponent.hpp"
#include "StraightComponent_Impl.hpp"
#include "Curve.hpp"
#include "Curve_Impl.hpp"
#include "CurveCubic.hpp"
#include "CurveCubic_Impl.hpp"
#include "CurveExponent.hpp"
#include "CurveExponent_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "ZoneHVACWaterToAirHeatPump.hpp"
#include "ZoneHVACWaterToAirHeatPump_Impl.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner.hpp"
#include "ZoneHVACPackagedTerminalAirConditioner_Impl.hpp"
#include "ZoneHVACFourPipeFanCoil.hpp"
#include "ZoneHVACFourPipeFanCoil_Impl.hpp"
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
#include "AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl.hpp"
#include "AirflowNetworkFan.hpp"
#include "AirflowNetworkFan_Impl.hpp"
#include <utilities/idd/OS_Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    FanOnOff_Impl::FanOnOff_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == FanOnOff::iddObjectType());
    }

    FanOnOff_Impl::FanOnOff_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == FanOnOff::iddObjectType());
    }

    FanOnOff_Impl::FanOnOff_Impl(const FanOnOff_Impl& other, Model_Impl* model, bool keepHandle) : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& FanOnOff_Impl::outputVariableNames() const {
      static const std::vector<std::string> results{"Fan Electricity Rate",   "Fan Rise in Air Temperature", "Fan Heat Gain to Air",
                                                    "Fan Electricity Energy", "Fan Air Mass Flow Rate",      "Fan Runtime Fraction"};
      return results;
    }

    IddObjectType FanOnOff_Impl::iddObjectType() const {
      return FanOnOff::iddObjectType();
    }

    std::vector<openstudio::IdfObject> FanOnOff_Impl::remove() {
      return StraightComponent_Impl::remove();
    }

    std::vector<ModelObject> FanOnOff_Impl::children() const {
      std::vector<ModelObject> result;
      if (boost::optional<Curve> intermediate = fanPowerRatioFunctionofSpeedRatioCurve()) {
        result.push_back(*intermediate);
      }
      if (boost::optional<Curve> intermediate = fanEfficiencyRatioFunctionofSpeedRatioCurve()) {
        result.push_back(*intermediate);
      }
      return result;
    }

    // Inlet and Outlet nodes

    unsigned FanOnOff_Impl::inletPort() const {
      return OS_Fan_OnOffFields::AirInletNodeName;
    }

    unsigned FanOnOff_Impl::outletPort() const {
      return OS_Fan_OnOffFields::AirOutletNodeName;
    }

    std::vector<ScheduleTypeKey> FanOnOff_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Fan_OnOffFields::AvailabilityScheduleName) != e) {
        result.emplace_back("FanOnOff", "Availability");
      }
      return result;
    }

    Schedule FanOnOff_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value = optionalAvailabilitySchedule();
      if (!value) {
        // it is an error if we get here, however we don't want to crash
        // so we hook up to global always on schedule
        LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
        value = this->model().alwaysOnDiscreteSchedule();
        OS_ASSERT(value);
        const_cast<FanOnOff_Impl*>(this)->setAvailabilitySchedule(*value);
        value = optionalAvailabilitySchedule();
      }
      OS_ASSERT(value);
      return value.get();
    }

    bool FanOnOff_Impl::setAvailabilitySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_Fan_OnOffFields::AvailabilityScheduleName, "FanOnOff", "Availability", schedule);
      return result;
    }

    double FanOnOff_Impl::fanTotalEfficiency() const {
      boost::optional<double> value = getDouble(OS_Fan_OnOffFields::FanTotalEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool FanOnOff_Impl::isFanTotalEfficiencyDefaulted() const {
      return isEmpty(OS_Fan_OnOffFields::FanTotalEfficiency);
    }

    double FanOnOff_Impl::pressureRise() const {
      boost::optional<double> value = getDouble(OS_Fan_OnOffFields::PressureRise, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> FanOnOff_Impl::maximumFlowRate() const {
      return getDouble(OS_Fan_OnOffFields::MaximumFlowRate, true);
    }

    bool FanOnOff_Impl::isMaximumFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Fan_OnOffFields::MaximumFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "autosize");
      }
      return result;
    }

    double FanOnOff_Impl::motorEfficiency() const {
      boost::optional<double> value = getDouble(OS_Fan_OnOffFields::MotorEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool FanOnOff_Impl::isMotorEfficiencyDefaulted() const {
      return isEmpty(OS_Fan_OnOffFields::MotorEfficiency);
    }

    boost::optional<double> FanOnOff_Impl::motorInAirstreamFraction() const {
      return getDouble(OS_Fan_OnOffFields::MotorInAirstreamFraction, true);
    }

    std::string FanOnOff_Impl::endUseSubcategory() const {
      boost::optional<std::string> value = getString(OS_Fan_OnOffFields::EndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool FanOnOff_Impl::isEndUseSubcategoryDefaulted() const {
      return isEmpty(OS_Fan_OnOffFields::EndUseSubcategory);
    }

    bool FanOnOff_Impl::setFanTotalEfficiency(double fanTotalEfficiency) {
      bool result = setDouble(OS_Fan_OnOffFields::FanTotalEfficiency, fanTotalEfficiency);
      return result;
    }

    void FanOnOff_Impl::resetFanTotalEfficiency() {
      bool result = setString(OS_Fan_OnOffFields::FanTotalEfficiency, "");
      OS_ASSERT(result);
    }

    bool FanOnOff_Impl::setPressureRise(double pressureRise) {
      bool result = setDouble(OS_Fan_OnOffFields::PressureRise, pressureRise);
      OS_ASSERT(result);
      return result;
    }

    bool FanOnOff_Impl::setMaximumFlowRate(boost::optional<double> maximumFlowRate) {
      bool result(false);
      if (maximumFlowRate) {
        result = setDouble(OS_Fan_OnOffFields::MaximumFlowRate, maximumFlowRate.get());
      } else {
        resetMaximumFlowRate();
        result = true;
      }
      return result;
    }

    void FanOnOff_Impl::resetMaximumFlowRate() {
      bool result = setString(OS_Fan_OnOffFields::MaximumFlowRate, "");
      OS_ASSERT(result);
    }

    void FanOnOff_Impl::autosizeMaximumFlowRate() {
      bool result = setString(OS_Fan_OnOffFields::MaximumFlowRate, "autosize");
      OS_ASSERT(result);
    }

    bool FanOnOff_Impl::setMotorEfficiency(double motorEfficiency) {
      bool result = setDouble(OS_Fan_OnOffFields::MotorEfficiency, motorEfficiency);
      return result;
    }

    void FanOnOff_Impl::resetMotorEfficiency() {
      bool result = setString(OS_Fan_OnOffFields::MotorEfficiency, "");
      OS_ASSERT(result);
    }

    bool FanOnOff_Impl::setMotorInAirstreamFraction(boost::optional<double> motorInAirstreamFraction) {
      bool result(false);
      if (motorInAirstreamFraction) {
        result = setDouble(OS_Fan_OnOffFields::MotorInAirstreamFraction, motorInAirstreamFraction.get());
      } else {
        resetMotorInAirstreamFraction();
        result = true;
      }
      return result;
    }

    void FanOnOff_Impl::resetMotorInAirstreamFraction() {
      bool result = setString(OS_Fan_OnOffFields::MotorInAirstreamFraction, "");
      OS_ASSERT(result);
    }

    bool FanOnOff_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
      bool result = setString(OS_Fan_OnOffFields::EndUseSubcategory, endUseSubcategory);
      OS_ASSERT(result);
      return result;
    }

    void FanOnOff_Impl::resetEndUseSubcategory() {
      bool result = setString(OS_Fan_OnOffFields::EndUseSubcategory, "");
      OS_ASSERT(result);
    }

    boost::optional<Schedule> FanOnOff_Impl::optionalAvailabilitySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Fan_OnOffFields::AvailabilityScheduleName);
    }

    boost::optional<ModelObject> FanOnOff_Impl::availabilityScheduleAsModelObject() const {
      OptionalModelObject result = availabilitySchedule();
      return result;
    }

    // Curves

    Curve FanOnOff_Impl::fanPowerRatioFunctionofSpeedRatioCurve() const {
      boost::optional<Curve> curve =
        getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_OnOffFields::FanPowerRatioFunctionofSpeedRatioCurveName);
      OS_ASSERT(curve);
      return curve.get();
    }

    bool FanOnOff_Impl::setFanPowerRatioFunctionofSpeedRatioCurve(const Curve& curve) {
      if (model() != curve.model()) {
        return false;
      }
      return this->setPointer(OS_Fan_OnOffFields::FanPowerRatioFunctionofSpeedRatioCurveName, curve.handle());
    }

    Curve FanOnOff_Impl::fanEfficiencyRatioFunctionofSpeedRatioCurve() const {
      boost::optional<Curve> curve =
        getObject<ModelObject>().getModelObjectTarget<Curve>(OS_Fan_OnOffFields::FanEfficiencyRatioFunctionofSpeedRatioCurveName);
      OS_ASSERT(curve);
      return curve.get();
    }

    bool FanOnOff_Impl::setFanEfficiencyRatioFunctionofSpeedRatioCurve(const Curve& curve) {
      if (model() != curve.model()) {
        return false;
      }
      return this->setPointer(OS_Fan_OnOffFields::FanEfficiencyRatioFunctionofSpeedRatioCurveName, curve.handle());
    }

    ModelObject FanOnOff_Impl::clone(Model model) const {
      // TODO: why is this calling ModelObject_Impl::clone and not StraightComponent_Impl::clone?
      return ModelObject_Impl::clone(model);
    }

    // Fan:OnOff can not be added to an AirLoopHVAC.
    // It can only be contained within another HVAC Component, such as Unitary, ZoneHVAC, etc.
    bool FanOnOff_Impl::addToNode(Node& node) {
      if (node.loop()) {
        return false;
      } else if (node.airLoopHVACOutdoorAirSystem()) {
        return false;
      } else {
        return StraightComponent_Impl::addToNode(node);
      }
    }

    boost::optional<HVACComponent> FanOnOff_Impl::containingHVACComponent() const {
      // Process all types that might contain a FanOnOff object.

      auto t_handle = handle();

      // AirLoopHVACUnitarySystem
      std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

      for (const auto& airLoopHVACUnitarySystem : airLoopHVACUnitarySystems) {
        if (boost::optional<HVACComponent> fan = airLoopHVACUnitarySystem.supplyFan()) {
          if (fan->handle() == this->handle()) {
            return airLoopHVACUnitarySystem;
          }
        }
      }

      // WaterHeaterHeatPump
      {
        auto hpwhs = model().getConcreteModelObjects<WaterHeaterHeatPump>();
        for (const auto& hpwh : hpwhs) {
          if (hpwh.fan().handle() == t_handle) {
            return hpwh;
          }
        }
      }

      // AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass
      std::vector<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass> bypassSystems =
        this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass>();

      for (const auto& bypassSystem : bypassSystems) {
        if (boost::optional<HVACComponent> fan = bypassSystem.supplyAirFan()) {
          if (fan->handle() == this->handle()) {
            return bypassSystem;
          }
        }
      }

      // AirLoopHVACUnitaryHeatPumpAirToAir
      std::vector<AirLoopHVACUnitaryHeatPumpAirToAir> airLoopHVACUnitaryHeatPumpAirToAirs =
        this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAir>();

      for (const auto& airLoopHVACUnitaryHeatPumpAirToAir : airLoopHVACUnitaryHeatPumpAirToAirs) {
        if (boost::optional<HVACComponent> fan = airLoopHVACUnitaryHeatPumpAirToAir.supplyAirFan()) {
          if (fan->handle() == this->handle()) {
            return airLoopHVACUnitaryHeatPumpAirToAir;
          }
        }
      }

      return boost::none;
    }

    boost::optional<ZoneHVACComponent> FanOnOff_Impl::containingZoneHVACComponent() const {
      std::vector<ZoneHVACComponent> zoneHVACComponent = this->model().getModelObjects<ZoneHVACComponent>();
      for (const auto& elem : zoneHVACComponent) {
        switch (elem.iddObject().type().value()) {
          case openstudio::IddObjectType::OS_ZoneHVAC_FourPipeFanCoil: {
            auto component = elem.cast<ZoneHVACFourPipeFanCoil>();
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
          case openstudio::IddObjectType::OS_ZoneHVAC_PackagedTerminalAirConditioner: {
            auto component = elem.cast<ZoneHVACPackagedTerminalAirConditioner>();
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
          case openstudio::IddObjectType::OS_ZoneHVAC_WaterToAirHeatPump: {
            auto component = elem.cast<ZoneHVACWaterToAirHeatPump>();
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
          default: {
            break;
          }
        }
      }
      return boost::none;
    }

    AirflowNetworkFan FanOnOff_Impl::getAirflowNetworkFan() {
      auto opt = airflowNetworkFan();
      if (opt) {
        return opt.get();
      }
      return AirflowNetworkFan(model(), handle());
    }

    boost::optional<AirflowNetworkFan> FanOnOff_Impl::airflowNetworkFan() const {
      std::vector<AirflowNetworkFan> myAFNitems =
        getObject<ModelObject>().getModelObjectSources<AirflowNetworkFan>(AirflowNetworkFan::iddObjectType());
      auto count = myAFNitems.size();
      if (count == 1) {
        return myAFNitems[0];
      } else if (count > 1) {
        LOG(Warn, briefDescription() << " has more than one AirflowNetwork EquivalentDuct attached, returning first.");
        return myAFNitems[0];
      }
      return boost::none;
    }

    boost::optional<double> FanOnOff_Impl::autosizedMaximumFlowRate() const {
      return getAutosizedValue("Design Size Maximum Flow Rate", "m3/s");
    }

    void FanOnOff_Impl::autosize() {
      autosizeMaximumFlowRate();
    }

    void FanOnOff_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedMaximumFlowRate();
      if (val) {
        setMaximumFlowRate(val.get());
      }
    }

    std::vector<EMSActuatorNames> FanOnOff_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{
        {"Fan", "Fan Air Mass Flow Rate"}, {"Fan", "Fan Pressure Rise"}, {"Fan", "Fan Total Efficiency"}, {"Fan", "Fan Autosized Air Flow Rate"}};
      return actuators;
    }

    std::vector<std::string> FanOnOff_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types{"Fan Maximum Mass Flow Rate", "Fan Nominal Pressure Rise", "Fan Nominal Total Efficiency"};
      return types;
    }

    ComponentType FanOnOff_Impl::componentType() const {
      return ComponentType::None;
    }

    std::vector<FuelType> FanOnOff_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> FanOnOff_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> FanOnOff_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  FanOnOff::FanOnOff(const Model& model) : StraightComponent(FanOnOff::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::FanOnOff_Impl>());

    auto availabilitySchedule = model.alwaysOnDiscreteSchedule();
    setAvailabilitySchedule(availabilitySchedule);

    bool ok = setFanTotalEfficiency(0.6);
    OS_ASSERT(ok);
    setPressureRise(300);
    autosizeMaximumFlowRate();
    ok = setMotorEfficiency(0.8);
    OS_ASSERT(ok);
    ok = setMotorInAirstreamFraction(1.0);
    OS_ASSERT(ok);

    CurveExponent fanPowerFtSpeedCurve(model);
    fanPowerFtSpeedCurve.setName("Fan On Off Power Curve");
    fanPowerFtSpeedCurve.setCoefficient1Constant(1.0);
    fanPowerFtSpeedCurve.setCoefficient2Constant(0);
    fanPowerFtSpeedCurve.setCoefficient3Constant(0);
    ok = setFanPowerRatioFunctionofSpeedRatioCurve(fanPowerFtSpeedCurve);
    OS_ASSERT(ok);

    CurveCubic fanEfficiencyFtSpeedCurve(model);
    fanEfficiencyFtSpeedCurve.setName("Fan On Off Efficiency Curve");
    fanEfficiencyFtSpeedCurve.setCoefficient1Constant(1.0);
    fanEfficiencyFtSpeedCurve.setCoefficient2x(0.0);
    fanEfficiencyFtSpeedCurve.setCoefficient3xPOW2(0.0);
    fanEfficiencyFtSpeedCurve.setCoefficient4xPOW3(0.0);
    ok = setFanEfficiencyRatioFunctionofSpeedRatioCurve(fanEfficiencyFtSpeedCurve);
    OS_ASSERT(ok);
  }

  FanOnOff::FanOnOff(const Model& model, Schedule& availabilitySchedule) : StraightComponent(FanOnOff::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::FanOnOff_Impl>());

    setAvailabilitySchedule(availabilitySchedule);

    bool ok = setFanTotalEfficiency(0.6);
    OS_ASSERT(ok);
    setPressureRise(300);
    autosizeMaximumFlowRate();
    ok = setMotorEfficiency(0.8);
    OS_ASSERT(ok);
    ok = setMotorInAirstreamFraction(1.0);
    OS_ASSERT(ok);

    CurveExponent fanPowerFtSpeedCurve(model);
    fanPowerFtSpeedCurve.setName("Fan On Off Power Curve");
    fanPowerFtSpeedCurve.setCoefficient1Constant(1.0);
    fanPowerFtSpeedCurve.setCoefficient2Constant(0);
    fanPowerFtSpeedCurve.setCoefficient3Constant(0);
    ok = setFanPowerRatioFunctionofSpeedRatioCurve(fanPowerFtSpeedCurve);
    OS_ASSERT(ok);

    CurveCubic fanEfficiencyFtSpeedCurve(model);
    fanEfficiencyFtSpeedCurve.setName("Fan On Off Efficiency Curve");
    fanEfficiencyFtSpeedCurve.setCoefficient1Constant(1.0);
    fanEfficiencyFtSpeedCurve.setCoefficient2x(0.0);
    fanEfficiencyFtSpeedCurve.setCoefficient3xPOW2(0.0);
    fanEfficiencyFtSpeedCurve.setCoefficient4xPOW3(0.0);
    ok = setFanEfficiencyRatioFunctionofSpeedRatioCurve(fanEfficiencyFtSpeedCurve);
    OS_ASSERT(ok);
  }

  FanOnOff::FanOnOff(const Model& model, Schedule& availabilitySchedule, Curve& fanPowerRatioFunctionofSpeedRatioCurve,
                     Curve& fanEfficiencyRatioFunctionofSpeedRatioCurve)

    : StraightComponent(FanOnOff::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::FanOnOff_Impl>());

    setAvailabilitySchedule(availabilitySchedule);

    bool ok = setFanTotalEfficiency(0.6);
    OS_ASSERT(ok);
    setPressureRise(300);
    autosizeMaximumFlowRate();
    ok = setMotorEfficiency(0.8);
    OS_ASSERT(ok);
    ok = setMotorInAirstreamFraction(1.0);
    OS_ASSERT(ok);

    ok = setFanPowerRatioFunctionofSpeedRatioCurve(fanPowerRatioFunctionofSpeedRatioCurve);
    OS_ASSERT(ok);

    ok = setFanEfficiencyRatioFunctionofSpeedRatioCurve(fanEfficiencyRatioFunctionofSpeedRatioCurve);
    OS_ASSERT(ok);
  }

  IddObjectType FanOnOff::iddObjectType() {
    return {IddObjectType::OS_Fan_OnOff};
  }

  // Field Availability Schedule Name

  Schedule FanOnOff::availabilitySchedule() const {
    return getImpl<detail::FanOnOff_Impl>()->availabilitySchedule();
  }

  bool FanOnOff::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::FanOnOff_Impl>()->setAvailabilitySchedule(schedule);
  }

  // Field Fan Efficiency

  double FanOnOff::fanTotalEfficiency() const {
    return getImpl<detail::FanOnOff_Impl>()->fanTotalEfficiency();
  }

  bool FanOnOff::isFanTotalEfficiencyDefaulted() const {
    return getImpl<detail::FanOnOff_Impl>()->isFanTotalEfficiencyDefaulted();
  }

  bool FanOnOff::setFanTotalEfficiency(double fanTotalEfficiency) {
    return getImpl<detail::FanOnOff_Impl>()->setFanTotalEfficiency(fanTotalEfficiency);
  }

  void FanOnOff::resetFanTotalEfficiency() {
    getImpl<detail::FanOnOff_Impl>()->resetFanTotalEfficiency();
  }

  double FanOnOff::fanEfficiency() const {
    return getImpl<detail::FanOnOff_Impl>()->fanTotalEfficiency();
  }

  bool FanOnOff::isFanEfficiencyDefaulted() const {
    return getImpl<detail::FanOnOff_Impl>()->isFanTotalEfficiencyDefaulted();
  }

  bool FanOnOff::setFanEfficiency(double fanTotalEfficiency) {
    return getImpl<detail::FanOnOff_Impl>()->setFanTotalEfficiency(fanTotalEfficiency);
  }

  void FanOnOff::resetFanEfficiency() {
    getImpl<detail::FanOnOff_Impl>()->resetFanTotalEfficiency();
  }

  // Field Pressure Rise

  double FanOnOff::pressureRise() const {
    return getImpl<detail::FanOnOff_Impl>()->pressureRise();
  }

  bool FanOnOff::setPressureRise(double pressureRise) {
    return getImpl<detail::FanOnOff_Impl>()->setPressureRise(pressureRise);
  }

  // Field Maximum Flow Rate

  boost::optional<double> FanOnOff::maximumFlowRate() const {
    return getImpl<detail::FanOnOff_Impl>()->maximumFlowRate();
  }

  bool FanOnOff::isMaximumFlowRateAutosized() const {
    return getImpl<detail::FanOnOff_Impl>()->isMaximumFlowRateAutosized();
  }

  bool FanOnOff::setMaximumFlowRate(double maximumFlowRate) {
    return getImpl<detail::FanOnOff_Impl>()->setMaximumFlowRate(maximumFlowRate);
  }

  void FanOnOff::resetMaximumFlowRate() {
    getImpl<detail::FanOnOff_Impl>()->resetMaximumFlowRate();
  }

  void FanOnOff::autosizeMaximumFlowRate() {
    getImpl<detail::FanOnOff_Impl>()->autosizeMaximumFlowRate();
  }

  // Field Motor Efficiency

  double FanOnOff::motorEfficiency() const {
    return getImpl<detail::FanOnOff_Impl>()->motorEfficiency();
  }

  bool FanOnOff::isMotorEfficiencyDefaulted() const {
    return getImpl<detail::FanOnOff_Impl>()->isMotorEfficiencyDefaulted();
  }

  bool FanOnOff::setMotorEfficiency(double motorEfficiency) {
    return getImpl<detail::FanOnOff_Impl>()->setMotorEfficiency(motorEfficiency);
  }

  void FanOnOff::resetMotorEfficiency() {
    getImpl<detail::FanOnOff_Impl>()->resetMotorEfficiency();
  }

  // Field Motor In Airstream Fraction

  boost::optional<double> FanOnOff::motorInAirstreamFraction() const {
    return getImpl<detail::FanOnOff_Impl>()->motorInAirstreamFraction();
  }

  bool FanOnOff::setMotorInAirstreamFraction(double motorInAirstreamFraction) {
    return getImpl<detail::FanOnOff_Impl>()->setMotorInAirstreamFraction(motorInAirstreamFraction);
  }

  void FanOnOff::resetMotorInAirstreamFraction() {
    getImpl<detail::FanOnOff_Impl>()->resetMotorInAirstreamFraction();
  }

  // Field Fan Power Ratio Function of Speed Ratio Curve

  Curve FanOnOff::fanPowerRatioFunctionofSpeedRatioCurve() const {
    return getImpl<detail::FanOnOff_Impl>()->fanPowerRatioFunctionofSpeedRatioCurve();
  }

  bool FanOnOff::setFanPowerRatioFunctionofSpeedRatioCurve(const Curve& curve) {
    return getImpl<detail::FanOnOff_Impl>()->setFanPowerRatioFunctionofSpeedRatioCurve(curve);
  }

  /* Field Fan Efficiency Ratio Function of Speed Ratio Curve */

  Curve FanOnOff::fanEfficiencyRatioFunctionofSpeedRatioCurve() const {
    return getImpl<detail::FanOnOff_Impl>()->fanEfficiencyRatioFunctionofSpeedRatioCurve();
  }

  bool FanOnOff::setFanEfficiencyRatioFunctionofSpeedRatioCurve(const Curve& curve) {
    return getImpl<detail::FanOnOff_Impl>()->setFanEfficiencyRatioFunctionofSpeedRatioCurve(curve);
  }

  std::string FanOnOff::endUseSubcategory() const {
    return getImpl<detail::FanOnOff_Impl>()->endUseSubcategory();
  }

  bool FanOnOff::isEndUseSubcategoryDefaulted() const {
    return getImpl<detail::FanOnOff_Impl>()->isEndUseSubcategoryDefaulted();
  }

  // Field End-Use Subcategory

  bool FanOnOff::setEndUseSubcategory(const std::string& endUseSubcategory) {
    return getImpl<detail::FanOnOff_Impl>()->setEndUseSubcategory(endUseSubcategory);
  }

  void FanOnOff::resetEndUseSubcategory() {
    getImpl<detail::FanOnOff_Impl>()->resetEndUseSubcategory();
  }

  AirflowNetworkFan FanOnOff::getAirflowNetworkFan() {
    return getImpl<detail::FanOnOff_Impl>()->getAirflowNetworkFan();
  }

  boost::optional<AirflowNetworkFan> FanOnOff::airflowNetworkFan() const {
    return getImpl<detail::FanOnOff_Impl>()->airflowNetworkFan();
  }

  /// @cond
  FanOnOff::FanOnOff(std::shared_ptr<detail::FanOnOff_Impl> impl) : StraightComponent(std::move(impl)) {}
  /// @endcond

  boost::optional<double> FanOnOff::autosizedMaximumFlowRate() const {
    return getImpl<detail::FanOnOff_Impl>()->autosizedMaximumFlowRate();
  }

}  // namespace model
}  // namespace openstudio
