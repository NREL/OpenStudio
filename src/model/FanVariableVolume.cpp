/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "FanVariableVolume.hpp"
#include "FanVariableVolume_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "Model.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"
#include "ZoneHVACFourPipeFanCoil.hpp"
#include "ZoneHVACFourPipeFanCoil_Impl.hpp"
#include "ZoneHVACUnitHeater.hpp"
#include "ZoneHVACUnitHeater_Impl.hpp"
#include "ZoneHVACUnitVentilator.hpp"
#include "ZoneHVACUnitVentilator_Impl.hpp"
#include "ZoneHVACEvaporativeCoolerUnit.hpp"
#include "ZoneHVACEvaporativeCoolerUnit_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "SetpointManagerMixedAir.hpp"
#include "AirflowNetworkFan.hpp"
#include "AirflowNetworkFan_Impl.hpp"
#include "FanSystemModel.hpp"
#include "CurveQuartic.hpp"
#include "CurveQuartic_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Fan_VariableVolume_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include "../utilities/data/DataEnums.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    FanVariableVolume_Impl::FanVariableVolume_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == FanVariableVolume::iddObjectType());
    }

    FanVariableVolume_Impl::FanVariableVolume_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == FanVariableVolume::iddObjectType());
    }

    FanVariableVolume_Impl::FanVariableVolume_Impl(const FanVariableVolume_Impl& other, Model_Impl* model, bool keepHandle)
      : StraightComponent_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& FanVariableVolume_Impl::outputVariableNames() const {
      static const std::vector<std::string> results{"Fan Electricity Rate", "Fan Rise in Air Temperature", "Fan Heat Gain to Air",
                                                    "Fan Electricity Energy", "Fan Air Mass Flow Rate"};
      return results;
    }

    IddObjectType FanVariableVolume_Impl::iddObjectType() const {
      return FanVariableVolume::iddObjectType();
    }

    std::vector<ScheduleTypeKey> FanVariableVolume_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_Fan_VariableVolumeFields::AvailabilityScheduleName) != e) {
        result.push_back(ScheduleTypeKey("FanVariableVolume", "Availability"));
      }
      return result;
    }

    Schedule FanVariableVolume_Impl::availabilitySchedule() const {
      boost::optional<Schedule> value =
        getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Fan_VariableVolumeFields::AvailabilityScheduleName);
      if (!value) {
        // it is an error if we get here, however we don't want to crash
        // so we hook up to global always on schedule
        LOG(Error, "Required availability schedule not set, using 'Always On' schedule");
        value = this->model().alwaysOnDiscreteSchedule();
        OS_ASSERT(value);
        const_cast<FanVariableVolume_Impl*>(this)->setAvailabilitySchedule(*value);
        value = getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_Fan_VariableVolumeFields::AvailabilityScheduleName);
      }
      OS_ASSERT(value);
      return value.get();
    }

    double FanVariableVolume_Impl::fanTotalEfficiency() const {
      boost::optional<double> value = getDouble(OS_Fan_VariableVolumeFields::FanTotalEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool FanVariableVolume_Impl::isFanTotalEfficiencyDefaulted() const {
      return isEmpty(OS_Fan_VariableVolumeFields::FanTotalEfficiency);
    }

    double FanVariableVolume_Impl::pressureRise() const {
      boost::optional<double> value = getDouble(OS_Fan_VariableVolumeFields::PressureRise, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<double> FanVariableVolume_Impl::maximumFlowRate() const {
      return getDouble(OS_Fan_VariableVolumeFields::MaximumFlowRate, true);
    }

    bool FanVariableVolume_Impl::isMaximumFlowRateAutosized() const {
      bool result = false;
      boost::optional<std::string> value = getString(OS_Fan_VariableVolumeFields::MaximumFlowRate, true);
      if (value) {
        result = openstudio::istringEqual(value.get(), "Autosize");
      }
      return result;
    }

    std::string FanVariableVolume_Impl::fanPowerMinimumFlowRateInputMethod() const {
      boost::optional<std::string> value = getString(OS_Fan_VariableVolumeFields::FanPowerMinimumFlowRateInputMethod, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool FanVariableVolume_Impl::isFanPowerMinimumFlowRateInputMethodDefaulted() const {
      return isEmpty(OS_Fan_VariableVolumeFields::FanPowerMinimumFlowRateInputMethod);
    }

    double FanVariableVolume_Impl::fanPowerMinimumFlowFraction() const {
      boost::optional<double> value = getDouble(OS_Fan_VariableVolumeFields::FanPowerMinimumFlowFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool FanVariableVolume_Impl::isFanPowerMinimumFlowFractionDefaulted() const {
      return isEmpty(OS_Fan_VariableVolumeFields::FanPowerMinimumFlowFraction);
    }

    boost::optional<double> FanVariableVolume_Impl::fanPowerMinimumAirFlowRate() const {
      return getDouble(OS_Fan_VariableVolumeFields::FanPowerMinimumAirFlowRate, true);
    }

    double FanVariableVolume_Impl::motorEfficiency() const {
      boost::optional<double> value = getDouble(OS_Fan_VariableVolumeFields::MotorEfficiency, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool FanVariableVolume_Impl::isMotorEfficiencyDefaulted() const {
      return isEmpty(OS_Fan_VariableVolumeFields::MotorEfficiency);
    }

    double FanVariableVolume_Impl::motorInAirstreamFraction() const {
      boost::optional<double> value = getDouble(OS_Fan_VariableVolumeFields::MotorInAirstreamFraction, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool FanVariableVolume_Impl::isMotorInAirstreamFractionDefaulted() const {
      return isEmpty(OS_Fan_VariableVolumeFields::MotorInAirstreamFraction);
    }

    boost::optional<double> FanVariableVolume_Impl::fanPowerCoefficient1() const {
      return getDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient1, true);
    }

    boost::optional<double> FanVariableVolume_Impl::fanPowerCoefficient2() const {
      return getDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient2, true);
    }

    boost::optional<double> FanVariableVolume_Impl::fanPowerCoefficient3() const {
      return getDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient3, true);
    }

    boost::optional<double> FanVariableVolume_Impl::fanPowerCoefficient4() const {
      return getDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient4, true);
    }

    boost::optional<double> FanVariableVolume_Impl::fanPowerCoefficient5() const {
      return getDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient5, true);
    }

    std::string FanVariableVolume_Impl::endUseSubcategory() const {
      boost::optional<std::string> value = getString(OS_Fan_VariableVolumeFields::EndUseSubcategory, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool FanVariableVolume_Impl::isEndUseSubcategoryDefaulted() const {
      return isEmpty(OS_Fan_VariableVolumeFields::EndUseSubcategory);
    }

    bool FanVariableVolume_Impl::setAvailabilitySchedule(Schedule& schedule) {
      if (boost::optional<AirLoopHVAC> _airLoop = airLoopHVAC()) {
        LOG(Info, briefDescription() << " is connected to an AirLoopHVAC '" << _airLoop->nameString()
                                     << "', therefore its Availability Schedule will be overriden by the AirLoopHVAC's one.");
      }
      bool result = setSchedule(OS_Fan_VariableVolumeFields::AvailabilityScheduleName, "FanVariableVolume", "Availability", schedule);
      return result;
    }

    bool FanVariableVolume_Impl::setFanTotalEfficiency(double fanTotalEfficiency) {
      bool result = setDouble(OS_Fan_VariableVolumeFields::FanTotalEfficiency, fanTotalEfficiency);
      return result;
    }

    void FanVariableVolume_Impl::resetFanTotalEfficiency() {
      bool result = setString(OS_Fan_VariableVolumeFields::FanTotalEfficiency, "");
      OS_ASSERT(result);
    }

    bool FanVariableVolume_Impl::setPressureRise(double pressureRise) {
      bool result = setDouble(OS_Fan_VariableVolumeFields::PressureRise, pressureRise);
      OS_ASSERT(result);
      return result;
    }

    bool FanVariableVolume_Impl::setMaximumFlowRate(boost::optional<double> maximumFlowRate) {
      bool result = false;
      if (maximumFlowRate) {
        result = setDouble(OS_Fan_VariableVolumeFields::MaximumFlowRate, maximumFlowRate.get());
      } else {
        result = setString(OS_Fan_VariableVolumeFields::MaximumFlowRate, "");
      }
      return result;
    }

    void FanVariableVolume_Impl::resetMaximumFlowRate() {
      bool result = setString(OS_Fan_VariableVolumeFields::MaximumFlowRate, "");
      OS_ASSERT(result);
    }

    void FanVariableVolume_Impl::autosizeMaximumFlowRate() {
      bool result = setString(OS_Fan_VariableVolumeFields::MaximumFlowRate, "Autosize");
      OS_ASSERT(result);
    }

    bool FanVariableVolume_Impl::setFanPowerMinimumFlowRateInputMethod(const std::string& fanPowerMinimumFlowRateInputMethod) {
      bool result = setString(OS_Fan_VariableVolumeFields::FanPowerMinimumFlowRateInputMethod, fanPowerMinimumFlowRateInputMethod);
      return result;
    }

    void FanVariableVolume_Impl::resetFanPowerMinimumFlowRateInputMethod() {
      bool result = setString(OS_Fan_VariableVolumeFields::FanPowerMinimumFlowRateInputMethod, "");
      OS_ASSERT(result);
    }

    bool FanVariableVolume_Impl::setFanPowerMinimumFlowFraction(double fanPowerMinimumFlowFraction) {
      bool result = setDouble(OS_Fan_VariableVolumeFields::FanPowerMinimumFlowFraction, fanPowerMinimumFlowFraction);
      return result;
    }

    void FanVariableVolume_Impl::resetFanPowerMinimumFlowFraction() {
      bool result = setString(OS_Fan_VariableVolumeFields::FanPowerMinimumFlowFraction, "");
      OS_ASSERT(result);
    }

    bool FanVariableVolume_Impl::setFanPowerMinimumAirFlowRate(boost::optional<double> fanPowerMinimumAirFlowRate) {
      bool result(false);
      if (fanPowerMinimumAirFlowRate) {
        result = setDouble(OS_Fan_VariableVolumeFields::FanPowerMinimumAirFlowRate, fanPowerMinimumAirFlowRate.get());
      } else {
        resetFanPowerMinimumAirFlowRate();
        result = true;
      }
      return result;
    }

    void FanVariableVolume_Impl::resetFanPowerMinimumAirFlowRate() {
      bool result = setString(OS_Fan_VariableVolumeFields::FanPowerMinimumAirFlowRate, "");
      OS_ASSERT(result);
    }

    bool FanVariableVolume_Impl::setMotorEfficiency(double motorEfficiency) {
      bool result = setDouble(OS_Fan_VariableVolumeFields::MotorEfficiency, motorEfficiency);
      return result;
    }

    void FanVariableVolume_Impl::resetMotorEfficiency() {
      bool result = setString(OS_Fan_VariableVolumeFields::MotorEfficiency, "");
      OS_ASSERT(result);
    }

    bool FanVariableVolume_Impl::setMotorInAirstreamFraction(double motorInAirstreamFraction) {
      bool result = setDouble(OS_Fan_VariableVolumeFields::MotorInAirstreamFraction, motorInAirstreamFraction);
      return result;
    }

    void FanVariableVolume_Impl::resetMotorInAirstreamFraction() {
      bool result = setString(OS_Fan_VariableVolumeFields::MotorInAirstreamFraction, "");
      OS_ASSERT(result);
    }

    bool FanVariableVolume_Impl::setFanPowerCoefficient1(boost::optional<double> fanPowerCoefficient1) {
      bool result(false);
      if (fanPowerCoefficient1) {
        result = setDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient1, fanPowerCoefficient1.get());
      } else {
        resetFanPowerCoefficient1();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void FanVariableVolume_Impl::resetFanPowerCoefficient1() {
      bool result = setString(OS_Fan_VariableVolumeFields::FanPowerCoefficient1, "");
      OS_ASSERT(result);
    }

    bool FanVariableVolume_Impl::setFanPowerCoefficient2(boost::optional<double> fanPowerCoefficient2) {
      bool result(false);
      if (fanPowerCoefficient2) {
        result = setDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient2, fanPowerCoefficient2.get());
      } else {
        resetFanPowerCoefficient2();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void FanVariableVolume_Impl::resetFanPowerCoefficient2() {
      bool result = setString(OS_Fan_VariableVolumeFields::FanPowerCoefficient2, "");
      OS_ASSERT(result);
    }

    bool FanVariableVolume_Impl::setFanPowerCoefficient3(boost::optional<double> fanPowerCoefficient3) {
      bool result(false);
      if (fanPowerCoefficient3) {
        result = setDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient3, fanPowerCoefficient3.get());
      } else {
        resetFanPowerCoefficient3();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void FanVariableVolume_Impl::resetFanPowerCoefficient3() {
      bool result = setString(OS_Fan_VariableVolumeFields::FanPowerCoefficient3, "");
      OS_ASSERT(result);
    }

    bool FanVariableVolume_Impl::setFanPowerCoefficient4(boost::optional<double> fanPowerCoefficient4) {
      bool result(false);
      if (fanPowerCoefficient4) {
        result = setDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient4, fanPowerCoefficient4.get());
      } else {
        resetFanPowerCoefficient4();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void FanVariableVolume_Impl::resetFanPowerCoefficient4() {
      bool result = setString(OS_Fan_VariableVolumeFields::FanPowerCoefficient4, "");
      OS_ASSERT(result);
    }

    bool FanVariableVolume_Impl::setFanPowerCoefficient5(boost::optional<double> fanPowerCoefficient5) {
      bool result(false);
      if (fanPowerCoefficient5) {
        result = setDouble(OS_Fan_VariableVolumeFields::FanPowerCoefficient5, fanPowerCoefficient5.get());
      } else {
        resetFanPowerCoefficient5();
        result = true;
      }
      OS_ASSERT(result);
      return result;
    }

    void FanVariableVolume_Impl::resetFanPowerCoefficient5() {
      bool result = setString(OS_Fan_VariableVolumeFields::FanPowerCoefficient5, "");
      OS_ASSERT(result);
    }

    bool FanVariableVolume_Impl::setEndUseSubcategory(const std::string& endUseSubcategory) {
      bool result = setString(OS_Fan_VariableVolumeFields::EndUseSubcategory, endUseSubcategory);
      OS_ASSERT(result);
      return result;
    }

    void FanVariableVolume_Impl::resetEndUseSubcategory() {
      bool result = setString(OS_Fan_VariableVolumeFields::EndUseSubcategory, "");
      OS_ASSERT(result);
    }

    unsigned FanVariableVolume_Impl::inletPort() const {
      return OS_Fan_VariableVolumeFields::AirInletNodeName;
    }

    unsigned FanVariableVolume_Impl::outletPort() const {
      return OS_Fan_VariableVolumeFields::AirOutletNodeName;
    }

    bool FanVariableVolume_Impl::addToNode(Node& node) {
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

    std::vector<std::string> FanVariableVolume_Impl::fanPowerMinimumFlowRateInputMethodValues() const {
      return FanVariableVolume::fanPowerMinimumFlowRateInputMethodValues();
    }

    boost::optional<ModelObject> FanVariableVolume_Impl::availabilityScheduleAsModelObject() const {
      OptionalModelObject result = availabilitySchedule();
      return result;
    }

    bool FanVariableVolume_Impl::setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject) {
      if (modelObject) {
        OptionalSchedule intermediate = modelObject->optionalCast<Schedule>();
        if (intermediate) {
          Schedule schedule(*intermediate);
          return setAvailabilitySchedule(schedule);
        }
      }
      return false;
    }

    boost::optional<HVACComponent> FanVariableVolume_Impl::containingHVACComponent() const {
      // AirLoopHVACUnitarySystem
      std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

      for (const auto& airLoopHVACUnitarySystem : airLoopHVACUnitarySystems) {
        if (boost::optional<HVACComponent> fan = airLoopHVACUnitarySystem.supplyFan()) {
          if (fan->handle() == this->handle()) {
            return airLoopHVACUnitarySystem;
          }
        }
      }
      return boost::none;
    }

    boost::optional<ZoneHVACComponent> FanVariableVolume_Impl::containingZoneHVACComponent() const {
      // ZoneHVACFourPipeFanCoil

      std::vector<ZoneHVACFourPipeFanCoil> zoneHVACFourPipeFanCoils;

      zoneHVACFourPipeFanCoils = this->model().getConcreteModelObjects<ZoneHVACFourPipeFanCoil>();

      for (const auto& zoneHVACFourPipeFanCoil : zoneHVACFourPipeFanCoils) {
        if (boost::optional<HVACComponent> fan = zoneHVACFourPipeFanCoil.supplyAirFan()) {
          if (fan->handle() == this->handle()) {
            return zoneHVACFourPipeFanCoil;
          }
        }
      }

      // ZoneHVACUnitHeater

      std::vector<ZoneHVACUnitHeater> zoneHVACUnitHeater;

      zoneHVACUnitHeater = this->model().getConcreteModelObjects<ZoneHVACUnitHeater>();

      for (const auto& elem : zoneHVACUnitHeater) {
        if (boost::optional<HVACComponent> fan = elem.supplyAirFan()) {
          if (fan->handle() == this->handle()) {
            return elem;
          }
        }
      }

      // ZoneHVACUnitVentilator

      std::vector<ZoneHVACUnitVentilator> zoneHVACUnitVentilator;

      zoneHVACUnitVentilator = this->model().getConcreteModelObjects<ZoneHVACUnitVentilator>();

      for (const auto& elem : zoneHVACUnitVentilator) {
        if (boost::optional<HVACComponent> fan = elem.supplyAirFan()) {
          if (fan->handle() == this->handle()) {
            return elem;
          }
        }
      }

      // ZoneHVACEvaporativeCoolerUnit
      std::vector<ZoneHVACEvaporativeCoolerUnit> zoneHVACEvaporativeCoolerUnit;

      zoneHVACEvaporativeCoolerUnit = this->model().getConcreteModelObjects<ZoneHVACEvaporativeCoolerUnit>();

      for (const auto& elem : zoneHVACEvaporativeCoolerUnit) {
        if (boost::optional<HVACComponent> fan = elem.supplyAirFan()) {
          if (fan->handle() == this->handle()) {
            return elem;
          }
        }
      }

      return boost::none;
    }

    AirflowNetworkFan FanVariableVolume_Impl::getAirflowNetworkFan() {
      auto opt = airflowNetworkFan();
      if (opt) {
        return opt.get();
      }
      return AirflowNetworkFan(model(), handle());
    }

    boost::optional<AirflowNetworkFan> FanVariableVolume_Impl::airflowNetworkFan() const {
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

    boost::optional<double> FanVariableVolume_Impl::autosizedMaximumFlowRate() const {
      return getAutosizedValue("Design Size Maximum Flow Rate", "m3/s");
    }

    void FanVariableVolume_Impl::autosize() {
      autosizeMaximumFlowRate();
    }

    void FanVariableVolume_Impl::applySizingValues() {
      boost::optional<double> val;
      val = autosizedMaximumFlowRate();
      if (val) {
        setMaximumFlowRate(val.get());
      }
    }

    FanSystemModel FanVariableVolume_Impl::convertToFanSystemModel() const {
      FanSystemModel fan(this->model());
      fan.setName(this->nameString() + " FanSystemModel");
      auto availSch = this->availabilitySchedule();
      fan.setAvailabilitySchedule(availSch);
      fan.setFanTotalEfficiency(this->fanTotalEfficiency());
      fan.setDesignPressureRise(this->pressureRise());
      if (this->isMaximumFlowRateAutosized()) {
        fan.autosizeDesignMaximumAirFlowRate();
      } else if (boost::optional<double> value = this->maximumFlowRate()) {
        fan.setDesignMaximumAirFlowRate(value.get());
      }
      fan.setMotorEfficiency(this->motorEfficiency());
      fan.setMotorInAirStreamFraction(this->motorInAirstreamFraction());
      fan.setSpeedControlMethod("Continuous");
      fan.setElectricPowerMinimumFlowRateFraction(0.0);
      fan.autosizeDesignElectricPowerConsumption();
      fan.setDesignPowerSizingMethod("TotalEfficiencyAndPressure");
      fan.setEndUseSubcategory(this->endUseSubcategory());

      CurveQuartic curve(this->model());
      curve.setName(fan.nameString() + " Curve");
      if (boost::optional<double> value = this->fanPowerCoefficient1()) {
        curve.setCoefficient1Constant(value.get());
      }
      if (boost::optional<double> value = this->fanPowerCoefficient2()) {
        curve.setCoefficient2x(value.get());
      }
      if (boost::optional<double> value = this->fanPowerCoefficient3()) {
        curve.setCoefficient3xPOW2(value.get());
      }
      if (boost::optional<double> value = this->fanPowerCoefficient4()) {
        curve.setCoefficient4xPOW3(value.get());
      }
      if (boost::optional<double> value = this->fanPowerCoefficient5()) {
        curve.setCoefficient5xPOW4(value.get());
      }
      curve.setMinimumValueofx(0.0);
      curve.setMaximumValueofx(1.0);
      curve.setMinimumCurveOutput(0.0);
      curve.setMaximumCurveOutput(5.0);
      curve.setInputUnitTypeforX("Dimensionless");
      curve.setOutputUnitType("Dimensionless");
      fan.setElectricPowerFunctionofFlowFractionCurve(curve);

      return fan;
    }

    std::vector<EMSActuatorNames> FanVariableVolume_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{
        {"Fan", "Fan Air Mass Flow Rate"}, {"Fan", "Fan Pressure Rise"}, {"Fan", "Fan Total Efficiency"}, {"Fan", "Fan Autosized Air Flow Rate"}};
      return actuators;
    }

    std::vector<std::string> FanVariableVolume_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types{"Fan Maximum Mass Flow Rate", "Fan Nominal Pressure Rise", "Fan Nominal Total Efficiency"};
      return types;
    }

    ComponentType FanVariableVolume_Impl::componentType() const {
      return ComponentType::None;
    }

    std::vector<FuelType> FanVariableVolume_Impl::coolingFuelTypes() const {
      return {};
    }

    std::vector<FuelType> FanVariableVolume_Impl::heatingFuelTypes() const {
      return {};
    }

    std::vector<AppGFuelType> FanVariableVolume_Impl::appGHeatingFuelTypes() const {
      return {};
    }

  }  // namespace detail

  FanVariableVolume::FanVariableVolume(const Model& model, Schedule& schedule) : StraightComponent(FanVariableVolume::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::FanVariableVolume_Impl>());

    bool ok = setAvailabilitySchedule(schedule);
    if (!ok) {
      remove();
      LOG_AND_THROW("Unable to set " << briefDescription() << "'s availability schedule to " << schedule.briefDescription() << ".");
    }
    setEndUseSubcategory("");
    setFanTotalEfficiency(0.6045);
    setPressureRise(1017.592);
    autosizeMaximumFlowRate();
    setFanPowerMinimumFlowRateInputMethod("FixedFlowRate");
    setFanPowerMinimumFlowFraction(0.0);
    setFanPowerMinimumAirFlowRate(0.0);
    setMotorEfficiency(0.93);
    setMotorInAirstreamFraction(1.0);
    setFanPowerCoefficient1(0.0407598940);
    setFanPowerCoefficient2(0.08804497);
    setFanPowerCoefficient3(-0.072926120);
    setFanPowerCoefficient4(0.9437398230);
    setFanPowerCoefficient5(0.0);
  }

  FanVariableVolume::FanVariableVolume(const Model& model) : StraightComponent(FanVariableVolume::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::FanVariableVolume_Impl>());

    auto schedule = model.alwaysOnDiscreteSchedule();
    setAvailabilitySchedule(schedule);

    setEndUseSubcategory("");
    setFanTotalEfficiency(0.6045);
    setPressureRise(1017.592);
    autosizeMaximumFlowRate();
    setFanPowerMinimumFlowRateInputMethod("FixedFlowRate");
    setFanPowerMinimumFlowFraction(0.0);
    setFanPowerMinimumAirFlowRate(0.0);
    setMotorEfficiency(0.93);
    setMotorInAirstreamFraction(1.0);
    setFanPowerCoefficient1(0.0407598940);
    setFanPowerCoefficient2(0.08804497);
    setFanPowerCoefficient3(-0.072926120);
    setFanPowerCoefficient4(0.9437398230);
    setFanPowerCoefficient5(0.0);
  }

  IddObjectType FanVariableVolume::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Fan_VariableVolume);
    return result;
  }

  std::vector<std::string> FanVariableVolume::fanPowerMinimumFlowRateInputMethodValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Fan_VariableVolumeFields::FanPowerMinimumFlowRateInputMethod);
  }

  std::vector<std::string> FanVariableVolume::validFanPowerMinimumFlowRateInputMethodValues() {
    return FanVariableVolume::fanPowerMinimumFlowRateInputMethodValues();
  }

  Schedule FanVariableVolume::availabilitySchedule() const {
    return getImpl<detail::FanVariableVolume_Impl>()->availabilitySchedule();
  }

  // New Fan Total Efficiency
  double FanVariableVolume::fanTotalEfficiency() const {
    return getImpl<detail::FanVariableVolume_Impl>()->fanTotalEfficiency();
  }

  bool FanVariableVolume::isFanTotalEfficiencyDefaulted() const {
    return getImpl<detail::FanVariableVolume_Impl>()->isFanTotalEfficiencyDefaulted();
  }

  bool FanVariableVolume::setFanTotalEfficiency(double fanTotalEfficiency) {
    return getImpl<detail::FanVariableVolume_Impl>()->setFanTotalEfficiency(fanTotalEfficiency);
  }

  void FanVariableVolume::resetFanTotalEfficiency() {
    getImpl<detail::FanVariableVolume_Impl>()->resetFanTotalEfficiency();
  }

  // Deprecrated methods
  double FanVariableVolume::fanEfficiency() const {
    return getImpl<detail::FanVariableVolume_Impl>()->fanTotalEfficiency();
  }

  bool FanVariableVolume::isFanEfficiencyDefaulted() const {
    return getImpl<detail::FanVariableVolume_Impl>()->isFanTotalEfficiencyDefaulted();
  }

  bool FanVariableVolume::setFanEfficiency(double fanTotalEfficiency) {
    return getImpl<detail::FanVariableVolume_Impl>()->setFanTotalEfficiency(fanTotalEfficiency);
  }

  void FanVariableVolume::resetFanEfficiency() {
    getImpl<detail::FanVariableVolume_Impl>()->resetFanTotalEfficiency();
  }

  double FanVariableVolume::pressureRise() const {
    return getImpl<detail::FanVariableVolume_Impl>()->pressureRise();
  }

  boost::optional<double> FanVariableVolume::maximumFlowRate() const {
    return getImpl<detail::FanVariableVolume_Impl>()->maximumFlowRate();
  }

  bool FanVariableVolume::isMaximumFlowRateAutosized() const {
    return getImpl<detail::FanVariableVolume_Impl>()->isMaximumFlowRateAutosized();
  }

  std::string FanVariableVolume::fanPowerMinimumFlowRateInputMethod() const {
    return getImpl<detail::FanVariableVolume_Impl>()->fanPowerMinimumFlowRateInputMethod();
  }

  bool FanVariableVolume::isFanPowerMinimumFlowRateInputMethodDefaulted() const {
    return getImpl<detail::FanVariableVolume_Impl>()->isFanPowerMinimumFlowRateInputMethodDefaulted();
  }

  double FanVariableVolume::fanPowerMinimumFlowFraction() const {
    return getImpl<detail::FanVariableVolume_Impl>()->fanPowerMinimumFlowFraction();
  }

  bool FanVariableVolume::isFanPowerMinimumFlowFractionDefaulted() const {
    return getImpl<detail::FanVariableVolume_Impl>()->isFanPowerMinimumFlowFractionDefaulted();
  }

  boost::optional<double> FanVariableVolume::fanPowerMinimumAirFlowRate() const {
    return getImpl<detail::FanVariableVolume_Impl>()->fanPowerMinimumAirFlowRate();
  }

  double FanVariableVolume::motorEfficiency() const {
    return getImpl<detail::FanVariableVolume_Impl>()->motorEfficiency();
  }

  bool FanVariableVolume::isMotorEfficiencyDefaulted() const {
    return getImpl<detail::FanVariableVolume_Impl>()->isMotorEfficiencyDefaulted();
  }

  double FanVariableVolume::motorInAirstreamFraction() const {
    return getImpl<detail::FanVariableVolume_Impl>()->motorInAirstreamFraction();
  }

  bool FanVariableVolume::isMotorInAirstreamFractionDefaulted() const {
    return getImpl<detail::FanVariableVolume_Impl>()->isMotorInAirstreamFractionDefaulted();
  }

  boost::optional<double> FanVariableVolume::fanPowerCoefficient1() const {
    return getImpl<detail::FanVariableVolume_Impl>()->fanPowerCoefficient1();
  }

  boost::optional<double> FanVariableVolume::fanPowerCoefficient2() const {
    return getImpl<detail::FanVariableVolume_Impl>()->fanPowerCoefficient2();
  }

  boost::optional<double> FanVariableVolume::fanPowerCoefficient3() const {
    return getImpl<detail::FanVariableVolume_Impl>()->fanPowerCoefficient3();
  }

  boost::optional<double> FanVariableVolume::fanPowerCoefficient4() const {
    return getImpl<detail::FanVariableVolume_Impl>()->fanPowerCoefficient4();
  }

  boost::optional<double> FanVariableVolume::fanPowerCoefficient5() const {
    return getImpl<detail::FanVariableVolume_Impl>()->fanPowerCoefficient5();
  }

  std::string FanVariableVolume::endUseSubcategory() const {
    return getImpl<detail::FanVariableVolume_Impl>()->endUseSubcategory();
  }

  bool FanVariableVolume::isEndUseSubcategoryDefaulted() const {
    return getImpl<detail::FanVariableVolume_Impl>()->isEndUseSubcategoryDefaulted();
  }

  bool FanVariableVolume::setAvailabilitySchedule(Schedule& schedule) {
    return getImpl<detail::FanVariableVolume_Impl>()->setAvailabilitySchedule(schedule);
  }

  bool FanVariableVolume::setPressureRise(double pressureRise) {
    return getImpl<detail::FanVariableVolume_Impl>()->setPressureRise(pressureRise);
  }

  bool FanVariableVolume::setMaximumFlowRate(double maximumFlowRate) {
    return getImpl<detail::FanVariableVolume_Impl>()->setMaximumFlowRate(maximumFlowRate);
  }

  void FanVariableVolume::resetMaximumFlowRate() {
    getImpl<detail::FanVariableVolume_Impl>()->resetMaximumFlowRate();
  }

  void FanVariableVolume::autosizeMaximumFlowRate() {
    getImpl<detail::FanVariableVolume_Impl>()->autosizeMaximumFlowRate();
  }

  bool FanVariableVolume::setFanPowerMinimumFlowRateInputMethod(const std::string& fanPowerMinimumFlowRateInputMethod) {
    return getImpl<detail::FanVariableVolume_Impl>()->setFanPowerMinimumFlowRateInputMethod(fanPowerMinimumFlowRateInputMethod);
  }

  void FanVariableVolume::resetFanPowerMinimumFlowRateInputMethod() {
    getImpl<detail::FanVariableVolume_Impl>()->resetFanPowerMinimumFlowRateInputMethod();
  }

  bool FanVariableVolume::setFanPowerMinimumFlowFraction(double fanPowerMinimumFlowFraction) {
    return getImpl<detail::FanVariableVolume_Impl>()->setFanPowerMinimumFlowFraction(fanPowerMinimumFlowFraction);
  }

  void FanVariableVolume::resetFanPowerMinimumFlowFraction() {
    getImpl<detail::FanVariableVolume_Impl>()->resetFanPowerMinimumFlowFraction();
  }

  bool FanVariableVolume::setFanPowerMinimumAirFlowRate(double fanPowerMinimumAirFlowRate) {
    return getImpl<detail::FanVariableVolume_Impl>()->setFanPowerMinimumAirFlowRate(fanPowerMinimumAirFlowRate);
  }

  void FanVariableVolume::resetFanPowerMinimumAirFlowRate() {
    getImpl<detail::FanVariableVolume_Impl>()->resetFanPowerMinimumAirFlowRate();
  }

  bool FanVariableVolume::setMotorEfficiency(double motorEfficiency) {
    return getImpl<detail::FanVariableVolume_Impl>()->setMotorEfficiency(motorEfficiency);
  }

  void FanVariableVolume::resetMotorEfficiency() {
    getImpl<detail::FanVariableVolume_Impl>()->resetMotorEfficiency();
  }

  bool FanVariableVolume::setMotorInAirstreamFraction(double motorInAirstreamFraction) {
    return getImpl<detail::FanVariableVolume_Impl>()->setMotorInAirstreamFraction(motorInAirstreamFraction);
  }

  void FanVariableVolume::resetMotorInAirstreamFraction() {
    getImpl<detail::FanVariableVolume_Impl>()->resetMotorInAirstreamFraction();
  }

  bool FanVariableVolume::setFanPowerCoefficient1(double fanPowerCoefficient1) {
    return getImpl<detail::FanVariableVolume_Impl>()->setFanPowerCoefficient1(fanPowerCoefficient1);
  }

  void FanVariableVolume::resetFanPowerCoefficient1() {
    getImpl<detail::FanVariableVolume_Impl>()->resetFanPowerCoefficient1();
  }

  bool FanVariableVolume::setFanPowerCoefficient2(double fanPowerCoefficient2) {
    return getImpl<detail::FanVariableVolume_Impl>()->setFanPowerCoefficient2(fanPowerCoefficient2);
  }

  void FanVariableVolume::resetFanPowerCoefficient2() {
    getImpl<detail::FanVariableVolume_Impl>()->resetFanPowerCoefficient2();
  }

  bool FanVariableVolume::setFanPowerCoefficient3(double fanPowerCoefficient3) {
    return getImpl<detail::FanVariableVolume_Impl>()->setFanPowerCoefficient3(fanPowerCoefficient3);
  }

  void FanVariableVolume::resetFanPowerCoefficient3() {
    getImpl<detail::FanVariableVolume_Impl>()->resetFanPowerCoefficient3();
  }

  bool FanVariableVolume::setFanPowerCoefficient4(double fanPowerCoefficient4) {
    return getImpl<detail::FanVariableVolume_Impl>()->setFanPowerCoefficient4(fanPowerCoefficient4);
  }

  void FanVariableVolume::resetFanPowerCoefficient4() {
    getImpl<detail::FanVariableVolume_Impl>()->resetFanPowerCoefficient4();
  }

  bool FanVariableVolume::setFanPowerCoefficient5(double fanPowerCoefficient5) {
    return getImpl<detail::FanVariableVolume_Impl>()->setFanPowerCoefficient5(fanPowerCoefficient5);
  }

  void FanVariableVolume::resetFanPowerCoefficient5() {
    getImpl<detail::FanVariableVolume_Impl>()->resetFanPowerCoefficient5();
  }

  bool FanVariableVolume::setEndUseSubcategory(const std::string& endUseSubcategory) {
    return getImpl<detail::FanVariableVolume_Impl>()->setEndUseSubcategory(endUseSubcategory);
  }

  void FanVariableVolume::resetEndUseSubcategory() {
    getImpl<detail::FanVariableVolume_Impl>()->resetEndUseSubcategory();
  }

  AirflowNetworkFan FanVariableVolume::getAirflowNetworkFan() {
    return getImpl<detail::FanVariableVolume_Impl>()->getAirflowNetworkFan();
  }

  boost::optional<AirflowNetworkFan> FanVariableVolume::airflowNetworkFan() const {
    return getImpl<detail::FanVariableVolume_Impl>()->airflowNetworkFan();
  }

  boost::optional<double> FanVariableVolume::autosizedMaximumFlowRate() const {
    return getImpl<detail::FanVariableVolume_Impl>()->autosizedMaximumFlowRate();
  }

  FanSystemModel FanVariableVolume::convertToFanSystemModel() const {
    return getImpl<detail::FanVariableVolume_Impl>()->convertToFanSystemModel();
  }

  /// @cond
  FanVariableVolume::FanVariableVolume(std::shared_ptr<detail::FanVariableVolume_Impl> impl) : StraightComponent(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
